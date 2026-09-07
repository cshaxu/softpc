# Product UX

## Direct Launch

`softpc32.exe` and `softpc64.exe` read only the adjacent `softpc.ini`. They do
not accept command-line arguments. Startup opens the monitor console and does
not automatically run the guest.

## Monitor

The monitor owns human-facing commands, not machine state. It presents a
small fixed command set:

- `start`, `resume`, `pause`, `reset`, and `stop` request lifecycle changes;
- `floppy insert <image>` and `floppy eject` swap configured drive-A media
  while stopped or paused; and
- `help` and `exit` are local monitor commands.

Commands enqueue requests and receive a published result. They never directly
call a CPU, controller, BOP, or renderer function.

## Window And Input

The optional Win32 window displays copied text or graphical frames published
by the runtime. It sends normalized keyboard and mouse records to the runtime
queue. It never decodes guest VRAM, locks a SoftPC device, waits for the
executor, or executes a guest instruction on the UI thread.

The console and window are equivalent input producers. RDP is supported by
the same normalized key path; no frontend may depend on raw local-only key
state as its sole input source.

## UX Component And Console Product Policy

SoftPC distinguishes static `display=console|window`, the active component set
`{window_enabled, console_enabled}`, and the one Current Console Object bound
by host. That object is either the SoftPC cooked monitor or the UX raw VM
object. A monitor never implements SoftPC hotkeys; it accepts normal line
commands only.

| Running condition | Active component set | Current Console Object |
| --- | --- | --- |
| `display=console`, text frame | `{false,true}` | UX raw VM object |
| `display=console`, graphic frame, `console_control=0` | `{true,true}` | UX raw VM object |
| `display=console`, graphic frame, `console_control=1` | `{true,false}` | SoftPC cooked monitor |
| `display=window`, text or graphic frame | `{true,false}` | SoftPC cooked monitor |

`console_control` is read only for `display=console`; it is `0|1` and defaults
to `1`. Window display ignores it. Paused uses the monitor object with
`console_enabled=false`; an existing Window remains only when product intent
retains it. Stopped has no UX component. Resume first restores the derived
running component set and Current Console Object, then resumes the VM.

Window X is a SoftPC close request: running first reaches paused, host switches
to monitor, then SoftPC destroys the Window component. Until native Window
destruction, normal Window input remains valid.

## UX Components And Registered Hotkeys

Shared UX is three independent components, not one controller that combines
their lifecycles: `lib/ux-base/` contains copied values, generic event construction,
and reusable private-mailbox helpers;
`lib/ux-window/` owns one Window lifecycle; and `lib/ux-console/` owns one VM
Console lifecycle. Each of Window and VM Console has its own Win32 and Linux
implementation. Neither component owns the SoftPC monitor Console, native
Console handles/modes, or the product decision to exist.

SoftPC creates either component with a copied table of registered host-hotkey
chords and identifier strings. `ux-base` provides only generic source-local
matching. A matched chord is discarded as normal input and produces one copied
`UX_HOTKEY(identifier)` event at SoftPC's queue entry; lib does not interpret
the identifier. Unmatched input is emitted as ordinary copied key/text/mouse
events in its original order. Matcher state is per component instance: keys
from Window and VM Console never combine into one chord.

The matcher withholds only a possible registered-chord prefix until it can
decide match versus mismatch. Thus Ctrl+Alt+P registered as `pause-toggle`
emits only that identifier, never partial Ctrl/Alt/P guest input. Ctrl+Alt+X
when not registered flushes Ctrl, Alt, and X as normal input in order. SoftPC
alone maps identifiers to pause/resume, stop/reset/start, mouse release, or
synthetic guest input such as Ctrl+Alt+Del and Alt+Enter. The cooked monitor
does not use a UX component or hotkey registry and accepts only monitor lines.
Each matcher derives its modifier state solely from its own raw sequence; it
does not trust process-global modifier state, so keys from two UX components
cannot compose a chord.

Every user input produced by either UX component is a copied `ux_input_event`:
ordinary key/text/mouse input and registered-hotkey input are variants of that
one UX event family. `ux-base` provides its single construction path and each
such value carries its originating Window or VM-Console component handle for
lifetime tracing; SoftPC does not assign product semantics by source. A cooked
monitor line is instead a `monitor_input_event` and carries no UX handle.
SoftPC's one input queue accepts both event families as distinct payloads in
arrival order; its control thread is their sole consumer. No monitor line is
mislabeled as a UX event, and no UX component parses monitor commands.

## Responsiveness

The frontend remains responsive while the guest runs. Frame presentation is
coalesced, not tied to input delivery, and idle execution must wait rather
than busy-spin. Guest timing remains owned by the original host-timer contract,
not by a UI frame rate or an instruction-count throttle.
