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
