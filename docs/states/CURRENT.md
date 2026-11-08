# Project Status

## Current Work

T99 S39 implementation is verified: three local wrappers removed; both EXEs
published and all 390 tests pass. Awaiting post-push coordinator review and
owner-authorized closure. S38 remains closed.

## M9 T99 S39 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorizes the three proposed simplifications within S39, dual builds, publication, tests, commit/push and closure. S39 was still open, so no identifier is reused. |
| Objective | Remove three redundant local SOFTPC_STANDALONE wrappers, preserving their bodies and all original conditional structure. |
| Non-goals | No #error-body audit, broader rollback, shared API, device behavior, guest/media/config changes or claim that all mixed mirror hunks are minimal. |
| Reference Baseline | 4524002b; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89 for provenance only. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Mirror host/src/nt_cga.c, base/system/ica.c, base/keymouse/ppi.c; manifest/ledger/evidence and refreshed dual EXEs. No interface change. |
| Applicable Rules | Execution, architecture, coding, documentation and source-research policy. Mirror CRLF; other changed text LF. |
| Verification | Compare preprocessed content under actual x86/x64 compile commands; cached dual builds; all registered background and serial desktop/native-audio tests; manifest and documentation gates. |
| Expected Markers | Three wrapper pairs removed, bodies intact; production +0/-6; no second path or new state. |
| Asset Needs | Refresh softpc32.exe and softpc64.exe only; preserve user INI, media and snapshots. |
| Reporting Requirements | Actual production/test diff, compiler equivalence, artifact differences, full test counts, quality review and cleanup. |
| Stop Conditions | If any real build selects a different body, retain the wrapper and report rather than silently changing behavior. |
| Exit Criteria | Passing verification, published dual EXEs, pushed complete implementation P, coordinator actual-diff review, then owner-authorized closure P. |
| Original Owner Request | 重开刚才的S任务，把精简候选处理一下，老规矩编译测试发布提交推送收口本S。 |
| Similar-Issue Sweep | Recheck actual build/test consumers of the three wrappers; retain original NTVDM alternatives and other selections outside this admitted scope. |

The prior [review](../etc/evidence/softpc/m9-t99-s39-mirror-necessity.md) is a
frozen research result, not full hunk-level minimality proof. Scratch owner is
S39 at build/t99-s39; runners have finished and concise evidence is promoted
in [S39 delivery](../etc/evidence/softpc/m9-t99-s39-wrapper-cleanup.md).
Task scratch is retired before delivery; reusable caches remain.

## Current Technical Baseline

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

- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
