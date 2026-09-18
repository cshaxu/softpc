# Project Status

## Current Work

M9 T66 S3 is admitted: narrow VM snapshot temporary ownership to its actual
callback lifetime and remove a confirmed unused snapshot-entry copy.

## M9 T66 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 开始吧 做完架构整理任务; standing commit/push approval. |
| Objective | Return one-shot VM snapshot capture data to the callback that owns its lifetime and remove an unused snapshot-entry copy, without changing snapshot transaction behavior. |
| Non-goals | No public ABI, Lib/Common, thread, UI, snapshot format, CPU/device behavior, archive-order refactor, original-mirror behavior, or externalized original state owner. |
| Reference Baseline | T66 S2 delivery pending coordinator closure; current clean committed baseline after its P; historical T63/T61 evidence is candidate context only. |
| Candidate Proposal | [Snapshot and component architecture simplification](../proposals/m9-snapshot-architecture-simplification.md) |
| Files And ABI Surface | Initially audit `src/vm/driver.c`, `src/vm/snapshot.c` and their focused tests. Precise paths follow the required pre-implementation audit. Public ABI unchanged. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and selected governance skills; original-mirror and VM/Compat ownership boundaries. |
| Verification | Snapshot roundtrip, failure cleanup and ownership inspection, x86/x64 full regression and shared gates. |
| Expected Markers | Capture image ownership has one callback-local lifetime; no unread snapshot-entry copy remains. |
| Asset Needs | Existing test media/fakes; refresh both EXEs only after implementation; never modify owner INI/media. |
| Reporting Requirements | Before code: exact ownership/callback paths, component/file ownership, production/test and MVDM mirror-diff estimates, boundary rationale and stop condition. After: actual production/test/component/mirror numstat, retained/moved/deleted disposition, focused evidence, x86/x64 build/full-test results, commit hash and both EXE links. |
| Stop Conditions | Need for a new public ABI, altered snapshot behavior/format, Lib/Common/thread/UI change, original-mirror change, or a capture object whose lifetime crosses callback completion requires owner review. |
| Exit Criteria | One-shot capture data has one local owner, unused entry copy is removed, dual-width builds/tests, manifests, commit/push and S3 closure. |
| Original Owner Request | 开始吧 做完架构整理任务 |
| Similar-Issue Sweep | VM snapshot images/entries with long-lived members despite single-callback use; no unbounded whole-tree claim. |

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
