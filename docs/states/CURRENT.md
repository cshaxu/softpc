# Project Status

## Current Work

Owner closes T100 S1 and admits M9 T100 S2 to review eight outstanding
repair candidates. D005/D006 repairs and minimal tests are now owner-approved
and implemented in S2 P2. D004 is implemented and qualified in P4; owner
approves TEST01 two-line scheduling repair as P6 and full dual-width tests. T100 stays open. See the
[S1 closure](../history/M9-T100-S1-original-first-audit.md).

## M9 T100 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner closes S1 and admits S2 to go through the candidates; seven mirror items plus one test issue make eight, not nine. Owner subsequently approves D005/D006 minimal code repairs, minimal regressions, cached full dual-width build/publication/test and one P commit/push. Owner now approves D004 twelve-store scalar correction, minimal regression and cached full dual-width build/publication/test/commit/push as P4. P6 TEST01 adds RUN_SERIAL to the two existing test property assignments, preserves budgets/coverage and runs all tests before commit/push and owner acceptance. L001/D001-D003 stay declined. |
| Objective | Recheck D001-D006, L001 and TEST01, establish actual failures and consumers, and present minimal coherent repair or non-repair dispositions for owner review. |
| Non-goals | No unapproved new mirror patch, bulk-path activation, duplicate implementation, guest changes, sibling writes, excluded-body audit or blanket timeout increase. |
| Reference Baseline | S1 P189 4951c99a; final production P164 21e56aa077334ed5db04a3ab1d64e026bcc2656f; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [Eight-item review](../proposals/m9-xp-mirror-original-first-audit.md#s2-owner-admitted-candidate-review). |
| Files And ABI Surface | Candidate C-VID rules, timer.c, egwrtm12, PIC snapshot/Compat archive and their tests; shared Emulator/Lib test timing investigation and the two approved RUN_SERIAL property changes. Disclose any proposed public or archive contract change. |
| Applicable Rules | docs/README.md reading set; EXECUTION, DOCUMENT, CODING, ARCHITECTURE and source-research-policy. XP is the only original comparison baseline; no OpenNT source reads. |
| Verification | Existing failures are leads; new probes use actual production bodies/callers on x86/x64 with independent expected values. Source/ABI reasoning, controlled probes and real guest reproduction remain distinct. P2 requires focused pre-fix failure controls, corrected tests, full cached dual-width builds/publication, background and native regression. Preserve actual failures; no stale P164 result substitutes for P2 qualification. |
| Expected Markers | Eight-item finite ledger with actual path, evidence, minimal diff estimate, regression scope and owner decision; distinguish D004 applicability from confirmed defects. |
| Asset Needs | Preserve configuration/media/snapshots. S2 owns build/t100-s2 if needed; hidden background jobs, two-hour total and fifteen-minute no-progress budgets; remove disposable output after delivery. |
| Reporting Requirements | Explain cause, failure example, production reachability, proposed single-path fix and source/XP diff separately. Report uncertainty without presenting it as a repair. |
| Stop Conditions | D005/D006 P2 and D004 scalar stores P4 are authorized; TEST01 two-line scheduling repair P6 is authorized; no bulk-path activation or other candidate patch is authorized. Missing evidence is investigation, not justification for added state, fallback, guest-specific logic or dropping tests. |
| Exit Criteria | Every one of eight items has a sourced review disposition and executable plan or reason not to modify; later implementation requires its own authorized scope and qualification before closure. |
| Original Owner Request | Owner asks to close S1, admit S2 and go through the outstanding mirror/test candidates; original wording and corrected count are preserved in the proposal. |
| Similar-Issue Sweep | For each confirmed defect, inspect adjacent operations and actual caller families; do not silently add unrelated candidates or activate unused APIs. |

## Current Technical Baseline

TEST01 P6 implements the approved two-line RUN_SERIAL repair; 60/180-second
budgets and all coverage remain unchanged. Cached dual-width configuration
and full tests pass 392/392 without retries/skips, including native desktop,
audio and package tests. Registration comparison proves only the two serial
properties change; scheduling logs prove exclusive execution. P4 EXEs remain
byte-identical. [P6 evidence](../proposals/m9-xp-mirror-original-first-audit.md#test01-p6-delivery)
records timings, review and scope. Await owner acceptance; S2/T100 remain open.


T100 S2 P4 repairs CHAIN4 mode2 scalar plane-byte selection: production +12/-12,
tests +52/-0, no public/snapshot ABI change. Fixed-XP normalized diff totals for
the two mirror files remain unchanged (2308 added+deleted lines); the local
patch's 24 changed lines are not 24 additional XP diff lines. Fresh cached
x86/x64 builds publish both EXEs, focused tests and four separate pre-fix
negative controls succeed as expected, and full regression passes 392/392
without test retries or skips. Actual-diff review finds no blocker; evidence,
artifact hashes and boundaries are in the [D004 delivery](../etc/evidence/softpc/m9-t100-s2-d004.md#p4-owner-approved-implementation).


T100 S2 P2 implements D005/D006: positive active PIT backlog delay and correct
mode2 CHAIN2 plane2 enable bit. Production +3/-1, tests +35/-0; anchored XP
diff increases four lines. Both cached builds publish fresh dual EXEs. Same-test
negative controls reproduce both original faults on x86/x64; corrected focused
tests pass. Fresh full regression passes 392/392 (188 background plus eight
native per architecture), without retries or skips. See [repair evidence](../etc/evidence/softpc/m9-t100-s2-repairs.md).
Owner declines L001 and D001-D003 repairs this round: retain evidence and
known limits without new code or snapshot-format changes. D004 hardware/path
investigation confirms a scalar plane-byte selection defect on both widths;
[diagnostic evidence and candidate](../etc/evidence/softpc/m9-t100-s2-d004.md)
are delivered in P4 with fresh full qualification. TEST01 P6 is implemented and qualified, pending owner acceptance. S2 and T100 stay open.


T100 S1 P164 restores only ev_glue.c's XP include-line comment:
production +1/-1, tests unchanged, mirror diff-line count unchanged. Dual cached
builds/publication pass; complete EXE comparison shows metadata-only changes.
After the owner returned to the desktop, fresh full regression passed 392/392
with no retries or skips, including both native desktop/audio/package suites.
All 472 frozen files have review dispositions. P160 verified inventory/delivery
integrity; P187 verifies final identity and indexes positions 1-472 plus D001-D006/L001 in the
[owner decision view](../etc/evidence/softpc/m9-t100-owner-decisions.md).
381 reconciled residual IDs remain unapproved (375 semantic and six
format/compiler), with nineteen historical notice items separately identified.
P188's fixed-XP normalized Git comparison reduces added-plus-deleted diff by
7328 lines across 123 changed mirror files; this is distinct from source-line
net growth and anchored per-point counts. P189 provides the
[compact owner decision packet](../etc/evidence/softpc/m9-t100-owner-decisions.md#compact-owner-decision-packet):
existing functional/ABI differences, six compiler items, historical notices,
and unimplemented new repairs are separate choices. The audit-delivery
completion review is recorded. S1 is owner-closed; its remaining candidates are under S2 review. T100 remains open. See the
[audit evidence](../etc/evidence/softpc/m9-t100-progress.md#p189-compact-owner-decision-packet).


S43 implementation 51d0bc15 adds tools/Build.cmd: PowerShell-free x86/x64
build/publication via existing cached Ninja presets. Two dual-width runs and
four package integration tests pass. The owner requested reverting subsequent
local EXE changes; published files match the accepted 51d0bc15 delivery.


S42 final implementation 60bdbaa4 removes unused mouse_io.c, nop.c, vglob.c
and their obsolete wiring/state. Both published EXEs pass 392/392 tests.
Snapshots now use SCP3; older SCP2 files are rejected without compatibility.
The retained mirror has 472 files. User media/configuration are preserved;
the owner-provided assets/binary/snapshot update ships in S42 P6.


S41 delivery ecf263a8 completes the five admitted mirror cleanup/guard units:
production +16/-29, with XP mirror added+deleted diff reduced by 13 lines.
Snapshot command IDs and the existing device paths remain unchanged.
See [S41 closure](../history/M9-T99-S41-mirror-cleanup.md).

Td S23 delivery db76bb60 repairs the cached compiler timestamp, updates
execution rules and verifies embedded EXE literals. Both architectures pass
188 background plus eight native tests each: 392/392. Published EXEs carry
Nov 8 timestamps. See [timestamp closure](../history/M9-Td-S23-build-timestamp.md).
Only Emulator build configuration/verifier/manifest changed within the public
eight trees; no fresh NXVM alignment is claimed.

Original GenericByteFill, Unchained DWORD and Chain4 bulk-fill defects belong to the
[pending diff decisions](../etc/evidence/softpc/m9-t100-progress.md#pending-original-defect-diff-decisions), not TODO.
Shared-fixture aggregate timing moved from [TODO](TODO.md) to S2 TEST01. The runtime-performance candidate
remains in [Queue](QUEUE.md), unnumbered and unadmitted. JIT/WHP discussion
did not admit S44 or a new implementation task.

## Recent M9 Closures

- [S43 build entry](../history/M9-T99-S43-build-entry.md).

- [S42 unused mirror retirement](../history/M9-T99-S42-unused-mirror-retirement.md).

- [S41 mirror cleanup](../history/M9-T99-S41-mirror-cleanup.md).
- [S40 retained-difference audit](../history/M9-T99-S40-retained-difference-audit.md).

- [S39 mirror wrapper cleanup](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
- [S38 C-VID rules](../history/M9-T99-S38-cvid-rules.md).

- [S37 original-device audit](../history/M9-T99-S37-original-device-audit.md).
- [S36 CHAIN2 DWORD reads](../history/M9-T99-S36-chain2-dword-read.md).
| Task | Closure |
| --- | --- |
| T99 | [Completion audit](../history/M9-T99-completion-audit.md). |
| T98 | [Completion audit](../history/M9-T98-completion-audit.md). |

## Recent Governance

[Td S23 timestamp governance](../history/M9-Td-S23-build-timestamp.md) is closed.
Task scratch is removed; reusable Ninja/ccache roots remain. Follow existing
build-cache and task-owned probe cleanup rules.
