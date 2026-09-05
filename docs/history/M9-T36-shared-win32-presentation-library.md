# M9 T36: Shared Win32 Presentation Library

## Outcome

SoftPC now has a local, synchronized-source Win32 presentation component at
`src/lib/platform/win32/`.  It owns a copied-value frame ABI, normalized
keyboard transitions and RDP-safe text input, DIB dirty-rectangle geometry,
native/aspect-preserving window sizing, and explicit relative-mouse capture.
`src/app/keyboard.c` is the thin SoftPC binding that alone maps normalized
Win32 records through the original selected key encoder and queues guest keys.
The window binding alone continues to choose monitor hotkeys, console/window
routing, pause semantics, and the original InPort mouse request.

`MANIFEST.sha256` plus its CMake test provides an LF-normalized source hash
contract for a later byte-identical copy into NXVM and NTVDM64.  This is a
source synchronization mechanism only: neither project is a SoftPC build,
runtime, or acceptance dependency.

## Boundaries Preserved

- No file under `src/mvdm/softpc.new/` changed.
- No CPU, C-VID, controller, BIOS, ROM, BOP, timer, guest-media, or INI
  behavior changed.
- The single-presenter console/window handoff and all product hotkeys remain
  app policy, outside the reusable component.

## Verification

- WinLibs GCC x64: full CTest, 23/23 passed.
- MSYS2 MinGW32 GCC x86: full CTest, 23/23 passed.  Its task-local PATH puts
  `D:\programs\msys64\mingw32\bin` before the inherited x64 toolchain so the
  relocated `cc1.exe` resolves the matching x86 DLLs; no global PATH changed.
- Refreshed `assets/binary/softpc64.exe` and `softpc32.exe`; the adjacent
  user-owned `softpc.ini` was not modified.
