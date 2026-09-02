# M7 T1 S1: Original Raw-Media Host Closure

## Result

The original OpenNT raw-media host files have been audited at symbol level.
The fixed standalone profile already retains the one independently useful
`nt_rflop` algorithm, multi-sector `update_chrn`, behind original FLA/GFI/FDC
ownership. No remaining candidate can be copied wholesale without adding NT
physical-device, PDB, VDM-session, DOS-product or second-controller behavior.

## Evidence

- The [symbol disposition](../etc/evidence/m7-t1-original-media-host-audit.md)
  covers `nt_rflop.c`, `nt_fdisk.c` and `nt_vflop.c`, including media-change,
  geometry, read/write, format, locking, lifecycle and sidecar-media routes.
- Existing FDC coverage proves original command/DMA/result behavior, including
  raw 512-byte restrictions, CHRN progression, format and empty-drive
  restoration. Fixed-disk and dual-media coverage prove original POST,
  controller and overlay/write behavior.
- The Win32 smoke fixture now waits for its owning UI thread to publish the
  fixed guest client geometry before checking it; this removes a test-only
  startup race without changing the machine or frontend behavior.
- Clean x64 and x86 serial CTest both passed 18/18.

## Closure

M7 T1 meets its source-disposition and dual-width proof criteria. M7 T2 may
proceed with the single-image, guest-owned partition compatibility proof.
