# Project Status

## Current Work

Owner admits M9 T100 S4 for a fresh ordered audit of all current mirror
differences. T100 remains open; S1-S3 stay closed. Owner explicitly approves all points reviewed in files 1-14 through fpu.c.
Unreviewed files and automatic small-diff delegation remain unapproved.

## M9 T100 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 好 准入新s 再次对比和审计所有镜像diff按照dir顺序来进行. Fresh audit authorized; owner subsequently approves all reviewed points in files 1-14 through fpu.c. No approval of unreviewed files. |
| Objective | Screen the frozen 472 files in literal dir /a /b /s order; exclude 254 identical and 97 exclusion-scaffolding-only files. List the 121 real-difference files first, then audit their differences and report purpose, evidence, size and decision status. |
| Non-goals | No excluded-body bookkeeping, new bug repairs, guest changes, sibling writes, unapproved semantic patches or claims that historical tests are fresh. Owner authorizes S4 P6 limiter removal and S4 P7 event-dispatch consolidation; retain all three dispatch sites and the interrupt-map accessor. P8 owner authorizes removing finite budgets and migrating tests to existing DEBUG trace/breakpoint plans. Owner accepts P8, authorizes deleting its unused executor-event declaration, and now authorizes P9 original-stack depth unification, safe TLS teardown, minimal regressions, dual publication and full tests. No snapshot schema change or duplicate depth owner. Owner now authorizes restoring the three original ntstubs empty functions with static linkage (P15); keep external original owners and verify dual artifact equivalence. |
| Reference Baseline | SoftPC 6ff24aee, final S3 production cda2d427; XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89. |
| Candidate Proposal | [S4 ordered re-audit](../proposals/m9-xp-mirror-original-first-audit.md#s4-fresh-ordered-re-audit). |
| Files And ABI Surface | All tracked files under src/app-softpc/softpc.new and read-only inspection of real build/caller/test dependencies. Start with audit, not new code changes. |
| Applicable Rules | docs/README.md reading set; EXECUTION, DOCUMENT, CODING, ARCHITECTURE and source-research-policy; XP only, never OpenNT. |
| Verification | Pin both identities; capture literal directory/file order and hashes; classify retained differences in ascending file/location order; corroborate each semantic point with current callers and supported configurations. Documentation-only P uses documentation, manifest and whitespace checks. Any later authorized source delivery uses cached dual builds/publication and proportional tests, with the established complete-artifact-equivalence exception. |
| Expected Markers | Fresh finite file ledger, sequential frontier, explicit unresolved/qualified/restored dispositions; category sums reconcile without treating Git hunks as semantic points. No approval inferred from old IDs or passing tests. |
| Asset Needs | Preserve all user configuration, media and snapshots. S4 owns build/t100-s4; two-hour job and fifteen-minute no-progress budgets; hidden background long runs; promote concise evidence and remove disposable work at delivery/closure. |
| Reporting Requirements | Per file: ordinal, XP/local anchors, purpose category, added/deleted diff, actual consumer, proof and limits, counter-review and decision. Report global progress and do not label unreviewed matching groups audited. |
| Stop Conditions | Unproved necessity remains investigation. No new unjustified mirror diff, no blanket approval, no uncertain original source substitution. |
| Exit Criteria | Every frozen file and nonexcluded difference has a fresh reconciled disposition and category totals; remaining evidence-backed decisions are presented for owner review, with no unfinished investigation hidden as retention. |
| Original Owner Request | 好 准入新s 再次对比和审计所有镜像diff按照dir顺序来进行. |
| Similar-Issue Sweep | Audit the actual current universe rather than only historical patch IDs; read coupled consumers without advancing later files' audit status. |

## Current Technical Baseline

S4 P15 implements the owner-selected static linkage for the three original
ntstubs.c empty functions. Selected XP diff decreases from +1/-12 to +4/-4:
five fewer changed lines. Source +12/-1 includes the corrected original line
anchor. Dual cached builds/publication pass; both ntstubs objects are byte
identical and full EXEs differ only in compiler time / PE metadata. Under the
approved artifact-equivalence exception no runtime suite is repeated. Existing
P9 full qualification remains historical. No S4/T100 closure or frontier advance.

S4 P14 records fpu.c approval and presents ntstubs.c (review 15 / order 182):
+1/-12, four points preventing duplicate original symbol definitions. Existing
Gdp and empty-function owners are verified; historical dual link-failure
controls are not fresh tests. Recommend retention, pending owner. No code
changes. 15/121 presented, 106 remain; ntthread.c is next.

S4 P13 records c_tlb.c approval and presents fpu.c (review 14 / order 121):
+8/-5, two points concerning representation access and FIST M64I rounding.
Each has an independent historical dual-architecture negative control; current
source/test review does not constitute a new test run. Recommend both for
retention, pending owner. 14/121 presented, 107 remain; ntstubs.c is next.

S4 P12 records owner approval of all c_page.c points and presents c_tlb.c
(review 13 / order 98): +58/-0 selected content, plus one diagnostic line.
All four IDs concern private TLB snapshot capture/restore and its declarations
and line mapping. Recommend retention; no fresh runtime test or code changes.
13/121 presented, 108 remain; next is fpu.c, order 121. Details and evidence
limits are in the current file-by-file decision ledger.

S4 P11 presents review 12 / order 88, c_page.c: +20/-6, 15 points, all
host DEBUG operand-observation integration. Recommend retention; owner
approval pending. Fresh source/caller review and historical negative-control
evidence are recorded separately in the decision ledger; no new runtime test.
12/121 files presented, 109 remain; next is c_tlb.c, order 98.

S4 P9 implements the owner-approved original simulation-depth authority.
Compat's second counter and c_main entry/leave hooks are removed; ntthread
provides a read-only query and safe TLS detachment. Snapshot format is unchanged.
Final cached dual builds/publication and full tests pass 392/392, plus focused
regressions and P8/P9 snapshot interoperability in both directions on both widths.
Production +18/-25 (net -7), tests +24/-5 (net +19); total XP mirror diff increases
11 lines for the accepted ownership simplification, including the independent
one-line declaration cleanup. Self-review corrected independent registration
cleanup on TLS failure; final qualification includes that correction. See
[S4 P9 evidence](../etc/evidence/softpc/m9-t100-s4-audit.md#p9-original-simulation-stack-depth-unification).
Owner accepts the reviewed c_main.c dispositions and requests the next file,
c_page.c (review 12 / order 88). All reviewed points in files 1-11 are approved;
S4/T100 remain open. Evidence limits and restored/deleted dispositions remain.

S4 P8 removes the owner-approved finite-budget implementation and parameter.
Production keeps continuous execution and existing DEBUG/lifecycle paths;
tests reuse actual trace and execution breakpoints without a second counter.
Final cached dual builds/publication and full regression pass 392/392, including
all native desktop/audio/package cases. Earlier focused fixture-address error
and correction are recorded in the [S4 evidence](../etc/evidence/softpc/m9-t100-s4-audit.md#p8-finite-budget-removal-and-debug-reuse).
Production +9/-45 (net -36), tests +106/-127 (net -21); fixed-XP mirror diff
reduces eight lines. Actual-diff review finds no new path/state or public-package
change. Owner accepts P8; S4/T100 remain open at review file 11.
The follow-up unused declaration cleanup is delivered with P9 above.

S4 P7 implements the owner-approved single Compat pending-event dispatcher.
All three CCPU call sites, clock-before-wake order and one-record consumption
remain; interrupt-map access is unchanged. Cached dual builds/publication pass.
Full regression plus corrected-fixture reruns passes 392/392; first-run fixture
setup failures are recorded, not claimed as first-pass success. Production
+15/-34, tests +53/-2; XP mirror diff decreases twelve lines. Actual-diff review
finds no new state, alternate path or scheduling change. Await owner acceptance;
S4 remains open at file 11. Details are in the S4 audit evidence.

S4 P6 owner authorizes removing the instruction-rate limiter. Scope: c_main.c
declaration/call, Compat platform limiter/state/idle compensation, existing
platform fixture assertion, mirror manifest and evidence. Device clock, HLT
wait, input wake and finite-budget contract stay unchanged. Cached dual builds,
publication and full background/native regression pass: 392/392 first run, no
retries/skips. Production +0/-85; tests +2/-6; XP mirror diff decreases three
lines. P6 is ready for owner acceptance; S4 remains open at file 11.

S4 P3 records actual purpose classifications for the first ten of 121 manual
file reviews: +133/-38, 41 text hunks. Only reset.c has explicit fresh retention
approval; next-file requests do not approve other points. The
[decision ledger](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-current-file-by-file-classifications)
is updated after each subsequent file review. No source or runtime change.
S4 P4 adds c_main.c classification: 11 files presented, +269/-54, 70 text
hunks cumulatively; 110 files remain. Pointer exposure, pacing and budget
precision are explicit discussion boundaries, not blanket approvals.
S4 P5 counter-review separates necessary capabilities from unproved minimal
forms: no unconditional equivalent restoration established; pacing remains a
product-policy decision and budget consumes the initial pre-fetch boundary.
No fresh ablation/runtime test or new source change is claimed.

S4 P2 adds per-file counts to the complete 121-file list: 15,778 zero-context
text diff blocks, +24,764/-24,069 selected-content lines relative to fixed XP.
Every frozen hash matches; excluded bodies and separately handled line controls
are not counted. Text blocks are not semantic-point approvals. This is read-only
statistical delivery; no new runtime verification or production change is claimed.

S4 P1 freezes 472 current mirror files in literal dir order (matching S1).
Owner narrows the workload: 254 identical files and 97 with only excluded-body
and line-control scaffolding differences are excluded from further audit.
The [121-file real-diff list](../etc/evidence/softpc/m9-t100-s4-real-diff-files.md)
retains original ordinals. All 121 have differences outside exclusion bodies;
this is not a bug count or retention approval. Initial selected-body inspection
of positions 1-9 identifies three unapproved points (+2/-4), recorded in the
[S4 evidence](../etc/evidence/softpc/m9-t100-s4-audit.md). No production, test or
artifact changes; remaining semantic review is not claimed complete.

S3 P16 delivers the owner-approved nonsemantic restoration: 37 mirror files,
176 text hunks, production +192/-207, no test changes. Selected text diff drops
277 lines; raw fixed-XP diff drops 243. Final cached x86/x64 builds publish fresh
EXEs; full-byte comparison permits only compiler time and PE metadata changes.
Under the existing owner exception, runtime tests are not repeated. Manifest
and documentation gates pass. [P16 evidence](../etc/evidence/softpc/m9-t100-s3-p16.md)
records exclusions, correction of a failed intermediate candidate, historical
ID relationships, and limits. S3 is owner-closed; semantic retention approvals
remain revoked, and the uncompleted full classification stays with open T100.

Owner rejects the limited P15 purpose summary and revokes all prior mirror
retention approvals, including automatic small-point delegation. The outstanding requirement is
a fresh all-point audit with category diff-block/line/point accounting. No source
is reverted by this instruction; previous approvals are historical only. The
[point-level ledger](../etc/evidence/softpc/m9-t100-owner-decisions.md#s3-approval-progress)
owns individual decisions. That prior documentation-only instruction is followed
by the narrowly authorized P16 restoration above, without new runtime claims.

TEST01 P6 implements the approved two-line RUN_SERIAL repair; 60/180-second
budgets and all coverage remain unchanged. Cached dual-width configuration
and full tests pass 392/392 without retries/skips, including native desktop,
audio and package tests. Registration comparison proves only the two serial
properties change; scheduling logs prove exclusive execution. P4 EXEs remain
byte-identical. [P6 evidence](../proposals/m9-xp-mirror-original-first-audit.md#test01-p6-delivery)
records timings, review and scope. Owner accepts S2; S3 is now owner-closed and T100 remains open.


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
are delivered in P4 with fresh full qualification. TEST01 P6 is accepted with S2 closure. Remaining difference review stays with open T100 after S3 closure.


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
completion review is recorded. S1-S3 are owner-closed; remaining existing-difference approvals await further T100 work. T100 remains open. See the
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

- [T100 S3 mirror review and text restoration](../history/M9-T100-S3-mirror-review.md).

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
