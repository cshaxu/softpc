# T99 S30 independent recursive mirror review — in progress

## Scope and first-pass results

The universe is the 475 paths in the current XP SP1 mirror manifest. Each
same-path XP file exists, and its manifest identity matches after the same
LF normalization used by the repository's text-hash gate (binary files are
hashed as bytes). The comparison baseline is XP SP1
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`. There are 471 text files and
four binary files. Binary files have no textual preprocessor branch tree.

The first pass lexically constructs nested conditional groups and their sibling
arms on both sides. It stops recursion when the complete current arm consists
of one `#error`, and otherwise compares the corresponding arms recursively.
[File inventory](m9-t99-s30-independent-file-ledger.tsv) records 349 text
files without a detected difference or repeated condition name, 105 whose
sequence matches but that have repeated condition names requiring review, and
17 with structural comparison candidates. The
[58 candidates](m9-t99-s30-independent-tree-candidates.tsv) are positional
matching leads, not a count of confirmed defects or exceptions.

## Important unresolved matching work

The matcher uses ordered condition signatures, so equal signatures are not
proof of correct pairing when siblings repeat. The separate
[repeated-sibling queue](m9-t99-s30-independent-repeated-queue.tsv) lists
2,394 current positions in 118 files, including locations from the 17 files
with other candidates. When compared by current path and line number against
the three previous repeated-review tables, 1,407 positions had a prior row
and 987 did not. The latter number is conservative: a line can have shifted,
a group can be covered by another record, or its whole arm can be excluded.
It is not a defect count. A prior row also does not automatically establish
that its parent and sibling boundaries were matched correctly.

The parser and all sequence comparisons are audit aids, not formal proof.
Each retained group and necessary local difference must be matched to its
actual XP parent and source boundary, or remain explicitly unresolved. Groups
inside entirely replaced error arms must not be reconstructed or reviewed as
current retained children. No source correction is claimed by this first pass;
no build, runtime test or guest behavior has been established by it.

A second report adds the matcher's candidate XP position and the distance
between it and the current source's `#line`-derived location for each repeated
position. Of the 2,394 entries, 1,554 have a candidate within three lines,
814 have a more distant location, and 26 have no candidate at this level.
A nearby line number is only a clue, not proof: the `#line` mapping may itself
be stale or a repeated sibling may be incorrectly aligned. The queue records individual independent reviews; the existing prior-row flag
is informational and does not silently accept old results.
The unmatched candidates include accepted local CCPU, ABI, snapshot and V7
guards, but their individual parent/arm pairing still requires confirmation.

The 26 unpaired repeated locations all occur in files with a prior structural
disposition. Twenty-four match a prior entry by exact path and line; the two
PPI locations correspond to lines 219 and 236 in the earlier inventory,
before four preceding lines were removed. This establishes a review trail,
not by itself proof that every prior disposition or nested alignment is correct.

For 2,368 repeated groups that the tree matcher paired, a separate heuristic
looked for unique matching source lines both before and after the candidate
boundaries. It found two directional context anchors for 1,747 pairs, one for
446, and none for 175. These are search aids only: proximity and unique text
do not prove the complete parent/arm boundaries or branch semantics. The
queue records the counts without marking these entries as reviewed. Among
3,613 paired groups, no case had a condition unique among current siblings
but repeated among XP siblings. Unpaired groups and descendants of unequal
groups require separate handling.

The first-pass comparer does not recurse through unequal parent signatures.
A targeted descendant sweep found one XP child and one current child under
such parents. XP `base/ccpu386/c_main.c:4876` (`#ifndef PROD`) belongs to the
original `c_cpu_unsimulate` NTVDM `#else` body; its corresponding current
non-TLS arm is wholly replaced by `#error`, so the child is intentionally
skipped. Current `host/src/nt_graph.c:1648` (`#ifdef V7VGA`) belongs to the
local standalone packed-painter width correction and is already listed as a
local exception in the earlier disposition. These checks do not settle other
repeated-boundary pairings.

The first four high-risk locations have now been inspected against the
actual XP source: one `JAPAN` branch within the video cursor path and three
trace branches in `com_inb` have matching surrounding source boundaries.
Their large XP-versus-logical-line distances are not themselves evidence of
a structure defect; the queue records each reviewed location and source
anchor. The remaining repeated locations are not yet independently cleared.

A potentially significant placement mismatch is retained for follow-up:
`host/src/nt_sbdsp.c:1508` has an error-only `#if 0` after `ContinueDMA`,
whereas the apparently matching XP `#if 0` at 2793 is inside the original
DMA reader-thread body. The earlier `#if 0` at current 712 corresponds to
XP 2233 in a different routine. The subtree's XP host-thread adaptation is
a substantial existing local divergence; sequence equality alone does not
explain whether the second disabled frame should remain at its present
location or how it belongs in the subset exception. No production edit has
been made for this candidate.

Follow-up: T20 explicitly mapped current `#if 0` to XP 2793 and recorded
that the native wave-output reader thread was replaced by the existing audio
endpoint. XP names the removed host routine `DspThreadEntry`; current code
contains no such routine. Its selected-source relocation has now been
registered specifically in the pristine-divergence ledger. The error-only
frame and line control remain; no dormant host worker or extra path is added.

A stricter context pass records individual original/current line pairs before
and after repeated groups, requiring each source line to be unique in its
file and excluding anchors that cross a same-condition occurrence. It found
two such anchors for 1,761 paired groups, one for 441, and none for 166.
These still require parent/arm review: anchors only help establish position
and do not independently validate selected code or prove logical equivalence.
The queue contains the line numbers so the actual source can be rechecked.

The parser's applicability was checked across the entire 471-file text
universe: neither current nor XP has an apparent conditional directive
immediately following a line ending in a continuation backslash. No parser
reported an unmatched or unclosed conditional. The repeated-sibling scan
refines the original per-file classification: 352 files have matching
recursive conditional signatures and no duplicate signature under the same
parent; 102 matching files have same-parent repetitions requiring alignment
evidence; 17 have mismatch candidates; four are binary. The three additional
unambiguous files contain repeated names under different parents, not under
the same parent. This is a structural check and does not attest source-body
behavior or replace checks of differences in the 17 files.

An earlier group-alignment pass reported 1,109 single-per-parent groups,
1,675 repeated groups with two condition-adjacent anchors, and 829 other
groups. Those results were provisional and have been superseded by the
outside-boundary correction below. The corrected
[group alignment](m9-t99-s30-independent-group-alignment.tsv) records all 3,613 paired
groups and their current parent chains. The classifications are mechanical
evidence, not completed independent human review; they do not include the
58 unmatched/mismatched candidate entries.

Correction to the earlier anchor passes: their after-anchor search started
after the opening directive and could select text inside the conditional group.
Those counts do not prove the `#endif` boundary. A fresh pass first finds each
group's matching `#endif` and searches strictly outside the whole group;
unique non-directive source anchors do not cross a same-condition occurrence.
For 2,368 repeated matched groups, 1,422 have both outside anchors, 756 have
one, and 190 have none. After requiring a verified-anchor chain through
repeated parents and rejecting arm mismatch markers, 1,289 repeated groups
have both boundary anchors and 1,066 unambiguous groups have an anchored
parent chain; 1,258 groups still need review. The group-alignment table now
uses the corrected outside-boundary method. Even both anchors establish
location evidence only, not equivalent code behavior.

Independent review identified a real omission behind the earlier mouse.c
"alignment-artifact" disposition. In the selected NEC_98 else arm, XP has
nine `#ifndef PROD` groups while the pre-edit local source has eight. The
last local group corresponds to XP 442, not XP 425; the omitted group is XP
393, in the discarded legacy timed-interrupt body. The existing replacement
timer keeps `case 0x1` through `0x4` but not that legacy runtime if. A
condition-only error frame was restored immediately before the shared break,
which is XP line 406. This retains the existing timer implementation and
records the unavoidable runtime-scope placement difference in the pristine
ledger. Both actual architecture compile commands define PROD, and pre-edit
versus candidate selected preprocessor output is byte-identical. A private localized probe temporarily undefines PROD only at the new frame;
the corresponding error is reached on both architectures. Full batch build and tests subsequently completed as recorded below.

The first-pass inventories, queue lines and source anchors are frozen to
committed S30 baseline `72e16acd`; lines after the mouse edit need remapping.
Do not interpret their status as a scan of the edited file.

The source-boundary review separately inspected the NTVDM-to-standalone
predicate changes in `reset.c`, `c_main.c`, `ppi.c`, and `ios.h` against the
same XP locations. The 20 paired matcher entries represent ten original
frames and ten corresponding local frames, not twenty independent defects.
The local predicate terms select the existing CCPU TLS, CMOS/FDC, PPI or I/O
behavior; they are retained and recorded in the candidate disposition.
The V7VGA case was also independently realigned by its enclosing function,
retained code and both original siblings; the added packed-scanout guard is
a distinct local exception. Six additional low-anchor video/serial groups
were checked against their actual XP surrounding source; their selected
siblings and operation contexts match.

A separate retained-arm pass compares non-directive code inside each matched
sibling arm. It skips any current arm wholly replaced by `#error` and skips
nested condition bodies; an anchor must be unique among matching-condition
siblings on both sides and occur in the same arm. It found at least one such
anchor for 1,108 of 2,369 currently matched repeated groups. The frozen
pre-edit review queue receives 1,104 directly corresponding anchors; the
changed mouse file is excluded from this cross-version merge. This evidence
helps locate groups and does not itself prove all child structures or resolve
unequal parent conditions.

When the outside-boundary and same-arm anchor evidence are combined and
recursively constrained by the matched parent chain, the current experimental
matcher finds locating evidence for 1,875 repeated and 1,168 unique-sibling
groups. It leaves 494 repeated and 77 unique-sibling matched groups without a
complete evidence chain. These 571 groups, as well as the 57 unmatched or
arm-mismatch candidate entries, remain under review. This is an aid to narrow
manual checking, not a declaration that all evidenced matches or all ancestor
exceptions are correct; same-arm uniqueness is calculated among the paired
siblings, and files containing unmatched siblings must be checked separately.

In `host/inc/cfpu_def.h`, six added `LITTLEND` guards independently wrapped
adjacent locally reordered floating-point layout declarations. No intervening
conditional mutates that macro. The first guard already rejects non-little-
endian configurations, so the following five were redundant and have been
removed while retaining the original structs, layout and existing `#line`
directives. Both actual architecture compilers produce byte-identical
selected preprocessor output before and after; forcing LITTLEND undefined
still reaches the retained first error. The first guard is recorded as the
necessary local exception. Batch build/test subsequently completed as recorded below.

`base/inc/cpu_vid.h` had two added checks of the identical conjunction
`CPU_40_STYLE && C_VID`. The first already emits a hard error when either
term is absent, and no intervening code changes either macro. The later
conditional around the local glue prototypes was removed, preserving the
prototypes and the necessary first ABI guard. Both compilers produce identical
selected preprocessor output, and each of the three unsupported combinations
still reaches the retained error. Full batch build/test subsequently completed as recorded below.

`base/system/ica.c`'s local PIC snapshot capture already enforces
`CPU_40_STYLE && !NTVDM` and `HOOKED_IRETS`, each with an explicit error
otherwise. The same checks around later restore statements were removed; no
macro mutation occurs between the sections. Actual x86/x64 compile-context
preprocessing remains byte-identical, and localized probes forcing each of
the three invalid states hit the respective first guard. Snapshot state and
runtime ordering are unchanged. Full batch build/test subsequently completed as recorded below.

`base/keymouse/ppi.c` first rejects the unsupported case where neither
NTVDM nor SOFTPC_STANDALONE is defined, inside an XP-original NTVDM parent.
After confirming no intervening macro change, duplicate local rejection was
removed in ppi_outb and snapshot capture/restore. The original selected
ppi_outb NTVDM parent remains, with its intentionally omitted old speaker
body recorded as an exception. The restore-only HostPpiState call remains
conditioned on standalone. Preprocessing is identical in actual x86/x64
compile contexts and in two isolated local NTVDM macro combinations; a fourth
localized combination still triggers the first error. These macro simulations
do not establish support for NTVDM. Batch qualification subsequently completed as recorded below.

`base/bios/reset.c`'s local standalone-only guard inside the retained
original `#ifndef NTVDM` path rejects `!NTVDM && !standalone`. The later
equipment-selection frame already rejects the same configuration, and an
independent NEC_98 error precedes it when that alternative is selected.
There is no intervening mutation of NTVDM or SOFTPC_STANDALONE. The extra
local guard was removed; the original outer frame and explanation of the
omitted DOS mouse-driver body remain. Actual x86/x64 preprocessing is
identical, and forcing neither macro at the local point reaches the later
error in both architectures. Full batch build/test subsequently completed as recorded below.

`host/src/nt_graph.c` first rejects !SOFTPC_STANDALONE at top level. Three
later identical error guards and one standalone wrapper around an existing V7
selection were redundant: no include or macro mutation intervenes. They were
removed; the first guard, V7 condition, original NEC sibling and rendering
code remain. Both actual architecture preprocessor outputs are byte-identical;
locally undefining standalone at the first check triggers its retained error.
The XP native fullscreen content remains an explicitly described subset
omission. Full batch build/test subsequently completed as recorded below.

In `base/video/gfx_updt.c`, the original CPU40 guard in the retained NEC
parent's CGA-mark setup already rejects the unsupported CPU mode before the
local CHAIN2 repair. The added guard around `SetMarkPointers(4)` was removed,
but that T88 functional repair remains and is still accounted for. No
relevant macro changes between the two checks. Actual x86/x64 preprocessing
is identical, and a localized invalid CPU mode hits the original guard. Full batch build/test subsequently completed as recorded below.

`base/inc/egacpu.h`'s local `VGAGLOBALSETTINGS` forward declaration is
required for the selected generated type, but its extra `#ifdef C_VID` frame
is not required to declare an incomplete C tag. The frame was removed while
the declaration and later selected type/fail-fast check remain. All 50 actual
x86/x64 dependent compile contexts were preprocessed before and with the
candidate header through a verified include overlay; outputs were byte-identical.
The non-CVID configuration can now see an incomplete tag, so identity of all
hypothetical consumers is not asserted; previous actual-context probes showed
non-CVID is rejected. Full batch build/test subsequently completed as recorded below.

`host/inc/host_cpu.h` contained an added `#ifdef CCPU` around common WRITE
ABI declarations immediately before XP's original CCPU-specific WRITE group.
No macro mutation intervenes. The extra first frame was removed and its
mandatory non-CCPU error moved to a new else on the original frame; that
else is recorded as the remaining local structural exception. All ABI
declarations remain. Existing `#line 442` incorrectly mapped XP's original
CCPU-specific group (XP line 419), and was corrected to `#line 419`. With
both architecture compilers the selected header preprocessing is identical
before and after; CCPU undefined triggers the retained error both before
and after. Full batch build/test subsequently completed as recorded below.

## Ten-file delivery batch — qualification

Relative to `72e16acd`, mirror production numstat from `git diff --numstat`:

| File | Added | Deleted |
| --- | ---: | ---: |
| `base/bios/reset.c` | 0 | 3 |
| `base/inc/cpu_vid.h` | 0 | 2 |
| `base/inc/egacpu.h` | 0 | 2 |
| `base/keymouse/mouse.c` | 4 | 0 |
| `base/keymouse/ppi.c` | 1 | 10 |
| `base/system/ica.c` | 0 | 4 |
| `base/video/gfx_updt.c` | 0 | 4 |
| `host/inc/cfpu_def.h` | 0 | 20 |
| `host/inc/host_cpu.h` | 3 | 5 |
| `host/src/nt_graph.c` | 0 | 11 |
| **Total** | **8** | **61** |

The original actual-context tests for each change are recorded above; they
prove the stated preprocessor behavior, not guest use of every display mode.
Cached builds and mirror/documentation gates passed on x64/x86. The two EXEs were refreshed. x64 passed all 186 background and 8 native
tests. x86 passed all 186 background tests; its first native run passed seven
and failed `package_smoke/compact_console` (stage 1, error 6, child exit
0xc0000142). A subsequent read-only session query showed the local Console
active, so disconnection is not established as the cause. A complete x86
native-group rerun passed all eight. The failure and rerun are separate facts;
this is not an all-green first run and does not establish the cause.

Published SHA-256:

- `softpc32.exe`: `b852275a70b719f064873afac04b944353f4a5b97d68b32f548d5186ca03af71`
- `softpc64.exe`: `a4b48e493e54a1bb4211cdc3be144fd29e2b9dde9f7fd73c77299211051f7267`

Build logs contain 5,246 x64 and 5,195 x86 original-mirror warning lines;
no non-mirror warning line was detected. No test source, INI, guest media
or snapshot was changed. S30's independent structural review remains open.


An arm-body structure sweep additionally found zero current arms with a
direct `#error` plus other non-comment code or nested conditions. That
lexical fact does not prove an excluded branch was correctly classified.

## Post-P6 continuation

After `6e510e9d`, the parser was rerun against all 475 manifest paths: 471
text files, four binary files, zero missing original files and 40 overlapping
structural candidate rows in 15 files. This is a candidate count, not a count
of defects. The refreshed [P6 tree disposition](m9-t99-s30-p6-tree-disposition.tsv)
records each current/XP location and distinguishes directly rechecked local
exceptions from prior evidence that still needs independent review. The older
independent repeated-group queue remains anchored to `72e16acd`, so changed
files must be remapped before its line numbers are used. No claim is made that
the remaining repeated siblings have all been audited.

The [post-P6 repeated queue](m9-t99-s30-p6-repeated-queue.tsv) was rebuilt
from `6e510e9d` instead of applying stale physical line numbers. It contains
2,381 repeated-sibling occurrences, including 12 for which the sequence
matcher has no current/XP pair. Twenty-one previously hand-reviewed or
registered locations were carried only when path, condition and XP line
matched; all other entries remain explicitly pending. Outside-group and
same-retained-arm source anchors are provided as leads, not automatic approval.

The refreshed [P6 group accounting](m9-t99-s30-p6-group-accounting.tsv)
records 3,614 automatically paired groups. To avoid certifying a false
alignment, its parent-chain evidence excludes all 15 files with unmatched
condition candidates. Of the remaining groups, 1,031 have unique sibling
conditions, 1,219 have both outside-group unique source anchors, and 407 have
a unique shared token within a retained corresponding arm; each also has an
evidence-bearing parent chain. Another 460 groups still lack such a chain,
and 497 paired groups lie in candidate files. An arm anchor never inspects
the original body when the current arm is an entire `#error`. These are
structural matching evidence, not proof of unchanged runtime behavior.

Within the 471 text-file universe, 267 have no conditional groups on either
side. Of 204 files with groups, 110 have this evidence chain for all their
paired groups and no mismatch candidate; the remaining 94 contain either
ambiguous pairings or candidate differences. Four binary paths are outside
the preprocessor-tree comparison. The remaining evidence chains and 94 files
still require independent completion audit before S30 can be declared done.

The ten manually paired XP-to-local NTVDM predicate extensions were then
re-entered as paired parents, rather than letting the sequence matcher stop
at their differing conditions. The [extended-parent review](m9-t99-s30-p6-extended-parent-review.tsv) records the ten pairs: their
sibling kinds match, no retained direct child group remains unpaired, and
wholly error-replaced local arms are skipped. One skipped XP arm contains
its original `#ifndef PROD`, intentionally not reconstructed inside the
local error. The differing parent predicates themselves remain registered
as explicit standalone or CCPU TLS exceptions.

## XP diagnostic-line corrections after P6 — batch in progress

Seven same-condition competing-line candidates were checked against their
actual XP function and condition boundaries. `base/inc/gvi.h` already has
the correct post-guard `#line 645` (XP guard ends at 644). Six other existing
line controls in five files referred to old locations that coincided with
other XP guards. Their values were corrected to the immediately following XP
source line without changing any conditional directive or body:

| File | Old -> XP line | XP anchor |
| --- | --- | --- |
| `base/keymouse/mouse_io.c` | 4735 -> 4938 | Selected `get_min_max_virtual_coords` guard at XP 4937. |
| `base/system/at_dma.c` | 351 -> 417 | Selected master-clear guard at XP 416. |
| `base/video/ega_prts.c` | 2617 -> 2667; 2760 -> 2824 | Selected `ega_gc_outb_index` / `ega_gc_mode` guards at XP 2666 / 2823. |
| `base/video/vga_mode.c` | 386 -> 439 | Excluded `GISP_SVGA` group ends at XP 438. |
| `base/video/vga_vide.c` | 432 -> 451 | Selected `vga_set_palette` guard at XP 450. |

The actual x86 and x64 command for each of the five translation units was
preprocessed before and after: all ten selected outputs were byte-identical.
This establishes those selected preprocessing outputs, not every possible
macro configuration. The ten-file batch subsequently passed dual-architecture build and full
testing; submission follows the actual-diff review. Other line-control locations and branch structures remain unaudited.

For repeated groups without unique single-line anchors, an additional pass
compares unique contiguous two- through four-line source sequences strictly
outside the whole conditional group; sequences do not cross conditional
directives, and an anchor is rejected if the same condition occurs between
it and the group. The current 2,369 paired repeated groups yield 1,433 with
two such anchors, 550 with one and 386 with none. Against the earlier
outside/same-arm evidence, 72 previously unsupported pairs gained two n-gram
anchors; 298 gained only one and 121 gained none. These remain search leads,
not automatic independent review or proof of local behavior. The P6 queue
records the physical current and XP line spans for reproducibility.

A subsequent boundary scan of already paired condition groups found 338
additional diagnostic-line candidates in 40 files. A candidate requires an
immediate `#line` after a retained arm or complete group and an identical
next source line at the paired XP boundary; group/parent matching remains
an explicit prerequisite, not an assumption. Within four of the five files
already in the next batch, 65 such mappings were corrected; the six directly
inspected corrections above make 71 line controls in five files. The
[per-location ledger](m9-t99-s30-p7-line-mapping.tsv) lists the old/new
numbers and adjacent XP content. The remaining candidate rows are not yet
classified; the [candidate ledger](m9-t99-s30-p7-line-candidates.tsv) records
all 338 rows, including 128 still awaiting review in 31 files. Actual x64/x86 preprocessing before versus after all 71 changes
is byte-identical for each of the five translation units. This does not
prove a global line-number audit or any not-yet-inspected configuration.

The same adjacent-source test was applied to another five compiled mirror
files: `gfx_updt.c` (43 locations), `vga_prts.c` (31), `ega_vide.c` (28),
`keyba.c` (27) and `printer.c` (16). Together with the initial five files,
this batch corrects 216 individual line controls in ten files; the
[per-location ledger](m9-t99-s30-p7-line-mapping.tsv) identifies every
old/new value and matching XP adjacent source. Actual compile-command
preprocessing for all ten files is byte-identical before and after on both
architectures. The scanner is conservative but its individual matches remain
subject to batch review; this does not claim every possible line control was
autonomously classified. Both architectures built and published successfully. Each passed 186/186
background tests and 8/8 native desktop/audio tests. Both architecture mirror
and documentation gates passed; submission follows actual-diff review.
The x86 and x64 EXE SHA-256 values are respectively
`e6acd5a0fa85c05a056d55cbca42a4f4ec07d1933be7276d902b8f4d451e1481` and
`4aff101082327b1a2cbaf1dc0d3dde0581ebbb6fe57a467909d672b3088793f7`.

P8 independently checked the adjacent condition boundary and next source for
89 previously pending line candidates in ten compiled files. All corresponding
XP originals contain no existing line controls. The [P8 mapping](m9-t99-s30-p8-line-mapping.tsv)
records original and corrected values; the [updated queue](m9-t99-s30-p8-line-candidates.tsv)
leaves 39 candidates in 21 files pending. The actual x86 and x64 preprocessing
outputs for all ten translation units match byte for byte before and after.
This is diagnostic line mapping, not proof that all structural differences
have been audited. Both architectures built and each passed 186/186 background
and 8/8 native tests; actual-diff review precedes submission. Build logs contain
840 x64 and 807 x86 warning lines, all from the preserved mirror paths; no
non-mirror warning line was observed. Published x86 and x64 EXE SHA-256 are
`9b7acd9d865de7350b8226bd839b5b7c26cdaa8da1c857fec7738b5651f296d8` and
`24a4e0758c16653303f92dd94c58203ff6d4c388c1e235766fa5a76245754a2b`.

P9 maps 25 further line candidates in ten compiled C files, using the
matching XP boundary and next source line; the [mapping](m9-t99-s30-p9-line-mapping.tsv)
and [candidate queue](m9-t99-s30-p9-line-candidates.tsv) preserve the evidence.
Fourteen line candidates remain pending, including header cases requiring
actual inclusion-context validation. P9 preprocessing and qualification
passed: both architectures built, selected preprocessing remained byte-identical
for ten translation units, and each passed 186/186 background and 8/8 native
tests. Build logs contained 820 x64 and 803 x86 mirror-path warnings, with no
non-mirror warning line observed. Published x86 and x64 EXE SHA-256 values are
`cde8f48c3cb0fc7a204070b1a8fcc0a2ae48c1c35b80e77b0c8aaa12d58d52cf` and
`19982265e27369d51504016aec8e248f7297359db44ed1013aeabd207675e5e7`.
No conclusion about the remaining conditional trees is implied.

P10 maps ten further XP line candidates in six C files and four headers.
The [mapping](m9-t99-s30-p10-line-mapping.tsv) and [queue](m9-t99-s30-p10-line-candidates.tsv)
record old and new lines, leaving four `sas.h` candidates pending. The actual
source units including the changed headers are included in two-architecture
preprocessing comparisons. Nine translation units per architecture matched
before and after, covering all six changed C files and representative actual
consumers of the four headers. Both architectures built and each passed all
186 background and 8 native tests. Build logs have 2292 x64 and 2242 x86
warning lines, all with mirror paths; no non-mirror warning line was observed.
The x86 and x64 EXE SHA-256 values are respectively
`74a5b6afea8fb7edd3797eeee1820c7f490ab70ddcb586052e7a0d472035d570` and
`c141e2b77ca92d37242772638f9413ee201a52cfde5753fe08f5277295701753`.
This queue concerns line mapping, not complete conditional-structure review.

P11 checks the four remaining adjacent-line candidates in `base/inc/sas.h`,
recorded in the [mapping](m9-t99-s30-p11-line-mapping.tsv). Four actual
translation units that include sas.h were preprocessed on each architecture;
all outputs were byte-identical before and after. This exhausts the 338-row
line-boundary candidate queue, not all possible line controls. A separate
[descendant-source anchor lead list](m9-t99-s30-p11-descendant-anchor-leads.tsv)
examines the 460 previously unresolved paired groups: 35 have distinct
descendant anchors, 425 lack them. Anchors are search leads, not proof or
classification. Conditional structures remain unresolved. P11 built and
published both architectures, each passing all 186 background and 8 native
tests. Build logs contained 4247 x64 and 4196 x86 warning lines with mirror
paths, and no non-mirror warning line was observed. The x86 and x64 EXE
SHA-256 values are respectively
`e9b96f3eea0ac481cdf149f3c3f077ead96832a6427d99a74c654fcacf91b3c6` and
`6d0e5c0d07d6c32882d9d46fd65b02c21c79ba3ab78dae057f2d255a1aa0cf9e`.

P12 inspected opening and closing boundaries and original adjacent context
for nine root groups with descendant anchors. All nine root boundary pairs
match. After the gfx_updt.c XP group, XP line 5967 declares
`host_stream_io_update`; SoftPC omits this unused declaration with the
owner-approved NT Console stream exclusion. A repository source search finds
no remaining consumer, and S20 records exclusion of the corresponding host
implementation. This omitted declaration is a specific local subset exception. Each child
remains a separate audit item; root boundary evidence alone does not clear it.
The [root review](m9-t99-s30-p12-root-boundary-review.tsv) retains the exact
locations. This manual review uncovered seven stale line controls in three
files, including cases with blank lines or nested directives that the earlier
scanner deliberately did not classify. The [mapping](m9-t99-s30-p12-line-mapping.tsv)
records each XP next line. P12 selected preprocessing matched for three actual
translation units per architecture, including an actual gvi.h consumer. Both
architectures built and each passed 186 background and 8 native tests. Both
build logs contained 1526 mirror-path warning lines and no non-mirror warning
line was observed. Published x86 and x64 EXE SHA-256 values are respectively
`0666972945093cc8fcfde280f5548b3c3be81cbac338d4b6cbf079d4cff09418` and
`0d613f0e51d03ccec783c6e3274154e2f86748d70d07d8423f892373ac260b02`.

P13 extends the boundary search to three following nonempty source lines,
including cases where the next line is blank or a directive. Of 913 leads in
55 files, 850 also have an exact normalized prefix through the first anchor;
63 have a differing prefix or offset and require individual review. The
[lead list](m9-t99-s30-p13-following-line-candidates.tsv) remains a search aid,
not proof of parent matching. The first batch changes 603 line controls in ten
files with matching prefixes; every location appears in the
[mapping](m9-t99-s30-p13-line-mapping.tsv). P13 selected preprocessing matched for nine actual translation units on
each architecture, including a cpu4gen.h consumer; both builds passed. Both
architectures passed 186/186 background and 8/8 native tests. On x64 the first
two complete native runs each failed compact_console before AttachConsole,
with child status 0xc0000142 and fixture stage 1 / error 6; the desktop session
was active. A standalone compact test passed, as did two subsequent complete
native runs, the last without concurrent x86 background testing. No cause has
been established; the initial failures are not erased by the reruns. x86
passed its native group on its first run. Build logs contained 5230 x64 and
5179 x86 mirror-path warning lines and no non-mirror warning line was observed.
Published x86 and x64 EXE SHA-256 values are respectively
`78dc57790f0760827da3367f35a82cf5eea8e972df33e5034256c34cd964eb69` and
`fb13d3919f78e44622ed658a7264e7181ac28810854b21b0583985c893b764c6`.

P14 changes 135 matching-prefix candidate line controls across ten compiled
files. The [mapping](m9-t99-s30-p14-line-mapping.tsv) and [updated queue](m9-t99-s30-p14-following-line-candidates.tsv)
record the batch and retain 175 pending candidates, including the 63 divergent
prefix/offset cases. Selected preprocessing matched in ten actual translation
units per architecture; both architectures built and passed 186 background
and 8 native tests. The first x86 native group had one compact_console child
initialization failure, stage 1 / error 6 / 0xc0000142; the complete group
subsequently passed 8/8. The cause is not established. The x64 native group
passed first run. Each build log contained 807 mirror-path warning lines and
no non-mirror warning line was observed. Published x86 and x64 EXE SHA-256
values are respectively
`60b351fb55a7be92c805ece5ae50f89b8bcbce5e7c717bce5d7ad121f72ec3b8` and
`4c3a053dfb487f5a16cf2104aab2c752ed9e626e0bd506601ae1dd64cd1320a3`.
No structural audit completion is implied.

P15 changes 59 matching-prefix candidate line controls across ten compiled
files. The [mapping](m9-t99-s30-p15-line-mapping.tsv) and [updated queue](m9-t99-s30-p15-following-line-candidates.tsv)
record the batch, with 116 candidates still pending. Actual preprocessing
matched for all ten translation units on both architectures; both built and
each passed 186 background and 8 native tests. Build logs contained 762 x64
and 712 x86 mirror-path warning lines, with no non-mirror warning line observed.
Published x86 and x64 EXE SHA-256 values are respectively
`3ed83ba43ea2b5d220f2296c89ada58c53233f368dbea67b4989784a3193ff53` and
`bee2010cefdf4aa2e5dca711d5d05862d438d1cc209bcaa8316ffa81977252b4`.
This does not complete conditional structure review.

P16 changes 34 matching-prefix candidate line controls across ten compiled
files. The [mapping](m9-t99-s30-p16-line-mapping.tsv) and [updated queue](m9-t99-s30-p16-following-line-candidates.tsv)
record the batch, with 82 candidates still pending. Actual preprocessing
matched for all ten translation units on both architectures; both built and
each passed 186 background and 8 native tests. Both build logs contained 536
mirror-path warning lines, with no non-mirror warning line observed.
Published x86 and x64 EXE SHA-256 values are respectively
`07fc48290579d94b3890e5f9c68b39f6d732526aa5d63c4a0687259070bca4bf` and
`296b89e6d81a6bf0003c360cd00f9dc7b5fbd9bc02fb7e88a390083730f536f7`.
This does not complete conditional structure review.

P17 changes 16 matching-prefix candidate line controls across seven headers
and three C files. The [mapping](m9-t99-s30-p17-line-mapping.tsv) and
[updated queue](m9-t99-s30-p17-following-line-candidates.tsv) record the
batch, with 66 candidates pending. Actual header consumers were identified
using Ninja dependency records. Seven actual consumer/changed translation
units per architecture produced byte-identical preprocessed output. Both
architectures built and each passed 186 background and 8 native tests. The
first x64 native group had one compact_console child initialization failure
(stage 1 / error 6 / 0xc0000142); the complete rerun passed 8/8. Its cause
remains unconfirmed. The x86 native group passed first run. Build logs had
3092 x64 and 3041 x86 mirror-path warning lines, with no non-mirror warning
line observed. Published x86 and x64 EXE SHA-256 values are respectively
`f3adbde1cbc8e0f8e074a192ca26f117ccfc12067665ac13648ec7bab6425608` and
`d994b0479441cd62fba15be455438b1d114d18192dd419dc6d5b4ee0cd760c15`.
Conditional structure review is not complete.

P18 classifies the 63 differing-prefix/offset leads: 46 already map the
first actual XP source anchor correctly, 11 do not, and six have another
line directive between the candidate and first anchor. The
[offset review](m9-t99-s30-p18-offset-review.tsv) does not claim that a
correct first anchor proves subsequent contents. This batch corrects nine
of the direct mismatches and three previously matching-prefix leads in ten
files using the actual anchor offset; see [mapping](m9-t99-s30-p18-line-mapping.tsv)
and [updated queue](m9-t99-s30-p18-following-line-candidates.tsv). Two direct
mismatches, six intervening-directive cases and further structural group
checks remain open. Seven actual consumer/changed translation units per
architecture produced byte-identical preprocessing; both architectures built
and passed 186 background and 8 native tests each. Build logs contained
4425 x64 and 4374 x86 mirror-path warning lines, with no non-mirror warning
line observed. Published x86 and x64 EXE SHA-256 values are respectively
`e89c1a793dbd6ff3604cdd3345a00bd62893cf0c1c5c12c5935d81b6b40d919e` and
`3587a8472b39df209eeac623a5df8d71ed7e9234396a92ef3eb9d4ac59803456`.

For the 46 leads with a correct first source anchor, a further comparison of
the next three nonempty lines and their XP diagnostic line numbers verified
34 for those three anchors; 12 need more review, including intervening
directives or unequal subsequent line distances. The [three-anchor record](m9-t99-s30-p18-three-anchor-review.tsv)
is evidence only for those tested anchors, not complete line or branch proof.

P19 independently checked the 12 first-anchor cases with fewer than three
visible subsequent anchors: each available source anchor before the next
line directive matches its XP logical line. Of six intervening-directive
cases, four earlier mappings and one effective later mapping were corrected;
two earlier immediately overridden directives have no intervening source
and are recorded as inert, not as proof of other code. Two remaining direct
anchor mismatches were corrected. The [disposition](m9-t99-s30-p19-offset-disposition.tsv)
and [seven-value mapping](m9-t99-s30-p19-line-mapping.tsv) retain the evidence.
No conclusion is made about as-yet-unreviewed nested conditional structures.
Six actual translation units per architecture produced byte-identical selected
preprocessing; both architectures built and each passed 186 background and 8
native tests. Build logs contained 541 x64 and 540 x86 mirror-path warning
lines, with no non-mirror warning line observed. Published x86 and x64 EXE
SHA-256 values are respectively
`0ee983daeb4772ccb93da32e6a30bb40aee968791d079f02a5f45c3930c67f36` and
`a58d1e9da3285c321ece3cf99e247493395fe458afb5dfa8ea3da3a9e747f595`.

The remaining 26 descendant-anchor leads were inspected at their opening and
closing directives and adjacent original source. Their reported boundary pairs
correspond; cases where the next XP arm contains code but the current arm is
whole-arm #error are not descended into. The [nested-boundary record](m9-t99-s30-p19-nested-boundary-review.tsv)
records exact positions and anchors. Some parent pairs remain pending, and
children are separate audit items, so these boundary observations do not claim
that all 26 complete subtrees or their full ancestor chains are verified.

P20 propagates independently inspected group-boundary evidence only through
verified ancestor chains: 27 of the previously inspected boundaries have
supported ancestors, and nine unique sibling groups under those ancestors were
rechecked for equal arm signatures. The [propagation record](m9-t99-s30-p20-group-propagation.tsv)
and [nine-group proof](m9-t99-s30-p20-unique-branch-proof.txt) retain their
locations. Six further original root groups in bootstra.c and tape_io.c were
matched to existing whole-arm #error groups by their original opening/closing
and adjacent context; their excluded contents were not examined. The
[excluded root review](m9-t99-s30-p20-excluded-root-review.tsv) records each.
Six additional c_intr.c root PIG groups were checked against XP in their
actual real-mode, protected-mode, declaration and fault-reset contexts. Two
retain a matching else arm; wholly excluded arms were not descended into.
The [interrupt root review](m9-t99-s30-p20-interrupt-root-review.tsv) records
each boundary. Together these support 48 of the previous 460 pending paired
boundaries; 412 remain pending. Matched boundaries do not automatically prove children,
whole files, or remaining differing-parent cases. Both incremental builds
and all 186 background plus 8 native tests per architecture passed on the
first run. No production source changed; the x86 and x64 EXE hashes remain
`0ee983daeb4772ccb93da32e6a30bb40aee968791d079f02a5f45c3930c67f36` and
`a58d1e9da3285c321ece3cf99e247493395fe458afb5dfa8ea3da3a9e747f595`.
