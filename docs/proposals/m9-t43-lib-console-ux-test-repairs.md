# M9 T43 — Shared-library Console/UX test repairs

## Purpose

T43 follows the accepted T42 Console/Window integration. It contains only
bounded shared-library behavior repairs discovered by real product testing;
SoftPC remains the product-policy owner.

## S1 — Native Window-to-Console focus handoff

`ux-window` already foregrounds and focuses its native Window when it is
created. The symmetric operation is missing when `host/win32/console` makes a
raw VM Console or cooked monitor Console the broker's Current Console. Fix
that inside the host-native Console implementation.

The app continues to request only a logical broker replacement. It neither
calls Win32 focus APIs nor decides which native Console receives focus. A
successful activation—initial cooked creation, raw replacement, cooked
restoration, or restoration after a failed next activation—may request native
Console focus only after that binding is active. Preparation and failed
activation must not request it.

**Exit:** a focused native seam proves the successful paths request focus once
for the new Current Console and all non-committing paths request none; x64/x86
build/test evidence passes; the owner validates Window-to-Console input.

## Boundaries

- May change: generic `lib/host/win32` Console implementation and its narrow
  host/broker tests.
- Must not change: `src/mvdm/softpc.new/**`, SoftPC app/reconciler policy,
  Window component behavior, package configuration, guest media, or native
  focus calls outside `lib/host/win32`.
- The shared library remains platform-neutral above the Win32 leaf; a future
  Linux leaf may implement the same logical activation contract separately.
