# Project Status

## Current Work

M9 T51 S2 is active: make the shared-library and every active SoftPC consumer
rename in one atomic, buildable migration.

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

## M9 T51 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved T51’s full normalization plan and automatic sequential S admission; S1 ledger closure found one atomic migration is required to preserve a buildable root tree. |
| Objective | Atomically rename the library to `types`, `console`, `host`, `storage`, `ui-base`, `ui-window`, and `ui-console`, and migrate every frozen SoftPC consumer, target, include path, and public `ux_*` ABI spelling. |
| Non-goals | No behavior, lifecycle, threading, mailbox, MVDM, package, media, user `softpc.ini`, or Linux-support change; no compatibility alias, forwarding header, or duplicate old path. |
| Reference Baseline | T51 S1 frozen ledger and closed T50 x86/x64 package proof; current source is the old `base`/`ux-*` corpus. |
| Candidate Proposal | [M9 T51 proposal](../proposals/m9-t51-lib-component-normalization.md) |
| Files And ABI Surface | All 72 `src/lib` files, `src/app`, `src/host`, enumerated tests, root/library CMake, library manifest, source-boundary verifier, and the documentation-governance continuation self-test; public include paths and `ux_*` names intentionally become `types`/`console`/`ui-*`. |
| Applicable Rules | `docs/rules/EXECUTION.md`, `ARCHITECTURE.md`, `CODING.md`, and `DOCUMENT.md`; `src/mvdm/softpc.new/` remains immutable. |
| Verification | `git mv`-based path audit; standalone library manifest build; root build/configure; old-name zero scan over the ledger scope; focused library/component tests and source-boundary gate. |
| Expected Markers | No old directory, target, public include path, `ux_*` public ABI, or aggregate alias remains in the active corpus; root CMake names direct normalized targets. |
| Asset Needs | Build output beneath owned `build/` paths and agent-owned `assets/binary/softpc32.exe` / `assets/binary/softpc64.exe` may refresh. `softpc.ini` and guest media are not modified in S2. |
| Reporting Requirements | Record changed source/test paths and line accounting; identify each deleted aggregate/old path, direct app ABI consumer, remaining target link, and any migration discovery. |
| Stop Conditions | Stop before behavior/API move if an undisclosed consumer, cycle, MVDM touchpoint, or public binary compatibility commitment is found. |
| Exit Criteria | Every S1-ledger member uses the normalized path/name or is removed, the root and standalone library configure/build, no old path or target alias remains, focused tests/gates pass, and implementation P is pushed for coordinator review. |
| Original Owner Request | “base should become type(s); ux components should become ui components; types may be depended on by all components and ui-base only by the two UI leaves.” |
| Similar-Issue Sweep | Re-run the S1 full-corpus scan after every `git mv`; inspect all zero-scan exceptions individually. Search for hidden aggregate/alias target names and non-interface private-header crossings in the renamed corpus. |
