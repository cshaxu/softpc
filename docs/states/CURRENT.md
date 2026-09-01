# Current

## Current Work

### M1 T2 S3 P1: Classify remaining declaration and carrier overlays

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the remaining CCPU and `base/inc` semantic-diff rows
  classified `port-abi-overlay`.
- **Output boundary:** a declaration/carrier roster distinguishing generated
  source overlays from compiler declarations that can live in port ABI headers.
- **Scope:** inspect only type widths, return carriers, and missing prototypes.
  Preserve machine execution and all guest-visible values.
- **Non-goals:** do not implement host contracts, runtime policy, devices, or
  BOP/BIOS behavior.
- **Focused verification:** each proposed overlay is traced to an actual
  source definition and selected compile consumer on both widths.
- **Stop condition:** stop if a candidate changes a guest-visible value.
- **Exit criteria:** every remaining CCPU/declaration row has one minimal,
  testable extraction method and no duplicate declaration strategy.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
