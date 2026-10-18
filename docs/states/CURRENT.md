# Project Status

## Current Work

T96 remains open. Owner accepted and closed S7 after the public-test governance
delivery and actual-commit review. See the
[S7 closure](../history/M9-T96-S7-frame-and-test-governance-closure.md).
Owner admits T96 S8: reproduce and repair Win3.1 MPU-401 installation,
post-restart mouse loss and silent MIDI. Guest A/B evidence establishes an
IRQ9 conflict with InPort and an incorrect MIDI Mapper destination. Corrected
disposable guest playback is verified on both widths and audible on x64.
S8 is verified, pending executor delivery/review and owner acceptance.

S1-S7 are complete. Before whole-T closure, reconcile the final RGB/schema and
four-slot ownership contracts, final fixed-frame storage accounting, downstream
adoption instructions and qualification evidence against the original request.
The Win3.1 prompt-roundtrip readiness investigation remains explicitly deferred
in [TODO](TODO.md); passing later runs do not establish its repair.
User-owned untracked assets/binary/snapshot.debug remains untouched and excluded.

## M9 T96 S8 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly admits a new S to reproduce and repair MIDI after Roland MPU-401 installation; T96 remains open. |
| Objective | Establish the installation error, mouse regression and silent MIDI causes against the previously working Canyon route; minimally repair the responsible path and prove mouse/MIDI/WAV coexistence. |
| Non-goals | No FM synthesizer, second audio route, unrelated frame redesign, user-media rewrite or automatic T closure. |
| Reference Baseline | 5e76dd49; T95 recorded successful MPU Canyon followed by WAV, but did not prove this new installation sequence or mouse coexistence. |
| Candidate Proposal | [T96 ledger](../proposals/m9-lib-rgb-graphics-frame.md); [prior MIDI evidence](../history/M9-T95-win31-midi-wav-playback-proposal.md). |
| Files And ABI Surface | Investigate App-SoftPC original MPU/InPort/PIC paths, Compat MIDI and machine integration tests; select minimal production changes only after evidence. No public ABI change planned. |
| Applicable Rules | Execution, Document, Architecture and Coding; single-person executor/coordinator actual-diff review. |
| Verification | Disposable guest replay from available checkpoints, installation/restart and MIDI Mapper inspection, IRQ/vector/mouse and complete MIDI-message probes; deterministic owner-local regression plus cached x86/x64 builds, full unit/gates and applicable full integration including native audio/desktop. No Computer Use. |
| Expected Markers | One existing device/output path; music messages rather than only reset messages; mouse and keyboard survive restart and playback; subsequent WAV completes. |
| Asset Needs | Preserve owner INI, media and snapshot.debug. Use task-owned disposable copies/overlays and bounded probes under ignored build/t96/s8; refresh only dual EXEs upon verified code repair. |
| Reporting Requirements | Distinguish source deductions, guest reproduction and host output proof. Initial production estimate deferred until cause is located; report proposed and actual added/deleted/net lines before and after repair, including mirror diff if touched. |
| Stop Conditions | Public ABI redesign, new device capability or unrelated mirror change requires scope revision. Do not substitute arbitrary IRQ changes or skipped native tests for diagnosis. |
| Exit Criteria | Reproduced cause chain, minimal repair or evidenced configuration correction, similar-class sweep, dual-width proof/artifacts, actual-diff review, commit/push and owner acceptance. |
| Original Owner Request | 安装了Roland MPU-401 MIDI以后，出现一个错误提示但是仍然允许我继续选择 330h + IRQ 2/9，我都点了，重启系统，发现鼠标无法使用但是键盘正常；然后打开mplayer播放midi，没有听到任何声音。准入一个新的S任务来复现和修复这个MIDI问题；BTW之前你在S1里面调研的时候我确实听到过播放的canyon MIDI音乐 |
| Similar-Issue Sweep | MPU/InPort IRQ ownership, PIC vector/mask/EOI and reset/load, guest Mapper route, host acquisition/message status; each hit receives an explicit disposition, not a second implementation. |

## Current Technical Baseline

### S8 MIDI configuration qualification

- Correct configuration is MPU330/IRQ7, SB220/IRQ5 and all sixteen Mapper
  ports routed to Roland MPU-401; [Product UI](../design/UI.md#windows-31-midi-playback)
  owns the instructions. IRQ9 conflicts with the existing InPort mouse.
- Both widths complete Canyon, subsequent Chord, mouse movement and Windows
  exit/re-entry using disposable overlay snapshots. Owner confirms audible
  Canyon on x64. First-install warning is observed before resource selection;
  no recurring warning after correct configuration, but no driver-internal
  branch disassembly or new MPU hardware-fix claim.
- Cached dual builds and all 191 distinct tests per architecture pass, no
  failures/skips: 175 unit/gates, eight background integrations, five desktop
  and three native audio. Evidence: build/t96/s8/qualification; exit=0.
- Production/test/mirror diff +0/-0. Source and public manifests unchanged.
  x86 Ninja log recovery rebuilt/published the EXE; unchanged x64 stays current.
  Existing warnings remain disclosed. SHA-256: softpc32
  6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A;
  softpc64 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
- User INI, media and snapshot.debug remain untouched. Probe guests have exited;
  corrected overlay checkpoint is retained under the task evidence directory.

### S7 public-test governance

- Against 56f65ece: no production changes. Test C/H +86/-189, net -103:
  Lib +75/-115, Emulator +1/-50, X86 IBM PC +10/-24; X86 Debug unchanged.
  Test CMake +46/-40, net +6. Three duplicate targets removed with assertion
  receivers recorded in the proposal; new Base validation/failure/allocation
  assertions and diagnostic-specific negative fixtures retain meaningful proof.
- Cached dual builds complete. Each architecture passes focused 25/25 and
  all 191 distinct registered tests: 175 unit/gates, eight background
  integration, five desktop and three native audio; zero failures/skips.
  Evidence: build/t96/s7/test-governance/qualification.log and XML files;
  qualification.exit=0. No qualification retry or test-budget change.
- All 289 public manifest entries, documentation and whitespace checks pass.
  No shared source or root test infrastructure changed. Existing mirror/ABI
  warnings and x86 Ninja log recovery remain, not a zero-warning claim.
- Package hashes: softpc32
  D16647A87671559B1D114AD282E904529615DB25267D91640D72039758D5297B;
  softpc64 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
  x86 rebuilt after Ninja recovery; unchanged x64 production target remains
  up to date and its hash is unchanged. User snapshot.debug stays untouched
  and excluded. Coordinator review found no blocking issue in the admitted
  ledger. Owner has now accepted and closed S7.

### S7 active-extent and font readiness follow-up

- Against 104bab46, production C/H +16/-16, net zero in Lib Console; tests
  +57/-0, net +57 (Lib +28, App-SoftPC +29). No API, exchange algorithm,
  mirror, INI, media or snapshot change. Conversion visits active cells only;
  real sequencer writes prove font-only updates reach Driver produced.
- Cached x64/x86 builds publish both EXEs. Per architecture: focused 18/18,
  unit/gates 178/178, background integration 8/8, native desktop 5/5 and
  native audio 3/3 pass. All 194 distinct registered tests pass without skips
  in the corrected qualification; qualification.exit=0. Logs and XML live
  under build/t96/s7/extent-font. Initial focused test scheduling failure
  (two ticks instead of the full mode-selection plus flush cycle) is retained
  separately; no production workaround or relaxed assertion was added.
- All 292 shared manifest entries match. Documentation and whitespace gates
  pass. Existing mirror/test ABI warnings and x86 Ninja log recovery remain;
  this is not a zero-warning or measured-performance claim.
- Published SHA-256: softpc32
  42EF30B5984A1E5B0B12C845E2F3F7F59BCDA54A4A56E925124B54032BAC5AC3;
  softpc64 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
  User-owned untracked snapshot.debug remains untouched and excluded.

### S7 fixed four-slot revision

- Against d24ab19c, production C/H +105/-89, net +16: Lib +57/-47,
  Emulator +7/-42, App-SoftPC +41/-0. Tests +175/-119, net +56:
  Lib +119/-82, Emulator +19/-37, App-SoftPC +37/-0. Documentation,
  manifests and EXEs are separate. No mirror, INI, media or snapshot changes.
- Base owns one four-slot exchange, without a three-slot variant. Both
  consumers retain processing and displayed inputs until finish. Machine
  publishes every produced frame; SoftPC alone decides text readiness.
  Console compares mapped held cells; broker no longer retains cell arrays.
  Native invalidation overrides unchanged cells, including palette-induced
  shrink and partially failed output. Window's RGB dirty-area path is retained.
- Fresh dual-width focused 32/32, unit/gates 178/178 and background integration
  8/8 pass per architecture. All eight x86 native tests pass. x64 seven native
  cases pass; compact package fails before the tested handoff at stage=1/error=6.
  That existing startup TODO is not repaired by this frame work. Its independent
  unchanged recheck passes; final x64 header-refresh build and all 178 unit/gates
  pass again. All 194 distinct tests per architecture have passing evidence,
  not an all-green first aggregate. qualification.exit=1 and final-check.exit=0
  are retained under build/t96/s7/four-slot. No tests were skipped.
- Published SHA-256: softpc32
  532816D4D5313CC1090866A2480543609C09F8BB6431C7FB0A94A13DC0EBF0A2;
  softpc64 877CDB10D4ED1D60205C9186D193120FDCBB1A1C984644EB779B4101A257F28F.
- All 292 shared manifest entries match. Original mirror warnings and local
  x86 Ninja-log recovery remain disclosed. User snapshot.debug is preserved
  and excluded; no zero-warning, measured speedup or manual acceptance claim.

### S7 retained-Console follow-up

- Against aa8f75c3, production C/H +49/-10, net +39; tests C/H +154/-10,
  net +144, plus test CMake +1/-0. Only Lib production/tests change; no
  Machine/Session, mirror, Compat, INI, media or snapshot-format changes.
- Both architectures rebuilt and published. Each has passing evidence for all
  194 distinct registered tests: 178 unit/gates, eight background integration,
  five desktop and three native audio; focused 25/25 also pass each width.
  Evidence: build/t96/s7/console-retained/{unit,integration,focused}-*.xml and
  native-*-0..7.xml. No skipped tests. qualification.exit remains 1 because
  x64 compact package first failed at child-startup stage=1/error=6; unchanged
  independent package-recheck.xml passes (exit 0). This does not fix or close
  the existing startup TODO. All x86 cases and other x64 cases passed first run.
- Native retained-frame proof publishes once, then performs repeated narrow
  raw/cooked/raw activation; cells, attributes, cursor and cooked saved state
  pass without another producer commit. Ordinary native no-rewrite retention
  and deliberate-resize repair remain separately checked.
- Deployed SHA-256: softpc32
  C3BA854432D6575FF122A3EB6BD77CA12DB4B179BE0E63145D26F6699A9431C2;
  softpc64 2C6AFAD676F04CE64EB7DDA88CAA00EA5A637C04227FE06E6A8F7C0A0A21533B.
  Existing mirror warnings and x86 Ninja-log recovery remain disclosed.
  User-owned untracked snapshot.debug is preserved and excluded.

### Original S7 delivered checkpoint

- S7 production C/H +237/-298, net -61; tests C/H +749/-671, net +78.
  Component/path accounting and bounded copy audit are in the proposal.
  The Machine envelope is deleted; Window's sequence and mutually exclusive
  payload pointers use slot-owned backing. Machine owns both exchanges and
  Session/UI route references; Console maps never enter Window graphics.
- Final evidence covers all 194 distinct registered tests per architecture,
  zero skips/failures: 178 unit/gates in build/t96/s7/final1/unit-*.xml;
  eight background integrations in final-check/integration-*.xml;
  five desktop and three native audio in final1/native-*-0..7.xml.
  Both 13-test focused sets pass. Final-check qualification.exit is 0.
- Earlier final1 and recheck exits remain 1: intermittent x86 Win3.1
  prompt-roundtrip failures were followed by three unchanged independent
  passes, then one aggregate failure. Added failure-stage/frame diagnostics;
  subsequent probe and final-check integrations pass. This is not a proven
  repair of that intermittent case; TODO records the investigation boundary.
  Native deliberate-viewport narrowing also has a separately recorded finding.
- Both EXEs were rebuilt/deployed from the current production code. SHA-256:
  softpc32 1744CB9D976FD714A2FB425B85AFD54FA8232017AFF32B8EC8B5686BFAB7B9DE;
  softpc64 69622273FFE889C7F9C035DBE42CCED900FDF5F670075E537849A21253291D17.
  Later failure diagnostics change only the integration test executable.
- All 292 shared manifest entries match. Documentation gate and diff whitespace
  check pass. Existing mirror warnings, unchanged VGA test casts and x86 Ninja
  log recovery remain disclosed; no zero-warning or speedup claim.
- No mirror/Compat, Base exchange algorithm, INI, media or snapshot-format
  changes. Owner-owned untracked assets/binary/snapshot.debug is excluded.
  This earlier checkpoint is superseded by the accepted S7 qualification above.

### S5 delivered checkpoint

- S5 implementation accounting: production +385/-196, net +189; tests/build
  checks +553/-270, net +283. Full path ledger is in the active proposal.
  One four-slot Window exchange and three-slot Console exchange replace copied
  transport; native surface, original mirror, Compat and App production remain.
- Final x86 evidence: build/t96/s5/full1/unit-x86.xml (178/178),
  integration-x86.xml (8/8), native-x86-0..7.xml (8/8), no skips.
- Final x64 evidence: build/t96/s5/final-x64/unit.xml (178/178),
  integration.xml (8/8), and native-reconnected-x64/native-0..7.xml (8/8).
  Reconnected native qualification.exit is 0; session query reports Active.
- Earlier full1 exit 1 retains intermediate stale gate/manifest failures;
  final-x64 exit 1 retains three native audio and two package failures while
  the user session was disconnected. The unchanged connected rerun passes
  all eight; no production workaround, assertion weakening, skips or timeout
  termination was used. This supports an environmental dependency, not proof
  of every earlier failure's internal cause.
- S5 deployed EXEs: softpc32 SHA-256
  1905A15F29C309B416922E99FD4A0C928EC1CCB70E8D0DE0FD30CA5F043F44CA;
  softpc64 461EAF8CAFA353106BFDDF5EBE592C51871C8F3C213A0E67DD4101F289787AD6.
  Both rebuilt artifacts have completed final qualification.
  User INI/media and untracked assets/binary/snapshot.debug remain untouched.

### Prior T96 qualified checkpoints

- S3 audits the unchanged S2 corpus at 94a0debd: no residual indexed graphics
  route found in the bounded path; all 288 manifest entries match. Exact eight
  Git tree identities and atomic-import checklist are in the
  [S3 ledger](../proposals/m9-lib-rgb-graphics-frame.md#s3-rgb-corpus-audit-and-downstream-adoption).
  Production/test +0/-0; no new runtime tests/builds or EXE refresh for this
  documentation-only audit. Prior 193/193 per architecture XML and EXE hashes
  were rechecked; no downstream product acceptance is claimed.
- T96 S2: production +27/-19 net +8; tests +147/-53 net +94. Both
  architectures pass 193/193 distinct tests, no skips: 177 unit/gates, eight
  background integrations, five desktop and three native audio integrations.
  Evidence: build/t96/s2/retry1 (focused, x86 units, all integrations) and
  build/t96/s2/recheck (final x64 177/177 and documentation gate). Earlier
  failed test-fixture attempts remain recorded, not counted as passes.
- S2 archived EXE SHA-256: softpc32
  D6E24F0FD1E414A2C8AD4C6A36C0BBAF4911E196FE3BD5C43376E631F6699DFB;
  softpc64 BDADAE74E8FA0B766F6392837FD02A44C9B8168895468211EA37A78B53CFCE95.
  Existing original-mirror compiler warnings, three unchanged VGA-test pointer
  casts and x86 Ninja log recovery remain disclosed; no zero-warning claim.
- Owner INI/media and untracked assets/binary/snapshot.debug are untouched.
  S4 closes with the owner-approved retain disposition; runtime is unchanged.

### Previous qualified baseline

- T95 repair 66055d92 and review 6c3b604f: existing paused DMA service guard;
  production +2/-1 net +1, tests +12/-0 net +12.
- x86/x64 each 193/193 distinct passes, no skips: 177 unit/gates, eight
  background, five desktop and three native audio integrations. Actual guests
  pass repeated WAV, Windows exit/restart and MPU Canyon then WAV.
- Published SHA-256: softpc32
  A00E025691F82AA746597E76A0E281343416EE59869DA0766AA5D8F79CEFC72A;
  softpc64 47D8281698DCC44F67A5E381D70D7A3450DACC4EDF9DC2C001C2DADC8F081FA0.
- Owner inputs unchanged. MIDI requires documented MPU mapping; no new FM or
  SB-MIDI capability. Parser debt remains in [TODO](TODO.md). Existing mirror
  warnings and Ninja recovery remain disclosed. Governance closure does not
  rebuild or rerun unchanged runtime code.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T95 | Delegated S2/T closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |
| T94 | S1-S3 accepted, 2026-10-17. | [Audit](../history/M9-T94-completion-audit.md) |
| T93 | S1-S15 accepted, 2026-10-16. | [Audit](../history/M9-T93-completion-audit.md) |

## Recent Governance

- **M9 Td S21:** Owner-approved bounded frame ownership rule delivered and
  reviewed; [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 S5 resumes; no runtime or artifact change is claimed by this Td.

T95 proposal is archived. T96 remains open for whole-task closure audit;
S7 is owner-accepted and S8 is active.
