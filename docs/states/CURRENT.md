# Project Status

## Current Work

M9 T69 S2 is owner-reopened after the latest package still reproduced periodic
KVM Window width alternation when Win3.1 initially launches a windowed MS-DOS
Prompt. The previous V7 correction is retained, but its earlier closure claim
was incomplete.

## M9 T69 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Owner-Reopen |
| Admission And Approval | The owner reported: “初始是窗口状态的msdos提示符程序确实还是隔一会就引发kvm-window窗口宽度闪烁 感觉是我们的显示模拟无法很好的支持win31的显示驱动”. Standing commit/push approval applies. |
| Objective | Trace the first periodic completed geometry change in the standalone Win3.1 display-driver route, then repair the unique upstream display-model owner with a source-neutral invariant. |
| Non-goals | No KVM Window debounce/filter, PIF/application/mode-specific branch, timer retry, forced repaint, guest-media/configuration change, Lib/Common API change, second renderer or silent geometry coercion. |
| Reference Baseline | T69 S1 completed-frame transaction, S2 V7 current-controller geometry and S3 detached text/cursor guards remain the baseline. Owner reports width alternation still occurs; therefore none may be represented as the sole root cause. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | Read-only tracing begins at original `nt_graph.c`, `nt_vga.c`, `nt_ega.c`, `nt_cga.c`, controller state and Compat DIB/text surface metadata, then follows VM copied-frame extraction. A source edit is limited to the proven owner. Any MVDM edit needs a narrow port-ABI reason and pristine-ledger disposition. No public ABI change. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. The original renderer/Compat surface remains the only display state owner; KVM consumes copied complete frames only. |
| Verification | Trace confirms completed upstream `1280x480 -> 640x480` source frames; `1280` is the original generic 256-colour calculation. Focused x86/x64 VGA smoke now proves a second controller invalidation re-arms the existing settle gate. Run proportional full suites and both package builds before manual Win3.1 verification. |
| Expected Markers | No completed frame combines pixels/stride/geometry from different renderer states. No geometry is inferred downstream from dirty bounds or a previous frame. The selected correction has no customer/source/mode identity condition. |
| Asset Needs | Existing owner package/media only; no image writes. Refresh only package EXEs, preserving owner INI. |
| Reporting Requirements | Record each geometry source and disposition, the exact pre/post mirror diff, tracked code accounting, focused/full evidence and a manual checklist for initial windowed Prompt, fullscreen roundtrip and Win95 Setup transition. |
| Stop Conditions | A downstream filter, mode/PIF/application branch, timing workaround, unproven MVDM behavior change, second rendering route or Lib/Common scope expansion stops work for owner direction. |
| Exit Criteria | The periodic source geometry transition is traced to one owner and corrected or disproven by deterministic evidence; no downstream workaround exists; both widths build/test; the owner confirms the initial-windowed Win3.1 route. |
| Original Owner Request | “初始是窗口状态的msdos提示符程序确实还是隔一会就引发kvm-window窗口宽度闪烁”. |
| Similar-Issue Sweep | Every standalone-reachable graphics geometry calculation, DIB bind/reset and completed-frame copy route receives a recorded disposition. |

## Current Technical Baseline

- Source: T68 is closed. The preserved Scan-1 table mapper rejects both first
  out-of-range input values without changing valid key mappings. No Lib,
  Common, App, VM, Compat interface or input behavior changed.
- Both widths have 107 passing test cases; package EXEs were refreshed without
  changing owner INI/media. See [T65 audit](../history/M9-T65-completion-audit.md).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- T63 snapshot behavior remains the owner-accepted baseline. Current package
  EXEs include T67; the owner’s current package INI edit ships with T67.
- T69 is reopened: its source-neutral Compat display transaction, V7
  controller geometry, and detached text/cursor updates remain in force, but
  a periodic initial-windowed Win3.1 geometry transition still requires audit.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T69 | S1--S3 previously closed; owner-reopened as S2 after a later manual reproduction. | [Prior audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T69 closes the standalone display-route repair. The remaining candidates stay
in [Queue](QUEUE.md).
