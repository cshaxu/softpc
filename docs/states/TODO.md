# Long-Term Review Ledger

CURRENT owns active work; the runtime-performance candidate remains in Queue.

- [ ] TODO(Medium): Original GenericByteFill loses its local storage after a
  nested GenericByteWrite continuation, producing access violation on x86/x64.
  Current CCPU fills use individual stores; no actual guest-triggered batch call
  has been established. Admit separate original-rule repair before enabling
  that batch path, or when a production consumer is demonstrated. Reproduce the
  recorded actual-body negative control and audit related same-file nested calls.
  [T100 S1 P34 evidence](../etc/evidence/softpc/m9-t100-progress.md#actual-body-controls-and-newly-exposed-original-entry-defect).

- [ ] TODO(Medium): Emulator verifier-negative aggregate timing margin.
  P37 x86/x64 full runs exceed the existing 60-second budget, while unchanged
  isolated retries pass in 25.81/25.35 seconds. Exact scheduling cause is not
  established. Investigate repeated CMake-launch cost and aggregate scheduling
  in the next shared-test governance task; preserve every negative check and
  do not treat blanket budget increases as the fix.
  [P37 evidence](../etc/evidence/softpc/m9-t100-progress.md#p37-final-qualification-and-actual-change-review).
