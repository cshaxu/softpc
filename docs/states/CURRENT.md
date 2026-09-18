# Project Status

## Current Work

M9 T65 S1 is admitted: Common Machine synchronous-request publication order.
T64 is closed. T65 implementation has not started; existing EXEs remain T64.

## M9 T65 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner: 可以，批准收口提交推送T64，接下来准入T65; standing commit/push approval. |
| Objective | Prepare complete parameters and completion wait before publishing an executor-consumable synchronous request. |
| Non-goals | No Lib/VM/Compat/MVDM changes, public API changes, thread changes, safe-point policy, UI/session rewrite or S2 request-slot refactor. |
| Reference Baseline | T64 closure df5df14; code delivery 21e0fe0; clean worktree. |
| Candidate Proposal | [Common request simplification](../proposals/m9-common-machine-request-simplification.md) |
| Files And ABI Surface | common/machine implementation/private declarations and test/common; README/manifests; public ABI unchanged. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and selected governance skills; existing Common single-executor ownership. |
| Verification | Deterministic fake/barrier tests for preparation/publication, immediate completion, rejection and shutdown; shared gates and full x86/x64 regression after implementation. |
| Expected Markers | No consume-before-prepare or reset-after-completion; no rejected request overwrites active payload; no second request path. |
| Asset Needs | Existing test fakes and media; refresh two package EXEs after implementation, never owner INI/media. |
| Reporting Requirements | Before code: actual call-path audit and production/test diff estimate; after: numstat, focused/full evidence and EXE links. No invented estimate at admission. |
| Stop Conditions | Need for new public contract, Lib/VM change, product semantic change or unproven request mutual exclusion requires review. |
| Exit Criteria | Actual-change review, deterministic proof, dual-width EXEs/tests, manifests, commit/push; S2 separately follows after S1 closure. |
| Original Owner Request | 可以，批准收口提交推送T64，接下来准入T65 |
| Similar-Issue Sweep | State read/write, removable media, debug and their executor completion/cancellation paths; finite ledger in proposal. |

## Current Technical Baseline

- Source: T64 S2 delivery `21e0fe0`; narrowed frame copying, layout and workers unchanged.
- Both widths have 107 passing test cases after owner-authorized INI restoration
  and package recheck. See [S2 evidence](../history/M9-T64-S2-frame-copy.md).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- T63 snapshot behavior remains the owner-accepted baseline. Current package
  EXEs include T64 S2; INI is restored to its tracked configuration and was not committed.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T64 closure is pushed as df5df14. T65 takes the former Queue head; the other
candidates retain their relative order. Admission changes documents only.
