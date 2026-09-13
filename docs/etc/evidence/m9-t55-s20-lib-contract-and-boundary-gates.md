# M9 T55 S20: lib contract and boundary gates

The admitted S20 scope is retained in
[`M9-T55-S20-lib-contract-and-boundary-gates.md`](../../../history/M9-T55-S20-lib-contract-and-boundary-gates.md).

## Frozen-ledger disposition

- Pointer-owning output APIs across `storage`, `host`, `console`, and both UI
  leaves now clear a non-null output before validating unrelated inputs. The
  ownership-transfer API `lib_storage_medium_replace` is intentionally
  excluded: invalid replacement must preserve its existing lease/retired-slot
  contract.
- The component dependency verifier now scans every quoted include in each
  library source/header: each must use canonical `lib/<component>/...`
  spelling, then its edge is validated against the same exact map as CMake
  linkage. Tracked fixtures prove both `host -> ui-window` and a relative
  include are rejected.
- `ui_console_publish_text_frame` rejects null required arguments but retains
  a graphics-frame no-op. `lib_storage_medium_open` no longer performs the
  known no-op close after successful file ownership transfer.
- The S20 P1 technical baseline is recorded as its actual 57/57 dual-width
  and 5/5 strict-library result.
- The one-caller internal `ui_window_display_rect()` forwarding helper was
  removed; its only Window caller constructs the unchanged full client rect.

## Verification

- `softpc-lib-failure-output-contract-smoke` passes at both widths and covers
  every public object-output API whose failure can be reached without native
  allocation or a running worker.
- x64 and x86 both build and pass full CTest: 57/57.
- Strict-library CTest passes 6/6, including the normal source-DAG gate and
  its forbidden-edge and relative-include fixtures.
- Manifest, source boundary and documentation-governance gates pass.

## Accounting

S20 P1 tracked production paths: 15; tracked test/gate paths: 3. Its
source/test change was +151/-40 lines before documentation and generated
package executables. S20 P2 changes five production/gate paths (+25/-21) and
two focused test paths (+6/-4), excluding the regenerated manifest,
documentation, and package executables. The retained public paths keep their
existing ownership; no duplicate runtime path, queue, worker, or callback was
added.
