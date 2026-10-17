# M9 T95 S1 Windows 3.1 Playback Diagnosis

## Request and scope

Owner admits investigation of silent Canyon MIDI and repeated WAV playback
hangs after installing the Sound Blaster driver. The supplied checkpoint
follows a restart with audible startup Tada. Later clarification identifies
Chord stopping midway after the other WAVs, followed by hangs on subsequent
playback or Windows exit. Preserve owner media and configuration.

## S1 closure

Owner request on 2026-10-17: "批准收口S1和准入S2".
S1 delivery: 9df788229cb841a66ba74502ca81b20398eda0ce.
The coordinator reviewed the actual documentation-only delivery and the
[diagnostic evidence and repair design](../proposals/m9-win31-midi-wav-playback.md).
Production and tracked tests changed +0/-0; no EXEs changed.

- WAV: the service path can submit a pending DMA block after a pause. A bounded
  diagnostic guard supports the minimal repair design; it is not a release fix.
- MIDI: the initial Ad Lib destination requires unimplemented FM synthesis.
  Correctly committed MPU-401 mapping completes Canyon through the existing
  native MIDI path, followed by successful Chord playback and player exit.
- Unsupported SB-MIDI command/data collision is a distinct finding, transferred
  to TODO rather than silently included in the paused-transfer repair.

The S1 diagnosis/design criteria are satisfied and owner-accepted. S1 is closed.
Full production qualification remains an S2 obligation; T95 stays open.

## S2 admission

The same owner request admits S2. Its sole active packet is in
[CURRENT](../states/CURRENT.md). Implementation and qualification are pending
at admission; the accepted design adds only the paused-state service guard
and its owner-local regression, with no new state, interface or completion path.
