# Project Status

## Current Work

T95 S1 diagnosis/design is owner-accepted and closed on 2026-10-17.
Owner admits S2 to repair paused DMA submission through the existing service
path and qualify both widths. The minimal guard and regression are implemented;
both unmodified architectures failed the new assertion, and repaired dual-width
focused tests pass. Full qualification passed 193/193 distinct tests per width,
including desktop and native audio. Both guest widths pass the four-WAV
sequence, twelve further Chord plays, Windows exit/restart, full Canyon via
MPU-401 and subsequent WAV. Delivery is qualified and awaits owner acceptance;
S2 delivery 66055d92 is pushed and coordinator-reviewed; S2 and T95 remain
open pending owner acceptance. Original Ad Lib/SB-MIDI capabilities are not expanded.
See the
proposal's S1 delivery review and the [task record](../history/M9-T95-S1-win31-midi-wav-playback.md).
Owner admits the former queue head on 2026-10-17 and supplies a post-driver-
restart snapshot after audible startup WAV. T94 is closed; see its
[completion audit](../history/M9-T94-completion-audit.md).

## M9 T95 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly approves closing S1 and admitting S2 on 2026-10-17. |
| Objective | Prevent submission of pending DMA audio while DSP-paused; preserve existing completion/resume behavior and verify repeated WAV plus the working MPU MIDI route. |
| Non-goals | No new audio model, SB-MIDI implementation, FM synthesis, forced IRQ or public component change; no Win95 volume/restart repair. |
| Reference Baseline | 9df788229cb841a66ba74502ca81b20398eda0ce; accepted S1 diagnosis and unchanged T94 production binaries. |
| Candidate Proposal | [Win3.1 playback investigation](../proposals/m9-win31-midi-wav-playback.md). |
| Files And ABI Surface | Existing host/src/nt_sbdsp.c service guard, App machine device-contract test and mirror-accounting evidence; no ABI, additional state or shared-component change. |
| Applicable Rules | Execution, Document, Architecture and Coding authorities; single-person executor/coordinator actual-diff review. |
| Verification | Red/green mock backpressure test in both DMA directions; retain completion/reset/snapshot coverage. Cached x86/x64 builds, publish both EXEs, run full unit/gates and background, desktop and native-audio integrations. Replay four-WAV sequence, repeated Chord, Windows exit/restart and configured MPU route through existing guest input/capture probes without Computer Use or diagnostic pause gate. Report native availability separately. |
| Expected Markers | Paused service accepts no more bytes or newly schedules completion; Continue resumes the same pending block. Already armed completion is retained. Repeated guest WAV completes without the observed masked-IRQ wait; musical MIDI delivery and completion use MPU. |
| Asset Needs | Preserve owner snapshot, media and INI. Disposable overlays and evidence under ignored build/t95; hidden owned jobs with logs/exit markers. Guest observations: 1800-second run, 300-second no-progress and 128 MiB trace cap; qualification jobs use declared per-phase budgets. Executor owns cleanup. |
| Reporting Requirements | Estimate production logic +1/-1, net zero, plus a short comment; test +15 to +30/-0. Report actual production/test added/deleted/net and mirror-relative diff separately, proof limits, published hashes and fixed commit. |
| Stop Conditions | Broader interrupt/parser redesign, new capability or public API becomes necessary: report evidence and obtain revised scope. No product retries or relaxed test assertions to mask a failure. |
| Exit Criteria | Focused and prescribed full qualification pass, dual EXEs published, similar-issue disposition and actual-diff review complete, committed/pushed delivery awaits owner acceptance before S2 closure. |
| Original Owner Request | "批准收口S1和准入S2"; implement the minimal paused-service repair designed in S1, preserving owner inputs. |
| Similar-Issue Sweep | Inspect all DspServiceTransfer/DspProcessBlock callers, pause/continue, auto-init EOI, completion and snapshot/running transitions. Track the separately evidenced unsupported SB-MIDI parser collision as debt, not as a repaired capability. |

## Current Technical Baseline

- T94 S2 delivery cd665a3d and review 7f64ec53 fix reset sound binding, physical
  DMA accesses and input-channel PCM gain. Owner confirms Win3.1/Win95 effects.
- T94 S3 delivery 3e5c61e4 and review 1451522f establish Base's sole neutral input
  contract, preserving values/layout and behavior. See the
  [S3 record](../history/M9-T94-S3-base-input-ownership.md).
- Latest cached full builds and qualification: x64/x86 each 193/193 distinct
  tests, no skips (177 units/gates, eight background integrations, five desktop
  and three native audio integrations), rerun for the T95 S2 guard change.
- Published SHA-256: softpc32
  A00E025691F82AA746597E76A0E281343416EE59869DA0766AA5D8F79CEFC72A;
  softpc64 47D8281698DCC44F67A5E381D70D7A3450DACC4EDF9DC2C001C2DADC8F081FA0.
  Both EXEs rebuilt for S2; owner INI/media/checkpoint are unchanged.
- MIDI/WAV diagnosis and repair belong to active T95. Win95 volume-control
  and restart remain queued; SB-MIDI parser collision is tracked in [TODO](TODO.md).
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
