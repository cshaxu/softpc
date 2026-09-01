# M1 T2 S4 SAS Declaration Overlay Evidence

## Restored Inputs

The following repository inputs match the selected OpenNT SoftPC files by
SHA-256:

- `base/inc/sas.h`: `63832958D3A6B9EF2F9F209E5ADA4AF8067FAB3B5CDB6F317C5663CBD2B5F2EF`
- `base/ccpu386/c_bsic.h`: `5A7E4CEFA28C674AD3047741CF8F78C9882F3C2362AF09AE3A3DC0DF336EED17`
- `base/ccpu386/ccpusas4.c`: `2802840EF84106AE0F99C6F17AF370DA66C9381A1355570F5998039FBB33534E`

## Build Overlay

`transform_cvid_gdp.py` emits generated `sas.h`, `c_bsic.h`, and
`ccpusas4.c` inputs.  The generated SAS header makes `Length_of_M_area` a
fixed `PHY_ADDR` in every selected historical declaration branch, matching
the original CCPU provider definition.  It also supplies the existing
`sas_overwrite_memory` declaration.  The generated CCPU SAS source declares
the already-selected native host allocation functions; the generated basic
header carries `c_getEFLAGS`.

Both the CCPU and machine-device object targets depend on, and include, the
same generated-header directory.  No controller behavior, allocation logic,
or guest address value is changed.

## Focused Verification

- x64 rebuilt and linked `softpc64.exe`; `softpc-bop-smoke` passed.
- x86 rebuilt and linked `softpc32.exe`; direct `softpc-bop-smoke.exe` exit
  code was zero.
- The legacy machine-run smoke remains intentionally outside this packet: M3
  owns its executor bound.
