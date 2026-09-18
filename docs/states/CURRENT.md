# Project Status

## Current Work

T63 remains open pending owner acceptance of complete snapshot media state.
The prior attempted closure and T64 admission were withdrawn; T64 is not
allocated.

S10's implementation is pushed and awaits owner Overlay acceptance. M9 T63 S11
is active: stop raw VM-Console mouse records from controlling the guest while a
Window is actually displayed. Lib optimization remains the first queued
candidate.

## Current Technical Baseline

- Source: T63 S10 P14 (`df0eda4`), with S11 input routing awaiting owner
  test.
- Snapshots are host-width-independent, fixed-order streams with no format
  version, magic or section identifier. Save is running-to-paused; load is
  initial/stopped-to-paused and does not create a Window until resume.
  Ordinary pause/debug behavior is retained.
- Final recorded x64 and x86 suites each pass 106/106; both cross-width
  save/load directions include post-resume keyboard repaint and Overlay media
  replacement. Owner accepted restored Win3.1 display and input; the new
  media-state behavior still needs owner acceptance.
- Current x86/x64 package EXEs include the S10 media implementation and
  versionless-layout simplification.
- FDD/HDD Overlay payload is implemented, awaiting manual verification.
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

## M9 T63 S11 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner reports that `display=console, console_control=0` raw Console mouse movement moves the guest pointer visible in its concurrently displayed Window. Owner directs that Common UI discard raw-Console mouse records whenever a Window is displayed. |
| Objective | Keep only Window-origin mouse input while a Window instance is live; retain raw Console keyboard/hotkeys and all existing behavior when no Window exists. |
| Non-goals | No Lib, VM, Compat, MVDM or Common Session API change. Do not change Window mouse input, keyboard input, hotkeys, guest mouse protocol, Console ownership or presentation derivation. |
| Reference Baseline | T63 S10 P14 versionless Overlay snapshot layout (`df0eda4`); full x86/x64 106/106. |
| Candidate Proposal | [Snapshot design](../proposals/m9-machine-snapshots.md), S11 ledger. |
| Files And ABI Surface | Common UI private input routing and its existing composition test; UI behavior documentation and task records. No public ABI changes. |
| Applicable Rules | docs/rules/EXECUTION.md, ARCHITECTURE.md, CODING.md, DOCUMENT.md; docs/design/ARCHITECTURE.md, CODING.md and UI.md. |
| Verification | Fake Window + VM Console prove Console mouse is acknowledged but not forwarded while Window exists, then passes once Window is destroyed; Window mouse always forwards; Console key/hotkey always forwards. Build and run complete x86/x64 suites. |
| Expected Markers | One source-local Common UI routing point owns the exception. Session and VM receive no surface-policy branch. |
| Asset Needs | Preserve owner media and INI; refresh only the two package EXEs. |
| Reporting Requirements | Record root cause, changed-path accounting, source-route sweep, focused proof, full regression, commit/push and both EXE links. |
| Stop Conditions | Any need to expose KVM private layout, change KVM event ABI, or add another event queue; report before implementation. |
| Exit Criteria | The four stated input combinations are covered; no source-policy branch is added outside Common UI; both build widths and full suites pass; pushed package binaries await owner test. |
| Original Owner Request | When a Window is displayed, discard raw Console mouse events in Common UI. |
| Similar-Issue Sweep | Every Common UI KVM input callback, raw Console path, Window path and source-retirement/failure path; document retained routing. |
