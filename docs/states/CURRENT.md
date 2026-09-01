# Current

## Current Work

### M1 T1 S2 P1: Recover original execution-source boundaries

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the selected OpenNT source tree at
  `nt/private/mvdm/softpc.new`, and the current `src/core/softpc` tree.
- **Output boundary:** restored original CCPU/C-VID execution-source inputs,
  plus only the narrowly evidenced port-ABI declarations needed to compile
  them. Runtime scheduling, BOP providers, and host callbacks remain outside.
- **Scope:** recover the original `c_main.c` BOP and execute flow and C-VID
  event glue; remove standalone scheduler policy from those machine files;
  move no helper implementation into pristine source.
- **Non-goals:** do not implement a new CPU executor, guest clock, device,
  BIOS behavior, media path, or a synthetic BOP selector service.
- **Focused verification:** source diff against original for the recovered
  flows; static absence of standalone scheduler markers in those sources;
  x64 and x86 configure/build/test after each buildable cut.
- **Full regression:** all configured CTest cases on both host widths.
- **Similar-issue sweep:** examine every CCPU/C-VID policy marker and direct
  host call, not only the previously edited HLT branch.
- **Stop condition:** stop before a portability repair changes guest-visible
  execution semantics; record it for the port-ABI overlay instead.
- **Exit criteria:** original execute/BOP/event semantics are restored or
  every remaining exception has a bounded port-ABI rationale and proof.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
