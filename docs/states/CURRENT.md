# Project Status

## Current Work

## M9 T98 S9 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly admits S9 with a 64-bit budget, zero continuous and all nonzero values finite; build, publish, test, commit and push, then wait for acceptance. S10 design is revised but implementation is not admitted. |
| Objective | Eliminate finite-budget truncation in the existing Machine/Compat execution path, identical on x86 and x64. |
| Non-goals | No public component, mirror, guest, scheduler, snapshot format or file-I/O implementation changes. |
| Reference Baseline | `34e53055`; S8 verified delivery f2677d01 and actual-change review 34e53055. Preserve user INI/media and untracked snapshot.debug. |
| Candidate Proposal | [T98 64-bit budget](../proposals/m9-nonpublic-quality-performance-audit.md#s9---64-bit-execution-budget). |
| Files And ABI Surface | Private Machine run contract, Compat budget declarations/storage, Driver and test callers; existing owner-local regressions. |
| Applicable Rules | [Execution](../rules/EXECUTION.md), [Architecture](../rules/ARCHITECTURE.md), [Coding](../rules/CODING.md), [System Architecture](../design/ARCHITECTURE.md), [Source Layout](../design/CODING.md). |
| Verification | 64-bit boundary decrement, finite completion and zero-budget lifecycle stop; real CCPU entry preserves high bits; cached dual builds, complete background/desktop/native-audio regression. |
| Expected Markers | One lib_u64 budget declaration/storage, no narrowing or old sentinel callers; existing active flag separates infinite mode from finite exhaustion. |
| Asset Needs | Refresh only `assets/binary/softpc32.exe` and `softpc64.exe` if final production linkage changes. Preserve `assets/binary/softpc.ini`, media, and `snapshot.debug`. |
| Reporting Requirements | Report production/test added, removed and net lines, execution semantics and proof limits. Executor owns disposable build/t98-s9; hidden jobs have logs/exit files, 45-minute budget, 10-minute no-progress investigation and 200MB scratch budget; retire after evidence promotion. |
| Stop Conditions | No second counter, execution path, polling, compatibility sentinel or mirror/public ABI changes. |
| Exit Criteria | Verified single-path implementation, dual packages, required tests, pushed executor and actual-diff review, then owner acceptance. |
| Original Owner Request | S9 uses a 64-bit integer throughout: zero means infinite and every nonzero value finite. S10 will unify UTF-8 path contracts through Lib after a full entry-point audit. Admit and execute S9 now. |
| Similar-Issue Sweep | All budget declarations, assignments, decrement and lifecycle consumers, run callers, saved-state and documentation references. |

T98 S3 and S4 remain closed in [S3 history](../history/M9-T98-S3-shared-import.md) and [S4 history](../history/M9-T98-S4-public-api-consolidation.md). Td S22 remains verified awaiting owner acceptance; its [cache governance](../proposals/m9-build-cache-governance.md) delivery f805483f is preserved, not implicitly owner-closed.

## Current Technical Baseline

- T98 S9 executor dbff43df is pushed and actual-change review is complete;
  verified delivery awaits owner acceptance. Budget storage and interface are lib_u64 on both widths:
  zero is continuous, every nonzero value finite. Existing active state and
  original safe-point counting remain; no public component or mirror change.
  Production +14/-20 (net -6); tests +62/-4 (net +58). Cached dual builds and
  focused tests pass. Each full regression has 191/192 first-pass successes;
  the sole compact package attach failure passes in an unchanged isolated run,
  yielding passing evidence for all 192 registrations with no skips. This is
  not an all-green aggregate or a repair of the existing package intermittency.
  Original header warnings remain. User INI/media/snapshot.debug are unchanged.
  softpc32 SHA-256 D10C1108705F623CBA05BE1C9EA130585C76FC30067854AC63F2346D1988F76B;
  softpc64 SHA-256 00FCAD94A0EB8D3CB8FB9A6E3C1F3279025B71F68963F9722D876BA07D5BAF25.
  S10's unified UTF-8 file-entry audit is planned, not implementation-admitted.

- T98 S8 implementation and cached dual builds are verified. Final x64 and x86
  each pass all 192 registrations: 184 background, five desktop and three
  native-audio tests, with exact registration/XML equality and no skips.
  Initial package readiness alone uses a measured 30-second fixture budget;
  product timeouts and subsequent test waits are unchanged. Separate historical
  initialization exits remain disclosed in TODO, not claimed repaired.
  Production +431/-190 (net +241); tests +767/-564 (net +203). Only Lib and
  Emulator src/test public trees change; App integration and one narrow mirror
  host-stride repair (+12/-8) complete the path. Executor f2677d01 is pushed;
  coordinator actual-change review is complete with no delivery blocker.
  Owner authorizes advancement to S9; [S8 closure](../history/M9-T98-S8-dynamic-frame-storage.md)
  records this transition without claiming an additional owner-run test.
  softpc32 SHA-256 E9E574CE63573AB1C8346470528BA70908552D663FC174654813A6B9A3E42BAD;
  softpc64 SHA-256 5012E3AD96F289752AF6681C64A4B4C048F23F25DAF613F68024A4BAC35A88C2.
  User INI/media and pre-existing untracked snapshot.debug are preserved.
- T98 S7 executor ed9bb7b8 is pushed and actual-change review is complete;
  owner accepted it; see the linked closure and proposal verification record.
  Production +16/-7 (net +9); tests +55/-3 (net +52). Only src/lib and test/lib
  of the public eight trees change; App and mirror production are unchanged.
  x86 final 192/192 passes; x64 final 190/192 plus two unchanged isolated passes
  provides passing coverage for all 192 registrations. Intermittent failures
  remain disclosed and deferred, not repaired by this sizing change.
  S9 is now admitted; S10 remains planned.
  softpc32 SHA-256 BEAE1647F8F177123045F99AACE6A6195212A5F89F611C47F57EA18C9A346C5B;
  softpc64 SHA-256 9314E59A4B8143F230E0A2D1833C388BD8A214F72DF0513114149B97DCED9CEB.

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
