# M9 T50 S3 — Post-Library Dual-Width Proof

## Objective

Correct T50's evidence gap: after the shared-library MSVC atomics portability
change, rebuild and test the actual x64 and x86 SoftPC package paths from the
current source.

## Scope

- Configure, build, package, and run the repository CTest suite for both
  checked-in release presets.
- Verify the resulting PE width and fixed package names.
- Refresh only agent-owned `assets/binary/softpc32.exe` and `softpc64.exe` if
  the admitted package targets do so; preserve `softpc.ini` and media.

## Non-goals

- No source, UX, MVDM, library API, or CI change.
- No manual guest acceptance claim: this is build/regression evidence only.

## Exit criteria

- Fresh x64 and x86 configure/build/package/CTest evidence is recorded from
  current HEAD.
- Both package executables have the expected PE architecture, user
  configuration is untouched, and the corrective S is pushed and reviewed.

## Completion evidence

- `mingw-gcc-x64-release` and `mingw-gcc-x86-release` each freshly configured
  with their declared compiler width, then rebuilt the `softpcvm` package
  target from the post-S2 source tree.
- Each resulting full CTest run recorded 36 test records and 36 `Test Passed`
  records, including the package smoke.
- The refreshed fixed package files were verified as PE `0x014C`
  (`softpc32.exe`) and `0x8664` (`softpc64.exe`). Their SHA-256 values are,
  respectively, `E50A4EBFE97957F2D94D44478D52B8C4949A462F19015E786B28C6BAB72A2E0E`
  and `083313AD43E109FEA3BB3496735FA740E2F04BF94232AE954EC5B095CB7CE950`.
- `assets/binary/softpc.ini` and reusable media had no diff. The build emitted
  existing preserved-source warnings only; this S changed no source and makes
  no manual guest-behavior claim.
- Similar-issue sweep: the two checked-in release presets are the only package
  targets; both declared widths, fixed output names, build paths, and full
  test paths were exercised.
