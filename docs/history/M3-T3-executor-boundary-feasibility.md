# M3 T3: Executor-Boundary Feasibility

## Admission

The owner approved the NXVM-style standalone architecture under
single-person dual-role execution: one SoftPC executor, copied frontend
records, and preservation of original CPU, device, BIOS, ROM, VGA-ROM and BOP
behavior.

## S1: Reject Instruction-Count Return As A Runtime Boundary

**Status:** complete.

The original CCPU40 executor does not honor the standalone wrapper's
instruction budget as a public return contract. A generated `DO_INST` trial
that used `c_cpu_unsimulate()` could return from an unrelated nested
`host_simulate()`/BOP-FE frame, and was removed. The paired-width C-VID GDP
port-ABI correction remains separately verified.

## S2 P1: Identify The Original Event Rendezvous

**Status:** complete with predecessor transfer.

Original `nt_timer.c` proves that a host heartbeat publishes
`CPU_TIMER_TICK`, after which CCPU invokes `host_timer_event()` on its own
thread. This is a valid event/input-consumption point, not a lifecycle return
boundary. The detached build lacks the original timer/ICA locking contract;
recovering it requires the queued M2 host-compatibility work. M3 did not add
an instruction hook, synthetic clock, BOP reinterpretation, or frontend lock.

## Transfer

M2 T4 S1 owns restoration of the finite original timer/ICA compatibility
contract: the original heartbeat order, ICA lock boundary, and `REAL_TIMER`
translation-unit selection. It must first prove tick cardinality and bounded
dual-media POST on x86 and x64. M3 runtime work resumes only after that proof.

## Evidence

[M3 executor boundary and C-VID width audit](../etc/evidence/m3-t3-s1-boundary-and-cvid-width.md).
