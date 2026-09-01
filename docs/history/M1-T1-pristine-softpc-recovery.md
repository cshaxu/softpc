# M1 T1: Pristine SoftPC Recovery

## Admission

The owner admitted Queue item 1 under single-person dual-role execution.
This record will contain the completion evidence for the numbered task; the
live packet remains in [Current](../states/CURRENT.md) until T1 closes.

## S1: Freeze The Pristine Recovery Baseline

**Status:** in progress.

**Intent:** establish an evidence-backed, file-complete divergence ledger
before moving any machine source. The selected original is the OpenNT
`nt/private/mvdm/softpc.new` tree. The expected result is a boundary decision
for every current divergence, not a speculative rewrite.

**Evidence required:** a hash manifest, policy-marker sweep, and a reviewed
disposition for every changed or added C/H file.
