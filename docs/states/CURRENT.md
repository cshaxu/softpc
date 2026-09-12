# Project Status

## Current Work

M9 T55 S3 is active: normalize types into common C/compiler headers and
explicit win32/linux declaration headers, moving clock/input composition
back to their owning components without changing behavior.

Implementation and review evidence are ready for owner inspection: x64
41/41; x86 serial 41/41 after one recorded concurrent package-smoke failure
and three isolated passes; strict library 3/3. Linux execution is unverified.

T55 remains open. S2 is superseded for this boundary cleanup by the owner's
S3 admission, not accepted as a completed whole-library migration.
[S2 handoff record](../history/M9-T55-S2-types-boundary-handoff.md).

## M9 T55 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved: “同意，按照这个准入一个S任务清理lib types，要求符合收口标准后供我检查。” |
| Objective | Remove OS selection from top-level types headers; localize SDK/POSIX declarations by platform and purpose; retain compiler-only atomic selection; move clock composition and modifier interpretation into host/ui-base. |
| Non-goals | No `src/mvdm/softpc.new/`, guest media, `softpc.ini`, app/host product policy, UI semantics, NXVM checkout mutation, or duplicate platform path. |
| Reference Baseline | SoftPC `1b368d4`; preserve the owner's existing formatting changes in types/atomic.h. |
| Candidate Proposal | [M9 T55 Types-Owned External Boundary](../proposals/m9-t55-lib-types-external-boundary.md) |
| Files And ABI Surface | Entire `src/lib/` corpus, its CMake/manifest/READMEs, static boundary tests, required behavior tests, and only agent-owned package EXEs. `types` is header-only: it defines `lib_*` copied values, opaque native representations, and typed inline façades; component platform sources compose those façades into their own internal operations. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout, and UI authorities. |
| Verification | S3 seven-header ledger and include-site sweep; executable types layout gate; clock and atomic smoke; existing keyboard tests; strict library build; manifest; x86/x64 full CTest and package smoke; Linux compile/run if available, otherwise explicit coverage limitation; diff and governance gates. |
| Expected Markers | types remains header-only/INTERFACE; top-level headers have no OS branch or platform includes; no win32.h/posix.h umbrella; each platform consumer explicitly includes its platform vocabulary; host owns counter composition and ui-base owns modifier interpretation. |
| Asset Needs | Refresh only agent-owned `assets/binary/softpc32.exe` and `softpc64.exe`; preserve user-owned INI and all media. |
| Reporting Requirements | Report frozen ledger count/dispositions, contract/API changes, direct-link removal evidence, x86/x64 results, changed-path accounting, commits, and package links. |
| Stop Conditions | Stop for any required MVDM, media, INI, product-semantics, app/host policy, or platform dependency that cannot be represented as a neutral `types` primitive. |
| Exit Criteria | S3 ledger and focused/full checks pass; actual diff reviewed for behavior preservation; all changes committed/pushed with clean worktree; deliver EXEs for owner inspection. Keep S3 awaiting owner acceptance and do not close T55. |
| Original Owner Request | “同意，按照这个准入一个S任务清理lib types，要求符合收口标准后供我检查。” |
| Similar-Issue Sweep | Enumerate every types header, OS selection, SDK/POSIX include and old-header consumer; move or retain each with the S3 ledger disposition. Check moved clock/input bodies against baseline and preserve other platform bodies. This is not certification of all S2 raw-call wrappers. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 37/37
  fixed x64 and x86 CTest; its path-scoped standalone MSVC
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

## Recent M9 Task Progress

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
