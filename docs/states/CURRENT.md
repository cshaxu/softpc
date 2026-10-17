# Project Status

## Current Work

T93 S13 is verified: minimal platform-neutral PCM progress and lifecycle.
Cached x64/x86 build and publication completed; each width passed 167/167,
including all ten native desktop cases, zero skips. Production C/H +19/-30
(net -11); tests +36/-20 (net +16). No API/state/helper was added.
See the [S13 delivery](../proposals/m9-xp-sound-blaster-standalone.md#s13-implementation-and-qualification).
Executor 9960b37f is pushed and its separate coordinator actual-change review
passes. Await owner acceptance; do not close S13 or admit S14 automatically.
S12 is owner-accepted and closed after executor 2fc25069 and review f43ea903:
167/167 per architecture including ten desktop cases. Its intermittent raw-input
failure remains explicit TODO, not a claimed repair.

## M9 T93 S13 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts S12 and admits S13 through delivery with before/after briefs and minimum total code. |
| Objective | Neutral PCM progress and batch-independent destruction, retaining one worker/FIFO/backend. |
| Non-goals | No new API/state/worker, recovery framework, MIDI/FM, capture, guest/mirror or INI/media/snapshot changes. |
| Reference Baseline | Accepted S12 f43ea903; S12 closure is documentation only. |
| Candidate Proposal | [S13 PCM](../proposals/m9-xp-sound-blaster-standalone.md#s13--pcm-progress-and-lifecycle-contract). |
| Files And ABI Surface | Lib PCM interface/common/Win32 backend and two existing tests/manifests; inspect real Compat callers. |
| Applicable Rules | Task Reading Set and four governance skills; single-agent dual-role, Types authority, inward dependencies. |
| Verification | Blocked short/full destruction, flush, epochs/progress, native format limit; cached dual-width build, shared then other tests, ten native desktop cases and both EXEs. |
| Expected Markers | Destroy cancels/releases, flush submits, clear discards; no batch-dependent lifecycle or public native padding/byte-rate contract. |
| Asset Needs | Hidden build/t93/s13 jobs/logs/exit files; build 1800s, suite 1200s, desktop 600s observation budgets, 30 MiB logs; executor cleanup; no Computer Use. |
| Reporting Requirements | Before/after brief, actual source/test additions/removals/net and changed-file totals, evidence/hashes and separate review. |
| Stop Conditions | Expanded scope or failed proof; no unapproved behavior change, no interactive-timeout termination. |
| Exit Criteria | Build/publish/test/commit/push and actual-change review; await owner acceptance, no automatic S14. |
| Original Owner Request | 很好。验收通过，准入S13，开始前和结束后都要进行简报汇报；请你以极简方案实施，意思不是说diff一定要减少，而是完成后总代码数量要最少 |
| Similar-Issue Sweep | PCM destroy/flush/clear/set_active consumers, public native terminology, format guards and lifecycle tests. |

Brief: remove destroy's sub-batch flush branch. Preserve cancellation, join,
ownership checks and existing deactivation flush-then-clear behavior. Move
Windows byte-rate representation limit to its existing backend guard.
No new state/helper. Estimate production +15-25/-30-45 (net -10 to -25);
tests net +40-80. Preserve necessary overflow and live-worker safety checks.

The owner accepts and closes T92 on 2026-10-15 and admits queue head as T93.
The owner accepts the reopened S7 delivery and admits S8: "验收通过，准入s8".
S7 is closed; executor 7bb98d79 and review e1e44d0c are pushed. Both widths
passed 161/161 including all ten native desktop tests, with both EXEs refreshed.
See the [accepted import ledger](../proposals/m9-xp-sound-blaster-standalone.md#s7-approved-import-continuation)
and [S7 history](../history/M9-T93-S7-shared-contract-quality.md).
S8 is closed under delegated sequential acceptance after executor 980984d0
and coordinator actual-change review. Both architectures pass 165/165,
including all ten native desktop tests, with refreshed EXEs and unchanged
public corpora/user configuration. The checkpoint timeout remains explicit
test-reliability debt, not a claimed repair. See the
[S8 closure](../history/M9-T93-S8-xp-midi-fm-attachment.md).
The owner overrides the prior sequential admission: S8 stays closed; the
unimplemented archive packet initially moved to S10 and must not proceed now.
The owner now approves splitting the neutrality work into multiple S tasks.
The owner accepts S10 and admits S11 through closure: "验收通过，准入执行S11到收口".
S10 and S11 are closed. S11 executor 33a9c9f7 is pushed and its separate
coordinator actual-change review passes under the owner's through-closure
authorization. T93 remains open; the subsequent S12 admission is recorded above.
See the [S11 closure](../history/M9-T93-S11-utf8-paths.md). Final
x64/x86 each pass 166/166 with zero skips, including all ten native desktop
cases; both EXEs are refreshed. S11 changes only Lib source/test contracts
and governance: production C/H +199/-45 (net +154), tests +160/-39 (net +121).
User INI, media, snapshots and original mirror remain unchanged. S14 and later
remain unadmitted. The accepted S10 executor
e7d37b65 is committed and pushed; the separate coordinator actual-change
review and subsequent owner acceptance close S10. Its final x64/x86 qualification passes 165/165 each, zero skips,
including all ten native desktop cases; both EXEs are refreshed. Production
C/H is +542/-534 (net +8), test C/H +268/-181 (net +87), with zero mirror,
INI, media or snapshot changes. See the
[S10 delivery](../proposals/m9-xp-sound-blaster-standalone.md#s10-executor-qualification).
The owner also authorizes necessary SoftPC App consumer
adaptation, superseding the earlier no-App-edit limit for this input migration.
Planned S10-S14 cover input, paths, interface text, PCM lifecycle and capture
respectively; the archive task moves to S15. These are planned, not concurrent
implementation admissions. See the proposal for scope and acceptance budgets.
The owner now requires every S delivery to build, test, commit and push, then
wait for explicit acceptance. Prior delegated sequential admission is revoked:
do not admit the next S automatically. S9 audit/design and dual-width
qualification are pushed in executor 896aa2fd and actual-change reviewed;
S9 is owner-accepted. Each width passed 165/165
with zero skips, including five native
desktop cases. S14-S15 remain planned and must not auto-start.
No S9 archive production changes occurred before this owner-directed reorder.
The following S8 checkpoints are retained evidence, not active work:
XP MIDI/FM device attachment through existing
Lib capabilities. The first checkpoint is the pinned-source subset and port/
lifecycle audit. A local FM subset checkpoint now passes the exhaustive
index/data/status matrix on both widths. FM now also passes actual I/O-table
alias/conflict/reset/detach tests through the existing DSP device owner.
The MIDI device parser now passes both-width complete-message/running-status,
SysEx/realtime/bounds and retained-tail fixture tests. It is now linked into
the device archive and actual MPU port dispatch. The combined attachment
checkpoint passes four focused tests per width, including Mixer gain forwarding
and host-output failure classification. The Compat host-output adapter also
passes ownership/backpressure/cancellation/volume fixture tests. Complete
pause/resume/destruction and native-path qualification remain unfinished.
Cancellation-failure propagation now passes two focused tests per width: the
Compat handle lock protects the retained fault, and the existing device status
path rejects machine execution with an I/O error. No public API changed.
First full x64 qualification passes 95/95 shared and 64/65 other background
tests; the sound test's frame-ratio failure also reproduces in isolation.
A probe proves unequal counting windows across cold-reset host cleanup; the
bounded three-block measurement correction passes ten repetitions per width.
Faulted-MIDI stop cleanup and resume admission are now repaired locally;
the new actual-machine regression fails before repair, and the focused
device/parser/Compat group passes 3/3 per width after repair. The second full
x64 run passes 95/95 shared and 63/65 other tests. Both failures are paused-to-
stopped transitions: a repeated isolated probe proves paused MIDI service
returns INVALID_STATE before the CCPU stop signal arrives, faulting the run.
The service now idles successfully while paused; guest-byte admission remains
rejected. Deterministic device/parser checks pass both widths; both affected
integrations pass twenty repetitions each on each width. The third full x64
run passes 95/95 shared and 64/65 other tests: checkpoint_smoke times out.
Ten isolated repetitions and the unchanged 65-test parallel rerun pass;
the timeout cause is not established. The fourth complete qualification passes
165/165 on each width (95 shared, 65 other, five native desktop), no skips.
Both EXEs are refreshed; user INI/media remain unchanged. This proves that
run passed, not that the earlier intermittent timeout has been explained.
Executor 980984d0 is pushed and actual-change reviewed as recorded above.
The proposal records focused evidence and pending coverage. Public
eight-directory changes are not planned. Sound-state serialization stays
in S15; T93 is open. Earlier retained S9 archive references below describe
the prior numbering, superseded by this explicit owner reorder.
S6 executor `065ac9ed` is pushed and actual-change reviewed; S6 closes under
delegated sequential acceptance. Dual-width 160/160 qualification includes
all ten native desktop cases and refreshed EXEs. The required post-edit
Emulator audit passes for its +10/-4 production change; broader findings are
the S7 scope. See the [S6 closure](../history/M9-T93-S6-xp-dsp-pcm-mixer.md).
The earlier S7 delivery completed implementation and qualification: both widths
pass 161/161 (95 shared, 61 other, five native desktop), and final changed
static checks pass another 8/8 per width. Both EXEs are refreshed; production
C/H is +264/-242, net +22, with no mirror or audio change. The post-edit
public-code audit and complete ledger are recorded in the
[S7 delivery](../proposals/m9-xp-sound-blaster-standalone.md#s7-executor-qualification).
Executor b1934914 was pushed and separately actual-change reviewed; that prior
closure is retained in the [S7 history](../history/M9-T93-S7-shared-contract-quality.md).
The owner has now separately accepted that import continuation above.
The retained earlier automatic S8 admission was withdrawn during review;
the current explicit admission supersedes it without undoing accepted repairs.
The following S6 checkpoints are retained implementation history, not pending
work or a second active packet.
The owner explicitly approves pause/resume use of the existing Emulator
set_heartbeat hook and requires a public-component code-quality audit after
the change. This clears the prior approval blocker. Source-only DSP/Mixer/EOI checkpoints are retained
in the proposal. The approved Emulator pause hook is now implemented locally;
its deterministic worker regression passes x64/x86. The existing PIC EOI
callout now has a Compat owner; its real-PIC regression also passes both
widths. Delayed IRQ eligibility and cancellation now pass real master/slave
PIC regressions on both widths. The Compat PCM conversion/output seam also
passes both widths, including failed-startup cleanup and retained ownership;
DSP lifecycle integration remains unfinished. The XP Mixer subset is now
retained and passes its dual-width register/gain matrix; real port/DSP output
integration remains pending, rather than stubbed as successful.
The selected DSP rate/controller-access functions also pass dual-width real
DMA-port tests. DSP command parsing also passes dual-width focused regressions,
including address wrap and real immediate/delayed PIC interrupt delivery;
the retained virtual DMA-position operation also passes its readiness,
direction, wrap and block-threshold matrix on both widths;
frozen guest-block capture and partial PCM-prefix submission also pass both
widths against actual DMA/memory and a scripted host sink;
the completion/IRQ/cancellation matrix and explicit time-unit regression
pass both widths; guest DMA pause/continue now use the real XP state transition,
with their focused command/reset regression passing both widths;
DSP/Mixer stream gain and output-format ownership also pass their focused
dual-width host-fixture regression; real start/stop and EOI continuation now
pass the DMA/PIC/host-fixture chain on both widths;
the actual DMA address/count port reads also pass the selected-channel,
low/high-byte and direction matrix on both widths;
the actual DSP/Mixer port-handler bodies and ResetAll now replace the last
protocol fixture override and pass focused dual-width regressions;
actual I/O-table registration, conflict rejection, detach and reattach now
pass both widths, including linkage through the production device archive;
machine initialization/reset/destruction now invoke that device owner, and
close-failure retention plus real cold-reset regressions pass both widths;
expanded machine regressions pass 5/5 per width. Executor PCM service and
failure propagation pass the three focused DMA/DSP regressions per width.
Compat active/progress failure forwarding also passes both widths. Device-owned
paused-block retention passes final dual-width focused (3/3) and expanded
machine (5/5) regression. The real DSP/Compat/Lib-worker first/repeated PCM
delivery smoke also passes both widths. Final package qualification passes
160/160 per width (94 shared, 61 other background, five native desktop cases),
with both EXEs refreshed. The owner-required post-edit Emulator quality audit
passes for the bounded pause-hook change; known wider findings stay in S7.
S6 executor and actual-change acceptance are recorded in its closure above.
Owner now orders the shared-quality repair
immediately after S6: planned S7 precedes MIDI/FM attachment (renumbered S8)
and final archive qualification (renumbered S9). Following reviewed S6 closure,
S7 was the prioritized packet. The [S7 brief](../proposals/m9-xp-sound-blaster-standalone.md#s7--shared-contract-and-quality-repair)
owns the finding-to-regression ledger and local implementation checkpoints.
S5 executor a34f858f is pushed and actual-change
reviewed; S5 closes under delegated sequential acceptance. Its existing DMA
copy/split repair is +28/-15 production and +107/-5 tests. Both widths pass
94 shared + 55 other + five individually isolated native desktop cases.
Sequential CTest child DLL-initialization failures remain in TODO; they are
not claimed fixed. Both EXEs are refreshed. S6's shared Emulator pause-hook
change is now explicitly approved, with a mandatory quality audit. S4 executor
`46ad2e3f` is pushed and actual-change reviewed; S4 is closed under delegated
sequential acceptance. It adds neutral host MIDI and truthful raw-FM capability. Both
widths pass 154/154 including all ten native desktop tests; both EXEs are
refreshed. Production C/H +444/-0 and test C +356/-0; no mirror or existing
PCM/Compat/Machine/Emulator change. The S4 proposal ledger records native
capability versus mocked-output evidence and artifact hashes. S3 PCM
executor b31b6e17 is pushed and actual-change reviewed, with both widths
passing 150/150 tests including all ten native desktop cases. S2 structural
relocation is verified and reviewed in 7f642848. Host-output
directions are recorded below; no sound payload/backend change is imported. Final T92 executor
`48caa854` and review `93d0b8d6` are pushed. Both Release packages and
150/150 tests per width pass, including all ten reserved desktop cases.
See the [T-level audit](../history/M9-T92-completion-audit.md) and
[S6 review](../history/M9-T92-S6-xp-baseline-qualification.md).

Owner explicitly admits S1 design: "好的 准入s1设计 不要留下未知".
The proposal's source-recheck checkpoint records seven pinned XP objects,
host/DMA/MIDI/mixer exclusions and the host progress/lifecycle contracts.
S1 now has a per-entry XP PCM/MIDI/FM host-call ledger and proposed S2-S8
implementation briefs. It is not implementation-qualified. No production/test
code or package artifact changed.

Owner supersedes the initial software-MIDI choice on 2026-10-15: use host MIDI
and accept approximate note/controller restoration rather than exact audible
phase. TinySoundFont, SoundFont packaging and the approximately 10,000-line
combined engine recommendation are withdrawn. No such source was imported.
The owner subsequently prioritizes matching XP over guaranteed audible FM.
All software-OPL recommendations and software-synthesis archive/budget plans
are withdrawn too. S1 design delivery is reviewed; those earlier drafts are not authority.

FM source audit now reaches XP's Ki386AdlibEmulation: the no-hardware fallback
only emulates status and does not generate sound. The direct-I/O branch uses
real ports. Read-only native enumeration here finds one MIDI output, Microsoft
GS Wavetable Synth (technology 7), and no MOD_FMSYNTH output (technology 4).
This is not a playback test. The revised proposal keeps WASAPI PCM, adds host
MIDI, and explicitly reports raw FM as unsupported on current standalone
platforms; XP-compatible silent device handling stays outside Lib.
The owner requests audio/pcm, audio/midi and audio/fm ownership planning.
PCM needs rate admission, real host-consumption progress, short-tail delivery
and stream-local gain; unused XP pause/restart and header APIs are not copied.
S2-S8 separate relocation, PCM, MIDI/FM host capabilities, DMA repair, XP PCM
device integration, XP MIDI/FM integration and portable-state qualification.
The disclosed shared pause-hook behavior change still requires approval in
its implementation packet. S1 executor 2860b484 is pushed and actual-change
reviewed. The continued owner implementation goal admits S2 below; no S2
production change was made at admission. S2's PCM relocation is now implemented
and verified: x64/x86 each pass 150/150 including all ten native desktop cases.
Both package EXEs are refreshed; no playback behavior changed. Executor
delivery 7f642848 is pushed and actual-change reviewed; S2 is closed.
The owner authorizes native desktop test scheduling without repeated approval
requests; background operations remain hidden and Computer Use remains forbidden.
No manual audio acceptance is inferred.

S3 implementation is closed under delegated sequential acceptance after
executor b31b6e17 and separate actual-change review. Its PCM stage has
submitted/consumed accounting, native progress-event wait, per-channel stream
gain, format-error classification and arithmetic validation. The existing
native adapter unit passes on x64/x86 with seven rates, priming/tail/clear,
partial-failure and gain coverage. The root admission now accepts representable
nonzero PCM rates rather than only three fixed rates; root and native PCM
tests both pass on x64/x86 (2/2 each), including seven rates in mono/stereo
and zero/byte-rate overflow rejection. This is mocked endpoint evidence, not
proof of every physical endpoint's rate support. Short prefixes now submit
without flush or subsequent input. Root/native PCM and Compat first-delivery
tests each pass 50 repetitions per width; clear retains deterministic discard
coverage. Native startup now completes before create publishes a stream;
attach rejection and failed startup signaling are covered. The public channel
gain request reuses the existing control slot and worker. The same three
targeted tests pass another 50 repetitions each on both widths after these
changes. Copied epoch/accepted/submitted/consumed progress and a borrowed wake
sink are now integrated into the existing worker. Final-tail consumption,
enqueue interruption of a tail wait, clear cancellation, counter limits and
sink failure pass the same repeated dual-width targeted set. Both Release
builds pass; each width passes 90 shared, 55 other background and five native
desktop tests (150/150). Both package EXEs are refreshed and hashed in the
proposal. No manual hearing or Linux runtime result is claimed.

## M9 T93 S11 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts S10 and explicitly admits S11 through closure; no automatic S12 admission. |
| Objective | Owner-reopened S11 test ownership only: move direct Base UTF-8 validator assertions from Storage to Base; retain all Storage public API rejection tests. |
| Non-goals | No App configuration migration, Console text/title/audio/capture repairs, new OS backend, user configuration/media/snapshot edits or NXVM writes. |
| Reference Baseline | Accepted S11 endpoint 58140a9a; prior full qualification 166/166 per width. |
| Candidate Proposal | [S11 UTF-8 paths](../proposals/m9-xp-sound-blaster-standalone.md#s11--utf-8-paths-and-native-directory-rules). |
| Files And ABI Surface | Lib Types native encoding vocabulary, Base process/UTF-8 validation and Storage platform files, public path contracts, owner-local tests/manifests/docs. Storage uses the inward Base validator; no outer-layer or mirror changes. |
| Applicable Rules | Task Reading Set, architecture/coding and source/research policy; single-agent dual-role execution; necessary-source subset, Types authority and inward dependencies. |
| Verification | Owner override: build only base_text and storage_utf8 on x86/x64; run those tests plus Lib test-manifest, test-boundaries and test-types-boundary gates, documentation and diff checks. No full rebuild/regression, desktop test or EXE refresh. |
| Expected Markers | Same UTF-8 interpretation for reads and writes, explicit invalid/unsupported paths, unchanged byte data and locking; one platform route, no fallback or path framework. |
| Asset Needs | Hidden supervised build/t93/s11 jobs with log/exit files; 1800s build, 1200s suite, 600s desktop, 30 MiB logs; executor cleanup. Reuse cached trees, no Computer Use; native desktop scheduling preapproved. Disposable generated fixtures only. |
| Reporting Requirements | Audit findings with exact ownership and evidence; implementation brief with per-component added/deleted/net estimates before edits; actual diff/test/artifact accounting and separate actual-change review. |
| Stop Conditions | Unjustified abstraction, product-specific public API, new backend scope, behavior change without authority or failed required regression; resolve before delivery. |
| Exit Criteria | Relocated assertions and retained Storage rejection coverage pass focused dual-width proof; production and EXEs unchanged; executor commit/push and separate actual-change review. Close only this owner-reopened S11 correction; T93 stays open. |
| Original Owner Request | 验收通过，准入执行S11到收口。 Follow-up: Storage 测试包含 Base UTF-8 校验器的纯边界断言，建议移回 Base 自有测试；重开本 S，仅重新编译测试改动部分。 |
| Similar-Issue Sweep | Base/Storage path APIs, narrow native opens, status propagation and root parsing; account for every production hit and distinguish deferred S12 text APIs. |

## M9 T93 S1 Retained Design Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admits S1 design, selects host MIDI with approximate restoration and XP fidelity over guaranteed FM sound, then requests an exhaustive PCM/MIDI/FM host-interface ledger and audio leaf/S-task plan. |
| Objective | Recheck the fixed XP sound subset and design a complete standalone DSP/PCM/Mixer/FM/MIDI integration with one device owner; produce executable later-S briefs. |
| Non-goals | No production import, new backend dependency, shared-package change, guest asset mutation, or claim of working sound in S1. |
| Reference Baseline | Accepted T92 endpoint 93d0b8d6; XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89, necessary-source-subset policy. |
| Candidate Proposal | [Sound capability and S1 brief](../proposals/m9-xp-sound-blaster-standalone.md#t93-s1-admission-and-design-brief). |
| Files And ABI Surface | Audit seven XP sound candidates and existing DMA/port, Compat audio, machine lifecycle/snapshot seams; change planning/evidence only. No ABI change. |
| Applicable Rules | Task Reading Set; single-agent dual-role execution/documentation governance; architecture/coding and source-research authorities for design. |
| Verification | Fixed Git-object source recheck, dependency/call-path census, requirement-to-owner/proof matrix, executable estimates, documentation gate and actual-change review. No invented runtime result. |
| Expected Markers | All 24 loaded WinMM entries and FM native operations accounted for; Lib/Compat/device boundaries, no fake FM output, proposed audio/pcm+midi+fm layout, source provenance and S2-S8 proof/budget matrix. |
| Asset Needs | Read-only upstream Git objects; any long audit uses hidden bounded build/t93/s1 logs, 600-second budget and 30 MiB cap, executor-owned cleanup. No Computer Use. |
| Reporting Requirements | Prechange scope and estimates; per-file proposed source/test add/delete/net, subset exclusions and patch causes; distinguish established evidence from unresolved decisions. |
| Stop Conditions | Missing source admission, unapproved backend/public capability, second device/timing path, or unresolved guest semantics. Present choices before implementation. |
| Exit Criteria | Owner-reviewed host-fidelity design and complete S2-S8 briefs, documentation proof and pushed executor delivery followed by separate actual-change review; no automatic payload admission or S1 closure from this planning revision. |
| Original Owner Request | 验收通过，准入下一个T任务！ Queue head retains the complete XP Sound Blaster capability separated from T92, not a DSP-only substitute. |
| Similar-Issue Sweep | Inventory NTVDM/native-device dependencies, host-pointer narrowing, polling/global-volume assumptions, missing MIDI tails and reset/teardown hooks; assign every finding to an owner or explicit decision. |

The following retained progression records how this baseline was reached:

T91 and S5 are owner-accepted and closed on 2026-10-15.
T92 S1 source audit is the reviewed planning baseline. The owner's subsequent
implementation goal admits S2's bounded original-device repair work.
All 997 XP paths are accounted for. S2 changed three of the 498 retained
mirror files by +7/-7. S3's uncommitted whole-file candidate is superseded by
the owner's necessary-source-subset correction. Its reviewed unused NTVDM
branches are removed; final corrected qualification passes on both widths before
S3 commit or further subsystem import. Accepted fixes and user changes remain.
The first corrected subset passed all 148 registered tests per width, including
five native desktop cases. Owner subsequently permits removal of proven useless
pre-existing conditional branches within the migration scope. This cleanup is
complete for the reviewed groups. The final candidate separately passes 148/148
registered tests per width, including five native desktop cases; both EXEs are
refreshed. Executor P1 89b67c44 is pushed. Coordinator actual-change review finds
two leftover imported A20 declarations in tape_io.c. They are removed; renewed
dual builds and 148/148 tests per width pass, including all ten desktop cases.
Corrective P2 3cf6906a is pushed and coordinator actual-change review passes.
S3 is implementation-closed under the owner's async delegation. S4's final
input/timing subset now passes dual Release builds and 148/148 tests per width,
including all ten native desktop cases; both package EXEs are refreshed.
Its production delta is +5,843/-8,750 (net -2,907), tests +164/-3 (net +161).
Executor 5c5af37a is pushed and its separate coordinator actual-change review
passes. S4 is implementation-closed under the owner's async delegation;
S5's reviewed video subset is now applied across 27 files, +3,644/-6,544
(net -2,900). All 71 admitted paths match either the reviewed candidate or
the frozen local input; 44 remain unchanged, including 40 C-VID files.
Initial dual-width background compilation passes. A 73-line owner-local BIOS
regression now checks all 256 DAC entries through block set/read and video-state
save/restore, including modified-palette and buffer-boundary assertions. Its x64
and x86 runs pass. Final dual-width regression passes 148/148 per architecture,
including all ten reserved native desktop tests. The 71-path subset/range ledgers
pass independent pinned-object reconstruction; both package EXEs are refreshed
and hashed. Executor commit/push and separate actual-change review
are complete: executor 41aa0bc2 is pushed and coordinator actual-change review
passes. S5 is implementation-closed under delegated async acceptance; S6 was
then admitted for the final XP manifest/gate and T-level qualification.
S6's initial 498-path manifest reconstruction and the new local-only mirror
gate/selftest pass. All 997 XP paths reconcile without overlap or omission;
the active-baseline/lineage documentation is reconciled. Full qualification
passes all 150 registered tests per width, including all ten native desktop
cases. S6 executor delivery and separate T-level review are complete.
This is not an invented owner manual-pass result. See the
[T92 audit](../etc/evidence/softpc/xp-sp1-hunk-semantic-audit-zh.md).
Owner approves the two-task plan on 2026-10-15: T92 retains core migration;
new Sound Blaster capability transfers to the first unnumbered Queue candidate.
S2 independently repairs DMA, disk BIOS and V7 and guards ROM CS; S3-S5
migrate controller/BIOS, input/timing and video; S6 qualifies the XP baseline.
S2 executor delivery e884ff9b is pushed and its coordinator actual-change
review is recorded in the proposal. Each width passed 90 shared + 51 other
background + 5 desktop tests (146/146), with both EXEs refreshed; final
focused/gate rechecks pass 7/7 each. On 2026-10-15 the owner authorizes async
acceptance and automatic serial admission through S6, waiting only before
T92 closure. S2 is implementation-closed on that delegated basis, not marked
as manually tested by the owner. S3-S6 are implementation-closed; T closure
waits for the owner.
Owner explicitly approves XP source import on 2026-10-15. The pinned source,
original notices and local-patch ledger govern S3; no third-party license or
redistribution grant is inferred. S2 repaired existing code without XP bodies.
T92 remains open; sound is not a T92 exit claim.
Executor audit `2616deb8` is pushed; its coordinator actual-change review is
recorded in the linked proposal. Proceeding to S2 does not claim runtime
qualification of the source audit or close T92.

## M9 T92 S6 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorizes automatic serial S admission with async acceptance, waiting only before T closure; S5 actual-change review passes. |
| Objective | Make the pinned XP necessary subset the auditable routine mirror baseline; qualify all retained paths and the complete migrated product. |
| Non-goals | No new device, sound capability, production rewrite, public-package change, user configuration/media change, or external build dependency. |
| Reference Baseline | Reviewed S5 executor 41aa0bc2; fixed XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89 and OpenNT pins in the proposal. |
| Candidate Proposal | [S6 baseline gate](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md#s6--xp-baseline-gate-and-opennt-lineage-freeze). |
| Files And ABI Surface | All 498 retained and 499 unretained paths for accounting; existing S3-S5 range ledgers; repository-owned verification/test registration and architecture/provenance docs. No production ABI change. |
| Applicable Rules | Task Reading Set; execution, coding, architecture and documentation governance; source-research and necessary-subset policy. |
| Verification | Independent fixed-object reconstruction; complete manifest and negative gate tests; protected-path/diff audit; dual Release artifacts, all shared/other/desktop tests and requirement-by-requirement T audit. |
| Expected Markers | No unaccounted path; local patches remain visible; routine checks need no external checkout; OpenNT lineage is frozen rather than active baseline. |
| Asset Needs | Reuse existing Ninja intermediates; bounded ignored build/t92/s6 evidence; hidden build 1800s, suites 1200s, desktop/audit 600s, 30 MiB log cap; executor owns cleanup. |
| Reporting Requirements | Prechange audit and estimates; added/deleted/net by production/test/metadata; full evidence, artifact hashes, executor push and separate actual-change review. |
| Stop Conditions | Missing provenance, unexplained selected-source drift, new production behavior, public ABI expansion, or failed qualification. |
| Exit Criteria | Verified/pushed S6 and coordinator review; T-level audit complete, then wait for owner before formal T closure. |
| Original Owner Request | 按照治理规范和执行规范，单人双角色模式，实现现有softpc核心迁移到winxp源码的基线上；必要子集，保留真实本地补丁；异步验收，T收口前等待确认。 |
| Similar-Issue Sweep | Stale OpenNT/whole-file active claims, missing/extra paths, hidden local patches, unpinned reference or external build dependency; disposition each hit. |

## M9 T92 S5 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorizes automatic serial S admission with async acceptance, waiting only before T closure, on 2026-10-15; S4 actual-change review passes. |
| Objective | Migrate the necessary XP video subset across 31 changed candidates and preserve accepted patches across 40 unchanged-base C-VID paths. |
| Non-goals | No Sound Blaster, new presenter or mode, public-package change, user INI/media/snapshot change, or generated production sources. |
| Reference Baseline | S4 executor 5c5af37a; fixed XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89 and OpenNT pins in the proposal. |
| Candidate Proposal | [S5 video migration](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md#s5--xp-first-c-vid-and-video-basis). |
| Files And ABI Surface | 71 original-batch-S4 paths in the retained-path ledger, with 31 possible XP payload changes and 40 C-VID preservation checks; owner-local tests as justified. No public ABI expansion. |
| Applicable Rules | Task Reading Set, execution/documentation/architecture/coding governance, source-research policy and necessary-source-subset policy. |
| Verification | Fixed-object three-way/range accounting; local video/marker/snapshot preservation; focused mode/cursor/stride/geometry matrices; dual Release builds and packages; all shared, other background and reserved desktop tests. |
| Expected Markers | Useful XP video changes retained without foreign host routes; selected markers, stable geometry, completed paint, cursor/palette/font and snapshot contracts remain correct. |
| Asset Needs | Reuse Ninja intermediates; ignored bounded audit evidence under build/t92/s5; hidden jobs with 30-minute build, 20-minute suite, 10-minute desktop budgets and 30 MiB log cap; executor owns cleanup. |
| Reporting Requirements | Audit first with per-file changes and estimates; actual added/deleted/net and XP-subset patch/exclusion counts; dual artifacts, test evidence, executor push and separate actual-change review. |
| Stop Conditions | New device/presentation path, public ABI expansion, unsupported guest semantic change, conflicting source terms or unexplained regression. |
| Exit Criteria | Complete verified/pushed S5 and coordinator actual-change review permit S6 admission; T closure still waits for owner. |
| Original Owner Request | 按照治理规范和执行规范，单人双角色模式，实现现有softpc核心迁移到winxp源码的基线上。采用必要源码子集；既有同类完全无用分支可删除。各 S 顺序推进，只在 T 收口前等待确认。 |
| Similar-Issue Sweep | Video register/value narrowing, selected dirty-marker dispatch, geometry/publication and host-only declaration dependencies; disposition every hit without introducing alternate paths. |

## M9 T92 S4 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner authorizes automatic admission and execution of subsequent S tasks with async acceptance, waiting only before T closure, on 2026-10-15. |
| Objective | Audit and migrate the necessary XP input/reset/IRQ/ROM/timing subset in the 18 S4 candidates; preserve accepted local input, snapshot and timing behavior. Prove usefulness before retaining XP branches or deleting existing unused conditionals. |
| Non-goals | No Sound Blaster integration, S5 video migration, public-package change, user INI/media/snapshot edit or second device path. |
| Reference Baseline | S3 P2 3cf6906a; fixed XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89 and OpenNT revisions in the proposal. |
| Candidate Proposal | [S4 migration scope](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md#s4--xp-first-input-reset-irq-rom-and-timing-basis). |
| Files And ABI Surface | 18 paths listed in the S4 admission brief; owner-local tests and root registration as needed. No public ABI expansion. Audit first; quantified payload brief precedes source edits. |
| Applicable Rules | Task Reading Set, execution/documentation/architecture/coding governance, source-research policy and minimal explicit mirror patches. |
| Verification | Independent XP/local range ledgers; scan-set translation/type/reset and mouse/IRQ/timing/ROM tests; dual Release builds and refreshed EXEs, shared then all other background tests and reserved desktop cases. |
| Expected Markers | Useful XP input improvements retained, valid physical-key set-3 mappings verified without changing reserved non-key 14; ROM CS and mouse BX not truncated; accepted InPort, snapshot, IRQ and event-wait behavior preserved. |
| Asset Needs | Reuse Ninja intermediates; disposable audit/test fixtures under ignored build/t92/s4; hidden jobs with 30-minute build/20-minute suite/10-minute desktop budgets and 30 MiB log cap; executor owns cleanup. |
| Reporting Requirements | Pre-change file/hunk estimates before payload edits; actual added/deleted/net code and mirror differences; dual artifacts, test outcomes, pushed P and separate actual-change review. |
| Stop Conditions | Owner source-import direction is recorded; stop for conflicting/new source terms, new device paths, public ABI expansion or unapproved guest semantics. |
| Exit Criteria | Verified and pushed implementation plus separate coordinator actual-change review permit automatic S5 admission under owner delegation; only T closure waits for acceptance. |
| Original Owner Request | 请立即纠正 T92 当前 XP 源码导入方案。采用必要源码子集，不是保留文件就整文件导入。先暂停后续导入和提交，审计当前未提交改动；不要整体撤销已有正确修复，也不要干扰用户改动。各 S 可顺序推进，只在 T 任务正式收口前等待确认。 |
| Similar-Issue Sweep | Keyboard forward/reverse table consistency, narrowed register setters, conditional host-only declarations, selected mouse/IRQ/timer contracts; disposition every hit without expanding scope silently. |

## M9 T92 S2 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner directs implementation of the existing-core XP migration goal after approving the two-task plan and supplying the independent repair brief. |
| Objective | Correct DMA auto-init address reload, INT 13h AH=15h sector-count splitting and V7 cursor enable comparison; protect correct ROM CS from later XP narrowing. |
| Non-goals | No bulk XP payload import, sound wiring, shared-package changes, new device path, user INI/media/snapshot edit or unrelated mirror formatting. |
| Reference Baseline | f55428f8; retained XP/OpenNT pins and audit ledgers in the proposal. |
| Candidate Proposal | [S2 repair brief](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md#s2--independent-original-device-repairs-and-rom-narrowing-regression). |
| Files And ABI Surface | Three original files at_dma.c, diskbios.c, v7_ports.c; ROM production unchanged; App owner-local device tests and root build registration. No public ABI change. |
| Applicable Rules | Full Task Reading Set; execution/documentation/architecture/coding governance, source-research policy, single executor and minimal mirror diff. |
| Verification | Red/green tests through actual DMA, BIOS command, V7 port and ROM initialization paths; both Release builds and refreshed EXEs; shared suites then all other unit/integration tests; reserved native desktop tests separately. |
| Expected Markers | Second DMA round starts at base address; 16-bit CX:DX/AH/CF correct; cursor transitions invoke existing drawing/clear path; ROM initialization observes full CS. |
| Asset Needs | Disposable generated media/ROM only beneath build/t92/s2; existing Ninja graphs reused. Long jobs hidden with log/exit files, 30-minute build and 20-minute suite budgets, 30 MiB log cap; executor owns cleanup. |
| Reporting Requirements | Before/after production/test file counts, added/deleted/net lines, OpenNT and XP mirror delta, red/green evidence, regression results, EXE hashes and links. |
| Stop Conditions | Need for another device path, shared ABI change, unadmitted source payload, unsupported hardware assumption or unexplained regression. Desktop work waits for reservation. |
| Exit Criteria | Complete repair and required qualification, executor P commit/push, coordinator actual-change review, then owner acceptance; T stays open. |
| Original Owner Request | 按照治理规范和执行规范，单人双角色模式，实现现有softpc核心迁移到winxp源码的基线上。 Supplement: independent DMA/disk/V7 repairs and ROM-CS regression from the owner brief. |
| Similar-Issue Sweep | Narrowed register setters and equivalent DMA reload/enable comparisons in selected original device paths; report other findings without expanding S2. |

## M9 T92 S1 Retained Audit Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner accepts T91 and admits the next queued XP SoftPC core task on 2026-10-15. |
| Objective | Establish the immutable XP/OpenNT/current three-way migration ledger and executable import batches for the selected IBM PC/AT/V7 profile. |
| Non-goals | No source import, runtime edits, public package changes, EXE refresh, media/configuration changes or external repository writes in S1. |
| Reference Baseline | SoftPC 087e512fc4bb02225d3d8cb519a12ef87aa489ac; XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89; OpenNT 5e4619ab61c2aa76151e03973cce340be2933e61. |
| Candidate Proposal | [XP SP1 selected mirror upgrade](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md). |
| Files And ABI Surface | Audit all 498 retained mirror paths and relevant XP-only candidates; enumerate Compat/port ABI consequences without modifying interfaces. |
| Applicable Rules | Task Reading Set, execution/documentation governance, architecture/coding rules and source-research-policy; source admission precedes any later copy. |
| Verification | Reproducible immutable-object inventory, three-way diff classification, selected-profile branch review, source/notice provenance and documentation/whitespace gates. No runtime qualification claimed. |
| Expected Markers | Every retained path has an explicit disposition; necessary local fixes are preserved; later batches have bounded file lists and tests. |
| Asset Needs | Read-only reference repositories; ignored evidence under build/t92/s1. Hidden supervised audit jobs: 10-minute/time and 30 MiB/log limits, executor owns cleanup and checkpointing. No guest asset needed. |
| Reporting Requirements | Per-batch files, additions/deletions/net and mirror divergence estimates; authority for changes, exclusions, risks and executable recommendation. |
| Stop Conditions | Unresolved provenance/licensing before import, unavailable immutable source, or a proposed loss of accepted behavior; no blind working-tree replacement. |
| Exit Criteria | Complete source ledger and source-admission findings, explicit S2-S6 scope/verification plan, coordinator review and committed report for owner decision. |
| Original Owner Request | 好，验收通过，收口本T，准入队列下一任务：导入winxp的softpc核心。 |
| Similar-Issue Sweep | All selected CCPU, C-VID, BIOS, input, device and host boundaries; distinguish XP fixes, excluded host routes and existing local fixes, including snapshot state. |

## M9 T91 S5 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly admits deletion of src/x86-nxvm and test/x86-nxvm. |
| Objective | Retain only the eight reusable source/test components used by SoftPC; remove unused NXVM contracts and their registrations. |
| Non-goals | No surviving production behavior change, external repository write, INI/media edit or rule-file change. |
| Reference Baseline | 3af19559, accepted S4. |
| Candidate Proposal | [Removal ledger and qualification](../history/M9-T91-S5-unused-nxvm-package-removal.md). |
| Files And ABI Surface | Ten unused package files, root CMake, test and source boundary gates, current layout documents. |
| Applicable Rules | Reading Set; execution, architecture, coding and documentation governance. |
| Verification | Dual incremental Release builds; all remaining shared and App background tests; dependency inventory and absence sweep; documentation and whitespace gates. No native implementation changes: retain S4 desktop qualification without claiming a fresh run. |
| Expected Markers | Eight reusable roots, no active NXVM-package references, 138 background tests per width; accepted desktop binaries unchanged or explicitly accounted for. |
| Asset Needs | Existing Ninja graphs and EXEs; hidden supervisor under build/t91/s5, 30-minute build/20-minute suite budgets; preserve INI and guest assets. |
| Reporting Requirements | Deleted files, production/test/build additions/deletions/net, actual build/test outcomes and artifact hashes. |
| Stop Conditions | Surviving production dependency on removed package, unrelated edits or behavioral failure. |
| Exit Criteria | Tested removal, complete P push, coordinator actual-change review, wait for owner acceptance; T stays open. |
| Original Owner Request | 好。准入一个新S，删掉这俩没用的组件。 |
| Similar-Issue Sweep | All NXVM package paths/symbols/targets in source, tests, scripts, build registration and current design; historical records remain factual. |

## M9 T91 S4 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly admits the three-way source/test split. |
| Objective | Move debugger to x86-debug/product, assembler to x86-debug/xasm32, shared PC shell to x86-ibmpc/product, and retain NXVM-only contracts in x86-nxvm/product. |
| Non-goals | No Core import, runtime policy change, mirror/Compat change, or modification of NXVM. |
| Reference Baseline | SoftPC 7d2ca0f6; current shared roots previously matched NXVM byte-for-byte. |
| Candidate Proposal | [S4 design, convergence ledger and qualification](../history/M9-T91-S4-x86-package-split.md). |
| Files And ABI Surface | Three source/test packages, App includes, build/link names, manifests, boundary gates and current layout docs. Public symbols follow their new owners; no compatibility aliases. |
| Applicable Rules | Execution, architecture, coding and documentation authorities; existing X86 tier is refined by this admitted split. No rule files edited. |
| Verification | Six standalone configure/dry-run build graphs; compile once in each existing aggregate graph; x64/x86 Release builds; each width 97 shared + 48 other background + 5 reserved desktop tests pass (150/150). |
| Expected Markers | All registered tests pass; old paths absent; speed/info only in x86-nxvm; two refreshed EXEs. |
| Asset Needs | Refresh only package EXEs; preserve INI, media and snapshots. Ignored evidence under build/t91/s4; hidden supervised jobs, 30-minute build and 20-minute test limits, bounded logs. |
| Reporting Requirements | Before/after source/test counts, relocation-aware diff, unchanged runtime semantics, actual test results and dual EXE links. |
| Stop Conditions | Public behavior changes, unexpected user edits, or failure requiring a second runtime path. |
| Exit Criteria | Complete import-independent split, all required validation, executor P push and coordinator actual-change review; wait for owner acceptance. T91 remains open. |
| Original Owner Request | Split x86/debug -> x86-debug/product; x86/xasm32 -> x86-debug/xasm32; x86/product -> x86-ibmpc/product plus retained NXVM-only x86-nxvm/product; split matching tests. |
| Similar-Issue Sweep | Search old paths/targets/symbols, private speed/info leakage, outward source/test/build/document dependencies and cross-suite fixtures. |

## Recent acceptance
S5 removes the unused NXVM source/test pair; only eight reusable roots remain.
Both Release build graphs pass; x64/x86 each pass all 138 background tests.
Native desktop tests are not rerun in S5; S4's 5/5 per width remains separate
prior evidence. x64 EXE is unchanged; x86 EXE is refreshed after Ninja's cache
recovery rebuild. Owner configuration and media remain unchanged. Executor P1
`1f0c0f53` is pushed and reviewed in `087e512f`; owner accepts S5 and T91 on 2026-10-15.
S4 splits source and test ownership into x86-debug, x86-ibmpc and x86-nxvm.
Executor P1 `8046b085` is pushed; its actual-change review is retained with S4.
Both EXEs are refreshed. All 150 tests per architecture pass; initial migration
gate/build failures are retained in the delivery record. Owner accepted and
closed S4 on 2026-10-15, against P1 `8046b085` and review P2 `ffe0a702`.
T91 is now closed; see the [completion audit](../history/M9-T91-completion-audit.md).
S3 is owner-accepted and closed on 2026-10-13. Implementation is delivered in
`e96b2983`; coordinator actual-change review is recorded in `74b45795`.
Both builds and all 137 tests per width pass, including the reserved desktop
recheck. Initial unexpected-input failures remain recorded, not hidden.
At S3 delivery, six roots matched NXVM `97d3cf948` exactly (246 files).
S4 intentionally supersedes that directory/symbol layout, not its behavior.
See the [S3 delivery](../history/M9-T91-S3-nxvm-cursor-contract-import.md).
S2 audit is accepted as the basis for this import on 2026-10-13.
See the [S2 audit](../history/M9-T91-S2-nxvm-cursor-contract-audit.md).
S1 is owner-accepted and closed on 2026-10-12. Implementation is delivered in
`62ec8e06`; coordinator review is recorded in `9611f5b6`.
Both Release EXEs are refreshed. Each width passes 84 shared + 48 other
background tests and all five native desktop cases (137/137).
The S1 six-package baseline was reviewed NXVM `73d7967244` (246 files).
T91 was kept open at S1 delivery and is now owner-closed.
See the [delivery review](../history/M9-T91-S1-nxvm-shared-glyph-import.md)
and [retained proposal](../history/M9-T91-nxvm-shared-glyph-import-proposal.md).
T90 is owner-accepted and closed.
S1's five repairs were accepted on 2026-10-11; runnable baseline is `dae5d2bb`.
Both Release EXEs are refreshed. Each width passed 84 shared + 48 other
background tests and all five native desktop cases (137/137).
See the [S1 delivery and diff ledger](../history/M9-T90-S1-nonmirror-quality-cleanup.md).
T89 remains owner-accepted and closed.
The owner confirms the RDP capture escape issue is fixed; its TODO is removed.
TODO retains only the null BOP investigation. The former XP SP1 queue head is
now admitted as T92; the owner-approved sound follow-on is first in Queue.

## M9 T91 S3 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approves import, compilation, deployment, tests, commit/push and waiting for acceptance. |
| Objective | Import all six roots exactly from audited NXVM and preserve SoftPC HOST cursor semantics. |
| Non-goals | No NXVM writes, mirror/Compat/snapshot changes, user configuration/media edits or new blink owner. |
| Reference Baseline | SoftPC 2ea997fe; NXVM 97d3cf9482913f2a0e0872158149ff646a563f3c. |
| Candidate Proposal | [S2 exact-import audit](../history/M9-T91-S2-nxvm-cursor-contract-audit.md). |
| Files And ABI Surface | Six source/test roots; App driver and two integration fixtures use cursor_mode instead of cursor_visible. |
| Applicable Rules | Reading Set and four governance skills; exact shared corpus, Types authority, inward dependencies, hidden supervised execution. |
| Verification | Recheck upstream hashes; dual Release builds; all shared tests first, other unit/integration tests next; five desktop cases per width separately with owner reservation; inspect committed diff. |
| Expected Markers | 246 exact files; HOST for present cursor and DISABLED for absent cursor; each width complete 137-test inventory. |
| Asset Needs | Reuse Ninja intermediates; ignored build/t91/s3 logs. Hidden supervisor, 30-minute build/20-minute suite bounds and 20 MiB per phase; preserve guest assets. |
| Reporting Requirements | Separate imported/shared and receiving code additions/deletions/net; dual EXE links and actual tests. |
| Stop Conditions | Upstream drift, unrelated failure, mirror requirement or unreserved desktop use. |
| Exit Criteria | Complete tested delivery pushed, coordinator actual-change review, wait for owner acceptance. |
| Original Owner Request | 批准开始导入，完成后编译、部署、测试、提交、推送后等我验收。 |
| Similar-Issue Sweep | Search every shared/App cursor field reference; keep native Compat/Console booleans and original mirror distinct. |

## M9 T91 S2 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits a new S to audit latest NXVM six-component code quality and exact-import readiness. |
| Objective | Account for every changed shared path and examine cursor ownership, compatibility, tests and inward dependencies. |
| Non-goals | No source import, NXVM writes, fixes, EXE rebuild or runtime/desktop testing. |
| Reference Baseline | SoftPC 9295da5c; imported NXVM 73d7967244; current NXVM 97d3cf9482913f2a0e0872158149ff646a563f3c, initially clean. |
| Candidate Proposal | [Retained shared import scope](../history/M9-T91-nxvm-shared-glyph-import-proposal.md). |
| Files And ABI Surface | src/test lib, emulator and x86; changed text cursor schema and SoftPC receiving uses. Documentation only locally. |
| Applicable Rules | Reading Set; execution, documentation, architecture and coding governance; Lib Types and inward dependencies. |
| Verification | File/hash inventory, all changed source/test hunks, manifest validation, receiving-use search and source-level lifecycle analysis; distinguish this from runtime qualification. |
| Expected Markers | Exact diff totals, justified findings, import verdict and explicit receiving adaptation. |
| Asset Needs | Read-only external repository; no guest media or foreground automation. |
| Reporting Requirements | Per-component additions/deletions/net and actionable code-quality assessment. |
| Stop Conditions | Upstream drift requires recheck; no unapproved import or external writes. |
| Exit Criteria | Evidence-backed audit delivered for owner decision; T stays open. |
| Original Owner Request | 准入一个新的s任务 请你审计nxvm最新6组件，代码质量情况 是否可原样导入 |
| Similar-Issue Sweep | All cursor mode/phase consumers, host timer, Console conversion, frame comparison, manifest/build boundaries and App producers. |

## M9 T91 S1 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner approves exact import, single-person executor/coordinator review, dual builds/deployment, all shared and other unit/integration tests, commit and push. |
| Objective | Import the reviewed NXVM six roots exactly and adapt the existing SoftPC font producer/default-height consumer; qualify and deliver for owner acceptance. |
| Non-goals | No NXVM writes, mirror change, unrelated cleanup, INI/media/snapshot mutation, new rendering path or automatic closure. |
| Reference Baseline | SoftPC cc7365a6; reviewed clean NXVM 73d7967244c1591038d0c2f4dcec819192d809a1 supersedes the initial uncommitted audit. |
| Candidate Proposal | [Shared glyph-capacity import](../history/M9-T91-nxvm-shared-glyph-import-proposal.md). |
| Files And ABI Surface | Six shared source/test roots; Window copied font slots change from 16 to 32 bytes. SoftPC Compat font producer, Machine declarations and owning tests need matched layout. |
| Applicable Rules | Task Reading Set, four governance skills, inward dependencies, Lib Types, source research policy and hidden supervised execution. |
| Verification | Audit all path/hash differences and manifests first. Import phase: focused glyph/consumer checks, dual Release builds and refreshed EXEs, shared tests before other background tests, native desktop separately in reserved period, then actual diff review. |
| Expected Markers | 246 exact imported files; default/short/tall glyphs and both banks; no packed-16 producer remaining on the new frame boundary. Dual-width 137/137 including desktop passed. |
| Asset Needs | None for read-only audit. Later reuse build trees and ignored build/t91/s1 logs; hidden supervision with 30-minute build and 20-minute suite limits, 20 MiB output per phase; preserve user assets. |
| Reporting Requirements | Per-component/file additions, deletions and net; distinguish shared import from App adaptation and upstream claims from local verification. |
| Stop Conditions | Upstream drift, unreviewed source/license changes, mirror changes, unexplained semantic regression or unrelated failing tests; no blind directory overwrite. |
| Exit Criteria | Reviewed exact corpus imported, receiving adaptation verified, dual EXEs and full evidence delivered through complete P commit/push; wait for owner acceptance. |
| Original Owner Request | 准入一个T任务，S1负责审计和引入nxvm最新的公共六组件。你先审计它的diff，给我一个汇报，到底改了什么 |
| Similar-Issue Sweep | All primary/secondary font producers, packed glyph offsets, active-height validation, frame size/copy/comparison and receiving App fixtures; do not modify unchanged Console/input/audio paths. |

## M9 T90 S1 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner admits an independent T for all five audited quality items and explicitly includes native desktop tests. |
| Objective | Remove obsolete snapshot READY reuse, report terminal Compat audio errors once, share App mode parsing, delay save-file creation until first write, and correct architecture descriptions. |
| Non-goals | No mirror or shared-six implementation change, snapshot-format change, retry/rollback framework, automatic T closure, INI/media modification or Computer Use. |
| Reference Baseline | 1684475c, accepted T89. |
| Candidate Proposal | [Retained five-item quality repair](../history/M9-T90-nonmirror-quality-cleanup-proposal.md). |
| Files And ABI Surface | App product, machine driver, Compat audio, existing owner tests and design documentation; no public ABI change. |
| Applicable Rules | Task Reading Set, architecture/coding/execution/documentation governance skills, inward dependencies, Types authority and hidden supervised execution. |
| Verification | Focused failure/content tests, dual Release builds and EXE publication; each width shared background first, other background next, then five native desktop tests serially in reserved period; gates, hashes and actual diff review. |
| Expected Markers | Five ledger items proved, repeated snapshots contain current state, pre-write failure preserves old file, audio failure reports once but shutdown cancellation does not. |
| Asset Needs | Reuse Ninja trees; ignored build/t90/s1 logs and owned fixtures only. Hidden supervisor: 30 minutes per build, 20 minutes per suite, 20 MiB per phase; kill only owned timed-out trees and report inconclusive. |
| Reporting Requirements | Estimate production net -10 to +15 lines, tests +120 to +240; report actual additions/deletions/net by component, unchanged mirror/shared roots, dual EXE links and separate desktop results. |
| Stop Conditions | Need for mirror/shared changes, competing worktree edits, incompatible semantics, unreserved desktop or failures outside this finite scope. |
| Exit Criteria | All five items implemented and audited, required builds/tests pass, EXEs published, complete P committed/pushed, stop for owner acceptance. |
| Original Owner Request | 准入一个独立的T任务，完成以上5项代码质量治理工作；完成后编译发布测试提交推送后等我验收。原生桌面测试也一起安排起来！ |
| Similar-Issue Sweep | Review all snapshot begin/result branches, Compat audio worker error exits, both App media parsers and save output lifecycle; retain existing safe-boundary and teardown owners. |

## Accepted T89 Baseline

S1--S5 are closed. Owner approved T89 closure after the final desktop
qualification, before T90 admission.
[Completion audit](../history/M9-T89-completion-audit.md).

S4 P1 refreshes both package EXEs: x64/x86 each pass 84 shared then 47 other
background tests (131/131 per width), including eight background App integration
cases. S3 also passed eight focused cases. S5 now passes all five native desktop
tests per width without retry: combined qualification is 136/136 per width.
Owner INI, disks and snapshots remain unchanged. The last accepted runnable
baseline is `d1583eda`; see the [S2 closure](../history/M9-T89-S2-existing-endpoint-cleanup.md)
and [S3 delivery/accounting](../history/M9-T89-S3-host-header-policy.md).
The [S4 audit](../history/M9-T89-S4-retained-port-audit.md) accounts for all
36 original directives and whole-T counts: production +48/-78 (net -30),
build/tests +88/-7 (net +81), original-mirror diff reduced by 58 lines.

## M9 T89 S5 Retained Delivery Brief

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 验收通过 收口 准入next s task. Close accepted S4 and admit only the outstanding whole-T qualification as S5. |
| Objective | Run the five existing native desktop cases per width, reconcile with accepted background evidence and prepare the final T89 closure decision. |
| Non-goals | No new functionality, production/test/build implementation, guest media, INI, cache repair or XP migration; no Computer Use and no automatic T closure. |
| Reference Baseline | d1583eda accepted S4; whole-T comparison 3c2e6e69; S4 ledger and artifact hashes remain authoritative. |
| Candidate Proposal | [Archived Mirror/Compat convergence](../history/M9-T89-mvdm-compat-boundary-convergence-proposal.md). |
| Files And ABI Surface | Existing CTest desktop entries, accepted binaries and task records. No ABI or source changes planned. |
| Applicable Rules | Task Reading Set, linked governance skills, minimum original-mirror diff, sole original state owner, inward dependencies and source research policy. |
| Verification | Confirm five desktop entries per width with CTest -N; then serial -L desktop --no-tests=error in a reserved period. Cover both package variants, Window retirement/modal and Console display. Reconcile with S4's 131 background passes per width, actual Git diff and artifact hashes. |
| Expected Markers | Ten actual desktop outcomes, no skipped cases counted as passes, unchanged accepted artifacts and complete finite T89 ledger. |
| Asset Needs | Reuse accepted dual EXEs and intermediates; no redundant rebuild solely for governance. Hidden owned supervisor under ignored build/t89/s5, captured logs/exit codes, 20 minutes per architecture and 20 MiB output; native test windows may appear. Preserve user INI/media. |
| Reporting Requirements | Expected production +0/-0, tests/build +0/-0; documentation and bounded evidence only. Report background versus desktop outcomes separately and retain whole-T counts. |
| Stop Conditions | No reserved desktop period, unavailable interactive desktop, test failure needing code changes, artifact drift or unrelated host interference. Do not waive assertions, retry away a failure or modify code without a scoped decision. |
| Exit Criteria | Dual desktop qualification complete, evidence and whole-T requirements reviewed, governance checks pass, complete P committed/pushed for owner closure decision. |
| Original Owner Request | 验收通过 收口 准入next s task |
| Similar-Issue Sweep | Verify the exact desktop-labelled test inventory on both widths and every remaining S4 qualification exclusion; no new test mechanism or implementation path. |

[S1's audit](../history/M9-T89-S1-mirror-compat-convergence.md) remains the
finite 36-directive/14-file ledger. S4 verifies its final dispositions.
The accepted S2 reduced original-mirror diff by 27 lines;
S3 actual production +39/-28, net +11; build/tests/checks +49/-1, net +48.
Its two original-header diffs fall from 70 to 41 added-plus-removed lines
against OpenNT, a reduction of 29. The linked S3 delivery owns its actual
results and evidence; S4 does not reopen that accepted implementation.

## Historical M9 T85 S20 delivery record

This retained delivery record is superseded for task state by the
[T85 completion audit](../history/M9-T85-completion-audit.md).  In particular,
the earlier P2/P3 handoff wording below is historical; P4 is accepted and T85
is closed.

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner reopens S20 after acceptance found that P2 removed the startup monitor preamble and its ERROR guard blocked debugger entry; after P3, owner reports duplicate `Machine paused.` output after successful snapshot load and approves the narrowed Product-monitor correction. |
| Objective | Restore the generic Product Surface preamble, narrow ERROR command admission so debugger entry remains available, and ensure the PAUSED runtime fact after a synchronous snapshot load does not duplicate the App-owned load result. |
| Non-goals | Do not change NXVM directly; do not add an App-specific banner, parser, executor, Session state machine, Machine event-cause field or compatibility shim; do not change Core, Compat, user INI, guest media or unrelated worktree changes; do not claim manual product acceptance. |
| Reference Baseline | SoftPC `8cbe40da`; NXVM remains read-only comparison material. |
| Candidate Proposal | [Product/Emulator component reorganization](../history/M9-T85-product-emulator-component-reorganization.md#s20-p4--prevent-duplicate-paused-completion-after-snapshot-load) |
| Files And ABI Surface | `src/product/surface/entry.c`, `src/emulator/product/monitor.c`, their manifests, and the focused Product/Emulator tests. The existing Product name input remains generic; no new ABI, App configuration or Machine event field is added. |
| Applicable Rules | Task Reading Set; Execution, Architecture, Coding and Documentation Rules; exact shared-corpus boundary; C11 six-package rule; user configuration/media preservation. |
| Verification | Run Product/Emulator focused tests on x64/x86, corpus/manifest/dependency gates, then proportional dual-width package builds and background CTest excluding desktop. |
| Expected Markers | Opening output is `<name>\n\nBuilt on <date> <time>\n\n`; ERROR rejects only `start`/`stop`/`pause`/`resume`/`reset`/`save`/`load` while dispatching debug and every App extension; only `RUNNING -> PAUSED` creates the generic pause text/prompt; dependency direction remains `lib < emulator < product`; package configuration/media are untouched. |
| Asset Needs | Refresh package EXEs because production sources change; never modify `assets/binary/softpc.ini`, media or snapshots. |
| Reporting Requirements | Pre/post path and line accounting by source/test root; focused behavior proof, dual-width evidence, refreshed EXE links/hashes, and one complete pushed P followed by owner validation. |
| Stop Conditions | A required App-specific API, Core/Compat change, broken manifest/dependency boundary, or failing dual-width verification requiring behavior design beyond the admitted two contracts. |
| Exit Criteria | The three failed contracts have focused proofs; task-owned changes are committed and pushed; worktree is clean except preserved owner changes; await owner validation. |
| Original Owner Request | 同意，请你重开本S任务予以修复。 |
| Similar-Issue Sweep | Inspect all monitor ERROR routes and Product startup-output owners; record each fixed-command/extension disposition and ensure no other imported path silently removes the monitor opening. |

### S20 planned delivery

The cache route stays product-external: it is a compiler launcher selected
only by its named optional presets, with cache output held in the
developer-selected `CCACHE_DIR` during measurement and the local ccache
default thereafter. Regular Ninja and Makefiles retain their existing,
dependency-free invocation and output behavior.

### S20 P2 delivery awaiting owner validation

The six shared roots and `test/register.cmake` were first copied byte-for-byte
from NXVM `9f966695e` plus its uncommitted Emulator Product cleanup.  The
admitted follow-up makes one deliberate shared-corpus delta: Surface owns a
default parsed SAVE/LOAD handler that reports `Feature not implemented.`, while
an App can register one callback for its existing snapshot implementation.
SoftPC registers its existing state-I/O route; it no longer reparses these two
commands.  The callback uses the existing Machine executor rendezvous and adds
no request queue, Session state or asynchronous completion protocol.

The corresponding canonical paths for NXVM adoption are
`src/product/surface/{command.c,command_interface.h,command_provider.c}`,
`src/emulator/product/monitor.c`,
`test/product/surface/command_smoke.c`, and their manifest records.  Local
SoftPC adaptation is confined to the App Product extension/composition and its
integration proof.  Dual Release package builds and background CTest pass
129/129 on each architecture; package INI, media and snapshots are untouched.

### S20 P3 corrective delivery

Owner acceptance found that P2 incorrectly dropped the existing generic
Surface opening, and review found its ERROR guard also blocked debugger entry.
P3 is retained with its corrective evidence; P2 is not accepted or closed.

### S20 P4 accepted closure

P4 preserves Machine's one `PAUSED` runtime fact after a successful state
write.  Product Monitor alone distinguishes an ordinary `RUNNING -> PAUSED`
completion from `INIT/STOPPED -> PAUSED` restoration, so the App-owned
`Machine loaded and paused.` result remains the only load completion line and
the queued runtime fact does not arm a second prompt.  P4 adds only focused
Emulator monitor coverage and manifest updates; it does not alter Machine,
Session, App, snapshot format or any public interface.

Focused Emulator monitor, source/test manifest, corpus and boundary checks
pass 5/5 on x64 and x86.  Both Ninja Release package targets rebuild the
published EXEs: x86 `F859627096569874C1AAD4DD713B2C4E1024F22F41E3EB9EC344F74C1A980F7C`,
x64 `E7FFCA2BF0DB84B0C89E3857F94F756E73EBC80FE26AAD543A482D175D411369`.
Production is +1/-2 (net -1); focused test code is +6/-0, with two manifest
hash replacements.  Owner accepts the package behavior; complete executor
delivery is pushed as `d3bd20f9`.  The actual-change review finds one
Product-monitor presentation rule, its focused regression proof, two manifest
updates and the refreshed package EXEs; Machine, Session, App, Core, Compat,
INI and snapshot format remain unchanged.
[S20 closure and queue record](../history/M9-Td-S20-s20-closure-and-display-queue.md).

### S20 delivery awaiting owner validation

The optional `ninja-ccache-*` configure, build and background-test presets
use only `CMAKE_C_COMPILER_LAUNCHER=ccache`; normal Ninja and Makefiles do not
mention ccache. A dedicated disposable cache measured near-cold full builds at
475.05 seconds on x64 and 219.93 seconds on x86 (the initial probe left 15 of
430 cacheable calls already present). After cleaning build outputs while
retaining that cache, the same full rebuilds took 58.46 and 38.04 seconds:
about 88% and 83% faster. The formal presets subsequently built at 33.18 and
36.15 seconds with 71.91% cumulative cache hits.

The cache does not cache test results: dual background CTest passed 129/129,
at 146.48 seconds on x64 and 138.44 seconds on x86, excluding desktop tests.
The 0.28 GiB measurement cache and all S20 build trees are disposable and are
removed after recording this evidence. No package EXE is refreshed because
source and link inputs are unchanged. P is pending; T85 remains open.

M9 T85 S19 is closed by the owner-directed admission of this next S; its
delivered rename evidence is retained in the
[S19 closure](../history/M9-T85-S19-emulator-rename-closure.md).

### S14 executor delivery

The shared `ibmpc/product` now owns the sole SoftPC command, hotkey and Common
composition path.  The App supplies only `app_composed_machine` and
`app_composed_ui`; it alone parses `softpc.ini`, constructs the private
machine and registers its snapshot/floppy extension.  There is no retained
SoftPC command/keyboard/composition implementation.  The six neutral shared
roots are byte-identical to current NXVM; the IBM PC Product receivers are the
admitted SoftPC adaptation, with no NXVM runtime dependency.

P2 restores the missed monitor preamble without expanding the Product ABI:
the App-provided identity is followed by the shared fixed `Built on <date>
<time>` line and the two required blank-line separators before the help menu.
It changes one Product source path plus its manifest record; focused x64/x86
Product/boundary tests and both package builds pass. P2 is awaiting owner
validation with S14; it does not close T85.

P3 restores the prior initial snapshot-load command contract in the
App-owned extension: Common `INIT` and `STOPPED` both admit `load`; successful
loading still reaches the existing paused completion.  This corrects the
extension's direct Common-state interpretation only and adds its focused
provider proof; it does not alter Common, Lib, shared Product or any ABI. Both
Release packages and the focused x64/x86 Product/boundary set pass; P3 awaits
owner validation with S14.

Both Release packages build.  Background CTest excluding native desktop tests
passes x64 476/476 and x86 476/476 after the x86 `ibmpc.command` smoke declares
its actual Console entry point.  Corpus, manifest, Product, dependency and
documentation gates pass.  P is pending; owner validation, and only then any
closure decision, remains outstanding.

## S12 delivery baseline

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly directs: “直接原样导入八组件吧，完成以后编译测试提交推送后等我验证，跑全量测试。” NXVM remains read-only. |
| Objective | Exactly import all eight shared source/test packages from clean NXVM HEAD `a397b925c48b629908869d295dfd97316acb6b8f`; qualify test/lib, test/emulator, test/x86, test/ibmpc independently on both widths. |
| Non-goals | No original SoftPC CPU/device replacement, App/Core or mirror change, Lib/Common private fork, guest media/config/snapshot edit, upstream write, desktop testing or invented manual acceptance. |
| Reference Baseline | SoftPC `4c36853c`; prior exact corpus `43e9700e`; NXVM clean HEAD `a397b925c48b629908869d295dfd97316acb6b8f`, verified with no uncommitted changes before copying. |
| Candidate Proposal | [S12 synchronization](../proposals/m9-windows-101-startup-repair.md#s12-current-nxvm-corpus-refresh-and-four-suite-verification) |
| Files And ABI Surface | All files below src/{lib,common,x86,ibmpc}, test/{lib,common,x86,ibmpc}, plus shared test registration/boundary scripts required by those packages. Exact final path/count ledger is measured from the clean upstream tree. x86 CPU internals and IBM PC media-release semantics evolve; SoftPC runtime links neither shared Core nor IBM PC. |
| Applicable Rules | Task Reading Set and current Architecture/Coding/Execution/Document rules and their shared governance skills; source-research policy and exact shared corpus boundary. |
| Verification | Stable path/hash ledger confirms zero differences from frozen `a397b925c`; full dual Release package builds and background CTest pass 479/479 on x64 and x86. Labels: Lib 33, Common 14, x86 176 and IBM PC 176. Native desktop tests remain excluded by the presets. |
| Expected Markers | All eight roots exact to frozen upstream; four distinct suite results per width, no production fallback or weakened assertions; desktop excluded. |
| Asset Needs | Existing build trees and owned decoder/negative-test outputs. Preserve existing INI/media/snapshot; refresh the two package EXEs after successful builds. |
| Reporting Requirements | Pre/post changed-path and added/removed/net counts separately for production and tests, clean upstream revision and fingerprint, actual import review, four-suite and full regression results, EXE links, pushed P; wait for owner. |
| Stop Conditions | Upstream revision changes during copy, invalid manifest/license provenance, outward dependency, need for App/mirror integration or local shared shim, failed verification requiring an unapproved behavior change. |
| Exit Criteria | Exact import and qualification are complete. Record/push the complete P and independent actual-diff review, then wait for owner validation; no automatic T closure. |
| Original Owner Request | 没关系，没关系，你可以准入一个新的 S 任务，把 NXVM 的所有有差异的部分导入进来，然后再跑这个测试，四个测试。 |
| Similar-Issue Sweep | Every frozen shared path and changed CPU/control/test receiver; decode/address/stack/arithmetic negative gates, reset/NMI/shutdown tests and complete inward test ownership. No second SoftPC executor/device owner. |

The owner subsequently accepts the newer `assets/binary/snapshot` as an owned
delivery artifact. It replaces the older package snapshot without changing
source or configuration.

## S11 delivered baseline

All 843 paths match pinned NXVM 43e9700e exactly. Production +0/-0;
32 test/build units +1430/-122, net +1308, with five inward relocations.
Dual full Release builds pass; focused 28/28 plus PC timeline 1/1 each;
serial background x64 469/469 (460.53s), x86 469/469 (327.16s).
Five desktop tests per width are excluded. EXEs retain accepted hashes.
Owner INI edit ships unchanged; original snapshot remains untouched/excluded.
Implementation P1 b45b2180 is pushed. Independent actual-change review confirms
all 43 delivered units, the exact committed 843-path corpus and unchanged
source/API/EXEs; only the preexisting owner snapshot edit remains uncommitted.
T85 remains open; no S11/manual or earlier unreported acceptance is inferred.
[S11 delivery](../proposals/m9-windows-101-startup-repair.md#s11-verified-delivery).

S10 implementation f97ecbea and actual-change review a51bf19b are pushed.
All 48 delivered paths match its finite ledger: production +71/-91 (net -20),
test/build +106/-7511 (net -7405). Dual Release and focused 27/27 each pass;
background x64/x86 468/468 each pass, desktop excluded. All 234 retained
IBM PC test C/H paths are compiled on both widths. Existing EXEs, owner INI
and snapshot are unchanged by this document-only closure. Original excluded
owner asset edits remain; task-owned changes are committed.
[S10 closure](../history/M9-T85-S10-ibmpc-simplification-closure.md).

S8 replaces only whole-frame invalidation with the existing valid markers:
one production C path +2/-2, net zero; two existing tests +27/-0. Combined
code +29/-2, net +27, excluding manifests/documents/artifacts. Initial zero
allocation, generation/locking, complete-frame driver contract and first-frame
publication remain unchanged. The finite reader/driver sweep found no ungated
read or additional implementation. Standard x86/x64 EXEs are rebuilt; original
owner INI/snapshot remain unchanged and excluded. The intermittent native x86
shutdown-test timing is recorded, not hidden or repaired by this optimization.
Executor P1 9fed40d1 is pushed. Coordinator actual-change review confirms the
eleven delivered paths, exact code accounting and unchanged artifact hashes;
only original excluded owner INI/snapshot edits remain in the worktree.
Owner now authorizes direct S9 continuation without intermediate testing.
S8 delivery is complete, manual acceptance not inferred; T85 remains open.

S7 imports all 126 IBM PC production paths byte-identically and owns 269 test
paths. All 51 prior PC exclusions are dispositioned: 45 reachable paths return,
six unused helpers remain omitted; 38 unique PC cases return. The retained
suite has no cross-test-package source dependency; duplicate protected IRET
remains owned once by x86. Code/build +64,583/-10, net +64,573, principally
imported source/tests. Existing Lib/Common/x86/App/Core production is unchanged;
IBM PC and the shared x86 Core are absent from SoftPC's EXE link inputs.
The [S7 ledger](../etc/evidence/m9-t85-s7-ibmpc-import-ledger.md) records exact
scope, local adaptation, failed/interrupted attempts and final qualification.
Executor P1 `8a967acf` is pushed. Coordinator actual-change review confirms
the 413 delivered paths match the packet and exclusion ledger, all 126 source
paths remain exact, no existing production/API/runtime link changes, and the
dual regression/architecture results recorded above. Owned scratch is removed;
only the two preexisting owner INI/snapshot changes remain. S7 awaits owner
testing and T85 stays open; delivery is not an S/T closure.

S6 P1 376f9278 and P2 45a461a2 are delivered: dual Release/background
282/282 each; isolated Lib/Common/x86 48/48, 20/20, 167/167.
[Delivery handoff](../history/M9-T85-S6-shared-boundary-delivery.md).
New S7 admission does not fabricate a manual-test acceptance or close S6/T85.

S5 remains verified awaiting owner acceptance; admitting S6 does not infer
an unreported manual-test result or undo its delivery.
Actual S5 code: stream.c +19/-10 (net +9), existing Audio smoke +42/-1
(net +41); combined +61/-11 (net +50). Two component README changes add
10 lines; three manifest hashes are refreshed. No Common runtime, x86,
App/Core, public ABI or mirror change. Dual EXEs are refreshed; owner INI and
snapshot edits are preserved outside this delivery. See proposal S5 evidence.
Executor P1 `23990ab2` is pushed. Coordinator actual-change review confirms
the eleven delivered paths match the packet, no prohibited boundary change,
and unchanged EXE hashes after verification. S5 awaits owner testing; not closed.

M9 T84 S8 is owner-accepted and closed. It converges raw Console backing-store
preparation with exact frame-owned output coverage: native viewport, font fit
and scroll position remain host-owned; steady 25-row output writes 25 rows;
former tails remain covered across partial writes and are clamped after an
external backing-store shrink. Both Release packages, focused Console tests
and hidden-background CTest pass 121/121 on x64 and x86. Executor P1
`b8c1ed3f` and P2 `b79769c1` are pushed. T84 remains open for later owner
direction. [S8 closure](../history/M9-T84-S8-console-convergence-closure.md).

M9 T84 S9 is owner-accepted and closed. It restores source-local key balance
after a KVM producer loses host focus or freezes through one semantic reset and
the existing Common Session delivered-key ledger. P1 `3186afcd` delivers the
behavior; P2 `40da7d00` adds direct native-loss and rejected-reset coverage.
Serial hidden-background CTest passes 122/122 on x64 and x86. Package EXEs
remain the accepted P1 artifacts. [S9 closure](../history/M9-T84-S9-input-reset-closure.md).

M9 T84 S10 is owner-accepted and closed. It synchronizes complete `test/lib`
and `test/emulator` to read-only NXVM `0c71110b0`, including deletion of the
Common-private Window reset smoke in favor of canonical Lib-owner coverage.
P1 `bbdc1d68` passes exact 71-path equality and serial x64/x86 background
CTest 121/121. [S10 closure](../history/M9-T84-S10-shared-test-corpus-sync-closure.md).

M9 T84 S11 is owner-accepted and closed. It documents the direct checked-in
x64 package route and portable MSYS2 x64/x86 source-build routes, and removes
the x86 preset's private developer path. P1 `e238a470` passes fresh x64 package
build, real 32-bit MinGW configuration, marker sweep, whitespace and
documentation governance. No product source, INI, media, snapshot or package
EXE change is retained. [S11 closure](../history/M9-T84-S11-quick-start-closure.md).

M9 T84 S12 is owner-accepted and closed. It imports NXVM `572293efc`'s
source-local unmatched-release behavior into the canonical Lib corpus without
an API or state expansion. P1 `b8c0f5fb` passes exact `src/lib` (109-path) and
`test/lib` (51-path) equality, focused lifetime proof, Release builds and
serial background CTest 121/121 on x64 and x86. [S12 closure](../history/M9-T84-S12-nxvm-orphan-release-import.md).

M9 T84 is owner-accepted and closed. Its twelve accepted S deliveries cover
the bounded 80x50 KVM route, subsequent shared-corpus convergence, raw Console
output/input correctness and fresh-checkout usability. The task-level ledger
and retained boundary/deferral dispositions are in the
[T84 completion audit](../history/M9-T84-completion-audit.md).

## T85 historical delivery record

The following individual-S records retain their contemporary evidence.  Their
references to T85 remaining open predate the task-level owner closure recorded
in the [completion audit](../history/M9-T85-completion-audit.md).

M9 T85 S1 is owner-accepted and closed on 2026-09-28. P1 `fc74d65e`
repairs mono-painter publication; P2 `ee62ad01` repairs InPort scheduling and
snapshot ownership; P3 `9d102563` records delivery review. Both Release
packages and background suites pass 121/121 per width; cross-width snapshot
tests pass. The owner accepts the updated-driver guest installation after
the separately authorized, backed-up test-HDD replacement.

[S1 closure](../history/M9-T85-S1-windows-101-startup-repair.md) records the
whole-S path counts, acceptance, retained artifacts and verification limits.

T85 S3 is owner-accepted and closed on 2026-10-04. P1 cec8a3dc and review
P2 a6a777d1 are pushed. The 219-file
NXVM shared subset plus test/register.cmake is byte-identical; eight local x86
packaging files retain debug/xasm32-only scope. Both Release builds pass.
x86 background passes 121/121; x64 passes 120/121 initially, with the sole
documentation-record ordering failure corrected and its rerun passing 1/1.
No production change; EXEs retain accepted S2 hashes. T85 remains open.

[S3 closure](../history/M9-T85-S3-nxvm-shared-subset-sync.md).

T85 S4 is owner-accepted and closed on 2026-10-04. Observation P1
`1bc02185` and repair P2 `a841b5d3` are pushed. Original idle detection and
the existing executor event now park idle DOS execution. Headless idle CPU
falls from x64 97.27/x86 96.78 percent to x64 0.26 percent and x86 below
accounting resolution. Final background suites pass 121/121 per width;
the earlier intermittent x86 roundtrip failure and passing reruns remain
disclosed, not diagnosed by repetition. Production +20/-7, tests +56/-0:
combined code net +69. No shared-corpus or original-mirror change.
[S4 closure](../history/M9-T85-S4-dos-idle-cpu-repair.md) retains acceptance,
actual-change review and limits. Owner INI/snapshot edits remain preserved.
T85 stays open; S5 is subsequently admitted above.

M9 T85 S2 is owner-accepted and closed on 2026-09-28. P1 `8c15e893`
repairs indexed DIB ownership; P2 `f8fe3df8` records actual-change review.
Both Release builds and serial background suites pass 121/121 per width.
The owner accepts the mouse regression repair. T85 remains open; the next
Windows 1.01 color question is read-only investigation, not implementation.
[S2 closure](../history/M9-T85-S2-indexed-pointer-repair.md).

M9 T84 S6 is owner-accepted and closed. It closes the admitted product
host-boundary defects without changing Lib, Common, x86, the Core mirror, user
INI, snapshots or media: parallel output retains only an unconfirmed suffix on
short writes; parallel snapshot validation completes before allocation; audio
teardown propagates failure through the App/VM lifetime boundary; quoted INI
paths retain `#`/`;`; and the obsolete permanent VM trace path is deleted.
Focused injected cases, both Release packages and hidden-background CTest all
pass 121/121 on each width. The final x86 SHA-256 is
`566CD3F1922C6F0F6427DE4B307E3E974562ABB3478CC4C8DD6D05BBA6275D8F`; x64 is
`8885A1C99E98CEEB623A644ED50B2CDE87EB4314161F59D88E0B9F014B22DF1B`.
Executor `13b4aef3` is pushed. T84 remains open for later owner direction.

M9 T84 S5 is owner-accepted and closed: NXVM's complete six-component
shared corpus, including Audio, was imported from clean `057d8c9aa5`; the generic
test helper is now at the owner-approved shared `test/register.cmake` location.
All six roots and that helper are byte-identical to NXVM. The `lib_bool` ABI change
required a complete dual-width rebuild and six narrow SoftPC callback/test receiver
adjustments; it did not add a product behavior branch.
Both Release packages and both background CTest suites pass 120/120. Refreshed
x86 `F1E748E86FF800ACF4C1BE8FF75B675BC4995CD3FC39752B948D5D32620E8658`,
x64 `840347402647A6F4988DCC16A4B22E6E5FF853D5E15E7C78A26331DB74410856`.
Executor `e2b81d70` is pushed. T84 remains open for later owner direction.

Owner defers the pre-existing x86 BIOS[0x52] null-dispatch fault to
[TODO](TODO.md) on 2026-09-25; no CPU/BOP repair belongs to S1.
The 80x50 implementation builds on both widths; final background regression
passes 120/120 on x64 and x86. The deferred fault is not claimed repaired.
T84 S1 is owner-accepted and closed. Executor P1 `503d6632`, delivery
review P2 `dc9c34ce`, and S1 acceptance P3 `39366670` are pushed. Owner
accepts S2's document-only whole-task audit and admits S3 on 2026-09-25.
S3 is owner-accepted and closed. It imports NXVM shared commit `b7cbb30a9`'s
deterministic Audio adapter test exactly: test/build +220/-212 (net +8),
production +0/-0. Both Release packages build, focused Audio tests pass on
both widths, and background CTest passes x64 120/120 (250.98s) and x86 120/120
(249.05s). Six manifests, Lib DAG, Common/x86 corpus and documentation
governance pass. NXVM later acquired uncommitted shared-root edits; they are
not part of S3. [S3 record](../history/M9-T84-S3-nxvm-audio-test-import.md).
Refreshed packages: x86 `FEA84DEF15C2A25DC962579087D60F5DC94E11B9E46A792E13B3C3B60408D8A5`,
x64 `0EC6096600E1702CE92751F4AEE1BC16C5FD009FF4E6D2DF537A97774672148F`.
The established S1 evidence confirms 24 paths,
production +19/-11 and tests +108/-19, no mirror/INI/media change, matching
dual-EXE hashes and the seven-member coverage ledger. Both Release builds,
full background 120/120 and delivery rechecks 18/18 pass per width.
Five desktop tests per width remain excluded. Owner reports manual acceptance
on 2026-09-25. S2 performs no product-code change; no T84 closure is claimed.

## Current Technical Baseline

- T88 closes after owner acceptance of S1--S6 and explicit task closure.
  Mirror: 12 files +388/-1212 net -824; Compat: three files +46/-9 net +37.
  No shared-root or snapshot-format change. Final x64/x86 136/136 qualification
  and accepted EXE hashes are in [the audit](../history/M9-T88-completion-audit.md).

- Owner closes T83 after S1--S6 delivery and admits T84 for bounded 80x50
  text frames. [T83 completion audit](../history/M9-T83-completion-audit.md)
  preserves scope and verification; this admission changes documents only.

- T83 S6 is owner-accepted and closed. The selected C-VID glue ABI is declared
  once in `cpu_vid.h`; EGA dot-read is declared once in `egavideo.h`. Both
  Release builds and background CTest presets pass. [S6 audit](../history/M9-T83-S6-cvid-abi-closure-audit.md).

- The completed T83 App Types/Base convergence covers 109 tracked non-mirror App files; 54 direct Types consumers migrated and four mirror-shared Compat ABI bridge headers remain. Both Release builds and their full CTest runs pass. EXE hashes and the full ledger are in [the S4 audit](../history/M9-T83-S4-lib-types-base-closure-audit.md).


- T82 closes after owner acceptance of S2. The six shared directories have
  220 paths: 214 content paths are byte-identical to NXVM's current worktree;
  the only six differences are local `MANIFEST.sha256` revision/hash records.
  Base process discovery now owns executable-directory lookup and App appends
  the unchanged `softpc.ini` filename. S2 P1/P2 are `2ef228f8`/`0448fa4f`;
  x64 and x86 builds passed, each background suite passed 116/116 before the
  manifest-only P2, and the final dual-width manifest/corpus/negative checks
  passed. Package EXEs remain from P1; INI, media, Core, Compat and VM are
  unchanged. [T82 audit](../history/M9-T82-completion-audit.md) records the
  complete changed-path and difference ledger.

- S7 shares one internal write algorithm; ordinary write flushes on success,
  nonempty DIRECT fill flushes once even after partial failure, preserving the
  first error. No public signature, platform, cache or rollback change.
  Production +23/-7 (net +16; three added comment lines), tests +88/-0;
  combined +111/-7 (net +104). Strict shared C11 dual Release builds pass.
  Background x64 111/111 (214.52s), x86 111/111 (194.39s); standalone Lib
  41/41 per width (60.92/63.05s). Desktop excluded; no Linux runtime claim.
  Six manifests/DAG pass; task scratch removed, EXEs refreshed; INI/media and
  Common/x86/Core unchanged. [S7 evidence](../history/M9-T80-S7-storage-fill.md)
  records actual-change review of pushed executor `75dfd5a6`;
  S7 is owner-accepted and closed with T80.

- S6 moves 512 fixed disassembler handlers to file-local const tables; parsing
  context remains per-call. Production +520/-521 (net -1), tests +42/-0;
  total code +562/-521 (net +41). No handler/output/public API change.
  Strict C11 Release builds and background x64 111/111 (177.48s), x86 111/111
  (169.15s) pass; standalone x86 suites 10/10 per width. Five desktop tests
  excluded per product width; no Linux run. Temporary task outputs removed.
  EXEs refreshed, INI/media and Lib/Common/Core unchanged. Owner closes S6;
  Executor `5d0515fd` pushed;
  [S6 evidence](../history/M9-T80-S6-xasm32-dispatch.md) records actual review.

- S5 removes Common's stale standard-header allowlist and rejects raw integer
  limits. Seven Machine constants use equivalent existing Types aliases.
  Runtime +7/-7, checker +4/-4, tests +24/-0: total net +24, no ABI change.
  Both Release/background builds pass: x64 111/111 (273.67s), x86 111/111
  (178.46s). Copied four-directory tests 59/59 and six-directory tests 69/69
  pass on both widths, strict C11; all 180/205 copied files remain identical.
  Desktop tests excluded; no Linux runtime claim. Isolated task outputs removed;
  package EXEs refreshed; Lib/Core/x86, INI/media unchanged. S5 accepted for continuation.
  Executor `c7c42ea5` pushed; actual-change review retained in
  [S5 evidence](../history/M9-T80-S5-emulator-types-gate.md).

- S4: frame publication skips 0 on u32 wrap; Session's two ordering sites share
  one private modular comparator. Production +16/-4 (net +12), tests +69/-0;
  public widths and UI equality deduplication unchanged. Compared serials must
  be less than 2^31 apart. No new state, queue or cache; Lib/Core/x86 unchanged.
  Both Release builds and focused 3/3 pass per width. Background x64 111/111
  (173.24s), x86 111/111 (160.81s); five desktop tests excluded per width,
  no Linux execution. Manifests/DAG and final documentation gate pass.
  Standard EXEs refreshed; hashes in the proposal, INI/media unchanged.
  Executor `478d04ec` pushed; actual-change review recorded in
  [S4 evidence](../history/M9-T80-S4-frame-sequence-wrap.md). Owner accepts S4 closure.

- S3: production +6/-4 net +2, tests +56/-0; reject an unrecordable new key
  before machine delivery, retaining original repeat/identity/retirement logic.
  Both Release builds and background suites pass: x64 111/111 (141.46 s),
  x86 111/111 (140.30 s). Five desktop tests excluded per width; no Linux run.
  Package EXEs refreshed; hashes and finite sweep are in the proposal.
  Executor `aea41c16`, review `2cd9e889`; owner accepts and closes S3.

- S2 audit baseline `1b5d0fd7`: production/tests +0/-0, existing EXEs unchanged.
  Existing Common suites pass 18/18 per width; three focused cases pass twenty
  repetitions each per width. No rebuild, new fault injection, full-product,
  desktop or Linux run. The owner accepts the documented native-failure limit;
  no new fatal callback, queue or synchronization recovery mechanism is added.

- T80 S1 is owner-accepted and closed: production +11/-2 (net +9), tests/build
  +149/-0; public API and debugger callers unchanged. Executor `73a5a889`,
  review `f4941481`; strict C11 isolated x86 10/10 and background 111/111 on
  each width. Five desktop cases excluded each; no Linux runtime claim.
  [S1 review](../history/M9-T80-S1-xasm32-boundary.md) and proposal retain hashes.

- T79 S2 implements independent GFI physical identity and media lifetime.
  Production +56/-23 (net +33), tests/scripts +187/-37 (net +150); three
  production files, no shared-corpus or Core mirror changes. Both Release
  packages, focused 7/7 per width, background x64 110/110 (178.87s) and x86
  110/110 (163.39s) pass; five desktop cases excluded. Empty/present snapshots
  pass both cross-width routes. The device stream adds eight identity bytes;
  old-format snapshots are not accepted. Owner reports testing passed and
  approves S2/T79 closure. Executor `cba189b5`, review `b9413d65`; unchanged
  artifact hashes and coverage are in [the closure audit](../history/M9-T79-completion-audit.md).
- T78 S1 commit `0fb40f48` removes Common's direct integer-limit import and
  aligns the unsigned run-generation storage with its public `lib_u32`
  contract. Production/test C/H +52/-17 (net +35); no behavior or public ABI
  change. Lib/Common 44/44 and background regression 109/109 plus the direct
  long case pass on x64/x86. Owner manually accepted the package EXEs; closure
  evidence is in [the audit](../history/M9-T78-completion-audit.md).
- T77 S3 executor 7a6ac879 completes final acceptance. Product tests now have
  App/Core/Integration/checks ownership and one Integration machine fixture.
  Whole-task code/build +229/-2129 (net -1900); production/shared corpora unchanged.
  Final Release/background x64 110/110 (155.50s), x86 110/110 (151.26s).
  Isolated Lib 41/41, Common 18/18 and x86 9/9 pass on both widths;
  180/204 copied files identical. Both EXEs retain T76 hashes. Five desktop
  cases excluded per width; no new Linux or downstream integration claim.
- T77 S2 executor 217ec7a5 removes obsolete test wrappers/diagnostics and keeps
  one Integration fixture; code/build +181/-2096 net -1915. Release and background
  x64/x86 110/110 pass; production/shared corpora and EXE hashes unchanged.
- T77 S1 executor be23165f relocated 33 unchanged test blobs (8373 lines).
  All 115 CTest definitions identical; both Release/background 110/110 pass.
  Code/build +50/-35 net +15; production/shared corpora unchanged.
- T76 uses one direct page-pointer array for Storage overlay lookup; no public
  API or snapshot format change. Production +39/-30 (net +9); tests +89/-2
  (net +87). No benchmark per owner direction; array memory scales with capacity.
  Final Release/background x64 110/110 (146.29s), x86 110/110 (146.52s), five
  desktop cases excluded per width. Old/new x86/x64 snapshot matrix 16/16 passes;
  golden media payload matches old linked-list codec on both widths. Artifacts
  and hashes are recorded in the archived T76 proposal; INI/media unchanged.
- T75 uses the existing task cancellation object for the outer Machine wait;
  deterministic lost-command-wake coverage and repeated native shutdown pass.
  All six shared packages select strict C11 in standalone and embedded builds.
  Public API, App/Core, INI and media remain unchanged. Endpoint source/test/build
  +126/-79 (net +47), of which production C/H is +34/-33 (net +1).
- T75 S4 verifies sixteen independent four/six-package builds on both widths;
  each width passes isolated Lib 41/41, Common 18/18, and optional x86 9/9.
  All 180/204 copied files remain identical. Final product background x64
  110/110 (127.19s), x86 110/110 (182.23s); both Release EXEs rebuilt and match
  S3 hashes. Desktop cases excluded; no Linux runtime or NNES integration claim.
- T74 S1 moves 556 files into src/core/{machine,compat,softpc.new}.
  498 mirror blobs are identical; 107 product/test C/H/RC files pass path-only
  comparison. Runtime symbols and behavior remain unchanged. Both Release
  builds and background suites pass: x64 110/110 (165.45s), x86 110/110
  (157.47s); five desktop cases excluded per width. Shared corpora, INI and
  guest media are untouched. [T74 proposal](../history/M9-T74-core-layout-rename-proposal.md)
  holds the +421/-409 (net +12) source/test/build/tool ledger and EXE hashes.
- Accepted T73 implementation 879c30ac, actual-change review ad665a86.
  Owner manual S5 testing passed. Closure changes documents only.
- Shared source corpora are src/lib, src/emulator, src/product; matching suites
  are test/lib, test/emulator, test/product. The six-directory set is ordered
  `lib < common < product`; each outer component consumes only its inward
  public contracts.
- Common contains machine/session/ui only. Machine retains one executor and
  copied opaque 128/1536-byte debug transport with its existing paused lease.
  Product Debug owns CPU protocol and DOS/X CLI; Product xasm32 owns
  assembly/disassembly. VM validates product debug requests; App explicitly
  connects Product capabilities.
- Lib and test/lib are unchanged by T73. Host-PC key identity and existing KVM
  capacities are retained; receiving adapters own guest input mapping.
- Final background x64 110/110 (181.72s), x86 110/110 (165.87s); both Release
  builds passed. Five desktop cases per width were excluded. Native Common
  tests each passed 50 repetitions per width. Owner testing does not imply
  those excluded automated cases or Linux/NEC integration were exercised.
- Isolated neutral copy: Lib 41/41, Common 18/18. Isolated six-directory copy:
  Lib 41/41, Common 18/18, x86 9/9. No product resources/build are required.
- T73 endpoint C/H production +487/-428 (net +59), tests +1104/-819 (net +285);
  complete ledger and artifact hashes are in the completion audit. S5 runtime
  code is unchanged; its EXEs remain byte-identical to accepted S4.
- VM owns character mapping and device-attribute decoding. KVM uses four-byte
  text cells; Window owns fonts and Console owns character maps. Base transports
  opaque control FIFO and latest-wins frames. Window compares decoded pixels
  against its own surface; native relative/absolute mouse records remain.
- Common Session/UI, Compat, MVDM, snapshot format, user INI and media were not
  changed by T73. T75 S2 subsequently repairs the debug-close/paused-destroy
  wake race; its bounded schedule proof does not certify all interleavings.
- TODO remains empty under the owner's tracking policy, not proof of universal
  correctness. Queue candidates are unadmitted.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T92 | Owner accepted and closed 2026-10-15; S1-S6 complete, dual-width 150/150 including all desktop tests. | [Completion audit](../history/M9-T92-completion-audit.md) |
| T91 | Owner accepted and closed 2026-10-15; all S1-S5 obligations complete. | [Completion audit](../history/M9-T91-completion-audit.md) |
| T90 | S1 owner-accepted and closed; five non-mirror quality repairs, dual-width 137/137 including native desktop. | [Audit](../history/M9-T90-S1-nonmirror-quality-cleanup.md#t90-completion-audit-and-owner-acceptance) |
| T89 | S1--S5 owner-accepted and closed; mirror/Compat convergence, dual-width 136/136 qualification. | [Audit](../history/M9-T89-completion-audit.md) |
| T88 | S1--S6 complete; owner accepts final mirror/Compat video repairs and approves closure; x64/x86 136/136. | [Audit](../history/M9-T88-completion-audit.md) |
| T87 | S1 complete; ordinary repository text converges on LF while the preserved OpenNT mirror and user-owned INI retain explicit CRLF checkout policy. | [Audit](../history/M9-T87-repository-eol-convergence.md) |
| T86 | S1--S6 complete; owner accepts final six-root convergence and approves task closure. | [Audit](../history/M9-T86-completion-audit.md) |
| T85 | S1--S20 complete; owner approves task closure. | [Audit](../history/M9-T85-completion-audit.md) |
| T84 | S1--S12 complete; owner approves task closure. | [Audit](../history/M9-T84-completion-audit.md) |
| T83 | S1--S6 complete; owner approves task closure. | [Audit](../history/M9-T83-completion-audit.md) |
| T82 | S1/S2 complete; owner accepts the six-component convergence. | [Audit](../history/M9-T82-completion-audit.md) |
| T81 | S1--S8 complete; owner accepted cold first-run `AUDIO.COM` playback; neutral Audio and the PC-speaker handoff close. | [Audit](../history/M9-T81-completion-audit.md) |
| T80 | S1--S7 complete under approved scopes; owner acceptance and separate whole-task audit close the task. | [Audit](../history/M9-T80-completion-audit.md) |
| T79 | S1 investigation and S2 repair complete; owner testing passed and closure approved. | [Audit](../history/M9-T79-completion-audit.md) |
| T78 | S1 complete; owner manual test accepted; Common type boundary closure. | [Audit](../history/M9-T78-completion-audit.md) |
| T77 | S1--S3 complete; owner-authorized self-review closure; test ownership cleanup, isolated packages and dual-width acceptance. | [Audit](../history/M9-T77-completion-audit.md) |
| T76 | Owner testing passed; S1 measurement cancelled by owner, S2/S3 complete; overlay index and snapshot compatibility accepted. | [Audit](../history/M9-T76-completion-audit.md) |
| T75 | S1--S4 complete; owner approved closure; strict C11 six-package and dual-width acceptance. | [Audit](../history/M9-T75-completion-audit.md) |
| T74 | S1 complete; owner accepted; dual-width 110/110 background; pure Core relocation. | [Audit](../history/M9-T74-completion-audit.md) |
| T73 | S1--S5 complete; owner accepted; final dual-width 110/110 background. | [Audit](../history/M9-T73-completion-audit.md) |
| T72 | S1--S8 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T72-completion-audit.md) |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |

## Recent Governance

- **M9 Td S21:** T87 S1 closes after the independent actual-change review:
  61 ordinary working-tree text paths converge to LF, `.gitattributes` makes
  LF the default, and only the preserved OpenNT mirror plus user-owned package
  INI retain CRLF checkout policy. The File Manager diagnostic returns to queue
  head. [T87 audit](../history/M9-T87-repository-eol-convergence.md).

- **M9 Td S20:** Owner accepts T85 S20 P4; its Product-monitor completion
  repair is closed without closing T85. Two independent display candidates are
  placed at the queue head: raw-to-cooked Console handoff, then Win3.1 File
  Manager VGA width transition.

- Owner reports acceptance and approves T80 closure. S7 closes, the proposal
  is archived, and no next task is admitted. This closure changes documents
  only; accepted EXEs, configuration and media remain unchanged.

- Owner closes T80 S6, admits S7 and approves its narrowed one-write-algorithm
  design. S7 delivered for acceptance; T80 not closed automatically.

- Owner admits T80 S6 after S5 delivery. S6 delivered for testing; S7 remains
  unadmitted. No separate S5 manual-test result is inferred.

- Owner approves T80 S4 closure and admits S5's Common Types boundary repair.
  T80 remains open; S6/S7 are not started.

- Owner closes T80 S3 and admits S4. Document-only handoff preserves tested
  package EXEs; T80 remains open.

- Owner approves T80 S2 closure after the narrowed audit, then admits S3.
  Document-only handoff preserves the accepted binaries; T80 stays open.

- Owner closes T80 S1 and admits S2's notification-failure design audit.
  Handoff is document-only; accepted EXEs unchanged; T80 remains open.

- T79 closes after owner manual acceptance; its proposal is archived. Queue
  head is admitted as T80 S1; seven-step plan retained, XP rebase stays queued.
  This handoff changes documentation only; accepted EXEs are unchanged.

- T78 closure archives its proposal after owner manual acceptance. No active
  packet remains; queue candidates retain their original order until separately
  admitted.
- T77 closure archives its proposal and accounts for all 48 original product
  files and 115 CTest definitions. Task-owned disposable outputs removed;
  TODO remains empty and the two unrelated queue candidates remain unadmitted.
- T76 closure archives its proposal and verifies scope, hashes and full-task
  coverage. Owner admits queue head as T77 S1; this handoff changes docs only.
- T74 closure archives the proposal and records S1/T-level requirement coverage,
  actual-change review and owner acceptance. Sources and tested EXEs unchanged.
- T73 closure records S5 acceptance, all-S requirement/changed-path audit and
  the separate queued wake-race receiver, and archives its proposal. No new
  implementation task or T number is allocated.
- M9 Td S18: T71 closure, TODO retirement and floppy-identification candidate;
  [record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
- M9 Td S19: refreshes the unadmitted XP candidate so XP SP1 is its intended
  routine mirror baseline, while OpenNT remains lineage evidence;
  [record](../history/M9-Td-S19-xp-baseline-proposal-refresh.md).
