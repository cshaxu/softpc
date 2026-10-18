# Project Status

## Current Work

Owner accepted T96 S8 and explicitly closed T96 after confirming Win3.1 MIDI
works. The [T96 completion audit](../history/M9-T96-completion-audit.md) reconciles
S1-S8, final frame contracts, fixed storage, adoption and qualification.
Owner admits the next queue head as T97 S1: investigate Windows 95 unavailable
volume controls. No production repair or new hardware capability is presumed.
User-owned untracked assets/binary/snapshot.debug remains untouched/excluded.

## M9 T97 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner: "works! 好了收口本S和本T，准入下一个T任务"; next Queue head is Win95 volume control. |
| Objective | Reproduce disabled Win95 volume slider/taskbar checkbox despite working startup PCM; identify driver, model, mixer and guest-software cause; propose the smallest correct remedy. |
| Non-goals | No restart/shutdown repair, SB16 identity spoof, new mixer model, forced UI flag, unrelated Lib redesign or automatic user-media modification. |
| Reference Baseline | T96 accepted runtime 5ea4e55e; final closure audit and unchanged qualified packages. |
| Candidate Proposal | [Win95 volume control and S1 ledger](../proposals/m9-win95-volume-control.md). |
| Files And ABI Surface | Read-only investigation of App-SoftPC driver/Compat/original DSP and mixer, installed guest drivers and volume-control software. No public ABI change admitted in S1. |
| Applicable Rules | Execution and Document for admission; Architecture/Coding before code work. Single-person executor/coordinator actual-diff review. |
| Verification | Disposable overlay/snapshot guest replay, bounded port/capability probes and source-to-request trace. No Computer Use. Reconcile each finding with actual driver/configuration; design-only delivery runs documentation/whitespace checks. Any later production repair requires focused and full dual-width qualification. |
| Expected Markers | Working PCM and disabled controls observed together; exact advertised capability and first divergence; no claim that host gain equals guest mixer support. |
| Asset Needs | Preserve all owner INI/media/snapshots. Evidence under ignored build/t97/s1; bounded logs/captures and explicit job log/exit before launching diagnostics. No EXE refresh for admission alone. |
| Reporting Requirements | S1 returns causal evidence, source/runtime distinction, minimal per-file proposal, added/deleted/net estimate, compatibility limits and follow-up S plan. |
| Stop Conditions | New hardware identity/capability, mirror behavior expansion or public API redesign needs owner approval; do not silently force the controls enabled. |
| Exit Criteria | Finite six-item S1 investigation ledger resolved with evidence/limits, actionable design, actual-diff review and committed/pushed report for owner direction. |
| Original Owner Request | 启动开机有声音，但是任务栏无法显示声音图标，为啥; owner subsequently requests admission of the next queue task. |
| Similar-Issue Sweep | Check PCM volume capability, mixer lines/controls, MIDI versus PCM ownership, guest volume-control installation and both slider/tray predicates; classify rather than add product special cases. |

## Current Technical Baseline

- T96 runtime/reference: 5ea4e55e; S8 delivery a404740f and review 5ea4e55e.
  All 191 tests per architecture pass with zero failures/skips: 175 unit/gates,
  eight background integrations, five desktop and three native audio.
  Evidence: build/t96/s8/qualification, qualification.exit=0; exact CTest/XML
  registration equality was checked. Existing compiler/Ninja warnings remain.
- Both cached builds completed; x86 rebuilt after existing Ninja log recovery.
  Packages remain T96 until a later qualified T97 runtime delivery:
  softpc32 SHA-256 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A;
  softpc64 SHA-256 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
- Current transport: RGB graphics, separate leaf text contracts, Machine-owned
  fixed four-slot exchanges shared through opaque Base mechanics; Driver owns
  readiness and Machine publishes every produced frame. Session/UI route
  references. See current architecture and T96 audit for accounting.
- Win3.1 verified MIDI configuration is MPU330/IRQ7, SB220/IRQ5, all sixteen
  Mapper channels to MPU. [Product UI](../design/UI.md#windows-31-midi-playback)
  owns instructions. No FM or SB-MIDI capability was added.
- Existing [TODO](TODO.md) items remain open; closure does not repair intermittent
  prompt readiness, Ninja recovery, unsupported SB-MIDI or other unrelated debt.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T96 | Owner accepted S8 and T96, 2026-10-17. | [Audit](../history/M9-T96-completion-audit.md) |
| T95 | Delegated closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |
| T94 | Owner accepted S1-S3, 2026-10-17. | [Audit](../history/M9-T94-completion-audit.md) |

## Recent Governance

- M9 Td S21 approved bounded frame ownership; see the
  [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 completed that implementation; no rule change accompanies T97 admission.
