# Current

## Current Work

### M1 T2 S7 P1: Classify remaining CCPU generated-source overlays

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** remaining semantic-diff rows in original CCPU generated
  sources, including `fpu.c`, `sascdef.c`, and `zfrsrvd.c`.
- **Output boundary:** a ranked, evidence-backed roster separating pure
  declaration/carrier overlays from candidates that must wait for M3.
- **Scope:** trace each remaining direct edit to its provider, consumer, and
  width requirement; admit one bounded source only if its overlay cannot
  change CCPU rule logic.
- **Non-goals:** do not modify FPU arithmetic, exception behavior, generated
  instruction rules, BOP dispatch, controller state, or runtime pacing.
- **Focused verification:** source diffs, owner traces, and generated-output
  idempotence for any admitted source.
- **Stop condition:** stop at any candidate whose recovery needs an executor
  run-slice decision.
- **Exit criteria:** each remaining row is either an admitted minimal overlay
  or explicitly deferred to M3 with concrete blocking evidence.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
