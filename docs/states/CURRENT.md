# Project Status

## Current Work

No implementation subtask is active.

T93 S15 is owner-accepted and closed. The owner requests whole-T audit and
closure, requiring every build, unit, gate and integration to pass. The separate
T93 completion audit passes after final delivery 161305b2 and actual-change
review: T93 is closed. No new task is admitted.

See the [T93 audit](../history/M9-T93-completion-audit.md),
[S15 closure](../history/M9-T93-S15-portable-sound-archive.md) and
[retained proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md).
Historical requests/checkpoints are retained there and in the individual S
records, not as competing active packets in this status file.

## Current Technical Baseline

- Program task/build version remains T93. Shared source/test packages are
  Lib, Emulator, X86 Debug and X86 IBM PC; App privately composes the existing
  machine and Compat over the selected XP SP1 core. The 505-body mirror manifest
  pins upstream, necessary subset and local identities independently.
- Lib provides neutral PCM/MIDI/FM contracts. PCM uses its existing copied
  FIFO/worker/native output; MIDI uses the native capability, not synthesis.
  FM honestly reports unsupported host capability. XP DSP/Mixer/MIDI/FM device
  state remains in the recovered owners, with Compat supplying host services.
- Portable snapshots include active sound state and reconstruct host resources.
  Four separate-process x86/x64 archive routes pass. Old snapshot formats are
  intentionally incompatible; MIDI restoration is approximate, not full host
  instrument/envelope restoration. No native Linux or downstream-product
  qualification is inferred from Windows-based contract tests.
- Latest complete receiving qualification: x64 193/193 and x86 193/193, no
  failures, skips, disabled or missing tests. Each comprises 121 shared checks,
  64 other background tests, five real desktop and three real audio integrations.
  Exact name-set comparison with unfiltered CTest registration passes. Both
  full-target cached Release builds pass; x64 reused current objects, x86 rebuilt
  after the existing Ninja log recovery. This is not a clean-cache rebuild or
  zero-warning claim; unchanged mirror warnings remain disclosed in the audit.
- Accepted executables in assets/binary:
  softpc32 SHA-256 `CA4D9B152C44E901354ADE7DB7635717B47BD11596B39E41F792BD584A60ACBE`;
  softpc64 SHA-256 `56BEA1E29C4EB06C48050D444E640631AFF7865203A2653F89AE13495D9E5CCF`.
  Final qualification delivery changes no production/test source. User INI,
  media and current owner snapshot remain unchanged during closure.
- Shared units reside under their tested source owners, use inward dependencies
  and do not require an interactive/audio endpoint. Native audio and desktop
  tests remain integrations and were actually run; no native failure is mocked
  into a passing product test. Four Lib negative fixtures are flat.
- Queue has one unnumbered candidate: Win95 Sound Blaster recognition/playback.
  Code 24 is not declared repaired by T93. TODO retains six independent debts;
  their causes are not claimed fixed by the successful receiving run.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T93 | S1-S15 complete; owner accepts S15 and authorizes whole-T closure, 2026-10-16. Fresh dual-width 193/193 including all native integrations and four archive routes. | [Completion audit](../history/M9-T93-completion-audit.md) |
| T92 | Owner accepted and closed 2026-10-15; S1-S6 complete, dual-width 150/150 including all desktop tests. | [Completion audit](../history/M9-T92-completion-audit.md) |
| T91 | Owner accepted and closed 2026-10-15; all S1-S5 obligations complete. | [Completion audit](../history/M9-T91-completion-audit.md) |

Earlier closures and superseded technical checkpoints remain in history and
Git, rather than repeating stale active/delivery statements here.

## Recent Governance

T93 closure archives its proposal, repairs all direct references, records S15
acceptance and full-task request/S/evidence/debt accounting, and reduces this
status file to its current authority. The final artifact/evidence P was pushed
before the separate actual-change review. Existing Queue order and TODO items
are unchanged; no next T number is allocated. No rule is edited by this closure.
