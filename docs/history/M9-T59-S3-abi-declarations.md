# M9 T59 S3 ABI declarations

Executor delivery 6db7c45 pushed; the [proposal](../history/M9-T59-product-boundary-cleanup-proposal.md)
records every extern group and retained original-header limitation.
Coordinator inspected actual changes: only declarations/includes/comments moved,
no function body algorithm, CPU protocol or new wrapper. Both final builds and
86/86 suites pass. 21 production C/H paths +122/-107, net +15; no test source
change. Lib/Common/MVDM and user media/configuration remain unchanged.
S3 is closed; S4 single-instance admission proceeds. T59 stays open.
