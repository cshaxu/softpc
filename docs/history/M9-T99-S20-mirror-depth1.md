# M9 T99 S20: Mirror Conditional Depth 1

Owner closes S20 after the first-depth reconciliation. T99 remains open;
each later conditional depth is a separate S, automatically admitted after
the preceding depth completes its required delivery and closure.
The first-depth framework and classification audit covers 2,208 current
arms and 1,703 pinned-XP opening directives. The next-depth queue is provisional:
1,381 present arms, 11 known absent original groups, and 140 unverified
candidates. The first-depth source delivery is `9f9dd90f`; x86 and x64 builds
passed, each with 186 non-native and eight serial native tests. Source changes
from the S19 baseline `23f65b5c` to the S20 close: 173 mirror files,
+8,066/-16,071 lines (net -8,005); no other source or test paths changed.
The published EXE SHA-256 values are x86
`84edd752d8923462df563b2333ed8291dfeedb41c43398d620b50134d8bb90ae` and x64
`7ec56c5321a8e31bcbb03b6a4a451e15a561ea1687c99ed6e75699665b337b77`.
This closes only depth 1, not the remaining mirror audit or T99.

## M9 T99 S20 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner closes S19 and admits S20 audit. |
| Objective | Audit retained mirror conditional branches and reconcile proven excluded bodies with the owner-directed outermost `#error` rule, preserving selected code and validating x86/x64 builds. |
| Non-goals | No unverified bulk pruning, new feature import, guest behavior change, or modifications outside the admitted mirror and provenance scope. |
| Reference Baseline | S19 delivery 23f65b5c, selected XP SP1 mirror and existing manifest. |
| Candidate Proposal | [Public handoff simplification review](../proposals/m9-public-handoff-simplification.md). |
| Files And ABI Surface | Audited retained mirror conditional structure, XP subset/provenance records and relevant build verification; no public API change. |
| Applicable Rules | Execution, documentation, architecture, coding and mirror source policy. |
| Verification | Account for the finite branch universe; validate selected and deliberately rejected branches, diagnostic line mappings, actual x86/x64 builds, mirror gate and published binary comparison; build, publish, test and push the excluded phase before the retained-arm audit at each depth. |
| Expected Markers | Document original condition restoration, excluded-body `#error`, real local patches and potential original-code reuse with evidence. |
| Asset Needs | Preserve user INI, media and snapshots; build both architectures and publish EXEs if produced by the approved validation. |
| Reporting Requirements | Explain evidence limits, classify candidates with reasons and estimate mirror diff; propose independently reviewable batches. |
| Stop Conditions | Do not alter unverified mirror paths, assume unobserved means safely removable, or use external material as a build dependency. |
| Exit Criteria | Reconcile in-scope mirror branches and previously removed structures against pinned XP, including complete OR expressions and independently proven single-macro numeric conditions, record excluded and retained dispositions, and validate the x86/x64 builds and line mappings. |
| Original Owner Request | 收口s19 准入s20审计；保留或恢复镜像条件结构，被排除分支仅用 #error，保留必要 #line；被处理的最外层分支体整体替换后不保留其中嵌套条件结构，写入规范并全面统一处理镜像代码。 |
| Similar-Issue Sweep | Include source and header branch chains, compile profiles, shared declarations, and existing local fixes. |

Disposable research scratch: `build/t99-s20`, owned by S20; two-hour total and
30-minute no-progress observation budgets. Retire after evidence promotion.

S19 is owner-accepted and closed; see [closure](../history/M9-T99-S19-compat-boundary.md).
Its delivery is 23f65b5c. T99 remains open.

S20 first application: six mirror files +64/-6 restore five original
conditional structures and replace one excluded segment body with `#error`
and preserved `#line`. Both cached architecture builds and five focused tests
per width pass; the rejected condition and line marker are verified. Both EXEs
match S19 outside PE timestamp/checksum. This is not the conversion of all
mirror subsets or a full regression; the proposal owns remaining scope.

S20 macro inventory at 31ee31e1 records 241 mirror conditional identifiers
and 3,072 identifier occurrences across 664 x86/x64 App-related compile
commands; two `#elif` uses are unprobed. The proposal records the observational
limits and linked tables. Coupled conditions remain unmodified.

Owner directs that an excluded outer branch body replaces its nested
directives. Previously removed structures are being restored where uniquely
anchored and independently classified. Existing excluded arms are changed
only after checking both local and XP parent conditions. An initial local-depth
classification mistakenly changed 59 XP-coupled arms; they were individually
restored and recorded in the coupled-deferred ledger. The current intermediate state contains 1,572 excluded existing arms and
892 restored structures, plus the earlier com.c pilot. These include PIG arms
with local additions, proven false single and AND conditions, and selected
sibling chains. Original copyright notices remain outside rejected arms.

Several earlier wrapper-based incremental verification reports were invalid:
Windows PowerShell execution policy rejected the child scripts and the wrapper
read stale status files. Those reports are not acceptance evidence. Explicitly
invoked builds subsequently passed for x86 and x64; both Ninja trees reported
no work after the latest depth-1 changes as well. Current literal EXE SHA-256
values are x86
`bb0735f3317ad713d5969ece3fa821574cc69f47dd0d936583ab914bed229ec8`
and x64
`b64d20464e33326190281bda1ef82747981644ae4bce4f9cc94efd0ba9e7dd79`.
Original structure restoration, coupled OR/value conditions and unobserved
contexts remain under review; the comprehensive audit is not complete.

### S20 breadth-first correction

The owner requires a breadth-first audit of the complete retained mirror
conditional tree. The outermost condition is depth 1; sibling `elif`/`else`
arms remain at the same depth. Process and account for every eligible arm at
a depth before advancing: an arm proven excluded in every supported context
is replaced wholesale by one `#error` and never enqueues descendants; an arm
selected in any supported context retains its original framework and queues
its inner conditions. Unreached observations do not prove exclusion.
Before finishing each depth, independently review every arm at that depth:
verify the supported build contexts and the reason to retain its contents or
replace them with `#error`, including originally omitted frameworks and
selected-content mirror exceptions. Record unresolved cases rather than count
them as completed. This review must not process descendant-arm details;
only after the current depth is complete may the next-depth queue be processed.

Maintain a task-local per-depth work queue and report totals, classified,
processed, pending, and deferred arms. Do not use the global T queue for this
internal S work. Probe mixed or missing contexts separately; the owner has
lifted the OR deferral. Single-macro numeric conditions also require explicit
value and source-point proof, not an assumption from the macro name. Retain
already selected code. Batch source changes and run
builds after the current depth or a meaningful batch, rather than after each
individual arm. The previous selective passes are intermediate work, not
evidence of breadth-first completion.

Initial depth-1 inventory covers 470 retained text C/header files: 2,017
current arms, including 1,030 already containing an `#error`, 564 with prior
selected observations, 41 recognized include guards, and 382 requiring
classification. These are provisional classifications, not completed reviews:
each still requires comparison with the pinned XP structure and proof of its
supported contexts. The original XP inventory contains 2,166 depth-1 arms;
the difference is not itself a count of missing structures because the two
trees differ in local adaptations and scope. The task-local tree and queue are
in `build/t99-s20`; descendants of unclassified arms are not yet eligible.
The three independently confirmed `#if 0` arms were replaced wholesale and
recorded in `m9-t99-s20-depth1-constant-exclusions.tsv`; the branch-structure
scan finds 2,043 `#error` arms and no nested content in those arms. That scan
does not prove the original structures are complete.

An additional actual-command preprocessor probe covered 30 C files in both
architecture build trees: all 60 contexts completed without errors and all
167 formerly unclassified `#ifndef NEC_98` / `#ifndef NTVDM` arms were observed
selected in both architectures. The evidence records the exact source hashes
and locations in `m9-t99-s20-bfs-depth1-selected.tsv`. This reduces the
provisional unclassified depth-1 arms from 382 to 215; it does not prove XP
structure reconciliation or coverage of additional inclusion contexts.
The same probe subsequently checked 158 `#else` arms in C files across 52
architecture contexts without errors: 152 were selected in both builds and
six were not observed, all under coupled OR conditions retained for separate
review. The selected evidence now contains 319 entries and the provisional
unclassified count is 63, including header contexts and deferred expressions.
The header overlay probe then ran 192 actual dependency contexts with zero
preprocessor errors. It observed 24 additional header arms selected on both
architectures, recorded in the same hash-anchored evidence; the provisional
unclassified count is now 39. Six instrumented header arms were not observed;
the probe does not prove those arms unreachable or cover future contexts.
Another 12 architecture contexts confirmed seven remaining simple or negated
AND conditions are selected in both builds. The hash-anchored selected ledger
now has 350 entries and 32 depth-1 arms remain provisionally unclassified.
Those 32 are now explicitly inventoried, not excluded: 30 involve OR groups
and two involve numeric `HOST_BIAS` comparisons, all deferred under the owner
scope. A separate unique-anchor comparison across all retained C/header files
found 18 potential missing XP frameworks; it records 15 coupled OR candidates,
two existing local override cases and one nested-parent case. This heuristic
list is not proof of completeness and cannot substitute for reconciliation of
all XP depth-1 structures.
See `m9-t99-s20-bfs-depth1-deferred.tsv` and
`m9-t99-s20-bfs-baseline-candidates.tsv`.
Expanding the anchor threshold to one matching source line found 131 heuristic
candidates, including 41 at provisional XP lexical depth 1; some are local
overrides or false matches. Eight were independently verified against XP and
actual x86/x64 compile contexts, then their original frames were restored in
`diskbios.c`, `keybd_io.c`, `cmos.c`, `timer.c`, `com.c`, `keyba.c`, and
`idetect.c`. Four excluded NTVDM sibling arms now contain `#error`; selected
code is retained.
See `m9-t99-s20-bfs-depth1-four-guard-restorations.tsv`. The latest structural
scan reports 2,047 error arms and zero mixed arms. Both architecture builds
subsequently passed and Ninja reports no work. Focused testing initially
passed 35/36 on each architecture; `checks/xp_mirror` failed because ten
recent manifest hashes incorrectly used raw CRLF bytes. Correcting those
entries to the gate's normalized-LF hash made that test pass separately on
both architectures. This is targeted evidence, not a full regression.
Four additional XP depth-1 frames were subsequently restored in `com.c`,
`video.c`, `at_dma.c`, and `reset.c`, after both architecture contexts proved
the selected macro state. These newer changes have not yet been built or
tested; the recorded EXEs predate them. The normalized XP manifest gate passes
and a structural scan reports 2,050 error arms with no mixed arms.
Four more depth-1 NEC-98 frames were verified and restored in `video_io.c`,
`at_dma.c`, and two sites in `ica.c`; the selected branch remains unchanged.
The XP manifest gate and whitespace check pass, and the structural scan now
finds 2,054 error arms with no mixed arms. These newest frames still need the
next dual build; the selected `video_io.c` branch also has an original inner
condition to review when depth 2 becomes eligible.
Five further original depth-1 frames were restored after actual dual-context
checks: two adjacent frames in `mouse_io.c` and three NEC-98 frames in
`nt_graph.c`. The XP manifest and whitespace gates pass; the structural scan
now reports 2,058 error arms and zero mixed arms. These changes also await the
next dual build and focused tests.

Latest depth-1 snapshot across the 470 retained C/header files contains 2,051
current arms: 1,044 already contain `#error`, 934 have positive selection
observations, 41 are recognized include guards, and 32 are the explicitly
deferred coupled/value cases. These are inventory dispositions, not proof that
all 1,044 exclusions or XP baseline structures have been independently
reconciled. A provisional task-local depth-2 queue contains 1,334 current
arms beneath positively selected or include-guard parents; six children of
unclassified parents remain held. Depth 2 is not being analyzed yet. The
queue must be refreshed as XP depth-1 reconciliation changes parent structure.
Per owner direction, restore and classify only the current depth, replace an
excluded body as a whole, and enqueue reachable children without investigating
their internal details until the current depth is complete.
The owner additionally requires each depth to be compiled, published, tested,
committed and pushed before moving to the next depth. Depth 1 remains active;
no depth-1 delivery or depth-2 work has been claimed.
For the original `gvi.h` depth-1 frame, all 56 actual x86/x64 dependency
contexts observed `NEC_98` undefined; the original frame was restored with an
excluded `#error`. For the `ios.h` frame, 108 of 110 dependency contexts
observed `NTVDM` undefined via the overlay. The remaining `time_day.c` context
on each architecture was separately checked using actual preprocessor output;
both reached the declaration with `NTVDM` undefined. That original frame was
also restored. The XP manifest and whitespace gates pass, and the structural
scan now finds 2,060 error arms with no mixed arms. This latest source state
still needs the next dual build.
Six original `com.c` NTVDM parity-selection frames were subsequently restored
after 12 x86/x64 location probes all observed NTVDM undefined. The selected
parity expressions remain unchanged. The XP manifest and whitespace gates
pass; 2,066 error arms have no mixed content. This source state also awaits
the depth-1 build and tests.
Two further original depth-1 NEC-98 frames were restored in `gfx_updt.c` and
`reset.c` after x86/x64 location probes confirmed the selected branch. The
existing inner `gfx_updt.c` condition is now depth 2 and its previous root
classification was removed; the missing inner XP condition in `reset.c` is
queued for later reconciliation rather than restored during depth 1. The XP
manifest and whitespace gates pass; 2,068 error arms contain no mixed body.
The `keyba.c` NEC-98 and `tape_io.c` NTVDM/MONITOR depth-1 frames were also
restored after x86/x64 source-position probes. The existing SoftPC BIOS table
segment fix remains in the selected `tape_io.c` branch. Manifest and whitespace
checks pass, and 2,070 error arms contain no mixed body. Depth 1 remains open;
the accumulated source changes require its eventual full delivery validation.
The first-depth whole-omission scan also found XP `nt_graph.c:584-586`:
its `#ifndef NEC_98` branch is selected in the supported builds, while the
sole declaration in its body was explicitly excluded by the S5 subset as
`excluded-NT-stream-state`. The owner directs retaining the selected original
framework, removing or commenting the unwanted content, and recording the
mirror exception. The framework has been restored with a brief explanation;
the unused NT stream global remains omitted and the coding rule is updated.
The other two whole-omission hits are deeper and remain unexamined for now.
An independent first-depth directive-count comparison across all retained
C/header files produced 105 candidate keys / 177 count differences in 49
files. These are review candidates only: equivalent spellings, local overrides
and changed structure can produce false positives. The tracked candidate list
is `m9-t99-s20-bfs-depth1-xp-directive-candidates.tsv`. Two directly verified
omitted `nt_cga.c` MONITOR and NEC-98 frames were restored after x86/x64
location probes; their bodies are `#error`. The adjacent XP JAPAN/KOREA OR
frame remains in the deferred scope. XP manifest and whitespace checks pass,
and the structural scan finds 2,072 error arms without mixed content.
Two additional `nt_cga.c` depth-1 MONITOR frames, covering `TEXT_INCVAL` and
text initialization, were restored after x86/x64 position probes. The XP
manifest and whitespace gates pass and 2,074 error arms have no mixed body;
the pending depth-1 build and tests have not yet run.
Subsequently both architecture builds completed successfully with no remaining
Ninja work; the focused test selection then passed 36/36 on each architecture.
This supersedes earlier pending-build statements for the current intermediate
source state, not the requirement for a full depth-1 delivery. Current literal
EXE SHA-256 values are x86
`dc9f42dcc0689e5baea39231eb3e6b401dedee7231b0db468b36b7483f17b105`
and x64
`ca5c4dad4ea3110930410c8c10d9ec88bae32fa83d61e9b75adf23917fd3a3b5`.
After that checkpoint, two original first-depth `nt_cga.c` tail frames
(`MONITOR` and `NEC_98`) were restored as whole excluded bodies after both
architecture location probes confirmed the conditions false. The XP manifest
and whitespace gates pass; the structural scan finds 2,076 error arms with
no mixed bodies. This newer state requires another dual build and tests.
The original first-depth MONITOR tail frames in `nt_ega.c` and `nt_vga.c`
were subsequently restored as whole excluded bodies after x86/x64 location
probes; no child conditions were retained or inspected. The manifest and
whitespace gates pass, and 2,078 error arms have no mixed body. These changes
also await the depth-1 build and test delivery.
Two further original depth-1 MONITOR frames in `nt_graph.c` were restored as
whole excluded bodies after both architecture position probes confirmed the
condition false. The XP manifest and whitespace checks pass; the structural
scan now reports 2,080 error arms with no mixed bodies. This newer state still
requires the depth-1 build and test delivery.
The original depth-1 `MONITOR` frame in `host/inc/nt_graph.h` was restored as
one excluded body. Actual preprocessor output reached its declaration in all
22 x86/x64 dependency contexts, with MONITOR undefined in each. The manifest
and whitespace gates pass; 2,081 error arms have no mixed bodies. Current
source changes await the depth-1 dual build and tests.
The latest first-depth XP directive comparison found three omitted constant
false frames in `tape_io.c`, `egavideo.h`, and `ica.c`. Their exact original
boundaries were restored with one `#error` body and appropriate `#line`; no
child conditions were examined. The XP manifest and whitespace gates pass and
the structural scan finds 2,084 error arms without mixed content. This source
state awaits the depth-1 dual build and tests.
The selected XP `#ifndef X86GFX` frame around an NT fullscreen clear guard in
`nt_graph.c` has also been restored. The current standalone path deliberately
omits that guard; its existing unsupported-nonstandalone boundary remains,
and the selected-content omission is recorded in the mirror exception ledger.
Both x86/x64 probes confirmed X86GFX undefined. Manifest and whitespace checks
pass; this new source state awaits the depth-1 dual build and tests.
The original first-depth constant-false frame in `nt_sbfm.c` was restored as
one `#error` body, leaving the selected status implementation untouched. The
XP manifest gate passes; the new source state still awaits the depth-1 build.
The two remaining original `#if 0` timer-selection frames in `nt_sbfm.c`
were restored at first depth; their selected immediate-expiry assignments
retain the existing T93 behavior. The excluded bodies each contain one
`#error`. Manifest and whitespace checks pass and 2,087 error arms have no
mixed body; this source state still awaits the depth-1 build and tests.
The original NTVDM-only DMA accessor declaration and implementation frames
were restored at depth 1 as excluded `#error` bodies after position and header
context checks. The already-admitted T93 standalone accessor and declaration
remain outside those frames, preserving the single required DSP integration
path. The manifest and whitespace checks pass and 2,089 error arms have no
mixed body; the resulting state still awaits the depth-1 build and tests.
The subsequent intermediate dual build completed successfully; both Ninja
trees report no work, and the focused mirror/machine/governance selection
passed 36/36 on each architecture. The current published EXE SHA-256 values
are x86 `d0016bb8c0379e233829f232c3397f7999bf58da4d8ca283e84088b3573d7d8e`
and x64 `67acf6a7e6b8f5950af6cfe3db2ee02a8622a41e3d4b3601b8d0aa6cf69cbc25`.
This supersedes prior intermediate artifact hashes and pending-build notes;
depth 1 is still incomplete and needs its final delivery validation.
Three omitted depth-1 `floppy_i.c` frames (two NEC-98, one NTVDM) were then
restored as whole excluded bodies after dual-architecture location probes.
The XP manifest and whitespace checks pass; the structural scan finds 2,092
error arms without mixed bodies. This newer state awaits the depth-1 build.
The original `floppy.h` NTVDM tail frame was restored as one excluded body;
all six actual x86/x64 dependency contexts reached its location with NTVDM
undefined. The XP manifest and whitespace checks pass, and 2,093 error arms
have no mixed bodies. This change awaits the depth-1 build and tests.
An additional original `cmos.c` NTVDM input-path frame was restored as a
single excluded body after x86/x64 location probes. Manifest and whitespace
checks pass; the structural scan finds 2,094 error arms with no mixed bodies.
This change awaits the depth-1 build and tests.
Five additional original depth-1 NTVDM/NTVDM-conjunction frames were restored
in `com.c`, `keyba.c`, and `timer.c`, using unique neighboring XP anchors and
actual x86/x64 source-position probes. Each entire excluded body contains one
`#error`; no child directives were inspected. Manifest and whitespace checks
pass; 2,099 error arms have no mixed bodies. This state awaits the depth-1
build and test delivery.
The original `sas.h` NTVDM-and-not-X86GFX frame was restored as one excluded
body. All 114 actual x86/x64 dependency contexts reached the preceding macro
definition with NTVDM undefined; the manifest and whitespace gates pass and
2,100 error arms have no mixed bodies. This awaits the depth-1 build and tests.
Two original depth-1 NEC-98 frames in `dma.h` were restored as entire excluded
bodies after 14 actual x86/x64 dependency contexts observed NEC_98 undefined.
No nested condition was retained in either excluded body. The XP manifest and
whitespace gates pass, and 2,102 error arms have no mixed bodies; the current
state still awaits the depth-1 build and tests.
The original depth-1 NTVDM tail in `base/support/ios.c` was restored as an
entire excluded body after both architecture position probes. The XP manifest
and whitespace gates pass, and 2,103 error arms have no mixed bodies. This
newer source state awaits the depth-1 build and tests.
The original depth-1 X86GFX tail frame in `host/inc/nt_fulsc.h` was restored
as one excluded body; all four x86/x64 dependency contexts observed X86GFX
undefined at its preceding declaration. Manifest and whitespace checks pass,
and 2,104 error arms have no mixed bodies. This awaits depth-1 build and tests.
Two XP `#ifndef NTVDM` first-depth frames around RTC initialization in
`cmos.c` were restored after both x86/x64 location probes confirmed NTVDM
undefined. Existing inner `PROD` conditions remain untouched and are queued
for depth 2. The XP manifest and whitespace gates pass; current source still
awaits the depth-1 build and tests.
The exact-adjacency omitted-frame heuristic raised `dma.h` XP lines 155 and
261 as candidates. Both are false positives: the BIT_ORDER2 condition and its
selected DMA_COMMAND/DMA_STATUS body are already present. Fourteen actual
x86/x64 dependency contexts observed BIT_ORDER2 defined. Their dispositions
are recorded in `m9-t99-s20-bfs-depth1-whole-omissions.tsv`; no source change
was made and the heuristic result is not treated as a completeness proof.
The same heuristic's `c_reg.c` SPC486 candidate was already restored at its
correct XP-aligned location, and its `dma.h` LITTLEND candidate already has the
selected original definition. Fourteen actual x86/x64 dependency contexts
observed LITTLEND defined. Both are recorded as false positives; neither
caused a source edit.
The first-depth directive-count candidates now record two local-structure
dispositions: `ica.h` retains its existing CPU40-aware local guard rather than
blindly replacing it with XP's `CPU_30_STYLE` test, and one original
`cpu_vid.h` `NO_STRING_OPERATIONS` condition now sits under a selected local
CPU40/C-video parent and is queued for depth 2. No child branch was reviewed.
For `c_main.c`, six XP `#ifdef NTVDM` comparisons correspond to the existing
local `NTVDM || SOFTPC_CCPU_TLS_SIMSTACK` port-ABI condition. Both actual
architecture compile commands define the TLS simstack selector and do not
define NTVDM. This selected local condition is retained and recorded in the
candidate ledger rather than replaced by an NTVDM-only condition; no source
change was made.

Latest refreshed current-tree depth-1 inventory has 2,099 arms: 1,083 contain
`#error`, 944 have positive selection evidence, 41 are recognized include
guards, and 31 are deferred coupled/value cases. The 11 newly restored selected
arms were joined to their previously acquired location evidence. These are
classification counts, not a claim that all original XP frames have been
reconciled. The provisional depth-2 queue now has 1,348 current arms beneath
selected parents; six remain held behind unclassified parents. The queue will
be refreshed again before depth-1 delivery; no depth-2 details were audited.

The original depth-1 NEC-98 frame in `host/inc/nt_cga.h` was restored as one
excluded body. Four x86/x64 dependency contexts reached the preceding macro
with NEC_98 undefined. The XP manifest and whitespace checks pass, and 2,105
error arms have no mixed bodies; the source still awaits depth-1 build/tests.

Four original depth-1 conditions in `host/inc/nt_graph.h` were restored after
22 x86/x64 contexts confirmed NEC_98 undefined: three excluded NEC-98 bodies
and the selected `#ifndef NEC_98` framework around the already-excluded NT
stream declaration. The selected-path omission follows the owner's mirror
exception rule. Manifest and whitespace checks pass; 2,108 error arms have no
mixed body. This state awaits the depth-1 dual build and tests.

The original depth-1 NEC-98 CMOS-adapter frame in `base/inc/ios.h` was
restored as an excluded body. All 110 actual x86/x64 dependency contexts
reached the adjacent macro with NEC_98 undefined. Manifest and whitespace
checks pass; 2,109 error arms have no mixed bodies. This source state awaits
the depth-1 build and tests.

Two original depth-1 NEC-98 frames in `host/src/nt_sound.c` were restored as
whole excluded bodies following x86/x64 position probes. The selected sound
path is unchanged. Manifest and whitespace checks pass; 2,111 error arms have
no mixed bodies. This source state awaits the depth-1 build and tests.

Three original first-depth NEC-98 frames in `host/src/nt_keycd.c` were
restored as entire excluded bodies after x86/x64 position probes; the selected
scan-code range checks remain unchanged. The XP manifest and whitespace gates
pass, and 2,114 error arms have no mixed bodies. This source state awaits the
depth-1 build and tests.

The subsequent intermediate x86/x64 builds both completed successfully with
no remaining Ninja work, and the focused mirror/machine/governance selection
passed 36/36 on each architecture. Current literal published EXE SHA-256:
x86 `48a9ff5ed4374a969801db49a463fecb0c4568016786c4b75c3520b226059c32`,
x64 `ab0c4e7531bb8791cdbf697710706eafa438cc962c9b6a40a790e6d4b3e38257`.
This supersedes earlier intermediate pending-build notes, not the required
depth-1 final delivery; the first-depth audit remains incomplete.

The original `vga_mode.c` depth-1 NEC-98 tail frame was restored as a single
excluded body after x86/x64 location probes. Manifest and whitespace checks
pass and 2,115 error arms have no mixed bodies. This change awaits depth-1
build and tests.

The original first-depth NEC-98 include frame in `base/video/video.c` was
restored as an excluded body after x86/x64 source-position probes. Manifest
and whitespace checks pass; 2,116 error arms have no mixed bodies. This state
awaits the depth-1 build and tests.

The original first-depth NEC-98 tail in `base/inc/sas.h` is restored as one
excluded body. All 114 actual x86/x64 header compilation contexts reached the
adjacent retained definition with NEC_98 undefined. The mirror and whitespace
gates pass; the structural scan finds 2,117 error arms without mixed bodies.
The depth-1 compilation, full tests, publication and commit remain outstanding.

The original first-depth NEC-98 tail in `base/inc/bios.h` is restored as one
excluded body after all 70 actual x86/x64 header contexts reached the adjacent
retained declaration with NEC_98 undefined. The mirror and whitespace gates
pass; the structural scan finds 2,118 error arms without mixed bodies. Original
directive counts are candidates only: the `cpu_vid.h` NO_STRING_OPERATIONS
frame remains present within the existing local ABI frame, and the `c_reg.c`
SPC486 candidate is already restored at its true location.

Two original first-depth NEC-98 alternative frames in `base/inc/dma.h` have
been restored while retaining the existing non-NEC definitions. All 14 actual
x86/x64 header contexts reached the retained adjacent macro with NEC_98
undefined. The mirror and whitespace gates pass; the structural scan finds
2,120 error arms without mixed bodies. Depth-1 build and full tests remain due.

Two first-depth NEC-98 bodies in `base/inc/gvi.h` were restored as excluded
outer frames, with no nested directives retained. All 56 actual x86/x64
header contexts reached the adjacent declaration with NEC_98 undefined; the
header has no intervening NEC_98 definition or include. Mirror and whitespace
gates pass and the structural scan reports 2,122 error arms without mixed
bodies. The owner-required point-by-point audit remains due for this depth.

The first-depth NEC-98 tail in `base/comms/printer.c` is restored as one
excluded outer body after source-position probes showed NEC_98 undefined in
both actual x86 and x64 commands. Mirror and whitespace gates pass and the
structural scan reports 2,123 error arms without mixed bodies. Original NTVDM
directive-count differences in `c_main.c` include existing TLS adaptation and
are not evidence that those active expressions should be mechanically changed.

The first-depth NEC-98 tails in `base/disks/floppy.c` and
`base/video/gfx_updt.c` are restored as one excluded body each. Both actual
x86/x64 source-position probes found NEC_98 undefined. Mirror and whitespace
gates pass; the structural scan reports 2,125 error arms without mixed bodies.
First-depth point-by-point review and the required full qualification remain.

The first-depth NEC-98 tail in `base/bios/reset.c` is restored as one excluded
outer body. Actual x86 and x64 source-position probes showed NEC_98 undefined.
The mirror and whitespace gates pass; the structural scan reports 2,126 error
arms without mixed bodies. Depth-1 point-by-point audit and qualification are
still required.

The current batch has passed intermediate x64 and x86 builds, with both Ninja
trees reporting no pending work afterward. This does not qualify depth 1:
point-by-point review, full tests, publication, commit and push remain due.

First-depth excluded tail frames were restored in `base/keymouse/keyba.c`
(NEC_98) and `base/inc/egavideo.h` (NTVDM and MONITOR). Actual x86/x64 probes
found NEC_98 undefined at the source point and NTVDM undefined in all 18 header
contexts, respectively. Mirror and whitespace gates pass; the structural scan
reports 2,128 error arms without mixed bodies. These changes postdate the
intermediate dual build and await the depth-1 qualification.

The first-depth NEC-98 tail in `base/disks/floppy_i.c` was restored as one
excluded body after x86 and x64 source-point probes confirmed NEC_98 undefined.
Mirror and whitespace gates pass; the structural scan finds 2,129 error arms
without mixed bodies. This change also postdates the intermediate dual build.

The first-depth NEC-98 post-operation frame in `base/disks/floppy_i.c` was
restored as one excluded body after both x86 and x64 source-point probes found
NEC_98 undefined. Mirror and whitespace gates pass; the structural scan finds
2,130 error arms without mixed bodies. Full depth-1 review and qualification
remain outstanding.

The original first-depth NEC-98 initialization-constant alternative in
`base/system/ica.c` was restored after actual x86/x64 source-point probes
found NEC_98 undefined. Existing selected PC constants remain unchanged; the
original absent nested NTVDM frame is queued for depth 2. Mirror and
whitespace gates pass; the structural scan reports 2,149 error arms without
mixed bodies. Compilation and depth-1 qualification remain outstanding.

Three original first-depth NEC-98 frames in `base/bios/reset.c` were restored
around device and keyboard initialization: two excluded bodies and the
selected non-NEC keyboard-init body. Actual x86/x64 source-point probes found
NEC_98 undefined at both sites. Mirror and whitespace gates pass; the
structural scan finds 2,132 error arms without mixed bodies. The selected
keyboard initialization retains its existing order and code.

Two first-depth NEC-98 frames in `base/keymouse/mouse.c` were restored around
mouse interrupt delivery and the unchanged-motion branch. Both actual x86/x64
source-point probes found NEC_98 undefined; the selected PC arm retains the
existing mouse-mode and HOLD checks. Mirror and whitespace gates pass; the
structural scan reports 2,134 error arms without mixed bodies. These edits
await the depth-1 build and full qualification.

Two original first-depth NEC-98 alternative frames around address and count
reads in `base/system/at_dma.c` were restored. Actual x86/x64 source-point
probes found NEC_98 undefined; the selected address/count reads and existing
Sound Blaster position queries retain their order and local fixes. Mirror and
whitespace gates pass; the structural scan finds 2,136 error arms without
mixed bodies. These edits await dual compilation and full depth-1 review.

The original first-depth NEC-98 ROM_START override frame in
`base/system/at_dma.c` was restored as one excluded body after both actual
x86/x64 source-position probes showed NEC_98 undefined. Mirror and whitespace
gates pass; the structural scan reports 2,137 error arms without mixed bodies.
The refreshed original-directive heuristic reports 65 keys, 104 candidate
differences in 33 files; candidates still require individual classification.

The first-depth NEC-98 page-register read frame in `base/system/at_dma.c`
was restored with the existing PC cases retained. Actual x86/x64 source-point
probes showed NEC_98 undefined. Mirror and whitespace gates pass; the
structural scan reports 2,138 error arms without mixed bodies. The batch still
requires dual compilation and the complete first-depth qualification.

The original first-depth ROM filename NEC-98 alternative in
`base/system/rom.c` was restored after x86/x64 source-point probes showed
NEC_98 undefined. Existing non-NEC filename definitions are retained; their
five immediate conditional arms are queued for depth 2, not examined in this
depth. Mirror and whitespace gates pass; the structural scan reports 2,139
error arms without mixed bodies. Compilation and depth qualification remain.

Four original first-depth NEC-98-related frames in `base/disks/diskbios.c`
were restored: three excluded bodies and the selected `IDLE_disk()` frame.
The actual x86/x64 source-position probes found NEC_98 undefined at the three
relevant positions. Mirror and whitespace gates pass; the structural scan
reports 2,142 error arms without mixed bodies. Existing disk capacity and
register logic was not changed. Dual compilation and full qualification remain.

Two original first-depth NEC-98 frames in `base/system/timer.c`, around serial
baud handling and a counter-local declaration, were restored as excluded
bodies. Both x86/x64 source-point probes found NEC_98 undefined; the existing
selected timer path remains intact. Mirror and whitespace gates pass and the
structural scan reports 2,144 error arms without mixed bodies. Compilation and
depth qualification remain due.

Three first-depth NEC-98 frames in `base/comms/com.c` were restored after
actual x86/x64 source-point probes found NEC_98 undefined. The selected
non-NEC send-wait function remains unchanged; its original absent nested
NTVDM guard is explicitly queued for depth 2. Mirror and whitespace gates
pass; the structural scan finds 2,147 error arms without mixed bodies.
Dual compilation and full depth-1 qualification remain outstanding.

The original first-depth NEC-98 `SetRSBaud` frame in `base/comms/com.c` was
restored as one excluded body after both source-position probes found NEC_98
undefined. Mirror and whitespace gates pass; the structural scan reports
2,148 error arms without mixed bodies. Compilation and full qualification
remain outstanding.

Two original first-depth NTVDM frames around `host_reset()` in
`base/bios/reset.c` were restored after both actual source-position probes
found NTVDM undefined. The non-NTVDM host reset remains in its existing order;
the NTVDM arm is an excluded body. Mirror and whitespace gates pass, with
2,150 structurally clean error arms. Dual compilation and depth-1 review and
qualification remain outstanding.

Three first-depth NEC-98-related frames were restored in
`host/src/nt_graph.c`: two excluded bodies and the selected non-NEC NT Console
stream frame. Actual x86/x64 source-point probes found NEC_98 undefined. The
selected frame's omitted native host streaming function has no project source
caller and is registered as a standalone-presentation mirror exception.
Mirror and whitespace gates pass; the structural scan reports 2,152 error
arms without mixed bodies. Dual compilation and depth qualification remain.

The original first-depth NEC-98 graphics-resize alternative in
`host/src/nt_graph.c` was restored after both source-point probes found NEC_98
undefined. The selected non-NEC arm records the existing standalone omission
of the NT fullscreen return as a mirror exception; its existing local
standalone condition is queued for depth 2. Mirror and whitespace gates pass;
the structural scan reports 2,153 error arms without mixed bodies. Dual
compilation and full depth-1 qualification remain outstanding.

Two first-depth NEC-98 frames around graphics screen-buffer activation in
`host/src/nt_graph.c` were restored after actual x86/x64 source-position probes
found NEC_98 undefined. The existing activation call remains between the two
excluded bodies. Mirror and whitespace gates pass; the structural scan reports
2,155 error arms without mixed bodies. Dual compilation and depth-1 review and
qualification remain outstanding.

The refreshed first-depth inventory has 2,143 current arms. Merging the
existing source-point selection and explicit deferred ledgers yields 1,116
error arms, 955 selection-evidenced arms, 41 structural include-guard arms,
and 31 deferred arms; ledger locations and raw file hashes match the current
tree. This is an inventory classification, not the required independent
point-by-point selection/exclusion audit or proof that every original frame
has been recovered. No children of excluded arms are admitted to the next
depth; deferred arms remain unresolved.

The selected first-depth `#ifndef NEC_98` controller-selection frame in
`base/system/at_dma.c` was restored around the existing PC code. Both actual
x86/x64 source-point probes showed NEC_98 undefined. Mirror and whitespace
gates pass; no selected code or DMA behavior was intentionally changed.

The first-depth reconciliation has identified two original child guards that
are absent from currently retained selected arms; their XP positions and
current source hashes are preserved in
`docs/etc/evidence/softpc/m9-t99-s20-bfs-depth2-absent-original-children.tsv`
for depth 2. This supplements, and does not replace, the queue generated from
all retained first-depth arms after the first-depth audit.

The selected first-depth `#ifndef NEC_98` tail in `base/support/time_day.c`
was restored after both source-position probes found NEC_98 undefined. Its
original missing nested NTVDM condition is recorded in the depth-2 absent-child
ledger; its contents were not processed at depth 1. Mirror and whitespace
gates pass. Dual compilation and depth qualification remain outstanding.

The original selected first-depth non-NEC stream frame in
`base/video/gfx_updt.c` was restored after both x86/x64 source-position probes
showed NEC_98 undefined. The original missing NTVDM child is recorded in the
depth-2 absent-child ledger without processing its body. Mirror and whitespace
gates pass; this source change awaits dual compilation and depth qualification.

The current depth-1 batch passed intermediate x64 and x86 builds and 36/36
targeted tests per architecture; both Ninja trees reported no pending work.
This is not the required full depth-1 audit, full regression or publication.

The original selected first-depth non-NEC stream declaration frame in
`base/inc/gfx_upd.h` was restored after all 58 actual x86/x64 header contexts
found NEC_98 undefined. Its original missing NTVDM child is recorded in the
depth-2 absent-child ledger. Mirror and whitespace gates pass; this edit still
requires dual compilation and full depth-1 qualification.

The original selected first-depth non-NEC stream-state frame in
`base/video/video.c` was restored after x86/x64 source-position probes found
NEC_98 undefined. The original missing NTVDM child is recorded in the depth-2
absent-child ledger without processing its contents. Mirror and whitespace
gates pass; compilation and full depth-1 qualification remain due.

The first-depth NTVDM frame for empty-port input in `base/support/ios.c` was
restored as an excluded body after x86/x64 source-position probes found NTVDM
undefined; the existing default-value path remains unchanged. Mirror and
whitespace gates pass; the structural scan finds 2,156 error arms without
mixed bodies. Dual compilation and full depth-1 qualification remain due.

The original first-depth NTVDM declaration frame in `base/disks/floppy.c` was
restored as one excluded body after x86/x64 source-point probes found NTVDM
undefined. Mirror and whitespace gates pass; the structural scan finds 2,157
error arms without mixed bodies. Dual compilation and full depth-1 review and
qualification remain outstanding.

Five first-depth NTVDM-related frames in `base/keymouse/keybd_io.c` were
restored as excluded bodies, including two conjunctions whose NTVDM operand
is false in both actual x86/x64 source-position probes. Existing keyboard
handling remains unchanged. Mirror and whitespace gates pass; the structural
scan finds 2,162 error arms without mixed bodies. Dual compilation and full
depth-1 qualification remain outstanding.

Three original first-depth NTVDM/KOREA frames in `base/keymouse/keyba.c` were
restored as excluded bodies after actual x86/x64 source-position probes found
the corresponding macros undefined. Keyboard input behavior remains unchanged.
Mirror and whitespace gates pass; the structural scan finds 2,165 error arms
without mixed bodies. Dual compilation and depth-1 qualification remain due.

Six original first-depth NTVDM/JAPAN frames around mouse installation in
`base/keymouse/mouse_io.c` were restored as excluded bodies. Actual x86/x64
source-position probes found the respective macros undefined; existing mouse
installation and string paths remain unchanged. Mirror and whitespace gates
pass; the structural scan finds 2,171 error arms without mixed bodies. Dual
compilation and depth-1 review and qualification remain outstanding.

Three original first-depth NTVDM frames in `base/system/timer.c` were restored
after x86/x64 source-position probes found NTVDM undefined. The selected
`checktimelock()` alternative stays unchanged; the other NTVDM bodies are
excluded. Mirror and whitespace gates pass and the structural scan finds
2,174 error arms without mixed bodies. Dual compilation and full depth-1
qualification remain due.

Two original first-depth NTVDM frames in `base/system/ica.c` were restored
after actual x86/x64 source-position probes found NTVDM undefined. The selected
hook definition and IRET/delayed-interrupt local fix remain intact; the
existing nested hook condition is queued for depth 2. Mirror and whitespace
gates pass; the structural scan finds 2,175 error arms without mixed bodies.
Dual compilation and complete depth-1 qualification remain outstanding.

The original first-depth NTVDM-and-MONITOR frame in `base/system/illegalp.c`
was restored as an excluded body after both x86/x64 source-position probes
found NTVDM undefined. Mirror and whitespace gates pass; the structural scan
finds 2,176 error arms without mixed bodies. Dual compilation and full depth-1
qualification remain outstanding.

The original first-depth NTVDM serial line-status frame in `base/comms/com.c`
was restored as one excluded body after both actual source-position probes
found NTVDM undefined. Its nested conditions remain excluded with the body;
the existing serial path is unchanged. Mirror and whitespace gates pass; the
structural scan finds 2,177 error arms without mixed bodies. Dual compilation
and depth-1 qualification remain outstanding.

The original first-depth `#if 1` and `#if 0` DMA-progress frames in
`host/src/nt_sbdsp.c` were restored without changing the selected T93 logic.
The excluded constant-false body is a single `#error`; its selected sibling's
missing original DESCENT child is recorded for depth 2. Mirror and whitespace
gates pass; the structural scan finds 2,178 error arms without mixed bodies.
Dual compilation and full depth-1 qualification remain outstanding.

The latest intermediate batch passed x64 and x86 builds and 14/14 targeted
Sound Blaster, DMA and audio unit tests per architecture. This is not the
first-depth point-by-point review, full regression or publication.

Three original first-depth NTVDM frames in `base/system/timer.c` were restored
after x86/x64 source-position probes found NTVDM undefined. Two selected
non-NTVDM frames retain their existing nested conditions for depth 2; one
NTVDM body is excluded. Mirror and whitespace gates pass; the structural scan
finds 2,179 error arms without mixed bodies. Compilation and full depth-1
qualification remain outstanding.

The original first-depth NTVDM-and-FIFO_ON timeout frame in
`base/comms/com.c` was restored as an excluded body after both x86/x64
source-position probes found NTVDM undefined. Mirror and whitespace gates
pass; the structural scan finds 2,180 error arms without mixed bodies. Dual
compilation and full depth-1 qualification remain outstanding.

The first-depth error-arm review now has independent temporary source probes:
all 916 current root `#error` arms in 81 compiled mirror `.c` files were
checked against 164 actual x86/x64 compilation contexts. Every source was
reached, no probed error arm was selected, and all preprocessors exited 0.
Per-arm current lines, source hashes and context counts are recorded in
`docs/etc/evidence/softpc/m9-t99-s20-bfs-depth1-error-source-audit.tsv`.
This proves only those recorded `.c` contexts; original frame completeness,
other supported contexts and the depth qualification remain under review.

The XP `nt_sound.c` `#ifndef PROD` diagnostic framework has been restored
at depth 1 with an excluded `#error` body. Both actual architecture commands
define `PROD`; targeted probes reached all three root error arms in the file
without selecting any. The surrounding NT beep-device function remains the
previously replaced host endpoint, not a newly restored implementation. The
structural audit reports 2,181 clean error arms and the 475-body mirror gate
passes. The affected `nt_sound.c` object also compiles on x86 and x64. Full
dual builds and depth-1 qualification remain outstanding.

The missing XP `nt_keycd.c` ordinary-key `#ifdef JAPAN` framework was restored
with an excluded `#error`; the enhanced-key framework already existed and its
following `#line` was corrected to XP line 804. `JAPAN` is not selected in the
recorded x86/x64 contexts. All seven current root error arms in this file pass
targeted dual-architecture probes, and its x86/x64 objects compile. The
structural audit now finds 2,182 clean error arms; the mirror gate passes.
Depth-1 qualification and complete builds, testing and publication are pending.

In `rom.c`, XP closes the `read_rom` `NEC_98` branch immediately before the
function's closing brace. The worktree had closed it after the later
`do_search_for_roms` loop instead, incorrectly nesting later conditions. The
existing `#endif` was moved back to the XP boundary and the two following
line controls were corrected. All 13 current root error arms in that file
passed the two actual-architecture probes; both architecture objects compile
with mirror warnings. Structural and mirror gates pass. Reclassification of
newly exposed root arms remains required before this depth can finish.

An original/current matched-boundary scan produced one additional candidate,
`ppi.c`: inspection shows its original outer `#ifndef NTVDM` is already closed
at the correct boundary; the apparent mismatch is an extra inner local guard,
not the `rom.c` misplaced-closure defect. The original speaker-data declaration
was omitted by the existing standalone audio port, which uses the full PPI
state path; the selected-content exception is recorded in the depth-1 ledger.
The inner guard belongs to the next-depth review, not this root audit. The
comparison only covers original openings and closings that map unambiguously;
it is not a proof that all original frameworks exist.

`tape_io.c` lacked two XP root `JAPAN && i386` frames: the native header block
and the Japan-specific extended-memory implementation. Both are now restored
as excluded branches because `JAPAN` is absent in the actual x86/x64 builds.
All 14 current root error arms passed those architecture probes and both
architecture objects compile with mirror warnings. Structural and 475-body
mirror gates pass. This brings the current inventory to 2,201 root arms, of
which 1,163 are error arms; full depth-1 review and qualification are pending.

`video.c` also lacked XP's root Japan-only state frame and its separate
Japan/Korea sibling frame. Both original frames are restored; each excluded
sibling has one `#error` and no nested directives. The affected retained
line controls were aligned to XP lines 178, 670 and 671. All 11 root error
arms in this file passed the actual x86/x64 probes, and both objects compile
with mirror warnings. The structural check reports 2,190 clean error arms and
the 475-body mirror gate passes. The current root inventory is 2,204 arms,
including 1,166 error arms; original-frame review remains incomplete.

The XP file-opening `ega_vide.c` `!i386 && JAPAN` frame was missing; it is
restored as one excluded branch because `JAPAN` is absent in both recorded
architectures. The following original line number is restored. Its two actual
architecture probes and both object builds pass, with mirror warnings. The
structural scan reports 2,191 clean error arms and the mirror gate passes;
full depth-1 qualification remains pending.

In `mouse_io.c`, the missing XP `NTVDM && MONITOR` outer frames around the
native mode-change declaration and conditional-off notification have been
restored with excluded bodies. The actual x86/x64 probes reached all 52 root
error arms without selecting them; both objects compile with mirror warnings.
The structural scan reports 2,193 clean error arms and the 475-body mirror gate
passes. Current depth-1 inventory: 2,207 arms, including 1,169 error arms;
the remaining original-frame review and full qualification are outstanding.

The original `mouse_io_interrupt` outer `#ifndef NEC_98` around an NTVDM
stream-disable call was also absent. It is restored as a selected frame; the
native stream content remains omitted and is recorded as a mirror exception.
The original inner `#ifdef NTVDM` is recorded for depth 2, without processing
it now. The selected outer frame was observed in both actual architecture
probes, all 52 root error arms remain unselected, both objects compile with
mirror warnings, and the structure and mirror gates pass.
Current root accounting is 2,208 arms: 1,169 error arms, 972 observed selected
arms, 37 include guards and 30 deferred. This accounting does not complete
the outstanding comparison with the pinned original.

The XP `nt_cga.c` root NEC-98 text-initializer frame has been restored as
an excluded branch, with the next retained original line numbered correctly.
All 15 root error arms in that file were reached without selection in both
actual architecture contexts; both objects compile. The structural scan
reports 2,194 clean error arms and the 475-body mirror gate passes. Depth-1
original-framework review, full builds, tests and publication remain pending.

The missing XP `ios.h` root `#ifdef NTVDM` extension-type frame is restored
as an excluded branch, and the retained NEC-98 alternative's `#line` is
aligned with XP line 301. Probes reached this header in all 55 recorded
contexts per architecture, with no selected error arm; a dependent object
compiles for each architecture with mirror warnings. The structural scan
reports 2,195 clean error arms and the mirror gate passes. Full depth-1
review, builds, tests and publication remain outstanding.

The missing `mouse_set_position` NTVDM outer frame is restored as one excluded
branch, preserving the existing standalone cursor-update path. The selected
mouse stream frame and all 53 root error arms were probed in both actual
architecture contexts; both affected objects compile with mirror warnings.
The structural check now reports 2,196 clean error arms, and the 475-body
mirror gate passes. Root review and full qualification remain unfinished.

The second original `nt_sbdsp.c` root `#if 0` was inside the former native
WaveOut reader thread, already replaced by the current audio endpoint. Its
constant-false framework is retained with a single `#error`; no old thread
or device path was restored. Both architecture objects compile, and targeted
preprocessing observes neither of the file's two root error arms. The structure
scan reports 2,197 clean error arms and the mirror gate passes. The original
framework audit and complete layer qualification remain unfinished.

After the latest restorations, the XP/current root-directive comparison has
19 unmatched expression groups representing 31 candidate directives in 13
files. The individually classified pending list is
`docs/etc/evidence/softpc/m9-t99-s20-bfs-depth1-original-unresolved.tsv`.
It distinguishes existing local combined-condition adaptations from original
OR and numeric-value cases deferred under the current owner scope. Counts are
heuristic comparison candidates, not a claim that each directive is missing;
no item is treated as resolved merely by being listed.

Source-point probes evaluated seven missing original OR frames using the actual
x86/x64 commands: the whole-file CGA frame, four ROM frames, and two mouse
frames were selected in both architectures. Their hashes and observed states
are recorded in `docs/etc/evidence/softpc/m9-t99-s20-bfs-depth1-original-or-probes.tsv`.
These frames are genuinely absent, not false branches; the previously deferred
coupled-condition scope still applies. In particular, restoring the CGA frame
would change the apparent depth of its existing child conditions, so the
current depth-2 queue is provisional and cannot yet qualify the layer.

Ten more missing XP OR/AND frames were evaluated at corresponding source
locations in `video.c`, `nt_cga.c` and `nt_graph.c` using both actual compiler
commands. Every complete expression was unselected in both architectures.
The two-architecture evidence is appended to the original-OR probe ledger.
No OR frame was modified: the owner-deferred coupled scope still applies, and
these observations do not establish behavior for unrecorded contexts.

The 31 already-present deferred root arms were independently instrumented at
their current source positions. Across actual x86/x64 source contexts, one of
25 source arms was selected on both architectures and the other 24 were not
observed. Six header arms were not observed in 144 recorded contexts; six
overlay misses were rechecked using a fresh source-tree copy. Per-arm hashes,
context counts and observations are in
`m9-t99-s20-bfs-depth1-deferred-source-observations.tsv` and
`m9-t99-s20-bfs-depth1-deferred-header-observations.tsv`. These are bounded
observations only: deferred expressions and original frames are not yet
modified or considered fully audited.

Correction to the earlier 37-include-guard count: matching each candidate's
actual `#endif` and file boundary shows 22 structural header guards and 15
ordinary default-macro conditions. All five defaults in `.c` files and all ten
header defaults are selected in at least one actual context on each
architecture. Four header defaults are selected in two of three contexts on
each architecture, so they must remain conditional. The 548 header-context
probes and 12 fresh-tree confirmation probes completed successfully; the
per-point counts are recorded in `m9-t99-s20-bfs-depth1-default-contexts.tsv`.
The provisional second-depth queue is unchanged because
none of the default conditions contains nested directives. The per-point
classification is `m9-t99-s20-bfs-depth1-guard-classification.tsv`. Earlier
counts calling all 37 header guards must not be used as semantic evidence.
The corrected current-tree accounting is 2,212 arms: 1,173 excluded arms,
987 selected arms, 22 structural include guards and 30 deferred arms. This
does not resolve missing original frames or complete the depth-1 audit.
All 22 structural guards were additionally matched against their pinned XP
`#ifndef`, paired `#define`, and matching `#endif`, with both source hashes
and locations recorded in `m9-t99-s20-bfs-depth1-guard-xp-comparison.tsv`.
This verifies those outer guard identities and boundaries, not their contents
or any of the remaining unresolved original frameworks.
An independent current-tree ledger integrity check matched all 1,173 excluded,
987 selected, and 22 structural guard entries to their current file hashes,
directive locations and recorded context evidence. The remaining 30 root
arms all have deferred entries; the deferred ledger has one additional entry
that overlaps an already-selected arm. This check validates the recorded
classification inputs and coverage, not the correctness of missing XP frames
or the ultimate disposition of deferred conditions.
An additional exact-line XP-to-current alignment checked the depth of 1,645
unchanged original root directives: 1,644 remain at depth 1; the one now at
depth 2 is `cpu_vid.h`'s `NO_STRING_OPERATIONS`, already recorded as coupled
to the local C-VID ABI condition. This alignment does not account for original
directives deleted or textually changed, so it does not replace the unresolved
original-frame ledger or authorize a change to that ABI adaptation.
The complementary exact-line scan now inventories 58 original root starts
that did not align verbatim (out of 1,703 original root starts, excluding
sibling arms). They are location-review candidates, not 58 confirmed missing
frames: same-condition occurrences at other locations are not proof of a
match. One candidate, XP `host/inc/nt_uis.h:114`, was verified at the current
DBCS exclusion with the following `#line`. A further 16 were matched to their
current root frameworks using original end boundaries, `#line`, and the
existing context ledgers: 13 excluded, three selected. Their inner conditions
are not reviewed here; known missing direct children remain in the depth-2
queue. Eleven more original starts were then matched by their enclosing
first-depth frame and source context: nine excluded and two selected. The
other 30 have now been individually matched to the unresolved ledger: eight
original starts correspond to existing local combined-condition adaptations,
17 are deferred coupled-OR frames (seven observed selected and ten unselected
in both architectures), and five are deferred numeric conditions. Those are
classifications, not fixes or authorization to alter the deferred cases.
File hashes, candidate locations, and dispositions
are recorded in `m9-t99-s20-bfs-depth1-xp-unaligned.tsv`.
One deferred case materially affects BFS accounting: XP `cga.c:4` is a
selected outer OR frame enclosing the file after its first two includes.
It is absent from the current subset. Its restoration would move the 22
currently counted first-depth arms inside that frame to depth 2 and move the
three currently queued depth-2 arms to depth 3. Therefore the current counts
and queue are explicitly provisional, and the first-depth audit cannot claim
completion from current-tree classification alone. The frame remains
unchanged under this packet's coupled-condition deferral.
The owner has since lifted that deferral for complete OR expressions. The
original selected `cga.c` outer condition and its matching `#endif` are now
restored, with `#line 5` at the original body boundary; the already-retained
body is unchanged. Current-tree depth 1 consequently has 2,191 arms, and the
provisional depth-2 queue has 1,376 arms; 22 former root arms now belong to
that queue and its three former depth-2 children are no longer in it. The
XP manifest gate and whitespace checks pass, as do subsequent cached x64/x86
builds; the builds refreshed both EXEs. Other original OR frames remain in
the first-depth audit, so this is not layer completion or final regression.
Four further original selected OR frames were restored in `rom.c` at the
original `search_for_roms`, `rom_checksum`, `patch_rom`, and `display_string`
boundaries. The locally adapted `display_string` cursor update stays outside
the original OR frame. Before changing the source, temporary x86/x64 drafts
produced byte-identical `-E -P` preprocessor output to the respective current
source. The original `ARCX86` direct child of the display frame is absent and
has been queued for depth 2 without processing its body. Refreshed current
depth-1 accounting is 2,190 arms: 1,157 excluded, 981 selected, 22 include
guards and 30 unresolved; the current depth-2 queue has 1,378 arms plus nine
separately recorded absent original children. All current root ledger hashes,
locations and classifications match; XP mirror and whitespace gates pass.
The intermediate EXEs predate the `rom.c` restoration and need a rebuild.
The two selected original OR frames in `mouse_io.c` have also been restored;
the original unselected `else` now contains one `#error`. Temporary x86/x64
draft preprocessing matched the prior source byte for byte with `-E -P`.
Thus all seven original OR frames observed selected in both architectures
have been restored. The ten original OR frames observed unselected have also
been restored in `video.c`, `nt_cga.c`, and `nt_graph.c` with their excluded
bodies replaced by `#error`; the one original selected `else` retains its
existing code. x86/x64 `-E -P` preprocessing matched before/after for each
file. Explicit rejected-configuration probes triggered all ten target errors
on each architecture; their concise records are in
`m9-t99-s20-bfs-depth1-restored-or-negative.tsv`. Current depth 1 has 2,201
arms: 1,167 excluded, 982 selected, 22 structural guards and 30 unresolved
current conditions. The depth-2 queue has 1,381 present arms plus nine
separately recorded missing original direct children. Root ledger integrity,
the XP mirror gate and whitespace check pass; current EXEs predate these
restorations and must be rebuilt.
After restoration of those 17 original OR frames, cached x86/x64 full-project
builds passed and refreshed both EXEs. The focused App unit plus XP mirror
selection passed 49/49 on each architecture. At that intermediate point the
EXE SHA-256 values were x86
`06475283511636f36654994384d7ec58b35d72f175b8813e9e428e18825a833c`
and x64
`836b593b58f940a869633da861bed08c21122508c0d87901b33006fb1788165f`.
This is not the required full first-depth delivery or desktop regression.
Eleven `NTVDM || macintosh` first-depth arms in `cmos.c`, each unselected in
recorded x86/x64 contexts, were then reduced to one `#error` each. Their XP
condition/sibling boundaries match in order; no removed body carried a local
divergence marker, and all 11 removed-body hashes match their pinned XP
counterparts byte for byte. The selected siblings retain their contents and receive
original-boundary `#line` control. Before application, x86/x64 temporary
drafts had byte-identical `-E -P` output to the prior source. Negative probes
with `macintosh` set triggered all 11 intended errors on each architecture.
Per-arm deletion hashes and probe counts are in
`m9-t99-s20-bfs-depth1-cmos-or-conversions.tsv` and
`m9-t99-s20-bfs-depth1-cmos-or-negative.tsv`. Current root accounting is
2,201 arms: 1,178 excluded, 982 selected, 22 structural guards, and 19 still
unresolved. Root integrity, XP mirror and whitespace checks pass. The current
EXEs predate this CMOS batch.
Thirteen further currently present original OR arms across 13 files were
reduced to single `#error` bodies. Eleven removed bodies matched their XP
counterparts byte for byte; `keybd_io.c` and `timer.c` had prior subset
omissions inside the now wholly excluded bodies. The existing selected
siblings were preserved. Before applying, the seven source drafts matched
the previous `-E -P` output on both architectures, and one actual dependency
context for each of the six header drafts matched on each architecture.
Negative probes reached 24 target errors in actual source/compiler contexts;
for ICA, whose source defines `HOOKED_IRETS` internally, two task-local
copies used an immediately preceding `#undef` and each reached the target.
Records are in `m9-t99-s20-bfs-depth1-current-or-conversions.tsv` and
`m9-t99-s20-bfs-depth1-current-or-negative.tsv`.

The four unselected `else` bodies of the local CCPU TLS OR adaptation were
also reduced to single `#error` bodies, leaving the selected TLS path intact.
Each removed body matched its original XP NTVDM `else` byte for byte; x86/x64
draft preprocess outputs were identical to the prior source, and two negative
probes with both selectors undefined reached all four new errors. Evidence is
in `m9-t99-s20-bfs-depth1-ccpu-or-conversions.tsv` and
`m9-t99-s20-bfs-depth1-ccpu-or-negative.tsv`. The existing locally combined
condition itself remains an explicit adaptation; this does not claim its XP
outer framework has been restored.

Current depth-1 accounting is 2,201 arms: 1,195 excluded, 982 selected,
22 structural guards, and two numeric `HOST_BIAS` arms awaiting the subsequent
conversion described below. The
depth-2 queue contains 1,381 current direct children plus nine separately
recorded absent original children. The two whose hashes changed after the OR
cleanup were revalidated against their original direct-child directives and
the current selected-parent anchors without processing their bodies. Root
ledger integrity, the XP mirror gate, and whitespace checks pass. The package
EXEs and prior builds predate the later OR cleanups.
Following that numeric conversion, refreshed current-tree depth-1 accounting
is 2,201 arms: 1,197 excluded, 982 selected, and 22 structural guards; zero
current arms lack a ledger classification. This does not resolve the nine
original local-adaptation candidate starts or the five original sound diagnostic
frames, and does not by itself establish first-depth completion.
For the owner-directed exclusion-first delivery, the two missing original
`DBG` frames in `nt_sb.h` and `nt_sbfm.c` were restored with one `#error` in
each false arm. All 32 actual x86/x64 header dependency contexts observed
`DBG` false, and all four FM source compile contexts did likewise; temporary
draft `-E -P` output matched the previous source in all of those contexts.
Four negative source probes with `DBG=1` each reached both intended errors.
The header's selected no-op native debug macros remain omitted as the existing
T93 subset policy, with an explicit record in the whole-omissions ledger;
the original FM frame is retained after the last surviving function because
its enclosing native FMPortWrite function is outside the selected subset.
Current tree has 2,204 first-depth arms: 1,199 excluded, 983 selected, and 22
structural guards, with no unclassified current-tree arms. The remaining
original selected/adapted sound and ABI frameworks are for the retained-arm
phase; do not count the first depth complete. The exclusion phase still needs
its requested build, tests, actual-diff review, commit, and push.
OpenNT comparison of 463 corresponding retained C/header files found 1,628
original first-depth opening directives; 1,279 align exactly to XP first-depth
directives, of which 1,261 also align exactly to current source. The other
18 require disposition, not automatic restoration. Another 315 OpenNT roots
align to non-root XP positions and 34 do not align exactly; those counts alone
do not establish that a frame is absent from the retained XP subset. The seven
XP-only Sound Blaster files have no same-path OpenNT counterpart. The complete
cross-version disposition remains under review. A known excluded `#if 0` in
`base/ccpu386/c_xtrn.c` now restores XP diagnostic line 25 before the next
retained include. The full-tree structural scan finds 2,240 existing error
arms with no remaining nested content; this is not a deeper-level audit.
For the 38 arms without an immediate line directive at the next boundary,
retained code is reached only after an existing line directive (36 cases) or
not at all before EOF (two cases); adding directives to a rejected arm would
not restore diagnostics on the selected path.
Exclusion-batch verification after this correction: x86 and x64 cached builds
passed; each architecture passed all 186 non-native and all eight serial
native desktop/audio tests. The published x86 and x64 EXE SHA-256 values are
`38850ac6c3a6861db0df34e77f170725c46a733b9a355c2f803d9f3f3b283ee2`
and `c369f63e7b8b9ec9b837ce63c7ab88365093543e80cb09f11d56c3697549ff7d`.
This qualifies the implemented exclusion batch, not the unreviewed retained
arms or the completeness of all OpenNT-to-XP mappings.
The exclusion batch was committed and pushed as `f6596bc9`. Retained-arm
review is now active. The refreshed hash-anchored selected ledger has 983
first-depth arms: 982 were selected in both architectures and one `i386`
arm was selected only in x86. Together with 22 include guards, their current
direct children form a provisional 1,381-arm depth-2 queue in
`m9-t99-s20-bfs-depth2-provisional-queue.tsv`; no depth-2 body has been
processed in this phase. Nine additional original-child candidates remain
separately recorded. Four stale parent hashes were revalidated against the
original and current parent anchors and refreshed. The queue is provisional
until the 12 selected/adapted original-frame candidates and the OpenNT/XP
cross-version alignment questions are reviewed.
Retained-arm review found three additionally omitted first-depth
`REPORT_SB_MODE` frames in the T93 sound subset. The XP header defines this
only for native diagnostics; that definition and implementation are outside
the current subset. Actual preprocessing observes the condition unselected
in all 32 x86/x64 header dependency contexts. Restoring the three original
frames with `#error` preserves all 32 header and six MIDI source `-E -P`
outputs. Six injected `REPORT_SB_MODE=1` source contexts reach all three
errors. Diagnostic `#line` mappings preserve both the XP directive locations
and the existing local MIDI source location. Both cached builds passed;
x86 and x64 each passed 186 non-native tests and eight serial native tests.
Final EXE SHA-256: x86 `177f6ac9038e54c18eb8af9bdd455bb38583e9b28565d00a696a01584f1492a4`;
x64 `23282ef325d191f2bd3e8b660e1145026f3cbadfd607a5ee5e776b91dcb24ae5`.
The first build was superseded by a final dual build after removing a redundant
line directive. The correction was committed and pushed as `33e8bf0b`;
the depth-2 queue remains provisional until original first-depth candidates
are fully reconciled.
Following delivery `33e8bf0b`, a first-depth line-control review found
78 `#line` directives in 33 files whose immediately following retained XP
code has a unique original anchor and at least two consecutive matching
nontrivial source lines. These were adjusted to the verified XP anchor. A
disposable copy of the full current source/test tree was preprocessed using
all 550 actual x86/x64 compile contexts that depend on those files: before
and after `-E -P` outputs were identical in all 550. Evidence is recorded
in `m9-t99-s20-bfs-depth1-line-anchor-reconciliation.tsv`; this is not a
claim that every first-depth line directive or original condition has been
proved. Build, release, full tests, review, commit and push remain pending.
A second, same-depth comparison using unique contiguous XP sequences and
manual adjacent-context review identified 151 further line directives.
Of 226 actual affected x86/x64 compile contexts, 224 had identical `-E -P`
output; the other two differed solely in the CCPU Invalid Task Reg diagnostic
`__LINE__`, corrected from 1853 to the actual XP line 1851. The two passes
cover 229 markers in 46 files, and do not establish completeness for
unanchored or ambiguous conditions. Both final cached builds passed; each
architecture passed all 186 non-native and eight serial native tests. The
final binary SHA-256 values are x86
`b4f92cbfef7fbc208cddc74455c40dcf7e3ed5a17cfd469d5c3d7e494841dff7` and x64
`5f61fe1c01226dea3845637880a63bc123286661db89da0109ec558305acdde`.
No non-mirror compiler warnings occurred. Actual mirror diff independently
contains exactly 229 `#line` replacements and no other production changes.
That exclusion-line batch was committed and pushed as `9893e779`.
Additional first-depth boundary review identified 52 more `#line` markers in
19 files, anchored by unique original closing context or unique next
conditional directives. All 84 actual affected x86/x64 preprocessor outputs
were identical before and after. The line markers were adjusted and the
current-source hashes refreshed. Final x86/x64 builds passed; each width
passed 186 non-native and eight serial native tests. EXE SHA-256 values are
x86 `47b0d6e5c658cad1d11aed79b56591da3e72faf55e1f56b22008f5de185f334d`
and x64 `902c3cc909923a04c241bc6ecd5170d283f7eed4563b375024424e1fc0bda90d`.
The 52-marker batch was committed and pushed as `5a9577e6`.
Continued boundary review used comment-aware matching of the original
first-depth condition, corresponding `else`/`endif`, and either its
following selected code or the next direct conditional directive. This
uniquely anchored 64 further markers in 26 files; all 158 actual affected
x86/x64 `-E -P` outputs matched before/after. Evidence is in
`m9-t99-s20-bfs-depth1-line-group-reconciliation.tsv`. This does not cover
ambiguous and unanchored first-depth cases. Final x86/x64 builds passed;
each width passed all 186 non-native and eight serial native tests. Final
EXE SHA-256 values: x86
`20e520fef8ede23cd2bd7166f6f1929330eb53f3a134f32412834b023661e0b6` and x64
`021e257daeae6ca6db1716f90131562bf1600638bba9397f60b6ca8c64dae99f`.
Actual production diff was 64 `#line` replacements only; the batch was
committed and pushed as `d49c6729`. Further first-depth review required exact
correspondence between the original condition group, its `else` or `endif`,
and the adjacent retained code or direct condition. This identified 66
additional markers in 25 files, with 152/152 affected x86/x64 `-E -P`
outputs unchanged. The current production diff contains only those marker
replacements. Final x86/x64 builds passed; each architecture passed 186
non-native and eight serial native tests. Final EXE SHA-256 values: x86
`b056ddbfb1858dbddef3aa2702c6fd2936bc087f0b3a1c197da430b83a5a4623` and x64
`e020e5a0f1d23e0336dcd278b1180a3f7b30b888d59b94c3f49af7474620af61`.
The 66-marker correction was committed and pushed as `490ee9e8`.
First-depth follow-up reconciled 30 more line markers in 18 files against
exact original selected context; all 40 affected x86/x64 `-E -P` outputs
matched. The 23 first-token mapping gaps were reviewed separately: nine
original-context corrections, five already correct, and nine local additions
without a unique XP line. The XP `rom.c:342` direct child of the selected
NEC_98 else was found absent and recorded in the provisional depth-2
absent-child queue; its body has not been examined in this phase. The 27
previously ambiguous next-directive mappings were reviewed: 20 corrected
and seven already correct; see `m9-t99-s20-bfs-depth1-next-directive-review.tsv`.
Final x86/x64 builds passed; each width passed all 186 non-native and eight
serial native tests. EXE SHA-256 values: x86
`6d4e681a756c61763a3c2d3b682a3eef589d8c900968adae161e1760c314c3bd` and x64
`1ffe128ccae1aa669a9efa037803bf100b86df06428184b3ef0cf8a4e4361dd6`.
The 30-marker correction was committed and pushed as `5ff794f8`. During
retained-arm review, the original `NO_STRING_OPERATIONS` first-depth frame
in `cpu_vid.h` was moved outside the existing local ABI guard, retaining the
guard and the structure layout; all 30 actual x86/x64 consumer outputs
matched before/after. The original `CPU_30_STYLE` outer frame in `ica.h`
was restored, preserving the existing `CPU_40_STYLE` restriction as a direct
child; all 64 actual consumer outputs matched. Both original outer
conditions were observed selected in every respective actual context. The
provisional depth-2 queue removes the now-first-depth string guard and adds
ICA's direct `CPU_40_STYLE` child without reviewing its body. Evidence is
in `m9-t99-s20-bfs-depth1-selected-framework-restorations.tsv`. The current
first-depth inventory has 2,208 arms: 1,202 excluded, 984 selected and 22
include guards, with no unclassified or stale selected/guard evidence. The
1,381-entry provisional direct-child queue exactly matches the current tree.
Final x86/x64 builds passed; each width passed 186 non-native and eight serial
native tests. EXE SHA-256: x86
`84edd752d8923462df563b2333ed8291dfeedb41c43398d620b50134d8bb90ae` and x64
`7ec56c5321a8e31bcbb03b6a4a451e15a561ea1687c99ed6e75699665b337b77`.
The selected-framework restoration was committed and pushed as `9f9dd90f`.
The six existing CCPU NTVDM-or-TLS conditions have now been independently
reviewed: both actual CCPU contexts select TLS at all six points, and
removing TLS deliberately triggers the four existing unsupported errors.
The local OR extension retains the original NTVDM predicate and one shared
implementation; it must not be simplified by considering NTVDM alone.
For IOS, 108 actual header contexts select 0x10000 entries and two mixer
contexts select 0x400; the existing original-OR-plus-standalone condition
preserves both and supports masked 16-bit IO dispatch. These necessary local
condition extensions are recorded in `m9-t99-s20-bfs-depth1-local-condition-review.tsv`.
The three OpenNT root expressions without the same XP root signature were
reviewed: the XP reset guard is a direct child now recorded in the provisional
absent-child queue, IOS PRINTER is already a queued XP/current child, and XP
ROM includes malloc.h unconditionally. See `m9-t99-s20-bfs-depth1-opennt-xp-version-review.tsv`.
The 34 previously unaligned OpenNT root candidates have cross-version review
records; this does not prove all first-depth frames have been reconciled.
A direct-child-only XP comparison also records 140 unverified child
candidates separately from the 1,381 present queued arms and 11 known absent
children. Candidate matching is provisional, not a claim that every candidate
is absent; unmatched parent mappings still require review. No depth-2 body
was processed. See `m9-t99-s20-bfs-depth2-xp-child-candidates.tsv`.
Three unmapped parent records were checked against direct original and current
condition boundaries: ICA 1549 and mouse 3018/1126 have their direct children
already queued. The mouse 3018 selected body has a first-depth `#line 3018` followed by an
inserted blank line, so XP source line 3019 already has the correct line.
The previous proposed 3019 correction was erroneous and has been reverted. See
`m9-t99-s20-bfs-depth1-unmapped-parent-review.tsv`.
All 13 parent records omitted by the automated direct-child mapping now have
boundary classifications in that review. Excluded parents do not enqueue
children; missing original direct children remain separately queued. The large
video parent has 27 provisional child candidates, not confirmed absences.
This resolves these mapping records only, not the full first-depth audit.
The 315 OpenNT root directives that map exactly to nested XP directives
all have selected current first-depth parents: 238 exact mappings and 77
boundary-verified mappings. Their direct children are accounted for as 274
present in the depth-2 queue, eight known absent, and 33 provisional
candidates. These categories do not verify any child body. Evidence:
`m9-t99-s20-opennt-roots-nested-in-xp.tsv`.
A fresh current-tree classification cross-check accounts for all 2,208 root
arms: 984 selected, 22 include guards, 935 source-probed exclusions, 264
header-probed exclusions and three independently probed REPORT_SB_MODE
exclusions. The sole stale source-probe hash was revalidated: `c_xtrn.c`
retains `#if 0`, and the later edit only replaced its excluded body and
restored `#line 25`; its current hash is now recorded. This classification
check does not by itself establish completeness of the original XP structures.
The first-depth original-framework reconciliation was independently refreshed
against all 470 retained text C/H files: all 1,703 XP first-depth opening
directives are accounted for, 1,665 by exact current first-depth line
mapping and 38 by the existing hash-valid individual dispositions. Original
first-depth `else`/`elif` siblings are present in each of those 1,703 matched
groups, allowing the seven registered local condition extensions. The 315
OpenNT-first-depth directives moved inward in XP were accounted for separately.
The refreshed current direct-child queue matches all 1,381 eligible current
arms; no excluded parent retains a direct child. Eleven known absent XP child
groups and 140 unverified candidates remain separately listed for depth 2.
First-depth framework and classification audit is complete for the retained
XP subset; the previously delivered final source at `9f9dd90f` passed the
dual builds and 186 plus eight tests per architecture. S20 remains open and
continues at depth 2; its candidates are not claimed verified.
This audit is documentation/evidence only; cross-version root mappings and
first-depth completeness remain under review.
After those OR cleanups, cached x86/x64 whole-project builds passed and
refreshed the package EXEs; focused App unit and XP mirror checks passed
49/49 per architecture. The intermediate EXE SHA-256 values are x86
`6d0ca0e7323e3df71a3a2e980bbc248fc67e36fbb97dea6f26fef11b9bdf6573`
and x64
`3cdd8032d053a00f4a7e5399c0173c93161822fc7cd832c9a53fa3af95236d32`.
Those focused tests do not replace first-depth full regression or desktop
qualification.
The two `HOST_BIAS >= 16383` first-depth conditions were independently
measured at both source points in x86/x64: each value was 1023. Both false
bodies matched the pinned XP originals byte for byte and were replaced with
single `#error` directives; the selected `else` remains intact. Before
application x86/x64 `-E -P` output matched the previous source byte for byte;
negative probes with `HOST_BIAS=16383` triggered both intended errors on
each architecture. Evidence is in
`m9-t99-s20-bfs-depth1-hostbias-conversions.tsv` and
`m9-t99-s20-bfs-depth1-hostbias-probes.tsv`. This batch postdates the
intermediate EXEs.
Subsequent cached x64/x86 builds passed and refreshed the EXEs. The first
x64 focused 49-test run timed out in `audio_failure_smoke`; its injected
`cannot join audio worker` diagnostic alone does not establish the timeout
cause. That mock-based test passed separately, and a complete x64 focused
rerun and the x86 focused run each passed 49/49. The initial timeout remains
recorded as an unexplained intermittent failure. Current intermediate EXE
SHA-256 values are x86
`735e43ac8430b3e1e5f4d689ad72741d9a961014cf44e22d49fc00fbe9b944b4`
and x64
`2ab7e2fbebba8908eba38410a6e229d88eaf52ccd86cba33486b6077cb2149c0`.
The selected non-NEC arm of the first-depth `egacpu.h` condition had a stale
`#line 138`: its retained `MAGIC ega_state` is XP line 161. The directive was
corrected to `#line 161`, without changing the arm's data or its conditional
structure. The manifest and current-hash audit records were refreshed; the
XP mirror and whitespace checks pass. The existing dual build and test
evidence predates this change, and no new final-layer qualification is claimed.
The same first-depth selected-arm check found 127 further stale `#line`
directives across 33 files. Each corrected target was anchored to a unique XP
line and at least three consecutive matching original lines, including at
least two non-comment lines; the original files have no preceding `#line` at
those targets. A second pass verified all 127 new logical targets and adjacent
original text. The per-location evidence and old/new hashes are in
`m9-t99-s20-bfs-depth1-selected-line-fixes.tsv`. Mirror and whitespace gates
pass; this diagnostics-only source batch still requires the final layer's
dual builds and regression, and the other line candidates remain under review.
The other 40 candidates from that direct first-depth selected-arm scan were
then corrected using each retained line's unique exact match in the pinned XP
file; those lack the longer contiguous-text proof used for the 127 above.
Both batches were rechecked against the original target line and current
directive, and their file hashes, line locations, and old/new targets are in
`m9-t99-s20-bfs-depth1-selected-line-fixes-single.tsv` and the preceding
contiguous-match ledger. Mirror and whitespace gates pass; dual build and
final depth-1 qualification remain outstanding.
Four more first-depth boundary `#line` directives were corrected in `egacpu.h`
and `mouse_io.c`, using the original condition boundary and adjacent retained
XP source as anchors. They are recorded in
`m9-t99-s20-bfs-depth1-boundary-line-fixes.tsv`; the XP mirror and whitespace
checks pass. These diagnostics-only corrections do not qualify the layer or
replace the required final x86/x64 build and tests.
After these latest line-control edits, cached x64 and x86 whole-project builds
both completed with exit 0 and refreshed the package EXEs. The focused
`app-softpc/unit/` and XP mirror checks passed 49/49 for each architecture.
The current EXE SHA-256 values are x86
`dafd167b9de2e69f0a92df454ccfe5ce1a7399b5062068b361e4f1da58a180a8`
and x64
`3466132f94f019e30458d157c8c3535fe2198443fc0a75ebb081e22455d03870`.
These are intermediate artifacts and focused tests: first-depth review,
full regression, coordinator review, and the depth delivery remain outstanding.
The subsequent first-depth boundary scan found 70 candidate `#line` errors
after complete outer conditions. Sixty-six were corrected across 26 files
after unique XP anchoring and at least three consecutive matching lines;
the four weaker candidates remain pending. A second pass confirmed each of
the 66 target mappings; details and old/new hashes are in
`m9-t99-s20-bfs-depth1-after-frame-line-fixes.tsv`. The XP mirror and
whitespace gates pass. The intermediate EXEs and builds above predate this
batch and must be refreshed after the first-depth audit is completed.
The four weaker boundary candidates were subsequently verified against the
original adjacent `#endif` and retained line and corrected in three files;
the per-point record is `m9-t99-s20-bfs-depth1-after-frame-line-fixes-short.tsv`.
The mirror and whitespace gates pass. These four corrections also postdate the
intermediate dual build and do not complete first-depth qualification.
The first-depth condition-to-condition line scan identified 29 candidates.
Twenty-eight were corrected in 16 files after matching both the unique next XP
root directive and the preceding original condition; a second pass verified
their line targets. The other candidate is the local C-VID forward declaration
in `egacpu.h`, not the unrelated same-named XP condition. Evidence is in
`m9-t99-s20-bfs-depth1-next-root-line-fixes.tsv` and
`m9-t99-s20-bfs-depth1-next-root-line-candidates.tsv`. Mirror and whitespace
checks pass; the intermediate dual build predates this batch.
An additional direct-body scan found 24 selected first-depth arms with no code
and no currently present child condition. Two have an empty original selected
body, five have already identified original children queued for depth 2, and
four have recorded selected-content omissions. Thirteen were subsequently
matched to their original condition and complete comment-only arm; no code
was omitted from those selected bodies.
The inventory is `m9-t99-s20-bfs-depth1-selected-empty-review.tsv`. These
selected arms are not candidates for an unconditional `#error`.
An independent recheck of the five first-depth line-fix ledgers matched all
265 recorded current directives and retained anchors to their stated XP
locations after subsequent edits; it did not evaluate other unmapped markers
or prove the remaining original-frame dispositions.
After the later first-depth line-control batches, both cached whole-project
builds completed with exit 0. Background CTest excluding `desktop` and
`native_audio` passed 186/186 on x64 and 186/186 on x86, including the build
cleanup selftest in an environment permitting its Windows junction. The
build refreshed the package EXEs; their SHA-256 values at this intermediate
point are x86 `b9c58334896f0aa19aa51a9fbbcac9609ccaa077134ef89d4c63f41815d66706`
and x64 `58e08fdb5207128a52ef3ea2e182291427ffcd3651c39169ed8aecf4980a2812`.
Desktop/native-audio tests and full first-depth audit/delivery remain open;
these background results do not resolve the deferred original frames.

Further original-frame review traced the numeric-condition candidates in
`nt_sb.h`, `nt_sbfm.c`, and `nt_sbmid.c` to T93 sound-subset exclusions:
respectively native debug, native FM-driver diagnostics, and native MIDI
diagnostics. The T93 S6/S8 range ledgers record those excluded original lines;
the current mirror and Compat contain no `DisplaySbMode`, `DisplayFlags`,
`DebugLevel`, or `REPORT_SB_MODE` references. XP itself defines
`REPORT_SB_MODE` as 1, so its original bodies cannot be classified as false
merely because the replacement subset no longer defines it. The original-frame
disposition and any required selected-content exception remain unresolved;
numeric-condition treatment is deferred by this packet. No source change or
claim of a completed depth-1 audit follows from this observation.

An intermediate cached full-project build subsequently completed for x86
and x64 (exit 0 on each), with mirror compiler warnings. Both architectures
then passed all 47 `app-softpc/unit/` tests and both XP mirror checks (49/49
per architecture). These are intermediate results, not full layer regression
or layer delivery. The build's link targets directly refreshed both EXEs in
`assets/binary`; those binaries are intermediate and have not passed complete
layer qualification. The local `ios.h` standalone selection was also traced:
it sets `PC_IO_MEM_SIZE` to 0x10000, and `ios.c` masks I/O addresses using
that size; changing it to the original condition's 0x400 selection would
alias high port addresses. The existing selected behavior remains intact.
Intermediate executable SHA-256: x86
`965722521f91d72f0677e96b5acd7f465eabda2a23e004daa4c9cbf9e86bbe2e`;
x64 `e15814f74038fedfbfcfd91f7d09bcb6badaa30ce79b5d144be6c9a8f1eea82a`.

An intermediate broad CTest run excluding `desktop` and `native_audio` ran
186 tests per architecture. Each initial run passed 185 and failed only
`checks/build_cleanup/selftest`: the sandbox rejected creation of its test
Junction. That exact test passed independently for each architecture when
rerun outside the sandbox. The initial CTest runs remain recorded as failures;
this split rerun is not represented as a single clean full run. Desktop and
native audio integration were not included, and layer qualification remains
outstanding.

An independent current-worktree consistency check verified the source hash,
physical line and directive of every existing root evidence record: 972
selected, 916 source-error, 257 header-error and 31 deferred rows; no stale
record was found. A row's consistency does not itself prove its semantic
classification or eliminate the unresolved original-frame candidates.

A provisional depth-2 queue has been generated from the current tree without
inspecting child bodies: `build/t99-s20/bfs-depth2-queue.tsv` contains 1,357
direct child arms under observed-selected root arms or retained include
guards. Five further child arms belong to deferred root parents and are not
yet eligible. The separately recorded eight absent original children remain
pending restoration at depth 2. No current excluded parent contains a child
directive. This queue is provisional while depth-1 original-frame review is
open and must be regenerated after any parent or source change; depth 2 has
not been started.

A depth-1 boundary line-number scan found 160 provisional XP/source mapping
candidates in 39 files; exact text matches are only hints and require context
review. In `reset.c`, eight candidates and three additional selected-boundary
line controls were individually matched to their XP context and corrected.
The x86/x64 object builds and all 35 root error-arm probes pass; mirror
warnings remain. The 475-body mirror gate and structural scan pass. The
candidate ledger is `docs/etc/evidence/softpc/m9-t99-s20-bfs-depth1-line-candidates.tsv`;
152 original heuristic candidates remain unreviewed. The depth-2 queue was
regenerated after this edit. The scan does not prove every line control is
correct, and depth-1 qualification remains incomplete.

Four `c_main.c` and four `com.c` additional root-boundary `#line` values were
then checked against their surrounding XP source and corrected. Both files'
x86/x64 objects compile with mirror warnings; their 115 root error arms passed
the four actual-context probes, and the mirror and structural gates pass.
The provisional depth-2 queue was regenerated. Of the original 160 heuristic
line candidates, 16 are now reviewed and corrected and 144 remain pending;
three additional directly identified line controls were also corrected.

Eight more root-boundary line controls in `ios.c`, `time_day.c`, `ica.c` and
`rom.c` were checked against XP and corrected. All eight architecture objects
compile with mirror warnings; 106 root error arms pass the eight actual-context
probes. The mirror and structural gates pass and the provisional depth-2 queue
was refreshed. Of the initial 160 heuristic line candidates, 75 are corrected
and 85 remain pending; three additional directly identified controls were also
corrected. These intermediate checks do not complete depth-1 qualification.

Six more root-boundary line controls in `fpu.c`, `zfrsrvd.c`, `idetect.c`,
`cga.c`, `gfx_updt.c` and `ppi.c` were checked against XP and corrected. All
twelve architecture objects compile; mirror warnings remain. Forty-four root
error arms pass the twelve actual-context probes, the mirror and structural
gates pass, and the provisional depth-2 queue is refreshed. Of the initial
160 heuristic line candidates, 81 are corrected and 79 remain pending; three
additional directly identified controls were also corrected.

Thirteen root-boundary line controls in seven headers (`egacpu.h`,
`gfx_upd.h`, `gvi.h`, `ios.h`, `nt_fulsc.h`, `nt_graph.h` and `cpu_vid.h`)
were checked against XP and corrected. The 152 recorded architecture contexts
preprocessed successfully; 12 overlay misses were independently rechecked in a
fresh source-tree copy, reaching their actual headers without selecting an
error arm. All 55 affected root error arms have refreshed evidence. Three
representative dependent objects compile in each architecture with mirror
warnings. Mirror/structural gates pass, and the provisional depth-2 queue is
refreshed. Of the initial 160 heuristic line candidates, 94 are corrected and
66 remain pending; three additional directly identified controls were also
corrected. Full layer builds and review remain outstanding.

Ten weaker-match root-boundary line candidates in `timer.c` and `at_dma.c`
were individually checked against the adjacent XP directives and retained code,
then corrected. Both architecture objects compile with mirror warnings; 78
root error arms pass all four source probes. Mirror and structural gates pass,
and the provisional depth-2 queue is refreshed. Of the initial 160 heuristic
line candidates, 104 are corrected and 56 remain pending; three additional
directly identified controls were also corrected. Full depth-1 review remains
outstanding.

Eleven additional root-boundary line candidates in `keyba.c` and
`keybd_io.c` were individually checked against XP and corrected. Both
architecture objects compile with mirror warnings; 72 root error arms pass
all four source probes. Mirror and structural gates pass, and the provisional
depth-2 queue is refreshed. Of the initial 160 heuristic line candidates,
115 are corrected and 45 remain pending; three additional directly identified
controls were also corrected. Full depth-1 review remains outstanding.

Fifteen further root-boundary line candidates in `nt_graph.c`, `mouse_io.c`
and `ica.c` were individually matched to XP context and corrected. Both
architecture objects compile with mirror warnings. The six source probes
confirm 173 root error arms remain unselected and the previously restored
mouse stream outer frame remains selected on both architectures. The mirror
and structural gates pass, and the provisional depth-2 queue is refreshed.
Of the initial 160 heuristic line candidates, 130 are corrected and 30 remain
pending; three additional directly identified controls were also corrected.
Full depth-1 review remains outstanding.

The final 18 originally enumerated root-boundary line candidates were checked
against XP and corrected across ten sources and six headers; local byte-order
and other already-selected adaptations were retained. All twenty affected
source objects compile with mirror warnings. Source probes cover 95 root error
arms in 20 contexts. Header probes cover 126 recorded contexts, with 12
overlay misses independently rechecked in a fresh source-tree copy; 63 header
root error arms have refreshed evidence. The 475-body mirror gate, structural
scan, evidence hash/line consistency check and refreshed provisional depth-2
queue pass. The original 160 heuristic candidates are all reviewed and
corrected, plus three directly identified controls; rerunning that same
heuristic finds no new candidate. This does not prove all line controls are
correct, resolve the 31 original-framework candidates, or complete depth-1
qualification. Full builds, tests and publication remain outstanding.

Twelve further root-boundary line candidates in `c_main.c`, `com.c`,
`floppy.c`, `mouse.c`, `ios.c` and `cga.c` were individually checked against
XP and corrected. All twelve architecture objects compile with mirror
warnings; 193 root error arms pass the twelve source probes. The mirror and
structural gates pass, and the provisional depth-2 queue is refreshed. Of the
initial 160 heuristic line candidates, 142 are corrected and 18 remain
pending; three additional directly identified controls were also corrected.
Full depth-1 review remains outstanding.

Six further XP-aligned root-boundary line controls were corrected in
`fdisk.c`, `fla.c`, `floppy.c` and `floppy_i.c`. All eight architecture
objects compile with mirror warnings; 58 root error arms passed all eight
source probes, and the mirror and structural gates pass. The depth-2 queue
was refreshed without processing its children. Of the initial 160 heuristic
line candidates, 22 are corrected and 138 remain pending; three additional
directly identified line controls were also corrected.

All 16 strong-match `timer.c` root-boundary line candidates were individually
checked against their adjacent XP directives and code, then corrected. Both
architecture objects compile with mirror warnings; all 62 root error arms
remain unselected in the actual x86/x64 probes. The mirror and structural
gates pass and the provisional depth-2 queue is refreshed. Of the initial
160 heuristic line candidates, 38 are corrected and 122 remain pending; three
additional directly identified line controls were also corrected.

Fourteen additional root-boundary line controls in `nt_graph.c`, `at_dma.c`,
`mouse.c` and `keybd_io.c` were checked against adjacent XP directives and
code, then corrected. All eight architecture objects compile with mirror
warnings; 113 root error arms pass the eight actual-context probes. Mirror and
structural gates pass, and the provisional depth-2 queue is refreshed. Of the
initial 160 heuristic line candidates, 67 are corrected and 93 remain pending;
three additional directly identified line controls were also corrected.

Fifteen more root-boundary line controls in `keyba.c` and `mouse_io.c` were
checked against their adjacent XP directives and code, then corrected. Both
architecture objects compile with mirror warnings. The combined probes
confirm all 98 root error arms remain unselected and the restored mouse stream
outer frame remains selected on both architectures. The mirror and structural
gates pass and the provisional depth-2 queue is refreshed. Of the initial
160 heuristic line candidates, 53 are corrected and 107 remain pending; three
additional directly identified line controls were also corrected.

Six apparent missing `c_main.c` `#ifdef NTVDM` frames were checked against
XP individually. Existing CCPU thread-stack port code changed each original
condition into `NTVDM || SOFTPC_CCPU_TLS_SIMSTACK`; this is a real local
condition change rather than six simply absent blocks. It is recorded as an
unresolved coupled-OR case, not counted as restored or removed in depth 1.

The missing XP `ica.c` outer `#if defined(NTVDM)` around the IRET-hook block
was restored with a single `#error`; its excluded inner conditions were not
recreated. The original following `#ifdef HOOKED_IRETS` is at depth 1 again.
All 65 current root error arms in that file passed x86/x64 probes, and both
architecture objects compile with mirror warnings. The structural audit finds
2,185 clean error arms, and the 475-body mirror gate passes. A separate
selected-arm probe remains in progress; depth-1 qualification is not complete.

That selected-arm probe has now completed: 86 source files and 172 actual
x86/x64 contexts all reached their source and preprocessed successfully. It
newly observed 358 root arms selected in both architectures, with per-source
hashes added to the selected-arm ledger. The 24 unobserved arms from this batch
are all already deferred coupled/value cases; non-observation is not treated as
proof of exclusion. The current root inventory has 2,199 arms: 1,161 contain
an `#error`, 767 have recorded selection, 37 are include guards, 30 are
deferred and 204 remain unclassified (all headers). These figures classify the
current worktree only and do not prove original-framework completeness or
the correctness of every retained body.

The subsequent selected-header probe used a task-local source-tree copy with
the original include layout. All 558 recorded x86/x64 dependency contexts
preprocessed successfully and reached the 54 expected headers. All 204
previously unclassified header arms were selected: 203 on both architectures;
`host/inc/host_def.h` `#ifdef i386` was observed only on x86. These observations
and current source hashes are recorded in the selected-arm ledger. The current
root accounting is therefore 1,161 error arms, 971 selected arms, 37 include
guards and 30 deferred arms. This classifies the existing root inventory, not
the still-required semantic review or original-framework completeness audit.

The missing XP `nt_uis.h` root `#ifdef DBCS` frames are restored with excluded
bodies and original diagnostic numbering for the following retained lines.
Targeted probes cover all four recorded dependency contexts per architecture;
each reaches the header without selecting either excluded arm. All four
affected objects compile for each architecture; the mirror compiler emits
warnings. The structural scan finds 2,184 clean error arms and
the 475-body mirror gate passes. This is intermediate evidence only.

An independent first-depth header error-arm probe recorded 257 arms. A targeted
source-tree copy preserving the include layout resolved the overlay misses and
basename conflicts: all 257 were reached in their recorded x86/x64 dependency
contexts, with no selected `#error` and successful preprocessing. Per-arm
context counts and source hashes are in
`docs/etc/evidence/softpc/m9-t99-s20-bfs-depth1-error-header-audit.tsv`.
An attempted higher-priority `-iquote` overlay changed other header choices
and caused two temporary preprocess failures; that retry is not counted as
evidence or as a product build failure. The probes do not establish original
frame completeness or behavior in unrecorded compilation contexts.
