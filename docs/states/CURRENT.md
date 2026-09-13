# Project Status

## Current Work

M9 T55 S20 is admitted: make library pointer-output failures deterministic and
turn the approved component DAG into a source-level as well as build-level
gate, including canonical source include spelling. It also removes narrow local
contract/ownership and no-op forwarding tails. T55 remains open.
Owner package INI and media are preserved.

## M9 T55 S20 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved the post-S19 audit's pointer-output contract sweep, source-DAG verifier, and two narrow local contract cleanups. |
| Objective | Make lib failure outputs deterministic and component boundaries mechanically enforceable without changing product behavior. |
| Non-goals | No MVDM/media changes, product lifecycle redesign, frame/broker/UI-leaf merger, Linux parity expansion, new error framework, extra queue/worker/retry path, ABI signature change, or T closure. |
| Reference Baseline | 88e52bb; clean worktree, dual-width S19 56/56 CTest, strict library gates, preserved package INI/media. |
| Candidate Proposal | [S20 contract and boundary gates](../proposals/m9-t55-s20-lib-contract-and-boundary-gates.md). |
| Files And ABI Surface | Applicable lib output APIs, component-DAG verifier and focused tests, ui-console/storage local status/ownership paths, one internal ui-window forwarding helper, and current baseline prose; no public function signature change. |
| Applicable Rules | Architecture, coding, execution and documentation rules/skills and design authorities. |
| Verification | Focused output-contract/DAG/status proofs, fresh x64/x86 builds and full CTest, strict library CTest, manifest/DAG/documentation gates. |
| Expected Markers | Applicable pointer outputs are null after failure; forbidden direct and relative source includes fail the gate; local status/ownership and no-op forwarding tails are gone. |
| Asset Needs | Refresh fixed x86/x64 EXEs only; preserve media and INI bytes. |
| Reporting Requirements | Frozen ledger, changed-path/line accounting, focused/full tests, executor P push and separate actual-diff coordinator review. |
| Stop Conditions | Do not merge components, change UI/product semantics, create a runtime boundary layer, loosen the DAG, or expand storage behavior. |
| Exit Criteria | Each ledger row is implemented/proved, all applicable pointer-output hits are disposed, builds/tests/gates pass, commit/push; wait for manual testing and keep T55 open. |
| Original Owner Request | “批准。” |
| Similar-Issue Sweep | All production output-pointer APIs, all quoted lib component includes, and all same-class null/no-op or transferred-ownership cleanup paths. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus has S20 x64/x86
  builds with 57/57 CTest;
  strict standalone library gates pass 6/6. Its path-scoped standalone MSVC
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
