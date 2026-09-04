# M8 T15: Pristine Mirror And Patch-composition Harness

## Status

Active. This record retains the task brief, evidence, closure review, and any
approved deferral.

## Original Request Ledger

| ID | Owner request | Planned result | Status |
| --- | --- | --- | --- |
| R1 | “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” | Execute the next admitted source-layout task in order. | Active |
| R2 | M8 source-mirror proposal | Create a pristine source mirror and patch-composition harness. | Active |

## Task Brief

T14 found the transitional recovered tree differs from selected OpenNT in 60
paths. T15 establishes the target mirror/overlay layout and makes CMake build
only composed copies in ignored `build/`. It does not evacuate direct source
differences: T16 owns that transformation after composition is proven.

The mirror preserves source provenance and relative paths. Each selected patch
is visible under `src/overlay/mvdm/softpc.new/` with an explicit manifest
record. Standalone files remain outside the mirror. The original external tree
is an admitted import/comparison input for establishing this source mirror;
the resulting mirror is repository-owned and the external tree is never a
runtime dependency.

## Required Evidence

- source mirror hash inventory against the selected T14 revision;
- patch manifest with original path, patch path, reason, owner, defines, and
  x86/x64 evidence fields;
- failure proof for a stale/unregistered patch;
- CMake selected-input proof that compiled original code is composed under
  `build/` rather than read directly from the mirror;
- x64/x86 focused test evidence compared to T14 baseline.
