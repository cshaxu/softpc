# System Architecture

## Target Ownership

```text
mvdm/softpc.new
  selected original CCPU, C-VID, controllers, BIOS, firmware, BOP and renderers
        ↑
overlay/mvdm/softpc.new
  reproducible x86/x64 source/ABI adaptation only; no machine policy
        ↑
host/
  standalone host callback implementations: timer, media, BOP services,
  conapi-compatible surfaces, input, audio, serial and parallel endpoints
        ↑
app/
  one executor, command/input queues, lifecycle, frame mailbox, monitor
  console, and Win32 window; no machine-state access
```

`mvdm/softpc.new` is the repository-owned selected recovered-machine layout.
It retains original relative paths and names for every included file so T14 can
compare them with the read-only OpenNT reference. It is a selected source
subset: wholly host-specific NT endpoint files may be absent when a standalone
host endpoint replaces that contract. Its corresponding overlay path, if
needed, holds only repository-owned reproducible patches for host-width or
toolchain representation; it never owns machine policy. `host/` supplies
standalone host-facing symbols but does not own guest-visible state.
`app/` owns the single executor and presentation shell.

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
