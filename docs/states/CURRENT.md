# Project Status

## Current Work

M9 T99 S42 is verified, awaiting owner acceptance: original EGA mouse coordinates restore on all drawing outcomes.
S41 and Td S23 remain owner-accepted and closed.

## M9 T99 S42 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits S42 and authorizes audit, minimal repair, regression, publication, commit and push. |
| Objective | Restore both temporarily scaled mouse coordinates on every return from EGA_graphics_cursor_display. |
| Non-goals | No guest modification, driver activation, rendering rewrite, public-component change or unrelated cleanup. |
| Reference Baseline | 2c0b0646; pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [T99 retained scope](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | mouse_io.c, source-local regression, CMake and mirror evidence/manifest; no public ABI. |
| Applicable Rules | Execution, architecture, coding, documentation and source/research policy. |
| Verification | Actual-function before/after cases; actual supported compile/call graph; cached x86/x64 builds and full applicable runtime/native regression. |
| Expected Markers | One entry scaling decision, one paired restoration outside clipping; no new dispatch path. |
| Asset Needs | Refresh both EXEs; preserve all INI, guest media and snapshots. |
| Reporting Requirements | Distinguish compiled retained code from guest reachability, injected proof from real guest regression; report mirror diff and limits. |
| Stop Conditions | No external code import, original guest recompilation, Computer Use, new recovery path or hiding failures. |
| Exit Criteria | Reviewed, built, published, tested, committed and pushed; wait for owner acceptance. |
| Original Owner Request | 准入s42处理：审计并修复原版鼠标绘制中临时坐标未恢复的问题。 |
| Similar-Issue Sweep | Inspect jap_mouse, temporary coordinate assignments and paired restoration in mouse_io.c and adjacent cursor display/undisplay paths. |

See [S42 investigation and proof](../etc/evidence/softpc/m9-t99-s42-mouse-coordinate-restoration.md).

S42 used disposable build/t99-s42 with hidden jobs and a 15-minute no-progress
review budget; all jobs have finished and scratch is removed before delivery.
Both EXEs are published; x64/x86 each pass 197/197 cases, 394/394 total.
The actual-function negative fails before and passes after on both architectures.
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
