# Project Status

## Current Work

T94 S1 is active: investigate Windows 95 Sound Blaster recognition and playback
using the owner-supplied snapshot. T93 remains closed; its receiving results
do not prove Win95 driver initialization.

See the [T93 audit](../history/M9-T93-completion-audit.md),
[S15 closure](../history/M9-T93-S15-portable-sound-archive.md) and
[retained proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md).
Historical requests/checkpoints are retained there and in the individual S
records, not as competing active packets in this status file.

## M9 T94 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admits the next queued T and authorizes independent Win95 driver installation on 2026-10-16. Single-session executor/coordinator roles remain separate. |
| Objective | Restore the supplied Win95 snapshot, install the appropriate existing SB driver on disposable overlay media, and identify the first failed hardware contract, if any. |
| Non-goals | No speculative production fix, SB16 upgrade, software FM, new device path, public interface change or modification of original user media/configuration. |
| Reference Baseline | c55dd73d; accepted T93 binaries and selected XP device implementation. Subsequent disconnected-session rerun passed all units but not all native integrations. |
| Candidate Proposal | [Win95 SB proposal and admission ledger](../proposals/m9-win95-sound-blaster-recognition.md). |
| Files And ABI Surface | Admission documents plus ignored diagnostic harnesses; inspect App machine/Compat/XP DSP, DMA and PIC paths. No production ABI change admitted in S1. |
| Applicable Rules | Execution and Document rules; Architecture and Coding rules/design for probes; shared governance skills; UI policy for guest-independent product behavior. |
| Verification | Check snapshot media identity/mode before loading; restore through existing machine executor; capture copied frames and inject guest keys without Computer Use. Record driver files/resources and bounded DSP/DMA/IRQ observations. Documentation gate and actual-change review for S1; dual-width complete regression and publication belong to subsequent implementation qualification. |
| Expected Markers | Snapshot restored, guest frame captured, driver installation outcome and earliest failed exchange evidenced; absence of evidence is not success. |
| Asset Needs | Owner-supplied Win95 snapshot/media read-only originals; disposable copies and overlays allowed. Ignored build/t94/s1 probes: 600 seconds per guest run, 120 seconds without progress, 30 MiB trace cap, one owned hidden job with log/exit paths. Executor owns shutdown and cleanup; retain bounded checkpoint. No host audio mock counts as native playback proof. |
| Reporting Requirements | Before/after brief; production/test added/deleted/net counts; source inference distinct from deterministic and guest evidence; minimal repair scope and XP-subset diff estimate after diagnosis. |
| Stop Conditions | Unexpected writes to original assets, unsafe snapshot attachment, unbounded trace, or necessary out-of-scope public change: stop that operation, preserve evidence, revise scope before implementation. |
| Exit Criteria | Reproducible guest/driver outcome and evidenced causal repair plan, or explicit unresolved boundary with next probe; S1 is not Win95 playback acceptance. |
| Original Owner Request | Owner admits the next T, supplies a Win95 snapshot with no audio driver installed, and authorizes handling installation independently; task objective remains full XP-NTVDM-parity standalone audio usable for Win95 SB playback. |
| Similar-Issue Sweep | Follow reset/version, DSP command parsing, DMA completion, IRQ acknowledgement and native endpoint acquisition through their existing owners; classify observations without attributing unrelated defects. |

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
- Queue is empty after admission of Win95 Sound Blaster recognition/playback.
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

T94 S1 admission consumes the sole queued candidate. Existing TODO items and
T93 closure evidence are unchanged. No rule is edited.
