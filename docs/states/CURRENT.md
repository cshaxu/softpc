# Project Status

## Current Work

M9 T69 is owner-reopened as S4. Its preceding accounting audit proved that two pieces of
T69-introduced code are obsolete: a preserved-MVDM BIOS helper refactor that
the final painter-contract repair no longer uses, and an uncalled Compat
linear-fill wrapper family. S5 removes them within T69 instead of transferring
them to TODO. No behavioral repair or new abstraction is admitted.

## M9 T69 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Owner-Reopen |
| Admission And Approval | Owner: “这本应该是T69引入和T69应该收口的。你重开T69，追加S任务清理这些；不要留在TODO。” |
| Objective | Remove every S4-proven obsolete T69 artifact: restore the original local V7 BIOS conversion expressions and remove the unused Compat text-surface fill wrapper family. |
| Non-goals | No change to accepted transaction publication, packed painter width, detached text/cursor behavior, geometry policy, Lib/Common/API, guest media, INI, package, or PIF/mode/application special case. |
| Reference Baseline | T69 S4 closure commit `505001b`; OpenNT `5e4619ab61c2aa76151e03973cce340be2933e61` remains the preserved-MVDM comparator. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md), S4 cleanup findings. |
| Files And ABI Surface | `src/mvdm/softpc.new/base/inc/egavideo.h`, `base/video/ega_vide.c`, `base/video/v7_video.c`, `host/src/nt_graph.c`, `src/compat/dib_surface.c/.h`, affected focused tests and task records only. No public ABI changes. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. Preserved source minimum-diff priority applies; original local repetition is preferable to a no-longer-needed helper. |
| Verification | Full caller search before deletion; OpenNT no-index diff for restored files; x86/x64 build; x86/x64 VGA/text smoke and snapshot regression; full regression evidence proportional to changed renderer/Compat paths; documentation governance and `git diff --check`. |
| Expected Markers | No `v7vga_current_mode` declaration, definition or caller remains. No standalone linear-fill wrapper remains. Packed width still derives only from the selected painter. Logical-coordinate fill remains the unique live fill route. |
| Asset Needs | None. Preserve owner INI, HDD, floppy images and existing package artifacts unless a verified source build requires normal EXE refresh. |
| Reporting Requirements | Record pre/post production/test counts, per-file pristine diff reduction, caller-search evidence, retained live fill owner, and dual-width results. |
| Stop Conditions | A discovered external caller, need for a new interface/state, behavior difference in original V7 BIOS services, renderer regression, or scope beyond the named paths stops implementation for owner direction. |
| Exit Criteria | Both obsolete families are absent; restored MVDM paths match OpenNT where expected; live fill/painter contracts retain focused coverage; all required dual-width gates pass; no T69 cleanup remains in TODO. |
| Original Owner Request | “这本应该是T69引入和T69应该收口的。你重开T69，追加S任务清理这些；不要留在TODO。” |
| Similar-Issue Sweep | Search all production, test, CMake and documentation references to the helper and fill family; inspect every T69-only MVDM/Compat helper introduced by the task for a live receiver. |

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
- T69 S4 is active only to delete its own proven obsolete code. Its accepted
  source-neutral Compat display transaction, painter-width contract and
  detached text/cursor updates remain in force. The fullscreen native
  Window-height observation remains reproducibility-gated TODO debt.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T69 | S1--S3 completed; owner-reopened as S4 for task-owned cleanup. | [Prior audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T69 is owner-reopened for S4 to remove only its own audit-proven obsolete
code; its final closure record is retained as historical context, not current
authority.
