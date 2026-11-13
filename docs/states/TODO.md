# Long-Term Review Ledger

CURRENT owns active work; the runtime-performance candidate remains in Queue.

- [ ] TODO(Medium): Shared fixture aggregate timing margin.
  Emulator verifier-negative exceeds its existing 60-second budget in P37,
  P44, P45 and P48 aggregate runs; unchanged-budget isolated retries pass.
  P48 x86/x64 time out at 60.09/60.16 seconds, then pass alone in
  22.67/19.20 seconds. P48 x64 Types layout also reaches its 180-second
  budget, then passes alone in 58.65 seconds; x86 passes in aggregate at
  174.99 seconds. Exact scheduling cause is not established. Investigate
  repeated CMake-launch cost and aggregate scheduling in the next shared-test
  governance task; preserve every negative check and do not treat blanket
  budget increases as the fix. Original failures remain in the
  [P37 evidence](../etc/evidence/softpc/m9-t100-progress.md#p37-final-qualification-and-actual-change-review)
  and [P48 evidence](../etc/evidence/softpc/m9-t100-progress.md#p48-final-qualification-and-actual-change-review).
