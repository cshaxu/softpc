# M9 T84 S11 — fresh-checkout Quick Start closure

The owner accepted S11 on 2026-09-27. P1 `e238a470` makes both supported
new-user routes explicit:

- A 64-bit Windows user can clone the repository and directly launch the
  checked-in `assets/binary/softpc64.exe`, beside its default INI and bundled
  Windows 3.1 overlay media.
- A source user can install the documented MSYS2 UCRT64 toolchain and build
  the same package through the existing x64 CMake preset. An optional MSYS2
  MINGW32 route builds x86.

The x86 preset no longer contains an author's `D:/programs` toolchain path or
private `SOFTPC_I686_*` environment variables. Both x86 and x64 now select
`gcc` from the architecture-specific shell PATH, while retaining the existing
package-width validation.

Against `0e59f84d`, the only CMake change is `+1/-6` (net `-5`). Documentation
is `+180/-22` (net `+158`), including the retained proposal. The total tracked
change is `+181/-28` (net `+153`) across five paths. No C/C++ source, ABI,
Lib/Common/x86 corpus, App/Core, user INI, guest media, snapshot, or package
EXE change is retained.

A fresh x64 build directory configured with the exact preset settings,
identified a 64-bit GNU compiler and built `softpcvm` successfully. A separate
fresh x86 build directory with a real 32-bit MinGW PATH configured with a
32-bit GNU compiler, and both named presets reconfigured successfully.
Documentation governance, stale-marker sweep and whitespace checks passed.
The temporary build directories were removed; the package EXE generated for
the proof was restored to its accepted tracked content.
