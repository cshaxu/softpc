# Project Status

## Current Work

No implementation subtask is active.
Open task awaiting owner: T64.
S2 is closed after dual-width verification and pushed delivery; T64 remains open
for owner testing. Thread unification was cancelled by owner decision.

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
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |
| T60 | S1–S7 complete; owner acceptance; D6 retained, dual-width 97/97. | [Audit](../history/M9-T60-completion-audit.md) |

## Recent Governance

T64 is admitted from the former Queue head. S1 source audit is recorded in
[history](../history/M9-T64-S1-lib-simplification.md). S2 implementation and actual
change review are complete; this does not close T64.
