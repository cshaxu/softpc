# M3 T4 S3 Frontend Client Result

## Closure

The console and Win32 window no longer execute, reset, lock, or inspect the
SoftPC machine.  They are both clients of `vm/runtime`:

- `main` builds the fixed machine from adjacent `softpc.ini`, creates one
  runtime, and routes monitor requests through it;
- console input is translated by the existing original `nt_keycd` mapping,
  then queued as key records; console text comes from copied runtime frames;
- window messages enqueue key/mouse records; its 16 ms paint timer copies a
  runtime frame and renders only that host-owned copy;
- window graphics calls `StretchDIBits` on the original `nt_cga`/`nt_ega`/
  `nt_vga` DIB copied by the runtime.  No alternate plane or DAC decoder was
  introduced.

The prior window runner, its machine lock, private input queue and direct DIB
access were removed.  There is now one executor, in `runtime`, instead of one
per frontend.

`pause` waits for the runtime's CCPU-thread safe point before it returns.  It
therefore cannot race the monitor's next `resume`.  `floppy insert/eject` is a
stopped-machine request executed by the persistent runtime worker, preserving
the one-owner rule.

## Enforcement and Verification

`standalone_source_boundary.cmake` now rejects any `softpc_machine_*` access
from either display frontend.  The only remaining machine calls in `src/vm`
are in `runtime.c`, where the executor owns them.

The following passed on x64 and x86:

- rebuild of both fixed launchers (`build/output/softpc64.exe` and
  `build/output/softpc32.exe`);
- `softpc-runtime-smoke`, including pause/resume, repeat start/stop and a
  serialized stopped-machine floppy eject;
- `softpc-lifecycle-smoke`, `softpc-bop-smoke`, `softpc-vga-frame-smoke`, and
  `softpc-quick-time-smoke`;
- `softpcvm-reject-arguments`, preserving the fixed-INI/no-CLI contract.

This closes M3 frontend ownership.  M4 remains responsible for actual RDP
interaction and dual-width real-media/Windows Setup acceptance, not merely
the compile-time mailbox boundary.
