# ui-base

`ui-base` is the shared UI foundation. It depends only on `types` and provides
copied frame/input values, source-local hotkey matching, and private mailbox
mechanics to `ui-window` and `ui-console`.

Its internal component emission helper may let a leaf filter matcher output,
but source attribution and chord matching always remain in this component.
This permits a frozen Window to discard ordinary content input while forwarding
registered hotkeys without creating a second matcher path.

`worker_interface.h`, `mailbox_interface.h`, `mailbox_wake_interface.h` and
the root `input_interface.h` are shared leaf-support contracts, not
application entry points. Each leaf owns its own state; Window capture is
Window-local. Outstanding suppressed keys survive subsequent matched chords
until their breaks. Repeats never add duplicate suppression entries; capacity
exhaustion fails delivery explicitly rather than overwriting held-key state.

No leaf includes anything under `ui-base/win32` or `ui-base/linux`. The root
input declarations expose copied input normalization with common surrogate,
recovery and delivery state. Same-shape platform operations decode raw keys and
text layout; Linux terminal text uses TEXT rather than inventing physical keys.
Window message decoding and key-state queries belong only to ui-window.
Mailbox wake operations remain selected by the build. Frame damage accumulates
until consumption even when intermediate complete pixel frames are replaced.

Mouse and close events never flush a keyboard prefix. Mismatch/keyboard release
replays pending keyboard events in order; keyboard/mouse interleaving is not
buffered. A rejected input sink closes the source and clears pending state;
only the worker's quiesced exit reports failure and retirement. STOP/fault closes
frame and control admission under the same short lock; FIFO processing up to
STOP is unchanged. Window's sole final filter discards ordinary frozen input,
not registered hotkeys or lifetime events.
