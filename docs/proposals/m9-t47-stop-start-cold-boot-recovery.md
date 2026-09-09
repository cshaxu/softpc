# M9 T47 S9 — Stop/start cold-boot recovery

## Original request

After the monitor accepts `stop`, a later `start` must re-enter the configured
guest boot path. It currently can remain at BIOS.

## Objective

Repair the single standalone cold-run lifecycle so `running -> stop -> start`
fully re-enters the same configured boot path as the first `start`. The
runtime must not report a misleading successful restart merely because the
executor re-entered; guest timer, media, original reset, and executor-host
state required to progress beyond BIOS must be live for the new run.

## Baseline and suspected boundary

`app_runtime_start()` clears the app stop request, increments the SoftPC-only
run generation, and asks its persistent executor worker to call the public
`softpc_machine_reset()` boundary. Existing runtime smoke verifies it reaches
`RUNNING`, but uses a tiny synthetic image and does not prove a configured DOS
boot progresses past firmware. The observed package failure is therefore an
incomplete lifecycle proof, not permission to edit MVDM.

The audit must trace, on the executor thread, the order of: CCPU exit request
clear, cold reset, media reattachment, original timer reset/setup, standalone
heartbeat, executor callback, and first continuous `c_cpu_simulate` entry.
It must identify the real stale or missing fact before changing behavior.

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

1. Add focused executor-host observability/fakes or deterministic probes that
   distinguish a merely-entered `RUNNING` state from a second cold boot whose
   original timer/executor rendezvous is live.
2. Reproduce the `pause -> stop -> start` chain with configured installed
   boot media. A test-only paused snapshot may prove post-BIOS progress; the
   product frontend remains unable to inspect guest state.
3. Apply the smallest fix at the owning standalone runtime/host boundary and
   extend the existing `runtime_smoke` lifecycle chain to prevent regression.
4. Audit the adjacent `reset -> stop -> start -> pause -> resume` chain for
   the same stale lifecycle fact.
5. Build x64 and x86, run their full CTest suites and package smoke, refresh
   only the two agent-owned package executables, then retain evidence in S9
   history.

## Exit criteria

The configured package no longer stalls at BIOS after monitor `stop` then
`start`; deterministic regression proves post-BIOS progress on both cold runs
without assuming a fixed boot duration; MVDM is unchanged; x64/x86 full CTest
passes; all changes are committed and pushed with a clean worktree.
