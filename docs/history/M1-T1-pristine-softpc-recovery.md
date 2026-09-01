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

**Status:** complete.

**Intent:** restore original CCPU execute/BOP flow and C-VID event glue before
the compatibility host or runtime is redesigned. Any host-width declaration
needed to compile belongs to a mechanically explainable port-ABI boundary, not
to scheduler or product behavior in machine source.

**Progress evidence:** [S2 execution-boundary evidence](../etc/evidence/m1-t1-s2-execution-boundary.md)
records the recovered source flow, external `EDL_fast_bop` contract, successful
x64 CCPU compile/link, and BOP smoke. It also records why the old full CTest
cannot close this task: its transitional run-slice contract is unbounded once
the CCPU instruction-budget workaround is correctly removed.

## S3: Extract C-VID Port-ABI Inputs

**Status:** complete.

**Intent:** restore original `ev_glue.c` as a machine input while relocating
GDP storage and generated-rule adaptations to `softpc-port-abi`. The build
will consume a deterministic transformed copy for the x64 representation; the
checked-in machine source must not carry that host policy.

The resulting transform is idempotent and both x64 and x86 focused BOP smoke
executables pass. See [S3 C-VID port-ABI evidence](../etc/evidence/m1-t1-s3-cvid-port-abi.md).

## S4: Reconcile Remaining Pristine-Source Divergences

**Status:** complete.

The audit now compares canonical source text rather than historical line
endings. It has 75 actionable rows, each assigned to `restore-pristine`,
`port-abi-overlay`, or `compat-host`; there is no unclassified state.

The clear original-machine recoveries landed as commits `8b72547`, `b304479`,
and `958b48f`: the BIOS BOP service group, the original 8272A floppy
controller input, and the build-ID BIOS service. Both x64 and x86 rebuilt
after each recovery; BOP smoke passed after the BIOS group and FDC smoke
passed after the controller recovery. The updated
[semantic ledger](../etc/evidence/m1-t1-pristine-divergence-ledger.md) is the
full per-file evidence.
