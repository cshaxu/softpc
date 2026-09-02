# M5 T1: Original Renderer Dispatcher Restoration

## S1: Original Renderer Dispatcher Restoration

**Status:** complete.

The standalone renderer no longer owns a parallel graphics dispatcher.  The
compiled path uses the original `nt_graph` dispatcher and original
`nt_cga`/`nt_ega`/`nt_vga` painter flow, with a compatibility host that owns
only DIB/text surface allocation and invalidation.

The same change established the generated CCPU finite-run return at the
original `NEXT_INST` boundary.  Its outer return adapter now follows the
original simulation-stack decrement invariant before returning to the saved
outer context, including after nested host simulation.

Commit `1b09b1e` contains the implementation and the supporting
[acceptance evidence](../etc/evidence/m5-renderer-executor-acceptance.md).
Clean x64 and x86 builds, 16/16 CTest suites, isolated bounded-machine
smokes, and overlay-media Windows Setup welcome-screen acceptance all passed.

## S2: Pristine-Source And Host-Contract Ledger Closure

**Status:** complete.

The canonical-text audit has 60 current C/H rows.  Its fail-closed route map
assigns exactly 35 to reproducible port-ABI overlays, 19 to compatibility-host
endpoints, and 6 to later pristine source restoration.  The exact coverage and
bounded next cut are in the [M5 route ledger](../etc/evidence/m5-pristine-host-contract-routes.md).
No machine behavior changed in this audit subtask.

## S3: Machine/Host Contract Declaration Extraction

**Status:** complete.

The six remaining machine-source restoration rows now have named, existing
host capability owners. In particular, standalone Set-1 keyboard ingress uses
the original `nt_keycd` table through a narrow host endpoint, leaving
`keyba.c` without its locally reconstructed scan-code lookup. The only
keyboard controller difference retained is the CPU_40 port-ABI reset request:
the selected CCPU keeps its interrupt map private, so the device uses its
declared `cpu_interrupt(CPU_HW_RESET, 0)` entry rather than exposing state to
the host.

The source map and focused x64 smoke proof are in the
[M5 machine/host contract evidence](../etc/evidence/m5-machine-host-contracts.md).

## S4: Original Timer, Quick-Event And 8042 Contract Recovery

**Status:** complete.

The detached runtime now consumes frontend wakes only at the original CCPU
instruction and HLT safe points.  Original `keyba.c`, `quick_ev.c`, the 8042,
PIC and guest IRQ flow retain ownership of keyboard delivery; the host timer
records a heartbeat and the executor performs the original callback order.

The generated CCPU overlay contains a single HLT mailbox bridge before the
original quick-event dispatch.  A guest-owned IRQ1 regression proves that a
runtime key wakes HLT through original 8042/PIC delivery rather than through a
frontend shortcut.  Both x64 and x86 full suites passed 16/16, and both widths
passed the overlay-media Windows Setup probe.  Details are in the
[M5 event/8042 contract evidence](../etc/evidence/m5-event-8042-contract.md).
