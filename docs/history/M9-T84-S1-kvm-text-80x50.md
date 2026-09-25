# M9 T84 S1 — bounded 80x50 KVM text frames

## Record

T84 S1 implements the owner's bounded 80x50 KVM text-frame request.  The
implementation baseline is `9fbf7369`; executor, delivery review and owner
acceptance are `503d6632`, `dc9c34ce` and `39366670`, respectively.  S1 is
closed by owner acceptance on 2026-09-25.  T84 remains open for S2's separate
whole-task audit and no task-level closure is claimed here.

## Scope and result

The shared KVM and logical Console capacities are 80 columns by 50 rows while
the active row count remains a per-frame field and the row stride remains 80.
The native raw-Console broker establishes and verifies the active extent,
preserves its no-shrink native-surface policy, and clears stale lower rows
after a larger frame is replaced by a smaller one.  Common's graphics status
page remains an explicit 80x25 raw-Console page.  App-SoftPC already had a
bounded 80x50 Compat surface and required no production change.

Four production C/H paths changed by +19/-11 (net +8), and nine test C paths
changed by +108/-19 (net +89).  No original Core mirror, INI, guest media,
snapshot format, build-system path or public interface beyond the jointly
rebuilt text-frame capacities changed.  The full changed-path ledger,
coverage mapping, artifact hashes and documented exclusions are retained in
[the T84 design record](M9-T84-kvm-text-80x50.md#s1-implementation-audit).

## Verification and limits

Both Release widths build.  The final serial background suites pass 120/120:
x64 in 371.65 seconds and x86 in 373.98 seconds.  Delivery rechecks pass
18/18 per width; five desktop tests per width remain excluded, and no Linux
runtime or manual guest 50-row-mode claim is made.

During S1, an intermittent existing x86 runtime-smoke null BIOS[0x52]
dispatch reproduced on unchanged `9fbf7369`.  The owner explicitly deferred
that pre-existing CPU/BOP investigation to [TODO](../states/TODO.md); S1 does
not claim to repair it.  S2 must preserve that limitation while auditing T84
as a whole.
