# Project Status

## Current Work

T71 S10 is implemented and verified, awaiting P1 delivery/coordinator review.

Owner requested "收口当前S9". S9 is accepted and closed; see its
[closure review](../history/M9-T71-S9-latest-frame-delivery.md).
S10's semantic/stride and modal-test audit is retained in
the [regression brief](../proposals/m9-kvm-mode-transition-regression.md).
Default tests remain background-only; desktop tests require explicit execution.

## M9 T71 S10 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits the four reviewed items and S10 closure after verification. Explicitly approves the two-line original nt_cga host-layout correction. |
| Objective | Correct fixed-80 destination layout; align KVM cursor interpretation; document Machine request/context ownership; remove Console destroy alias. |
| Non-goals | No neutral attribute redesign, Compat compensation, capacity change, new state/thread/cache, input or snapshot-format change. |
| Reference Baseline | a1e7b945; clean main. |
| Candidate Proposal | [Regression brief](../proposals/m9-kvm-mode-transition-regression.md). |
| Files And ABI Surface | Original nt_cga.c host copy; KVM text contract/Console conversion; Machine header comments; Console release-only public lifetime; shared/product tests and manifests. |
| Applicable Rules | Execution, Coding, Architecture, Document; Source Layout, System Architecture, UI; linked governance skills. Owner-approved original-host layout defect is the sole mirror correction. |
| Verification | Real nt_text memory matrix; KVM cursor conversion and Window geometry; six alias consumers; dual Release builds and default background suites; static modal-path audit, no unreserved desktop execution. |
| Expected Markers | Correct destination rows with distinct source stride; matching effective cursor extent; no lib_console_destroy; all background tests pass. |
| Asset Needs | Existing test-generated disposable media only; no INI/media edits. Refresh both package EXEs. |
| Reporting Requirements | Before/after added/deleted/net production/test lines, mirror diff, test coverage and limits, artifact links; commit/push then actual-change coordinator review. |
| Stop Conditions | Any additional production architecture/semantic change or failed unexplained regression; do not claim desktop verification from background tests. |
| Exit Criteria | Four-item finite ledger verified, gates and builds pass, artifacts pushed, coordinator actual-change review and S10 closure; T71 stays open. |
| Original Owner Request | 准入修复，S10完成上述4个项目收口。 |
| Similar-Issue Sweep | TextBuffer writer strides, both leaf cursor conversions, synchronous request callbacks, all alias consumers; retained modal-test observation audited without speculative repair. |

## Current Technical Baseline

- S10 four-item repair: original nt_text fixed-80 destination layout (+3/-2),
  KVM Console cursor normalization, Machine request/context documentation and
  removed Console destroy alias. Production C/H +24/-14 (net +10); tests
  +80/-7 (net +73). Dual Release builds and background suites pass:
  x64 105/105 (163.44s), x86 105/105 (145.85s). Five desktop tests per width
  not rerun; earlier unexplained modal-test exit retained explicitly in TODO.
  Both EXEs refreshed; no Compat/VM/INI/media or snapshot-format change.
  See the regression brief for finite proof, shared manifests and hashes.

- S9: Base only copies latest complete bytes; Window compares its existing RGB
  surface and invalidates changed bounds. No additional buffer/thread, input or
  snapshot-format change. Production +41/-89 (net -48); tests +174/-44 (net +130).
  Final x64 110/110 (173.66s), x86 110/110 (163.13s); both EXEs rebuilt.
  Compat/MVDM, INI and media unchanged. Exact hashes and bounded performance
  measurements are in the regression brief. Owner approved S9 closure.
  Background-test follow-up: both widths 105/105, five desktop tests explicit-only;
  configuration +8/-3 (net +5), no additional product code or binary change.

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
  Owner accepted S7/S8/S9; S10 is the current bounded follow-up.

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

T70 is closed; T71 S9 is owner-accepted and closed. S10 is active; T71 remains open.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
