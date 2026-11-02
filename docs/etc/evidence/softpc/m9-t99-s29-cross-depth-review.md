# M9 T99 S29: mirror conditional cross-depth review

Baseline: S28 closure `107ed85c`; fixed XP SP1 source
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`; OpenNT is read-only comparison.
The on-disk mirror has exactly the 475 manifest files: 471 text and four
binary, with no missing or extra paths. The structural scan inspected all
471 text files, stripped comments for directive recognition and checked raw
excluded bodies separately. It found balanced conditional nesting and no
extra code, comments or nested frames in any of 2,427 direct single-error
bodies. This is structural evidence, not a proof of all preprocessor values.

| Depth | Current arms | Direct single-error arms | Other arms |
| --- | ---: | ---: | ---: |
| 1 | 2,208 | 1,202 | 1,006 |
| 2 | 1,549 | 796 | 753 |
| 3 | 600 | 256 | 344 |
| 4 | 327 | 123 | 204 |
| 5 | 99 | 39 | 60 |
| 6 | 14 | 7 | 7 |
| 7 | 6 | 3 | 3 |
| 8 | 1 | 0 | 1 |
| 9 | 2 | 1 | 1 |

These 4,806 current arms and 2,427 error bodies reconcile with the S20–S28
per-depth evidence. The S20 provisional 140 second-depth XP entries are
accounted in the [S21 closure](../../../history/M9-T99-S21-mirror-depth2.md):
99 exclusion restorations, four separately reviewed, and 37 subsequently
resolved as 36 restorations and one existing frame. The three remaining
known-absent selected wrappers were also restored there. Counts overlap the
current queues and are not additive. The final selected ninth-depth body has
no direct child in either current or XP, and both depth-10 queues are empty.

Nine earlier local-only parent frames (seven in S21, one in S22, one in S23)
have no current direct child left unqueued. They comprise existing input, ABI
and video local extensions; their no-one-to-one-XP status is retained rather
than treating a local frame as original XP. The CHAIN2 local frame in S24 is
likewise recorded separately. These checks do not assert byte equivalence
with whole XP files, nor do they independently re-prove every historical
original-source mapping or every diagnostic line number. Those facts remain
supported by each earlier layer's recorded anchor and build evidence.

The S20 original-unresolved ledger described `base/inc/ica.h`'s local
`#ifndef CPU_40_STYLE` as an exclusion. That wording is too broad: probing
64 actual header dependency contexts found the arm selected in four test
targets per architecture. It must remain. The [context ledger](m9-t99-s29-ica-contexts.tsv)
records all probe outcomes; absence of a marker is not treated as proof that
the arm is excluded. The earlier retained-source result was correct.

No production or test source was changed in S29. S28's x86/x64 194/194
full qualification and published binary hashes remain applicable; S29 uses
its own structural and preprocessing checks rather than claiming a new full
product or customer-runtime validation. The separate x64 verifier timeout
and successful retries in S27 remain recorded; its root cause was not proven.
