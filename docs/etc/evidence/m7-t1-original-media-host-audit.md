# M7 T1 Original Raw-Media Host Audit

## Scope

This audit compares the original OpenNT SoftPC media-host candidates to the
fixed standalone raw-image endpoints. It classifies host algorithms; it does
not make the original physical-drive, VDM-session, or DOS-product layers part
of the standalone machine.

## `nt_rflop.c`

| Original behavior | Standalone disposition |
| --- | --- |
| `update_chrn` multi-sector result progression | **Retained.** `softpc_gfi_update_chrn` preserves this algorithm after original FLA/GFI DMA transfer. |
| GFI command/result vector shape | **Retained at boundary.** The standalone adapter installs the same GFI callbacks; original FLA owns command phases, ports, DMA and IRQ result delivery. |
| Raw read/write/format data movement | **Standalone endpoint.** A `.img` is a sequential 512-byte stream, so the adapter supplies only bounded stream I/O behind the original controller. |
| Physical-drive open, verify, retry, locking and PDB ownership | **Excluded.** These require NT device handles, filesystem locking and MVDM PDB/session state. |
| Physical density probing and motor/change-line heuristic | **Excluded for the fixed mounted image.** Image size selects the drive geometry; a configured image is stable until detached. |
| FDC worker thread and host timer ownership | **Excluded.** The independent runtime already preserves one executor and original controller timer ordering; a second worker would violate that boundary. |

## `nt_fdisk.c`

| Original behavior | Standalone disposition |
| --- | --- |
| `host_fdisk_get_params`, read and write endpoint | **Retained in role.** The standalone fixed-disk endpoint supplies image geometry and bounded sector I/O to the original `fdisk.c` controller. |
| NT volume open, sharing, alignment IOCTLs, verify IOCTL and retry UI | **Excluded.** They target host drive letters and NT privileges rather than a configured image. |
| PDB ownership, idle close and DOS termination/reset cleanup | **Excluded.** A single configured image has process lifetime ownership and no DOS product dispatcher. |
| Controller-visible disk presence and geometry | **Retained.** Original CMOS/fixed-disk POST sees only the fixed image attachment; the host does not parse partitions or assign guest drive letters. |

## `nt_vflop.c`

| Original behavior | Standalone disposition |
| --- | --- |
| B:-only virtual disk and `.SID` variable-sector map | **Excluded.** The selected fixed profile uses a normal IBM raw A: image, and there is no generic media/container contract for the sidecar format. |
| Host configuration directory, ownership acquisition and UI-triggered re-open | **Excluded.** These are product/session mechanisms. |
| FDC command implementation | **Excluded.** It would become a second FDC-server implementation beside original FLA/GFI. |

## Conclusion

No additional source file can be imported wholesale without importing the
MVDM product host. The one independently useful original raw-floppy algorithm,
`update_chrn`, is already retained. The current raw-image endpoints are the
correct thin termination for original controller ownership. M7 T2 therefore
tests one complete physical image and its guest-owned partition table rather
than adding host drive policy.
