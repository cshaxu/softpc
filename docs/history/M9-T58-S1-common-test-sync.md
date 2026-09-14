# M9 T58 S1: Common Test Sync Acceptance

Original request and finite ledger: [proposal](../proposals/m9-common-test-sync.md).
Owner approved S1 closure but explicitly requires T58 to remain open, and
approved SoftPC as canonical after discovery of the upstream stale manifest.

Reviewed actual pushed executor commit ed03020: only the imported test's
monitor assertions and its corrected hash change the shared test corpus.
No src, ABI, configuration or media change. All 16 paths reviewed; only the
manifest now differs from NXVM, by owner decision. No test assertions weakened.
The tooling-only idle-wait support has positive and missing/wrong/closed-task
negative fixtures; it does not fabricate a continuation or close the T.

Both builds succeed; final x64 serial full test 85/85 (49.60 s), x86 85/85
(58.60 s). Initial x64 compact Console stage-5 timeout is recorded in TODO,
not claimed repaired; src and EXE hashes remain equal to the accepted T57
package. Common tests and manifest pass in both widths. The two package EXEs
remain available unchanged; no new runtime behavior is delivered by tests.

Test C accounting: +10/-1; manifest +1/-1. Product source: zero. Supporting
governance tool: +28/-2 including four fixtures. Every admitted S1 requirement
passes; no S1 implementation is deferred. S1 closes after this reviewed
commit/push, while T58 remains open awaiting owner feedback.
