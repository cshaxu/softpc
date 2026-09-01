# M1 T2 S5 CCPU Main Overlay Evidence

## Restored Input

`base/ccpu386/c_main.c` matches the selected OpenNT SoftPC source by SHA-256:
`F6CD45A30B6575D38EF95083601594F1403A34EBEF1278DBB8362C19F4A14700`.

## Build Overlay

The generated `c_main.c` restores only compilation declarations for existing
CCPU owners (`c_addr`, `c_seg`, PIC/timer declarations, `force_yoda`, and
`TakeNpxExceptionInt`).  It converts a same-page private host-pointer delta
to `IU32` before it joins original guest EIP arithmetic.  It does not replace
the CCPU dispatch loop, BOP path, original NTVDM conditional branches, or
standalone long-jump flow.

The transform is idempotent: applying `--ccpu-main` to its own output yields
the same SHA-256 output.

## Focused Verification

- x64 rebuilt and linked `softpc64.exe`; `softpc-bop-smoke` passed.
- x86 rebuilt and linked `softpc32.exe`; direct `softpc-bop-smoke.exe` exit
  code was zero.
- The unbounded legacy machine-run smoke remains outside this packet; M3 owns
  the executor run-slice boundary.
