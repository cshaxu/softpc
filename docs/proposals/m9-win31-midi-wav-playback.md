# Windows 3.1 MIDI Silence And Repeated WAV Hang

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

## Exit evidence

On admission, diagnose and estimate the minimal owner-local repair before edits.
Add deterministic regressions with mock audio, then actual guest MIDI-route and
repeated-WAV verification. Report native availability separately; any capability
expansion needs explicit approval. Build/publish both EXEs, run prescribed full
regression, review actual diff, commit/push and await acceptance. Preserve user
configuration, media and snapshots.
