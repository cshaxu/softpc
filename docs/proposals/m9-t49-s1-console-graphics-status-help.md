# M9 T49 S1 — Console graphics status help reuse

## Original request

For `display=console` with `console_control=0`, a running graphical frame
retains the raw VM Console. Its status text must keep the existing first line,
then one blank line, then the hotkey section already printed by monitor `help`,
then one trailing blank line. Do not duplicate the hotkey text.

## Objective

Make `app/command.c` own two reusable monitor-help constants: command help and
raw-VM-Console hotkey help. The monitor's full help composes both constants;
the graphical raw-Console status frame composes its existing first line and
the same hotkey constant.

## Boundaries

- Product text and composition remain in `src/app/`.
- `lib`, `host`, and `mvdm/softpc.new` do not change.
- The existing raw-Console selection, frame routing, and hotkey semantics do
  not change.

## Verification

- Add a unit assertion for the reusable hotkey section and the existing full
  help path.
- Fresh x64/x86 build and full CTest.
- Refresh both package EXEs; run `git diff --check`; commit and push.

## Exit criteria

The raw graphical Console status reads as one first-line status, a blank line,
the monitor's exact hotkey section, and a final blank line; there is one source
of truth for those hotkey lines.
