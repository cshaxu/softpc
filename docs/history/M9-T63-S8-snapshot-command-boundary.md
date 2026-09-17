# M9 T63 S8: snapshot command boundary

## Scope and closure

S8 connected the already-approved opaque Common machine-state operations to
the product monitor.  It added `save <file>` for a running machine and
`load <file>` for the initial/stopped monitor state.  A successful operation
reaches ordinary paused state; App owns command wording, copied paths, prompt
flow and file ownership.

S8 did not change Common or Lib.  The VM remained the sole executor/state
owner and retained its safe capture/restore transaction.

## Result

P1--P7 were delivered through commit `46baf5c`.  P7 established a temporary
fixed-32-bit header for cross-width proof.  The owner then admitted S9 to
replace that temporary container with the canonical width-free streaming
format.  S8 is closed as the App command-boundary step; S9 owns only the
format and streaming refinement.

## Preserved boundaries

- No second executor or App-side machine state exists.
- Common remains limited to its two opaque machine-state operations.
- Direct/readonly media remain configured references.  Overlay-page payload
  remains separately planned and is not claimed by this record.
