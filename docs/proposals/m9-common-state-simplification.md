# Common state ownership simplification

## Admitted request

T59 S11, baseline 04d551b. Owner admits three Common repairs, dual-width
build/test, complete commit/push and packages for manual testing:

1. Common Debug: actual command state becomes common_debug; remove the
   pointer-only allocation and open/close/submit/observe forwarding shell.
   Preserve public API, original command implementation, output and semantics.
2. Common Session: merge control_state and its embedded reconciler into one
   internal state implementation. Preserve pure presentation derivation,
   published-frame sequence gate and completion ordering.
3. Common UI: publish fixed graphical Console status on content-kind changes,
   not each Window frame. Text keeps sequence-driven publication. Recreated
   Console must receive its initial content; failed publication is not cached.

## Finite convergence ledger

Universe: these three production owners, all references to their removed
internal interfaces and their existing focused tests. No whole-corpus closure.
Each member must be implemented and proven; no silent deferral.

| Member | Owner / proof | Initial disposition |
| --- | --- | --- |
| Debug shell | common/debug; existing original CLI and machine tests, symbol scan | verified |
| Session shell | common/session; state/completion matrix and frame regression | verified |
| Fixed Console status | common/ui; counted fake publication, text/status/recreation/failure | verified |
| Direct references | build, shared tests and manifests; obsolete-symbol scan | verified |

No Lib, App, VM, Compat, MVDM or public ABI changes. No new state machine,
cache object or input path. Stop if preserving semantics requires those changes.
Existing Window and Console frame sequence ownership remains separate.

Use architecture/coding/execution/documentation governance. Executor implements
and independently checks all members, runs both full suites and documentation/
four-corpus checks, refreshes only fixed EXEs, commits/pushes P1. Coordinator
then reviews the actual committed changes and records/pushes P2. Report C/H
line accounting separately for production/tests. Preserve INI/media; reuse
existing build trees. T59 remains open, manual acceptance is not inferred.

## Implementation and focused proof

The existing command implementation now defines the public opaque common_debug
and its six operations directly. command.c changes only its include, state type
and operation names; its original parsing, commands, output and continuations
are unchanged. Removed debug.c and command.h, with no replacement forwarding API.

Session state owns the previous embedded fields directly. Existing reducer
bodies now update that state; removed reconciler.c/h and migrated every caller.
Monitor INIT versus presentation STOPPED and ERROR normalization remain distinct
facts, not silently merged. The existing monotonic sequence guard, close-request
lifetime, in-flight completion barrier and Console-before-Window order remain.
The pure presentation_plan implementation is untouched. Historical test names
remain, but all tests invoke the one state implementation, not a compatibility
alias. Build declarations refer to the retained implementation.

UI adds one control-thread-local content-kind boolean, reset on successful
Console creation and updated only after successful frame submission. Window
retains its independent frame sequence. The existing composition test counts
large frame clearing and deliveries: repeated graphics do neither for Console,
text advances, failures preserve prior markers, same-sequence kind changes are
submitted and a fresh Console receives the retained frame. Rebinding does not
pretend the existing KVM instance was recreated. Native handoff is unchanged.

Focused x64 and x86 each pass 8/8: reconciler, control_state_matrix,
control_reconciler_integration, session_frame, session_monitor, debug_output,
composition and common_machine. Full regression is recorded below once complete.

Similar-issue sweep used rg for common_debug_command, common_session_reconciler,
old C/H paths and nested state.presentation across Common, shared tests and root
CMake: no obsolete production or test entry remains. Public headers are unchanged.
Lib, test/lib, App, VM, Compat, MVDM and user INI/media are untouched. No additional
out-of-scope defect was identified in this bounded sweep.

Changed C/H accounting against 04d551b via git diff --numstat: production
src/common adds 151/removes 300 (net -149); test/common adds 109/removes 71
(net +38). Four internal C/H files are removed; no new code file/object/layer.
Root CMake adds 2/removes 2 lines; Common CMake files remove 2 source-list lines.
Both Common manifests use shared-t59-s11-p1; both unchanged Lib manifests pass.
No new build children, captures or guest media were created.

Fixed executable packages:

- softpc32.exe: 3,487,048 bytes; SHA256
  2A67998238122D589792E962A580B6E221BAD09228031F847F9E40D1FC0C0896.
- softpc64.exe: 2,846,218 bytes; SHA256
  592A1CD9DF9E190780FBDFD86E438927E5206FBA852A2A14021082D011B038AC.

## P1 executor delivery

Both package builds succeed. Full x64 passes 91/91 in 90.47 seconds; x86 passes
91/91 in 83.67 seconds. Focused suites are 8/8 each. Documentation governance,
all four manifests, Common DAG/negative checks and git diff --check pass.
Reread the original three requests: each ledger member is verified; no public
API or debugger semantics changed. Complete delivery is ready for push followed
by independent actual-patch review. Manual product acceptance remains the owner.

## P2 coordinator review

After ac56ae2 was pushed, switched roles and reviewed its committed production,
test/build and manifest changes against the original three requirements.
A literal comparison proves command.c differs only by the admitted include,
state type and operation renames. Public signatures and command bodies remain
unchanged; allocation/destruction now have one owner. Session retains original
completion order, close lifetime and frame gates in the single state object;
session.c only changes field access. The fixed-status flag has one writer,
is reset for new Console instances, and cannot acknowledge a failed submission.
Focused tests exercise the real Common owners with recording Lib boundaries.
Both full 91/91 suites, package tests and four-corpus checks passed. No new
failure or deferred item was found in the bounded actual-change review.
HEAD equaled origin/main and worktree was clean before recording this review.
S11 delivery is accepted for owner testing; T59 remains open.
