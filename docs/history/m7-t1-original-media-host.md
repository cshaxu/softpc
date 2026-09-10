# M7 T1: Original Raw-Media Host Recovery

## Goal

Audit the currently independent raw-image adapters against original
`nt_rflop.c`, `nt_fdisk.c`, and `nt_vflop.c`; recover only algorithmic host
behavior that is independent of MVDM product state.

## Scope

- Preserve original FLA/GFI/FDC and fixed-disk controllers as the sole device
  owners.
- Prefer original CHRN, disk-change, geometry, error and lifecycle algorithms
  when they can terminate at the standalone file/overlay endpoint.
- Keep the existing fixed single-floppy and single-fixed-disk profile.

## Exclusions

Do not import NT handles, VDM session paths, product configuration, DOS open
tracking, or `nt_vflop` virtual-floppy product policy merely to copy a file.

## Exit Evidence

Record a symbol-level disposition for each candidate algorithm and run x64/x86
FDC, fixed-disk, dual-media and overlay/write-protection regression tests.
