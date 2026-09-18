# Project Status

## Current Work

T63 remains open pending owner acceptance of complete snapshot media state.
The prior attempted closure and T64 admission were withdrawn; T64 is not
allocated.

S10's implementation awaits owner Overlay acceptance; S11 is accepted and
pushed. M9 T63 S12 is active: allow `save` from PAUSED by reusing an already
safe snapshot checkpoint or advancing privately to one. Lib optimization
remains the first queued candidate.

## Current Technical Baseline

- Source: T63 S12 P16 (`a7278d9`).
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

## M9 T63 S12 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts S11 and directs the next S: `save` must work from PAUSED. If VM is already at a snapshot-safe checkpoint, write it directly; otherwise arm its safe checkpoint and resume only until it is reached. |
| Objective | Permit `save <file>` from RUNNING or PAUSED while preserving the existing result: a successful save leaves the machine PAUSED. |
| Non-goals | No Lib change, new public Common API, Session/UI state, MVDM change, debug semantic change, new executor, product-visible Running transition, or input acceptance while an internally continued paused save runs. Load remains stopped-only. |
| Reference Baseline | T63 S12 P16 (`a7278d9`); snapshots are versionless, cross-width fixed-order streams with Overlay media state. |
| Candidate Proposal | [Snapshot design](../proposals/m9-machine-snapshots.md), S12 ledger. |
| Files And ABI Surface | App command validation/help, Common Machine's existing state-read internals, VM snapshot driver state and focused Common/VM/App tests. The existing `common_machine_read_state` signature and driver callbacks remain unchanged. |
| Applicable Rules | docs/rules/EXECUTION.md, ARCHITECTURE.md, CODING.md, DOCUMENT.md; docs/design/ARCHITECTURE.md and CODING.md; snapshot design. |
| Verification | Prove running save stays as-is; normal paused save internally advances to a safe point without a Running fact or guest-input admission; checkpoint-paused save writes immediately without running; timeout/failure remains PAUSED; command matrix/help and x86/x64 full suites pass. |
| Expected Markers | Common owns only generic request rendezvous and paused-loop continuation. VM alone decides whether its checkpoint is already reusable and owns the one-second safe-boundary search. |
| Asset Needs | Preserve owner media and INI; use disposable test snapshots and refresh only the two package EXEs. |
| Reporting Requirements | Before code, record paused-safe evidence and exact state/control flow; after code, give changed-path/net-line accounting, focused proof, full regressions, commit/push and both EXE links. |
| Stop Conditions | A required public Common API, Lib change, MVDM source change, inability to suppress an internal Running fact, or proof that ordinary paused state cannot safely advance through the existing executor; report before expanding scope. |
| Exit Criteria | RUNNING and PAUSED save contracts are separately proven, no guest input/lifecycle drift occurs during internal advance, save outcomes stay PAUSED, full dual-width proof passes and pushed packages await owner test. |
| Original Owner Request | Save should also run from PAUSED: write immediately if already safe; otherwise set the safe breakpoint and resume only to that point. |
| Similar-Issue Sweep | Running and paused read admission, existing snapshot-ready/failed states, state-read completion, paused command loop, input gating, lifecycle notifications, writer failure, timeout, repeated save and all command/help matrix entries. |
