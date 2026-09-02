# M5 Outer Runtime Frame Cadence

## Defect

The original SoftPC renderer publishes a copied frame from the executor.  The
standalone console and Win32 window then copied that complete frame on every
10 ms / 16 ms frontend turn, even when no executor publication had occurred.
For a graphics frame this repeatedly copied up to 786432 bytes; the window
also invalidated and repainted its full client area every 16 ms.  This was an
outer-shell cost, not a SoftPC CPU, C-VID, VGA, BIOS, ROM, BOP, PIC, timer or
controller behavior.

## Change

`softpc_runtime_publish` now atomically exposes the sequence number only
after it has finished the existing copied-frame publication.  Console and
window keep their ordinary input/message cadence, but ask for the full copied
snapshot and schedule paint only when that number changes.  There is still:

- one executor-owned original renderer;
- one runtime-owned double-buffered copied frame;
- one lock-protected snapshot interface;
- no frontend access to guest or controller state.

This follows the NXVM presentation-cadence principle: host input polling may
remain responsive without turning an unchanged guest display into repeated
large snapshots and repaints.

## Proof

`softpc-runtime-smoke` now requires a nonzero published sequence and verifies
that the copied frame reports that exact sequence.  Full CTest passes 16/16
on the x64 MinGW build and 16/16 on the x86 Clang build.

## Explicitly Not Closed

This is not evidence that Windows Setup graphics, console/window/RDP input or
interactive perceived latency are fully resolved.  Those require the current
S5 real-media frontend observations and probes.  The change is deliberately
limited to the proven redundant outer-frame work.
