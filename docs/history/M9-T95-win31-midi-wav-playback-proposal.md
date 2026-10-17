# Windows 3.1 MIDI Silence And Repeated WAV Hang

Retained T95 proposal. Owner delegated S2/T closure on 2026-10-17;
see the [completion audit](M9-T95-completion-audit.md). Earlier status statements
below record their historical checkpoints, not current active work.
The supplied screenshot selects port 220 and IRQ 5; these match the model.
Inspection confirms Sound Blaster 1.5; the initial MIDI destination is Ad Lib.
Owner supplies a checkpoint after driver installation, restart and audible
startup Tada playback. Preserve it as an input, not a mutable test fixture.

Owner refines reproduction: Windows 3.1 has four installed WAV files; the
other three play successfully first, then Chord stops midway. The system
subsequently becomes unstable and can hang while exiting Windows, which also
plays a WAV. Cover that sequence, not merely repeated identical playback.

## Sequential plan and finite ledger

S1 diagnoses two separate obligations: MIDI silence and repeated-WAV hang.
For each, record exact configuration, first divergence, source evidence and
dynamic evidence; unsupported FM must not be silently classified as a repair.
S2 is proposed for the evidenced minimal repair and deterministic regressions,
dual-width full qualification/publication and actual guest replay. It is not
automatically admitted by S1. T closure requires both obligations resolved or
an explicit owner-approved capability/scope decision, not only startup sound.

## S1 observation checkpoint

The supplied archive restores successfully, with matching base-media digest,
16 MiB RAM and overlay attachment containing 25 modified pages. Original inputs
are unchanged. Installed configuration confirms sndblst2.drv (Creative Labs
Sound Blaster 1.5), port 220, IRQ 5, and msadlib.drv. The four installed WAVs
are all 22050 Hz mono unsigned 8-bit PCM; Chord is not a format-switch outlier.

A native-output diagnostic linked from accepted objects opens the actual guest
Media Player using queued guest keys, not desktop automation. First Chord play
produces non-neutral PCM and increasing native consumed frames. Later repeated
plays cease completing normally, but subsequent Space inputs toggle pause;
this is not yet proof of whole-system hang. The owner confirms the observed
symptom matches and adds that later file playback/Windows exit can hang.
An abnormal-state checkpoint was saved. Saving leaves the machine paused;
later rejected input on that paused machine is diagnostic procedure, not
additional proof of a guest failure. The first probe shuts down cleanly.

A fresh restore plays Ding, Chimes, Tada then Chord to stopped state this time;
failure is not established as deterministic. Opening MIDI through the Sound
MCI device rejects the file; explicitly selecting MIDI Sequencer then opens
Canyon and reaches playing state. Bounded I/O observations show continued
OPL writes at 0388/0389. Current nt_sbfm.c has status compatibility only, no
synthesis; this explains silence on the observed FM route, not a PCM failure.
No new synthesizer or owner-media configuration change is admitted by this
finding. This was the initial checkpoint; subsequent evidence follows below.

### Paused-transfer and MIDI follow-up

Repeated native-output observations identify a copied, not yet submitted DMA
block surviving a pause command. Periodic DspServiceTransfer does not check
bDspPaused and can submit that block afterward. An ensuing completion occurs
while paused; the following playback is observed with IRQ5 masked and pending,
without the completion expected by the driver. This is evidence of the paused
handoff defect, not permission to force interrupt delivery or unmask the PIC.

A diagnostic-only gate on external service calls while paused completed twelve
Chord plays followed by twelve mixed WAV plays. Already scheduled completions
were not cancelled. No production change or release qualification is claimed.
The proposed narrow regression pauses during partial host acceptance, restores
host capacity, proves no further submission while paused, then continues using
the existing path. Existing coverage pauses only after full submission.

MIDI investigation in a disposable overlay installed the bundled MPU-401 driver
and observed reset/UART acknowledgements. Mapper editing alone did not establish
successful Canyon output: later playback still wrote repeated DSP command 38h
and MIDI bytes to the Sound Blaster port. The unsupported command leaves the
command parser unchanged; a following 91h byte selects DSP_WRITE_HS. The trace
then shows 5000 Hz PCM state and continuous FFh write-status reads. A paused CPU
inspection independently finds execution with DX=022Ch; the native source
returns busy whenever bHighSpeedMode is set. Thus this MIDI trial also induces
a concrete command-parser collision, distinct from the paused-WAV defect.

An additional trial edited all sixteen channel ports as MPU-401, exited to DOS,
restarted Windows and explicitly opened Canyon through Media Player's MIDI
Sequencer device. Playback still reached the same DSP 38h/91h collision. Thus
a missing Windows restart alone does not explain the routing. A checkpoint
before playback preserves the complete mapping for further inspection. Two
mistyped Run commands produced ordinary file-not-found dialogs and are excluded
from playback evidence; the later file-open and device selection were captured.

A read-only decode of the saved mapper file, using the packed legacy table
layout retained in Microsoft's mapper source, gives current setup index 5
(General MIDI). Its data still names Sound Blaster on all sixteen channels;
MPU names occur in another setup's data. This contradicts the earlier assumption
that the edited display proved the active on-disk route was changed. Recheck
the save/selection and guest-visible reopened record before attributing this
discrepancy to a device or CPU defect. The decoder changes no guest bytes.

The discrepancy was diagnostic procedure: Escape on the outer Mapper dialog
cancels its temporary-file transaction even after saving the inner setup edit.
Reopening the setup showed the old ports. The legacy Microsoft Mapper source
likewise commits its temporary file only when the outer dialog returns success.
Using the actual Close button instead preserves all sixteen MPU-401 routes,
confirmed independently by the saved file's active setup and channel records.
Canyon then enters playing and sends musical Note On/Off messages through the
existing MPU/host MIDI path; completed native calls inspected so far succeed.
Canyon subsequently returned to stopped without intervention. Opening Chord
through the Sound device then produced progressing PCM, returned to stopped,
and Media Player closed normally. This run uses the diagnostic paused-service
gate; it is not qualification of an unchanged or released executable. Native
restore/control-reset messages alone are not counted as musical output proof.
This neither implements FM synthesis nor adds SB-MIDI support. The unsupported
SB-MIDI command/parser collision is a distinct limitation requiring an explicit
scope decision, not an excuse to route MIDI bytes silently into PCM commands.
Original media/configuration/checkpoint and published EXEs remain unchanged.

### Minimal repair design for owner admission

The WAV production candidate is a paused-state check in the existing
DspServiceTransfer early return in host/src/nt_sbdsp.c, not a new interrupt,
timer, queue or host-audio path. Leave already armed completions intact and
retain the existing ContinueDMA flag transition. Expected production logic
change is one replaced condition (+1/-1, net zero), plus a brief local reason
comment and mirror-accounting updates. The existing device-contract test owns
the mock backpressure regression: pause with only seven bytes accepted, restore
capacity, assert no extra submission/completion while paused, continue, and
assert original bytes/address/count/IRQ behavior in both DMA directions.
Expected test change is approximately +15 to +30 lines without new fixtures.
Also retain full-submission pause, reset and snapshot regressions; qualification
must replay the four-WAV sequence and repeated Chord on both published widths.

MIDI requires the bundled Roland MPU-401 driver at the existing 330h endpoint
and an active Mapper setup whose sixteen channels select that output. Commit
the outer Mapper dialog using Close, not Escape. This uses current hardware
and native MIDI delivery; it does not require new Lib code or synthesis.
The owner image remains untouched; any delivered configured media is a separate
explicit asset decision. Adding SB-MIDI command support or FM synthesis is not
part of this minimal repair and must not be smuggled in as configuration work.

## Owner observation

After sound-driver installation, startup sound works. Media Player playing
the installed Canyon MIDI is silent; playing multiple WAV files exhibits a
hang. Owner places this first in Queue after T94 closure. Neither root cause
nor whether these observations share a cause is established.

## Investigation and repair boundary

Record driver/version, MIDI destination and exact playback sequence on disposable
media/overlay. Determine whether MIDI targets FM or external MIDI; trace guest
commands and host output. Existing unsupported FM is a capability limitation,
not a proven explanation. Distinguish application wait, guest execution hang,
DSP/DMA/IRQ completion and host stream lifetime during repeated WAV playback.
Use bounded probes/snapshots, not Computer Use. Preserve original single device
paths; no forced completion, guest special case or speculative Lib rewrite.

## S1 delivery review

Executor: production +0/-0, tracked tests +0/-0. The bounded diagnostic work
establishes an actionable paused-submission repair and verifies a working MPU
route without adding hardware or a synthesizer. Published EXEs are unchanged;
the implementation build and full regression are deliberately not claimed.

Coordinator: reviewed the actual three documentation paths against the S1
packet. Discarded/cancelled Mapper edits are explicitly excluded from successful
configuration evidence. Full Canyon completion, post-MIDI Chord and normal
player closure were observed only in the diagnostic build. The final probe
reports shutdown/destroy/VM destroy success and exit zero. Documentation gate
and whitespace checks pass. Owner checkpoint remains untracked and unmodified.

The finite S1 ledger is: WAV diagnosis/design evidenced, repair qualification
pending S2; MIDI original FM limitation evidenced, existing MPU route evidenced,
owner media unchanged; SB-MIDI parser collision evidenced, new capability scope
not admitted. Owner explicitly accepted and closed S1 on 2026-10-17 after
delivery commit 9df788229cb841a66ba74502ca81b20398eda0ce. The closure review
confirms that this is diagnosis/design acceptance, not production qualification.

## S2 admission

Original owner request: "批准收口S1和准入S2". S2 is admitted on 2026-10-17;
CURRENT contains its sole executable packet. Implement the existing service
guard described above, without a diagnostic wrapper or additional state.
Qualification must cover partial-acceptance pause/continue in both DMA
directions and preserve already armed completion behavior. The finite repair
ledger requires: paused submission fixed and tested; existing completion/reset/
snapshot behavior retained and tested; repeated guest WAV replay qualified;
existing MPU route replay qualified; unsupported FM/SB-MIDI not implemented.
Native output availability is reported separately from mock unit proof.
The independent SB-MIDI parser collision is transferred to TODO for separately
admitted capability work. Original inputs are not changed by admission.

## Repair exit evidence

### S2 implementation checkpoint

The deterministic regression fails against the unmodified implementation on
both x64 and x86 at the paused partial-submission assertion: returning host
capacity permits more than the original seven accepted bytes. The failure
occurs through the existing DSP command and service paths, not a synthetic
alternate implementation. The repaired SB DMA/command/archive trio passes on
both widths. Qualification completed with exit zero and 193/193 distinct cases
per width: 177 unit/gates, eight background integration, five native desktop,
and three native audio. Per-phase JUnit names were checked for uniqueness and
failure counts; repeated focused cases are not counted twice. Guest replay is
proved separately below, not inferred from these totals. Both red logs remain.

The rebuilt x64 guest probe contains no DspServiceTransfer wrapper and no
diagnostic pause gate. From the original owner checkpoint, Ding, Chimes, Tada,
then Chord each return to stopped with non-neutral native PCM and progressing
consumed frames. Twelve more Chord plays each return to stopped; their captured
frames were individually inspected. The same sequence, twelve further Chord
plays, player closure, Windows exit to text mode and typing win to return to
Program Manager also pass on x86. Both probes shut down and destroy their
machine/VM with status zero. Native backpressure (LIMIT_EXCEEDED) occurs during
the x86 sequence and subsequent playback completes. Original image and
checkpoint SHA-256 identities remain unchanged.

The repaired x64 build also restores the S1 disposable MPU-configured archive,
plays Canyon to natural stopped state, plays Chord afterward to stopped, and
closes the player normally. The bounded MIDI logger records 10,000 complete
short-message calls, including musical Note On/Off, with status zero; later
calls exceed the logger cap and are not individually claimed inspected. Full
song completion is independently visible in the captured player state. Probe
shutdown/destroy/VM destroy and process exit are zero. The x86 counterpart also
finishes Canyon naturally. Its first subsequent Chord capture was still playing
when the player was closed, so that attempt is not counted as natural-completion
proof. Reopening Chord, playing and waiting yields an explicit stopped capture;
player closure and shutdown/destroy/VM destroy then succeed with exit zero.
The x86 bounded log has 8,980 complete parsed MIDI records with status zero;
fragmented output records are excluded rather than assumed successful. The
original owner media still selects its unchanged original MIDI route.

Current tracked code changes: nt_sbdsp.c +2/-1 (one guard term and one reason
comment), device_contract_smoke.c +12/-0. The test covers incrementing and
decrementing DMA, repeated service while paused, unchanged DMA address/count
and no new IRQ, followed by continued submission of the original bytes. Existing
fully submitted block pause/EOI checks remain. No state, callback, queue or
snapshot format is added. The current mirror manifest updates only the local
body identity; upstream/subset hashes and selection remain unchanged.
The retained selected XP body was verified against its manifest hash
c6829e9f48df70ea9cd8b0761bb60927b0c3bfe368e6cddc6406e20e93ca3340
before comparison. `git diff --no-index --numstat` gives the current total
local patch as +835/-1462 against that subset; this includes the earlier host
adaptation and must not be presented as the size of this two-line repair.

Similar-issue source sweep (`DspServiceTransfer|DspProcessBlock` throughout
App-SoftPC C/H): DspProcessBlock has only the service caller. Fresh start clears
pause before service; auto-init EOI already rejects paused state; machine resume
and periodic SbService now reach the same guarded entry. Initialization also
uses that entry after reset. Snapshot restores the existing pause bit and
pending bytes; no new archive field is needed. DspCompleteBlock remains
unchanged, so already scheduled completions are not cancelled by guest pause.
These are source conclusions; runtime proof is reported separately.

Owned qualification job: ignored build/t95/s2, qualification.log/.err and
qualification.exit, serial architecture builds with eight-way unit/background
testing and serial native cases. Overall observation budget is two hours,
no-progress investigation threshold ten minutes; per-test configured timeouts
remain unchanged. The executor owns process inspection and cleanup. The x86
red build again reports the already-recorded Ninja log recovery warning; it is
not presented as a new build-system repair or a warning-free run.

### S2 delivery audit

Executor actual-diff audit: one production owner and one existing test changed,
plus the local mirror digest, governance evidence and both rebuilt launchers.
Production +2/-1, net +1; tests +12/-0, net +12; total code +14/-1, net +13.
No Lib/Emulator/X86 code, configuration, firmware, guest image or snapshot is
changed. The retained single service path owns the pause check; no obsolete
parallel production route is added or left behind by this repair.

Finite acceptance ledger: paused partial submission has dual-width red/green
proof; existing completion/reset/snapshot contracts pass the complete suites;
both guest widths pass four WAVs, twelve further Chord plays and Windows exit/
restart; both pass full Canyon via MPU and subsequent WAV; both launchers are
rebuilt; original inputs retain their identities. FM and SB-MIDI remain explicit
unsupported capabilities, with the separate parser collision assigned to TODO.
The qualification job and all four guest probes have terminated normally.
Retain their bounded evidence until owner acceptance; no diagnostic process is
left running. S2 awaits owner acceptance, and T95 is not closed by this delivery.

For owner MIDI testing, retain the working Sound Blaster WAV driver. Install
the bundled Roland MPU-401 driver at port 330h using its IRQ 2/9 selection,
then select a MIDI Mapper setup whose sixteen ports use Roland MPU-401.
Save its inner edit and use the outer Close button to commit (Escape cancels).
Open Canyon through Media Player's MIDI Sequencer. This is configuration of
the already supported route, not a promise that the original Ad Lib selection
will become audible merely by replacing the executable. Owner media is not
reconfigured by this delivery.

### S2 post-push coordinator review

Reviewed actual pushed commit 66055d92fa464933ddf442dc50fff4d880093f80,
not only its executor summary. Its seven paths match the admitted boundary:
one guard, existing device test, local digest, two documents and two launchers.
No change to completion callbacks, interrupt masks, machine state ownership or
snapshot layout is hidden in the commit. Rechecked full qualification exit zero,
193 distinct passing cases without skips per architecture, all four guest-run
zero exits, and the published hashes in CURRENT. Original media/configuration
remain unchanged; the owner checkpoint remains untracked and preserved.
No task-owned diagnostic process remains. Existing mirror compiler warnings
and Ninja recovery are disclosed, not claimed fixed. This review qualifies the
delivery for owner testing; it does not close S2 or T95 without acceptance.

On admission, diagnose and estimate the minimal owner-local repair before edits.
Add deterministic regressions with mock audio, then actual guest MIDI-route and
repeated-WAV verification. Report native availability separately; any capability
expansion needs explicit approval. Build/publish both EXEs, run prescribed full
regression, review actual diff, commit/push and await acceptance. Preserve user
configuration, media and snapshots.
