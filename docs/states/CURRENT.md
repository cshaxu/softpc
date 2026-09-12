# Project Status

## Current Work

M9 T55 S2 is active: make `lib/types` header-only external vocabulary while
each owning component supplies its selected same-shape platform source,
without changing component behavior.

## M9 T55 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved: “准入S2，目标是：所有lib里面的原始 c 标准库和win32/linux等外部库的类型和函数定义，全部收归 lib/types 所有”. |
| Objective | Make `lib/types` the header-only shared-library vocabulary for C runtime, Windows SDK, Linux/POSIX and compiler atomics; each owning component supplies its selected same-shape platform implementation; retain existing behavior. |
| Non-goals | No `src/mvdm/softpc.new/`, guest media, `softpc.ini`, app/host product policy, UI semantics, NXVM checkout mutation, or duplicate platform path. |
| Reference Baseline | SoftPC `aa90f1e`; canonical library corpus imported from NXVM `64d211c9`. |
| Candidate Proposal | [M9 T55 Types-Owned External Boundary](../proposals/m9-t55-lib-types-external-boundary.md) |
| Files And ABI Surface | Entire `src/lib/` corpus, its CMake/manifest/READMEs, static boundary tests, required behavior tests, and only agent-owned package EXEs. `types` is header-only: it defines `lib_*` copied values, opaque native representations, and typed inline façades; component platform sources compose those façades into their own internal operations. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout, and UI authorities. |
| Verification | Ledger-backed static external-boundary scan; approved component-DAG link audit; strict library build; manifest; fresh x86/x64 build and full CTest; package smoke; diff and governance gates. |
| Expected Markers | `types` contains no `.c` and is an INTERFACE target; neutral component bases contain no platform branch; `win32`/`linux` peer sources implement the same component-private operation shape using `types` vocabulary; package EXEs refresh without INI/media change. |
| Asset Needs | Refresh only agent-owned `assets/binary/softpc32.exe` and `softpc64.exe`; preserve user-owned INI and all media. |
| Reporting Requirements | Report frozen ledger count/dispositions, contract/API changes, direct-link removal evidence, x86/x64 results, changed-path accounting, commits, and package links. |
| Stop Conditions | Stop for any required MVDM, media, INI, product-semantics, app/host policy, or platform dependency that cannot be represented as a neutral `types` primitive. |
| Exit Criteria | Every ledger member is disposed by the static gate; `types` owns external declaration vocabulary while each selected component platform source owns only its own operation implementation; all required builds/tests pass; owner manually accepts packages; a separate T-level closure audit is pushed. |
| Original Owner Request | “准入S2，目标是：所有lib里面的原始 c 标准库和win32/linux等外部库的类型和函数定义，全部收归 lib/types 所有”. |
| Similar-Issue Sweep | Enumerate all `#include <...>`, native type names, native function calls, and direct native CMake link entries below `src/lib/`; every hit is migrated to `types`, explicitly whitelisted as a language/compiler builtin, or transferred to TODO with owner approval. |

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
