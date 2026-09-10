# M9 T48 S5 — Control desired / actual state ownership

## Original request

Make SoftPC control the sole product-state owner.  Presentation and runtime
must be independent actuators: they receive commands and return copied
completion facts; neither stores a second product lifecycle or routing model.

## Objective

Introduce one app-owned control state that retains the completed VM state,
latest copied frame route, desired component set, actual component set, and
Current Console fact.  The control loop alone updates it from its FIFO and
derives two independent work streams: runtime commands and presenter/broker
actions.

## Boundaries

- `app_presentation` becomes an actuator and copied-frame delivery adapter. It
  may own native component handles and delivery sequence numbers, but not a
  product reconciler, lifecycle state, monitor state, or console policy.
- `app_command_session` remains monitor parsing/output policy, but its stable
  state is read from control rather than independently mirrored.
- The runtime and shared lib contracts remain unchanged.  No MVDM changes.

## Design

1. `app_control_state` is created alongside the control FIFO and is mutated
   only on the monitor/control thread.  It owns actual runtime state, latest
   copied frame route, component/broker completion facts, desired plan, and
   any in-flight actuator action.
2. On every FIFO fact, control updates actual facts, derives an
   `app_presentation_plan`, and issues at most one presentation/broker action.
   Presentation reports completion through the existing queue; it does not
   choose the next action.
3. A lifecycle request is control-private.  It is dispatched independently of
   presenter derivation; the only ordering rule retained is resume waits for
   the derived required presenter/Current-Console route to be actual.
4. Window close is an input fact.  Control requests pause when necessary, then
   applies the close override after the paused completion; it does not let a
   Window callback mutate lifecycle or destroy itself.

## Verification

- Unit-test the control state with copied facts: text/graphics transitions,
  `console_control=0|1`, pause/stop/Window-close, and stale component/broker
  completions.
- Prove presenter code contains no lifecycle request, monitor-state, desired
  plan, or product Current-Console derivation.
- Fresh x64/x86 full CTest and package refresh.

## Exit criteria

One control-owned actual/desired state drives both runtime and presenter work.
Presentation is an actuator only; all product decisions are made at the one
control queue consumer.
