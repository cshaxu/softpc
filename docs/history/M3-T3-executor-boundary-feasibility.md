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

## S2 P2: Reject A Timer Retrofit Beneath The Legacy Slice API

**Status:** complete; no source change retained.

The standalone timer queue was temporarily changed from a pending counter to
the original `CPU_TIMER_TICK` publication.  The existing
`softpc-machine-smoke` immediately became CPU-bound: its `softpc_machine_run`
"slice" enters an unbounded CCPU call and has no lifecycle return after the
timer event begins servicing original `time_strobe()` work.  Delaying the
producer until after reset does not repair that contract; it merely moves the
failure into the first legacy run call.  The experiment and its generated
CMOS image were removed.

This proves that timer/ICA recovery is not an independent predecessor.  Its
producer must be owned by the new runtime, started only after that runtime
owns execution, and retired before it destroys the machine.  It must not be
enabled under the legacy direct-run API.

## Transfer

M3 T4 owns the single-executor runtime foundation.  It must establish the
runtime lifecycle and copied command/input mailbox first; that runtime then
becomes the only permitted owner of original heartbeat activation.  M2 timer
and ICA capability recovery remains a nested implementation item of that
runtime package, not a pre-runtime device retrofit.

## Evidence

[M3 executor boundary and C-VID width audit](../etc/evidence/m3-t3-s1-boundary-and-cvid-width.md).
