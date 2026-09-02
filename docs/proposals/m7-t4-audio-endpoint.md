# M7 T4: Original Speaker Host Endpoint Completion

## Goal

Improve the standalone host audio sink while preserving original `nt_sound`
and PIT/PPI Timer-2 state transitions.

## Dependency

M7 T3 must close first.

## Scope

- Measure the current speaker endpoint's latency, continuity and shutdown.
- Replace only the final host playback endpoint if evidence shows `Beep` is
  insufficient for the fixed profile.

## Exclusions

No sound-card emulation, guest timer synthesis, mixer policy, or replacement
of original sound state-machine logic.

## Exit Evidence

Dual-width sound regressions prove original control transitions and clean
lifecycle behavior; any new endpoint is bounded and optional at runtime.
