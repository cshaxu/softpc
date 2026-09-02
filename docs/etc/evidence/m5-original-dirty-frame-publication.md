# M5 Original Dirty-Frame Publication

## Defect

The runtime copied a complete graphics DIB at every executor callback.  The
frontend cadence gate avoided redundant client copies, but did not prevent
this earlier outer-runtime copy.  Static graphics modes could therefore spend
executor time repeatedly copying up to 786432 bytes without a renderer update.

## Resolution

The runtime now consumes the existing original `nt_graph`/`nt_ega`/`nt_vga`
dirty-DIB signal before copying a graphics snapshot.  It always copies the
first frame of a text-to-graphics transition, and otherwise publishes only an
originally invalidated frame.  Text publication and all guest timing/input
paths are unchanged.

## Verification

- x64 and x86 `softpc-vga-frame-smoke` passed, including original V7 painter
  dirty-rectangle assertions.
- x64 and x86 `softpc-runtime-smoke` passed.
- Full serial CTest passed `17/17` on both host widths.
- `softpc64.exe` and `softpc32.exe` were relinked into `build/output`.
