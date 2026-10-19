# Project Status

## Current Work

M9 T98 S3 is verified, preparing pushed-change review: exact shared import and full
qualification. See the [proposal](../proposals/m9-nonpublic-quality-performance-audit.md).
The former S3/S4 repairs move to S4/S5 and are not admitted. Td S22 remains
verified awaiting owner acceptance; its [cache governance](../proposals/m9-build-cache-governance.md)
delivery f805483f is preserved, not implicitly owner-closed.
User-owned untracked assets/binary/snapshot.debug remains untouched/excluded.

Both packages are freshly linked in the persistent cached trees. Each width
passed 194/194 registered tests: 119 shared unit/gates, 59 other unit/gates,
eight background integrations, five desktop and three native audio. All 298
shared files match NXVM; no root helper import. Review scratch is owned until
actual-change review, then removed before final owner handoff.

## M9 T98 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 准入一个s任务完成原样导入并构建双exe并跑全量测试. Explicitly adds shared import ahead of the remaining private repairs. |
| Objective | Import the audited NXVM eight shared trees exactly; build/release both SoftPC packages and pass every registered test in both architectures. |
| Non-goals | No root NXVM test-helper import, runtime feature change, original mirror edit, new framework, cache reset or owner INI/media/snapshot mutation. No S4/S5 admission or T98 closure. |
| Reference Baseline | SoftPC f805483f; NXVM worktree at b2f8b93112897529c0437f37e26c2a70e4b87cb9, shared math delivery 9b41a82ca, no shared-tree uncommitted changes. |
| Candidate Proposal | [T98 owner-directed S3 import](../proposals/m9-nonpublic-quality-performance-audit.md#s3---owner-directed-exact-shared-import). |
| Files And ABI Surface | Exactly src/test lib, emulator, x86-debug and x86-ibmpc; expected six changed Lib paths. Optional CRT header only, no existing ABI change. Task records plus assets/binary double EXE. |
| Applicable Rules | Execution, Document, Architecture, Coding and their governance skills; single-person executor/coordinator review; no Computer Use. |
| Verification | Byte/hash identity of all eight trees, manifest/LF and Types/dependency/naming/test gates; cached x64/x86 builds and dual package publication; unit/gates then background integration, serial desktop and native audio; exact XML/registration coverage. |
| Expected Markers | Only four optional ISO C math aliases added; no importer build/runtime dependency on NXVM; all registered cases execute, with failures/skips reported honestly. |
| Asset Needs | Owned build/t98-s3, 45-minute overall/10-minute no-progress/80-MiB logs budget. Existing .ninja/.ccache retained. Native desktop/audio tests are preauthorized; no guest edits. |
| Reporting Requirements | Actual source/test/config/doc and package diff, fixed source pin/eight manifest hashes, exact per-phase counts, native capability limits, binary hashes, review and commit evidence. |
| Stop Conditions | Source drift, unrelated overlapping changes, active product handles blocking replacement or native environment failures must be investigated, not hidden by retries, skips, code weakening or killing unrelated jobs. |
| Exit Criteria | Exact import, both freshly linked packages, all registered tests passing, actual pushed-change review and final scratch cleanup; then wait for owner acceptance. |
| Original Owner Request | 准入一个s任务完成原样导入并构建双exe并跑全量测试. |
| Similar-Issue Sweep | All eight path sets and bytes; unchanged six trees; Lib Types aliases/optional math linkage; root helper differences reviewed but deliberately not imported; protected package/media hashes. |

## Current Technical Baseline

- T98 S3 optional Types math import: public source +12/-0, test C +10/-0,
  CMake +3/-0, docs +5/-0 and manifests +4/-3; total +34/-3 (net +31).
  No existing ABI or product/mirror implementation changes. All 194 tests
  pass per architecture with exact XML/registration equality and zero skips.
- S3 packages: softpc32 SHA-256
  704D873D121307CDAB765454C4CAF388C6AD221208B5FE12722D016C566C3873;
  softpc64 SHA-256
  78A32B672B1A2D49BF742C99EA6B2647FC364717B7BE4414CB73CFF54D0E4838.
- T98 S2 fixes Compat media close reporting and address bounds only. Production
  +30/-16 (net +14); tests +195/-0; root test registration +3/-2. Public and
  mirror code unchanged. Historical qualification: build/t98/s2/qualification,
  exit 0; raw logs retired by Td S22 after preserving the checkpoint;
  x64/x86 each 177 unit/gates, eight background integrations, five desktop and
  three native audio, exact XML/registration match. Existing Ninja recovery
  and original compiler warnings remain; no warning-removal claim is made.
- Historical accepted S2 packages: softpc32 SHA-256
  4E67DBD9AAEBE99BB874BA77E0BFA7DBE7819DE5927D920BE4998769089231AC;
  softpc64 SHA-256
  5C692E0DE7ADFDF9D1AEF2446D90C7C4B733FD3ECA589B94CBA33D2E89D216A8.
- T97 is investigation/documentation only; production, tests and source mirror
  have no changes. Installed Win95 driver rejects mixer/volume capabilities for
  DSP 2.01. The observed hardware-change confirmation explicitly requests
  shutdown; normal Restart succeeds twice. The owner's original installation
  dialog was not recorded, so its exact wording/action remains unverified.
- T96 runtime/reference: 5ea4e55e; S8 delivery a404740f and review 5ea4e55e.
  All 191 tests per architecture passed with zero failures/skips: 175 unit/gates,
  eight background integrations, five desktop and three native audio.
  Evidence: build/t96/s8/qualification, qualification.exit=0; exact CTest/XML
  registration equality was checked. Existing compiler/Ninja warnings remain.
- Previously accepted T96 package reference, without a T97 rebuild:
  softpc32 SHA-256 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A;
  softpc64 SHA-256 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
- Current transport: RGB graphics, separate leaf text contracts, Machine-owned
  fixed four-slot exchanges shared through opaque Base mechanics; Driver owns
  readiness and Machine publishes every produced frame. Session/UI route
  references. See current architecture and T96 audit for accounting.
- Win3.1 verified MIDI configuration is MPU330/IRQ7, SB220/IRQ5, all sixteen
  Mapper channels to MPU. [Product UI](../design/UI.md#windows-31-midi-playback)
  owns instructions. No FM or SB-MIDI capability was added.
- Existing [TODO](TODO.md) items remain open. No unrelated debt is repaired or
  removed by this investigation closure; the [Queue](QUEUE.md) is empty.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T97 | Owner accepted S1 and T97, 2026-10-18. | [Audit](../history/M9-T97-completion-audit.md) |
| T96 | Owner accepted S8 and T96, 2026-10-17. | [Audit](../history/M9-T96-completion-audit.md) |
| T95 | Delegated closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |

## Recent Governance

- M9 Td S21 approved bounded frame ownership; see the
  [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 completed that implementation; no rule change accompanies T97 closure.
