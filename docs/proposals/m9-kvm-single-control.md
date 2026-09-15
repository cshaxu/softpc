# KVM single-control admission

## Owner request and boundary

“freeze 只提交一条消息，Window worker 执行冻结时直接调用现有鼠标释放函数。
这样可以把 control 入队收窄为单条操作，删除批量机制；FIFO、容量错误、
STOP 和独立 frame mailbox 不变。” Owner admits a new S for this repair.

M9 T59 S8 baseline: 0b853eb. Window public API remains unchanged. The base
leaf-support enqueue API takes one copied control, without a count or legacy
alias. No changes to Common/App/VM/Compat/MVDM, configuration or media.

## Finite convergence ledger

1. Window freeze producer: one SET_WINDOW_FROZEN record, accepting the final
   free ordinary slot. Full FIFO rejection leaves existing records unchanged.
2. Window worker: set frozen, release through the existing mouse cleanup,
   retain blink/filter behavior. Unfreeze never captures. Failure uses the
   existing component fault path; no following controls run after failure.
3. Base enqueue and all callers: single-record validation/copy, unchanged
   reserved STOP slot/idempotence/terminal admission and FIFO order. Independent
   frame lock and latest-wins publication remain unchanged.
4. Shared tests and documentation: remove batch-specific expectations, add
   one-slot freeze and worker release proof, preserve control/frame/STOP tests.
   Search src/lib and test/lib for every old enqueue symbol; none may remain.

Implementation P includes all source/test/docs/manifests and both EXEs after
full x86/x64 tests. Coordinator independently reviews the pushed patch and
records scope/counts/evidence. No whole-library or whole-T closure is claimed.

## Implementation and similar-issue review

The production search found exactly two component enqueue callers: Window's
single-record API helper and common STOP. The component forwards to the mailbox's
single acceptance point and keeps its existing post-acceptance notification/fault
contract. All six shared test callers were migrated; no old plural enqueue symbol,
count parameter, compatibility alias or batch validation remains. The queue's
occupancy count is still necessary for the bounded FIFO; it is not batch state.

Window freeze now sets the frozen flag, invokes existing release cleanup and
checks the existing terminal input guard before timer/invalidation or later
controls. Already-frozen calls retain idempotence; unfreeze does not capture.
Linux Window remains its explicitly deferred unsupported implementation, not a
second freeze implementation. No public Window or Console signature changed.

Focused tests prove the final free slot accepts freeze, a full queue rejects
without overwriting, reserved STOP and repeat STOP remain valid, frame/control
locks are independent, native capture is released by consuming freeze, and a
release failure prevents later unfreeze. The new capture setup itself requests
focus; the first probe therefore needed to reset its focus counters after setup,
before checking freeze's absence of a focus request. Production needed no change
for that fixture correction. All six focused cases pass at both widths.

S7's reviewed record was moved intact into history to permit the next unused S8;
its outstanding owner-testing status was not turned into manual acceptance.

## P1 verified delivery

Both tests-x64/tests-x86 builds succeeded. Final full x64 tests passed 90/90
in 130.16 seconds; x86 passed 90/90 in 119.89 seconds. Four corpus manifests,
documentation governance and diff whitespace pass. Source search confirms no
enqueue_controls symbol remains; no Common/App/VM/Compat/MVDM source changed.

Accounting against 0b853eb, excluding docs/manifests/binaries: six production
C/H paths add 34/delete 49 lines (net -15); six test C paths add 35/delete 27
(net +8). No new state, allocation, thread or ownership path. Existing build
trees were reused; no new diagnostic logs or temporary trees need removal.

- softpc32.exe: 3,490,066 bytes; SHA256
  EE721C033BE9E16DC262809F7D8CF53983C15127AC0310BA408ABD18BE8D59B7.
- softpc64.exe: 2,848,781 bytes; SHA256
  DB01648DC0DEC32DFDEE7CF42C8D5260513D2E7C8A974877C55EBEBAE6B3AE8D.

Executor reread the original request and checked each ledger item against its
source/test evidence. Complete delivery is ready for push and coordinator review;
owner manual testing and T59 closure are not claimed.
