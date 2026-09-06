# M9 T40: XP-Obsolete Header Retirement

## Outcome

Closed.  Removed 33 unselected headers from the preserved MVDM mirror: the
30 remaining OpenNT-only MIPS/PPC headers absent from XP SP1, plus
`base/inc/get_env.h`, `base/inc/gfisflop.h`, and `host/inc/monsim32.h`.
The latter three respectively declared an already-retired implementation,
supported already-retired slave-PC GFI backends, and belonged only to an absent
Sim32 parent route.  No selected x86/AT/V7 source changed.

## Verification

- Exact source/CMake/script/test searches found no selected reference to any
  removed path; generated x64/x86 compiler dependencies did not name
  `monsim32.h`.
- Fresh GCC x64 build and full CTest: 23/23 passed, including package smoke.
- Fresh MSYS2 i686 GCC build and full CTest: 23/23 passed, including package
  smoke.
- `assets/binary/softpc.ini` SHA-256 remained
  `EC99916966C44CFE8F3D231AFE22B23E5C73F87E6090BF2E2828B277F0EA87CD`.
- Documentation governance and `git diff --check` passed.

## Scope Kept

This was a selected-source cleanup only.  It did not import XP code, adopt an
XP A/B hunk, alter the standalone host boundary, remove empty directories, or
change ROMs, guest media, or user configuration.
