# M9 T99 S26: seventh-depth reconciliation

Baseline: S25 closure `2bfd64ac`; the [exclusion delivery](m9-t99-s26-depth7-excluded.md)
was pushed at `01236cb8` after dual builds, publication checks and all 194
tests per architecture passed without skips. The source authority is the fixed
XP SP1 subset, with read-only OpenNT for diagnostic comparisons.

The six seventh-depth arms comprise three existing single-error arms and
three retained arms observed on both architectures. All three retained frames
correspond to the XP direct-child candidates, and no frame restoration was
needed. Production and test source changes for S26 are zero. The retained
review changed only documents and queues, so the exclusion batch's build,
published binary hashes and test results remain the final qualification.

The [current eighth-depth queue](m9-t99-s26-depth8-provisional-queue.tsv)
and [XP eighth-depth queue](m9-t99-s26-depth8-xp-direct-queue.tsv) each
contain one corresponding direct child, not two separate issues. No
eighth-depth body was reviewed or changed. The checks establish supported
configuration behavior, not support for intentionally excluded conditions.
