# M9 T90: Non-mirror quality cleanup

Owner admits five audited items as one bounded S1 over 1684475c. CURRENT owns
execution status. The XP candidate remains queued and unadmitted.

## Finite scope and acceptance ledger

1. Machine snapshot: remove the READY branch incorrectly described as normal
   repeated-save reuse. Normal finish clears READY before the pause callback;
   restart failure retains it only as a terminal condition. Preserve the one
   existing capture path and verify repeat saves plus paused debug mutations.
2. Compat audio: unify worker failures into one diagnostic/exit path. Preserve
   original error status, distinguish normal shutdown cancellation, no retries,
   restart framework or machine ERROR transition. Test enqueue/wait/flush and
   cancellation using the existing owner-local fake backend.
3. App mode parsing: one private readonly/direct/overlay parser shared by INI
   and floppy CLI; retain each caller's case and argument policy. No new module.
4. Save output: a stack context holds path and optional writer. The existing
   write callback lazily opens/truncates on first nonempty write. A failure
   before any write leaves the old file intact; partial-write rollback is
   explicitly not promised. Test old-file preservation and real output.
5. Documentation: describe the existing Lib Audio queue/worker and actual App
   files. Correct descriptions, not runtime behavior, to match reality.

All five require a receiver and actual-change review. No change to the mirror,
shared six packages, snapshot format, INI or user media is authorized here.
Estimated production net -10 to +15; tests +120 to +240 lines. Existing tests
and fixtures are preferred; no additional test framework. Compile/publish both
EXEs and run shared then remaining background suites on each architecture,
plus all five desktop cases per width serially in an owner-reserved period.
Long operations use a hidden bounded supervisor. Commit/push and wait for owner
acceptance; this is not automatic T closure.
