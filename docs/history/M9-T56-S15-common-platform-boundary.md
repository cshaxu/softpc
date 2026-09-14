# M9 T56 S15 — Common platform boundary

Owner requested platform-independent Common and minimal Lib support. Delivered
in `864a74f`, actual-diff review in `4cf8629`, both pushed. Common now uses Host
mutex/event/task/wait and Types atomics, with no OS implementation branches.
Independent manifest/build/DAG proof was restored. x86/x64 68/68, strict Lib
8/8 and isolated Common 4/4 passed. Native Linux runtime was not available.
S16 admission supersedes the test placement with test/common and test/lib;
S15's user GUI acceptance is not claimed. T56 and its S13 audit remain open.
