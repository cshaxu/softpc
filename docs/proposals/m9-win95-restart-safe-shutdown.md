# Windows 95 Restart Stops At Safe Shutdown

## Owner observation

Choosing Yes to restart after sound-driver installation reaches the safe-to-
turn-off screen and stays there. Owner places this third in Queue.

## Investigation and repair boundary

The screen proves restart did not complete, not which boundary failed. Existing
keyboard-controller/CPU reset paths and T94's successful observed guest restart
do not establish this dialog's behavior. Trace guest restart intent, reset
commands, CPU reset entry and BIOS progress. Compare normal and installation-
requested restart using disposable media/checkpoints. No automatic reboot on
screen detection, Windows-specific branch or assumed APM diagnosis.

## Exit evidence

Identify the first failed contract and present a minimal single-path repair,
including mirror delta if needed. Add deterministic owner-local regressions;
verify guest restart, sound rebinding and distinct normal-shutdown semantics.
Use probes, not Computer Use; preserve user media/configuration. Dual builds,
publication, prescribed regression, review and commit/push precede acceptance.
