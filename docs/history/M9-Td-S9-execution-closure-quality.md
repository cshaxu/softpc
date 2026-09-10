# M9 Td S9 — Execution Closure Quality

## Objective

Raise SoftPC's future-task governance to the relevant NXVM execution standard:
explicit T-level closure audit, bounded coverage ledgers for whole-domain
claims, complete implementation-P discipline, code-path cleanup accounting,
and safe build-tree hygiene.

## Scope

- Strengthen the local execution rule without importing NXVM product policy.
- Add only mechanically checkable state-gate requirements to the existing
  documentation verifier.
- Keep the existing one-packet, queue, TODO, and direct-source boundaries.

## Non-goals

- No product, MVDM, lib ABI, package, or CI behavior change.
- No fictional external-ROM integration gate: a runnable task names its real
  focused and full regression evidence in its packet.
- No duplicate authority in `CURRENT.md`, history, or supporting evidence.

## Exit criteria

- The strengthened rules distinguish S acceptance from T closure, require an
  actual-diff coordinator review, and make coverage claims ledger-backed.
- Future runnable tasks have an explicit clean-build/output cleanup rule and
  source/test-path accounting rule.
- The documentation gate rejects a structurally incomplete closure state that
  it can reliably detect, and its self-test proves the rejection.

## Completion evidence

### Adopted from the NXVM comparison

- T-level closure audit separate from S acceptance; complete implementation P
  followed by coordinator actual-diff review.
- Finite convergence ledger for whole-domain claims only.
- Changed source/test-path and net-line accounting with obsolete-path
  disposition.
- Safe build-tree and raw-recording hygiene.
- A mechanical state check that the highest recorded numeric task still has a
  compact closure row in `CURRENT.md`, with a rejecting self-test fixture.

### Already covered or intentionally not imported

- SoftPC already has the one active packet, identifier modes, queue/TODO
  separation, source-policy boundary, pushed P rule, similar-issue sweep, and
  proportional x86/x64 runnable proof requirements.
- NXVM's product artifact naming/revision scheme is not imported: SoftPC has
  its fixed `softpc32.exe`/`softpc64.exe` package contract and separate T
  identity rule.
- No mandatory external-media integration gate is invented. A runnable task
  must name its actual evidence; unavailable protected inputs cannot become a
  fictional closure criterion.

### Verification

- `cmake -DSOFTPC_SOURCE_DIR=. -P tools/Verify-DocumentationGovernance.cmake`
  passed.
- The self-test passed including the new stale-highest-task rejection fixture.
