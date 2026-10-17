# Project Status

## Current Work

Owner delegates T95 S2/T closure and next queue admission on 2026-10-17.
See the [completion audit](../history/M9-T95-completion-audit.md).
T96 S1 design is delivered and reviewed at 7d0d3dea; owner directs continuing
the RGB upgrade while scheduling buffer simplification as final S4.
See the [S1 record](../history/M9-T96-S1-rgb-frame-design.md). Existing five-buffer
ownership remains unchanged during S2. Implementation and dual-width full
qualification are complete. Executor fa569001 is committed/pushed and its
actual diff reviewed. Owner says "没空验收 你继续做s3吧": proceed without
claiming user runtime acceptance. S2 delivery is recorded in
[its handoff](../history/M9-T96-S2-rgb-migration.md). S3 audit and migration
instructions are complete; executor cba71230 is pushed and actual-diff reviewed.
Owner acknowledges S3 and admits S4: "哦这样啊 那准入s4 你告诉我如何干净的".
Owner approves S4 closure and explicitly keeps T96 open.
Owner now admits S5: "非常好，我希望就按照现有架构+这个方案来做！请你开工".
S5 implementation replaces frame copies with bounded pointer ownership circulation.
Final qualification is complete: x86 and x64 each pass 194/194, no skips.
The x64 native set passed unchanged after the owner reconnected the desktop.
Earlier disconnected-session failures remain recorded below. Executor P1 ccecf953 is committed and pushed. Coordinator review of that actual
commit confirms bounded ownership, no large payload transport copies, no mirror
or Compat changes, and the final XML/artifact ledger. S5 awaits owner acceptance;
S/T closure is not claimed.
S4 remains closed; see the
[S4 closure](../history/M9-T96-S4-frame-lifetime-audit.md). S3's
[documentation delivery](../history/M9-T96-S3-rgb-corpus-audit.md) is complete
at cba71230/3758a33e; S2 manual runtime
acceptance remains unclaimed.

S6 is now active by owner admission; [S5 delivery](../history/M9-T96-S5-frame-exchange-delivery.md)
remains an unaccepted checkpoint, not a second active task.

## M9 T96 S6 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 准入新S完成本次审计问题的修复。 S5 delivery remains recorded without claiming runtime acceptance. |
| Objective | Close S5 audit: concurrent consumer turnover regression; remove unused Session frame payload; validate immutable frames once at publication; reconcile stale ownership documentation. |
| Non-goals | No mirror/Compat, guest timing, media, INI, surface replacement, generic resource framework or second transport path. |
| Reference Baseline | d76355ed (S5 delivered); keep exchange roles, FIFO notifications and Machine observer contract unchanged. |
| Candidate Proposal | [RGB contract and S5 circulation ledger](../proposals/m9-lib-rgb-graphics-frame.md). |
| Files And ABI Surface | Lib private Window helpers and exchange tests; Emulator Session wake-hint signature and direct callers; manifests and ownership documentation. Machine observer ABI, App/Core and exchange algorithm unchanged. |
| Applicable Rules | Execution, Document, Architecture and Coding; single-person executor/coordinator review. M9 Td S21 reconciles the frame ownership rule; no rules edits in S6. |
| Verification | Deterministic overwrite/take/retire races, slow-consumer progress, failure/no-frame, pause/reset/load, component recreation and teardown; dual-width focused then complete unit/gate/integration runs and both EXEs. Hidden owned jobs under build/t96/s6, no Computer Use or interactive timeout termination. |
| Expected Markers | Window roles produce/pending/processing/displayed, Console produce/pending/processing; bounded allocations, no pixel memcpy between transport stages, no outward Lib dependency. |
| Asset Needs | Preserve user configuration, media and untracked checkpoint; refresh only package EXEs after successful build. |
| Reporting Requirements | Production +10..25/-25..50, net -10..-30; tests +100..160/-20..50. Report actual accounting and evidence, not a promised speedup. |
| Stop Conditions | More slots, consumer-dependent producer wait, extra pinned historical frame, or changed failure/snapshot semantics requires explicit revised design, not hidden fallback. |
| Exit Criteria | Single transport ownership path, bounded complete lifecycle proof, full qualification, artifacts, actual diff review, commit/push and owner acceptance. |
| Original Owner Request | 准入新S完成本次审计问题的修复。 |
| Similar-Issue Sweep | All frame producers/copy readers, event routes, native consumers, observer callbacks and reset/destroy paths; small Console conversion and explicit diagnostic copies distinguished from forbidden transport copies. |

S6 admitted. Expected production +10..25/-25..50 (net -10..-30), tests
+100..160/-20..50. Scope: Lib private Window helpers and exchange tests;
Emulator Session notifications and callers; manifests and current design docs.
Hidden owned qualification under build/t96/s6; full dual-width tests and EXEs.
S6 implementation, executor self-review and full qualification are complete.
Executor commit/push and coordinator actual-commit review follow. Owner
acceptance and S/T closure are not claimed.

## Current Technical Baseline

- S6 production C/H +19/-17, net +2; tests C/H +88/-20, net +68.
  Lib production +10/-7, Emulator +9/-10; Lib tests +75/-0,
  Emulator tests +13/-20. Manifests/docs/artifacts are excluded.
  Full changed-path and four-finding convergence ledger is in the proposal.
- x86 and x64 each pass 194/194 distinct tests, no skips: 178 unit/gates,
  eight background integrations, five desktop and three native audio.
  Evidence: build/t96/s6/full1/{unit,integration}-x86.xml and -x64.xml,
  native-{x86,x64}-0..7.xml; qualification.exit is 0. Focused sets are 15/15.
  Final x64 turnover-test counter cleanup was rebuilt and passed ten repeats
  in build/t96/s6/recheck-x64/turnover.xml (qualification.exit 0), superseding
  that test's earlier x64 binary. x86 full qualification used the final test.
- Both package EXEs are rebuilt and deployed. SHA-256 softpc32:
  37FECBCEDB6C8B0897DF4EF882E84974FF61872FF0A44A8E55550DF1245C2B40;
  softpc64: 70FC59C1498712AC84CCB6EEF2EF058FF3F291FA976704132C7B58881D6194B7.
- Existing original-mirror macro-redefinition warnings and x86 Ninja log
  recovery remain disclosed; no new warning in changed production files was
  observed. No zero-warning or measured performance claim is made.
- No App/Core/mirror, INI, media, snapshot or exchange-algorithm changes.
  Untracked assets/binary/snapshot.debug remains owner-owned and excluded.

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
