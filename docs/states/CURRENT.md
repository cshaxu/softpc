# Project Status

## Current Work

M9 T55 S11 is verified: stale Window close request repaired; executor delivery
and committed-diff closure review are pending.
Executor P1 `16fa420` was pushed and its actual diff reviewed; T55 remains open.

## M9 T55 S11 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: “噢好的。请你修复这两点。” Build, test, commit and push authorized. |
| Objective | Complete focus ordering and expire an old X request on a new RUNNING transition, so later CAP retains Window; mode-roundtrip display defect separately deferred. |
| Non-goals | No MVDM, media, INI, lifecycle, capture or new public focus API changes. |
| Reference Baseline | `54fe10e`, S10 delivery. |
| Candidate Proposal | [T55 proposal](../proposals/m9-t55-lib-types-external-boundary.md) |
| Files And ABI Surface | Window unfreeze behavior and SoftPC control/reconciler activation order, tests and docs; no new public API. |
| Applicable Rules | Execution, Architecture, Coding, Documentation, System Architecture, Source Layout, Product UI and referenced skills. |
| Verification | Fresh x64 52/52 (34.96 s), x86 52/52 (36.02 s), lib checks 3/3; eight-case stale-X matrix fails before and passes after repair; earlier strict build and focus/order probes retained. |
| Expected Markers | Console binding precedes Window activation; transition-only activation through existing FIFO; no speculative text changes. |
| Asset Needs | Refresh fixed softpc32.exe and softpc64.exe; preserve INI/media. |
| Reporting Requirements | Four-entry ledger, peer sweep, line accounting and pushed committed-diff review. |
| Stop Conditions | Escalate MVDM or product policy changes. |
| Exit Criteria | Focus repairs verified, mode-roundtrip defect explicitly transferred, dual EXEs built, committed/pushed and clean worktree for owner testing; T55 stays open. |
| Original Owner Request | “噢好的。请你修复这两点。” Follow-up: “还真是，之前没点过X就没事！” and “请帮我修复，然后收口提交推送这个S任务！” |
| Similar-Issue Sweep | Character/attribute fill and scroll callers; initial/repeated unfreeze and native focus callers. |

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
