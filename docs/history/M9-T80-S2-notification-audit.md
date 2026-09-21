# M9 T80 S2: notification failure reachability audit

## Owner decision and boundary

Original request: “收口s1 准入s2”; the owner subsequently requested a concrete
scheme, questioned its complexity, approved a narrowed read-only audit with
“可以 开始审计”, and accepted its report with “批准收口”.
The [proposal](../proposals/m9-shared-corpus-boundary-and-simplification.md)
retains the original objectives and their explicit supersession. No new fatal
callback, alternate event, polling, request state machine or Lib change is
admitted. S2 closes as an audit, not as comprehensive notification-fault recovery.
T80 remains open. The owner separately requested “下一个s任务准入” during the
closure handoff; CURRENT therefore admits S3 without starting its implementation.

## Finite audit ledger

Baseline: `1b5d0fd7`. Universe: Common Session queue push/latch/take/dispose;
Machine lifecycle, input, media, debug and state-I/O submit/service/completion,
executor exit and shutdown; Base event/task lifetime and Windows/Linux backing
implementations; App composition teardown and snapshot context release.

| Path | Finding and disposition |
| --- | --- |
| Session queue | Push, fault latch and last-item reset use the same queue mutex. No normal lost-wake interleaving was found; allocation failures retain existing fixed fault slots. No new queue mechanism. |
| Machine submit/claim | One control caller serializes lifecycle/media/debug/state payloads; atomic request flags publish to the sole executor. request_lock serializes registration against terminal cleanup, not arbitrary concurrent callers. No new replay path found under that contract; do not turn it into an unrequested multi-caller API. |
| Lifecycle/input wakes | Reviewed command/resume/input reset and predicate order, outer and paused waits, and driver wake/stop ownership. Existing cancellation-aware outer wait covers the prior debug-close/shutdown wake loss. No new valid-object wake loss established. |
| Completion and driver failure | Media/debug complete after their driver calls. State completion or terminal cleanup releases pending waiters after the relevant driver work; existing frame-failure and exit tests exercise this path. No new normal borrowed-context release defect established. |
| Teardown | App joins Machine while callback targets remain alive, then destroys UI and Session. Base task destruction retains ownership on failed cancel/join. Events are disposed after worker shutdown; no normal premature event destruction found. |
| Native signal/reset failure | Ignored results remain. Neither return-value checks nor re-enqueuing a fault on the same failed event guarantees wakeup. No valid-object normal-use trigger was established; broad recovery redesign is cancelled, not reported fixed. |
| App snapshot wait failure | app/command.c closes reader/writer after every returned status; Common's state-I/O contract requires retaining context when native wait failure has not established completion. This conditional mismatch remains, but no normal-use native-wait failure trigger was found. Owner accepts the disclosed limit without a new recovery mechanism or hidden follow-up task. |

Searches included `rg -n 'base_sync_(event_(signal|reset|wait|destroy)|wait_any|task_(request_cancel|destroy|join))' src/common`,
and state read/write, shutdown and writer/reader references in App/Core driver.
Source review included the existing wait, queue-failure and native-machine test
implementations; repeated tests are not a proof of all possible interleavings.

## Verification

Existing builds, no recompilation or new fault injection:

- `ctest --preset test-x64 -R '^common\.' --output-on-failure`: 18/18, 8.97 s.
- `ctest --preset test-x86 -R '^common\.' --output-on-failure`: 18/18, 9.12 s.
- On each preset, `-R '^common\.(common_machine|machine_wait|session_queue_failure)$' --repeat until-fail:20 --output-on-failure`: all three cases pass all twenty repetitions (60 executions per width).
- Common manifest, corpus and negative verifier checks are included in the
  eighteen-case suite. No new full-product, desktop or Linux runtime claim.

Production/source/test/build changes: +0/-0, net zero. Package EXEs, their S1
hashes, INI and media are unchanged. Closure changes only CURRENT, Queue, the
proposal and this audit record. No task-specific diagnostics were created.

## Closure review

Owner acceptance explicitly replaces the original implementation/fault-injection
exit criteria with the bounded audit and retained limitations above. This is
not evidence that arbitrary synchronization infrastructure failures are safe.
Review of the closure diff confirms documentation-only changes; S3 admission
has separate owner authorization. Native failure evidence or a reproducible lifetime
violation would require fresh owner admission rather than speculative recovery.
