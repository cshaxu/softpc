# M3 T4 S2 Runtime Mailbox Result

## Closure

`src/vm/runtime` now supplies the only VM executor.  It owns the original
SoftPC machine for its whole lifetime and has one persistent worker thread.
That worker alone performs reset, enables the original heartbeat, enters the
continuous CCPU invocation, drains queued input, copies original presenter
surfaces, and disables the heartbeat after a requested outer-frame stop.

`start` does not create a replacement CCPU thread.  It asks the persistent
worker to reset and run again.  `stop` requests the proven outer-frame exit
and leaves that worker alive, waiting for the next start.  This is necessary
because the original CCPU thread-local simulation state belongs to the thread
that initialized it; repeatedly creating execution threads would be an
unverified change to the historical executor contract.

## Boundary

- Frontends can enqueue scan-code and mouse records only.
- The CCPU-thread timer rendezvous drains those records and calls the existing
  machine input API.
- The same rendezvous copies the original text surface or original `nt_*ga`
  DIB surface into a double-buffered runtime mailbox.
- Frame readers receive copies under the mailbox lock; they never retain a
  SoftPC pointer or call its presenter.
- Pause waits at the CCPU-thread rendezvous, retaining the original CCPU
  frame.  Stop releases that wait and uses the S1 outer-frame request.

No controller, renderer, BIOS, ROM, BOP selector, or guest-media code changed.

## Verification

`softpc-runtime-smoke` starts a guest `jmp $` floppy through the runtime,
copies a presentation frame, pauses/resumes, stops, then starts/stops again
on the same persistent executor before teardown.  Its temporary guest disk
uses overlay access.

Focused CMake/Ninja regression passed on both host widths:

- `softpc-runtime-smoke`
- `softpc-lifecycle-smoke`
- `softpc-bop-smoke`
- `softpc-vga-frame-smoke`
- `softpc-quick-time-smoke`

The x86 build used the required `C:\\msys64\\mingw32\\bin` compiler runtime
path.  The x64 and x86 launchers were rebuilt to `build/output/softpc64.exe`
and `build/output/softpc32.exe` respectively.
