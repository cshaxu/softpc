# Current

## M7 T5 S1: Original Speaker Host Endpoint Completion

- **Owner and approval:** Codex is the sole implementer and reviewer under
  the approved single-person dual-role mode; M7 T4 has closed.
- **Objective:** audit and complete the standalone endpoint for original
  `nt_sound` and PIT/PPI Timer-2 speaker state transitions.
- **Input/output boundary:** measurements, host playback endpoint, optional
  runtime configuration, and tests may change. Original `nt_sound`, PIT, PPI,
  timer scheduling, BIOS, BOP, and speaker state remain sole owners.
- **Focused verification:** prove the original control transitions reach the
  endpoint, with bounded latency and clean lifecycle on reset/destruction.
- **Full regression requirement:** clean x64/x86 build and serial CTest.
- **Similar-issue sweep:** test no sound request, repeated tone transition,
  endpoint failure/unavailability, reset and stop without guest-tick synthesis.
- **Stop condition:** stop if a proposal needs a sound-card model, mixer
  policy, a second guest clock, or replacement of original sound state.
- **Exit criteria:** dual-width evidence establishes the original speaker
  controller's endpoint lifecycle, or proves the existing bounded endpoint is
  sufficient without a replacement.
