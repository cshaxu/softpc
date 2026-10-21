# Project Status

## Current Work

## M9 T98 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner requests a small API consolidation across the public eight-component corpus: remove unused FM, internalize staged Emulator Product composition, and remove the over-wide frame-exchange locked-pointer API without changing product behavior. |
| Objective | Remove no-consumer capability and public API that exists only for tests or exposes unnecessary ownership details, while retaining one correct production path. |
| Non-goals | Do not reduce Linux/POSIX support; alter x86 DEBUG/xasm32; alter four-slot exchange/latest-wins/continuous production; alter Console switching, Storage, PCM/MIDI, App, mirror, user INI/media/snapshot; add compatibility aliases, test-only production APIs, a second path, a runtime switch, or a framework. |
| Reference Baseline | `9ebb9887`; eight shared trees exactly match the latest NXVM import at S3. User-owned untracked `assets/binary/snapshot.debug` is excluded. |
| Candidate Proposal | [T98 audit](../proposals/m9-nonpublic-quality-performance-audit.md), amended by this owner admission. |
| Files And ABI Surface | Expected: `src/test/lib`, `src/test/emulator`, their CMake/README/manifests, and current-state/proposal records. Public removal is allowed only for FM and obsolete staged/locked-pointer APIs after whole-tree caller proof. A scoped callback observation replaces `lock_latest/unlock`; Session serves its real metadata need from its Machine cache, and no caller receives a retained exchange pointer. |
| Applicable Rules | [Execution](../rules/EXECUTION.md), [Architecture](../rules/ARCHITECTURE.md), [Coding](../rules/CODING.md), [System Architecture](../design/ARCHITECTURE.md), [Source Layout](../design/CODING.md). |
| Verification | Before edits, scan all tracked source/test/CMake/docs callers. Build x64/x86 affected targets; run complete public-component unit/gate suites and focused frame/product tests; build and publish both EXEs; run required package/integration regression proportionate to the frame/product ABI change. Reconcile manifests, dependency/naming/LF gates and actual diff. |
| Expected Markers | No `audio/fm` source/test/CMake/manifest/README/architecture references; no staged composition functions in the public header; no public `lock_latest/unlock`; no public consumer obtains a frame pointer while an exchange lock is held. |
| Asset Needs | Refresh only `assets/binary/softpc32.exe` and `softpc64.exe` when the final package link changes. Preserve `assets/binary/softpc.ini`, media, and `snapshot.debug`. |
| Reporting Requirements | Report actual callers and disposition before deletion; production/test/build-doc added/removed/net lines; retained needed production behavior; test proof versus source proof; fixed commit and any non-eight-tree support change required for NXVM import. |
| Stop Conditions | Stop an individual deletion if a real production caller or contractual consumer is found and no smaller single-path replacement preserves behavior. Do not replace it with a compatibility alias. |
| Exit Criteria | FM is either removed after no-caller proof or explicitly retained with proof; composition has only `emulator_product_run` public; frame observation no longer exposes manually held exchange locks and all affected tests use the narrowed observation contract; builds/tests/manifests/gates pass, packages are refreshed, actual diff reviewed, one complete P is pushed, worktree is clean except the protected snapshot. |
| Original Owner Request | “对公共八组件进行一次‘小而彻底’的 API 收紧；目标是删除无消费者能力与仅为测试暴露的生产接口”，with the three explicitly supplied S1/S2/S3 scopes and verification requirements. |
| Similar-Issue Sweep | Search the entire repository for FM contracts, staged composition APIs, frame-exchange lock exposure, other test-only public APIs in the affected headers, and all pointer-under-lock consumers. Each hit receives an in-scope removal/replacement/retention disposition. |

T98 S3 remains closed in [S3 history](../history/M9-T98-S3-shared-import.md). Td S22 remains verified awaiting owner acceptance; its [cache governance](../proposals/m9-build-cache-governance.md) delivery f805483f is preserved, not implicitly owner-closed.

## Current Technical Baseline

- T98 S4 is awaiting owner acceptance: FM's no-consumer placeholder leaf is
  removed; the staged Product assembly functions are implementation-private;
  and frame exchange no longer exports manually acquired locks. Machine caches
  publication metadata for Session, while bounded observers cannot retain the
  exchange pointer. Production +55/-81 (net -26), tests +94/-91 (net +3), and
  docs/build +45/-17 (net +28), excluding rebuilt packages. Both architectures
  built successfully. Each non-desktop/non-native-audio suite passed 184/185
  normally; the sole `checks/build_cleanup/selftest` requires a Junction and
  passed separately under its required elevated host permission, yielding all
  185 qualified tests per architecture. Build residue check passed.
- Pending S4 packages: softpc32 SHA-256
  EDE3E5A8F89A5064E1B93D347BCCF76CDFC58FC2E936B6880358C54019022174;
  softpc64 SHA-256
  2AF3E37332AD9DC982A6AAD6D41B1E488868DD9F4716296DAC63D2A65FEC80CE.
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
