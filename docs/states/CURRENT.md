# Project Status

## Current Work

M9 Td S7 is active: executable governance state.

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  strict x64/x86 CTest baseline is 35/35 following M9 T49 S6.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |

## Recent Governance

- **M9 Td S6:** Console-object design was promoted to current architecture/UI
  authorities. [Record](../history/M9-Td-S6-console-object-governance.md)

## M9 Td S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Governance |
| Admission And Approval | Owner approved the NXVM-parity governance route in this thread on 2026-09-10; this first bounded task is limited to task/state enforcement. |
| Objective | Make SoftPC's active packet, identifier mode, state relationships, and closure-state checks machine-verifiable. |
| Non-goals | No production/lib/MVDM/package behavior, CI, build-preset, artifact, or source-policy work. |
| Reference Baseline | Current SoftPC governance authorities and NXVM `docs/rules/EXECUTION.md` plus `tools/Verify-DocumentationGovernance.ps1`, inspected read-only on 2026-09-10. |
| Candidate Proposal | [M9 Td S7 executable governance state](../proposals/m9-td-s7-executable-governance-state.md). |
| Files And ABI Surface | `CMakeLists.txt`, `docs/rules/{EXECUTION,DOCUMENT}.md`, `docs/states/{CURRENT,QUEUE,TODO}.md`, `tools/Verify-DocumentationGovernance.cmake`, and task records only; no runtime ABI. |
| Applicable Rules | Documentation and Execution Rules; architecture/coding rules are not applicable because no source or runtime surface changes. |
| Verification | Documentation-governance verifier self-tests; real repository gate; link/structure checks; `git diff --check`. |
| Expected Markers | Fixed `Field | Required record` packet table; `Identifier Mode`; explicit Queue/TODO schemas; deterministic accepted/rejected verifier fixtures. |
| Asset Needs | None. This Td creates no executable or package artifact. |
| Reporting Requirements | Report the accepted schema, every new failure condition, pushed commits, and any governance boundary transferred to the next task. |
| Stop Conditions | Stop and request owner direction if parity requires changing production behavior, historic records beyond compact current state, or external CI/tool installation. |
| Exit Criteria | Rules and verifier enforce the admitted state contract; all self-tests and real gate pass; actual changes are reviewed, committed, and pushed with a clean worktree. |
| Original Owner Request | “接下来，按照nxvm的治理规范，对本项目的治理规范进行审计。目标是本项目应该达到nxvm同等水平以上的治理规范。请你按照以上路线开始。” |
| Similar-Issue Sweep | Sweep every governance state file and the existing CMake verifier for parallel prose-only rules; record each disposition in the closure evidence. |
