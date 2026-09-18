# Project Status

## Current Work

M9 T69 S2 is admitted after owner closure of S1. It investigates the remaining
Win3.1 windowed MS-DOS Prompt width alternation without reviving any
source-/mode-specific publication filter.

## M9 T69 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepted S1's source-neutral transaction architecture and explicitly instructed: “先收口S1吧，然后S2继续做。现在窗口的msdos提示符的时候，kvm-window窗口宽度还是会不断跳跃”. Standing commit/push approval applies. |
| Objective | Find the earliest producer of the still-completed alternating Window geometry in the Win3.1 windowed MS-DOS Prompt route, then repair that producer with one general geometry/publication invariant. A copied graphics frame must carry only the original renderer's completed current surface geometry; KVM Window remains a passive consumer. |
| Non-goals | No KVM Window resize debounce/filter, no mode-number/PIF/application/palette/pointer/dirty-shape condition, no timing retry or forced repaint, no customer image/configuration change, no Lib/Common API change and no second renderer. Do not reopen S1's source-specific publication paths. |
| Reference Baseline | S1 P4 (`10d6c0f`) made Compat publication source-neutral. Owner observes normal-width/two-width alternation only when a Win3.1 MS-DOS Prompt initially starts windowed. If that same Prompt first enters guest fullscreen and returns windowed, it stabilizes. Thus a first-mode initialization/cache fact differs from a later display-mode transition; candidate ownership is limited to original `nt_graph.c` geometry selection/cache setup, Compat DIB bind/surface metadata, and VM copied-frame extraction. No conclusion is assumed. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | Initial audit is read-only across `src/mvdm/softpc.new/host/src/nt_graph.c`, `src/compat/dib_surface.[ch]`, `src/compat/graphics_console_compat.c`, `src/compat/video.c`, and VM frame extraction. Any implementation is limited to the proven owner; an MVDM change requires an explicit port-ABI reason and pristine-ledger disposition. No public product ABI change. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. One shared text-surface owner remains required; no presenter or native Console bypass is introduced. |
| Verification | Produce an evidence ledger for every completed geometry setter/bind/copy route. Reproduce or deterministically emulate consecutive 640/1280 completed candidates and prove the chosen owner publishes one coherent geometry. Distinguish Win3.1 Prompt initially windowed (fails) from fullscreen → windowed (stable), then cover ordinary DOS and a Win95 Setup graphical transition when available. Run x86/x64 focused proof and full regression proportionate to the changed path; build both package EXEs. |
| Expected Markers | No frame geometry is inferred from dirty bounds or suppressed by KVM. Each accepted graphics frame's pixels, stride and geometry derive from one completed DIB surface. The original geometry formula is either retained with evidence or corrected at its actual owner, never patched downstream. |
| Asset Needs | Existing owner package/media only; no image writes. Refresh only package EXEs, preserving the owner INI. |
| Reporting Requirements | Record the completed-geometry sequence, all setter/bind/copy-route dispositions, pre/post MVDM mirror diff and tracked code accounting, focused/full regression and manual route checklist. |
| Stop Conditions | Any proposed KVM filter, producer/mode-specific condition, timer/retry workaround, second rendering path, Lib/Common API change, or MVDM behavior branch without a narrow port-ABI reason stops S2 for owner direction. |
| Exit Criteria | The alternating completed geometry is traced to one owner and either repaired with a source-neutral invariant or disproven with deterministic evidence; no downstream KVM workaround exists; both widths build/test; only intended paths change; owner INI is preserved. |
| Original Owner Request | “现在窗口的msdos提示符的时候，kvm-window窗口宽度还是会不断跳跃” |
| Similar-Issue Sweep | Audit all graphics geometry setters, DIB binds, surface readers and copied-frame geometry derivations, including V7 and standard VGA paths. Each hit receives an evidence-based owner/disposition. |

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

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T68 closes the bounded original keyboard-table safety repair. T67's independent
media-policy ledger remains recorded in its T-level audit.
