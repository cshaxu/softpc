# Project Status

## Current Work

Active task: M9 T72 S6, delivered/reviewed Lib text-render simplification awaiting owner testing.
S1--S5 are closed. Owner split the quality-audit follow-up into S6 (Lib) and
S7 (Common), in that order. S7 implementation waits for S6 owner testing.
See [proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md) and
[audit](../etc/evidence/softpc/m9-t72-post-s5-quality-audit.md).
S6 delivery details: [evidence](../etc/evidence/softpc/m9-t72-s6-text-render-simplification.md).
P1 01a737b2 is pushed and actual-change reviewed; S6 remains open, S7 not started.

## M9 T72 S6 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: split into two S tasks, Lib first, Common second. |
| Objective | Remove the redundant full-surface clear before complete text rendering; prove output coverage. |
| Non-goals | No Common fixes, ABI changes, input/mouse changes, graphics policy, new cache or state. |
| Reference Baseline | c635988a, clean; S5 owner accepted. |
| Candidate Proposal | [T72 proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md), section thirteen. |
| Files And ABI Surface | lib/kvm-window/render.c, existing frame-damage test, two manifests; public ABI unchanged. |
| Applicable Rules | Execution, architecture, coding, documentation rules; current architecture/layout/UI; shared governance skills. |
| Verification | Poisoned surface full-pixel proof for small/max grids, heights 0..16, both banks, blank glyphs; x86/x64 Release builds and full background test presets, corpus/DAG/docs gates. |
| Expected Markers | No untouched sentinel pixels, no guard writes, invalid input leaves output unchanged; all background tests pass. |
| Asset Needs | Refresh only assets/binary EXEs; no INI/media changes; existing build trees, no raw traces. |
| Reporting Requirements | Before: production +0/-2, tests about +40--60; after: tracked C/H counts, test evidence, EXE links and actual-change review. |
| Stop Conditions | Incomplete pixel coverage, ABI/output change, or need for new state requires reassessment. |
| Exit Criteria | Verified delivery committed/pushed, clean workspace, actual-change review; then wait for owner test before closure/S7. |
| Original Owner Request | 分成两个s任务 第一个先优化lib 第二个处理common |
| Similar-Issue Sweep | Inspect explicit clears in both KVM leaves; remove only proven overwritten text clear; retain native surface initialization and input resets for distinct responsibilities. |

## Current Technical Baseline

- S6 production +0/-2 (net -2), test +46/-1 (net +45), no ABI/Common changes.
  Both Release builds pass. Background x86 105/105 (147.31s); x64 104/105
  (161.37s), documentation-only failure corrected and rerun 1/1. Runtime tests
  all pass; five desktop tests per width excluded. S6 remains open for owner test.
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
- TODO tracking is retired by owner decision, not proof of repair. S5 delivery,
  review and owner acceptance are complete; formal T72 closure is not requested.

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
