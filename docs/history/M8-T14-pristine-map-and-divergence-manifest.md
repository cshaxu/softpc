# M8 T14: Pristine-map And Divergence Manifest

## Status

Active. This record was created on owner admission; it will retain the task
brief, evidence, closure review, and any approved deferral.

## Original Request Ledger

| ID | Owner request | Planned result | Status |
| --- | --- | --- | --- |
| R1 | “准入队列的下一个任务” | Admit the next ordered candidate as M8 T14. | Active |
| R2 | M8 source-mirror proposal | Create a rerunnable pristine-map and divergence manifest. | Active |

## Task Brief

**Objective.** Compare the current recovered SoftPC source inventory with the
selected read-only OpenNT `mvdm/softpc.new` tree and publish an auditable,
rerunnable classification and disposition ledger.

**Scope.** The task reads `src/core/softpc/`, build-selected inputs and the
read-only comparison root
`O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new`. It inventories
source, headers, scripts, resources, generated inputs/outputs, and checked-in
binary/object inventory.

**Non-goals.** This task does not move or edit recovered source, introduce an
overlay, change CMake, change generated code, or alter VM behavior. It does
not import comparison material.

**Required classification.** Every scoped path is exactly one of: identical
original, original with direct diff, original omitted, local standalone,
generated input, or generated output. Every direct diff has a proposed
overlay, host/app, remove, or unresolved disposition.

**Verification.** Re-run the inventory; inspect all non-identical controller,
BIOS, CCPU, C-VID and `nt_*` rows; cross-check CMake-selected inputs; run the
documentation governance gate after publishing evidence.

**Stop conditions.** Stop for owner direction if the comparison baseline is
ambiguous, a finding requires a machine-semantic decision, or the task would
need to copy/import external source or modify a recovered file.

## Baseline

- `daa565d` is the standalone package and T13 test-boundary baseline.
- `7add9fe` is the public README identity baseline.
- `src/core/softpc/` is transitional recovered source; it is not yet the
  pristine mirror defined by the M8 design.
- The selected OpenNT tree is comparison material only and never a build,
  runtime, or acceptance dependency.

## Planned Evidence

The completed manifest and the manual-review record will be owner-indexed
under `docs/etc/evidence/` and linked here at closure. No audit results have
been claimed at admission.
