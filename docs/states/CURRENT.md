# Current

## Current Work

### M3 T4 S1: Establish the single-executor runtime foundation

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** the owner directed execution of the ordered
  standalone SoftPC queue and approved the NXVM-style outer architecture.
  M3/T3 proved that legacy direct `softpc_machine_run(slice)` has no safe
  scheduling boundary. Its follow-up timer experiment proved that a real
  heartbeat cannot be enabled beneath that API.
- **Objective:** introduce a VM-owned executor lifecycle and copied host
  command/input mailbox outside pristine SoftPC. The runtime, not a frontend,
  owns reset, run, heartbeat activation and teardown; console/window become
  producers and snapshot consumers only.
- **Input and output boundary:** input is the current `src/vm` direct-run
  shell, `softpc_machine` public mechanics, original CCPU event handling, and
  standalone timer port. Output is `vm/runtime` with explicit state ownership,
  command records and copied presentation snapshots. Pristine SoftPC source,
  controllers, BIOS/ROM/VGA ROM and BOP tables remain unchanged.
- **Non-goals:** no NTVDM/WOW/DEM/CSR/VDD/product-service import; no controller
  rewrite; no BOP selector change; no guest-media mutation; no profile or
  multi-session feature; no instruction-budget, BOP-FE or `c_cpu_unsimulate()`
  scheduling shortcut.
- **Verification:** a bounded runtime lifecycle probe, copied keyboard input
  during continuous execution, start/pause/stop/reset behavior, and a
  presentation snapshot freshness check; then x64/x86 BOP, timer, IRQ, VGA
  and dual-media checks. User media and `build/output/softpc.ini` remain
  untouched.
- **Similar-issue sweep:** nested `host_simulate`, FDC POST, HLT wake, timer
  start/stop/reset, pause/stop semantics, ICA ownership, RDP input and both
  host widths.
- **Stop condition:** a requested runtime lifecycle action has no finite
  CCPU-level boundary without reinterpreting BOP, changing controller timing
  or terminating a live CCPU thread unsafely.
- **Exit criteria:** one executor owns all machine mutation, frontends do not
  call `softpc_machine_run()` or mutate input/device state, and a real host
  heartbeat is active only while that executor runs. The full lifecycle and
  copied-input proof must pass on x64 and x86.

## Current Technical Baseline

The machine remains direct-launch transitional behavior on `main`. M3/T3
closed its executor-boundary audit and rejected an unsafe timer retrofit under
the legacy slice API. M3/T4 further established that the original
`c_cpu_unsimulate()` transfer targets the current nested `host_simulate` frame,
not a VM lifecycle frame; its unintegrated runtime trial was removed. The
active packet is at its stop condition pending explicit approval of the narrow
outer-frame port-ABI adapter recorded in
[M3 T4 lifecycle-boundary evidence](../etc/evidence/m3-t4-s1-lifecycle-boundary.md).
The target architecture remains in [System Architecture](../design/ARCHITECTURE.md).
