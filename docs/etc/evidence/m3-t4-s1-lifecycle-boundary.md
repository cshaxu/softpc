# M3 T4 S1 Lifecycle Boundary Result

## Finding

The original CCPU40 implementation has one outward non-local transfer:
`c_cpu_unsimulate()`.  Its documented use is BOP `FE` or a PIG return.
On the selected standalone build, the TLS implementation in
`base/ccpu386/ntthread.c` decrements the current thread's simulation-stack
level and longjmps to that exact frame.

This is not a VM lifecycle boundary.  The original machine invokes
`host_simulate()` recursively from floppy, keyboard, mouse and video paths.
An asynchronous `stop` or `reset` request observed during a timer event could
therefore longjmp out of one of those device operations rather than the outer
VM invocation.  It would abandon the caller's restored register/interrupt
state and alter original controller behavior.

`host_timer_event()` remains a valid CCPU-thread rendezvous for copied input
and frame publication.  It does not prove that its current simulation frame is
the outer one, nor does it cause `c_cpu_simulate()` to return.  Pausing there
is safe only while retaining the live CCPU frame; it cannot implement the
required `stop`, `reset`, teardown, or media swap lifecycle.

## Rejected Uncommitted Trial

An uncommitted `vm/runtime` worker was built on the CCPU-thread timer
rendezvous.  It could queue keyboard records and wait for a pause request,
but it had no safe join/teardown path.  Its destructor would either leave a
live executor or risk freeing the callback context under CCPU.  The trial was
removed before frontend integration; no runtime or build-graph change remains.

## Required Approval Before Implementation Continues

The smallest possible next experiment is a reproducible CCPU port-ABI overlay,
not a device, BIOS, BOP, or renderer change.  It must expose the current CCPU
simulation depth and add a dedicated runtime-exit transfer that is accepted
only at outer depth one.  Requests observed at a nested frame are deferred
until an outer original heartbeat rendezvous.  The adapter must have separate
x64/x86 proofs showing that BOP `FE` retains its original one-frame semantics.

Without explicit approval for that narrow generated port-ABI adapter, M3 T4
cannot truthfully claim the required lifecycle contract and frontends cannot
be migrated to it.
