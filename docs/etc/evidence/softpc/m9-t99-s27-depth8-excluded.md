# M9 T99 S27: eighth-depth exclusion audit

Baseline is S26 closure `975596e6` and the fixed XP SP1 subset, with read-only
OpenNT for diagnostic comparisons. The sole current and XP eighth-depth
candidate is `#ifndef GISP_CPU` in `base/video/ega_writ.c`. Actual x86 and
x64 preprocessing selected it in both supported contexts; there is no
excluded eighth-depth body or production change in this batch. No ninth-depth
body is assessed here.

Both builds and mirror/documentation gates passed; published binaries were
unchanged from S25, SHA-256:

- softpc32.exe: `be3c1f1d26766762254947f70fad76180ffb772f5a3b7b7ebeac2549df6584f4`
- softpc64.exe: `1dca80444393d841ccb77849efd32691aa9fc78f423df920c58999916f1b7763`

The initial x64 non-native run had 185/186 pass; `emulator.verifier-negative`
exceeded its existing 60-second budget. The Types layout selftest also took
169.84 seconds on that run. The verifier passed alone in 35.31 seconds, then
passed in 17.50 seconds in the complete `-j2` rerun. Both architectures then
passed all 186 non-native and eight serial native tests without skips. This
does not establish why the first timeout occurred; no test budget was changed.
Production and test-source changes are zero.
