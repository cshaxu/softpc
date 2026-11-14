# Project Status

## Current Work

No implementation subtask is active. Open task awaiting owner: T100.
Owner accepts P16 and closes M9 T100 S3. No next S is admitted. See the
[S3 closure](../history/M9-T100-S3-mirror-review.md). Remaining whole-mirror
classification and retention decisions stay in the T100 proposal for later admission.

## Current Technical Baseline

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
