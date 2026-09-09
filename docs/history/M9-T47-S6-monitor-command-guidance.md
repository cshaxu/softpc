# M9 T47 S6 — Monitor command guidance

## Outcome

Every unavailable monitor lifecycle command now tells the user which lifecycle
commands are usable in the current stable state:

- init/stopped: `start` or `reset`;
- paused: `resume`, `reset`, or `stop`;
- running: `pause`, `reset`, or `stop`.

The command reducer still returns no intent for those cells, so this is only a
local monitor-message improvement. The exact guidance is part of the existing
twenty-cell deterministic command matrix test.

## Evidence

- `softpc-monitor-command-smoke` passed with all twenty exact intent/message
  cases.
- Fresh x64 and x86 package builds each passed full CTest, 33/33.
- Documentation governance verification passed.

## Closure

This changes no lifecycle behavior, MVDM source, shared lib, host/UX,
guest media, or user-owned package configuration. T47 remains active; any
later subtask requires separate owner admission.
