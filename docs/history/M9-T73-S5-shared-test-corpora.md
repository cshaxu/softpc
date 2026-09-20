# M9 T73 S5: Independent Shared Test Corpora

Owner request: 测试通过 准入s5 构建test的三组件包。
Owner acceptance: 测试通过，收口S5. 然后验收T任务（如果没有其他S任务）。

Baseline fb07a0b7; implementation 879c30ac; actual-change review ad665a86.
S5 is closed. The [retained proposal](M9-T73-shared-x86-dependency-audit-proposal.md)
contains the finite ledger, assertion preservation, failures, costs and hashes.

test/lib is unchanged. test/common contains neutral mechanism tests and one
shared fake-machine fixture. test/x86 owns CLI/protocol/assembler tests and reuses
that fixture, without a second executor loop. Every suite has its own CMake entry
and manifest. Common test/source closure has no x86 dependency.

Isolated four-directory copies passed Lib 41/41 and Common 18/18; isolated
six-directory copies additionally passed x86 9/9. Copy hashes matched 384 files.
Both Release builds passed; final background x64 110/110 (181.72s), x86 110/110
(165.87s), with five desktop cases per width excluded. Native Common/x86 tests
each passed 50 repetitions on each width. Owner manual tests now pass as well.
Copied builds used the supported x64 Windows host, not a Linux/NEC runtime.

Production C/H +0/-0; test C/H +621/-519 (net +102); CMake +50/-18 (net +32).
Four C tests and one verifier are exact Git moves. Fixture synchronization makes
fault injection deterministic without weakening expected errors. Original CLI
test bodies retain their content except the fixture type name.

An immediate debug-close/paused-destroy race discovered in extra testing remains
in the separate, unadmitted [terminal-wake proposal](../proposals/m9-common-machine-shutdown-wake.md).
S5 retains the original stop-completion teardown and does not claim that race
fixed. This is an explicit follow-up, not a missing test-package migration.

Actual-change review preceded owner acceptance. Closure changes only documents;
accepted EXEs, source, tests, user INI and media remain untouched. No rebuild is
needed for this closure. T-level acceptance is recorded in the
[completion audit](M9-T73-completion-audit.md).
