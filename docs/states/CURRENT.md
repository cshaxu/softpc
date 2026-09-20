# Project Status

## Current Work

M9 T71 S6 is active: integration and simplification audit.
Owner: "好的，接下来准入修复，开始执行。" S5 delivery 193ff7f0 is reviewed,
closed and pushed; T71 remains open through S6 and owner acceptance.

## M9 T71 S6 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner-approved serial T71 S3--S6 plan; coordinator S5 actual-change review passed. |
| Objective | Close the T71 finite coverage ledger, verify final ownership and removed legacy paths, audit storage/copy costs and task-introduced simplification opportunities. |
| Non-goals | No unrelated cleanup, capacity expansion, new state/threads, guest fallback, MVDM/snapshot/INI/media changes or new platform support. |
| Reference Baseline | 193ff7f0 S5 delivery; dual-width Release and final 110/110 per width. |
| Candidate Proposal | [T71 plan](../proposals/m9-kvm-text-frame-contract.md), S6 integration audit. |
| Files And ABI Surface | All T71 changed frame/control contracts and producers/consumers under Lib/Common/VM/Compat and direct tests; current design, manifests and finite ledger. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and shared governance skills. |
| Verification | Requirement-to-source/test mapping, old-symbol and dependency sweep, exact storage/copy accounting; serial dual-width full suites and four corpus/DAG/package/snapshot gates. |
| Expected Markers | Base transports opaque payloads, leaf-owned interpretation, VM-owned mapping, no legacy alias or second frame path; bounded validation and explicit errors. |
| Asset Needs | Existing builds and both EXEs; bounded synthetic tests only, no user media/configuration changes. |
| Reporting Requirements | Initial production estimate +0/-0; explain any necessary revision before code changes. Report actual additions/removals/net and artifacts; no whole-hardware correctness claim. |
| Stop Conditions | Material repair beyond the approved ownership migration, new state owner/thread/frame path, guest behavior workaround or unrelated scope growth. |
| Exit Criteria | Every T71 ledger member has inspected evidence; obsolete task paths removed or justified; required verification passes, P pushed and coordinator review complete; T remains open for owner acceptance. |
| Original Owner Request | kvm-base 只管两个不同mailbox的实现和传输机制 (fifo and latest-wins)，实际命令处理都交给消费者；写入本T任务的proposal并进行S任务拆分；好的，接下来准入修复，开始执行。 |
| Similar-Issue Sweep | Task-wide frame/control type ownership, mapping constants, mailbox interpretation, validation/copying and stale interfaces across all production/test callers. |

## Current Technical Baseline

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

T70 is closed; T71 S6 is the sole active implementation task.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../proposals/m9-kvm-text-frame-contract.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
