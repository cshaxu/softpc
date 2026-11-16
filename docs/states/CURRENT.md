# Project Status

## Current Work

Owner admits M9 T100 S5: ordered Compat simplification audit. S1-S4 remain
closed; T100 remains open. Baseline2464862d. Start with evidence and concrete
removal/merge candidates; preserve behavior and existing ownership boundaries.
[Progress](../etc/evidence/softpc/m9-t100-s5-compat-audit.md).
S5 P3 implements file1/50 audio.c: production +5/-29,net-24; tests
+15/-17,net-2. Cached dual builds/publication and392/392 full tests passed,
including native desktop/audio/package. Owner accepted P3 and requested the next
file. P5 corrects audio.h ownership wording and removes platform.c's unused
include: production +2/-3,net-1; no test/mirror/API changes. Cached dual full
builds/publication and392/392 tests passed; self-review passed. Mirror-header
warnings only. Owner accepts continued execution. P6 delivered086a8cc2 and P7 completes
all50 frozen Compat reviews and safe simplifications.49 files remain:8957 ->
8851 lines; whole-S production +43/-149,net-106, tests +89/-86,net+3; one
source file removed and one build-list line removed. No mirror/shared/schema
or user-asset changes. P6 and P7 each pass cached dual builds/publication and
392/392 full tests; final builds have zero non-mirror warnings. Actual-diff
self-review and inventory reconciliation pass. Scratch cleaned; S5 is ready
for owner acceptance, T100 stays open. Detailed proof and retained boundaries
are in the linked evidence. P7 uses the original BIOS table in tests, row-copy
V7 restore, dominated private archive-check removal and unified queue traversal.


The following admissions are retained historical records, not active work.

Owner additionally admits the standalone clock lifecycle repair within S4:
"好，你现在帮我把这个最好的修复做进去啊，架构一定要一致统一啊，不要有第二路径，不要分叉，要在正确的点位呢统一处理。然后千万不要 bloated。"
Scope: Compat timer pause/resume ordering, snapshot completion without restart,
restore-time host notification isolation, existing failure/readiness contracts
and focused regressions. No mirror/shared-component/schema/guest changes or
quick-event multiplier. Use main worktree and build/t100-s4-clock only (two-hour
job and fifteen-minute no-progress budgets). Preserve all original assets.
Qualify cached dual builds, focused/full tests and native-audio guest playback;
the original failing snapshot is a separate mandatory check, not replaced by
a newly generated snapshot. Publish/commit/push only a qualified delivery;
Owner subsequently reports that the new snapshot works normally. The original
input was checked unsuccessfully before owner replacement; no old-snapshot
recovery claim is made. S4's ordered mirror audit remains open and its frontier
is unchanged.

Owner approves S4 P84 DSP boundary correctness cleanup (baseline 44c8cdac):
restore DSP_MODE to nt_sbdsp.c; remove internal helper declarations from nt_sb.h;
migrate device tests to DSP commands, preserving staged PCM/DMA/error/completion
and snapshot coverage. No schema, shared-package or guest changes. Scratch:
build/t100-s4-p84, this packet owns cleanup; two-hour job / fifteen-minute
no-progress budgets. Cached dual build, publication, full background/native
regression, actual-diff review, commit/push then owner acceptance. Similar-issue
sweep covers all callers of the five withdrawn header definitions.

Owner approves S4 P85 completing the analogous DSP/DMA boundary cleanup:
remove eleven header declarations and relocate the private DMA record; reuse
original local forwards and existing DSP commands/service plus DMA observation
in the existing fixture. No private-type copy, new production test API, schema,
behavior or guest change. Baseline cb89ef52. Own scratch build/t100-s4-p85,
two-hour job/fifteen-minute no-progress budget. Full cached dual build,
publication, regression, review and commit/push, then await acceptance.

Owner admits S4 P86 reviewing order457 host/src/nt_cga.c against fixed XP:
prove each retained nonexcluded difference, restore unjustified differences,
then cached dual build/publication/full regression, commit/push/self-review.
Baseline 76b10b1e; own scratch build/t100-s4-p86, two-hour job and fifteen-minute
no-progress budget. Preserve guest/media/configuration; do not advance beyond
this file or change unrelated implementations. Await acceptance after delivery.

Owner approves P86's nt_cga.c code change; manual acceptance is deferred.
Owner approves nt_ega.c R0595-R0600 retention. P87 admits a mirror-only sweep
of added duplicate includes against fixed XP; original duplicates and all
non-mirror source remain untouched. Baseline 1581635d. Prove redundancy and
actual compiler equivalence before restoring; no transitive-include cleanup
outside that boundary. Own build/t100-s4-p87, two-hour/fifteen-minute no-progress
budgets. Cached dual build/publication, full regression, actual diff review,
commit/push, await owner acceptance; do not approve unrelated later-file diffs.

Owner approves S4 P89 implementing P88's five exact restorations in nt_sbdsp.c:
two original-position forwards and three original USHORT spellings. Baseline
afe8100b. No DSP/DMA behavior, public API, snapshot schema, guest or other
source change. Own build/t100-s4-p89, two-hour job/fifteen-minute no-progress
budget and cleanup. Cached dual full builds/publication, complete background
and native regression, actual-object equivalence, self-review, commit/push,
then owner acceptance. Preserve user configuration/media and unrelated work.

Owner approves P92 restoring proven nonsemantic nt_sbmid.c differences and
rechecking every remaining diff. Baseline 11baea7f; own build/t100-s4-p92,
two-hour/fifteen-minute no-progress budgets. Compile all affected x86/x64
production/test object configurations; identical outputs require no tests or
publication. Different outputs require full qualification/publication before
delivery. Preserve all other sources, original helpers, assets and exclusions.

Owner admits P97 auditing all 34 deferred generated C-VID files (sevid000-030,
sinit011-013) for unsupported retained differences. Read-only source work;
classify individual current differences against fixed XP and existing evidence,
without treating prior broad passing tests as per-point proof. Own disposable
build/t100-s4-p97; two-hour/fifteen-minute no-progress budgets. No production
restoration, publication or sibling writes in this investigation. Promote the
finite evidence-gap ledger and remove scratch before delivery. P97 source audit
finds eight unwired entries (145 blocks +228/-228); see the
[bounded evidence report](../etc/evidence/softpc/m9-t100-s4-p97-cvidc-evidence.md).
Owner subsequently approves uniform ABI retention and S4 closure. The P97
restoration candidate is withdrawn; no production change was made.

## M9 T100 S5 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits a new S auditing Compat in fixed dir order to reduce code/file/line count. P3 owner approves the audio.c candidate and its existing regression adaptations; P5 approves audio.h ownership comment and unused platform.c include removal; Owner now authorizes continuous ordered audit and evidence-backed behavior-preserving simplification across all Compat, with batched complete deliveries; do not stop for per-file approval. |
| Objective | Audit every frozen Compat file for unnecessary code, duplicated owners/paths, thin wrappers and coherent file consolidation; favor fewer concepts and actual repository-wide reduction. |
| Non-goals | No guest/configuration/media changes; no mirror diff growth, shared-package changes, new framework, alias, polling or second path merely to shrink Compat. No performance claims without measurement. |
| Reference Baseline | SoftPC2464862d; existing in-tree XP mirror is comparison material, not a new import. |
| Candidate Proposal | [S5 ordered Compat audit](../proposals/m9-xp-mirror-original-first-audit.md#s5-ordered-compat-simplification-audit). |
| Files And ABI Surface | src/app-softpc/compat plus read-only caller/build/test inspection. Approved changes must identify all consumers before implementation; evidence-backed mirror diff reduction is allowed without adding a second path. |
| Applicable Rules | docs/README.md reading set; execution, architecture, coding, documentation and source policies. |
| Verification | Frozen dir order, hashes and tracked-file equality; per-file real callers and ownership proof. Documentation-only checks now; approved implementation uses cached dual builds and proportional/full regression, publication and review. |
| Expected Markers | Every file has ordinal, lines/hash, finding, evidence and status; no missing file hidden by traversal changes. |
| Asset Needs | Preserve assets/INI/media. If needed S5 owns only build/t100-s5, two-hour total and fifteen-minute no-progress budgets; hidden background jobs and delivery cleanup. Each batch owns scratch until delivery cleanup. |
| Reporting Requirements | Per-file purpose, consumers, removable/merge candidates, exact or explicitly estimated source/test/file delta, mirror impact and proof limits. |
| Stop Conditions | Unproved equivalence is an investigation item; do not remove required failure handling or add mirror differences to improve Compat metrics. |
| Exit Criteria | Every frozen file reviewed and dispositioned; owner-approved changes qualified and delivered, unresolved candidates explicitly decided; self-review and clean workspace before owner closure. |
| Original Owner Request | 现在准入一个新s 按照dir固定顺序审计compat 目标是精简compat代码规模 减少文件数和行数。追加：自行设定目标连续执行，统一架构，尽可能减少 Compat 自主实现 footprint；有证据时顺带减少镜像 diff。 |
| Similar-Issue Sweep | Trace matching redundancy across consumers without advancing unreviewed file status; distinguish original required adaptation from accidental duplication. |

## Current Technical Baseline

S4 closure P98: final audit delivery eefd99e9. All34 deferred generated files
are reviewed; the eight unwired entries (145 blocks,+228/-228) are approved
as uniform native ABI adaptation, without a production-consumer claim.
Current inventory remains106 real-diff files,15,751 blocks,+24,585/-24,014.
Latest full qualification remains P89 392/392; P92 subsequently verified
byte-identical affected compiler objects for indentation-only changes.
This closure changes no source, tests, EXEs, configuration or snapshots.

Earlier delivery summaries below are historical; their pending/active wording
is superseded by the owner decisions and S4 closure above.


Owner approves nt_vga.c R0683-R0686. S4 P96 confirms the forward audit reaches
order468, the last current real-diff file. Fresh checks find all four trailing
ROM/resource files byte-identical to fixed XP. No further forward diff file.
The 34 previously skipped generated cvidc files remain unapproved; first is
order283 base/cvidc/sevid000.c. Do not infer S4/T100 closure or audit approval
from reaching the end. No source/test/EXE change. See [P96 record](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p96-accept-vga-review-and-confirm-end-of-forward-order).

Owner approves nt_sound.c R0677/R0678/R0680/R0681. S4 P95 audits order468
nt_vga.c at 7529e4d5: all thirteen retained blocks,+37/-16 map to four standard
painter boundary/geometry contracts (R0683-R0686). Current real dispatch and
fixture assertions corroborate historical original/guard-only negative controls.
No restoration candidate, source/test/EXE change or fresh runtime claim. Await
owner approval; do not advance. See [P95 audit](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p95-nt_vgac-retained-painter-differences).

Owner approves nt_sbmxr.c R0674-R0676. S4 P94 audits order467 nt_sound.c at
bfbe3a59: twelve comparison blocks,+7/-80, including two moved exclusion
scaffold lines each way; substantive retained differences are +5/-78. Current
Timer/PPI/service callers and Compat endpoint match the original-state/host-sink
boundary. No justified restoration candidate; original cleanup/duplicate includes
remain. No source/test/EXE changes or new dynamic claim. Await owner approval,
do not advance. See [P94 audit](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p94-nt_soundc-current-tone-boundary-audit).

Owner approves nt_sbmid.c residuals and P92 restoration. S4 P93 reviews
order466 nt_sbmxr.c at 0e765b4f: six additions,+48/-0, with every original XP
line preserved in original order. All additions map to archive state, actual
output application or composed gain. No restoration candidate or production
change; fresh exhaustive integer check covers 131072 raw gain pairs, distinct
from prior C/runtime evidence. Await owner approval; do not advance. See
[P93 audit](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p93-nt_sbmxrc-complete-addition-only-audit).

S4 P92 restores nine original nt_sbmid.c indentation prefixes: +9/-9, with no
non-whitespace or line-count change. All six actual x86/x64 production/unit/
native object compilations match pre-edit SHA256 byte-for-byte. Under owner
instruction, no tests/link/publication ran; published EXEs remain unchanged.
x64 retains twelve original inactive-helper pointer warnings, x86 zero.
XP diff reduces 18 lines to +275/-391; matching lines split 33 blocks into 37.
Global inventory: 106 files,15751 blocks,+24585/-24014. All seven semantic IDs
have explicit current-contract and bounded evidence; historical negative tests
are not described as new runs. Await owner review; do not advance past order465.
See [P92 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p92-restore-midi-switch-indentation-and-review-residual-evidence).

Owner approves P90's nt_sbfm.c residuals. S4 P91 reviews order465 nt_sbmid.c
at fb8d9df4: all 33 blocks,+284/-400 map to storage, lifetime/archive, parser
and send contracts. Nine XP-identical switch statements differ only in
indentation; isolated formatting restoration reduces XP diff to +275/-391.
No source/test/EXE modification or new runtime claim. Await owner decision on
retention and formatting restoration; do not advance. See [P91 ledger](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p91-nt_sbmidc-current-retained-content-audit).

Owner accepts P89. S4 P90 audits order464 nt_sbfm.c at 0570fad6: four real
blocks,+20/-104; recommended retention is host-boundary omissions and two-byte
FM snapshot state. No source/test/EXE change or fresh runtime claim. Corrected
inventory total is 106 files,15747 blocks,+24594/-24023; two previously missed
comment terminators explain the accounting-only change. All four points are now owner-approved. See [P90 audit](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p90-nt_sbfmc-current-retained-content-audit).

S4 P89 implements the five owner-approved nt_sbdsp.c restorations from P88:
two original-position forwards and three original USHORT spellings. Source
+13/-9, tests unchanged. Actual cached DSP objects are byte-identical on x86
and x64; published EXEs have refreshed build timestamps. Full cached builds
are warning/error-free; all 392 tests pass first-try (196 per architecture,
including native desktop/audio and both package cases). Self-review confirms
no behavior/API/schema change, preserved CRLF and conditional/line controls.
Git numstat corrects the P88 estimate: XP diff +541/-1006 becomes +541/-1002,
four fewer changed lines, not fourteen; alignment produces 104 vs 103 blocks.
The unchanged nt_sb.c inventory row separately corrects five missed deletions.
Current global inventory: 106 files,15747 blocks,+24594/-24021. Other 97 P88
blocks retain bounded source/alignment/historical evidence, not independent
runtime reproductions. P89 is owner-accepted. S4/T100 remain
open. See [P89 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p89-restore-approved-dsp-declarations-and-cast-spelling).

S4 P87 removes exactly two mirror-added duplicate includes in nt_graph.c and
nt_sbmid.c after scanning 467 mirror C/H files (74 added include occurrences).
Original duplicate includes and non-mirror code remain untouched. Four actual
x86/x64 object comparisons are byte-identical. Source +0/-2; tests unchanged;
XP changed-line count reduces two. Dual cached publication and all 392 cases
pass without retry (188 background, six native and two separate package tests
per width). Existing same-runner package P2 remains open. Final object hashes
still match the unchanged baseline. Await owner acceptance.
P86 code and nt_ega.c retention are owner-approved; P86 manual acceptance remains
deferred. S4/T100 stay open. See [P87 sweep](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p87-mirror-added-duplicate-include-sweep).

S4 P86 reviews order457 nt_cga.c completely outside existing exclusions.
R0587 redundant include is restored through original conapi.h; exact dual-width
objects are unchanged. R0586/R0589-R0593 retain current evidence. Production
+0/-1; tests unchanged; XP diff now nine blocks,+21/-52 (one fewer changed line).
Fresh dual-width negative controls reproduce the compilation/text/CGA failures
that require the other changes. Final dual publication succeeded. Background
188/188 each; native 7/8 each initially, compact passes isolated on both widths;
existing P2 remains open. Final focused/gate 3/3 each; production objects are
byte-identical across the restoration. Await acceptance, S4/T100 remain open. See
[P86 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p86-nt_cgac-retained-difference-proof).

S4 P85 completes the owner-approved analogous cleanup: eleven nt_sb.h
prototypes removed, private DMA record relocated, original local forwards reused;
four added DSP helpers now static. Tests drive original commands/service and
DMA observations. Source +165/-171; tests +152/-117. Matched-XP changed-line
count decreases four; old provider deleted-line accounting is corrected
separately, not attributed to this change. No function-body/schema/shared-package
change. Dual publication/build succeeded; background 188/188 each, x64 native
8/8; x86 native 7/8 initially plus the interrupted compact case passed in an
isolated rerun. Existing P2 startup debt remains, not a clean aggregate claim.
Final affected SB tests 3/3 each. Await owner acceptance, no S4 closure.
See [P85 delivery](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p85-complete-dsp-and-dma-declaration-ownership).

S4 P82 records owner approval of R0563/R0564. Eight intervening host headers
have no retained differences. nt_graph.h has R0571/R0573, one block,+2/-1:
scanline panning parameter and shared group-helper declaration. Recommend
retain pending review; no source/artifact change. See [P82 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p82-approved-pointer-carriers-and-ega-panning-declarations).

S4 P81 records owner approval of R0558/R0559. Six intervening host headers
have no retained differences. insignia.h has R0563/R0564, three blocks,+10/-1:
native IHPE and source-scoped C-VID IUH. Recommend retain pending owner review;
no source/artifact change or fresh runtime claim. See [P81 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p81-approved-host-profiles-and-pointer-carrier-review).

S4 P80 records owner approval of R0556 and reviews host_def.h: two blocks,
+0/-2, omitting NTVDM and CPU_30_STYLE definitions. R0558/R0559 are recommended
for retention as selected product-profile adaptations, pending owner approval.
No code/artifact changes or fresh runtime claims. See [P80 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p80-approved-register-bytes-and-host-profile-review).

S4 P79 records owner approval of R0554. error.h/host.h have no retained
difference. host_cpu.h has only R0556 (+1/-1, two zero-context move hunks):
low/high byte order required by original port consumers. Recommend retain
pending review; no source/artifact change or fresh runtime claim. See
[P79 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p79-approved-fpu-layouts-and-register-byte-review).

S4 P78 delivers the standalone host-clock pause/save/load repair: one runtime
restart owner, no mirror/shared/schema changes. Five production files +37/-20;
two tests +79/-6. Both cached EXEs are published and 392/392 tests pass. Owner
confirms a new snapshot works; normal native playback/load/pause/resume probe
also passes. The original snapshot's unsuccessful restoration remains an
explicit limit, not a success claim. Own clock scratch is retired; mirror audit
continues. See [P78 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p78-standalone-pause-and-snapshot-clock-lifecycle).

S4 P77 records approval of R0550/R0551. video_io.c has zero retained diff.
Next true-diff file cfpu_def.h has13 blocks,+10/-8: R0554's six host-layout
adjustments and technical ABI explanation. Recommend retain pending review;
no production/artifact change or fresh runtime claim. See [P77 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p77-approved-bios-video-and-host-fpu-layout-review).

S4 P76 records owner approval of R0543-R0548 and reviews order431 video.c:
15 blocks,+15/-19 including the already-approved P46-A stream omission.
R0550/R0551 are recommended for retention pending owner review; no safe
restoration candidate or code/artifact change. Historical guest/negative proof
is distinguished from fresh source review. See [P76 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p76-approved-vga-controller-and-bios-video-review).

S4 P75 records approval of R0539-R0541 and reviews order429 vga_prts.c:
11 blocks,+188/-23; R0543-R0548 await owner review. Recommend retain based on
source/caller review and historical negatives. Only a missed comment identifier
and its manifest hash change; executable tokens and artifacts are unchanged.
Concurrent clock work is excluded. See [P75 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p75-approved-display-units-and-vga-controller-review).

S4 P74 reviews order428 vga_mode.c at0854c857: R0539-R0541 are three real
condition replacements,+3/-3. Recommend retain display-stride/wrap unit fixes;
historical individual/whole-group negatives and register-level reachability
support necessity. No safe restoration candidate or production/artifact change.
No fresh runtime test claim. Await owner review. Own build/t100-s4-p74 gate
scratch is retired. See [P74 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p74-approved-v7-validation-and-display-mode-unit-review).

S4 P73 reviews order427 v7_video.c at7306f514: only R0537,+1/-1, masking the
preserve-memory bit for V7 mode validation. Owner approves R0537. Historical actual-source negatives and permanent different-mode BIOS
regression support necessity; no fresh runtime result or source/artifact change.
Own build/t100-s4-p73 gate scratch is retired. See [P73 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p73-approved-pointer-condition-and-v7-mode-validation-review).

S4 P72 restores the owner-approved original gfx_updt.c prototype (+1/-1),
reducing its XP diff by one block/two lines to130,+230/-210. Cached dual EXEs
are published; both changed source objects are byte-identical and whole EXEs
are equal apart from build metadata, so full tests are not repeated under the
owner's equivalence exception. P69 full392/392 remains runtime evidence.
Next-file v7_ports.c review finds only R0536 (+1/-1), now owner-approved; no change to that file. Own build/t100-s4-p72 is retired after
gates. See [P72 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p72-restore-original-stream-prototype-and-review-v7-pointer-condition).

S4 P71 reviews order424 gfx_updt.c at66b60b87:131 blocks,+231/-211. Recommend
retain R0530-R0535's six functional groups; one extra original stream prototype
omission lacks current necessity and is recommended for restoration, pending
owner decision. Restoring it would yield130 blocks,+230/-210. No production or
artifact change. Historical negative controls and P69 full392/392 are reused,
not rerun. Own build/t100-s4-p71 scratch is retired. See [P71 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p71-approved-chain2-writers-and-graphics-updater-review).

S4 P70 reviews order423 egwrtm12.c at825ca42d: eleven real replacements
+11/-11. R0519-R0528 are CHAIN2 writer address corrections; D006 is the
already accepted plane2 enable fix. Fresh source tracing and historical
individual/whole-group dual-width negatives support retention. P69 full392/392
is reused, not freshly rerun. No safe restoration candidate or source/artifact
change. Owner approves R0519-R0528. Own gate scratch build/t100-s4-p70
is retired. See [P70 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p70-mode1-and-mode2-chain2-writer-review).

S4 P69 repairs conditional fall-through EIP truncation in the main worktree.
Three decoder entries share full-width target arithmetic and instruction-start
based sequential wrap; CALL/JMP/RET remain unchanged. Production +14/-13;
tests +49/-27, covering576 combinations on each width. Cached dual publication
and all392 tests pass (376 background,16 native). A60-second overlay Win95 run
observes eight correct executions of the previously failing instruction and no
extra reset. No visual desktop acceptance is claimed. Owner-approved closure
of the occupied x86 EXE enabled publication. Guest media/INI are unchanged;
own scratch build/t100-s4-eip-fix is retired. S4/T100 remain open. See
[EIP repair qualification](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p69-owner-approved-conditional-eip-repair-qualification).

S4 P67 reviews order421 ega_vide.c at225b0b3a: R0517 is the sole real
insertion +13/-0, using current-VM virtual CRTC height for VGA/V86 BIOS font
row calculation. Historical P124 original-restoration negatives fail on both
widths; unchanged current coverage passed in P65. Owner approves R0517; source diff comments must contain technical reasons,
not task or decision identifiers. No fresh runtime or guest tests claimed. No production,
test or artifact changes. Preserve the concurrent Win95 investigation's
uncommitted record; only P67 documentation is staged. Own gate scratch
build/t100-s4-p67 is retired. See [P67 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p67-approved-reset-access-and-virtual-text-height-review).

S4 P66 reviews order419 ega_prts.c at c6bdd276: one real insertion +6/-0,
R0514 read-only snapshot access to the original EGA reset owner. VGA writes
use that receiver while register-zero readback returns constant3. Existing
reset0..3 capture/restore assertions passed on both widths in P65; this review
reuses that unchanged-source evidence, without claiming fresh runtime tests.
No safe restoration candidate or production/artifact change. Owner approves
R0514. Own gate scratch build/t100-s4-p66 is retired; Win95 investigation
untouched. See [P66 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p66-approved-mode0-writers-and-ega-reset-snapshot-review).

S4 P65 reviews order416 egawrtm0.c at030a5e6e: four real replacements +4/-4,
R0510-R0513 CHAIN2 BIOS byte/word mapping. Fresh producer/reader wiring review
and historical per-line dual-width negatives support retention. Existing VGA
regression freshly passes2/2; no new source/artifact changes or safe restoration
candidate. Own scratch build/t100-s4-p65 is retired after verification, Win95
investigation untouched. Owner approves R0510-R0513. See [P65 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p65-approved-unexpected-interrupt-omission-and-mode0-writer-review).

S4 P64 reviews order414 unexp_nt.c at1caff181: one real diff +0/-1, R0508
terminal Ctrl-Z omission. Whole handler matches pinned XP. Fresh x86/x64
compiler controls pass; restoring only0x1a fails both with stray-byte errors.
Owner approves R0508. No production/test/artifact
change. Own scratch build/t100-s4-p64 is retired after verification; concurrent
Win95 investigation remains untouched. See [P64 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p64-approved-pit-and-unexpected-interrupt-file-review).

S4 P63 reviews order412 timer.c at b95ccba1: eight real diff blocks +371/-2.
Snapshot support accounts for +360/-0; four device corrections account for
+11/-2, including already accepted S2 D005. Fresh PIT/checkpoint/sound tests
pass6/6 across x86/x64. Owner approves R0502-R0505; no safe restoration
candidate established. No production/test/artifact change. Own disposable
scratch build/t100-s4-p63 is isolated from Win95 investigation; S4/T100 remain
open. See [P63 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p63-accepted-p62-and-pit-file-review).

S4 P62 restores the owner-approved original display_string brace pair; R0499
is approved and unchanged. Production +2/-0, tests unchanged; rom.c becomes
one real diff block +1/-1, two fewer blocks/changed lines. Dual cached builds
and publication pass; all 392 tests pass after correcting and rerunning the
x86 manifest gate (initially the hash was written to the wrong column). All
16 native tests pass. Actual-diff self-review passes. Only own P62 scratch is
retired; investigation scratch is protected. Owner explicitly authorized
closing the occupied published EXE and copied the investigation binary apart.
Owner accepts P62 (b95ccba1); S4/T100 remain open. See [P62 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p62-restore-original-rom-display-scope).

S4 P61 reviews order411 rom.c at eb8e706d against pinned XP: three blocks
+1/-3. Recommend retain R0499's 16-bit ROM segment repair (+1/-1), and restore
the omitted original pair of display_string braces (two deletion-only blocks).
Fresh existing ROM fixture passes on both widths; original-cast negative and
full regression are not repeated. No source/artifact change. Own disposable
scratch is build/t100-s4-p61; concurrent Win95 work remains protected. Decisions
await owner review. See [P61 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p61-accepted-p60-and-rom-file-review).

S4 P60 implements owner-approved R0496 measure-only cleanup at cefa3fae:
four unused assignments removed, all capture/restore code unchanged. Fixed-XP
diff becomes one block +226/-0 (four fewer lines); source +0/-4, tests unchanged.
Dual cached builds/publication and full regression pass 392/392 on the first
run, including all 16 native tests. Actual-diff self-review passes; snapshot
and INI are unchanged. Own scratch build/t100-s4-p60 is retired; concurrent
Win95 work in build/t100-s4 is protected. R0496 body is approved; owner accepts P60 (eb8e706d). S4/T100 remain open. See [P60 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p60-remove-unused-queue-measurement-outputs).


S4 P58 reviews order407 ica.c at49f0729f: seven blocks +119/-13, original
snapshot/EOI interfaces, delayed IRQ gating, master withdrawal, CCPU IRET-hook
adaptation and accepted P53 phase capture. Fresh callers and historical actual-
controller negatives support retention; no new safe restoration is established.
Owner approves R0489-R0493; L001 remains accepted. No source/artifact or
runtime changes; P56 work preserved. See [P58 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p58-pic-file-review).

S4 P57 reviews order405 cmos.c at29c41ecb against pinned XP: four blocks
+122/-4, R0482 host HDD configuration and R0483 snapshot access. Fresh callers
and historical dual-width negatives support retention; no further restoration
candidate is confirmed. Snapshot phase/mode exhaustiveness is not claimed.
Owner approves both groups. No production/artifact change; intervening
P56 work is preserved. See [P57 review](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p57-cmos-file-review).

S4 P56 retains the corrected 16-bit EIP wrap semantics and the direct CCPU
timer dispatch; the two diagnostic reversions are discarded. Embedded BIOS/VGA
ROM inputs are now binary checkout material, configure-time checked byte-for-byte
against the pinned XP manifest, and explicit dependencies of every firmware
resource object. This prevents a checkout conversion or stale resource object
from producing a mixed-ROM launcher or test executable. See [P56 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p56-firmware-rom-provenance-and-resource-dependencies).

S4 P55 implements the owner-approved R0479 placement restoration and marks
R0476-R0480 necessary functionality approved. at_dma.c now has 15 blocks
+112/-17, five fewer fixed-XP changed lines. No non-comment/non-line-control
token changes, DMA behavior or snapshot schema changes. Cached dual builds and
publication pass; full regression passes 392/392 on the first run, including
all desktop/native-audio tests. Production +9/-9, tests unchanged. Actual-diff
self-review and manifest/whitespace gates pass; P55 awaits owner acceptance. See [P55 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p55-restore-dma-accessor-placement).
S4/T100 remain open at the at_dma.c frontier.


S4 P53 implements owner-approved PIC phase archiving and removes the redundant
PPI derived field. R0469/R0470 remain approved and unchanged; R0471 retains
only register/gate snapshot state. SCP5 is the sole reader format. The existing
asset is converted with reversible byte-exact proof and loaded successfully
through the real executor on both architectures. Final cached dual publication
and full regression pass 392/392, including all 16 native tests; both missing-
phase negative controls fail as expected. Production +20/-15 (net +5), tests
+61/-2 (net +59). Relative to fixed XP, mirror changed lines increase 14 across
the same two files; the current total is 106 files, 15,709 blocks, +24,617/-24,018.
Actual-diff self-review finds one original phase owner, no replay/state machine,
legacy reader or shared-component change. Owner accepts P53 (908f6803); S4/T100
remain open. See [P53 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p53-pic-phase-and-ppi-snapshot-correction).


S4 P51 narrows owner-approved R0466 to cold initialization and restores XP
guest device-reset data behavior. Owner also approves correcting Console
metadata endpoints and removing helper re-expansion; cooked restoration rejects
lost capacity, while raw/new frames retain existing surface preparation.
The native fixture now respects actual desktop geometry without weakening
content retention. Final cached dual builds/publication and full regression
pass 392/392 (188 background and eight native per architecture), with no
retry or skip in this final run. Earlier failures and isolated comparisons
remain recorded as investigation evidence. Production +10/-16 (net -6), tests
+47/-21 (net +26); fixed-XP mirror diff decreases two lines. Actual-diff
self-review finds no added API, state, recovery path or snapshot change.
Owner accepts P51 (17bccc35). P52 reviews ppi.c; S4/T100 remain open.
See [P51 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p51-confine-r0466-data-reset-to-cold-initialization).

S4 P50 restores owner-approved R0463 to original finite InPort bursts,
removes periodic callback wiring, and archives the original remaining budget.
Current-only snapshot format has no legacy reader. The owner-authorized existing
package snapshot is converted once; only two bytes differ, and both widths
restore it through the real executor with saved PIC state intact. Regression
exposes VGA replay altering shared IRQ9 after PIC restore; move the one PIC
restore to the end of the existing Compat device path, with no mirror addition.
Final cached dual publication and full tests pass 392/392 after correcting the
recorded fixture assumption and restore-order defect. Production +45/-73
(net -28), tests +38/-78 (net -40); real XP diff decreases 90 changed lines.
Self-review and gates pass. See [P50 evidence](../etc/evidence/softpc/m9-t100-owner-decisions.md#s4-p50-restore-original-inport-bursts-and-current-snapshot).
P50/P51 are owner accepted. P52 is the current ppi.c review; S4/T100 remain open.

S4 P49 completes owner-requested isolated x64 coherent-original/current
R0463 comparison: identical cold-boot overlay media, real Win1.01/Win3.1
movement and File menus work in both. Original burst count reaches zero;
menu pixels match exactly. Twelve probe runs exit zero. No causal need for
R0463 is established; recommend restoration review rather than automatic
retention. No production/test/EXE/media change; no x86/all-guest claim.
P48 blanket recommendation for R0463 is superseded; other points unchanged.
S4/T100 remain open at mouse.c pending owner decision.

S4 P48 records keyba.c R0457/R0458/R0460 approval and reviews order 400
mouse.c: 14 blocks +115/-34. R0463 replaces finite interrupt bursts with the
existing periodic event path; R0464 archives private controller state; R0465
initializes undefined reads; R0466 resets stale data. Recommend retention
with stated evidence limits; all four await owner approval. No source, tests
or EXE changes. Next is order 401 ppi.c; S4/T100 remain open.

S4 P47 reviews order 398 keyba.c: four blocks +121/-0, three decisions.
R0457 adds snapshot state/callback access (+119); R0458 supplies C0 input-port
response (+1); R0460 clears consumed IRQ1 (+1). Current source/callers and
historical paired negatives support retention; no equivalent restoration found.
Owner approves all three. No production/test/EXE change; next is order
400 mouse.c. S4/T100 remain open.

S4 P46 removes the owner-approved unused stream bodies, four declarations,
globals and empty Compat function; the active graphics tick remains intact.
Cached dual builds/publication and full regression pass 392/392 first run,
with no retries/skips. Production +3/-28 (net -25), tests unchanged; real XP
diff files decrease 107 to 106, changed mirror lines increase 11. R0455 is
removed, not retained; two original-body omissions are explicitly approved.
Actual-diff review finds no added state/path or shared-package change.
Owner accepts P46; S4/T100 remain open.

S4 P45 records owner approval of tape_io.h R0451 and reviews order 394 video.h:
R0455, one block +4/-0, exposes original stream declarations to the selected
non-NTVDM build. Pinned XP teletype code itself references them unconditionally;
Compat keeps the stream disabled. Historical dual compiler controls support
retention, not active stream support. No equivalent restoration identified;
R0455 awaits approval. No source/runtime changes. Next is order 398 keyba.c;
S4/T100 remain open.

S4 P44 records owner approval of sndblst.h R0448/R0449 and reviews order 386
tape_io.h: R0451, one block +0/-1, deletes XP's final Ctrl-Z byte. Complete
retained-content comparison finds no other difference or replacement blank.
Historical actual dual tape_io.c compiler controls reject the restored byte;
recommend retention, pending owner approval. No source or runtime changes.
Next real-diff file is order 394 video.h; S4/T100 remain open.

S4 P43 records owner approval of sas.h R0444 and reviews order 383 sndblst.h:
two blocks +2/-1. R0448 exposes real teardown failure; R0449 declares the
0x36 unsupported-UART framing command. Actual callers and historical failure/
framing fixtures support retention; neither is newly dynamically replayed.
No further safe restoration identified; both points await owner approval.
Production/tests/EXEs unchanged. Next is order 386 tape_io.h; S4/T100 open.

S4 P42 records owner approval of quick_ev.h R0442 and reviews order 381 sas.h:
R0444, one block +1/-1, matches Length_of_M_area's declaration to the original
PHY_ADDR storage. Fresh pinned-XP definitions and historical dual compiler
controls support retention; no runtime corruption is claimed. Other former
header differences are already restored. R0444 awaits owner approval.
No source change or fresh runtime test. Next is order 383 sndblst.h;
S4/T100 remain open.

S4 P41 records owner approval of ios.h R0440 and reviews order 377 quick_ev.h:
R0442, two blocks +35/-0, defines the real pending-event snapshot contract.
Private queue ownership and original add semantics rule out equivalent restore
through existing add/delete APIs; historical dual checkpoint tests support the
contract. No unused declaration or safe equivalent restoration found. R0442
awaits owner approval. No source/runtime changes or fresh runtime tests.
Next real-diff file is order 381 sas.h; S4/T100 remain open.

S4 P40 records owner approval of R0438/R0439 and removes only ica.h's
replacement EOF blank. Its real diff is now +2/-1; aggregate is 107 files,
15,716 blocks, +24,669/-24,033. Reviews ios.h R0440, one block +1/-1:
selecting the existing full I/O map prevents unregistered high ports from
aliasing COM1. Historical paired CPU I/O controls support retention, pending
owner approval. Cached dual builds/publication pass; complete artifacts are
equivalent except PE timestamp/checksum and compiler time. Runtime suites are
skipped under the approved equivalence rule; mirror/documentation checks pass.
Next actionable file is order 377 quick_ev.h; S4/T100 remain open.

S4 P39 records owner approval of gvi.h R0434 and reviews order 367 ica.h:
three blocks +3/-1, two semantic/compiler decisions. R0438's two guard lines
prevent PIC/CCPU name collision; R0439 removes a compiler-rejected Ctrl-Z.
Recommend retaining both necessary changes, but deleting the replacement EOF
blank (one added diff line). Decisions await owner approval; no source change
or fresh runtime test. Next actionable file is order 369 ios.h; S4/T100 open.

S4 P38 records owner approval of egaports.h R0428 and reviews order 359
gvi.h R0434: one block +2/-0 excluding necessary diagnostic line control.
The lane-preserving CHAIN2 offset macro serves the real painter/update paths;
historical dual-architecture direct-offset controls fail the text-cell oracle.
No equivalent restoration identified; R0434 awaits approval. No production
change or fresh runtime test. Next real-diff file is order 367 ica.h.
S4/T100 remain open.

S4 P37 records owner approval of egacpu.h R0426 and reviews order 342
base/inc/egaports.h: one declaration, R0428, +1/-0. Current production
painters and dirty-neighbour marking consume the original-register query;
historical paired pixel/compiler controls support retention. No further safe
restoration identified; R0428 awaits owner approval. No source/runtime change
or fresh runtime test. Next real-diff file is order 359 gvi.h; S4/T100 stay open.

S4 P36 records owner approval of dma.h R0424 and reviews egacpu.h R0426:
one block +4/-1, matching the globals pointer to its actual generated provider.
Original/current assignment and historical compiler controls support retention;
no runtime corruption or pointer-width change is claimed. Earlier redundant
wrapper/tag/include differences are already restored. R0426 awaits approval.
No source/runtime changes. Next is order 342 egaports.h; S4/T100 remain open.

S4 P35 records owner acceptance of P34 and reviews order 337 dma.h:
one point +3/-0, R0424, exposes the existing pointer-returning DMA accessor
to the retained DSP outside NTVDM. Actual production callers and historical
paired compiler controls support retention; no equivalent restoration found.
R0424 awaits owner approval. No source/runtime change or fresh runtime test.
Next file is order 339 egacpu.h; S4/T100 remain open.

S4 P34 completes the owner-authorized redundant conditional-wrapper sweep:
109 real-diff files scanned, twelve candidate files checked, ten unnecessary
non-XP wrappers removed in eight files. Production +1/-43; tests unchanged.
Real XP differences decrease 29 lines and five text blocks; cpu_vid.h/gmi.h
leave the list: now 107 files, 15,716 blocks, +24,670/-24,033. Necessary type,
endian, snapshot, TLS and host-route behavior remains. See the P34 audit.
Cached x86/x64 package and complete test-target builds pass. Of 1,088 objects,
only two build-timestamp objects differ; complete EXEs are equal except for
PE timestamp/checksum and the compiler time string. Runtime suites are skipped
under the owner's artifact-equivalence rule; no new full-suite pass is claimed.
Affected source/documentation/manifest/dependency gates pass 8/8.
S4/T100 remain open; next manual file is dma.h.

S4 P33 records owner approval of fla.c R0417. Current cpu_vid.h review finds
one unnecessary non-XP conditional wrapper, counted as two blocks +3/-0.
The selected declarations already match XP and actual builds define both
required macros. Recommend removing only this added wrapper (five physical
lines including its error/line control), pending owner approval. No source
change or runtime test. Next file after decision is dma.h; S4/T100 stay open.

S4 P32 records owner approval of fdisk.c R0415 and reviews order 321 fla.c:
three real text blocks +97/-0, all R0417 FDC snapshot integration. Current
production callers and original state/callback ownership support retention;
existing tests cover register restoration, not every buffer or event timing.
No further equivalent restoration identified. R0417 awaits owner approval;
source/tests/EXEs unchanged. Next real-diff file is order 335 cpu_vid.h.
See the owner decision ledger; S4/T100 remain open.

S4 P31 reviews order 320 fdisk.c: two real text blocks +120/-0, all R0415 HDD
snapshot integration, with per-piece sizes, actual archive/event consumers,
current fixture coverage and explicit untested boundaries. No equivalent XP
restoration preserving snapshot behavior identified. Retention awaits owner
approval; no source/runtime change. Next manual file after decision is fla.c.
S4/T100 remain open; see the owner decision ledger for the complete review.

S4 P30 implements owner-approved nonsemantic restoration across the current
114-file input: 24 groups in 15 mirror files, production +34/-34, no token or
behavior change. Owner approves diskbios R0414; it remains +2/-2. Five files
now have no real retained-content diff and are removed from the current list.
Totals: 109 files, 15,721 text blocks, +24,699/-24,033 (58 fewer diff lines).
See [P30 audit](../etc/evidence/softpc/m9-t100-s4-audit.md#p30-nonsemantic-restoration-across-114-files).
Cached dual builds/publication and full tests pass 392/392, no retries/skips.
Mirror objects remain byte-identical; EXEs differ only in timestamp/checksum.
Actual-diff self-review passes. Semantic review of skipped files remains
pending; S4/T100 remain open. Next manual file is fdisk.c.

S4 P29 records owner approval of vglfunc.c R0412 (+5/-0). At owner direction,
intervening generated cvidc files are skipped, not approved. Current review is
order 319 diskbios.c: three text points +4/-4; recommend restoring two
nonsemantic directive comment/whitespace changes and retaining R0414's
capacity split (+2/-2). No source changes; decisions await owner review.
Details and historical evidence limits are in the owner decision ledger.
Next is fdisk.c; S4/T100 remain open.

S4 P28 records owner approval of ev_glue.c R0254/R0255/R0263 and updates the
[current real-diff list](../etc/evidence/softpc/m9-t100-s4-real-diff-files.md)
to 114 files, removing all seven now-zero-diff rows and using P25 current counts.
Frozen TSVs remain historical; original ordinals are retained. Next is order
283 sevid000.c. Documentation-only; S4/T100 remain open.

S4 P27 records owner retention approval of printer.c's five pieces, skips
printer_.c/rs232_io.c now without real selected differences, and presents
base/cvidc/ev_glue.c (frozen review 22 / order 275): three points +3/-2.
GDP include/allocation and CCPU aggregate selection are native ABI adaptations;
actual callers/flags and historical independent controls support retention with
stated limits. No source change or fresh runtime test. Owner decision pending;
next real-diff file is order 283 sevid000.c. S4/T100 remain open.

S4 P26 rechecks current printer.c: two selected text blocks +71/-0, comprising
one include and four snapshot functions (65 function lines plus five separators).
All are snapshot integration, with actual callers, current P24 checkpoint proof
and separately identified historical deletion controls. Recommend retention;
no source/runtime changes or new file-frontier advance. The decision ledger
records per-function sizes and proof limits; S4/T100 remain open.

S4 P25 reconciles current mirror statistics at accepted production 327f9960:
254 identical, 104 exclusion/line-control-only and 114 real-diff files (472 total).
Real selected differences: 15,761 text blocks, +24,728/-24,062, 48,790 changed
lines. Text blocks are not a completed semantic-point count. All current hashes
and seven changed-code recounts reconcile; no production or runtime change.
See [P25 aggregate](../etc/evidence/softpc/m9-t100-s4-audit.md#p25-accepted-p24-and-current-aggregate).

S4 P24 implements the owner-approved printer cleanup and mirror-wide same-issue
sweep across 472 files. Nineteen files lose 18 extracted SCCSID comments and four
avoidable added declarations through three placement adjustments. Original
mirror duplicates, function bodies and all conditional/#error/#line sequences
remain. Cleanup removes 21 grouped points (22 individual lines); selected XP
diff in the changed files decreases +1712/-1072 -> +1690/-1072, 22 fewer lines.
Git production +99/-121 (net -22); tests unchanged. Final cached dual builds,
publication and full regression pass 392/392 without retries/skips. Actual-diff
review and gates pass. See [P24 evidence](../etc/evidence/softpc/m9-t100-s4-audit.md#p24-mirror-wide-added-diff-cleanup).
Owner accepts P24; unrelated semantic review remains at file 19 and
S4/T100 remain open. Frozen file/count ledgers are not rewritten.

S4 P23 records owner acceptance of com.c and presents printer.c, review 19 /
order 268: three selected text points +74/-0. Snapshot include/controller/event
mapping have actual production callers and historical negative-control evidence.
One extracted notice and two added declarations can be removed by the same
minimal cleanup approach, yielding +71/-0; no source change yet. Details and
proof limits are in the owner decision ledger. 19/121 presented, 102 remain;
printer_.c is next. S4/T100 remain open.

S4 P22 implements the owner-approved com.c declaration cleanup: move the added
snapshot block after original declarations and remove only its three added
forward declarations. Original mirror duplicates remain untouched. Production
+118/-121 (net -3), tests unchanged; selected XP diff +125/-3 -> +122/-3.
Cached dual builds/publication and full regression pass 392/392 without retries
or skips. Function placement changes prevent using the binary-equivalence
exception. Exact source reconstruction and actual-diff self-review confirm no
other source change, snapshot-format change or new path. See
[P22 evidence](../etc/evidence/softpc/m9-t100-s4-audit.md#p22-added-uart-declaration-cleanup).
Owner accepts P22 and the reviewed com.c points; S4/T100 remain open.

S4 P21 rechecks the five remaining com.c points. A concrete source-only
candidate relocates the added snapshot block after original declarations,
removing three duplicate forward declarations: block 121 -> 118, selected XP
diff +125/-3 -> +122/-3. At P21 the candidate existed only in memory; P22 above now implements it. Per-point evidence strength and limits are
recorded in the decision ledger. No file-frontier advance or new runtime proof.

Owner corrects P19: content inside a wholly excluded branch must not be
relocated outside it as a retention exception. P20 removes the one com.c
comment extracted from SCCSID and retires R0173 from current retention review;
conditional framework, #error and required #line remain. Dual cached builds
and publication pass; full EXEs are metadata/time-only different, so no new
runtime run under the existing artifact-equivalence exception. The five functional
com.c points stay pending. This explicit owner instruction supersedes P19's
notice-retention recommendation; no general notice or license change is inferred.

S4 P19 is com.c counter-review only: the extra notice is a nonexecuting
preservation of original conditional SCCSID text; its removal would not change
runtime behavior but changes the retained notice. Snapshot minimization finds
no established substantial reduction without native-layout coupling, changed
failure semantics or new mapping abstraction. No production changes/tests;
full reasoning and alternatives are in the decision ledger. Frontier stays 18.

S4 P18 records owner approval of zfrsrvd.c R0172 and presents comms/com.c
(review 18 / order 267): +126/-3, UART snapshot bridge, three pointer-width
corrections and historical notice. Current source/caller/test review separates
historical per-cast x64 failures and queued-callback proof from untested UART
combinations. No production/runtime changes. Recommend retention, pending owner.
18/121 presented, 103 remain; printer.c is next.

S4 P17 records owner approval of ntthread.c R0160-R0162 (P9 items already
approved) and presents zfrsrvd.c, review 17 / order 265: R0172 +17/-0 private
saved-NPX-IP snapshot accessors. Fresh source/caller/test inspection corroborates
historical dual negative controls; no new runtime test or code changes.
Recommend retention, pending owner. 17/121 presented; comms/com.c is next.

S4 P16 presents ntthread.c (review 16 / order 183): current +36/-3,
five points including owner-approved P9 depth-query and safe TLS teardown.
Recommend existing outer-unwind and explicit failure-return differences;
null-return changes are not claimed as complete caller failure handling.
No source/runtime changes; historical proofs are separately identified.
16/121 presented, 105 remain; zfrsrvd.c is next.

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

- [T100 S4 ordered mirror audit](../history/M9-T100-S4-ordered-mirror-audit.md).

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
