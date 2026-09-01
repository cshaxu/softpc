# M1 T1 S3 C-VID Port-ABI Evidence

## Recovered Input

`src/core/softpc/base/cvidc/ev_glue.c` compares cleanly with the selected
OpenNT `softpc.new/base/cvidc/ev_glue.c` input (`git diff --no-index --quiet`,
exit 0). It contains no `softpc_gdp_slots`, `softpc_gdp_state`, or
`softpc_gdp_rule_access` reference.

## External Transformation

`scripts/transform_cvid_gdp.py --event-glue` supplies the host-width includes,
the native GDP allocation hook, and declaration-width adaptation only in the
build output. Applying it twice produced the same SHA-256:

`D532E1529F911F4576BCA2A89CC83EA76972756DFBCA50A73763B3D95624554A`.

Both recovery build graphs compile
`generated-cvid/ev_glue.c`, generated from the restored source input. The
GDP side table is external at `src/core/softpc-port-abi/cvidc/`.

## VGA Aggregate Finding

The original CCPU creates the shared `VGAGLOBALSETTINGS` aggregate. C-VID
rules address its historical 32-bit offsets. The port layer therefore maps
those offsets to the CCPU-owned native aggregate layout, including the shared
1388 slot (`video_base_ls0` in CCPU, `video_base_lin_addr` in C-VID). Slot
records grow without moving their separately allocated field storage. This
fixed the x64 null slot observed while `vga_init` ran generated C-VID rule
`S_0428_CiSetVideov7_bank_vid_copy_off_00000041`.

## Focused Verification

- Fresh x64 configure/build: `build-recovery-x64`, WinLibs MinGW GCC.
- Fresh x86 configure/build: `build-recovery-x86`, MSYS2
  `i686-w64-mingw32-gcc` with its `mingw32/bin` runtime path.
- `softpc-bop-smoke.exe` exit 0 on both widths.
- `build/output/softpc32.exe` and `build/output/softpc64.exe` were relinked
  from those builds.

The prior full CTest suite is not cited as S3 evidence. Its legacy
`softpc-machine-smoke` enters the recovered original unbounded execution path
after CCPU instruction-budget pacing was removed; M3 owns the safe executor
boundary needed to replace that test contract.
