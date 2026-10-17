# Project Status

## Current Work

Owner delegates T95 S2/T closure and next queue admission on 2026-10-17.
See the [completion audit](../history/M9-T95-completion-audit.md).
T96 S1 inventory/design is implemented and undergoing delivery review. The
proposal records three expected production files, nine test files, five existing
heap frame buffers (+14740480 bytes total), and atomic ABI/producer migration.
Dual-width layout/allocation probes pass; no production edit, new EXE or full
runtime qualification is claimed. S2 is not admitted automatically.

## M9 T96 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner delegates next queue admission on 2026-10-17 and confirms "批准开始"; queue head is RGB graphics frames. |
| Objective | Freeze producer/consumer/allocation inventory and coherent single-format RGB migration plan, including x86 storage costs. |
| Non-goals | No renderer rewrite, second pixel format, alpha, conversion cache or external repository edits; no production change in this design checkpoint. |
| Reference Baseline | 6c3b604fa6df3c7979285df2a94428f516a30033, qualified T95 delivery. |
| Candidate Proposal | [RGB graphics-frame contract](../proposals/m9-lib-rgb-graphics-frame.md). |
| Files And ABI Surface | Read Lib Window, Emulator transport, SoftPC publisher and tests; S1 changes planning documentation only. |
| Applicable Rules | Execution and Document; Architecture and Coding before source/design audit. Single-person executor/coordinator actual-diff review. |
| Verification | Enumerate producers, consumers, copy sizes, fixed buffers, source bit depth/stride/row order/palette ownership. Documentation gate and actual-diff review; later implementation requires both builds, publication and full qualification. |
| Expected Markers | One 0x00RRGGBB contract, exact storage census, coherent ABI/producer migration without broken intermediate delivery. |
| Asset Needs | Preserve owner inputs. Disposable layout/allocation probe in ignored build/t96/s1; hidden owned job, log/exit marker, 120-second budget and 1 MiB output cap, executor cleanup. No guest/desktop/audio use. |
| Reporting Requirements | Per-owner files and expected added/deleted/net code, memory costs and implementation split; S1 production/test +0/-0. |
| Stop Conditions | Missing source colour information, unjustified x86 storage cost or second rendering path required: report before implementation. |
| Exit Criteria | Finite inventory and migration design reviewed, documentation gate passed, committed/pushed design delivery; implementation needs its own packet. |
| Original Owner Request | "我没空收口。你帮我收口准入下一个t任务吧". |
| Similar-Issue Sweep | Indexed graphics assumptions, byte/pixel copy sizing and frame allocation; explicitly exclude legitimate text/private machine palettes. |

## Current Technical Baseline

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
