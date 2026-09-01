# M1 T2 S11 Software-Interrupt And POPF Recovery Evidence

`into.c`, `intx.c`, and `popf.c` are restored from their selected OpenNT
inputs.  INTO and INTx retain their original `#ifdef NTVDM` software-interrupt
hook blocks, which are inactive for the standalone build.  The generated POPF
input provides only the diagnostic `stdio` declaration and the existing
`note_486_instruction` CCPU declaration.

No software-interrupt, flags, or guest-state logic is changed.  x64 rebuilt
and passed `softpc-bop-smoke`; x86 rebuilt and direct
`softpc-bop-smoke.exe` returned zero.
