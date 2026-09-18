# Project Status

## Current Work

No implementation subtask is active. M9 T69 is closed at the owner's approval.
Its accepted repair publishes only complete renderer updates, derives the
selected packed painter's width from its one-byte/one-pixel contract, and keeps
the detached text surface authoritative through fullscreen/windowed transitions.
The final owner test accepted the repaired route. The intermittent fullscreen
native Window-height report and two audited simplification candidates remain
explicit TODO debt; neither is silently represented as fixed.

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
- T69 is closed: its source-neutral Compat display transaction, painter-width
  contract and detached text/cursor updates remain in force. The previously
  reported fullscreen native Window-height oscillation remains reproducibility-
  gated TODO debt, not an accepted regression or a speculative repair target.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T69 | S1--S4 complete; owner accepted the repaired Win3.1 display route. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T69 closed after S4's finite accounting audit. The archived task proposal and
closure audit retain the complete causal and verification record.
