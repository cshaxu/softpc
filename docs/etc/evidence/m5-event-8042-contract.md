# M5 Original Timer, Quick-Event And 8042 Contract

## Result

The standalone runtime retains one executor-owned copy of the original
machine callback path.  A frontend never updates guest state directly:

```text
frontend input queue
  -> executor wake mailbox
  -> original host_key_down/host_key_up ingress
  -> original keyba.c 8042 buffer and quick_ev event
  -> generated CCPU original HLT / instruction safe point
  -> original quick-event dispatch
  -> original PIC IRQ1
  -> guest interrupt handler
```

The host 20 Hz timer queue only records a pending heartbeat.  At the same
executor safe point it calls the original `host_timer_event` order:
graphics tick, quick-event recalibration, serial and parallel heartbeats,
`time_strobe`, continuous tone, then the outer-runtime frame/input callback.
It does not mutate CCPU, PIC, PIT, 8042, media, or renderer state from a host
thread.

## HLT Boundary

The CCPU generator overlay now installs one mailbox consumption point in the
original HLT loop immediately before its original `SYNCH_TICK` /
`QUICK_EVENT_TICK` dispatch.  HLT waits on the host event only when neither a
CPU interrupt nor an original queued quick event remains.  This is a port-ABI
overlay around generated CCPU source; no controller or CPU protocol was
rewritten.

`softpc-runtime-boot-smoke` adds a boot-sector regression that owns IRQ1 with
a small guest handler.  It reaches `HLT`, receives a normal runtime key event,
acknowledges the original master PIC, and observes the guest marker written by
that handler.  This avoids relying on DOS or BIOS keyboard service behavior
while proving the full original 8042 -> PIC -> CCPU wake path.

## Verification

- x64 MinGW: full CTest `16/16` passed, including the HLT keyboard regression,
  IRQ, FDC, quick-time, runtime and source-boundary tests.
- x86 Clang: full CTest `16/16` passed with the same tests.
- Both widths: `softpc-runtime-boot-smoke --floppy O:\\assets\\fdd.img --hdd
  O:\\assets\\hdd.img --windows-setup` passed using overlay media; the supplied
  images were not modified.

No ROM byte, BOP selector, C-VID behavior, device protocol, DOS service, or
NTVDM product dependency was added or changed.
