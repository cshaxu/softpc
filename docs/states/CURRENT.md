# Project Status

## Current Work

Active task: M9 T72 S8, delivered and reviewed; awaiting owner test.
T72 remains open; S1--S7 are closed. Owner explicitly admitted S8 after snapshot push.
See [proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md) and
[audit](../etc/evidence/softpc/m9-t72-post-s5-quality-audit.md).
S6 delivery details: [evidence](../etc/evidence/softpc/m9-t72-s6-text-render-simplification.md).
P1 01a737b2 / review 2cf87250 accepted; [S6 closure](../history/M9-T72-S6-text-render-simplification.md).
S7 details: [unified damage evidence](../etc/evidence/softpc/m9-t72-s7-window-pixel-damage.md).
P2 c813f698 / review 6fa729a1 accepted; [S7 closure](../history/M9-T72-S7-window-pixel-damage.md).
Owner-approved snapshot delivery c0355b2f pushed; clean baseline verified.

## M9 T72 S8 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 确认推送。完成后准入下一个S8。 Snapshot pushed before admission. |
| Objective | Resolve Common audit A/B/C/D/F without new state machines, queues or Lib changes. |
| Non-goals | No Lib, VM, Compat, MVDM, frame schema, debug command semantics, INI or media changes. |
| Reference Baseline | c0355b2f, clean; accepted S7. |
| Candidate Proposal | [T72 proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md), section fifteen. |
| Files And ABI Surface | common/ui/ui.c and common/ui/ui_interface.h, session/control.c/session.c, machine/machine.c, debug/command.c; App composition.c terminal teardown receiver; existing tests, root CMake test-only source selection and manifests. No API shape changes. |
| Applicable Rules | Execution, architecture, coding, documentation; project design/layout/UI and shared governance skills. |
| Verification | Destroy failure at each child and callback dependency retention; status RGB contrast; running/paused TEXT and sink rejection; dirty out create failures; debug close/reopen/continuations/argument limits; x86/x64 builds and background regression. |
| Expected Markers | Failed teardown preserves live dependencies; one input route; constructors clear outputs; no heap lifecycle for fixed debug argument table. |
| Asset Needs | Refresh both EXEs only; preserve INI/media and owner snapshot. No desktop tests without reserved time. |
| Reporting Requirements | Estimate production seven files +35--55/-30--50; tests +110--180/-10--25. Report actual added/removed/net, verified scope and EXE links. |
| Stop Conditions | Lib change, new recovery machinery, altered debug semantics or unsafe callback lifetime requires reassessment. |
| Exit Criteria | Finite A/B/C/D/F proof, dual-width verification, actual-change review, pushed clean delivery, then wait for owner test. |
| Original Owner Request | 分成两个s任务 第一个先优化lib 第二个处理common；Common 顺延 S8；确认推送。完成后准入下一个S8。 |
| Similar-Issue Sweep | UI destroy/action/create cleanup, all Common constructors and input variants, argument table references, final App teardown consumer; each hit gets proof or retained-role disposition. |


## Current Technical Baseline

- S8 production seven C/H +33/-36 (net -3); six C tests +149/-5 (net +144).
  Implementation 95850780 pushed; coordinator actual-change review accepted
  the bounded A/B/C/D/F delivery, not owner acceptance or whole-tree closure.
  Test build/gate +2/-2 (net0). Both Release builds pass; final background
  x64 105/105 (134.41s), x86 105/105 (135.13s). Five desktop tests per width
  excluded. Lib/VM/Compat/MVDM, INI/media/snapshot unchanged. See
  [S8 evidence](../etc/evidence/softpc/m9-t72-s8-common-cleanup.md).
  S8 and T72 remain open pending owner test, not automatically closed.
- S7 production three C/H +58/-62 (net -4); three tests +97/-27 (net +70).
  P2 removes row scratch in render.c, +26/-34 (net -8) relative to P1.
  Both Release builds pass; background x64 105/105 (169.98s), x86 105/105
  (160.76s). Five desktop tests per width excluded; owner manual test passed.
  No public ABI, Common, VM, Compat, MVDM, input, INI or media change.
- S6 production +0/-2 (net -2), test +46/-1 (net +45), no ABI/Common changes.
  Both Release builds pass. Background x86 105/105 (147.31s); x64 104/105
  (161.37s), documentation-only failure corrected and rerun 1/1. Runtime tests
  all pass; five desktop tests per width excluded. S6 owner accepted.
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
