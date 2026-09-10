# M9 T48 S7 — Presenter completion hardening

## Original request

Ensure that component and broker lifetimes cannot create a second product path:
all UX and host facts must arrive at control, stale facts must not mutate a new
run, and Window close must remain a request rather than a frontend action.

## Objective

Make presenter/broker completion application fail-closed and visibly one-way.
Control issues one action, waits for its matching completion fact, then derives
the next action.  Input from retired sources is rejected before guest delivery.

## Design

- Component create/destroy and broker binding remain synchronous native calls
  wrapped as copied control completions; their local handle is never treated as
  an actual product fact before control consumes that completion.
- Control rejects all nonzero-run facts whose run generation differs from the
  current runtime generation.  `SOURCE_RETIRED` still reaches control so its
  pressed-key ledger can be cleared, but no retired ordinary input reaches VM.
- Window close enters the control queue, requests pause when running, then the
  existing desired-plan override destroys Window only after paused completion.
- Presentation callbacks perform no lifecycle action, no broker replacement,
  no monitor output, and no direct guest injection.

## Verification

- Cover stale component/broker and UX facts, source retirement, Window-close
  pause/teardown order, and one-action-in-flight behavior.
- Source audit proves presentation has no lifecycle, monitor-output, or
  runtime-input call.
- Fresh x64/x86 full CTest and package refresh.

## Exit criteria

No late or stale completion/input can mutate a newer product path, and all
presenter/host callbacks have the control FIFO as their only product exit.
