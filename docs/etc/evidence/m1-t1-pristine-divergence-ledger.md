# M1 T1 Pristine Divergence Ledger

## Selected Baseline

- Current tree: `src/core/softpc`
- Original tree: OpenNT `nt/private/mvdm/softpc.new`
- File universe: C and C header files only
- Comparison: SHA-256 byte comparison of same relative paths

## Initial Reproducible Count

| Status | Count |
| --- | ---: |
| Identical original-peer files | 716 |
| Divergent original-peer files | 106 |
| Current files without an original peer | 9 |

The initial count is an inventory checkpoint, not evidence that every
divergence is semantically necessary.

## Required Dispositions

Every divergent or no-peer item will receive exactly one of these
dispositions before T1 closes:

| Disposition | Meaning |
| --- | --- |
| `restore-pristine` | Restore the original source verbatim; move any required standalone behavior outside it. |
| `port-abi-overlay` | Preserve behavior through a reproducible x86/x64 source/ABI adaptation outside the pristine layer. |
| `compat-host` | Implement an original host contract outside the machine source. |
| `remove-transitional` | Delete a standalone-only workaround after its replacement exists. |
| `blocked` | Stop for an evidenced missing contract; no guessed replacement. |

## Source-Family First Cut

| Family | Current evidence | Provisional next cut |
| --- | --- | --- |
| CCPU (`base/ccpu386`) | scheduler/HLT, BOP decode, pointer-width and standalone helper deltas | S2: restore original execution/BOP flow; then M1 T2 port-ABI overlay for evidenced width changes |
| C-VID (`base/cvidc`) | event glue and GDP state/slot helpers | S2: recover original event source; M1 T2 extracts GDP portability helpers |
| BIOS/controllers (`base/bios`, `base/disks`, `base/keymouse`, `base/system`, `base/video`, `base/comms`) | standalone configuration and host-call substitutions | S2: restore original code where the changed branch is host policy; M2 supplies contracts |
| Imported host renderers (`host/src/nt_*`) | standalone DIB, window, input, sound and render-output substitutions | M2: original host-contract and renderer compatibility; do not keep a parallel renderer in core |
| Current no-peer helpers | CCPU facade, GDP helpers, standalone DIB, generated x86 headers | M1 T2/M2 placement review; none are pristine machine source |

## Policy-Marker Sweep

`SOFTPC_STANDALONE` appears in CCPU, C-VID, BIOS/system, and original host
renderer paths. `runner_pacer` is a VM-layer transitional workaround, not a
machine capability; M3 must remove it once a proven executor boundary exists.

## S1 Completion Table

The following complete per-file table is deliberately filled by the audit
itself. No file is treated as restored merely because its family is known.

| Path | Original peer | Current difference | Disposition | Evidence / next task |
| --- | --- | --- | --- | --- |
| _pending hash-manifest audit_ |  |  |  |  |
