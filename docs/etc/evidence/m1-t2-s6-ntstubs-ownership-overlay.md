# M1 T2 S6 CCPU Compatibility-Stub Ownership Evidence

## Restored Input And Owners

`base/ccpu386/ntstubs.c` matches the selected OpenNT SoftPC source by
SHA-256: `E25542825EB5EFB323DEAE41E8D8368E75E5DCC2ACABAD02A974F602E67641E8`.

The selected standalone link already has these original owners:

- C-VID `ev_glue.c`: `Gdp` allocation and lifetime;
- system `rom.c`: `copyROM`;
- CCPU `fpu.c`: `initialise_npx` and `npx_reset`.

## Build Overlay

The generated compatibility source routes `evidgen.h` to the active C-VID
vector, declares `Gdp` rather than defining it, and removes only the three
empty duplicate ROM/FPU stubs.  It adds the standard C declaration for its
existing diagnostic `printf` call.  CCPU vector behavior, SAS wrappers, and
machine initialization order are unchanged.

Applying `--ccpu-ntstubs` to its own output is idempotent.

## Focused Verification

- x64 rebuilt and linked `softpc64.exe`; `softpc-bop-smoke` passed.
- x86 rebuilt and linked `softpc32.exe`; direct `softpc-bop-smoke.exe` exit
  code was zero.
- The unbounded legacy machine-run smoke remains owned by M3.
