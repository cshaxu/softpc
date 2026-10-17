# Project Status

## Current Work

T94 S2 is active under the owner's continuing implementation goal. S1
diagnosis/design is complete after delivery 20298b44 and separate review.
S2 implements reset binding, physical DMA access and input-channel PCM gain
repairs. Delivery cd665a3d is pushed and coordinator-reviewed; S2 awaits owner
acceptance, and T94 remains open. See the
[S1 review](../history/M9-T94-S1-win95-sb-diagnosis.md).

See the [T93 audit](../history/M9-T93-completion-audit.md),
[S15 closure](../history/M9-T93-S15-portable-sound-archive.md) and
[retained proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md).
Historical requests/checkpoints are retained there and in the individual S
records, not as competing active packets in this status file.

## M9 T94 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner's active goal explicitly requests continued implementation of standalone XP-parity sound and Win95 SB playback; S1 causal design is complete. Single-session executor/coordinator review remains separate. |
| Objective | Preserve SB registration across guest BIOS reset, correct physical DMA access, and repair the approved Lib PCM gain defect blocking native Win95 checkpoint playback. |
| Non-goals | No guest-specific branch, SB16 model, synthetic successful detection or native-audio fallback. Owner approves the four DSP physical-memory adaptations and the proven Lib PCM input-channel gain repair with regressions; no device reimplementation. |
| Reference Baseline | 4a675c00; S1 controlled experiment proves missing binding after io_init. |
| Candidate Proposal | [Win95 sound repair](../proposals/m9-win95-sound-blaster-recognition.md). |
| Files And ABI Surface | Compat platform.c, Machine machine.c, nt_sbdsp.c's four physical DMA accesses and mirror manifest, owner-local tests, publication and evidence; no public ABI change. Restore assertion enablement for audio lifecycle and parallel failure. Owner additionally approves Lib PCM Win32 gain implementation, Types vocabulary cleanup, contract documentation, regression and manifests. |
| Applicable Rules | Execution, Document, Architecture and Coding authorities and shared governance skills. |
| Verification | Deterministic real reset/port checks and callback-failure injection with host audio fixtures; cached hidden x86/x64 builds, background units/gates/integrations and explicit native desktop/audio runs. Report environmental failures separately, never convert to passes. |
| Expected Markers | Reset AA and version 02/01 survive repeated guest reset; pending sound events terminate before queue reconstruction; callback failures reject execution. |
| Asset Needs | Ignored build/t94/s2 bounded logs; owned hidden build/test job, 3600-second total and 600-second no-progress observation budget; no timeout-kill. Diagnostic guest runs retain S1's 600-second/120-second budget. Protect owner INI/media/snapshot; refresh only dual EXEs after build success. |
| Reporting Requirements | Production estimate +20-35/-8-12, net +8-27 across two files; tests initially +100-180 lines, refine from existing owners. Report actual added/deleted/net, mirror delta, source versus test versus native evidence. |
| Stop Conditions | Public changes beyond the approved PCM gain repair, mirror changes beyond the four DMA accesses, unexpected original-media write or out-of-scope behavior: stop affected operation and revise scope before proceeding. |
| Exit Criteria | Minimal production repair, deterministic regressions, dual-width publication and required regression outcomes reviewed and pushed; no whole-T playback claim. |
| Original Owner Request | Continue toward standalone XP-NTVDM-parity audio usable for Win95 Sound Blaster playback under single-person dual-role governance. |
| Similar-Issue Sweep | Enumerate SbInitialize/SbTerminate and original reset callback sites. The original I/O table is the sole registry; preserve cold teardown, warm teardown and destroy as distinct lifecycle boundaries, not alternative initialization paths. |

## Current Technical Baseline

- Latest S2 gain-repair qualification supersedes the failed native attempts
  recorded below: x64 and x86 each pass 193/193 distinct registered tests,
  comprising 121 shared, 64 other background and eight native integrations.
  Full-target cached builds refresh both EXEs. Win95 checkpoint load and gain
  setup succeed; two WAV plays reach the 1.24-second end. Native PCM consumption
  progresses, eleven whole DMA-block observations match physical RAM, and
  post-play snapshot save and resume succeed. Shutdown, runtime destruction
  and VM destruction all return success. This proves device/output progress,
  not a human audibility judgment. S2 delivery cd665a3d is pushed and reviewed;
  owner acceptance remains pending.
  Current candidate SHA-256: x86 AA3797FD2335F8730827FCAB2077C4C2BF816EC033F09DCCA823A1F865E563F2;
  x64 86330C3F2518D49F31B33C3425397C39682F47C8FC85773B0F0263A0D30BEFC4.
- Earlier disconnected-session failures and the native gain diagnosis remain
  in the proposal as historical evidence, not current qualification results.
  The separate Compat physical-memory defect remains an explicitly scoped TODO.
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
