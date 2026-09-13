# Project Status

## Current Work

M9 T55 S13 implementation and verification are complete and await owner testing.
Executor P1 `6609562` is pushed; coordinator review of that committed diff passed.
The four repairs passed x64 52/52, x86 52/52 and strict-library 3/3 tests.
S12's verified delivery is retained without inferring owner acceptance.
T55 remains open; no next S is admitted while awaiting this test response.

## M9 T55 S13 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: “准入一个新的S任务完成修复、编译、测试、提交、推送，并等我测试”. |
| Objective | Complete the four S13 ledger entries without additional owners or execution paths. |
| Non-goals | No matcher snapshot restriction, MVDM, media, INI, app lifecycle, mouse scaling, deferred mode-roundtrip repair or T closure. |
| Reference Baseline | fccf9ab, clean main after S12 delivery. |
| Candidate Proposal | [Shared-library audit repairs](../proposals/m9-lib-activation-audit-repairs.md). |
| Files And ABI Surface | Console INPUT_RESET event, host binding helper, ui-console input state, Window surface/geometry, host text cache; corresponding tests and design/READMEs. |
| Applicable Rules | Architecture, coding, execution and documentation rules and linked skills; current architecture, source layout and UI authorities. |
| Verification | Reset before native activation, prepare failure/rollback and input rebind probes; SelectObject failure, cursor scanline mapping, mixed text/frame writes; full x64/x86 CTest and strict lib. |
| Expected Markers | Four ledger rows proved; accepted modifier snapshots preserved; no new worker, lock or persistent state. |
| Asset Needs | Existing non-mutating package media; refresh only fixed x86/x64 EXEs; no trace recordings. |
| Reporting Requirements | Per-item proof, similar-issue dispositions, production/test line accounting, executor commit/push then coordinator actual-diff review. |
| Stop Conditions | Do not expand product semantics or hide failed tests; unresolved boundary changes require owner direction. |
| Exit Criteria | Four repairs verified, both EXEs built, manifest/DAG/governance and full regression pass, executor push and coordinator review, clean worktree awaiting owner testing. |
| Original Owner Request | Keep matcher snapshots; reset component input before the next reader; check SelectObject failure; honor cursor top/bottom; invalidate frame cache after text writes. |
| Similar-Issue Sweep | All binding/activation and logical event consumers; surface creation/selection cleanup; cursor calculations; native text/frame writers and cache updates. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 52/52
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
