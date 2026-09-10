# M9 T47 S6 — Monitor command guidance

## Objective

When a monitor lifecycle command is unavailable, its local response must name
the lifecycle commands that are usable in the current stable state. The reply
remains product text owned by SoftPC; it never changes the lifecycle matrix or
enqueues an intent.

| State | Available lifecycle commands | Rejected-command guidance |
| --- | --- | --- |
| init | `start`, `reset` | “not started; use start or reset” |
| stopped | `start`, `reset` | “stopped; use start or reset” |
| paused | `resume`, `reset`, `stop` | “paused; use resume, reset, or stop” |
| running | `pause`, `reset`, `stop` | “already running; use pause, reset, or stop” |

## Boundaries and exit

- May change only the pure SoftPC monitor command reducer, its exact-message
  matrix assertions, current UI wording, task record, and package executables.
- Does not change MVDM, runtime/reconciler intent behavior, host/lib/UX,
  media, or user-owned `assets/binary/softpc.ini`.
- All 20 command/state matrix cases must assert the revised message where
  rejected. Fresh x64/x86 full CTest must pass before closure.
