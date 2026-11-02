# M9 T99 S24: fifth-depth reconciliation

Baseline: S23 closure `5b7a5bee`; exclusion batch `2434946c` is pushed.
The fixed XP SP1 subset is the mirror authority; OpenNT is used to validate
existing diagnostic line controls. No public component, test source, INI,
guest media or snapshot changed.

Final fifth depth contains 99 arms: 39 structurally single-error and 60
selected. One former fifth-depth error moved to depth 6 after restoring the
selected enclosing frame. Six original selected frames were restored. The
[selected parent map](m9-t99-s24-depth5-selected-map.tsv) maps 59 selected
parents to XP; the remaining parent is the already accepted local T88 CHAIN2
frame, for which XP has no corresponding condition. No new behavior was added.

The two sixth-depth queues contain 14 current arms and 14 XP direct-child
candidates; these are corresponding overlapping candidates, not 28 separate
issues or sixth-depth dispositions. See the [current queue](m9-t99-s24-depth6-provisional-queue.tsv)
and [XP queue](m9-t99-s24-depth6-xp-direct-queue.tsv).

Compared with the exclusion commit, preprocessed output of each of the five
changed source files was byte-identical for x86 and x64 using its actual build
commands. The actual source diff relative to S23 is five mirror files +23/-1
(net +22); test-source diff is zero. The selected phase contributes +21/-0.
The 40 direct error arms at the exclusion point, guard-negative checks and
first full qualification are recorded in the [exclusion report](m9-t99-s24-depth5-excluded.md).

Build and test evidence covers supported configurations; it does not establish
correctness of configurations deliberately rejected by the mirror.

Final dual-architecture builds and mirror/documentation gates passed. On each
architecture, all 186 non-native tests and eight serial native desktop/audio
tests passed without skips. Published asset SHA-256 values:

- softpc32.exe: `195f37b77907d13bf9dd079997348edaf327a208af8a6977e16d8523150fc6b2`
- softpc64.exe: `dbe694fa3e3b26c3b972fd01eac6195d65a10837d32d42a4a17fb788eb62348d`

Actual diff review, selected-path comparison, condition mapping, queue hashes
and `git diff --check` passed. Scratch remains owned by S24 until closure.
