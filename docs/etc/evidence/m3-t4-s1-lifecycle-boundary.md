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

## S1 Closure: Generated Outer-Frame Adapter

Under the owner-approved standalone/NXVM architecture, the selected adapter
was implemented solely through the reproducible CCPU port-ABI path:

- generated `c_main.c` brackets each original `c_cpu_simulate()` invocation
  with a thread-local depth record;
- a standalone request is honoured by `c_cpu_unsimulate()` only when that
  depth is one, from the original CCPU-thread `host_timer_event()` rendezvous;
- original nested `host_simulate`, BOP `FE`, device timing, BIOS and renderer
  paths retain their existing transfer behavior;
- the VM executor explicitly registers with the original `ntthread.c` TLS
  simulation stack before it enters CCPU, and unregisters after return.

The lifecycle smoke executes a real guest `jmp $` loop in a worker, enables
the original host heartbeat, requests stop from outside that worker, waits for
the outer CCPU call to return, then destroys the machine.  It uses an overlay
fixture and leaves no guest-media mutation.

## Verification

- x64 CMake/Ninja build completed; `softpc-lifecycle-smoke`,
  `softpc-bop-smoke`, `softpc-vga-frame-smoke`, and
  `softpc-quick-time-smoke` passed.
- x86 CMake/Ninja build completed with `C:\\msys64\\mingw32\\bin` on `PATH`
  (required for the MinGW32 compiler backend runtime); the same four tests
  passed.

This closes the lifecycle-boundary predecessor.  M3 T4 now proceeds to its
runtime-owned command/input/frame mailbox; it does not yet claim frontend or
Windows Setup acceptance.
