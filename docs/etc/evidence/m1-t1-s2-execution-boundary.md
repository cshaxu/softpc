# M1 T1 S2 Execution-Boundary Evidence

## Recovered Control Flow

`base/ccpu386/c_main.c` again contains the original C4 C4..C7 and D6 BOP
decode/control flow. The standalone instruction budget, executor wake,
standalone clock consumption, private BOP dispatcher, and trace-only interrupt
branch were removed from that machine source.

The remaining `c_main.c` divergence is only the existing x64 declaration and
pointer-difference representation work. It is carried forward to the M1
port-ABI overlay task; it contains no scheduler, host wait, or BOP service
policy.

## Historical Host Contract Found

The restored original C4 fast-BOP path references `EDL_fast_bop`, but neither
the selected OpenNT `softpc.new` tree nor the broader OpenNT MVDM tree defines
it. The selected fixed ROMs use 65 C4 BOPs, all encoded as `C4 C4`; no ROM
contains C4 C5, C4 C6, or C4 C7. Their BOP-FE return form is handled directly
by original `c_main.c` and does not call the missing symbol.

`src/host/softpc_compat/edl_fast_bop.c` therefore owns the historical host
symbol. It rejects an unselected-ROM fast-BOP explicitly rather than supplying
product behavior. This is a compatibility-host boundary, not a core fallback.

## Focused x64 Proof

On the clean `build-recovery-x64` Ninja graph:

1. The recovered `c_main.c` compiled with the x64 MinGW GCC toolchain.
2. `softpc64.exe` linked successfully after the external host symbol was
   supplied.
3. `softpc-bop-smoke.exe` exited with status 0, proving that the existing
   finite firmware/hardware `BIOS[]` table still serves original BOP calls.

## Runtime-Boundary Finding

Full CTest cannot yet be used as an S2 success substitute. Its first
`softpc-machine-smoke` calls the transitional public run-slice API. With the
standalone instruction budget removed, that call enters original unbounded
`c_cpu_simulate()` and did not return after more than 90 seconds; the exact
CTest and smoke processes started for the observation were then stopped.

This is evidence for M3, not permission to restore instruction pacing in
CCPU. M3 must prove a historical safe return boundary and place single-
executor scheduling outside the machine. x86 full regression waits for the
port-ABI overlay; it is not claimed by this evidence.
