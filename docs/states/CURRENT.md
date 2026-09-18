# Project Status

## Current Work

M9 T69 S1 is admitted: investigate the Win3.1 MS-DOS prompt display roundtrip
failure before choosing a repair owner.

## M9 T69 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner supplied the direct Window/fullscreen-return symptom and `edit` counterexample, requested proposal update, queue priority and an admitted investigation. Standing commit/push approval applies. |
| Objective | Reproduce or bound the Win3.1 MS-DOS prompt residual-character failure and identify the first owner whose state diverges from the correctly displayed `edit` path. |
| Non-goals | No speculative CLS workaround, forced repaint, customer image modification, focus/input repair, broad display refactor or MVDM source change. No implementation repair is admitted in S1. |
| Reference Baseline | T68 (`86de8eb`) is closed. In Window display mode, or after returning from client fullscreen, a Win3.1 MS-DOS prompt leaves residual characters; running and exiting `edit` makes subsequent prompt output clean. Ordinary DOS CLS is known good. |
| Candidate Proposal | [Win3.1 MS-DOS prompt display roundtrip repair](../proposals/m9-win31-display-roundtrip.md) |
| Files And ABI Surface | Read-only investigation spans original text/video output, Compat frame capture, VM frame adaptation, Common UI routing and Lib Window/Console drawing. No source or ABI change is authorized. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. One frame/state owner remains required; no display bypass is introduced. |
| Verification | Establish a reproducible observation matrix for direct Window prompt, fullscreen-to-Window prompt, `edit` enter/exit and ordinary DOS CLS. Trace copied facts only far enough to locate the earliest divergence. |
| Expected Markers | A finite state/route comparison with an explicit first divergent layer or a documented non-reproduction boundary; no behavior change. |
| Asset Needs | Existing owner package and media only; no image writes, artifact refresh or trace fixture. Temporary ignored diagnostics require a bounded build path and cleanup. |
| Reporting Requirements | Record exact paths inspected, each candidate disposition, evidence separating source/frame/route/draw layers, and a narrowly proposed next S if repair is justified. |
| Stop Conditions | Any need to modify source, guest image or package configuration requires a separately approved implementation S. If the observation cannot be reproduced, report the environment and do not claim a repair. |
| Exit Criteria | Root cause is proven or the current reproducibility boundary is recorded; all candidate layers have a disposition and any fix is split into a new approved S. |
| Original Owner Request | “Win3.1 MS-DOS prompt display roundtrip repair…将现象写入proposal，把这项提升到队首，然后准入调研任务” |
| Similar-Issue Sweep | Examine all text-frame dirty/complete publication, frame mode route, Window surface invalidation and Console output-cache transitions reached by Window/fullscreen/`edit`; classify each as source fact, copied frame, route or draw state. |

## Current Technical Baseline

- Source: T68 is closed. The preserved Scan-1 table mapper rejects both first
  out-of-range input values without changing valid key mappings. No Lib,
  Common, App, VM, Compat interface or input behavior changed.
- Both widths have 107 passing test cases; package EXEs were refreshed without
  changing owner INI/media. See [T65 audit](../history/M9-T65-completion-audit.md).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- T63 snapshot behavior remains the owner-accepted baseline. Current package
  EXEs include T67; the owner’s current package INI edit ships with T67.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T68 closes the bounded original keyboard-table safety repair. T67's independent
media-policy ledger remains recorded in its T-level audit.
