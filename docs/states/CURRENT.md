# Project Status

## Current Work

T99 S38 is implemented and verified, awaiting owner acceptance. Five GDP
subfield aliases and the Unchained DWORD +2 address are repaired; production
+8/-1, test +63/-0. Final x86/x64 builds each pass 187 background and eight
native cases (390 total), and both EXEs are published. Bulk-fill activation
or removal is not part of this delivery. See
[S38 evidence](../etc/evidence/softpc/m9-t99-s38-gdp-subfields.md).

## M9 T99 S38 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves the five GDP aliases and Unchained DWORD fix; asks whether the bulk-fill entry should be used. |
| Objective | Restore one native GDP field owner for subfield accesses; use consecutive guest WORD addresses for Unchained DWORD; explain safe bulk-fill admission. |
| Non-goals | No bulk-path activation/deletion, public-component edits, guest/media/configuration changes, second device path or broad generated-code rewrite. |
| Reference Baseline | SoftPC 0d0ef251; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | compat/cvidc/gdp_state.c, mirror base/cvidc/sevid000.c, existing App chain2_rule_smoke fixture and mirror ledger. No public ABI change. |
| Applicable Rules | Execution, architecture, coding, documentation and source-research policy; owner approves original semantic correction. |
| Verification | Real rule before/after evidence, nonuniform alias values, mask-controlled reads, all four read planes, x86/x64 cached builds and complete background/native regression, published EXEs. |
| Expected Markers | Five aliases share corresponding low bytes/WORDs with the original native fields; Unchained returns consecutive bytes; existing paths and ownership remain. |
| Asset Needs | Refresh both EXEs; preserve INI, snapshots and guest media. |
| Reporting Requirements | Precise bulk-fill contract distinction, code/test counts, alias sweep, mirror diff, failures/reruns and actual-diff quality review. |
| Stop Conditions | Do not bypass SAS translation/type boundaries or infer a measured bulk performance gain; no production fallback/second path. |
| Exit Criteria | Approved repairs and tests, dual build/publish/full qualification, actual-diff review, commit/push, scratch cleanup and owner acceptance. |
| Original Owner Request | 批准 GDP 五个入口统一映射和 Unchained DWORD 修复；批量入口是否应该在合适的时候调用，而不是因为未调用就删掉？ |
| Similar-Issue Sweep | Enumerate nonaligned VGA GDP rule offsets and their field owners; distinguish original DWORD address/pair composition; inspect actual SAS fill and C-VID glue consumers. |

Executor scratch build/t99-s38 is retired after concise evidence promotion;
reusable Ninja/ccache trees remain. T99 and S38 await owner acceptance.

## Current Technical Baseline

S37 deliveries 7a77c818 and fd87c1bd are owner-accepted. BIOS permissions,
slave EOI, virtual CRTC text height and full-width ROM fallback reads are
repaired in the existing paths. PIC and keyboard retain their tested behavior.
Both EXEs are published; x86/x64 each pass 187 background and eight native
cases (390 total), including recorded connected-session native reruns.
See [S37 evidence](../etc/evidence/softpc/m9-t99-s37-original-device-audit.md).

Public eight trees are unchanged by S36/S37; S34's imported baseline remains
intact. This is not a fresh comparison with NXVM's current worktree. S35 was
withdrawn without a code delivery. The bulk-fill finding remains deferred in [TODO](TODO.md); S38 owns the
other two admitted C-VID repairs. The runtime-performance
candidate remains in [Queue](QUEUE.md). T99 still needs its overall completion
audit and owner-approved closure.

## Recent M9 Closures

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
