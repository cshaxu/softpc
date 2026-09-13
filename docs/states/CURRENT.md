# Project Status

## Current Work

M9 T55 S12 is implemented and verified: all seven approved shared-library
audit repairs, both fixed EXEs and 52/52 CTest at each width. Executor delivery
is ready for commit/push and coordinator review. S11 remains closed; T55 is open.

## M9 T55 S12 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: “对啊，那就这么办。其他的按你说的来。” |
| Objective | Repair all seven S12 ledger entries through their existing component owners. |
| Non-goals | No MVDM, media, INI, app lifecycle, mouse scaling, or deferred mode-roundtrip repair; no T closure. |
| Reference Baseline | a0d0d37, clean main after S11 closure. |
| Candidate Proposal | [Shared-library audit repairs](../proposals/m9-lib-activation-audit-repairs.md). |
| Files And ABI Surface | lib console activation event; host private rollback contract; shared frame acknowledgement; hotkey, Window, Linux input; corresponding tests and READMEs. |
| Applicable Rules | Architecture, coding, execution and documentation rules and linked skills; current architecture, source layout and UI authorities. |
| Verification | Deterministic rollback, physical-key lifetime, frame acknowledgement/activation, geometry failure and blink-deadline probes; full x64/x86 CTest and strict lib checks. |
| Expected Markers | All seven ledger rows have focused evidence; independent UI leaves and component DAG retained. |
| Asset Needs | Existing non-mutating package media; refresh only fixed x86/x64 EXEs; no trace recordings. |
| Reporting Requirements | Per-item proof, similar-issue dispositions, production/test line accounting, executor commit/push then coordinator actual-diff review. |
| Stop Conditions | Do not expand product semantics or hide failed tests; unresolved boundary changes require owner direction. |
| Exit Criteria | Seven repairs verified, both EXEs built, manifest/DAG/governance and full regression pass, complete pushed delivery and clean worktree for owner testing. |
| Original Owner Request | Bind ui-console: draw its pending mailbox if any, otherwise nothing; “其他的按你说的来。” |
| Similar-Issue Sweep | Scan broker activation/retirement callers, all frame consumers, key identity comparisons, geometry results, worker blink wake paths and host product comments. |

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
