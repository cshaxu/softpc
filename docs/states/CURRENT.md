# Project Status

## Current Work

M9 T55 S3 is active: complete types-owned external vocabulary adoption,
retain component-owned behavior, and correct the keyboard layout modifier map.

Follow-up repairs are implemented and verified: x64 41/41, x86 41/41,
strict library 3/3 and ten standalone common/Windows headers pass. Delivery
awaits owner inspection; Linux execution remains unverified.

T55 remains open. S2 is superseded for this boundary cleanup by the owner's
S3 admission, not accepted as a completed whole-library migration.
[S2 handoff record](../history/M9-T55-S2-types-boundary-handoff.md).

## M9 T55 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved S3 and its full audit repair continuation: “准入使用当前或者新的S任务修复以上所有问题，完成后工作区清理干净并让我检查。” |
| Objective | Complete the approved audit ledger in the proposal: actual typed external vocabulary adoption, component-owned policy, correct keyboard layout modifier translation, and executable bypass checks; preserve component platform behavior. |
| Non-goals | No `src/mvdm/softpc.new/`, guest media, `softpc.ini`, app/host product policy, NXVM checkout mutation, or duplicate platform path. Preserve UI behavior except the approved VkKeyScan modifier correction. |
| Reference Baseline | Original S3: `1b368d4`; follow-up P2: `2f54899`. Preserve the owner's types/atomic.h formatting. |
| Candidate Proposal | [M9 T55 Types-Owned External Boundary](../proposals/m9-t55-lib-types-external-boundary.md) |
| Files And ABI Surface | Entire `src/lib/` corpus, its CMake/manifest/READMEs, static boundary tests, required behavior tests, and only agent-owned package EXEs. `types` is header-only: it defines `lib_*` copied values, opaque native representations, and typed inline façades; component platform sources compose those façades into their own internal operations. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout, and UI authorities. |
| Verification | Nine-class audit ledger, all C/H external-token sweep, negative bypass tests, deterministic eight-combination keyboard map/emission test, clock/atomic/keyboard tests, strict library build, manifest, x86/x64 full CTest and package smoke, standalone headers, actual diff and governance review; explicitly unverified Linux execution. |
| Expected Markers | types remains header-only/INTERFACE; top-level headers have no OS branch or platform includes; no win32.h/posix.h umbrella; each platform consumer explicitly includes its platform vocabulary; host owns counter composition and ui-base owns modifier interpretation. |
| Asset Needs | Refresh only agent-owned `assets/binary/softpc32.exe` and `softpc64.exe`; preserve user-owned INI and all media. |
| Reporting Requirements | Report frozen ledger count/dispositions, contract/API changes, direct-link removal evidence, x86/x64 results, changed-path accounting, commits, and package links. |
| Stop Conditions | Stop for any required MVDM, media, INI, product-semantics, app/host policy, or platform dependency that cannot be represented as a neutral `types` primitive. |
| Exit Criteria | S3 ledger and focused/full checks pass; actual diff reviewed for behavior preservation; all changes committed/pushed with clean worktree; deliver EXEs for owner inspection. Keep S3 awaiting owner acceptance and do not close T55. |
| Original Owner Request | “同意，按照这个准入一个S任务清理lib types，要求符合收口标准后供我检查。” Follow-up: “准入使用当前或者新的S任务修复以上所有问题，完成后工作区清理干净并让我检查。” |
| Similar-Issue Sweep | Entire library C/H corpus: external includes, raw types/functions/constants, copied virtual keys, null text-length callers, and NOT_CURRENT consumers. Gate derives prohibited tokens from types declarations; component workers remain token-equivalent after alias substitution, except the approved keyboard conversion. |

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
