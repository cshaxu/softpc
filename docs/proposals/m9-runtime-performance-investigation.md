# Runtime Performance Investigation

## Status and origin

Owner requests on 2026-10-22 that the four performance candidates from T98
be grouped into one separate future implementation task and placed in Queue.
This proposal is unnumbered and not admitted; allocate its T identifier only
on admission. T98 remains open for separately requested public-component
governance. Moving these candidates is not a performance fix or a T98 closure.

## Scope and evidence boundary

Investigate the following four candidates together. These are source-level
leads from the T98 audit, not measured bottlenecks. Recheck current source and
freeze the implementation baseline on admission; historical counts and sizes
below must not be mistaken for current runtime measurements.

1. **C-VID slot lookup.** `compat/cvidc/gdp_state.c` linearly searches slot
   records per access, including aggregate carriers for mapped VGA members.
   The original audit counted 22603 textual rule-slot call sites, not dynamic
   calls. Measure slot count, hit positions and cost under controlled VGA work
   before considering a local ABI lookup improvement.
2. **Mouse refresh and graphics conversion.** `machine/machine.c` requests
   original full refresh after mouse delivery; `machine/driver.c` resolves
   complete produced graphics frames before Window damage comparison. A
   1024x768 RGB frame writes 3 MiB; 20 such frames/s would write 60 MiB/s.
   This is arithmetic, not a measured bottleneck. Measure refresh, conversion
   and presentation separately; retain correct V7 software-pointer updates.
3. **Execution hot-path checks.** `compat/platform.c` pending/pacing reads use
   synchronized operations called from original instruction/quick-loop hooks.
   Debug/snapshot observers also have inactive fast returns. Measure their
   instruction-throughput cost; preserve stop/control/HLT wake and snapshot
   semantics rather than deleting checks speculatively.
4. **Snapshot I/O.** `compat/media_snapshot.c` hashes base media and reads
   base/effective overlay blocks; load validates before mutation and after
   reopening. Measure hashing, reads and validation separately. These protect
   distinct obligations; do not remove validation or introduce an unsafe hash
   cache simply to reduce I/O.

## Proposed execution

- First S: re-audit all four paths and establish reproducible x86/x64 baseline
  workloads, counters and timings; distinguish instrumentation overhead and
  host/session effects. Zero production changes is an acceptable outcome.
- Subsequent S tasks: propose and admit minimal owner-local changes only for
  demonstrated worthwhile costs. Choose their grouping and diff estimates
  from evidence, not from a preselected cache/index/framework design.
- Final S: compare before/after performance and correctness, account for all
  four candidates, and perform task-level qualification and review.

The finite ledger is these four candidates. Each receives measured improvement,
measured no-change justification, or an explicit owner-approved deferral with
receiver and missing evidence. All four must have a disposition before closure;
no speedup is promised before measurement.

## Constraints and acceptance

Keep one original device/execution/rendering path and complete immutable frames.
No partial-frame ABI, second renderer, speculative cache framework, polling or
lost input/cursor updates. Audit local mirror patches as context; a functional
mirror or public-contract change requires explicit admission before editing.
Do not modify user configuration, guest media or snapshots for benchmarking;
use task-owned disposable data where needed.

For each implemented change, report actual production/test diff, measured cost
and behavior invariants, run focused plus required dual-architecture regression,
publish affected EXEs and complete actual-diff review, commit/push and acceptance
under repository governance. Task-local probes/logs retire after concise results
are promoted; only the prescribed build caches persist.
