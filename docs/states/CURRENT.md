# Project Status

## Current Work

M9 T71 S1 is implemented and verified, awaiting owner testing: text-only Console
frame admission. Owner: "准入T71".
T70 remains closed. Later text-schema/capacity batches await concrete design review.

## M9 T71 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admits T71 from the queue head; start bounded batch A. |
| Objective | Reject graphical Console publications synchronously without replacing pending text or waking the worker. |
| Non-goals | No character/layout ABI, mouse, routing policy, MVDM, VM, INI or guest-media change. |
| Reference Baseline | 7557ca5; T70 closed; shared implementation 8cb23e9; dual-width 109/109. |
| Candidate Proposal | [T71 design and S1 brief](../proposals/m9-kvm-text-frame-contract.md). |
| Files And ABI Surface | kvm-console console.c/public documentation; existing Lib retirement test and Common composition proof; manifests. No signature change. |
| Applicable Rules | Execution, Documentation, Architecture, Coding and Product UI; linked governance skills. |
| Verification | Deterministic invalid/unsupported/publication/activation/STOP matrix, Common text-status route; dual-width full builds/tests, manifests/DAG/documentation checks. |
| Expected Markers | Valid graphics returns UNSUPPORTED; malformed input INVALID_ARGUMENT; pending text and generation unchanged; no rejection wake/failure; stopped text INVALID_STATE. |
| Asset Needs | Existing package fixtures only; no media edits; bounded logs under ignored build/t71-s1, no raw tracing. |
| Reporting Requirements | Before/after production/test counts, both EXE links and actual validation limits. |
| Stop Conditions | Need for new frame ABI, fallback policy, ownership or unrelated repair. |
| Exit Criteria | All S1 ledger cases verified, actual-diff coordinator review, x86/x64 EXEs and tests, complete P committed/pushed for owner testing. |
| Original Owner Request | 准入T71 |
| Similar-Issue Sweep | All Console frame public callers, worker conversion paths, Common status routing and independent Window/mailbox admission. |

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

T70 is closed; T71 S1 is the sole active implementation task.

- **M9 Td S17:** Owner requested a concrete KVM text/frame correction proposal
  at queue head. Recorded capability rejection, explicit character/glyph
  semantics, bounded layout, migration batches and verification; no source or
  artifact changes. [Proposal](../proposals/m9-kvm-text-frame-contract.md).
  Documentation checks and actual-change review pass. At that delivery T70 remained open; the
  candidate is not implementation admission.
