#include "vm/machine.h"
#include "compat/ccpu/abi.h"
#include "compat/ccpu/lifecycle.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>

extern void (*BIOS[256])(void);

typedef struct checkpoint_probe {
    softpc_machine *machine;
    unsigned calls, nested, halted;
    int nested_returned;
} checkpoint_probe;

static checkpoint_probe probe;

static void nested_bios(void)
{
    /* Deliberately keep return state on a real C stack, exactly like the
       original keyboard/video BIOS callbacks. BOP FE must return here. */
    unsigned long saved_ip = c_getEIP();
    c_setEIP(0x600u);
    c_cpu_simulate();
    assert(c_getEIP() == 0x604u);
    c_setEIP(saved_ip);
    probe.nested_returned = 1;
}

static void observe(void *context, unsigned long depth, int halted)
{
    checkpoint_probe *state = context;
    unsigned long ip = c_getEIP();
    assert(state == &probe);
    assert(++state->calls < 100u);
    assert(depth == 1u || depth == 2u);
    if (depth == 2u) {
        assert(!state->nested_returned && !halted);
        assert(ip >= 0x600u && ip < 0x604u);
        ++state->nested;
        return; /* An inner CPU entry is not an outer capture boundary. */
    }
    if (halted) {
        assert(state->nested_returned && state->nested != 0u);
        assert(ip == 0x505u); /* HLT has already advanced IP. */
        ++state->halted;
        softpc_machine_request_stop(state->machine);
    }
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
    assert(probe.halted == 1u && probe.nested_returned);
    assert(c_getEAX() == 0x1234u && c_getEIP() == 0x505u);
    softpc_machine_destroy(probe.machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
