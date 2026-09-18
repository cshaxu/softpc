# Project Status

## Current Work

M9 T67 S3 is admitted: restore snapshot media independently of startup INI
paths, while converting fixed-disk access according to the current INI mode.

## M9 T67 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner requires snapshot media to remain compatible with different INI settings: floppy uses the snapshot image and mode; hard disk uses the snapshot image while mapping its mode to the current `hard_disk_mode`. Standing commit/push approval applies. |
| Objective | Persist media image paths in the existing snapshot media section and restore each live medium through one Compat preparation/commit path, preserving the owner-approved floppy and hard-disk conversion matrix. |
| Non-goals | No Lib, Common, MVDM, lifecycle, input, display, snapshot magic/version, or new command-route change. No compatibility reader for pre-S3 snapshots, because the format deliberately has no discriminator. |
| Reference Baseline | T67 S2 (`7f81853`) has independent startup modes and modeful transactional floppy insertion; snapshot media still validates both paths and modes against the startup INI and does not serialize paths. |
| Candidate Proposal | [Independent floppy and hard-disk media modes](../proposals/m9-independent-disk-modes.md) |
| Files And ABI Surface | Compat media archive and GFI/HDD restore contracts; VM media preparation; snapshot tests and disposable test images. No public Common or Lib ABI, no App command grammar, and no preserved MVDM source. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT; VM owns the current fixed-disk policy; Compat owns copied archive paths, validation, candidate construction and native media commit; Lib remains the unmodified medium primitive owner. |
| Verification | Matrix proof: floppy ignores INI path/mode; hard disk readonly/direct interconvert; snapshot overlay to INI readonly rejects; readonly/direct to INI overlay opens overlay; overlay to INI direct materializes pages to the snapshot base. Verify failed preparation retains live media, snapshot bytes stay width-independent and untagged, and no test image enters `assets/`. Run focused/full x64/x86 suites and both packages. |
| Expected Markers | One persisted path per present snapshot slot; no snapshot path comparison against INI; no floppy INI mode/path input to restore; hard-disk target mode is derived only in one matrix function; no `media_mode`, magic, version, or Lib/Common modification. |
| Asset Needs | Test creates and removes small images under its working directory; no asset-media mutation. Preserve owner INI and guest media. |
| Reporting Requirements | Before code: file/ownership and estimated production/test/mirror delta. After: actual numstat, retained boundary reasons, focused/full dual-width evidence and package links. |
| Stop Conditions | Any needed Lib/Common/MVDM change, second snapshot reader/version discriminator, or a request to recover pre-S3 snapshots requires owner revision. |
| Exit Criteria | The complete matrix, no stale INI-path validation, disposable-image coverage, x86/x64 full proof and refreshed packages are pushed in one complete P; coordinator reviews actual paths and byte layout. |
| Original Owner Request | “软驱完全采用 snapshot 里的 image 和访问模式；硬盘模式转换为 INI 规定的模式：readonly/direct 互转、overlay→readonly 拒绝、readonly/direct→overlay 按 overlay 打开、overlay→direct 直接写入镜像。” |
| Similar-Issue Sweep | Search every `softpc_media_archive_prepare`, `*_media_restore`, snapshot path/mode comparison, and mode open/write call. Retain one preparation path and one commit path per native medium family. |

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
