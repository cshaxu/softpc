# Public Handoff Simplification Review

## Admission and scope

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
- S3: planned, not admitted. Determine one correct minimal pause-key contract
  across source ledger, FIFO admission and driver delivery. Give concrete
  ordering and make/break/source-retirement examples, reject extra ledgers and
  product callbacks, and obtain approval before changing semantics.
- S4: planned, not admitted. Investigate observations 2 through 5 using exact
  missing completion and child/input boundary evidence. Distinguish startup
  death, live blank output and post-start input loss. Do not add retries or
  shared recovery machinery before identifying the first failing operation.

S3/S4 remain here, not duplicated in the T queue or TODO. S2 does not delete
the unrelated redundant Session predicate, which belongs to the later review.

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
