# M9 T99 S42: unused mirror and snapshot retirement closure

Owner accepts and explicitly closes S42 on 2026-11-08. Final implementation
60bdbaa4 is committed and pushed. No subsequent S is admitted; T99 remains
open pending its separate completion audit and owner approval.

P1 (42533c7d) repaired temporary mouse coordinate restoration, then call-graph
review established that the old INT 33h driver was not dispatched by the
product. Owner authorized P2 (421b2fcb) to retire mouse_io.c and its dedicated
adapter/test, P3 (b12bc8be) to retire unused nop.c/vglob.c, and P4 (60bdbaa4)
to remove the remaining snapshot compatibility payload. All admitted units
are complete. See [proof and phase accounting](../etc/evidence/softpc/m9-t99-s42-mouse-coordinate-restoration.md).

The retained production paths are InPort mouse with the guest driver, CPU
NOP handling in the decoder, and generated C-VID accessors. No replacement
implementation or compatibility reader was added. SCP3 snapshots omit the
1808-byte old driver payload; SCP2 is rejected at the existing header check.
Existing snapshot files are not rewritten. The retained mirror has 472 files.

Cumulative source accounting, using git diff 42533c7d^ 60bdbaa4 --numstat
for src and test: production/source lists +4/-7212 (net -7208); tests +5/-30
(net -25). This counts whole-file retirement, not reduction in local XP
patches. P4 alone has zero mirror-diff change; phase-specific mirror patch
and subset accounting remains in the evidence record.

Final dual-width builds publish both EXEs. Each architecture passes 188
background plus eight native tests, 392/392 total, zero failures/skips.
Warnings are confined to unchanged mirror source. Coordinator actual-change
review confirms symmetric archive deletion, early old-format rejection and
retained live device state. No new quality defect was found in this scope.

Closure changes documentation only; no redundant runtime rebuild is needed.
Documentation, whitespace and build-residue checks pass. Owned scratch is
removed; reusable caches remain. A new owner-side assets/binary/snapshot
modification is preserved and excluded from this documentation commit.
