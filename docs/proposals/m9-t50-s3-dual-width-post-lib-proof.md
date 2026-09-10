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
