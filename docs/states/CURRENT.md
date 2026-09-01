# Current

## Current Work

### M1 T2 S6 P1: Externalize CCPU compatibility-stub ownership

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** selected original `ntstubs.c`, C-VID GDP ownership, and
  the original ROM/FPU providers selected by the standalone link.
- **Output boundary:** a generated compatibility source that declares the
  selected owners without duplicating them.
- **Scope:** externalize only C-VID global ownership, source-header routing,
  and duplicate empty ROM/FPU stubs from the pristine compatibility input.
- **Non-goals:** do not change CCPU vector behavior, SAS wrappers, ROM/FPU
  machine behavior, BOP dispatch, or runtime pacing.
- **Focused verification:** source hash, definition-owner trace, dual-width
  rebuild, and bounded BOP smoke.
- **Stop condition:** stop if a selected provider is absent or an ownership
  change affects a guest-visible initialization order.
- **Exit criteria:** `ntstubs.c` is pristine in the tree and each formerly
  duplicate definition has one selected standalone owner.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
