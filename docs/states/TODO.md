# Long-Term Review Ledger

- [ ] TODO(Medium) Original keyboard table boundary: `KeyMsgToKeyCode()` uses
  `>` instead of `>= sizeof(table)`, so regular scan 133 or enhanced scan 84
  indexes one past its array. This predates T61 and is identical in OpenNT.
  Owner: product VM/Compat input boundary. Admit a bounded input-safety repair
  with invalid-scan tests before changing the original behavior; T61 S4 only
  removes duplicate source and preserves selected tokens. No current user
  failure is attributed to this finding.
