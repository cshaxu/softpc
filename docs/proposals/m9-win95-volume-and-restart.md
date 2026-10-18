# Windows 95 Volume Control And Restart Investigation

## Status

Owner admits T97 S1 on 2026-10-17 after T96 acceptance: "works! 好了收口本S和本T，准入下一个T任务".
CURRENT owns its active packet; this proposal retains the investigation plan.
No causal finding or production repair is claimed yet. Baseline is 5ea4e55e.
Owner subsequently directs: "本项和队列剩余一项合并调查吧". The queued
safe-shutdown restart investigation is merged into T97 S1, without allocating
another T/S. Combined investigation does not presume a shared cause.

## Owner observation

Startup sound works but Multimedia Properties' playback volume slider and
taskbar volume-control checkbox are disabled. This became the next Queue head
after the previously admitted playback and frame work.

Separately, choosing Yes to restart after sound-driver installation reaches
the safe-to-turn-off screen and stays there. This was originally the third
queued candidate and is now part of the same investigation.

## Investigation and repair boundary

The model reports SB 2.0/DSP 2.01 and contains original CT1335 mixer registers.
PCM playback does not prove driver volume-control exposure. Query installed
driver capabilities and trace mixer exchanges to distinguish an authentic
model/driver limitation from a hardware contract defect. No dynamic causal
proof currently exists. Do not spoof SB16 identity or force UI flags; host
PCM gain alone cannot establish guest mixer capability.

For restart, that screen proves restart did not complete, not which boundary
failed. Existing keyboard-controller/CPU reset paths and T94's successful
observed guest restart do not establish this dialog's behavior. Trace guest
restart intent, reset commands, CPU reset entry and BIOS progress. Compare
normal and installation-requested restart using disposable media/checkpoints.
No automatic reboot on screen detection, Windows-specific branch or assumed
APM diagnosis. Preserve distinct normal-shutdown semantics and check sound
rebinding after a genuine restart.

## Exit evidence

Provide a reproducible capability explanation and minimal repair if defective,
with deterministic regressions and actual guest verification. If new hardware
capability is required, seek approval rather than claim a fix. Use disposable
media and bounded probes, not Computer Use. Dual builds/publication, prescribed
regression, diff review and commit/push precede owner acceptance.

## Admitted investigation plan

- S1: investigate volume controls and restart together, with separate evidence
  and causal conclusions. Distinguish driver/software/model limits from device
  defects, and guest shutdown intent from a failed reset. Return minimal repair
  designs with per-file diff estimates or evidenced limitations requiring direction.
- Subsequent S tasks, subject to findings and admission: implement approved
  corrections separately by responsible boundary unless a common cause is proved;
  focused mocked tests and actual guest verification, cached dual
  builds/publication and required full regression; actual-diff review and push.

S1 does not authorize spoofing SB16, a new mixer model, forcing guest UI flags,
or modifying the user's disk/INI/snapshot. Preserve the existing PCM path.
Native host gain is not proof of a guest-exposed mixer capability. Use
task-owned overlays and bounded probes without Computer Use. Long diagnostics
need owned hidden jobs/logs/exits; observation timeout is not job failure.

## Finite S1 investigation ledger

1. Reproduce disabled controls and working PCM in the same guest state.
2. Identify installed driver/model and advertised volume/mixer capabilities.
3. Trace guest requests through original DSP/mixer registers and Compat.
4. Check volume-control application installation and guest configuration.
5. Reproduce both ordinary restart and installation-requested restart; retain
   normal shutdown as the control case.
6. Trace guest intent, controller reset requests, CPU reset and BIOS progress;
   identify the first divergent stage without inferring it from the screen.
7. Verify subsequent audio rebind separately from the restart result.
8. Classify each symptom's first divergence, responsible owner and minimal change;
   state whether a common cause has evidence or the causes are independent.
9. Specify focused proof, full verification, compatibility effects and diff
   for each proposed correction, including mirror delta when relevant.

Each member needs direct evidence or an explicit limitation. Neither issue is
complete merely because the other is resolved. Scope amendment changes documentation only; no
production/test diff or EXE rebuild is claimed before S1 findings.
