# T100 S4 fresh ordered mirror audit

## Frozen universe and method

SoftPC baseline: `6ff24aee`; source delivery: `cda2d427`. Original: XP SP1
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`, counterpart prefix
`Source/XPSP1/NT/base/mvdm/softpc.new/`. No OpenNT comparison is used.

The [fresh file ledger](m9-t100-s4-files.tsv) captures raw original/local SHA-256
and literal `cmd /d /c "dir /a /b /s"` file order, without an ordering option.
There are 472 files and 15 directories; the file subsequence matches S1 exactly
and equals the tracked mirror set. Directories are structure, not audit points.
254 files are identical after CRLF/LF normalization (binary bytes remain exact).
218 differ before excluding deliberately removed bodies and line controls.
These identity results are not 218 qualified retention decisions or completed
semantic reviews. The hashes/order are frozen; the comparison column records
the completed screening disposition, separately from semantic review below.

Owner then directs excluding the 254 identical files and all remaining files
whose only changes are excluded bodies and their line-control scaffolding.
Screening excludes another 97 files and yields the ordered
[121-file real-diff list](m9-t100-s4-real-diff-files.md). That list is the remaining
review workload; the 351 excluded files need no repeated semantic audit.

Existing excluded branch bodies are matched to their original conditional
parent/arm and omitted without recording their contents or counting them.
Other differences are reviewed in ascending original/local source order.
Line controls are diagnostic scaffolding and are not hidden as semantic fixes.
Counters distinguish selected text hunks, semantic points, added/deleted lines,
evidence-qualified pending decisions and approval. All previous retention
approvals and automatic small-diff delegation remain revoked.

## P1 source review, positions 1-9

| Order | File under base/bios | Selected-body result | Review disposition |
| ---: | --- | --- | --- |
| 1 | bootstra.c | No residual selected-body difference | No retention decision in this body. |
| 2 | build_id.c | Identical after newline normalization | No retention decision. |
| 3 | cmos_bis.c | No residual selected-body difference | No retention decision in this body. |
| 4 | equipmnt.c | No residual selected-body difference | No retention decision in this body. |
| 5 | mem_size.c | No residual selected-body difference | No retention decision in this body. |
| 6 | reset.c | One deletion, +0/-2 | S4-001: product selection, pending decision. |
| 7 | rom_basc.c | No residual selected-body difference | No retention decision in this body. |
| 8 | slave_bs.c | One replacement, +1/-1 | S4-002: original device defect correction, pending decision. |
| 9 | tape_io.c | One replacement, +1/-1 | S4-003: selected CPU40 declaration adaptation, pending decision. |

This first selected-body pass covers nine files and finds three semantic points,
three text hunks and **+2/-4 (six added+deleted lines)**. After owner-directed
screening, 118 of the 121 real-difference files have not received this sequential
semantic review. Diagnostic-anchor necessity
still requires the separate all-file reconciliation; this table does not claim
completion of every source-location difference. No production change or fresh
dynamic test is made in P1.

### S4-001: reset.c, retired software-driver lifecycle

XP lines 910-911 call mouse_driver_termination and mouse_driver_initialisation
on warm reset; current reset.c omits them before its line-912 anchor (local 629).
Category: product/subsystem selection, not an original mouse hardware bug.
Historical correspondence R0006 is context, not approval.

Fresh source inspection finds reset.c in the production device list and its
per-file Compat BIOS include boundary in CMakeLists.txt. The complete current
src/test search finds neither software-driver lifecycle symbol. The actual
hardware mouse initialization remains mouse_init at reset.c:926; the keyboard
controller and host input path are not removed by this omission. Restoring just
the calls is not a coherent subsystem restoration: the old optional software
driver implementation has been retired.

Counter-review: absence of the symbols does not prove an XP defect or that a
coherent reintroduction could never work. This is an explicit current product
scope decision, distinct from make/break input or PS/2 controller correctness.
No original-restoration build or guest reproduction is claimed in this P.
Recommendation: present the two-line product-selection difference for owner
disposition; do not label it a hardware fix or manufacture stub replacements.

### S4-002: slave_bs.c, slave PIC EOI port

XP line 110 sends 0x20 to ICA1_PORT_1; current line 107 sends it to ICA1_PORT_0.
Category: original device logic correction. Historical correspondence: R0008.
Production BOP registrations in Compat device_bop.c bind D11_int to unused
slave vectors 72h/73h/74h/76h/77h. ios.h defines the command port as A0h and
the data/mask port as A1h. In SWPIC_outb, the data-port write sets ica_imr,
whereas the command-port OCW2 non-specific EOI clears an in-service request.

The current D11 path first masks the serviced slave line, sends slave EOI,
then master EOI. The original port would overwrite the slave mask with 20h
instead of acknowledging the slave ISR. This follows the current actual port
implementation, not merely an analogy to another emulator.

Counter-review: this affects D11's serviced-slave branch, not every interrupt
or a claim that normal DOS always exhibits a fault. The existing
device_contract_smoke fixture checks slave service, master service, spurious
cascade and no-service cases, including ISR and both masks. Its code was read
fresh; no new execution or pre-fix failure is claimed. Recommend retaining the
one-token correction, subject to fresh owner approval.

### S4-003: tape_io.c, descriptor limit output type

XP line 96 declares the limit output as word*; current line 83 uses sys_addr*.
Category: selected CPU implementation/ABI declaration adaptation, not an
unqualified claim that the original pre-CPU40 product was broken. Historical
correspondence: R0010.

Production tape_io.c supplies sys_addr source_limit/target_limit to its two
retrieve_descr_fields calls. The built Compat ccpu/facade.c definition takes
sys_addr* and delegates descriptor decoding to read_descriptor_linear, then
copies descriptor.limit. It does not introduce another descriptor decoder.
The original declaration's narrower pointer type disagrees with this real
definition and its callers. The fixture contains 64-KiB, 0x12345 and 0xffffffff
limits with neighboring canaries and exercises the BIOS block-copy caller.

Counter-review: a mismatched pointer declaration is not itself proof that the
machine-code call immediately truncates its result; the current function writes
through its own sys_addr* declaration. The proven issue is the incompatible C
interface for the selected CPU path. Do not misreport this as a demonstrated
runtime truncation. Recommend the one-line declaration correction pending owner
approval; no new dynamic control or build is claimed here.

## Progress and verification boundary

All 472 identities and order are frozen; first selected-body frontier is 9/472.
Category subtotal for the first three points: original defect +1/-1, selected
CPU ABI +1/-1, product selection +0/-2. All three are unapproved. There is no
restored source, new helper, second execution path or changed artifact in P1.
The historical 381-ID count is not asserted as the new audit's final count.
Documentation/manifest checks qualify this read-only delivery, not runtime
behavior or all-file audit completion. After screening, the next remaining file
is position 41, base/ccpu386/c-files; dependency reads do not advance that frontier.

## P2 per-file text counts

Owner requests counts for every file in the remaining list. The
[121-file table](m9-t100-s4-real-diff-files.md#difference-counting-convention)
now includes zero-context Git Myers hunk counts and XP-relative added/deleted
lines: 15,778 hunks, +24,764/-24,069, 48,833 added+deleted lines. All frozen raw
hashes match. Excluded bodies are omitted without their own entries; line
controls remain separately handled. New/changed conditional framing is retained
in the counts even when its excluded body is omitted. No unknown correspondence
is accepted as an excluded original body.

These are text blocks, not a claim of 15,778 semantic decisions. Repeated generated
GDP access changes can create many blocks for one kind of ABI adaptation, while
one large block can contain several purposes. Manual review must determine that
ownership rather than infer it from this arithmetic. P2 advances statistical
coverage to 121/121, not semantic review or owner approval. No source, test or
artifact changes; documentation, manifest and table reconciliation checks apply.

## P3 manual review classification update

The first ten remaining files have been presented in order and their points
classified in the [current decision ledger](m9-t100-owner-decisions.md#s4-current-file-by-file-classifications).
This supersedes P1's pending/reset and sequential-progress counts above, not
its evidence limits. Owner explicitly approves reset.c; subsequent next-file
requests do not constitute retention approval. Purpose categories distinguish
product selection, declarations, native pointer ABI, snapshots and behavioral
corrections. Future file reviews update the same ledger immediately.
No production edits, builds or fresh runtime tests are part of this update.

## P6 instruction-rate limiter removal

Owner approves removing the limiter only; implementation and purpose accounting
are in the [decision ledger](m9-t100-owner-decisions.md#s4-p6-owner-approved-instruction-rate-limiter-removal).
The frozen file/count lists remain the original audit baseline; this delivery
records a subsequent three-line mirror-diff reduction rather than silently
changing the frozen identities. Final qualification is recorded below.

P6 artifact identity (fresh compiler literals verified, not file timestamps):

| Artifact | Bytes | Compiler date/time | SHA-256 |
| --- | ---: | --- | --- |
| softpc32.exe | 3477770 | Nov 14 2026 16:00:10 | `b875d9b071440b717037b28a71004bb51620e0087f4132510aae89f9f3aba180` |
| softpc64.exe | 3095223 | Nov 14 2026 16:04:06 | `99043f9f3e9d44dfb27d6aa10db4ca6b7855292fcdddced8ebffab8fbf8c78fd` |

Actual-diff self-review: complete removal of the single limiter owner and its
sole mirror consumer, without alternate governor, runtime switch or new helper.
Test wake/count assertions remain; only obsolete limiter-origin state is gone.
Production +0/-85; tests +2/-6. Mirror remains CRLF, Compat/test/docs remain LF.
Build diagnostics outside the preserved mirror are absent on both architectures;
original mirror warnings remain exempt and are not reported as zero warnings.

P6 final qualification: cached GCC x86/x64 full builds and dual publication
succeeded. Each architecture passed 188 background and eight native tests,
392/392 total on first run, with no retry, skip, desktop deferral or changed
timeout. The existing platform-failure and checkpoint cases preserve idle
wake/failure, finite-budget, nested execution and snapshot checks. Native
coverage includes audio, MIDI, Console/Window and product package tests.
This is automated regression, not manual DOS/Win3.1/Win95 performance or
application-speed acceptance. Source review and zero residual limiter symbols
prove removal; no duplicate timing model or test-only production API is added.
Documentation governance, XP manifest, whitespace and line-ending checks pass.
Task-owned scratch is cleaned after promoting these results; caches remain.

## P7 pending-event consolidation

Owner approves the minimal plan after the interrupt-map/event-site investigation.
[Decision and purpose](m9-t100-owner-decisions.md#s4-p7-owner-approved-pending-event-dispatch-consolidation)
record the retained accessor and the three preserved execution positions.
Baseline is P6 `4abcc2f4`. No shared component, guest, INI, medium or snapshot
change. Production +15/-34 (net -19); test +53/-2 (net +51), by Git numstat.
Fixed-XP normalized full-file Myers comparison changes +281/-541 to +269/-541;
selected-content +133/-16 becomes +121/-16. Both reduce mirror diff by twelve
lines. Frozen original S4 statistics are not rewritten.

Cached x86/x64 complete builds and publication succeeded (32.13/38.13 seconds).
Each full background run passed 187/188; the only failure was the new platform
fixture calling machine_run with the clock intentionally stopped, which correctly
fails readiness. A focused correction used the existing fake timer; temporary
phase diagnostics proved the real CPU callback/outer exit completed, then the
following legacy HLT fault injection timed out after inherited device activity.
Resetting the machine between these independent test scenarios fixes isolation.
No product behavior, timeout or skip was changed to obtain a pass. Diagnostics
were removed. Final focused rebuild/runs pass x86 0.27 seconds, x64 1.96 seconds.
The remaining 187 cases per architecture already passed against unchanged final
production code; they were not needlessly rerun after test-only setup fixes.
Both native sets pass 8/8 (24.87 seconds x86, 24.72 seconds x64). Final result coverage is 392/392, not first-run green.

The fixture directly observes clock-before-wake, one-tick consumption from a
pending count of two, remaining tick delivery, empty dispatch, and real CCPU
outer exit leaving wake pending. Existing HLT failure, checkpoint, IRQ, restart,
PC input and package tests also ran. No new manual DOS/Win3.1/Win95 session or
exhaustive interleaving proof is claimed.

| Artifact | Bytes | Compiler date/time | SHA-256 |
| --- | ---: | --- | --- |
| softpc32.exe | 3477700 | Nov 14 2026 16:19:10 | `1c0a1f8187d9ccb07f3129aab7a75129369c8d915fe92d819e98104588e4d989` |
| softpc64.exe | 3095154 | Nov 14 2026 16:23:24 | `a56a4bf5226fe2aee4c7bd0dbddbfb4ddaf73e3fea0dc7f28e921e4120d268ed` |

Self-review of actual diff: only the duplicate pending-consumption bodies and
exports move into the one Compat dispatcher. No original instructions or event
checks reorder; counted ticks remain executor-owned and control wake stays
separate from device time. All obsolete consumption symbols have zero src/test
hits. Original timer-bit consumers remain intentionally distinct. Header and
mirror declaration signatures match. No new ownership, lock, poll or fallback.
Build diagnostics outside the preserved mirror are absent on both architectures;
original mirror warnings remain exempt. Changed mirror source is CRLF; all
other changed text is LF. Manifest, documentation and whitespace gates pass.
Temporary run material is removed after promoting these results; persistent
Ninja and ccache trees remain. P7 waits for owner acceptance, not S4 closure.

## P8 finite-budget removal and DEBUG reuse

Owner approves this exact removal after the production/test caller investigation.
Baseline: P7 9ead43ec. The production driver used only continuous execution;
finite budgets existed for tests. Delete c_main's budget declaration/check,
Compat's counter and active flag, its idle exception, and machine_run's parameter.
No new production branch, callback, counter, poll or public API is introduced.
The existing DEBUG begin/retired hooks and lifecycle stop remain their owners.

The test-local execution fixture binds production DEBUG state, submits TRACE or
an execution breakpoint, observes its real completion result in the existing
executor callback, then requests the existing outer stop. It unbinds both
callback and thread-local debug pointer before its stack state expires. It never
counts instructions. JNZ/return tests use TRACE 1 rather than two pre-fetch
boundaries. Boot, media, audio and mouse fixtures stop at existing completion
PCs; the broader machine smoke uses one TRACE 96000 instead of sixteen budget
6000 slices. IRQ/reset assertions and all guest fixture bytes remain intact.
Lifecycle smoke now waits for actual CCPU entry before requesting external stop,
replacing a fixed sleep and a budget that could terminate before that request.
Error-path tests call continuous run and retain their IO_ERROR assertions.

Actual Git numstat: six production files +9/-45 (net -36); eleven test files
including the 45-line execution fixture +106/-127 (net -21). No build/config
source changes. Normalized fixed-XP Myers numstat for c_main changes +269/-541
to +261/-541; selected content changes +121/-16 to +113/-16. This removes eight
mirror diff lines, not merely eight source lines. No excluded body changes.
Other current mirror patches and the frozen audit inventory are unchanged.

Initial focused x86 run passed 40 cases; partition's incorrectly entered
breakpoint 7c36 was in the JMP operand and could not fire. The specifically
identified test process was stopped after proving that fixture mistake; this
termination is inconclusive, not a product test failure or pass. Correcting the
test to instruction start 7c35 restores the intended completion check. No guest
bytes, product logic or timeout were changed. Subsequent x64 focused run passed
41/41. Final full qualification is recorded below.

Actual-diff self-review and similar-issue sweep: src/test contains no remaining
instruction_budget/consume_instruction_budget symbols. All machine_run callers
use the sole continuous signature. Existing DEBUG command-provider tests retain
product trace, breakpoint and instruction-edge behavior coverage; checkpoint
fixtures retain nested simulation and snapshot assertions. Removing the obsolete
counter tests does not remove tests for a surviving product capability. Test
completion callbacks only request stop and never recurse into execution.
No new public dependency, shared component edit or original guest modification.
The source review is not a new manual DOS/Win3.1/Win95 acceptance claim.

Final cached dual builds/publication pass (12.10 seconds x86, 4.10 seconds x64).
The complete final run passes 392/392 without retries or skips: each architecture
passes 188 background cases and eight native cases. Background job durations:
x86 192.07 seconds, x64 188.07; native 28.13/26.14. Native qualification includes
PCM/MIDI, both product package modes, Window retirement/modal behavior and
Console broker display. This final full run is green; the earlier focused
partition experiment above is not erased or described as first-pass success.

| Artifact | Bytes | Compiler date/time | SHA-256 |
| --- | ---: | --- | --- |
| softpc32.exe | 3477513 | Nov 14 2026 16:58:09 | `1510406695626e840a419ad53871bcbd4ad1a689c391a3bfd1e3d178fe03b6e0` |
| softpc64.exe | 3094678 | Nov 14 2026 17:02:02 | `48f26f2899c0fb7cfb61d987e750091bee21b18f5172a0a5b8089dd60dcbfeec` |

Publication verifies embedded compiler literals. All build diagnostics outside
the preserved mirror are absent; original mirror warnings remain exempt.
Changed mirror source remains CRLF, all other changed text LF. Documentation,
XP manifest and whitespace checks pass. No INI, media or snapshot changes.
Owned background jobs have completed; disposable task material is removed after
promoting this evidence, retaining only the two reusable cache roots. P8 waits
for owner acceptance; S4 remains open at review file 11, with 110 files remaining.

## Accepted P8 follow-up: declaration cleanup and depth investigation

Owner accepts P8 and authorizes only removal of the now-unused
softpc_platform_executor_event declaration in c_main; depth changes remain
unimplemented. One local mirror addition is removed, manifest updated; selected
c_main diff becomes +112/-16. Recompile with each cached compile_commands entry,
redirecting only the object output into task scratch: both exit zero and produce
objects byte-identical to the corresponding P8 cache objects (SHA-256 x86
8791c3f5a63638982dc200e1f9f308072f5b3b1e8208ca7dc0f0205166123b4c;
x64 230ff024fafeee0031e36b7b6c5d5da42f6f06b379d40e975b380b3f09724f98).
No new product build/publication or runtime test is claimed. P8 EXEs are unchanged.

Read-only depth investigation compares fixed XP ntthread.c with the current
file and all actual callers. Original ThreadSimBuf.level increments inside
ccpu386SimulatePtr, decrements before ccpu386Unsimulate longjmp, and remains
unchanged during ccpu386GotoThrdExptnPt exception refetch. Compat independently
increments before simulate setup and decrements after the longjmp returns.
At every actual guest checkpoint both represent the active nesting depth.
Outer stop sets Compat depth to one, then the added ntthread outer-exit helper
sets original level to one and invokes original unsimulate: original level
becomes zero, then the c_main leave hook makes Compat depth zero.
Snapshot capture admits depth one only; resume rejects an active simulation;
exit_requested uses depth to distinguish an active CPU from other callers.
Normal nested BOP exits must continue returning one level, not to the owner.

A coherent single-owner candidate would add a read-only original TLS level
query, declare it in Compat abi.h, replace the three Compat depth reads, remove
its counter/enter/leave/outer-reset assignment and two c_main hooks. Existing
thread-local observer/resume state remains necessary. Original simulate_level
in c_main cannot substitute: this TLS-selected build does not update it.
Existing pointer-return APIs are not queries: one pushes a frame, the other
indexes level minus one and is invalid at depth zero. Reproducing the private
ThreadSimBuf layout in Compat or multiplexing a query into an unwind API is
rejected as added coupling/alternate semantics.

Mirror accounting is unfavorable to this candidate: an ordinary seven-line
query plus separator adds eight lines to ntthread.c; deleting the two c_main
hooks removes two, for net six MORE mirror diff lines before any TLS teardown
safety adjustment. Original exitthread frees its TLS payload without clearing
the TLS slot, so a query must not read that slot after teardown; extending its
contract to that state requires clearing it safely (at least one further mirror
line, with failure semantics resolved), not pretending a non-null pointer is
live. Expected production net reduction is roughly 10-20 lines, but overall
mirror diff grows roughly 6-10 lines. These are design estimates, not measured
implementation/test results. The fake enter/leave fixture must move onto a real
CPU checkpoint if hooks are removed; existing nested BOP, HLT, exception,
resume rejection and outer-exit tests must verify the selected candidate.

Recommendation under the owner's current mirror-diff reduction objective:
keep the two depth hooks for now. No existing read-only original API eliminates
both bookkeeping and total mirror diff; this is a state-ownership tradeoff, not
a confirmed correctness bug. No depth code, guest, shared package or test was
modified by this investigation. The one-line cleanup is independent.

## P9 original simulation-stack depth unification

Owner supersedes the preceding research-only recommendation after explicitly
prioritizing one state authority. Baseline is accepted P8 5d3f3b86; the one-line
unused declaration cleanup was local and is included in this delivery.
Finite scope: c_main hooks, ntthread stack query/teardown, Compat lifecycle and
ABI, existing platform-failure/checkpoint/lifecycle fixtures. No public eight
tree, original guest, user INI, media, snapshot layout or serialization edit.

The original ThreadSimBuf.level alone counts simulation frames. Its query does
not allocate, push, pop or unwind. Missing TLS/initialization gives zero. Compat
removes its thread-local counter and enter/leave APIs, reads original depth at
checkpoint/resume/exit, and no longer repairs its own count before outer exit.
Exit-request lookup checks the existing atomic request first, avoiding a native
TLS lookup when no exit is pending. Observer/context/resume-entry TLS remains
because it is independent operation state, not duplicate depth. Original nested
BOP unwind and outer-owner unwind remain separate existing operations.

Thread teardown clears the native TLS slot before freeing its object. If that
operation fails, it reports the error and retains the still-referenced object,
while continuing removal of the independent thread-list entry. This avoids both
use-after-free and stale registration interfering with later thread-ID reuse;
it can retain resources on native cleanup failure and is not reported as full
recovery. No new retry, cleanup worker, status protocol or allocation is added.
Original allocation/error behavior outside this admitted teardown is unchanged.

Actual Git numstat: production +18/-25 (net -7), tests +24/-5 (net +19).
Five production and three existing test paths change; no build configuration
change. c_main fixed-XP normalized diff moves +261/-541 to +258/-541;
ntthread moves +23/-2 to +36/-3. Total mirror added+deleted diff increases 11
lines. c_main selected-body diff becomes +110/-16. Excluded bodies/line controls
are unchanged. This is a deliberate single-authority tradeoff, not a claim that
net source reduction implies mirror diff reduction.

The platform failure test now invokes failed sound shutdown from a real CPU
checkpoint and proves outer completion with IO_ERROR, instead of manufacturing
an active frame via the deleted API. Existing nested BIOS/HLT/checkpoint tests
assert original depth 1/2/1, zero after outer unwind and repeated resume, and
reject restore while execution is active. Lifecycle smoke checks a new worker's
zero depth, active depth one, zero after stop, and zero after TLS destruction.
No fixed sleep or new test-only production API is introduced.

A disposable probe compiles the actual ntthread.c body with the real x86/x64
compiler options and narrow TlsSetValue/free interception. Both pass: missing
TLS depth zero, nested 1/2/1/0, same-level exception longjmp, failed TLS clear
retains the exact object without free, successful clear frees once and yields
zero, and repeated teardown does not double-free. A real worker also verifies
failed TLS clearing still removes its independent thread registration, and later
explicit teardown releases the retained allocation. Expected failure diagnostics
are captured. This isolated fault injection is additional evidence, not a
substitute for the real executor and snapshot regression; it is not a new
permanent CTest case.

Pre-build P8 snapshot fixture executables are retained only in task scratch.
The existing cross-process script runs P8 writer -> P9 reader and P9 writer ->
P8 reader separately per architecture, including populated/empty media,
wrong-memory-size rejection, resume and input-driven pixel redraw. This tests
actual snapshot compatibility without touching user snapshots or modifying a
guest. Results are recorded below; compatibility with every historical snapshot
version is not claimed.

Similar-issue review: all src/test frame_depth and lifecycle_enter/leave symbols
are gone. Original ntthread owns level increments/decrements; c_main's historical
simulate_level is not a replacement authority in the selected TLS configuration.
No mirrored private structure is copied into Compat. The two exception-pointer
return corrections and existing outer-unwind helper in ntthread are unchanged.
Snapshot archive types/encoders and original instruction/event behavior are
unchanged. Full review of later audit files is not inferred from this work.

The first implementation returned immediately on TLS-clear failure. Actual-diff
self-review identified that this unnecessarily retained the independent thread
registration and could interfere with a reused native thread ID. Final code
keeps the object live but continues original registration removal, with no new
state or recovery loop; the worker fault probe above confirms this distinction.
The final qualification rebuilds both architectures after this correction;
initial-run results are not substituted for final-source qualification.

P8 compatibility writer/reader fixture identities (copied before rebuilding):
x86 SHA-256 `274117a82964325173c642a21c79b1380b3e70e81d134c460f3ce8cba58aa1d5`;
x64 `9238869fd95b29854ef19d54015b1d2f2924081518733218f6895d05fac1a7a6`.
The first candidate passes 392/392 full regression and both compatibility
directions on both widths. A second complete qualification runs after the
self-review correction above, rather than treating those earlier binaries as
the delivered implementation. Temporary probe bodies, executables and generated
snapshot/media files are removed at delivery after promoting concise results.

Final artifact identities (publication verified embedded compiler timestamps):

| Artifact | Bytes | Compiler date/time | SHA-256 |
| --- | ---: | --- | --- |
| softpc32.exe | 3477387 | Nov 14 2026 17:40:29 | `11a1f8a2ae5c501fd51c85a01c2e1bad40a1a472f9bbf5812204e17d0288eb3a` |
| softpc64.exe | 3095066 | Nov 14 2026 17:44:31 | `4273fc824ce426a9f5574887086bf613fdbcc052146340d0f1b8321a7b3576a4` |

The changed original free line's trailing blank was removed for the whitespace
gate. An isolated x86 recompile proves the resulting complete object is byte-
identical to the final x86 build object; x64 final build includes that whitespace
cleanup. No runtime change or hidden source qualification gap is inferred from
this formatting-only correction. Changed mirror files retain CRLF; all other
changed text retains LF. No non-mirror compiler warning/error appears in either
final build; preserved-mirror diagnostics remain exempt.

Final-source qualification passes without test retries, skips or desktop deferral:

| Architecture | Cached full build | Focused lifecycle/checkpoint/failure | P8 -> P9 snapshot | P9 -> P8 snapshot | Background | Native |
| --- | --- | --- | --- | --- | --- | --- |
| x86 | pass, 16.26 s | 3/3 | pass | pass | 188/188, 190.07 s job | 8/8, 26.02 s job |
| x64 | pass, 14.12 s | 3/3 | pass | pass | 188/188, 210.08 s job | 8/8, 24.02 s job |

Total final full coverage: 392/392. Native includes PCM/MIDI, both package modes,
Console display, Window retirement and modal tests. This is automated coverage,
not a fresh manual DOS/Win3.1/Win95 desktop acceptance or performance benchmark.
The old/new snapshot directions use real independently started processes and
existing immutable fixtures; user snapshots are neither edited nor loaded here.
Snapshot format/version/encoders have zero diff. The source/lifetime review,
actual production-path tests and isolated failure injection are separate proofs.

Final self-review confirms removed APIs/state have zero src/test callers, the
original level is executor-thread-local, no private layout is duplicated, and
all normal/exception/outer-return paths retain the original unwind mechanism.
The native TLS-clear failure can retain its allocation until process exit; it
reports failure and clears independent registration, without claiming recovery
or introducing a retry manager. This retained-resource limitation is explicit.
Documentation, XP manifest and whitespace gates pass. Owned jobs are complete;
temporary probes, old test binaries and generated snapshot/media are cleaned
with the task scratch, leaving reusable Ninja/ccache roots only. P9 awaits owner
acceptance; S4/T100 remain open at file 11, with 110 ordered files remaining.

## P11 c_page.c ordered review

Review 12 / order 88 is recorded in the current file-by-file decision ledger.
Fixed-XP selected-content diff remains +20/-6: 15 points, 14 text hunks, all
host DEBUG operand observation. Actual CMake, operand callers, observer owner,
guest DR machinery and existing command-provider assertions were read. No new
production changes or runtime verification. S1 route-removal negative controls
remain historical evidence with their original coverage limits. Recommend
retention, pending owner decision. Frontier: 12/121, 109 remain, c_tlb.c next.
Owner's P10 approval covers files 1-11 only and supersedes their earlier
pending dispositions above; it does not cover this newly presented file.

## P12 c_tlb.c ordered review

Owner approves c_page.c R0115-R0129. Review 13 / order 98 freshly compares
c_tlb.c against fixed XP and traces original private storage, archive callers
and the checkpoint fixture. +58/-0 selected content; one further line control
explains historical +59. Four IDs recommended for retention, pending owner.
Historical flush-only negative controls are not new test runs. Restoration is
not transactional on invalid input; no broader guarantee is claimed. No code
changes. Frontier 13/121, next fpu.c; classifications are in the decision ledger.

## P13 fpu.c ordered review

Owner approves c_tlb.c R0135-R0138. Review 14 / order 121 freshly compares
fpu.c against fixed XP: +8/-5, five text hunks, two remaining semantic points.
R0146 is representation-access correctness; R0147 restores existing rounding
semantics to FIST M64I. Production selection, original conversion callers,
format definitions and current real-instruction tests were checked. Historical
S1 P15 individually reverted each fix and failed on x86/x64; no fresh runtime
test is claimed. Recommend retention, pending owner. No code changes; 14/121
presented, next ntstubs.c. Full point classifications reside in the decision ledger.

## P14 ntstubs.c ordered review

Owner approves fpu.c R0146-R0147. Review 15 / order 182 freshly compares
ntstubs.c to fixed XP: +1/-12, four symbol-ownership points. Current source
confirms original Gdp ownership in ev_glue.c and original empty definitions in
rom.c/fpu.c, with selected build and existing facade/CPU/SAS consumers. No new
runtime tests; S1 P17 duplicate-symbol negative controls remain historical.
Recommend retention, pending owner. No code changes. Frontier 15/121, next
ntthread.c. Per-point counts and evidence reside in the decision ledger.

## P15 original empty stubs with internal linkage

Owner authorizes retaining three original empty functions with static rather
than omitting their bodies. Only ntstubs.c production changes (+12/-1 net +11);
no test source changes. Relative to fixed XP selected content drops from
+1/-12 to +4/-4, five fewer diff lines. Restore original spacing; the excluded
branch's following #line becomes 91, giving copyROM original logical line 92.
No new conditional/build selection or replacement implementation is introduced.
External original definitions remain sole exported owners.

Hidden background cached x86/x64 configure/build/publication succeeds. No
static-vs-extern declaration conflict occurs. ntstubs objects match their
pre-change files byte for byte. Full executable lengths also match. Nine x86
and ten x64 bytes differ, solely in PE COFF timestamp, PE checksum and the
compiler time literal. Every other byte is identical. Runtime tests are not
repeated under the owner-approved artifact-equivalence rule; P9's 392/392 is
historical, not this delivery's fresh test result. Self-review covers actual
diff, selected symbol ownership, line mapping, artifact equivalence, manifest
and whitespace. Existing mirror macro-redefinition/printf warnings remain;
no new nonmirror warnings/errors. No guest/media/config/public-component change.

| Artifact | Bytes | Compiler timestamp | SHA-256 |
| --- | ---: | --- | --- |
| softpc32.exe | 3477387 | Nov 14 2026 18:51:12 | a31a4afc56dae6d2ea4a67a42529a6e5ef12bf099f0e7b88f0f7c6c2d03780cd |
| softpc64.exe | 3095066 | Nov 14 2026 18:51:26 | 19f27e4c52afed7568363ae8b12fecd8ac3ced77d9908c0aca3fc1d298c0fbc0 |

Disposable comparison/build logs are removed after promotion of these results.
The four-point file frontier remains 15/121; next file is ntthread.c.

## P16 ntthread.c ordered review

Review 16 / order 183 compares the entire current ntthread.c with fixed XP:
+36/-3, five points; frozen counts predate approved P9. Fresh caller review
covers worker teardown, lifecycle checkpoint/resume/exit and c_main exception
setjmp. P9 query/teardown retains owner approval; outer unwind and two explicit
null returns are recommended with evidence limits. In particular null return
is not complete caller recovery. No new runtime tests or production changes;
historical S1 negative controls and P9 qualification remain separately identified.
Point details are in the decision ledger. Frontier 16/121; zfrsrvd.c next.

## P17 zfrsrvd.c ordered review

Owner approves ntthread.c R0160-R0162; P9 points were already approved.
Review 17 / order 265 compares zfrsrvd.c to fixed XP: R0172 +17/-0, one hunk,
two snapshot accessors. Current private saved-IP state, original delayed IRQ
functions, actual archive consumers and checkpoint assertions were inspected.
Historical coherent-removal failures on both architectures support retention;
no fresh runtime tests or code changes are claimed. Counts and limits reside
in the decision ledger. 17/121 presented, next comms/com.c, 104 remain.

## P18 com.c ordered review

Owner approves zfrsrvd.c R0172. Review 18 / order 267 freshly compares com.c
with fixed XP, excluding excluded bodies/anchors: +126/-3, six text hunks,
five functional IDs and historical notice. Production CMake, ioctl consumer,
archive callback/state consumers and current checkpoint fixture were checked.
All three pointer conversions have historical independent x64 failures;
queued receive callback removal has historical dual capture failures. No live
serial or comprehensive field coverage is claimed, and no fresh runtime test.
Point sizes, snapshot sub-block sizes and limits reside in the decision ledger.
No code changes; 18/121 presented, printer.c next.

## P20 excluded SCCSID-content correction

Owner rejects moving original excluded SCCSID content into a new unconditional
comment. Remove that one line in com.c; do not restore the excluded declaration
or alter its conditional/#error framework. R0173 is no longer a retention item.
Five functional points stay pending. Source +0/-1; tests unchanged. Selected
XP diff +126/-3 becomes +125/-3, reducing mirror diff by one line.

Hidden cached x86/x64 builds/publication pass. Full EXEs preserve sizes and each
has nine changed bytes, only in PE COFF timestamp, OptionalHeader checksum and
the compiler time string at its original location. All other bytes are identical.
No fresh runtime suite under the owner-approved equivalence exception. Mirror
warnings remain; no new nonmirror warning/error. Documentation, XP manifest and
whitespace gates pass; task scratch removed after job completion.

- softpc32.exe: 3477387 bytes, Nov 14 2026 19:22:20; SHA-256
  be426a92a927f15764816ed832c8fde5d8115831f9d47e0f082543cc435f769c.
- softpc64.exe: 3095066 bytes, Nov 14 2026 19:22:33; SHA-256
  83b4dc49764ee6d1f1041efe4d1efa32240c039441052cf47393f322859094ca.

Actual-diff review confirms only the extracted comment is removed; no UART
state/schema/behavior, guest or user configuration changes. P19 notice reasoning
is superseded by the explicit owner direction, not silently retained as policy.


## P22 added UART declaration cleanup

Owner authorizes removal of duplicate declarations introduced by local mirror
patches only; original XP duplicate declarations remain permitted. One source
file changes: base/comms/com.c, +118/-121 (net -3); tests unchanged. Move the
added snapshot functions after original declarations and remove the three added
recv_char, do_wait_on_send and tx_pacing_enabled declarations. Byte comparison
reconstructs the final source exactly as that move minus those three lines;
all four function bodies and original text remain unchanged, with CRLF intact.
Selected fixed-XP diff decreases +125/-3 to +122/-3: three fewer mirror-diff
lines, not the gross moved-block Git count. No schema or shared interface change.

Cached x86/x64 configuration, complete test builds and dual publication pass.
Existing mirror warnings remain; no new nonmirror warning or build error was
observed. Full EXEs have nonmetadata differences, so the equivalence exception
is not used. Isolated before-source compilation with actual production flags
and old/new symbol tables confirms changed function placement; this does not
by itself establish runtime equivalence.

- softpc32.exe: 3477387 bytes, Nov 14 2026 19:31:25; SHA-256
  8cfecce14a81202736cc1ad07577a6c9342635693cf72fd60d7d9a07ea5aa4cd.
- softpc64.exe: 3095066 bytes, Nov 14 2026 19:31:40; SHA-256
  81849b233937300808d6fec7a24ec6b5732e81680312e454870bb4a7956f769b.

Same-file declaration/use review confirms the original declarations precede
all moved uses. No original duplicate was removed, no excluded-body content
was promoted, and no test-only interface, alternate state owner or new path
was introduced. This is not an audit of every declaration in unreviewed files.
Fresh full regression passes 392/392, without retries or skips:

| Architecture | Background | Native desktop/audio/package | Elapsed seconds |
| --- | --- | --- | --- |
| x86 | 188/188 | 8/8 | 195.78 + 25.07 |
| x64 | 188/188 | 8/8 | 199.85 + 24.34 |

Commands use ninja-ccache-test-x86/x64 presets for background work and explicit
serial desktop|native_audio label selection for native tests. Existing checkpoint
coverage runs in this full suite; no new live serial-device or exhaustive UART
combination proof is claimed. Actual-change self-review finds no blocking issue.
Documentation, fixed-XP manifest and whitespace checks pass. Owned jobs completed;
concise evidence is retained here and disposable task scratch is removed before
delivery, preserving reusable caches. S4/T100 stay open at review file 18.


## P23 printer.c ordered review

Owner accepts com.c after P22. Fresh pinned-XP comparison and current caller,
compile-definition, original callback and checkpoint-fixture review covers
printer.c (19/121, order 268), +74/-0 in three text points. Decision ledger
records snapshot necessity, historical negative controls, limits and a three-line
reduction candidate (notice plus two added declarations). No production change,
new runtime proof or general approval of unreviewed files. Documentation,
manifest and whitespace gates pass; no disposable build work is created.


## P24 mirror-wide added-diff cleanup

Owner approves printer.c cleanup and extends the same-issue audit to the entire
mirror. Scope is added local differences only: excluded SCCSID content extracted
as unconditional comments and avoidable forward declarations created by placement
of added functions. Original declarations/duplicates, existing exclusions and
unrelated semantic differences are not cleanup targets. This is not approval of
all unreviewed semantic points or advancement of their manual review frontier.

### Finite sweep and dispositions

The current manifest supplies all 472 paths; dispositions reconcile with the
frozen directory-order ledger;
468 are text, four binary resources are not C declaration/comment candidates.
Compare every text file with fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Collect added declaration/comment candidates from nonmatching text, inspect
multiline declarations and original providers/callers; original unchanged
statements are not candidates. Existing excluded bodies receive no ledger.
All candidate classes below have a disposition; this is a bounded same-issue
sweep, not a new full semantic audit of the remaining mirror differences.

- Remove 18 added SCCSID comments. Each literal is verified against its original
  SCCSID branch, and the local excluded branch already contains #error. No text
  from that branch is retained outside it; original unconditional notices stay.
- printer.c: move four added snapshot functions after original callback
  definitions, inside the original PRINTER guard; remove two added declarations.
- fla.c: move only the added callback encoder/decoder after the original callback
  definition; remove one added declaration. Other snapshot functions stay put.
- nt_sbdsp.c: move the added StartDspTransfer function after original
  AutoInitEoiHook's current definition; remove one added declaration. Preserve
  both function bodies and the existing public declaration in nt_sb.h.

| Remaining candidate family | Disposition / evidence |
| --- | --- |
| c_main.c seven extern declarations; c_page.c two DEBUG declarations | Actual x86/x64 preprocessing shows exactly one declaration per name; external providers, not duplicate local forward declarations. Retain. |
| ccpusas4.c host_sas_init/term; bios.h host_reset | Actual consumers have exactly one declaration at both widths; pointer-return ABI / reset interface, not redundant. Retain. |
| vga_prts.c Currently_emulated_video_mode / ega_snapshot_seq_reset; nt_graph.c DAC_data_bits; nt_sb.c bExitDMAThread | Exactly one visible declaration each in actual dual-width production units; external state/providers. Retain. |
| c_bsic.h, cpu4gen.h, egaports.h, quick_ev.h, sas.h, nt_graph.h, nt_sb.h | Public-to-mirror provider declarations or existing type/signature corrections, not local ordering duplicates. Retain; no second declarations introduced. |
| ntstubs.c Gdp and three static functions | Existing separate symbol owner and approved linkage changes, not removable duplicates. Retain. |
| at_dma.c backward-copy declarations/definitions | Existing declarations with corrected address types; original declarations are not removed. |
| nt_graph.c local static prototypes | Original forward-declaration sites with linkage/type correction. Removing or moving original functions would change a different mirror-diff purpose. Retain. |
| nt_sbdsp.c DspCompleteBlock | DspScheduleCompletion schedules this callback; callback calls DspScheduleCompletion. Ordering alone cannot remove the forward declaration without replacing it or changing the call graph. Retain. |
| Other candidate static/global lines | Data definitions, actual function definitions, K&R parameter declarations, calls and macro expansions; not duplicate forward declarations. No cleanup. |

No permanent regex gate is added: declaration necessity depends on supported
preprocessing, scope and dependency cycles; a spelling-only gate would reject
permitted original duplicates. Existing provenance/manifest gates remain.

### Exact accounting

Before-source SHA-256 for each changed file matches its frozen S4 file ledger.
Current source changes only move existing added functions and remove the listed
added lines; seven moved function bodies are byte-identical. All preprocessor
lines, including #error/#line, remain byte-identical in sequence; mirror CRLF
is preserved. No schema, ABI, guest/media/configuration or shared-package change.

Semantic cleanup accounting: 18 complete notice points plus three declaration
placement groups = 21 cleanup points. The declaration groups contain four
individual declaration lines, so the removed added lines total 22. This is not
a claim that Git's textual hunk count decreases by 21: moved blocks can split
or merge textual matches.

For these 19 files, selected fixed-XP differences decrease from +1712/-1072 to
+1690/-1072: 2784 -> 2762 changed lines, reduction 22. Deleted-XP-line count is
unchanged. Original frozen ledgers remain immutable. The 16 comment-only edits
are exact one-line deletions. Structural XP comparison independently verifies
printer.c +74/-0 -> +71/-0 and fla.c +99/-0 -> +97/-0. nt_sbdsp.c moves one
already-added function after its dependency and removes one added prototype;
its selected difference decreases +524/-1000 -> +523/-1000. Existing unrelated
local conditional structures are not rewritten merely to simplify the counter.

| File | Before selected + / - | After selected + / - | Reduction |
| --- | --- | --- | --- |
| base/comms/printer.c | +74/-0 | +71/-0 | 3 |
| base/comms/printer_.c | +1/-0 | +0/-0 | 1 |
| base/comms/rs232_io.c | +1/-0 | +0/-0 | 1 |
| base/disks/diskbios.c | +5/-4 | +4/-4 | 1 |
| base/disks/fla.c | +99/-0 | +97/-0 | 2 |
| base/disks/gfi.c | +1/-0 | +0/-0 | 1 |
| base/disks/gfi_mpty.c | +1/-0 | +0/-0 | 1 |
| base/keymouse/keyba.c | +126/-4 | +125/-4 | 1 |
| base/keymouse/keybd_io.c | +1/-0 | +0/-0 | 1 |
| base/keymouse/mouse.c | +116/-34 | +115/-34 | 1 |
| base/keymouse/ppi.c | +30/-13 | +29/-13 | 1 |
| base/support/time_day.c | +2/-1 | +1/-1 | 1 |
| base/system/dummy_nt.c | +1/-0 | +0/-0 | 1 |
| base/system/ica.c | +121/-9 | +120/-9 | 1 |
| base/system/quick_ev.c | +231/-0 | +230/-0 | 1 |
| base/system/timer.c | +376/-6 | +375/-6 | 1 |
| base/system/unexp_nt.c | +1/-1 | +0/-1 | 1 |
| host/inc/host_hfx.h | +1/-0 | +0/-0 | 1 |
| host/src/nt_sbdsp.c | +524/-1000 | +523/-1000 | 1 |

Git production diff (moves included): +99/-121, net -22, 19 files. No tests changed.
Counts are separate from the selected XP baseline accounting above. Snapshot
behavior is not reimplemented; original/private state remains in its owner.

Final-source qualification is recorded below.


### P24 qualification

Cached x86/x64 configure/full test builds and dual EXE publication pass.
Existing mirror warnings remain; no nonmirror warning or build error is found
in the final build logs. Executable code placement changes, so no artifact-
equivalence exemption is taken. A first 392/392 run covered the initial source;
the extra nt_sbdsp declaration cleanup was discovered while that run was active.
Its artifacts were not overwritten: the final runner waited for that run to
exit, rebuilt both architectures, then started a fresh complete qualification.
Only the latter result qualifies the final source. No tests were modified.

- softpc32.exe: 3477387 bytes, Nov 14 2026 20:01:55; SHA-256
  51ae2100d996fdd2c112c3ddaaba4787086d1dd5f66bfa45aa46ecaae1627f82.
- softpc64.exe: 3095066 bytes, Nov 14 2026 20:02:09; SHA-256
  0f0a34e9795f16b9fb95182aa3da37af8135fb8f5cda0c0e961dd47379caf886.


Final-source full regression passes 392/392, no retries or skips:

| Architecture | Background | Native desktop/audio/package | Elapsed seconds |
| --- | --- | --- | --- |
| x86 | 188/188 | 8/8 | 185.03 + 24.39 |
| x64 | 188/188 | 8/8 | 203.54 + 24.06 |

Existing checkpoint and Sound Blaster tests run on final code; this does not
claim exhaustive serial/printer hardware or every audio timing combination.
Actual-diff self-review confirms narrow scope, unchanged bodies/conditional
sequence, original duplicate preservation and correct PRINTER scope. Fixed-XP
manifest, documentation and whitespace gates pass. No blocking finding remains.
Owned background jobs are complete; disposable scan/preprocess/build/test data
is removed before delivery, leaving only reusable caches. S4/T100 stay open.


## P25 accepted P24 and current aggregate

Owner accepts P24 and requests current whole-mirror statistics. Production
baseline is 327f996099cfd6b6851dfa40dad5220abee47e95; fixed XP remains
a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
All 472 current paths are reconciled with the frozen S4 raw identities:
449 unchanged, 16 exact single-comment deletions, seven fully recounted files.
For the seven, pair conditional branches, omit existing excluded bodies and
line controls, retain changed condition text, and use zero-context Git Myers.
Recomputed frozen counts exactly match the old ledger before applying current
counts. The sixteen removed comments were isolated one-line text blocks.
CRLF/LF normalization is the same as the frozen comparison convention.

| Current classification | Files |
| --- | ---: |
| Identical to XP after newline normalization | 254 |
| Exclusion/line-control scaffolding only | 104 |
| Real retained-content differences | 114 |
| Total | 472 |

Current real-diff totals: 15,761 text blocks, +24,728/-24,062, 48,790 added-plus-
deleted lines, net +666. Frozen S4 totals were 121 files, 15,778 blocks,
+24,764/-24,069 (48,833): seven fewer files, seventeen fewer text blocks and
43 fewer changed lines. Text blocks are not semantic points; no complete
semantic-point total is asserted before the outstanding manual review.
This documentation-only accounting makes no new runtime claim and does not
close S4/T100. Documentation/manifest/whitespace checks pass; scratch removed.


## P30 nonsemantic restoration across 114 files

Owner approves diskbios.c R0414 and authorizes a bounded sweep of all 114
current real-difference files against fixed XP SP1. Freeze input at 24e0964a;
retain the original dir ordinals. Audit only the nonsemantic cleanup class,
not renewed approval of every semantic patch. Existing #error bodies receive
no accounting. Compare paired conditional arms and retained text, including
comments/blank lines on their outer directives. Local added conditional
frameworks remain visible; they are not silently treated as original branches.

All 114 files were scanned. Candidate generation compares retained-text Git
Myers differences, joins nearby blocks, and checks whitespace/comment-neutral
matches inside mixed blocks. Such matches are candidates only: string contents,
preprocessor structure, real control-flow context and ownership are reviewed
before restoration. The generated family has no independent restorable text
candidate in this sweep; this does not approve its remaining semantic patches.

Production change is +34/-34, net zero; tests are unchanged.
Restore 24 groups across 15 mirror files: original directive comments/spacing,
blank-line placement, one field's original whitespace, and two still-accurate
MIDI inline comments. Keep the new scope indentation around actual semantic
changes: guarded interrupt delivery, TLS release, mouse interrupt guards,
new video loops/cursor handling, audio allocation/transfer and MIDI parsing.
Unrelated identical braces/return lines matched by text are not evidence that
they represent the same original statement. Do not restore an obsolete memory
release comment onto a replacement callback/snapshot body. Added explanations
of real fixes remain; no semantic implementation or excluded body is changed.
The two MIDI comments are restored without reverting the current necessary
nesting. They and the field-spacing restoration improve textual fidelity but
do not reduce line-based counts because their surrounding semantic hunk remains.

| File | Restored groups | Before text blocks / + / - | After text blocks / + / - |
| --- | ---: | --- | --- |
| `base/disks/diskbios.c` | 1 | 3 / +4 / -4 | 1 / +2 / -2 |
| `base/inc/config.h` | 1 | 2 / +2 / -2 | 0 / +0 / -0 |
| `base/inc/ios.h` | 1 | 3 / +3 / -3 | 1 / +1 / -1 |
| `base/inc/rs232.h` | 2 | 4 / +3 / -3 | 0 / +0 / -0 |
| `base/keymouse/keyba.c` | 4 | 10 / +125 / -4 | 4 / +121 / -0 |
| `base/support/time_day.c` | 1 | 2 / +1 / -1 | 0 / +0 / -0 |
| `base/system/ica.c` | 1 | 7 / +120 / -9 | 5 / +118 / -7 |
| `base/system/timer.c` | 2 | 12 / +375 / -6 | 8 / +371 / -2 |
| `base/video/ega_prts.c` | 1 | 3 / +7 / -1 | 1 / +6 / -0 |
| `base/video/ega_read.c` | 1 | 2 / +1 / -1 | 0 / +0 / -0 |
| `base/video/ega_vide.c` | 1 | 3 / +15 / -2 | 1 / +13 / -0 |
| `base/video/ega_writ.c` | 1 | 2 / +1 / -1 | 0 / +0 / -0 |
| `base/video/gfx_updt.c` | 4 | 139 / +235 / -215 | 131 / +231 / -211 |
| `host/inc/cfpu_def.h` | 1 | 13 / +13 / -8 | 13 / +13 / -8 |
| `host/src/nt_sbmid.c` | 2 | 33 / +285 / -399 | 33 / +285 / -399 |

The 13 files with numerical deltas were recounted by structural XP pairing and
zero-context Git Myers; their before counts exactly match the published P25
inventory. The two other changed files have zero count delta. Other 99 source
bodies remain byte-identical to the batch start. Preserve all frozen TSVs.

Current totals: 109 real-diff files, 15,721 textual blocks, +24,699/-24,033,
48,732 added-plus-deleted lines. Compared with P25: five fewer files, 40 fewer
text blocks and 58 fewer diff lines (-29 added/-29 deleted). Net source length
is not the measure of this improvement. Text blocks are not semantic points.
Remove config.h, rs232.h, time_day.c, ega_read.c and ega_writ.c from the current
real-diff list: only exclusion/line scaffolding remains. Whole-mirror classes
are now 254 identical, 109 exclusion-only, 109 real-diff, total 472.

All changed full-file C token streams (including directive expressions and
string contents, excluding comments/whitespace/line controls) are identical
before/after. CRLF remains intact. Existing #error text/order is unchanged;
rs232.h's moved boundary anchor is corrected from 326 to 327 so BIT_ORDER2
continues to map to XP line 328. Other necessary line controls remain.
One moved original tab-only line is retained exactly as XP; ordinary Git
blank-at-EOL diagnostics there are an original-whitespace restoration, not a
compiler warning or new nonmirror style exception. Review mirror whitespace
under its preserved-source policy and other changed paths with normal checks.

Cached production/package and complete test-target builds pass on both
architectures. tools/build.cmd remains CMD-only; the owned background runner
captured completion without interactive timeout cancellation. Full regression:

| Architecture | Background | Desktop / native audio / package | Seconds |
| --- | ---: | ---: | --- |
| x86 | 188/188 | 8/8 | 190.54 + 23.29 |
| x64 | 188/188 | 8/8 | 226.62 + 24.59 |

All 392 tests passed on their first run, with no retries or skips. Compared
with the cached pre-change 1,088 object identities, only the two intentional
build-timestamp composition objects change; all 1,086 others remain byte-equal.
Both complete EXEs equal the previous published artifacts after normalizing
only PE timestamp/checksum and the single embedded compiler time string.
No other executable bytes differ. This is artifact equivalence evidence,
separate from the completed full runtime tests.

- softpc32.exe: 3477387 bytes; Nov 14 2026 21:20:39; SHA-256 c3dd1ea9fe09111ce0c55f5d4dec096164d32a39043414c6c20634f3ab026d84.
- softpc64.exe: 3095066 bytes; Nov 14 2026 21:24:31; SHA-256 098a06ff4a8815d98af996419bc34053b5b2bb3400710072960aab4afe4c3eb7.

Actual-change self-review confirms the only production modifications are the
listed original-text restorations and the corrected line anchor. No test,
Compat/shared implementation, configuration, media or snapshot is modified.
Build warnings are confined to the preserved mirror; no nonmirror warning
was emitted in these logs. Documentation, mirror manifest and applicable
whitespace checks pass. Owned scratch is removed after promoting this evidence;
only reusable build caches remain. S4/T100 remain open, not closed by this P.


## P34 redundant non-XP conditional wrappers

Owner approves cpu_vid.h restoration and the same-issue sweep of all current
real-difference files. Input is ccee5c80: 109 paths in the current inventory,
compared with fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Frozen dir ordinals remain authoritative. This sweep concerns unnecessary
added conditional framing, not approval of every remaining semantic patch.
Existing original excluded bodies are neither audited nor booked.

Enumerate conditional-directive sequences in all 109 full local/original files;
inspect inserted/replaced sequences in context against original parent/arm
structure. Twelve files have candidates. Eight contain ten redundant wrappers
whose sole reachable body is already selected by every applicable supported
build. Remove those local wrappers, preserving selected C statements and all
original conditional structures. In egacpu.h shorten the stale explanation
that claimed an alternate type was retained for other profiles. No test changes,
new APIs, alternate runtime paths or guest/media changes are involved.

| File | Wrappers removed | Before blocks / + / - | After blocks / + / - | Preserved responsibility |
| --- | ---: | --- | --- | --- |
| base/inc/cpu_vid.h | 1 | 2 / 3 / 0 | 0 / 0 / 0 | Exact original READ_POINTERS fields. |
| base/inc/gmi.h | 1 | 2 / 3 / 0 | 0 / 0 / 0 | Exact original MEM_HANDLERS fields. |
| base/inc/egacpu.h | 1 | 1 / 8 / 1 | 1 / 4 / 1 | Necessary generated VGA state pointer type stays. |
| host/inc/cfpu_def.h | 1 | 13 / 13 / 8 | 13 / 10 / 8 | Existing little-endian FPU field ordering stays. |
| host/inc/host_cpu.h | 1 | 2 / 4 / 1 | 2 / 1 / 1 | Existing register byte ordering stays. |
| base/system/ica.c | 3 | 5 / 118 / 7 | 5 / 109 / 7 | CCPU hook body and PIC snapshot field/callback checks stay. |
| base/keymouse/ppi.c | 1 | 6 / 29 / 13 | 6 / 27 / 13 | Original enclosing NTVDM condition and intentional content omission stay. |
| host/src/nt_graph.c | 1 | 11 / 32 / 13 | 10 / 30 / 13 | Existing clear-screen path stays; duplicate line anchor removed. |

Actual production delta: eight files, +1/-43, net -42. Tests: +0/-0.
The retained-content XP difference decreases by 29 added lines, no deleted-line
change; five fewer textual blocks. Those are distinct from physical line counts.
Changed-file before counts exactly reconcile with the existing inventory using
paired original/local conditional arms and zero-context Git Myers comparison.
Only bodies corresponding to existing exclusions and line controls are omitted;
added wrapper directives remain counted until physically removed.
Current totals: 107 real-diff files, 15,716 blocks, +24,670/-24,033; 48,703 total
difference lines. cpu_vid.h and gmi.h leave the actionable list. Whole mirror:
254 identical + 111 exclusion/line-control-only + 107 real-diff = 472 files.

The remaining candidate structures are deliberately retained:

- c_main.c's six TLS selector changes activate the existing original stack
  storage/access/reset path, not an empty configuration check.
- ica.h's CPU_40_STYLE guard prevents original PIC bit-number macros from
  colliding with CCPU interrupt-kind values. Prior coherent P89 controls failed
  with duplicate cases on both widths without it; no new negative test claimed.
- ios.h's selector chooses the complete port-address dispatch table. Prior P90
  controls demonstrated aliasing with the original narrower table.
- ppi.c's remaining selector enables the actual HostPpiState speaker callback.
- insignia.h's CVIDC_RULE_WORD selects pointer-sized generated-rule words only
  in their owning source group; other translation units retain original IUH.
- nt_graph.c's V7VGA guard encloses a real Video7 width correction dependent on
  that mode's rendering function. It is not an empty rejected alternative.

These six candidate files overlap two of the eight edited files. Four candidate
files are unchanged; 97 files have no added/changed directive sequence. Thus
109 = 97 + 12, and 12 = 8 edited + 4 unchanged. Remaining semantic differences
are not approved merely by passing this bounded scan.

Build evidence uses actual CMake definitions: CPU_40_STYLE/CCPU/C_VID and
SOFTPC_STANDALONE on the original production groups; host_def.h defines
LITTLEND; ica.c derives HOOKED_IRETS from the selected CPU_40_STYLE/non-NTVDM
path. The selected C text in all eight files is unchanged after excluding
comments/directives; all eight preserve CRLF. The other 101 input files match
HEAD after Git checkout newline normalization. Necessary existing line-control
anchors are retained; orphaned wrapper anchors in cpu_vid/gmi and the duplicate
nt_graph anchor are removed. No XP conditional or original branch body is
removed by this batch.


Verification and delivery:

- Cached x86/x64 package builds and complete test-target builds pass. The owned
  hidden CMD runner has explicit exit 0. Its first completion marker was empty
  because CMD parsed echo 0> as descriptor redirection; the marker was corrected
  and the cached build rerun successfully. No build was killed or timed out.
- Compare all 1,088 cached object identities before/after: 1,086 are byte-equal;
  only the required two timestamp-bearing composition objects differ.
- Both complete published EXEs are byte-equal after normalizing exactly the PE
  TimeDateStamp, PE CheckSum and the single compiler __TIME__ string. No other
  bytes differ. This is whole-artifact evidence, not only a text-section check.
- Under the owner's explicit equivalence exception, do not rerun runtime suites
  when artifacts have no substantive difference. No new full regression or
  guest/desktop acceptance is claimed; the prior P30 392/392 remains historical.
- CTest source-boundary, documentation, XP-manifest and inward-dependency gates
  pass 4/4 on each architecture (8/8 total; x86 16.37s, x64 8.66s). Final direct
  documentation/manifest and whitespace checks pass. No new permanent tests
  are needed for deleting compile-time-only wrappers.

Published softpc32.exe: 3,477,387 bytes, Nov 14 2026 22:07:21,
SHA-256 2d791e16a3a412a6633fb74c48d919a385d98c11b5d6326c5a7c11c6f5a5dbf3.
Published softpc64.exe: 3,095,066 bytes, Nov 14 2026 22:07:26,
SHA-256 2ea34cce3f3bc4e39dce96b0b8ee69ad3161f3bce1fd80b9518d355d6c4d7a56.

Actual-change coordinator review: only the eight listed mirror files, their
manifest, current inventory/decisions/evidence/packet and the two EXEs change.
No shared source/test, build tool, configuration, guest medium or snapshot changes.
The same original runtime owners remain; no second implementation is introduced.
Disposable probes, XP copies and logs are removed after promoting this evidence.
