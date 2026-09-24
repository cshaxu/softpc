# M9 T83 S5 NXVM six-component import audit

S5 copied all 227 paths from clean NXVM commit `440ae83bc` into SoftPC's six
shared roots. SHA-256 comparison after copy found zero mismatches and no
SoftPC-only path. The import adds seven NXVM paths and makes no local shared
adaptation. All temporary build/test logs remained under `build/`.

Both Release package builds passed and refreshed the two EXEs. Full CTest
passed 125/125 on x86 in 479.73s and 125/125 on the final x64 rerun in
432.27s. The initial x64 124/125 result failed only `library.kvm_window_modal`;
it passed three focused repetitions and the complete final rerun. Manifests,
Types layout/boundary, component DAG, documentation governance, and diff
hygiene passed. User INI and media were untouched.
