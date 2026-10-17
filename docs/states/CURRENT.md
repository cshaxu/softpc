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
S3 awaits owner review; S4 is not admitted.

## M9 T96 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly authorizes proceeding: "没空验收 你继续做s3吧". S2 is technically verified, not user runtime accepted. |
| Objective | Audit the delivered RGB contract and publish reproducible corpus identities plus downstream Machine migration instructions. |
| Non-goals | No external repository edits, new renderer/format/cache or five-buffer cleanup; S4 remains last. |
| Reference Baseline | 94a0debd, reviewed S2 delivery fa569001. |
| Candidate Proposal | [RGB graphics-frame contract](../proposals/m9-lib-rgb-graphics-frame.md). |
| Files And ABI Surface | CURRENT, proposal and S2 handoff; audit src/test Lib, Emulator and direct frame consumers. No planned source/test/ABI change. |
| Applicable Rules | Execution and Document; Architecture and Coding before source/design audit. Single-person executor/coordinator actual-diff review. |
| Verification | Source/dataflow audit, eight tree identities and manifests, prior S2 dual-width 193/193 evidence, current documentation gate. No rebuild of unchanged runtime for documentation-only delivery. |
| Expected Markers | No indexed graphics consumer or alternate format; exact atomic-import instructions with source/test hashes and proof limits. |
| Asset Needs | Read existing evidence only; preserve EXEs, INI, media and untracked checkpoint. No guest or desktop session required. |
| Reporting Requirements | Estimate production/test +0/-0 net 0; roughly 70-110 added and 15-35 removed documentation lines. Record actual scope and audit dispositions. |
| Stop Conditions | Newly found runtime defect or required ABI change must be reported and scoped before implementation. |
| Exit Criteria | All bounded audit obligations disposed, import contract and fixed corpus identified, documentation gate and actual-diff review passed; commit/push for owner review. |
| Original Owner Request | "没空验收 你继续做s3吧". |
| Similar-Issue Sweep | All graphics member/copy/render callers and fixed allocations; classify retained text/private palettes rather than delete them. |

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
  S3 is downstream adoption audit; final S4 owns five-buffer simplification.

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
