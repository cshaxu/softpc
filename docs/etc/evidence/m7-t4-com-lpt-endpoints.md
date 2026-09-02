# M7 T4 Original Serial And Parallel Endpoint Proof

## Endpoint Boundary

The original `base/comms/com.c` UART and `printer.c` controller state machines
remain unchanged. The existing standalone carriers `nt_com.c` and `nt_lpt.c`
retain their original host-side queues, status, buffering and lifecycle, with
only the final virtual-sink exit made optionally useful.

`softpc.ini` accepts two optional paths:

- `serial_output=` attaches COM1 transmit bytes to a synchronous append-only
  file endpoint; and
- `printer_output=` attaches LPT1's original buffered/flush output to a
  synchronous append-only file endpoint.

An absent or empty option retains the bounded virtual sink. No host hardware,
background I/O thread, host-to-guest receive path, DOS open tracking, VDD, or
controller replacement is introduced. The serial file opens at the first
original transmit callback because the original NTVDM-only open-on-LCR branch
is intentionally absent in a standalone build.

## Regression Evidence

- `softpc-serial-smoke` sets an endpoint, verifies a direct original host
  transmit and a controller UART transmit, then repeats reset and confirms
  the same endpoint cleanly records `S`, `A`, and `R` before destruction.
- `softpc-printer-smoke` sets an endpoint, writes through original LPT1 I/O,
  and confirms the original buffered byte is flushed as `A` on teardown.
- The Win32 smoke now accepts a positive client rectangle when a constrained
  RDP work area clamps the requested fixed guest surface; it still proves a
  non-resizable frame, copied-frame presentation and input delivery.
- x64 clean serial CTest: 19/19 passed.
- x86 clean serial CTest: 19/19 passed.
