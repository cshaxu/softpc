# Project Status

## Current Work

Owner accepts S14 and admits T99 S15 to repair V7 pointer trails. T99 remains
open; Queue retains the unnumbered runtime-performance candidate.

S15 final implementation and dual-width qualification are recorded in the
[proposal](../proposals/m9-public-handoff-simplification.md#s15-admission-v7-pointerbody-handoff).
Both architectures pass all 193 registered cases with exact inventory match;
the earlier x64 package stage-5 failure and proof limits remain documented.
Both EXEs were refreshed and manually accepted. Owner then approves a further
minimal repair for the audited monochrome palette-update pointer defect.
Follow-up builds refresh both EXEs; each architecture passes all 193 tests
using its background run and connected-session desktop/native runs. The earlier
disconnected package failures are retained in the proposal. Executor 099e2104
is pushed and actual-commit reviewed; S15 awaits owner acceptance. T99 stays open.

## M9 T99 S15 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves the V7 pointer repair, minimal regression, dual builds, release, tests and push; then approves a minimal unified monochrome palette-update follow-up. |
| Objective | Preserve current body pixels when the original V7 hardware pointer is cleared, redrawn or moved across display updates. |
| Non-goals | No public component, guest configuration, media or snapshot format changes; no separate renderer or polling. |
| Reference Baseline | S14 accepted at 844c7e17. Read-only comparison found stale saved background can overwrite the newly painted body. |
| Candidate Proposal | [Public handoff simplification review](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Existing Compat DIB transaction and pointer callbacks, existing VGA fixture, task records and dual EXEs; preserve original video owner. |
| Applicable Rules | Execution, architecture, coding, documentation and source policies. |
| Verification | Focused actual-path pixel and transaction tests; dual cached builds and complete registered background, desktop and native-audio regression. |
| Expected Markers | Clear before body painting; redraw after body and before publication, retaining nested transactions and mode changes. |
| Asset Needs | Publish both assets/binary EXEs; preserve user INI, guest media and snapshots. No Computer Use. |
| Reporting Requirements | Record actual source/test counts, mirror diff, tests, artifacts and review. Scratch build/t99-s15; 30-minute job budget, five-minute no-progress, 300 MB cap. |
| Stop Conditions | Report if correct repair requires a second path, unapproved external mutation, or unsafe lifecycle change. |
| Exit Criteria | Correct minimal repair, focused and full dual-width qualification, two published EXEs, actual diff review, commit and push; wait for owner acceptance. |
| Original Owner Request | 好，请你准入一个新的S完成此项修复，并补上最小必要测试覆盖此情况，老规矩完成后编译发布测试提交推送等我验收. |
| Similar-Issue Sweep | Examine body refresh, partial and split paths, nested updates, pointer register changes, palette, 1-bit conversion, resize and teardown. |

## Technical Baseline

Latest published binaries are S15's palette follow-up build; their hashes and
full 193-case-per-width qualification are in the
[implementation record](../proposals/m9-public-handoff-simplification.md#s15-admission-v7-pointerbody-handoff).
The following S13 and earlier paragraphs retain their delivery-time evidence.

S13 executor 350dcd95 imports fixed NXVM ad5c532a. All 296 committed paths,
blobs and modes match exactly. Production is unchanged; test C -7 lines and
one manifest hash changes. Cached dual builds rebuild only the affected test;
each width passes 193/193 with exact inventory equality and no failures/skips/
retries. Both existing EXEs remain up-to-date and pass package integration.
Actual-commit review finds no delivery blocker, source-boundary expansion or
unqualified native-capture claim. Scratch is retired, gates pass, and the
[closure](../history/M9-T99-S13-public-import.md) records delegated completion.
This is S13 closure, not T99 closure or a fresh exhaustive source audit.

S12 is closed under the owner's bounded no-code disposition; see its
[closure](../history/M9-T99-S12-ninja-integrity.md). Current incremental behavior
is measured correct, not proof of a historical corruption repair. Research
5d44c2cf and actual review 0108e47e retain the six-member ledger and limitations.
The owner additionally approves the external snapshot.midi -> snapshot-midi
rename; both names have the same Git blob, with no payload change. EXEs,
INI, production, tests and build scripts remain unchanged. Scratch is retired.
S11 is owner-accepted and closed; see its
[closure](../history/M9-T99-S11-sb-midi-framing.md). The following records retain
historical qualification, not a new full regression for S12 closure.

S11 source and actual-port research confirms the framing defect and its UART
siblings. On each width the baseline misclassifies checked state/IRQ for 15
of 256 payload values after 38h, enters busy for 90h/91h and collides after
all four 34h..37h entries. The private bounded-unsupported candidate has zero
checked effects/busy/IRQ in the same sweep, preserves reset and pending archive
states, and passes four UART byte sweeps. The unchanged registered SB command
test passes but contains the incorrect MIDI oracle. The proposal owns the
finite ledger, primary hardware reference, private evidence and minimum design.
Recommended production scope is two mirror files +24/-2 (net +22), preserving
unsupported SB-MIDI while fixing command/data framing; the supported MPU path
does not change. Those research results preceded implementation approval and
made no shipping repair or full-suite claim.
Research df70efc0 is pushed. Coordinator actual-diff review confirms its
three-document-only scope, honest capability/verification limits and preserved
S10 artifacts. Scratch is retired; mirror, documentation, whitespace and
residue checks passed for that research. Owner now approves the displayed
minimum; production +24/-2 and test +67/-2 are implemented. Focused actual-source
tests pass 2/2 per width. Final cached builds publish both EXEs; each width
passes all 193 registrations (185 background, five desktop, three native audio),
exact executed-name equality and zero failures/skips. No non-mirror compiler
warning remains. Mirror, documentation and whitespace gates pass. Public eight
trees, root build/registration and user INI/media/snapshots are unchanged. The
proposal owns corrected historical mirror accounting, hashes, initial test-draft
compile failure, quality review and proof limits. S11 awaits owner acceptance;
S12 is not admitted. Scratch is retired after evidence promotion.
Executor 478c680f is pushed. Coordinator review of the actual eleven committed
paths confirms the approved parser-only scope, permanent port/IRQ/archive
regression, unchanged public trees and original reset/MPU ownership. Published
EXE blobs equal the committed artifacts; residue checks pass. No delivery
blocker is found. S11 remains verified awaiting owner acceptance, not closed.

S9 is accepted and closed; see its [closure](../history/M9-T99-S9-public-import.md).
Its verified baseline remains: all 296 public paths equal the pinned NXVM
commit; both cached builds refresh the EXEs and each architecture passes all
193 tests. That historical S9 qualification is not S10 implementation proof.

S10 research confirms two original-path defects through real CRTC writes and
Driver frame production: disable-only writes can be discarded, and shape
recomputation can re-enable a saved disabled cursor. Private x64/x86 probes
each show six baseline violations in nine checkpoints and 9/9 passes with the
two-site candidate. Existing VGA unit passes unchanged on both widths, exposing
its bit-5 coverage gap. Proposed production diff is +5/-12 in vga_prts.c only;
shared eight trees need no change. The proposal owns the complete evidence,
hardware references, test design and adjacent shape-semantic findings. That
research changed no tracked source/test or EXE and ran no full qualification.
Research 2b20b2ae is pushed; coordinator actual-diff review confirms the
three-document-only scope and honest proof/coverage boundaries. Task scratch
was retired and documentation, mirror, whitespace and residue gates passed.

Owner now approves implementation. The two-site production repair is +5/-12;
the existing VGA fixture adds 70 lines for real register/readback, font heights,
V7 protection, mode reset and both Driver text leaves. Focused tests pass on
x64/x86. Final cached full builds refresh both EXEs; each width passes 193/193
(185 background, five desktop, three native audio), exact name equality and
zero failed/skipped/disabled cases. Mirror, documentation and whitespace gates
pass. Public eight trees, Compat/Driver and user INI/media/snapshots are unchanged.
The proposal records test-draft correction, complete mirror accounting, hashes,
legacy-only warnings and quality review. Executor ad9d5fcf is pushed; coordinator
actual-commit review confirms the nine-path scope, unchanged original shape/
protection/dirty route, permanent register/frame regression and correct mirror
identity/accounting. Published EXE blobs equal the committed artifacts; scratch
is retired and residue checks pass. Owner now accepts and closes S10; see its
[closure](../history/M9-T99-S10-vga-cursor-disable.md). S11 research is active;
the S10 results above remain historical qualification, not new S11 proof.

### S8 reviewed delivery (closed by owner-authorized advancement)

S8 replaces two Compat linear calls with original physical SAS and corrects
the approved phyR range condition, with one marker and updated XP accounting.
Production +7/-6, test C +111/-1, root registration +2/-0; all eight shared
trees are unchanged. Final-source x64/x86 each pass 193/193 (185 background,
five desktop, three native audio), exact registration/execution equality and
zero failures/skips. Both EXEs are refreshed; user INI and guest assets are
unchanged. Original-header warnings are retained; no non-mirror warning is
reported. The proposal owns complete evidence, hashes and validation limits.
Executor 349057b1 is pushed. Coordinator actual-diff review confirms the
three-statement production scope, six regression groups, original SAS/device
ownership and unchanged shared/configuration/media paths. Published and
committed EXEs match; documentation, 505-body mirror, whitespace and build
residue gates pass. Scratch is retired and the worktree is clean.

S6 is closed by owner-authorized advancement; see its
[closure](../history/M9-T99-S6-debug-fixtures.md). S7 is accepted and closed; see its
[closure](../history/M9-T99-S7-app-test-contracts.md). S8 closes by advancement;
see its [record](../history/M9-T99-S8-physical-memory.md).
The following S7 and S6 delivery
record is historical evidence, not current S7 qualification.

S7 implements the approved bounded proposal: retain checkpoint's budget under
the owner's no-change disposition, consolidate timer oracles, observe initialized
Program Manager/Run readiness and read actual viewport rows. Actual test C is
+151/-114 (net +37), four App files only. Fresh final-source R3 qualification
passes 192/192 per width (184 background, five desktop, three native audio),
exact inventory equality and zero failures/skips. Both full cached builds have
no compiler warnings. Public eight trees, production/mirror, root CMake, media
and INI are unchanged; shipping EXE identities remain S6's. The proposal owns
counterfactuals, failed test-draft dispositions and verification limits. S7
is pushed as executor 13e32442. Coordinator review of its actual six paths
confirms the four-file test scope, retained mode/IRQ assertions, exact counters
and absence of production/public/asset changes. Documentation, mirror,
whitespace and build-residue checks pass; scratch is retired and the worktree
is clean. The owner accepts S7 and admits S8; T99 remains open.

S6's final four-case ledger and research are in the proposal. The existing local
debug hook clears quick_mode before refetch; fixtures own a non-HLT program and
restore ejected boot media before reset/restart. Production +4/-0, tests +20/-0.
No public component, BIOS/BOP, user configuration or media changes. Both EXEs
are rebuilt/published. Fresh full qualification passes 192/192 per architecture;
the focused final group passes 120 executions, and 24 additional desktop package
executions pass. Temporary probes are removed and their test targets rebuilt.
Native tracing is retired, scratch is cleaned, and validation limits are
preserved in the research history. The owner's latest decision removes the
native observation from active/deferred work without asserting a Windows fix.
Executor 8566eb42 is pushed. Coordinator review of its actual eight changed
paths confirms the three-file code scope, retained assertions and single CPU
path, normalized mirror hash and matching committed/published EXEs. Final
documentation, mirror, whitespace and residue checks pass; the workspace is
clean. The owner now closes S6 and admits S7; T99 stays open.

S5 is closed by owner-authorized advancement to S6; see its
[closure](../history/M9-T99-S5-public-quality.md). The following S5 records
are historical qualification, not active status.

Owner additionally admits S5 P6 at eac62924: move the unused public Product
opaque typedef into its implementation and remove the now-private source-ID
allocator's impossible null-argument check. No lifecycle or runtime contract
change. Verification for this narrow P is cached dual build/publication,
related component/composition tests and shared boundary/manifest checks;
prior full-suite evidence is retained, not rerun or claimed fresh. Scratch
build/t99-s5 remains owned by S5 with the packet's existing budgets.
P6 is implemented and verified: source +2/-5, test +3/-3; both EXEs rebuilt
and published, 22/22 focused tests per width pass without build warnings.
The IBMPC entry stub now owns its dummy type; no public alias is restored.
No full-suite rerun is claimed. S5 remains awaiting acceptance.
Executor P6 8ae39951 is pushed. Coordinator reviewed the actual eleven-path
commit: only the two admitted cleanups and the necessary entry-fixture naming
adaptation are present; assertions and lifecycle barriers are unchanged.
Manifests, artifact hashes and focused evidence agree; scratch is retired.

S4 is owner-accepted and closed; see [closure](../history/M9-T99-S4-console-retirement.md).
S5 follow-up is implemented and verified against 6a9bf601, awaiting acceptance.
All five audit items are complete. Production C/H +19/-43 (net -24), test C/H
+149/-155 (net -6), test CMake +2/-4. Cached dual builds publish both EXEs;
focused tests pass 14/14 each and full runs pass 192/192 each, exact registered
name equality and no failures/skips/retries. No non-mirror compiler warnings.
The proposal records neutral fixture synchronization, preserved coverage and
platform verification limits. S6 remains planned, not active.
Executor 29cf8a95 is pushed. Coordinator actual-commit review confirms the
22-path scope, unchanged public run validation/ownership barriers and retained
concurrency assertions. Documentation, whitespace and residue checks pass;
scratch is retired and S5 awaits owner acceptance.
The following qualification records the preceding P2/P3, not this follow-up.
Former S5's five private
research cases transfer intact to planned S6; S6 is not active. T99 remains open.
Against 164a5f70, production C/H is +72/-78 (net -6), test C +88/-49
(net +39), test CMake/fixture +20/-5. Cached dual builds publish both EXEs;
focused tests pass 12/12 each and full qualification passes 192/192 each
(184 background, three native audio, five desktop/package), exact registered
name equality, zero failures/skips. No non-mirror compiler warnings. Contracts,
failure evidence and self-review are recorded in the proposal. INI/media,
App/mirror and X86 trees are unchanged. No S6 work is admitted.
Executor 55a08f0c is pushed. Coordinator reviewed the actual 31-path commit,
ownership changes, failure fixtures and exact test inventory: no delivery
blocker found. Documentation, whitespace and build-residue checks pass; task
scratch is removed after jobs exit. S5 awaits owner acceptance, not closure.

### Historical S4 verification and research

Executor 52d8fcc3 is pushed. Coordinator review of the actual eight changed
paths confirms the approved two-site repair, synchronized fixture ownership,
matching manifest/artifact hashes and complete qualification. No new state,
API, polling or mirror change is present. Task scratch is removed; documentation,
whitespace and build-residue checks pass. S4 awaits owner acceptance; T99 stays
open and S5 is not admitted.

S3 is owner-closed at 8072abdc; see [closure](../history/M9-T99-S3-pause-contract.md).
S4's approved minimal repair is implemented and verified, awaiting owner
acceptance, not closed. Dual cached builds publish both EXEs without compiler
warnings. Exact registered-name verification proves 192/192 per architecture:
184 background, three audio and five desktop/package, zero failures/skips.
Corrected focused Console groups also pass 7/7 per width. Production C is
+6/-13 (net -7); test C +31/-11 (net +20); no public API/state change.
The proposal records self-review, evidence and the initial zero-test selection
mistake. S5 remains unadmitted. The following is earlier research evidence,
not the status of the now-implemented repair.
The owner's latest split is recorded in the proposal's S4/S5 section: shared
Console cancellation/input-loss repair planning remains S4; private HLT/TF/BOP,
pre-main native startup and App fixture investigations transfer to planned S5.
S5 is not admitted. New x86 captures prove null BOP dispatch after invalid
guest execution; the first invalid reset/boot transition remains unresolved.
Native hidden-start initialization passed 20/20 but its complete fixture only
16/20: no full repair claim follows. No production or shipping artifact changed.
Temporary command-provider diagnostics are removed; its normal cached test
executables rebuild successfully for both architectures.
The shared-reader audit also reproduces old long-line tail delivery after
retirement. A private same-path candidate removes native cancellation and
moves the stop check after the existing through-LF read loop; real native
broker probes pass 400 cycles per architecture. Controlled wake-failure and
completed-line tests also pass both widths. A private x64 product containing
only the candidate broker change passes 40/40 actual raw compact-package runs.
This is bounded counterfactual evidence, not a production repair, fresh full
regression or proof that unrelated native startup failure is fixed. All owned
jobs have exited; the proposal owns detailed evidence and the minimal repair
plan. S4 remains open for that plan; S5 retains its explicit unresolved items.
Research scratch was retired; implementation scratch is retired after evidence
promotion and confirmation that all owned verification jobs have exited.
The following S3/S2 records are historical evidence.
T99 S1 research is now owner-closed; its report and actual-commit review are
retained in the [closure](../history/M9-T99-S1-handoff-research.md) and proposal.
S2 is closed by owner-approved advancement; see [closure](../history/M9-T99-S2-handoff-cleanup.md).
S3 is implemented and verified, awaiting owner acceptance after delivery and
committed-diff review. Cached dual builds and focused 14/14 tests per width pass;
full qualification passes all 192 registrations per width (184 background,
three audio and five desktop/package), exact XML coverage, zero skips/failures.
Native tests ran after the owner's RDP reconnection. Dual EXEs are published;
production C is +1/-3 and test C +5/-0. No new pause state or reconciliation
path exists. The proposal owns the full evidence and similar-issue review.
Executor c4a624ee is pushed. Coordinator actual-commit review confirms the
two-predicate scope, unchanged FIFO/ledger ownership, matching regression
assertions and complete verification record. Manifest, documentation,
whitespace and residue checks pass; task scratch is removed after jobs exit.
S3 awaits owner acceptance; T99 stays open and S4 remains unadmitted.
The following is historical S2 evidence.
Owner-requested fresh full regression at 27946581 has now executed all 192
registrations per width: each passes 187, fails five, skips zero. Background
184/184 and Lib desktop 3/3 pass; two package and three native-audio cases per
width fail. Session changed from Active at launch to Disc when inspected.
All test names match registration; no prior passes substitute for these failures.
Owner now requests repair and full passing verification. The package fixture
now stops waiting when its child exits and captures exit status and final
Console content. Both widths build without warnings; the disconnected failure
now reports exit 1 / cannot compose machine in under one second, rather than
a misleading 30-second missing-prompt timeout. A direct native probe independently
reports no default PCM endpoint (0x80070490) and WinMM MIDI open error 1.
No production audio behavior is changed. After another interrupted full run,
the owner's final connected retry passes all eight native cases per width:
three audio and five desktop/package. Combined with the latest background
184/184 per width, exact registration/XML equality proves passing coverage
for all 192 per width, zero failures/skips in the final evidence set.
This is supplemented qualification, not an all-green initial aggregate or a
repair claim for historical loader/input intermittency. The follow-up changes
only the App package fixture (+11/-3) and task records. No production source,
EXE, public manifest, INI or media changes. S2 awaits acceptance after delivery
and actual-commit review; S3/S4 remain unadmitted.
Executor P4 e6090b97 is pushed. Coordinator actual-commit review confirms the
fixture-only change, preserved assertions/ownership, complete 192-case evidence
per width and explicit disconnected-failure history. Documentation and residue
checks pass; task scratch is retired. S2 is verified awaiting owner acceptance.
The following records the earlier bounded delivery, not this fresh run:
S2 implementation and cached dual builds are complete, no compiler warnings.
Background regression passes 184/184 per width; focused final evidence is 4/4
per width, retaining the initial x86 command wait failure. Native Lib cases pass
3/3 per width. Connected package x86 passes 2/2; x64 ordinary passes, compact
first fails stage 1/error 6 then passes unchanged in isolation. Earlier Disc
stage 3 failures remain disclosed. Passing evidence covers all 189 applicable
registrations per width, excluding three unrelated native-audio cases. This
is supplemented qualification, not an all-green first run or an intermittency
repair. EXEs are refreshed; INI/media remain unchanged. All jobs have exited;
scratch is retired for delivery. S2 awaits owner acceptance after commit/review.
Executor delivery a3fa9062 is pushed. Coordinator reviewed the actual committed
code, test, manifest, package and documentation diff: both approved cleanups
are complete, first-error/ownership order is retained, and no pause semantics
or recovery path changed. Checks pass and the workspace is clean. S2 awaits
owner acceptance; S3/S4 remain unadmitted.
The following records the completed S1 research:
The proposal records all five dispositions and a bounded source/caller review.
Existing cached x64/x86 focused tests pass 7/7 each; no full-suite, desktop,
guest stuck-key reproduction or intermittent-failure repair is claimed.
Production, permanent tests, configuration and EXEs are unchanged. Recommended
next work is small owner-local cleanup and discriminating fixture evidence,
not a new shared recovery model. Pause-release semantics require a decision.
Executor research 29c260b5 is pushed. Coordinator actual-commit review confirms
only the proposal and CURRENT changed, all five observations remain accounted
for, and no unproved repair/full-regression claim was introduced. Documentation,
diff-whitespace and build-residue checks passed. Only S2 is admitted above;
T99 remains open.
The owner transfers the five related observations out of TODO into T99 S1;
the proposal retains their evidence and is their sole investigation record.
QUEUE has no matching candidate; its product-performance task remains separate.
S15 is owner-closed; see [closure](../history/M9-T98-S15-pause-input-handoff.md).
The owner subsequently authorized deletion of untracked snapshot.debug;
it was removed without changing tracked media or configuration. T98 is now closed;
see the [completion audit](../history/M9-T98-completion-audit.md).

S14 is closed under the owner's authorization to complete self-verification
and close after reconnection; see [closure](../history/M9-T98-S14-kvm-lifetime-failure.md).

S13 remains [closed](../history/M9-T98-S13-directional-sizing.md). T98's four
performance candidates are transferred to a separate unnumbered
[queued proposal](../proposals/m9-runtime-performance-investigation.md);
that performance candidate remains unnumbered and unadmitted.

T98 S3 and S4 remain closed in [S3 history](../history/M9-T98-S3-shared-import.md) and [S4 history](../history/M9-T98-S4-public-api-consolidation.md). Owner now explicitly closes Td S22; its [closure](../history/M9-Td-S22-build-cache-governance.md) preserves delivery/review 7758a9c4/f805483f and verification limits.

## Current Technical Baseline

- T99 S8 delivery 349057b1 is the current verified runtime: physical adapter routing and the
  original range-check correction pass cached dual build/publication and
  193/193 final-source tests per architecture. Public eight trees retain their
  prior identities. The proposal records new EXE identities and unchanged INI.
  S8 awaits owner acceptance. S6's debug-refetch correction is preserved;
  its owner-closed evidence is in [history](../history/M9-T99-S6-debug-fixtures.md).

- T99 S2 delivery a3fa9062: Broker fail-closed cleanup is consolidated and the
  App completion fixture no longer resets an already auto-reset event. Cached
  x86/x64 builds refresh both EXEs. Applicable 189 tests per width have passing
  evidence with disclosed supplements; three unrelated native-audio tests per
  width were not rerun. Production C +4/-18, tests +27/-15; combined net -2.
  Manifests and dependency boundaries remain valid. S2 is not a repair of the
  historical command/package intermittency; see the proposal for all failures.

- Final T98 accepted runtime: edb91df1, reviewed in 453edcfa, closure through
  8165664a. S15 provides passing evidence for all 192 tests per architecture,
  with the disclosed unchanged x64 compact-package supplement. T99 S1 has
  made no production changes; published EXEs remain T98. The entries below
  retain historical per-S qualification, not current active-task status.

- S14 executor 0d9f1803 is pushed and coordinator actual-commit review passes;
  S14 is closed, T98 stays open. Production C/H
  +64/-66 (net -2); test C/H +225/-49 (net +176); test CMake +26/-2.
  Cached x64/x86 builds and final focused tests pass. Final production has
  184/184 background passes on each width. The final test-only held-payload
  assertion supplement passes, followed by all 177 unit/gate registrations
  per width passing while RDP is Disc. After owner reconnection, both widths
  pass five desktop and three native-audio cases, including >5-second worker
  retirement and both package routes. Exact registration/XML equality proves
  192/192 per width with zero failures/errors/skips in the final evidence set.
  Earlier disconnected failures and Active compact stage 1/error 6 remain
  disclosed in the proposal; no causal repair of existing TODO is claimed.
  All owned jobs have exited; scratch is retired after evidence promotion.
  User INI/media/snapshot.debug are unchanged; no App/mirror/X86 changes.

- S13 executor 1e07dcce is pushed and actual-diff review is complete. Private
  snap classification selects fit/width/height in the existing native size
  correction; manual eight-edge behavior is restored. Production C/H +76/-10
  (net +66), test C +108/-4 (net +104). Both EXEs have been rebuilt with cache.
  x86 and final x64 background each pass 184/184, exact registration/XML name
  equality and zero failure/error/skip nodes. Documentation and diff checks pass.
  RDP Active integration rerun executes all 15 per architecture: x86 first-run
  15/15; x64 14/15 plus unchanged isolated compact package pass. The initial
  stage 1/error 6 failure remains existing TODO, not claimed repaired.
  The owner reports manual verification. Actual shell side-snap ordering is
  not independently automated: SendInput still returns zero/error 5 while
  Active. The probe verifies native maximize/restore ordering only; controlled
  and native geometry tests remain distinct evidence. Executor delivery and
  actual-diff review are complete; user INI/snapshot are unchanged.
  Exact registration/XML comparison yields passing evidence for all 192 tests
  on each width, without skips; x64 includes the disclosed isolated supplement.
  Build scratch is retired and residue/documentation checks pass.

- S11 is owner-closed; [closure](../history/M9-T98-S11-window-scale-capture.md).
  Owner diagnostic proves non-edge WM_SIZING/9 is incorrectly promoted to
  terminal failure. S12 handles that native handoff and width-driven sizing.
  Temporary instrumentation is removed before qualification; no extra ratio,
  lifecycle, polling or rendering path is introduced. Final x64/x86 each pass
  all 192 registrations (184 background, five desktop, three native audio),
  exact XML/registration equality and zero skips. Both cached builds are clean.
  Production +18/-21 (net -3), tests +41/-2 (net +39). Initial new fixture errors
  and disconnected native failures are retained in the proposal, not hidden.
  INI/media/snapshot.debug are preserved. Executor commit/review precede owner
  acceptance; S12 is now owner-closed in [history](../history/M9-T98-S12-native-sizing-handoff.md).
  S13 owns the revised nine-case sizing policy. Executor fc2fa687 is pushed and its actual-diff
  review passes; task scratch is retired. Published softpc32 SHA-256:
  9F113DFE380E62F12705BE69A2102A49205B02AEFD9A53956D623660DF527B02;
  softpc64: 71E95D1D90A2828604C1529012488759790076468D8A239422471ADF27E37253.

- S10 is owner-accepted and closed; [closure](../history/M9-T98-S10-utf8-paths.md).
  S11 implements stable Window scale and actual clipped capture bounds in the
  existing Win32 paths. Production +40/-3 (net +37), tests +123/-3 (net +120).
  No public ABI, App, mirror or frame-exchange change. Focused controlled and
  native tests pass on both architectures. Cached dual builds and all 192
  registered tests per architecture have passing evidence with zero skips.
  x64 passes first run; x86 compact package first failed stage 1/error 6,
  then the unchanged desktop group passes 5/5. No repair of that existing
  intermittency is claimed. Executor 1aab6116 is pushed; coordinator actual-diff
  review is complete. S11 is owner-closed; T98 remains open with S12 active.
  softpc32 SHA-256 D1AE94255C19A53217F0E0580308EC79D4C42B4642DB2822225106B9996610FC;
  softpc64: 1696924D7125B779F9C0EB0E1524F658D46715E07637B257BC56810FE357F0A5.
  The reported occasional exit has not been reproduced or attributed; its
  precise remaining investigation is recorded in TODO, not claimed fixed.

- Owner closes S9 and admits S10; [S9 closure](../history/M9-T98-S9-execution-budget.md).
  S10 audit finds COM/LPT still using locale fopen; other owned file entries
  already use Storage. Executor 0151689d is pushed; actual-diff coordinator
  review is complete. Owner-admitted warning/ABI follow-up is implemented and
  verified: bf01eead is pushed and its actual-diff coordinator review passes.
  S10 is now owner-closed; T98 stays open with S11 active.
  Follow-up changes no production code: test +9/-8 and build +2/-0. All 242
  non-mirror C objects per architecture were rebuilt with cache; no non-mirror
  warnings remain. Mirror header warnings are retained by owner instruction.
  Both architectures pass all 192 registrations without test retries this run.
  Current softpc32 SHA-256:
  78FE670609EDAB63B5B6532920C54544D217941C02004CF7F4EABFEDD828F888;
  softpc64: 0A52927A7B91AEE1295928C62E505100BBC97A55A13D55FE40F1C97A1F0F314A.
  Original delivery production +55/-23
  (net +32), tests +136/-29 (net +107); mirror unchanged. Cached dual builds,
  focused 12/12 each and all 192 registrations per architecture have passing
  evidence. x86 compact package first failed stage 1/error 6; the unchanged
  full desktop repeat passes 5/5. This is not a repair of that known debt.
  Native audio passes 3/3 each after reconnection. See the proposal for exact
  qualification, package hashes and pre-existing warning disclosure.

- T98 S9 executor dbff43df is pushed and actual-change review is complete;
  owner has accepted and closed S9. Budget storage and interface are lib_u64 on both widths:
  zero is continuous, every nonzero value finite. Existing active state and
  original safe-point counting remain; no public component or mirror change.
  Production +14/-20 (net -6); tests +62/-4 (net +58). Cached dual builds and
  focused tests pass. Each full regression has 191/192 first-pass successes;
  the sole compact package attach failure passes in an unchanged isolated run,
  yielding passing evidence for all 192 registrations with no skips. This is
  not an all-green aggregate or a repair of the existing package intermittency.
  Original header warnings remain. User INI/media/snapshot.debug are unchanged.
  softpc32 SHA-256 D10C1108705F623CBA05BE1C9EA130585C76FC30067854AC63F2346D1988F76B;
  softpc64 SHA-256 00FCAD94A0EB8D3CB8FB9A6E3C1F3279025B71F68963F9722D876BA07D5BAF25.
  S10's unified UTF-8 file-entry work is now admitted above.

- T98 S8 implementation and cached dual builds are verified. Final x64 and x86
  each pass all 192 registrations: 184 background, five desktop and three
  native-audio tests, with exact registration/XML equality and no skips.
  Initial package readiness alone uses a measured 30-second fixture budget;
  product timeouts and subsequent test waits are unchanged. Separate historical
  initialization exits remain disclosed in TODO, not claimed repaired.
  Production +431/-190 (net +241); tests +767/-564 (net +203). Only Lib and
  Emulator src/test public trees change; App integration and one narrow mirror
  host-stride repair (+12/-8) complete the path. Executor f2677d01 is pushed;
  coordinator actual-change review is complete with no delivery blocker.
  Owner authorizes advancement to S9; [S8 closure](../history/M9-T98-S8-dynamic-frame-storage.md)
  records this transition without claiming an additional owner-run test.
  softpc32 SHA-256 E9E574CE63573AB1C8346470528BA70908552D663FC174654813A6B9A3E42BAD;
  softpc64 SHA-256 5012E3AD96F289752AF6681C64A4B4C048F23F25DAF613F68024A4BAC35A88C2.
  User INI/media and pre-existing untracked snapshot.debug are preserved.
- T98 S7 executor ed9bb7b8 is pushed and actual-change review is complete;
  owner accepted it; see the linked closure and proposal verification record.
  Production +16/-7 (net +9); tests +55/-3 (net +52). Only src/lib and test/lib
  of the public eight trees change; App and mirror production are unchanged.
  x86 final 192/192 passes; x64 final 190/192 plus two unchanged isolated passes
  provides passing coverage for all 192 registrations. Intermittent failures
  remain disclosed and deferred, not repaired by this sizing change.
  S9 is now closed; S10 is active.
  softpc32 SHA-256 BEAE1647F8F177123045F99AACE6A6195212A5F89F611C47F57EA18C9A346C5B;
  softpc64 SHA-256 9314E59A4B8143F230E0A2D1833C388BD8A214F72DF0513114149B97DCED9CEB.

- T98 S6 is owner-accepted and closed; see its linked closure record.
  Executor delivery is `8e66d797`; actual-change coordinator review is recorded
  in the S6 proposal. Task scratch is retired; build residue check passes.
  Exact Window underline/two-span cursor and documented native Console
  approximations use original video state through the existing frame path.
  Production +153/-114 (net +39), tests +261/-116 (net +145); the only mirror
  change is vga_prts.c +4/-0 for underline-location refresh. Cached x86/x64
  builds and applicable 184 background plus five desktop registrations have
  passing evidence per architecture, including targeted supplements. Package
  attach/prompt failures also occurred; their intermittent cause remains open,
  not proved fixed by later passes. See the S6 proposal evidence and TODO.
  Three unrelated native-audio integrations were not rerun.
- S6 published softpc32 SHA-256
  EA6786D56FC58D8BD873BAFC5E93C7D51F11BD9D12AF583585488373FD53C824;
  softpc64 SHA-256
  7D137F481626F4904AD33F634328E006D2C9E6662D4BACBD8AA34FF6EFBA6568.
  User INI/media and untracked snapshot.debug remain untouched.

- T98 S4 is owner-accepted and closed: FM's no-consumer placeholder leaf is
  removed; the staged Product assembly functions are implementation-private;
  and frame exchange no longer exports manually acquired locks. Machine caches
  publication metadata for Session, while bounded observers cannot retain the
  exchange pointer. Production +55/-81 (net -26), tests +94/-91 (net +3), and
  docs/build +45/-17 (net +28), excluding rebuilt packages. Both architectures
  built successfully. Each non-desktop/non-native-audio suite passed 184/185
  normally; the sole `checks/build_cleanup/selftest` requires a Junction and
  passed separately under its required elevated host permission, yielding all
  185 qualified tests per architecture. Build residue check passed.
- Pending S4 packages: softpc32 SHA-256
  EDE3E5A8F89A5064E1B93D347BCCF76CDFC58FC2E936B6880358C54019022174;
  softpc64 SHA-256
  2AF3E37332AD9DC982A6AAD6D41B1E488868DD9F4716296DAC63D2A65FEC80CE.
- T98 S3 optional Types math import: public source +12/-0, test C +10/-0,
  CMake +3/-0, docs +5/-0 and manifests +4/-3; total +34/-3 (net +31).
  No existing ABI or product/mirror implementation changes. All 194 tests
  pass per architecture with exact XML/registration equality and zero skips.
- S3 packages: softpc32 SHA-256
  704D873D121307CDAB765454C4CAF388C6AD221208B5FE12722D016C566C3873;
  softpc64 SHA-256
  78A32B672B1A2D49BF742C99EA6B2647FC364717B7BE4414CB73CFF54D0E4838.
- T98 S5 is owner-accepted and closed: Console and Window now use
  terminal wake plus actual worker join before disposal; the former 5000ms
  public destroy deadline is removed. Focused x64/x86 lifetime regressions,
  including a synchronized worker hold beyond five seconds, pass. Complete
  non-desktop suites pass except the privilege-required Junction selftest,
  which passes separately elevated. Package smoke fails with `cannot compose
  machine` before monitor startup; the particular initialization cause is not
  independently established by that message.
- T98 S2 fixes Compat media close reporting and address bounds only. Production
  +30/-16 (net +14); tests +195/-0; root test registration +3/-2. Public and
  mirror code unchanged. Historical qualification: build/t98/s2/qualification,
  exit 0; raw logs retired by Td S22 after preserving the checkpoint;
  x64/x86 each 177 unit/gates, eight background integrations, five desktop and
  three native audio, exact XML/registration match. Existing Ninja recovery
  and original compiler warnings remain; no warning-removal claim is made.
- Historical accepted S2 packages: softpc32 SHA-256
  4E67DBD9AAEBE99BB874BA77E0BFA7DBE7819DE5927D920BE4998769089231AC;
  softpc64 SHA-256
  5C692E0DE7ADFDF9D1AEF2446D90C7C4B733FD3ECA589B94CBA33D2E89D216A8.
- T97 is investigation/documentation only; production, tests and source mirror
  have no changes. Installed Win95 driver rejects mixer/volume capabilities for
  DSP 2.01. The observed hardware-change confirmation explicitly requests
  shutdown; normal Restart succeeds twice. The owner's original installation
  dialog was not recorded, so its exact wording/action remains unverified.
- T96 runtime/reference: 5ea4e55e; S8 delivery a404740f and review 5ea4e55e.
  All 191 tests per architecture passed with zero failures/skips: 175 unit/gates,
  eight background integrations, five desktop and three native audio.
  Evidence: build/t96/s8/qualification, qualification.exit=0; exact CTest/XML
  registration equality was checked. Existing compiler/Ninja warnings remain.
- Previously accepted T96 package reference, without a T97 rebuild:
  softpc32 SHA-256 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A;
  softpc64 SHA-256 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
- Current transport: RGB graphics, separate leaf text contracts, Machine-owned
  fixed four-slot exchanges shared through opaque Base mechanics; Driver owns
  readiness and Machine publishes every produced frame. Session/UI route
  references. See current architecture and T96 audit for accounting.
- Win3.1 verified MIDI configuration is MPU330/IRQ7, SB220/IRQ5, all sixteen
  Mapper channels to MPU. [Product UI](../design/UI.md#windows-31-midi-playback)
  owns instructions. No FM or SB-MIDI capability was added.
- Existing [TODO](TODO.md) items remain open. No unrelated debt is repaired or
  removed by this investigation closure; current candidates are in [Queue](QUEUE.md).

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T98 | Owner closes T98 on 2026-10-22; unimplemented S16 transfers to T99 S1. | [Audit](../history/M9-T98-completion-audit.md) |
| T97 | Owner accepted S1 and T97, 2026-10-18. | [Audit](../history/M9-T97-completion-audit.md) |
| T96 | Owner accepted S8 and T96, 2026-10-17. | [Audit](../history/M9-T96-completion-audit.md) |
| T95 | Delegated closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |

## Recent Governance

- M9 Td S21 approved bounded frame ownership; see the
  [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 completed that implementation; no rule change accompanies T97 closure.
