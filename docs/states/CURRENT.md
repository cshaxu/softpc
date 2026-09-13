# Project Status

## Current Work

M9 T55 S15 implementation and verification are complete: x64 53/53, x86 53/53,
strict library 3/3. Executor P1 `f36c560` is pushed; coordinator review of the
actual committed diff passed. The three repairs preserve application APIs. S14's verified
delivery is retained without inferring manual acceptance. T55 remains open;
this delivery awaits the owner's manual test.

## M9 T55 S15 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: “批准了 按照这个方案修复 s提交推送等我测试”. Internal selected mailbox notification adjustment explicitly approved. |
| Objective | Complete the three S15 repairs with one notification path and existing cleanup owners. |
| Non-goals | No matcher snapshot restriction, MVDM, media, INI, app lifecycle, mouse scaling, deferred mode-roundtrip repair or T closure. |
| Reference Baseline | e140ec5, clean main after S14 delivery and reproduced three-item audit. |
| Candidate Proposal | [Shared-library audit repairs](../proposals/m9-lib-activation-audit-repairs.md). |
| Files And ABI Surface | ui-base leaf-support wake selection, Window platform consumer, host Console surface cache, tests/contracts. Application public APIs unchanged. |
| Applicable Rules | Architecture, coding, execution and documentation rules and linked skills; current architecture, source layout and UI authorities. |
| Verification | Cache reconstruction and fault-during-control injection; real modal-loop notification/STOP barriers; full x64/x86 CTest, strict lib and static gates. |
| Expected Markers | Three finite ledger rows proved; one notifier per instance and one Window consumer; no additional worker or polling recovery loop. |
| Asset Needs | Existing non-mutating package media; refresh only fixed x86/x64 EXEs; no trace recordings. |
| Reporting Requirements | Per-item proof, similar-issue dispositions, production/test line accounting, executor commit/push then coordinator actual-diff review. |
| Stop Conditions | Do not expand product semantics or hide failed tests; unresolved boundary changes require owner direction. |
| Exit Criteria | Three repairs and similar-path sweep verified, both EXEs built, manifest/DAG/governance and full regression pass, executor push and coordinator review, clean worktree awaiting owner testing. |
| Original Owner Request | “批准了 准入一个新的S任务修复 编译 测试 提交 推送 等我测试”; selected internal notification design subsequently approved. |
| Similar-Issue Sweep | Every mailbox frame/control/STOP/fault notification, Window callback/consumer boundary and native Console cache-invalidating operation. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 53/53
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
