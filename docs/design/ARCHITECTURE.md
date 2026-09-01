# System Architecture

## Target Ownership

```text
core/softpc-pristine
  original CCPU, C-VID, controllers, BIOS, firmware, BOP and nt_* renderers
        ↑
core/softpc-port-abi
  reproducible x86/x64 source/ABI adaptation only; no machine policy
        ↑
host/softpc-compat
  original host callback implementations: timer, media, BOP services,
  conapi-compatible surfaces, input sink and audio sink
        ↑
vm/runtime
  one executor, command/input queues, lifecycle and frame mailbox
        ↑
vm/frontends
  monitor console and Win32 window; no machine-state access
```

`core/softpc-pristine` is the machine. `softpc-port-abi` exists only where a
host-width or toolchain representation makes the original source impossible to
compile. Its transformations are mechanically reproducible from pristine
inputs. `host/softpc-compat` supplies the original host-facing symbols but
does not own guest-visible state.

The runtime executor is the sole caller of the machine and compatibility host.
Input producers enqueue records and signal it. The executor publishes complete
text or graphic frame snapshots; frontends consume only those snapshots.

## BOP And Firmware Boundary

Original BOP instruction decoding and `BOP FE` control flow remain in the
original CCPU. The compatibility host implements only the finite hardware and
firmware BOP services actually reached by the selected ROM. DOS, WOW, NTVDM,
and product-service selectors remain unavailable.

## Timing Boundary

The original host timer remains the only source advancing original SoftPC
timer/device work. Runtime scheduling may wait or yield host CPU time but may
not manufacture guest ticks, map nominal MHz to instruction counts, or add a
second guest clock.
