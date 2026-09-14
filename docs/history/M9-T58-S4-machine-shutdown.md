# M9 T58 S4: Machine Shutdown And Ordered Disposal

Owner approved the minimal synchronous shutdown design: “好的。批准。开始执行。”
The [proposal ledger](../proposals/m9-common-test-sync.md) retains the scope,
similar-issue sweep, implementation accounting and full verification evidence.

Coordinator reviewed pushed executor 8dbe551 against f30b6c5, including the
actual production/test diff, callback lifetime, create-failure disposal and
post-shutdown admission paths. Shutdown extracts existing stop/join; destroy
delegates to it. No new state, thread, lock, queue or parallel cleanup route.
The owner must serialize shutdown and never invoke it from a worker callback.

App first joins machine callbacks with their targets alive. It then destroys
UI, session, command/debug, machine and VM. UI retirement still has a live
session, and debug cleanup still has live machine storage. The worker pointer
is cleared after join; repeated shutdown is harmless. Cold-run/media requests
reject the missing worker rather than waiting for an operation with no executor.
Product stop remains restartable. Partial creation still frees all initialized
resources; worker creation is last.

Shared machine tests block the final state callback during running and paused
shutdown and prove the shutdown thread cannot finish until that callback is
released. Never-started, stopped, repeated shutdown and subsequent destroy are
covered; notification count remains unchanged after quiescence. Existing normal
lifecycle/debug coverage remains. The product boundary gate checks teardown order.

Dual builds completed. Full x64 passed 85/85 (88.85 s), x86 85/85 (94.64 s).
The initial x86 parallel build command's diagnostics-free exit -1 is retained
in the proposal; serial rebuild was confirmed exit 0 without source changes.
Manifest and documentation checks pass. Both fixed EXEs are delivered; no
INI/media change. Lib, VM, Compat, MVDM and test/lib have zero diff.

Production C/H +21/-7 (net +14), shared test +64, product gate +3; documentation
and hashes are separate. All S4 ledger entries are complete. S4 closes;
T58 stays open awaiting owner testing/direction, with no next S admitted.
