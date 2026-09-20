# Project Status

## Current Work

M9 T71 S7 is implemented and verified, awaiting delivery review and owner retest.
Owner manual acceptance failed after a044fa36; T71 remains open. S7 adds the
missing real Win3.1 startup coverage; prior S6 evidence was insufficient.

## M9 T71 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner reports T71 startup/error-restart regression; continue the admitted architecture goal without widening its boundaries. |
| Objective | Reproduce and resolve win causing Machine error and subsequent start process crash. |
| Non-goals | No clipping, capacity expansion, guest-specific exception, new frame path, media/INI changes or unrelated queue work. |
| Reference Baseline | a044fa36; S6 dual-width 110/110 did not exercise Win3.1 entry. |
| Candidate Proposal | [Regression brief](../proposals/m9-kvm-mode-transition-regression.md). |
| Files And ABI Surface | Compat video extent uses original selected-renderer dimensions; Common Session preserves ERROR; App rejects machine commands in ERROR. Public Machine/KVM APIs, Lib and MVDM unchanged. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and shared governance skills. |
| Verification | Real overlay DOS win rejects 80x480 while selected renderer is 80x25; shipping baseline start after ERROR exits code 1, not an exception. Focused transition/ERROR matrix, real win entry and serial full x86/x64 plus package builds. |
| Expected Markers | Valid transition reaches Windows; genuine unsupported output remains explicit; start in terminal ERROR is rejected with a prompt rather than exiting, preserving Machine's existing contract. |
| Asset Needs | Existing Win3.1 image read via overlay; bounded owned build/t71-s7 probes, 120 seconds/4 MiB per run. |
| Reporting Requirements | Root cause and concrete diff estimate before production edits; afterward added/deleted/net and both EXEs. |
| Stop Conditions | Need for new public state/ownership, MVDM changes, guest-specific workaround or media mutation. |
| Exit Criteria | Both owner scenarios reproduced and repaired with focused and full evidence; reviewed P pushed, owner receives binaries. |
| Original Owner Request | 启动后输入win直接machine error；再次start程序崩溃退出。 |
| Similar-Issue Sweep | All frame-result/no-frame branches and all error-to-cold-start cleanup paths affected by T71. |

## Current Technical Baseline

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

T70 is closed; T71 S7 investigates failed owner acceptance.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
