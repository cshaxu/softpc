# M9 T42 S8 — Work-area-aware Window default bounds

## Outcome

The generic Win32 `ux-window` now retains its 680 by 560 desired outer bounds
when the selected monitor work area fits them.  On a narrower or shorter work
area it proportionally fits the initial outer bounds and centers them.  The
first copied frame uses the same limit for its natural client resize, so it
cannot restore an oversized initial Window.

## Boundaries

Only the generic `src/lib/ux-window/win32/` geometry and creation path,
focused geometry test, build registration, UI design note, and library
manifest changed.  There is no SoftPC product-policy, MVDM, user-configuration,
guest-media, or manual-resize change.

## Proof

- Geometry tests cover adequate, width-limited, and height-limited work areas.
- Fresh x64 and x86 builds both passed full CTest, 30/30.
- The owner tested and accepted normal and narrow-screen behavior.

## Next

S9 records the remaining owner Console/Window runtime acceptance and closes
T42.  It is an acceptance-only task; any new repair is separately admitted.
