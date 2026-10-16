# Long-Term Review Ledger

- TODO(Medium): Diagnose native package-test child startup under sequential
  CTest execution. T93 S5 x64/x86 compact-console cases intermittently fail
  before start at stage=1/error=6. A scratch diagnostic reproduces child exit
  0xC0000142 (DLL initialization failed), AttachConsole error 5, then code-page
  query error 6. Direct native/package probes pass; this does not identify the
  responsible DLL or prove a CTest defect. Admission condition: test-owned
  child-startup/Console lifetime investigation with loader evidence, without
  retrying product startup or changing guest/device behavior to hide it.
  Evidence: [T93 S5 checkpoint](../proposals/m9-xp-sound-blaster-standalone.md#s5--original-dma-prerequisite-repair).

- TODO(Medium): Replace the host-time-dependent oracle in
  `test/app-softpc/unit/machine/machine_smoke.c:run_int1a_tick_boot_image`.
  T92 S2's first x64 full run failed `bda_tick_low != 0`; the next full run
  passed unchanged. The guest repeatedly calls INT 1Ah, whose selected
  non-NTVDM implementation refreshes the BDA from host time; a zero low byte
  is legal and does not prove that PIT delivery failed. Admission condition:
  bounded test-only time/IRQ contract audit with a deterministic progress
  oracle, not a timer production change or unconditional retry. The exact
  failing host timestamp was not captured; no runtime regression is inferred.

- TODO(High): Investigate the intermittent x86 runtime-smoke null BOP dispatch
  through `BIOS[0x52]`. Both unchanged T83 baseline `9fbf7369` and T84 builds
  reproduce the same call to address zero; the guest trigger remains unknown.
  Owner explicitly defers repair on 2026-09-25. Admission condition: a separate
  owner-admitted CPU/BOP investigation, capturing guest instruction bytes,
  CS:EIP and reset context before choosing a repair; do not mask it with a
  no-op handler. Evidence: [T84 investigation](../history/M9-T84-S1-kvm-text-80x50.md).
