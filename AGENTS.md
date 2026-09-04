# Agent Instructions

Before changing this repository, read `docs/README.md` and follow its Task
Reading Set, including every authority named by the active packet.

## Authority

- The authorities selected by that reading set govern the work. This file is
  only an execution pointer, not a second design, rule, or status source.
- `src/core/softpc/` is the preserved SoftPC source baseline. Do not add a
  standalone-only behavior branch there without an admitted port-ABI task.
- `O:\repos.external\opennt-src-2` and `O:\repos.hobby\nxvm` are read-only
  comparison material, never runtime, build, or acceptance dependencies.

## Execution

- `docs/states/CURRENT.md` is the sole active packet and technical baseline.
- `docs/states/QUEUE.md` contains ordered, unnumbered candidates. `T` is the
  globally increasing implementation/build version and is allocated only on
  owner admission; `Td` never consumes a T number.
- Preserve user configuration in `build/output/softpc.ini`, guest media, and
  unrelated worktree changes.
