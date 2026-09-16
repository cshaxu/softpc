# Common / Lib bounded simplification

## Original owner request

“准入一个新的T任务，把上面这几条作为各自的S任务来处理。请你写好proposal，然后开始准入。每个S任务开始之前先告诉我你的方案、你的做法，大概会引起的 diff 的数量，然后开始执行。完成以后根据实际情况同样汇报给我，然后完成该 S 任务的收口、编译、测试、提交、推送，然后自动准入下一个 S 任务。所有 S 任务完成之后，等我验证，然后再收口这个 T 任务。”

## Objective and boundaries

Preserve public API and product behavior while reducing duplicate implementation
and allocation ownership. No App, VM, Compat or MVDM changes. No debugger rewrite,
generic queue framework, broker transaction change, input policy change or new
Linux functionality. T61 remains awaiting owner acceptance; this is separate work.

## Serial coverage ledger

The finite universe is these four audit findings, not every possible cleanup.
Each row requires implementation or an evidence-backed owner-approved disposition.

| Step | Owner / method | Estimated production diff | Required focused proof |
| --- | --- | --- | --- |
| S1 | Session: use two copied event fault slots and one latch/take path; preserve first failure per class, FIFO-before-fault and KVM-before-control priority | control.c, +25–45/-55–80 | Inject allocation failure; both slots, first failure preservation, order, draining, rearm |
| S2 | Machine: embed its private fixed input queue; replace allocation create/destroy with initialize/dispose | input_queue.c/h and machine.c, +15–30/-25–45 | Capacity 255, wrap, FIFO, clear/reuse, initialization and machine shutdown |
| S3 | Storage: root owns save-position/measure/restore; platform owns seek/tell and opening/locking | file.c/h and two platform file.c, +20–35/-30–50 | Both widths: nonzero-position length query restores position; empty and sparse files; error handling |
| S4 | KVM Console: move platform-independent event processing into component root; keep native worker and button translation in platform | console.c/h and win32/component.c, roughly 80–110 lines relocated, near-neutral net | Input reset before new records, activation wake/pending frame, key/text/mouse delivery and fault/retirement; Linux remains explicit placeholder |

Estimates exclude tests, manifests, documentation and EXEs. Before each step,
re-read the actual code and refine the estimate in its admission/brief. Relocation
is not deletion. Do not add abstraction merely to reach a negative line count.

## Verification and delivery

Each S: focused regression plus full x86/x64 configure/build/CTest using package
presets; refresh both fixed EXEs without editing package INI; update affected
four-directory manifests; pass corpus/DAG and documentation gates. Test additions
live in test/common or test/lib. Preserve external comparison trees read-only.
Record production/test/build numstat separately, using the S baseline commit;
record same-class search and retained necessary boundaries. Executor implementation
P is committed/pushed; coordinator reviews actual diff and focused post-commit
checks, then records S closure and next admission in a separate pushed P.

Build diagnostics use existing preset build directories; no new media or traces.
Remove owned disposable logs when no longer needed; retain reusable build trees.
After S4, leave T62 open with fixed EXE links for asynchronous owner testing.

## Stop conditions

Stop for public ABI/product behavior changes, weakened failure/ordering semantics,
new platform feature work, or complexity that exceeds the admitted direct cleanup.
Do not silently trade behavior for fewer lines or claim hand testing occurred.

## S1 executor delivery

Baseline 58d3d75. Production control.c +35/-69 (net -34); shared test source
+51/-0, test CMake +1/-0; governance verifier +9/-3. Two copied event slots
replace individual metadata fields; this intentionally uses two full event
records of fixed storage rather than a second fault schema. No failure-time
allocation is introduced. Fault class, first-wins and drain order are unchanged.
Allocation-failure test covers both slots, reverse arrival, repeated failure,
FIFO drain and reuse. Existing sync test covers concurrent producers/growth.
Search of Session latch/take and Machine/KVM queues found no same-contract
third latch; their bounded input/control ownership remains separate.

Both preset builds passed; full x64 99/99 (87.91s), x86 99/99 (73.45s), including
manifests, corpus/DAG and documentation self-tests. Both fixed EXEs refreshed;
INI/media unchanged. Documentation verifier now recognizes a prior open task
awaiting acceptance during New admission, with a positive regression fixture;
it retains rejection of wrong/closed waiting identifiers. T61 is not closed.

## S2 executor delivery

Baseline c7fafb8. Production three Machine paths +24/-37 = -13; test C/H
five paths +47/-13 = +34; test CMake +1/-0. Queue storage is embedded in Machine;
its lock still has one initialize/dispose owner. No allocation wrapper remains.
Partial Machine creation safely disposes a zero or failed queue. All source/test
create/destroy references were searched and converted, including the product
test-only compatibility header. Separate copied-frame allocations and Session
queue retain distinct ownership and are outside this bounded change.

Both preset builds passed and fixed EXEs refreshed. Full x64 100/100 (89.81s),
x86 100/100 (75.08s), including allocation-failure/600-cycle queue wrap test,
capacity/FIFO/clear and machine lifecycle tests, all manifests and gates.
INI, media, Lib, App, VM, Compat and MVDM remain unchanged.
