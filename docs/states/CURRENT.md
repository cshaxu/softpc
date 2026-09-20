# Project Status

## Current Work

No implementation subtask is active. T71 S1--S6 are complete; T71 remains
open for owner manual acceptance. S6 delivery 03fe7627 is reviewed and pushed.
Open task awaiting owner: T71.
See [S6 closure](../history/M9-T71-S6-integration-audit.md).
The open task retains its [T71 plan](../history/M9-T71-kvm-text-frame-contract-proposal.md).

## Current Technical Baseline

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

T70 is closed; T71 awaits owner acceptance with no active implementation task.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
