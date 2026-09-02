# M5 Paused Media Contract

## Defect

The standalone monitor advertised `floppy insert` and `floppy eject` while
paused, but the runtime accepted media changes only while stopped.  The
monitor could therefore make a documented request that the executor would
never service.

## Resolution

The monitor remains a command producer.  The sole executor now services a
pending floppy request while it is paused at the established CCPU callback,
then returns the result to the monitor.  The request still reaches the
existing `softpc_machine_set_floppy` machine boundary; no monitor thread reads
or writes FDC/controller state.

## Verification

- `softpc-runtime-smoke` now pauses a running machine, ejects floppy media,
  resumes, and stops it.
- Focused x64 MinGW and x86 Clang tests passed.
- Full serial CTest passed `17/17` on each width.
