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

## Active Packet And Identifier State

Every active S is represented by exactly one `## M<milestone> T<task> S<step>
Packet` or `## M<milestone> Td S<step> Packet` section in `CURRENT.md`.  It
uses the fixed two-column `Field | Required record` table and has a non-empty
record for each of these fields:

`Identifier Mode`, `Admission And Approval`, `Objective`, `Non-goals`,
`Reference Baseline`, `Candidate Proposal`, `Files And ABI Surface`,
`Applicable Rules`, `Verification`, `Expected Markers`, `Asset Needs`,
`Reporting Requirements`, `Stop Conditions`, `Exit Criteria`, `Original Owner
Request`, and `Similar-Issue Sweep`.

The packet is the executable task contract. A material scope, acceptance, or
risk change requires its owner-approved revision before implementation
continues. An idle `CURRENT.md` states that no implementation subtask is
active and contains no packet.

`Identifier Mode` is one of:

- `New`: the next global numeric T, beginning with S1;
- `Continuation`: the next unused S of the latest open numeric T;
- `Corrective`: a narrow in-scope repair on the latest closed numeric T;
- `Owner-Reopen`: an owner-directed reopening with its next unused S; or
- `Governance`: the next milestone-local Td S and no numeric T allocation.

No mode reuses a T or S identifier. A corrective or owner-reopen must state
the observed failed contract and preserve the original task boundary. The
documentation gate verifies packet shape, state mode, Queue/proposal links,
and TODO form; passing it proves structure, not truthfulness or completion.

## Review, P Commits, And Similar-Issue Sweep

One session may perform both coordinator and executor roles, but it must
switch roles before acceptance. The executor completes the admitted brief,
self-reviews, commits, and pushes one complete P. The coordinator then reviews
the actual worktree/Git changes, original request, applicable rules, and
verification evidence before accepting or closing that S. A test report or
executor summary never substitutes for this actual-change review.

Every defect repair records a similar-issue sweep: defect class, searched
scope and commands, every production-path hit with a disposition, and the
post-fix focused proof. Mechanically detectable forms receive a permanent
static gate when practical. Hits outside scope are explicitly transferred to
`TODO.md` with priority and an admission condition; they are never silently
ignored.

## User Package Configuration

`assets/binary/softpc.ini` is user-owned package configuration. An agent
must not create, rewrite, copy over, normalize, or otherwise modify it. Builds
and packaging may refresh only `softpc32.exe` and `softpc64.exe` in that
directory. When the owner has changed the INI, an agent must stage, commit,
and push that change with the current completed task; only the owner may edit
its contents.

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
