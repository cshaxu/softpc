# Project Status

## Current Work

T95 S1 diagnosis/design is delivered for owner review; S2 is not admitted.
Evidence identifies the paused DMA submission defect and verifies Canyon via
the existing MPU-401 route after correctly committing Mapper configuration.
Production code and published binaries are unchanged; repair qualification
remains pending. See the proposal's S1 delivery review.
Owner admits the former queue head on 2026-10-17 and supplies a post-driver-
restart snapshot after audible startup WAV. T94 is closed; see its
[completion audit](../history/M9-T94-completion-audit.md).

## M9 T95 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admits queue first candidate on 2026-10-17 and supplies a post-driver restart checkpoint. |
| Objective | Establish actual MIDI route and first failed contract in repeated WAV playback; present minimal repair design. |
| Non-goals | No speculative production patch, new sound model or synthesis backend; no Win95 volume/restart repair. |
| Reference Baseline | b10093e6; accepted T94 binaries and unchanged hardware model. |
| Candidate Proposal | [Win3.1 playback investigation](../proposals/m9-win31-midi-wav-playback.md). |
| Files And ABI Surface | Read existing App device/Compat/Lib paths and owner-local tests; diagnostic probes only, no production ABI change in S1. |
| Applicable Rules | Execution and Document governance; Architecture/Coding authorities before probe implementation; single-person executor/coordinator review. |
| Verification | Validate supplied snapshot and media identity; restore through existing machine path, capture frames and inject guest input without Computer Use. Trace MIDI destination and repeated WAV progress separately. Any diagnostic build uses cached dual-width baseline objects where applicable; full runtime qualification belongs to eventual repair. |
| Expected Markers | Exact driver/resource/MIDI route evidence; distinguish guest hang, DMA/IRQ wait and host output stall; no guessed cause. |
| Asset Needs | Preserve owner snapshot, media and INI. Disposable overlay and bounded evidence under ignored build/t95/s1; owned hidden jobs with logs/exit markers, 1800-second observation, 300-second no-progress and 128 MiB trace cap per run. Executor owns cleanup. |
| Reporting Requirements | Initial production/test estimate +0/-0; report source proof versus dynamic evidence, exact proposed files and added/deleted/net repair estimate after diagnosis. |
| Stop Conditions | Missing or incompatible snapshot/media, need for new hardware capability or broad public redesign: report evidence and revise scope before implementation. |
| Exit Criteria | Both symptoms have evidenced cause or explicit bounded limitation, minimal repair plan and test design, actual-diff review and complete diagnostic delivery; no false runtime acceptance. |
| Original Owner Request | Admit queue first task; verify screenshot installation choices. Owner supplies snapshot after driver install, restart and audible startup Tada playback. |
| Similar-Issue Sweep | Compare first/subsequent stream open/play/close, DSP completion/IRQ and MIDI/FM routing using existing owners; unrelated findings are reported separately. |

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
