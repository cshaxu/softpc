# M8 T15 Source-layout Build Verification

## Source-layout Checks

- `CMakeLists.txt` and `test/support/standalone_source_boundary.cmake` contain
  no remaining `src/core/softpc/` build reference.
- `src/mvdm/softpc.new/` tracks 252 C files, 575 headers, one required
  `base/ccpu386/c-files` CMake input list, and only the selected firmware:
  `roms/bios1.rom`, `roms/bios4.rom`, `roms/v7vga.rom`, and `roms/cmos.ram`.
- No tracked object, library, resource-object, or debugger intermediate exists
  below `src/`.
- The 12 unselected `suballoc/` and `xms.486/` C/H files stay outside the
  `softpc.new` mirror pending their T19 removal disposition; they are not
  selected by CMake.

## Build And Test Evidence

| Host width | Compiler | Build directory | Result |
| --- | --- | --- | --- |
| x64 | WinLibs MinGW GCC | `build/test-x64` | Clean rebuild; CTest 20/20 passed. |
| x86 | MSYS2 MinGW GCC | `build/test-x86` | Clean rebuild; CTest 20/20 passed. |

The x86 compiler requires `D:\programs\msys64\mingw32\bin` before the
ambient `PATH`; otherwise its existing `cc1.exe` cannot load its 32-bit
dependencies and exits with `0xC000007B`. This is a local toolchain process
environment requirement, not a source-layout change.

## Rebuilt Package Hashes

```text
artifacts/binary/softpc32.exe  2ee957e426eb2fa88c39f39d02bc26559d8f89ab960f1a811b8610538a9121bd
artifacts/binary/softpc64.exe  a8465002ff5eaab472d28ca03c964adf87a7e5b1a3db404e028ad81ec11999e3
```
