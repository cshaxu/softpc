# Project Status

## Current Work

M9 T99 S42 P2 is verified, awaiting owner acceptance: unused legacy DOS mouse driver and runtime wiring removed.
S41 and Td S23 remain owner-accepted and closed.

## M9 T99 S42 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorizes P2 whole-file retirement in current S42, including dependent wiring, builds, tests, publication, commit and push. |
| Objective | Remove mouse_io.c and its dedicated instance/test wiring; preserve snapshot wire layout and reject installed legacy-driver archives before device mutation. |
| Non-goals | No guest modification, driver activation, rendering rewrite, public-component change or unrelated cleanup. |
| Reference Baseline | 42533c7d (P1); pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [T99 retained scope](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | mouse_io.c removal, dedicated Compat instance removal, archive compatibility, checkpoint tests, CMake and mirror manifest/ledger; no public ABI. |
| Applicable Rules | Execution, architecture, coding, documentation and source/research policy. |
| Verification | Actual compile/call graph and removed EXE symbols; archive rejection/roundtrip and InPort regression; cached x86/x64 full runtime/native tests. |
| Expected Markers | One retained InPort hardware path; no old driver runtime or replacement stubs. |
| Asset Needs | Refresh both EXEs; preserve all INI, guest media and snapshots. |
| Reporting Requirements | Report whole-file subset retirement separately from local patch counts, archive compatibility, actual diff and regression limits. |
| Stop Conditions | No external code import, original guest recompilation, Computer Use, new recovery path or hiding failures. |
| Exit Criteria | Reviewed, built, published, tested, committed and pushed; wait for owner acceptance. |
| Original Owner Request | 太好了 准入s42 整个删除本文件及其接线。 P1 remains historical; P2 supersedes the repaired unused file. |
| Similar-Issue Sweep | Scan all old driver/instance/snapshot references and build/audit inventories; preserve shared headers and the real InPort owner. |

See [S42 investigation and proof](../etc/evidence/softpc/m9-t99-s42-mouse-coordinate-restoration.md).

S42 used disposable build/t99-s42 with hidden jobs and a 15-minute no-progress
review budget; all owned jobs are finished and scratch is removed before P2 delivery.
P2 publishes both EXEs and passes 392/392: each architecture 188 background
and eight native tests. Legacy symbols are absent; InPort remains. Production
net -6587, test net -101. Normal inactive legacy wire layout is unchanged;
installed legacy-driver archives return UNSUPPORTED before device restore.
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
