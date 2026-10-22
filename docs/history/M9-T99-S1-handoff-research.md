# M9 T99 S1: Public handoff research

The owner closes the research on 2026-10-22 by admitting the two definite
cleanups as S2, and scheduling separate pause-key and intermittency research.
T99 remains open. Delivery 29c260b5 and actual-commit review bf18a2f7 are pushed.

The [proposal](../proposals/m9-public-handoff-simplification.md#s1-research-delivery-2026-10-22)
retains the five observations, source traces and per-observation dispositions.
No production/test/ABI change was made. Existing cached x64/x86 focused cases
pass 7/7 each; full regression, desktop and guest stuck-key reproduction were
not performed. All historical failures remain unresolved, not hidden by later
passing tests. Documentation/diff/residue checks passed; scratch was retired.

S2 owns only repeated Broker cleanup and the redundant auto-reset event reset.
S3/S4 are planned but not admitted; the original five issues stay in the
proposal rather than returning to TODO or the T queue as duplicate entries.
