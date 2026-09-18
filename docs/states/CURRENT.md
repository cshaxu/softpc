# Project Status

## Current Work

No implementation subtask is active. M9 T63 is closed after owner acceptance
of its snapshot, Overlay-media and paused-save behavior. The first queued
candidate remains Lib frame-copy and Console task simplification.

## Current Technical Baseline

- Source: M9 T63 terminal P18 (`08f82ea`); the final code delivery is P17
  (`82f6a50`).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- Final x86 and x64 suites each pass 106/106. Owner accepted restored display,
  input, Overlay media and paused-save behavior. The fixed package EXEs are
  current; owner configuration remains in `assets/binary/softpc.ini`.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |
| T60 | S1–S7 complete; owner acceptance; D6 retained, dual-width 97/97. | [Audit](../history/M9-T60-completion-audit.md) |

## Recent Governance

T63's completed proposal is retained in history; its closure does not admit or
reorder a Queue candidate. No active packet exists.
