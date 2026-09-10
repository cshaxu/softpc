# Project Status

## Current Work

M9 T50 S3 is active: post-library dual-width proof.

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

## M9 T50 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Corrective |
| Admission And Approval | Automatic sequential governance route approved by owner; closure audit found current-source dual-width proof absent after T50 S2. |
| Objective | Re-establish x64/x86 package build and test evidence after the private shared-library atomics portability change. |
| Non-goals | No source, ABI, UX, MVDM, CI, or manual guest-behavior change. |
| Reference Baseline | T50 S1 preset contract; T50 S2 private atomics change; current `CMakePresets.json`. |
| Candidate Proposal | [M9 T50 S3 dual-width post-lib proof](../proposals/m9-t50-s3-dual-width-post-lib-proof.md). |
| Files And ABI Surface | Build trees and agent-owned `assets/binary/softpc32.exe`/`softpc64.exe` only; no source ABI. |
| Applicable Rules | Execution, Coding, Architecture Rules; package-configuration protection. |
| Verification | Fresh preset configure/build/package/CTest for both widths; PE architecture/name check; documentation gate. |
| Expected Markers | `softpc32.exe`, `softpc64.exe`, 32-bit/64-bit PE evidence, full CTest success, unchanged `softpc.ini`. |
| Asset Needs | Existing user-owned package configuration and reusable local media only; do not modify either. |
| Reporting Requirements | Report exact build/test results, artifact widths, configuration preservation, and no manual runtime claim. |
| Stop Conditions | Stop on unavailable matching x86 toolchain, configuration mutation, or build/test failure; do not substitute a different toolchain. |
| Exit Criteria | Both current-source package paths pass their full CTest suites and architecture checks; artifacts and closure evidence are pushed. |
| Original Owner Request | Upgrade this repository's governance quality to NXVM level or higher. |
| Similar-Issue Sweep | Audit every current release preset/package target for declared width, fixed output name, and post-S2 evidence. |
