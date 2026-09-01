# M1 T1 Pristine Divergence Ledger

## Selected Baseline

- Current tree: `src/core/softpc`
- Original tree: OpenNT `nt/private/mvdm/softpc.new`
- File universe: C and C header files only
- Comparison: SHA-256 byte comparison of same relative paths

## Initial Byte-Level Inventory

| Status | Count |
| --- | ---: |
| Identical original-peer files | 716 |
| Divergent original-peer files | 106 |
| Current files without an original peer | 9 |

This historical byte count is an inventory checkpoint, not a closure input.
The selected tree uses CRLF while the working tree can use LF; a byte hash
would therefore describe checkout format rather than machine ownership.

## Semantic Ledger

The repository-owned audit canonicalizes only carriage returns and terminal
line feeds before comparison. It does not collapse whitespace within source
lines. At the S4 checkpoint it produces 75 actionable rows: 71
same-relative-path semantic differences and four current-only compatibility
inputs. Every row receives one disposition.

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

## Classification Rule

The reproducible manifest is the complete per-file table. Its classification
is deliberately mechanical and has no unclassified state:

- CCPU, C-VID, and `base/inc` peers are `port-abi-overlay`: they carry
  source-width, generated-rule, or declaration adaptation and must be moved
  beside the pristine layer by M1 T2.
- `host/*` peers are `compat-host`: their original algorithms may remain, but
  their platform endpoints belong to M2.
- `base/support/ios.c` is `compat-host`; it is the I/O registration boundary.
- All remaining machine peers are `restore-pristine`.
- `softpc_ccpu_facade.c` and `softpc_standalone_dib.h` are `compat-host`;
  generated `host/inc/x86/prod` headers are `port-abi-overlay`.

Run the command above to enumerate every row. A row's `Next task` field gives
the first admitted cut allowed to act on it; no source-changing task may
silently reclassify a row.
