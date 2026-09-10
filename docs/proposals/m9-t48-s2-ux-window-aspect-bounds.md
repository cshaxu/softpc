# M9 T48 S2 — UX Window aspect-preserving bounds

## Original request

Keep the Window client area at the current guest frame aspect ratio at all
times. The guest image must fill that client area with uniform scaling: no
letterbox or pillarbox. A title-bar double-click restores the guest's natural
client size, or the largest fitting aspect-preserving size on a smaller work
area. Maximize likewise selects that work area's largest aspect-preserving
Window rather than a native full-work-area client rectangle.

## Objective

Make the copied guest frame aspect ratio a host Window geometry invariant. The
Win32 leaf alone owns this geometry; SoftPC continues to publish copied frames
and makes no native-size or scaling decision.

## Boundaries and non-goals

- May change `src/lib/ux-window/win32/**`, focused geometry tests, task/design
  documentation, and agent-owned package executables.
- Must not change `src/mvdm/softpc.new/**`, SoftPC lifecycle/input policy,
  copied frame dimensions, or user-owned `assets/binary/softpc.ini`.
- No black bars, source cropping, non-uniform image scaling, second renderer,
  or product callback.

## Design

1. Keep the display rectangle equal to the client area: painting remains one
   uniform source-to-client `StretchBlt`.
2. Centralize the source-aspect outer-rectangle calculation in the existing
   Win32 geometry helper. It uses the current decoration dimensions and always
   returns the largest client rectangle of the source ratio that fits a given
   client/work-area bound.
3. Use that helper for `WM_SIZING`, for the first-frame/natural-size path, and
   as a final `WM_SIZE` correction. The latter covers shell snap, maximize,
   DPI/window-manager adjustment, and any route that bypasses `WM_SIZING`.
4. Intercept a maximize request and replace it with the largest
   aspect-preserving Window in the current monitor work area. The title-bar
   double-click remains a restore-to-natural-size action.
5. The final correction is reentrancy guarded and does not move, foreground,
   or capture input. It only changes the outer size when the observed client
   ratio is not the current copied source ratio.

## Verification

- Unit-test aspect fitting for horizontal, vertical and corner sizing;
  narrow-work-area initial fit; and maximal work-area fit.
- Add a geometry regression proving every returned client rectangle has the
  copied source ratio and fills the resulting client area without bars.
- Fresh x64/x86 build and CTest; refresh only `softpc32.exe` and
  `softpc64.exe`.

## Exit criteria

Normal drag, shell/system resizing, maximize, and title-bar double-click leave
the Window at the copied guest aspect ratio. Painting fills the full client
area uniformly, with neither distortion nor black bars; both widths build and
test; all changes are committed and pushed from a clean worktree.
