# M9: Shared Win32 Presentation Library

## Purpose

Extract SoftPC's proven Win32 presentation behavior into
`src/lib/platform/win32/`, while preserving SoftPC as the first consumer and
behavioral baseline. The same directory is intended to remain byte-identical
in SoftPC, NXVM, and NTVDM64; it is a synchronised source component, not a
cross-repository build, link, runtime, or acceptance dependency.

The library owns only host presentation mechanics: a Win32 window, copied
frame painting, resizing, cursor presentation, normalized keyboard input,
relative mouse scaling, focus handling, and mouse capture/release. It does
not own a machine, a guest, a renderer surface, a runtime lifecycle, or a
fixed hotkey policy.

## Design Direction

The first implementation combines SoftPC's interaction quality with NXVM's
presentation boundary:

```text
project runtime/session
  -> product adapter copies a frame and publishes it to the library mailbox
  -> library routes console/window and waits for mailbox/event or Win32 messages
  -> library paints the newest copied frame and emits normalized input/actions
  -> product adapter enqueues input and decides lifecycle semantics
```

The common frame contract supports text cells, fonts, cursor metadata,
indexed pixels, palette data, a dirty rectangle, a `full_refresh` marker, and
a monotonically changing generation. It contains copied values only; it never
exposes VRAM, a SoftPC DIB pointer, a renderer lock, a CPU/machine pointer, or
a session mapping.

## Required Library ABI

T36 is complete only when this directory supplies the following generic ABI.
The names may evolve, but the ownership and behavior are mandatory.

| Component | Library owns | Binding supplies |
| --- | --- | --- |
| `frame` | Value-only text/graphics/font/palette/cursor frame format. | A safe machine/session snapshot copied into a frame; never a live VRAM/DIB pointer. |
| `mailbox` | Two-slot latest-frame storage, synchronization, publication sequence, and auto-reset Win32 wake event. | Calls `publish`; no app runtime owns a parallel frame mailbox/event. |
| `input` | Host physical-key and RDP packet normalization: scan recovery, extended keys, duplicate `WM_CHAR` suppression, UTF-16/Unicode scalar handling, and generic key/text events. | Converts generic events to the product guest-key/device queue. |
| `event_queue` | Bounded synchronized generic input/action queue plus its wake event, so no presenter writes a product runtime's key/mouse arrays directly. | Drains generic events and translates them to its guest keyboard/mouse/controller ingress. |
| `actions` | Per-presenter registered shortcut table matching host chords to typed actions, with consumed modifiers safely released before dispatch. | Registers product-selected chord/action entries and decides action meaning. |
| `mouse` | Relative coordinate calculation, button transitions, click capture, cursor hiding, `ClipCursor`, focus-loss release, and generic mouse events. | Converts generic relative/button events into its mouse-controller protocol. |
| `window` | Window class, message pump, copied-frame painting, cursor overlay/blink, DIB conversion/dirty invalidation, geometry, input, and action dispatch. | Title, initial configuration, generic event/action callbacks, and a non-machine running query for host cursor blinking. |
| `console` | Private-console lifetime, copied-frame text painting, generic keyboard/RDP/mouse input, registered actions, and event/message waits. | Product title/help strings, generic event/action callbacks, and frame publication. |
| `router` | `WINDOW` always presents in a window; `CONSOLE` presents text in console, graphics in window, then returns after the defined stable-text threshold. | Maps product configuration syntax to `WINDOW` or `CONSOLE`. |

The public binding boundary emits generic events, not `KEY_EVENT_RECORD`.
`KEY_EVENT_RECORD` is permitted only inside the Win32 collection layer. RDP
Unicode handling therefore belongs entirely to the library; SoftPC only turns
the resulting generic event into its existing original key encoder and guest
input queue.

The only machine-adjacent SoftPC adapter is deliberately narrow:

```text
SoftPC machine snapshot APIs -> copied generic frame -> mailbox.publish()
generic input event           -> original SoftPC key/InPort adapter
```

That adapter is not a presenter, mailbox, hotkey table, display router, or
Win32 event loop.

## Migration Order And Completion Proof

1. Move the copied-frame mailbox, publication event, and generic input event
   queue into the library; remove duplicate runtime-owned presentation and
   input transport storage.
2. Replace the public `KEY_EVENT_RECORD` callback with generic key, Unicode
   text, and mouse events. Keep the original SoftPC key table solely in the
   SoftPC delivery adapter.
3. Install the generic action registry in both presenters. SoftPC registers
   its current Ctrl+Alt P/D/F/M entries; another product may register a
   different table without editing lib source.
4. Move the text/DIB rendering, Win32 window lifecycle/message pump, console
   lifecycle/message pump, and their common wait behavior into library
   presenters driven only by the mailbox and binding callbacks.
5. Move the existing text/graphics/text transition into the generic router;
   delete SoftPC-specific `auto_switch` and presentation-choice branches.
6. Prove, on x64 and x86, generic ABI tests plus the current SoftPC boot,
   text, graphics, resize, RDP typing, mouse capture/release, hotkey,
   pause/resume, and console/window transition tests. The source-boundary
   test must prove the library has no `app_runtime`, `softpc_machine`, C-VID,
   BOP, or guest-controller reference.

## Partial-Update Contract

The shared library must present graphics incrementally. A project binding
publishes a complete safe snapshot plus either a bounded changed rectangle or
`full_refresh`. The library converts indexed pixels and invalidates only that
rectangle in its Win32 RGB surface. It performs a complete conversion and
paint only for the first frame, a surface-size or mode change, a palette/cache
change, a recreated window, an explicit `full_refresh`, or when it cannot
safely combine skipped generations' changed regions.

This preserves the original SoftPC V7/VGA renderer's dirty-rectangle benefit
without letting the frontend alias its DIB. A project that cannot yet publish
a dirty rectangle sets `full_refresh`; that is a correct fallback, not a
different presentation route.

## Shared Capability Matrix

The common library's initial behavior is the following combination of current
project strengths. A listed SoftPC behavior is a frontend baseline, not an
authority to import SoftPC machine state or lifecycle policy.

| Capability | Baseline | Common-library responsibility | Project binding responsibility |
| --- | --- | --- | --- |
| Snapshot ownership | NXVM frame/mailbox; NTVDM64 session snapshots | Accept and capture copied values only. | Obtain and release safe snapshots; retain all renderer/DIB/VRAM/mapping locks and lifetimes. |
| Frame publication | SoftPC runtime | Coalesce a newest-frame generation and wait on a Win32 wake event with UI messages. | Publish a copied frame and signal the channel. |
| Graphics | SoftPC V7/VGA presenter | Indexed-pixel to RGB32 conversion, palette application, dirty-rectangle invalidation, and safe full-refresh fallback. | Supply copied pixels, palette, geometry, dirty rectangle and generation. |
| Text | SoftPC presenter | Render text cells, attributes, primary/secondary fonts, font-select state, palette and a host-only cursor overlay. | Supply copied text/font/cursor data and decide whether text is eligible for a window. |
| Cursor timing | SoftPC presenter | Maintain a bounded host cursor-blink deadline without polling frame presentation; freeze the displayed frame while a project declares execution paused. | Publish running/paused presentation state. |
| Window geometry | SoftPC presenter | Native guest-size initialization, aspect-ratio-preserving sizing, guest-coordinate scaling, and explicit restore to native geometry. | Choose title, initial placement and project-specific display policy. |
| Keyboard | SoftPC presenter | Normalize scan/extended transitions, suppress duplicate `WM_CHAR` after physical keys, support scan-less RDP UTF-16/Unicode input, and release host modifier state before a consumed host action. | Convert generic input events into the project's queued guest input protocol and register product hotkeys. |
| Mouse | SoftPC presenter | Click-to-capture, relative movement scaled from client to guest coordinates, button state, host cursor hiding, `ClipCursor`, and release on focus loss or a binding action. | Convert relative events into the project's mouse/device request and choose whether capture is supported. |
| Wait loop | SoftPC presenter | Wait jointly for frame wakeups, Win32 messages and bounded cursor/title deadlines; never drive guest timing or busy-poll. | Own executor wakeup, guest clock, and all machine scheduling. |
| Display arbitration | NTVDM64 | Own the common display policy: `WINDOW` always presents in a window; `CONSOLE` presents text in the console, opens a window for graphics, and returns to the console after a stable text return. | Parse product configuration into the common policy and supply copied frames. |

The current NXVM mailbox establishes the correct copied-value ownership
direction but not the final wake strategy: its spin-lock capture/publish
implementation must not require a presenter to poll. The shared channel
instead uses the SoftPC latest-frame/event behavior while keeping NXVM's
machine-independent frame boundary.

## Explicitly Project-Specific Behavior

The library must not decide or embed any of the following:

- product shortcut assignments or typed-action meaning; the library owns
  registration and matching but never a product's shortcut table;
- guest key encoding, mouse-controller protocol, BOP, DOS, WOW, debugger or
  Console semantics;
- whether closing the window pauses a machine, cancels a session, returns to
  a debugger, or stops a process;
- a product's configuration syntax or persistence for choosing the common
  `WINDOW` or `CONSOLE` display policy;
- fullscreen-controller, Console Server, VDD, VRAM, DIB, renderer-lock,
  guest-memory, CPU, timer, machine or session ownership.

The channel should retain SoftPC's coalesced wake/event and latest-frame
publication behavior rather than copying NXVM's current spin-lock mailbox
unchanged. The project-specific binding remains responsible for obtaining a
safe snapshot and for all lifetime/locking rules.

## Project Bindings

- SoftPC supplies a machine-snapshot producer and generic keyboard/mouse
  delivery adapter, then registers its title/help strings and action table.
- NXVM binds its machine/composition presentation mailbox and value-copy
  display frame.
- NTVDM64 binds its session-owned graphics/text snapshot and Console
  arbitration path; it retains its original SoftPC/COMMAND/BOP boundaries.

The common library does not define product hotkeys. A binding registers its
own mapping and receives typed host actions such as pause, close, display
toggle, or mouse-release. SoftPC's current mouse capture, focus-loss release,
relative-coordinate scaling, and RDP-safe keyboard normalization are the
initial behavior baseline. Closing a window remains a binding decision:
SoftPC may pause, NTVDM64 may request cancellation, and NXVM may follow its
own session/debug policy.

## Admission Scope

An admitted implementation task must:

1. move or extract the reusable Win32 implementation with `git mv` where
   applicable, without changing recovered `mvdm/softpc.new` source;
2. leave only a SoftPC-specific frame-producer and generic input-delivery
   binding outside `src/lib/platform/win32`;
3. prove SoftPC's existing text, graphics, resize, keyboard, mouse-capture,
   focus-loss, RDP, pause and Console/window-transition behavior on x64 and
   x86; and
4. define a source-hash manifest/check that later permits all three projects
   to verify identical `src/lib/platform/win32` contents without turning a
   sibling repository into a build dependency.

## Non-goals

- No modification of original SoftPC CPU, C-VID, VGA, BIOS, BOP, guest RAM,
  timer, ROM, or media behavior.
- No new fullscreen controller, Console Server, NTVDM, DOS, WOW, or VDD
  semantics.
- No common product lifecycle, fixed hotkey table, machine callback, or guest
  input protocol. The generic shortcut registry and generic display router
  are required, but action meaning and configuration syntax remain bindings.
- No change to user-owned `assets/binary/softpc.ini`.

## Admission Questions

Before implementation, choose the initial common frame ABI and the exact
three-repository synchronisation/hash gate. The first task should port only
SoftPC to the new local library and prove no UX regression; NXVM and NTVDM64
adoption are separate admitted tasks.
