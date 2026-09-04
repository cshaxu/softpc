# M8 T14: Pristine-map And Divergence Manifest

## Status

Closed on 2026-09-03.

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

The completed [path/hash manifest](../etc/evidence/m8-t14-pristine-map.tsv)
and [review summary](../etc/evidence/m8-t14-pristine-map.md) are indexed
evidence. The generator is `scripts/generate_pristine_map.ps1`.

## Closure Review

The manifest has 1,222 path rows and a fixed selected OpenNT revision. All
1,029 source/header rows have a classification. It identifies 60 direct
original differences: 39 proposed overlays, 17 proposed host moves, and four
unselected XMS/allocator removals. The remaining output rows explicitly record
identical/original-omitted/local/binary status; no differing path is called
pristine.

The direct-difference inventory was reviewed per file with `git diff --no-index
--numstat`, including BIOS, CCPU, C-VID, controller, and `nt_*` families. The
task made no machine, firmware, CMake, generated-source, or product behavior
change. T15 is the next queued task and owns source-mirror composition; T16
owns moving applied modifications out of the recovered tree.
