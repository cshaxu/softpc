# M9 T48 S3 — Paused monitor prompt completion

## Original request

After the monitor pauses a machine, it prints `Machine paused` but does not
show or arm the next `SoftPC>` prompt.

## Objective

Make every accepted lifecycle turn complete exactly once at its settled public
state. When the monitor owns the Current Console, control publishes its
completion text, then publishes and arms the next cooked prompt. This includes
`Machine started.` and `Machine resumed.` on a monitor route, alongside paused,
stopped, and reset-paused outcomes.

## Boundaries and non-goals

- May change the SoftPC app command/monitor/reconciler integration, focused
  tests, task documentation, and agent-owned package executables.
- Must not change MVDM, generic lib/host Console semantics, guest input, or
  user-owned package configuration.
- This is not a new prompt/input path and does not change the lifecycle command
  matrix.

## Design

1. Runtime only reports copied state completion to SoftPC control. It never
   writes monitor text or arms input.
2. The command session retains only a pending product outcome plus
   `prompt_due`; it no longer mirrors host-reader state. The generic broker is
   the single authority for its one idempotent cooked reader.
3. After control has reconciled a completion, and only once the monitor is the
   actual Current Console, it performs one transaction: write pending outcome,
   write `SoftPC> `, then request one cooked line. A failed write/arm is a
   control-path failure; success clears the pending outcome/prompt demand.
4. If a settled running route is raw VM Console, control discards a pending
   started/resumed monitor outcome rather than printing it later on an
   unrelated handoff. Reset's internal stopped/running facts produce neither
   outcome nor prompt; its final paused fact produces the reset-paused outcome.

## Verification

- Reproduce an outstanding Window-monitor reader followed by CAP pause and
  assert the final paused outcome and a fresh prompt arm follow in order.
- Cover started and resumed outcomes when monitor is Current, and prove they
  are discarded for a settled raw VM Console route.
- Cover both display routes where the monitor is Current Console.
- Build x64/x86, run applicable CTest, refresh both package executables, and
  manually verify pause followed by a second monitor command.

## Exit criteria

An accepted pause reliably returns to an armed monitor prompt after its one
settled paused completion; no second intent is dispatched; both widths pass
their applicable tests; changes are committed and pushed from a clean tree.
