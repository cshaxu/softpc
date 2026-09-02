# M5 Outer Input Latency

## Scope

This record covers only the standalone outer input path.  It does not change
or reinterpret the original keyboard controller, PIC, CCPU executor, BIOS,
ROM, BOP, guest time or device scheduling.

## Acceptance probe

`softpc-runtime-boot-smoke` boots a private one-sector image whose guest:

1. installs its own IRQ1 vector;
2. reaches `HLT` with interrupts enabled; and
3. records `A5h` at physical `0500h` from its IRQ1 handler before returning.

The test queues a SoftPC key through the standalone runtime and requires the
guest marker within 250 ms. This proves queueing, the independent executor
wake, and original 8042 -> PIC -> CCPU delivery are not deferred to the
50 ms host heartbeat or presentation cadence. The real configured A:+C:
Windows Setup invocation also passes after that probe on both host widths.

`softpc-win32-window-smoke` uses the same HLT guest, but sends `WM_KEYDOWN`
through the actual standalone window procedure and its shared RDP-capable
normalizer. It requires the same original guest IRQ1 marker within 250 ms.

## Current evidence

- x64 MinGW: runtime probe plus A:+C: Windows Setup Welcome path passed;
  the Win32 window end-to-end input probe passed; complete serial CTest
  passed 18/18.
- x86 Clang: runtime probe plus A:+C: Windows Setup Welcome path passed;
  the Win32 window end-to-end input probe passed; complete serial CTest
  passed 18/18.

## Boundary

The 250 ms limit is deliberately an input-control bound, not an artificial
guest-speed throttle or a claim about arbitrary guest application throughput.
It prevents a future outer-shell change from making a key wait behind a
frontend timer, copied frame or executor heartbeat. Guest instruction
execution remains owned by the restored SoftPC CCPU path.
