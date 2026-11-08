# Project Status

## Current Work

M9 Td S23 is verified, awaiting owner acceptance: build-timestamp freshness repaired.
T99 S41 remains verified, awaiting owner acceptance; it is not closed.
Open task awaiting owner: T99.

## M9 Td S23 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Governance |
| Admission And Approval | Owner reports stale Nov 6 banner and explicitly orders timestamp cache exclusion, execution/gate repair and implementation. |
| Objective | Refresh the timestamp-bearing object on each product build, bypass its compiler cache, verify both published EXEs, retain other caches. |
| Non-goals | No guest, mirror, banner ABI, device behavior or user configuration change; do not close S41. |
| Reference Baseline | ecf263a8, T99 S41 delivery. |
| Candidate Proposal | [Build timestamp governance](../proposals/m9-build-timestamp-governance.md). |
| Files And ABI Surface | Emulator Product CMake/verifier/manifest, root publication/source gate, execution rules and evidence; no C API change. |
| Applicable Rules | Execution, architecture, coding and documentation; Td owns execution-rule update. |
| Verification | Repeated cached builds without source edits; fresh timestamp object and EXE content checks; stale-artifact rejection; dual full regression. |
| Expected Markers | Only composition.c forced/no launcher; normal source cache remains; EXEs contain fresh date/time. |
| Asset Needs | Refresh package EXEs only; preserve INI/media/snapshots. |
| Reporting Requirements | Root cause, build dependency and cache proof, actual timestamp strings, test results and public-tree changes. |
| Stop Conditions | No blanket cache clearing, touching tracked source, generated C/H or runtime date substitution. |
| Exit Criteria | Verified/published/committed/pushed; clean workspace; report owner acceptance status. |
| Original Owner Request | 带timestamp的这部分不可以缓存；去做！ |
| Similar-Issue Sweep | Search __DATE__/__TIME__ and build-time generators across production sources and build scripts. |

See [timestamp evidence](../etc/evidence/softpc/m9-td-s23-build-timestamp.md).

Td S23 used disposable build/td-s23 with hidden jobs and a 15-minute
no-progress review budget. All jobs completed; scratch is removed at delivery.
Both architectures pass 196/196 tests; both published EXEs contain Nov 8 timestamps.

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
