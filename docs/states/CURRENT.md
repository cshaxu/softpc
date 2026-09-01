# Current

## Current Work

### M1 T1 S1 P1: Freeze the pristine recovery baseline

- **Owner:** repository owner (single-person dual-role execution)
- **Input boundary:** the selected OpenNT source tree at
  `nt/private/mvdm/softpc.new`, and the current `src/core/softpc` tree.
- **Output boundary:** a tracked divergence ledger and source manifest only;
  no machine-source transformation is admitted in S1.
- **Scope:** enumerate every same-name C/H source divergence, identify the
  source families that contain standalone policy, and assign the next
  recovery cut to pristine restoration, port-ABI overlay, or compatibility
  host work.  Record files with no original peer separately.
- **Non-goals:** do not modify guest media, change controller behavior, add a
  scheduler, or infer a source contract that cannot be evidenced.
- **Focused verification:** reproducible hash comparison against the selected
  source tree; static sweep for standalone policy markers; local-link check.
- **Full regression:** not applicable: S1 does not modify the runnable path.
- **Similar-issue sweep:** include CCPU, C-VID, BIOS, controllers, BOP/ROM,
  and every imported `nt_*` host source; do not limit the audit to the prior
  pacing changes.
- **Stop condition:** stop and report if a changed current file lacks a
  recognizable original peer or a supported disposition.
- **Exit criteria:** the ledger accounts for all 106 divergent original-peer
  files and 9 no-peer files, and defines the first source-changing S2 cut.

## Current Technical Baseline

The current runnable baseline is the direct-launch SoftPC VM on `main`. It is
transitional: standalone-specific behavior still appears in original CCPU and
original host-renderer paths. The target architecture is defined in
[System Architecture](../design/ARCHITECTURE.md).  Local real-boot traces are
uncommitted evidence and are not task inputs unless a later packet names them.
