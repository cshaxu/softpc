# Project Status

## Current Work

M9 T66 S1 is admitted: repair the VM `snapshot_finish()` failure path before
any code-reduction candidate.

## M9 T66 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner: 下一个t任务准入; standing commit/push approval. |
| Objective | Attach `snapshot_finish()` failure to the existing VM operation termination/result path, preserving the snapshot format and completed state semantics. |
| Non-goals | No public ABI, Lib/Common, thread, UI, lifecycle, CPU/device semantic, archive-order refactor, VM-local ownership simplification, or mirror-diff cleanup. |
| Reference Baseline | T65 closure `8e6bcd6`; clean worktree; historical T63 diff evidence is candidate context only. |
| Candidate Proposal | [Snapshot and component architecture simplification](../proposals/m9-snapshot-architecture-simplification.md) |
| Files And ABI Surface | Initially audit `src/vm` snapshot/driver completion paths and product tests; precise changed paths follow the required pre-implementation audit. Public ABI unchanged. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and selected governance skills; original-mirror and VM/Compat ownership boundaries. |
| Verification | Deterministic `snapshot_finish()` failure injection, result/clock/operation-order proof, x86/x64 full regression and shared gates. |
| Expected Markers | A finish failure cannot report successful completion or leave the VM operation/clock path inconsistent; no second completion or error route exists. |
| Asset Needs | Existing test media/fakes; refresh both EXEs only after implementation; never modify owner INI/media. |
| Reporting Requirements | Before code: exact ignored-result call path, current failure behavior, files and numstat estimate. After: actual production/test numstat, deterministic failure evidence, verification and EXE links. |
| Stop Conditions | Need for new public ABI, snapshot-format, Lib/Common, thread, product state semantic or unproven post-failure recovery change requires owner review. |
| Exit Criteria | Finish-failure path has one terminal result/cleanup owner; deterministic proof, dual-width builds/tests, manifests, commit/push and S1 closure. |
| Original Owner Request | 下一个t任务准入 |
| Similar-Issue Sweep | All `snapshot_finish()` call sites and result/clock restoration paths; no unbounded whole-tree claim. |

## Current Technical Baseline

- Source: T65 S1 delivery pending this closure commit; state read/write reset
  their completion event before publishing executor-visible work. No public ABI,
  new synchronization state or request-slot abstraction was added.
- Both widths have 107 passing test cases; package EXEs were refreshed without
  changing owner INI/media. See [T65 audit](../history/M9-T65-completion-audit.md).
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
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T65 closes the admitted Common request-order repair. Queue ordering is unchanged.
