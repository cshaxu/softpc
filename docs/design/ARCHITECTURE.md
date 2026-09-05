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
  one executor, machine snapshot producer, guest-input adapter, lifecycle,
  monitor, and product binding; no machine-state access from frontends
        ↑
lib/platform/win32/
  copied-frame mailbox/event, generic input/action queues, reusable Win32
  console/window presenters, geometry, sizing, mouse capture, and display
  router; no guest input protocol or lifecycle policy
```

`mvdm/softpc.new` is the repository-owned selected recovered-machine layout.
It retains original relative paths and names for every included file so T14 can
compare them with the read-only OpenNT reference. It is a selected source
subset: wholly host-specific NT endpoint files may be absent when a standalone
host endpoint replaces that contract. Its corresponding overlay path, if
needed, holds only repository-owned reproducible patches for host-width or
toolchain representation; it never owns machine policy. `host/` supplies
standalone host-facing symbols but does not own guest-visible state.
`app/` owns the single executor, machine snapshot producer, guest-input
adapter, monitor, and product binding.
`lib/platform/win32/` is a local synchronized-source component, not a runtime
or build dependency on NXVM or NTVDM64. It consumes and produces copied host
values only. It owns the generic mailbox, console/window message loops,
host-input normalization, action registration/matching, mouse capture, and
`WINDOW`/`CONSOLE` routing. The app binding alone converts events to the
guest's input protocol and makes all product lifecycle and action decisions.

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
