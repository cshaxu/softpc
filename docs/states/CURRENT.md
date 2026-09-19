# Project Status

## Current Work

M9 T70 S9 is closed after the package-test root cause was repaired and both
widths passed 109/109. Implementation deliveries are `5d6fe18` and `7c0c786`.
M9 T70 S10 is owner-accepted after delivery 62eec11 and dual-width 109/109.
M9 T70 S11 investigates and repairs captured-pointer escape, jitter and motion
boundaries. T70 remains open.

## M9 T70 S11 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepted S10 and explicitly admitted S11 investigation and repair; Win3.1 also affected, more apparent at higher resolution. |
| Objective | Establish and repair the capture/motion contract causing escape, jitter and invisible motion boundaries. |
| Non-goals | No guest-specific sensitivity adjustment, guest media mutation or snapshot-format change; previous Lib/Common modification approval boundary remains. |
| Reference Baseline | 62eec11; both widths 109/109; owner confirms display repairs. |
| Candidate Proposal | [S11 mouse investigation](../proposals/m9-window-mouse-capture.md). |
| Files And ABI Surface | Audit KVM Window native capture and motion, Common routing and VM/InPort input; name proven repair owner before editing shared corpus. |
| Applicable Rules | Execution, documentation, architecture, coding, source/research and product UI authorities. |
| Verification | Deterministic capture/edge/motion probes; owner snapshot where useful; dual-width focused and full tests for implementation. |
| Expected Markers | No ordinary input while uncaptured; captured relative movement does not exhaust a finite host-coordinate range; capture and clip agree across release/geometry changes. |
| Asset Needs | Owner snapshot and media, unchanged; build/t70-s11 diagnostics bounded to 300 seconds and 32 MiB per run; terminate owned probes. |
| Reporting Requirements | Actual mirror/test diff, dual-width results and executable links; distinguish observation from inference. |
| Stop Conditions | Need for guest-media mutation, unapproved shared-corpus change, or missing reproduction evidence. |
| Exit Criteria | Each reported symptom explained and verified repaired, or explicit blocker; dual-width tested delivery before owner validation. |
| Original Owner Request | Captured mouse can escape to host yet controls content on reentry; jitter and invisible boundaries require reverse movement to escape. |
| Similar-Issue Sweep | Capture loss, clipping replacement, focus, resize, coordinate rebasing, scaling and duplicate motion routes. |

## Current Technical Baseline

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

T70 S10 is accepted; S11 is the sole active task. Queue order is unchanged.
