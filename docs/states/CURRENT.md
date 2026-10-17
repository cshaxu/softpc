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
S5 is interrupted for owner-approved M9 Td S21; it resumes after governance delivery.
Open task awaiting owner: T96.
S4 remains closed; see the
[S4 closure](../history/M9-T96-S4-frame-lifetime-audit.md). S3's
[documentation delivery](../history/M9-T96-S3-rgb-corpus-audit.md) is complete
at cba71230/3758a33e; S2 manual runtime
acceptance remains unclaimed.

## M9 Td S21 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Governance |
| Admission And Approval | Owner approves inserting this narrow Td before resuming T96 S5. |
| Objective | Permit bounded immutable frame ownership transfer without weakening guest-state isolation. |
| Non-goals | No runtime implementation, API change, build, asset refresh or unrelated rules edits. |
| Reference Baseline | 9cf0ab43 plus the admitted, uncommitted S5 design packet. |
| Candidate Proposal | [Pointer circulation](../proposals/m9-lib-rgb-graphics-frame.md). |
| Files And ABI Surface | Architecture rule, architecture migration note, current status and history; governance gate only if needed to represent an interrupted open T. |
| Applicable Rules | Execution, Document and Architecture; single-person dual-role review. |
| Verification | Actual diff and wording audit, documentation gate/self-tests, preserved dependency and single-executor restrictions. |
| Expected Markers | Dedicated immutable frames are distinct from guest/live renderer state; ownership return, bounds and no render-wait or steady-state heap churn. |
| Asset Needs | None; preserve all source/test binaries, INI and user checkpoint. |
| Reporting Requirements | Documentation-only; report exact rule delta and any governance-check adjustment. |
| Stop Conditions | No wider pointer exception or unrelated lifecycle change. |
| Exit Criteria | Narrow policy reviewed, validated and pushed, then resume already admitted S5; T96 stays open. |
| Original Owner Request | "批准" to the proposed narrow Td permitting bounded frame-pointer circulation. |
| Similar-Issue Sweep | Copied-frame language in rules and architecture identified as current implementation versus target; ordinary copied input/command contracts retained. |

## Current Technical Baseline

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
- Current EXE SHA-256: softpc32
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

T95 proposal archived. T96 starts with design: reconcile the original proposed
Lib/SoftPC implementation split into one buildable ABI migration.
