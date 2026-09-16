# M9 T62 S4: Console event ownership

Original request: [retained proposal](M9-T62-common-lib-simplification-proposal.md).
Baseline 6a60759; implementation aa1eabd committed and pushed.

Coordinator actual-diff review confirms the existing receiver, reset and key
normalizer state now belong to component root. The native worker still owns
start/join, sink installation/detachment and callback quiescence before source
retirement. Only platform button translation remains beside it. Window-specific
input decoding and the shared kvm-base matcher are unchanged. There is no second
receiver, state machine, queue or public API. Existing native worker allocation
remains its platform lifetime boundary, not a new wrapper or allocation.

Four production C/H paths +85/-71 = +14, with 66 lines of relocated processing
explicitly not counted as behavior deletion. Two test C paths +10/-1 = +9;
no CMake changes. Existing copied lib_i32 stores native COORD positions, with
unchanged X8/Y16 scaling; no new types alias or platform type in root.

Both preset builds passed; full x64 101/101 (61.73s), x86 101/101 (63.32s).
Strict standalone Lib build and Linux placeholder compile passed; Linux startup
remains UNSUPPORTED. Actual-commit focused checks 14/14 each (2.87s/2.77s)
include keyboard/text equivalence, callback retirement, shutdown, manifests,
component DAG and documentation. Added mouse-button/source checks preserve
baseline reset and scaling. Public headers and protected product paths are unchanged.

S4 closes after this review. No next implementation S is admitted; T62 remains
open awaiting the owner's manual validation of both fixed EXEs.
