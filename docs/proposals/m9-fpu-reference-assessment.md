# Existing FPU accuracy assessment

T60 S7 follows the [admitted plan](../history/M9-T60-reference-repairs-proposal.md).
The owner requires retained compatibility and minimum necessary OpenNT diff;
reference behavior is not permission to replace existing contracts.

Assess the selected fpu.c/zfrsrvd.c executor and existing x87 layout tests.
Finite coverage: finite arithmetic, rounding modes and integer conversion,
range/extended precision, zero/infinity/NaN, and the existing save/load layouts.
Separate concrete reproducible faults from the existing double-precision
representation limits. Use product-owned bounded probes and selected-build
evidence; do not claim exhaustive x87 conformance from a small matrix.

No zfrsrvdx86 import, new backend, firmware/media change, asymmetric x86/x64
implementation or Lib/Common/shared-test change. External NTVDMx64 and OpenNT
remain read-only comparison material. Do not implement a compatibility-breaking
substitution or request another exception to the owner's rejection.

For a justified narrow fix: record exact original diff, focused regression,
both fixed EXEs, serial full suites, commit/push and actual-commit review.
For retained behavior: record the tested boundary and reason; do not relabel
a known representation limit as repaired. Whole T60 closure separately checks
S1-S7 and all added original-source divergence.
