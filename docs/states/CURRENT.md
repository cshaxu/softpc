# Project Status

## Current Work

M9 T67 S4 is admitted: repair snapshot-media preparation and restoration so
the S3 mode/path contract holds for fresh machines and live media alike.

## M9 T67 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts that a failed new snapshot-media mount need not restore the old mount; all other S3 media path/mode requirements remain approved. Standing commit/push approval applies. |
| Objective | Separate snapshot preflight from media mutation, make a fresh machine initialize from the snapshot attachment, and restore clean target media for every approved floppy/fixed-disk conversion. |
| Non-goals | No Lib, Common, MVDM, lifecycle, input, display, snapshot magic/version, or command-route change. No compatibility reader for pre-S3 snapshots. |
| Reference Baseline | T67 S3 (`d2f969f`) persists paths and derives fixed-disk target modes, but direct preparation holds its own verifier open, overlay reuse can retain later pages, and an uninitialized reset still mounts INI media first. |
| Candidate Proposal | [Independent floppy and hard-disk media modes](../proposals/m9-independent-disk-modes.md) |
| Files And ABI Surface | Compat archive target description and GFI/HDD restore contracts; VM's private copied current-attachment state; snapshot tests and disposable test images. No public Common or Lib ABI, no App command grammar, and no preserved MVDM source. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT; VM copies the selected snapshot attachment before a first reset; Compat owns target-mode derivation, verification and native replacement; Lib remains the unmodified medium primitive owner. |
| Verification | Matrix proof for fresh and initialized machines: floppy ignores INI path/mode; hard disk readonly/direct interconvert; snapshot overlay to INI readonly rejects; readonly/direct to INI overlay starts clean; overlay to INI direct materializes pages. Verify a direct preflight closes its verifier before direct open, and a restore mount failure may leave that slot detached as owner-approved. Snapshot bytes remain width-independent and untagged; no test image enters `assets/`. Run focused/full x64/x86 suites and both packages. |
| Expected Markers | Preparation holds no candidate medium; one target-mode derivation; overlay target always receives a new clean medium; initial reset reads copied snapshot attachment; no `media_mode`, magic, version, or Lib/Common modification. |
| Asset Needs | Test creates and removes small images under its working directory; no asset-media mutation. Preserve owner INI and guest media. |
| Reporting Requirements | Before code: file/ownership and estimated production/test/mirror delta. After: actual numstat, retained boundary reasons, focused/full dual-width evidence and package links. |
| Stop Conditions | Any needed Lib/Common/MVDM change, second snapshot reader/version discriminator, or recovery of pre-S3 snapshots requires owner revision. |
| Exit Criteria | The complete matrix in fresh/initialized paths, disposable-image coverage, x86/x64 full proof and refreshed packages are pushed in one complete P; coordinator reviews paths, target modes and byte layout. |
| Original Owner Request | “允许新挂载失败的时候不用恢复旧挂载；其他都同意，请你重开 t 任务准入一个或多个 s 任务进行修复。” |
| Similar-Issue Sweep | Search every `softpc_media_archive_prepare`, target-media description, `*_media_restore`, startup media attachment, snapshot path/mode comparison and mode open/write call. Retain one pure preparation path and one mutation path per native medium family. |

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

T66 closes the admitted snapshot/component simplification ledger. T67 S1--S3
are recorded in history; owner-reopened S4 repairs their snapshot-media
preparation/restore boundary.
