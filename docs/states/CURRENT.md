# Project Status

## Current Work

M9 T55 S7 executor delivery is verified, pending committed-diff review.
Final x64/x86 passed 50/50; strict lib passed 3/3. One earlier package CAP
timeout is retained in TODO; subsequent full and repeated runs passed.
Baseline is `5be6fc4`; T55 remains open for owner inspection.

## M9 T55 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: “准入修复 要求代码设计有全局观 干净”. |
| Objective | Eliminate duplicate physical/character production, share held-key ownership during text synthesis, preserve native Unicode and repeat records. |
| Non-goals | No MVDM/media/INI, app policy, Console mouse scale, DAG or Linux UI parity change. |
| Reference Baseline | `5be6fc4`, owner-tested S6. |
| Candidate Proposal | [T55 proposal](../proposals/m9-t55-lib-types-external-boundary.md) |
| Files And ABI Surface | ui-base, both UI leaves, host raw input, types vocabulary, tests, docs/manifest and dual EXEs; app-facing ABI unchanged. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout and Product UI; referenced skills. |
| Verification | Interleaved physical/text input, held-key synthesis, real native Unicode entry, repeat batches, failure paths, full x64/x86 and strict lib. |
| Expected Markers | One held-key ledger; no character-credit table; native translation only for unmapped transitions; Unicode raw reader. |
| Asset Needs | Rebuild softpc32.exe and softpc64.exe; preserve INI/media. |
| Reporting Requirements | Four-item ledger and native-entry peers, evidence/accounting, complete P push and actual-diff review. |
| Stop Conditions | Escalate new product semantics or dependency edge; no unrelated repair. |
| Exit Criteria | Findings and peers verified; dual artifacts, pushed complete P, reviewed diff and clean worktree for owner inspection; T55 open. |
| Original Owner Request | “请问这次修复会加代码还是减代码 是否能干净的实现”; “准入修复 要求代码设计有全局观 干净”. |
| Similar-Issue Sweep | Every physical/text producer, synthesis lifetime, native character encoding and repeat boundary in lib. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 50/50
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
