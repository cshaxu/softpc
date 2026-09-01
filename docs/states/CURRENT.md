# Current

## Current Work

### M1 T2 S5 P1: Externalize remaining generated CCPU declaration overlays

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the selected original CCPU generated-source inputs
  identified in the declaration roster, beginning with `c_main.c` and
  `ntstubs.c`.
- **Output boundary:** generated compilation inputs plus narrowly scoped
  port-ABI declarations for pre-existing standalone providers.
- **Scope:** externalize only missing prototypes, native pointer carriers, and
  historic C grammar incompatibilities necessary to compile the selected
  CCPU executor on both widths.
- **Non-goals:** do not alter executor control flow, BOP dispatch, guest
  register values, controller behavior, or runtime pacing.
- **Focused verification:** source hashes, generated-output idempotence,
  dual-width rebuild, and bounded BOP smoke.
- **Stop condition:** stop if an overlay needs a change to emitted CCPU rule
  logic or guest-visible state.
- **Exit criteria:** each admitted generated-source input is pristine in the
  tree and has one traceable, repeatable compilation overlay.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
