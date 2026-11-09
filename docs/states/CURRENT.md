# Project Status

## Current Work

M9 T99 S42 P3 is verified, awaiting owner acceptance: unused nop.c and vglob.c and their build wiring are removed.
S41 and Td S23 remain owner-accepted and closed.

## M9 T99 S42 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves P3 retirement of vglob.c and previously explicitly requests deletion of unused nop.c; build, publish, test, commit and push in current S42. |
| Objective | Remove unused nop.c and vglob.c with source-list wiring; retain the current generated C-VID path and decoder NOP semantics. |
| Non-goals | No guest modification, driver activation, rendering rewrite, public-component change or unrelated cleanup. |
| Reference Baseline | 421b2fcb (P2); pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [T99 retained scope](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | nop.c and vglob.c removal, c-files, CMake, audit inventory and mirror manifest/ledger; no public ABI. |
| Applicable Rules | Execution, architecture, coding, documentation and source/research policy. |
| Verification | Dual-width object reachability and final EXE symbols; retained CPU/video regression; cached x86/x64 full runtime/native tests and publication comparison. |
| Expected Markers | One generated C-VID accessor path; NOP handled directly by the existing CPU decoder. |
| Asset Needs | Refresh both EXEs; preserve all INI, guest media and snapshots. |
| Reporting Requirements | Report whole-file retirement separately from local patch counts, unchanged active path, actual diff and binary comparison. |
| Stop Conditions | No external code import, original guest recompilation, Computer Use, new recovery path or hiding failures. |
| Exit Criteria | Reviewed, built, published, tested, committed and pushed; wait for owner acceptance. |
| Original Owner Request | nop完全没被生产调用 当然也要删了啊？ / 既然如此 那就还是应该删除吧。 |
| Similar-Issue Sweep | Audit all 228 pre-removal mirror C files for unused whole objects and isolated reference groups; nop.c and vglob.c are the only remaining candidates. |

See [S42 investigation and proof](../etc/evidence/softpc/m9-t99-s42-mouse-coordinate-restoration.md).

S42 used disposable build/t99-s42 with hidden jobs and a 15-minute no-progress
review budget; P3 owned jobs are finished and scratch is removed before delivery.
P3 publishes both EXEs and passes 392/392: each architecture 188 background
and eight native tests. Production source/list net -491, test code unchanged.
Both executable code sections are identical to P2; only build time/PE metadata
changes. The 472-file mirror manifest passes. P2 archive compatibility remains.
No external repository is a build dependency.

## Current Technical Baseline

S41 delivery ecf263a8 completes the five admitted mirror cleanup/guard units:
production +16/-29, with XP mirror added+deleted diff reduced by 13 lines.
Snapshot command IDs and the existing device paths remain unchanged.
See [S41 closure](../history/M9-T99-S41-mirror-cleanup.md).

Td S23 delivery db76bb60 repairs the cached compiler timestamp, updates
execution rules and verifies embedded EXE literals. Both architectures pass
188 background plus eight native tests each: 392/392. Published EXEs carry
Nov 8 timestamps. See [timestamp closure](../history/M9-Td-S23-build-timestamp.md).
Only Emulator build configuration/verifier/manifest changed within the public
eight trees; no fresh NXVM alignment is claimed.

[TODO](TODO.md) has no deferred entries. The runtime-performance candidate
remains in [Queue](QUEUE.md). T99 still requires its overall completion audit
and owner-approved closure.

## Recent M9 Closures

- [S41 mirror cleanup](../history/M9-T99-S41-mirror-cleanup.md).
- [S40 retained-difference audit](../history/M9-T99-S40-retained-difference-audit.md).

- [S39 mirror wrapper cleanup](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

[Td S23 timestamp governance](../history/M9-Td-S23-build-timestamp.md) is closed.
Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
