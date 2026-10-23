# M9 T99 S14: Original VGA Cursor Shape

Owner accepts and closes S14: 批准收口S14.然后列出所有的S任务、T任务、TODO.
Executor 77326df5 and actual-commit review 755d0445 are pushed. T99 remains
open for a separate task-level completion audit; no next S/T is admitted.

The original request admits VGA end-scanline/wrap research and transfers its
TODO lead into S14. Research f074452f/review ff97104b establishes the minimum;
the owner then approves implementation. The original VGA do_new_cursor owner
now includes the end scanline, hides reversed VGA shapes, preserves disable,
protection and overflow behavior, and continues through existing notifications
and dirty handling. EGA/CGA and public eight trees are unchanged. The existing
App fixture covers actual register writes, both Driver text leaves, four font
heights and snapshot register replay. No new state/API/thread/alternate path.

Production +6/-33 (net -27); test C +49/-19 (net +30); combined net +3.
Relative to the immutable XP necessary subset, the file's historical changed
line count grows by 39; source deletion is not reduced mirror divergence.
The [implementation record](../proposals/m9-public-handoff-simplification.md#s14-approved-implementation)
retains primary references, five-member research dispositions, exact mirror
identities, hashes, counts, timings, corrected runner selector and proof limits.

Cached x64/x86 builds refresh both EXEs. Each architecture passes every 193
registration: 118 shared background, 67 other background, five desktop and
three native-audio cases; exact executed/registered names, zero failures/skips.
Separate focused VGA tests pass on both widths. Permanent coverage includes
160 shape observations and 48 selected controller snapshot replays per width.
Second builds report no work; no warning outside preserved mirror headers.
INI, media, snapshot payloads/formats and shared components remain unchanged.
No Computer Use or new manual guest test is claimed. End==cell-height physical
hardware behavior retains the research limit; this is not universal adapter
certification. User acceptance supplies the owner decision, not extra tests.

Actual committed changes were reviewed against scope and source policy, with
no delivery blocker. Documentation, mirror, whitespace and build-residue gates
passed; owned scratch is retired and reusable caches remain. This closure only
updates records and reruns documentation/whitespace/residue checks, not builds
or full tests. S1 through S14 are closed; TODO is empty. The sole unnumbered
performance candidate remains in Queue, unaffected by this repair.
