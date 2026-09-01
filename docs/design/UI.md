# Product UX

## Direct Launch

`softpc32.exe` and `softpc64.exe` read only the adjacent `softpc.ini`. They do
not accept command-line arguments. Startup opens the monitor console and does
not automatically run the guest.

## Monitor

The monitor owns human-facing commands, not machine state. It presents a
small fixed command set:

- `start`, `pause`, `reset`, and `stop` request lifecycle changes;
- `floppy` reports or swaps the configured floppy according to `softpc.ini`;
- `status` reports the published lifecycle state; and
- `help` and `quit` are local monitor commands.

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

## Responsiveness

The frontend remains responsive while the guest runs. Frame presentation is
coalesced, not tied to input delivery, and idle execution must wait rather
than busy-spin. Guest timing remains owned by the original host-timer contract,
not by a UI frame rate or an instruction-count throttle.
