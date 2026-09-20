# Project Status

## Current Work

M9 T71 S4 is active: typed leaf frames and opaque latest-wins transport.
Owner: "好的，接下来准入修复，开始执行。" S3 delivery c1782fcc is reviewed,
closed and pushed; T71 remains open through S4--S6 and owner acceptance.

## M9 T71 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner-approved serial T71 plan; coordinator S3 review passed. |
| Objective | Shared text base with leaf-owned maps/fonts, Window-owned graphics/dirty, opaque latest-wins mailbox; one coherent Common/VM migration and VM-owned CP437. |
| Non-goals | No per-cell attribute redesign, capacity expansion, MVDM/Compat/snapshot format/INI/media or input/lifecycle changes; no new thread, registry, per-frame allocation or parallel ABI. |
| Reference Baseline | c1782fcc; dual-width Release and 110/110 per width. |
| Candidate Proposal | [T71 plan](../proposals/m9-kvm-text-frame-contract.md). |
| Files And ABI Surface | Lib frame/mailbox and logical Console contracts; Common machine/UI/session frame composition; VM producer; every direct test consumer and corpus/DAG manifests. Exact layout preflight required before code. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and their shared governance skills. |
| Verification | Text resource-only repaint, both banks, typed Console output, graphic dirty/late acknowledge/mode changes, NOT_CURRENT replay, shared/product/snapshot full serial x86/x64, manifests/DAG/docs. |
| Expected Markers | No monolithic kvm_frame, CP437 in VM only, no graphics/font/map interpretation in mailbox, no graphics/bitmap capacity in Console; one copied frame route. |
| Asset Needs | Existing builds and bounded build/t71-s4 probes; both EXEs; no INI/media changes or external runtime dependencies. |
| Reporting Requirements | Recount original provisional 420--700 production/150--250 test changed-line estimate after exact API and consumer inventory; report added/deleted/net and actual sizeof/copy bytes separately. |
| Stop Conditions | New worker/lifecycle, second frame path, unexplained scope growth, MVDM change or unsupported Linux feature implementation. |
| Exit Criteria | Frozen ownership ledger fully migrated without legacy aliases, typed leaf buffers and maps proven, dual-width tests pass, both EXEs/P committed/pushed then coordinator review. |
| Original Owner Request | 好的，接下来准入修复，开始执行。 |
| Similar-Issue Sweep | Inventory kvm_frame, copy/validate/publish/capture/acknowledge, fonts/maps/dirty and logical Console callers under src/test; inspect snapshot dependence explicitly. |

## Current Technical Baseline

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

T70 is closed; T71 S4 is the sole active implementation task.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../proposals/m9-kvm-text-frame-contract.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
