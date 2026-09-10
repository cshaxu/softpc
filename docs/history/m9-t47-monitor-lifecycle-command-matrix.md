# M9 T47 S5 — Monitor lifecycle command matrix

## Objective

Make monitor lifecycle commands total over the four stable product states:
`init`, `stopped`, `paused`, and `running`. A command must either produce one
specific lifecycle intent or produce one local explanatory response; it must
never silently replace an incompatible intent and leave the monitor without a
prompt.

| Command | init | stopped | paused | running |
| --- | --- | --- | --- | --- |
| `start` | cold start → running | cold start → running | local rejection: use `resume` | local rejection: already running |
| `pause` | local rejection: not started | local rejection: stopped | local rejection: already paused | pause → paused |
| `resume` | local rejection: not started | local rejection: stopped | resume → running | local rejection: already running |
| `reset` | cold start → paused | cold start → paused | cold reset → paused | stop → cold start → paused |
| `stop` | local rejection: not started | local rejection: already stopped | stop → stopped | stop → stopped |

## Design

Extract a small pure `app_monitor_command` reducer from `main.c`. It receives
one parsed lifecycle command and the current stable monitor state, and returns
exactly either an `app_reconciler_intent` or a fixed local message. It has no
runtime pointer, UI object, Console, queue, or async side effect. `main.c`
remains the one control-loop caller: it writes a local message and rearms the
prompt, or submits the returned intent and drives the reconciler.

`init` is distinct only for human-facing feedback: it is the monitor's state
before the first runtime completion. It shares start/reset mechanics with
`stopped`. Pending runtime transitions do not arm another cooked line, so a
second lifecycle command cannot be admitted until the current completion has
returned and the monitor publishes its next prompt.

## Boundaries

- May change: `src/app/main.c`, a narrow pure app command module, CMake, unit
  tests, current task record, closure evidence, and package executables.
- Must not change: `src/mvdm/softpc.new/**`, the reconciler's cold-reset
  sequence, host/lib Console behavior, UX policy, guest media, or user-owned
  `assets/binary/softpc.ini`.

## Verification and exit

- A deterministic unit matrix asserts all twenty command/state cells,
  including exact intent versus local-rejection classification.
- Similar-issue sweep confirms incompatible lifecycle commands cannot replace
  an active intent or strand the monitor prompt.
- Fresh x64/x86 builds and full CTest pass; package executables refresh.
- Each P is committed and pushed; closure records exact evidence and leaves a
  clean worktree.
