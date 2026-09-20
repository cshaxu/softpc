# Project Status

## Current Work

No implementation subtask is active.
Open task awaiting owner: T72.

S1 design accepted; S2 implementation and review complete; S3 technical audit
is delivered for review. T72 is not formally closed. Both Release packages are
ready for owner testing; no next queued task is admitted.
See [readiness audit](../history/M9-T72-S3-completion-readiness-audit.md).

## Current Technical Baseline

- Code delivery 12bf7c96; S2 actual-change review recorded in
  [S2 history](../history/M9-T72-S2-neutral-text-migration.md).
- Final background suites: x64 105/105 (164.85s), x86 105/105 (147.06s),
  including headless Win3.1 PIF roundtrips, snapshots and restart.
  Five native desktop tests per width were not rerun in S2; no native Linux
  execution or whole-emulator correctness claim.
- VM owns character mapping. KVM Base transports opaque control FIFO and
  latest-wins complete frames; leaf presenters interpret their own frames.
  Window compares frames with its existing surface, not upstream dirty history.
- Renderer readiness, required fonts, terminal request completion, fixed-80 text
  layout and cursor normalization are recorded in the
  [T71 completion audit](../history/M9-T71-completion-audit.md).
- T72 uses neutral parallel text arrays, with device decoding only in VM and
  native Console colour encoding only in Broker. No frame transport change.
- TODO tracking is retired by owner decision, not proof of repair. Owner visual
  acceptance and formal T72 closure remain pending.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner approved. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner accepted. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; narrowed after audit. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit and narrowed S2 accepted. | [Audit](../history/M9-T64-completion-audit.md) |

## Recent Governance

- **M9 Td S18:** T71 closure audit, TODO retirement and new floppy-identification
  candidate second in queue; documentation only. P1 957c809d pushed;
  actual-change coordinator review and documentation checks passed; Td closed.
  [Record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
- **M9 Td S17:** Earlier text/frame proposal subsequently admitted as T71;
  [retained proposal](../history/M9-T71-kvm-text-frame-contract-proposal.md).
