# M9 T56 S6: xasm32 source provenance

## Frozen source

`common/xasm32` is a byte-for-byte source import from NXVM commit
`e894ef8949a0d92719678f9ebd6cec2793256ac0`, directory
`src/common/xasm32`. It adds no SoftPC command, executor, UI, MVDM, or
runtime route.

| File | SHA-256 |
| --- | --- |
| `aasm32.c` | `A8F551CF16479656B10C0584FB508563992DCEA1568FB9F47914A173CDE0A63C` |
| `aasm32.h` | `2844ADD54C5F3F1729E9D64E034A004019737E70C70D4C29025D2F60E0D789A7` |
| `dasm32.c` | `A34778A9FC87A0D3B661E84144D84E32EA6228A3E8DB2290FAE6E66C8F257CE4` |
| `dasm32.h` | `2A004BADFB44BAA0F32C6719572DD103A53E51C638D3CB36196C0D260F2B1516` |
| `xasm32.c` | `B7A981F097E14CAB38F7FC8676B342A1C1C52944821EA0CAB72532807377C523` |
| `xasm32.h` | `94084A16FFDC73AEE1402E8B9052F14C4473B9EA1BC707A45546C21D73281C8E` |
| `xasm32_interface.h` | `D52696BB34EDC97BA3A6BD4D427403D5D72BA1AB800CAE679D8AE531BDA148E2` |

## Verification and closure

`common-xasm32` compiles the original three C files and exports the copied
interface. The copied smoke and contract tests prove assembly, disassembly,
paragraph assembly, capacity limits and unchanged-output error contracts.
The imported file set and every hash above were compared against the frozen
source before closure. x64 and x86 full CTest both passed 62/62; strict lib
CTest remained 8/8. Package hashes are `softpc32.exe`
`58D3B35C91B07FF3A3E2C4E0A343BDF235B0C018BCEE7BD603AB800A57282230` and
`softpc64.exe` `01DF2F0754A8B89944A26F871085C0C79B62BA42C9D33E5A8B436C4B0D352E5E`.

S6 is closed. S7 may import the separately frozen debug corpus; it must not
turn this dormant xasm32 component into an unadmitted product command path.
