# Current

## Current Work

### M1 T2 S9 P1: Externalize the SAS vector null-slot carrier

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** selected original `sascdef.c` and its `cSasPtrs`
  function-vector initializer.
- **Output boundary:** a generated source with the selected unavailable
  `Sas_overwrite_memory` slot expressed in its actual function-pointer type.
- **Scope:** externalize only the null carrier's type; preserve vector order,
  slot value, SAS calls, and all memory behavior.
- **Non-goals:** do not add SAS services, alter allocation, change guest
  addresses, or modify CCPU execution, BOP, devices, or runtime pacing.
- **Focused verification:** source hash, generated-output idempotence,
  dual-width rebuild, and bounded BOP smoke.
- **Stop condition:** stop if vector ordering or the selected null value must
  change.
- **Exit criteria:** `sascdef.c` is pristine in the tree and both widths
  compile its vector through a reproducible generated carrier overlay.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
