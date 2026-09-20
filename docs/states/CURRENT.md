# Project Status

## Current Work

M9 T71 S3 is active: opaque control FIFO. Owner: "好的，接下来准入修复，开始执行。"
S2 design is accepted after actual documentation review; T71 remains open.
S3 executor verification passed: x64 and x86 110/110; P1 delivery/review follows.

## M9 T71 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorizes implementation of the approved T71 plan; S2 design review is recorded in history. |
| Objective | Base transports opaque bounded control records; Window owns ordinary commands and interpretation; preserve STOP/FIFO and wake semantics. |
| Non-goals | No frame ABI/dirty/mapping changes (S4), capacity redesign (S5), Common/VM/MVDM/INI/media changes, new allocation, worker or callback. |
| Reference Baseline | 85940465; runtime baseline 04d76945 with dual-width 109/109. |
| Candidate Proposal | [T71 plan](../proposals/m9-kvm-text-frame-contract.md). |
| Files And ABI Surface | kvm-base control envelope/admission; Window command definitions and producer/worker; Console worker validation; shared tests/manifests/design and both EXEs. Public leaf APIs unchanged. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and their shared governance skills. |
| Verification | FIFO/full/copy/STOP/fault/wake/unknown-opcode focused tests; serial complete x86/x64 build/tests; manifest/DAG/documentation gates. |
| Expected Markers | Base knows only STOP; opaque opcode and 128-byte payload preserve existing record footprint; no Window commands in base; one FIFO. |
| Asset Needs | Existing builds and test fixtures only; bounded build/t71-s3 logs; refresh both EXEs, never INI/media. |
| Reporting Requirements | Preflight 70--130 production and 90--170 test changed lines; actual added/deleted/net, test evidence and EXE links. |
| Stop Conditions | New lifecycle, frame changes, additional threads/allocations or unbounded transport design required. |
| Exit Criteria | All control call sites migrated, no legacy Window envelope in base, focused/full tests pass, both EXEs and complete P committed/pushed; coordinator review before closure. |
| Original Owner Request | 好的，接下来准入修复，开始执行。 |
| Similar-Issue Sweep | rg control kinds/enqueue/take under src/test; migrate Window producer/worker, Console consumer and eight direct test consumers; Linux unsupported workers have no control loop. |

## Current Technical Baseline

- S3 control ownership: production C/H +44/-32 (net +12), verifier +4/-0;
  test C/H +102/-28 (net +74), test registration +6/-0. Both EXEs rebuilt;
  final serial suites 110/110 each. No Common/VM/Compat/MVDM/INI/media changes.
  Detailed evidence and the isolated test-input correction are in the proposal.
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

T70 is closed; T71 S3 is the sole active implementation task.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../proposals/m9-kvm-text-frame-contract.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
