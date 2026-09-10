# M9 T48 S8 — Deterministic control matrix

## Original request

Prove the new control/runtime/presenter architecture with a readable,
deterministic matrix rather than relying on timing-sensitive manual discovery.

## Objective

Use controllable copied facts and completion barriers to cover control state,
presenter set, Current Console, monitor outcome/prompt, lifecycle request, and
input-source retirement. No test may use `Sleep` to establish a transition.

## Matrix

- initial start, running text, running graphics with `console_control=0|1`,
  static Window display, pause, resume, stop, reset-completed, and error;
- Window close while running and while paused;
- monitor command versus Window/raw Console hotkey completion origin;
- broker/component completion order, stale run facts, source retirement, and
  raw/cooked ownership handoff;
- presenter action at-most-once until its matching completion arrives.

## Verification

- Add a control-state unit matrix with fake copied VM/frame/component/broker
  facts. It validates actions and monitor effects without worker timing.
- Retain the existing real-thread library barriers and run fresh x64/x86 full
  CTest plus package builds.
- Run documentation governance, source-boundary, and manifest checks.

## Exit criteria

The architecture's control decisions are deterministic and covered at the
actual event boundary; both shipped-width packages are rebuilt and the tree is
clean for owner runtime acceptance.
