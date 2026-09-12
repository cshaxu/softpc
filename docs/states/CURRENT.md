# Project Status

## Current Work

M9 T55 S4 is admitted and active: the seven approved UI input, shutdown,
blocking-lock and storage cleanup dispositions. S3 P5/P6 is delivered and
reviewed at `5ea133c`; it is not the active implementation packet.
T55 remains open. S4 P1 `fe620cb` is implemented, verified and pushed;
post-push actual-diff review passed. Owner package inspection remains pending.

## M9 T55 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: “按照这些准入新的S任务修复以上7条反馈意见。” |
| Objective | Unify frozen delivery, isolate keyboard prefixes, terminate failed input, close request admission on STOP/fault, replace three long-held spin gates, flatten storage allocation, and annotate deferred overlay indexing. |
| Non-goals | No MVDM, media, INI, app policy, mouse scaling, overlay algorithm or public synchronization API changes. |
| Reference Baseline | `5ea133c`, T55 S3 P6. |
| Candidate Proposal | [T55 proposal](../proposals/m9-t55-lib-types-external-boundary.md) |
| Files And ABI Surface | lib console/host/storage/ui-base/ui-window/ui-console, selected platform helpers and types declarations if needed; tests, CMake, manifest, docs and two package EXEs. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout and Product UI. |
| Verification | Seven-item finite ledger; frozen close/prefix/mouse and partial sink failure tests; STOP/frame race and FIFO barriers; blocking Console/broker barriers; storage regression; full x86/x64 and strict lib plus DAG/manifest/governance. |
| Expected Markers | One Window delivery filter; keyboard-only matcher state; fault ends input once; one mailbox admission boundary; long waits block without changing lock order; one stream owner; overlay unchanged except TODO. |
| Asset Needs | Rebuild only softpc32.exe and softpc64.exe; preserve user INI and guest media. |
| Reporting Requirements | Seven dispositions, exact tests, production/test accounting, actual diff review, pushed commit and EXE links; keep T55 open. |
| Stop Conditions | Escalate any required product policy, protected input or component DAG change; do not silently expand. |
| Exit Criteria | All seven dispositions verified (overlay optimization explicitly deferred), dual-width artifacts/tests, complete P committed/pushed and actual diff reviewed; clean worktree for owner inspection. |
| Original Owner Request | “按照这些准入新的S任务修复以上7条反馈意见。” The preceding approvals require removing duplicate frozen filtering, keyboard-only prefix order, fail-stop delivery, atomic STOP admission, three concrete long-held lock replacements, storage flattening and only an overlay TODO. |
| Similar-Issue Sweep | Every Window sink path, matcher replay, UI worker fault/STOP exit, Console output/event gate and broker transaction lock, storage stream allocations/close paths. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 48/48
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
