#include "insignia.h"
#include "host_def.h"
#include "xt.h"
#include "ica.h"
#include "quick_ev.h"
#include "eoi.h"
#include "lib/types/types_interface.h"
#include "devices/snapshot.h"

/* One registration per original PIC line, like XP's host EOI boundary.
 * Device teardown removes its registration before withdrawing pending IRQs. */
static EOIHOOKPROC softpc_eoi_hooks[16];
unsigned long DelayIrqLine;
static q_ev_handle softpc_eoi_delay_handles[16];

static void softpc_eoi_release(long irq)
{
    IU32 adapter = (IU32)irq >> 3;
    IU8 line;
    softpc_eoi_delay_handles[irq] = 0;
    DelayIrqLine &= ~(1ul << irq);
    line = ica_scan_irr(adapter);
    if (line & 0x80) ica_interrupt_cpu(adapter, line & 7);
}

void softpc_device_snapshot_capture_eoi(softpc_device_eoi_state *state)
{
    lib_u32 irq;
    state->mask = (lib_u32)DelayIrqLine;
    for (irq = 0; irq < 16; ++irq)
        state->handles[irq] = (lib_u32)softpc_eoi_delay_handles[irq];
}

int softpc_device_snapshot_restore_eoi(const softpc_device_eoi_state *state)
{
    lib_u32 irq;
    if (state->mask > 0xffffu) return FALSE;
    for (irq = 0; irq < 16; ++irq)
        if (state->handles[irq] > 0xfffeu ||
            ((state->mask >> irq) & 1u) != (state->handles[irq] != 0)) return FALSE;
    DelayIrqLine = state->mask;
    for (irq = 0; irq < 16; ++irq)
        softpc_eoi_delay_handles[irq] = state->handles[irq];
    return TRUE;
}

int softpc_device_snapshot_encode_eoi_callback(Q_CALLBACK_FN callback,
    unsigned long *callback_id)
{
    if (callback != softpc_eoi_release) return FALSE;
    *callback_id = SOFTPC_DEVICE_QUEUE_DELAYED_IRQ;
    return TRUE;
}

Q_CALLBACK_FN softpc_device_snapshot_decode_eoi_callback(unsigned long callback_id)
{
    return callback_id == SOFTPC_DEVICE_QUEUE_DELAYED_IRQ ? softpc_eoi_release : NULL;
}

int host_DelayHwInterrupt(int irq, int count, unsigned long delay)
{
    unsigned long bit;
    q_ev_handle handle;
    if (irq < 0 || irq >= 16 || count < 0) return 0;
    bit = 1ul << irq;
    if (delay == 0xfffffffful) {
        if (softpc_eoi_delay_handles[irq] != 0) {
            delete_q_event(softpc_eoi_delay_handles[irq]);
            softpc_eoi_delay_handles[irq] = 0;
            DelayIrqLine &= ~bit;
        }
        return 1;
    }
    if (!(DelayIrqLine & bit)) {
        if (delay < 1000) delay = 1000;
        /* Preserve XP's interpreted-CPU quick-event delay and overhead offset.
         * This is not a second wall-clock timer or a physical-audio deadline. */
        handle = add_q_event_i(softpc_eoi_release, delay - 200, irq);
        if (handle == 0 || handle == 0xffff) return 0;
        softpc_eoi_delay_handles[irq] = handle;
        DelayIrqLine |= bit;
    }
    if (count != 0) ica_hw_interrupt((IU32)irq >> 3, (IU32)irq & 7, count);
    return 1;
}

int RegisterEOIHook(int irq, EOIHOOKPROC hook)
{
    if (irq < 0 || irq >= 16 || hook == LIB_NULL ||
        softpc_eoi_hooks[irq] != LIB_NULL) return 0;
    softpc_eoi_hooks[irq] = hook;
    return 1;
}

int RemoveEOIHook(int irq, EOIHOOKPROC hook)
{
    if (irq < 0 || irq >= 16 || hook == LIB_NULL ||
        softpc_eoi_hooks[irq] != hook) return 0;
    softpc_eoi_hooks[irq] = LIB_NULL;
    return 1;
}

void host_EOI_hook(int irq, int count)
{
    if (irq >= 0 && irq < 16 && softpc_eoi_hooks[irq] != LIB_NULL)
        softpc_eoi_hooks[irq](irq, count);
}
