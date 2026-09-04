# M8 T18: Application-Shell Migration

## Status

Closed on 2026-09-04.

## Outcome

The repository-owned application shell moved by `git mv` from `src/vm/` to
the approved layout:

```text
src/app/
  main.c
  softpc_firmware.rc
  runtime/
  frontends/{console,win32}/
```

Only private include paths, CMake source routing, the T16 current-path ledger,
and the source-boundary test changed with the move. The resource file remains
at `src/app/softpc_firmware.rc` and still embeds the fixed ROMs from the
recovered `src/mvdm/softpc.new/roms/` tree. No CCPU, controller, BIOS, ROM,
VGA, BOP, renderer, or host-device behavior changed.

The runtime remains the sole app component that holds a `softpc_machine *` and
executes machine callbacks. Console and Win32 frontends continue to submit
queued input and consume copied runtime frames; the source-boundary test
rejects direct frontend machine access and a revived `src/vm/` aggregate.

## Verification

- GCC MinGW x64: full CTest, 20/20 passed.
- GCC MinGW x86: full CTest, 20/20 passed after a transient host-window
  capture assertion passed on its isolated rerun.
- Both package smoke tests started their respective EXE from
  `artifacts/binary/` without command-line arguments, validated the adjacent
  `softpc.ini`, and verified both configured media paths under
  `artifacts/media/`.
- The documentation-governance and direct-difference remap checks passed; all
  ten former `src/vm/*` rows now resolve to their `src/app/*` paths.
