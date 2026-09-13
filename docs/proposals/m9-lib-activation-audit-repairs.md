# Shared-library activation and audit repairs

The owner approved one simple whole-library interaction contract: status for
rejection before acceptance, once-only component failure after accepted
asynchronous work, and a terminal application boundary only for a live worker
that cannot be joined.  This is not an instruction to propagate status from
ordinary local synchronization or cleanup through every caller.
Modifier-snapshot matching is retained.

The admitted design, original approval, finite ledger, non-goals
and verification requirements are recorded in the retained
[S18 brief](../etc/evidence/m9-t55-s18-unified-status-contract.md).
Current admission and execution status belong only to
[CURRENT](../states/CURRENT.md).
