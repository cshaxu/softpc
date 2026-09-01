# Current

## Current Work

### M2 T4 S1: Recover original timer/ICA host compatibility

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** the owner directed execution of the ordered
  standalone SoftPC queue. M3/T3 established that the original timer/ICA
  contract is the immediate prerequisite for the requested single-executor
  NXVM-style runtime.
- **Objective:** recover the finite original SoftPC host heartbeat contract:
  an ICA synchronization boundary, worker-owned `time_tick()`/RTC progression,
  and CCPU `CPU_TIMER_TICK` publication. Preserve original CCPU40, PIT/PIC,
  CMOS/RTC, devices, BIOS, ROM/VGA ROM and BOP behavior.
- **Input and output boundary:** input is the selected original
  `host/src/nt_eoi.c`, `host/src/nt_timer.c`, `base/system/timestrb.c`, and
  current standalone host port. Output is a reproducible port-ABI build
  selection plus a `host/softpc-compat` Win32 capability implementation; no
  standalone-only branch is added to pristine SoftPC source.
- **Non-goals:** no NTVDM/WOW/DEM/CSR/VDD/product-service import; no controller
  rewrite; no BOP selector change; no guest-media mutation; no runtime
  mailbox/frontend conversion; no pause/reset/stop policy; no profile or
  multi-session feature.
- **Verification:** focused heartbeat/tick-cardinality fixture and bounded
  dual-media POST probe, then x64 and x86 build plus the existing BOP, timer,
  IRQ and VGA checks. User media and `build/output/softpc.ini` remain untouched.
- **Similar-issue sweep:** `time_strobe`, RTC, PIT/IRQ0, HLT wake, FDC POST,
  keyboard wake, timer start/stop/reset, ICA locking, x86/x64 CCPU40 builds.
- **Stop condition:** a required dependency is NTVDM product behavior rather
  than a finite machine/host capability; a change would require CCPU/BOP
  semantics, a guest clock derived from instruction count, or unsynchronised
  controller mutation.
- **Exit criteria:** one original-source-shaped heartbeat path advances timer
  and RTC work exactly once per host pulse under the recovered lock, publishes
  `CPU_TIMER_TICK`, survives bounded dual-media POST, and passes focused x86
  and x64 verification. Only then may M3 runtime be re-admitted.

## Current Technical Baseline

The machine remains direct-launch transitional behavior on `main`. M3/T3
closed its executor-boundary audit and transferred the timer/ICA prerequisite
to this packet. The target architecture remains in
[System Architecture](../design/ARCHITECTURE.md).
