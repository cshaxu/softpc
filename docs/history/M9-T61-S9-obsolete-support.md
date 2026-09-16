# M9 T61 S9: obsolete support contracts

Original request: retire test-maintained obsolete support without weakening
production-path proof, then continue the approved S10-S13 ownership cleanup.
Owner additionally requires per-S before/after file, function and line accounting.

Implementation 1b43329 is pushed. Coordinator reviewed all actual source/test
diffs: deleted unused status/PIG interfaces, presentation option and single-font
wrappers; retained double-font behavior and changed shutdown proof to real App
composition/Session with only UI faked. No lifecycle implementation changed.

Six production paths +0/-124 (estimated -120 to -130), twenty test paths
+134/-99, net +35 (estimated +40 to +70; removed redundant calls/initializers).
No relocation or mirror/shared-corpus changes. Detailed census, initial test
migration failures and artifact hashes are in the
[evidence ledger](../etc/evidence/softpc/pristine-divergence-current.md).

Both builds and full suites passed 98/98. Actual-commit shutdown, VGA/font,
command-provider, source-boundary and documentation checks passed 5/5 per
width (x64 3.95s, x86 4.50s). User INI change committed unchanged; owned logs
removed, existing build trees retained. No manual GUI acceptance claimed.

S9 closes; S10 is admitted under the owner's serial authorization. T61 remains
open. Queue and unrelated keyboard-boundary TODO are unchanged.
