# Base Event reuse

## S15 admission

Original owner request: 批准实施 完成后编译测试提交推送 目标是减少重复功能和代码 降低复杂度提高正确性和复用性。

Baseline: 1ae987f, clean main. S14 delivery remains in
[history](../history/M9-T59-S14-base-mutex-unification.md).
Only S15 is admitted. T59 stays open; S16 task/worker reuse is not admitted.

Base owns the reusable Event primitive. Extend its existing creation API with
explicit manual/automatic reset, and return checked signal/reset status.
Existing consumers retain manual reset; KVM's default notifier uses automatic
reset. Window keeps its selected native-message notifier without an Event.
Delete KVM's separate wake type, platform implementations and forwarding getter.
No mailbox policy, FIFO, STOP, frame/control lock, worker or product change.
KVM Console directly consumes Base's public wait contract; declare that edge.
Do not add a second notification path or another wrapper allocation.

## Finite convergence ledger

| Member | Required result | Proof |
| --- | --- | --- |
| Base Event | Two reset modes; checked native signal/reset; failure leaves create output null | Win32 actual primitive plus injected failures; Linux controlled waits |
| KVM wake | Default notifier directly owns Base auto-reset Event; no duplicate primitive | One-time selection, signal before/during wait, timeout/failure, STOP/retirement |
| Existing consumers | Explicit manual reset, unchanged sequencing | Common/task/audio call-site scan and dual-width regressions |
| Boundaries | Removed wake API/sources/Threads edge; direct Base consumer declared | Source/build DAG and Linux build checks |
| Delivery | Both EXEs, complete manifests, source/test accounting, P1 push then coordinator review/P2 push | Full x86/x64 tests, docs gate, clean tree |

Signal/reset failure propagation already checked by KVM must remain checked.
Existing manual-event callers keep their present lifecycle/error policy;
this task does not introduce a product failure state machine. All Event calls
and duplicate primitive implementations form the similar-issue sweep universe.
Host native reader cancellation and Window startup/message primitives retain
their distinct ownership and are outside this default-mailbox migration.

## Remaining plan

S16 will separately resolve bounded task completion/join/disposal before KVM
worker reuse. It requires owner admission. Do not move native I/O cancellation
or Window message-loop policy into Base.

## S15 implementation and bounded sweep

Base Event now takes an explicit reset mode and signal/reset return lib_status.
Win32 checks SetEvent/ResetEvent; Linux checks lock/broadcast/unlock and retains
one monotonic deadline across spurious wakeups. The existing process-lifetime
wait-any predicate lock is the only Linux Event implementation. No new object
wrapper, state machine, retry or product error protocol was added.

KVM removes mailbox_wake_interface.h and both platform mailbox.c files.
The default notifier owns one Base auto-reset Event and reports the checked
signal result through the existing accepted-request failure path. Console waits
on it directly. The old pointer getter is deleted. Window native messages,
mailbox lock scopes/FIFO/latest-wins/STOP and native worker ownership are intact.

Creation call-site sweep: six Common machine events, one session queue event,
two Compat audio events and Base task cancellation all explicitly retain manual
reset. Common/Compat changes are only those arguments. Existing manual callers'
failure policy is not redesigned; KVM continues to check notification/wait faults.
Base's native Event, Host reader stop Event and Window startup-ready Event are
the remaining Win32 primitive hits. The latter two retain native I/O/startup
responsibility outside this default-mailbox migration. Host's atomic process
claim is not a spinlock. No KVM pthread mutex/condition implementation remains.

Tests reuse existing suites: manual persistence/reset, automatic signal
coalescing/consumption, invalid arguments and Win32 create/signal/reset failures;
Linux condition preparation failures, signal before/during wait, spurious returns,
fixed deadline/infinite wait, timeout and checked failure; mailbox one-time
selection, no Window Event allocation, failed default selection and retirement
after wait/notification faults. Three negative fixtures prevent old wake files
returning. The full 64-edge source/build DAG admits Console's direct Base use.
Linux Threads linkage is solely Base-owned, not propagated by Types or KVM.

Initial x64 full regression found a stale product-test DAG assertion (91/92,
81.75 seconds). The product test already invokes the complete shared DAG check;
deleted its duplicate subset rather than maintaining another edge map. Its
focused rerun passed. This is a test-authority correction, not a runtime fix.
A direct script diagnostic without its CTest context was invalid; only the
normal registered test is counted as proof.

Count method: git diff 1ae987f --numstat, tracked src/test C/H only.
Production 14 paths: +63/-206, net -143. Tests seven paths: +101/-67, net +34.
Docs, CMake/gates, manifests and fixed binaries are excluded. S14 history is
preserved unchanged. Four shared manifests advance to shared-t59-s15-p1.
No App/VM/MVDM, INI or media changes; no new disposable build/media directory.
Native Linux execution is not claimed; its production wait algorithm is covered
through controlled platform fakes.

## S15 executor verification

All five ledger members are implemented and verified. Strict GNU Lib builds
and complete package/test builds pass for both widths. Final full regression:
x86 92/92 (67.94 s); x64 92/92 (48.85 s), run sequentially for native UI isolation.
The initial x64 stale-DAG failure remains recorded above. Native modal coverage
passed this time; S14's separate intermittent-test TODO is not claimed fixed.
Four shared manifests and documentation gate pass; diff whitespace check passes.
Manifest entry order is retained to avoid unrelated diff noise.

Fixed tested outputs:

- softpc32.exe: 3,485,052 bytes,
  78B155A9C0598612937C9316AE816E062EDF2E302513559F1DA789C126517E7A.
- softpc64.exe: 2,844,671 bytes,
  BBD601897615097C69A8BE014317545EAA07D699252A7E7BCCD5B0D44F9134C7.

Both packages are smaller than S14. Existing build trees are retained for
reproducible testing; no new scratch media or recording remains. Executor P1
will deliver code/tests/docs/manifests and these binaries together, then switch
to coordinator review of the actual commit. Await owner testing afterwards.
