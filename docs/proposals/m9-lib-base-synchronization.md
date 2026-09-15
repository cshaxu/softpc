# Base Task allocation flattening

## S16 admission

Original owner request: 批准，准入S任务实现；编译测试提交推送

M9 T59 S16 continues from ea1b0c7, clean main. S15 delivery is retained in
[history](../history/M9-T59-S15-base-event-reuse.md); owner testing is not
inferred from admission. T59 stays open.

Approved scope: embed common task fields as the first member of the selected
platform task. Allocate one task object, retain one cancellation Event, and
call the user entry directly from the native trampoline. Keep the public API,
cooperative cancellation and infinite join semantics unchanged. No KVM migration,
optional cancellation, timed join, readiness framework or product changes.

The public create validates arguments and creates the cancellation Event, then
passes it to platform creation. Failure destroys that Event; success transfers
it to the task. All fields exist before thread creation. Destroy cancels, joins,
releases the Event and disposes the single task allocation. Win32/Linux have
the same private signatures; platform types stay in their platform files.

## Finite convergence ledger

| Member | Required result | Proof |
| --- | --- | --- |
| Allocation | One task allocation plus existing cancellation Event; no outer task pointer or startup forwarding | Both platform source scan; allocation-count tests |
| Startup/failure | Copied entry/context and task identity correct before execution; all create failures leave null output and no leak | Allocation/Event/thread failure injection and entry assertions |
| Shutdown | Existing cancellation/wait-any and join-before-free preserved | Native Win32 task test, controlled Linux task test and full consumer regression |
| Scope | Public API and KVM/Common/Compat/MVDM unchanged; no new state or wrapper | Baseline diff and old-symbol scan |
| Delivery | Strict dual-width build/test, manifests/docs, both fixed EXEs and clean pushed P1/P2 | Recorded results and actual-commit coordinator review |

Similar-issue sweep is bounded to Base task allocation, trampolines and disposal
in both platform files plus all task API consumers. Existing native KVM and
Host workers retain their distinct lifecycle; no claim of whole-library thread
unification. Existing TODOs remain separate.

Use existing build trees; no scratch media, INI changes or raw capture.
Production/test line counts use git diff ea1b0c7 --numstat on tracked C/H.
Complete executor P1 before coordinator review and P2. Wait for owner testing.

## Implementation and bounded sweep

Both platform structs embed base_sync_task as their first member. The shared
root no longer allocates an outer task or forwards its startup callback. Event
creation precedes platform creation, so every worker sees complete cancellation,
entry and context fields. A failed platform allocation/thread creation releases
its own allocation and the root releases the Event. Normal destroy joins before
releasing either resource. No public API, cancellation mode, joined flag or
consumer behavior changed; KVM and Host native workers are outside this task.

The ownership test now instruments both root and platform source, rather than
only counting platform allocations. It proves exactly two allocations including
the existing Event, task/context identity, cancellation and immediate-return
entries, both allocation failures, native Event/thread failures and one join on
destroy. Identity is copied to an integer before disposal, avoiding comparisons
through dangling test pointers. Linux controlled fakes run the actual platform
entry on join, verifying the same identity, cancellation, two allocations,
thread-create failure cleanup and retained joined behavior. No Sleep added.

Sweep: rg over Base for base_sync_platform_task_entry, the former platform task
struct, base_sync_task_main and task->platform returns no hits. Both remaining
platform creation/join/disposal functions have the same private signature.
Task production consumers are Common machine and Compat audio; baseline diff
confirms both unchanged, as are all KVM/public sync interfaces and MVDM.

Count: git diff ea1b0c7 --numstat on tracked C/H. Production four files:
+57/-67, net -10. Tests three files: +88/-6, net +82. Documentation,
manifests and binaries excluded. No new implementation file, state, Event,
framework or compatibility alias. Only src/lib and test/lib manifests change;
Common corpora are byte-identical. S15 history retains the previous proposal.

Both strict GNU package builds passed. A direct x86 target rebuild without its
preset environment failed before compiler diagnostics; rerunning the admitted
x86 preset supplied its toolchain PATH and succeeded. No source workaround or
toolchain change was made. x86 full regression passed 92/92 (81.18 s); the final
test-only identity refinement then passed the three focused tests (0.23 s).
Native Linux execution is not claimed; Linux uses controlled boundary fakes.
No new scratch tree/media or configuration changes; existing build trees retained.

## Executor verification

All five ledger members passed. Final x64 full suite: 92/92 (81.13 s).
x86 full suite: 92/92 (81.18 s), followed by final focused 3/3 as recorded above.
Both Base builds use -O3 -DNDEBUG -std=c17 -Wall -Wextra -Wpedantic -Werror.
Four shared manifests, documentation governance and diff whitespace pass.
No test failure or product regression observed; no manual UI acceptance claimed.

Fixed packages: softpc32.exe 3,484,995 bytes, SHA256
FE75CF47EAE7330A88DDD04921A217E4C20BF98C0EA7D50CA7B3C7C2C8AA4083;
softpc64.exe 2,844,615 bytes, SHA256
9CB56D4812BC476C94D771CCEF1601EB6E7A0D4CAD2D6DC541C4AD5746EB3552.
Executor P1 includes the entire delivery; coordinator actual-commit review follows.

## Coordinator review

P1 927bac3 is committed and pushed. Switched to coordinator and inspected the
actual commit, original request and all five ledger rows. Both platform task
objects begin with the common fields; their native entry invokes the public
callback directly. Cancellation exists before thread start, task creation
failure cannot publish an object, and the successful join precedes Event and
task disposal. No extra allocation, callback, lifetime state or API was added.
The allocation test instruments the formerly hidden root allocation too; the
Linux fake preserves the platform joined guard. Existing ignored native join
results are unchanged, not claimed repaired by this structural task.

Baseline comparison confirms no public/KVM/Common/Compat/App/VM/MVDM changes.
The archived S15 proposal has the identical Git blob as its original baseline.
Reviewed two updated manifests, strict builds, both full suites and final x86
focused rerun. Production -10/test +82 counts are verified against P1. No new
in-scope finding; existing TODOs are neither changed nor claimed resolved.
P2 records acceptance of this bounded implementation and leaves S16 delivered
for owner testing. T59 remains open; no next S is admitted.
