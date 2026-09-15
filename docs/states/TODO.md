# Long-Term Review Ledger

The prior investigation is recorded in
[T59 S1](../history/M9-T59-product-boundary-cleanup-proposal.md).
Overlay optimization remains in [Queue](QUEUE.md).

- P2 — Native modal-test isolation. Owner: test/lib Window integration.
  T59 S14's first x86 full run observed WM_EXITSIZEMOVE before freeze/STOP
  in kvm_window_modal_smoke.c:75. The fixture assumes an OS modal loop remains
  active until its own request; the cause of that early exit is not established.
  Admission: reproduce/characterize native input/focus interference and improve
  fixture isolation without weakening the modal STOP assertion or changing
  product behavior. Retain S14 evidence and rerun results in its proposal.

- P2 — Common debugger inherited edge contracts. Owner: Common debug.
  XU narrows counts to 8 bits; XS retains real-address parsing in its linear
  path and scans candidate starts beyond the pattern's end; XM copies forward
  on overlap; XE/XF may partially write before rejecting an invalid tail;
  address arithmetic and permissive argument handling need explicit semantics.
  Admission: owner approves corrections beyond original-source restoration,
  with bounded range/invalid-input tests and no new parser architecture.
