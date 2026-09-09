# M9 T47 S8 — Neutral shared UX input ABI

## Objective

Make `ux_input_event` a platform-neutral copied ABI. Shared UX must not expose
Windows virtual-key values or Win32 injection flags to an importing product.
This is one existing UX-input route with stricter values, not a second input
path and not a change to MVDM.

## Baseline and gap

`ux-base/event.h` currently calls the key identity `virtual_key` and exposes a
`modifiers` value documented as native guest-injection state. The Win32
adapter writes `VK_*` and `ENHANCED_KEY`; `app/keyboard.c` copies them into a
Win32 `KEY_EVENT_RECORD`. Linux already emits a separate partial set of
neutral special-key identities. Thus the public ABI has no Windows type, but
does have Windows numeric semantics.

## Intended contract

`ux_input_event.key` becomes one neutral, copied physical-key record:

- `key`: a `ux_key` identity defined entirely by `ux-base`. It covers every
  key admitted by the current Windows adapter: printable keys, modifiers,
  navigation, keypad, locks, function, and system keys. Printable identities
  use their Unicode ASCII scalar where applicable; non-printable identities
  occupy explicit `UX_KEY_*` ranges and do not equal `VK_*` values.
- `scan_code`: the retained physical scan value when the host has one; zero
  means unavailable, as for a terminal.
- `flags`: neutral injection facts. Initially the only defined bit is
  `UX_KEY_FLAG_EXTENDED`; no public header names `ENHANCED_KEY` or a Win32
  control-state value.
- `modifiers`: the generic current Ctrl/Alt/Shift mask used by generic chord
  matching. Its values are defined by `ux-base`, not by an adapter.
- `pressed`: make/break. Unicode remains a separate `UX_EVENT_TEXT` scalar.

`ux-window` and `ux-console` continue to call the same `ux-base` construction
path. The Win32 adapter translates native messages/records into the neutral
record before invoking a sink. The Linux adapter emits that same record.

SoftPC remains the sole guest-protocol binding: `app/keyboard.c` maps
`ux_key` plus `UX_KEY_FLAG_EXTENDED` into its private Win32
`KEY_EVENT_RECORD` immediately before its existing `KeyMsgToKeyCode` call.
Synthetic CAD/CAF events use the neutral constructors too. No shared API
mentions SoftPC, a product queue, `KEY_EVENT_RECORD`, `VK_*`, or
`ENHANCED_KEY`.

## Scope and proof

- May change `src/lib/ux-base`, `ux-window`, `ux-console`, the external
  SoftPC app binding, shared manifest, focused tests, and task/UI/architecture
  documentation.
- Must not change `src/mvdm/softpc.new`, add an input queue, add a product
  callback, or alter registered-hotkey semantics.
- Add deterministic table tests for Windows-native-to-neutral and
  SoftPC-neutral-to-Win32 mappings, including extended navigation keys,
  Ctrl/Alt/Shift, make/break, text, and unsupported values.
- Extend the source boundary to reject `VK_*`, `ENHANCED_KEY`, and Windows
  control-state numeric semantics from public `ux-base` headers.
- Require fresh x64/x86 full CTest and package builds. The app's private
  Win32 adapter is permitted to use Win32 values; that is its defined binding
  boundary.
