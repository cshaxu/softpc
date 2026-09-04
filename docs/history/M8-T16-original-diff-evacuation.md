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

## S1 Current-path Remap

`scripts/generate_t16_direct_diff_remap.ps1` reads only the committed T14 TSV
and verifies that all 98 direct-original-diff/local-standalone rows have a
current repository path after T15. The generated remap reports zero missing
paths: 56 rows are direct original differences and 38 are local standalone
files. Its owner labels are intentionally preliminary; they preserve the T14
disposition while T16 audits actual implementation responsibility.

## S2 First Reference-restoration Batch

Three direct differences were solely formatting or include-search adaptation
and are now absent from the recovered source tree:

- `base/bios/equipmnt.c`: removed an added blank line;
- `base/video/v7_ports.c`: restored the original comment indentation;
- `base/support/time_day.c`: restored `#include "error.h"`.

For `time_day.c`, CMake now supplies `base/inc` through a source-local
`-iquote` option, so the original header spelling resolves to the same base
error declaration without relying on host include-order accident. Read-only
reference comparison reports zero differences for all three paths. GCC x64
and GCC x86 both rebuilt; each full CTest suite passed 20/20.

## S2 CCPU Bridge Evacuation

`softpc_ccpu_facade.c` was a local standalone bridge incorrectly placed under
the recovered `base/ccpu386` tree. Its CCPU bootstrap, real-mode address, and
descriptor-translation entry points are now unchanged at
`src/core/softpc-port-abi/ccpu/softpc_ccpu_facade.c`. The `softpc-ccpu-core`
object target still compiles it with the CCPU/C-VID executor because it owns
those symbols, but the source boundary and CMake paths now make the standalone
compatibility ownership explicit. GCC x64/x86 full CTest both passed 20/20.
