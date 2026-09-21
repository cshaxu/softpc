# M9 T80 S5: Common Types boundary gate

## Request and scope

Owner: “批准收口S4，准入S5开始修复”. S4 is closed; S5 is delivered for owner
testing, not closed. Baseline `0be6a12c`; T80 remains open, S6/S7 unstarted.
The proposal records the pre-change brief, finite include/limit ledger and
similar-gate dispositions. Apply the existing architecture/coding/execution/
documentation authorities; no new verification framework or runtime policy.

Common's checker no longer permits twelve direct standard C headers, including
the redundant public-stdint special case. Its existing token scan also rejects
unwrapped integer-limit families. Seven UINT32_MAX uses in Machine become the
existing, exactly equivalent LIB_UINT32_MAX alias. Wait/timeout/error semantics
and public ABI are unchanged. Lib, Core and x86 production remain untouched.

## Change and verification evidence

Runtime machine.c +7/-7; checker verify_corpus.cmake +4/-4; test
verify_negative.cmake +24/-0. Total +35/-11, net +24, entirely test growth.
Expected runtime +7/-7, checker +4--8/-4--8, test +25--40/-0; reusing the
existing fixture makes tests one line below the lower estimate. Counts use
`git diff --numstat 0be6a12c` restricted to these paths; manifests/docs/EXEs
are separate. No duplicate checker or runtime state added.

New negative tests first failed on the original checker accepting assert.h.
After repair, 24 standard-header spelling cases, 16 integer-limit expression/
alias cases, a public-header case and a legal Types/comment/string/link case
pass along with existing tests. This is finite lexical enforcement, not a
complete C preprocessor. Shared tests themselves may use standard C headers.

Both package Release builds and background suites pass: x64 111/111 (273.67s),
x86 111/111 (178.46s). Five desktop tests per product width excluded.
Independent copied four-directory suites pass 59/59 per width; six-directory
suites pass 69/69 per width, excluding three desktop tests per suite. Strict
C11 warning flags verified. All 180/205 copied source/test files remain
identical; isolated task outputs removed after capture. Six manifests and DAG
checks pass; package hashes and detailed timings are in the proposal.
No native Linux or downstream integration claim. INI and media untouched.

## Actual-change review

Executor `c7c42ea5` is pushed. The same session switched to coordinator and
reviewed `git diff 0be6a12c..c7c42ea5`: one runtime source, one existing checker,
one existing test script, two manifests, five task documents and two package
EXEs, twelve paths total. The seven runtime edits are direct macro aliases,
not changed timeouts; source/build edge checks remain in the existing checker.
Negative and positive cases exercise the removed allowlist and raw-limit gate.
No unrelated source, public ABI, INI or media changed. Counts and package
hashes match the proposal, isolated outputs are removed, final documentation
governance passes. No remaining implementation gap found within the frozen
S5 boundary. At delivery, owner manual testing remained the S5 closure gate.
Subsequently the owner admitted S6: S5 is accepted for continuation, without
claiming a separately reported manual-test result. T80 stays open.
