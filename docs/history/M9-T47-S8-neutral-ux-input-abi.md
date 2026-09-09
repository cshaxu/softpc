# M9 T47 S8 — Neutral shared UX input ABI

## Outcome

`ux-base` now owns the copied, platform-neutral UX keyboard contract:

- `ux_key` names printable and non-printable key identities without reusing
  host virtual-key values;
- generic Ctrl/Alt/Shift modifiers and `UX_KEY_FLAG_EXTENDED` replace public
  Win32 control-state semantics;
- the private Win32 adapter translates native records before a UX sink sees
  them, while the Linux adapter produces the same public record;
- SoftPC performs the single inverse conversion only at its existing private
  guest-keyboard binding immediately before `KeyMsgToKeyCode`.

Hotkey matching, Window input, Console input, synthetic CAD/CAF, and the
runtime diagnostic therefore all retain the single existing UX input route.
MVDM is unchanged.

## Evidence

- Focused keyboard coverage proves ordinary, scan-less RDP-style Return, and
  extended Right navigation translate to neutral identities and flags.
- Existing runtime and reconciler tests prove the neutral event reaches the
  unchanged guest-keyboard continuation path.
- The source boundary rejects `VK_*`, `ENHANCED_KEY`, and
  `KEY_EVENT_RECORD` in the public `ux-base` event header.
- The shared manifest and documentation governance checks pass.
- Fresh x64 and x86 package builds each passed full CTest, 33/33.

## Closure

This is an ABI tightening, not a new queue or policy layer. Platform-native
values remain confined to platform adapters; product guest protocol remains
confined to the application binding. T47 remains active; a later subtask must
be separately admitted.
