# ui-base

`ui-base` is the shared UI foundation. It depends only on `types` and provides
copied frame/input values, source-local hotkey matching, and private mailbox
mechanics to `ui-window` and `ui-console`.

Its internal component emission helper may let a leaf filter matcher output,
but source attribution and chord matching always remain in this component.
This permits a frozen Window to discard ordinary content input while forwarding
registered hotkeys without creating a second matcher path.

`worker_interface.h`, `mailbox_interface.h`, `mailbox_wake_interface.h` and
the root `input_interface.h` and `actions_interface.h` are shared leaf-support contracts, not
application entry points. Each leaf owns its own state; Window capture is
Window-local. Outstanding suppressed keys survive subsequent matched chords
until their breaks. Repeats never add duplicate suppression entries; capacity
exhaustion fails delivery explicitly rather than overwriting held-key state.

No leaf includes anything under `ui-base/win32` or `ui-base/linux`. The root
input/actions declarations expose copied Windows record conversion and a
key-state query; only their implementation lives in `win32`. Existing Linux
input helpers stay local, without invented parity or a forwarding header.
The same-shape mailbox wake contract remains selected by the build.
