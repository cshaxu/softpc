# M5 Original BIOS Reset Restoration

## Boundary

The BIOS `reset.c` source declares the original GWI table storage used by
video, keyboard and error host endpoints. The standalone build had moved that
storage ownership into platform code, changed media-equipment handling and
embedded an in-source LIM product suppression. Those are host composition
concerns, not firmware behavior.

## Resolution

`base/bios/reset.c` now matches the selected original source after
line-ending normalization. `scripts/transform_reset.py` produces the
build-only standalone copy, and a source-specific `host_def.h` overlay imports
the selected original definition before excluding the unselected LIM product
feature. The generated copy makes only these host ABI substitutions:

- its equipment word reads the actual CMOS floppy topology rather than the
  original product default that always exposed a floppy;
- its pointer-encoded graphics scalar is converted through `ULONG_PTR` for
  x64 safety.

Before first firmware reset, `softpc_machine_reset` calls one outer host
binding function that assigns the already-existing renderer, keyboard and
error endpoint tables to the original firmware-owned globals.

The original reset code again determines its equipment flags and executes its
original non-NT reset path. The binding contains no BIOS state and runs before
any firmware action; the frontend still receives only copied presentation
frames through the runtime.

## Verification

- Direct comparison with the selected original `reset.c` succeeds after
  line-ending normalization.
- Freshly relinked `softpc-machine-smoke`, `softpc-dual-media-smoke` and
  `softpc-runtime-smoke` passed on x86 Clang and x64 MinGW.
- Fresh serial CTest passed `18/18` on both host widths.
- Fresh x86 and x64 `softpc-real-boot-smoke --floppy O:\assets\fdd.img
  --hdd O:\assets\hdd1.img --overlay --windows-setup` runs reached the
  Windows Setup Welcome screen without changing either guest image.
