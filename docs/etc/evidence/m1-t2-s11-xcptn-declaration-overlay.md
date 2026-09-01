# M1 T2 S11 CCPU Exception Declaration Overlay Evidence

`base/ccpu386/c_xcptn.c` matches the selected OpenNT SoftPC source by SHA-256:
`E1F8F19A1F513402A10DC13EAF3B43D45A208FD71A72692F690F808A7E7762B2`.

The generated source adds the original Yoda diagnostic header and removes only
the historical macro-local `host_getenv` declaration, because the standalone
host header already maps it to the C runtime's correctly declared `getenv`.
The original `#ifdef NTVDM` exception-hook blocks remain present and inactive
in the standalone configuration.  No exception flow, interrupt handling, or
guest state operation changes.

The transform is idempotent.  x64 rebuilt and passed `softpc-bop-smoke`; x86
rebuilt and direct `softpc-bop-smoke.exe` returned zero.
