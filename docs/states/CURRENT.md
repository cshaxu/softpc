# Project Status

## Current Work

No implementation subtask is active. T72 remains open; S1--S7 are closed.
Open task awaiting owner: T72.
Owner reports S7 tests passed and approves closure. Common work remains S8, not started.
See [proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md) and
[audit](../etc/evidence/softpc/m9-t72-post-s5-quality-audit.md).
S6 delivery details: [evidence](../etc/evidence/softpc/m9-t72-s6-text-render-simplification.md).
P1 01a737b2 / review 2cf87250 accepted; [S6 closure](../history/M9-T72-S6-text-render-simplification.md).
S7 details: [unified damage evidence](../etc/evidence/softpc/m9-t72-s7-window-pixel-damage.md).
P2 c813f698 / review 6fa729a1 accepted; [S7 closure](../history/M9-T72-S7-window-pixel-damage.md).


## Current Technical Baseline

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
