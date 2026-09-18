# M9 T64 S1 — Lib simplification audit

Original request: “准入第一条T64，S1审计lib里类似可以简化的点”.
Baseline: `3f1c961`. Source-only audit; production/tests +0/-0, no new EXEs.

## Bounded review ledger

Review units are frame copying, worker ownership and component forwarding;
dispositions are implement, retain, or existing queued receiver. This is not
an exhaustive correctness claim about every Lib function.

| Unit | Source evidence | Disposition |
| --- | --- | --- |
| Frame mailbox | kvm-base/mailbox.c publish/capture whole-frame assignments | S2 shared narrowed copy; preserve prefix and active pixels. |
| Published frame | common/machine/machine.c copy under frame_lock | Same S2 helper, unchanged ownership/sequence. |
| Base thread | base/sync.c and platform sync.c | Retain cancellation plus infinite join. |
| Window thread | kvm-window/win32/component.c startup/join | Retain ready-or-exit wait and native message loop. |
| Console thread | kvm-console/win32/component.c startup/join | Retain STOP and callback detach; no new completion/cancel layer. |
| Console/broker gates | console/console.c; console-broker/console.c | Retain distinct callback/output barriers and generation-bound contexts. |
| Storage file wrappers | storage/file.c and platform file.c | Already embed stream ownership; platform opening/locking differs. |
| Overlay lookup | storage/medium.c | Existing queued overlay-index proposal, not T64. |
| Types wrappers | types headers and CMake INTERFACE target | Retain external vocabulary boundary. |

The historical 5000ms join was introduced in `6500f1d` (T55 S16); it is an
existing implementation contract, not an original owner product requirement.
Unification would need API/platform/caller changes for bounded join and Window
startup waiting. Owner accepted not doing it: “行，那就这样，下一个S呢？”

Owner then approved S2's narrowed alternative: prefix remains a full copied
value, only inactive graphics capacity is omitted. This avoids field-by-field
mode-dependent metadata rules. A text publish/capture pair can avoid copying
2 * 983040 pixel bytes; this is byte accounting, not a speed benchmark.

Audit was delivered in conversation before this durable record. Review confirms
no implementation was accidentally made during discussion. S1 is complete;
T64 remains open and S2 is separately approved. Documentation gate and
git diff --check are the applicable source-only verification.
