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
