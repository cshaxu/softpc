# Project Status

## Current Work

M9 T72 S3 is active: audit the completed neutral text migration.
Owner approved execution through T-level audit; formal T closure awaits review.

## M9 T72 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 批准执行到t收口标准 然后等我审核后再正式收口 |
| Objective | Map the owner's whole T request and every S to actual delivery evidence; prepare for owner review without closing T72. |
| Non-goals | No cell struct, second interface, new state/cache/thread, graphics/input/lifecycle/media/snapshot change; MVDM and Compat untouched. |
| Reference Baseline | Implementation 12bf7c96; original task baseline e0ad4e8f. |
| Candidate Proposal | [Approved design](../proposals/m9-kvm-text-cell-glyph-refactor.md). |
| Files And ABI Surface | Documentation audit only; inspect ten changed production C/H paths, tests and unchanged boundaries. No new ABI. |
| Applicable Rules | Execution, Coding, Architecture, Documentation, UI and source layout; corresponding governance skills. |
| Verification | Recheck actual commit, evidence ledger, hashes, sizeof/cost accounting, manifests/DAG, documentation governance and clean synchronized Git. S2 full dual-width proof remains the runtime baseline. |
| Expected Markers | PC decoding only VM; native encoding only Broker; rejected publication does not mutate mailbox. |
| Asset Needs | None; do not launch native UI or alter user configuration/media. |
| Reporting Requirements | Expected production/test +0/-0; confirm actual counts, finite T ledger and pending owner acceptance. P1 push then actual-change P2 review. |
| Stop Conditions | Unexpected device/behavior change, broad architecture or unexplained significant growth. |
| Exit Criteria | Complete audit committed/pushed and reviewed; clean worktree; wait for owner, do not formally close T72. |
| Original Owner Request | 行 那就维持现在设计; 批准执行到t收口标准 然后等我审核后再正式收口 |
| Similar-Issue Sweep | Old fields/decoding, producers, comparisons, native output and shared/product fixtures. |

## Current Technical Baseline

- Code delivery 12bf7c96; S2 actual-change review recorded in
  [S2 history](../history/M9-T72-S2-neutral-text-migration.md).
- Final background suites: x64 105/105 (164.85s), x86 105/105 (147.06s),
  including headless Win3.1 PIF roundtrips, snapshots and restart.
  Five native desktop tests per width were not rerun in S2; no native Linux
  execution or whole-emulator correctness claim.
- VM owns character mapping. KVM Base transports opaque control FIFO and
  latest-wins complete frames; leaf presenters interpret their own frames.
  Window compares frames with its existing surface, not upstream dirty history.
- Renderer readiness, required fonts, terminal request completion, fixed-80 text
  layout and cursor normalization are recorded in the
  [T71 completion audit](../history/M9-T71-completion-audit.md).
- T72 uses neutral parallel text arrays, with device decoding only in VM and
  native Console colour encoding only in Broker. No frame transport change.
- TODO tracking is retired by owner decision, not proof of repair. Owner visual
  acceptance and formal T72 closure remain pending.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner approved. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner accepted. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; narrowed after audit. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit and narrowed S2 accepted. | [Audit](../history/M9-T64-completion-audit.md) |

## Recent Governance

- **M9 Td S18:** T71 closure audit, TODO retirement and new floppy-identification
  candidate second in queue; documentation only. P1 957c809d pushed;
  actual-change coordinator review and documentation checks passed; Td closed.
  [Record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
- **M9 Td S17:** Earlier text/frame proposal subsequently admitted as T71;
  [retained proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
