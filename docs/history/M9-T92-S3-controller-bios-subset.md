# M9 T92 S3: Necessary XP Controller/BIOS Subset

## Request and boundary

Owner requests migration of the selected core to XP, not wholesale import of
retained files. Preserve local ABI/snapshot/Compat fixes, exclude unrelated
XP product/hardware branches, use XP formatting and independently account for
necessary-subset differences. Owner additionally permits deleting proven
useless existing conditional branches. Async serial S admission is authorized;
T closure still waits for owner acceptance.

## Delivery and proof

Executor P1 `89b67c44` migrates the reviewed 38-path candidate set, including
CMOS corrections and the descriptor-limit ABI pair. Its actual-change review
found two unused imported A20 declarations. Executor P2 `3cf6906a` removes
exactly those declarations and updates independent XP/local range accounting.

Cumulative production against 5ba0a557: 33 mirror files +1755/-3329 and one
Compat file +2/-2; total +1757/-3331, net -1574. One test file adds 91 lines;
root CMake adds four. Counted production/test/build +1852/-3331, net -1479.
Necessary-XP-subset local patches are +681/-23, including historical repairs.
Excluded XP source is separately counted at 8993 lines. Source selection and
local patches are detailed in the linked proposal and its range ledgers.

P2 rebuilds and refreshes both EXEs. Each architecture passes all 148 tests:
90 shared, 53 other background and five native desktop. The focused five-device
subset also passes. Every phase and supervisor exit is zero. No Computer Use,
user configuration/media/snapshot edit or manual guest-session claim.

SHA-256:

- softpc32.exe: `269f738ee4b52c1227f1babecfc4a6acfcca216e1427cff92e2d0700dbab4f01`
- softpc64.exe: `84447c00e1630bf570315dca3047cd9242023d61bca9383e267a4da617b01ca9`

## Coordinator actual-change review and closure

After P2 push, independently reviewed its eight changed paths: production
contains only the two declaration deletions; three ledgers reflect that same
selection, two EXEs have the recorded hashes, and two documents retain the
failure/correction and qualification evidence. The S3 cumulative source
accounting was rechecked. Public packages and protected assets are unchanged.
Remaining guarded A20 declarations/callers in sas.h/keyba.c are S4 candidates,
not silently removed. Initial whole-file import and positional merge mistakes
remain documented; passing tests do not erase these findings.

S3 is implementation-closed under delegated async authority, not marked as
manually accepted by the owner. T92 stays open. S4 is serially admitted for
the input/reset/IRQ/ROM/timing subset. Scope, source selection, failed attempts,
test timings and per-path evidence remain in the
[migration proposal](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md).
