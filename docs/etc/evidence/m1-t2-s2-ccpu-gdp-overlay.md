# M1 T2 S2 CCPU GDP Overlay Evidence

## Restored Inputs

`base/ccpu386/cpu4gen.h` and `base/ccpu386/vglob.c` match the selected
OpenNT inputs by SHA-256. Neither source directly includes
`softpc_gdp_slots.h`.

## Build Overlay

`transform_cvid_gdp.py --ccpu-cpu4gen` emits the generated header used by
`cpu4.h`; it adds the GDP side-table declaration and the exact CCPU return
carriers needed by the selected generated profile. `--ccpu-vglob` emits the
VGA-global access source with the C-VID aggregate declaration and the same
side table. `softpc-ccpu-port-abi-inputs` makes both generated outputs explicit
Ninja dependencies.

The x64 compile command and compiler diagnostics identify
`build-recovery-x64/generated-ccpu/cpu4gen.h` as the included header, rather
than the pristine source input.

## Focused Verification

- `--ccpu-cpu4gen` applied twice produced equal SHA-256 output.
- x64 and x86 both rebuilt and relinked their standalone launchers.
- `softpc-bop-smoke.exe` exited zero on both widths.

This does not claim the legacy unbounded machine-run smoke as a regression;
M3 still owns the executor boundary required for that proof.
