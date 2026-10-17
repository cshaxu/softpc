# Windows 95 Sound Blaster Recognition And Playback Repair

## Owner request and queue boundary

> 好的，请你追加到队首一个T任务proposal，修复这个，确保win95能识别和正确使用我们新加的sound blaster模拟硬件。

The owner admitted this candidate as T94 after T93 closure on 2026-10-16.
The supplied fresh Win95 snapshot has no audio driver installed; independent
installation on disposable media is authorized. Historical Code 24 is a lead,
not a presumed property of this fresh starting state.
[CURRENT](../states/CURRENT.md) remains the sole implementation authority.
The [XP sound capability proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md) records
the imported device and host capability baseline, not Win95 driver acceptance.

## Observed failure and evidence limits

S1 admission baseline is clean c55dd73d. Estimated production and formal test
changes are +0/-0, net zero; diagnostic probes remain ignored. The current
external INI selects a different guest than the supplied snapshot and its EXE
predates the accepted package. Verify snapshot-owned attachments before launch.
The post-closure disconnected-session rerun passed every unit and repository
check but failed two native MIDI and two native package cases per architecture.
Do not infer device failure or native playback availability from these results.
The completed S1 record will enter history with its evidence; the repository's
New identifier gate treats any T94 history filename as an already recorded T.

### S1 diagnostic checkpoint

The supplied archive restores 16 MiB Windows 95 4.00.950 successfully through
the existing executor, with a 640x480 captured desktop. Its sole attached disk
is overlay mode, with 127 saved pages; the backing digest matches. The original
archive and image have not been changed. Diagnostic input is injected through
the machine queue, not host desktop automation.

Native `vm_create` initially fails with I/O error. Isolation identifies speaker
PCM acquisition failing in this host session; MIDI preparation itself only
creates a lock and is not the proven startup cause. The diagnostic harness
uses the existing test PCM/MIDI receivers to continue guest investigation.
This is not native playback acceptance and no production fallback was added.

Guest installation selects the built-in Creative Labs Sound Blaster driver,
not SB16 or Pro. Its actual default resources are I/O 0220-022F and 0388-0389,
DMA 1 and IRQ 5, matching the model. The wizard copies mssblst.drv and completes;
an overlay checkpoint preserves the installation. A normal guest restart
returns to the desktop. Device Manager then shows a yellow warning on Creative
Labs Sound Blaster. At this checkpoint its exact error code and failed exchange
were not yet captured; the later port experiment below resolves the first
failed exchange. Thus missing installation alone is not the explanation. Actual
playback remains unverified; installation completion alone does not close any
driver/playback ledger item. Preserve pre-reboot and post-reboot checkpoints
for bounded port tracing through the existing adapter callbacks.

### Confirmed reset binding defect

A registration probe confirms normal guest restart enters original BIOS
`reset()` and `io_init()`: SB read port 022A and write port 022C change from
adapter 36 to empty adapter 0. `host_reset()` returns with both still empty;
no subsequent SB registration was observed in that replay. The standalone
Machine calls `SbInitialize()` only after its own explicit cold-reset call,
whereas guest restart enters original BIOS without returning through that
composition site. Original `io_init()` is correctly rebuilding the device map;
the missing standalone reset connection is the defect, not an IRQ/DMA choice.

Candidate single-owner repair: use the original reset host callbacks, with
sound teardown before the existing queue reset and sound initialization after
device-map reconstruction; remove the Machine-only post-reset initialization.
`NIDDB_System_Reboot()` and `host_reset()` are existing Compat endpoints, not
new BIOS paths. Before implementation, prove exact ordering, failure propagation
and subsequent driver exchange. A probe-only hook experiment is permitted to
test causality; it is not the production repair or a successful native playback
qualification. No public component or mirror change is presumed necessary.

### Controlled callback experiment

The baseline port trace records reset 1 then 0 at 0226, followed by FF at
022E and 022A: no DSP reset acknowledgement reaches the guest. In the ignored
diagnostic harness only, wrapping the existing reboot callback with
`SbTerminate()` and the existing host-reset callback with `SbInitialize()`
restores adapter 36. The same installed guest then receives AA, DSP version
02/01, and complement-test response 55; it proceeds through IRQ test F2 and
DMA playback command 1C. Device properties explicitly report normal operation.
The driver page identifies Microsoft's mssblst.drv version 4.0.950, alongside
mssblst.vxd and msopl.drv. This establishes the reset-binding causal defect;
it does not prove transfer completion or audible native output.

Evidence is retained under build/t94/s1: baseline-sb-ports.csv, run12.log,
run13.log, sb-ports.csv, frame-53.bmp (normal device status) and frame-59.bmp
(driver version). The experiment exited with shutdown/destroy/vm_destroy all
zero. Original media and snapshot digests remain unchanged. No production or
formal test code was modified, and no replacement EXE has been released.

The minimal implementation candidate is confined to Compat platform callbacks
and Machine reset composition. Preserve existing cold-reset teardown, terminate
warm-reset sound before queue reconstruction, initialize after port-map rebuild,
and remove the redundant Machine-only initialization. Void callback failures
must feed executor readiness and its existing exit request, not be swallowed
or introduce another recovery path. Expected production scope is two files,
approximately +20-35/-8-12 lines (net +8-27), with no XP mirror/public-component
delta. Regression scope must include actual guest restart, repeated restart,
pending sound-event teardown and initialization/termination failure injection.
These are planning estimates, not an implemented or qualified patch.

### Historical pre-admission observation

The owner manually selected Creative Labs Sound Blaster in Windows 95.
Device Manager reports Code 24: the device is absent, not working correctly,
or lacks a driver. Its resource page reports no conflicts and shows
0220-022F, 0388-0389 and IRQ 05. DMA is outside the supplied screenshot;
the assigned DMA channel and persistence after a guest reboot are unverified.
Do not turn these unknowns into claims that the owner configured it wrongly.

Source inspection identifies SB_VERSION=0x201 (SB 2.0/DSP 2.01), base 0x220,
IRQ 5 and DMA 1. Optional MPU attachment depends on host MIDI availability.
At that earlier checkpoint no root cause had been established. Code 24 alone
does not distinguish DSP reset/version failure, resource mismatch, IRQ/DMA
failure or driver loading.
Existing mock/device tests do not prove this guest driver works.

## Objective and boundaries

Make the Windows 95 Sound Blaster driver recognize and correctly use the
existing SB 2.0 hardware model, retaining one device implementation and the
existing DMA/PIC/Compat/Lib output owners. Do not spoof successful detection,
special-case the guest OS, upgrade the model to SB16, or bypass hardware
protocols with a second playback route.

Primary scope is the existing App-SoftPC device and Compat integration and
their owner-local tests. Changes to the preserved XP subset must be necessary,
minimal and accounted for against that subset. Public component changes need
evidence of a defect at their actual boundary and an explicit scope decision;
they are not presumed necessary to fix guest detection.

PCM playback is required. Host MIDI remains optional and separately reported;
FM synthesis is not introduced by this task. FM's existing unsupported-host
limitation must not silently become an excuse for broken PCM initialization.
Recording, new sound models and software synthesis are outside this proposal.
Protect original user INI, images and snapshots. Guest installation and writes
use authorized disposable copies/overlays, not the owner's original media.

## Proposed sequential subtasks

### S1 - Reproduce and identify the first failed hardware contract

Record exact EXE/build, Win95 edition and driver files/version, configured
resources, reboot state and installation results. Reproduce Code 24 on a
disposable guest copy. Use bounded probes through existing port/DSP/DMA/PIC
and host-output paths; Computer Use is not required or authorized here.
Snapshots may shorten replay after confirming they match the device format.

Follow reset response, version commands, command parsing, IRQ/DMA probing and
driver startup in execution order. Locate the first divergence from the
documented hardware/driver contract, rather than assume any candidate cause.
Distinguish source reasoning, deterministic test proof and real guest evidence.
Deliver the causal chain, minimal patch design, files and added/deleted/net
estimate before production edits. Do not estimate a fix size before this audit.

### S2 - Repair the existing owner and add deterministic regressions

Implement the confirmed minimal repair in the existing path. Add a failing
before/passing after test of the actual protocol or lifecycle boundary; use
test-owned host audio mocks so unit tests need no interactive session or audio
device. Sweep equivalent reset, command, DMA completion and IRQ acknowledgement
paths as justified by the root cause. Preserve unrelated original code.
Build both widths, refresh both package EXEs and qualify the changed path.

### S3 - Real Win95 qualification and task closure audit

Prove successful driver initialization and actual known-WAV PCM playback in
Win95, not merely an installed device icon or AUDIO.COM beep. Verify first and
repeated playback, completion without hangs or stuck IRQs, cold/warm restart,
pause/resume and save/load continuation under the existing snapshot contract.
Check the existing DOS speaker/SB and shared regression coverage for regressions.
Run cached dual-width builds, all required shared and other unit/integration
tests, including separately scheduled real audio and native desktop tests.
Publish both EXEs; commit/push complete deliveries, then wait for owner acceptance.

## Finite acceptance ledger

### S2 implementation checkpoint

The initial production patch changes only Compat platform.c and Machine
machine.c: +21/-8, net +13. Original sndblst.h already declares initialization
and termination, so the callback owner consumes it without a mirror edit.
The sound-reset failure latch is host-lifetime state, cleared when binding a
new machine. Failed warm teardown prevents reinitialization and requests the
existing CPU exit; executor readiness preserves the failure after callbacks.
Machine's cold teardown and final destruction remain their distinct boundaries.

The owner-local platform failure test now reads reset AA and version 02/01
after cold initialization and two original BIOS warm resets, and injects sound
initialization/termination failure. The revised focused4 test passes on both
widths; x86's existing Ninja log-recovery warning caused broader rebuilds.
This is not full qualification.
Earlier diagnostic test attempts are retained: one used a C99 loop declaration
in this legacy-C target, and exit-request assertions incorrectly queried outside
an active CPU frame. Both test defects are corrected, not hidden with retries.
Machine's existing device-contract test additionally starts a real DMA block,
confirms a pending completion event, enters original warm BIOS reset and checks
cleared transfer/event state plus new DSP reset/version replies. Tracked test
delta is +98/-0 across the two existing owners, with no new target or fixture.
Full qualification is running. Its first shared selector incorrectly required
a slash where registered shared names use a dot; that group selected no tests
and is not a pass. The complementary group consequently runs all 185 background
cases. Preserve this run and supplement the corrected grouping after completion.
x64 full-target build refreshed its package EXE; dual-width publication and
complete test/native qualification are not yet established. Delivery remains
outstanding.

The first complete S2 run finishes both widths: 185/185 background tests pass
per width, including the new pending-event restart. Each width runs all eight
native cases: audio lifecycle, Window retirement/modal and Console display
pass; native MIDI, SB native MIDI and both package cases fail. Thus each width
has 189/193 actual tests passing, not full acceptance. Package reports stage 3;
its cause is not proven solely by the MIDI failures. No failing native case is
reclassified as passed or skipped. The existing shared MIDI code is unchanged.
Correctly grouped tests are being supplemented without another compilation.
Both full-target builds finish and refresh package EXEs; x86 again reports
Ninja log recovery and unchanged mirror warnings, so no zero-warning or
ideal-cache-reuse claim is made.

Current candidate binary SHA-256 values are
32-bit `1E560D700E52F03D620DA8502F9149001C50B227EC9AC0DCB6C9F019819FCB20`
and 64-bit `5D1519C16860E7BF034F5612366F041B52064AB284CFA2245E032D15ABA20747`.
The Win95 probe is linked from the repaired objects with observation-only PCM
wrappers, no reset overrides. It still uses test host receivers and cannot
establish native playback. It waits for qualification jobs before guest replay.

### Qualification correction: assertions compiled out

The corrected grouping reports 121 shared and 64 other cases passing per width;
their name sets plus eight native cases exactly match all 193 registrations.
However, inspecting the contradictory native PCM observation exposed invalid
test evidence: audio_lifecycle_smoke has -DNDEBUG without -UNDEBUG and puts all
operations inside assert. Its success is vacuous. Scanning every generated test
compile rule with assert.h finds one additional omission, parallel_failure_smoke.
Both are absent from the existing SOFTPC_TEST_TARGETS list. No other matching
assert-based test compile rule is missing assertion activation in that scan.

Restore both targets to the existing list (+2 build lines), with a three-line
NDEBUG rejection in each affected source. This is a required qualification
repair, not an audio production workaround. The previous 189/193 receiving
count is not a valid acceptance claim; requalify after fixing these omissions.
The repaired-object Win95 replay produces repeated non-neutral PCM, but explicit
WAV opening/playback is not yet established. Preserve its observation checkpoint;
do not confuse driver test patterns with application playback. A separate real
PCM probe fails speaker startup and vm_create with I/O error, consistent with
the newly exposed lack of native proof. MIDI is mocked only in that diagnostic;
shipping code remains unchanged.

With assertions enabled, the parallel test exposes an incorrect oracle, not
a new parallel implementation defect. A short write retains twenty bytes;
the next byte appends below the flush threshold, leaving twenty-one bytes.
The corrected test verifies every retained byte and the appended byte, then
explicitly flushes through host_print_doc. Parallel production stays unchanged.
The corrected full run has 121/121 shared checks and 11/11 focused reset checks
per width; other checks are 63/64 pending this oracle recheck. Native cases are
3/8 per width: audio lifecycle now genuinely fails, as do both MIDI and both
package cases. Window retirement, modal and Console display pass.

An independent native SDK probe, without Lib or SoftPC, confirms successful
COM/enumerator creation but default render endpoint lookup returns 0x80070490;
MIDI reports one device but mapper open returns 1. These are current host
capability failures, not evidence of successful native playback. Package stage
3 and the real-PCM VM probe fail at startup; no silent audio fallback is added.

The corrected parallel test subsequently passes on x64 and x86; all 185
background cases per width now have actual passing evidence (the full run plus
this focused correction). Native failures remain outstanding.

The second repaired-object guest run again reaches the desktop and produces
PCM through the diagnostic receiver. Wave-file playback remains unproven.
Its final captured publication shows desktop icons without application windows;
this does not prove either an application crash or a display fault. A disposable
observation checkpoint is saved and the probe shuts down normally. Reused frame
filenames can display stale image previews; subsequent evidence must use unique
capture names and verify file hashes before drawing conclusions from previews.
After assertion qualification, the x86 candidate hash is
`2F4C4B6EF586392BCF9DC0897C6BB03D15191F0F7D3054BCEC2158AC15152F05`;
the x64 candidate hash above is unchanged. Neither is a qualified delivery yet.

### Additional physical-address defect: owner-approved correction

A second probe restores the observation checkpoint and confirms the recorder
is responsive. Its File/Open menu opens the installed Tada WAV successfully;
the displayed duration is 1.24 seconds. Read-only FAT inspection independently
finds 22050 Hz, mono, 8-bit PCM with 27459 data bytes. Ctrl+O was not a proven
shortcut for this recorder, so earlier commands did not prove file opening.

Actual playback then exposes a deterministic data mismatch. At DMA page FF,
address 8000, count 0ABF, mode 59, block size 1376, the submitted samples start
00 01 02 ... 1F. Reading the same physical range through c_sas_PRS returns
80 repeated. At address 8560, submitted bytes start 60 61 62 ... 7F while
physical bytes start 81 7C 7A 85 94 99 A2 9E. This is not an audible-output
claim: the observation uses the existing diagnostic host receiver.

Source chain: nt_sbdsp.c DmaDataToDsp passes dma_system_address to sas_load;
under this CPU, sas_load reaches c_sas_hw_at -> bios_read_byte, which translates
through guest page tables when paging is enabled. DMA addresses are physical.
The existing at_dma.c transfer owner correctly uses sas_PRS. The four sound
hits are one read at DmaDataToDsp and three sas_store calls using
GetDMATransferAddress (ADC silence/identification writes). All require physical
memory semantics; no new DMA implementation or address translation is needed.

Proposed next repair uses the existing physical read/write interfaces in these
four places, retaining the byte-wise wrap/decrement loop and guest commands.
Tests must enable paging with deliberately different linear/physical mappings,
then verify submitted playback bytes and the three write destinations. Current
real-mode tests cannot establish this contract. This affects the selected XP
mirror and its ledger. After reviewing the four adaptations and mirror delta,
the owner approves this correction within S2, retaining the original shared
device logic rather than reimplementing it. The repaired reboot
binding remains valid and is not reverted. A disposable pre-play checkpoint
now preserves the open WAV at position zero for deterministic follow-up.

Incremental estimate: production +5/-4 (net +1), plus one explanatory source
comment; tests reuse existing command/transfer assertions under nonidentity
paging, approximately +45-65 lines. No shared component changes. The four
linear accesses originate in T93 S6 065ac9ed, not XP's native pointer accesses.
Normalized unified-diff accounting against the fixed necessary subset gives
777/1389 added/deleted before, 778/1389 for the four substitutions before
comments. This is whole-file cumulative divergence, not this repair's patch.

The implemented four-site change is +6/-4, net +2 including that comment.
Its existing XP subset selection and raw/subset hashes remain unchanged;
only the local body identity changes. The address increment/decrement loop,
16-bit wrap, guest DSP command handlers, transfer lifetime and IRQ paths are
retained. No public component, native backend or snapshot schema is changed.

Focused x64/x86 SB commands pass with a live nonidentity page mapping. The
existing ADC and two identification writes, playback capture, both address
directions, 64-KiB wrap/full bank, auto-init, partial submission and IRQ checks
are reused rather than copied. A separate shadow range must remain untouched.
Fixture preparation initially exposed missing A20/user-page setup; the test
explicitly establishes both before checking that its mapping is live. Its
non-CPU_40_STYLE sas.h aliases physical names to linear functions, so the test
calls the original c_sas_PRS/PWS symbols explicitly, as other CPU probes do.

The same test exposed Compat memory.c's named physical adapters using linear
c_sas_loads/stores. This is recorded as a separate High TODO, not silently
repaired in the four-site mirror authorization. The sound tests use the actual
physical SAS functions for their oracle. Similar-issue scan of nt_sb*.c finds
exactly the four corrected memory accesses; scanning Compat/Machine for the
linear block calls finds those two adapters (debug_memory already documents
and avoids a second translation).

Controlled Win95 checkpoint replay opens the same installed Tada WAV and
finishes at its 1.24-second end position. The receiver records 66 successful
submissions, 30272 accepted bytes and 28243 non-neutral bytes. The first eleven
submission observations compare each entire 1376-byte captured block against
physical DMA RAM with zero mismatches. This replaces the prior ramp-byte
failure; it does not assert bit-exact equality of the whole submitted stream
to the WAV file, nor audible output. The guest probe shuts down/destroys all
three owners successfully; source media and original checkpoint are untouched.
An independent native probe still reports no default render endpoint
(80070490) and MIDI mapper open failure (1); native qualification is pending.

The refreshed full-target builds pass on x64/x86 (cache retained; x86 again
reports the known Ninja log recovery and unchanged mirror compiler warnings).
Both widths pass 121/121 shared and 64/64 other background cases; 11/11 focused
cases also pass. Each width actually runs all eight native cases, with three
Window/Console passes and five failures: audio lifecycle, native MIDI, SB MIDI,
and both packages. x64 normal/x86 compact packages fail at stage 3; x64 compact/
x86 normal fail at stage 1/error 6, the existing independent startup debt.
Do not attribute the latter conclusively to missing audio. No test is skipped
or reclassified as passing. Thus the repair is implemented and deterministically
verified, but S2 is not qualified for a complete P or closure.

Single-session coordinator review checks the actual diff: only four existing
DSP memory adaptations change; original device state, command parser and DMA
sequencing remain. No shared src/test package changes or original media writes.
Entire pending S2 production: +27/-12, net +15 across three files; tests:
+166/-19, net +147 across four files; root build list +2/-0. The mirror-only
repair contributes +6/-4 (net +2), with normalized subset diff now +779/-1389,
total 2168 versus 2166 previously. Documentation and binaries are separate.
The controlled guest and qualification jobs have terminated; current evidence
retains the first failed fixture attempts, final paging pass, all native failures
and byte observations. Required native evidence remains a blocker, not a claim
that all Win95 playback requirements have been met.

Fresh host-only revalidation after the repair still returns 80070490 for the
default render endpoint and 1 for MIDI mapper acquisition. Session query shows
the executor's user session 1 is disconnected. Audiosrv and AudioEndpointBuilder
are both running, and enumerated physical audio devices report healthy status.
Thus restarting audio services or changing the product backend is not supported
by this evidence. Native acceptance needs an available output in the executing
session; no host configuration, service or session was changed by this check.

### S2 native gain defect and approved extension

After reconnecting, native PCM lifecycle and MIDI integrations pass on both
widths. Native Win95 checkpoint loading nevertheless fails in gain setup:
22050 Hz mono stream creation succeeds, but set_gain returns IO_ERROR. An
independent SDK probe confirms native conversion exposes two volume channels;
SetAllVolumes with the one input channel returns E_INVALIDARG (80070057).
The old unit stub assumed native volume channels equal input channels and
therefore missed this contract mismatch. This is distinct from the earlier
disconnected-session missing-endpoint failures.

Owner approves the public Lib repair in S2. The existing native-buffer copy
now applies input-channel gains before native conversion. Remove the native
volume service acquisition, release and calls rather than adding a mapping or
fallback path. No stream API, queue, worker, timing or guest state is added.
Gain applies on subsequent submission, not retroactively to already submitted
frames; this timing boundary is now explicit. Default unity is bit-preserving.
Signed 32-bit multiplication is sufficient: -32768 * 65535 remains representable.

Unit regressions verify mono/stereo, independent mute/unity channels, half gain
on signed extrema and small values, default unchanged samples, clear retaining
gain, invalid counts and native submission failures. No OS volume service is
acquired. Existing initialization failure coverage shrinks by the removed
service call only. Public production +13/-24 (net -11), test +32/-22 (net +10),
excluding manifests.

Final S2 qualification: full-target cached x64/x86 builds pass and publish
both EXEs. Each width passes all 193 distinct cases: 121 shared, 64 other
background cases and eight real audio/desktop integrations; no failures or
skips. The focused eleven-case subset also passes. Both normal and compact
package cases pass in the restored session. Prior failures remain evidence,
not erased retries; the known loader debt is not declared repaired.

Native Win95 checkpoint loading now opens 22050 Hz mono PCM and applies gain
16383 successfully. Two plays of the installed Tada WAV reach 1.24 seconds.
The first play records native consumption advancing to at least 27643 frames
and 30272 accepted bytes. Eleven entire 1376-byte DMA block observations match
physical RAM. The second opens successfully and submits non-neutral PCM again;
the diagnostic's cross-stream progress-print threshold suppresses second-play
consumption totals, so those are not claimed. Both end-position captures and
input actions come from the actual guest, without desktop automation. This is
native output progress, not an assertion that a human listened to the sound.
Post-play snapshot save succeeds, resume is accepted and all shutdown/destroy
owners return success. The probe exits zero and releases its resources.

Actual-change review: all production delta is +40/-36 (net +4) across six
files; tests +198/-41 (net +157) across five files; root build list +2/-0.
Manifests, documentation and binaries are counted separately. Gain has one
worker-owned input-sample path, no endpoint-volume object, native retry or
guest-specific branch. Original DMA/IRQ loops remain intact. The mirror delta
remains the four approved physical accesses (+6/-4). Public non-audio owners
are unchanged. Final delivery still requires the executor commit/push and
separate coordinator actual-commit review; whole-T acceptance is not implied.

Coordinator review after executor push cd665a3d: inspected the actual committed
production/test diff and complete changed-path list against the approved scope.
The reset lifecycle uses existing callbacks; physical DMA uses original SAS;
gain has one native-buffer submission path. Removed failure injection applies
only to the removed OS volume call; remaining native calls retain injection.
All 193 registered names match executed names exactly on each width. No user
INI/media or Emulator/X86 production change is included. HEAD matches remote
after the executor push. S2 is delivered for owner acceptance, not closed;
T94 remains open for its separate goal-level acceptance audit.

- Driver loading: selected driver/resources recorded; no Code 24 after restart.
- DSP initialization: reset/version and observed initialization exchange pass.
- DMA/IRQ: the guest driver completes transfers and acknowledges interrupts.
- PCM output: first/repeated WAV produces non-silent PCM through the actual
  output path; native playback is separately verified, not inferred from mocks.
- Lifecycle: restart, pause/resume and compatible snapshot continuation pass.
- Regression: both widths, shared/nonshared units and required integrations pass.
- Capability limits: MIDI/FM results and unsupported capabilities stated honestly.

Each member needs concrete evidence or an owner-approved scope decision;
unexplained Code 24 or failed WAV playback blocks closure. A green unit suite
alone is not acceptance. Report production/test additions, deletions and net
change separately, plus XP-subset mirror delta and retained state ownership.
The final task audit maps all requirements to evidence before owner closure.
