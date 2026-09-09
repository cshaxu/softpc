# M9 T44 — Cold Restart Lifecycle Repair

## Objective

Make the monitor's `start` and `reset` commands match their product contract:
each begins a cold machine reset.  A stopped machine must start a fresh running
run, and a reset run paused at firmware entry must resume normally.

## Scope

- In the standalone host boundary, make the public machine reset operation a
  cold reset on every invocation.  Preserve the recovered machine's own
  hardware warm-reset route (including guest Ctrl+Alt+Del).
- Add an actual executor regression using the existing test-local machine:
  `stop -> start -> running`, then the reconciler's
  `reset -> stop -> start -> pause -> resume -> running` chain.
- Build, run asserted x64/x86 tests, refresh the two package executables, and
  preserve the user-owned package INI and all guest media.

## Boundaries

- May change: `src/host/machine.c`, application/runtime tests, active task
  state, and agent-owned package EXEs.
- Must not change: `src/mvdm/softpc.new/**`, lib Console/UX behavior, guest
  media, or `assets/binary/softpc.ini`.

## Exit

The public reset operation never selects the original warm-reset branch;
guest-originated warm reset remains unchanged; the two real execution chains
reach their stated completed runtime states at both widths; full asserted
regressions and package builds pass; worktree is clean except any user-owned
INI change.
