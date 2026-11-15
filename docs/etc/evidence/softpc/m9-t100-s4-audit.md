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
