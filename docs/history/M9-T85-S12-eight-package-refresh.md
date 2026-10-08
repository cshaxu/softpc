# M9 T85 S12 eight-package refresh closure

S12 imports the complete shared Lib, Common, x86 and IBM PC source/test corpus
from NXVM and keeps the shared test registration helper LF-canonical locally.
The implementation P is `073e9d8b`.

The frozen import ledger contains 109 Lib, 23 Common, 95 x86 and 125 IBM PC
source paths, plus 49 Lib, 20 Common, 196 x86 and 235 IBM PC test paths.
Every imported path and `test/register.cmake` was byte-identical to the
admitted frozen source. A final read-only comparison shows the same exact
equality with current NXVM `9ab7ed6b9a067ded717d7f75c138ac5be896ad18`.

Both package configurations build successfully. Full background CTest passes
479/479 on x64 in 74.29 seconds and 479/479 on x86 in 224.21 seconds. Native
desktop tests were not run because no desktop interval was reserved. The only
local non-corpus change is a `RUN_SERIAL` declaration for the root
`softpc-command-provider-smoke`: it owns fixed test fixtures and otherwise
collides with concurrent repository tests. No production shared-component API
or product receiver changes were added for that correction.

The owner subsequently requested actual clean-first rebuilds of both package
executables and explicitly authorized shipping the owner-edited package INI.
P6 therefore carries `assets/binary/softpc32.exe`, `assets/binary/softpc64.exe`
and `assets/binary/softpc.ini`. This exception does not authorize future agent
edits to the INI. T85 remains open; only S12 is closed.
