# M3 T3 S1 Return Boundary And C-VID Width Audit

## Procedure

The standalone machine wrapper sets an instruction budget and calls original
`c_cpu_simulate()`.  A generated-only hook was temporarily placed at CCPU's
`DO_INST` label to consume that budget by its existing
`c_cpu_unsimulate()` transfer.  The pristine `c_main.c` was not changed.

The bounded real-media probe returned while each call was limited to 100
instructions, but a probe with the normal 50,000-instruction wrapper budget
did not return before a 10-second host watchdog terminated it.  The original
CCPU can remain inside an individual generated C-VID/string action long enough
that `DO_INST` is not a public yield point.  Therefore instruction accounting
is neither a safe executor boundary nor a permitted guest clock.  The temporary
hook and diagnostic exception filter were removed.

Original source comparison shows that `c_cpu_unsimulate()` is designed to
return nested `host_simulate()` calls and the firmware's BOP `FE` exit.  It is
not a general scheduler yield.  M3 must next inventory those original return
reasons and select a host-owned execution model without repurposing BOP FE or
adding instruction-rate pacing.

A two-second debugger sample of the stopped main machine thread was at the
ordinary `ccpu` next-instruction bookkeeping path, not inside a controller or
renderer callback.  This confirms that the direct launcher is an unbounded
CPU loop, rather than evidence for a second machine executor.

## C-VID x64 Finding

The same real boot initially reached a generated C-VID rule which treated a
historical 32-bit GDP base plus offset as a native pointer.  The reproducible
generator already translated unsigned dynamic and indirect forms but omitted
the otherwise identical signed `IS32`, `IS16`, and `IS8` fields.  The generator
now routes those forms through `softpc_gdp_rule_slot` or
`softpc_gdp_rule_address`, preserving the original GDP offset ABI while using
native storage only behind the port-ABI layer.

## Verification

- Regenerated and built the x64 and x86 CMake/Ninja graphs successfully.
- In each width, `softpc-vga-frame-smoke`, `softpc-bop-smoke`, and
  `softpc-quick-time-smoke` passed.
- A generated-source scan found no remaining direct `I[US](32|16|8|H)` dynamic
  GDP dereference based on the rule's `r1` GDP field.

This is not Windows Setup or frontend acceptance evidence.  The missing safe
executor boundary remains the active M3 stop condition.

## Rejected Mailbox Trial

A subsequent generated CCPU experiment attempted to combine copied-runtime
mailbox draining with delivery of the standalone timer queue through
`c_cpu_interrupt(CPU_TIMER_TICK, ...)`. The first implementation crossed host
functions at every guest instruction; a gated variant still changed the
original reset/FDC POST behavior. The runtime remained in POST and the
existing dual-media reset probe became CPU-bound. The implementation and its
generated-hook rule were removed. A future M3 task must restore the original
host-event delivery contract independently of mailbox lifecycle work.

## S2 P1 Source-Backed Rendezvous Result

The original `host/src/nt_timer.c` establishes the actual Windows SoftPC
ordering. Its heartbeat thread enters the original ICA critical section,
advances `time_tick()` and RTC work, then calls
`cpu_interrupt(CPU_TIMER_TICK, 0)`. In the selected CCPU40 build this is
`c_cpu_interrupt(CPU_TIMER_TICK, 0)`. At the next CCPU interrupt check,
including the HLT loop, the original executor clears
`CPU_SIGALRM_EXCEPTION_MASK` and invokes `host_timer_event()` on the CCPU
thread.

That makes `host_timer_event()` a valid original **input/event consumption
point**: a future copied input mailbox may be drained there by the executor.
It is not a CCPU return, pause, reset, stop, or media-swap boundary. The only
ordinary CCPU outward transfer remains the nested `host_simulate()`/BOP-FE
path already rejected in S1.

The detached platform currently has a 50 ms timer-queue callback that only
increments `softpc_clock_pending_ticks`; no compiled source consumes that
pending value. Its `ica.c` build is the non-NTVDM source branch, where
`host_ica_lock()` and `host_ica_unlock()` are empty. Consequently making that
callback call `time_tick()` would introduce unsynchronised controller mutation
from a host worker and violate both M3's stop condition and the runtime
proposal. It must not be repaired by a generated instruction hook, UI slice,
or a new guest clock.

The required predecessor is the queued M2 timer/ICA host-compatibility
package: recover the finite original heartbeat order and its locking boundary
through the port-ABI/host-compat layers, without importing NTVDM lifecycle,
WOW, console-server, or product service code. Only after that package has a
bounded dual-media proof can M3 attach a mailbox callback to the original
`host_timer_event()` consumption point. A separate owner-approved lifecycle
boundary is still required for pause, reset, stop, and live media swap.

### M2 Timer/ICA Recovery Closure

The required original-source closure is deliberately smaller than either the
whole `nt_eoi.c` or the whole NTVDM host product:

- `host/src/nt_eoi.c` supplies the three ICA lock operations
  `InitializeIcaLock`, `host_ica_lock`, and `host_ica_unlock`. Its delayed IRQ,
  monitor-TEB, WOW-idle, VDM virtual-ICA and IRET-hook product paths are not
  required by the selected standalone CCPU40 machine.
- `host/src/nt_timer.c` supplies the heartbeat order: initialise the lock and
  timer source; under the ICA lock run `time_tick()` and the original
  `cmosnt.c` `RtcTick`; then publish `CPU_TIMER_TICK`. Its console switching,
  DEM/WOW heartbeat, PIF priority, thread alert and CSR exception paths are
  unavailable product behavior, not compatibility requirements.
- `base/system/timestrb.c` already carries the original `REAL_TIMER` contract.
  When the external heartbeat owns timer/RTC progression, compiling this one
  translation unit with `REAL_TIMER` suppresses its otherwise non-NTVDM
  duplicate `time_tick()` and `rtc_tick()` calls while retaining the original
  tic-event and display-strobe flow. Defining `NTVDM` to obtain this effect is
  forbidden because it would change unrelated product branches.

M2 must make this selection reproducible through the port-ABI build graph,
place the new Win32 timer/lock implementation in `host/softpc-compat`, and
prove no duplicate tick with a bounded IRQ/BDA-time fixture on both x86 and
x64. The current `softpc-quick-time-smoke` only verifies wall-clock timestamp
measurement; it does not prove heartbeat delivery or tick cardinality.
