# Project Status

## Current Work

M9 T50 S2 is active: shared-library CI.

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  strict x64/x86 CTest baseline is 35/35 following M9 T49 S6; repository-wide
  CTest is now 36/36 after the Td S7 governance test.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |

## Recent M9 Task Progress

- **M9 T50 S1:** Checked-in x64/x86 presets now bind compiler width to package
  identity and passed fresh 36/36 CTest at both widths.
  [Record](../history/M9-T50-S1-build-presets-artifact-identity.md)

## M9 T50 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Automatic sequential governance route approved by owner; T50 S1 is closed. |
| Objective | Add a path-scoped standalone shared-library CI gate. |
| Non-goals | No full VM/package CI or runtime behavior change. |
| Reference Baseline | NXVM `.github/workflows/lib.yml` and SoftPC `src/lib/CMakeLists.txt`. |
| Candidate Proposal | [M9 T50 S2 lib CI](../proposals/m9-t50-s2-lib-ci.md). |
| Files And ABI Surface | `.github/workflows/lib.yml`, task records only; no ABI. |
| Applicable Rules | Execution, Documentation, Architecture Rules. |
| Verification | YAML review; standalone lib configure/manifest/build/CTest locally; governance gate. |
| Expected Markers | `src/lib/**` paths, manifest target, build and CTest commands. |
| Asset Needs | None. |
| Reporting Requirements | Report trigger scope and exact CI proof. |
| Stop Conditions | Stop before adding VM/package CI or changing source. |
| Exit Criteria | Workflow is committed/pushed and standalone library proof passes. |
| Original Owner Request | Upgrade governance to NXVM level. |
| Similar-Issue Sweep | Compare all existing CI/workflow surfaces and standalone lib targets. |

## Recent Governance

- **M9 Td S6:** Console-object design was promoted to current architecture/UI
  authorities. [Record](../history/M9-Td-S6-console-object-governance.md)

- **M9 Td S7:** Active-packet contract, identifier-mode checks, Queue/TODO
  integrity, CTest self-test, and all stale-proposal archival are now
  executable. [Record](../history/M9-Td-S7-executable-governance-state.md)

- **M9 Td S8:** Source provenance, research, legacy-material, and
  license-decision boundaries are now explicit and verifier-backed.
  [Record](../history/M9-Td-S8-source-research-policy.md)
