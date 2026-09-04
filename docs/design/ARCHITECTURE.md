# System Architecture

## Target Ownership

```text
mvdm/softpc.new
  original CCPU, C-VID, controllers, BIOS, firmware, BOP and nt_* renderers
        ↑
overlay/mvdm/softpc.new
  reproducible x86/x64 source/ABI adaptation only; no machine policy
        ↑
host/
  original host callback implementations: timer, media, BOP services,
  conapi-compatible surfaces, input sink and audio sink
        ↑
app/runtime
  one executor, command/input queues, lifecycle and frame mailbox
        ↑
app/frontends
  monitor console and Win32 window; no machine-state access
```

`mvdm/softpc.new` is the pristine machine mirror. Its corresponding overlay
path holds only reproducible source patches required by host-width or toolchain
representation; it never owns machine policy. `host/` supplies original
host-facing symbols but does not own guest-visible state. `app/` owns the
single executor and presentation shell.

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
