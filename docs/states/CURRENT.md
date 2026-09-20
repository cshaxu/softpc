# Project Status

## Current Work

M9 T71 S5 is active: fixed capacity and explicit frame failures.
Owner: "好的，接下来准入修复，开始执行。" S4 delivery 17c9da90 is reviewed,
closed and pushed; T71 remains open through S5--S6 and owner acceptance.
S5 preflight found backing-capacity and font-clamping prerequisites in Compat;
owner approved the narrow Compat changes ("批准修改compat"). S5 implementation
and dual-width verification are complete; executor P delivery/review is next.

## M9 T71 S5 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner-approved serial T71 plan and explicit S5 capacity/result-contract refinement; coordinator S4 review passed; owner additionally approved narrow Compat edits. |
| Objective | Validate fixed text/font/map/graphics limits at their owning boundaries; remove producer clipping and distinguish no new frame from failure through the existing Common completion path. |
| Non-goals | No capacity expansion, dynamic frame allocation, auto presenter fallback, guest-mode change, new side-channel error flag, MVDM/snapshot/INI/media changes or input/lifecycle redesign. Compat changes are limited to truthful presentation extent/font metadata. |
| Reference Baseline | 17c9da90 S4 delivery; dual-width Release and final 110/110 per width. |
| Candidate Proposal | [T71 plan](../proposals/m9-kvm-text-frame-contract.md), S5 fixed-capacity contract. |
| Files And ABI Surface | Base common-text validator; Window font/graphics and Console map admission; logical Console output; Common driver copy_frame result/caller and VM producer; Compat text extent/font queries; direct fakes/tests, manifests and relevant contracts. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and shared governance skills. |
| Verification | Zero/min/max/over-limit, font default/range, map representation, hidden/offscreen cursor, graphics stride/extent, no mutation/wake/cache on rejection; no-frame versus failure through real Common/VM path; full serial dual-width and package/snapshot gates. |
| Expected Markers | No silent producer clipping or boolean conflation; validators owned by the responsible component, one error path, no frame fallback. |
| Asset Needs | Existing builds and both EXEs; bounded synthetic tests only, no user media/configuration changes. |
| Reporting Requirements | Refresh preflight estimate after direct callback inventory before editing code; report additions/removals/net for production, tests, gates/docs separately and EXE sizes. |
| Stop Conditions | Additional executor/state owner, out-of-band error flags, unrelated scope growth, capacity expansion, MVDM changes, Compat changes beyond presentation queries or guest fallback. |
| Exit Criteria | Fixed-capacity matrix and actual failure propagation verified; clipping removed; all callers migrated, both builds/full tests pass; implementation P pushed then coordinator review. |
| Original Owner Request | 保留现有容量，不扩容、不动态分配；统一做完整校验，超限明确失败，禁止静默裁剪。好的，接下来准入修复，开始执行。 |
| Similar-Issue Sweep | All validation/copy_frame producers, callers and fakes under src/test; include implicit clipping and bool failure conflation, not only min() patterns. |

## Current Technical Baseline

- S5 working delivery: production C/H +137/-59 (net +78), test C/H +139/-17
  (net +122); four manifests +23/-23. Release builds succeed on both widths;
  final x64 110/110 (71.06 seconds), x86 110/110 (100.87 seconds). Both package
  EXEs rebuilt. No MVDM, INI, media or snapshot-format changes.

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

T70 is closed; T71 S5 is the sole active implementation task.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../proposals/m9-kvm-text-frame-contract.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
