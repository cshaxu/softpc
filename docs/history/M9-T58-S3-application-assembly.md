# M9 T58 S3: Application Assembly Acceptance

Original request: “准入一个s任务进行搬迁。” The owner-approved design moves
entity assembly from main into composition, without new product behavior.
The complete ledger and verification are in the [proposal](../proposals/m9-common-test-sync.md).

Reviewed pushed executor commit 2c6a695 against 1cc141e: VM/options conversion,
machine/session/UI creation, both machine completion callbacks, sink binding,
run and cleanup all moved into composition. The moved assembly/cleanup body
matches the old body after config pointer syntax and status-return normalization.
Main performs existing argument/config validation and returns the same exit
code. Diagnostic trace reset now occurs after config validation; a rejected
startup preserves prior trace. No trace wrapper or second implementation.

Composition alone includes the VM public interface. Positive/negative boundary
fixtures enforce the relocated exception and reject main/header VM access.
No changed Lib/Common/VM/Compat/MVDM or shared test paths; no INI/media edits.
Existing provider test assertions remain unchanged; its link now includes the
Common session that the composition implementation creates and runs.

Both builds completed. Final full x64 85/85 (49.34 s), x86 85/85 (57.23 s).
Earlier package debugger-help stage-17 failure and diagnostics-free tool exit
remain recorded in proposal/TODO, not claimed repaired. Focused reruns passed,
and no source/test assertion changed to make the final suites pass.
Documentation and source-boundary gates pass; both fixed EXEs are committed.

Counted production src/app +118/-110, net +8; test C unchanged; product test
support +6/-4; CMake +1/-1. No added thread/state or residual assembly path.
All S3 ledger entries are complete. S3 closes; T58 remains open for owner tests
and direction, with no next subtask admitted.
