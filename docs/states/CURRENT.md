# Project Status

## Current Work

M9 T55 S19 is admitted: simplify three audited lib seams without changing the
approved product model.  It corrects stale public create prose, makes storage
result handling linear and preserves concrete local status, and makes Window's
one startup notifier replacement impossible to repeat. T55 remains open.
Owner package INI and media are preserved.

## M9 T55 S19 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved the post-S18 audit's stale-contract, linear-storage-status, and notifier-selection cleanup. |
| Objective | Improve lib maintainability and preserve correct failure semantics by removing misleading contracts, result-collapsing chains, and repeatable startup-only notifier setup. |
| Non-goals | No MVDM/media changes, product lifecycle redesign, frame/broker/UI-leaf merger, Linux parity expansion, new error framework, extra queue/worker/retry path, or T closure. |
| Reference Baseline | f700315; clean worktree, dual-width S18 builds/tests, strict library gates, preserved package INI/media. |
| Candidate Proposal | [S19 simplification sweep](../proposals/m9-t55-s19-lib-simplification-sweep.md). |
| Files And ABI Surface | ui-window/ui-console public prose, ui-base private mailbox support, storage medium result flow and focused tests; no public function signature change. |
| Applicable Rules | Architecture, coding, execution and documentation rules/skills and design authorities. |
| Verification | Focused storage/notifier/header proofs, fresh x64/x86 builds and full CTest, strict library CTest, manifest/DAG/documentation gates. |
| Expected Markers | Failed create is truthfully documented; storage does not discard a prior concrete result; notifier selection cannot silently replace an active wake path. |
| Asset Needs | Refresh fixed x86/x64 EXEs only; preserve media and INI bytes. |
| Reporting Requirements | Frozen three-item ledger, changed-path/line accounting, focused/full tests, executor P push and separate actual-diff coordinator review. |
| Stop Conditions | Do not merge required components, change UI/product semantics, create a second notification path, or expand storage behavior beyond local result preservation. |
| Exit Criteria | Each ledger row is implemented/proved, all touched callers and similar forms are disposed, builds/tests/gates pass, commit/push; wait for manual testing and keep T55 open. |
| Original Owner Request | “准入s任务予以清理”. |
| Similar-Issue Sweep | All public create documentation, all storage OR-chain result collapses, and all default-wake replacement call sites across lib and tests. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus has S17 x64/x86
  builds with 55/56 CTest (package fixture expects Console with a Window INI);
  standalone library gates pass 3/3. Its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |
| T50 | Build presets/artifact identity and standalone shared-library MSVC CI closed. | [T50 S2 history](../history/M9-T50-S2-lib-ci.md) |
| T51 | Shared library normalized to `types`, `console`, `host`, `storage`, and independent `ui-*` components; dual-width verification closed. | [T51 S4 history](../history/M9-T51-S4-lib-component-normalization-closure.md) |
| T52 | Native Console pointer capture was withdrawn: supported terminal hosts cannot provide a reliable visible client rectangle; no implementation remains. | [T52 record](../history/M9-T52-console-pointer-capture-withdrawn.md) |
| T53 | Raw Console one-cell input now maps to eight logical units on both axes; dual-width 37/37 verification closed. | [T53 S1 closure](../history/M9-T53-S1-console-logical-mouse-scale.md) |
| T54 | Restored the original X×8/Y×16 raw-Console InPort conversion, removed app-side mouse merging, and proved original InPort accumulation; owner accepted the balanced physical-input behavior. | [T54 S1 closure](../history/M9-T54-S1-console-mouse-scale-closure.md) |

- **M9 T50 S1:** Checked-in x64/x86 presets now bind compiler width to package
  identity and passed fresh 36/36 CTest at both widths.
  [Record](../history/M9-T50-S1-build-presets-artifact-identity.md)

- **M9 T50 S3:** The final post-library source tree rebuilt both fixed package
  widths and passed 36/36 CTest at each width.
  [Record](../history/M9-T50-S3-dual-width-post-lib-proof.md)

- **M9 T51 S2:** The entire library and every active SoftPC consumer moved in
  one buildable rename to `types`, `console`, and `ui-*`; fixed x64/x86 CTest
  each passed 36/36. [Record](../history/M9-T51-S2-lib-component-normalization.md)

- **M9 T51 S3:** Current architecture, UI, source-layout, component README,
  and executable boundary authority now agree on the normalized component DAG.
  [Record](../history/M9-T51-S3-component-boundary-authority.md)

- **M9 T51 S4:** Final manifest, boundary/governance, zero-old-name, and fresh
  fixed x64/x86 36/36 proofs passed; T51 is closed.
  [Record](../history/M9-T51-S4-lib-component-normalization-closure.md)

## Recent Governance

- **M9 Td S6:** Console-object design was promoted to current architecture/UI
  authorities. [Record](../history/M9-Td-S6-console-object-governance.md)

- **M9 Td S7:** Active-packet contract, identifier-mode checks, Queue/TODO
  integrity, CTest self-test, and all stale-proposal archival are now
  executable. [Record](../history/M9-Td-S7-executable-governance-state.md)

- **M9 Td S8:** Source provenance, research, legacy-material, and
  license-decision boundaries are now explicit and verifier-backed.
  [Record](../history/M9-Td-S8-source-research-policy.md)

- **M9 Td S9:** Future-task closure audit, whole-domain convergence, complete
  P discipline, path accounting, and build hygiene now match the relevant
  NXVM governance standard. [Record](../history/M9-Td-S9-execution-closure-quality.md)
