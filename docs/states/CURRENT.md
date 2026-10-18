# Project Status

## Current Work

T96 remains open. Owner accepted S6; see [closure](../history/M9-T96-S6-audit-closure.md).
S7 is active by explicit owner admission: direct Console production, native
rebind-without-rewrite regression and equivalent transport-copy audit. Owner
additionally approves pointer-discriminated Window frames, removal of valid/
graphics booleans and removal of the Machine frame envelope; sequence belongs
to the Window frame. Session remains the sole routing coordinator.
The [S7 ledger](../proposals/m9-lib-rgb-graphics-frame.md#t96-s7-direct-console-production) owns its
bounded convergence ledger. S7 is implemented and verified, ready for executor
commit/push and coordinator actual-commit review; owner acceptance remains
pending. User snapshot.debug is preserved and excluded.

## M9 T96 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts S6 and admits S7: 那你准入一个新的S修复吧。另外一个测试的问题也一起修复。同类问题审计扫描一起修复。然后给我提交推送等我验收。 |
| Objective | Produce Console frames directly into Machine-owned three-slot storage; remove Session/UI guest-text copying; add native rebind-without-rewrite proof and audit equivalent transport copies. |
| Non-goals | No mirror/Compat, guest timing, media, INI, surface replacement, generic resource framework or second transport path. |
| Reference Baseline | b25f9969 (S6 accepted); preserve exchange roles, FIFO notices, diagnostic reads and native rendering behavior. |
| Candidate Proposal | [RGB contract and S5 circulation ledger](../proposals/m9-lib-rgb-graphics-frame.md). |
| Files And ABI Surface | Lib Window frame schema and consumers; Emulator Machine driver/publication, Session/UI routing and Product assembly; App machine driver; direct caller tests, Lib native Console regression, manifests and current design. Remove Machine frame envelope; mutually exclusive text/graphics pointers and sequence belong to Window. No mirror or Compat changes. |
| Applicable Rules | Execution, Document, Architecture and Coding; single-person executor/coordinator review. M9 Td S21 reconciles the frame ownership rule; no rules edits in S6. |
| Verification | Direct producer/consumer pointer identity, both slot lifetimes, text/graphics/status transitions, failed/no-frame publication, native raw rebind before rewriting, reset/load/recreation; dual-width focused and full unit/gate/integration tests and EXEs. Hidden owned jobs under build/t96/s7; five-minute no-progress inspection, no automatic termination; executor owns cleanup. |
| Expected Markers | Window roles produce/pending/processing/displayed, Console produce/pending/processing; bounded allocations, no pixel memcpy between transport stages, no outward Lib dependency. |
| Asset Needs | Preserve user configuration, media and untracked checkpoint; refresh only package EXEs after successful build. |
| Reporting Requirements | Production +120..200/-130..220, net -100..+70; test net +100..200; report actual component accounting and evidence, not a promised speedup. |
| Stop Conditions | More slots, consumer-dependent producer wait, extra pinned historical frame, or changed failure/snapshot semantics requires explicit revised design, not hidden fallback. |
| Exit Criteria | Single transport ownership path, bounded complete lifecycle proof, full qualification, artifacts, actual diff review, commit/push and owner acceptance. |
| Original Owner Request | 那你准入一个新的S修复吧。另外一个测试的问题也一起修复。同类问题审计扫描一起修复。然后给我提交推送等我验收。 |
| Similar-Issue Sweep | All driver producers, Session/UI payload writes, mailbox/leaf handoffs, status pages, observers and diagnostic readers. Remove presentation staging copies; retain only source decoding, native rendering and explicit diagnostic copies with distinct evidence. |


## Current Technical Baseline

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
  T96 remains open and S7 awaits owner acceptance after delivery/review.

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

T95 proposal archived. T96 starts with design: reconcile the original proposed
Lib/SoftPC implementation split into one buildable ABI migration.
