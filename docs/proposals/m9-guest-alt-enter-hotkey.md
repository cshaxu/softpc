# M9: Guest Alt+Enter Hotkey

## Objective

Reserve `Ctrl+Alt+F` in both standalone frontends and inject the guest-keyboard
sequence `Alt+Enter`.  The hotkey is a frontend command only: it must never
toggle a host fullscreen mode or alter the window, monitor, machine profile,
guest timing, BIOS, BOP, or controller behavior.

## Candidate Work

- Add the same chord recognition to the monitor console and Win32 window
  input paths.
- Consume the host chord and enqueue guest left-Alt and Enter make/break scan
  codes through the existing single runtime input path.
- Preserve `Ctrl+Alt+P` pause/resume, `Ctrl+Alt+D` Ctrl+Alt+Del, and
  `Ctrl+Alt+M` mouse release exactly as they are.
- Rewrite the monitor-console help so every `Ctrl+Alt` command is shown on
  its own line, including the new `Ctrl+Alt+F` guest Alt+Enter action.
- Keep existing focus and capture rules: only the active frontend may issue
  the command; paused input behavior remains unchanged.

## Verification

1. Unit-test the emitted guest scan-code order, including releases.
2. Test console and window chord handling independently, asserting that the
   host chord does not leak as guest Ctrl/Alt/F input.
3. Assert monitor help contains one line each for `Ctrl+Alt+P`,
   `Ctrl+Alt+D`, `Ctrl+Alt+M`, and `Ctrl+Alt+F`.
4. Run GCC x64 and x86 full CTest plus package smoke.

## Exit

The focused guest receives Alt+Enter from either frontend, host window state
does not change, all existing special hotkeys retain their behavior, and both
host widths pass regression.
