# Long-Term Review Ledger

- TODO(Medium): Investigate intermittent captured mouse escape under RDP.
  Owner reports escape shortly after capture or after variable movement;
  re-entry sometimes controls the guest without clicking and sometimes does not.
  Local-desktop behavior is untested and reproduction is currently unstable.
  Suspected owner: Lib kvm-window native capture/clip/raw-input boundary;
  RDP pointer synchronization remains an alternative, not a confirmed cause.
  Admission condition: owner-admitted diagnostic task recording release reason,
  focus/capture owner and actual versus expected clip bounds on transitions.
  Distinguish deliberate release from clipping loss with capture still active;
  do not add recapture polling or guest-specific workarounds without evidence.

- TODO(High): Investigate the intermittent x86 runtime-smoke null BOP dispatch
  through `BIOS[0x52]`. Both unchanged T83 baseline `9fbf7369` and T84 builds
  reproduce the same call to address zero; the guest trigger remains unknown.
  Owner explicitly defers repair on 2026-09-25. Admission condition: a separate
  owner-admitted CPU/BOP investigation, capturing guest instruction bytes,
  CS:EIP and reset context before choosing a repair; do not mask it with a
  no-op handler. Evidence: [T84 investigation](../history/M9-T84-S1-kvm-text-80x50.md).
