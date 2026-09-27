# M9 T84 S9 — KVM input-state reset closure

The owner accepted S9 on 2026-09-27.  P1 `3186afcd` adds the one semantic
`INPUT_RESET` event for a KVM source that can no longer observe physical key
releases.  P2 `40da7d00` closes the two remaining test gaps without changing
production behavior or rebuilding packages.

Window emits reset after native focus/application loss and before frozen input
filtering begins; raw Console emits it on native focus loss and broker reader
handoff/destruction.  Each producer clears only its own normalizer, matcher and
mouse baseline.  Common Session remains the sole owner of delivered guest-key
state: reset and source retirement share its existing release routine, so only
keys actually delivered by that source receive synthetic breaks.  Reset remains
recoverable cleanup; retirement remains the final lifetime event.

Against the S8 baseline `b79769c1`, endpoint production C/H paths are
`+92/-11` (net `+81`) and endpoint test C/H paths are `+248/-22` (net `+226`).
The latter includes P2's direct Window-procedure-to-Session test and reset-sink
rejection test.  Manifests, registration and documentation are excluded from
those C/H counts.  No App, VM, Compat, Core mirror, product API, configuration,
media or snapshot change is retained.

P2 proves actual `WM_KILLFOCUS` and `WM_ACTIVATEAPP(FALSE)` delivery reaches
the Session ledger and produces one break per delivered make, with duplicate
native notifications idempotent.  It also proves rejected reset delivery enters
the existing terminal failure path, discards matching state, blocks later input
and does not turn the following retirement into a second failure.

Focused x64/x86 tests pass.  Serial hidden-background CTest passes 122/122 on
x64 (119.84 s) and x86 (119.24 s); desktop-labelled cases remain excluded.
Lib/Common manifests, the Lib component DAG, documentation governance and
whitespace checks pass.  The accepted package EXEs are P1's x86
`AE8D25BA183B73432887ED9D32C049E1567E7E357B065137543364A9B8B851AA` and x64
`03FB1E9E745A831DDA39417917E02487AF7239C0200E1DB80D4BDD59B7146C6A`.
