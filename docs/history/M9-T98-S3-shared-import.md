# M9 T98 S3 - Exact NXVM shared import

Owner admission: 准入一个s任务完成原样导入并构建双exe并跑全量测试.
Owner acceptance on 2026-10-19: 验收通过 批准收口s任务.

Baseline SoftPC f805483f. Executor delivery d08ac104 and coordinator review
2d5a67f6 are pushed. The [S3 ledger](../proposals/m9-nonpublic-quality-performance-audit.md#s3---owner-directed-exact-shared-import)
contains the frozen source identity, all five proof members and their limits.

All 298 files in the eight shared trees now byte-match NXVM revision
930075de1f406f9eddee258fd65714aa19343b36. The import changes six Lib paths:
an optional ISO C math-alias header, its documentation, a focused Types test,
its UNIX math linkage and the two manifests. Shared accounting is production
+12/-0, tests +10/-0, CMake +3/-0, documentation +5/-0 and manifests +4/-3:
+34/-3, net +31. Existing ABI and product/mirror behavior are unchanged. Root
test helpers were reviewed and intentionally not imported.

Both cached package builds freshly linked the package EXEs. Each architecture
executed all 194 registered cases exactly once: 119 shared unit/gates, 59 other
unit/gates, eight background integrations, five desktop and three native-audio
tests. Both XML unions equal the current CTest registration set, with zero
failures, errors or skips. Published package hashes are:

- softpc32.exe: `704D873D121307CDAB765454C4CAF388C6AD221208B5FE12722D016C566C3873`
- softpc64.exe: `78A32B672B1A2D49BF742C99EA6B2647FC364717B7BE4414CB73CFF54D0E4838`

The task-local x64 runner completed its tests before an unavailable
post-test hash command stopped reporting; that command was removed and only
the unexecuted x86 run resumed. Independent XML and package-object checks
completed successfully. Scratch was retired after the coordinator review;
`Clean-Build.ps1 -Check` retains only `.ccache` and `.ninja`. User-owned
`assets/binary/softpc.ini`, guest media and untracked `snapshot.debug` remain
untouched.

S3 is closed after owner acceptance. T98 remains open; S4/S5 are not admitted
by this closure.
