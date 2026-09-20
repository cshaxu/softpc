# Explicit KVM Text Cells, Colours And Raster Glyphs

## Request And Queue Status

Original owner request: "第二项请你拆分到新的t任务proposal加入队列".
This remains an unnumbered, queued candidate; no implementation is admitted.
The subsequent owner-approved [T71 plan](../history/M9-T71-kvm-text-frame-contract-proposal.md) now owns
frame resource separation, transparent mailboxes and CP437 relocation.
Those are not repeated here. The earlier combined per-cell Unicode/atlas draft
is superseded; its provenance is T71 S2 P1 d1afdbd5, not a current design.

## Remaining Objective

After T71, assess whether the shared text's parallel glyph/attribute arrays
should become explicit cells with glyph selection and foreground/background
values. Preserve distinct Window bitmap and Console mapping extensions.
Normalize product attribute interpretation only if the result is simpler and
preserves accepted rendering. Do not reintroduce Unicode into every common cell,
a monolithic frame, an encoding registry or a separate resource lifecycle.

## Candidate Batches

1. Audit post-T71 producers/renderers and show before/after structs, attribute
   semantics, memory cost and exact diff estimate. Reconfirm benefit before
   implementation; retaining parallel arrays is valid if simpler.
2. If approved, migrate all producers/consumers and tests together, removing
   old attribute decoding without permanent aliases or another conversion path.
3. Audit actual diff/ownership, dual-width full tests and snapshot behavior;
   publish both EXEs and wait for owner acceptance before T closure.

No changes to graph rendering, input, timing, MVDM or snapshot format. Rebase
estimates on the completed T71 source instead of recycling the superseded draft.
