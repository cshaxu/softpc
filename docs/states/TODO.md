# Long-Term Review Ledger

The prior investigation is recorded in
[T59 S1](../history/M9-T59-product-boundary-cleanup-proposal.md).
Overlay optimization remains in [Queue](QUEUE.md).

- P2 — Original 16-bit debugger/parser edge behavior. Owner: Common debug.
  Linear X-command defects are admitted in T59 S21. Original S can read a
  pattern beyond its end offset and wrap its 16-bit cursor; U's decode-failure
  and offset-boundary progress and M's wrapped ranges need a separate contract.
  Generic permissive argument parsing remains unchanged. E/F/XE/XF partial
  writes are explicitly retained semantics, not pending transactional work.
  Admission: owner approves changes to original 16-bit/parser behavior with
  bounded tests. Do not fold those semantics into the linear-command repair.

- P2 — Compact package Console input fixture. Owner: product integration tests.
  S21's first x64 full run failed stage 14 (DOS ver text absent, error=0);
  the unchanged executable passed three immediate isolated reruns. This route
  does not enter debugger. Cause is unconfirmed, not claimed repaired by S21.
  Admission: reproduce and distinguish input/reader timing from test observation
  without weakening the DOS input assertion or changing debugger semantics.
