# M9 T47 S9 — Stop/start cold-boot recovery

## Original request

After the monitor accepts `stop`, a later `start` must re-enter the configured
guest boot path. It currently can remain at BIOS.

## Objective

Repair the one standalone `running -> stop -> start` product path so the
second run both re-enters the configured boot path and replaces the former
run's visible presentation fact. An executor/IP observation alone is not
sufficient: the new run must publish a copied frame with its own SoftPC run
generation, and the app reducer must rebuild its route from that new frame
rather than retaining an old Window/Console image.

## Baseline and suspected boundary

`app_runtime_start()` clears the app stop request, increments the SoftPC-only
run generation, and asks its persistent executor worker to call the public
`softpc_machine_reset()` boundary. Existing runtime smoke verifies it reaches
`RUNNING`, but uses a tiny synthetic image and does not prove a configured DOS
boot progresses past firmware. The observed package failure is therefore an
incomplete lifecycle proof, not permission to edit MVDM.

The audit must preserve T44's approved `soft_reset = 0` public cold-start
policy. The earlier S9 claim that changing this original internal fact solved
the issue is superseded: it did not change the owner-observed failure. The
audit must instead trace the completed runtime frame, run generation, and app
route after the second start.

## Boundaries and non-goals

- May modify only standalone `src/app/`, `src/host/`, focused tests, task
  documentation, and package executables.
- Must not modify `src/mvdm/softpc.new/**`, interpret guest BIOS/DOS state in
  the frontend, create a second executor, or add a second restart path.
- `stop`, `start`, and `reset` retain the approved monitor matrix. A start is
  still a cold run; guest Ctrl+Alt+Del remains a distinct machine warm reset.
- This task does not alter lib UX, Current Console ownership, or user-owned
  `assets/binary/softpc.ini`.

## Implementation and proof

1. Prove with configured installed media that both runs leave firmware and
   each commits a copied frame tagged with its own run generation.
2. Invalidate the completed snapshot at the explicit new-run boundary; retain
   monotonic frame sequence identity, but never let a new run inherit the old
   frame as its current output.
3. Prove the reducer retires a stopped graphical route and waits for the new
   run's first frame before rebuilding a text/graphic route.
4. Audit the adjacent `reset -> stop -> start -> pause -> resume` chain for
   the same stale presentation fact.
5. Build x64 and x86, run their full CTest suites and package smoke, refresh
   only the two agent-owned package executables, then retain corrected S9
   evidence in history.

## Exit criteria

The configured package no longer retains a prior-run display after monitor
`stop` then `start`; deterministic regression proves post-BIOS progress and a
new-generation published frame on both cold runs; MVDM is unchanged; x64/x86
full CTest passes; all changes are committed and pushed with a clean worktree.
