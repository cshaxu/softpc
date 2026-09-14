# M9 T58 S2: App Composition Acceptance

Owner request and finite ledger: [proposal](M9-T58-common-test-sync-proposal.md).
Executor delivery a472600 was committed and pushed before this actual-change
review. This S closes; T58 remains open awaiting owner testing.

Reviewed the old binding functions, state, all direct consumers, build targets
and test relocation against 00d4461. Command now owns the unchanged CLI/debug
policy and context; keyboard owns identifier interpretation and sequences;
composition installs the provider callbacks directly. Its sole adapter passes
the machine context to keyboard and reserves the existing command admission
boundary before returning a lifecycle request. Main retains entity assembly.
No new state, worker, duplicate dispatcher or compatibility forwarding path.
The old binding C/H are deleted and guarded against reintroduction.

The real provider test retains debugger/register/memory/execution assertions
and adds callback wiring, paused CAD/CAF, CAM and duplicate CAP admission
checks. Both full suites pass: x64 85/85 (53.31 s), x86 85/85 (61.74 s).
Both package builds succeed. Governance/self-test and source-boundary checks
also pass after evidence edits. No assertions were weakened. These checks
support unchanged behavior; owner interactive acceptance is still pending.

Actual commit accounting: src/app +335/-304, net +31, mostly direct callback
declarations; product test C +40/-24, net +16; CMake +8/-7; boundary gate +17/-0.
All src/lib, src/common, src/vm, src/compat, src/mvdm, test/lib and test/common
are unchanged. INI and guest media are untouched. Both fixed EXEs are committed;
their hashes and complete executor evidence are in the proposal.

All admitted S2 ledger entries are disposed; no S2 work is deferred. Existing
package timing TODOs remain distinct and are not claimed repaired by this
structural change. Queue and the open T remain available for owner direction.
