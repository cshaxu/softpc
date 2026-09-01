# M4 Runtime Media And Input Result

## Fixed-media boot

The runtime-owned real-media probe starts the machine with the fixed 1.44 MiB
floppy and hard-disk images using `SOFTPC_MEDIA_OVERLAY`.  It accepts the
original DOS date/time prompts if present and consumes copied text frames only.
Both x64 and x86 reached `A:\\>` and returned success.  The guest image files
were not modified.

The earlier direct-slice `softpc-real-boot-smoke` is not valid for M4: it can
spin indefinitely in `softpc_machine_run(slice)`.  That is an obsolete test
shape, not evidence against the persistent runtime.

## Input rendezvous

The old runtime callback occurred only through the original 20 Hz timer.  That
made UI input wait up to a timer period before its mailbox was drained.

The original CCPU already defines `CPU_SIGIO_EVENT`.  The reproducible CCPU
port-ABI generator now consumes that event at both original instruction and
HLT event sites.  The standalone host requests it when a runtime input wake is
needed; its handler invokes only the executor callback.  It does not call
`host_timer_event`, so no PIT, video, controller, BIOS, BOP, or renderer time
is advanced by keyboard/mouse input.

Focused BOP, VGA, lifecycle, quick-time, runtime, and source-boundary tests
passed on both x64 and x86 after the change.  The real fixed-media runtime
probe also passed on both widths.

Windows Setup graphics and interactive RDP acceptance remain active M4 work.

## Snapshot responsiveness and test-boundary result

The executor may be copying an original renderer DIB while a console or window
asks for its next published frame.  `softpc_runtime_copy_frame` now uses a
non-blocking critical-section acquisition: a client retains its existing frame
when publication is in progress and retries on its next UI turn.  This changes
only the copied-frame boundary; it does not lock, inspect or alter SoftPC,
CCPU, device, BIOS, ROM, BOP or renderer state.

Focused x64 and x86 runtime, lifecycle, keycode, source-boundary and fixed-INI
tests passed, as did the overlay-mode real-media runtime boot/input probe on
both widths.  The legacy `softpc-machine-smoke` and
`softpc-dual-media-smoke` direct-run forms remain unsuitable for M4 because
they enter unbounded `softpc_machine_run`; they must be replaced by
runtime-owned equivalents under M5 rather than used as a full-suite gate.
