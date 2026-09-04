# M8 T19: Transitional-Layout Removal And Proof

## Status

Closed on 2026-09-04.

## Outcome

The last transitional source routes are gone. Every retained source now has
one durable owner below:

```text
src/{mvdm/softpc.new,overlay/mvdm/softpc.new,host,app}
```

The recovered machine and selected BIOS/VGA/CMOS ROMs remain in
`src/mvdm/softpc.new/`. Required CCPU, C-VID, CMOS, keyboard, illegal-opcode,
and reset portability adapters were moved with `git mv` into matching
`src/overlay/mvdm/softpc.new/` paths. `softpc-machine-overlay` only composes
those existing overlay sources.

Per the owner-approved cleanup, unselected NTVDM XMS and suballocation
remnants were removed. They had no CMake selection, runtime caller, or
replacement route. The regenerated direct-difference ledger preserves their
historical rows with `current_path` set to `-` and an explicit
`removed-unselected` disposition.

No controller, BIOS, ROM, BOP, CCPU, C-VID, renderer, timing, media, or guest
behavior changed. The source-boundary gate now rejects a revived `src/core` or
`src/vm`, while retaining the original-controller and frontend-boundary checks.

## Verification

- Fresh GCC MinGW x64 build: complete build and CTest, 20/20 passed.
- Fresh GCC MinGW x86 build: complete build and CTest, 20/20 passed.
- Both builds regenerated their executable in `artifacts/binary/` and passed
  `softpc-package-smoke`, which launches the no-argument executable against
  the adjacent INI and declared media roots.
- The source-boundary and documentation-governance gates passed.
- The final remap ledger has 98 historical direct/local rows; every retained
  row resolves to an extant owner and each deliberate removal is explicit.
