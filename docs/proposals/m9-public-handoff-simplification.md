# Public Handoff Simplification Review

## Admission and scope

### S6 delivery and owner disposition

The owner rejects carrying the historical native-start observation as deferred
work after the passing current-environment qualification: "干嘛保留？你都测试通过了，保留个毛线？"
Accordingly, its research stays below as history, without an active TODO, Queue
candidate or delivery blocker. This is an explicit acceptance disposition,
not a claim that a Windows initializer was repaired or that RDP causation was
proved. S6 is verified pending committed-diff review and owner acceptance;
T99 stays open and S7-S11 remain planned.

The frozen four-case ledger is complete under that owner decision:

| Case | Final disposition and evidence |
| --- | --- |
| HLT/debug timeout | Correct the fixture to enter its own EB FE loop before resume/pause. Preserve the original HLT/interrupt contract; checkpoint regression passes on both widths. |
| TF quick eligibility | Clear quick_mode on the existing local debug-hook refetch edge. Source invariant and historical stale-TF observation support the correction; focused repetitions and complete regression pass. Do not claim a new deterministic failing-before TF reproduction. |
| Invalid boot/null BOP | Restore the fixture media ejected by earlier assertions before reset/restart. Paired actual-path probes change 7C00 from zeros to EB FE; original CPU reset returns F000:FFF0. No BIOS/BOP workaround is introduced. |
| Native Console initialization | Historical rejection is before main. Current full qualification and 24 additional desktop package executions pass. Owner accepts this disposition without retaining a deferred task; no native-failure repair is claimed. |

Production changes only c_main.c's existing port hook: +4/-0, net +4 relative
to the S6 implementation baseline (898e4054 and 7d3b711d have identical code).
Test changes only command_provider_smoke.c (+17/-0) and runtime_smoke.c
(+3/-0): +20/-0, net +20. The same four added source lines increase the local
patch against the XP necessary subset; upstream and subset hashes are unchanged.
Its manifest changes only c_main.c's normalized local hash. The public eight
trees, package fixture, user INI, media and snapshots have no diff. No new API,
state owner, execution path, retry, timeout, thread or polling is introduced.

Verification is the earlier fresh cached dual build/publication and exact
192/192 passing registrations per architecture (184 background, three audio,
five desktop/package), supplemented by 120 uninstrumented focused executions
and the separately recorded native diagnostics. The final temporary PID probe
is removed; both package fixtures rebuild and their four registered executions
pass. The additional desktop package total is 24 (4 initial capture, 16 bounded
compact samples, 4 restored-fixture cases), all passing. It is not another full
suite rerun or a repair claim for an unreproduced native failure.

Self-review checks the actual three code-path diffs, retained assertions,
single ownership, normalized mirror accounting and the artifact hashes below.
Both ETW sessions are confirmed absent; all owned jobs exit before scratch
retirement. Only build/.ccache and build/.ninja remain. Final documentation,
mirror, whitespace and residue checks precede commit/push and coordinator
review of the actual committed changes. User acceptance, not this record,
will close S6.
Final checks pass: documentation governance, all 505 XP mirror bodies,
diff whitespace and build residue. One manual relative-root mirror invocation
cannot resolve its manifest; the exact registered absolute-root invocation
passes. No source change is made to obtain that result.

### S6 preceding research and repair record

The following records research states before the final disposition above.

Implementation progress after the owner's repair authorization: the integration
fixture now installs EB FE at 0000:0520 before its deliberate resume and checks
that paused CS:EIP remains there. This adds 11 test lines, removing none. With
that setup alone and the original quick-mode code, 20 command-provider runs per
architecture pass (40/40). This is not a failing-before/passing-after TF proof.
The local debug-hook return now clears quick_mode before the existing NEXT_INST
edge, adding four production lines and no second execution path. Another 20
command-provider runs per architecture pass (40/40). Both jobs complete; these
are bounded diagnostic runs, not full qualification. Mirror accounting updates
only c_main.c's local normalized hash; upstream/subset identities are unchanged.
The rebuilt checkpoint test passes on both widths, preserving its existing HLT
and interrupt-shadow assertions. XP mirror and its negative selftest also pass
on both widths (six focused test executions). No EXE publication or completed
repair P is claimed.

Further actual-path probes distinguish CPU reset, device POST and the first
executor callback. CPU reset and POST both return F000:FFF0. RESET_COMPLETED is
not this return: Emulator installs its executor callback and runs the driver
before publishing that fact at the pause rendezvous. The earlier inference of
already-corrupt reset output was therefore premature. The original nested POST
simulation is real, but its register save/restore is not implicated by these runs.

The outer fetch sequence reaches F000:FFF0 -> F000:E05B (reset BOP) -> E05E
(INT 19h) -> the original bootstrap -> 0000:7C00. At 7C00 the original fixture
reads 00 00 00 00, not its intended EB FE. command_provider_smoke previously
executed a real floppy eject while testing ERROR command admission, then reset
without reinserting its only boot medium. The fixture's supplied monitor state
is ERROR but the actual Machine is stopped, so that eject succeeds. The original
bootstrap reports failed boot but still transfers control to 7C00; executing
uninitialized guest memory can reach arbitrary BOP bytes. No BIOS no-op handler
or CPU reset workaround is appropriate for this test preparation error.

Keep the eject test, assert its actual completion, and reinsert the fixture image
before reset (+6 lines). With the same temporary probes this changes 7C00 to
EB FE and the first callback to the intended loop; both widths pass 20 runs
(40/40). An initial added assertion omitted the existing CRLF output suffix and
failed before boot; correcting that assertion is not counted as a product fix.
The earlier 40 no-media probe runs also pass as tests despite observing wrong
boot bytes, demonstrating why pass counts alone missed this precondition.

A same-class scan of all App tests finds only runtime_smoke's two direct ejects
and command_provider_smoke's provider eject. runtime_smoke ejects before its
restart/reset sequence too, matching the owner of the historical T84 null-BOP
report. It now reinserts its fixture at the stopped boundary (+3 lines); its
final eject remains valid teardown coverage. This does not prove every possible
guest null BOP has the same cause or reproduce the historical AV instruction.
All temporary production probes are removed. Uninstrumented command-provider,
runtime and checkpoint each pass 20 repetitions on each width (120 executions).
The subsequent complete cached dual build publishes both EXEs. Each width passes
184 background, three native-audio and five desktop/package tests, 192/192,
without failed/skipped registrations; build logs contain no compiler warnings.
This is fresh full regression, not proof of the still-unexplained native failure.
Published SHA-256: softpc32.exe
0ea1a248bc280f9ca04b52005ee2594c5d9568afcf07311dd81262b37e08f6bf;
softpc64.exe 337ebe62da50318fe53c82b4275c0f73cdba2389edfee8be29261208ecbf6077.
Actual production C/H delta is +4/-0 in one local CCPU hook; tests +20/-0 in
two App fixtures. Public components, original BIOS/BOP, INI and media are
unchanged. Actual-diff review finds no additional execution path or polling.

Native-start follow-up uses an independent small Win32 child (no SoftPC linkage)
with an event set from main. Forty alternating launches compare immediate
AttachConsole/hide against attachment only after that event. Both groups reach
main and attach (20/20 each); this provides no evidence for a fixture timing
change. The first diagnostic launch was incomplete because PowerShell treated
its stderr record as a terminating script error; switching diagnostic output to
stdout fixed the harness, not a native initialization failure. A separate
sandbox-launched group stops after seven recorded successes when its owner
environment disposes the job, without an observed child failure status; it is
inconclusive, not another 40-run pass. Four real package cases in full regression
also pass. Neither these results nor absence of a matching host crash establishes
the historical Console connection refusal's cause. No startup retry, window
hiding policy, timeout increase or system setting change is justified. S6 remains
active with that fourth case unresolved; full qualification does not close it.

The next native-source review finds a useful experimental distinction in
Microsoft Terminal's [handoff selection](https://github.com/microsoft/terminal/blob/main/src/server/IoDispatchers.cpp):
STARTF_USESHOWWINDOW with SW_HIDE suppresses delegation, while an ordinary
visible initial Console can delegate. Its handoff failure handler can continue
with the original server. Therefore the earlier hidden-start result neither
proves a SoftPC fix nor identifies a failing handoff operation. The current
source's [VT startup](https://github.com/microsoft/terminal/blob/main/src/host/VtIo.cpp)
also has a connection-abort path when its pipes close during startup, but no
local failing stack identifies that path. These mutable upstream references
explain candidate boundaries; they are not a matched-source diagnosis of the
installed binary. No external code is imported.

Read-only host inspection identifies Terminal 1.24.12741.0 and an OpenConsole
COM server. The default command token has Low integrity; the unsandboxed user
token has Medium integrity, not administrative elevation. A serial same-source,
same-EXE package comparison runs both registered variants on each architecture
under both tokens: Low 4/4 and Medium 4/4, zero failed/skipped cases. The current
unchanged user configuration selects Console, and the fixture reaches its raw
DOS/input/restart assertions. Thus this comparison covers raw presentation;
neither restricted integrity nor early attachment alone is proved causal.

An attempted bounded native ETW capture cannot start: the initial repeated -p
syntax is rejected; the corrected provider-file form is denied by Windows
permissions before any package tests run. No trace or startup evidence is
claimed from those attempts, and no session remains active. The installed
event channels expose no corresponding Console host diagnostic channel. This
is a diagnostic limitation, not evidence that the historical server failure
has disappeared. Case 4 remains open; no retry, hidden-start shipping policy,
timeout change or privilege-dependent product behavior is added.

The owner permits UAC and an administrator ETW session starts successfully.
Only error-level Console/Terminal providers are enabled, with a 64 MB circular
cap. The separate ordinary-user test job runs both existing package variants
on both widths: 4/4 pass, no native startup failure. Named events delimit those
executions. This distinguishes diagnostic elevation from elevating the product.
The administrator's finally block attempts stop and delete, but logman returns
an unrecognized-provider GUID error and the ETL continues growing after that
job exits. The planned four-minute lifetime is therefore exceeded; the size
cap remains. A cleanup UAC is cancelled before process creation; a normal-user
fallback returns access denied (5). After renewed owner confirmation, native
ControlTrace reports query=0, stop=0, then query=4201 (session absent). Cleanup
succeeds and exports 1,329 records. No persistent data-collector configuration
exists. The failed logman cleanup and excess duration are disclosed, not
represented as a correctly bounded initial capture.

Initial error payload inspection finds E_FAIL at SystemConfigurationProvider
line 212 in both inbox conhost and OpenConsole, pipe-not-connected at the inbox
ConDrvDeviceComm line 147, and invalid-parameter at OpenConsole screenInfo line
1410. These occur during a trace containing passing package executions and
other Console activity; an error code alone is not a causal failed-start record.
The historical first connection refusal has not been captured in this trace.
Raw traces remain disposable research data, not checked-in evidence or inputs.

A second diagnostic corrects session retirement to that proven native stop/
query sequence. It samples only the historical compact Console path, alternating
architectures with an eight-run limit per width and stopping at the first
failed test. A temporary package-fixture PID record correlates each child;
no production code changes. All 16 executions pass. Native stop=0 and subsequent
query=4201 confirm normal session retirement, and the exported trace has 455
records. Its error-site inventory repeats configuration/icon E_FAIL, process
handle/pipe closure and screen-parameter diagnostics seen alongside successful
starts. No historical 0xc0074005 connection refusal is captured. Both fixture
executables are then rebuilt without the temporary PID probe, and the ordinary
and compact package registrations pass again on each width (4/4). These bounded
diagnostics supplement, not replace, the earlier complete 192/192 qualification.

Read-only retained System events identify 89 SoftPC 0xc0000142 popups, from
14:03:19 through 15:41:38 on 2026-10-22 (Pacific). Session events show remote
reconnection at 13:27:47 and transfer to the local Console at 15:45:51, with
no intervening recorded connection transition. No retained popup follows that
transfer. This is an environmental association, not proof that RDP causes the
failure. HKCU Console enumeration finds no SoftPC-specific application key;
it does not exclude every possible external configuration source. The owner
confirms current testing is on the desktop and will not be asked to switch
merely to obtain a failing result. Available desktop runs establish their own
passes, not a repair of the historical remote-session initializer.

The administrator permission/cleanup blocker is removed. The remaining case
has an evidenced native failure phase and healthy-current-environment evidence,
but still lacks the first server rejection cause and a justified repair. S6's
four-case exit criterion remains unmet; accepting an external limitation or
deferring this case requires an explicit owner decision. The three local fixes
are preserved, and no partial implementation delivery or completed S is claimed.

The following records the initial research/design before implementation.
Owner admits S6 audit/research/design at 898e4054 and advances from S5.
This report separates four cases; it does not claim a completed four-root-cause
investigation or authorize permanent production patches. Existing case 5's
fixture-oracle work remains assigned to S7. No public component change is
proposed for any of these four cases.

**HLT continuation and execution-plan fixture.** Current c_main.c enters
RESUME_HALT for a restored halted continuation and stays there until its
original RESET or enabled-INTR condition is met. The same loop handles an
ordinary live HLT. Debug writes in machine/debug.c change registers only;
they do not unwind the CCPU's live instruction phase. command_provider_smoke
resumes arbitrary post-reset/BIOS execution, pauses it, then execution_plans
sets EFLAGS=2 and CS:EIP=0:500. That is not proof of a fetch-boundary pause.
Historical capture of RESUME_HALT with executed=0 explains that timeout.
Do not make EIP writes secretly cancel HLT or fake a guest interrupt.
Minimal fixture design: before its first deliberate resume, use the existing
paused debug access to install/select a test-owned non-HLT loop with a marker;
observe that loop before the pause used by execution_plans. Keep a separate
existing-path HLT/interrupt test so deterministic fixture setup does not erase
halt coverage. This is test setup, not a new guest/production execution path.
Estimated permanent delta: one App integration fixture, +15..30/-0..10;
production/mirror zero. Counterfactual still needed before choosing exact code.

**TF quick-path invariant.** DO_INST's local softpc_host_debug_begin() return
goes directly to NEXT_INST. Unlike normal retirement at c_main.c:4439, this
edge does not clear quick_mode. The hook parks the executor and permits flag
edits. RESUME_FETCH resamples TF but only sets quick_mode TRUE when eligible;
it never assigns FALSE in the ineligible branch. Therefore TF can be sampled
as one while stale quick_mode remains true, and subsequent retirement skips
the ordinary trap path. Historical controlled recomputation evidence is
consistent, but this turn did not execute a new patched counterfactual.
Preferred minimum candidate at the already-local hook:

```c
if (softpc_host_debug_begin())
   {
   quick_mode = FALSE;
   goto NEXT_INST;
   }
```

This reuses the original slow/refetch path and leaves global quick eligibility
and original device logic unchanged. Estimated production +4/-1 including a
short reason marker; existing App debug tests +10..25 lines to make the
quick-to-paused-TF transition deterministic. No public API change. Compare
TF=0/1, interrupt shadow, ordinary resume and stepping on both widths; count
this as a local-hook mirror correction, not an upstream XP bug. Do not broaden
the repair to every NEXT_INST edge without evidence.

**Null BOP dispatch.** Current c_cpu_reset initializes F000:FFF0, then the
App cold-reset path calls original reset/device POST before reporting done.
That larger sequence is not equivalent to the CPU reset setter alone. Original
reset.c has explicit CS:IP restore branches gated by soft_reset, while the App
cold path sets soft_reset=0. Thus blindly clearing CMOS or forcing F000:FFF0
again after POST is not justified by the old bad-register capture. The first
bad reset-completed sample predates synchronous_access, so that fixture's later
register edits cannot explain that sample. No first corrupting writer is yet
identified. Next probe must record CPU registers/phase at CPU reset return,
original reset entry/return, host_reset and POST boundaries, then the first
executor fetch; preserve the earliest mismatch and its call stack. Only then
choose an owner-local correction. Estimated production diff: undetermined,
not a fabricated one-line fix. No null BIOS handler, retry or new CPU state.

**Pre-main Console startup.** package_smoke still launches the shipping child
with CREATE_NEW_CONSOLE; it subsequently attaches and hides only the Console.
Historical matched-symbol evidence places the rejection in KernelBase native
Console connection (0xc0074005), followed by loader rollback (0xc0000142),
before App/Lib code. It proves the failure phase, not the server rejection's
cause. Moving FreeConsole, SW_HIDE or debugging the server has not supplied a
complete causal repair. Next probe should preserve the ordinary launch mode,
compare successful/failed native connection returns with child/server lifetime,
and capture the first rejecting server operation without global registry or
terminal configuration changes. No shared Lib repair is justified. A fixture
change is acceptable only if its own lifetime/ordering violation is proven;
otherwise report the native environment limit rather than hiding/retrying it.
Production diff remains undetermined; shipped window behavior must not change.

Fresh bounded verification on unchanged published binaries: x64 and x86 each
pass five command-provider executions and three ordinary package executions,
16/16 total, zero failed or skipped testcases. Session is Active at launch.
This is repeated diagnostic sampling, not a retry-to-green qualification;
no compact-package, patched TF, controlled HLT or first-BOP-write experiment
ran in this turn. No historical intermittent case is declared repaired.
Hidden job 31636 exits; source, test, EXE, INI and media are unchanged.
Scratch build/t99-s6 is retired after promoting evidence. S6 remains open for
the two required counterfactuals and two unresolved first-failure captures;
the owner receives this bounded design, not a claim of completed repair.

### TODO audit and owner-admitted T99 expansion

On 2026-10-22 the owner requests an audit of every TODO: remove confirmed
resolved items and move all unresolved work into this T as S tasks. Frozen
universe: the eight TODO entries at 9ee4af66. Source/caller/history inspection
confirms four unchanged defect paths and four unresolved intermittent reports;
none has sufficient repair evidence to be classified resolved. This is a
read-only source/status audit, not a fresh hardware or failure reproduction.
All eight leave TODO by transfer, not by an assertion of successful repair.
The separate runtime-performance Queue candidate is unchanged. S5 remains
verified awaiting acceptance; only one S may become active at a time.

Audited dispositions and implementation sequence:

- **S6, private CPU/debug and native startup (existing plan).** Merge the high
  priority historical BIOS[0x52] null-dispatch TODO into existing case 3 below,
  without a duplicate task. Current first-invalid-reset and BOP evidence is
  stronger than the old report, but no permanent repair or first corrupting
  transition is proved. Preserve HLT, TF and pre-main startup cases 1-4.
  Existing case 5's test-oracle work is assigned to S7, not implemented twice.
  Receiver: App/CCPU/debug and native startup fixture, not public Lib.
  Original evidence: [T84](../history/M9-T84-S1-kvm-text-80x50.md).
- **S7, deterministic App test contracts.** Consolidate three TODOs: checkpoint
  aggregate timeout, INT 1Ah zero-byte oracle, and Win3.1 prompt-roundtrip
  readiness. CMake still gives checkpoint_smoke 15 seconds, with no diagnosed
  repair; its prior isolated passes do not close the parallel failure.
  machine_smoke.c still asserts bda_tick_low != 0 in run_int1a_tick_boot_image,
  although zero is legal. runtime_restart_boot_smoke.c still observes graphics
  for 15000 ms before menu interaction, without Program Manager readiness.
  Instrument actual phase/waits under parallel load; replace the proven invalid
  timer oracle with deterministic guest/IRQ evidence; establish observable
  readiness before changing the menu sequence. Preserve frame assertions,
  budgets and failure evidence; no blind retries. Cover existing S6 case 5's
  viewport/buffer read distinction and host-device attribution in this same
  fixture audit, without treating disconnected devices as product failures.
  Historical evidence: [T93 S8](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s8-repeated-stop-proof-and-third-qualification),
  T92 S2 timer failure and T96 S7 graphics sequence 36. Pass/fail repetition
  alone cannot establish a cause or justify closure of the intermittent cases.
- **S8, physical memory adapter correctness.** memory.c still routes named
  physical write/read through c_sas_stores/c_sas_loads. machine.c's public
  physical wrappers call these adapters; the direct physical Sound DMA fix
  did not repair them. Audit all callers and use the original physical SAS
  route, preserving SAS_VIDEO mapping, not a direct-RAM second implementation.
  Test nonidentity paging for both directions plus mapped video and boundaries.
  Retain T94 S2's observation that c_sas_PRS sees the actual DMA write while
  the old named-physical oracle reads another page. Owner: Compat memory.
- **S9, original VGA cursor-disable semantics.** vga_prts.c case 0x0a still
  reads the old cursor_off, updates the register only when start bits differ,
  then calls do_new_cursor whose shape branches set visibility true. T98 S6's
  port observation (start=0x20/end=15 remaining visible) therefore has no
  identified repair. Verify authoritative VGA/EGA and V7 write semantics,
  then propose a minimal single original-path mirror correction; test off-bit
  only changes, enable/disable, start/end and mode transitions through ports.
  Do not change public text-frame transport or assume this explains perceived
  cursor thickness. Mirror accounting and focused dual-width proof required.
- **S10, Sound Blaster MIDI parser boundary.** sndblst.h still defines 38h,
  but nt_sbdsp.c has no MIDI_WRITE_POLL handler and still treats 91h as
  DSP_WRITE_HS. The original header explicitly excludes SB-MIDI support.
  MPU-401 playback success therefore does not fix the observed SB command/data
  collision. Establish original command lengths/state and a correct supported
  or explicit unsupported disposition before implementing; no second audio
  path or silent expansion to full SB MIDI. Test 38h followed by MIDI data
  including 91h, subsequent valid PCM commands and busy/IRQ behavior.
  Evidence: [T95 S1](../history/M9-T95-win31-midi-wav-playback-proposal.md#paused-transfer-and-midi-follow-up).
- **S11, Ninja incremental build integrity.** The current reusable x86 log
  has a v7 header and recent builds succeed, but no captured damaged metadata
  or causal repair explains T93 S15's repeated premature-end recovery and
  large rebuilds. Cache relocation is not proof of repair. Preserve affected
  metadata if recurrence occurs, compare command/dependency hashes and no-op
  rebuilds under the normal serialized build flow. Do not clear caches first
  or create another cache manager. Establish normal incremental behavior and
  either repair a proved tooling cause or report a bounded unresolved host
  limitation for explicit owner disposition; do not declare fixed from absence.

These S tasks own all eight original entries: cursor->S9, SB-MIDI->S10,
physical memory->S8, Ninja->S11, checkpoint/timer/readiness->S7, BOP->S6.
No original evidence is discarded. Source inspection commands include searches
for cursor_off/do_new_cursor, MIDI_WRITE_POLL/DSP_WRITE_HS, physical adapter
callers, bda_tick_low, checkpoint registration and the 15000-ms graphics wait,
plus relevant path history and the existing S6 failure ledger.

Each S starts with current-source confirmation and a minimal diff estimate,
then owns its narrow implementation, focused dual-width proof, proportionate
regression/publication and actual-commit review. Mirror changes require the
existing source-policy accounting; no standalone branches or fake-success
fallbacks. Unreproduced failures must have an explicit remaining-evidence
disposition rather than an invented fix. T99 cannot close until all S and
these eight transferred entries have owner-accepted dispositions. This plan
does not launch S6-S11 concurrently or silently close S5.

### Current S5 quality audit and S6 transfer

#### S5 P6 final private-surface cleanup

Owner-requested complete shared-unit rerun at 40c2860e: both architectures
pass exactly 118/118 registered unit tests (Lib 73, Emulator 23, X86 Debug 12,
X86 IBMPC 10), including their static gates. Sorted executed names match each
architecture's independently queried CTest unit inventory; zero failures,
disabled/skipped tests or retries. x64 takes 70.09 seconds; x86 57.90 seconds.
Existing P6 binaries are reused, without rebuilding or changing production,
tests, EXEs or configuration. Native desktop/audio and App tests are outside
this requested unit universe. Hidden owned job 38432 exits zero; evidence is
promoted here before retiring its task scratch.

Owner approves the two residual audit items at eac62924. Move the Product
opaque typedef to composition.c (no public function uses it); remove the
static source-ID allocator's null-argument check. Its only production caller
passes the static counter and a local output; owner-local exhaustion tests
also pass real addresses. Preserve exhaustion, public initialization validation,
composition ordering and ownership checks. No test deletion or new abstraction.
Estimated source delta is a few removed lines; two source manifests change.
Frozen proof scope: dual cached builds/publication, component/composition and
shared boundary/manifest regressions. Unrelated native/audio/package tests
are not repeated for this declaration/private-precondition cleanup.

P6 delivery: production C/H +2/-5 (net -3); test C +3/-3 (net zero).
The complete caller scan also identifies the IBMPC entry stub borrowing the
old typedef; rename its local dummy to fixture_product, without changing any
assertion. Three source files and one test file change, with three corresponding
manifests. No App, mirror, INI, media or root test-infrastructure change.
Both cached full builds succeed and publish both EXEs without compiler warnings.
Each architecture passes 21 selected component/composition/manifest/boundary/
corpus tests, plus the separately selected IBMPC entry test: 22/22, no failures
or skips. These are focused P6 results, not a new full-suite qualification.
The initial sandboxed hidden launches exited without executing the script;
the authorized hidden job 22928 completes with exit zero, and scratch is retired
after confirming it exited. No test or build was terminated by a time budget.
Caller/actual-diff review confirms the sole allocator production caller supplies
valid addresses, and no production consumer names the private Product type.
Public validation, identity exhaustion and cleanup coverage remain intact.
Documentation governance and whitespace checks pass.
Published SHA-256: softpc32.exe
8D99A7F11BFB204C6A0178F81A46677D5D82C8C5F5A08DDFAC414AA5D8BB5546;
softpc64.exe
19B3FCB294D93F8DC9908083872E39E9E6DB7CAC0616A1503439468C22A89235.

#### S5 approved follow-up at 6a9bf601

Owner admits all five audit findings as a single bounded follow-up: remove
duplicate preconditions from implementation-private composition stages while
retaining real failure/ownership checks; delete unused composition.h; make
the component-local source identity allocator static; use char text buffers
and consistent booleans/direct returns; replace Machine fixture's Win32
synchronization with existing Base events/tasks and Types atomics. Keep thread
affinity and shutdown completion assertions, without a new production API.
Estimate: production net -25..-40 lines, test changes primarily mechanical
replacement across three Machine fixture files, with no new test framework.
The frozen verification universe remains all 192 Windows registrations per
width plus focused changed-owner tests; removing WIN32-only registration makes
the neutral Machine test available on Linux but does not claim Linux execution.
No App/mirror/media changes. Scratch and budgets remain those of CURRENT.

Follow-up implementation and qualification: all five items are complete.
Private composition stages no longer validate impossible caller order or repeat
the public run preconditions. Allocation, downstream failure and ownership
barriers remain. Remove only the two corresponding invalid-order assertions;
creation failure, destroy ordering, retained-owner failure and ordinary run
coverage remain. The source identity allocator is static; its existing boundary
test includes the owner implementation rather than exporting a test entry.
The unused forwarding header is deleted. UTF-8 buffers use char directly, and
presentation booleans/direct-return style are consistent at the audited sites.

Machine fixture events/tasks now use Base; counters use Types atomics. C11
thread-local fixture identity preserves executor-affinity assertions. An explicit
completion event plus task join replaces the native helper-thread wait, retaining
the blocked-final-callback proof without sleeps. The existing Machine test is
registered on every platform, not just Windows. No new production interface,
platform test framework, polling, retry or lifecycle state was introduced.

Against 6a9bf601, production C/H +19/-43 (net -24), test C/H +149/-155
(net -6), test CMake +2/-4 (net -2). Five public trees change: src/lib,
test/lib, src/emulator, test/emulator and src/x86-ibmpc. Other public trees,
App, mirror, INI and media are unchanged. Manifests are synchronized. No root
test-infrastructure changes are needed for downstream adoption.

Cached x64/x86 builds refresh both EXEs. Focused tests pass 14/14 each.
Full runs pass 192/192 each (184 background, three native audio and five
desktop/package), with exact registration/passing-name equality, zero skips,
failures or retries. Desktop session was Active. No non-mirror warnings occur;
preserved mirror warnings are not changed. This proves Windows dual-width
qualification, not an executed Linux or MSVC runtime qualification.

Self-review scans all allocator/header callers, composition stages and test
assertions, UTF-8 buffer uses, and Machine native-symbol references. No stale
header or native Machine fixture reference remains; exported lifetime contracts
from the preceding P remain intact. No in-scope delivery blocker was found.
The larger textual test diff is primitive substitution, not extra behavior.

Published SHA-256 for this follow-up:
- softpc32.exe: 0BE8C7308FB3BD94C4A9CD261FCC2B4483837DC13294764F26F59C58D8D20CEA
- softpc64.exe: EAC612787CFA5C5AE5DB16AD8E16854F8A97BD9A409236337D9B7381ABEB047E

After job exit, retire task scratch and preserve only reusable caches. The
follow-up awaits committed-diff review and owner acceptance; S6 is not admitted.

Coordinator review of pushed 29cf8a95 confirms all 22 changed paths match the
five-item admission. Public run validation and actual allocation/lifetime
errors remain checked; deleted assertions concern only impossible private
order. Machine callback-affinity, input ordering and join barriers remain
observable through neutral primitives. Allocator exhaustion coverage remains
owner-local. No old forwarding-header or Machine Win32 references remain.
Actual diff, manifests, artifact hashes and complete test-name evidence agree.
Documentation, whitespace and build-residue checks pass; S5 awaits acceptance.

#### Approved S5 implementation qualification

Owner approves the unified minimal repair after the audit delivery 164a5f70.
Frozen production scope: Broker root/native owner, PCM root/native creation
cleanup, Emulator UI and Product (six C files), with directly affected contracts,
existing owner-local fixtures and manifests. No Base, Types, X86, App or mirror
implementation change is admitted. Broker/UI destroy consume on ordinary return;
Machine/private owner failures and public PCM destroy retain their existing
contracts. No new lifecycle state, callback, recovery thread or retry is added.

Finite verification ledger: inventory both cached CTest trees after configure;
every registered name belongs to background, native_audio or desktop. Require
the complete union per architecture with no skipped/failing cases, plus focused
Broker/PCM/UI/Product tests. Expected universe is 192 names per width. Native
audio availability and desktop/package readiness are separately reported.
Retain initial failures and their dispositions rather than calling an unchanged
rerun a repair. All jobs use S5 scratch and CURRENT's finite budgets.

Source review checks: completed-reader I/O failure still closes its handle;
post-join cleanup continues and reports first error; failed creation releases
the process claim; unconfirmed exit terminates without atexit; UI consumption
propagates through both normal and failed-assembly Product paths. Protected
Machine/driver ownership barriers remain. Existing tests are extended rather
than new production test entry points or a second cleanup implementation.

Initial configure failed because the new child-process wrapper attempted to
register two existing CTest names twice. The registration now uses the existing
lifetime-script registration loop with each test registered once; no test name
is added or removed. This configure failure is not a product failure or pass.

#### S5 executor verification and self-review

Final cached x64/x86 builds publish both EXEs. Focused regressions pass 12/12
per width. Full qualification passes 192/192 per width: 184 background, three
native-audio and five desktop/package tests. Sorted actual passing names equal
the frozen registration inventory exactly (192 unique names each); no failure,
skip or individual-test retry occurs in this final run. Native tests run in an
Active desktop session. Initial builds report only preserved mirror-header
warnings; no public-package or other non-mirror compiler warning is present.

Against 164a5f70, `git diff --numstat` counts six production C files +58/-69
(net -11); including five contract headers, production C/H +72/-78 (net -6).
Seven test C files total +88/-49 (net +39); existing test CMake/fixture script
changes add +20/-5 (net +15). Documentation, manifests and two binary artifacts
are counted separately. Only Lib and Emulator source/test public trees change;
X86, App, original mirror, user INI and media remain unchanged. Root test
infrastructure does not change.

The retained path is stop, confirm worker exit, then independent cleanup.
An exited reader's I/O status does not prevent handle disposal. Post-join
flush/close/mode errors remain observable without retaining inaccessible
objects. Raw and cooked handles are closed by ownership, not the mutable output
alias. Broker/UI ordinary destroy returns consume their objects; Product clears
those pointers and continues independent cleanup. Machine shutdown/unbind and
private owner failures retain their existing dependency safety barriers; those
later errors take precedence over an earlier UI cleanup error. Public PCM
destroy ownership is unchanged; only creation rollback and the detached native
wrapper are repaired. Linux unsupported paths create no worker/object and stay
unchanged. No timeout, public signature, state, thread, retry or callback added.

Similar-issue sweep uses `rg` for all Broker/UI destroy callers and inspects
Broker create/replace/cancel/destroy, PCM root/native cleanup and Product normal
and failed assembly. All actual consumers are adapted. Existing Window/Console
worker ownership and Machine dependency barriers have distinct contracts and
are retained, not mechanically rewritten. Source review found no new in-scope
blocker. Tests exercise real worker retirement plus injected native failures;
isolated child tests verify non-returning fatal exit without atexit cleanup.
These are controlled fault proofs, not claims of naturally occurring OS handle
failure, Linux runtime qualification, or repair of private S6 intermittency.

Published SHA-256:
- softpc32.exe: F74CBF62E248D46D8F28004A67D904CBE0C78CBC3C79407A1C2FA75418A7C811
- softpc64.exe: BE3F330A1B9953FED8BD5FC623BDFE29608122D5139AA2740E2888A255FDF541

Owned qualification jobs have exited. Concise evidence is promoted here before
scratch retirement. S5 is verified pending committed-diff review and owner
acceptance; S6 remains planned and T99 remains open.

Coordinator review of pushed executor 55a08f0c inspects the actual 31 changed
paths, including production ownership transitions, consumed-object test doubles,
child-process fault registration, manifest and artifact changes. The change
matches the approved six-C-file scope; dependency barriers are not weakened,
test failures are not converted into success, and no new recovery path exists.
The full passing-name ledger and published hashes agree with recorded evidence.
Whitespace, documentation and build-residue checks pass. Task scratch is removed
after job exit, retaining the two reusable caches. S5 awaits owner acceptance.

The owner's latest direction supersedes the historical S4/S5 split below:
S5 now owns confirmed public-package quality issues; the former five-case
private research packet transfers intact to planned S6. No private evidence
is discarded and S6 is not simultaneously active.

Audit baseline is cf60e14f. This is source inspection, not a new dynamic
qualification. The bounded coverage ledger is:

- Lib source/test: inspect Types/Base boundaries, UTF-8 directory ownership,
  storage I/O, Console broker lifecycle and associated failure fixtures,
  PCM/MIDI ownership, dynamic frame storage/exchange and Window rendering.
  Confirmed finding is the broker cleanup issue below. Other inspected paths
  did not establish a new repair requirement.
- Emulator source/test: inspect Machine input admission/FIFO, Session output,
  UI destruction and Product composition; scan direct platform/outer-package
  references. No confirmed new defect; do not reopen accepted pause semantics.
- X86 Debug and X86 IBMPC source/test: inspect package dependencies, command
  adaptation and registration, scan outer-package/platform references. No new
  confirmed violation. This is not exhaustive xasm32 instruction verification.
- Whole-tree structural searches are candidate filters, not proof that every
  branch or edge case was exercised. No fresh unit/native tests were run;
  accepted S4's 192/192 per width remains historical evidence only.

**Confirmed P2: Console reader failure and cleanup ownership are conflated.**
In the native broker, retire_reader observes WAIT_OBJECT_0 but returns
reader_status before closing/clearing the reader handle. Thus a completed
reader with an I/O error repeatedly prevents deactivate/destroy from reaching
cleanup. Separately, deactivate can fail after successful retirement (flush,
stop-handle close or mode restoration). Root broker create treats every such
rollback error as an unquiesced reader and returns without releasing the
broker, binding or process claim. The output stays null, so the caller has no
object to destroy. This is a source-proven ownership defect; no claim is made
that it caused the owner's cursor or input symptoms.

Existing reader-failure tests prove notification, not cleanup after a completed
failed reader. Root broker smoke covers activation failure with successful
retirement, not activation failure plus post-retirement cleanup failure.

Minimal repair direction: retain one stop/join/cleanup sequence; use actual
exit evidence, not the worker's I/O result, to decide whether cleanup is safe.
Preserve the first error while completing independent safe cleanup. A genuinely
unconfirmed live worker remains a separate safety case; do not silently free it
or introduce a half-object/retry protocol. Establish its exact existing policy
before changing it. Do not alter retirement timeouts as part of this fix.
Reuse existing root broker and Win32 failure fixtures, including recreate after
a safely cleaned failed creation. No new public API or persistent state is
currently justified. Planning estimate: two production files +15..35/-20..45;
existing tests +30..60/-5..20, plus contract/manifest edits if required. This is
an estimate, not an implemented or verified diff.

No production, test, EXE, INI or media changes accompany this audit. Before
implementation, confirm the ordinary-error cleanup contract against all broker
callers and test the concrete failure chains. The audit does not justify a
general lifecycle framework or rewriting other public components.

Owner initially admitted T98 S16 against 8165664a, then on 2026-10-22 closed
T98 and immediately transferred this unimplemented research to new T99 S1.
CURRENT owns status. No S16 research completion is claimed; scope and evidence
transfer intact.
The aim is fewer competing responsibilities and states, not clearing TODOs
with individual patches. Review the public eight src/test trees: Lib,
Emulator, X86 Debug and X86 IBMPC. Actual App callers and fixtures are evidence
for attribution, not permission to repair App or the preserved core.
Do not read or modify downstream repositories.

S1 was research-only: no production/API or permanent test repair was authorized
by its original admission. The subsequent owner-approved S2 scope is below.
Temporary probes may exercise existing paths without a second implementation,
guest modifications or Computer Use. Further implementation options require
owner approval; S3 and S4 do not inherit S2's repair admission.

## Owner-approved sequence after S1

On 2026-10-22 the owner directs completion of the two definite cleanups first,
with pause-input semantics and intermittent failures investigated separately.
S1 research (29c260b5, reviewed in bf18a2f7) is closed by that advancement.

- S2: admitted minimal cleanup. Merge Broker deactivate/reset failure cleanup;
  delete the extra ResetEvent after auto-reset wait in the App fixture. Preserve
  first error, cleanup order, rollback, all timeouts and input semantics. Add
  reset-delivery failure coverage by reusing the existing retirement scenario.
  Estimate production net -15 lines; test net +5 to +20. Build cached x86/x64,
  publish dual EXEs, run focused plus complete background regression and the
  affected desktop group. Unrelated native audio is not required. Commit/push
  and actual-diff review precede owner acceptance. No repair of the historical
  intermittent failures is claimed merely by removing redundant code.
- S3: owner admits minimal cleanup after accepting that pause need not preserve
  ordinary host transitions or resynchronize keys on resume. Keep Machine FIFO,
  KVM filtering and Session key ledger unchanged. Remove only duplicate
  SOURCE_RETIRED admission and no-op paused zero-button mouse admission.
  Update the existing Session test, contract documentation and manifests.
  Estimate production C +1/-3, test C about +5; no API/state/thread changes.
  Run cached dual builds, publish EXEs and all 192 tests per width; commit,
  push, review actual changes and wait for owner acceptance.
- S4: owner-admitted after S3 acceptance. Investigate observations 2 through 5 using exact
  missing completion and child/input boundary evidence. Distinguish startup
  death, live blank output and post-start input loss. Do not add retries or
  shared recovery machinery before identifying the first failing operation.

S3 is owner-closed; S2 is closed by the prior advancement. The owner's latest
scope split retains public-package repair planning in S4 and transfers the
other investigation to the next S5 described below. S5 is planned, not active;
neither it nor S4 is a separate T-queue candidate.

## Owner-directed S4/S5 split

### S4: shared Console reader retirement

Owner now approves the two-site minimal production repair, without S5 scope.
Implementation baseline is 2e85ed53. Frozen qualification universe: both cached
Ninja trees' current CTest registrations (expected 192 per architecture), each
classified as background, native audio or desktop/package. Record actual names
before execution and require exact coverage, no skips and no failures for a
full-pass claim. Existing S5 failures retain their own disposition rather than
being concealed by test retries or scope expansion. Production estimate is
+5..10/-10..20 lines; existing tests +20..50/-5..15; manifests and records are
counted separately. Scratch owner is S4, build/t99-s4, with the packet budgets.

The confirmed shared-package candidate is the Win32 Lib Console broker's
cooked-reader retirement before raw input. The actual product failure loses
input before the broker's raw delivery; independent native experiments reproduce
the loss with either cancellation API. The terminating-record/join control
retains all input in 10,000 cycles. The detailed counts below remain evidence,
not a qualified production fix.

Plan the minimal repair at the existing native reader owner: reuse the existing
terminating-record wake and join, removing cancellation only if the actual
reader lifecycle proves it sufficient. Do not add a reader, retry, timeout,
input replay, Machine/Session admission state or application-specific branch.
Before implementation, verify empty and partially edited cooked lines, completed
lines racing retirement, raw-reader retirement, wake failure, shutdown and
rollback. Preserve real error reporting and join before releasing reader context.
Keep ordinary text/echo and line-completion behavior unchanged; a wake record
must not become a user command or leak into the next raw reader.

Retirement and the reader are both private to
`src/lib/console-broker/win32/console.c`; Base owns neither operation. The two
cancellation wrappers have no other production consumers in Lib. Root broker
serialization excludes new request/activation until retirement ends. Raw reading
already waits on stop and input together; cooked reading needs the existing
terminating-record wake. No Base or public API change is currently indicated.
Companions are existing Lib broker contract/integration tests and manifest.
Qualification must include both architectures, controlled cancellation/retirement
regressions and the actual raw package handoff. Unit proof and native desktop
proof are separate. Other public packages remain unchanged unless a concrete
call-site dependency is established. No production repair is implemented yet.

#### Real-reader counterfactual and long-line discovery

A task-private probe includes the actual native broker implementation and real
Console I/O, using cached Lib dependencies. It cycles empty cancellation,
partial-line cancellation, over-capacity cancellation and a completed `ok` line.
Only the candidate's two native cancellation calls are removed initially;
wake, reader, join, pending state and flush remain the real implementation.
After the oversized line, the next delivered command is 33 old `x` characters,
not `ok`. The original unchanged native broker also fails the same scenario,
delivering an old long-line tail. This is not merely a mock-specific failure.

Source explains the separate defect: ReadConsoleW can return a fragment of an
already completed native line. The reader checks stop and returns immediately
after that fragment, before reaching LF; FlushConsoleInputBuffer does not
discard the native line's unread fragment. Thus thread exit plus input-record
flush does not prove that the cooked line has been fully retired. This is the
same ownership boundary as the input-loss candidate, not another input path.

The second private candidate moves the existing stop check after the existing
through-LF read loop, before UTF-8 conversion/event delivery, and removes the
two cancellation calls. It neither enlarges line storage nor delivers an
abandoned line. The x64 and x86 real-reader tests each complete all 400 cycles:
100 completed lines per width, no abandoned-line event, no I/O failure, no
pending input record, ordinary join and cleanup. Both jobs exit zero. This is
local controlled proof, not shipping or complete
package qualification. An initial probe compile incorrectly compared a void
release result; it was corrected before runtime evidence was collected.
The x86 launch initially misread the CMake cache compiler property's type;
correcting that task-only script enabled the recorded x86 build/run.

The minimal production candidate is one file: remove two calls, relocate
the stop check, and correct nearby cancellation comments (roughly +5 to +10 /
-10 to -20 lines, net reduction). Reuse current unit scenarios with wake-driven
completion and add native long-line retirement coverage to the existing
integration owner. Test estimate is about +20 to +50 / -5 to -15 lines; actual
accounting is required before delivery. Wake failure, delayed reader entry,
completion races and raw handoff require targeted proof. No existing timeout
or fatal policy changes are proposed.

#### S4 approved implementation and verification ledger

The actual production patch is +6/-13 (net -7), confined to the native broker:
remove both cancellation calls, move the existing stop observation past LF,
and update two adjacent comments. No API, state, timeout, allocation or thread
is added. Existing transaction serialization, stop wake, join, completion
observation and post-join input flush retain their owners and order. Raw-reader
stop remains its existing event wait; read errors remain on its existing path.

The existing worker cancellation unit is +31/-11 (net +20). Its wake mock now
uses the actual terminating-record route rather than cancellation mocks. It
checks an over-capacity two-fragment line is fully drained without delivery,
a completed command survives retirement, repeated cancellation is harmless,
and failed wake reports IO_ERROR while the worker/context remain owned. The
fixture explicitly releases its blocked read and joins before freeing context;
this introduces no production retry. Existing deactivate-flush failure coverage
is preserved. All shared values are read after event/join synchronization.

Final test placement is narrower than the research estimate: retain native
long-line evidence above and make its deterministic regression permanent in the
existing worker unit. The display integration deliberately replaces reader
threads with signaled events; adding a second reader mode there would conflate
its geometry fixture with input ownership. Existing actual package integration
supplies native handoff regression; no duplicate integration runner is added.

Similar-issue sweep: search `cancel_io_ex|cancel_synchronous_io|read_console_w`
under src/lib and test/lib/console-broker. The broker is the only production
read/cancellation owner; removed calls have no remaining production consumer.
Types retains its legitimate one-to-one SDK declarations. Reader-failure and
fragment/UTF-8 tests use the same source inclusion and remain in qualification.
Normal read, raw stop, replacement/rollback and destroy use the same retirement
helper; no other path or public package needs alteration. S5 is not implemented.

The first task runner's focused regex used a slash where registered names use
a dot, selecting zero tests. That invocation is not passing evidence; full
qualification still includes every registered test, and the corrected focused
invocation uses `--no-tests=error`. No production or test budget is enlarged.

Final implementation qualification on 2026-10-22: both cached ccache/Ninja
builds exit zero, 33 incremental steps each, with no compiler warning/error.
Both published EXEs are rebuilt from this source; no user INI/media is touched.
The corrected focused groups pass 7/7 per architecture (0.33s x64, 0.37s x86).
The full run passes on its first execution, without retries:

- x64: background 184/184 in 122.03s; audio 3/3 in 2.54s;
  desktop/package 5/5 in 14.27s.
- x86: background 184/184 in 114.89s; audio 3/3 in 2.86s;
  desktop/package 5/5 in 13.78s.

Each width's combined JUnit names exactly equal its 192-name frozen registry,
with 192 unique cases, no missing/extra names, failures or skipped cases.
Audio claims remain those of each test: default MIDI open/release alone is
not audible-playback proof. Normal and compact package paths pass on both
published products. Session was Active at launch. Historical S5 intermittency
is not declared repaired by this successful run.

Published SHA-256:

- softpc32.exe: CFD828584B0CB8F9BBB54593E5482FA7C599487B52CAEC9D58DBBCCE4C13619E
- softpc64.exe: E66A8962E3362CF2A0399B93DE4BE30E79DC4301B0061774A9C8D00E73172B35

Executor self-review checks the actual production/test diff against the two
approved corrections: no second path, polling, new timeout/state/API, changed
dependency or product policy. The existing bounded native line buffer and
join-before-release contract are unchanged. Test cleanup waits for the reader;
failed wake does not masquerade as completion. Both manifests and all corpus,
Types, naming, dependency and documentation gates pass in the full run.
Counted C paths are precisely the two files above; relative to 2e85ed53,
`git diff --numstat -- src test` reports production +6/-13, test +31/-11,
and manifest hashes separately +2/-2. Original mirror diff is zero. Only Lib's
src/test pair changes among the public eight trees; other shared owners are
unchanged. Both binaries and task documentation are separate from C counts.
All jobs must exit before task scratch retirement; S4 waits for acceptance.

Coordinator reviews pushed executor 52d8fcc3, including actual C, manifest,
documentation and binary differences: the eight changed paths match the packet,
tests preserve successful completion and failure ownership, and no extra public
contract or alternate input path exists. The complete name ledger and artifact
hashes above match verification. All owned jobs exited; disposable scratch was
removed after evidence promotion. Documentation, whitespace and residue checks
pass. This accepts the executor delivery for owner review, not S4/T99 closure.

Follow-up controlled tests now pass on both widths using the existing cancellation
fixture with the candidate reader and wake-driven completion. They preserve
already committed lines, discard cancelled fragments and verify a failed wake
returns IO_ERROR while the reader/context remain alive. Fixture-controlled
completion then permits safe join; this is not a new production retry protocol.
The builds use Wall/Wextra/Wpedantic/Werror without diagnostics.

A private x64 product links only the candidate broker object ahead of the
existing archive; all other cached production objects remain unchanged. The
actual package fixture, pointed only at its private raw-Console/overlay
configuration, passes 40/40 complete compact runs, including initial `ver`,
pause, stop and restart. All attempts are counted; none is retried or waived.
This supplements, but does not turn into, full dual-product qualification or
proof that S5's pre-main initialization failure is fixed. The shipping x64
hash remains AFA8D42BC1C3C0B21DCBD30527C4D6DCC054E40914946F74E575700416A88D94.
Temporary tracked command-provider diagnostics were removed and both cached
test executables rebuilt successfully. Production and test tracked code are
unchanged. All task-owned jobs exited before evidence retirement.

### S6: private CPU/debug and native startup investigation (transferred from S5)

Originally admitted as S5 at c5d6cf64, now transferred intact to planned S6.
This carries current
evidence forward rather than restarting the investigation. The five entries
below are the finite research ledger: each needs source ownership, evidence
class, first-bad-transition findings, candidate scope and explicit unknowns.
Prior passing experiments are not new runs. No permanent fix is authorized.
It must separate these cases:

1. **HLT instruction-plan timeout (App/CCPU/debug fixture).** The failed executor
   remains at RESUME_HALT after the fixture changes PC and clears IF. No trace
   instruction retires, so PAUSED cannot complete. Establish a deterministic
   valid fixture entry state and preserve the documented architectural HLT
   contract; do not infer that public Machine event delivery is broken.
2. **TF quick-path invariant (local CCPU debug hook).** The debug-begin jump
   bypasses the original quick_mode clearing invariant. The private
   recomputation experiment passes 60 runs and observes the stale TF case.
   Audit the smallest original-path correction and mirror accounting before
   proposing implementation; no public X86 Debug API change is established.
3. **x86 null BOP dispatch (private machine/boot execution).** Timing probes now
   capture actual AVs, not merely a timeout: one executes BDA bytes at
   0000:046f and dispatches BOP 0f; another reaches physical f1c00's BOP 52
   with CS=e7f4. Both reach an empty BIOS slot. In the latter run the first
   reset-completed register read already shows CS:IP=0000:5ce1, before the
   synchronous debug mutation sequence and first resume. This moves the next
   investigation to reset/bootstrap's first invalid transition. It does not
   prove that small fixture media, reset itself or a shared data race caused
   it. The last-instruction ring proves the eventual dispatch, not the first
   corrupt transition. Do not install a no-op BIOS handler. Cross-reference the
   historical BIOS[52] TODO without claiming its entire history explained.
4. **Pre-main native Console startup.** Matching KernelBase symbols locate
   0xc0074005 at ConsoleCreateConnectionObject's NtCreateFile after successful
   server launch; loader rollback surfaces 0xc0000142. Owned-server observation
   sees Windows Terminal OpenConsoleProxy loaded. All 40 starts under server
   debugging succeed, so that instrument perturbs reproduction. A separate
   SW_HIDE-start counterfactual has 20/20 successful native initializations,
   but only 16/20 complete fixture successes; the four later failures were not
   attributed and cannot be counted as repaired. Terminal delegation is a
   lead, not a proved server-side root cause. Compare normal-start reversal
   and obtain the first server rejection before suggesting a host/test change.
   This occurs before shared Lib or App main, so it is not assigned to S4's
   broker repair. Do not hide the shipping window or add startup retries.
5. **App fixture oracles and environment attribution (transferred to S7).** Preserve the demonstrated
   viewport-versus-buffer linear-read limitation and the already tracked legal
   zero timer-byte assertion. Keep disconnected PCM/MIDI composition failure
   separate from native loader failure. Repair only proven test-owned defects;
   an online rerun is not a repair of unavailable host devices.

S6 must report source inference, controlled counterfactual and real failure
captures separately, retain existing assertions until their contract is proved
wrong, and request approval for permanent private/mirror repairs. Do not expand
into work outside the owner-expanded S6-S11 allocation or downstream repositories.
The following asset restriction describes this research transfer, not later
owner-admitted implementation and publication: no guest, shipped EXE,
INI or media changes are part of this transfer. The completed diagnostic jobs
are stopped and S4 scratch has been retired. Future S6 diagnostics need an
admitted CURRENT packet and bounded scratch policy; publish concise evidence
and remove disposable files, rather than retaining raw logs permanently.

#### Historical S5 admission source recheck (retained for S6)

At admission, current source still matches the earlier captures. In
`c_main.c`, a retained halted entry goes directly to RESUME_HALT; ordinary
PC/register edits do not change that entry. The actual command-provider test
resumes the booted machine, requests pause, then execution_plans clears IF and
sets CS:EIP to 0:500. These operations do not establish that the CPU was parked
at a fetch boundary. First research target: establish a deterministic fixture
entry without changing architectural HLT or creating a second resume path.
This is source confirmation, not a newly reproduced timeout.

The debug-begin hook's return jumps to NEXT_INST. Its implementation explicitly
permits paused register/code edits before returning. At RESUME_FETCH the CPU
resamples TF, but the eligibility block only assigns quick_mode=TRUE, never
FALSE. Thus the existing local hook can bypass the original clearing invariant.
Compare clearing at that newly introduced return edge against broader changes
to original recomputation; prefer the narrower hook-local correction if all
entry paths and the controlled TF test prove it sufficient. No patch is made.

Next evidence targets are a controlled HLT/fetch fixture comparison, a TF
return-edge counterfactual on both widths, the first invalid bootstrap register
transition for the BOP case, and native startup before application entry.
No new dynamic test, all-case root-cause claim or S5 EXE publication is reported
at admission. Current production and tests remain identical to accepted S4.

## S4 bounded investigation report

Baseline 8072abdc; owner closes S3 and admits root-cause research. This report
does not complete the root-cause objective: S4 remains active. No production,
permanent test, public manifest, package EXE or user configuration is changed.
Temporary probes and raw logs are retired after promoting these results.

The finite investigation ledger keeps four distinct cases:

1. Command-provider wait: 40 unchanged isolated repetitions per width all pass.
   The existing single wait_for assertion is shared by running, paused, stopped
   and program-marker waits. Historical completion=2 does not identify which
   next wait failed. The fixture directly feeds runtime notifications and does
   not exercise the complete Session event loop. Its program marker is observed
   through the executor copy_frame callback. No causal proof of a lost wake,
   delayed callback, driver hang or fixed error is available. Receiver remains
   this fixture/executor boundary; next discriminating evidence is the call-site
   identity and producer signal sequence under aggregate load, not a longer
   deadline or a second completion mechanism.
2. Pre-prompt composition failure: previous disconnected experiments established
   unavailable native PCM/MIDI endpoints. Source chain is main -> compose_machine
   -> vm_create -> softpc_platform_audio_start -> PCM stream acquisition; failure
   is returned before monitor assembly. This explains those recorded exit-1
   cannot-compose-machine executions, not every blank screen or loader exit.
   No audio fallback or startup policy change is authorized by this research.
3. Loader/init exit: online x64 ordinary package passes twice then fails on its
   third attempt; compact passes once then fails on its second. Both failures
   report stage=1/error=6 and child exit 0xc0000142. x86 ordinary and compact
   each pass all 12 bounded repetitions. Stage 1 is the failed GetConsoleCP
   check after attachment attempts; it is not evidence that Broker restoration
   failed. A separate scratch copy using the unchanged fixture and shipping
   x64 EXE, Console mode and overlay media, passes odd attempts and fails all
   six even attempts out of 12 with the same loader exit. This also excludes
   CTest as a necessary trigger. No failure reached raw-input stage 14.
4. Post-boot raw input: current user INI selects Window. Both registered package
   names therefore take the Window branch; the compact name alone does not
   prove raw Console input coverage. The scratch Console configuration exercised
   the real raw route, including ver, in its six successful starts. There is no
   reproduced post-boot loss in this run and no evidence tying it to the loader
   failure. Startup failures remain failures, not waived raw-input passes.

Additional discriminating startup experiments:

- A process-local Win32 debug-event collector enabled loader diagnostics only
  in its debugged processes (no IFEO/registry change). Six direct original
  package-fixture executions and 12 original CTest executions all pass under
  that instrumentation. Thus the failing initializer is still unidentified;
  debugger timing is not an adequate negative proof. DLL export/probing warnings
  also occur in successful runs and are not assigned as the root cause.
- An independent native program creates a child Console, attaches, waits for
  a named event proving child main was entered, detaches and terminates its own
  child. Twenty repetitions pass. Adding the same six native DLL imports as
  SoftPC passes another 20; adding immediate Console hiding passes another 20.
  This does not reproduce the full product startup and cannot exonerate all
  attachment timing, but disproves an unconditional failure of those operations.
- No matching Application-log failure record identified a DLL. No source-level
  TLS/DllMain implementation was found in the repository. The loader status
  narrows the phase; it does not establish a particular module or OS defect.

Source audit also confirms package_screen_contains reads viewport area as a
linear buffer count, although native linear reads wrap at backing-buffer width.
For differing widths this is not a rectangular viewport oracle. This is a
real fixture limitation, but stage 14 uses package_buffer_contains instead;
it does not explain the recorded raw ver failure. Keep any repair test-owned.

The raw fixture's first scratch link omitted existing Base clock/sleep symbols
and failed; linking the existing cached Base archive fixed the probe build.
No source was rewritten to obtain a pass. All reported runtime counts above
are from the linked probe. This run is research, not a fresh full qualification.

Recommendation: do not alter shared lifecycle, pause or Console recovery paths
on this evidence. The next bounded step should capture the specific initializer
on the uninstrumented failing startup and name the exact command wait under
aggregate load. There is no justified minimal production fix yet. Preserve
separate dispositions for live delayed prompts, exit 1, exit 0xc0000142 and
post-boot input; only the exit-1 native-endpoint chain is explained here.

### S4 continued failure-site evidence

These are additional active-research observations, not a completed repair or
full root-cause closure. Temporary fixture diagnostics and task-local probe
objects remain outside any downstream import baseline.

- The real command-provider first execution plan timed out again. A debugger
  attached only after its ten-second wait failed. Its executor stack is in
  `ccpu`, `c_main.c:4065`, the `RESUME_HALT` checkpoint loop. The live debug
  state is TRACE, target_count=3, executed=0, stop_pending=false,
  result_ready=false, in_instruction=false; quick_mode=0 and interrupt_map=1.
  The test's preceding requests set CS:EIP=0:500 and EFLAGS=2. Thus the CPU
  retained its pre-pause HLT continuation while debug changed the next PC and
  cleared IF. The HLT loop does not reach the ordinary debug begin/retirement
  hooks; neither its enabled-INTR nor RESET exit condition is satisfied.
  This explains this reproduced PAUSED timeout, not every historical wait.
  It is not an event-wait deadlock or an undersized ten-second budget.
- Separately, x64 trace with guest TF set sometimes returns IP=702 rather
  than taking INT1 to 700. The local debug-begin hook jumps to NEXT_INST
  without clearing quick_mode; the original resampling code only sets it
  true when eligible, relying on earlier callers to clear it. A task-local
  counterfactual clears it before recomputation: 60 iterations pass and one
  records stale quick_mode with TF set at IP=701. This is evidence for a port
  hook violating the original quick-path invariant, not proof that it also
  repairs the distinct HLT continuation problem. No mirror change is made.
- Parallel x86 command-provider also raised an access violation before the
  execution-plan section. Fifty runs under a debugger pass. Its root remains
  unproved; debugger-altered timing is not an exoneration.
- The unchanged x64 package fixture reproduced stage 1 with a still-live
  child, not just an exited child. Post-failure attachment shows the sole
  ordinary thread in ntdll's loader initialization failure path, invoking
  ZwRaiseHardError with STATUS_APP_INIT_FAILURE (0xc0000145) and parameter
  STATUS_DLL_INIT_FAILED (0xc0000142). Only ntdll is in the loaded module list.
  Matching Microsoft ntdll symbols identify LdrpInitializationFailure,
  LdrpInitialize, LdrpInitializeInternal and LdrInitializeThunk. This proves
  that this live/no-Console sample is a native initialization failure before
  SoftPC main, not application monitor delay. Which earlier native operation
  returned the error still requires evidence. Merely extending attachment
  timeout is not a demonstrated correction.

Further native evidence narrows this last case: the failed child's existing
LdrpSnapsUnicodeString log explicitly reports KernelBase.dll's process-attach
initializer returning failure, then Kernel32 load failure, then process
initialization failure. Read-only observation catches BasepInitializationStage
at 600. Matching KernelBase symbols/disassembly place stage 600 immediately
before ConsoleInitialize; its false return exits the DLL initializer without
advancing to stage 700. Thus this sample's native Console initialization is
the failed operation, not native PCM or SoftPC main. VirtualQueryEx confirms
only EXE/ntdll remain mapped after loader rollback; an empty later DLL list
does not mean KernelBase was never mapped. The trigger of ConsoleInitialize's
failure remains under investigation. A task-local A/B keeps the same EXE and
fixture but moves the parent's FreeConsole before versus after CreateProcess;
results must be inspected, not presumed. KernelBase symbols match RSDS
79B505276F140450EB83DF0440BF06C6 age 1.

The FreeConsole-order comparison is now complete: the original order fails
one of ten starts, while releasing before CreateProcess fails three of ten.
Moving that call alone is therefore not a demonstrated fix. A separate
15-start diagnostic retains the parent's Console and does not AttachConsole
until the child advances beyond native Console initialization. Two starts
still fail at initialization stage 600 with the same KernelBase initializer
failure. This rules out early parent detach/attach as a necessary trigger in
these samples. Its later stage-3 prompt report observes the retained parent
Console, not a running child; the native loader evidence is the useful result.
The failed primary thread's final LastError is 6, but that stale value alone
does not identify the originating native API. A minimal Console child that
immediately returns, with no SoftPC code or attachment, passes 60/60 starts;
this does not reproduce the package lifecycle or exonerate its native path.
Raw compact-input experiments now use a private overlay configuration so the
real raw route is exercised without changing the user's Window configuration.
Those results remain pending; no passing Window route is counted as raw input
coverage.

The unchanged raw compact fixture now reproduces the historical post-boot
failure: attempt 47 reports stage 14, a live child, and a fresh DOS prompt
with no characters from `ver`. Of these 47 attempts, 39 pass the complete
route, seven fail native startup, and one fails post-boot input. This is a
separate reproduced failure, not a window-mode result or a guessed input
cause. A subsequent task-local fixture attaches the debugger only after
failure to capture the actual blocked threads. The private package executable
hash matches the shipping executable; only its private configuration selects
raw Console with overlay media.

An additional minimal native-only experiment reproduces parent detach,
child Console attachment, hiding and forced child termination, but passes
60/60 cycles. Along with the immediate-exit child experiment, this shows that
these simplified lifecycles have not reproduced the package initialization
failure; it does not prove that native initialization is sound under the full
package lifecycle. Separately, 500 consecutive real provider resume/debug-pause
cycles complete without access violation; the following instruction-plan wait
times out again. The access violation still lacks a captured faulting stack.

The 60-start raw post-failure-stack experiment completes with 40 successful
routes and 20 native-initialization failures, but no new stage-14 input failure.
This does not supersede the reproduced stage-14 sample above. Input-chain
diagnostics now instrument task-local copies of the broker, KVM Console,
Session and App keyboard adapter. Their private link reuses existing objects,
does not replace shipping executables, and writes only to task scratch. A
lower-perturbation variant records fixed-size in-memory observations and reads
them only after failure, avoiding per-event file I/O. It also records original
keyboard-controller disable, scanning and buffer facts; no controller behavior
or production input routing is changed. Results remain pending.

The file-observation input-chain experiment reproduces stage 14 on attempt
80. PID 16656's raw reader observes only Enter release, not the three letters
or Enter press. The adapter decodes that release correctly; the unmatched
release is not delivered as a guest make. No App keyboard delivery occurs.
The native reader subsequently waits for input, Session waits for its queue,
and the guest executor waits normally in BIOS keyboard idetect. Thus this
sample loses the command before Broker native key delivery, not in Machine
FIFO, Session admission or the guest keyboard adapter. The native cause
(flushing, another reader or host input handling) remains to be distinguished.

A native-only controlled experiment now reproduces the exact missing-input
shape without SoftPC, Session or Machine. It starts a cooked ReadConsoleW,
cancels through the two existing native cancellation calls, joins the reader,
flushes input, switches to raw mode and successfully writes all eight records
for `ver` plus Enter. On cycle 470, only Enter release remains. A fresh repeat
reproduces on cycle 388. In both failures ReadConsoleW returned TRUE with zero
characters and ERROR_OPERATION_ABORTED; the reader thread had exited before
new input was written. Thus joining the application reader does not prove the
native cancelled cooked-read state can no longer consume future input.

The counterfactual removes only the two cancellation calls, retaining the
existing terminating-key wake, join, flush and raw transition. All 10,000 cycles
retain all eight key records. This is dynamic causal evidence for cancellation
in this native probe, corroborating the real product's pre-Broker loss. It is
not qualification of a permanent broker repair or every terminal implementation.
The native observation is consistent with Microsoft's public Terminal issue
[17791](https://github.com/microsoft/terminal/issues/17791), which reports a
successful cancelled ReadConsole and discarded first subsequent input. The
independent local experiment, not that issue alone, supplies the evidence here.
Per-call isolation remains useful before choosing the smallest correction.
That isolation now also reproduces with CancelIoEx alone (cycle 220) and
CancelSynchronousIo alone (cycle 203), each leaving only Enter release.
Replacing one cancellation API with the other is therefore not a supported
correction. The existing terminating-record/join route is the useful minimal
candidate; its product-level output/editing behavior still needs qualification.

A one-shot debugger observes ConsoleInitialize's ConsoleAllocate return in
40 private package children. Every observed status is success; each child is
then terminated before product execution. This deliberately limited experiment
does not reproduce the native initialization failure or the full package
lifecycle, and cannot close that root-cause item. All these diagnostic children
and the cancellation isolation/control jobs have exited.

The x86 fixture's exact prefix through its first resume and second pause now
runs in 1,000 fresh processes. All pass; later attempts overlap an eight-way
Machine/unit stress group. The prefix retains its original register/memory,
port, command-open/close and pause calls, then uses ordinary owner teardown;
only subsequent execution-plan/snapshot sections are omitted. This separates
the historical access violation from the known later HLT/TF assertions, but
does not prove the violation repaired. Source review confirms debug cancellation
is queued atomically and applied on the executor, not a direct frontend write
to the driver's debug state. No unsupported shared-state-race claim follows.
The supporting stress group remains active until its bounded repeats finish.
Read-only System log inspection finds the native 0xc0000142 popup records but
no Win32k or Resource-Exhaustion event in the preceding twelve hours. This is
not proof that resource pressure is impossible or the initialization root cause.

The startup debugger now retains the actual complete package fixture lifecycle
instead of terminating immediately after successful native initialization.
Its second start observes ConsoleAllocate returning 0xc0074005; subsequent
starts reproduce the same return. The child never reaches main in those
failures. This provides the first live originating status beneath the loader's
generic 0xc0000142. A serialized follow-up observes the individual allocation,
server-launch, connection and standard-I/O returns in the same native chain;
its results must be checked before attributing that status to a particular call.
The one-shot breakpoints affect only private diagnostic child memory and are
removed before each observed instruction executes; no DLL on disk is changed.

The complete-lifecycle experiment finishes 40 starts: 22 native allocation
successes and 18 failures with 0xc0074005. The serialized deeper observation
then captures one success and one failure. In the failing child, both initial
NtCreateFile operations and ConsoleLaunchServerProcess return success;
ConsoleCreateConnectionObject returns 0xc0074005, propagated unchanged through
ConsoleAllocate. Matching local disassembly shows the connection routine
obtains its failure from its NtCreateFile request; allocation failure would
instead return 0xc000009a. Standard-I/O creation is never reached in that
sample. This establishes the failing client/server connection operation before
main, not why the native server rejects it. No product retry or timeout change
is justified by this evidence. Both native diagnostic jobs have now exited.

The supporting x86 eight-way repeated Machine/unit group completes in 609.15
seconds: 30 of 31 registrations pass; machine_smoke fails its existing
run_int1a_tick_boot_image assertion bda_tick_low != 0 (exit 0xc0000409).
This is the already-recorded host-time oracle debt in TODO, not a new access
violation: the fixture repeatedly executes INT 1Ah, whose original time_of_day
overwrites TIMER_LOW from get_host_timestamp. A zero low byte is legal.
The failed timestamp was not captured, so no timer-delivery regression is
inferred and that test-only repair remains outside this S.

The HLT finding also requires a contract distinction: Product UI explicitly
preserves HLT until an interrupt. The fixture resumes firmware before pausing
and installing its instruction-plan program, so a retained HLT continuation is
a possible entry state. Editing PC and clearing IF does not itself provide an
architectural interrupt. The timeout is explained; automatically clearing HLT
in production is not yet an approved or demonstrated correct repair. A repair
must distinguish deterministic fixture setup from any separately intended
debugger override of halted CPU state.

The x86 first-chance exception-observed prefix adds 200 successful fresh runs.
No access violation is observed; neither a captured crash nor a repair is
claimed. The debug cancellation call-chain review remains negative evidence,
not a substitute for the missing fault site.
This 80-attempt group has 61 complete passes, 18 startup failures and this
one input failure. Its private executable is instrumented; it is not a claim
about unmodified production timing. Shipping hashes remain unchanged.

The loader symbol file is the public Microsoft ntdll.pdb matching RSDS
C031D45B0DA18A6E74D06ED6AB2DD9E1 age 1. Symbol lookup corrects GDB's nearest
export names; no OS registry, system binary, downstream source, user INI or
guest media is changed. The documented status meanings are in Microsoft's
[NTSTATUS reference](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55).

## S3 implementation and verification record

Baseline 22cdeb7f. Production C changes only session/control.c: +1/-3,
net -2. The existing control_reconciler test adds five lines, net +5.
No ABI, state, thread, queue, driver callback or resume synchronization is added.
Only src/emulator and test/emulator change among the public eight trees;
their manifests and the architecture/contract documentation are synchronized.
The other six trees, Machine implementation, App, mirror, INI and media remain
unchanged. Dual published EXEs are rebuilt from this implementation.

Similar-issue sweep inspected accept_kvm_event and dispatch_input for every
KVM event type, plus Session callers and existing Machine/physical-key tests.
SOURCE_RETIRED and INPUT_RESET already enter before state/generation filtering;
their ledger cleanup remains necessary. Paused key-up also clears the local
ledger without guest delivery. Paused mouse has no ledger or dispatch action;
its redundant admission is removed. Hotkeys and close retain control semantics.
Machine admission/FIFO and KVM filtering remain their existing single owners.
No additional gate or abstraction is warranted for these two predicates.

Cached x64/x86 builds each complete 23 incremental steps, no warning/error
matches in their logs. Focused Machine/Session tests pass 14/14 per width.
Full background runs pass 184/184 each (113.70s x64, 114.62s x86).
Native tests were held while the owner session was disconnected, then executed
after reconnection: audio 3/3 and desktop/package 5/5 on each width, no skips
or failures. No native test was attempted during the disconnected interval.
Frozen qualification universe: all 192 registered CTest names per width.
Exact registration/XML name equality across background/audio/desktop proves
192 unique passing cases per width, zero missing/extra/failed/skipped cases.
This proves this build's regression result, not repair of historical S4
intermittency or acceptance by other products. No downstream repo was read.

Published SHA-256:

- x86: EB924BBA391EB0F88E989EBC9A7F7CDCC4331B8D1D124296474BF2678DD0937E
- x64: AFA8D42BC1C3C0B21DCBD30527C4D6DCC054E40914946F74E575700416A88D94

Self-review confirms the two removed predicates had no paused guest-delivery
effect. The new assertions retain retirement acceptance and distinguish paused
mouse rejection from running admission. Necessary ownership and error handling
are unchanged. S3 waits for owner acceptance after delivery and committed-diff
review; S4 is not admitted.

Coordinator review of pushed c4a624ee confirms the actual code/test diff matches
the approved two-predicate cleanup, no competing input path or dependency was
added, and both published binaries match the recorded hashes. Manifest,
documentation, whitespace and build-residue checks pass. Owned jobs exited;
disposable S3 scratch was removed after evidence promotion, caches retained.
S3 is verified awaiting owner acceptance, not closed or advanced to S4.

## S2 implementation and verification record

Baseline bf18a2f7. Production cleanup changes only
`src/lib/console-broker/console.c`: +4/-18, net -14. A successful deactivate is
followed by reset; failure of either enters the single existing broken-binding
cleanup. The original error is retained, reset is not attempted after failed
retirement, and lock release/output unbinding/reference-release order is
unchanged. Activation rollback remains necessary and untouched.

Tests: `test/lib/console-broker/console_broker_smoke.c` +27/-14 and
`test/app-softpc/integration/command_provider_smoke.c` +0/-1: total +27/-15,
net +12. The existing retirement-failure assertions run for both retirement
and old-input reset failure. Added assertions prove the fault was consumed,
next activation was not attempted, and a second replacement is refused.
No test-only production entry or extra test framework is introduced.

Similar-issue review: all `wait_for` callers in command_provider_smoke use its
three completion handles or program_completed; all four are auto-reset events.
No remaining ResetEvent call exists in that fixture. Broker's prepare,
allocation, activation/rollback and notification failures are distinct phases,
not duplicate copies of the retired-binding cleanup; they remain unchanged.
No adjacent Session change is included. Public changes are limited to src/lib
and test/lib with their manifests; other six public trees are unchanged.

Qualification is complete for the S2 scope, with the supplements below.
Cached builds completed on both widths (28 incremental steps each), without
compiler warnings in either build log. x64 focused 4/4 and background 184/184
pass. The initial x86 focused run is 3/4: command_provider_smoke reaches debug
resume (completion 2) then fails its unchanged 10000ms wait assertion with
0xc0000409, while all three Broker tests pass. This is the historical symptom,
not proof of an identical root cause or a repair; S4 retains investigation
ownership. No code or timeout was changed for the supplemental run.
The x86 full background run passes 184/184, followed by focused 4/4. Both
background XML sets exactly match registered names, with zero failures/skips.
Background elapsed: x64 125.76s, x86 123.23s. Both desktop groups were attempted
while the owner session was Disc: three Lib cases pass on each width, but both
App package routes on each width fail stage 3/error 0 waiting for the initial
prompt. This records correlation with session state, not causal proof. Desktop
elapsed: x64 69.81s, x86 69.18s; group exit 8. These failures are not waived and
do not constitute complete qualification. All launched jobs have exited.

After owner reconnection, session query confirmed Active. Connected package
results: x86 2/2 (24.62s); x64 ordinary passes, compact fails stage 1/error 6
(17.32s group). An unchanged isolated x64 compact run passes in 12.41s. No
code, fixture budget or product behavior changed between these attempts. The
earlier command wait and package failures remain S4 evidence, not repaired by
this cleanup. Passing JUnit names combined with the original three successful
Lib desktop cases and background results exactly cover 189 registered cases
per width excluding native_audio, without skipped cases in the passing set.
This is supplemented qualification, not an all-green first run. Three unrelated
native-audio cases per width were not rerun under this bounded S2 packet.

Published build hashes:

- softpc32: F657EC43BADB3BE205C837A0605376F2F0E29EC0F863298712AE9D0C786A63E4.
- softpc64: B5E940DC7F101AF8D484C2E6FE272114C8A39AA58EBE13355944FD722BC5E362.

INI/media, other six public trees and mirror are unchanged. Source/test diff
review preserves a single fail-closed path without new state or interface;
combined production and test C net is -2. Manifest, documentation and whitespace
checks pass. All owned jobs exited; concise evidence is retained here and task
scratch is retired before delivery. No ABI, platform dependency, lock order,
queue, retry, timeout, guest behavior or native rendering path is added.
Executor delivery is followed by actual-commit coordinator review; S2 awaits
owner acceptance and neither S3 nor S4 is automatically admitted.

## Observation ledger retained

### S2 owner-requested fresh full regression

On 2026-10-22 the owner expands verification to every registered test, including
native audio. Baseline 27946581; no source, binary, INI or timeout changes.
The frozen universe is all 192 registrations per architecture, partitioned into
184 background, five desktop and three native-audio cases. All six groups ran
to completion; XML names exactly match registration, with no missing, duplicate
or skipped cases. No older result substitutes for this run.

- x64: background 184/184, desktop 3/5, audio 0/3; total 187/192.
- x86: background 184/184, desktop 3/5, audio 0/3; total 187/192.

Both widths fail the same five names: app-softpc/integration/package_smoke,
app-softpc/integration/package_smoke/compact_console,
app-softpc/integration/audio_lifecycle_smoke,
app-softpc/integration/sb_midi_native_smoke and
lib.integration/native_midi_output. x64 package failures are stage 1/error 6;
x86 package failures are stage 3/error 0. Audio failures are the existing
assertions for PCM startup or native MIDI output opening, not skipped capability
checks. Command-provider passes in both fresh background groups.

Session was Active at job launch and Disc at the subsequent audio-failure
inspection. This is recorded environmental evidence, not proof that every
failure has that single cause. Full execution is complete but full passing
qualification is not. Owner has been asked to reconnect for the five failing
cases on each width. No disconnected retry, deadline change or production
repair was made. Prior supplementary passes remain historical evidence only.
The owned job exited with result 1; results are promoted here before scratch
retirement. S2 remains awaiting complete qualification and owner acceptance.

These five observations originate in TODO, not five established public bugs.
Owner explicitly transfers all five entries out of TODO into this S. Their
unresolved status and historical evidence are retained below, not marked fixed.
This proposal is their sole investigation record; no duplicate TODO or queued
candidate remains. QUEUE contained none of these five entries; its separate
product-performance candidate is unchanged and outside this admission.

1. Paused key release/source retirement. Session control removes host pressed
   records while PAUSED without delivering release to Machine. Source behavior
   is confirmed; an end-to-end stuck-key reproduction is not. Emulator Session
   is the direct public owner to inspect. Distinguish host observation, FIFO
   acceptance and actual driver delivery. S15's frozen FIFO is accepted and
   is not authority to add pause-release exceptions or restore-time bursts.
2. Command-provider completion wait. T98 S7 observed a 10000ms assertion after
   debug resume/pause in the App integration fixture; unchanged follow-up
   passed. No Window is created. Trace X86 IBMPC command handling, Emulator
   request/completion and actual driver safe points. Identify the exact missing
   completion before blaming RDP, Machine or test event consumption.
3. Console attach/prompt failure. Package tests have observed empty live
   buffers, missing prompts and machine-composition exits, stages 1/error 6
   and 3/error 0. Some occurred while Active. A 30-second startup budget fixed
   one observed slow-prompt case, not all failures. Inspect Lib Console/Broker,
   Emulator Product and App fixture boundaries without assuming one root cause.
4. Compact Console input loss. T93 S12 reached DOS but failed stage 14/error 0
   after injected ver; unchanged rerun passed. Distinguish native injection,
   Broker read, KVM decoding, Session/FIFO acceptance and guest consumption.
   Startup failure and post-start input loss are separate observations.
5. Native child initialization failure. Earlier package probes observed
   0xC0000142, AttachConsole error 5 and subsequent error 6. This may explain
   a subset of observation 3, but the responsible DLL/path is unproved.
   Investigate jointly with startup ordering; do not treat downstream Console
   errors after child death as proof of a Console implementation defect.

All five require a disposition: confirmed public defect, removable redundancy,
necessary behavior retained, product/fixture/host-owned, or unresolved with
specific missing evidence and receiver. Completion means all five have honest
dispositions, not that all intermittent failures were reproduced or fixed.

## Three responsibility reviews

### Input ownership

Inventory existing Session/KVM ledgers and Machine FIFO, their writers,
readers, locks and teardown. Separate their distinct necessary purposes from
genuine duplicate authority. Trace press-before-pause, release-during-pause,
reset/retirement, genuine resume, capture continuation and stop/reset.
First evaluate correcting/removing contradictory accounting branches. Do not
add a second ledger, release queue, epoch or product callback. If a coherent
solution needs changed admission semantics, report the tradeoff before coding.

### Command completion

Trace request acceptance to executor action to completion publication to the
waiting consumer. Check whether status reads or extra waits duplicate a known
completion fact, but do not assume all state reads are redundant. Preserve
necessary request identity and synchronization. A wrong test wait should be
fixed in its owner, not compensated by a public retry/acknowledgment protocol.

### Console ownership

Use the existing Broker as the native handoff owner. Identify actual callers
of activation, display restore and input-reader handoff. Seek duplicated upper
layer interference or a missing operation in the one existing path. Do not add
replay, redraw, reattach loops or recovery state to mask child startup failure.
Combine observations 3 and 5 where evidence supports it; keep 4 distinct.

## Decision and evidence standard

For each candidate report actual user impact, source/call locations, one
responsible owner, the current sequence and synchronization, any duplicate
state/branch, the smallest alternative and a no-change alternative. Estimate
production/test additions, deletions and net size separately. Explain the
number of states, owners and paths removed, not merely line-count savings.
Necessary memory/resource safety and explicit errors must remain.

Use existing owner-local tests and bounded real-path probes to discriminate
hypotheses; no fixed-sleep proof, test-only production API or replacement
implementation. Record failure stages, session state and whether a run is a
repeat. Passing later does not erase earlier failure or prove repair.
Do not rerun the full suite merely to make a source-only review appear complete;
name exactly what was exercised and what was not. Follow background execution
and scratch retirement rules for any potentially long probe/test.

If the only proposed gain requires new compensation state for an unconfirmed
edge case, recommend no change or explicit deferral. Zero production diff is
an acceptable result. Shared layer dependence and lib/types authority remain
unchanged; broad shared-library cleanup is outside this S.

## Exclusions and delivery

The separate queued performance T owns C-VID lookup, product mouse/graphics
conversion, CPU hot-path checks and snapshot I/O. Original VGA/SB/BOP faults,
Compat physical-memory adapters and Ninja corruption are not folded into T99.
X86 Debug is reviewed only where the admitted command path actually reaches
it; do not manufacture changes to touch all eight trees.

Deliver the five dispositions, exact responsibility maps, concrete proposed
deletions/changes and minimal verification plan for any recommended repair.
Only return unresolved work to TODO on explicit deferral with a receiver;
do not keep duplicate active and deferred records. Complete
actual-diff self-review and coordinator review under the single-person mode,
documentation checks and one complete research delivery before acceptance.
This is not a planning-only implementation P or an automatic repair admission.

## S1 research delivery (2026-10-22)

Reviewed repository baseline: 4bc1ff35; runtime remains edb91df1. This is a
bounded review of the three named handoffs, not a whole-library correctness
certificate. No production or permanent test change accompanies this report.

### 1. Input: different records, not interchangeable copies

`src/lib/kvm-base/hotkey.c` owns source-local pending/consumed/delivered-to-sink
keys for chord recognition. `component.c` discards that matcher on input reset
or retirement and emits the corresponding event. It does not synthesize every
guest break. `src/emulator/session/control.c` owns source-key records used to
synthesize those releases while running. Machine's `input_queue.c` owns
accepted events, with admission and push ordered by the same mutex. Only the
Machine executor delivers them to the driver. None of these records is a
guest device's held-key state.

The concrete sequence is:

1. KVM worker posts copied events to Session; Session's control thread alone
   updates its source ledger.
2. Session dispatches to `deliver_machine_input()` in `session.c`. That adapter
   treats both acceptance and INVALID_STATE refusal as successfully handled;
   other failures remain failures.
3. Machine queue mutex orders push against the pause cutoff. S15 freezes
   existing FIFO contents, closes admission, and publishes PAUSED from the
   executor. It does not drain the queue. Genuine resume reopens admission;
   state-read continuation does not.
4. A release/reset seen while Session believes the machine PAUSED removes its
   record without calling the Machine sink. On resume there is no retained
   Session release to send for that record.

Observation 1 is therefore a confirmed accounting/semantic gap, not a newly
reproduced guest fault. Existing `physical_key_identity_smoke.c` explicitly
asserts that paused reset forgets the record without another break; the focused
run passes that assertion. `control_reconciler_smoke.c` also tests suppression
of paused guest input. The problematic state is therefore currently deliberate
test behavior, not an uncovered accidental branch alone.

Do not delete either ledger: removing KVM's loses chord/prefix behavior;
removing Session's loses source-wide cleanup after matcher discard. Moving
`remember_pressed` after a boolean sink call is insufficient: handled refusal
still returns true, and successful enqueue is not driver delivery. Session's
queued completion facts can also lag the actual Machine admission boundary.

Recommendation: retain the accepted frozen-FIFO contract in this research
delivery. Before repair, explicitly choose the semantics for a release that
occurs during pause: retain an ordered release obligation until genuine resume,
or permit a defined paused release operation. Both change present behavior;
neither is authorized merely by calling it cleanup. Do not introduce a second
ledger, special product callback or restore-time release burst. A minimal repair
estimate cannot honestly be fixed until that choice and source-retirement
ordering are established. Current recommended production/test diff: zero.
Receiver remains T99 input-semantics investigation, not a duplicate TODO.

One independent redundant predicate exists in `accept_kvm_event()`: PAUSED tests
SOURCE_RETIRED again after the early unconditional acceptance. Removing that
single line is safe simplification, but does not repair observation 1 and is
not sufficient reason to reopen the input architecture.

### 2. Command completion: preserve one fact, remove redundant fixture reset

`src/x86-ibmpc/product/command.c` translates debug lifecycle requests to Session
requests and delegates completed-runtime reporting to the Emulator monitor.
It does not independently complete a pause. Machine `machine.c` publishes
PAUSED from its executor after closing input and stopping heartbeat; genuine
resume publishes RUNNING. Internal state-read continuation suppresses a false
user pause/resume pair. Session reduces copied completed facts; its desired
state is a request, not a second completed-state authority.

The affected App integration fixture directly drives Machine and feeds provider
runtime notifications; it does not exercise the complete Session run loop.
`command_provider_smoke.c:wait_for()` waits for an auto-reset event and then
calls `ResetEvent()` again. All its completion handles, including
`program_completed`, are created with manual-reset FALSE. The second reset is
unnecessary and could erase a signal arriving between successful wait and reset.
This proves a removable operation, not that such an interleaving caused the
historical debug timeout. Its exact missing event remains unidentified.

Recommendation: App-owned fixture cleanup, production +0/-0, test +0/-1.
No new completion ID, acknowledgment, retry or public event model. Verify all
wait callers and rerun the fixture on both widths. If the original failure
recurs, record the particular wait and preceding executor completion before
changing shared code. Observation 2 remains unresolved; receiver is the App
command-provider fixture/executor boundary. This run passes unchanged on both
widths and does not reproduce the historical aggregate failure.

### 3. Console: one owner already exists; one duplicate cleanup is removable

`console_broker_replace()` is the native handoff transaction owner. It prepares
the next binding, takes the output gate, retires/joins the old reader, resets
old source input, invalidates the old binding, activates the next binding, then
commits current identity/generation. Old output bindings are removed after the
output gate is released so in-flight writers can settle. Failed activation
restores the old object through the same activation helper; failed restoration
marks the broker broken rather than claiming a usable binding.

The transaction lock serializes replace; the output lock prevents concurrent
native output during the switch; generations reject stale writers. These are
different obligations. `emulator/ui/ui.c` calls replace and emits a completed
handoff only on success. Session's broker completion and Lib's activation
notification have different consumers: control/prompt sequencing versus render
activation. Neither is evidence for a second native restoration path.

In `src/lib/console-broker/console.c`, adjacent deactivate-failure and
reset-bound-failure branches duplicate the same broken-state/output-binding
cleanup. The minimal alternative is:

```c
status = console_broker_backend_deactivate(...);
if (status == LIB_STATUS_OK)
    status = console_broker_reset_bound(...);
if (status != LIB_STATUS_OK) {
    /* one existing broken-state cleanup block */
}
```

Projected production +2/-15 (approximately net -13, final comment placement
affects count); no new helper, state or API. Existing broker fault tests are
the starting point; budget test +0 to +15/-0 only if reset-failure assertions
need an explicit counterpart. This is cleanup, not a claimed fix for package
attach/input failure. Keep the old-binding restoration path: it is necessary
to preserve the replace failure contract, not a second renderer.

Native deactivate and activate both flush input at different boundaries:
retired-reader cancellation residue versus records arriving before new-reader
activation. Do not remove either merely because the same API occurs twice.

### 4. Package observations 3, 4 and 5: improve attribution, not Lib recovery

`test/app-softpc/integration/package_smoke.c` retries AttachConsole within its
fixture deadline without checking child exit in that loop. Stage 1 is assigned
by the subsequent code-page query, so its error alone cannot identify the
original startup fault. A historical 0xC0000142 is distinct from a live blank
buffer and from an explicit machine-composition failure. Observations 3 and 5
share an evidence boundary, not a proved common root cause.

The compact route sees DOS, hides its Console again, injects `ver`, and waits
for Version. Stage 14 combines injection failure and successful injection with
missing output into one result. A visible prompt alone does not identify which
input boundary later discarded a record. Hide/focus, activation flush, KVM
decode, Session admission and guest delivery need discriminating evidence;
none is proved responsible here. Observation 4 stays separate from startup.

Recommended receiver for all three is the App package fixture/native handoff
investigation. No shared production change is justified yet. Minimal next
probe: record child liveness/exit and first attach error, distinguish injection
return from output timeout, and timestamp existing activation/read/admission
boundaries only if needed. Estimated temporary fixture diagnostics +15 to +30
lines, production zero; remove temporary traces after evidence promotion.
No retry of startup, extra Session redraw, larger product timeout or recovery
manager. Do not combine these observations into one purported Console bug.

The visible-screen helper also uses a linear read count based on viewport area;
that is not a rectangular viewport read when backing stride exceeds viewport
width. Review its intended oracle before changing it; stage 14 uses the separate
buffer-text helper, so this is not evidence of its cause. This candidate remains
within the package-fixture receiver and is not an admitted test rewrite.

### Verification, scope and decision

Existing cached binaries, without rebuilding or changing source, were exercised
with CTest on both x64 and x86. Selection:
`physical_key|control_reconciler|command_provider|session_monitor|console_broker`,
excluding `desktop|native_audio`. Each width passes 7/7, zero failed tests:

- app-softpc/integration/command_provider_smoke;
- lib.console-broker/console_broker;
- lib.console-broker/win32/console_broker_reader_failure;
- lib.console-broker/win32/console_broker_cancel;
- emulator.session/control_reconciler;
- emulator.session/physical_key_identity;
- emulator.session/session_monitor.

Times: x64 1.02s, x86 1.19s; job exit 0. The first hidden launch left no results;
it is not counted as a test run. The subsequent logged launch completed. No
desktop/package/audio test, aggregate-stress reproduction, guest stuck-key
experiment or fresh compilation was performed. Source traces and cached-test
evidence are reported separately. Scratch is retired after this record.

All five observations have an owner and an honest disposition above; none is
declared repaired. The research recommends fewer duplicated statements, not
fewer necessary safety boundaries. Suggested next implementation is the tiny
Broker cleanup and auto-reset fixture cleanup, separately approved; input
semantics and package diagnosis remain research. No production/test code was
changed here (both +0/-0), no ABI or published artifact changed, and no downstream
repository was read. The task awaits owner review, not automatic repair admission.

## Historical evidence retained below

The following five former TODO entries are retained as source observations,
including their original admission conditions, now questions owned by T99 S1.
Moving them does not establish a fix or a public-component root cause.

### Observation 1: Audit key release across paused Session input. Existing
  session/control.c forgets host held keys on release/source retirement while
  PAUSED without delivering them to the driver. T98 S15 freezes Machine FIFO
  but does not turn the host ledger into a guest-delivery ledger or resolve
  device-held keys. Source ordering is confirmed; a complete user-visible stuck
  key reproduction is not claimed. Owner: Session/driver input ownership.
  Admission condition: reproduce make-before-pause and release-during-pause,
  then define neutral reconciliation without product callbacks, treating
  host-pending FIFO and already-delivered device input as different states.

### Observation 2: Diagnose the intermittent command-provider event wait failure.
  T98 S7's second x64 aggregate run failed the 10000ms event assertion after
  debug resume/pause observations; the first aggregate and unchanged isolated
  follow-up pass. This fixture does not create a Window; RDP is not an
  established explanation. Owner: App command-provider integration fixture/
  executor event boundary. Admission condition: identify the exact awaited
  event and producer/consumer ordering under aggregate execution; do not relax
  assertions or increase product timeouts. S7 does not claim this repaired.

### Observation 3: Diagnose package integration Console attach/prompt intermittency.
  T98 S6 observed stage 1/error 6 and stage 3/error 0, including after RDP
  reconnect. Unchanged final tests later pass on both architectures; this is
  passing execution evidence, not a repair or proof of environmental cause.
  Owner: App package fixture/Console integration boundary. Admission condition:
  capture child exit, active buffer identity and cursor at the first failure
  against the unchanged baseline; do not weaken prompt assertions or add retries
  as a production fix. Temporary diagnostic code was removed. T98 S7 also
  observed a live blank initial buffer and, in a separate attempt, child exit 1
  with cannot compose machine. Some attempts failed even after RDP was Active;
  later unchanged package tests pass. These are distinct observations, not
  proof that all stage-3 failures share one cause.
  T98 S8 also observed these failures while Active: a live child with blank
  31x30 buffer/cursor 0,0, and a separate pre-attach exit 0xC0000142. Ordinary
  package subsequently passed on both architectures while compact failed.
  The temporary diagnostic was removed; no causal repair is claimed.
  A subsequent S8 probe proved one live child reached the unchanged prompt
  another 5157 ms after the five-second check. The startup-only test budget
  was corrected to 30 seconds, without a retry or product timeout change.
  This does not explain the separate initialization exits or prove that all
  attach/prompt failures are timing-budget failures.
  T98 S9 repeats stage 1/error 6 in compact package on each architecture while
  RDP is Active; ordinary package and all other cases pass. Each unchanged
  isolated compact rerun passes. No fixture/product change or causal repair
  is claimed; retain both first failures and supplemental qualification facts.
  T98 S10 final x86 compact package again failed stage 1/error 6; an unchanged
  complete desktop-group repeat passed 5/5. No causal repair is claimed.
  T98 S13 x64 compact package repeats stage 1/error 6 while RDP is Active;
  its unchanged isolated repeat passes. x86 passes all desktop cases first run.
  S13 does not claim to repair this existing package-initialization debt.
  T98 S15 x64 repeats compact stage 1/error 6 after reconnection while Active;
  the unchanged isolated case passes. All other desktop/audio cases pass.
  This supplemental qualification does not establish a repair or root cause.

### Observation 4: Diagnose intermittent package compact-Console raw input loss.
  T93 S12's first desktop x64 compact case reached DOS but timed out at
  stage=14/error=0 waiting for Version after injected ver; the captured screen
  contained only C:\>. Normal package passed immediately before it. A complete
  unchanged ten-case desktop rerun passed both widths. This differs from the
  stage=1 child-startup debt below and does not prove the cause or a repair.
  Owner: App-SoftPC package-test/input handoff. Admission condition: bounded
  native input/activation instrumentation around first raw command and hide/
  focus transition; do not add retry or relax assertions. See
  [S12 verification](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s12-implementation-and-review-record).

### Observation 5: Diagnose native package-test child startup under sequential
  CTest execution. T93 S5 x64/x86 compact-console cases intermittently fail
  before start at stage=1/error=6. A scratch diagnostic reproduces child exit
  0xC0000142 (DLL initialization failed), AttachConsole error 5, then code-page
  query error 6. Direct native/package probes pass; this does not identify the
  responsible DLL or prove a CTest defect. Admission condition: test-owned
  child-startup/Console lifetime investigation with loader evidence, without
  retrying product startup or changing guest/device behavior to hide it.
  Evidence: [T93 S5 checkpoint](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s5--original-dma-prerequisite-repair).

### S2 owner-authorized failure investigation, after fresh full regression

The owner requests repair and full passing verification. This expands S2 to
investigate the five failing native registrations; it does not admit pause
semantics work or claim the historical intermittent failures repaired.

An independent system-API probe, in the disconnected owner session, reports
`midiOutOpen(MIDI_MAPPER)=1` (Undefined external error), with one enumerated
device, and `GetDefaultAudioEndpoint=0x80070490` after successful COM setup.
Enumeration is not usable-device proof. No Lib status is changed to conceal
these failures and no native integration is skipped.

The package fixture had a real diagnostic defect: after its child exited it
continued waiting for a prompt, reporting stage 3/error 0 with no screen.
The existing attach/prompt waits now also observe process exit; failure captures
the final buffer before detachment and reports the child exit code. This adds
no retry and leaves successful-path assertions and budgets intact. Both widths
compile without warnings. In the same disconnected environment all four package
executions now terminate in approximately 0.7-0.8 seconds, reporting child exit
1 and `softpcvm: cannot compose machine`, instead of waiting 30 seconds.
This proves accurate failure classification, not successful product startup.
Source inspection places native PCM acquisition in driver creation before
monitor composition, consistent with the absent endpoint; it does not identify
the separate historical 0xC0000142 loader failure.

Follow-up test C diff is +11/-3 (net +8), no production or EXE change.
At this checkpoint real-host verification was blocked on an active session;
it was not delivered as a completed P. Subsequent evidence is recorded below.

On the owner's subsequent reconnection, direct native acquisition succeeds:
MIDI open returns 0 and default PCM endpoint lookup returns S_OK. The new
complete run uses the existing cached dual builds (both report no work).
Both widths again execute exactly 192/192 registered names with no skips:
184/184 background, 3/5 desktop and 0/3 native audio. Background durations
are 117.49 seconds x64 and 112.26 seconds x86. The session is recorded Active
at launch but is Disc when the x64 native failures are inspected and remains
Disc at completion. Both package tests per width report exit 1 and cannot
compose machine. This is not full passing qualification; reconnection with
the session held active is required for the eight native cases per width.
Their next invocation is ordered before any further background suite.

### S2 P4 connected verification and review

The owner reconnects and requests retry. Both native groups pass serially on
both widths, unchanged code and budgets: x64 audio 3/3 in 41.21 seconds and
desktop/package 5/5 in 31.20 seconds; x86 audio 3/3 in 41.25 seconds and
desktop/package 5/5 in 31.36 seconds. No skip or individual-test retry occurs
inside this connected invocation. The preceding disconnected failures remain
above; this is supplemented qualification, not a repaired loader-race claim.

The frozen universe is the 192 CTest registrations per width. Final evidence
combines the latest full-run background XML (184 cases) with connected desktop
(five) and audio (three). Exact sorted name equality with registration proves
192 unique cases per width, zero missing/extra names, failures or skipped cases.
Both cached full builds report no work; the changed fixture's preceding dual
builds compiled and linked without warnings. Published EXE hashes remain:

- x86: F657EC43BADB3BE205C837A0605376F2F0E29EC0F863298712AE9D0C786A63E4
- x64: B5E940DC7F101AF8D484C2E6FE272114C8A39AA58EBE13355944FD722BC5E362

Actual-change self-review: only the fixture's existing attach and monitor wait
paths observe child exit, and failure captures output before detachment. There
is no new retry, thread, API or production path. Successful prompt assertions
are unchanged. Similar-issue sweep covers all waits in this fixture: subsequent
guest/debug operation waits retain bounded budgets and now share final child
exit/screen diagnostics; they are not startup waits and are not broadened here.
The historical 0xC0000142 failure remains unproved and belongs to planned S4.
No shared tree/manifest, guest, configuration or media changes in this P.
Task scratch is retired only after owned jobs exit and evidence promotion.

Coordinator review of pushed executor commit e6090b97 confirms the three actual
changed paths match the admitted follow-up, test assertions are not weakened,
production/EXEs remain unchanged and no new public dependency or recovery path
exists. Both success and early-exit behavior have direct execution evidence.
The complete name ledger and disconnected-history qualifications above agree
with the inspected XML results. Documentation, whitespace and build-residue
checks pass; scratch has been removed after all owned jobs exited. S2 remains
verified awaiting owner acceptance, with T99 open and S3/S4 not admitted.
