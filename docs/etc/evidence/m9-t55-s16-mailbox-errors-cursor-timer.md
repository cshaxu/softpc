# M9 T55 S16: mailbox errors and cursor timer

Owner accepted S15 (“测试通过”) and admitted: “请你准入s任务 干净修复
类似问题下次审计不要再出现了！” Baseline f319017, clean main.

## Design and finite ledger

Frozen universe: UI mailbox signal implementations and every production caller;
Window cursor timing startup, freeze/unfreeze, modal dispatch and destruction.
Disposition is fixed with focused proof, retained for a distinct responsibility,
or an explicit owner decision if the wait/destroy contract must change.

| Requirement | Owner / proof | Status |
| --- | --- | --- |
| Native wake failure reaches requester, including Console activation | ui-base selected signal result and ui-console failure entry; injected Windows/Linux errors | Focused proof passed |
| One cursor timing path, including modal interaction | Window WM_TIMER; remove outer-loop blink scheduling; deterministic transitions and real modal test | Focused proof passed |
| Persistent notification failure does not get hidden by a test rescue | Controlled waiting worker and rejected notifications; retained resources and checked destroy | Focused proof passed |
| Similar paths and regression | All wake callers, timer create/stop/destroy/failure; full dual-width tests, strict library and gates | Passed |

The notifier result describes notification, not queue rollback. An error must
not replay an already copied request. Linux keeps the same selected interface.
Window native timer replaces, rather than supplements, outer-loop blinking.
Freeze preserves drawn phase; unfreeze resets once, never on repeated requests.
Startup/transition timer failure follows existing creation/terminal cleanup.

No second notifier, worker, periodic failure polling, forced thread termination
or unsafe release of a running worker is admitted. No MVDM,
media, INI, product lifecycle or mouse scaling change. T55 stays open.

Complete implementation, tests, dual EXEs and evidence must travel in executor
P1 before coordinator committed-diff review. No partial P is claimed complete.

## Persistent-failure investigation: decision required

The diagnostic compiles the real ui-console Win32 worker and selected mailbox
implementation, overriding only SetEvent to return FALSE. A readiness barrier
establishes the worker's wait entry. STOP returns IO_ERROR; the terminal path
also fails notification. Actual result: attempts=2, stopping=1,
WaitForSingleObject(worker,50)=WAIT_TIMEOUT, retired=0. An explicit probe-only
SetEvent then permits join/cleanup. That rescue is not production success.

This proves that merely returning the signal error does not establish shutdown
liveness. With infinite waits, no alternate wake and no polling, changing a flag
cannot wake the waiter. The approved brief explicitly requires owner direction
before changing waiting/destruction contracts. Work pauses at that boundary.
No build, full test, commit, push or completed delivery is claimed. Local edits
are incomplete and package EXEs remain S15. Diagnostic executable was removed.

## Owner-approved destruction contract

Owner subsequently approved (“批准”) the explained API change. Both UI destroy
APIs return lib_status. One synchronous join waits at most 5000 ms; OK means
the worker completed and the component was disposed. A failed/timed-out join
returns IO_ERROR without closing worker handles, freeing component/mailbox or
callback context, or inventing retirement. The caller must treat this as fatal,
not reuse the object or continue normal cleanup. NULL destruction remains OK.
SoftPC checks every UI destroy result and reports/exits without retrying cleanup
or releasing any context still accessible by the retained worker. No lib API
terminates the process. Tests may explicitly rescue their own failed worker
after proving retention, solely to clean up the test process.

The selected platform join has the same shape on both platforms, accepting the
common fixed timeout. Linux leaves remain unsupported placeholders. The normal
STOP FIFO and successful disposal path are unchanged. This supersedes the
earlier pause for owner direction, not its recorded diagnostic evidence.

## Executor implementation and similar-path review

- `rg -n 'mailboxes_notify|wake_signal|worker_join' src/lib/ui-base
  src/lib/ui-window src/lib/ui-console`: Windows SetEvent and Linux mutex/
  condition signal now return checked status. Default notification returns it;
  frame/control/STOP already propagate it. Console ACTIVATED now records failure.
  The one discarded result in `ui_component_fail` is retained intentionally:
  failure is already recorded and admission closed; recursive failure handling
  cannot repair a permanently broken notification primitive. Checked destroy
  is the final application boundary, not a second wake channel.
- Window and Console selected joins both accept the shared deadline, free only
  on WAIT_OBJECT_0, and leave all resources intact otherwise. Linux selected
  signatures match; unsupported leaf startup remains an explicit placeholder.
  All four SoftPC destruction call sites use the same terminal helper. A static
  source gate rejects unchecked UI destruction in app code.
- `rg -n 'advance_cursor_blink|cursor_blink_timeout|set_timer|kill_timer'
  src/lib/ui-window`: one blink definition, one WM_TIMER invocation; no outer
  wait scheduling. Startup/unfreeze create the timer, freeze removes it, Window
  destruction owns timer disposal. Existing frozen/due checks reject queued
  stale ticks; repeat freeze/unfreeze has no timer side effect. A static gate
  rejects restoration of the removed outer-loop timing path.
- Startup readiness events and host synchronization primitives retain their
  distinct existing contracts; this is the admitted UI mailbox/timer/destruction
  audit, not a claim that every OS call throughout lib was changed or proven.

## Focused evidence and limits

`ui_shutdown_failure_smoke` covers Console STOP/control/frame/activation with
every notification rejected. Its barrier establishes a real waiting worker;
destroy reports failure, worker remains alive, no premature retirement or free.
Window retirement case 18 proves the equivalent retained-worker contract.
Deadline expiry is injected at the real join boundary (asserting 5000 ms), not
measured by sleeping five seconds in each fixture. Explicit test-only rescue
happens after assertions solely to reclaim fixtures, never as product behavior.

Window capture/retirement tests cover timer phase, repeat transitions, stale
ticks, creation/unfreeze/freeze failures and exactly-once retirement. Native
modal smoke observes actual WM_TIMER dispatch inside size, move and menu loops,
then proves FIFO control and STOP still complete. The actual SoftPC fatal helper
is exercised in a child process which exits with failure without normal cleanup.
Linux wait tests use deterministic POSIX fakes; no Linux runtime parity claim.

Initial integration exposed an old no-Window matcher fixture calling real timer
APIs: it now fakes timer operations while preserving every input assertion.
The active brief was also incorrectly placed in history; it now lives in indexed
evidence, and accepted S15 has a closure pointer. The unchanged documentation
gate now passes. Earlier test-target linkage/signature build failures were
corrected before final dual-width rebuild; no assertion or gate was weakened.

## Changed-path accounting

Relative to f319017, excluding Markdown, manifest and EXEs: 21 production/build
paths +127/-60 (net +67); 13 test paths +226/-50 (net +176). Reproduce with
`git diff --numstat f319017 -- CMakeLists.txt src/app src/lib test`, excluding
`.md`/`.sha256`; new test files count as additions. These include the shared
status contract and its real adapters, not extra wrappers or background workers.
Normal STOP remains FIFO; no retry, polling or forced-thread-termination path.

The owned ignored `build/audit-f319017` probes were removed after their findings
became permanent tests and this evidence. User configuration, media and MVDM
were not modified.

## Final verification

- `cmake --build --preset tests-x64 --parallel 6`: passed.
- `cmake --build --preset tests-x86 --parallel 6`: passed using the existing
  D:/programs/msys64/mingw32 compiler variables, not a new toolchain.
- Full `ctest --test-dir build/mingw-gcc-x64-release --output-on-failure`:
  55/55 passed, 35.47 seconds.
- Full `ctest --test-dir build/mingw-gcc-x86-release --output-on-failure`:
  55/55 passed, 79.26 seconds.
- `cmake --build build/lib-strict --parallel 6`: passed with
  `-std=c11 -Wall -Wextra -Wpedantic -Werror`; standalone CTest 3/3.
- Manifest `shared-t55-s16-p1`, component DAG, source boundary, documentation
  and documentation selftests passed; `git diff --check` passed.
- Package SHA256: x86
  `ED0BD117BD670854B7F74C282B2C4D6BB1B4378DDB81B396C1F498311C23D9EA`;
  x64 `4FACFEB524CD434B0D141B9842237A1A8B60D0E6B1177AEBFB5930B29BD077FA`.

Executor self-review completed against the finite ledger and actual diff.
P1 carries all implementation, tests, both EXEs and this evidence; coordinator
review follows its push. Owner manual testing is pending; T55 is not closed.
