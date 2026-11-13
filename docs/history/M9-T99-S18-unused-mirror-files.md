# M9 T99 S18: Remove Unused Mirror Files

The owner accepts and closes S18, including the follow-up removal of three
unobserved locally patched NTVDM headers. Executor deliveries 664be095 and
bcbf0bb8 and actual reviews 1866b308 and 8383891d were pushed. Thirty files
were removed with XP raw/subset/local identities recorded; 475 mirror bodies
remain. Both widths passed all 193 registered tests first run following the
final removal, and EXEs were byte-identical to S18 P1. The initial S18
relink differed from S17 only in PE timestamp and checksum, as documented in
the [proposal](../history/M9-T99-public-handoff-simplification-proposal.md#s18-whole-file-mirror-selection).
No public component, user INI, media or snapshots changed. T99 remains open.
