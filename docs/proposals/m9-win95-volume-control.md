# Windows 95 Unavailable Volume Control

## Owner observation

Startup sound works but Multimedia Properties' playback volume slider and
taskbar volume-control checkbox are disabled. Owner places this second in Queue.

## Investigation and repair boundary

The model reports SB 2.0/DSP 2.01 and contains original CT1335 mixer registers.
PCM playback does not prove driver volume-control exposure. Query installed
driver capabilities and trace mixer exchanges to distinguish an authentic
model/driver limitation from a hardware contract defect. No dynamic causal
proof currently exists. Do not spoof SB16 identity or force UI flags; host
PCM gain alone cannot establish guest mixer capability.

## Exit evidence

Provide a reproducible capability explanation and minimal repair if defective,
with deterministic regressions and actual guest verification. If new hardware
capability is required, seek approval rather than claim a fix. Use disposable
media and bounded probes, not Computer Use. Dual builds/publication, prescribed
regression, diff review and commit/push precede owner acceptance.
