# KVM Lifetime Failure Audit (Retained T98 S14 Proposal)

## Scope and status

T98 S14 is admitted on 2026-10-22 against 9514c823 for audit/design of the
owner's proposed repair. Audit P1 is 06a86ad7. The owner subsequently approved
implementation and dual qualification. On reconnection the owner authorizes
closure after successful self-verification and delivery.
T98 stays open; the queued performance work is unrelated.
S14 is now closed; [closure and actual-commit review](../history/M9-T98-S14-kvm-lifetime-failure.md)
record executor 0d9f1803 and the completed qualification.

The owner explicitly permits process termination when an internal lifetime
failure cannot safely return. Ordinary parameter/allocation/resource/I/O errors
remain ordinary failures. No fatal callback, half-created object, retry manager,
polling or frame architecture redesign is allowed.

## Finite source ledger

1. **Window create rollback: confirmed contract defect.** window.c initializes
   output to NULL. win32/component.c worker_start calls worker_join after failed
   startup/wait, but returns startup_status even when join fails. window.c then
   skips disposal while worker_state remains non-NULL and returns an error.
   The caller receives neither the object nor a guarantee that callbacks have
   stopped. Its comment claims a process-terminal application path that is not
   implemented here. A failed terminal signal or wait can therefore leave a
   potentially live worker referencing caller-owned context after return.
2. **Console create rollback: same confirmed defect.** Failed event-sink
   attachment calls worker_join; failed signal/wait returns IO_ERROR with
   worker_state retained. console.c likewise leaves output NULL. Existing
   kvm_shutdown_failure_smoke tests failed attachment with successful join,
   not the combination with failed terminal signal/join.
3. **Normal destroy: not universally unsafe today.** Generic component destroy
   returns early on stop/join failure and documents retained object/context.
   Emulator UI obeys that contract at all four Window/Console destroy sites.
   Do not describe this normal path as an already-proven use-after-free.
   The proposed fail-fast policy would simplify this particular contract, not
   repair every caller. Never join from callbacks/worker; owner serialization
   and lifetime constraints still apply.
4. **Post-join resource errors are mixed with thread safety.** Window ignores
   all three CloseHandle results after a successful wait, releases state and
   returns OK. Console returns immediately if closing the already-joined
   worker handle fails, retaining the stop event and state. These should not
   be treated as live-worker failures or grounds for process termination.
5. **Window cleanup stops prematurely.** destroy_surface returns at the first
   selection/bitmap/DC failure; native destroy returns on DestroyWindow or
   surface cleanup failure before independent cursor cleanup. Cursor success
   does not clear the cursor field. Current repeated cleanup risk is not
   dynamically demonstrated. Preserve dependency constraints: do not delete
   an owned bitmap while it may remain selected; a failed deselection need not
   prevent deleting an independent cursor or retiring the owning DC safely.
6. **Linux leaves:** both native starters currently return UNSUPPORTED without
   creating a worker. Their no-worker cleanup remains an ordinary failure;
   no Linux backend implementation or new lifecycle is required.
7. **Frame tests:** frame_exchange_smoke has real concurrent fixed payloads;
   kvm_frame_copy_smoke exercises dynamic size/type retirement and allocation
   failure, but its held-reader scenario is single-threaded. Combining real
   dynamic payloads and a synchronized slow reader is justified test coverage,
   not evidence that exchange production code is defective.

These are source/test inspection conclusions, not injected runtime proof or
an explanation for an observed product crash. Searches covered worker_start,
worker_join, create/destroy/dispose, native cleanup and all kvm_*_destroy
production callers. Only Emulator UI has outward production destroy calls;
no App or original mirror adapter needs a new fatal callback.

## Minimal recommendation

Proceed, but constrain the brief to the confirmed paths above. Keep all four
frame slots, lazy payloads, formats and exchange algorithms unchanged.

- Add a one-to-one Types wrapper for standard C _Exit if supported by both
  configured toolchains; no new Base subsystem is needed. Existing Win32
  TerminateProcess vocabulary is also available, but raw Windows declarations
  must not escape into platform-neutral component code. Confirm non-returning
  behavior in isolated x86/x64 child tests. Do not use assert, exit or a fatal
  callback. Microsoft documents _Exit as process termination without atexit/
  onexit execution; verify the actual MinGW-linked CRT in the child test.
- In the existing native stop/join path distinguish inability to establish
  worker quiescence (fatal, no return) from handle release after confirmed exit
  (collect error, continue independent cleanup). Do not promote every API error.
  Ordinary worker I/O failure still uses the existing failure notification.
- Creation rollback then either completes disposal and returns NULL/error or
  terminates; delete the misleading retained-worker create branches/comments.
- Recommend one explicit KVM destroy contract: on any normal return from a
  valid owner call the object is consumed; status describes cleanup errors.
  Unsafe quiescence cannot return. Keep the lib_status signature. This needs
  the four Emulator UI callers to clear consumed pointers before handling
  status, plus their fixture expectations. Do not alter broker or generic Base
  task ownership contracts under this narrow task.
- Native cleanup records first failure while releasing independent resources;
  successful release clears its handle. Maintain select/DC/bitmap dependency
  order, including resize-time surface replacement. A failed native deletion
  may leave an OS resource leaked; report it without a residual-object manager
  or pretending that a live thread is safe. Do not preserve freed pixel aliases.

The current architecture text says native stop/join failure retains KVM objects;
that specific contract must be updated if implementation is approved. Other
Base task and broker contracts are outside this change. This is a deliberate
behavioral change to downstream KVM callers, not a binary signature change.

Reference: [Microsoft CRT exit/_Exit/_exit](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/exit-exit-exit?view=msvc-170).

## Estimated change budget, not a patch commitment

- Production: about 9-11 C/H files; +60-110/-40-80, net approximately +20-40.
  Types wrapper, KVM generic destroy/contracts, both leaf create wrappers,
  Win32 worker/cleanup owners and Emulator UI's four call sites. No frame
  implementation edit is planned. Header-comment edits contribute to these counts.
- Tests: about 4-6 existing fixtures/build entries; +150-250/-20-60, net about
  +130-190. Reuse native-call injection, child-process support and Base events;
  avoid a new general failure-injection framework or duplicate lifecycle suite.
- Manifests, owner README/interface comments, architecture contract and task
  records follow actual changed ownership. No downstream repository edit.

An implementation needing several hundred production lines, a new manager,
per-error states or a frame rewrite should stop for reassessment. Most added
lines should be proof of the rare fatal boundary, not runtime machinery.

## Verification plan

Use child exit status and a parent-owned synchronization observation to prove
the fatal branch is reached and does not return; a generic nonzero exit alone
could be a crash and is insufficient. Keep a child-return marker and prove no
cleanup callback ran. Cover Window and Console startup rollback with terminal
signal/wait failure and ordinary rollback with successful quiescence.

Reuse existing STOP/retirement tests for normal and repeated STOP, and inject
post-join handle/GDI cleanup failures to assert all independent cleanup and
correct error/consumed-pointer semantics. Do not confuse repeated STOP with
destroying an already freed pointer. Preserve no-self-join calling constraints.

Extend the Window leaf's dynamic allocation fixture with event barriers and
real producer/consumer threads: hold an old text/graphics payload, switch sizes
and types, release it, and verify contents and allocation/free accounting.
Use synchronized/atomic test counters rather than introducing test data races.
No fixed sleeps or new test-only production API.

After implementation: dual cached builds, affected/complete public unit tests,
Emulator UI caller regressions and required native/product integrations; publish
dual EXEs, actual-diff review, commit/push and owner acceptance. This audit has
not run those tests and does not claim a repaired or qualified implementation.

## Implementation and bounded quality review

The implemented path is stop -> terminal wake -> actual worker exit -> join ->
independent resource cleanup -> object disposal. An unconfirmed native stop/join
calls the Types one-to-one `_Exit` wrapper. It never returns a hidden live worker,
calls application cleanup, or creates a recovery owner. A failed terminal wake
uses the same wait once with zero duration to observe an already-exited worker;
confirmed exit permits ordinary error/cleanup, not process termination. This is
not a polling loop, deadline or retry. Ordinary cleanup errors
after join consume the object and return IO_ERROR. Emulator UI clears all four
consumed pointers before examining status; its own broker/UI contract is unchanged.

Window deletes its owned memory DC before its bitmap: successful DC deletion
removes the selection, but does not delete the bitmap. Failed DC deletion prevents
deletion of a still-selected bitmap, without skipping independent cursor cleanup.
The existing worker exit code carries cleanup status to join; no state field or
new lifecycle path is introduced. Console preserves the original sink-attachment
error after successful rollback, rather than replacing it with INVALID_STATE.

The frozen review universe is the seven source-ledger items above plus their
real callers and tests. Dispositions/proof:

- Both failed-create paths: ordinary NULL/error and isolated stop/wait failure
  children in the existing Window capture and Console shutdown fixtures.
- Both normal destroy paths: isolated stop/wait faults and ordinary post-join
  handle-close errors; exact fatal exit 6, a reached-boundary marker, absence of
  atexit output and rejection of ordinary returns distinguish termination from
  an unrelated crash. The test controller remains a separate CMake process.
- Independent GDI cleanup: DC/bitmap/cursor faults verify subsequent releases,
  cleared successful handles and preservation of a still-selected bitmap.
- Four Emulator UI consumers: composition and action failure tests prove a
  consumed Window/Console is not destroyed twice. Retained broker behavior stays.
- Existing retirement tests still own STOP/input/callback concurrency and a
  synchronized wait beyond the former five-second deadline.
- Dynamic frame proof extends the existing leaf fixture, with a real producer
  replacing sizes and text/graphics while a consumer holds old text, then old
  graphics. Only that producer mutates allocation counters until join; at most
  four payloads coexist and final allocation/free counts must balance.
- Linux unsupported leaves, frame exchange, Base task and broker production
  paths remain unchanged. No downstream repository is read or modified.

Searches cover all production kvm_window/console/component_destroy calls,
worker_start/join and stale retained-KVM contract text. The only outer consumers
are the four Emulator UI sites. Ordinary notification faults retain their existing
asynchronous sink; no fatal callback, polling, retry, alias API or second frame
path was added. Shared-corpus type/dependency/naming/manifest tests will provide
whole-package static evidence, not a claim of a new line-by-line audit of every
unchanged public implementation.

Limits remain explicit: callers must serialize destroy, stop external publishers
before object disposal, and never synchronously destroy from a worker/callback.
Post-join native resource failure can leak that failed OS resource; it reports
failure without a residual-object manager. Base event destruction is still void,
so its private handle-close failure is not newly observable from KVM; that generic
Base contract was deliberately not expanded. No actual host stop/join corruption
is claimed reproduced: these paths are controlled injected faults.

Initial focused x64 run caught an obsolete test count: switching specifications
reclaims the spare writer immediately, leaving two then one payload rather than
the old fixed-spec three then two. The test's exact counts were corrected from
the existing retire contract; exchange production code was not changed. Final
focused x86/x64 runs each pass all four selected fixtures. Full qualification
and delivery evidence follows below when completed.

The first native retirement runs exposed its obsolete retained-object/retry
assertion after injected join failure. That block and its join override are
removed: the new unit children cover this fatal boundary without a real desktop.
All 24 native scenarios and the >5-second synchronized retirement remain.
The existing native startup-signal failure also proves safe rollback when the
worker has already exited; Console has the equivalent controlled test. Native
cursor-delete failure now expects IO_ERROR on the consumed-object return.
Both native retirement fixtures passed after this contract update. An initial
x64 compact package run and a subsequent x86 compact run separately failed at
stage 1/error 6, matching the retained TODO; neither is attributed to this repair
or silently replaced by the following qualification record.

## Earlier qualification checkpoint

- Cached builds pass on x64 and x86; changed public components have no compiler
  warnings. Original mirror-header warnings from the wider rebuild are retained.
- Final production background runs each pass 184/184. A final test-only change
  adds direct release-hook rejection of the two held payload pointers, so early
  freeing cannot pass merely because old memory still contains expected bytes.
  Its focused four-fixture runs pass on both widths. All 177 unit/gate cases per
  width subsequently pass with the host session Disc, including new manifests.
- The updated real native retirement test passes on both widths, including the
  synchronized >5-second case. The final native batch coincided with the session
  becoming Disc: PCM/MIDI open assertions fail and package startup routes fail.
  This is a qualification blocker, not a reason to change product audio or to
  claim final integration success using earlier Active-session passes.
- User reconnection has been requested. No owned build/test job remains running;
  only this packet's small scratch directory is retained for native rerun and
  final evidence promotion. No executor P2 commit or push has been made yet.
- Count basis is `git diff --numstat 06a86ad7` for tracked src/test C/H plus the
  new 18-line test fixture header. Production: +64/-66, net -2. Tests: +225/-49,
  net +176. Test registration/controller CMake: +26/-2, net +24. Manifests,
  documentation and EXEs are separate, not disguised as production reductions.
  Necessary fatal/cleanup/concurrent-allocation proof explains test growth;
  no established lifecycle coverage was deleted to force a negative total.
- Changed shared trees: src/lib, test/lib, src/emulator, test/emulator. Both
  X86 source/test pairs, root test infrastructure, App and mirror are unchanged.
  Downstream consumers must adopt the consumed-on-normal-return destroy contract;
  no downstream synchronization is claimed and no uncommitted import baseline
  is offered.
- Published softpc32 SHA-256:
  A583D165349382242CAB44F132DA98573CE1A431249D785436437304AFC5F891.
  Published softpc64 SHA-256:
  BECB19DBD2A64C72727BB7E9F142E77C2B57832C23176C30EDF187795B9F48B1.
  INI and pre-existing user snapshot.debug hashes match the admission baseline.

This checkpoint preceded the successful rerun below; it is retained to disclose
the failures, not as the final task status.

## Final executor qualification, 2026-10-22

The owner reconnects and authorizes self-verification followed by S closure.
RDP is confirmed Active. Hidden serial native execution returns zero:

- x64: desktop 5/5, native audio 3/3.
- x86: desktop 5/5, native audio 3/3.
- Both ordinary and compact package cases pass in this complete rerun; no
  isolated replacement, skip, assertion relaxation or product retry is used.
- Together with final-production background 184/184 on each architecture,
  XML testcase names exactly equal all 192 registrations per architecture;
  there are no duplicate names, failures, errors or skipped cases in that set.
- The final test-only held-pointer assertion supplement also has the later
  177/177 unit/gate run on each width, in addition to its focused four fixtures.
  These executions are supplementary, not extra registrations.
- Both published EXE hashes and protected INI/snapshot hashes still match the
  checkpoint above. No additional production rebuild is necessary for this
  session-only rerun. Changed public code is warning-free; original mirror
  header warnings remain outside scope as required.

Earlier Active compact attach failures and disconnected audio/package failures
are not erased by this result. The existing package TODO remains open. Injected
faults prove terminal/cleanup contracts, not real host corruption; the native
tests separately prove real creation, retirement, modal and Console behavior.

Executor review finds no additional in-scope blocker: one terminal join path
per native leaf, ordinary failure remains ordinary, all four outer destroy
consumers clear consumed pointers, no new production state/polling/recovery,
and no frame algorithm change. Public Types/dependency/naming/manifests and
all public unit tests pass. This is a changed-path review plus whole-package
gates, not an exhaustive new audit of unchanged implementations.

Counts remain production C/H +64/-66 (net -2), test C/H +225/-49 (net +176),
test CMake +26/-2 (net +24). All task-owned jobs have exited. Concise evidence
is promoted here before retiring disposable scratch; reusable caches remain.
Executor 0d9f1803 is committed/pushed. Coordinator actual-commit review and
authorized closure are recorded separately in the linked history. T98 remains
open; no next S is admitted.
