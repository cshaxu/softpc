# Project Status

## Current Work

No implementation subtask is active. Owner accepted S3 and closed T94 on
2026-10-17. See the [completion audit](../history/M9-T94-completion-audit.md).
Three ordered debug candidates are in [Queue](QUEUE.md); none is admitted.

## Current Technical Baseline

- S2 delivery cd665a3d and review 7f64ec53 fix reset sound binding, physical
  DMA accesses and input-channel PCM gain. Owner confirms Win3.1/Win95 effects.
- S3 delivery 3e5c61e4 and review 1451522f establish Base's sole neutral input
  contract, preserving values/layout and behavior. See the
  [S3 record](../history/M9-T94-S3-base-input-ownership.md).
- Latest cached full builds and qualification: x64/x86 each 193/193 distinct
  tests, no skips (177 units/gates, eight background integrations, five desktop
  and three native audio integrations). Closure does not rerun runtime tests.
- Published SHA-256: softpc32
  BF8CDD793C762FA275D25AFAB0ABEA5B001C239E18B88B5CB31EC0A1391777C9;
  softpc64 2EB65B25A4F5E8EBA1CCCC9233C61D024D7A175BBACB354A3AA92F88CEC37DDE.
  Binaries unchanged by closure; owner INI edit ships unchanged.
- Newly reported MIDI/WAV, volume-control and restart symptoms are transferred
  to Queue, not claimed fixed. Existing [TODO](TODO.md) debt is unchanged.
  Existing mirror warnings and Ninja log recovery remain disclosed.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T94 | S1-S3 accepted; owner authorizes T closure and three follow-ups, 2026-10-17. | [Audit](../history/M9-T94-completion-audit.md) |
| T93 | S1-S15 accepted, 2026-10-16. | [Audit](../history/M9-T93-completion-audit.md) |
| T92 | S1-S6 accepted, 2026-10-15. | [Audit](../history/M9-T92-completion-audit.md) |

## Recent Governance

T94 proposal archived. Its unadmitted S4 plan is superseded by this closure
audit and owner-directed follow-ups, not reported as an executed S.
