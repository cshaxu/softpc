# Project Status

## Current Work

Active: T72 S5, aggregate KVM text fields into one cell array.
Implementation, dual-width background verification and actual-change review
complete. P1 da26717b pushed; awaiting owner testing before closure.
T72 S1--S4 are closed. T72 remains open; no next queued candidate admitted.

Owner confirmed desktop and RDP tests passed and approved S4 closure.
See [S4 closure and T72 readiness](../history/M9-T72-S4-native-mouse-acceptance.md),
[original readiness audit](../history/M9-T72-S3-completion-readiness-audit.md) and
[T72 proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md).

## M9 T72 S5 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: "我希望是代码更干净，更易懂。看起来我们应该合并。准入一个新的S任务做这个。" |
| Objective | Replace four KVM text arrays with one array of four-byte cells; keep glyph/bank/foreground/background meaning and all product behavior. |
| Non-goals | No logical Unicode Console ABI change, new layer/cache/thread, renderer policy, mouse/input, MVDM/Compat, INI/media or snapshot change. |
| Reference Baseline | e118f058; clean at admission. |
| Candidate Proposal | [T72 proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md), S5 addition supersedes the earlier layout choice only. |
| Files And ABI Surface | Six production paths: kvm-base/frame_interface.h, kvm-window/render.c, kvm-console/console.c, common/machine/machine.c, common/ui/ui.c, vm/driver.c; nine current test consumers. KVM source ABI changes atomically, without old aliases. |
| Applicable Rules | Execution, architecture, coding and documentation governance; current architecture/source layout/UI. |
| Verification | Cell sizeof/offset assertions, existing 512-case output matrix, each-field change detection, hidden-tail/bounds/copy/publication tests, dual Release strict Lib builds and both full background suites, manifests/DAG/docs. |
| Expected Markers | cells[index] owns all four bytes; one comparison, no residual KVM parallel arrays; identical frame sizes and outputs; no unchecked adapter. |
| Asset Needs | Existing package EXEs only; preserve INI/media. Background presets exclude desktop automation; no diagnostic capture needed. |
| Reporting Requirements | Estimate six production C/H +45--70/-40--70, near-zero net; nine test C +80--130/-60--100. Report actual additions/deletions/net separately from docs/manifests/binaries. |
| Stop Conditions | No padding-dependent comparison, capacity/validation/behavior change, new interface bridge or unrelated cleanup. |
| Exit Criteria | Complete atomic migration and finite ledger proof, x86/x64 packages, commit/push and actual-change review; await owner testing before S/T closure. |
| Original Owner Request | "将文本帧平行数组改为单一单元格数组"; prioritize clean and understandable code. |
| Similar-Issue Sweep | All KVM field consumers including test text scanning, bulk initialization, comparison and copy; logical Console Unicode arrays intentionally retained at their independent boundary. |

## Current Technical Baseline

- S5 production +31/-33 (net -2); tests +94/-57 (net +37). Both Release builds
  pass; background x64 105/105 (159.37s), x86 105/105 (146.32s). Frame sizes
  unchanged; five desktop tests per width excluded. See
  [S5 evidence](../etc/evidence/softpc/m9-t72-s5-text-cells.md).
- S4 repair replaces pointer recentering with native motion; public input ABI
  unchanged. Source +148/-44 (net +104), test +142/-34 (net +108). Package hashes
  and exact verification results are in the
  [S4 evidence](../etc/evidence/softpc/m9-t72-s4-native-mouse-motion.md).
  Delivery aba5022c / review 9f65aa67; owner desktop/RDP acceptance now passed.
- Code delivery 12bf7c96; S2 actual-change review recorded in
  [S2 history](../history/M9-T72-S2-neutral-text-migration.md).
- S2 background suites: x64 105/105 (164.85s), x86 105/105 (147.06s),
  including headless Win3.1 PIF roundtrips, snapshots and restart.
  Five native desktop tests per width were not rerun in S2; no native Linux
  execution or whole-emulator correctness claim.
- Latest S4 background: x86 105/105 (150.75s); x64 104/105 (169.13s),
  then corrected documentation gate 1/1. All runtime cases passed; five desktop
  automation tests per width excluded. Owner manual desktop/RDP passed afterward.
- VM owns character mapping. KVM Base transports opaque control FIFO and
  latest-wins complete frames; leaf presenters interpret their own frames.
  Window compares frames with its existing surface, not upstream dirty history.
- Renderer readiness, required fonts, terminal request completion, fixed-80 text
  layout and cursor normalization are recorded in the
  [T71 completion audit](../history/M9-T71-completion-audit.md).
- S5 aggregates the four neutral KVM fields into four-byte cells. Device decoding
  stays in VM, native Console colour encoding in Broker. No frame transport change.
- TODO tracking is retired by owner decision, not proof of repair. S5 delivery
  and owner acceptance now precede formal T72 closure.

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
