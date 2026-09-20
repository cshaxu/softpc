# Project Status

## Current Work

M9 T72 S2 is active: migrate neutral text attributes end to end.
Owner approved execution through T-level audit; formal T closure awaits review.

## M9 T72 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 批准执行到t收口标准 然后等我审核后再正式收口 |
| Objective | One migration of VM/status producers, shared text values, both leaves and native Console output; preserve display semantics. |
| Non-goals | No cell struct, second interface, new state/cache/thread, graphics/input/lifecycle/media/snapshot change; MVDM and Compat untouched. |
| Reference Baseline | e0ad4e8f; accepted T71 behavior. |
| Candidate Proposal | [Approved design](../proposals/m9-kvm-text-cell-glyph-refactor.md). |
| Files And ABI Surface | Ten planned production C/H paths and fixtures; source ABI migrated atomically. |
| Applicable Rules | Execution, Coding, Architecture, Documentation, UI and source layout; corresponding governance skills. |
| Verification | 512-case equivalence, neutral bank/boundary/resource cases, dual-width sizeof, manifest/DAG; both Release builds and serial background presets including Win3.1 roundtrips/snapshots. Desktop tests require reserved access. |
| Expected Markers | PC decoding only VM; native encoding only Broker; rejected publication does not mutate mailbox. |
| Asset Needs | Existing test disposable media; owned build/t72-proof logs/probes limited to 10 MiB and 5 minutes each, removed after recording. Never alter user INI/media. |
| Reporting Requirements | Actual additions/removals/net, layouts, finite proof and package hashes/links; P1 then coordinator P2 review and S3 audit. |
| Stop Conditions | Unexpected device/behavior change, broad architecture or unexplained significant growth. |
| Exit Criteria | Verified complete P1 push and actual-change review; proceed S3, not formal T closure. |
| Original Owner Request | 行 那就维持现在设计; 批准执行到t收口标准 然后等我审核后再正式收口 |
| Similar-Issue Sweep | Old fields/decoding, producers, comparisons, native output and shared/product fixtures. |

## Current Technical Baseline

- Code delivery f3ff1b11; review 449dad66; owner acceptance 5bfceb36.
  Both Release packages are unchanged by this governance closure.
- Final background suites: x64 105/105 (163.44s), x86 105/105 (145.85s),
  including headless Win3.1 PIF roundtrips, snapshots and restart.
  Five native desktop tests per width were not rerun in S10; no native Linux
  execution or whole-emulator correctness claim.
- VM owns character mapping. KVM Base transports opaque control FIFO and
  latest-wins complete frames; leaf presenters interpret their own frames.
  Window compares frames with its existing surface, not upstream dirty history.
- Renderer readiness, required fonts, terminal request completion, fixed-80 text
  layout and cursor normalization are recorded in the
  [T71 completion audit](../history/M9-T71-completion-audit.md).
- TODO tracking is retired by owner decision, not proof of repair.
  Neutral text attributes are under T72 design review, not implemented.

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
