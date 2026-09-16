# Common / Lib bounded simplification

## Original owner request

“准入一个新的T任务，把上面这几条作为各自的S任务来处理。请你写好proposal，然后开始准入。每个S任务开始之前先告诉我你的方案、你的做法，大概会引起的 diff 的数量，然后开始执行。完成以后根据实际情况同样汇报给我，然后完成该 S 任务的收口、编译、测试、提交、推送，然后自动准入下一个 S 任务。所有 S 任务完成之后，等我验证，然后再收口这个 T 任务。”

## Objective and boundaries

Preserve public API and product behavior while reducing duplicate implementation
and allocation ownership. No App, VM, Compat or MVDM changes. No debugger rewrite,
generic queue framework, broker transaction change, input policy change or new
Linux functionality. T61 is closed by owner direction before further T62 work.

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

## Owner ordering correction

“T61要收口关闭才能开始T62 你先关闭T61”。T62 S1 and S2 P1 had already
been pushed when this direction arrived. They remain truthful immutable history;
no S3 implementation began. T61's whole-task audit/closure is committed before
resuming T62. The S1 verifier relaxation for simultaneous waiting/admission is
withdrawn, not retained as a new governance policy. S2 actual-commit checks
passed 6/6 per width; its closure follows only after T61 closure.

## S3 executor delivery

Baseline f4b090b. Production four Storage paths +26/-37 = -11; test C source
three paths +59/-0; test CMake +1/-0. The root owns the unchanged sequential
save-position/end/tell/restore algorithm; selected platform files own only
seek/tell primitives alongside existing open/lock differences. All old private
position/length names and consumers were searched; no duplicate measurement
algorithm remains. Public API, short-circuit failures and output-on-success
semantics are unchanged; no retry or compensation path was introduced.

Both preset builds and fixed EXEs refreshed. Full x64 101/101 (84.96s), x86
101/101 (72.09s); strict standalone Lib build passed. Focused proof covers
four injected failures, >4 GiB counts on both widths, empty/hole files and
nonzero position restoration, including controlled Linux platform calls.
Manifests, corpus/DAG and documentation gates passed. INI/media unchanged.

## S4 executor delivery

Baseline 6a60759. Four production C/H paths +85/-71 = +14, primarily moving
66 lines of event processing to the component root, not deleting behavior.
Two test C paths +10/-1 = +9; no build graph changes. Root now owns the same
keyboard normalizer and mouse baseline; native worker creation/join and button
mask translation stay in platform. Private declarations connect these owners;
no new public API, queue, callback path or state machine. Existing worker
storage remains platform-owned to preserve startup/join failure handling.
Mouse positions use existing lib_i32 copied fields, preserving native COORD
values and X8/Y16 scaling. The first compile caught a nonexistent lib_i16 name;
it was replaced with the existing copied type before final verification.

Both builds and fixed EXEs refreshed. Full x64 101/101 (61.73s), x86 101/101
(63.32s); strict standalone Lib build and strict Linux placeholder object
compilation passed. No Linux runtime support is claimed. Existing tests prove
keyboard/text equivalence, reset before input, activation/pending frame and
callback retirement barriers; added checks prove button translation/source
identity and unchanged mouse baseline/scale. Search covers every production
event receiver and button decoder: one root receiver and one selected platform
decoder, with no platform event-dispatch duplicate. Public interface hashes,
Common, App, VM, Compat, MVDM, INI and media are unchanged in S4.
