# M9 T80 S7: Storage fill flush boundary

## Admission and design

Owner: “收口S6，准入S7，预估diff规模，完成后编译测试提交推送。”
Baseline `d8ae087f`, clean worktree. S6 is owner-closed; T80 stays open.
After requesting a smaller explanation, the owner approved one internal write
algorithm with per-public-operation flush ownership. Follow existing
architecture/coding/execution/documentation authorities; no new public API,
switch parameter, cache, transaction layer or platform implementation.

The original write algorithm becomes static and loses only its flush.
Public write calls it and flushes only on successful DIRECT writes, preserving
old failure/zero-length behavior. Fill retains the 512-byte stack buffer,
range validation and chunk loop. Empty fill returns OK without I/O, including
READONLY. Nonempty DIRECT fill flushes once after the loop, including partial
failure; the earlier write failure wins. READONLY nonempty still fails;
OVERLAY still writes pages only. No rollback or power-loss durability claim.

## Finite sweep and change accounting

Frozen scope: Storage write/flush entry points and all production fill callers.
Search: `rg -n 'lib_storage_medium_fill_at|lib_storage_file_flush|lib_c_fflush'
src -g '*.c'`, plus write_exact/write_at in Storage.

- Medium shared write body: identical after rename and removal of flush.
  Preceding read/page allocation functions also identical.
- Public write: one existing-operation final flush; behavior retained.
- Fill: only avoidable explicit per-chunk flush site, now operation-scoped.
- Overlay creation: still calls public write; overlay never flushes its base.
- File writer: write/close stream lifetime is distinct; retained unchanged.
- Platform seek: retained. CRT seek may itself handle output buffering, so
  fewer explicit flush calls do not prove fewer physical I/Os or a speedup.
- Production fill callers: none outside its definition in this repository.
  This improves the reusable API, not a measured product startup hot path.

`git diff --numstat d8ae087f` restricted to code:
medium.c +20/-7 (net +13); medium_interface.h +3/-0 (comments only);
storage_file_writer_binary_smoke.c +88/-0. Production +23/-7 (net +16);
all code +111/-7 (net +104). Manifests/docs/EXEs separate. Production is
within initial +20--30/-6--12 estimate; tests below 100--150 by reusing the
existing fixture, not creating a framework or another test target.

## Focused proof

New counter assertion fails against original code, then passes after repair.
Real temporary stream, first/second chunk short-write and flush injection:
1 MiB = 2048 writes/one explicit flush; 1 MiB+7 tail = 2049/one;
full readback, unfilled neighbours, empty/end/overflow/NULL, all three modes,
overlay page/tail crossings, first/second short write with successful/failed
flush, standalone flush failure, unchanged public write success/failure.
Real test files live only in build trees or tmpfile, never assets.

## Verification and artifacts

Shared strict C11 -Wall -Wextra -Wpedantic -Werror, both Release builds pass.
Background presets: x64 111/111 (214.52s), x86 111/111 (194.39s), five desktop
cases excluded per width. Standalone test/lib builds and background tests:
x64 41/41 (60.92s), x86 41/41 (63.05s), three desktop cases excluded per width.
Six manifests and component DAG gates pass within product suites; final
documentation gate and diff checks pass. Existing Core warnings remain outside
the changed shared code; no claim of a warning-free original mirror.
No native Linux or new downstream integration acceptance claim.
Task-owned build/t80-s7 removed after evidence capture; no live task processes.
Common/x86/Core sources, INI and user media unchanged.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3693615 | F18C5628F4F906E081122094E254F369D7FBAC109C0E27BE69B8F8F33B368211 |
| softpc64.exe | 3080018 | D07BF9064A722BD28D29663F6A05FD778E39074004FAFDEE6078F9819D2C5EAD |

## Actual-change review

Executor `75dfd5a6` is pushed. The same session switched to coordinator and
reviewed `git diff d8ae087f..75dfd5a6`: twelve paths comprising two production
files (header comments only), one existing test, two manifests, five governance
documents and two EXEs. Rechecked shared write ownership, unchanged normal
write flush condition, early empty-fill return and unconditional DIRECT final
flush with first-error precedence. No second write algorithm or public option
was introduced. Fault/counter assertions, manifests, counts and package hashes
match evidence; no unrelated source/config/media changed. Owned scratch is
removed and final documentation governance passes. The admitted narrow scope
has no unresolved implementation item. S7 awaits owner acceptance; T80 stays
open for the separately required T-level acceptance/closure audit.
