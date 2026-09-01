# Current

## Current Work

### M4: Monitor, window, and real-media acceptance

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** the owner directed execution of the ordered
  standalone SoftPC queue and approved the NXVM-style outer architecture.
  M3/T3 proved that legacy direct `softpc_machine_run(slice)` has no safe
  scheduling boundary. Its follow-up timer experiment proved that a real
  heartbeat cannot be enabled beneath that API.
- **Objective:** exercise the completed runtime-client shell with fixed
  `softpc.ini` media: prove monitor control, console/window input (including
  RDP), and Windows Setup through the original renderer on both host widths.
- **Input and output boundary:** input is the current `src/vm` direct-run
  shell, `softpc_machine` public mechanics, original CCPU event handling, and
  standalone timer port. Output is `vm/runtime` with explicit state ownership,
  command records and copied presentation snapshots. Pristine SoftPC source,
  controllers, BIOS/ROM/VGA ROM and BOP tables remain unchanged.
- **Non-goals:** no NTVDM/WOW/DEM/CSR/VDD/product-service import; no controller
  rewrite; no BOP selector change; no guest-media mutation; no profile or
  multi-session feature; no instruction-budget, BOP-FE or `c_cpu_unsimulate()`
  scheduling shortcut.
- **Verification:** actual console and window interaction probes, fixed-media
  boot/Windows Setup evidence, and x64/x86 BOP, timer, IRQ, VGA and dual-media
  checks. User media and `build/output/softpc.ini` remain untouched.
- **Similar-issue sweep:** nested `host_simulate`, FDC POST, HLT wake, timer
  start/stop/reset, pause/stop semantics, ICA ownership, RDP input and both
  host widths.
- **Stop condition:** a runtime command requires direct frontend mutation,
  another executor, BOP reinterpretation, controller-timing change, or unsafe
  termination of a live CCPU thread.
- **Exit criteria:** the fixed launchers demonstrably boot and respond without
  input lag in console and window, and the Windows Setup path reaches its
  usable welcome screen on both host widths without replacing SoftPC devices.

## Current Technical Baseline

M3/T4 provided the generated outer-frame lifecycle adapter and a persistent
runtime executor. Console and window are now mailbox-only clients, enforced
by the source-boundary test. The active M4 work is runtime acceptance with
real fixed media and RDP-friendly interaction. See [lifecycle
evidence](../etc/evidence/m3-t4-s1-lifecycle-boundary.md), [runtime mailbox
evidence](../etc/evidence/m3-t4-s2-runtime-mailbox.md), and [frontend client
evidence](../etc/evidence/m3-t4-s3-frontend-clients.md). Fixed A:+C: boot and
the independent input rendezvous are recorded in [M4 runtime-media/input
evidence](../etc/evidence/m4-runtime-media-input.md); Windows Setup remains
the active acceptance gap.
The target architecture remains in [System Architecture](../design/ARCHITECTURE.md).
