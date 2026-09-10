# Project Status

## Current Work

M9 Td S8 is active: source and research policy.

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

## M9 Td S8 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Governance |
| Admission And Approval | Owner approved the NXVM-parity governance route and automatic sequential governance subtasks in this thread on 2026-09-10. |
| Objective | Establish a source/research policy that records provenance facts, prohibits unreviewed external material, and never invents a license conclusion. |
| Non-goals | No source import, license grant, copyright rewrite, MVDM/lib/package change, or release change. |
| Reference Baseline | NXVM `docs/etc/operations/policy/source-policy.md`, read-only on 2026-09-10; current SoftPC source layout, provenance history, and rules. |
| Candidate Proposal | [M9 Td S8 source and research policy](../proposals/m9-td-s8-source-research-policy.md). |
| Files And ABI Surface | `docs/etc/operations/policy/`, its index, `docs/rules/ARCHITECTURE.md`, `docs/states/CURRENT.md`, verifier and task records only; no runtime ABI. |
| Applicable Rules | Documentation and Execution Rules, plus Architecture Rules for the current source-ownership boundary. |
| Verification | Policy structure gate, accepted/rejected fixture, real documentation gate, provenance/reference sweep, and `git diff --check`. |
| Expected Markers | Indexed neutral policy; explicit mirror/shared-corpus/comparison/research categories; external-material stop condition; no invented license assertion. |
| Asset Needs | None. This Td creates no source, binary, media, or package artifact. |
| Reporting Requirements | Report the legal/provenance facts recorded, the intentionally unresolved legal boundary, every enforced rule, and pushed commits. |
| Stop Conditions | Stop for owner direction before asserting distribution rights, adding a license, importing external source/binaries, or changing a copyright notice. |
| Exit Criteria | Policy and concise rule are authoritative, verifier coverage passes, and committed/pushed closure has a clean worktree. |
| Original Owner Request | “接下来，按照nxvm的治理规范，对本项目的治理规范进行审计。目标是本项目应该达到nxvm同等水平以上的治理规范。请你按照以上路线开始。” |
| Similar-Issue Sweep | Sweep source/research/provenance language in root guidance, current rules/design, and historical mirror records; distinguish current authority from retained evidence. |
