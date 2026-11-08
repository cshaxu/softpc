# Project Status

## Current Work

T99 S39 is active: read-only necessity, special-case and reduction audit of
retained mirror differences against fixed XP SP1, excluding every branch
body already replaced by #error. S38 is owner-accepted
and closed. Production/test/artifact changes are not admitted by this audit.

## M9 T99 S39 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner closes S38 and requests investigation of all mirror differences for necessity, special cases and safe reduction. |
| Objective | Inventory retained mirror files; exclude already-#error branch bodies; audit remaining portability/semantic differences for necessity and safe reduction. |
| Non-goals | No re-audit of already-#error exclusions; no production rollback, public changes, guest/media/config edits, bulk activation, external import or claimed universal dynamic proof. |
| Reference Baseline | SoftPC 19a68782; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. No OpenNT source comparison. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | All retained src/app-softpc/softpc.new files; caller, build, provenance and existing test evidence read-only. Documentation and audit inventory only. |
| Applicable Rules | Execution, architecture, coding, documentation and source-research policy; preserve selected branches and accepted local semantics. |
| Verification | Fixed-revision per-file inventory, normalized diff accounting, category/caller review, existing evidence cross-check and documentation gates. No runtime rebuild without code changes. |
| Expected Markers | Every file has identity/disposition; special cases and reduction candidates have exact locations, reasons and required validation. |
| Asset Needs | No EXE or asset changes; preserve all user data. |
| Reporting Requirements | Separate excluded #error bodies from audited retained code; inventory is not semantic proof; report necessity, risks, cleanup candidates and estimated diff. |
| Stop Conditions | Unknown necessity is unresolved, not safe-to-delete; do not create compensating paths or optimize solely for diff counts. |
| Exit Criteria | Reproducible bounded audit, concrete findings/design, reviewed documentation committed and pushed; await owner decision on implementation. |
| Original Owner Request | 验收通过，收口S38。调研镜像diff的必要性、特判和缩减；追加：替换为#error的部分无需审计。 |
| Similar-Issue Sweep | Scan all changed mirror bodies for host/product/test branches, duplicate adaptation, redundant transforms and obsolete repair markers; account for retained and unresolved hits. |

S39 findings and the finite inventory are recorded in
[the retained-difference review](../etc/evidence/softpc/m9-t99-s39-mirror-necessity.md).
Three wrapper removals are proposed, not implemented; mixed-file hunk-level
minimality remains unproven. Task-owned scratch is retired after promotion.

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
