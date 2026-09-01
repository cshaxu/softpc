# M1 T2 S7 Remaining CCPU Generated-Source Classification

## Admitted Representation-Only Candidates

| Input | Direct edit to remove | Generated overlay responsibility | Owner evidence |
| --- | --- | --- | --- |
| `fpu.c` | native `stdio` include and private FPU-stack pointer-difference cast | retain fixed `IU32` register-index arithmetic only | FPU source owns both stack operands; no external scheduling call is involved |
| `sascdef.c` | typed null for `Sas_overwrite_memory` vector slot | retain the selected `TYPE_sas_overwrite_memory *` null carrier | `ccpusas4.c` declares the selected vector-function type |
| `zfrsrvd.c` | added address/interrupt includes, `FLDENV` declaration, and explicit K&R parameter form | supply original-owner declarations and modern callable signature only | `c_addr.h`, `c_intr.h`, `intx.h`, and `fpu.c` provide the called contracts |

## Deferred Boundary

No admitted row changes `c_cpu_simulate`, the long-jump execution path, BOP
dispatch, generated instruction rules, FPU arithmetic, or a device event
source.  Any source that requires such a change is deferred to M3, where the
run-slice and timer/event contract is explicit.
