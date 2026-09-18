# Project Status

## Current Work

M9 T67 S1 is admitted: split fixed floppy/hard-disk mode plumbing and
per-slot snapshot validation before changing the interactive floppy command.

## M9 T67 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner: “可以，准入实现。每个S任务开始前都要介绍如何实现的，以及代码变更估计；结束后要统计代码变更和为什么这样做是最干净的。” Standing commit/push approval applies. |
| Objective | Replace the one startup media mode with independent floppy/hard-disk modes, reject the removed `media_mode` key, and validate snapshot slots against their own mode without changing archive bytes. |
| Non-goals | No interactive insert grammar, Common removable-media API, GFI replacement transaction, Lib, MVDM, snapshot format, display, input or lifecycle change. |
| Reference Baseline | T66 closed at `46d6ba3`; mode currently flows as one `media_mode` through App, VM, Compat and archive preparation. |
| Candidate Proposal | [Independent floppy and hard-disk media modes](../proposals/m9-independent-disk-modes.md) |
| Files And ABI Surface | App startup configuration; VM copied options/machine state; Compat media archive private contract; product tests. No public Common or Lib ABI in S1. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT; one executor; Compat owns host media; MVDM and Lib remain unchanged. |
| Verification | Config parser independent/rejected-key cases; independent startup modes; media archive matching/mismatch proof; focused and full x64/x86 tests; both packages. |
| Expected Markers | No production startup path reads a global `media_mode`; the removed key is rejected; archive receives separate floppy/hard-disk modes. |
| Asset Needs | Existing disposable test images/fakes and package EXEs. Owner explicitly authorizes replacing the removed package INI key with the two equivalent overlay keys; no other INI setting or media changes. |
| Reporting Requirements | Before code: file/ownership and estimated production/test/mirror delta. After: actual numstat, retained boundary reasons, focused/full dual-width evidence and package links. |
| Stop Conditions | A need for Common API, interactive command, staged GFI replacement, archive-format or Lib/MVDM change moves to S2 or requires owner revision. |
| Exit Criteria | Independent startup/archive tests and dual-width packages pass; one complete pushed P; coordinator reviews the actual diff. |
| Original Owner Request | “磁盘访问模式有点不妙，应该是 floppy_mode 和 hard_disk_mode 分开，各自有 readonly / direct / overlay。” |
| Similar-Issue Sweep | Search every startup/options/archive `media_mode` consumer; classify test-only legacy literals versus production paths and leave no shared production mode. |

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

T66 closes the admitted snapshot/component simplification ledger. T67 S1 now
owns fixed-media mode separation; its interactive replacement work remains S2.
