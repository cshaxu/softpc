# M4 runtime-boundary audit

Date: 2026-09-01

## Reproducible findings

1. The public `softpc_machine_run(machine, instruction_budget)` contract is
   not a slice.  The wrapper records its budget in standalone globals, but the
   generated CCPU execution path never reads either value and enters
   `c_cpu_simulate()` indefinitely.
2. The standalone CCPU port ABI now replaces only the idle tail of the original
   `HLT` loop with an OS wait when no original interrupt or quick event is
   pending.  Timer and frontend wake records are mailbox state, consumed on
   the executor's original safe points; no UI thread mutates CCPU/PIC state.
3. The public executor still has no real slice boundary: `c_cpu_simulate()` is
   one outer invocation, so runtime ownership, stop and presentation must be
   defined around that invocation rather than inferred from the budget field.
4. Non-NT keyboard refill is original machinery: reading port `60h` schedules
   `allowRefill` through the original quick-event queue; the next scan byte is
   emitted only after that callback.  The bounded real-media probe reaches
   `A:\\>a`, then proves that `b` has reached the original 8042 output register
   (`output_full=1`, make code `30h`) and that the original PIC has queued IRQ1
   (`irr=03h`, `irq1_count=1`, CPU INT line asserted).  At that same point the
   CCPU interrupt-enable flag is clear (`IF=0`), so the original HLT loop
   correctly cannot accept the pending IRQ.  The direct ROM route is valid:
   IRQ1 enters `F000:E987`, whose original short jump leads to
   `F000:E942` (`BOP 09; POP AX; IRET`).  Both original keyboard buffer
   branches also issue their required EOI/keyboard-exit work.  The CCPU build
   was nevertheless enabling CPU_40 IRET-hook bookkeeping in `ica.c`, although
   its generated interrupt path receives and intentionally ignores
   `hook_address`.  The alternate original `ica.old` has the required
   `#ifndef CCPU` guard because CCPU does not support IRET hooks.  Restoring
   that guard makes the continuous real-media probe reach `A:\>abc` on both
   x64 and x86.  No controller, ROM, BOP or frontend mapping code changed.
5. `host_timer_event()` now restores the machine-facing graphics, quick-event,
   COM, LPT, time-strobe and speaker calls.  The original `host_flpy_heart_beat`
   only closes NT direct-floppy handles and services FDISK; it does not apply to
   the standalone raw-image GFI port and must remain absent.
6. The standalone HLT OS wait does not apply while the original quick-event
   counter is pending, so it is not a complete CPU-idle/pacing solution.  A
   temporary deadline bridge based on `calc_q_time_for_inst()` was rejected:
   the available synthetic lifecycle fixture never reached its intended HLT
   boot-sector loop and therefore cannot establish a performance claim.  In
   addition, the detached lifecycle wrapper had called
   `c_cpu_init()` on every reset even though that original routine allocates a
   thread-local simulation-stack slot.  It now initializes once per machine
   lifetime, and the cross-thread Setup probe explicitly enters/leaves the
   original CCPU TLS context.  The lifecycle/runtime/keycode regressions and
   real-media consecutive-key probe now pass on both host widths.

## Consequence

Do not add further window throttles, input queues, direct controller calls, or
instruction-budget hooks.  The next admitted implementation must define one
CCPU outer-frame wait/scheduling ABI that:

- leaves original controller, BIOS, BOP and quick-event algorithms intact;
- delivers frontend mailbox records only on the executor thread;
- waits while guest HLT is idle but honours the original timer and quick-event
  deadlines; and
- has x64/x86 evidence for CPU idle, FDC boot and Windows Setup progression.

This is evidence for the queued M5 outer-runtime/pristine-restoration work;
it does not admit a numeric implementation task.
