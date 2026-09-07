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

## Presenter And Console Product Policy

SoftPC distinguishes static `display=console|window`, the active presenter set
`{window_enabled, console_enabled}`, and the one Current Console Object bound
by host. That object is either the SoftPC cooked monitor or the UX raw VM
object. A monitor never implements SoftPC hotkeys; it accepts normal line
commands only.

| Running condition | Presenter set | Current Console Object |
| --- | --- | --- |
| `display=console`, text frame | `{false,true}` | UX raw VM object |
| `display=console`, graphic frame, `console_control=0` | `{true,true}` | UX raw VM object |
| `display=console`, graphic frame, `console_control=1` | `{true,false}` | SoftPC cooked monitor |
| `display=window`, text or graphic frame | `{true,false}` | SoftPC cooked monitor |

`console_control` is read only for `display=console`; it is `0|1` and defaults
to `1`. Window display ignores it. Paused uses the monitor object with
`console_enabled=false`; an existing Window remains only when product intent
retains it. Stopped has no UX presenter. Resume first restores the derived
running presenter set and Current Console Object, then resumes the VM.

Window X is a SoftPC close request: running first reaches paused, host switches
to monitor, then SoftPC destroys the presenter. Until native Window destruction,
normal Window input remains valid. Window CAP is a UX action; Console CAP exists
only while the UX raw object is current.

## Responsiveness

The frontend remains responsive while the guest runs. Frame presentation is
coalesced, not tied to input delivery, and idle execution must wait rather
than busy-spin. Guest timing remains owned by the original host-timer contract,
not by a UI frame rate or an instruction-count throttle.
