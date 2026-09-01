# Current

## Current Work

### M1 T2 S2 P1: Externalize the remaining CCPU GDP bridge

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the selected original CCPU `cpu4gen.h` and `vglob.c`,
  plus the existing C-VID GDP side table and transformation script.
- **Output boundary:** pristine CCPU inputs compiled through deterministic
  transformed copies that add the private GDP slot declaration externally.
- **Scope:** remove only direct `softpc_gdp_slots.h` inclusion from the two
  CCPU sources. Preserve their generated spelling, execution flow, and VGA
  global layout.
- **Non-goals:** do not change C-VID rule behavior, BIOS/ROM/BOP, devices,
  host services, or runtime scheduling.
- **Focused verification:** transform idempotence; x64/x86 compile graphs use
  the generated copies; focused BOP smoke passes on both widths.
- **Stop condition:** stop if the transformed copies require a non-ABI change
  to CCPU behavior or a product host service.
- **Exit criteria:** neither pristine CCPU source includes the private GDP
  slot header directly, and both builds consume the generated replacements.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
