# Current

## Current Work

### M1 T2 S11 P1: Reconcile the remaining CCPU divergence ledger

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** current semantic-diff ledger after all admitted CCPU
  source recoveries.
- **Output boundary:** an updated classification proving each remaining row is
  either pristine, a port-ABI overlay, a host compatibility boundary, or M3
  executor work.
- **Scope:** rerun the ledger and trace remaining source edits; remove only
  representation-only edits whose selected provider and consumer are known.
- **Non-goals:** do not change CCPU rule logic, event scheduling, BOP, BIOS,
  controllers, runtime pacing, or user-facing frontends.
- **Focused verification:** semantic ledger delta, source hashes, and bounded
  dual-width BOP smoke for any admitted final recovery.
- **Stop condition:** stop if a row needs a run-slice or timer/event choice.
- **Exit criteria:** no unclassified direct CCPU source edits remain before M2
  and M3 begin their respective host-contract work.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
