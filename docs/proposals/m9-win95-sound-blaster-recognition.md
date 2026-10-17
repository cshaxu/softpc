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
