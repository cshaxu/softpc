# M9 Td S21: bounded frame ownership transfer

Owner approves a narrow prerequisite Td before resuming T96 S5: "批准".
Executor cfe2a06d updates the architecture rule, labels the pending migration
in design, and records the already admitted S5 brief. No runtime source,
guest media, configuration, executable or shared manifest changes.

## Bounded review and result

- Frame transport: dedicated complete immutable presentation storage may move
  across threads with explicit ownership transfer and bounded reuse.
- Lifetime: replacement, failure, retirement and shutdown return ownership
  exactly once. No reuse during consumption, render-dependent producer wait,
  or steady-state per-frame allocation/free is authorized.
- Isolation: copied commands/input, the sole executor, inward dependencies and
  prohibition on exposing live CPU/RAM/controller/private-renderer state remain.
- Implementation status: existing copied-frame descriptions are explicitly
  marked as current implementation; S5 owns their migration, not this Td.
- Gate: four lines allow a Governance packet to retain an explicitly open T;
  existing validation still rejects a wrong identifier or closed task. Eleven
  self-test lines cover those two rejections and the accepted interruption.

The coordinator reviewed the actual cfe2a06d diff against these five obligations.
Architecture rule +12/-3, net +9; architecture design note +13/-0; verifier
+15/-0, net +15 (including its self-tests). Remaining changes record S5
admission and the prerequisite. Production/test corpus delta is +0/-0.

Documentation gate, diff whitespace check and all governance self-tests pass.
Negative fixtures intentionally emit errors; they are expected rejections.
The first self-test invocation omitted SELF_TEST and was rejected without
running tests; the corrected invocation passed. No build/runtime test or
performance improvement is claimed for a policy change.

Td is complete and T96 S5 resumes under its existing approval. T96 stays open.
User checkpoint remains untracked and untouched. This record closes neither
the frame migration nor its required dual-width runtime qualification.
