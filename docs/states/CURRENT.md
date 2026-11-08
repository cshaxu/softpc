# Project Status

## Current Work

T99 S40 is active: read-only audit of the remaining retained differences in
S39's 110 mixed files. Identical content, already-reviewed changes and already-
#error bodies are excluded. No production modification is admitted. The read-only report is ready for owner
review; three simplifications and two guard gaps affect four files. See
[the report](../etc/evidence/softpc/m9-t99-s40-retained-differences.md).

## M9 T99 S40 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner requests continued audit of the 110 mixed files for necessary differences and unified simplification. |
| Objective | Give each remaining retained change a necessity or concrete cleanup disposition tied to real consumers. |
| Non-goals | No repeat audit of identical/reviewed/#error content; no production edits, external import, guest/media/config modification or unsupported all-path runtime claim. |
| Reference Baseline | SoftPC f7537b1c; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89; S39 REVIEW rows define the 110-file universe. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Retained mirror differences in the 110 files, callers/build/tests read-only; documentation and evidence only. |
| Applicable Rules | Execution, architecture, coding, documentation and source-research policy. No OpenNT source comparison. |
| Verification | Mask excluded bodies, reuse exact prior proof, inspect remaining changes and consumers; documentation/whitespace gates. No rebuild for read-only work. |
| Expected Markers | Per-file/per-change disposition with evidence; necessary versus simplifiable versus unproven clearly separated. |
| Asset Needs | None; preserve published EXEs and all user data. |
| Reporting Requirements | Bounded coverage, skipped prior proof, concrete findings, unified minimal proposals and estimates; do not count inventory as semantic proof. |
| Stop Conditions | Unknown necessity is not permission to delete. Do not trade fewer mirror lines for duplicated logic elsewhere. |
| Exit Criteria | Audited remaining scope with traceable dispositions, reviewed report committed/pushed; await implementation decision. |
| Original Owner Request | 继续审计剩下110个混合差异文件；完全相同、已经审计过和#error替代部分不用审计。 |
| Similar-Issue Sweep | Compare new candidates across related headers, callbacks and device snapshot paths; report all remaining hits without broad code changes. |

S40 owns disposable build/t99-s40 for read-only extraction and comparison;
15-minute no-progress review budget, hidden long runners, concise evidence
promotion and cleanup before delivery. The 110-file list is frozen; each row
records reused proof or remaining-change disposition, not a new whole-file
review when only part is unreviewed.

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
