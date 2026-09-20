# Project Status

## Current Work

M9 T71 S2 is active, design only: leaf-owned frames and transparent mailboxes.
Owner: "准入s2 开始设计". S1 leaves the active slot at its verified delivery
boundary; no new manual acceptance is inferred. T70 remains closed.

## M9 T71 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits S2 design after S1 delivery 04d76945 and approves shared text base, leaf frame extensions, Window-owned graphics and opaque FIFO/latest-wins; latest request is to record the plan and S split. |
| Objective | Record the approved frame/mailbox ownership and S3--S6 plan, including owner-approved S5 fixed-capacity validation and explicit copy_frame failure results; retain CP437 ownership in VM and no duplicated production paths. |
| Non-goals | No code/build/EXE/INI/media change; no code-page detection, font recognition, dynamic frame, capacity expansion or MVDM edit. |
| Reference Baseline | 04d76945; S1 dual-width full suites 109/109 and pushed packages. |
| Candidate Proposal | [T71 S2 concrete design](../proposals/m9-kvm-text-frame-contract.md#t71-s2-concrete-design-for-owner-review). |
| Files And ABI Surface | Current and both proposals only. Planned leaf frame/mailbox/Common driver ABI migration is not yet implemented. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, Product UI and linked governance skills. |
| Verification | Source producer/consumer/codec inventory, S1 actual-diff review, documentation/whitespace gates. |
| Expected Markers | Shared text fields; leaf-owned bitmap/map/graphics; opaque FIFO/latest-wins; base STOP admission; finite migration ledger and per-S estimates. |
| Asset Needs | None; read-only source inspection; no runtime process or trace. |
| Reporting Requirements | Proposed structs, migration owners, preserved UX, memory/code estimates, open decisions and no implementation claims. |
| Stop Conditions | This documentation turn needs runtime edits; future preflights must settle exact storage/API shapes before code changes, and escalate new fallback or lifecycle semantics. |
| Exit Criteria | Revised proposal and S split reviewed, documentation gate passed, committed/pushed; later S packets not active yet. |
| Original Owner Request | 准入s2 开始设计; 第二项请你拆分到新的t任务proposal加入队列; 写入本T任务的proposal并进行S任务拆分。 S5: 保留现有容量，不扩容、不动态分配；统一做完整校验，超限明确失败，禁止静默裁剪。 |
| Similar-Issue Sweep | All text/frame publishers, copying/comparison/render/cache sites and snapshot entry points; confirm archive internals before implementation. |

## Current Technical Baseline

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

T70 is closed; T71 S2 is the sole active task, limited to design.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../proposals/m9-kvm-text-frame-contract.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
