# Project Status

## Current Work

The owner closes [T99 S30](../history/M9-T99-S30-mirror-structure.md) and
admits S31. T99 remains open. S31 independently inventories compiled mirror
sources and recursively verifies their conditional structures against the fixed
XP SP1 original; no OpenNT source is to be consulted.
The [per-file ledger](../etc/evidence/softpc/m9-t99-s31-file-ledger.tsv)
freezes 470 files found in both cached x86/x64 Ninja dependency databases;
all 475 retained XP source hashes match the pinned manifest. The 245 byte-equal
files are identified. Independent full-file scanning found 47 non-identical
pairs with no conditional, error or line directives in either file. Another 178
files have completed review, including 79 with documented local exceptions
and six corrected files. All 470 files have individual dispositions; the six
modified mirror files form the final partial batch; both architectures built
successfully and their loadable PE sections match the preceding EXEs. S30
cross-indexing and signature scans were treated only as leads. The inventory
includes 36 host and 434 non-host paths; the
build dependency evidence does not prove hypothetical configurations.
The owner corrected the execution order: do not run another batch build merely
for audit records with no changed mirror code. Review, correct and register
files first; deliver each ten changed code files as directed. T99 remains open.

## M9 T99 S31 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner closes S30 and admits S31 for complete compiled-mirror conditional reconciliation against XP SP1. |
| Objective | Inventory compiled MVDM-host and non-MVDM mirror files, map each to XP, reconcile selected recursive conditional structures and excluded arms, and record a disposition per file. |
| Non-goals | Do not read OpenNT code, alter selected behavior, public components, user configuration, guest media or snapshots, or inspect inside fully excluded error arms. |
| Reference Baseline | SoftPC ee2250bb; XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89, read-only. |
| Candidate Proposal | [Public handoff simplification](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Compiled mirror source and headers, per-file evidence and necessary provenance; preserve selected ABI and approved fixes. |
| Applicable Rules | Execution, documentation, architecture, coding, and source-research policy. |
| Verification | Compare recursively against XP and real x86/x64 compilation contexts; every ten changed source files build both architectures and compare executable runtime content. If changed, run full tests before publication; if not, skip tests as owner directed. Review, publish, commit and push each batch. |
| Expected Markers | Excluded arms retain their own original conditional frame and exactly one error, with required line controls; selected arms retain matching original nested structures or individually justified exceptions. |
| Asset Needs | Publish both EXEs after each build; preserve INI, guest media and snapshots. |
| Reporting Requirements | After each push report structurally matching files, changed-and-resolved files and total; separately report justified exceptions and unresolved work, plus actual changes and verification. |
| Stop Conditions | Do not guess unobserved configuration, discard a required local fix, or treat a heuristic match as proof; preserve unrelated changes. |
| Exit Criteria | Every compiled mirror file mapped and individually disposed, required corrections and exceptions reviewed and delivered, final dual build and applicable tests completed. |
| Original Owner Request | 对照 XP SP1 逐文件递归恢复条件结构；已排除分支以 error 和 line 处理，每十个修改文件集中构建、发布、提交推送并汇报进度。 |
| Similar-Issue Sweep | Enumerate compiled x86/x64 mirror inputs, selected and excluded arms at every depth, missing XP frames and necessary local differences. |

Disposable work belongs to build/t99-s31, with bounded owned background jobs;
retire it at each delivery when no longer needed.

## Current Technical Baseline

S30 final delivery and binary hashes are recorded in its
[final review](../etc/evidence/softpc/m9-t99-s30-final-review.md). The last
full suite passed at P43; later builds had unchanged loadable PE sections and
were not retested per owner direction. The prior intermittent compact-console
initialization failure is not claimed resolved.

## Recent M9 Closures

S30 is owner-closed; its retained evidence and archived packet are in the
[closure](../history/M9-T99-S30-mirror-structure.md).

| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Build caches and disposable work follow the execution rules.
