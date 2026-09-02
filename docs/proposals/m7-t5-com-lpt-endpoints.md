# M7 T4: Original Serial And Parallel Host Endpoints

## Goal

Make the original UART and printer controller paths useful through explicit,
standalone host endpoints rather than their current virtual sinks.

## Dependency

M7 T3 must close first.

## Scope

- Retain original `base/comms/com.c`, `printer.c`, `nt_com.c`, and `nt_lpt.c`
  state machines.
- Consider opt-in file, pipe or named-device backends with bounded I/O and
  deterministic shutdown.

## Exclusions

No NTVDM DOS-open tracking, VDD integration, hidden host threads that mutate
  controller state, or mandatory attachment to host hardware.

## Exit Evidence

Dual-width COM/LPT regressions prove guest-visible controller behavior and
the selected endpoint's lifecycle without changing the fixed VM profile.
