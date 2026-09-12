# Project Status

## Current Work

M9 T55 S1 is active: import the current NXVM shared library exactly and make
the narrow non-MVDM SoftPC writer adaptation required by its new contract.

## M9 T55 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner approved a new task to import the original latest NXVM library, with SoftPC-side adaptation only where required for recovery. |
| Objective | Replace `src/lib/` with NXVM `64d211c9` exactly, adapt the non-MVDM prompt trace to the explicit byte-writer API, and recover fixed x86/x64 packages. |
| Non-goals | No `src/mvdm/softpc.new/`, guest-media, `softpc.ini`, NXVM-repository, product-lifecycle, UI, or mass C-runtime-call refactor. |
| Reference Baseline | SoftPC `9f563db`; read-only NXVM `64d211c9` `src/lib/`. |
| Candidate Proposal | [M9 T55 Exact NXVM Library Refresh](../proposals/m9-t55-nxvm-lib-refresh.md) |
| Files And ABI Surface | Entire imported `src/lib/` corpus and manifest; `src/app/prompt_trace.c`; focused storage/trace proof and CMake registration only if required; refreshed `softpc32.exe`/`softpc64.exe`. The writer API changes from text to `{bytes, byte_count}`. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout, and UI authorities. |
| Verification | Compare complete relative-path SHA-256 inventories to frozen NXVM; verify manifest; build and full CTest x64/x86; package smoke; diff and governance gates. |
| Expected Markers | Zero SoftPC-only files or byte differences below `src/lib/`; two length-bearing prompt-trace writes; explicit trace line endings; both package EXEs refreshed while `softpc.ini` and media remain untouched. |
| Asset Needs | Refresh only agent-owned `assets/binary/softpc32.exe` and `softpc64.exe`; preserve user-owned INI and all media. |
| Reporting Requirements | Report imported corpus revision, exact app adaptation paths, SHA-256 equality evidence, dual-width results, changed-path accounting, commits, and package links. |
| Stop Conditions | Stop for any required MVDM, media, INI, NXVM-source, product-semantic, or non-writer SoftPC change; stop and re-audit if NXVM `src/lib/` moves from the frozen revision. |
| Exit Criteria | Exact corpus equality, prompt trace compiles under the byte writer without behavior drift, required tests/gates pass at both widths, packages are refreshed, and a separate T-level closure audit is pushed. |
| Original Owner Request | “好的，准入一个T任务，用于原版导入最新nxvm的lib”. |
| Similar-Issue Sweep | Search every SoftPC production caller of `lib_storage_file_writer_write`; inspect all `src/lib/` relative paths and hashes, with each mismatch either imported exactly or rejected under the stop condition. |

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
