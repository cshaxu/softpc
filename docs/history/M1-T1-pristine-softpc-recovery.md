# M1 T1: Pristine SoftPC Recovery

## Admission

The owner admitted Queue item 1 under single-person dual-role execution.
This record will contain the completion evidence for the numbered task; the
live packet remains in [Current](../states/CURRENT.md) until T1 closes.

## S1: Freeze The Pristine Recovery Baseline

**Status:** complete.

**Intent:** establish an evidence-backed, file-complete divergence ledger
before moving any machine source. The selected original is the OpenNT
`nt/private/mvdm/softpc.new` tree. The expected result is a boundary decision
for every current divergence, not a speculative rewrite.

**Evidence required:** a hash manifest, policy-marker sweep, and a reviewed
disposition for every changed or added C/H file.

**Completion evidence:** commit `0500ef1` adds the repository-owned audit.
It enumerates 115 non-identical current C/H files: 106 original peers assigned
`restore-pristine`, 2 compatibility-host files, and 7 port-ABI files.

## S2: Recover Original Execution-Source Boundaries

**Status:** in progress.

**Intent:** restore original CCPU execute/BOP flow and C-VID event glue before
the compatibility host or runtime is redesigned. Any host-width declaration
needed to compile belongs to a mechanically explainable port-ABI boundary, not
to scheduler or product behavior in machine source.
