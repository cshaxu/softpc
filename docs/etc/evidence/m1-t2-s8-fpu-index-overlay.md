# M1 T2 S8 FPU Private-Index Overlay Evidence

## Restored Input

`base/ccpu386/fpu.c` matches the selected OpenNT SoftPC source by SHA-256:
`1309239998F72B64583B184D33623C763C3C98EA185081F8D8B294CD9B8C524F`.

## Build Overlay

The generated FPU source provides the native C diagnostic declaration and
converts only `TOSPtr - FPUStackBase` to `IU32` before it joins the original
guest register-index arithmetic.  The two pointers delimit the source's own
eight-entry FPU stack; no FPU arithmetic, stack content, exception, or BOP
code is changed.  `--ccpu-fpu` is idempotent.

## Focused Verification

- x64 rebuilt and linked `softpc64.exe`; `softpc-bop-smoke` passed.
- x86 rebuilt and linked `softpc32.exe`; direct `softpc-bop-smoke.exe` exit
  code was zero.
- The unbounded legacy machine-run smoke remains owned by M3.
