# Raw Console to cooked monitor handoff repair

## T86 S1 admission

The owner admits this queue head as M9 T86 S1 on 2026-10-09.  S1 first
establishes the native transition facts without Computer Use: selected output,
backing buffer, viewport, cursor, logical monitor extent and reader order.
It compares the same ownership point with read-only NTVDM64 and does not make a
source change until one owner-local contract failure is proven.  User-owned
package INI, guest media and snapshots remain untouched.

### S1 evidence, rejected hypothesis and corrective repair

The actual handoff is broker-owned: UI activates the cooked binding, Monitor
writes its prompt, then the broker starts the one `ReadConsole` reader.  The
Win32 contract places `ReadConsole`'s cooked echo at the selected screen
buffer cursor.  The old last step did not re-submit that cursor after the raw
buffer had been active.  A disposable native probe confirmed that the ordinary
ConHost path has a 120x9001 backing buffer and a coherent cursor, while the
reported Terminal/RDP symptom remains consistent with host-side cursor state
being stale after buffer selection/reflow.

The first S1 delivery tried to re-submit the currently observed cursor before
starting `ReadConsole`. Owner validation disproved that hypothesis: the
newly typed cooked input still echoed above its prompt. That operation merely
repeated whichever cursor the host already exposed; it did not restore the
cooked surface's saved cursor as part of selecting that surface.

The corrective repair keeps the two existing buffers and moves the one cursor
operation to the only correct ownership point: display restoration during
raw-to-cooked buffer selection. It restores buffer metadata, explicitly sets
the saved cooked cursor, then restores the saved viewport because moving a
cursor can scroll it. Frame palette updates use the same metadata helper but
do not restore a cursor. Reader creation goes back to its single responsibility
of creating the one reader. This is the same native ordering invariant used by
the read-only NTVDM64 implementation, without importing its different
frontend architecture. No viewport policy, row constant, retry, state machine
or public API is added.

## Observed contract failure

After returning from a raw VM Console to the cooked monitor Console, the
prompt can be displayed at the bottom of the visible native Console while
typed characters and their native echo appear two or three rows above it.
This is a host Console handoff failure, not guest text output: it affects the
shared monitor used by more than one importing application.

The likely boundary is the Win32 Console broker's transition between the raw
surface and the cooked reader/writer.  Backing-buffer capacity, viewport,
cursor location, and the logical cooked output row must become one consistent
native Console state before the reader is armed.  The proposal records that as
a hypothesis only; implementation must first compare the current sequence to
the read-only NTVDM64 repair and reproduce the mismatched native values.

## Intended repair shape

1. Reproduce the raw-to-cooked transition with a native Console probe that
   records logical frame extent, actual buffer size, viewport and cursor
   position immediately before reader activation.
2. Compare that sequence with the read-only NTVDM64 implementation, identifying
   the smallest shared Console/Broker ownership point which establishes the
   cooked surface.
3. Repair one transition contract there: ensure the native surface can contain
   the cooked monitor's required output area, position the logical cursor in
   that area, then arm exactly one native reader.  Do not resize a viewport
   merely to disguise an invalid cursor position; do not add polling, retry
   loops, app-specific rows, or a second reader.
4. Add a focused Win32 desktop contract test where native Console availability
   permits it, plus non-desktop state/coverage proof for the ordering rule.

## Boundaries and acceptance

Likely production owner is `src/lib/console-broker/win32`, with a possible
small `src/emulator/ui` caller adjustment only if current handoff ordering is
wrong.  The task must not change guest presentation, Machine, Session command
state, App configuration, or use an NTVDM64 runtime/build dependency.

Acceptance requires a raw-to-cooked return whose prompt, input cursor and
echo occupy the same logical row; no unnecessary viewport growth; a single
cooked reader; dual-width focused verification and a manual native Console
check.  Any evidence that the condition is a Windows Terminal limitation or
requires a public API change stops for owner design review.
