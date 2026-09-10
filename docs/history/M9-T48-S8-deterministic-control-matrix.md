# M9 T48 S8 — Deterministic control matrix

## Outcome

The control-thread product facts moved out of `main.c` into the small,
worker-free `app_control_state` module.  It consumes only copied runtime,
frame, component, broker, and Window-close facts, and derives only presenter
actions.  Runtime command dispatch, native presentation work, and monitor
wording remain separate owners.

`control_state_matrix_smoke` exercises the completion barriers and the
Console/Window state matrix without a native worker or timing delay.  It
covers Console text, both graphical Console ownership modes, static Window,
pause, Window close, stop, reset-completed, resume, and stale frame sequence
rejection.

## Proof

- Fresh x64 CTest: 34/34 passed.
- Fresh x86 CTest: 34/34 passed.
- Documentation governance, source-boundary, and manifest checks passed as
  part of both full runs.

## Closure

The owner manually accepted the package behavior. S8 closes with `89be61c`.
