# Project Status

## Current Work

M9 T99 S42 P4 is verified, awaiting owner acceptance: retired DOS mouse snapshot payload removed without backward compatibility.
S41 and Td S23 remain owner-accepted and closed.

## M9 T99 S42 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves deleting all legacy snapshot compatibility residue, rebuilding, publishing, testing, committing and pushing within S42. |
| Objective | Remove the unused DOS mouse archive fields and mappings; advance the existing snapshot format discriminator and reject older files. |
| Non-goals | No guest modification, driver activation, rendering rewrite, public-component change or unrelated cleanup. |
| Reference Baseline | b12bc8be (P3); no new mirror changes. |
| Candidate Proposal | [T99 retained scope](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | compat/devices/archive.c and snapshot.h; machine/snapshot_image.c; checkpoint_smoke.c; task records. Snapshot SCP2 becomes SCP3; no public-component ABI change. |
| Applicable Rules | Execution, architecture, coding, documentation and source/research policy. |
| Verification | Old format rejected at header; current archive/image round trip and device restoration; cached x86/x64 full background and native tests. |
| Expected Markers | One current snapshot format, no legacy DOS mouse payload or compatibility reader. |
| Asset Needs | Refresh both EXEs; preserve all INI, guest media and snapshots. |
| Reporting Requirements | Report source/test diff, format break, full test evidence and published hashes. |
| Stop Conditions | No external code import, original guest recompilation, Computer Use, new recovery path or hiding failures. |
| Exit Criteria | Reviewed, built, published, tested, committed and pushed; wait for owner acceptance. |
| Original Owner Request | 不考虑兼容性，直接给我清理干净，然后重新编译发布测试提交推送。 |
| Similar-Issue Sweep | Scan src/test for all dos_mouse, DOS_MOUSE, legacy slot and version references; retain the actual InPort mouse state. |

See [S42 investigation and proof](../etc/evidence/softpc/m9-t99-s42-mouse-coordinate-restoration.md).

S42 P4 publishes both EXEs and passes 392/392: each architecture 188
background and eight native tests, no failures/skips. Production +4/-135
(net -131), tests +4/-14 (net -10). Snapshot format is now SCP3; earlier
SCP2 files are rejected at the header and must be recreated. Existing user
snapshot files remain untouched. The 472-file mirror and public eight trees
are unchanged. Build warnings are confined to existing mirror source.
P4 owned hidden jobs are finished; disposable build/t99-s42 is removed before
delivery, with only reusable Ninja/ccache roots retained.
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
