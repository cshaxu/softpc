# M9 T62 S3: shared Storage length measurement

Original request: [serial proposal](M9-T62-common-lib-simplification-proposal.md).
Baseline f4b090b; implementation 0811c31 committed and pushed.

Actual-diff coordinator review confirms one root save/end/tell/restore path,
same-shape platform seek/tell, unchanged public API/open/locking/short-circuit
failure semantics and output count updated only on success. Search of all
Storage position consumers found no retained duplicate length implementation.
Read/write/close were already shared and remain unchanged.

Production four paths +26/-37 = -11; three test C paths +59/-0 and test CMake
+1/-0. Both preset builds and fixed EXEs refreshed; full x64 101/101 (84.96s),
x86 101/101 (72.09s), strict standalone Lib build passed. Tests cover all four
injected positioning failures, >4 GiB simulated counts, empty/hole files and
nonzero position restoration including controlled Linux calls. Actual-commit
storage/manifests/corpus/documentation checks: 11/11 each (2.42s/2.26s).
INI/media and other production components unchanged.

S3 closed; automatically admit S4. T62 remains open for owner testing.
