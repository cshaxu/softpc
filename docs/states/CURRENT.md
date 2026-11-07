# Project Status

## Current Work

Owner accepted and closed T99 S36 on 2026-11-07. No implementation subtask is active.
Open task awaiting owner: T99. No successor S is admitted.

## Current Technical Baseline

S36 delivery 8c27a3b6 is owner-accepted and closed. CHAIN2 DWORD
readers and their odd mode-1 WORD dependency are repaired in one mirror file;
production +12/-20, existing test +33/-0. Both published EXEs and all 388
architecture/test cases are qualified, including the corrected x64 documentation
rerun. See [S36 evidence](../etc/evidence/softpc/m9-t99-s36-chain2-dword-read.md).
Public eight-tree content remains byte-identical to NXVM (296 files). S34's
import delivery remains intact; S35 was withdrawn without a code delivery.
Two independent findings are recorded in TODO, not concealed as passing coverage.
T99 remains open.

## Recent M9 Closures

See [S36 closure](../history/M9-T99-S36-chain2-dword-read.md) and
[S33 delivery](../history/M9-T99-S33-windowed-dos.md).

| Task | Closure |
| --- | --- |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

Follow existing build cache and task-owned probe cleanup rules.
