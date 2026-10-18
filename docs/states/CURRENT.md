# Project Status

## Current Work

Owner accepted T96 S8 and explicitly closed T96 after confirming Win3.1 MIDI
works. The [T96 completion audit](../history/M9-T96-completion-audit.md) reconciles
S1-S8, final frame contracts, fixed storage, adoption and qualification.
Owner admits T97 S1 and subsequently merges the remaining queue candidate:
investigate Windows 95 unavailable volume controls and restart stopping at the
safe-shutdown screen. Preserve separate evidence/conclusions for both symptoms.
S1 investigation now reproduces both symptoms and their independent guest
policies: the installed driver excludes SB2.01 mixer/volume capabilities;
its hardware-change dialog requests shutdown, while explicit Restart works.
The [S1 evidence reconciliation](../proposals/m9-win95-volume-and-restart.md#s1-nine-item-evidence-reconciliation)
records limits and verification. No production repair or new model is justified.
Await owner review; T97 and S1 are not owner-closed.
User-owned untracked assets/binary/snapshot.debug remains untouched/excluded.

## M9 T97 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admits next T after T96, then explicitly directs: "本项和队列剩余一项合并调查吧"; volume and safe-shutdown restart are both in S1. |
| Objective | Reproduce disabled Win95 volume controls despite working PCM and installation-requested restart stopping at safe shutdown; determine each first divergence and smallest correct remedy without presuming a common cause. |
| Non-goals | No production repair in this investigation stage, SB16 spoof, new mixer model, forced UI flag, screen-triggered reboot, unrelated Lib redesign or user-media modification. |
| Reference Baseline | T96 accepted runtime 5ea4e55e; final closure audit and unchanged qualified packages. |
| Candidate Proposal | [Win95 volume and restart S1 ledger](../proposals/m9-win95-volume-and-restart.md). |
| Files And ABI Surface | Read-only investigation of App-SoftPC driver/Compat/original DSP/mixer, keyboard-controller/CPU reset and BIOS, installed guest drivers, volume software and restart intent. No public ABI change admitted in S1. |
| Applicable Rules | Execution and Document for admission; Architecture/Coding before code work. Single-person executor/coordinator actual-diff review. |
| Verification | Disposable overlay/snapshot replay, bounded capability/reset/BIOS probes and source-to-request trace. Compare normal restart, installation restart and shutdown; verify audio rebinding. No Computer Use. Design-only delivery runs documentation/whitespace checks; later repairs require focused and full dual-width qualification. |
| Expected Markers | Working PCM with disabled controls; exact capability predicate; restart intent versus actual reset/BIOS progress; independent causal evidence for both issues. Host gain is not guest mixer support, and a shutdown screen alone is not a reset diagnosis. |
| Asset Needs | Preserve all owner INI/media/snapshots. Evidence under ignored build/t97/s1; bounded logs/captures and explicit job log/exit before launching diagnostics. No EXE refresh for admission alone. |
| Reporting Requirements | S1 report supplies separate binary/source/runtime evidence, reproduction limits and zero production/test/mirror delta; full mixer capability needs separate approval, not a version spoof. |
| Stop Conditions | New hardware identity/capability, mirror behavior expansion or public API redesign needs approval. No forced controls or reboot bypass; actual repair follows the investigated design. |
| Exit Criteria | Finite nine-item combined ledger resolved with evidence/limits for both issues, actionable designs, actual-diff review and committed/pushed report for owner direction. |
| Original Owner Request | 启动开机有声音，但是任务栏无法显示声音图标，为啥; 我看到的是现在可以安全地关闭计算机了，一直停在那里; 本项和队列剩余一项合并调查吧. |
| Similar-Issue Sweep | PCM volume/mixer exposure, MIDI versus PCM and guest control software; normal versus install restart, warm/reset paths, normal shutdown and post-reset audio acquisition. Classify rather than add product special cases. |

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
