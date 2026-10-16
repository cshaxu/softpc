#ifndef SOFTPC_COMPAT_EOI_H
#define SOFTPC_COMPAT_EOI_H

/* Original host ABI, owned and called only by the machine executor.
 * A negative call count is the PIC's pending-request withdrawal notice. */
typedef void (*EOIHOOKPROC)(int irq, int count);
int RegisterEOIHook(int irq, EOIHOOKPROC hook);
int RemoveEOIHook(int irq, EOIHOOKPROC hook);
void host_EOI_hook(int irq, int count);
/* Delay blocks delivery, not PIC request accounting. 0xffffffff cancels only
 * the scheduled release; the device separately withdraws its pending IRQ. */
extern unsigned long DelayIrqLine;
int host_DelayHwInterrupt(int irq, int count, unsigned long delay);

#endif
