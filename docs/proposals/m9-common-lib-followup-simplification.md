# Common / Lib follow-up simplification

## Original owner request

> Can you please admit 4 S tasks for each of them? Follow the same format: before: audit and estimate change; after: audit and confirm changed diffs and give links to test; no need to wait, keep admitting the next S task until all 4 S tasks are done and then wait for me to test and decide on next step.

Owner accepted T62 S1-S4 testing. Continue the open T62 with S5-S8;
do not close T62 until the next owner decision. Baseline: 8e82e85.

## Finite scope and sequence

| S | Single ownership simplification | Preliminary production estimate | Proof |
| --- | --- | --- | --- |
| S5 | Console binding validity derives solely from nonzero generation; remove duplicate flag | -4 to -6 lines | Unbound, zero/stale generation, invalidation/rebind; existing callback barriers |
| S6 | Session pressed ledger stores copied input event directly; remove duplicate source field/wrapper | -8 to -15 lines | Source isolation, repeat identity, retirement while running/paused |
| S7 | Storage selected platform open accepts existing readwrite argument directly; delete two forwarding wrappers per platform | -15 to -25 lines | Both modes, failed opens, existing native sharing/locking tests |
| S8 | Session embeds its queue; initialize/dispose replaces separate container allocation | -10 to -25 lines | Partial initialization failure, FIFO growth, fault latches, lifecycle tests |

Each row is one admitted S and the complete convergence universe. Estimates
exclude tests/docs/manifests/artifacts and are revised from actual pre-audit.
Each step removes only the named duplication, preserving public contracts,
locks, generation barriers, allocation failure behavior and product semantics.
No App/VM/Compat/MVDM, debugger, renderer or configuration changes.

## Delivery contract

For each S: audit actual callers, report plan/estimate before editing; update
shared manifests; build both fixed EXEs; run focused and complete x86/x64 CTest;
commit/push complete implementation P1. Switch to coordinator role, inspect
the actual committed diff, run post-commit focused checks, record production
and test numstat and close in P2. Automatically admit the next row only after
the preceding exit criteria pass. Provide fixed EXE links after each step.
Preserve user INI and media. Build logs are bounded per-S ignored files under
build, removed after recording results. No runtime captures are needed.
Stop on a public API/product behavior change, missing proof, or unexpected
ownership expansion. Do not substitute code compression for simplification.

## Execution evidence

S5 pre-audit: binding_active has only four implementation references; it is
written together with generation under the same mutex. The generation check
already rejects zero and stale events. No public header exposes this flag.

S5 executor evidence: one production C path +1/-5 = -4; one test C path
+12/-1 = +11. Both preset builds passed, full x64 101/101 (94.79s), x86
101/101 (77.05s). Strict standalone Lib build passed. Generation transition
assertions added to the existing Console smoke; no new test target. All flag
references removed; public headers, locks and broker unchanged. INI preserved.
