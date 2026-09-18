# Project Status

## Current Work

M9 T69 S1 is admitted: restore the standalone Compat text-Console operations
that update the shared presentation surface.

## M9 T69 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | This packet's read-only investigation proved that Compat reports text Console fills successful without mutating `textBuffer`. Owner then approved a correct repair, build, test, commit and push within the same bounded defect task. Standing commit/push approval applies. |
| Objective | Restore the original text-Console clear contract in the standalone Compat host so Win3.1 prompt and client-fullscreen-to-raw-Console presentation consume a correctly cleared shared text surface. |
| Non-goals | No forced repaint, CLS special case, customer image/configuration change, focus/input repair, Lib/Common/VM API change, or MVDM mirror change. Do not implement an unused generalized terminal/scroll system. |
| Reference Baseline | S1 proved `nt_clear_screen()` and `prepare_surface()` call original fill APIs while `softpc_compat_fill_console_character/attribute()` return success without writing the text surface. `GetConsoleScreenBufferInfo()` incorrectly returns pixel geometry as cell geometry. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | `src/compat/dib_surface.[ch]` owns the text-surface mutation primitive; `src/compat/graphics_console_compat.c` maps original Console fill calls to it; one Compat-focused test and its CMake registration. No public product ABI changes. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. One shared text-surface owner remains required; no presenter or native Console bypass is introduced. |
| Verification | Prove characters and attributes fill independently, zero-origin full surface fill clears all exposed cells, coordinates/counts clamp at the cell-grid boundary, and reported count equals actual writes. Run x86/x64 focused test and full suite; build both package EXEs. |
| Expected Markers | Compat reports character-grid geometry, both original fill operations mutate the shared `textBuffer`, and VM's normal copied frame can observe the update. |
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
