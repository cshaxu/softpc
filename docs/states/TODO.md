# Long-Term Review Ledger

- [ ] **Existing Linux wait/placeholder behavior (`TODO(Medium)`).** Owners:
  `lib/host/linux` and `lib/ui-base/linux`. The pre-existing mailbox wait sleeps
  instead of waiting on its signaled condition; host multi-event wait polls;
  combined mutex/condition initialization can destroy an uninitialized mutex;
  the Console placeholder lacks its declared cooked-line request operation.
  T55 S3 changes their external vocabulary only, not these execution paths.
  Admission condition: a dedicated Linux behavior/compile verification task
  with an available Linux toolchain and deterministic wake/failure tests.
