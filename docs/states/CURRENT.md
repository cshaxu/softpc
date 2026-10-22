# Project Status

## Current Work

## M9 T98 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts S6 and admits dynamic frame storage on 2026-10-21; audit and concise design precede integrated implementation. |
| Objective | Remove fixed maximum text/graphics backing; allocate actual leaf payloads lazily within the existing four-slot ownership exchange and integrate all producers/consumers. |
| Non-goals | No guest modification, new renderer, polling, recycler thread, timeout recovery, resource framework or writes to other repositories; retain decorations, font banks and latest-wins. |
| Reference Baseline | `1b95cfd7`; [accepted S6](../history/M9-T98-S6-text-decoration.md). User-owned untracked snapshot.debug remains excluded. |
| Candidate Proposal | [T98 dynamic storage](../proposals/m9-nonpublic-quality-performance-audit.md#s7---dynamic-frame-storage). |
| Files And ABI Surface | Lib Base exchange/text schema, KVM leaves, logical Console/broker; Emulator Machine/UI producer contracts; SoftPC Driver/Compat and audited original host text-stride boundary; owner-local tests. |
| Applicable Rules | [Execution](../rules/EXECUTION.md), [Architecture](../rules/ARCHITECTURE.md), [Coding](../rules/CODING.md), [System Architecture](../design/ARCHITECTURE.md), [Source Layout](../design/CODING.md). |
| Verification | Allocation/free byte records, lazy first use, same-spec reuse, all four roles, size/type changes during reads, failure without publication, dynamic dimensions/stride/overflow, decorations and actual producer path; cached dual builds, shared units/gates, App integration and serial desktop tests. |
| Expected Markers | Four metadata slots, no eager four-payload allocation; leaf-owned formats, immutable borrowed frames, single transfer path, no maximum-graphics allocation for text, no stale pointer or silent truncation. |
| Asset Needs | Refresh only `assets/binary/softpc32.exe` and `softpc64.exe` if final production linkage changes. Preserve `assets/binary/softpc.ini`, media, and `snapshot.debug`. |
| Reporting Requirements | Report interface/ownership migration, allocations/reuse/reclamation, callers, native limits and actual production/test/mirror diff. Executor owns disposable build/t98-s7; hidden jobs use log/exit files, 45-minute budget, 10-minute no-progress investigation and 200MB scratch budget; retire after evidence promotion. |
| Stop Conditions | Do not publish incomplete allocations, overwrite readers or invent hardware layouts. Explain any necessary mirror stride repair before changing it; do not rewrite device semantics. |
| Exit Criteria | Verified single-path implementation, dual packages, proportional tests, actual-diff quality review and pushed delivery, then owner acceptance. |
| Original Owner Request | Preserve leaf boundaries and four-slot flow, remove fixed maximum capacities, allocate by actual type/size/use, safely retire obsolete payloads, integrate production and prove nine stated allocation/lifetime scenarios. |
| Similar-Issue Sweep | Fixed row strides, embedded maximum arrays, sizeof-as-payload, shallow copies, cached payload pointers, unchecked dimensions/allocations and destruction with readers. |

T98 S3 and S4 remain closed in [S3 history](../history/M9-T98-S3-shared-import.md) and [S4 history](../history/M9-T98-S4-public-api-consolidation.md). Td S22 remains verified awaiting owner acceptance; its [cache governance](../proposals/m9-build-cache-governance.md) delivery f805483f is preserved, not implicitly owner-closed.

## Current Technical Baseline

- T98 S6 is owner-accepted and closed; see its linked closure record.
  Executor delivery is `8e66d797`; actual-change coordinator review is recorded
  in the S6 proposal. Task scratch is retired; build residue check passes.
  Exact Window underline/two-span cursor and documented native Console
  approximations use original video state through the existing frame path.
  Production +153/-114 (net +39), tests +261/-116 (net +145); the only mirror
  change is vga_prts.c +4/-0 for underline-location refresh. Cached x86/x64
  builds and applicable 184 background plus five desktop registrations have
  passing evidence per architecture, including targeted supplements. Package
  attach/prompt failures also occurred; their intermittent cause remains open,
  not proved fixed by later passes. See the S6 proposal evidence and TODO.
  Three unrelated native-audio integrations were not rerun.
- S6 published softpc32 SHA-256
  EA6786D56FC58D8BD873BAFC5E93C7D51F11BD9D12AF583585488373FD53C824;
  softpc64 SHA-256
  7D137F481626F4904AD33F634328E006D2C9E6662D4BACBD8AA34FF6EFBA6568.
  User INI/media and untracked snapshot.debug remain untouched.

- T98 S4 is owner-accepted and closed: FM's no-consumer placeholder leaf is
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
- T98 S5 is owner-accepted and closed: Console and Window now use
  terminal wake plus actual worker join before disposal; the former 5000ms
  public destroy deadline is removed. Focused x64/x86 lifetime regressions,
  including a synchronized worker hold beyond five seconds, pass. Complete
  non-desktop suites pass except the privilege-required Junction selftest,
  which passes separately elevated. Package smoke fails with `cannot compose
  machine` before monitor startup; the particular initialization cause is not
  independently established by that message.
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
