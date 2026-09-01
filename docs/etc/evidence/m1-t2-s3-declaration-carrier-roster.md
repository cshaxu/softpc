# M1 T2 S3 Declaration And Carrier Roster

## External Header Overlay

These rows add a declaration for an already-selected provider or make a host
pointer carrier native-width. They can be collected in a port-ABI header and
force-included only for the affected target: `c_bsic.h`, `c_page.c`, `c_reg.c`,
`c_xcptn.c`, `ccpusas4.c`, `popf.c`, `ckmalloc.h`, `emm.h`, `host_com.h`,
`host.h`, `sas.h`, and `timeval.h`.

Each declaration must be taken from the actual selected definition, not
inferred from an NT4 call site. `Length_of_M_area` remains a fixed guest
physical size; `host_sas_*`, storage IDs, and COM ioctl arguments remain
native host values.

## Generated Source Overlay

These inputs need a source-level adaptation because the old C grammar or a
generated C-VID aggregate must be rewritten before modern compilation:

- CCPU: `c_main.c`, `fpu.c`, `ntstubs.c`, `ntthread.c`, `sascdef.c`,
  `zfrsrvd.c`.
- C-VID/shared aggregate declarations: `cpu_vid.h`, `egacpu.h`, `gmi.h`.

The overlay may adjust declarations, explicit fixed-width pointer differences,
or selected include routing. It may not change CPU execution, vector ordering,
or VGA controller data.

## Configuration/Host-Contract Boundary

`ica.h` selects the CPU_40 interrupt type, while `ios.h` selects the full I/O
port table. `video.h` exposes optional stream-I/O state. They are not generic
declaration fixes: M2 must own their final standalone configuration after the
original source and ABI overlays are in place.

## Next Cut

The first declaration-header cut is `sas.h`/`ccpusas4.c` plus the CCPU callers
that currently rely on implicit `host_sas_init`, `host_sas_term`,
`sas_overwrite_memory`, and `c_getEFLAGS` declarations. It is a finite
representation-only change with direct x64 truncation evidence and no guest
semantic change.
