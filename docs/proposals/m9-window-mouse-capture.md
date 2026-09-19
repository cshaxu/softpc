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

References: [ClipCursor](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-clipcursor),
[WM_MOUSEMOVE](https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousemove),
[Raw Input registration ownership](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerrawinputdevices).
