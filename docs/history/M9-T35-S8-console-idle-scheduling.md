# M9 T35 S8: Console Idle Scheduling

## Outcome

The standalone console no longer uses a 10 ms `Sleep` loop.  It waits on the
console input handle and the runtime's auto-reset frame-publication event,
with a 250 ms bounded pause/stop state check.  `PeekConsoleInputA` remains
only to drain a signalled input queue, preserving its existing keyboard and
mouse record processing.

## Boundaries Preserved

- The recovered SoftPC CPU, C-VID, controllers, BIOS, ROMs, BOP services,
  host timer, and guest media behavior are unchanged.
- The mandatory one-presenter-at-a-time console/window transition remains:
  a graphics frame exits the console presenter and lets the existing caller
  create the window presenter.
- The deadline supplies no guest tick and does not create a second clock.

## Verification

- WinLibs GCC x64: full CTest, 21/21 passed, including runtime, window,
  keyboard, lifecycle, and package smoke tests.
- MSYS2 MinGW32 GCC x86: full CTest, 21/21 passed with the same coverage.
- Refreshed `assets/binary/softpc64.exe` and `softpc32.exe`; the user-owned
  adjacent `softpc.ini` was not modified.
