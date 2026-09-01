# M1 T2 Port-ABI Transformation Inventory

## Boundary

This inventory covers only rows classified `port-abi-overlay` by the semantic
divergence audit. It does not authorize a machine-controller, BIOS, BOP,
renderer, or runtime policy change.

## Inventory

| Group | Current source rows | Representation issue | M1 T2 disposition |
| --- | --- | --- | --- |
| C-VID generated rules | `evidfunc.h`, `evidgen.h`, `j_c_lang.c`, `j_c_lang.h`, `sascdef.c`, `sevid019.c`, `sevid020.c`, `vglfunc.c` | Historic rule words and GDP field offsets assume a 32-bit host aggregate. | Keep original inputs; build deterministic transformed copies and the existing `softpc-port-abi/cvidc` side table. |
| C-VID/CCPU GDP bridge | `cpu4gen.h`, `vglob.c`, `evid_c.h` | Generated rules address CCPU-owned VGA globals through 32-bit offsets while native host pointers are wider on x64. | Move the two direct private-helper includes to an external transformation; preserve the CCPU layout and map historical offsets in the side table. |
| CCPU declarations | `c_bsic.h`, `c_main.c`, `c_page.c`, `c_reg.c`, `c_xcptn.c`, `ccpusas4.c`, `fpu.c`, `popf.c`, `zfrsrvd.c` | K&R/implicit declarations and pointer-difference carriers are not valid native-width C interfaces. | Supply exact declarations or fixed-width carriers through generated/overlay headers; never alter guest register, address, or exception semantics. |
| CCPU compatibility vector | `ntstubs.c`, `ntthread.c`, `into.c`, `intx.c`, `sascdef.c` | Selected CPU_40_STYLE link needs profile/vector/thread declarations with modern C return carriers. | Isolate declaration and vector-layout corrections; product thread and interrupt hooks remain absent unless M2 defines a finite host contract. |
| Shared declarations | `base/inc/{ckmalloc,cpu_vid,egacpu,emm,gmi,host_com,host,ica,ios,sas,timeval,video}.h` | Legacy aliases mix guest-width values, host addresses, and compiler-inferred prototypes. | Audit each exported type; retain fixed guest values, make host addresses native-width only in the overlay. |
| Generated current-only inputs | `host/inc/x86/prod/{gdpvar,PigReg_c,sas4gen}.h` | Selected generated CCPU profile has no matching checked-in OpenNT peer. | Treat as reproducible port-ABI inputs and record their generator/source profile before any replacement. |

## First Transformation Cut

The first source extraction is deliberately narrow: `cpu4gen.h` and
`vglob.c` are the only remaining pristine CCPU files that directly include
`softpc_gdp_slots.h`. The build must instead consume transformed copies, as it
already does for `ev_glue.c`. The input/output transform must be idempotent
and the x64 and x86 focused BOP smoke must pass before the next group starts.

## Guardrail

If an overlay would change a value observable by guest code, it is not an ABI
transformation. Stop and assign that case either to original-machine recovery
or to a named M2 host contract.
