# M9 T84 S3 — NXVM deterministic Audio test import

The owner accepted S3 on 2026-09-25. Delivery `b728d1f0` replaces SoftPC's
physical Win32 endpoint/loopback probe with NXVM shared commit `b7cbb30a9`'s
deterministic adapter test. The imported CMake entrypoint,
`audio_win32_platform_smoke.c`, and `test/lib` manifest have identical Git
object hashes to that NXVM commit; the deleted local probe is deliberately not
retained as a second test path.

Production C/H changes are +0/-0. Test/build sources are +220/-212, net +8:
the 216-line deterministic test replaces the 208-line physical test, and the
two metadata substitutions complete the transition. No public API, ABI, Core,
Compat, VM, App, INI, media or snapshot change belongs to S3.

Focused Audio tests passed on both widths. Complete background CTest passed
x64 120/120 in 250.98s and x86 120/120 in 249.05s. Both package builds passed;
the package SHA-256 values are x86
`FEA84DEF15C2A25DC962579087D60F5DC94E11B9E46A792E13B3C3B60408D8A5` and x64
`0EC6096600E1702CE92751F4AEE1BC16C5FD009FF4E6D2DF537A97774672148F`.
All six manifests, the Lib component DAG, Common/x86 corpus, whitespace and
documentation-governance gates passed. Desktop tests remain excluded; S3 makes
no Linux-runtime or native-endpoint claim.

NXVM subsequently received uncommitted edits inside its shared roots. They
were discovered after S3's import and are not part of this accepted task;
S3's exact-import claim is intentionally pinned to `b7cbb30a9`.
