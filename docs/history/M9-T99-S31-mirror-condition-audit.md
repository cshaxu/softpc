# T99 S31 mirror condition audit

Owner admitted the next S after delivery of S31 commit dff86c90.

## M9 T99 S31 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner closes S30 and admits S31 for complete compiled-mirror conditional reconciliation against XP SP1. |
| Objective | Inventory compiled MVDM-host and non-MVDM mirror files, map each to XP, reconcile selected recursive conditional structures and excluded arms, and record a disposition per file. |
| Non-goals | Do not read OpenNT code, alter selected behavior, public components, user configuration, guest media or snapshots, or inspect inside fully excluded error arms. |
| Reference Baseline | SoftPC ee2250bb; XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89, read-only. |
| Candidate Proposal | [Public handoff simplification](../history/M9-T99-public-handoff-simplification-proposal.md). |
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

