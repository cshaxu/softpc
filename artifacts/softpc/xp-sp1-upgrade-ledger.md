# XP SP1 machine-baseline upgrade ledger

## Scope

Upgrade the standalone machine from its current OpenNT/NT4-era source
baseline to the `XPSP1/NT/base/mvdm/softpc.new` machine baseline without
reintroducing the XP NTVDM product host.

The selected target is the normal i386 AT/V7 configuration only:

```
CPU_40_STYLE + CCPU + C_VID + CPU_486 + V7VGA + PROD
```

This matches XP SP1's `obj.vdm/CDEFINE.INC` selected CCPU definition.  PC-98,
ARC, Alpha, MIPS, PPC and language-specific trees are out of scope.

## Frozen standalone compatibility boundary

The following files are owned by this repository and are not replaced by XP
product-host sources:

| Boundary | Owner | Reason |
|---|---|---|
| `softpc_machine.c` | standalone machine API | create/reset/run lifecycle |
| `softpc_standalone_platform.c` | standalone platform port | host clock, file and input endpoints |
| `softpc_gfi_image.c` | media port | readonly/direct/overlay image ownership |
| `softpc_standalone_dib.c` | presentation port | DIB surface and repaint callback |
| `softpc_device_bop.c` | ROM device bridge | narrow mechanical device services only |

XP `nt_bop.c`, `nt_vdm.c`, `nt_vdd.c`, `nt_pif.c`, `nt_inthk.c`,
`nt_sas.c`, `nt_mem.c`, `nt_umb.c`, `nt_emm.c` and `x86_emm.c` are product
host or DOS/memory-manager material.  They must not enter the standalone
build.

## First audit: system core

Source compared: `winxpscodes/Source/XPSP1/NT/base/mvdm/softpc.new` against
`src/core/softpc`.

| Group | XP relation | Upgrade treatment |
|---|---|---|
| `base/system/at_dma.c` | Current tree already removes XP-only SoundBlaster include and PC-98 branches. | Keep standalone selected-machine form; compare functional non-PC-98 changes during controller batch. |
| `base/system/ica.c` | Current tree already has the XP normal-AT PIC path; its remaining source delta is PC-98 removal and harmless typing cleanup. | Treat as baseline-aligned; retain it while updating shared headers. |
| `base/system/quick_ev.c` | XP's normal machine dispatcher is the appropriate source; XP `qevnt.c` is explicitly NTVDM-specific. | Keep `quick_ev.c`; never substitute `qevnt.c`. |
| `base/system/timer.c` | XP includes NTVDM timing paths; current standalone branch prevents zero-delay quick-event recursion on a modern host. | Preserve standalone branch; port only demonstrated machine fixes with a timer regression. |
| `base/system/cmos.c` and `rom.c` | Current files retain XP machine code but adapt config scalar width and machine-owned media presence. | Preserve those x86/x64 and image-backend adaptations. |
| `base/system/idetect.c` | XP references the original private MVDM include layout. | Keep standalone ABI include adaptation; do not import VDM host behavior. |
| `base/system/qevnt.c`, `system.c` | XP-only, NTVDM-specific startup/dispatch. | Exclude. |

## Upgrade order and acceptance gates

1. Shared machine headers and system/support code, retaining the entries
   above as explicit compatibility islands.
2. CCPU plus C-VIDC as one generated-executor batch.
3. Disk/FDC/GFI and BIOS startup as one media batch.
4. Keyboard/mouse and serial/parallel controller batches.
5. Video controller plus `nt_cga`, `nt_ega`, `nt_vga`, `nt_graph`, and
   `nt_munge`, retaining only the standalone DIB outlet.

Every batch must build both host widths, run the full CTest suite, cover
floppy-only, hard-disk-only and dual-media attachment, and preserve the
readonly/direct/overlay contract.  The video batch additionally requires
CGA/EGA/VGA/V7 rendering and dirty-DIB tests.
