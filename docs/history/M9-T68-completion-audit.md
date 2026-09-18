# M9 T68 completion audit

Owner request: “可以的，你加个T修复，然后加上正确的测试后编译测试提交推送关闭T”。

## Finite closure ledger

Universe: every standalone-reachable Scan-1 array-bound guard in the preserved
keyboard mapper, plus its unit proof.

| Member | Disposition | Evidence |
| --- | --- | --- |
| Regular `Scan1ToKeynum` lookup | Repaired | The exclusive byte-count upper bound now rejects scan 133. |
| Enhanced `Scan1ToKeynumExtended` lookup | Repaired | The same correction rejects scan 84. |
| Standalone `softpc_host_scan1_to_key` helper | Retained | It already uses correct `< sizeof(table)` logic. |
| Original non-standalone `aNullCharScan` route | Retained out of runtime scope | It is compiled only in the excluded non-standalone branch and is not a standalone product input route. |
| Valid table mappings | Preserved | Existing regular and enhanced full-table fingerprints remain unchanged. |
| New mapping/wrapper/state | Not added | The original tables and their direct mapper remain the sole owner. |

## Actual accounting

Tracked production source is `host/src/nt_keycd.c`: +3/-2 (net +1), consisting
of two strict bound comparisons and one local original-source safety marker.
Tracked test source is `test/unit/keycode_smoke.c`: +2/-0, asserting the two
first invalid indices return zero. Protected OpenNT mirror delta is the same
production +3/-2. Documentation and package artifacts are excluded. No
obsolete product path exists: the existing mapping table is retained as its
distinct original owner.

## Verification and delivery

- Focused `softpc-keycode-smoke` passed on both widths after the final source
  revision, preserving both table fingerprints and testing 133/84 rejection.
- Final x64 full suite: 107/107 in 94.51 seconds. One preceding full run hit
  the already-recorded intermittent BIOS tick smoke; standalone retry and the
  final full suite passed without a keyboard-related change.
- Final x86 full suite: 107/107 in 120.97 seconds.
- Documentation governance and diff checks passed.
- Both package EXEs were refreshed from the corrected source after the owner
  finished the running x64 test instance.

T68 is closed. The remaining candidates remain in
[Queue](../states/QUEUE.md); no new work is admitted by this record.
