#include "vm/machine.h"
#include "compat/ccpu/abi.h"
#include "compat/ccpu/lifecycle.h"
#include "vm/snapshot.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "ica.h"
/* base_def.h's non-ANSI compatibility macro must not alter this C17 test. */
#undef const

extern void (*BIOS[256])(void);
extern unsigned long c_cpu_q_ev_get_count(void);

typedef struct checkpoint_probe {
    softpc_machine *machine;
    unsigned calls, nested, halted;
    int nested_returned;
    softpc_ccpu_entry entry;
    unsigned long counter;
    softpc_snapshot snapshot;
    unsigned captures;
    unsigned timeouts;
} checkpoint_probe;

static checkpoint_probe probe;

static void nested_bios(void)
{
    /* Deliberately keep return state on a real C stack, exactly like the
       original keyboard/video BIOS callbacks. BOP FE must return here. */
    unsigned long saved_ip = c_getEIP();
    assert(softpc_snapshot_begin(&probe.snapshot) == LIB_STATUS_OK);
    c_setEIP(0x600u);
    c_cpu_simulate();
    assert(c_getEIP() == 0x604u);
    c_setEIP(saved_ip);
    probe.nested_returned = 1;
}

static void observe(void *context, unsigned long depth,
    const softpc_ccpu_entry *entry)
{
    checkpoint_probe *state = context;
    unsigned long ip = c_getEIP();
    assert(state == &probe);
    assert(++state->calls < 100u);
    assert(depth == 1u || depth == 2u);
    if (softpc_snapshot_checkpoint(&state->snapshot, depth, entry)) {
        assert(depth == 1u && state->nested_returned);
        assert(state->snapshot.phase == SOFTPC_SNAPSHOT_READY);
        ++state->captures;
        assert(softpc_snapshot_finish(&state->snapshot) == LIB_STATUS_OK);
    }
    if (depth == 2u) {
        assert(!state->nested_returned && !entry->halted);
        assert(ip >= 0x600u && ip < 0x604u);
        ++state->nested;
        assert(state->snapshot.phase == SOFTPC_SNAPSHOT_WAITING);
        return; /* An inner CPU entry is not an outer capture boundary. */
    }
    if (entry->halted) {
        assert(state->nested_returned && state->nested != 0u);
        assert(ip == 0x505u); /* HLT has already advanced IP. */
        ++state->halted;
        state->entry = *entry;
        state->counter = c_cpu_q_ev_get_count();
        softpc_machine_request_stop(state->machine);
    }
}

static void reentered_halt(void *context, unsigned long depth,
    const softpc_ccpu_entry *entry)
{
    checkpoint_probe *state = context;
    assert(depth == 1u && entry->halted && entry->trap == state->entry.trap);
    assert(c_getEIP() == 0x505u && c_getEAX() == 0x1234u);
    assert(c_cpu_q_ev_get_count() == state->counter);
    assert(++state->calls == 1u);
    /* Resume while an existing invocation is live must not nest the CPU. */
    assert(!softpc_ccpu_lifecycle_resume(entry));
    softpc_machine_request_stop(state->machine);
}

static void capture_shadow(void *context, unsigned long depth,
    const softpc_ccpu_entry *entry)
{
    checkpoint_probe *state = context;
    assert(depth == 1u && !entry->halted && ++state->calls < 10u);
    if (c_getEIP() != 0x702u) return;
    assert((c_getEFLAGS() & 0x200u) != 0u && c_getEAX() == 0x10u);
    state->entry = *entry;
    state->counter = c_cpu_q_ev_get_count();
    softpc_machine_request_stop(state->machine);
}

static void finish_shadow(void *context, unsigned long depth,
    const softpc_ccpu_entry *entry)
{
    checkpoint_probe *state = context;
    assert(depth == 1u && ++state->calls < 100u);
    if (state->calls == 1u) {
        assert(c_getEIP() == 0x702u && !entry->halted);
        assert(c_cpu_q_ev_get_count() == state->counter);
    }
    if (entry->halted) {
        unsigned char marker[2];
        assert(c_getEIP() == 0x705u && c_getEAX() == 0x11u);
        assert(softpc_machine_read_physical(state->machine, 0xa00u,
            marker, sizeof(marker)) == SOFTPC_MACHINE_OK);
        /* IRQ handler must observe the INC after STI, never the prior AX. */
        assert(marker[0] == 0x11u && marker[1] == 0u);
        ++state->halted;
        softpc_machine_request_stop(state->machine);
    }
}

static void verify_reentry(void)
{
    const unsigned char program[] = { 0xfa, 0xfb, 0x40, 0xfa, 0xf4 };
    const unsigned char handler[] = { 0xa3, 0x00, 0x0a, 0xcf };
    const unsigned char vector[] = { 0x00, 0x09, 0x00, 0x00 };
    softpc_ccpu_entry invalid = { 2, 0u };
    unsigned long flags = c_getEFLAGS();
    unsigned repeat;
    assert(!softpc_ccpu_lifecycle_resume(NULL));
    assert(!softpc_ccpu_lifecycle_resume(&invalid));
    invalid = (softpc_ccpu_entry){ 0, 1u };
    assert(!softpc_ccpu_lifecycle_resume(&invalid));
    invalid = (softpc_ccpu_entry){ 1, 2u };
    assert(!softpc_ccpu_lifecycle_resume(&invalid));
    softpc_ccpu_lifecycle_observe(reentered_halt, &probe);
    /* Debug may have changed TF while a previous HLT was paused. Its pending
       trap remains the one captured on instruction entry, not the new TF. */
    c_setEFLAGS(flags | 0x100u);
    for (repeat = 0; repeat < 2u; ++repeat) {
        probe.calls = 0u;
        assert(softpc_ccpu_lifecycle_resume(&probe.entry));
        softpc_ccpu_lifecycle_clear_exit();
        assert(probe.calls == 1u);
        assert((c_getEFLAGS() & 0x100u) != 0u);
    }
    c_setEFLAGS(2u);
    assert(softpc_machine_write_physical(probe.machine, 0x700u,
        program, sizeof(program)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x900u,
        handler, sizeof(handler)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x20u,
        vector, sizeof(vector)) == SOFTPC_MACHINE_OK);
    assert(c_setSS(0u) == 0 && c_setDS(0u) == 0);
    c_setESP(0x2000u);
    c_setEIP(0x700u);
    c_setEAX(0x10u);
    /* Configure the real master PIC for IRQ0 -> vector 8. */
    outb(0x20u, 0x11u); outb(0x21u, 0x08u);
    outb(0x21u, 0x04u); outb(0x21u, 0x01u);
    outb(0x21u, 0xfeu); outb(0xa1u, 0xffu);
    probe.calls = 0u;
    softpc_ccpu_lifecycle_observe(capture_shadow, &probe);
    assert(softpc_machine_run(probe.machine, UINT64_MAX) == SOFTPC_MACHINE_OK);
    assert(c_getEIP() == 0x702u && !probe.entry.halted);
    ica_hw_interrupt(ICA_MASTER, 0u, 1);
    probe.calls = probe.halted = 0u;
    softpc_ccpu_lifecycle_observe(finish_shadow, &probe);
    assert(softpc_ccpu_lifecycle_resume(&probe.entry));
    softpc_ccpu_lifecycle_clear_exit();
    assert(probe.halted == 1u);
    softpc_ccpu_lifecycle_observe(NULL, NULL);
}

static void observe_timeout(void *context, unsigned long depth,
    const softpc_ccpu_entry *entry)
{
    checkpoint_probe *state = context;
    ++state->calls; /* No instruction/iteration budget: only the host deadline. */
    if (!softpc_snapshot_checkpoint(&state->snapshot, depth, entry)) return;
    assert(depth == 2u && entry->halted && !state->nested_returned);
    assert(state->snapshot.phase == SOFTPC_SNAPSHOT_FAILED);
    assert(state->snapshot.status == LIB_STATUS_LIMIT_EXCEEDED);
    ++state->timeouts;
    /* The failed capture does not unwind. The test explicitly stops the CPU
       for cleanup; no saved continuation is claimed for this inner stack. */
    softpc_machine_request_stop(state->machine);
}

static void verify_timeout(void)
{
    const unsigned char outer[] = { 0xc4, 0xc4, 0xf0, 0xf4 };
    const unsigned char inner[] = { 0xfa, 0xf4 };
    void (*prior_bop)(void);
    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x500u, outer,
        sizeof(outer)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x600u, inner,
        sizeof(inner)) == SOFTPC_MACHINE_OK);
    assert(c_setCS(0u) == 0);
    c_setEIP(0x500u);
    probe.calls = probe.timeouts = 0u;
    probe.nested_returned = 0;
    prior_bop = BIOS[0xf0];
    BIOS[0xf0] = nested_bios;
    softpc_ccpu_lifecycle_observe(observe_timeout, &probe);
    assert(softpc_machine_run(probe.machine, UINT64_MAX) == SOFTPC_MACHINE_OK);
    softpc_ccpu_lifecycle_observe(NULL, NULL);
    BIOS[0xf0] = prior_bop;
    assert(probe.timeouts == 1u && !probe.nested_returned);
    assert(softpc_snapshot_finish(&probe.snapshot) == LIB_STATUS_OK);
}

int main(void)
{
    const char *path = "softpc-checkpoint-smoke.img";
    unsigned char sector[512] = {0};
    /* cli; BOP f0; hlt; inc ax. The last instruction must not execute. */
    const unsigned char outer[] = {0xfa, 0xc4, 0xc4, 0xf0, 0xf4, 0x40};
    const unsigned char inner[] = {0x90, 0xc4, 0xc4, 0xfe};
    softpc_machine_options options = {0};
    void (*prior_bop)(void);
    FILE *file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
    options.floppy_path = path;
    options.media_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    assert(softpc_machine_create(&options, &probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x500u, outer,
        sizeof(outer)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x600u, inner,
        sizeof(inner)) == SOFTPC_MACHINE_OK);
    assert(c_setCS(0u) == 0);
    c_setEIP(0x500u);
    c_setEAX(0x1234u);
    prior_bop = BIOS[0xf0];
    BIOS[0xf0] = nested_bios;
    softpc_ccpu_lifecycle_observe(observe, &probe);
    assert(softpc_machine_run(probe.machine, UINT64_MAX) == SOFTPC_MACHINE_OK);
    softpc_ccpu_lifecycle_observe(NULL, NULL);
    BIOS[0xf0] = prior_bop;
    assert(probe.halted == 1u && probe.nested_returned && probe.captures == 1u);
    assert(c_getEAX() == 0x1234u && c_getEIP() == 0x505u);
    verify_reentry();
    verify_timeout();
    softpc_machine_destroy(probe.machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
