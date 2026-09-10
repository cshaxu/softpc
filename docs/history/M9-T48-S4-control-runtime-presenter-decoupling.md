# M9 T48 S4 — Control / runtime / presenter decoupling

## Outcome

S4 removed the shared `app_reconciler_intent` route.  Monitor parsing now emits
an app-private lifecycle request; the one control consumer dispatches it to
runtime independently of presenter reconciliation.  The presentation reducer
now derives only component and Current-Console work from completed facts.

Runtime owns reset as one atomic command.  Its prior run is retired internally,
then a fresh cold run pauses at its first executor boundary and emits the
completion-only `RESET_COMPLETED` fact.  Control exposes neither an internal
reset stop nor an internal running state.

Lifecycle monitor text is completion-driven.  `RUNNING` is classified from the
prior stable state, so monitor `resume` and Window/raw-Console CAP both produce
`Machine resumed.` only after runtime reports running.  Reset, paused, stopped,
and error follow the same completion path.

## Boundaries

No MVDM or shared-lib semantics changed.  Runtime and app code, focused tests,
the agent-owned executable pair, and governing documentation changed; the
user-owned package INI and guest media did not.

## Proof

- `app_reconciler_intent` and its old runtime-action route are absent from
  `src/` and `test/`.
- The command matrix covers all twenty stable command cells, explicit runtime
  reset completion, and a hotkey-origin resumed completion.
- Fresh x64 and x86 builds each passed 33/33 applicable CTest cases; package
  smoke is excluded because the user-owned INI selects the Window route.
- Documentation governance verification passed.

## Closure

S4 closes with commit `6eb0380`.  S5 owns the remaining relocation of
actual/desired component facts from the presentation adapter into control.
