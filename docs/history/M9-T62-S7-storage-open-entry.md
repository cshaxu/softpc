# M9 T62 S7: one storage open entry

Request: [follow-up proposal](M9-T62-common-lib-followup-simplification-proposal.md).
Baseline 96fb039; implementation 666aa67 pushed.

Coordinator actual-commit review confirms native open/lock/error bodies are
unchanged. Only two constant-argument wrappers per platform, their declarations
and the medium conditional dispatch disappear. Private file.h declares the
same selected-platform entry on both systems. Public file/medium ABI unchanged.
rg of src/test finds only owned-byte and medium production callers; no old
readonly/readwrite forwarding entry remains. No new compatibility wrapper.

Five production C/H paths +6/-25 = -19; two test C paths +11/-6 = +5,
git diff --numstat 96fb039 666aa67, excluding manifests/docs/EXEs.
Both builds/full suites passed: x64 101/101 (84.69s), x86 101/101 (73.70s).
Strict Lib build passed. Post-commit storage/manifest/DAG checks 7/7 each.
Linux controlled tests verify selected lock modes and cleanup, not Linux
product execution. Fixed EXEs rebuilt; INI/media unchanged. S7 closes;
S8 proceeds automatically, then wait for owner with T62 still open.
