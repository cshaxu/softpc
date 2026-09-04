# M8 T14: Pristine-map And Divergence Manifest Proposal

## Boundary

Read-only comparison of the current recovered tree with the selected OpenNT
`mvdm/softpc.new` tree.

## Work

Produce a rerunnable path/hash manifest that classifies every source and
header as identical original, original with direct diff, original omitted,
local standalone source, generated input, or generated output. Assign every
direct diff a proposed overlay or host/app disposition. Establish the selected
original revision and excluded binary/object inventory.

## Verification And Exit

Re-run the manifest; inspect every non-identical controller, BIOS, CCPU,
C-VID, and `nt_*` row. The task exits only when no source path is ambiguously
described as original when it differs from OpenNT.
