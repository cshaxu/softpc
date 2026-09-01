# Current

## Current Work

### M1 T2 S1 P1: Define port-ABI transformation inventory

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the `port-abi-overlay` rows in the semantic divergence
  ledger, the selected OpenNT source tree, and the existing C-VID transform.
- **Output boundary:** one explicit transformation inventory showing which
  original CCPU/C-VID/declaration inputs compile directly, which are generated
  overlays, and their native-width rationale.
- **Scope:** plan and prove only pointer-width, generated-rule, and declaration
  adaptation. Preserve original execution, controller, BIOS, BOP, renderer,
  and frontend behavior.
- **Non-goals:** do not add host services, runtime scheduling, or a new
  controller implementation.
- **Focused verification:** deterministic transform output and dual-width
  compilation of each admitted target.
- **Stop condition:** stop for owner direction if an apparent ABI overlay
  changes a guest-visible value or needs a product service.
- **Exit criteria:** each `port-abi-overlay` row has an owner, transformation
  location, and dual-width proof plan; no core source change is made merely to
  hide a host contract.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
