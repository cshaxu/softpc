# Project Status

## Current Work

M9 T53 S1 is active: normalize raw-Console logical mouse scale.

## M9 T53 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admitted a new mouse-repair task and authorized implementation, x86/x64 test, package build, commit, and push. |
| Objective | Make one raw Console character-cell movement emit an eight-unit logical relative UI movement on both axes, removing the observed vertical two-cell jump. |
| Non-goals | Do not modify `src/mvdm/softpc.new/`, `ui-window`, the application queue, guest mouse policy, Console capture, or user-owned `assets/binary/softpc.ini`. |
| Reference Baseline | `291afe4` (M9 T51 closed; fixed x64/x86 36/36). |
| Candidate Proposal | [M9 T53 Console Logical Mouse Scale](../proposals/m9-t53-console-logical-mouse-scale.md). |
| Files And ABI Surface | `src/lib/ui-console/win32/component.c`, one focused unit smoke and CMake registration, shared-library manifest, active/closure records, and refreshed fixed package EXEs. No public ABI change. |
| Applicable Rules | [Execution](../rules/EXECUTION.md), [Architecture](../rules/ARCHITECTURE.md), [Coding](../rules/CODING.md), [Documentation](../rules/DOCUMENT.md), [Architecture design](../design/ARCHITECTURE.md), and [UI design](../design/UI.md). |
| Verification | Focused raw-Console mouse conversion smoke; fresh x64/x86 build, CTest, package artifacts, library manifest, documentation-governance, and `git diff --check` gates. |
| Expected Markers | First raw position is zero-relative baseline; one X cell emits `{8,0}`; one Y cell emits `{0,8}`; no presentation font-height constant participates in input conversion. |
| Asset Needs | Refresh only agent-owned `assets/binary/softpc32.exe` and `assets/binary/softpc64.exe`; preserve adjacent `softpc.ini` and all guest media. |
| Reporting Requirements | Record focused/full evidence, exact changed-path accounting, similar-issue sweep, commit IDs, and executable links. |
| Stop Conditions | Stop for any required MVDM, UI-window, application-queue, guest-protocol, capture-policy, or user-configuration change; record it for separate admission. |
| Exit Criteria | Both axes use the fixed logical scale, focused proof and dual-width regression pass, package EXEs are refreshed, all changes are pushed, and a coordinator review closes S1 and T53. |
| Original Owner Request | “raw console的鼠标，横向可以一格一格的移动，但是纵向都是两格两格的移动，咋修复？…没问题，请你帮我修复，准入一个T鼠标修复任务…完成后：测试、提交、推送，并汇报让我使用”. |
| Similar-Issue Sweep | Search raw Console coordinate conversions and all `* 8`/`* 16` occurrences under `src/lib/ui-console`; retain frame/font constants only when they are output presentation, not input conversion. |

## Current Technical Baseline

- The fixed standalone package is `assets/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent user-owned `softpc.ini`; reusable guest media
  is below `assets/media/`, and README captures are below `assets/readme/`.
- `src/mvdm/softpc.new/` is the preserved selected SoftPC baseline. Its ROM
  inputs are embedded source-mirror inputs; no runtime ROM artifact root is
  active.
- `src/lib/` is the canonical shared corpus for exact NXVM adoption. Its
  normalized `types`/`console`/`host`/`storage`/`ui-*` corpus passes 36/36
  fixed x64 and x86 CTest; its path-scoped standalone MSVC
  manifest/build/CTest gate is live in GitHub Actions.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T49 | Shared-library quality sequence S1–S6 closed after owner x86/x64 package acceptance. | [T49 S6 history](../history/M9-T49-S6-lib-neutral-corpus-prose.md) |
| T50 | Build presets/artifact identity and standalone shared-library MSVC CI closed. | [T50 S2 history](../history/M9-T50-S2-lib-ci.md) |
| T51 | Shared library normalized to `types`, `console`, `host`, `storage`, and independent `ui-*` components; dual-width verification closed. | [T51 S4 history](../history/M9-T51-S4-lib-component-normalization-closure.md) |
| T52 | Native Console pointer capture was withdrawn: supported terminal hosts cannot provide a reliable visible client rectangle; no implementation remains. | [T52 record](../history/M9-T52-console-pointer-capture-withdrawn.md) |

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
