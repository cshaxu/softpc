# M9 T55 S18: minimal failure and ownership contract

## Purpose

S18 completes the S17 audit repairs without turning every internal primitive
into an application-wide error protocol.  The shared library remains a set of
small components: it reports component failure, but does not select product
lifecycle, print product messages, or terminate the process.

## One interaction rule

An operation has exactly one of these outcomes:

1. A synchronous request is rejected before the component owns it and returns
   `lib_status` to its caller.
2. A copied UI control or frame is accepted and returns `LIB_STATUS_OK`.  A
   wake failure after that acceptance is a component failure, reported once
   through the existing failure sink; it is not replayed and is not a second
   request result for every caller to interpret.
3. Destruction of an active native reader or UI worker performs its one join
   barrier.  If that barrier cannot establish that the worker stopped, the
   application takes its existing terminal infrastructure boundary rather than
   exposing a partially destroyed object.

Normal wait outcomes such as signalled, cancelled, and timed out remain normal
values.  Event signalling, reset, locks, reference release, and ordinary local
cleanup remain component implementation details; no product-wide status
plumbing is added for them.

## Ownership rules

- A successful create returns one complete object.  A failed public create
  leaves its output null; it never returns a half-object that callers must
  remember to rescue.
- The UI worker accepts FIFO controls until STOP, then retires once.  The
  source-retired event remains distinct from the first failure notification.
- UI creation waits for ready **or** worker exit, so a failed startup cannot
  strand a creator waiting for a signal that will never arrive.
- The host Console broker retains its process-wide native ownership and its
  transactional raw/cooked replacement.  An unquiesceable native reader is a
  terminal application fault, not a public recovery state.
- Logical Console reference release is bookkeeping, not a fallible public
  lifecycle protocol.  Output/binding operations retain their real status
  because they can fail at an actual I/O boundary.

## S17 ledger disposition

| Family | S18 closure |
| --- | --- |
| G1 failure notification | First UI failure is reported once immediately; accepted work is never replayed. |
| G2 ownership completion | No failed-create half-objects; active worker/native-reader barriers are handled at the one terminal application boundary. |
| G3 startup | Window startup observes ready or worker exit. |
| G4 synchronization | Synchronization remains local; only real join/activation barriers escape to the terminal boundary. |
| G5 side effects | Required Window/Console/storage side effects preserve their concrete failure handling without a second control path. |
| G6 visual consistency | Cursor/glyph/phase corrections remain in the existing Window rendering path. |

## Verification

The test set proves accepted-then-failed wake handling, single failure/retire
delivery, FIFO STOP admission, startup exit without ready, broker replacement
and native-reader ownership, plus cursor/glyph paths.  Final S18 verification
is dual-width build/CTest, strict-library CTest, manifest, and documentation
boundary gates.  The owner alone performs final interactive package acceptance;
T55 remains open after S18 delivery.

## Executor and coordinator record

P1 is `07b149e` (`M9 T55 S18 P1: simplify shared failure boundaries`). The
post-commit coordinator audit found one over-broad application boundary only:
ordinary host synchronization had been treated like worker teardown. The final
corpus keeps `softpc_host_require_status` only for UI and native-Console
creation/destruction, where a live worker or reader may still exist. The
source-boundary gate enforces that narrow set and does not require application
handling for event signal, reset, lock, or reference-release mechanics.

Fresh x64 and x86 builds completed. Each width completed all 56 CTest cases;
the strict-library build completed its three library checks. The manifest,
component-boundary and documentation-governance checks also passed. The two
fixed package executables are refreshed in `assets/binary/`.

## Non-goals

No MVDM/media change, product lifecycle redesign, new error manager, retry
loop, compatibility path, worker, queue, or Linux parity expansion is part of
this task.
