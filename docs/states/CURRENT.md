# Project Status

## Current Work

M9 T64 S2 is active: narrowed frame copying. S1 audit is complete; owner
cancelled thread unification and approved the prefix-plus-active-pixels copy.

## M9 T64 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved narrowed implementation and standing commit/push; explicitly exclude INI this delivery. |
| Objective | One frame-copy helper copies the complete prefix and only active graphic pixels. |
| Non-goals | No worker/Base changes, field-by-field copy, layout changes, VM/Compat/MVDM or snapshot changes. |
| Reference Baseline | T63 closure `3f1c961`; S1 source audit and owner decisions recorded in history. |
| Candidate Proposal | [Lib simplification](../proposals/m9-lib-frame-copy-console-task.md) |
| Files And ABI Surface | KVM frame helper/mailbox; Common published-frame copy; tests/manifests; layout unchanged. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and their governance skills. |
| Verification | Prefix/tail/stride/mode tests, existing dirty/ack tests, shared manifests/boundaries, dual-width full regression and documentation gate. |
| Expected Markers | Text copies skip 983040 bytes; graphics copies exactly stride*height pixels; locks/sequence remain unchanged. |
| Asset Needs | Refresh only assets/binary/softpc32.exe and softpc64.exe; no INI/media/snapshot changes or commits. |
| Reporting Requirements | Estimate production +15–30/-4–8; report actual production/test diff and copy-byte accounting. |
| Stop Conditions | Need for new state, ownership, layout or unrelated semantic change requires review. |
| Exit Criteria | Focused and full tests pass, actual diff reviewed, packaged and pushed; T64 awaits owner testing. |
| Original Owner Request | 批准做这个收窄版修复。 |
| Similar-Issue Sweep | Inspect Lib/Common full-frame copies and pixel readers; initialization and VM producers retained outside scope. |

## Current Technical Baseline

- Source: M9 T63 terminal P18 (`08f82ea`); the final code delivery is P17
  (`82f6a50`).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- Final x86 and x64 suites each pass 106/106. Owner accepted restored display,
  input, Overlay media and paused-save behavior. The fixed package EXEs are
  current; owner configuration remains in `assets/binary/softpc.ini`.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |
| T60 | S1–S7 complete; owner acceptance; D6 retained, dual-width 97/97. | [Audit](../history/M9-T60-completion-audit.md) |

## Recent Governance

T64 is admitted from the former Queue head. S1 source audit is recorded in
[history](../history/M9-T64-S1-lib-simplification.md). No production code changed.
