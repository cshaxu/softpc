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
