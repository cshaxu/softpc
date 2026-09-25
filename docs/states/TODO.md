# Long-Term Review Ledger

- TODO(High): Investigate the intermittent x86 runtime-smoke null BOP dispatch
  through `BIOS[0x52]`. Both unchanged T83 baseline `9fbf7369` and T84 builds
  reproduce the same call to address zero; the guest trigger remains unknown.
  Owner explicitly defers repair on 2026-09-25. Admission condition: a separate
  owner-admitted CPU/BOP investigation, capturing guest instruction bytes,
  CS:EIP and reset context before choosing a repair; do not mask it with a
  no-op handler. Evidence: [T84 investigation](../proposals/m9-kvm-text-80x50.md#verification).
