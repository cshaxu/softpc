# M9 Td S12: common extraction planning

## Scope and baseline

Owner requested governance-only planning, proposal/task decomposition, commit,
push and a clean worktree. The verbatim request and live candidate are in
[Common corpus convergence](M9-T56-common-convergence-proposal.md).
No numeric implementation T was allocated; T55 remains closed.

Accepted production reference is `987d82e`: existing x86/x64 CTest 58/58 each
and strict library CTest 8/8. This Td did not rebuild or retest executables.
It did not change production source, MVDM, library code or guest media.

## Executor delivery

`39978b6` (P1) updated the proposal and was pushed to origin/main. It also
restored the missing historical index records for already-used Td S10/S11,
without reopening or reusing their identifiers. The owner's existing tracked
INI changes (`display=console`, `console_control=1`) were committed unchanged;
the agent did not edit configuration bytes.

## Reviewer: requirement coverage

| Owner requirement | Planned boundary and acceptance |
| --- | --- |
| Preserve SoftPC experience; NXVM is a partition reference | S1 freezes behavior, threads and migration ledger; each later S preserves that baseline rather than replacing it with NXVM control/UI |
| Five common components | S2 UI, S3 session, S4 machine, S6 xasm32, S7 debug; original-source imports follow SoftPC extraction |
| Original CLI injected into session | S3 retains one product command provider, removes old reducer/dispatch ownership, proves rejected commands never enqueue machine work |
| App config and entity/thread assembly | App keeps configuration decisions and lifecycle composition; common manages its owned resources, lib retains internal workers; no new thread framework |
| Unified UI takeover | S2 common/ui owns monitor/raw/Window/broker; session decides actions, UI executes handoff; no extra common/console or common/kvm |
| Only common calls lib | S5 includes media, clock, sync, config-file I/O and trace consumers; S8 validates complete production dependency boundaries |
| No old second implementation | Per-S source/function/target deletion evidence against S1 finite ledger; remaining independent responsibilities have explicit later S ownership |
| Testable EXE at every S | S1-S8 each deliver both fixed package EXEs, hashes, full CTest and focused hand-test instructions; user acceptance precedes next S |
| Clean, committed and pushed delivery | Every implementation P and its review/closure records must ship; this Td follows the same commit/push discipline |

Review checked the committed proposal against the original request, dependency
directions, callback/lifetime ownership and each S deletion/acceptance boundary.
No production migration is claimed. Debug/xasm32 original-source verification
and actual supported machine-contract tests are explicit; an unchanged default
CLI or dormant link alone does not prove debugger capability. NXVM adoption is
external future work, never a local build dependency.

## Verification and disposition

- Documentation governance verifier and `git diff --check` passed for P1.
- Closure reruns these checks with Current idle and this history record present.
- Candidate stays first in Queue; the next numeric T requires owner admission.
- P2 records this review and closes Td S12. No new executable is needed for a
  planning-only task; executable deliveries belong to the proposed S stages.
