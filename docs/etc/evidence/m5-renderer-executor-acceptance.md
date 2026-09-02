# M5 Renderer And Executor Acceptance

## Boundary

The compiled graphics coordinator is the recovered original `nt_graph`
dispatcher.  The standalone host provides a finite DIB/text surface and
invalidation endpoint only; copied runtime frames remain clients of that
surface.  No controller, firmware, ROM, BOP selector or guest-media byte is
changed by this result.

The bounded public machine run returns at the generated CCPU `NEXT_INST`
safe point.  Its standalone outer-unwind adapter now preserves the original
`ccpu386Unsimulate` simulation-stack invariant: the active saved context is
decremented before `longjmp`.  This prevents a second finite run from jumping
to an invalid earlier invocation.

## Verification

- Clean x64 and Clang x86 builds completed.
- The isolated `softpc-machine-smoke` completed with exit status zero on both
  widths after the generated CCPU adapter correction.
- Each complete CTest suite passed 16 of 16 tests on x64 and x86.
- The runtime real-media probe used overlay mode on the configured floppy and
  fixed-disk inputs.  On both widths it entered `c:\ewin31\setup.exe` via the
  original normalized-key path and accepted success only after the copied
  original text surface contained `Welcome to Setup.`.

The real-media probe is acceptance evidence only; no local guest path, guest
data or build output is a repository dependency.
