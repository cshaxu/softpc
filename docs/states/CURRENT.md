# Project Status

## Current Work

Owner delegates T95 S2/T closure and next queue admission on 2026-10-17.
See the [completion audit](../history/M9-T95-completion-audit.md).
T96 S1 design is delivered and reviewed at 7d0d3dea; owner directs continuing
the RGB upgrade while scheduling buffer simplification as final S4. S2 is active.
See the [S1 record](../history/M9-T96-S1-rgb-frame-design.md). Existing five-buffer
ownership remains unchanged during S2. Implementation and dual-width full
qualification are complete. Executor fa569001 is committed/pushed and its
actual diff reviewed; S2 awaits owner acceptance, with no S3 admission.

## M9 T96 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner delegates next queue admission on 2026-10-17 and confirms "批准开始"; queue head is RGB graphics frames. |
| Objective | Migrate Lib graphics ABI, direct-colour rendering and SoftPC producer atomically to RGB32; preserve text, dirty, snapshot and handoff semantics. |
| Non-goals | No renderer rewrite, alternative format, alpha, cache, external repository change or frame-buffer cleanup (reserved for S4). |
| Reference Baseline | 7d0d3dea, reviewed S1 design; production remains qualified T95. |
| Candidate Proposal | [RGB graphics-frame contract](../proposals/m9-lib-rgb-graphics-frame.md). |
| Files And ABI Surface | Lib Window frame header/render, App machine driver, ten affected test files (including discovered Session automatic fixtures), manifests/docs and both EXEs. Breaking copied-graphics ABI; rebuild all consumers. No original mirror change. |
| Applicable Rules | Execution and Document; Architecture and Coding before source/design audit. Single-person executor/coordinator actual-diff review. |
| Verification | RGB/channel/high-byte/stride/tail/damage tests; actual DIB conversion, palette-only publication, snapshot and restart regression. Cached x64/x86 full builds and publication; public/App unit/gates, background, desktop and native audio integrations, reported separately. |
| Expected Markers | One 0x00RRGGBB contract, exact storage census, coherent ABI/producer migration without broken intermediate delivery. |
| Asset Needs | Preserve owner INI/media/checkpoint. Hidden owned build/test jobs in build/t96/s2 with logs/exit markers, two-hour budget and ten-minute no-progress investigation, executor cleanup. Native tests use standing authorization. |
| Reporting Requirements | Estimate production +34 to +50/-18 to -28, net +16 to +22; tests +160 to +240/-62 to -98, net +98 to +142. Report actual paths/counts, hashes, proof limits and runtime costs separately. |
| Stop Conditions | Missing source colour information, unjustified x86 storage cost or second rendering path required: report before implementation. |
| Exit Criteria | Focused/full qualification passed, both EXEs published, manifests/gates and actual-diff review complete, delivery committed/pushed for owner acceptance. |
| Original Owner Request | "把这个无份清理的任务排到本t的最后一个s任务 然后接着做现在的升级". |
| Similar-Issue Sweep | Indexed graphics assumptions, byte/pixel copy sizing and frame allocation; explicitly exclude legitimate text/private machine palettes. |

## Current Technical Baseline

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
