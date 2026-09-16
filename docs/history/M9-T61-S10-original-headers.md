# M9 T61 S10: reuse original GDP/SAS declarations

Owner approved serial S8-S13 cleanup and per-S pre/post accounting.
Implementation e730f43 is pushed. Coordinator reviewed all actual changes,
original-header comparisons, selected preprocessor outputs and dependency census.

Removed two Compat duplicates, 1583 lines. Five source paths +3/-1589, net
-1586; CMake -4; product gate +8. No relocated functionality, new implementation
or shared-corpus change. Original mirror is 498 retained / 404 identical /
94 divergent, +23137/-22334 and 5131 hunks, reducing five changed lines and
one divergent file. sascdef now matches OpenNT bytes. Native-width GDP stays
in the existing Compat support. Details and before/after evidence are in the
[ledger](../etc/evidence/softpc/pristine-divergence-current.md).

Both builds and full suites passed 98/98 (x86 78.70s, x64 82.30s).
Actual-commit machine, VGA, input-continuation, source-boundary and documentation
checks passed 5/5 per width (x64 9.18s, x86 8.02s). No product test failed;
diagnostic script corrections are recorded. Fixed EXEs refreshed; INI and
media unchanged; owned preprocessor evidence/logs removed after recording.

S10 closes. S11 is next under the approved sequence; T61 stays open pending
the remaining ownership and convergence work. No manual GUI acceptance claimed.
