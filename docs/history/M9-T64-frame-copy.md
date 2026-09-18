# M9 T64 — Narrowed frame-copy delivery

## Request and boundary

Owner: “批准做这个收窄版修复。” Later clarification: “INI本次也不必提交”.
S1 decision and cancellation of worker unification are recorded in
[S1 audit](M9-T64-S1-lib-simplification.md). S2 baseline is `cd56b7b`.
Preserve full metadata/text/font/palette prefix; omit only inactive pixels.
No layout, thread, lock, dirty, sequence, snapshot or product-policy changes.

Before estimate: production +15–30/-4–8. Test scope: a portable Lib smoke and
Common frame-copy integration assertions. User snapshot/media remain untouched
and unstaged. Owner subsequently authorized restoring the INI to the tracked
Win3.1 configuration; it has no content diff and is not part of the commit.

## Frozen convergence ledger

| Production path / reader | Disposition and proof |
| --- | --- |
| kvm-base mailbox publish | Use kvm_frame_copy after existing validation; dirty union unchanged. |
| kvm-base mailbox capture | Same helper under existing frame lock; generation/ack unchanged. |
| Common machine copied publication | Same helper under existing frame lock and run gate. |
| kvm-window render.c pixel reads | Graphics rendering bounds rows by height and columns by width; stride is validated. No tail reads. |
| kvm-console text conversion | Reads prefix only; unchanged. |
| Mailbox palette comparison | Prefix remains completely copied; no added mode guards needed. |
| Common text comparison and routing | Reads prefix only; unchanged. |
| VM producers and Common initialization/status construction | Keep existing full initialization; these are construction, not copied-value handoffs. |

Scope searched with graphics_pixels, whole-frame assignment and sizeof(frame)
patterns across src/lib, src/common and src/vm. Completion requires each
handoff above to use the same rule, pixel readers to stay within active extent,
and focused plus full regression evidence. No whole-repository correctness claim.

## Implementation

Header-only kvm_frame_copy validates its source and destination, accepts
self-copy, then copies one continuous extent: offsetof(graphics_pixels) plus
graphics_stride*graphics_height for graphics, or just the prefix for text.
Types adds only the standard offsetof alias. Invalid input leaves the target
unchanged; inactive pixel capacity remains untouched and is not content.

This avoids 983040 bytes per text copy (1966080 for mailbox publish/capture).
It does not reduce allocated frame size and is not a measured speedup claim.
No field-by-field lists, new buffers, callbacks or additional mutable state.

## Verification and review

Both Release builds completed. Focused tests (frame copy, damage/ack, independent
lock, Common machine and session frame) passed 5/5 on each width. The new portable
Lib test checks full prefix, untouched tail, padded stride, maximum extent,
self-copy, invalid rejection and text/graphics transitions. Common asserts
that text publication leaves the destination graphics capacity untouched.

Initial full CTest runs each passed 105/107. The two package cases rejected the
owner's Win95 media location before launching, because their guard permits only
assets/media. No product failure was involved. A disposable x64 package with
baseline overlay configuration passed both cases. Owner then explicitly allowed
restoring the INI; both original package cases passed 2/2 on each architecture.
Thus all 107 cases per width have passing evidence after configuration repair.
No user media was modified; disposable package output is removed after review.

Shared source/test manifests, component DAG, Types layout, Linux contract probes,
documentation governance and diff whitespace checks passed. x64 Lib was built
with its strict warning gate enabled. Linux probes do not claim native Linux
runtime execution.

Production C/H: four files, +20/-4, net +16. Tests/build registration: three
files, +74/-0 (69-line new Lib smoke, four Common assertions, one registration).
README/manifests/history/package binaries are excluded from these code counts.
Reproduce with git diff --numstat cd56b7b on the named paths after staging.

Review confirms no changed thread, layout, lock or dirty/ack route. The helper
is the sole rule; component copies ignore its return only after their existing
validated-publication invariant. Common propagates invalid-source rejection.
No unrelated cleanup was included. T64 remains open for owner testing.
