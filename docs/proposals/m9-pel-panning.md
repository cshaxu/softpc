# Horizontal pel-panning repair

T60 S4 follows the owner-admitted [finite plan](../history/M9-T60-reference-repairs-proposal.md).
Baseline is the closed S3 implementation 6931729. Preserve original MVDM
structure and minimize original-source diff, not net line count. Lib/Common
and their shared tests remain protected; no source import or new renderer.

Inspect the original attribute-controller register write, panning state,
refresh invalidation and selected graphics painters before changing code.
The NTVDMx64 patch is read-only research, not an implementation payload.

| Member | Required proof |
| --- | --- |
| Register write | A panning-only change requests the required refresh. |
| Pixel displacement | Selected original graphics path reflects the register value. |
| Zero pan | Existing output stays identical. |
| Split region | Confirm the original split/panning compatibility contract and test it. |
| Dirty/full | Incremental and complete rendering agree after pan changes. |

Use product tests only. Each source hunk needs a defect-specific reason and
original-code comparison. Deliver both EXEs, focused proof, serial dual-width
full suites, protected-path and governance checks, then commit/push and review
the actual commit before closure. S5-S7 are not part of S4.
