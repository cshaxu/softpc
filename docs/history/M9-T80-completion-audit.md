# M9 T80: shared corpus boundary and simplification closure

## Request, acceptance and frozen coverage

Original request: audit the six Lib/Common/x86 source and test packages and
admit the seven reported repairs/simplifications. The retained
[proposal](M9-T80-shared-corpus-boundary-and-simplification-proposal.md) preserves
the original plan, estimates, owner-approved scope revisions and per-S proofs.
Final owner response: “验收通过！可以收口T了？” S7 and T80 are accepted and closed.
No next candidate is admitted by this closure.

Universe: the seven admitted findings, their actual changed production/test
paths, similar-issue dispositions and applicable regression. Unit: one S row
below, backed by its actual-commit review. Allowed dispositions: verified
implementation or explicit owner-accepted audit/no-change limitation.
Completion requires every row disposed, owner acceptance, no hidden scope
transfer, matching tested artifacts and successful documentation gates.
This is not a claim that the six corpora contain no other defects.

| S | Result and unique retained owner | Proof / disposition |
| --- | --- | --- |
| S1 | Existing xasm32 read/skip and assembly write boundaries reject before access; F1 uses existing undefined handler. No new decoder. | [S1](M9-T80-S1-xasm32-boundary.md): guard-page regression, output equivalence, strict isolated 10/10 and product 111/111 per width; owner closed. |
| S2 | Owner replaced speculative notification recovery with bounded reachability audit; existing single executor/queue retained. | [S2](M9-T80-S2-notification-audit.md): Common 18/18 per width and repeated focused tests; native-failure/context-lifetime limitation explicitly accepted, not repaired or silently deferred. |
| S3 | Session's existing physical-key ledger rejects an unrecordable new key before delivery. | [S3](M9-T80-S3-pressed-key-capacity.md): capacity/repeat/source/retirement regression, dual 111/111; owner closed. |
| S4 | Machine skips reserved zero; Session shares one private modular serial comparison. | [S4](M9-T80-S4-frame-sequence-wrap.md): wrap/stale/generation tests and dual 111/111; compared serials must be less than 2^31 apart; owner closed. |
| S5 | Existing Common verifier enforces Types header/limit boundary; seven equivalent macro substitutions. | [S5](M9-T80-S5-common-types-gate.md): negative/positive gate cases, copied four/six-package suites and dual 111/111; owner accepted continuation and now whole-task acceptance. |
| S6 | Immutable xasm32 dispatch tables replace per-call construction; parsing stays call-local. | [S6](M9-T80-S6-xasm32-dispatch.md): 512 mappings and remaining tokens equal, 24,576-case digest, isolated 10/10 and dual 111/111; owner closed. |
| S7 | One Storage write algorithm; public write/fill each owns its explicit flush boundary. | [S7](M9-T80-S7-storage-fill.md): old-code failing counter, short-write/flush faults, mode/range/data tests, isolated Lib 41/41 and dual 111/111; owner accepted. |

## Endpoint changed-path audit

Compared `b9413d65..71fb35c9` using `git diff --numstat -- src test`, excluding
MANIFEST.sha256 from code counts. Endpoint counts are not the sum of overlapping
per-S diffs:

| Category | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C/H | 9 | 582 | 544 | +38 |
| Existing Common checker | 1 | 4 | 4 | 0 |
| Tests/build C/CMake | 11 | 428 | 0 | +428 |
| Total code/checks/build | 21 | 1014 | 548 | +466 |

Production paths are Common machine.c and Session control.c, control_state.c/h,
session.c; Lib Storage medium.c and medium_interface.h; x86 xasm32 aasm32.c and
dasm32.c. The large textual portion is the 512-entry immutable-table relocation,
not new parsing logic. Public signatures, component ownership, Core mirror,
App, snapshots, INI and media are unchanged. No additional executor, event
channel, input ledger or mutable global parser was introduced.

Six manifests account for changed members/hashes. Two package EXEs and task
documentation are separate from code counts. Historical T79 archival/link
updates in this baseline interval are governance handoff, not T80 runtime work.
The closure itself moves the proposal, repairs references and records acceptance
only; production/tests/build/EXEs have no new changes.

## Final verification and preserved artifacts

Latest production endpoint has strict shared C11 dual Release build evidence:
background x64 111/111 (214.52s), x86 111/111 (194.39s); isolated Lib 41/41 on
each width. S5 independently verified identical copied four/six-directory
packages; S6 independently verified x86 10/10 on each width. Detailed commands,
timings, intermediate failures and dispositions remain in the linked records.
Five desktop cases per product width and three per isolated Lib width were
excluded; no Linux runtime or downstream product acceptance is claimed.
Existing original Core warnings are not recast as warning-free builds.

Closure rechecks the actual endpoint diff and these SHA256 hashes; it does not
rerun full tests or rebuild documentation-only changes. Final documentation
governance and diff-check must pass before this closure is committed.

| Accepted package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3693615 | F18C5628F4F906E081122094E254F369D7FBAC109C0E27BE69B8F8F33B368211 |
| softpc64.exe | 3080018 | D07BF9064A722BD28D29663F6A05FD778E39074004FAFDEE6078F9819D2C5EAD |

## Remaining work and closure decision

All seven scopes are disposed under owner-approved criteria. S2's disclosed
native-failure limitation is an accepted limit, not a guarantee of recovery.
S7 proves fewer explicit flush calls, not fewer physical I/Os or a speedup;
CRT seeks may still process buffers. No rollback/durability promise was added.

TODO remains empty under the owner's tracking policy. The sole Queue candidate
is the unrelated XP SP1 mirror rebase; it remains unadmitted. Task-specific
scratch cleanup is recorded in the S evidence; no task process remains.
The owner has accepted the final package, so no admitted T80 item awaits work.
T80 closes with the tested binaries unchanged and no active implementation task.
