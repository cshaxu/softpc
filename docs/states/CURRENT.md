# Project Status

## Current Work

M9 T70 S8 is implemented for owner testing: snapshot-owned floppy and
fixed-disk remounting. S7 is closed and reconfirmed in `596f36e`.
S8 and T70 remain open until owner acceptance.

## M9 T70 S8 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly admits S8 after withdrawing all uncommitted work and reconfirming S7 closure. |
| Objective | Load both floppy and HDD exclusively from snapshot paths and saved modes, verifying size/SHA and restoring overlay pages; detach and reopen every existing attachment even when its path is identical. |
| Non-goals | No Lib/Common/MVDM change, new snapshot format/version, fallback path search, user INI edit, or unrelated Setup investigation. |
| Reference Baseline | `596f36e` reconfirms `5074c28`; all abandoned uncommitted implementation was withdrawn. |
| Candidate Proposal | [Snapshot media remount](../proposals/m9-snapshot-media-remount.md). |
| Files And ABI Surface | Compat media archive/header, private VM call site and focused media/transaction tests; no public ABI change. |
| Applicable Rules | Execution, documentation, architecture and coding authorities; owner constraints on shared corpus and guest media. |
| Verification | x86/x64 focused media archive, snapshot transaction and cross-process tests, full CTest with failures disclosed, documentation gate; rebuild both fixed EXEs. |
| Expected Markers | No retained-attachment bypass; saved mode is independent of INI; both drive kinds demonstrably detach/reopen on same-path load; SHA rejection and overlay bytes remain correct. |
| Asset Needs | Disposable small test images under build only; refresh package EXEs; preserve user INI and guest images. |
| Reporting Requirements | Before/after production and test path/line counts; ownership, verification results, commit/push and EXE links. |
| Stop Conditions | Required Lib/Common change, changed snapshot codec, unrelated emulation defect, or need to modify user media. |
| Exit Criteria | Requested remount semantics verified for both drives, dual-width delivery complete and pushed, coordinator review and owner manual acceptance recorded; S8 stays open pending that acceptance and T70 remains open. |
| Original Owner Request | “将S7正确收口，提交推送，然后准入S8：当 load snapshot的时候，仅使用snapshot里提供的磁盘文件路径加载磁盘文件（floppy和hard_disk都是）。如果当前floppy和/或hdd已经有加载（即使是相同路径的同一个文件），也要将他们卸载后重新加载。” |
| Similar-Issue Sweep | All media archive preparation/restore/attachment consumers, same-path retention, INI mode selection, empty slots and overlay restoration; record disposition for each. |

## Current Technical Baseline

- S8 removes INI mode conversion and same-path attachment retention. Snapshot
  restore first detaches all slots, then reopens saved paths/modes and applies
  saved overlay pages. Codec and SHA/size checks are unchanged. See the
  [S8 delivery evidence](../proposals/m9-snapshot-media-remount.md#delivery-evidence).
- S8 x86/x64 builds and focused snapshot tests pass. Both widths pass 106/108
  aggregate regression cases; the two package Window checks fail at stage 16,
  before any snapshot load. Root cause is unestablished; no full pass is claimed.
- T70 implementation delivery `ce5f535` fixes the CCPU privilege-changing
  stack-load width using the existing SS-based helper in CALL, IRET and RETF.
  Owner confirmed that the supplied checkpoint passes Win95 hardware detection.
- The same delivery includes the bounded S2--S6 controller/PIC, SAS allocation,
  TLS setjmp and restored-continuation repairs; their distinct evidence is in
  the [investigation record](../history/M9-T70-S6-investigation-record.md).
- Both x86/x64 package EXEs are rebuilt. Non-integration tests pass 105/105
  on each width; focused checkpoint/snapshot/IRQ/BOP tests pass 4/4 each.
  Two package Window integration tests failed at Window observation; their
  cause remains unproven. Owner approved S7 closure with this disclosed limit
  and successful hardware-detection acceptance. No full-suite pass is claimed.
- Lib/Common, user INI and guest media are unchanged. Snapshots retain their
  width-independent format; S8 updates media semantics as above. Restoring a snapshot
  parks its exact continuation before another guest instruction executes.
- T69's accepted Compat display transaction, packed-painter width and text
  surface corrections remain. The nonreproducible native Window-height report
  remains in TODO; the later Win95 memory warning is not diagnosed by S7.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T69 | S1--S4 complete; owner-reopened cleanup removed its own obsolete code. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |

## Recent Governance

T70 S7 closure was reconfirmed and pushed in `596f36e`. S8 is the sole admitted
continuation under the owner's explicit numbering; queue order is unchanged.
