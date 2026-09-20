# Project Status

## Current Work

Owner accepted T71 S8: "测试通过。下一个S". S8 is closed. S9 implements
the revised complete-frame/latest-wins design and Window-local surface
comparison; dual-width full suites pass. P1 838b076c is committed/pushed and
coordinator-reviewed; S9 awaits owner testing. T71 remains open; S10 semantic
audit remains inactive.

## M9 T71 S9 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepted S8 and replaced the S9 FIFO plan with complete-frame/latest-wins transport and Window-local comparison. Each S builds/tests, commits/pushes and leaves a clean worktree, then waits for owner testing. |
| Objective | Publish complete snapshots on machine display change; keep Base opaque/latest-wins; Window derives damage by comparing its RGB surface with the latest complete frame. |
| Non-goals | No FIFO, new queue/thread/cache, input, snapshot format, display capacity, Compat/MVDM or media change; no second VM-to-presenter route. |
| Reference Baseline | 65ad144a; owner accepted S8 dual-width packages. |
| Candidate Proposal | [Regression brief](../proposals/m9-kvm-mode-transition-regression.md). |
| Files And ABI Surface | Base mailbox/component support, Window render/frame and Console call site; VM removes transported dirty coordinates; Common complete-frame contract and tests. Input ABI unchanged; graphics frame no longer carries producer dirty. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and shared governance skills. |
| Verification | Delayed upstream A/B and leaf latest-wins; compare actual pixels, no-change/palette/stride/size/mode, cumulative native invalidation, STOP/failure, save/load; serial full dual-width suites and PIF roundtrips. |
| Expected Markers | Latest full frame repairs all differences from surface even after skipped frames; Base has no merge callback; no new queue wait. |
| Asset Needs | Existing overlay-only fixtures; owned build/t71-s9/render-bench.c and two EXEs, 30-second runs and exact-file cleanup after results; no media or trace. Preserve owner INI verbatim. |
| Reporting Requirements | Report source sweep and estimate before edits; afterward actual production/test additions/deletions/net, render performance and storage footprint plus both EXEs. |
| Stop Conditions | Need for new frame cache/thread, product-specific workaround or a change to readiness, input or snapshot semantics. |
| Exit Criteria | Focused fault matrix and full dual-width suites pass; reviewed P pushed, clean worktree, both binaries supplied; wait for owner testing. |
| Original Owner Request | 我觉得可以用这个方案重构，准入修复S9，现在开始。Revised scheme: complete frames on machine dirty; Base opaque latest-wins; Window compares latest frame with its own rendered surface. |
| Similar-Issue Sweep | Every frame publication/notification/capture/ack path from Machine through Session/UI to both leaves; closure, inactive Console, notification failure and run replacement. |

## Current Technical Baseline

- S9: Base only copies latest complete bytes; Window compares its existing RGB
  surface and invalidates changed bounds. No additional buffer/thread, input or
  snapshot-format change. Production +41/-89 (net -48); tests +174/-44 (net +130).
  Final x64 110/110 (173.66s), x86 110/110 (163.13s); both EXEs rebuilt.
  Compat/MVDM, INI and media unchanged. Exact hashes and bounded performance
  measurements are in the active proposal. Owner acceptance remains pending.

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
  Owner accepted S7/S8; S9 is active and S10 remains inactive.

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

T70 is closed; T71 S8 is owner-accepted and S9 is active.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
