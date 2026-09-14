# M9 T59 S1 delivery review and continuation

S1 debt investigation and delivery evidence are retained in the
[task proposal](../proposals/m9-remaining-todo-investigation.md).
P1 2c05238 and P2 a7c2e86 are verified/pushed; no owner manual acceptance is inferred.

Owner request: “准入为当前T任务的多个s任务开始执行，记得先更新t任务的proposal”。
The owner admitted S2 build ownership, S3 original ABI declarations, S4 single-instance
admission and S5 architecture gates. CURRENT owns active status; the proposal owns
the finite scope/exit ledger. T59 stays open.

## S2 implementation record

Baseline a7c2e86. Preserve recovered machine OBJECT source order and all runtime
semantics. VM is an OBJECT target because original CCPU objects call VM debug
observers: placing both sides in separate ordinary archives would introduce an
archive-order dependency. Consumers link this one target, not another source list.
No additional runtime layer or executor is introduced.
