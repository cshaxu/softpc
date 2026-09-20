# Common Machine terminal wake ownership

## Absorbed By T75 S2

Owner admitted this repair through the [T75 plan](M9-T75-shared-corpus-quality-proposal.md).
The evidence below is historical intake, not a second active task or queue item.

Found while separating T73 S5 tests. No production change is authorized by the
test-corpus packet. This candidate owns investigation and a minimal fix for
closing a debugger and immediately destroying a paused Machine. It is not a
receiving-emulator task and does not change the copied debug protocol.

## Evidence and suspected cause

The newly separated x86 native-thread test intermittently timed out after all
CLI assertions and debug close/destroy completed. Both x64 and x86 full suites
observed the timeout. A repeated x64 run reproduced it; debugger attachment
showed the caller waiting in base_sync_platform_task_join through
common_machine_destroy, while the worker waited in base_sync_event_wait from
common_machine_worker. The fixture and runtime C/H were otherwise unchanged.

Source inspection suggests terminal command/resume notifications can be consumed
by the paused callback before the worker returns to its outer unconditional
command-event wait. That wait has no task cancellation argument. Confirm the
precise interleaving with a deterministic scheduling probe before modifying it.
Do not treat a successful retry as proof of safety.

T73 S5 retains the original mixed test's stop-completion barrier before disposal;
the pre-existing neutral active/paused shutdown tests remain enabled. This does
not fix or certify the newly observed immediate-close/destroy interleaving.

## Proposed bounded implementation

Keep one executor and one shutdown path. Audit cancellation and terminal-wake
ownership at the paused callback and outer worker wait; prefer an existing
cancellable wait contract or equivalent race-free termination boundary. Do not
add polling, sleeps, retries, timeout-based disposal, another thread or object.
Lib need not change unless the investigation proves its existing contract is
insufficient; any such expansion requires separate approval.

## Finite acceptance ledger

- Deterministic cancel-debug/paused/shutdown interleaving reproduces the old hang
  and completes with the fix; repeat native-thread tests on both widths.
- Stopped, running, paused, never-started, terminal-error and blocked-final-callback
  shutdown retain their existing ownership and exactly-once completion rules.
- Pending media/debug/snapshot callers finish before worker resources are freed.
- Full dual-width background regression, clean commits/push, two test EXEs and
  added/deleted/net accounting; manual acceptance before closure.

No source fix, runtime behavior change or task number is assigned here.
