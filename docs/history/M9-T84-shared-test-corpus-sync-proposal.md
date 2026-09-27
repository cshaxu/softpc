# T84 S10 — NXVM shared Lib/Common test-corpus sync

## Objective

Synchronize SoftPC's complete `test/lib` and `test/common` trees byte-for-byte
to NXVM commit `0c71110b0714fffee3ef8c40bb352ee3dd71ee40`, including deletions.
The reference is read-only provenance, never a build or runtime dependency.

## Audit ledger

Both source trees have the same shared production corpus; this task changes no
`src/` path, package configuration, media, snapshot, App, VM, Compat or Core
path.  The reference has 71 paths and SoftPC has 72.  The finite difference
ledger has eight paths:

1. Delete `test/common/window_input_reset_smoke.c` and its CMake registration.
   Its focused native-message coverage moves into the existing Lib admission
   smoke; retaining both would create a private-implementation include from a
   Common test and a second ownership boundary.
2. Add the source-identity assertion and both native focus-loss paths to
   `test/lib/kvm_input_admission_smoke.c`.
3. Add reset observation to `test/lib/kvm_window_retirement_smoke.c` for the
   existing native worker retirement scenario.
4. Add the pre-reset delivered-key assertion to Common's physical-identity
   smoke.
5. Add Common's negative check rejecting fixture imports of Lib implementation
   headers; Common tests may only consume Lib Types and root interfaces.
6. Import the two reference manifests, including their corpus revision and
   complete hash ledgers.

This preserves the S9 behavioral proofs while placing their host-message test
at the Lib owner boundary.  No production event route or public API changes.

## Verification and exit

Re-hash every path against the named NXVM commit after import; configure and
compile the affected x64/x86 tests; run their focused cases and serial
hidden-background x64/x86 CTest.  Run both test manifests, the Lib component
DAG, documentation governance and whitespace gates.  Record actual added,
removed and net test/build lines, push one complete P, then close S10.  A
reference mismatch outside these two test roots, a required production edit,
or any failed existing test stops the task for owner direction.

## P1 execution evidence

The import reaches exact content equality: reference paths `71`, SoftPC paths
`71`, mismatches `0`, using Git object hashes against
`0c71110b0714fffee3ef8c40bb352ee3dd71ee40`.  Production C/H is `+0/-0`.
Test C/H is `+62/-128` (net `-66`); including the two manifests and CMake
registration, test/build material is `+69/-137` (net `-68`).  The deletion of
the 127-line Common-private smoke is intentional and its message coverage is
retained at the Lib owner boundary.

Both x64 and x86 reconfigured and compiled the affected Common/Lib targets.
Their three focused cases passed per width.  Serial hidden-background CTest
passes x64 `121/121` in 127.11 s and x86 `121/121` in 123.92 s.  The full
background runs exclude desktop-labelled cases; the focused Window retirement
case was separately compiled and run once per width.  Both test manifests, the
Lib component DAG, documentation governance and whitespace checks pass.  No
package EXE is rebuilt because production inputs are unchanged.
