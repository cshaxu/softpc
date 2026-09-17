# Project Status

## Current Work

T63 remains open. The owner explicitly rejected closure because snapshots
do not yet include modified FDD/HDD overlay contents. The uncommitted T63
closure and T64 admission have been withdrawn; T64 is not allocated.

M9 T63 S10 is active: complete snapshot media state. S9's accepted CPU/device/
display repairs remain the baseline, not proof of complete snapshot coverage.
Lib optimization remains the first queued candidate.

## Current Technical Baseline

- Source: T63 S9 P9 (`aa2bc0d`); subsequent P10/P11 are documentation only.
- Version-5 snapshots have one host-width-independent streaming format.
  Save is running-to-paused; load is initial/stopped-to-paused and does not
  create a Window until resume. Ordinary pause/debug behavior is retained.
- Final recorded x64 and x86 suites each pass 105/105; both cross-width
  save/load directions include post-resume keyboard repaint. The owner
  accepted restored Win3.1 display and input.
- Existing x86/x64 package EXEs remain the accepted T63 binaries until a
  verified T64 code delivery. This admission does not claim a fresh build.
- FDD/HDD overlay payload is the active S10 requirement, not deferred debt.
- Existing owner deletions of obsolete bisect assets remain untouched.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T62 | S1-S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1-S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |
| T60 | S1-S7 complete; owner acceptance; D6 retained, dual-width 97/97. | [Audit](../history/M9-T60-completion-audit.md) |

## Recent Governance

The attempted T63 closure/admission was withdrawn before commit on owner
correction. No code, package or rule changed.

## M9 T63 S10 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner rejects T63 closure and explicitly requires an additional S for floppy/hard-disk overlay snapshot support. T63 remains open; no T64 admission. |
| Objective | Save all modified FDD/HDD overlay content in the same snapshot binary, then restore media and machine state to the same checkpoint. |
| Non-goals | No Lib optimization, second overlay implementation, guest image mutation, sidecar files or implicit copying of entire DIRECT/READONLY media. No new Common APIs. |
| Reference Baseline | Accepted T63 S9 P9 aa2bc0d; version-5 CPU/device snapshot and dual-width 105/105. Existing format does not archive overlays. |
| Candidate Proposal | [Snapshot design](../proposals/m9-machine-snapshots.md) |
| Files And ABI Surface | VM snapshot container/driver and Compat FDD/HDD media owners, relevant snapshot/media tests and docs. Reuse existing Lib Storage and two Common state operations; report any necessary shared API change before implementation. |
| Applicable Rules | docs/rules/EXECUTION.md, ARCHITECTURE.md, CODING.md, DOCUMENT.md; docs/design/ARCHITECTURE.md, CODING.md and UI.md; original snapshot media contract. |
| Verification | Audit every media lease and capture/restore call first. Test modified FDD and HDD content, no-change overlays, replacement rather than merge, base mismatch, malformed/truncated media sections, and fresh-process cross-width save/load/resume. Run complete x86/x64 tests and builds. |
| Expected Markers | One binary contains CPU/RAM/devices plus overlay state for each medium. DIRECT/READONLY retain external source semantics. Restore never silently combines old machine state with missing overlay writes. |
| Asset Needs | Preserve owner media and INI. Use disposable test media; refresh only the two approved package EXEs. |
| Reporting Requirements | Before implementation explain media inventory, base identity and restore ownership, estimate changed files/lines; after implementation give actual diff, full tests, commit/push and both EXE links. |
| Stop Conditions | Necessary Lib/Common API expansion, unrepresentable device state, or source media mutation beyond the original contract; report before proceeding. |
| Exit Criteria | All FDD/HDD media owners accounted for, overlay bytes archived and restored with tested replacement semantics, two-width cross-process proof, full regressions and pushed delivery. T63 stays open until owner acceptance and a complete original-request audit. |
| Original Owner Request | DIRECT/READONLY snapshots need not contain the whole disk; FDD/HDD overlays must be included. The complete snapshot is one binary. Owner explicitly says T63 cannot close with this gap. |
| Similar-Issue Sweep | All floppy slots, hard-disk slots, media modes, lease replacement, base identity, writable media state and error/cleanup paths; enumerate each owner and proof. |
