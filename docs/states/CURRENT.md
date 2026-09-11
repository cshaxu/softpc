# Project Status

## Current Work

M9 T51 S4 is active: perform whole-corpus verification and T51 closure audit
against the normalized component graph.

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  strict x64/x86 CTest baseline is 35/35 following M9 T49 S6; repository-wide
  CTest is now 36/36 after the Td S7 governance test. Its path-scoped
  standalone MSVC manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |
| T50 | Build presets/artifact identity and standalone shared-library MSVC CI closed. | [T50 S2 history](../history/M9-T50-S2-lib-ci.md) |

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

## M9 T51 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved T51’s full normalization plan and automatic sequential S admission; S2 and S3 are completed and pushed. |
| Objective | Verify the final normalized corpus, execute fresh fixed-width package/test proof, audit changed paths/assets/obsolete paths, and close T51 if every exit condition holds. |
| Non-goals | No behavior, lifecycle, threading, mailbox, MVDM, configuration, media, Linux-support, or compatibility-path change. |
| Reference Baseline | T51 S1 ledger, T51 S2/S3 records, and the normalized current architecture/source-layout authority. |
| Candidate Proposal | [M9 T51 proposal](../proposals/m9-t51-lib-component-normalization.md) |
| Files And ABI Surface | No planned source/API change. Closure evidence may update active state/history and refresh agent-owned x86/x64 package executables. |
| Applicable Rules | `docs/rules/EXECUTION.md`, `ARCHITECTURE.md`, `CODING.md`, and `DOCUMENT.md`; `src/mvdm/softpc.new/` remains immutable. |
| Verification | Manifest, library Linux contract, source-boundary, documentation-governance and old-name gates; fresh x64/x86 package build and all CTest proof; changed-path/asset/MVDM closure audit. |
| Expected Markers | No old component path/target/ABI name or aggregate alias; exact manifest; both fixed-width builds/tests pass; only declared agent-owned executables may change. |
| Asset Needs | Agent-owned `assets/binary/softpc32.exe` / `assets/binary/softpc64.exe` may refresh. `softpc.ini`, media, and MVDM must not change. |
| Reporting Requirements | Record command results, test totals, changed-path accounting, obsolete-path absence, asset scope, MVDM result, and final T51 decision. |
| Stop Conditions | Stop before closure if any test/gate fails, an old-name exception is not historical, a forbidden edge appears, or an undeclared asset/MVDM diff exists. |
| Exit Criteria | All verification passes on the final committed source, T51 history is complete, implementation P is pushed, and the worktree is clean. |
| Original Owner Request | Base becomes types; UI components use `ui-*`; types may be depended on by all components and ui-base only by the two UI leaves. |
| Similar-Issue Sweep | Re-run the S1 full-corpus scan after every `git mv`; inspect all zero-scan exceptions individually. Search for hidden aggregate/alias target names and non-interface private-header crossings in the renamed corpus. |
