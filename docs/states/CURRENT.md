# Project Status

## Current Work

No implementation subtask is active. T64 is closed after owner acceptance.
Thread unification was cancelled by owner decision; the narrowed frame-copy
delivery is accepted. T65 admission follows as a separate governance step.

## Current Technical Baseline

- Source: T64 S2 delivery `21e0fe0`; narrowed frame copying, layout and workers unchanged.
- Both widths have 107 passing test cases after owner-authorized INI restoration
  and package recheck. See [S2 evidence](../history/M9-T64-S2-frame-copy.md).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- T63 snapshot behavior remains the owner-accepted baseline. Current package
  EXEs include T64 S2; INI is restored to its tracked configuration and was not committed.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T64 closure maps all admitted scope and owner-cancelled scope in its completion
audit. The remaining Queue order is unchanged.
