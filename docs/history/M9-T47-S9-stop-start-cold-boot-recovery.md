# M9 T47 S9 — Stop/start cold-boot recovery

## Outcome

The owner-observed `pause -> stop -> start` path now reaches a new DOS
`C:\\>` prompt. The final root cause was not the original reset routine's
`soft_reset` fact: `c_cpu_reset()` leaves CCPU's private pending asynchronous-
interrupt map intact, so a bit from the completed run could redirect a new
BIOS immediately after its reset vector. The standalone CCPU port-ABI
lifecycle boundary clears that map only after the old executor run is complete
and before the public cold-run reset begins.

The task also retains two independent cold-run invariants: the VM input FIFO
is cleared before a new run, and ordinary guest key/mouse records—including
releases—cannot enter a paused or stopped VM. Monitor lines and registered
hotkeys remain on their independent product-control route. MVDM is unchanged.

## Evidence

- `runtime_restart_boot_smoke` now requires an exact copied `C:\\>` frame for
  the new run; it no longer accepts an incidental BIOS/DOS-like prompt.
- `runtime_input_continuation_smoke` clears its retained-RAM boot marker
  before the next run, so it proves a newly executed boot sector rather than
  mistaking prior RAM for a restart.
- `package_smoke` drives the actual package Console route:
  `start -> raw CAP -> monitor stop -> start`; it requires the old prompt to
  disappear, then observes a new boot banner and a new `C:\\>`.
- Fresh x64 and x86 CTest each passed 34/34. The actual package route passed
  three consecutive times at each width.
- `assets/binary/softpc32.exe` and `softpc64.exe` were refreshed. The
  user-owned `softpc.ini` and guest media were not changed.

## Historical correction

P2's `soft_reset` conclusion and P3's presentation repair did not alone fix
the product path. P3/P4 retain independent correct frame and VM-input
lifetime semantics; P5 supplies the final CCPU reset-boundary repair.

## Closure

T47 closes at S9 after owner acceptance. The next work, if any, must be
separately admitted from the queue.
