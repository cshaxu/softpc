# Window mouse capture and motion

Owner request: captured mouse sometimes escapes the Window but controls content
on reentry; pointer jitters and encounters invisible boundaries which require
reverse movement before continuing. Also occasional in Win3.1, more apparent
at higher resolution. Owner supplies an installed-system checkpoint for tests.

Baseline is S10 delivery 62eec11, accepted by the owner; both widths 109/109.
First audit native capture/clipping, coordinate motion, Common routing and
VM/InPort conversion. Preserve media/configuration; no sensitivity or guest
special cases. Existing Lib/Common approval restriction remains applicable.

Finite coverage: native capture versus clipping; finite-coordinate exhaustion;
resize/rebase and scale; input-source routing; release/freeze/focus transitions.
Each receives a demonstrated defect and repair, an unchanged justified path,
or an explicit unresolved observation. Do not claim all symptoms share a cause.
Before implementation record owning files and estimated diff. Afterward record
actual changes, dual-width tests and package results. Diagnostics are bounded
in the active packet; no captured guest files enter the corpus.

## Initial audit

`kvm-window/win32/mouse.c` clips the host pointer to the client rectangle once
on capture and on move/resize. `motion.c` derives relative input by subtracting
successive finite client coordinates. There is neither recentering nor an
independent motion source. A probe of the unchanged production function gives
319 on reaching x=639 from x=320, then zero while remaining at that edge.
After client width shrinks from 640 to 320 and the clipped x becomes 319,
the old baseline produces -640 content units without physical movement.
This demonstrates finite-coordinate exhaustion and a geometry-rebase defect,
not yet a reproduction of the owner's exact jitter sequence.

`WM_MOUSEMOVE` tests only the cached captured flag. Native capture loss and
keyboard focus loss do share release cleanup, but external clip changes are
not checked during motion. Actual pointer escape still needs native evidence;
do not claim its trigger has been identified. VM forwards copied deltas into
the original InPort receiver; no product-level capture correction belongs there.

The minimal candidate remains a single Window-local legacy motion path with
host recentering, baseline reset on geometry changes, and loss of native
ownership/clip treated as release, never automatic recapture. Synthetic
recenter movement must not become content movement. Raw Input is not adopted
casually: its per-process device-class registration could steal an importing
application's existing receiver. Public KVM events need no schema change.
Estimated shared production scope is the existing Window mouse/component
implementation and necessary Types API aliases, approximately 40--100 changed
lines plus tests; refine after native proof. Prior owner restriction requires
explicit Lib modification approval; none is assumed from research alone.

## Implementation admission

The owner approved the explained recentering design, dual-width build/test,
P commit/push and subsequent manual validation. Implement in the existing
Window native mouse owner: retain clipping bounds as the screen-coordinate
anchor; sample current pointer position (not stale queued message coordinates),
calculate motion with the existing remainder helper, then recenter. Geometry
refresh rebases without input. Capture/clip loss releases through the existing
cleanup; no automatic reacquisition. Shared Types gains only API aliases.
Expected production diff: approximately 40--100 changed lines across four
existing files, plus focused tests and manifest updates; no new state machine,
thread, registration owner or public interface.

References: [ClipCursor](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-clipcursor),
[WM_MOUSEMOVE](https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove),
[Raw Input registration ownership](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerrawinputdevices).

## P2 implementation and bounded review

The existing Window mouse owner now samples the current screen position,
calculates the existing integer scaled delta and recenters the hidden pointer.
It retains its clipping rectangle, not a second input queue. Move/resize rebases
the anchor; native capture/clip loss or failed positioning uses the existing
release path. Application deactivation also releases. Capture clicks remain
consumed, uncaptured motion remains discarded, and no automatic recapture is
introduced. Public events and APIs are unchanged.

The owner's symptom is intermittent blocked movement, not reverse movement.
Suppressing self-generated recenter motion is a prevention requirement of this
new implementation, not a claim that the owner reported reverse movement.
The finite-coordinate defect is proven; reproduction of every intermittent
escape/jitter trigger remains subject to manual validation. Legacy coordinate
sampling retains host sensitivity and may coalesce motion; it is not a claim
of lossless raw hardware counts.

Changed-path accounting against S11 P1, using `git diff --numstat`: four
production files (Window mouse.c, mouse.h, component.c, Types win32/window.h)
+68/-26, net +42. Two existing tests +76/-16, net +60. Documentation, manifests
and EXEs excluded. No Common, VM, Compat, MVDM, INI or media changes.

Similar-issue sweep (`rg` over Window capture, mouse_move, refresh_bounds,
WM_MOVE/SIZE/CAPTURECHANGED/KILLFOCUS/ACTIVATEAPP and event call sites):

| Path | Disposition and proof |
| --- | --- |
| Finite edge exhaustion | Recenter after sampled motion; 100 movements accumulate beyond client width. |
| Synthetic recenter | Repeated center sample emits zero; no second motion route. |
| Scale and geometry | Existing signed remainder helper retained; 2x scale, moved/resized bounds and zero-motion rebase tested. |
| Native ownership/clip | Motion validates both; foreign clipping releases without clearing the foreign rectangle; reentry emits nothing. |
| Focus, freeze and capture loss | Existing cleanup retained; application deactivation added and tested. |
| Native pointer failures | Position query/warp failure releases; existing sink/retirement tests remain passing. |
| Routing and hardware conversion | Common source filter and original VM/InPort path unchanged; no sensitivity workaround. |

Both package binaries build. Deterministic capture and frame/motion tests pass
on both widths. The first x64 parallel regression run passed 107/109; two
package tests failed at stage 16. Their captured monitor showed `hpause` and
`ia`, not the strings sent by the test. This establishes unexpected input in
that run, not its source or a repaired test defect. The two package tests
subsequently pass in isolation; no assertion, timeout or product command path
was changed to obtain a pass. Full regression results are recorded in Current.
The native user snapshot interaction is not claimed as manually verified.
Owner subsequently confirmed the repair works and requested the next Lib audit.
S11 is closed with that acceptance; T70 remains open.
