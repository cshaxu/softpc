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

## S2 — README and manifest contract closure

Bring the checked-in shared-library documentation and manifest contract into
line with the delivered split UX architecture. Remove obsolete descriptions of
one unified `ux presenter`. State the actual dependency graph:

```text
base -> ux-base + host + storage
ux-base -> ux-window + ux-console
ux-window -> base + ux-base
ux-console -> base + ux-base
```

No other component dependency is permitted.

Document the shared component mailbox contract precisely: every component has
a private FIFO control mailbox; it has a fixed documented capacity; enqueue
failure is returned to its caller and never overwrites a control request; the
worker drains control in FIFO order before considering its one-slot latest-wins
frame mailbox; encountering STOP discards later control and any pending frame,
retires native input/output, emits one `SOURCE_RETIRED`, then exits. The frame
mailbox always retains only the most recently published copied frame.

Document that every UX input event identifies its source with a globally
monotonic, non-reused source identity. `SOURCE_RETIRED` is the final
asynchronous lifetime event for that identity; applications use it to retire
per-source pressed-input state and must not treat it as an accessible component
handle. Finally, state that `base` logical Console is a copied neutral public
object: it has no platform handle, input mode, Window, monitor, VM, or product
lifecycle semantics.

**Exit:** README, public comments, manifest, and source-boundary tests agree
with the actual library; no legacy unified-presenter terminology remains.

## S3 — Fail-closed UX control-mailbox capacity

`ux-window` and `ux-console` control mailboxes must never overwrite a control
request. Their fixed 32-record ordinary FIFO returns an explicit, checkable
capacity error for the next title or mouse-control request. STOP remains a
separate one-record terminal reserve: it never overwrites an ordinary request,
is idempotent once queued, and prevents a full ordinary FIFO from making
synchronous destruction impossible. Multi-record control operations must be
all-or-nothing. The existing FIFO order, STOP barrier semantics, and
latest-wins frame behavior remain unchanged.

**Exit:** focused tests fill each component ordinary control mailbox and prove
that the next ordinary request returns the declared error, preserves all queued
records in order, and does not mutate/overwrite any request; STOP consumes its
documented reserve.

## S4 — SoftPC integration-path verification audit

Audit the actual SoftPC tests against these product-observable paths, then add
only the missing deterministic coverage:

- monitor CLI → guest raw Console → monitor CLI: neither input handoff loses
  an input record;
- Window → Console: the newly current native Console is immediately usable for
  input;
- Console → Window: an old Console input cannot be delivered into the active
  product path;
- Window closure and `SOURCE_RETIRED`: no late event reaches an already
  stopped product path;
- UX FIFO control-capacity failure: the product receives and reports the
  failure rather than silently allowing desired/actual state to diverge.

The audit must identify each existing test by file and assertion. A generic
library fake alone is insufficient where it does not traverse SoftPC's control
queue, reconciler, monitor, runtime envelope, and component binding. Missing
coverage must use controllable fakes/completion barriers rather than sleeps or
timing assumptions.

**Exit:** each path has either a named deterministic SoftPC-level test or a
bounded newly added test; the evidence states exactly what it proves and what
it intentionally does not prove.

## Boundaries

- May change: generic `lib/host/win32` Console implementation and its narrow
  host/broker tests.
- Must not change: `src/mvdm/softpc.new/**`, SoftPC app/reconciler policy,
  Window component behavior, package configuration, guest media, or native
  focus calls outside `lib/host/win32`.
- The shared library remains platform-neutral above the Win32 leaf; a future
  Linux leaf may implement the same logical activation contract separately.
