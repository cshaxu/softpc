# Project Status

## Current Work

M9 T56 S6 is active: establish the approved original xasm32 corpus as a
common component from its fixed NXVM source reference, with no change to the
existing SoftPC command surface or executor ownership.

## M9 T56 S6 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admitted T56 and authorizes automatic sequential S progression after each tested, reviewed and pushed delivery; async package testing does not block the next S. |
| Objective | Freeze the exact approved NXVM xasm32 source reference, import it unmodified into `common/xasm32`, link it as a standalone common component and add copied byte/text contract tests. The existing SoftPC product need not expose a new CLI command in this S. |
| Non-goals | Do not change MVDM, guest input protocol, UI/session/machine behavior, command vocabulary, lib behavior, deferred TODOs, owner configuration/media, or import debug yet. |
| Reference Baseline | T56 S5: app owns the explicit product audio lifecycle; x86/x64 60/60 and strict lib 8/8 passed, with refreshed dual package EXEs. |
| Candidate Proposal | [Common convergence](../proposals/m9-common-corpus-convergence.md). |
| Files And ABI Surface | New `src/common/xasm32` source corpus/CMake target, source provenance/hash record and focused byte/text contract tests; no existing product ABI replacement. |
| Applicable Rules | Execution, architecture, coding, documentation authorities; Product UI; shared execution/architecture/coding/documentation governance skills. |
| Verification | Exact source-tree/file hash ledger, focused xasm32 byte/text/error tests, static proof no duplicate corpus remains, fresh package-x86/x64, test-x86/x64, strict lib gates, documentation/DAG gates and diff hygiene. |
| Expected Markers | `common/xasm32` has one fixed provenance record and one build target; no app/host duplicate xasm32 implementation or unadmitted CLI/executor route exists. |
| Asset Needs | Refresh only `assets/binary/softpc32.exe` and `softpc64.exe`; preserve adjacent user-owned INI and all media bytes. |
| Reporting Requirements | P commit/push contains moved/deleted path ledger, tests, source/artifact hashes, x86/x64 EXE links and changed-path counts; user tests asynchronously. |
| Stop Conditions | Stop and record a proposal/TODO if preserving an accepted driver/lifecycle behavior requires MVDM change, external source import, product-semantics decision, or a second executor/driver route. |
| Exit Criteria | Imported corpus matches frozen source reference; target/test contracts are linked at both widths; all named tests/gates and dual packages pass, review/push complete and EXE links reported. |
| Original Owner Request | 建立 common 组件并提取 debug、xasm32、session、UI、machine；CLI 注入 session，app 保持配置和实体组装；每个 S 删除旧实现并给可验收双 EXE，体验不变；手测异步进行。 |
| Similar-Issue Sweep | Source provenance, CMake dependencies, public includes, byte/text/error semantics, duplicate copies, accidental CLI exposure and any executor/control/UI ownership drift. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`kvm-*` corpus has S21 x64/x86
  builds with 59/59 CTest;
  strict standalone library gates pass 8/8. Its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| T56 S4 | Generic executor, lifecycle/input queues, run generation and completed-frame publication now belong to `common/machine`; app retains only its injected SoftPC driver. | [S4 closure](../history/M9-T56-S4-common-machine-extraction.md) |
| T56 S5 | App/host direct lib ownership was audited; the speaker worker now has one explicit app-managed lifecycle. | [S5 closure](../history/M9-T56-S5-ownership-audio-lifecycle.md) |

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
