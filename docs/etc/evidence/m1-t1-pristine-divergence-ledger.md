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

## Reproduction

Run the repository-owned read-only audit with the selected original root:

```powershell
.\scripts\audit_pristine_divergence.ps1 `
  -OriginalRoot O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new
```

It emits the complete path-level manifest, including the source family,
standalone-marker count, and a unique disposition. The tracked ledger records
the boundary rationale; the script prevents its file universe from silently
drifting.

## Required Dispositions

Every divergent or no-peer item will receive exactly one of these
dispositions before T1 closes:

| Disposition | Meaning |
| --- | --- |
| `restore-pristine` | Restore the original source verbatim; move any required standalone behavior outside it. All 106 divergent original-peer files have this disposition. |
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

The reproducible manifest is the complete per-file table. Its classification
is deliberately mechanical:

- Every same-relative-path original peer is `restore-pristine`. A buildable
  port may later need a generated transform, but that belongs beside—not
  inside—the restored source.
- `softpc_ccpu_facade.c` and `softpc_standalone_dib.h` are `compat-host`.
  They cannot be pristine-machine files because neither has an original peer.
- GDP state/rule helpers and generated `host/inc/x86/prod` headers are
  `port-abi-overlay`; they represent host/compiler compatibility inputs.

Run the command above to enumerate every row. A row's `Next task` field gives
the first admitted cut allowed to act on it; no source-changing task may
silently reclassify a row.
