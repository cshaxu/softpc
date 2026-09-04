# Execution Rules

Apply the shared [execution-governance skill](https://github.com/cshaxu/skills/blob/main/execution-governance/SKILL.md).
This document is the SoftPC execution authority.

## Lifecycle And Authority

Only the owner admits, reorders, suspends, or closes implementation work.
Admission creates one active subtask in `states/CURRENT.md` with the original
request, objective, non-goals, baseline, affected boundaries, applicable rules,
focused verification, full regression, similar-issue sweep, stop conditions,
and exit criteria. There is one active subtask at a time.

```text
accepted -> planned -> active -> implemented -> verified -> closed
                                  |              |
                                  +-> blocked    +-> deferred
```

A build or one smoke does not close a task. Runnable-path work needs x64 and
x86 build/test evidence proportional to the change. Artifact media remains
non-mutating unless its active packet explicitly admits disposable output.

## User Package Configuration

`artifacts/binary/softpc.ini` is user-owned package configuration. An agent
must not create, rewrite, copy over, normalize, stage, commit, or otherwise
modify it. Builds and packaging may refresh only `softpc32.exe` and
`softpc64.exe` in that directory. Only the owner may modify the INI.

## Identifier And Build-Version Policy

`T<n>` is one repository-wide, strictly increasing implementation-task number;
it never resets with `M<n>`. `T<n>` is that task's program build version.
Milestones scope roadmap work only.

`Td` is a standalone documentation/governance identifier and never consumes a
`T` number. Use `M<milestone> Td S<subtask> P<part>: summary` for governance
commits and `M<milestone> T<task> S<subtask> P<part>: summary` for
implementation commits.

Historical milestone-local task labels remain immutable facts. The auditable
historical count is M1=2, M2=1, M3=2, M5=1, M6=1, M7=5: twelve tasks total.
The completed staged test-boundary task is M8 T13; future queued M8 tasks use
T14 and above. No future task may reuse an earlier T number.

Only an admitted Td may edit `docs/rules/`. Before any closure, run:

```powershell
cmake -DSOFTPC_SOURCE_DIR=. -P tools/Verify-DocumentationGovernance.cmake
```

## Change Discipline

Use `git mv` for structural relocation, repair every direct reference, then
run the applicable verification. Discoveries outside the active boundary become
a Queue proposal or TODO debt; do not silently fold them into the task. A
completed change is committed and pushed before it is reported closed.
