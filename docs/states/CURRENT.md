# Current

## Current Work

### M1 T1 S4 P1: Reconcile remaining pristine-source divergences

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the selected OpenNT source tree at
  `nt/private/mvdm/softpc.new`, and the current `src/core/softpc` tree.
- **Output boundary:** updated complete divergence ledger and restored original
  machine inputs, or a bounded external port-ABI/host-compat disposition for
  every remaining selected source difference.
- **Scope:** audit the current original-peer differences after S2/S3; recover
  machine behavior where the selected original source is available; identify
  any port-width declaration or host-boundary exception without implementing
  M2 host services or M3 runtime policy.
- **Non-goals:** do not add scheduling, product semantics, a replacement
  renderer/controller, or a new user-facing feature.
- **Focused verification:** reproduce the divergence manifest; source diff for
  each restored input; x64 and x86 compile of every changed production target.
- **Full regression:** retain the M3 safe-execution prerequisite before using
  the legacy machine-run smoke as a closure gate.
- **Similar-issue sweep:** inspect sibling variants and all CMake source lists
  for direct compilation of a divergence already classified as overlay or host
  compatibility.
- **Stop condition:** stop for owner direction if restoring an original input
  would require a DOS/WOW/NTVDM product service rather than a finite host
  contract.
- **Exit criteria:** no selected original-peer difference remains unclassified,
  and every change admitted to pristine or port ABI has dual-width evidence.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
