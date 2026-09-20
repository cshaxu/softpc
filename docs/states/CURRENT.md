# Project Status

## Current Work

Owner accepted T71 S7. S8 implementation and dual-width verification are
complete; preparing its reviewed delivery for owner testing. T71 remains open;
S9 FIFO and S10 semantic audit remain inactive.

## M9 T71 S8 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves S7 display repair, S8 request completion, S9 FIFO and S10 semantic audit. Each S must build/test, commit/push and leave a clean worktree, then wait for owner testing. |
| Objective | Finish outstanding synchronous requests exactly once after executor unwind; serialize admission with terminal cleanup. |
| Non-goals | No FIFO work, new public API, cancellation framework, timeout/retry, Lib/VM/Compat/MVDM or media/INI change. |
| Reference Baseline | 92be0e6a; owner accepted S7 dual-width packages. |
| Candidate Proposal | [Regression brief](../proposals/m9-kvm-mode-transition-regression.md). |
| Files And ABI Surface | common/machine/machine.c, test/common regression tests and manifests; existing public contracts unchanged. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and shared governance skills. |
| Verification | Inject frame/run/wait failures around pending save/load/debug/media; verify admission/termination order, completed-result preservation and normal lifecycle. Serial full x86/x64 suites and package builds. |
| Expected Markers | No outstanding request remains blocked after terminal unwind; no false PAUSED, duplicate completion or successful-result overwrite. |
| Asset Needs | Existing non-mutating fixtures. Owned build/t71-s8 baseline negative-test binary/source, 10-second run budget, removed after retaining the result; no media or trace. |
| Reporting Requirements | Root cause and concrete diff estimate before production edits; afterward added/deleted/net and both EXEs. |
| Stop Conditions | Need for a new driver cancellation contract, public state/ownership, platform or product-specific workaround. |
| Exit Criteria | Focused fault matrix and full dual-width suites pass; reviewed P pushed, clean worktree, both binaries supplied; wait for owner testing. |
| Original Owner Request | 测试通过。准入下一个S任务，先告诉我这个S任务简报。批准，开始。 |
| Similar-Issue Sweep | Snapshot read/write, debug, media and worker readiness/shutdown rendezvous; all run and permanent-worker exits. |

## Current Technical Baseline

- S8: synchronous request admission and terminal completion share a short
  Machine-owned lock; pending ordinary pause cannot prematurely complete save.
  The unused ready event is removed. Production +85/-39 (net +46), tests
  +161/-9 (net +152). Final x64 110/110 (169.85s), x86 110/110 (124.72s).
  Both packages rebuilt. No public ABI, Lib/VM/Compat/MVDM, INI, media or
  snapshot-format change. Earlier modal-test failures and their S10 audit
  receiver are disclosed in the active proposal; no modal repair is claimed.

- S7 continued repair: controller-font/readiness source and required-font result
  are corrected. Relative to 83e185c7, production +5/-3 (net +2), tests +117/-2
  (net +115). Final x64 110/110 (124.22s), x86 110/110 (142.06s), including
  both Win3.1 PIF initial modes and six roundtrips each. Both packages rebuilt
  with unchanged byte sizes. No Lib/Common/MVDM/INI/media change in this repair.
  Owner accepted S7; S8 is active, S9--S10 remain inactive.

- S7: selected-renderer text dimensions replace live-register sampling;
  ERROR reaches App unchanged and rejects machine commands without exiting.
  Production +16/-7 (net +9), tests +72/-5 (net +67). Both Release packages
  built; full serial x64 110/110 (123.75s), x86 110/110 (108.17s), including
  real overlay DOS win and subsequent stop/start to DOS. No Lib/MVDM/INI/media
  changes. See the active brief for reproduction, accounting and package hashes.

- S6 audit found default-height cursor geometry inconsistent with Window glyph
  rendering. The bounded repair resolves zero to the existing 16-row default;
  production +7/-6 (net +1), test +4/-0. The added x64 regression failed before
  and passes after the change on both widths. Both Release builds and serial
  full tests pass: x64 110/110 (113.24s), x86 110/110 (98.52s). Packages rebuilt
  with unchanged byte sizes. Delivery 03fe7627 and coordinator review are complete;
  the proposal records the finite-ledger audit and exact accounting.

- S5 delivery 193ff7f0: production C/H +137/-59 (net +78), test C/H +139/-17
  (net +122); four manifests +23/-23. Release builds succeed on both widths;
  final x64 110/110 (71.06 seconds), x86 110/110 (100.87 seconds). Both package
  EXEs rebuilt. No MVDM, INI, media or snapshot-format changes.
  [S5 review](../history/M9-T71-S5-frame-capacity.md) closes its finite boundary.

- S4 typed frames/opaque transport: 17c9da90; production +469/-345 (net +124),
  test C/H +406/-312 (net +94). Both EXEs rebuilt; final serial x64/x86 110/110.
  Console frame 7112 bytes; CP437 resides only in VM; no Compat/MVDM or media change.
  [S4 review](../history/M9-T71-S4-frame-ownership.md) records ledger and exact accounting.

- S3 control ownership: production C/H +44/-32 (net +12), verifier +4/-0;
  test C/H +102/-28 (net +74), test registration +6/-0. Both EXEs rebuilt;
  final serial suites 110/110 each. No Common/VM/Compat/MVDM/INI/media changes.
  [S3 review](../history/M9-T71-S3-control-transport.md) records actual diff and test-input correction.
- T71 S1 rejects graphical Console publications before mailbox mutation/wake;
  malformed frames reject separately. Existing text/status/STOP semantics remain.
  Production C/H +7/-3 (net +4); tests +37/-6 (net +31). Both EXEs rebuilt;
  serial x86/x64 full regression 109/109 each. Common/VM/Compat/MVDM production,
  INI and guest media are unchanged. See the active proposal for evidence.
- Latest implementation: `8cb23e9`; S12 closure: `f6ab0dc`.
  This is T71's predecessor baseline; its final x86/x64 regression passed
  109/109 each, including package and snapshot checks.
- Owner accepted Win95 hardware detection, installation/desktop, display and
  Window mouse repairs. Snapshot restore uses saved media paths/modes and
  reopens all attached slots. Detailed evidence is in the T70 audit above.
- S12 retains failed-join resources, stabilizes integer geometry, uses Linux
  per-open locks and consolidates startup cleanup. Linux-native lock tests
  remain unexecuted; controlled tests do not establish native Linux behavior.
- Existing TODOs and the new queued text/frame contract proposal remain
  separately scoped; no whole-emulator or whole-Lib correctness claim.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T70 | S1--S12 complete; owner approved T closure; final dual-width 109/109, disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; owner-reopened cleanup removed its own obsolete code. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |

## Recent Governance

T70 is closed; T71 S7 is owner-accepted and S8 is active.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
