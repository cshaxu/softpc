# Project Status

## Current Work

M9 T55 S21 is admitted: give the shared keyboard/video/mouse corpus its
accurate `kvm-*` identity without changing behavior or extending it to the
cooked monitor. T55 remains open.
Owner package INI and media are preserved.

## M9 T55 S21 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepted S20 manual testing and directed the three shared keyboard/video/mouse components and symbols to use the `kvm` namespace. |
| Objective | Make the shared corpus accurately name its keyboard/video/mouse responsibility while preserving every existing runtime behavior and boundary. |
| Non-goals | No MVDM/media change, monitor migration, product-interface redesign, lifecycle/input semantic change, ABI shape change other than mechanical identifier/path spelling, new queue/worker, Linux parity expansion, or T closure. |
| Reference Baseline | 2ea35ce; clean S20 P2 commit, x64/x86 57/57 CTest, strict library 6/6, preserved package media; owner accepted manual package testing. |
| Candidate Proposal | [S21 kvm corpus rename](../proposals/m9-t55-s21-kvm-corpus-rename.md). |
| Files And ABI Surface | The three presentation-component directories and all affected lib/app/test CMake paths, `kvm_`/`KVM_` symbols, lib prose/manifest/DAG fixtures, and application consumers; no behavioral API contract change. |
| Applicable Rules | Architecture, coding, execution and documentation rules/skills and design authorities. |
| Verification | Zero-old-name source/path gate, focused renamed KVM tests, fresh x64/x86 builds and full CTest, strict library CTest, manifest/DAG/source-boundary/documentation gates. |
| Expected Markers | No active production, test, CMake, manifest, or current documentation reference to the retired component namespace remains; `kvm-*` components retain the same allowed DAG. |
| Asset Needs | Refresh fixed x86/x64 EXEs only; preserve media and INI bytes. |
| Reporting Requirements | Frozen ledger, changed-path/line accounting, focused/full tests, executor P push and separate actual-diff coordinator review. |
| Stop Conditions | Do not rename or alter the cooked monitor as KVM, leave a parallel old spelling, change data/layout/control semantics, loosen the DAG, or alter MVDM. |
| Exit Criteria | Every admitted spelling/path is mechanically migrated, zero-old-name gate and all builds/tests/gates pass, commit/push; await manual package testing and keep T55 open. |
| Original Owner Request | “将共享 keyboard/video/mouse 组件改名为 kvm。” |
| Similar-Issue Sweep | All non-historical former presentation component paths, identifiers, CMake targets, source includes, test names, manifest entries, component prose, and app consumers. |

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
