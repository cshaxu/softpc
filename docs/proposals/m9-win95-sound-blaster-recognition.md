# Windows 95 Sound Blaster Recognition And Playback Repair

## Owner request and queue boundary

> 好的，请你追加到队首一个T任务proposal，修复这个，确保win95能识别和正确使用我们新加的sound blaster模拟硬件。

This is the first unnumbered candidate in [Queue](../states/QUEUE.md).
It neither allocates a T number nor interrupts the active T93 packet.
[CURRENT](../states/CURRENT.md) remains the sole implementation authority.
The [XP sound capability proposal](m9-xp-sound-blaster-standalone.md) records
the imported device and host capability baseline, not Win95 driver acceptance.

## Observed failure and evidence limits

The owner manually selected Creative Labs Sound Blaster in Windows 95.
Device Manager reports Code 24: the device is absent, not working correctly,
or lacks a driver. Its resource page reports no conflicts and shows
0220-022F, 0388-0389 and IRQ 05. DMA is outside the supplied screenshot;
the assigned DMA channel and persistence after a guest reboot are unverified.
Do not turn these unknowns into claims that the owner configured it wrongly.

Source inspection identifies SB_VERSION=0x201 (SB 2.0/DSP 2.01), base 0x220,
IRQ 5 and DMA 1. Optional MPU attachment depends on host MIDI availability.
No root cause has been established. Code 24 alone does not distinguish DSP
reset/version failure, resource mismatch, IRQ/DMA failure or driver loading.
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
