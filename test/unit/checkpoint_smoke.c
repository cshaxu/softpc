#include "vm/machine.h"
#include "compat/ccpu/abi.h"
#include "compat/ccpu/lifecycle.h"
#include "compat/devices/snapshot.h"
#include "vm/snapshot.h"
#include "compat/platform.h"
#include "../lib/cleanup.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "ios.h"
#include "ica.h"
#include "cmos.h"
#include "c_tlb.h"
#include "c_debug.h"
#include "quick_ev.h"
/* base_def.h's non-ANSI compatibility macro must not alter this C17 test. */
#undef const

extern void (*BIOS[256])(void);
extern unsigned long c_cpu_q_ev_get_count(void);
extern void c_cpu_q_ev_set_count(unsigned long count);
extern void dispatch_tic_event(void);
extern IU32 calc_q_inst_for_time(IU32 time);
extern IU32 calc_q_time_for_inst(IU32 count);
extern IBOOL DisableQuickTickRecal;
extern IU32 CCPU_DR[8];
extern IU32 NpxFIP;
extern IBOOL NpxIntrNeeded;
extern void DoNpxException(void);
extern void TakeNpxExceptionInt(void);
extern IU32 getNpxControlReg(void);
extern void setNpxControlReg(IU32 value);
extern IU32 getNpxStatusReg(void);
extern void setNpxStatusReg(IU32 value);

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
static unsigned event_count;
static void record_event(long param);

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

static void verify_translation(void)
{
    const unsigned char directory[] = {0x03, 0x20, 0, 0};
    const unsigned char old_page[] = {0x03, 0x30, 0, 0};
    const unsigned char new_page[] = {0x03, 0x40, 0, 0};
    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x1004u, directory,
        sizeof(directory)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x2000u, old_page,
        sizeof(old_page)) == SOFTPC_MACHINE_OK);
    c_setCR3(0x1000u);
    /* The original translation entry point is intentionally exercised without
       running instructions: page-table writes do not invalidate its TLB. */
    assert(lin2phy(0x400123u, 0) == 0x3123u);
    assert(softpc_machine_write_physical(probe.machine, 0x2000u, new_page,
        sizeof(new_page)) == SOFTPC_MACHINE_OK);
    assert(lin2phy(0x400123u, 0) == 0x3123u);
    flush_tlb();
    assert(lin2phy(0x400123u, 0) == 0x4123u);
    /* A load that merely flushed TLB would therefore not restore this state. */
    flush_tlb();
}

static long event_order[8];

static void verify_cpu_side_state(void)
{
    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    CCPU_DR[0] = 0x1234u;
    CCPU_DR[7] = 1u; /* Local instruction breakpoint 0. */
    setup_breakpoints();
    assert(nr_inst_break == 1u && nr_data_break == 0u);
    /* c_tsksw clears the local DR7 enable bits without rebuilding the table.
       Rebuilding during snapshot load would silently change this behavior. */
    CCPU_DR[7] &= ~0x155u; /* Original LOCAL_BRK_ENABLE, including LE. */
    CCPU_DR[6] = 0u;
    check_for_inst_exception(0x1234u);
    assert(CCPU_DR[6] == 1u && nr_inst_break == 1u);
    setup_breakpoints();
    CCPU_DR[6] = 0u;
    check_for_inst_exception(0x1234u);
    assert(CCPU_DR[6] == 0u && nr_inst_break == 0u);

    c_setEFLAGS(2u); /* Observe deferred NPX delivery without entering an ISR. */
    NpxFIP = 0x12345678u;
    DoNpxException();
    assert(NpxIntrNeeded);
    NpxFIP = 0xabcdefu;
    TakeNpxExceptionInt();
    assert(!NpxIntrNeeded && NpxFIP == 0x12345678u);
    /* The pending exception has its own saved IP, distinct from current FIP. */
}

static void verify_snapshot_archive(void)
{
    const unsigned char directory[] = {0x03, 0x20, 0, 0};
    const unsigned char old_page[] = {0x03, 0x30, 0, 0};
    const unsigned char new_page[] = {0x03, 0x40, 0, 0};
    const unsigned char marker[] = {0x5a, 0xa5};
    const unsigned char altered[] = {0, 0};
    softpc_snapshot_image image = {0};
    softpc_ccpu_entry entry = {1, 0u}, restored = {0};
    unsigned char readback[2];
    half_word cmos_value;

    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x1004u, directory,
        sizeof(directory)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x2000u, old_page,
        sizeof(old_page)) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_write_physical(probe.machine, 0x5000u, marker,
        sizeof(marker)) == SOFTPC_MACHINE_OK);
    c_setEAX(0x12345678u);
    c_setEIP(0x7654u);
    c_setCR3(0x1000u);
    assert(lin2phy(0x400123u, 0) == 0x3123u);
    CCPU_DR[0] = 0x1234u;
    CCPU_DR[7] = 1u;
    setup_breakpoints();
    setNpxControlReg(0x027fu);
    setNpxStatusReg(0x2800u);
    cmos_outb(CMOS_PORT, CMOS_SHUT_DOWN);
    cmos_outb(CMOS_DATA, 0x5au);
    /* A callback without a reviewed semantic identifier blocks capture rather
       than being silently omitted from the pending-work image. */
    assert(add_q_event_i(record_event, 100u, 61) != 0);
    assert(softpc_snapshot_image_capture(&image, &entry) == LIB_STATUS_IO_ERROR);
    assert(event_count == 0u);
    q_event_init();
    assert(softpc_snapshot_image_capture(&image, &entry) == LIB_STATUS_OK);

    c_setEAX(0u);
    c_setEIP(0u);
    CCPU_DR[0] = CCPU_DR[7] = 0u;
    setup_breakpoints();
    setNpxControlReg(0x037fu);
    setNpxStatusReg(0u);
    cmos_outb(CMOS_PORT, CMOS_SHUT_DOWN);
    cmos_outb(CMOS_DATA, 0x42u);
    assert(softpc_machine_write_physical(probe.machine, 0x5000u, altered,
        sizeof(altered)) == SOFTPC_MACHINE_OK);
    flush_tlb();
    assert(softpc_snapshot_image_restore(&image, &restored) == LIB_STATUS_OK);
    assert(restored.halted && restored.trap == 0u);
    assert(c_getEAX() == 0x12345678u && c_getEIP() == 0x7654u);
    assert(CCPU_DR[0] == 0x1234u && CCPU_DR[7] == 1u);
    assert(getNpxControlReg() == 0x027fu && getNpxStatusReg() == 0x2800u);
    cmos_outb(CMOS_PORT, CMOS_SHUT_DOWN);
    cmos_inb(CMOS_DATA, &cmos_value);
    assert(cmos_value == 0x5au);
    assert(softpc_machine_read_physical(probe.machine, 0x5000u, readback,
        sizeof(readback)) == SOFTPC_MACHINE_OK);
    assert(readback[0] == marker[0] && readback[1] == marker[1]);
    /* The restored TLB must remain live even after the restored page table is
       changed; rebuilding or flushing it would instead produce 0x4123. */
    assert(softpc_machine_write_physical(probe.machine, 0x2000u, new_page,
        sizeof(new_page)) == SOFTPC_MACHINE_OK);
    assert(lin2phy(0x400123u, 0) == 0x3123u);
    softpc_snapshot_image_dispose(&image);
}

static void verify_pit_archive(void)
{
    softpc_device_pit_state saved, restored;

    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    /* Program channel zero into a non-default active state, then prove that
       a later incompatible reprogram is replaced by the semantic archive. */
    outb(0x43u, 0x36u);
    outb(0x40u, 0x34u);
    outb(0x40u, 0x12u);
    assert(softpc_device_snapshot_capture_pit(&saved));
    outb(0x43u, 0x30u);
    outb(0x40u, 0x78u);
    outb(0x40u, 0x56u);
    assert(softpc_device_snapshot_restore_pit(&saved));
    assert(softpc_device_snapshot_capture_pit(&restored));
    assert(restored.counter[0].mode == saved.counter[0].mode);
    assert(restored.counter[0].read_load == saved.counter[0].read_load);
    assert(restored.counter[0].initial_count == saved.counter[0].initial_count);
    assert(restored.counter[0].state == saved.counter[0].state);
    assert(restored.counter[0].action_on_wait_complete ==
        saved.counter[0].action_on_wait_complete);
}

static void verify_controller_archives(void)
{
    softpc_device_dma_state dma_saved, dma_restored;
    softpc_device_pic_state pic_saved, pic_restored;
    softpc_device_fdc_state fdc_saved, fdc_restored;
    softpc_device_hdd_state hdd_saved, hdd_restored;
    softpc_device_ppi_state ppi_saved, ppi_restored;

    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);

    /* Drive each controller through its normal port interface before taking
       the archive.  The subsequent capture proves restore reinstalled the
       semantic registers rather than merely retaining the saved C object. */
    outb(0x21u, 0x5au);
    ica_hw_interrupt(ICA_MASTER, 1u, 1);
    assert(softpc_device_snapshot_capture_pic(&pic_saved));
    outb(0x21u, 0xffu);
    assert(softpc_device_snapshot_restore_pic(&pic_saved));
    assert(softpc_device_snapshot_capture_pic(&pic_restored));
    assert(pic_restored.adapter[0].imr == pic_saved.adapter[0].imr);
    assert(pic_restored.adapter[0].irr == pic_saved.adapter[0].irr);

    outb(DMA_CLEAR_FLIP_FLOP, 0u);
    outb(DMA_CH2_ADDRESS, 0x34u);
    outb(DMA_CH2_ADDRESS, 0x12u);
    outb(DMA_CLEAR_FLIP_FLOP, 0u);
    outb(DMA_CH2_COUNT, 0x78u);
    outb(DMA_CH2_COUNT, 0x56u);
    outb(DMA_FLA_PAGE_REG, 0x9au);
    softpc_device_snapshot_capture_dma(&dma_saved);
    outb(DMA_CLEAR_FLIP_FLOP, 0u);
    outb(DMA_CH2_ADDRESS, 0u);
    outb(DMA_CH2_ADDRESS, 0u);
    outb(DMA_FLA_PAGE_REG, 0u);
    assert(softpc_device_snapshot_restore_dma(&dma_saved));
    softpc_device_snapshot_capture_dma(&dma_restored);
    assert(memcmp(dma_restored.base_address[0][2],
        dma_saved.base_address[0][2], 2u) == 0);
    assert(memcmp(dma_restored.base_count[0][2],
        dma_saved.base_count[0][2], 2u) == 0);
    assert(dma_restored.page[1] == dma_saved.page[1]);

    outb(DISKETTE_DOR_REG, 0x1cu);
    outb(DISKETTE_DATA_REG, 0x03u); /* FDC SPECIFY */
    outb(DISKETTE_DATA_REG, 0xdfu);
    outb(DISKETTE_DATA_REG, 0x02u);
    softpc_device_snapshot_capture_fdc(&fdc_saved);
    outb(DISKETTE_DOR_REG, 0u);
    assert(softpc_device_snapshot_restore_fdc(&fdc_saved));
    softpc_device_snapshot_capture_fdc(&fdc_restored);
    assert(fdc_restored.dor == fdc_saved.dor);
    assert(fdc_restored.current_command == fdc_saved.current_command);
    assert(fdc_restored.command_count == fdc_saved.command_count);

    outb(0x1f2u, 0x03u);
    outb(0x1f3u, 0x05u);
    outb(0x1f4u, 0x07u);
    assert(softpc_device_snapshot_capture_hdd(&hdd_saved));
    outb(0x1f2u, 0u);
    outb(0x1f3u, 0u);
    assert(softpc_device_snapshot_restore_hdd(&hdd_saved));
    assert(softpc_device_snapshot_capture_hdd(&hdd_restored));
    assert(memcmp(hdd_restored.taskfile, hdd_saved.taskfile,
        sizeof(hdd_saved.taskfile)) == 0);

    outb(0x61u, 0x01u);
    softpc_device_snapshot_capture_ppi(&ppi_saved);
    outb(0x61u, 0u);
    assert(softpc_device_snapshot_restore_ppi(&ppi_saved));
    softpc_device_snapshot_capture_ppi(&ppi_restored);
    assert(ppi_restored.register_value == ppi_saved.register_value);
    assert(ppi_restored.gate_2_was_low == ppi_saved.gate_2_was_low);
    assert(ppi_restored.speaker_data_was_low == ppi_saved.speaker_data_was_low);
}

static void record_event(long param)
{
    assert(event_count < sizeof(event_order) / sizeof(event_order[0]));
    event_order[event_count++] = param;
}

static void verify_event_queue(void)
{
    q_ev_handle quick_cancel, tick_cancel;
    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_platform_set_clock_running(0));
    /* Own these queues for the test; no device callbacks or wall-time sleeps. */
    q_event_init();
    tic_event_init();
    assert(DisableQuickTickRecal);
    assert(calc_q_inst_for_time(100u) == 10u);
    assert(calc_q_time_for_inst(3u) == 30u);
    assert(add_q_event_i(record_event, 100u, 11) == 1);
    assert(add_q_event_i(record_event, 100u, 12) == 2);
    quick_cancel = add_q_event_i(record_event, 200u, 13);
    assert(quick_cancel == 3 && c_cpu_q_ev_get_count() == 10u);
    /* The live counter may be below the head's original delay. Inserting
       another event must preserve this progress and the equal-deadline order. */
    c_cpu_q_ev_set_count(3u);
    assert(add_q_event_i(record_event, 50u, 14) == 4);
    assert(c_cpu_q_ev_get_count() == 3u && event_count == 0u);
    c_cpu_q_ev_set_count(0u);
    dispatch_q_event();
    assert(event_count == 2u && event_order[0] == 11 && event_order[1] == 12);
    assert(c_cpu_q_ev_get_count() == 2u);
    delete_q_event(quick_cancel);
    c_cpu_q_ev_set_count(0u);
    dispatch_q_event();
    assert(event_count == 3u && event_order[2] == 14);
    dispatch_q_event();
    assert(event_count == 3u);

    assert(add_tic_event(record_event, 3u, 21) == 1);
    assert(add_tic_event(record_event, 3u, 22) == 2);
    tick_cancel = add_tic_event(record_event, 6u, 23);
    dispatch_tic_event();
    dispatch_tic_event();
    assert(event_count == 3u);
    dispatch_tic_event();
    assert(event_count == 5u && event_order[3] == 21 && event_order[4] == 22);
    delete_tic_event(tick_cancel);
    dispatch_tic_event();
    assert(event_count == 5u);
    /* Replaying the add API during load would execute this immediately. */
    assert(add_q_event_i(record_event, 0u, 31) == 0);
    assert(add_tic_event(record_event, 0u, 32) == 0);
    assert(event_count == 7u && event_order[5] == 31 && event_order[6] == 32);
    q_event_init();
    tic_event_init();
    assert(softpc_platform_set_clock_running(1));
}

static int snapshot_encode_event(Q_CALLBACK_FN callback,
    unsigned long *callback_id)
{
    if (callback != record_event || callback_id == NULL) return 0;
    *callback_id = 1u;
    return 1;
}

static Q_CALLBACK_FN snapshot_decode_event(unsigned long callback_id)
{
    return callback_id == 1u ? record_event : NULL;
}

static void verify_event_queue_archive(void)
{
    Q_EVENT_SNAPSHOT_STATE state;
    Q_EVENT_SNAPSHOT_ENTRY quick[4], tick[4];
    unsigned long saved_callback;

    assert(softpc_machine_reset(probe.machine) == SOFTPC_MACHINE_OK);
    assert(softpc_platform_set_clock_running(0));
    q_event_init();
    tic_event_init();
    event_count = 0u;
    assert(add_q_event_i(record_event, 100u, 41) == 1);
    assert(add_q_event_i(record_event, 100u, 42) == 2);
    assert(add_tic_event(record_event, 3u, 51) == 1);
    c_cpu_q_ev_set_count(4u);
    assert(q_event_snapshot_capture(&state, quick, 4u, tick, 4u,
        snapshot_encode_event));
    assert(state.quick_entries == 2u && state.tick_entries == 1u);
    assert(state.quick_count == 4u && state.tick_count == 3u);
    assert(event_count == 0u);
    /* Rebuild must be all-or-nothing: a bad late tick callback must not
       discard the still-live quick queue while it validates the archive. */
    saved_callback = tick[0].callback_id;
    tick[0].callback_id = 99u;
    assert(!q_event_snapshot_restore(&state, quick, 4u, tick, 4u,
        snapshot_decode_event));
    tick[0].callback_id = saved_callback;
    assert(event_count == 0u && c_cpu_q_ev_get_count() == 4u);
    q_event_init();
    tic_event_init();
    assert(q_event_snapshot_restore(&state, quick, 4u, tick, 4u,
        snapshot_decode_event));
    /* Restore links records only: it must never run an event callback. */
    assert(event_count == 0u && c_cpu_q_ev_get_count() == 4u);
    c_cpu_q_ev_set_count(0u);
    dispatch_q_event();
    assert(event_count == 2u && event_order[0] == 41 && event_order[1] == 42);
    dispatch_tic_event(); dispatch_tic_event(); dispatch_tic_event();
    assert(event_count == 3u && event_order[2] == 51);
    q_event_init();
    tic_event_init();
    assert(softpc_platform_set_clock_running(1));
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
    verify_translation();
    verify_cpu_side_state();
    verify_snapshot_archive();
    verify_pit_archive();
    verify_controller_archives();
    verify_event_queue();
    verify_event_queue_archive();
    softpc_machine_destroy(probe.machine);
    assert(softpc_test_remove_image(path));
    return 0;
}
