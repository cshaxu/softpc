# Project Status

## Current Work

M9 T69 S1 is admitted: restore the standalone Compat presentation publication
contract for Win3.1 prompt display-mode transitions.

## M9 T69 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | S1 first proved and repaired missing Compat text-surface fills. Owner's subsequent Window-only reproduction proved that repair insufficient: the source trace alternates completed-looking 640x480 and 1280x480 graphics frames during guest mode setup. Owner requested a correct repair, build, test, commit and push within the same bounded defect task. Standing commit/push approval applies. |
| Objective | Restore the standalone Compat publication contract: a newly bound DIB remains private until original-painter output completes the full surface; temporary destination bindings, palette changes and V7 pointer overlay updates must never resize or corrupt the KVM Window. |
| Non-goals | No forced repaint, CLS special case, customer image/configuration change, focus/input repair, Lib/Common/VM API change, or MVDM mirror change. Do not implement an unused generalized terminal/scroll system. |
| Reference Baseline | The first repair established that text fills mutate `textBuffer` and report character-grid geometry. The source trace still proves the VM publishes alternating 640x480/1280x480 graphics frames after bind-dirty suppression. Its 32x32 dirty rectangles match the V7 hardware pointer overlay exactly: an overlay or palette update can still publish a newly bound, unpainted temporary DIB. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | `src/compat/dib_surface.[ch]` owns text-surface mutation, painter-readiness and DIB publication; `src/compat/v7_pointer.c` consumes that readiness/generation boundary; `src/compat/graphics_console_compat.c` maps original Console fill calls to it; Compat-focused tests prove binding, painter and overlay publication. No public product ABI changes. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. One shared text-surface owner remains required; no presenter or native Console bypass is introduced. |
| Verification | Prove characters and attributes fill independently, zero-origin full surface fill clears all exposed cells, coordinates/counts clamp at the cell-grid boundary, and reported count equals actual writes. Prove DIB bind emits no dirty frame; partial original-painter work remains private until a full-surface dirty output opens the new generation; V7 overlay and palette changes cannot publish before that point; explicit post-rebuild full invalidation still does. Run x86/x64 focused test and full suite; build both package EXEs. |
| Expected Markers | Compat reports character-grid geometry, both original fill operations mutate the shared `textBuffer`; each DIB bind invalidates any old pointer cache and starts a private painter generation; only full original-painter output or explicit post-rebuild invalidation opens that generation to overlays. |
| Asset Needs | Existing owner package/media only; no image writes. Refresh only package EXEs, preserving the owner INI. |
| Reporting Requirements | Record changed-path line accounting, all original Console-operation call sites/disposition, focused proof, full regression and manual Win3.1 route checklist. |
| Stop Conditions | Any evidence requiring a Lib/Common/VM API or MVDM mirror change stops S1 for owner approval. Any unrelated Console operation found reachable becomes a separately proposed S. |
| Exit Criteria | Bounded fill model and geometry are implemented with focused proof; both widths build/test; only intended Compat/test/CMake/docs/package-EXE paths change; owner package INI is preserved. |
| Original Owner Request | “好，你帮我正确修复一下，编译测试提交推送后供我检查” |
| Similar-Issue Sweep | Audit all redirected original Console operations: fill character, fill attribute, buffer geometry, scroll and WriteConsole. Implement only the reachable clear contract; classify all others with code evidence. |

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
