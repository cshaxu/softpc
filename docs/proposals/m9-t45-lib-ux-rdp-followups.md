# M9 T45 — Library UX follow-ups

## Purpose

Close the reset/pause/resume lifecycle repair, then repair the separately
reported raw-Console registered-hotkey path. SoftPC remains the owner of all
lifecycle and hotkey meanings and guest-input mapping.

## S1 — Reset, pause, and resume lifecycle

The owner reported that `reset -> resume` could strand the VM at BIOS and
`stop -> start` could fail.  The implementation in `010c401` makes public
standalone reset a cold reset while retaining the original guest hardware warm
reset path.  Its real executor proof covers `stop -> start -> running` and
`reset -> stop -> start -> pause -> resume -> running`.

S1 independently reviews that implementation against the task boundary,
records the focused x64/x86 evidence, refreshes the package only if a rebuild
is required, and closes without changing MVDM source or reinterpreting guest
hardware reset.

**Exit:** the cold-reset boundary and both completed lifecycle chains are
reviewed and evidenced for x64 and x86; the result is committed and pushed
with a clean worktree.

## S2 — Raw-Console registered hotkeys

When a VM raw Console is active, registered Ctrl+Alt+D, Ctrl+Alt+P, and
Ctrl+Alt+F may fail even though ordinary raw keys work. RDP is the reported
environment, not an assumed exclusive trigger. Win32 `INPUT_RECORD`
already supplies the per-record Ctrl/Alt state in `dwControlKeyState`; host
copies it into the platform-neutral `lib_console_raw_key.modifiers` field.
The current `ux-console` adapter discards that field, and the common Win32
normalizer instead asks process-global `GetKeyState()`.  That global state is
not a valid substitute for the source-local raw Console record under RDP.

Repair the common normalization API so the producing leaf supplies its
platform-neutral Ctrl/Alt/Shift mask.  `ux-console` must pass the copied raw
record mask; `ux-window` must retain its normal Window-message mask.  The
normalizer must set `ux_input_event.hotkey_modifiers` from that supplied mask,
not query global keyboard state.  Keep `ux_input_event.modifiers` as the
native guest-injection state (including `ENHANCED_KEY`), so this change does
not reinterpret guest input.  Registered chords remain source-local generic
UX behavior; SoftPC still decides what identifiers such as
`send-ctrl-alt-del` and `pause-toggle` mean.

Add a deterministic Win32 normalization/matcher probe proving raw-console
Ctrl+Alt+D and Ctrl+Alt+P become their respective registered hotkey events;
their Ctrl/Alt/D-or-P make and break transitions are fully suppressed.  Also
prove a Window transition continues to use its supplied modifier mask.  No
RDP session, sleeps, machine source, or product callback is required for that
proof.

**Exit:** raw Console no longer reads process-global modifier state for chord
matching; focused test and fresh x64/x86 build/test evidence pass; package
executables are refreshed; all changes are committed and pushed with a clean
worktree.

## Boundaries

- May change: `src/lib/{base,ux-base,ux-console,ux-window}/`, focused tests,
  CMake test registration if needed, current/task documentation, and the
  agent-owned executable pair.
- Must not change: `src/mvdm/softpc.new/**`, SoftPC lifecycle policy, hotkey
  identifiers or meanings, guest keyboard protocol, user-owned `softpc.ini`,
  or guest media.
