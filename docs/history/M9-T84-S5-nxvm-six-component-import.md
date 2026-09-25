# M9 T84 S5 — NXVM six-component import

The owner accepted S5 on 2026-09-25. Executor `e2b81d70` imports NXVM clean
`057d8c9aa5edde1bda794a03fb77d07fc8b67bba`'s complete Lib, Common and x86
source/test corpus, with the owner-approved shared `test/register.cmake`
location. All six roots and the helper compare byte-for-byte with that source.

The imported corpus changes 104 paths, +1177/-1006 (net +171). Six necessary
SoftPC receivers adapt to the imported one-byte `lib_bool` callback/layout
contract, +40/-36 (net +4), without a second product behavior path. With task
documentation, the total is +1261/-1056 (net +205). Core mirror, Compat, VM,
App policy, user INI, snapshots and guest media are unchanged.

Both Release packages and both hidden-background CTest suites pass 120/120.
The x86 SHA-256 is
`F1E748E86FF800ACF4C1BE8FF75B675BC4995CD3FC39752B948D5D32620E8658`; the
x64 SHA-256 is
`840347402647A6F4988DCC16A4B22E6E5FF853D5E15E7C78A26331DB74410856`.
Strict standalone C11 builds and manifest/DAG/corpus/negative/documentation
gates pass. Desktop tests remain excluded; this import makes no Linux-runtime
or downstream-product claim.
