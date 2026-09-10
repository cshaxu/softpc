# M9 T48 S6 — Monitor completion transaction

## Outcome

Control classifies every lifecycle outcome from completed runtime facts and the
prior stable control state.  Monitor command and UX hotkey origins are not part
of classification.  The command transaction stores one pending outcome and,
only after monitor is actual Current Console, emits outcome text, `SoftPC> `,
and one cooked-line arm request in that order.

Raw VM Console ownership clears pending monitor-only running outcomes, so a
later handoff cannot replay old `Machine started.` or `Machine resumed.` text.
Error completes through the same prompt-safe path.

## Proof

- Focused x64/x86 command/reconciler/control tests pass.
- Tests cover hotkey-origin resumed completion, raw-route discard/no replay,
  error prompt transaction, and the full monitor lifecycle matrix.

## Closure

S6 closes with `646fffd`. S7 owns presentation/broker completion hardening.
