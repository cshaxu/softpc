# M9 T47 — Frozen Window hotkey recovery

## S1 — Preserve registered Window hotkeys while guest input is frozen

## Request

When a Window is frozen for a paused VM, Ctrl+Alt+P is currently discarded
before `ux-base` can recognize it. The paused SoftPC control gate also rejects
the resulting `UX_EVENT_HOTKEY`. The owner requires registered hotkeys to
remain available for product control, especially pause-toggle recovery, while
ordinary guest keyboard and mouse input remains unavailable.

## Objective

Keep hotkey recognition generic and source-local in `ux-base`. A frozen
`ux-window` must submit native key transitions through that matcher, then
silently consume every ordinary output and forward only a matched
`UX_EVENT_HOTKEY` to its supplied SoftPC sink. It must continue to discard
ordinary text and mouse input while frozen.

SoftPC must admit matched UX hotkeys for the current paused run. Its product
layer remains the only interpreter: `pause-toggle` requests resume;
guest-input-producing hotkeys such as CAD and CAF are consumed while paused
and may not write either synthetic or ordinary input to the VM input queue.
Cooked monitor commands remain a separate input family and remain available
while paused.

## Non-goals

- Do not change `src/mvdm/softpc.new/**`, guest timing, or VM pause semantics.
- Do not give lib hotkey identifiers product meaning or access to the SoftPC
  control queue.
- Do not make ordinary frozen Window input pending for later replay.
- Do not change monitor command parsing, host Console ownership, package
  configuration, or guest media.

## Boundaries and design

`ux_component_emit()` currently performs source attribution and generic matcher
submission in one operation. Add an `ux-base` internal delivery variant with
the same attribution/matcher path but an explicitly supplied post-matcher
delivery callback. `ux-window` alone uses it for native key transitions: when
frozen its callback returns success after discarding ordinary matcher outputs
and forwards only `UX_EVENT_HOTKEY` to the application sink. This is not a
second matcher and must not duplicate suppression or mismatch replay.
`ux-console` retains the normal default delivery path.

SoftPC continues to use one copied control FIFO. Its paused admission gate
allows current-run hotkey events, but the product handler consumes all
non-lifecycle hotkeys in paused state before they reach the guest adapter.

## Verification

- Extend the `ux-base` component contract test to prove the post-matcher
  delivery callback receives a matched hotkey but can silently consume normal
  key events, including their mismatch replay, with source identity intact.
- Extend the SoftPC control/reconciler integration test: a paused current-run
  `pause-toggle` hotkey is admitted; ordinary key make remains rejected;
  stale-run hotkeys remain rejected.
- Review the Window native boundary: frozen key transitions use the matcher
  while `WM_CHAR` and mouse remain guest-input gated.
- Run focused tests, full CTest for x64 and x86, refresh both package
  executables, run documentation-governance verification, and sweep every
  paused hotkey route for guest-input injection.

## Exit

Frozen Window CAP reaches the SoftPC control queue and can request resume;
ordinary frozen Window keyboard/text/mouse input never reaches the VM; paused
CAD/CAF cannot inject guest keys; cooked monitor commands remain independent;
both widths pass, executables are refreshed, and all task changes are committed
and pushed from a clean worktree.
