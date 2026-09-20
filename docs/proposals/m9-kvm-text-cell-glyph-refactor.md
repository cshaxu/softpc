# Neutral KVM Text Attributes After T71

## Request And Queue Status

Original owner request: "第二项请你拆分到新的t任务proposal加入队列".
Latest owner request: "你以T71为基准，更新这个proposal，并把它提前到队列第一条。"
This remains an unnumbered, queued candidate; no implementation is admitted.
The subsequent owner-approved [T71 plan](../history/M9-T71-kvm-text-frame-contract-proposal.md) now owns
frame resource separation, transparent mailboxes and CP437 relocation.
Those are not repeated here. The earlier combined per-cell Unicode/atlas draft
is superseded; its provenance is T71 S2 P1 d1afdbd5, not a current design.
Queue priority is first, after the active T71; this does not interrupt T71 or
claim that its planned frame migration is already implemented.

## Prerequisite And Excluded Duplicate Work

Use the completed, verified T71 source as the implementation baseline. T71 owns
shared text fields with separate Window bitmap and Console character-map
extensions, Window-only graphics/dirty handling, opaque frame/control transport,
VM ownership of CP437 and capacity/error-boundary clarification. This candidate
does not repeat those migrations or change their ownership and lifecycle.
At admission record T71's final commit and dispose of any objective it already
satisfies; do not recreate work merely to preserve the old proposal's scope.

## Remaining Objective

Remove remaining PC-specific attribute-bit interpretation from shared KVM text
consumers: the product producer resolves it into explicit foreground/background
and glyph/font-selection values. Preserve T71's distinct Window bitmap and
Console mapping extensions and their matching selection semantics.

Changing parallel arrays into an array of cell structs is an optional layout
choice, not the objective or an acceptance requirement. Retain parallel arrays
if they express these neutral values more simply. Do not reorganize data solely
for visual symmetry or promise line-count savings from a struct conversion.
Preserve accepted rendering, including existing intensity/font-selection
outcomes; do not silently repair hardware semantics during normalization.
Do not reintroduce Unicode into every common cell,
a monolithic frame, an encoding registry or a separate resource lifecycle.

## Candidate Batches

1. Audit post-T71 producers/renderers and show before/after structs, attribute
   semantics, memory cost and exact diff estimate. Reconfirm benefit before
   implementation; retaining parallel arrays is valid if simpler.
2. If approved, migrate all producers/consumers and tests together, removing
   old attribute decoding without permanent aliases or another conversion path.
3. Audit actual diff/ownership, dual-width full tests and snapshot behavior;
   publish both EXEs and wait for owner acceptance before T closure.

## Verification And Change Accounting

Freeze a finite producer/consumer ledger at admission: shared text definitions,
VM production conversion, Common-generated status text, Window rendering,
Console map selection, frame copying/comparison and their direct test fixtures.
For every hit record migrated, unchanged with reason, or already handled by T71.
Verify all admitted foreground/background combinations and both font selections
against the pre-change output, plus palette/mapping/font-only changes and
unchanged cursor behavior. Remove old decoding and compatibility aliases in the
same migration; do not add another rendering or conversion route.

Report per-file estimated and actual additions/deletions/net lines separately
for production and tests, and measure frame size/copy effects. The old combined
schema estimates are retired: only incremental changes beyond completed T71
count here. No numerical reduction is claimed before that baseline exists.
Run affected manifest/DAG checks and full serial x86/x64 regression, including
snapshot coverage. Stop for review if neutralization needs new persistent state,
new ownership, hardware behavior changes or a broader frame redesign.

No changes to graph rendering, input, timing, MVDM or snapshot format. Rebase
estimates on the completed T71 source instead of recycling the superseded draft.
