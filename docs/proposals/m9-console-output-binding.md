# Console output binding consolidation

## Original request and boundary

“把两个输出函数和 context 作为一个输出绑定，一次替换、一次清除。
保留正在执行的输出屏障、generation 和 broker 回滚，不去简化真正必要的
交接步骤。此项涉及 Console 的跨组件接口，需要同步更新测试。
准入下一个修复，完成后提交推送供我测试。”

T59 S9 starts from d35b941. Console owns one copied output binding containing
text/frame callbacks and their shared borrowed context. A null binding clears
both. Replacement waits for in-flight output before returning; callbacks must
not reenter replacement. Host retains generation-bound context ownership and
native transaction/rollback. No product, input, native mode or media changes.

## Finite convergence ledger

1. Console binding API/storage: one setter and one output gate; copied binding,
   shared context, null clear, missing callback returns NOT_CURRENT.
2. Host create/replace/rollback/destroy: one install/detach; remove impossible
   partial-setter failure branches only. Keep allocation/native failures,
   generation checks, rollback and old-context release after output barrier.
3. All source/test callers: migrate without aliases; deterministic barriers
   cover both output kinds and broker serialized reverse replacement.
4. Documentation/manifests/packages: update shared contract and tests, build
   both fixed EXEs, run focused and full dual-width suites and governance.

Completion requires every item proven, complete executor P pushed, independent
coordinator patch review pushed and clean worktree. T59 stays open for testing.
Stop if new state machines or native handoff changes are required. Search all
src/test for old setters; no parallel installation path may remain.

## Implementation and similar-issue sweep

The single Console setter copies text/frame/context under its existing output
mutex. Both writes use that mutex through callback return; the redundant short
state lock is unnecessary for these fields and removed from output operations.
Input event locking/generation are unchanged. Host still owns its allocated
generation-bound context: native validation needs that identity until old output
has quiesced. It is not another copy of the Console callback descriptor.

All create/replace/failed-activation/failed-restore/destroy paths now install or
clear once. Only impossible setter failures were removed. Replacement continues
to hold its transaction lock through old detach and context release, releasing
the backend output lock first so a blocked old writer can finish. Allocation,
reader retirement, activation, restoration and notification failure remain real
status paths. No platform, Common, App, VM, Compat or MVDM source changed.

Search of src/lib and test/lib found Host as the only production setter caller,
plus four test files; all migrated. No old setter or duplicate context fields
remain. The Console README also had obsolete claims that its private mutex
enter/leave and final release return status; corrected to the actual contract.

Focused tests pass at both widths (4/4): copied descriptors/shared context,
missing callbacks, replace/clear barriers against both text and frame writes,
generation and native rollback, reverse broker replacement serialization, KVM
retirement/activation. Barrier tests prove actual contention without Sleep.
Existing broker output assertions now check frame output alongside text.

Both full builds succeeded. One incremental x86 parallel make run returned
Error -1 at the gate-test link with no compiler diagnostic; the subsequent full
build with four jobs succeeded, and the rebuilt test passed. No source change
was made to mask that build-tool failure.

Accounting against d35b941 (tracked C/H only, excluding docs/manifests/packages):
three production files add 43/delete 81 lines, net -38; four test files add
91/delete 14 lines, net +77. One binding descriptor replaces two independently
installed pairs; no new lock, allocation, thread or state machine. Existing
build trees were reused and no new diagnostic artifacts require cleanup.

Fixed packages:

- softpc32.exe: 3,489,524 bytes; SHA256
  08803A0B89343C2A34D59CEB22C0A8CD64312250F84B8AD5CE7EE4A5E50A5008.
- softpc64.exe: 2,848,240 bytes; SHA256
  7F3A991862CF0521CEFD6E562F9D1F7AEE13FC7BCF48897E08145DAECB039860.

## P1 verification

Final full x64: 90/90 passed, 110.60 seconds. Final full x86: 90/90 passed,
96.11 seconds. Four corpus manifests, documentation governance and whitespace
checks pass. Executor reread the original request against all four ledger items;
each has source/caller/test proof above. The source ABI changes only for binding
adapters, with all in-tree callers migrated. S8's complete reviewed record was
moved intact to history without claiming owner manual acceptance. Ready for
executor push and coordinator review; T59 remains open for owner testing.

## P2 coordinator review

After bbc0607 was pushed, reviewed its actual source/API/test changes against
the original request and finite ledger. Console output fields are accessed only
under output_lock, held through callback return. Its input/generation path is
unchanged. Every Host detach still follows backend-output unlock; replacement
keeps transaction serialization until old context is detached/freed. Native
prepare/retirement/activation/restoration branches remain, including terminal
failure handling. Removed branches only tested setters that cannot fail for
the validated objects. No native implementation or product behavior changed.

The copied-binding test and five deterministic gate cases prove both outputs
switch together and both wait for old callbacks; broker tests retain reverse
replacement and generation/rollback checks with added frame assertions. Both
full suites and four corpus checks pass. HEAD matched origin/main and the tree
was clean before this review record. Delivery accepted for owner manual testing;
no claim that the owner has tested S9 or that T59 is closed.
