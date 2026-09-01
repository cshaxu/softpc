# Current

## Current Work

### M1 T2 S10 P1: Externalize ZFRSRVD CCPU declarations

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** selected original `zfrsrvd.c` and its existing CCPU/FPU
  declaration providers.
- **Output boundary:** generated source declarations that compile the original
  ZFRSRVD callable interface on both widths.
- **Scope:** externalize only published original headers, the `FLDENV`
  prototype, and the historic K&R parameter form.
- **Non-goals:** do not change FPU operations, exceptions, instruction
  decoding, BOP, SAS behavior, device state, or runtime pacing.
- **Focused verification:** source hash, generated-output idempotence,
  dual-width rebuild, and bounded BOP smoke.
- **Stop condition:** stop if an overlay reaches any ZFRSRVD operation body.
- **Exit criteria:** `zfrsrvd.c` is pristine and compiles through a bounded
  generated declaration overlay.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
