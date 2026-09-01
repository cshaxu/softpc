# M1 T2 S11 CCPU Page/Register Declaration Overlay Evidence

`base/ccpu386/c_page.c` and `base/ccpu386/c_reg.c` are restored byte-for-byte
from their selected OpenNT peers.  Their generated inputs add only the
existing Yoda diagnostic macro header and the standard variadic `printf`
declaration, respectively.  Neither source's paging or register logic is
rewritten.

Both transforms are idempotent.  x64 rebuilt and passed `softpc-bop-smoke`;
x86 rebuilt and direct `softpc-bop-smoke.exe` returned zero.  The remaining
CCPU exception source is deliberately handled separately because its original
NTVDM hook blocks must remain compile-time inactive in the standalone build.
