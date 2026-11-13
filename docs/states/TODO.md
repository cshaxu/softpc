# Long-Term Review Ledger

CURRENT owns active work; the runtime-performance candidate remains in Queue.

- [ ] TODO(Medium): Emulator verifier-negative aggregate timing margin.
  P37 x86/x64 full runs exceed the existing 60-second budget, while unchanged
  isolated retries pass in 25.81/25.35 seconds. P44 x86 repeats this pattern:
  60.01-second aggregate timeout, 28.31-second unchanged-budget isolated pass;
  x64 aggregate passes in 57.29 seconds. P45 x64 times out at 60.36 seconds
  and passes alone in 26.29 seconds with the same budget. Exact scheduling cause is not
  established. Investigate repeated CMake-launch cost and aggregate scheduling
  in the next shared-test governance task; preserve every negative check and
  do not treat blanket budget increases as the fix.
  [P37 evidence](../etc/evidence/softpc/m9-t100-progress.md#p37-final-qualification-and-actual-change-review).
