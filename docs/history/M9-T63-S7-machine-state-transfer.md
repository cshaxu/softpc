# M9 T63 S7: opaque machine-state transfer closure

## Scope

S7 delivered the two approved opaque Common machine-state operations and the
single VM-owned executor transaction behind them.  It did not add a product
command, a snapshot path, a Lib contract, or a second executor.

## Contract proven

- A running machine can read its state only after the VM reaches its own safe
  executor boundary; success is reported as the ordinary paused state.
- A stopped machine can write a validated state image and then becomes paused.
- The image is private, versioned and bounded.  It contains no host handles,
  native pointers, UI state or raw legacy structure dump.
- A fresh stopped process can restore the image, publish the restored paused
  frame, resume, and stop again.  Configuration/RAM mismatches are rejected
  before live restore begins.

## Evidence

P11 is commit `8647824` (`M9 T63 S7 P11: prove fresh-process snapshot
restore`).  It records the fresh-process staged-decode proof and retains the
S7 tests `softpc-snapshot-transaction-smoke` and
`softpc-snapshot-cross-process`.

The next step is S8: App-only monitor commands and bounded file I/O over the
already closed Common/VM contract.
