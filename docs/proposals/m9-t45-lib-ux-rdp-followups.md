# M9 T45 — Library UX follow-ups

## Purpose

Close the owner-validated pre-admission interaction repair without rewriting
its already-committed implementation history, then repair the separately
reported RDP raw-Console registered-hotkey path.  The work is limited to the
shared host/UX library and its deterministic probes.  SoftPC remains the
owner of all hotkey meanings and guest-input mapping.

## S1 — Owner-validated interaction closeout

The owner reports the immediately preceding interaction defect resolved.  The
working tree contains no uncommitted implementation for that repair: its code
and package evidence were already committed before this T45 admission.  S8
therefore records the acceptance and leaves the source tree untouched.  It
does not re-label or duplicate the earlier task's code commit.

**Exit:** this P1 record is committed and pushed; the worktree remains clean.

## S2 — RDP raw-Console registered hotkeys

When a VM raw Console is active through RDP, registered Ctrl+Alt+D and
Ctrl+Alt+P may fail even though ordinary raw keys work.  Win32 `INPUT_RECORD`
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
