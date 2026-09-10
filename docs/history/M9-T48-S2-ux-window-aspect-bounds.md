# M9 T48 S2 — UX Window aspect-preserving bounds

## Outcome

`ux-window` now owns the copied-frame aspect as a native Window geometry
invariant. Its client area fills the guest image uniformly: there are neither
black bars nor non-uniform stretching. `WM_SIZING` constrains ordinary drag;
the final `WM_SIZE` path corrects any actual shell/system dimensions that
bypass it. A maximize request selects the current work area's largest fitting
aspect-preserving Window instead of Windows' full-work-area rectangle.

Title-bar double-click continues to restore the copied frame's natural client
size, scaled down to the largest fitting work-area size when needed.

## Boundaries

Only generic `src/lib/ux-window/win32/` geometry, its focused test, the Window
UX design, task status, and the two agent-owned package executables changed.
No SoftPC product policy, MVDM, copied frame ABI, user configuration, or guest
media changed.

## Proof

- The geometry smoke covers width-limited and height-limited fitting plus
  maximal 4:3 work-area fitting.
- Fresh x64 and x86 builds completed. Each ran all 33 applicable CTest cases
  successfully; package smoke is intentionally excluded because the
  user-owned package INI selects the Window route rather than that test's raw
  Console route.
- The owner tested and accepted the Window behavior.

## Closure

T48 S2 closes after owner acceptance. Future Window geometry changes require
separate admission.
