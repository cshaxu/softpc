# Long-Term Review Ledger

- TODO(Medium): Replace the host-time-dependent oracle in
  `test/app-softpc/unit/machine/machine_smoke.c:run_int1a_tick_boot_image`.
  T92 S2's first x64 full run failed `bda_tick_low != 0`; the next full run
  passed unchanged. The guest repeatedly calls INT 1Ah, whose selected
  non-NTVDM implementation refreshes the BDA from host time; a zero low byte
  is legal and does not prove that PIT delivery failed. Admission condition:
  bounded test-only time/IRQ contract audit with a deterministic progress
  oracle, not a timer production change or unconditional retry. The exact
  failing host timestamp was not captured; no runtime regression is inferred.

- TODO(High): Audit original DMA decrement-copy memory safety in
  `src/app-softpc/softpc.new/base/system/at_dma.c`. Source inspection during
  T92 S2 finds `bwd_dest_copy_from_M` advances guest source and decrements the
  supplied host destination; Compat GFI supplies an ordinary buffer start.
  This can write before that buffer for a guest-programmed decrement READ.
  No dynamic memory-safety reproduction is claimed. Admission condition:
  separate owner-approved DMA copy-semantics repair with guarded buffers,
  byte/word increment/decrement and wrap coverage. T92 S2 isolates address
  reload in decrement VERIFY mode rather than changing this separate helper.

- TODO(High): Investigate the intermittent x86 runtime-smoke null BOP dispatch
  through `BIOS[0x52]`. Both unchanged T83 baseline `9fbf7369` and T84 builds
  reproduce the same call to address zero; the guest trigger remains unknown.
  Owner explicitly defers repair on 2026-09-25. Admission condition: a separate
  owner-admitted CPU/BOP investigation, capturing guest instruction bytes,
  CS:EIP and reset context before choosing a repair; do not mask it with a
  no-op handler. Evidence: [T84 investigation](../history/M9-T84-S1-kvm-text-80x50.md).
