# M9 T48 S7 — Presenter completion hardening

## Outcome

Control now admits `SOURCE_RETIRED` regardless of an obsolete run generation.
Source identity is globally monotonic, so this event can safely clear only its
own pressed-key ledger.  It never crosses the separate VM-input boundary when
the machine is paused or stopped.

Ordinary UX records and component/broker facts retain their current-run gate;
the control reducer remains the only product path for component/broker
completion and Window-close facts.

## Proof

- Focused control integration proves stale ordinary input is rejected while
  stale retirement performs only no-guest-input ledger cleanup.
- Fresh x64/x86 full CTest each passed 33/33.

## Closure

S7 closes with `52b8bdc`. S8 owns deterministic whole-control matrix proof.
