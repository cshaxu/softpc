# Standalone SoftPC controller audit

Scope: the fixed `CPU_40_STYLE` machine in this repository.  Each original
component is classified by its present standalone disposition.

| Original group | Disposition | Evidence |
| --- | --- | --- |
| CCPU / C-VID | retained | `base/ccpu386` manifest, C-VID generated rules, original SAS/PIC event glue |
| System | retained | original `ica.c`, `timer.c`, `quick_ev.c`, `at_dma.c`, `cmos.c`, ROM and interrupt code are target sources |
| BIOS machine services | retained | original reset/bootstrap/equipment/memory/time/keyboard/disk services; narrow BOP table is ROM-to-C routing only |
| FDC / GFI | retained plus raw-media port | original `fla.c`, `floppy.c`, `gfi.c`, `gfi_mpty.c`; `softpc_gfi_image.c` owns only raw image bytes |
| Fixed disk | retained plus raw-media port | original `fdisk.c` and `diskbios.c`; `host_fdisk_*` owns only raw file I/O and geometry |
| Video | retained plus DIB port | original CGA/EGA/VGA/V7 controller and `nt_cga/nt_ega/nt_vga/nt_graph`; window consumes DIB only |
| Keyboard / mouse | retained plus host mapping | original keyboard, PPI and mouse controllers; original `nt_keycd.c` maps Win32 key events |
| Serial / printer / sound | retained plus endpoint port | original `com.c`, `printer.c`, `rs232_io.c`; original `nt_com/nt_lpt/nt_sound` adapted at endpoint only |

## Explicit exclusions

| Original file/group | Reason it is outside the standalone machine |
| --- | --- |
| `cmosnt.c`, `nt_timer.c` | Windows NT timer, handle, thread and VDM lifecycle host code |
| `nt_rflop.c`, `nt_fdisk.c` | NT physical-device handles, I/O controls and async worker paths; raw images use a thin file port instead |
| `nt_vflop.c`, `gfi_ibm.c`, `gfi_sflp.c` | SID B: format or SLAVEPC remote floppy protocol, not the fixed raw-image machine |
| `nt_input.c`, `nt_mouse.c` | console focus/lock-state and NTVDM input lifecycle; mapping and controller portions are already covered above |
| `virtual.c`, DOS, WOW, VDD, DPMI, BaseSrv/CSR | product/session or multi-instance semantics, not SoftPC machine behavior |
| `bios.c` full table | contains DOS/VDD/WORM/product selections; standalone retains only its original-machine entries through the narrow BOP bridge |

## Required closure checks

1. `test/support/standalone_source_boundary.cmake` protects retained controllers and product exclusions.
2. Build and test both `build-verify-x64` and `build-verify-x86-i686`.
3. Run real-ROM boot probes with floppy, HDD and both media on both builds.
