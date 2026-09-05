# M9 T36: Shared Win32 Presentation Library

## Outcome

Closed. `src/lib/platform/win32/` now owns the reusable copied-frame mailbox,
generic input queue, RDP-safe input normalizer, registered action map, mouse
capture, display router, private-console presenter, and window presenter.
`src/app` is only the SoftPC product binding: it publishes a safe copied
machine frame, maps generic events to the original key/InPort entry points,
registers Ctrl+Alt P/D/F/M, supplies product titles, and decides pause/close
lifecycle effects.

`WINDOW` remains window-only. `CONSOLE` uses the common router to present text
in the console, graphics in a window, and stable text return in a fresh
console. The reusable library names no `app_runtime`, `softpc_machine`,
`KeyMsgToKeyCode`, renderer plane, BOP, or guest controller state.

## Scope

- Moved the former window loop with `git mv` into the shared component and
  extracted the console loop there.
- Added opaque mailbox/event queue, generic host events, action registry,
  binding ABI, and complete console/window presenter loops.
- Made package smoke accept the user-supported fixed hard-disk-only layout;
  an optional floppy remains validated when configured.
- Preserved the owner change that comments out the default floppy in the
  adjacent user-owned INI. No agent rewrote its configuration.

## Verification

- GCC x64: full CTest, 23/23 passed.
- GCC x86: full CTest, 23/23 passed.
- Documentation governance, source boundary, synchronized manifest, package
  launch, Win32 window/RDP/keyboard/mouse smoke all pass in both runs.
- `src/mvdm/softpc.new` has no task diff.

## Non-goals Kept

No original machine, C-VID, VGA, ROM, BOP, media, or guest timing behavior was
changed. The shared component does not own guest protocol encoding, product
hotkey meanings, or lifecycle policy.
