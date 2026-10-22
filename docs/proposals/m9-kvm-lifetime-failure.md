# KVM Lifetime Failure Audit

## Scope and status

T98 S14 is admitted on 2026-10-22 against 9514c823 for audit/design of the
owner's proposed repair. This delivery changes documentation only. Production
implementation awaits the owner's assessment of this recommendation. T98 stays
open; the queued performance work is unrelated.

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
