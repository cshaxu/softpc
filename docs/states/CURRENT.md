# Project Status

## Current Work

Owner accepts and closes T100 S2 and admits M9 T100 S3 to review the remaining
existing mirror differences together. T100 remains open. See the
[S2 closure](../history/M9-T100-S2-candidate-review.md).

## M9 T100 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 批准收口s2 准入s3 一起来审批剩余diff清单. This admits review, not blanket retention or restoration. |
| Objective | Obtain explicit retain/restore decisions for the remaining S1 residuals, with actual consumers, evidence and restoration consequences. |
| Non-goals | No unapproved source change, new repair, excluded-body inventory, sibling writes, guest changes or reopening S2-declined repairs. |
| Reference Baseline | Accepted S2 P6 62474beb; S1 frozen 472-file order and 381 residual IDs; pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [S3 remaining-difference review](../proposals/m9-xp-mirror-original-first-audit.md#s3-owner-admitted-remaining-difference-review). |
| Files And ABI Surface | Existing softpc.new residuals and their real Compat/Machine callers, read-only until a concrete owner implementation decision; task records own approval status. |
| Applicable Rules | docs/README.md reading set; EXECUTION, DOCUMENT, CODING, ARCHITECTURE and source-research-policy. XP only; no OpenNT reads. |
| Verification | Cross-check current source and prior evidence for each presented point. Separate source/ABI proof, controlled tests and guest reproduction. Documentation-only admission uses documentation/whitespace checks, not invented fresh runtime qualification. |
| Expected Markers | 381 IDs retain individual dispositions; nineteen historical notices stay separate. First batch R0006/R0008/R0010, in frozen file order. |
| Asset Needs | Preserve configuration/media/snapshots. S3 owns build/t100-s3 for disposable review scripts; two-hour run/fifteen-minute no-progress budget if probes are needed; clean at each delivery. |
| Reporting Requirements | Original/current code, necessity evidence and limits, restoration consequence and actual XP diff; distinguish recommendation from approval. |
| Stop Conditions | Admission does not authorize arbitrary restoration or new patches. Unproved necessity returns to investigation. Do not revive declined bulk repairs or approve a whole family from one representative. |
| Exit Criteria | Every in-scope residual has an explicit owner disposition, with any approved implementation separately qualified and delivered; owner accepts closure. |
| Original Owner Request | 批准收口s2 准入s3 一起来审批剩余diff清单. |
| Similar-Issue Sweep | Check related declarations/callers when presenting a point, without silently widening implementation scope or duplicating the existing residual ledger. |

## Current Technical Baseline

TEST01 P6 implements the approved two-line RUN_SERIAL repair; 60/180-second
budgets and all coverage remain unchanged. Cached dual-width configuration
and full tests pass 392/392 without retries/skips, including native desktop,
audio and package tests. Registration comparison proves only the two serial
properties change; scheduling logs prove exclusive execution. P4 EXEs remain
byte-identical. [P6 evidence](../proposals/m9-xp-mirror-original-first-audit.md#test01-p6-delivery)
records timings, review and scope. Owner accepts S2; S3 is active and T100 remains open.


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
are delivered in P4 with fresh full qualification. TEST01 P6 is accepted with S2 closure. S3 reviews remaining existing differences; T100 stays open.


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
completion review is recorded. S1 and S2 are owner-closed; remaining existing-difference approvals are under S3. T100 remains open. See the
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
Shared-fixture aggregate timing was resolved by accepted S2 TEST01. The runtime-performance candidate
remains in [Queue](QUEUE.md), unnumbered and unadmitted. JIT/WHP discussion
did not admit S44 or a new implementation task.

## Recent M9 Closures

- [S2 candidate review](../history/M9-T100-S2-candidate-review.md).

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
