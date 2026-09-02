# M7 T4 S1: Original Serial And Parallel Host Endpoint Closure

## Result

The fixed standalone VM now offers opt-in file output endpoints for its
original COM1 and LPT1 host contracts. Their absence leaves the historical
bounded virtual sinks intact.

## Evidence

- The [endpoint proof](../etc/evidence/m7-t4-com-lpt-endpoints.md) documents
  the original controller ownership, synchronous lifecycle, repeated reset,
  and dual-width regressions.
- Only INI parsing, machine option ownership, host carrier output exits, and
  frontend/test behavior changed. No BIOS, BOP, controller protocol, guest
  state, or external device attachment changed.

## Closure

M7 T4 is closed with clean x64/x86 serial CTest at 19/19. M7 T5 may now audit
and complete the original speaker host endpoint.
