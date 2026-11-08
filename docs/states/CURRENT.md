# Project Status

## Current Work

T99 S41 is verified and awaiting owner acceptance: S40 cleanup and existing
snapshot guard repairs. Cached dual builds publish both EXEs; 392/392 tests pass.
Mirror diff decreases by 13 lines. See [S41 evidence](../etc/evidence/softpc/m9-t99-s41-mirror-cleanup.md).
S40 read-only delivery 76e1d839 is accepted for implementation in this new S.

## M9 T99 S41 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves a new S to implement the S40 candidates, measured by mirror diff reduction, not source net lines. |
| Objective | One HDD command-ID table; remove three Mixer declarations; simplify next assignment and repair queue/keyboard capture guards. |
| Non-goals | No #error body review, public-component changes, new runtime path, guest/media/config change or snapshot ID renumbering. |
| Reference Baseline | SoftPC 76e1d839; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | fdisk.c, nt_sbmxr.c, quick_ev.c, keyba.c; owner tests, root test registration and mirror evidence. No public ABI change. |
| Applicable Rules | Execution, architecture, coding, documentation and source-research policy; single-agent executor/coordinator review. |
| Verification | Focused x86/x64 snapshot mapping and failure tests; cached dual builds, publication, all background and serial native tests; manifest and documentation gates. |
| Expected Markers | Five findings disposed; XP additions/deletions before/after separately counted; original IDs and normal behavior preserved. |
| Asset Needs | Existing package only; refresh two EXEs, preserve INI/media/snapshots. |
| Reporting Requirements | Mirror diff reduction primary; source/test change counts separately; actual failure proofs and limits. |
| Stop Conditions | Do not introduce a production test API, generic registry or compensating device implementation. |
| Exit Criteria | Build/publish/test/review/commit/push complete and clean workspace; await owner acceptance. |
| Original Owner Request | 批准，请你准入新的S任务予以清理。 |
| Similar-Issue Sweep | Snapshot callback maps, duplicate Mixer declarations, hash/list NULL guards and anomalous capture/restore conditions. |

S41 used disposable build/t99-s41 with hidden background jobs and a 15-minute
no-progress review budget. Jobs finished; scratch is removed before delivery.

## Current Technical Baseline

S39 delivery 26e659c8 removes six redundant local wrapper directives; original
bodies and behavior remain. Both published EXEs and all 390 tests are verified.
See [S39 evidence](../etc/evidence/softpc/m9-t99-s39-wrapper-cleanup.md).

S37 deliveries 7a77c818 and fd87c1bd are owner-accepted. BIOS permissions,
slave EOI, virtual CRTC text height and full-width ROM fallback reads are
repaired in the existing paths. PIC and keyboard retain their tested behavior.
Both EXEs are published; x86/x64 each pass 187 background and eight native
cases (390 total), including recorded connected-session native reruns.
See [S37 evidence](../etc/evidence/softpc/m9-t99-s37-original-device-audit.md).

Public eight trees are unchanged by S36/S37; S34's imported baseline remains
intact. This is not a fresh comparison with NXVM's current worktree. S35 was
withdrawn without a code delivery. S38 is owner-accepted and repairs all three admitted C-VID findings;
[TODO](TODO.md) has no remaining deferred entries. The runtime-performance
candidate remains in [Queue](QUEUE.md). T99 still needs its overall completion
audit and owner-approved closure.

## Recent M9 Closures

- [S39 mirror wrapper cleanup](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
