# M9 T55 S20: lib contract and boundary gates

The admitted S20 scope is recorded in
[`m9-t55-s20-lib-contract-and-boundary-gates.md`](../../../proposals/m9-t55-s20-lib-contract-and-boundary-gates.md).

## Frozen-ledger disposition

- Pointer-owning output APIs across `storage`, `host`, `console`, and both UI
  leaves now clear a non-null output before validating unrelated inputs. The
  ownership-transfer API `lib_storage_medium_replace` is intentionally
  excluded: invalid replacement must preserve its existing lease/retired-slot
  contract.
- The existing component dependency verifier now scans direct quoted
  `lib/<component>/...` includes in every library source/header and validates
  them against the same exact edge map as CMake linkage. A tracked fixture
  proves `host -> ui-window` is rejected.
- `ui_console_publish_text_frame` rejects null required arguments but retains
  a graphics-frame no-op. `lib_storage_medium_open` no longer performs the
  known no-op close after successful file ownership transfer.
- `CURRENT.md` now reports the prior S19's actual 56/56 dual-width baseline.

## Verification

- `softpc-lib-failure-output-contract-smoke` passes at both widths and covers
  every public object-output API whose failure can be reached without native
  allocation or a running worker.
- x64 and x86 both build and pass full CTest: 57/57.
- Strict-library CTest passes 5/5, including the normal source-DAG gate and
  its forbidden-include fixture.
- Manifest, source boundary and documentation-governance gates pass.

## Accounting

Tracked production paths: 15; tracked test/gate paths: 3. Source/test change
is +151/-40 lines before documentation and generated package executables.
The retained public paths keep their existing ownership; no duplicate runtime
path, queue, worker, or callback was added.
