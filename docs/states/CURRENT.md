# Project Status

## Current Work

M9 T50 S1 is active: build presets and artifact identity.

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

## Recent Governance

- **M9 Td S6:** Console-object design was promoted to current architecture/UI
  authorities. [Record](../history/M9-Td-S6-console-object-governance.md)

- **M9 Td S7:** Active-packet contract, identifier-mode checks, Queue/TODO
  integrity, CTest self-test, and all stale-proposal archival are now
  executable. [Record](../history/M9-Td-S7-executable-governance-state.md)

- **M9 Td S8:** Source provenance, research, legacy-material, and
  license-decision boundaries are now explicit and verifier-backed.
  [Record](../history/M9-Td-S8-source-research-policy.md)

## M9 T50 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner-approved NXVM-parity route; this is the next global implementation task after T49. |
| Objective | Make x64/x86 build, test, and artifact identity reproducible through checked-in CMake Presets. |
| Non-goals | No machine/lib/media/INI behavior or compiler-installation change. |
| Reference Baseline | NXVM `CMakePresets.json` and current SoftPC CMake/package contract. |
| Candidate Proposal | [M9 T50 build presets](../proposals/m9-t50-build-presets-artifact-identity.md). |
| Files And ABI Surface | `CMakePresets.json`, root CMake, README/governance test and task records; no runtime ABI. |
| Applicable Rules | Execution, Documentation, Architecture and Coding Rules. |
| Verification | Fresh preset configure/build/CTest at both widths; mismatch rejection; manifest/gate/diff checks. |
| Expected Markers | `package-x64`, `package-x86`, `test-x64`, `test-x86`, explicit x86 compiler variable, and width validation. |
| Asset Needs | Package EXEs may refresh; `softpc.ini` and media remain untouched. |
| Reporting Requirements | Report compiler inputs, commands, artifact paths, tests, and any host-only prerequisite. |
| Stop Conditions | Stop before altering user INI/media or inventing a compiler distribution path. |
| Exit Criteria | Both checked-in preset paths prove their package identity and pass regression, with a clean pushed closure. |
| Original Owner Request | Upgrade SoftPC governance to NXVM level and follow the admitted route. |
| Similar-Issue Sweep | Sweep build docs, CMake output names, package smoke, and existing toolchain assumptions. |
