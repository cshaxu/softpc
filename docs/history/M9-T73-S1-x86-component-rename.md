# M9 T73 S1: Explicit x86 Component Names

## Request And Boundary

Owner: S1负责debug和xasm32的改名，确保softpc依然能正确接入和跑通；
S2负责审计common/machine并切分后续的S迁移任务。准入S1并编译测试提交推送，
等我准入S2。

S1 was still open after preliminary audit f13c2780. Its scope was revised by
the owner, not assigned a reused closed identifier. This project's finalized
design is authoritative; NNES is only initial research. S2 is not admitted.
The [proposal](../proposals/m9-shared-x86-dependency-audit.md) retains that
preliminary evidence and the revised two-stage plan.

## Implementation And Ownership

- git mv common/debug -> common/x86-debug and common/xasm32 ->
  common/x86-xasm32; corresponding shared tests move with their owner.
- Public prefixes become common_x86_debug / COMMON_X86_DEBUG and
  common_x86_xasm32 / COMMON_X86_XASM32. Targets become common-x86-debug and
  common-x86-xasm32. Internal imported assembler names and debugger commands
  remain unchanged; no aliases or forwarding files are retained.
- App command and VM register vocabulary consume the renamed public headers.
  CMake, corpus DAG, product include allowlist and shared manifests follow the
  same owners. Negative probes reject both retired component include paths.
- Lib, Machine, Session, UI, Compat and MVDM implementations are unchanged.
  Machine's x86 payload is deliberately retained until S2's audit/migration
  plan; renaming does not claim ISA neutrality. Build selection also remains
  unchanged in S1, rather than implementing later neutral-only qualification.
- No CLI strings, instruction/register semantics, input path, lifecycle state,
  allocation, thread, snapshot format, guest media or INI behavior changes.

## Before And After Accounting

Before estimate: 25-35 code/build/test paths, production C/H +150..300 and
-150..300, tests +100..250 and -100..250, both net zero mechanical changes.

Actual, using `git diff --numstat -M10% f13c2780` and explicit renamed-path
pairs (exclude docs/manifests/EXEs from code counts):

| Group | Paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C/H | 13 | 168 | 168 | 0 |
| Test C/H | 6 | 181 | 181 | 0 |
| CMake/build/static-gate scripts | 6 | 26 | 24 | +2 |

The two extra lines reject old component includes. Git's default similarity
threshold may display heavily renamed public headers as delete/add; the lower
threshold and explicit pairs recover their true mechanical line changes.
There is no reduction/increase in runtime code structure or state.

## Finite Sweep And Verification

Frozen ledger: both component trees; App command and VM register consumers;
Common debug/xasm/machine tests and product command-provider tests; root/shared
CMake; corpus/product allowlists; two Common manifests; current architecture,
layout and README. Every member is renamed or explicitly unchanged above.
Historical task references and the original request preserve old spellings.

Legacy path/prefix search across src, test, tools, root CMake and current
design found only the two intentional negative-test strings. Changed C/H
comparison covered all nineteen surviving changed files: baseline contents,
with exactly the eight documented path/prefix substitutions, equal current
contents after host newline normalization. No executable logic was changed.
Git also confirms no diff in the unchanged implementation directories above.

Both Release builds completed using the tests-x64/tests-x86 build presets.
Background test-x64 passed 105/105 (168.90s); test-x86 passed 105/105 (156.37s).
This includes command-provider/command integration, x86_debug_output,
x86_debug_linear, Machine paused-debug execution, xasm32 and xasm32_contract,
manifest/DAG/negative probes and the restart-boot integration test.
Five desktop tests per width are excluded by policy; no interactive Windows
guest test is claimed for this rename. Documentation governance and diff
whitespace checks passed. Retired target archives, objects and old test EXEs
were removed only from the two validated build subtrees; all are rebuildable.

Package SHA-256:

- softpc32.exe: AE5A7CC16FDAE42219F5479B256679D53D4D682C48FB6552E02E494A2D7E5E78
- softpc64.exe: 6E1B6F1E56E2A8398D4BB9DFB69D1BDD747995A806021CBE68EA32452C8C4BB3

## Actual-Change Review And S1 Completion

Executor delivery 03a954dd was pushed before coordinator review. In the
coordinator role, the same agent reviewed f13c2780..03a954dd actual changed
paths, App/VM calls, Common build/DAG and test registrations, the original
owner request and the verification evidence above. Both manifests were
rechecked after push. No Lib, Machine/Session/UI implementation, Compat/MVDM,
INI or media changes occurred. No old production entry/compatibility alias
remains; the two intentional old includes are negative fixtures.

S1 meets its rename/integration/build/test/push boundary and is closed.
T73 remains open; S2's Machine audit and later migration breakdown require
owner admission. The package is ready for owner testing; this review does
not claim a new manual Windows guest acceptance.
