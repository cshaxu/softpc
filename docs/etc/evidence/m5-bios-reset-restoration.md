# M5 Original BIOS Reset Restoration

## Boundary

The BIOS `reset.c` source declares the original GWI table storage used by
video, keyboard and error host endpoints. The standalone build had moved that
storage ownership into platform code, changed media-equipment handling and
embedded an in-source LIM product suppression. Those are host composition
concerns, not firmware behavior.

## Resolution

`base/bios/reset.c` now matches the selected original source after
line-ending normalization. The standalone CMake target applies `-ULIM` only
to this compilation unit, keeping the absent LIM product layer out of the
link without changing the source. Before first firmware reset,
`softpc_machine_reset` calls one outer host binding function that assigns the
already-existing renderer, keyboard and error endpoint tables to the original
firmware-owned globals.

The original reset code again determines its equipment flags and executes its
original non-NT reset path. The binding contains no BIOS state and runs before
any firmware action; the frontend still receives only copied presentation
frames through the runtime.

## Verification

- Direct comparison with the selected original `reset.c` succeeds after
  line-ending normalization.
- `softpc-machine-smoke`, `softpc-dual-media-smoke` and
  `softpc-runtime-smoke` passed on x86 Clang and x64 MinGW.
- Serial CTest passed `18/18` on both host widths.
