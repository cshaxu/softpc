# Project Status

## Current Work

M9 T56 S1 is active: freeze the accepted SoftPC baseline and write the finite
common-extraction migration ledger. This is the first implementation/build
version for common extraction.

## M9 T56 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admitted the queued common-extraction work. |
| Objective | Freeze every tracked app/host responsibility, thread, lib call and test receiver; prove the current executable behavior with fresh x86/x64 packages before moving any production responsibility. |
| Non-goals | Do not create common production code, alter MVDM/lib/product behavior, resolve deferred TODOs, or change owner configuration/media. |
| Reference Baseline | T55 accepted corpus `987d82e`; current source `121de7c`; previous x86/x64 CTest 58/58 and strict lib 8/8 are historical reference only. |
| Candidate Proposal | [Common convergence](../proposals/m9-common-corpus-convergence.md). |
| Files And ABI Surface | Current packet; T56 S1 record; migration ledger/evidence and only focused baseline tests if a verified coverage gap exists; fixed x86/x64 EXEs. |
| Applicable Rules | Execution, architecture, coding, documentation authorities; Product UI; shared execution/architecture/coding/documentation governance skills. |
| Verification | Exact inventory and ownership review; existing behavior-matrix test audit; fresh package-x86/x64, test-x86/x64, strict lib gates, documentation/DAG gates, package hashes and diff hygiene. |
| Expected Markers | One finite ledger covers 24 app and 36 host tracked paths at admission; each has one disposition/receiver; every existing thread and lib call has an owner; no production relocation or second common path. |
| Asset Needs | Refresh only `assets/binary/softpc32.exe` and `softpc64.exe`; preserve adjacent user-owned INI and all media bytes. |
| Reporting Requirements | P1 commit/push contains ledger, test disposition, source/artifact hashes and x86/x64 EXE links; report changed-path counts and user hand-test steps. |
| Stop Conditions | Stop and record a proposal/TODO if a behavior gap requires MVDM change, external source import, product-semantics decision, or a broader test redesign. |
| Exit Criteria | Ledger is finite and reviewed, dual packages/tests/gates pass, no unintended production change exists, P1 is pushed, and user receives both EXEs for baseline confirmation. |
| Original Owner Request | 建立 common 组件并提取 debug、xasm32、session、UI、machine；CLI 注入 session，app 保持配置和实体组装；每个 S 删除旧实现并给可验收双 EXE，体验不变。 |
| Similar-Issue Sweep | All `src/app`/`src/host` tracked C/H/resource paths, CMake sources/targets, tests, threads, lib consumers, lifecycle queues and configuration/media boundaries. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`kvm-*` corpus has S21 x64/x86
  builds with 58/58 CTest;
  strict standalone library gates pass 8/8. Its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T55 | Canonical shared library refreshed, normalized, simplified, boundary-gated, and renamed to the KVM corpus; owner accepted final package testing. | [T55 closure](../history/M9-T55-closure-kvm-corpus.md) |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |
| T50 | Build presets/artifact identity and standalone shared-library MSVC CI closed. | [T50 S2 history](../history/M9-T50-S2-lib-ci.md) |
| T51 | Shared library normalized to `types`, `console`, `host`, `storage`, and independent presentation components; dual-width verification closed. | [T51 S4 history](../history/M9-T51-S4-lib-component-normalization-closure.md) |
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
  one buildable rename to `types`, `console`, and presentation components; fixed x64/x86 CTest
  each passed 36/36. [Record](../history/M9-T51-S2-lib-component-normalization.md)

- **M9 T51 S3:** Current architecture, presentation design, source-layout, component README,
  and executable boundary authority now agree on the normalized component DAG.
  [Record](../history/M9-T51-S3-component-boundary-authority.md)

- **M9 T51 S4:** Final manifest, boundary/governance, zero-old-name, and fresh
  fixed x64/x86 36/36 proofs passed; T51 is closed.
  [Record](../history/M9-T51-S4-lib-component-normalization-closure.md)

## Recent Governance

- **M9 Td S13:** Common extraction now permits responsibility-owned direct lib
  calls outside common; migration and acceptance reject duplicate ownership,
  not legitimate dependencies. [Record](../history/M9-Td-S13-common-lib-boundary.md)

- **M9 Td S12:** SoftPC-first common extraction now has five component ownership
  boundaries and eight sequential dual-width executable acceptance stages.
  [Record](../history/M9-Td-S12-common-extraction-plan.md)

- **M9 Td S6:** Console-object design was promoted to current architecture/presentation
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
