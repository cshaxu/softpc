# M1 T2 S10 ZFRSRVD Declaration Overlay Evidence

`base/ccpu386/zfrsrvd.c` matches the selected OpenNT SoftPC source by SHA-256:
`3BEAF1EC7CA8AC2092BB88DE4597A19DE8AECDD564FA2B55CCB1A4009041E961`.

The generated source adds only original-owner headers for address and interrupt
contracts, the existing `FLDENV` FPU prototype, and an explicit `IU32`
parameter for the historic K&R `ZFRSRVD` definition.  The ZFRSRVD operation
body, FPU behavior, exceptions, SAS behavior, and BOP dispatch are unchanged.
The transform is idempotent.

x64 rebuilt and passed `softpc-bop-smoke`; x86 rebuilt and direct
`softpc-bop-smoke.exe` returned zero.  The unbounded machine-run smoke remains
owned by M3.
