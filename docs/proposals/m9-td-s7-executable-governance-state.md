# M9 Td S7 — Executable governance state

## Objective

Raise SoftPC's task-governance state to the NXVM baseline: an active packet,
identifier allocation, Queue/proposal/TODO relationships, and closure status
must be machine-checkable rather than only described in prose.

## Scope

- Define the fixed active-packet record in the local execution authority.
- Define `New`, `Continuation`, `Corrective`, `Owner-Reopen`, and `Governance`
  identifier modes and their narrow allocation rules.
- Upgrade the CMake documentation-governance gate to validate active/idle
  state, one packet, required non-empty packet fields, ordered Queue entries,
  Queue-to-proposal and proposal-to-Queue links, and TODO priority shape.
- Add deterministic verifier self-tests for accepted and rejected fixtures.
- Register that self-test in CTest without changing a production target.
- Preserve and structurally archive every non-queued, non-active historical
  proposal under `docs/history/`; repair direct historical links after the
  move. This is an identity migration, not content deletion.

## Non-goals

- No production, lib, MVDM, package, artifact, CI, or build-preset change.
- No source/license policy; that is the next separately admitted governance
  task.
- No retroactive rewriting of historical task records beyond the compact
  current-state form required by the new verifier.

## Historical-proposal disposition

The baseline has 46 proposals that are neither the one queued XP candidate nor
the active S7 proposal. Some carry explicit historic task identifiers and some
are earlier design/candidate material without a one-to-one T record. All are
retained as history; none is deleted or treated as lacking value merely because
it is no longer a current candidate.

## Exit criteria

- The governing rules and `CURRENT.md` use one fixed, validated packet schema.
- The verifier rejects each malformed state covered by the scope and accepts
  the repository's valid active/idle state through self-tests.
- Documentation governance checks pass; the governance task is committed and
  pushed with a clean worktree.
