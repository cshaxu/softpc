# Machine paused wait failure

## S19 admission

Original owner request: 准入一个s任务修复 完成后编译 测试 提交 推送 并汇报代码净增减

M9 T59 S19 continues from clean main e15b70e. S18 evidence is retained in
[history](../history/M9-T59-S18-console-broker-naming.md). T59 remains open.

The paused executor currently retries every non-signaled wait result. A
persistent infrastructure failure therefore spins instead of ending the run.
Distinguish successful wake, cancellation and failure. On failure stop the
driver, suppress false RUNNING and reset continuation, and use the existing
outer cleanup and ERROR notification. Cancellation is not an error. Do not
add retries, timers, state machines or public APIs. Preserve normal lifecycle.

Scope: Common Machine wait handling, shared deterministic tests, manifests,
task records and the two fixed EXEs. Lib, App, VM, Compat, MVDM, INI and media
are unchanged. Inspect all Common waits for the same failure/retry class;
record each disposition before claiming completion.

## Finite convergence ledger

| Member | Required result | Proof |
| --- | --- | --- |
| Paused failure | One wait, stop driver, one ERROR after cleanup; no resumed fact or reset | Deterministic injected wait results |
| Cancellation and success | Cancellation stops normally; resume/stop/reset still work | Same fixture and existing Machine integration suite |
| Similar waits | All Common wait sites accounted for; no failure retry | Source sweep and focused assertions |
| Delivery | x86/x64 build/full tests, manifests/docs, fixed EXEs | Executor P1 push then actual-change coordinator P2 |

Use existing build trees, no new media/trace artifacts. Report tracked production
and test C/H additions/removals against e15b70e separately; CMake/docs/artifacts
are excluded. Stop if public lifecycle or platform contracts require redesign.
No owner manual acceptance is claimed by automated tests.

## Executor implementation and similar-issue sweep

Only machine.c changes in production: keep the wait result, distinguish
cancellation from fault, cancel any reset continuation, request driver stop
and leave the paused loop. The existing stop flag prevents a false RUNNING
notification. After driver unwind, existing cleanup disables heartbeat,
detaches the executor callback and invalidates debug access; the final result
also checks the retained ERROR state before choosing STOPPED or resetting.
No field, helper layer, public API, timer or retry is added.

The new shared machine_wait fixture runs the real worker/callback synchronously
with scripted scheduling waits and the existing Base events/mutexes. Nine
cases cover FAULT, INVALID_ARGUMENT, unexpected infinite-wait TIMED_OUT,
CANCELLED, failure/cancellation concurrent with an accepted reset, and normal
resume/stop/reset. It checks one failed wait, driver stop, cleanup before final
notification, exactly one terminal fact and no accidental reset/resume.
No Sleep or timing assertion is used; existing real-thread Machine tests remain.

Red/green proof: temporarily restoring the old paused-wait continue makes
common.machine_wait fail its second-wait assertion immediately. Restoring the
fix passes. No baseline mutation remains in the delivered source.

Sweep command: rg -n 'base_sync_.*wait|WAIT_SIGNALED' src/common. Five production
wait sites are accounted for: paused wait is repaired; idle Machine command
wait already breaks (no retry); media wait returns false; debug wait returns
IO_ERROR; Session queue wait returns false and Session run exits. The latter
already has an injected one-wait regression. This is a bounded failed-wait
retry audit, not a claim to redesign all asynchronous request failure modes.

Production C/H accounting against e15b70e: machine.c +12/-3, net +9.
The only new test source is machine_wait_smoke.c (+138/-0); test registration
is one additional CMake line. C/H total is +150/-3, net +147; including that
CMake line gives net +148. Documentation, manifests, archive move and EXEs are
excluded from code counts. App/Lib/VM/Compat/MVDM and INI/media are unchanged.

## Executor verification

Both tests-x86/tests-x64 builds passed. The changed production and test source
also passed both GCC widths with -std=c17 -Wall -Wextra -Wpedantic -Werror
-fsyntax-only. Full suites: x86 94/94 (66.78 s), x64 94/94 (75.52 s).
All four manifests, Common corpus, documentation and whitespace gates pass.
No native Linux runtime or owner manual acceptance is claimed.

Fixed outputs: softpc32.exe 3,484,363 bytes, SHA256
B8CBC2810E74B704867959A0AEE133156C8C4CA207A39081CD1E5712D040A5E9;
softpc64.exe 2,843,985 bytes, SHA256
02FB33A385B99CB02D652F0DD7030C50CDAF1356EFCE43C585DE489D80708360.
Existing build trees remain; only regenerated types-layout test fixtures are
removed after tests. No new build tree, media, trace or worker is retained.
All four convergence members are verified; P1 delivers these changes for
actual-commit coordinator review. T59 stays open for owner testing.
