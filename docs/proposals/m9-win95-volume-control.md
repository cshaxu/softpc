# Windows 95 Unavailable Volume Control

## Status

Owner admits T97 S1 on 2026-10-17 after T96 acceptance: "works! 好了收口本S和本T，准入下一个T任务".
CURRENT owns its active packet; this proposal retains the investigation plan.
No causal finding or production repair is claimed yet. Baseline is 5ea4e55e.

## Owner observation

Startup sound works but Multimedia Properties' playback volume slider and
taskbar volume-control checkbox are disabled. This became the next Queue head
after the previously admitted playback and frame work.

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

## Admitted investigation plan

- S1: reproduce and audit guest driver/model/mixer capabilities; distinguish
  missing guest software/configuration from model limits and emulation defects.
  Return a causal explanation and minimal repair design with per-file diff
  estimate, or an evidenced capability limitation requiring owner direction.
- S2, subject to findings and admission: implement the approved smallest
  correction, focused mocked tests and actual guest verification, cached dual
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
5. Classify first divergence, responsible owner and smallest admissible change.
6. Specify focused proof, full verification, compatibility effects and diff.

Each member needs direct evidence or an explicit limitation. Restart-at-safe-
shutdown remains a separate task. Admission changes documentation only; no
production/test diff or EXE rebuild is claimed before S1 findings.
