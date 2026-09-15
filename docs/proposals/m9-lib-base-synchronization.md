# Shared Base synchronization

## Admission

T59 S13, baseline 780c292, clean main. Owner: "批准这项实现 要求精简代码逻辑架构 完成后编译测试提交推送后等我审计 希望代码减少".
Approved design: extract generic sync/time from Host into Base; Host retains
native Console ownership. Types stays header-only. Reuse Base locks, remove
duplicate implementations and forwarding aliases; preserve product behavior.
Apply architecture/coding/execution/documentation governance. S12 is delivered,
not claimed manually accepted. T59 stays open.

## Finite convergence ledger

| Member | Implementation boundary | Required evidence |
| --- | --- | --- |
| Base | Move Host sync/clock and platform implementations; rename public symbols/callers | No old sync/clock declarations or forwarding layer; existing ownership/wait tests |
| Console | Use Base mutex for event/output gates; delete private mutex implementation | Existing callback/output contention barriers and create-failure cleanup |
| KVM frame | Base blocking mutex; independent short control lock unchanged | Deterministic frame contention, ordinary control progress, STOP closure, create failure |
| Dependency graph | Base depends only Types; Console/KVM-base depend Base; Host retains Console | Source/build DAG positive and negative gates; Common linkage updated |
| Delivery | Documentation, four corpus manifests, fixed x86/x64 packages | Full dual-width suite, line accounting, executor and coordinator commits pushed |

No MVDM, configuration, guest media, input, renderer, lifecycle or focus changes.
No new synchronization algorithm, thread, recovery loop or compatibility shell.
Host's native takeover/output locks remain backend-owned: they protect native
state and are not another allocated generic mutex implementation. KVM wake is
auto-reset and fallible, unlike the existing manual-reset Base event API; leave
that distinct contract intact. Console metadata and ordinary control locks stay
short spinlocks. Terminal admission still takes frame then control.

Move files with git mv. Adapt direct consumers, build edges and tests in the
same delivery. All existing Linux sync/time implementations move with Windows;
no new Linux presenter requirement. Stop if preserving behavior needs a new
public synchronization semantic or product policy. Record bounded similar-issue
sweep, actual production/test line deltas and test limitations before completion.
Executor P1 then separate actual-patch coordinator review P2; await owner audit.

## Executor evidence

All five ledger members are implemented and verified. Ten Base C/H files compare
exactly with 780c292 Host files after only path/prefix substitution. The five
Common/Compat production consumers similarly compare as mechanical substitutions;
no application, VM, MVDM, INI or media diff exists. Host Console implementation,
KVM native workers and Types declarations are unchanged.

Console uses the existing Base opaque mutex directly; its three private mutex
files are deleted, including both platform implementations. Its two gates retain
the same ownership, acquisition order, callback barrier and failure cleanup.
KVM creates one frame mutex, returns its creation failure and destroys it after
quiescence; no new thread or state machine. Publish/capture/ack use that mutex.
Terminal closure still locks frame then control, ordinary FIFO only control.
Frame/latest-wins/dirty and input/STOP behavior are unchanged.

The new kvm_frame_lock test hooks the real Win32 mutex boundary. Native
TryEnter failure proves actual contention before asserting the publisher,
reader or STOP thread remains blocked; control enqueue/take succeeds while the
frame lock is held. Unlock completes the contender. No Sleep or performance
threshold. Existing input_admission retains concurrent publish/STOP proof.
mailbox_selection now rejects mutex creation and disposes the failed mailbox;
console_blocking_gate rejects either gate creation and retains output/event
detach barriers. Both widths pass the eight focused cases, including Base task
ownership, Common synchronization and Linux wait fakes.

Source/build dependency self-test exhausts all 64 edges among eight components;
Base may only use Types, Console/KVM-base may use Base. Negative probes reject
retired sync/clock aliases. Common machine/session now link Base instead of Host;
Common UI retains Host. Host receives Base transitively through Console, without
inventing an unused direct edge. Storage has no new dependency. Threads linkage
belongs to Base and the distinct Linux KVM wake implementation.

Similar-issue sweep: rg over src/test/tools for old host sync/clock and Console
mutex names finds only deliberate rejection fixtures/gates, not production
aliases. The mutex/critical-section/pthread scan identifies Base implementation,
Console gates, KVM frame, Host native transaction/output and Linux wake condition.
The last two retain their distinct native state/condition responsibility as
admitted above; no duplicate allocated generic mutex remains. Types is still
header-only. Corrected the library README's stale multi-record admission text
while updating the mailbox contract; no code changes to control semantics.

Both existing build presets pass with LIBRARY_STRICT_WARNINGS=ON
(-Wall -Wextra -Wpedantic -Werror on Lib). Full x64: 92/92, 79.99 s;
x86: 92/92, 64.78 s. Four manifests, dependency/layout gates, documentation
governance and diff whitespace checks pass. Linux proof is existing fake wait
coverage and mechanical-source comparison, not a claimed native Linux run.
Existing build trees are reused for audit; no new diagnostic/media directory.

Against 780c292, git diff --numstat filtered to tracked src C/H reports
+531/-592, net -61; test C/H including the new 70-line contention test reports
+188/-89, net +99. Excludes docs, manifests, CMake and EXEs. The reduction deletes
one implementation, not compact formatting; tests intentionally grow. All four
shared manifests use shared-t59-s13-p1.

Fixed artifacts:

- softpc32.exe: 3,487,113 bytes, SHA256
  81B55040BDA76C9867DF5984E2D7FA6A509583BB4D18242FA35C5C85466421BD.
- softpc64.exe: 2,845,739 bytes, SHA256
  DD76638CF3442D151CA128178D1B09FC9652DA89801CFA9FC3C2A7F3859F07E8.

Ready for executor P1 commit/push, then independent actual-patch review P2.
Await owner testing/audit; neither S13 manual acceptance nor T59 closure claimed.
