# Project Status

## Current Work

M9 T51 S1 is active: freeze the exact shared-library component-normalization
ledger and public dependency contract before any source move.

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

## M9 T51 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner approved T51 admission, Queue insertion, and S1 execution on 2026-09-10. |
| Objective | Freeze the complete migration ledger and authoritative component graph for `types`, `console`, `host`, `storage`, `ui-base`, `ui-window`, and `ui-console`. |
| Non-goals | No source move, public API rename, runtime behavior, MVDM, package, media, or `softpc.ini` change in S1. |
| Reference Baseline | Closed T50 final source proof; current lib graph is `base -> ux-base + host + storage`, `ux-base -> ux-window + ux-console`, with legacy CMake target aggregates. |
| Candidate Proposal | [M9 T51 proposal](../proposals/m9-t51-lib-component-normalization.md) |
| Files And ABI Surface | Proposal, Queue, Current packet, indexed T51 ledger, and their documentation index; `src/lib/CMakeLists.txt` and public interfaces are read-only inventory inputs in S1. |
| Applicable Rules | `docs/rules/EXECUTION.md`, `ARCHITECTURE.md`, `CODING.md`, and `DOCUMENT.md`; `src/mvdm/softpc.new/` remains immutable. |
| Verification | Reproducible `rg` inventory of old source/build names; CMake documentation-governance gate; review that every ledger member has a disposition and no implementation occurred. |
| Expected Markers | Frozen path/target/API ledger, approved component DAG, explicit external-app exception for public `ui-base` ABI, and prohibited-old-name scan scope. |
| Asset Needs | None. No build output or package artifact is admitted. |
| Reporting Requirements | Report each path class, all direct SoftPC public-ABI dependencies, target-link changes planned for S3, and any discovery requiring proposal revision. |
| Stop Conditions | Stop before any behavior/API move if the inventory discovers a cycle, a non-SoftPC consumer, a public binary compatibility commitment, or a change touching MVDM. |
| Exit Criteria | The ledger is complete and reproducible, the authoritative graph is updated without duplicate rules, governance verification passes, and S1 is committed/pushed with coordinator review. |
| Original Owner Request | “base should become type(s); ux components should become ui components; types may be depended on by all components and ui-base only by the two UI leaves.” |
| Similar-Issue Sweep | Search all tracked `src/lib`, `src/app`, `src/host`, `test`, root CMake, and library CMake paths for old include, target, and identifier forms; record every hit rather than treating a directory rename as complete. |
