# Long-Term Review Ledger

- [ ] TODO(Medium) Intermittent x86 BIOS tick smoke: delivery recheck after
  T62 S8 failed once at test/unit/machine_smoke.c:1048 (bda_tick_low != 0),
  then passed five focused repetitions and full 101/101 without changes.
  Owner: product machine test/VM timing boundary. Admit a bounded investigation
  of the fixed run-budget assertion versus timer delivery before changing
  machine semantics; root cause is not established. See the
  [S8 recheck](../history/M9-T62-S8-session-queue-ownership.md#delivery-recheck).

- [ ] TODO(Medium) Original keyboard table boundary: `KeyMsgToKeyCode()` uses
  `>` instead of `>= sizeof(table)`, so regular scan 133 or enhanced scan 84
  indexes one past its array. This predates T61 and is identical in OpenNT.
  Owner: product VM/Compat input boundary. Admit a bounded input-safety repair
  with invalid-scan tests before changing the original behavior; T61 S4 only
  removes duplicate source and preserves selected tokens. No current user
  failure is attributed to this finding.
