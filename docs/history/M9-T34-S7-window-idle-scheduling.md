# M9 T34 S7: Window Idle Scheduling

## Outcome

The standalone Win32 window no longer wakes on a fixed 16 ms timer.  The
runtime signals an auto-reset event after publishing a complete copied frame;
the existing window message loop waits for that event, normal Win32 input, or
the 250 ms cursor/title maintenance deadline.  The copied frame remains the
only presentation data boundary.

## Boundaries Preserved

- No CCPU, C-VID, controller, BIOS, ROM, BOP, device timer, media, or INI
  behavior changed.
- The required one-presenter-at-a-time console/window transition lifecycle is
  unchanged.
- Window input continues to be serviced by normal Win32 message dispatch.

## Verification

- WinLibs GCC x64: full CTest, 21/21 passed, including runtime, Win32 window,
  keyboard, lifecycle, and package smoke tests.
- MSYS2 MinGW32 GCC x86: full CTest, 21/21 passed with the same coverage.
- Refreshed package executables: `assets/binary/softpc64.exe` and
  `assets/binary/softpc32.exe`.  The adjacent user-owned `softpc.ini` was not
  modified.
