# M9 T48 S5 — Control desired / actual state ownership

## Outcome

The single control consumer now owns `app_control_state`: completed runtime
state, monitor stable state, latest copied frame route, component/broker actual
facts, derived presentation plan, and in-flight presentation action.

`app_presentation` contains only native Window/VM-Console handles, copied
frame-delivery bookkeeping, and actuator calls requested by control.  It no
longer stores a reconciler, lifecycle state, display route, desired plan, or
Current-Console policy.  `app_command_session` likewise no longer mirrors a
machine state; control passes its stable monitor fact into parser validation
and completion-output classification.

## Proof

- x64 and x86 fresh builds each passed 33/33 applicable CTest cases.
- Focused source audit confirms no `session.state`, presentation reducer, or
  presentation lifecycle/Current-Console query remains.
- The user-owned INI and MVDM source were unchanged.

## Closure

S5 closes with `bbeed65`.  S6 owns the remaining monitor outcome/prompt
transaction proof across all completion routes.
