# M9 T62 S8: Session owns its queue storage

Request and pre-audit: [proposal](M9-T62-common-lib-followup-simplification-proposal.md).
Baseline c435fe3; complete implementation 44e9d0f pushed.

Coordinator reviewed actual committed control.c/control.h/session.c and every
test caller. The private queue structure moved to its existing private header,
not into a public ABI. Session embeds it; initialize/dispose own mutex, event
and growable event storage. Successful creation no longer separately allocates
the queue container. Partial failure uses the same dispose as final cleanup.
FIFO growth, first-fault slots, source ledger and producer quiescence ordering
are unchanged. No queue create/destroy references remain across src/test.

Three production C/H paths +38/-50 = -12; six test C paths +91/-34 = +57,
git diff --numstat c435fe3 44e9d0f, excluding manifests/docs/artifacts.
The first build failed on a missed private test/unit caller and an address
conversion in the sync test; both corrected without expanding production scope.
Final builds and full suites: x64 101/101 (59.91s), x86 101/101 (60.17s).
Post-commit sync, queue-failure, source identity, completion/monitor and
manifest/corpus checks: 8/8 each. Resource counts reach zero on each injected
mutex/array/event failure and final disposal; reinitialization succeeds.

S8 closes. All four admitted follow-ups are complete; T62 remains open until
owner testing and further direction. No next task is admitted.
