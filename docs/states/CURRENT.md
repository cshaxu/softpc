# Project Status

## Current Work

## M9 T99 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner closes T98 and immediately transfers its unimplemented S16 research to new T99 S1. |
| Objective | Establish one responsible owner and completion fact per handoff; identify removable duplicate state/logic, or justify no change. |
| Non-goals | No production repair, new queue/ledger/epoch/recovery framework, speculative performance work, downstream edits or guest changes. |
| Reference Baseline | 8165664a; S15 accepted, tracked and untracked worktree clean before admission. |
| Candidate Proposal | [Public handoff simplification review](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Review public src/test Lib, Emulator, X86 Debug and X86 IBMPC; read actual App callers/fixtures for attribution. Documentation only at admission; no ABI change authorized. |
| Applicable Rules | Execution, Architecture, Coding and Documentation rules and current design authorities. |
| Verification | Five-observation ledger, source/caller traces, existing tests and bounded discriminating probes as necessary; distinguish source proof, dynamic proof and unverified hypotheses. |
| Expected Markers | One input ownership explanation, one completion owner, one Console handoff path; no presumption that an integration failure is a public defect. |
| Asset Needs | None; preserve INI/media and published EXEs. No Computer Use. |
| Reporting Requirements | Per candidate: observed impact, ownership, redundant states/branches, minimal option/no-change option, projected production/test counts and evidence limits. Optional scratch build/t99-s1: 45-minute jobs, investigate after 10-minute no progress, 200MB, retire on delivery. |
| Stop Conditions | Seek approval before production/test repairs, changed pause/input semantics, new state or a broader product investigation. |
| Exit Criteria | All five observations attributed or explicitly unresolved with receiver; concrete simplification proposals and actual-diff review, governance checks, complete research delivery committed/pushed for owner review. |
| Original Owner Request | Bundle the public-component issues, describe observed phenomena, then research where code and mental models can genuinely be simplified and unified. |
| Similar-Issue Sweep | Duplicate input ledgers, request/completion guesses, Console activation/restoration intervention and overlapping startup failure records within the named paths. |

T99 S1 research is delivered for owner review, not implementation approval.
The proposal records all five dispositions and a bounded source/caller review.
Existing cached x64/x86 focused tests pass 7/7 each; no full-suite, desktop,
guest stuck-key reproduction or intermittent-failure repair is claimed.
Production, permanent tests, configuration and EXEs are unchanged. Recommended
next work is small owner-local cleanup and discriminating fixture evidence,
not a new shared recovery model. Pause-release semantics require a decision.
The owner transfers the five related observations out of TODO into T99 S1;
the proposal retains their evidence and is their sole investigation record.
QUEUE has no matching candidate; its product-performance task remains separate.
S15 is owner-closed; see [closure](../history/M9-T98-S15-pause-input-handoff.md).
The owner subsequently authorized deletion of untracked snapshot.debug;
it was removed without changing tracked media or configuration. T98 is now closed;
see the [completion audit](../history/M9-T98-completion-audit.md).

S14 is closed under the owner's authorization to complete self-verification
and close after reconnection; see [closure](../history/M9-T98-S14-kvm-lifetime-failure.md).

S13 remains [closed](../history/M9-T98-S13-directional-sizing.md). T98's four
performance candidates are transferred to a separate unnumbered
[queued proposal](../proposals/m9-runtime-performance-investigation.md);
that performance candidate remains unnumbered and unadmitted.

T98 S3 and S4 remain closed in [S3 history](../history/M9-T98-S3-shared-import.md) and [S4 history](../history/M9-T98-S4-public-api-consolidation.md). Td S22 remains verified awaiting owner acceptance; its [cache governance](../proposals/m9-build-cache-governance.md) delivery f805483f is preserved, not implicitly owner-closed.

## Current Technical Baseline

- Final T98 accepted runtime: edb91df1, reviewed in 453edcfa, closure through
  8165664a. S15 provides passing evidence for all 192 tests per architecture,
  with the disclosed unchanged x64 compact-package supplement. T99 S1 has
  made no production changes; published EXEs remain T98. The entries below
  retain historical per-S qualification, not current active-task status.

- S14 executor 0d9f1803 is pushed and coordinator actual-commit review passes;
  S14 is closed, T98 stays open. Production C/H
  +64/-66 (net -2); test C/H +225/-49 (net +176); test CMake +26/-2.
  Cached x64/x86 builds and final focused tests pass. Final production has
  184/184 background passes on each width. The final test-only held-payload
  assertion supplement passes, followed by all 177 unit/gate registrations
  per width passing while RDP is Disc. After owner reconnection, both widths
  pass five desktop and three native-audio cases, including >5-second worker
  retirement and both package routes. Exact registration/XML equality proves
  192/192 per width with zero failures/errors/skips in the final evidence set.
  Earlier disconnected failures and Active compact stage 1/error 6 remain
  disclosed in the proposal; no causal repair of existing TODO is claimed.
  All owned jobs have exited; scratch is retired after evidence promotion.
  User INI/media/snapshot.debug are unchanged; no App/mirror/X86 changes.

- S13 executor 1e07dcce is pushed and actual-diff review is complete. Private
  snap classification selects fit/width/height in the existing native size
  correction; manual eight-edge behavior is restored. Production C/H +76/-10
  (net +66), test C +108/-4 (net +104). Both EXEs have been rebuilt with cache.
  x86 and final x64 background each pass 184/184, exact registration/XML name
  equality and zero failure/error/skip nodes. Documentation and diff checks pass.
  RDP Active integration rerun executes all 15 per architecture: x86 first-run
  15/15; x64 14/15 plus unchanged isolated compact package pass. The initial
  stage 1/error 6 failure remains existing TODO, not claimed repaired.
  The owner reports manual verification. Actual shell side-snap ordering is
  not independently automated: SendInput still returns zero/error 5 while
  Active. The probe verifies native maximize/restore ordering only; controlled
  and native geometry tests remain distinct evidence. Executor delivery and
  actual-diff review are complete; user INI/snapshot are unchanged.
  Exact registration/XML comparison yields passing evidence for all 192 tests
  on each width, without skips; x64 includes the disclosed isolated supplement.
  Build scratch is retired and residue/documentation checks pass.

- S11 is owner-closed; [closure](../history/M9-T98-S11-window-scale-capture.md).
  Owner diagnostic proves non-edge WM_SIZING/9 is incorrectly promoted to
  terminal failure. S12 handles that native handoff and width-driven sizing.
  Temporary instrumentation is removed before qualification; no extra ratio,
  lifecycle, polling or rendering path is introduced. Final x64/x86 each pass
  all 192 registrations (184 background, five desktop, three native audio),
  exact XML/registration equality and zero skips. Both cached builds are clean.
  Production +18/-21 (net -3), tests +41/-2 (net +39). Initial new fixture errors
  and disconnected native failures are retained in the proposal, not hidden.
  INI/media/snapshot.debug are preserved. Executor commit/review precede owner
  acceptance; S12 is now owner-closed in [history](../history/M9-T98-S12-native-sizing-handoff.md).
  S13 owns the revised nine-case sizing policy. Executor fc2fa687 is pushed and its actual-diff
  review passes; task scratch is retired. Published softpc32 SHA-256:
  9F113DFE380E62F12705BE69A2102A49205B02AEFD9A53956D623660DF527B02;
  softpc64: 71E95D1D90A2828604C1529012488759790076468D8A239422471ADF27E37253.

- S10 is owner-accepted and closed; [closure](../history/M9-T98-S10-utf8-paths.md).
  S11 implements stable Window scale and actual clipped capture bounds in the
  existing Win32 paths. Production +40/-3 (net +37), tests +123/-3 (net +120).
  No public ABI, App, mirror or frame-exchange change. Focused controlled and
  native tests pass on both architectures. Cached dual builds and all 192
  registered tests per architecture have passing evidence with zero skips.
  x64 passes first run; x86 compact package first failed stage 1/error 6,
  then the unchanged desktop group passes 5/5. No repair of that existing
  intermittency is claimed. Executor 1aab6116 is pushed; coordinator actual-diff
  review is complete. S11 is owner-closed; T98 remains open with S12 active.
  softpc32 SHA-256 D1AE94255C19A53217F0E0580308EC79D4C42B4642DB2822225106B9996610FC;
  softpc64: 1696924D7125B779F9C0EB0E1524F658D46715E07637B257BC56810FE357F0A5.
  The reported occasional exit has not been reproduced or attributed; its
  precise remaining investigation is recorded in TODO, not claimed fixed.

- Owner closes S9 and admits S10; [S9 closure](../history/M9-T98-S9-execution-budget.md).
  S10 audit finds COM/LPT still using locale fopen; other owned file entries
  already use Storage. Executor 0151689d is pushed; actual-diff coordinator
  review is complete. Owner-admitted warning/ABI follow-up is implemented and
  verified: bf01eead is pushed and its actual-diff coordinator review passes.
  S10 is now owner-closed; T98 stays open with S11 active.
  Follow-up changes no production code: test +9/-8 and build +2/-0. All 242
  non-mirror C objects per architecture were rebuilt with cache; no non-mirror
  warnings remain. Mirror header warnings are retained by owner instruction.
  Both architectures pass all 192 registrations without test retries this run.
  Current softpc32 SHA-256:
  78FE670609EDAB63B5B6532920C54544D217941C02004CF7F4EABFEDD828F888;
  softpc64: 0A52927A7B91AEE1295928C62E505100BBC97A55A13D55FE40F1C97A1F0F314A.
  Original delivery production +55/-23
  (net +32), tests +136/-29 (net +107); mirror unchanged. Cached dual builds,
  focused 12/12 each and all 192 registrations per architecture have passing
  evidence. x86 compact package first failed stage 1/error 6; the unchanged
  full desktop repeat passes 5/5. This is not a repair of that known debt.
  Native audio passes 3/3 each after reconnection. See the proposal for exact
  qualification, package hashes and pre-existing warning disclosure.

- T98 S9 executor dbff43df is pushed and actual-change review is complete;
  owner has accepted and closed S9. Budget storage and interface are lib_u64 on both widths:
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
  S10's unified UTF-8 file-entry work is now admitted above.

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
  S9 is now closed; S10 is active.
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
  removed by this investigation closure; current candidates are in [Queue](QUEUE.md).

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T98 | Owner closes T98 on 2026-10-22; unimplemented S16 transfers to T99 S1. | [Audit](../history/M9-T98-completion-audit.md) |
| T97 | Owner accepted S1 and T97, 2026-10-18. | [Audit](../history/M9-T97-completion-audit.md) |
| T96 | Owner accepted S8 and T96, 2026-10-17. | [Audit](../history/M9-T96-completion-audit.md) |
| T95 | Delegated closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |

## Recent Governance

- M9 Td S21 approved bounded frame ownership; see the
  [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 completed that implementation; no rule change accompanies T97 closure.
