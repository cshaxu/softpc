# Project Status

## Current Work

M9 T67 S2 is admitted: make the one removable-media request explicitly modeful
and transactional, then require that mode in the monitor grammar.

## M9 T67 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admitted the two-mode design and required `floppy insert [readonly | direct | overlay] [image_file_path]`; automatic serial S admission and standing commit/push approval apply. |
| Objective | Give the existing removable-media request one explicit lib-defined mode, replace floppy media transactionally, and require mode plus exact path in the monitor command. |
| Non-goals | No new request queue, generic media manager, Lib, MVDM, fixed-media startup, snapshot format, display, input or lifecycle semantic change. |
| Reference Baseline | T67 S1 (`b88aa2b`) separates fixed-media startup policies and rejects `media_mode`; the live removable-media request is still path-only and GFI attach is destructive on open failure. |
| Candidate Proposal | [Independent floppy and hard-disk media modes](../proposals/m9-independent-disk-modes.md) |
| Files And ABI Surface | App command parser/effect/provider and documentation; Common machine's one existing copied request; VM driver/machine; Compat GFI host-media boundary; product/Common tests. No Lib or MVDM API/source. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT; Common serializes one request but owns no media policy; VM owns live floppy policy; Compat owns candidate preparation and commit; MVDM and Lib remain unchanged. |
| Verification | Parser accepts every valid mode and preserves exact path case; missing/unknown forms reject; Common forwards mode on its existing executor route; failed GFI insertion retains old medium; eject, stopped/paused admission, running rejection; focused and full x64/x86 tests; both packages. |
| Expected Markers | No path-only production removable-media call; no destructive old-medium teardown before candidate validation; no lowercase transformation of image path; command/help/UI/README use one required-mode grammar. |
| Asset Needs | Existing disposable media/fakes and package EXEs only. Preserve the owner-approved S1 INI values and all guest media. |
| Reporting Requirements | Before code: file/ownership and estimated production/test/mirror delta. After: actual numstat, retained boundary reasons, focused/full dual-width evidence and package links. |
| Stop Conditions | Any need for a second media command route, a public Lib change, MVDM/controller policy or snapshot-format change stops the task for owner revision. |
| Exit Criteria | One serialized modeful request, transactional candidate replacement, and exact required-mode grammar are proven by focused/full dual-width suites and packages; one complete pushed P; coordinator reviews actual diff. |
| Original Owner Request | “floppy insert [readonly | direct | overlay] [image_file_path]，必须指定模式才能insert floppy”。 |
| Similar-Issue Sweep | Search all `set_removable_media`, `set_floppy`, `floppy_attach`, and `floppy insert` paths; leave no path-only live insertion route, lowercased path, or destructive replacement-before-validation path. |

## Current Technical Baseline

- Source: T66 is closed. Snapshot finish errors publish their real result before
  Common completion; VGA/PIT restore reuse original device mechanisms; the VM
  no longer stores an unread checkpoint-entry copy. No public ABI, format,
  thread, Common or Lib behavior changed.
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
  EXEs include T64 S2; INI is restored to its tracked configuration and was not committed.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T66 closes the admitted snapshot/component simplification ledger. T67 S1 is
closed at `b88aa2b`; T67 S2 owns the one explicit, transactional removable-media
route.
