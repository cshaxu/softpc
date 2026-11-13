# Long-Term Review Ledger

CURRENT owns active work; the runtime-performance candidate remains in Queue.

- [ ] TODO(Medium): Original GenericByteFill loses its local storage after a
  nested GenericByteWrite continuation, producing access violation on x86/x64.
  Current CCPU fills use individual stores; no actual guest-triggered batch call
  has been established. Admit separate original-rule repair before enabling
  that batch path, or when a production consumer is demonstrated. Reproduce the
  recorded actual-body negative control and audit related same-file nested calls.
  [T100 S1 P34 evidence](../etc/evidence/softpc/m9-t100-progress.md#actual-body-controls-and-newly-exposed-original-entry-defect).
