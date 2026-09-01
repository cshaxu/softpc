# Current

## Current Work

### M1 T2 S8 P1: Externalize the FPU private-index carrier

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** selected original `fpu.c` and its eight-entry private
  FPU stack representation.
- **Output boundary:** a generated FPU compilation input with one fixed guest
  register-index carrier.
- **Scope:** externalize only the native diagnostic include and the bounded
  `TOSPtr - FPUStackBase` conversion before it joins original `IU32` index
  arithmetic.
- **Non-goals:** do not change FPU arithmetic, exception behavior, stack
  contents, initialization, BOP dispatch, or runtime pacing.
- **Focused verification:** source hash, transform idempotence, dual-width
  rebuild, and bounded BOP smoke.
- **Stop condition:** stop if any edit reaches an FPU operation or changes a
  guest-visible register result.
- **Exit criteria:** `fpu.c` is pristine in the tree and its sole host-width
  carrier is supplied by a reproducible generated overlay.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
