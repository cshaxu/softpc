# Session event-driven idle wait

## Admission and boundary

T59 S12, clean baseline fa14c62. Owner admits the selected Session 100 ms
timeout repair, dual-width build/test, commit/push and manual-test packages.
Use architecture/coding/execution/documentation governance. T59 stays open.

Original request: "准入一个s修复编译 测试 提交 推送 等我" for the selected
100 ms Session polling finding. Follow-up: "要求在s12内彻底正确修复" the
startup-test failure found during verification.

The sole Session consumer has no periodic maintenance. Use LIB_UINT32_MAX for
its queue wait. With no normal timeout, a failed take returns through the
existing session failure path; it must not retry. Preserve the bounded/zero
wait helper for tests. No new wait API, result enum, thread, state or timer.
Queue publication, locking, FIFO, prompt and completion semantics stay intact.

## Finite convergence ledger

| Member | Required proof | Disposition |
| --- | --- | --- |
| Idle wait | Actual run loop requests infinite wait | verified by session_monitor |
| Wait failure | Inject failed take; run returns failure after one call | verified by session_monitor |
| Event delivery | Existing monitor exit plus concurrent producer FIFO using infinite wait | verified by session_monitor/sync |
| Bounded sweep | All Session take callers and signal/reset paths reviewed | verified below |
| Package startup | Correct native test fixture capacity without weakening product output contract | verified by both full package suites |

Owner subsequently requires the reproduced package startup failure to be fixed
within S12. Extend the bounded ledger to its confirmed failure path and regression
proof; update affected owners on diagnosis, preserving existing public behavior.
INI/media and unrelated polling remain out of scope. Temporary failure-only
diagnostics may use build/s12-wait-probe.log (under 16 KiB, one test at a time,
test timeout enforced); remove diagnostic code/log before final delivery.

Executor completes the finite ledger, both full suites and manifests/docs gates,
updates packages and commits/pushes P1. Independent same-agent coordinator then
reviews the committed patch and records/pushes P2. Report production/test line
accounting separately. Leave a clean tree and wait for owner manual testing.

## Implementation and bounded proof

session.c changes one statement and adds its rationale comment. The helper's
bounded/zero-timeout behavior remains available to tests, but Session production
has no timeout outcome to distinguish from idle: false now terminates the run.
App already maps that failure to its I/O diagnostic and ordered shutdown; no
new failure callback or App change is needed.

session_monitor checks the real loop's infinite-wait argument on every take,
normal rejected/exit line consumption, and an injected failed take that must
return after one call without rearming the reader. sync uses the real queue
and two concurrent producers for 2000 FIFO events with infinite consumer wait;
CTest's existing 30-second watchdog bounds a lost-wake regression. No Sleep or
wall-clock performance threshold is added. Both focused suites pass 2/2.

rg over src/common/session and all queue_take callers confirms one production
consumer, no timer maintenance and no second production polling path. Normal
and latched-failure publication signal while holding the queue mutex; the sole
consumer resets only after draining pending records under that same mutex.
Windows passes the infinite value to its existing wait; Linux selects its
existing condition wait. No platform code changes. No additional in-scope defect
was found; this is not a claim about other components' polling.

Against fa14c62, git diff --numstat reports production C/H +2/-1 (net +1),
shared tests +27/-1 (net +26), product package test +17/-0, across one production
and three test files. No new
code file, API, state or allocation. Common/test-common manifests are revision
shared-t59-s12-p1; unchanged Lib/test-lib manifests verify. Existing build trees
are reused; the bounded diagnostic log was removed, with no new directory/media.

## Full-regression blocker

The first x64 full suite passed 89/91 in 118.12 seconds. Both package smokes
failed at stage 5: after start, no DOS prompt appeared and the child printed
host I/O error. Running just those tests with standard desktop permissions also
failed; this is not established as a sandbox-only limitation. A local A/B build
restoring 100 ms + continue reproduced compact-console's same failure. Temporary
stderr diagnostics were removed and the intended infinite-wait build restored.
This comparison does not identify the startup root cause; it demonstrates that
reverting the selected wait change alone does not fix it. No acceptance failure
has been waived. Owner now admits correct repair of this startup failure within
S12, not a waiver or a return to polling.

## Startup diagnosis and resolution

Failure-only probes identify KVM Console text-frame output, not queue waiting.
GetSystemMetrics reports a 480x911 test desktop; the child inherits an 8x16 font.
Its backing buffer is already 80x25, but GetLargestConsoleWindowSize reports
60x52 and SetConsoleWindowInfo for 80x25 returns ERROR_INVALID_PARAMETER (87).
The product correctly rejects an impossible visible surface under the existing
contract. The old polling build fails identically; no input-wait workaround
can establish the missing physical width. Initial x86 full also passed 89/91
in 90.10 seconds, failing only the same two package tests.

The package test now checks native capacity before sending start. Only if it
cannot fit 80x25, it sets an 8-pixel Consolas font on its own hidden child
Console and checks capacity again. Failure remains a distinct test setup
failure (stage 21); no skip or clipped-output success. This does not touch the
parent Console, registry, INI, media, application or library. Existing compact
viewport setup and all DOS/restart/input assertions are unchanged. It fixes the
test's missing environment precondition, not product font policy. Real x64
package/compact-console both pass (5.21/4.79 seconds) on the same narrow desktop.

All temporary source probes were removed. Lib and test/lib are byte-unchanged
against fa14c62; all four corpus manifests pass.

## P1 executor delivery

Both final builds succeed; full x64 passes 91/91 in 77.68 seconds and x86
91/91 in 60.95 seconds. Focused wait/failure suites pass 2/2 each. Both package
restart and compact-Console tests now pass on the same narrow desktop without
changing product font behavior. Four manifests, documentation governance and
git diff --check pass. Original request and added startup scope map to every
verified ledger member. No deferred failure or newly introduced API/state.

Fixed packages (no INI/media edits):

- softpc32.exe: 3,487,560 bytes; SHA256
  3F10D9A3133BD4BF3FFE136CDD4BB64329FFE0EA706D155B3BBDC8000DAE40BB.
- softpc64.exe: 2,846,218 bytes; SHA256
  A98961A99F696B1FCE4261C7236A642B9A97BCB3BF77CC16BE46BC759AA8F849.

Executor self-review is complete. Commit/push P1 before separate actual-patch
coordinator review. T59 remains open and manual testing belongs to the owner.

Push was attempted but the safety reviewer rejected external transfer without
explicit confirmation naming https://github.com/cshaxu/softpc.git main. No
workaround is attempted. P1 is locally committed; remote delivery and the
subsequent coordinator P remain blocked pending that confirmation.
