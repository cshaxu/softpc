# M8 T15: Pristine Mirror And Patch-composition Harness

## Status

Active. This record retains the task brief, evidence, closure review, and any
approved deferral.

## Original Request Ledger

| ID | Owner request | Planned result | Status |
| --- | --- | --- | --- |
| R1 | “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” | Execute the next admitted source-layout task in order. | Active |
| R2 | M8 source-layout proposal | Move the repository-owned recovered source to a reference-shaped path layout. | Active |

## Task Brief

T14 found the transitional recovered tree differs from selected OpenNT in 60
paths. T15 moves the existing repository-owned recovered files to the target
reference-shaped layout and routes CMake to that layout. It does not evacuate
direct source differences: their dispositions remain visible in the T14
ledger, and later tasks own any justified relocation.

The move preserves the existing repository provenance, relative paths, and
file names. `opennt-src-2` is read-only comparison material only: T15 must not
copy or import source, patches, build products, or resources from it. The
retained set is limited to C/H/ASM source, modern build configuration required
by this repository, and the selected BIOS/VGA/CMOS firmware ROMs. Standalone
files remain outside the mirror.

## Required Evidence

- repository path/provenance inventory proving every moved source pre-existed
  the move;
- retained-file-class audit proving no object/library/intermediate is kept;
- CMake selected-input proof that compiled machine code comes from the moved
  repository-owned tree;
- x64/x86 focused test evidence compared to T14 baseline.

## S1 Direction Correction

An uncommitted local experiment copied 1,007 files from the external reference
tree. Before any commit, the owner clarified that external copying is
prohibited. The experiment, its import scripts, and generated patches were
removed. The four firmware files already tracked at
`src/mvdm/softpc.new/roms/` were preserved. This task now moves only files
already owned by this repository.
