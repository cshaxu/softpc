# M8 T16: Original-diff Evacuation

## Status

Active. This record retains the task brief, evidence, closure review, and any
approved deferral.

## Original Request Ledger

| ID | Owner request | Planned result | Status |
| --- | --- | --- | --- |
| R1 | “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” | Execute the next ordered source-layout task. | Active |
| R2 | M8 source-layout proposal | Make direct standalone adaptation visible and correctly owned. | Active |

## Task Brief

T15 completed the repository-owned path migration. T16 now remaps the T14
direct-difference ledger to those paths and removes or relocates only
standalone adaptation that can be proven not to be original machine logic.
The external OpenNT tree is read-only comparison material: no source, patch,
or generated content may be copied from it. This task does not decompose the
standalone host or app; T17 and T18 own those moves.

## Required Evidence

- a current-path disposition for every T14 direct-difference row;
- evidence for every removal or relocation, including why original behavior is
  preserved;
- CMake selected-input proof and x64/x86 focused regression evidence;
- a similar-issue sweep across original headers, generated inputs, CCPU,
  C-VID, BIOS, controllers, and original `nt_*` renderers.
