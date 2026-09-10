# M9 T48 S6 — Monitor completion transaction

## Original request

Every VM state change must produce its human-facing status only after control
receives the VM completion.  When the cooked monitor is Current Console it
must print the status, print a fresh prompt, and arm exactly one cooked line.
Window and raw-Console hotkeys follow the same rule as monitor commands.

## Objective

Make one completion outcome transaction in control for started, resumed,
paused, stopped, reset-completed, and error.  No command source, presenter
callback, or broker callback may independently print lifecycle text or arm a
prompt.

## Boundaries

- SoftPC app control, command output transaction, focused tests, and agent
  executable packages may change.
- Runtime emits copied facts only; lib and MVDM do not change.
- The monitor owns literal product text, but not VM state.

## Design

1. The control state classifies runtime completions from the prior stable
   actual state. `INIT|STOPPED → RUNNING` is started; `PAUSED → RUNNING` is
   resumed; runtime's `RESET_COMPLETED` is reset-paused.
2. Control stores at most one pending monitor outcome and prompt demand.  It
   performs output only after the broker completion proves monitor is Current.
3. A settled raw VM-Console route clears monitor-only running outcome/prompt
   demand. It never prints the old outcome at a later handoff.
4. Every public terminal completion, including error, clears the transition
   gate and follows the same outcome decision. UX hotkeys only produce control
   requests; they never print status.

## Verification

- Deterministic command/control tests cover each completion state through
  monitor command and external hotkey sources.
- Assert exact monitor ordering: outcome, `SoftPC> `, then one arm request.
- Assert raw route discards started/resumed and later monitor handoff gets only
  a fresh prompt where product policy requires one.
- Fresh x64/x86 full CTest and package refresh.

## Exit criteria

Every VM completion has one control-owned output decision, no stale outcome is
replayed across Console ownership, and all accepted terminal transitions return
to an armed monitor prompt when monitor is Current.
