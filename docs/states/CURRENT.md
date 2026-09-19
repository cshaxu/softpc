# Project Status

## Current Work

M9 T70 S9 is closed after the package-test root cause was repaired and both
widths passed 109/109. Implementation deliveries are `5d6fe18` and `7c0c786`.
M9 T70 S10 is owner-accepted after delivery 62eec11 and dual-width 109/109.
M9 T70 S11 is owner-accepted after P2 delivery 58ac394.
M9 T70 S12 is owner-approved for closure after delivery `8cb23e9` and
dual-width 109/109 verification. T70 remains open awaiting owner direction.
No implementation subtask is active.
Open task awaiting owner: T70.

## Current Technical Baseline

- S12 preserves Base task ownership on failed cancel/join, propagates shutdown
  failure through existing consumers, makes integer Window fitting idempotent,
  uses per-open Linux file locks and consolidates startup cleanup. Production
  +93/-81, net +12 (Lib alone net -3); tests +177/-29. Both package EXEs rebuilt;
  x86 and x64 full regression 109/109 each. Linux-native locking has a new
  test but could not run here because WSL is absent; controlled Linux tests pass.
  See [S12 evidence and scope](../history/M9-T70-S12-lib-lifetime-geometry-cleanup.md).
- S11 P2 retains a single Window mouse path, samples and recenters the hidden
  host pointer, rebases on geometry changes and releases on ownership/clip
  loss. Four production files +68/-26; two tests +76/-16. Public events and
  APIs, Common/VM/Compat/MVDM, INI and media are unchanged. Both EXEs rebuild;
  x86 full regression passes 109/109, x64 serial full regression passes
  109/109. The initial x64 parallel run had two package failures with unexpected
  monitor input; these are disclosed, not claimed repaired by rerunning.
  See [S11 review and limits](../history/M9-T70-S11-window-mouse-capture.md#p2-implementation-and-bounded-review).
  Owner confirms the mouse problem is resolved.
- S10 repairs the planar 256-colour stride/wrap units and masks the V7
  preserve-memory flag before validating the mode number. Two mirror files
  +6/-5, existing VGA test +76/-0; Lib/Common/VM/Compat unchanged. Both widths
  pass 109/109, including snapshot and package tests. Overlay cold boot shows
  the complete logo and normal desktop without the former warning; a restored
  checkpoint reaches the same desktop. Both EXEs are rebuilt; source media
  and INI are unchanged. See [S10 evidence](../history/M9-T70-S10-win95-display-startup.md#implementation-evidence).
- S9 isolates read-time PIT guesses generating spurious IRQ0 work. The
  candidate timer.c repair and fixed-clock test are present; both package
  EXEs are rebuilt. Cold start reaches Win95's hardware/PnP dialog.
  Both widths pass the nine-case PIT test and 109/109 full regression tests.
  The stage-16 failures were a test-launch visibility conflict, now repaired
  without changing the product binaries or weakening assertions.
  Cold-start observation also reaches the date/time installation dialog.
  Owner subsequently confirms successful installation and normal desktop operation.
  See [bounded observations](../history/M9-T70-S9-win95-first-boot.md#cold-start-observation).
- S8 removes INI mode conversion and same-path attachment retention. Snapshot
  restore first detaches all slots, then reopens saved paths/modes and applies
  saved overlay pages. Codec and SHA/size checks are unchanged. See the
  [S8 delivery evidence](../history/M9-T70-S8-snapshot-media-remount.md#delivery-evidence).
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

T70 S12 is closed by owner approval. T70 remains open; no next S task is
admitted. Queue order is unchanged.
