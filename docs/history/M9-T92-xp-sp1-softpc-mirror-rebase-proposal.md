# M9: XP SP1 SoftPC Mirror Baseline Upgrade

## Admission checkpoint (2026-10-15)

Owner admits T92 after accepting and closing T91. S1 supplies the source audit
and migration design; the subsequent owner implementation objective activates
S2's independent original-device repairs. CURRENT owns its executable packet.
The following XP import batches remain planned, not silently executed by admission.
The current accepted baseline is `087e512f`, with 498 retained mirror paths.

Both reference HEADs match the pinned revisions below. However, the XP local
index reports staged removals and untracked replacement paths under softpc.new.
S1 must audit committed Git objects at the pinned revision, not assume that
working-tree bytes are pristine. Do not reset or otherwise modify that external
repository. Existing historical counts below require recomputation against
the accepted T91 endpoint. Source notices and admission findings must be
resolved before any later source copy; this checkpoint imports nothing.

S1's source audit and migration recommendation are recorded in the linked
semantic audit and its two complete path ledgers. The immutable-object census
gives 338 equal, 45 XP-only-change, 74 local-only-change and 41 three-way paths
after CRLF normalization. Source classification is complete for this frozen
profile; later runtime qualification and source admission remain separate.
The owner's clarified objective is to include IBM PC capability upgrades and
exclude non-IBM-PC capabilities; a host path or regional macro alone is not
proof that useful IBM PC semantics should be excluded.

## Objective

### Owner execution update (2026-10-15)

The owner subsequently states “批准导入xp源码。” This is the requested owner
direction under the source-admission stop condition, not a claim that a
third-party license was discovered. Import only fixed XP commit
a2f6d7c93aa4f11efc51147566d5961c59a8ef89 from
Source/XPSP1/NT/base/mvdm/softpc.new to the same relative paths under
src/app-softpc/softpc.new, preserving existing Microsoft/Insignia notices.
The fixed root/selected subtree contain no separately named license file.
No new license, altered notice or redistribution assertion is introduced.
S3's path ledger, local fixes, profile exclusions and tests bound this import.

Original direction: “我可以async验收，请你继续自动准入下一个和后续的各个S，
只在T任务收口前等我。” Subsequent S tasks no longer wait for individual manual
acceptance: complete implementation, required verification, pushed P and
coordinator actual-change review, then admit the next serial S. S2 meets that
technical handoff standard; no owner manual-pass result is invented. T92
closure still requires the owner. This changes the acceptance cadence, not
the external-source admission policy or the separate sound-task scope.

Upgrade the repository-owned selected SoftPC mirror from its OpenNT/NT4
baseline to the frozen XP SP1 SoftPC baseline:

```text
XP SP1: winxpscodes/Source/XPSP1/NT/base/mvdm/softpc.new @ a2f6d7c93
Target: src/app-softpc/softpc.new
OpenNT: opennt-src-2/nt/private/mvdm/softpc.new @ 5e4619ab6
```

At closure, the ordinary mirror review and divergence gate is **the necessary
XP SP1 subset -> `src/app-softpc/softpc.new`**. OpenNT remains a frozen lineage and migration
comparison source; it is not the routine acceptance baseline.

The selected product remains one normal i386 IBM PC/AT/V7 machine:

```text
CPU_40_STYLE + CCPU + C_VID + CPU_486 + SPC386 + V7VGA + WDCTRL_BOP + PROD
```

This is a source-baseline upgrade, not an import of the XP NTVDM product.
`NEC_98`, NTVDM/DOS/WOW/VDD/PIF/CSR host routes, and non-x86 machine
families are explicit exclusions. Regional IBM PC keyboard improvements are
not excluded merely because they concern a locale; regional host workarounds
remain separate from hardware semantics. A false compile-time guard is never by
itself an adoption decision: every retained or removed XP branch must have a
recorded disposition in the migration ledger.

## Current Baseline And Evidence Status

The former comparison used old roots (`src/mvdm/softpc.new`, then
`src/core/softpc.new`). T83 has since relocated the recovered mirror to
`src/app-softpc/softpc.new`, restored selected OpenNT CRLF bytes, and made
reviewed C-VID/Types boundary changes. The current mirror has 498 retained
paths, of which 493 are C/H paths. Consequently the former A=58, B=28 and
66-port-difference counts are historical evidence, not an admission-ready
current universe.

[XP SP1 SoftPC 逐文件语义审计](../etc/evidence/softpc/xp-sp1-hunk-semantic-audit-zh.md)
contains the current T92 checkpoint followed by explicitly historical T40
material. The ScanCode/virtual-key Pause distinction is in
host/src/nt_keycd.c, not keyba.c. The historical table is not an approved patch
list; the T92 checkpoint records both useful XP fixes and unsafe XP narrowing.
Before source import, S1 must re-run its classification against the current
path set and current selected defines. Its old “final source file destination
ledger” is superseded by the destination model below.

## Ownership And Source-Admission Boundary

`src/app-softpc/softpc.new/` becomes the selected XP-derived source mirror.
It remains the owner of recovered CPU, BIOS, controller, video, ROM and device
semantics. `app-softpc/compat` owns replacement original-host callbacks and
larger host adaptations; `app-softpc/machine` owns the injected driver,
guest-input/frame conversion, debug and trace; `app-softpc/product` owns
configuration and application policy. Moving the mirror baseline must not
create a second device, renderer, media or lifecycle implementation.

The first source-changing S records, before copying any XP source, its
immutable revision, origin/destination paths, applicable notice/license
information, owner authorization, intended boundary, semantic delta and
focused proof, as required by the Source And Research Policy. XP and OpenNT
checkouts remain read-only comparison material and never become build or
runtime dependencies.

## Per-Path Destination Model

S1 assigns every retained mirror path and every XP-only candidate one of these
four outcomes. The choice is based on selected IBM PC/AT requirements,
not on minimizing whole-file XP differences or its A/B text label.

| Outcome | Meaning | Required proof |
| --- | --- | --- |
| **XP necessary subset** | Include only reviewed required XP ranges, preserving XP formatting; an entire file qualifies only if all its content is justified. | Inclusion/exclusion ranges, dependencies, dual-width build and focused regression. |
| **XP necessary subset + local patch** | Reapply necessary standalone/x64/port-ABI and accepted functional fixes to that same subset. Never exclude a range merely because it has a local modification. | Exact patch rationale, ownership and runtime proof. |
| **Existing out-of-scope remainder** | Exclude newly introduced unrelated XP changes. Owner additionally permits removal of proven useless existing conditional branches within the migration scope; do not infer uselessness merely from a false condition. | Dependency proof and symmetric necessary-subset accounting, no new host/device route. |
| **Do not introduce** | Omit a path or XP change that is solely foreign hardware or product-host support and has no selected standalone value; classify regional IBM PC changes separately. | Explicit exclusion record and build-source proof. |

Necessary-source selection is mandatory within retained files. A false guard
does not justify foreign hardware, regional-host or NTVDM product imports.
Resolve shared declarations/data explicitly; do not erase all conditionals.
Extraction is migration/audit work only, never a build-time source transform.
Report separately S-start delta, patches against the necessary XP subset and
excluded upstream content. Existing unrelated content is not silently cleaned
up. Sound Blaster remains a separate candidate.

## Known Semantic Starting Points

The refreshed ledger is authoritative; the following old-audit conclusions are
starting hypotheses only.

| Area | Current conclusion to verify in S1 |
| --- | --- |
| CMOS, printer, GFI/FDC value narrowing | Check source/destination width and range, not just cast presence. XP also introduces unsafe 8-bit narrowing before 16-bit setters in ROM, disk BIOS and mouse paths; retain correct local widths. |
| Disk, floppy and BIOS media paths | IBM PC/AT hardware source is a candidate for XP-first adoption, while image/direct/readonly/overlay ownership remains a local standalone patch. |
| `nt_keycd.c`, `keyba.c` | Host translation distinguishes Pause from NumLock; controller tables separately add ordinary PC key mappings. Preserve local bounds checks and prove scan-set translation, make/break and Ctrl-Alt-Del. |
| AT DMA auto-init | XP restores current address as well as count; current code only reloads count twice. Adopt the fix independently of the sound-position host hooks. |
| CMOS extended-memory validation | XP fixes comparison/mask precedence in unselected cmosnt.c; the selected cmos.c still contains the same two errors. Transfer the narrow fix to the existing owner, without importing a second CMOS implementation. |
| V7 cursor-enable bit | XP adds necessary parentheses around the two bit masks; current comparison is always false. Adopt within the existing V7 port/render path. |
| Reset, PIC, ROM, timer and C-VID/video | XP-first three-way migration candidates. Existing x64 ABI, fixed-ROM/media, quick-event, IRQ and copied-frame constraints must be explicit local patches and regression criteria, not reasons to keep OpenNT as baseline. |
| Original `nt_*` host endpoints | XP NTVDM host implementations are excluded as host implementations. A self-contained selected-machine algorithm may be moved only with an explicit Compat/Machine owner and proof. |
| PC-98, non-x86, regional host services | Exclude foreign hardware and product-host dependencies; classify regional IBM PC hardware/table improvements separately. |

## Owner-approved two-task split (2026-10-15)

Original owner direction: “总体来说，按照你的划分，请你整理文档、队列，
划分为2个T和合适的各个S任务。” The supplied four-item repair brief is
captured below as a separate planned S2. This update admits planning only,
not production changes, source import, or automatic S1 acceptance.

T92 owns existing-core XP baseline migration and its necessary corrections.
The [XP Sound Blaster candidate](m9-xp-sound-blaster-standalone.md) owns new
DSP/FM/MIDI capability and is first in Queue, without allocating a T number.
T92 may close without sound only after accounting for this explicit owner-
approved transfer; it must not claim sound support. The earlier S1 audit's
batch identifiers are frozen evidence: old S2/S3/S4 map to new S3/S4/S5,
old S5 moves to the sound candidate, and S6 retains final qualification.

## CMOS implementation clarification

Pinned XP contains BOTH base/system/cmos.c and base/system/cmosnt.c.
XP base/system/sources.inc selects cmosnt.c; local CMakeLists.txt selects
cmos.c. The latter retains the general SoftPC host-callback implementation;
cmosnt.c is a substantial NT-integrated RTC implementation, not just a rename:
it includes nt.h/windows.h/nt_eoi.h, Rtl time conversion, GetLocalTime,
host_ica_lock/unlock, delayed IRQ delivery and the host-driven RtcTick path.
The local cmos.c already carries media-topology and snapshot adaptations.

The current build choice is proven; the original author's historical motive
is not independently established. Keeping the existing selected owner avoids
introducing the NT timing/interrupt implementation. The two comparison fixes
are not merely inspiration: the same precedence defect is present in the
selected cmos.c. S3 transfers and tests those expressions locally; it does not
copy cmosnt.c or assert that all of its RTC differences are irrelevant.

## Proposed Serial Work

### S1 — Current XP migration ledger and source admission

Freeze the current 498-path mirror universe, the XP path universe, and exact
raw/normalized hashes. Recompute the three-way relation using the current T83
source root. For every retained mirror file and XP-only candidate, record one
destination outcome, selected-profile reachability, required local patch,
owner, and test receiver. Record external-source admission facts before the
first payload enters the tree. No runtime source changes occur in S1.

### S2 — Independent original-device repairs and ROM narrowing regression

Re-read current source and applicable authorities before implementation.
The owner's supplied audit is a repair requirement, not implemented or dynamic
proof. Scope is three production files under softpc.new and owner-local tests;
rom.c stays unchanged if the current correct semantics are confirmed.

1. **DMA auto-init, base/system/at_dma.c:** keep the first count reload and
   replace the duplicated second reload with base_address -> current_address.
   Exercise actual DMA transfers through terminal count and a second round,
   auto-init disabled, increment/decrement and both controller widths where
   reachable. Do not import SB position hooks or audio/NTVDM wiring.
2. **INT 13h AH=15h, base/disks/diskbios.c read_dasd:** trace BIOS/BOP returns
   and capacity computation first, then split sector count into high/low
   16-bit CX:DX, not current /256 and %256. OpenNT is already wrong; rejecting
   XP's UCHAR cast alone is insufficient. Tests enter the actual command path,
   independently calculate expected registers, AH/CF, distinguish 8/16-bit
   splitting, cross 65,536 sectors, and cover absent drives. Geometry, mode
   and mounting policy are unchanged.
3. **V7 register 0xA5, base/video/v7_ports.c:** parenthesize both 0x80 masks
   in the enable transition comparison, retaining existing draw/clear and
   host_start_update/host_end_update. Port-write tests cover off->on, on->off,
   unchanged enable bit and other attribute changes without a false transition.
4. **ROM CS, base/system/rom.c do_search_for_roms:** preserve current
   setCS((addr & 0xf0000) >> 4); do not import XP's UCHAR narrowing or edit
   production merely for symmetry. A test-generated minimal valid ROM must
   prove full 16-bit initialization CS, without external firmware or assets.
   Record the retained expression as a necessary XP-local correction.

Provisional production estimate: approximately +5/-5 lines (net zero), three
mirror files; confirm exact statement layout before admission. Test estimate:
+150–300/-0 lines across about 2–4 owner-local fixtures/registration paths,
net +150–300, subject to existing harness reuse. Do not expand a test framework
to meet this estimate. Before coding provide a file-by-file brief; afterwards
report actual production/test additions, deletions, net and OpenNT/XP divergence.

Use one existing path; no fallback, special case, duplicated device logic,
unrelated formatting, condition reordering or EOL changes. No Lib/Emulator/
public X86 changes without approval, no user INI/media/snapshot edits.
Sweep newly narrowed conversions; report other findings for admission rather
than silently expanding S2. Distinguish source proof, tests and real guest
reproduction; do not attribute prior user symptoms to these defects without
evidence. S3/S4/S5 must preserve these repairs when applying XP bodies.

Deliver dual-width background builds, refreshed assets/binary EXEs, targeted
tests and required full shared/App unit/integration regression. Arrange native
desktop tests under execution rules without Computer Use. Commit/push the
complete P, actual-change review and evidence; await owner acceptance.

### S3 — XP-first mechanical and controller/BIOS basis

#### Owner-directed necessary-subset correction

Original request: “请立即纠正 T92 当前 XP 源码导入方案。先暂停后续导入和提交，
审计当前未提交改动；不要整体撤销已有正确修复，也不要干扰用户改动。”
The owner requires necessary XP source, not whole retained files; exclude new
foreign model/regional/NTVDM product bodies, retain valid XP/core and local
repairs, and adopt XP formatting for selected ranges. Preserve shared data
dependencies, avoid blanket preprocessor deletion, new runtime branches and
unrelated historical cleanup; keep sound separate. The subsequent owner amendment
allows deleting proven useless existing conditional branches within the migration
scope, while preserving the minimal necessary-subset difference. Record ranges at
the fixed revision, count S-start/local-subset/exclusion deltas separately,
then complete dual builds, all tests, publication, pushed P and actual-change
review. Async serial S acceptance remains; T closure still waits for the owner.

The earlier whole-file policy and its +7,118 production net addition were
wrong. No S3 implementation commit was made. Its old test results do not
qualify the corrected subset. The working source was backed up only as ignored
audit evidence, not rolled back wholesale. The 24-file regional correction
removes 7,053 lines including new branch delimiters, retaining the IBM PC
arms. A second pass excludes VDD dynamic loading, Sound Blaster adapter-number
changes, WOW fields, native NT menu/UI changes and other NT-only semantic
updates. The first corrected checkpoint retained existing unneeded branches.
The owner now permits their evidenced removal, without new runtime selection.

Conditional-cleanup checkpoint: first review covers base/bios/tape_io.c and
base/bios/cmos_bis.c. Remove the unselected NTVDM A20/configuration branches,
their private declaration and a disabled trace, plus the NTVDM exclusion around
the selected CMOS time implementation: expected +0/-20. Keep CPU/PM/segmentation
conditions and source notices. Compare actual x86/x64 preprocessor output before
and after, then include the final candidate in the full S3 regression. This is
an initial bounded cleanup, not a claim that all 38 paths have been cleaned.
Actual result: +0/-20, net -20; the four x86/x64 preprocessor outputs for these
two files are byte-identical before/after. Regenerated necessary-subset accounting
remains +681/-23: the deletion removes only excluded text, not a required local
patch. The updated range ledger records the new source positions. Other existing
At that first checkpoint, NTVDM ranges in 22 files remained under dependency
review rather than being automatically deleted; the reviewed groups below
supersede this intermediate status.
At this checkpoint the cumulative S3 production delta is +1,794/-1,790, net +4.
Both incremental device builds and all five device-contract cases per width pass
after cleanup. No new full-suite or refreshed
package claim is made for this intermediate source candidate.

Next reviewed cleanup group: diskbios.c, fla.c, at_dma.c, illegalp.c, time_day.c
and dma.h, expected +0/-201. Remove only the independently listed NTVDM ranges:
NT hard-disk hiding, asynchronous FDC completion, VDD DMA accessor and declaration,
NT clock acquisition and exception presentation. The synchronous AT/FDC/DMA/clock
implementations remain. Source search finds no outside fdc_command_completed
caller or dmaGetAdaptor consumer. Compare each affected translation unit's actual
dual-width preprocessed output before/after, then run the final S3 regression.
Actual second-group deletion is +0/-201; all ten preprocessor comparisons are
byte-identical. The necessary-subset local delta remains +681/-23. Together with
the BIOS cleanup this removes 221 lines across eight files without increasing
the functional subset patch. Both device builds and all five focused tests per
width pass. Remaining conditional ranges are not
yet removed. Full S3 delivery and publication are still outstanding.

Final reviewed NTVDM cleanup group: 16 files, expected +0/-1,375. It covers
com.c/printer.c/printer_.c, floppy.c/floppy_i.c, bios.h/config.h/floppy.h/gfi.h/
host_lpt.h/ios.h/printer.h/rs232.h, ios.c, cmos.c and host_inc.h. Delete only
the reviewed NTVDM range records and the two declaration/alias exclusions
nested inside those same arms. Retain the actual UART/parallel/FDC/RTC paths,
native pointer adaptations, snapshots, bit-order and CPU conditions. NT's
physical-host locking, WOW dispatch, alternate ROM locations, VDD slots,
Windows header aliases and alternate RTC owner are not SoftPC dependencies.
Fourteen actual device translation units are preprocessed for each width
before/after, including consumers of the changed declarations. Final dual
builds and all registered shared/App/background/desktop tests follow this group.
Actual final-group deletion is +0/-1,375, matching the estimate. The three groups
together remove 1,596 lines in 24 files; no replacement implementation is added.
The final post-cleanup 28 preprocessor comparisons all pass byte-for-byte.
The x64 build, five device cases and 90 shared tests pass; the full-regression
supervisor continues with App tests and x86. Published EXEs are not yet a
qualified final delivery while this run remains incomplete.
Final source accounting against S3 start is 34 production files (33 mirror and
one Compat), +1,759/-3,331, net -1,572. Git's aligned add/delete counts differ
from subtracting removed lines from the previous hunk totals; the net change
is consistent. The independent necessary-XP-subset comparison is still
+681/-23, net +658. The retained mixed DELAYED_INTS || NTVDM condition is not
dead code: its shared delayed-interrupt body remains selected by that separate
feature, so it is not deleted or rewritten just to remove the symbol spelling.

The review universe is exactly 38 paths: the original 37 XP-changed assigned
paths plus selected cmos.c. [XP ranges](../etc/evidence/softpc/xp-sp1-s3-subset-ranges.tsv)
partition each pinned XP file; [local ranges](../etc/evidence/softpc/xp-sp1-s3-local-ranges.tsv)
record the corresponding independent selection of current files.
[Per-file counts](../etc/evidence/softpc/xp-sp1-s3-subset-files.tsv) include
the XP subset SHA-256 and local added/removed lines. Line numbers are one-based,
inclusive, after CRLF-to-LF only; retained text is otherwise unchanged.
Concatenate include ranges in order, append LF after each line and compare
with git diff --no-index --numstat. Empty projections have one final LF for
the recorded hash. These are audit metadata, not build source generation.

Selection rejects NEC_98/JAPAN/KOREA/DBCS/ARCX86 and NTVDM-only arms;
unknown/shared conditions stay intact. Literal constant branches are reduced
symmetrically in audit only. The NT-only monregs/nt_pif/nt_reset headers have
no selected build consumers; retained old bodies are not deleted. Conversely,
Ninja dependencies show host_inc, host_rrr and nt_uis ARE included: only their
specific unused alias-reset/menu/resource additions are excluded, never the
whole headers. Source uses of the three old menu resource declarations are
declarations only. Independent explicit exclusions cover the two DMA sound
queries/header, Sound Blaster adapter constant, NT LPT BOP declarations and
VDD-only dispatch scaffolding. This selection does not depend on whether a
line differs locally. Full local snapshot/ABI repairs remain in comparison.

| File(s), relative to softpc.new | Required XP adoption and exclusion rationale |
| --- | --- |
| base/bios/cmos_bis.c | Byte-valued CMOS port arguments; preserve XP format. |
| base/bios/tape_io.c | Full descriptor limit/A20 declarations; exclude Japan NT virtual-memory/BIOS services; retain configuration-table segment and correct helper ABI. |
| base/comms/com.c, base/inc/rs232.h | Existing UART widths/prototypes; exclude PC-98 8251 state, ports, baud and IRQ implementation; retain UART snapshots and all intptr_t calls. |
| base/comms/printer.c | Existing PC parallel status widths; exclude PC-98 printer implementation and new changes in NT-only callbacks; retain snapshots. |
| base/comms/printer_.c | Selected BIOS formatting; exclude new foreign wrapper and NT-only BOP declarations; remove the proven unused old NT arm under the owner amendment. |
| base/comms/rs232_io.c | Existing INT 14h byte/port widths; no PC-98 route. |
| base/disks/diskbios.c | Existing AT BIOS register widths; exclude PC-98 BIOS services, retain S2 full CX:DX split rather than XP truncation. |
| base/disks/fdisk.c | AT geometry/ROM-byte casts and XP format; retain HDD snapshots; remove only new PC-98 wrappers. |
| base/disks/fla.c | FDC byte casts/XP format; preserve callback declaration and snapshot handlers, no new controller. |
| base/disks/floppy.c, base/disks/floppy_i.c | PC floppy declarations, byte writes and conditional retry label; exclude PC-98 BIOS/ports/transfer implementation. |
| base/disks/gfi.c, base/disks/gfi_mpty.c | Existing result/config-index widths and XP format; same media backend. |
| base/inc/base_def.h, base/inc/config.h | XP comments; no configuration policy change. |
| base/inc/bios.h | XP declaration spelling and format; no new PC-98 BIOS area or Japan/Korea BOP declarations. Existing unrelated declarations remain, not new services. |
| base/inc/dma.h, base/inc/floppy.h | PC declarations/layout; exclude PC-98 register/port arms and unused NT VDD/ROM declarations while preserving selected DMA and floppy contracts. |
| base/inc/gfi.h | XP byte-valued FDC packet setters; same command schema. |
| base/inc/host_lpt.h, base/inc/printer.h | PC declarations/XP format; no PC-98 one-port ABI. |
| base/inc/ios.h | XP formatting, retain full standalone I/O space; exclude PC-98 maps, new sound slot and associated VDD slot changes. |
| base/inc/tape_io.h | No necessary change; exclude new Japan BIOS constants. Original DOS EOF handling remains separately visible. |
| base/support/ios.c | Existing word output casts/XP format; exclude new dynamic VDD loader/retry and remove the unused old NT dispatch arms; preserve normal I/O table owner. |
| base/support/time_day.c | CMOS daylight bit cast/XP format; exclude PC-98 clock services. |
| base/system/at_dma.c | XP selected AT body/format, S2 reload retained; exclude sound-position hooks and PC-98 ports, retain private adapter and snapshots. |
| base/system/cmos.c | Two comparison fixes transferred from XP cmosnt.c; retain cmos.c owner, topology and snapshots. |
| base/system/illegalp.c | Correct defined() syntax and byte writes in existing path. |
| host/inc/host_def.h | Preserve selected declarations/ABI; no new PC-98 port counts or ARCX86 emulation-ROM declaration. Returns to baseline. |
| host/inc/host_gen.h | XP word-stack cast/format; no alternate CPU path. |
| host/inc/host_inc.h | Remove unused NT Windows-header/alias arm; retain selected standard host includes. |
| host/inc/host_rrr.h, host/inc/monregs.h, host/inc/nt_pif.h | No required new semantic changes; retain previous content, reject new NT/region host changes. |
| host/inc/insignia.h | XP spelling/pragma; retain native pointer and generated C-VID word ABI. Remove accidental duplicate STRINGIFY guard. |
| host/inc/nt_reset.h, host/inc/nt_uis.h | Only selected XP whitespace remains; reject new WOW session/NT resource semantics. |

First corrected checkpoint accounting against S3-start 5ba0a557: production 32 files
(31 mirror plus one Compat), +1,794/-1,770, net +24; tests +91/-0 and root
CMake +4/-0. This is not a claim of +24 new functionality: XP formatting is
included, notably at_dma.c and ios.h. The necessary-subset local difference is
+681/-23, net +658, mostly pre-existing snapshots and ABI adaptations. It
also explicitly counts the BIOS capacity repair, descriptor declaration,
CMOS correction and small whitespace/EOF differences. It is not the S delta.

The excluded-XP ledger counts 8,991 lines, including original out-of-profile
content and selection delimiters, NOT 8,991 newly introduced lines. The same
projection excludes 1,899 pre-existing local lines from functional comparison;
these were retained at the first corrected checkpoint. The subsequently approved
cleanup removes reviewed portions, accounted separately above. Do not subtract
the XP and prior-local exclusion totals to invent a patch
size. The actual corrective worktree reduction is reported separately.
The full-file source notices are preserved, including headers omitted from
the functional comparison; no license text is removed or changed.

The first corrected subset passed 148/148 registered tests on each width:
90 shared, 53 other background and five native desktop cases. This evidence
predates the subsequent BIOS conditional cleanup; its renewed qualification is
pending. Initial whole-file x64 passed its
background suites; x86's only later failure was an erroneous pending S3 record
under history, causing the gate to demand S4. The draft is now ignored evidence
until closure; CURRENT stays S3 and the documentation gate passes. Neither
that bookkeeping failure nor an earlier merge fault is hidden by the new run.

Superseded whole-file checkpoint (2026-10-15): the three-way import covered the
37 assigned files, plus cmos.c and the existing descriptor adapter. Estimate
approximately +8,900/-2,000 production lines before final compiler review;
most additions are upstream guarded source, not newly selected devices.
Overlap resolutions preserve the S2 disk split, DMA snapshot functions,
FDC callback declaration and native-pointer/C-VID scalar ABI. The single
legacy non-ASCII comment in com.c is preserved byte-for-byte during import;
DOS EOF bytes are removed as compiler input adaptation, not whitespace cleanup.
Focused descriptor/CMOS proof is estimated at +100-180 test lines, using the
existing device fixture. Final accounting will distinguish local delta and
XP-relative retained patches; preview is not build or runtime qualification.

Implementation checkpoint: first x64 build passed, but all five device tests
faulted on startup at host_com_ioctl's output write. A repeated-text merge
mistakenly treated a pointer-width patch at another call as proof that
flush_input's original intptr_t cast had survived. It had not. Restoring the
existing cast made all five ordinary CTest device cases pass; a debugger-only
pass was not accepted. Positional git merge-file review replaced substring
presence as the migration proof and also corrected a misplaced, unselected
NEC_98 closing guard in printer_.c. This is an executor merge defect, not an
XP device-policy defect. First failure logs remain retained separately.

The new descriptor case checks 0xffff, 0x12345 and 0xffffffff decoded limits,
adjacent sentinels, actual AH=87 copying above 1 MiB, A20 restoration and flags.
CMOS tests use the existing POST/read/write entry points for valid and corrupt
low/high bytes. They prove resulting bytes, not whether an invisible redundant
CMOS correction branch executed: that precedence repair also requires source
inspection. No claim ties these defects to an earlier user-reported symptom.

#### S3 final executor audit (verified delivery)

| Requirement | Actual implementation/evidence | Status |
| --- | --- | --- |
| Fixed necessary XP subset | The 38-path XP/local range ledgers partition the selected files at the pinned revision; every include/exclude interval and subset hash is recorded. | Source audit complete |
| Preserve accepted local behavior | Existing UART/HDD/FDC/CMOS/printer snapshots, native-width casts and I/O width remain in the local patch comparison; no shared-package or snapshot-schema change. | Source audit and final regression pass |
| Preserve S2 repairs | Disk capacity uses 16-bit CX:DX split; DMA reload restores address/count; V7 and ROM files are unchanged in S3. | Source checked; full regression passes both widths |
| Avoid unnecessary host/device imports | New PC-98/regional/VDD/sound/WOW payload rejected; 1,596 lines of independently reviewed old NTVDM/dead conditional content removed. | Implemented |
| Cleanup does not alter selected code | Fourteen actual translation units per architecture have byte-identical preprocessor output before/after final conditional cleanup. | 28/28 comparisons pass |
| Descriptor and CMOS correctness | Existing descriptor decoder ABI now exposes sys_addr limit; two CMOS mask comparisons fixed in selected cmos.c; two owner-local test cases added. | Five device cases pass both widths |
| Full regression and publication | Shared suites first, then other unit/integration, then five reserved desktop cases per width; assets/binary is the existing CMake executable output. | Each width passes 90 shared + 53 other + 5 desktop, 148/148; both EXEs refreshed |
| Protected boundaries | Git changed-path check shows no public src/test package, INI, media or snapshot edit; external references remain read-only. | Checked |
| P delivery and independent role review | Commit/push only after qualification; coordinator reviews actual executor commit separately. | Pending |

Counted source delta: mirror 33 files +1,757/-3,329 (net -1,572), Compat one
file +2/-2 (net zero), tests one file +91/-0, root CMake +4/-0. Total counted
production/test/build +1,854/-3,331, net -1,477. Documents, range ledgers and
EXEs are not disguised as production-code lines. Relative necessary-XP-subset
patches are independently +681/-23, including historical local repairs rather
than only S3 work. Selected XP formatting is preserved, including whitespace
that the original mirror exception permits; non-mirror diff checking passes.

Both final Release builds pass and refresh the existing package output. The
x86 Ninja graph again reports `premature end of file; recovering` and rebuilds
584 steps; no deliberate clean/cache deletion was performed. This build-cache
observation is not treated as a device failure or silently repaired in S3.
Verified final-candidate SHA-256 values:

- softpc32.exe: `5bca801ef022a5fd987ce0b019bbcf2cba845dd8467a663dab645eeb20e7dd89`
- softpc64.exe: `cdf400443abb60bffe0198477371b1260c4c2afba8cc2ecb2fa26778b4561134`

The final hidden supervisor exits zero, and every configure/build/preprocess/
focused/shared/App/desktop exit record is zero. Shared elapsed times are
x64 52.56 s and x86 52.04 s; other background 96.72 s and 75.21 s; native
desktop 13.29 s and 14.00 s. Focused device cases repeat five members of the
148 registered tests and are not double-counted. Actual package startup and
native Console/Window paths passed; no new manual Win3.x/Win95 guest session
or real sound-device qualification is claimed. INI, media, snapshots and all
public packages are unchanged. Documentation and non-mirror whitespace gates
pass. Earlier failed merge and superseded qualification records remain distinct.

#### S3 coordinator actual-change review of executor P1

Executor `89b67c44` is pushed. The coordinator inspected its changed paths,
source/Compat ABI pair, source additions, protected-tree diff, subset ledger,
EXE hashes and final zero exit records. P1's full tests pass, but the actual
source-addition review finds two newly imported xms A20 helper declarations in
tape_io.c left behind after deleting their NTVDM callers. These declarations
are unnecessary in that translation unit and must not survive merely to match
an unfiltered XP file. Remove exactly those two lines, preserve surrounding XP
spacing, and independently exclude them from the XP necessary subset. The
existing guarded declarations/callers in sas.h/keyba.c belong to S4, not this
repair. No implementation or ABI changes. Repeat the dual build/full regression
and publish a complete corrective P before accepting S3. This is a review
finding, not a guest-visible failure; closure and S4 admission remain pending.

#### S3 corrective executor P2 accounting

The actual corrective production diff is tape_io.c +0/-2: only the two
unused A20 declarations are removed. Their surrounding XP spacing and the
selected AH=87 implementation are unchanged. The XP range ledger excludes
these declarations independently; it does not erase a local implementation
patch from comparison. XP excluded lines are now 8,993; the frozen previous
local unselected count remains 1,899. Necessary-subset local patches remain
+681/-23.

Cumulative S3 production against 5ba0a557 is +1,757/-3,331, net -1,574:
33 mirror paths +1,755/-3,329 and one Compat path +2/-2. The one test path
remains +91/-0 and root CMake +4/-0, making counted production/test/build
+1,852/-3,331, net -1,479. This supersedes P1's cumulative totals, not its
historical verification record. The corrective delivery repeats both builds
and the full registered suites before commit; no additional runtime behavior
or public API is introduced.

Corrective qualification is complete: both Release builds pass; each width
passes 90 shared, 53 other background and five native desktop tests (148/148).
The focused five-device subset also passes on both widths. All supervisor and
phase exit records are zero. Shared/App/desktop wall times are x64
50.77/97.92/16.03 seconds and x86 64.42/71.35/15.42 seconds. No Computer Use,
INI/media/snapshot edit or new manual guest-session claim is involved.
Corrective package SHA-256:

- softpc32.exe: `269f738ee4b52c1227f1babecfc4a6acfcca216e1427cff92e2d0700dbab4f01`
- softpc64.exe: `84447c00e1630bf570315dca3047cd9242023d61bca9383e267a4da617b01ca9`

The similar-issue recheck finds no xms A20 helper reference in tape_io.c;
the four guarded declarations/calls in sas.h/keyba.c are explicitly assigned
to S4. The XP/local range changes affect only this two-line exclusion and the
resulting source length/hash. Documentation governance and non-mirror
whitespace checks pass. Executor P2 is ready for push and separate actual-change
review; S3 closure is not inferred from test results alone.

Adopt the approved necessary XP subset for representation, CMOS, printer,
GFI/FDC, disk/floppy and BIOS. Exclude newly introduced unneeded host/product
branches, retain required local patches and selected XP formatting. Keep
the selected image media contract and prove floppy-only, hard-disk-only and
dual-media operation as applicable. Include the AT DMA auto-init address reload
fix, but not the unresolved Sound Blaster host-position hooks. Explicitly
reject XP's newly introduced register truncation; do not label it mechanical.
Transfer XP's two CMOS memory-byte comparison fixes from cmosnt.c into the
selected cmos.c. Prove valid and corrupt memory-size bytes, including an
extended-memory value whose low unmasked expression exceeds 255. Unselected
NT memory, physical-media, VDD DMA and heartbeat implementations are not new
device owners; the semantic audit records their local receiver or exclusion.

For tape_io.c, adopt XP's sys_addr descriptor-limit locals only together with
the matching sys_addr-pointer declaration and existing Compat helper definition;
XP leaves that non-CPU_30 declaration inconsistent. This is one mirror
declaration-line correction plus an estimated Compat +2/-2, not a new decoder.
Prove full-width descriptor limits, adjacent sentinels and INT 15h AH=87 copy,
A20 restoration and flags. Do not mistake the current matching word-pointer
ABI for an already demonstrated stack overwrite, or import the separate NT
virtual-memory path as its fix.

### S4 — XP-first input, reset, IRQ, ROM and timing basis

S4 is automatically admitted after S3 P2 actual-change review under the owner's
serial async authority. Its frozen candidate universe is 18 relative paths:
base/bios/reset.c; base/ccpu386/fpu.c; base/inc/{ica.h,mouse.h,sas.h};
base/keymouse/{keyba.c,keybd_io.c,mouse.c,mouse_io.c,ppi.c};
base/system/{ica.c,idetect.c,rom.c,timer.c,timestrb.c,unexp_nt.c};
host/src/{nt_keycd.c,nt_sound.c}. A candidate is an audit obligation, not
permission to import its entire XP file. Initial audit changes no production
or test code (+0/-0). Before payload edits, give per-path disposition and
quantified source/test estimates based on the actual selected hunks.
Preserve the accepted InPort Win1/Win3 behavior, IRQ/event-wait/snapshot hooks,
zero-delay protection and current ROM CS width. Exclude XP's UCHAR truncation
of ROM CS and mouse BX, foreign-machine and product-host routes. nt_sound.c
does not authorize Sound Blaster integration. Use positional three-way review,
not substring matching, following the S3 merge-defect lesson.

#### S4 immutable-source audit checkpoint

Extract all 18 candidate bodies from the pinned XP/OpenNT Git objects and
freeze the local side at fbe255cf. The offline three-way inventory records
raw SHA-256 and normalized line diffs; no external working-tree bytes or
generated source become build inputs. This checkpoint changes production and
tests by +0/-0. Full-file churn is not the payload estimate: reset.c,
mouse_io.c and timer.c include extensive XP formatting and foreign branches.

Initial per-file disposition (range-level extraction remains to be completed):

| Candidate | Selected change / preservation requirement |
| --- | --- |
| base/bios/reset.c | XP declarations, selected scalar widths and formatting; retain local attached-floppy topology, native-width configuration access and omission of the software mouse-driver installation. Reject ARCX86/PC-98 startup routes. |
| base/ccpu386/fpu.c | XP changes one comment only. Keep accepted local FPU implementation and snapshot support; do not describe this as a CPU algorithm upgrade. |
| base/inc/ica.h | XP hardware additions are PC-98-only; preserve local IRQ ABI. DOS EOF marker is not required C source. |
| base/inc/mouse.h | XP additions are exclusively PC-98 port/IRQ definitions; do not import them. |
| base/inc/sas.h | XP adds PC-98 ROM layout. Keep local guest-width memory length and overwrite declaration; audit unused NT-only declarations independently. |
| base/keymouse/keyba.c | Adopt useful keyboard table changes, key-set representation and initialized temporary pointer; preserve snapshot codec and IRQ1 deassertion. Correct the omitted set-3 inverse entry for key 14 in the existing table. Exclude Korean host-disable policy. |
| base/keymouse/keybd_io.c | XP adds explicit byte-width stores and declaration. Existing native-pointer repairs are inside the NT setup route; establish complete branch exclusion before deleting that old route. Do not accidentally retain its repair without its caller or remove a shared definition. |
| base/keymouse/mouse.c | XP byte casts and port type coexist with the accepted InPort free-running timer/HOLD/IRQ and snapshot fixes. Do not restore the old Windows-specific interrupt bursts. |
| base/keymouse/mouse_io.c | Selected declarations/casts and initialization correction; retain local snapshot/cursor fixes. Reject unconditional SwitchToFullScreen and 8-bit truncation before setBX. Review register-range loop bounds before accepting its type narrowing. |
| base/keymouse/ppi.c | XP adds PC-98 disabling guards only; selected PPI and local snapshot/speaker behavior must remain. |
| base/system/ica.c | Selected width casts; retain local PIC/HLT/snapshot corrections. Exclude foreign IRQ mappings and NT hooks. |
| base/system/idetect.c | Preserve original selected idle algorithm and existing host event wait. XP threshold casts need a range justification because the selected state is int, unlike the NT word fields. No production caller of idle_set/IDLE_set was found beyond its declaration/macro. |
| base/system/rom.c | Adopt selected declarations and width-explicit byte output; reject ARCX86 host ROM routes and XP setCS UCHAR truncation. Existing ROM regression remains mandatory. |
| base/system/timer.c | Selected status-byte cast and XP formatting; retain zero-delay quick-event safeguard and complete timer snapshot state/rebasing. No second clock or sound path. |
| base/system/timestrb.c | PC-98 guard adds no selected behavior; preserve XP indentation of the retained rtc_tick call and existing scheduling. |
| base/system/unexp_nt.c | XP difference is a DOS EOF marker only; no device behavior to import. |
| host/src/nt_keycd.c | XP separates NumLock key 90 from Pause key 126 using virtual-key identity. Preserve table bounds checks; verify the actual standalone producer supplies the identity needed by that distinction. Native NT BIOS conversion is not needed. |
| host/src/nt_sound.c | XP SessionId/Beep endpoint policy is NT-host-only. Retain common tone/state logic and existing PCM endpoint; do not import NT device/session handling or new speaker semantics. |

The keyboard snapshot decoder validates scan_set in 1..3 before assigning
key_set and serializes through its existing copied integer state, not raw
sizeof(key_set). Thus adopting XP's byte-sized key_set does not by itself
require a snapshot schema change; runtime roundtrip proof is still required.
The added key 14 has set-3 make 0x5d but XP inverse[0x5d] remains zero, so
the existing per-key-type command would update slot zero rather than key 14.
This is a source-proven table inconsistency, not a claimed guest reproduction.

Two new narrowing reviews must remain distinct from confirmed defects. The
mouse register-range loop can wrap CH+CL after conversion to UCHAR, but valid
controller-register ranges are much smaller; do not claim a valid-input bug
without demonstrating reachability. idle_set has no selected production caller,
so its XP word cast is not evidence of a current idle-performance regression.
The final selection ledger must state their disposition rather than treating
all XP casts as automatically correct. No production edit has begun at this
checkpoint; payload estimates follow reviewed subset extraction.

Pause/NumLock producer check: machine/input.c converts KVM_KEY_PAUSE to
VK_PAUSE and copies both virtual-key and scan identity into KEY_EVENT_RECORD
before the existing KeyMsgToKeyCode call. Thus XP's Pause exception has the
required metadata on the real KVM path. The Compat scan-byte-only entry has
no virtual-key identity and should map 0x45 to NumLock, not fabricate Pause.
The current keycode fixture fingerprints the old table and never sets a
virtual-key identity; update its expected table proof deliberately and add
explicit Pause/NumLock cases, retaining all out-of-range and enhanced-key
checks. No second translation table or public input API is needed.

#### S4 host-subset review checkpoint

The offline candidate now covers all 18 admitted files, with 16 changed:
+5,848/-8,750, net -2,902 lines against fbe255cf after CRLF normalization.
These are provisional source-payload counts, not committed production changes
or final XP-subset patch counts. mouse_io.c (+3,774/-4,239), timer.c
(+1,199/-1,666), and reset.c (+672/-694) contain substantial XP formatting;
their textual churn must not be reported as equivalent new device logic.
Production sources, tests and package EXEs remain unchanged at this checkpoint.

Repository-wide source/test symbol searches confirm that the unused native
BIOS-to-input conversion in nt_keycd.c is the sole consumer of NULLCHARSCAN;
aNumPadSCode has only its definition and old header declaration. The selected
candidate removes that conversion, its tables and excluded NT includes while
retaining the real KeyMsgToKeyCode path and its accepted bounds checks.
The matching stale header declarations require disposition before delivery.
The native Beep handle/open/close names are confined to nt_sound.c's excluded
endpoint. Its candidate removes those routes but preserves all tone-state
calculations and the existing softpc_standalone_audio_set_tone call. This is
not an audio implementation change or Sound Blaster admission.

The unguarded XP AH=4f SwitchToFullScreen request is excluded explicitly;
conditional projection alone would wrongly retain this NT host policy.
XP's host_memset declaration uses int for length, whereas the actual Compat
definition uses unsigned int. The candidate matches that existing ABI with
a reason marker; no new adapter or API is introduced. Key 14's inverse mapping
is repaired in the existing table, not a second translation path.

Actual candidate review also caught a three-way merge scope defect: XP's
ARCX86 alternative leaves a redundant nested block around display_string,
which would hide the retained local stringLength from its final use. Both
shell braces are removed together; the retained body and native-width local
calculation are unchanged. This defect existed only in the offline candidate,
not a compiled or published source. The final independent XP/local range
ledger, full local-patch preservation audit and runtime proof remain pending;
these findings do not constitute S4 qualification.

#### S4 first production slice and focused proof

The reviewed nt_keycd.c subset is now applied, together with removal of the
two orphaned BiosKeyToInputRecord/aNumPadSCode declarations in nt_uis.h.
This is a justified S4 dependency follow-through into the header previously
reviewed by S3, not a new host interface. Production for this slice is
+14/-308, net -294; keycode_smoke.c is +27/-3, net +24. All other S4 source
candidates remain offline pending their final review and tests.

The test preserves the original mapping fingerprint except for the explicitly
asserted regular scan-45 change from Pause (126) to NumLock (90), and separately
checks VK_PAUSE with enhanced/non-enhanced make and break records. Existing
bounds and modifier invariants remain. x64 failed at the new NumLock assertion
on the old implementation, then passed after the migration. The initial x86
build overlapped the source update and passed; it is not admissible old-code
red evidence. A fresh post-change dual-width job is required, and its results
must be read independently before claiming both-width qualification.

Further actual-diff inspection caught the candidate's mouse bitmap declaration
reverting sys_addr to MOUSE_SCREEN_DATA*, despite retaining the fixed local
byte-address arithmetic. The offline candidate restores sys_addr; no published
source had that defect. This confirms that conflict resolution alone is not a
local-patch preservation proof. The redundant NT mouse-buffer declaration and
PC-98-only explanatory comment are also excluded with their absent consumers.
XP-retained whitespace currently produces a trailing-space diagnostic in one
nt_keycd.c table line; this must be accounted separately from functional diff,
not silently normalized contrary to the selected-source policy.

The independent post-change keycode run completed successfully on both widths
(one targeted test each). The next applied slice is keyba.c (+81/-698), with
the selected XP tables, inverse correction and existing snapshot/IRQ repairs.
irq_smoke.c adds 55 lines using actual controller ports: select sets 1/2/3,
roundtrip each through the existing keyboard snapshot codec, and verify the
three per-key type commands for keys 14/56/94/107 change only the addressed key.
The fixture restores its initial keyboard state before the existing IRQ/reset
tests. No production test hook, duplicate keyboard map or public API is added.
Both focused tests pass on x64; the corresponding x86 build/test remains an
owned background job at this checkpoint. Make/break and complete translation
coverage, remaining S4 payloads and full regression are still required.

That focused x86 run subsequently completed with both tests passing. The
fixture initially attempted make/break for all four table identities in all three
sets using original host_key_down/host_key_up, controller reads and the existing
continue_output entry; no implementation hook is added. The whole reviewed S4
candidate is now in the worktree: 17 changed production files, +5,846/-8,752,
net -2,906; two changed test files, +105/-3, net +102. The 18-file candidate
universe has two unchanged bodies (mouse.h and ppi.c), plus the two-declaration
nt_uis.h cleanup described above. CRLF mirror checkout is retained.

A fresh bounded hidden dual-width build and focused run is in progress for
this combined candidate. Earlier green results qualify only their earlier
slice, not the new make/break cases or the combined payload. Final independent
XP range accounting, exhaustive translation proof, full-suite qualification,
package refresh and coordinator actual-change review remain outstanding.

The combined x64 candidate built, but the new make/break fixture failed on
set 1 key 14 before any byte was queued. Dispatching the existing quick-event
queue did not fix it. Temporary diagnostics captured disabled=0, interface=0,
scanning_discontinued=0, fifo_count=0 and output_full=0. Source inspection
explains this: XP retains keytypes[14]=6 (not a key), whose make/break lengths
are zero, despite reserved values in its scan-code tables. This is a fixture
expectation defect, not evidence for changing production key classification.
The corrected fixture verifies no output for key 14 and actual make/break
for 56/94/107 across sets 1/2/3, while retaining the per-key command tests.
The corrected focused pair subsequently passes on both x64 and x86 (2/2
each). Temporary diagnostic output is removed. This does not qualify the
full suite.

The next fixture enumerates all 256 controller translation inputs through
the original FIFO/output path, using the existing snapshot codec to prepare
one bounded input at a time. The 255 ordinary outputs have fixed FNV-1a
fingerprint 9bd36c6f, independently extracted from the pinned XP selected
IBM-PC table; the test does not load that external source or production table.
F0 is separately checked as a split break prefix: no output before the next
byte, followed by Set-2 A (1c) becoming Set-1 break 9e. Initial parsing of the
unprojected multi-profile source was rejected because it contained more than
256 initializer entries; only the selected 256-entry table supplies the oracle.
Fresh focused tests pass on both x64 and x86 (2/2 each), including the full
translation enumeration. No source edits overlapped that running build.
After its zero exit and process termination were confirmed, full dual-width
qualification started: shared background tests, other background tests, then
the reserved serial native desktop tests. This run qualifies the current
candidate only; independent subset accounting/review still precedes delivery.

S4 accounting checkpoint: independent projection of the 18 pinned XP bodies
accounts for 31,371 lines, with 5,416 excluded by explicit conditional or
unused native-endpoint ranges. The current local bodies differ from that
subset by +1,389/-188; this includes pre-existing local repairs, not just S4
new functionality. The baseline-side conditional audit identifies 2,791
excluded old lines; this is not the entire deletion count because unguarded
unused host tables and XP formatting changes are counted separately. The two
orphan declarations removed from nt_uis.h are dependency follow-through and
are outside this 18-body sum. Native tone callback replacements remain in
the patch comparison rather than being hidden as excluded functions.
These review artifacts remain provisional until per-range checking and the
remaining narrowing dispositions are complete. x64 full background passes
90/90 shared plus 53/53 other tests. x86 and serial desktop verification are
still outstanding at this checkpoint; no full-pass or delivery claim follows.

The review ledger is now retained as [per-file subset counts](../etc/evidence/softpc/xp-sp1-s4-subset-files.tsv),
[pinned XP include/exclude ranges](../etc/evidence/softpc/xp-sp1-s4-subset-ranges.tsv)
and [pre-S4 conditional ranges](../etc/evidence/softpc/xp-sp1-s4-baseline-ranges.tsv).
Range numbers address normalized lines of the fixed XP object or fbe255cf
baseline respectively; hashes address LF-normalized selected XP bytes.
Every local body remains entirely in the comparison. Baseline conditional
counts are explicitly not a complete inventory of unguarded obsolete tables.
The files describe the current candidate; they are not a qualification result.

Review correction, before delivery: the earlier key-14 inverse "repair" was
inferred from a reserved forward-table value without checking keytypes[14].
That type is 6, explicitly not a physical key. There is consequently no
demonstrated valid-key requirement for changing XP reverse[0x5d] from zero
to 14. Remove this unnecessary new local patch after the active qualification
run ends; retain the no-output regression and per-key command coverage for
the actual newly supported keys 56/94/107. Earlier claims of a confirmed
inverse defect are superseded by this review, not evidence of an XP bug.
Both review corrections are now applied after the initial full candidate
passed 148/148 tests on each width, including all ten native desktop tests.
Final production is +5,843/-8,750, net -2,907; tests are +164/-3, net +161.
The 18-file XP-subset comparison is +1,389/-189. Sixteen pre-existing device
snapshot callbacks match their baseline selected bodies after whitespace
normalization; two timer callbacks differ only in removal of excluded NT
branches. This is supplemental source proof, not a substitute for runtime
snapshot tests. Final full qualification is running again against this exact
candidate, with no source edits during the run. The subset count ledger has
been regenerated; earlier candidate counts remain historical checkpoints.

The final ledger recheck rereads all 18 XP Git objects at the pinned revision,
verifies their original SHA-256 values, proves contiguous non-overlapping
range coverage and reconstructs each selected body byte-for-byte. All three
tracked TSV files match those outputs. The normal documentation/test diff
check passes. Mirror diff-check warnings are retained XP whitespace or
pre-existing local whitespace; four EOF tails match the selected XP tails.
unexp_nt.c additionally retains the empty line left by removing its DOS EOF
control byte, accounted as +1 rather than hidden. Thus the mirror-wide
diff-check command is not claimed clean. Final x64 background is again
90/90 shared plus 53/53 other tests; x86 and desktop stages remain pending.

Narrowing disposition: AH=F3's valid register ranges are bounded by the
existing CRTC/SEQ/GRAPH/ATTR capacities (25/4/9/20), so their valid end
indices fit UCHAR. Keep XP's narrowing; this is not a claim that malformed
out-of-range requests are validated by the old code. In contrast, idle_set
documents positive int thresholds and the selected storage is int, whereas
the excluded NT variant used word storage. Its two new word casts truncate
otherwise representable thresholds without serving the selected contract.
Preserve the original int assignments as an explicit local subset patch,
after the running tests end. No selected production caller was found, so
this prevents an import regression; it is not a demonstrated current CPU
idle defect or a reason to alter the accepted host wait implementation.

Perform the nontrivial three-way migrations for reset, keyboard/mouse, PIC,
ROM and timer by subsystem. Preserve required standalone topology in explicit
local patches. `nt_keycd.c` and `keyba.c` include Pause/NumLock/Ctrl-Alt-Del and
scan-set table proof; timer
includes zero-delay quick-event protection; controller work includes IRQ and
boot/media proof.
The seven selected keyboard tables have 60 changed elements. Preserve the
four changed table identities (14/56/94/107), preserving key 14's non-key
classification; verify all 256 translation inputs and the valid physical
keys' set-3 reverse mappings without changing the reserved 0x5d entry.
Test per-key type commands and reset as well as make/break; do not silently
import a new key that cannot receive its own attributes.

#### S4 final executor qualification

The final, frozen candidate completes both Release builds and all registered
tests on 2026-10-15. Each architecture passes 90 shared background tests,
53 other background unit/integration/gate tests and five native desktop tests:
148/148 each, with no excluded desktop case. The final supervisor and all
configure/build/test exit records are zero. Shared/App/desktop elapsed times
are x64 47.05/96.45/13.03 seconds and x86 51.56/68.95/13.80 seconds.
This supersedes the intermediate pending statements above, not their retained
failure/correction history. No source changes overlapped final qualification.

Production accounting against fbe255cf is 17 mirror paths, +5,843/-8,750,
net -2,907. Two test paths are +164/-3, net +161. The counted production/test
total is +6,007/-8,753, net -2,746; documentation, ledgers and EXEs are separate.
The independent 18-body necessary-XP-subset patch is +1,389/-189, including
accepted historical local repairs. The two orphan nt_uis.h declarations are
separately accounted dependency cleanup. Excluded XP content is 5,416 lines;
the pre-S4 conditional ledger identifies 2,791 excluded old lines, not the
entire deleted-line total. XP formatting churn is not new functionality.

Final package SHA-256:

- softpc32.exe: `eaeae823c62d16a434908a33d8c16f3deb2d77cdb01d54ebe090c25af373ad83`
- softpc64.exe: `49c0c725c852241c9c5fdd9e97a8cb0f131188664da8464071d58f88cf9e3ce9`

The source audit preserves the reserved non-key 14 without a new inverse-table
patch, full-width ROM CS/mouse BX and selected int idle thresholds. Actual
controller tests cover translation, valid-key make/break, per-key commands,
reset and snapshot roundtrips. The sixteen existing snapshot callback bodies
also pass the independent selected-source preservation check. These are source
and automated-test proofs, not a new manual Windows session or physical audio
claim. No public package, INI, media or snapshot file changed. The final builds
publish directly to the existing assets/binary executable outputs.

Documentation governance and non-mirror whitespace checks pass. The explicitly
recorded XP/local mirror whitespace exceptions remain; mirror diff-check is
not misreported as clean. Executor commit/push and subsequent coordinator
actual-change review remain required before S4 closure and S5 admission.

#### S4 coordinator actual-change review and delegated closure

Reviewed pushed executor 5c5af37a811eb8239beb4d32661a9ae9ec6b0a53 against
the S4 packet and owner subset-policy amendment. The actual 27 changed paths
are 17 mirror files, two owner-local tests, two EXEs and six evidence/governance
files. Added/deleted/net counts match the final executor ledger. The review
checks the actual source additions and selected local patches, rather than
accepting the green tests as a substitute for source review.

Full-width ROM CS, native configuration pointers and mouse bitmap addressing,
accepted InPort behavior, IRQ deassertion, selected idle thresholds and the
existing PCM endpoint remain. The reserved-key inverse change was withdrawn;
real-key command tests and reserved-key no-output proof remain. Removed native
NT endpoint declarations follow their removed consumers. No new device,
presenter, public interface or snapshot schema is introduced. The complete
fixed-object range recheck and sixteen selected snapshot-body comparisons pass.

All final phase exit records are zero; both architectures pass 148/148 including
the ten native desktop cases. Package hashes match the committed delivery and
origin/main matches the executor commit. Protected source/test packages, INI,
media and snapshots have no change. XP whitespace exceptions and earlier test
fixture mistakes remain disclosed. No additional blocking finding is raised.
S4 is implementation-closed under the owner's async delegation, not recorded
as a new manual owner test. T92 stays open; S5 is admitted next.

### S5 — XP-first C-VID and video basis

S5 begins at S4 executor 5c5af37a, following its actual-change review. The
frozen source-audit universe is the 71 paths assigned to original batch S4 in
xp-sp1-retained-paths.tsv: 31 XP-changed video/header/host paths and 40 local-only
C-VID paths whose XP/OpenNT base is identical. The latter are preservation
obligations, not permission to regenerate or rewrite accepted C-VID fixes.
Initial production/test delta is +0/-0. Before any payload edit, inspect the
fixed XP/OpenNT/current objects, declare per-file necessary ranges and local
repairs, and publish added/deleted/net estimates. Unknown conditions remain
until dependency review; neither all false guards nor whole files are imported
mechanically. Existing proven useless arms may be removed within this scope.

Preserve the T88 selected-marker, geometry and completed-paint publication
contracts, fixed text destination stride, glyph/palette/cursor state and
snapshot behavior. The targeted proof must map changed semantics to existing
video matrices or justified new tests, followed by both Release packages and
all shared/App background and reserved desktop tests. No frontend redesign,
new mode, Sound Blaster, user asset or public package change is admitted.

Migrate selected VGA/EGA/CGA/V7 source and declarations to the XP basis,
excluding product presentation routes while retaining the standalone copied
frame/surface path through the appropriate local owner. Verify CGA, EGA, VGA
and V7 mode, cursor, port and frame behavior on both host widths.

#### S5 reviewed payload brief (2026-10-15)

The frozen local input is ce19e215. Three-way selection covers all 71 admitted
paths; the 40 unchanged-base C-VID files remain byte-preservation obligations.
The current candidate changes 27 of the other 31 files: +3,644/-6,544,
net -2,900, including the selected XP formatting. These are pre-application
estimates, not tested delivery counts. Focused test additions are estimated
at 80-140 lines, reusing the existing video test and mode matrices.

Selected changes include explicit byte/word port and BIOS stores, complete
write-handler declarations, typed existing marker assignments, and matched
palette-loop counter changes. Keep the wide counters for CX-sized and
256-entry DAC operations. Eight merge overlaps were individually reviewed:
retain the existing glue declaration authority, dirty physical-row layout,
snapshot presentation-cache reset and detached DIB initialization; use XP's
equivalent corrected V7 enable-bit expression. Both video-controller snapshot
callback bodies remain lexically unchanged modulo whitespace in this candidate.
This preservation check does not replace execution tests.

Exclude PC-98/regional/NTVDM arms and the MONITOR/X86GFX native NT execution
and full-screen routes. Neither latter macro is defined by the selected build
or its headers. BIGWIN is defined and its painter implementations remain.
The old stream-output endpoint/state and full-screen declarations lose their
only users with those exclusions and are removed as a connected group.
Shared nt_fulsc.h definitions remain: the file is included by selected host
code and the video test, so its name alone does not justify deleting it.
Record original XP line ranges for exclusions, including orphan declarations
and comments; do not count them as new local functional repairs.

Before delivery, prove DAC operations at 256 entries, selected mode/cursor/
stride/geometry behavior and snapshot restoration, then run both Release
builds, all shared and other background tests, and ten reserved desktop tests.
Preserve all user configuration/media and public packages. No new rendering
path, generated build input or runtime source selection is introduced.

#### S5 applied candidate and verification checkpoint

The applied production candidate matches the reviewed 27-file delta:
+3,644/-6,544, net -2,900. The existing owner-local video test adds 73 lines
and removes none. Its actual video BIOS BOP entry verifies all 256 DAC entries
through block write/read, DAC state save, a different palette, and restoration;
canaries cover both guest-memory output boundaries. Both x64 and x86 focused
runs pass. Full regression is still pending; these results do not claim a new
manual guest test or S closure.

The finite 71-path ledger is retained as [file counts](../etc/evidence/softpc/xp-sp1-s5-subset-files.tsv),
[XP selection ranges](../etc/evidence/softpc/xp-sp1-s5-subset-ranges.tsv) and
[pre-S5 conditional ranges](../etc/evidence/softpc/xp-sp1-s5-baseline-ranges.tsv).
It excludes 12,095 XP lines and identifies 2,938 old conditional lines outside
the selected profile. These counts are not the Git deletion count: XP/local
versions differ, and selected XP formatting is also part of the migration.
The 71 current bodies relative to their XP subsets are +22,908/-23,257;
this includes the pre-existing 40-file C-VID ABI/marker patches, not 22,908
new lines introduced by S5. Independent reconstruction from the fixed XP Git
objects passes for all 71 bodies: original hashes, complete disjoint line ranges,
selected-body hashes and the three tracked ledgers agree. Both existing video
snapshot callback bodies remain unchanged modulo whitespace. Full regression
and final actual-change review remain necessary before delivery.

The whitespace check reports 19 mirror diagnostics: nonblank whitespace lines
match the selected XP source exactly, and EOF blank lines remain after excluded
tail removal. These are disclosed source-format exceptions, not a clean
whole-tree `diff --check` claim. New test and documentation whitespace checks
pass. Public packages and user INI/media/snapshots have no changes in this S.

#### S5 executor qualification (2026-10-15)

Final tracked production is 27 mirror files, +3,644/-6,544 (net -2,900);
one existing owner-local test is +73/-0 (net +73). No public package, Compat
API, snapshot schema, guest medium or configuration changes. The 40 C-VID
paths remain unchanged. Final selected source matches the reviewed candidate.

Both Release builds and all registered tests pass, 148/148 per architecture:
90 shared background, 53 other background, and five reserved native desktop
tests. x64 phase times are 56.08/97.35/13.60 seconds; x86 are
58.12/75.31/13.39 seconds. This includes real BIOS DAC entry testing, existing
CGA/EGA/VGA/V7/CHAIN2 mode and dirty matrices, snapshot restoration, package
flows, native Console and Window cases. No Computer Use was used. Evidence is
the S5 qualification-final phase logs and zero exit records, alongside the
independent 71-path pinned-object/range verification. These are automated
test results, not a claim of a fresh owner-operated Windows session.

The published package SHA-256 values are:

- x86: `f35cb644c0bead39baf7b2d29f8fa51c8ca44323c40adf2c2edae8dfc9003a38`.
- x64: `6e57b7e29088fdda0c80b219bdf6cdf1dd3eda7e013e4a9bb065c93eae8f6423`.

Similar-issue review covers the 31 XP-changed video candidates: byte port/BDA
stores retain their destination width; signed cursor offsets retain modulo
16-bit assignment; bit-field getters remain bounded; CX-sized and 256-entry
palette loops retain wide counters. Existing full-width pointer ABI and selected
dirty dispatch are preserved. Removed native NT declarations are checked with
their excluded callers, not removed by filename. The known out-of-scope TODOs
remain unchanged. Documentation and non-mirror whitespace checks pass; the
19 explained mirror-format diagnostics remain disclosed above. Executor push
and a separate coordinator review of the actual commit precede closure.

#### S5 coordinator actual-change review and delegated closure

Reviewed pushed executor `41aa0bc24bac4de61c6a5dd32b4fefb78bde95bb`
against the admitted 71-path universe and necessary-subset policy. Its 35
changed paths comprise 27 mirror files, one test, two EXEs and five governance/
ledger files. The actual diff preserves the original selected renderer,
T88 marker/geometry changes, fixed text destination stride and snapshot cache
reset. Removed MONITOR/X86GFX/NTVDM routes are not the application's debugger
or KVM Window paths. Their dependent original endpoint declarations are removed;
BIGWIN painters and shared header definitions remain. No second renderer or
standalone-only behavior branch is added.

The actual new test enters the existing video BIOS dispatcher, compares every
RGB byte independently against its supplied palette, changes the palette before
restore, and checks memory boundaries. All final phase exit codes are zero:
148/148 per width, including ten desktop cases. Package hashes match the
executor record; origin/main equals the reviewed commit. The independent
fixed-XP range check, local snapshot-body preservation and non-mirror whitespace/
documentation gates pass. Source-format exceptions remain explicit.
No new blocking finding is raised. S5 is implementation-closed under delegated
async acceptance, not a fresh manual owner acceptance. S6 is admitted next;
T92 remains open until its separate completion audit and owner confirmation.

### S6 — XP baseline gate and OpenNT lineage freeze

Make the XP manifest and XP-to-local destination ledger the routine mirror
gate. Freeze OpenNT-to-XP and OpenNT-to-local reports as migration lineage
evidence. A new future XP reference revision requires a separately admitted
audit; it cannot silently redefine the mirror.

S6 starts from the reviewed S5 executor `41aa0bc2`. Its finite universe is
all 498 retained mirror paths and the existing 499 excluded XP paths, with
S3/S4/S5 selected ranges taking precedence over whole-file comparison. The
manifest must distinguish pinned upstream identity, selected-subset identity
and current local identity; a local fix may not disappear into an exclusion.
The routine gate must run entirely on checked-in metadata/source, without an
external repository dependency. Independent migration reconstruction may read
the fixed external Git objects, but is not a build transformation or runtime.

Expected changes are metadata, one bounded verification path and its negative
test, root test registration and current architecture/provenance documentation;
approximately 500 manifest rows, 100-180 verification/test lines and 100-200
documentation lines. No production C/H changes or new machine feature are
planned. Audit the existing manifest/gate paths first to reuse their mechanisms.
Preserve OpenNT reports as historical lineage and correct obsolete active
whole-file/baseline language. Requalify both package widths and all registered
tests, then perform the separate T-level requirement audit. Stop before T closure
for the owner; do not silently promote sound capability or the three TODOs.

#### S6 initial audit and gate checkpoint

No existing whole-mirror routine gate was found. The shared test-manifest
verifier requires LF-only text and belongs to reusable test packages, so it is
not modified to accommodate this CRLF mirror. A root-owned CMake check reuses
the existing CTest check registration conventions instead. The new
[current manifest](../etc/evidence/softpc/xp-sp1-current-manifest.tsv) records
all 498 paths, pinned raw XP hashes, selected-subset hashes, local hashes,
text/binary treatment and patch/selection disposition. Its 127 selected bodies
reuse S3-S5 range records; the remaining bodies compare against their whole XP
counterparts. ROMs use exact binary bytes. Text identity normalizes CRLF to LF;
this does not reformat working files or erase other whitespace differences.

The offline reconstruction reads all fixed XP objects in one read-only Git
batch; no external path appears in the routine gate. The positive 498-file
check and small selftest pass. Negative cases reject changed/missing/extra
files, duplicate/escaping paths, inconsistent local-patch identity and an
unapproved XP revision. Binary identity and text newline handling are exercised
separately. An initial fixture failure was CMake's native Windows newline
conversion of an explicit CRLF string, corrected in the test; no production
failure is inferred. Registration adds two checks; full requalification and
the overall migration/lineage audit remain pending. No S6 production C/H edit
has been made.

The complete fixed-object census now independently confirms 498 retained plus
499 excluded paths equals all 997 XP paths, with no duplicates or omissions.
Of the current retained bodies, 375 match their selected XP counterpart and
123 carry explicit local patches; normalized subset-to-local totals are
+26,267/-23,562. These include historical patches and are not the migration
cost. Against the frozen T91 source endpoint `087e512f`, T92 changes 76 mirror
files by +11,244/-18,625 (net -7,381), one Compat file by +2/-2 (net zero), and
four owner-local tests by +545/-4 (net +541). S6 adds verification metadata and
root checks separately; it does not rewrite those production bodies.

Current Architecture/Coding descriptions now identify the fixed necessary XP
subset as the routine baseline. The old upgrade ledger and OpenNT divergence
report are explicitly historical rather than contradictory live instructions.
M8's historical roadmap wording and old task reports remain unchanged facts.
The `.gitattributes` change is comment-only; the mirror's checkout policy and
the user's INI policy are unchanged. Full S6 qualification now passes all
150 registered tests per width (the prior 148 plus two mirror checks), including
all ten reserved desktop cases. Both Release package builds succeed using the
existing Ninja intermediates. See the [completion audit](../history/M9-T92-completion-audit.md)
for exact phase times and artifact hashes. Executor `48caa854` is pushed;
the separate [coordinator actual-change review](../history/M9-T92-S6-xp-baseline-qualification.md)
passes. S6 is implementation-closed under delegated async acceptance. The
T-level requirement audit is complete; T closure still waits for the owner.

## S1 File-Scope And Diff Accounting Checkpoint

The current audit now assigns all 86 XP-changed retained paths to batches:
S3 owns 37, S4 owns 18, S5 owns 31 (the audit's original S2/S3/S4 labels). The report's path table is the precise file
list, not a request to rewrite every listed file. The 74 local-only paths have
unchanged XP/OpenNT bases: preserve their accepted patches in place, with the
explicit exception of S3's two CMOS comparison corrections. The 338 identical
paths need no content edit. S6 changes reference accounting, not those bodies.
The linked 499-path unretained ledger accounts for every remaining XP path;
only the seven Sound Blaster files are new device-model import candidates.

Measured LF-normalized source-reference changes (XP minus OpenNT), grouped by
that exact 86-path table, are:

| Batch | Changed retained paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| S3 | 37 | 8,838 | 1,712 | +7,126 |
| S4 | 18 | 8,475 | 5,890 | +2,585 |
| S5 | 31 | 12,945 | 3,748 | +9,197 |

These are reference-diff measurements, not final implementation estimates:
they include guarded PC-98/regional/NT host text, and do not preserve local
patches automatically. Blind current-to-XP replacement would respectively be
+8,844/-2,267, +8,560/-7,249 and +13,163/-4,378 and would remove required
local behavior. That replacement is expressly rejected. The implementation
brief must estimate the actual accepted patch before execution, and report
actual additions/deletions/net against both the preceding local commit and XP.
Do not present the 6,097-line SB source volume as an estimate of working audio.

The precise known extra corrections beyond an XP body are intentionally small:
S2 corrects the disk CX:DX split; S3 preserves that correction instead of XP narrowing, synchronizes one descriptor-helper declaration
and two Compat lines, and transfers two CMOS comparisons; S4 rejects ROM CS and
mouse BX truncation, preserves the keycode bounds correction and the selected
int-width idle thresholds. The proposed reserved-key reverse-table patch was
withdrawn after checking the key classification. These are in addition to retaining existing local patches,
not replacements for them. Sound-card attachment remains the larger independent
engineering batch; absent OPL/MIDI backends and unresolved source admission must
be resolved in its implementation brief, not hidden behind stubs.

S1 is a source audit and upgrade recommendation, not acceptance of the later
runtime implementation. Its complete path universe is recorded in the linked
retained/unretained ledgers. The final decision is to proceed by subsystem,
preserve local fixes, carry explicit XP bug corrections, and stop before any
source copy lacking admission or any sound implementation lacking the above
backend decision. Audit completion does not waive either stop condition.

## S2 implementation and qualification (2026-10-15)

The three production repairs use the existing device paths, +7/-7 net zero:
four DMA address-reload lines, two BIOS capacity-split lines and one V7 enable
comparison. ROM production remains unchanged. No shared package, snapshot
format, configuration, media or new device implementation is changed.

The owner-local fixture programs real DMA ports and calls dma_request on both
controllers, with two-round auto-init and non-auto terminal-count checks.
Disk cases dispatch through BIOS[BIOS_DISK_IO], using guest-owned geometry
tables and independent expected CX:DX values, plus absent-drive AH/CF checks.
The V7 fixture writes the actual A5 port and checks drawing, clearing, repeated
enable and unrelated-bit changes. A generated checksummed option ROM records
CS through the real initialization/return path; host firmware installation
does not pretend that guest bus writes can modify read-only ROM.

The x64 old-code DMA, disk and V7 cases fail their intended assertions; all
four focused tests pass after repair. Initial fixture compile, assertions,
media setup and ROM-installation mistakes were corrected and are not described
as production defects. Full dual-width qualification subsequently passed.
The first x64 run passed all 90 shared tests and 49/51 remaining tests. It
exposed a forbidden string.h/direct-memory use in the new fixture (corrected
to existing Lib Types functions) and the old machine_smoke low-byte clock
assertion. Neither failure is silently treated as a pass.

Final full-run result: each width passes 90 shared background tests, 51 other
background unit/integration/gate tests and five reserved native desktop tests,
146/146 per width. x64 phase wall times are 46.70s / 72.55s / 13.77s;
x86 is 48.31s / 69.26s / 13.22s. This includes snapshot and package integration.
Desktop cases are package_smoke, compact_console, kvm_window_retirement,
kvm_window_modal and console_broker_display. No Computer Use was used.
The clock test passed unchanged in the final full runs; its fragile oracle
remains explicit deferred test debt, not a claim that retries fixed it.

Source review also found a separate decrement-copy concern in at_dma.c;
its disposition is in TODO. S2 uses actual decrement VERIFY transfers for
address/count reload and increment READ for data movement on both controllers;
it does not claim reverse data-copy correctness. The production estimate is
corrected to +7/-7 before any explanatory comments (four DMA, two disk, one V7
line replacements), rather than the earlier approximate +5/-5.

Changed-path accounting against f55428f8: production three C files +7/-7,
net zero; two test C files +217/-1, net +216; root CMake registration +13/-0.
Total production/test/build +237/-8, net +229. The new fixture is 191 lines;
no generated firmware or external disk is committed. The OpenNT/XP whole-file
counts (including prior local fixes) are in the pristine-divergence ledger.

The finite similar-issue sweep covers the single auto_init terminal-count
branch in at_dma.c, masked hexadecimal equality expressions in base/system
and base/video, and UCHAR casts on setCS/setCX/setBX in base. Commands use
`rg` for these patterns and read the complete matched branches. The repaired
reload retains both count bytes and both address bytes; the masked comparison
pattern has no remaining matches; the register-cast pattern has no local
matches. The known CMOS comparisons remain assigned to S3, and XP-only ROM
CS/mouse BX truncation remains an explicit S4 rejection. DMA reverse-copy
and the pre-existing clock-test oracle are recorded in TODO, not silently
fixed here. These bounded checks are not a whole-core correctness claim.

The x86 Ninja graph reported recovery from a prematurely ended cache log and
rebuilt 584 edges; no cache deletion or manual duplicate build was requested.
The final targeted x86 build reported the same cache recovery and rebuilt
304 edges. This is an execution-cost issue, not a source change or failed test.
The x64 graph reused its existing intermediates. Both package EXEs were
refreshed; final targeted rechecks cover the fixture's exact CPU CF prototypes
and absent-drive register assertions after the full build.
Both final targeted runs pass 7/7 (four device cases and three static gates),
x64 6.40s and x86 8.74s. Full-suite evidence is retained separately from these
post-review focused results. Documentation governance and staged whitespace
checks pass. Final shared source/test roots and user INI/media have no diff.

SHA-256 of the refreshed package:

- softpc32.exe: `3bb7a8dfa2e0f1247178c7df108b5e0861c99d17831661f3723101258507ce61`
- softpc64.exe: `3601a64e0855bf324565d9fa9933b85471c523a99409611d2452faf89b456ef6`

These repairs have source and fixture evidence; they are not identified as
the cause of any earlier user-observed Windows/DOS symptom. No fresh manual
guest installation or hardware-output claim is made.

### S2 coordinator actual-change review

Reviewed pushed executor e884ff9bbf64e0e5dd5a98902c07a8afc4f277ba against
the original four repair requirements, active packet and applicable rules.
The actual 14 changed paths contain exactly three production repairs, two
test C files, root test registration, two package EXEs and six documentation
paths. Production/test/build accounting matches +237/-8, net +229; the other
text changes are governance/evidence, not hidden runtime expansion.

The DMA change replaces only the duplicated reload, the BIOS change leaves
geometry and mounting untouched, and the V7 change leaves its existing
draw/clear/update calls intact. ROM production is unchanged. Tests exercise
original entry points and the fixed marker/return contracts, with generated
inputs confined to isolated build working directories. CF prototypes match
the original cpu4gen declarations. Decrement VERIFY does not certify the
separately recorded reverse-copy implementation. No shared package or
user-owned configuration/media changed.

The full-run and final focused exit records are zero. Both widths passed
146/146 including native desktop cases; final rechecks pass 7/7 each.
Initial fixture mistakes, the clock assertion failure, cache-recovery rebuilds
and retained TODO limitations remain disclosed. This accepts the S2 delivery
for owner verification, not S closure, whole-core qualification or permission
to import unresolved XP payloads. T92 remains open.

## S1 Executor Verification

On 2026-10-15 the final immutable-object recheck passed: the retained and
unretained TSV sets are unique, disjoint and exhaust all 997 XP paths. All 498
local mirror bodies still match admitted commit 087e512f; their raw hashes and
the pinned XP/OpenNT object hashes match the audit inventory. The 86-path
semantic table has no missing or duplicated inventory member. The EGA cursor
comparison enumeration checked 33,792 legal-field combinations with no branch
difference; it is not a device runtime test. Documentation governance and
git diff --check pass. No production/test source, EXE, configuration or guest
asset changed, so no rebuild, deployment or runtime test result is claimed.

The executor delivery is documentation/research metadata only. Owner review
of the recommendation is required before admitting a source-changing batch;
neither S1 delivery nor the metadata commit closes T92.

## S1 Coordinator Actual-Change Review

Reviewed pushed executor commit `2616deb80ad710bc27c0fd1ed8cd270280b1d489`
against the original audit objective and CURRENT packet. Its five changed
paths are the report, two metadata ledgers, this proposal and CURRENT:
+1,853/-13 documentation/metadata lines, net +1,840; production/test/asset
changes are zero. The ledgers contain 498 and 499 unique members, not source
payloads. Their union and frozen-object identities were rechecked, and the
86 changed retained paths match the semantic table exactly.

The review accepts this as a source-audit delivery, not permission to import
the XP tree verbatim. At the time of this review IBM PC sound remained required within T92 and explicitly not ready
for implementation until the missing output solution and rights decisions;
CD-ROM declarations are not misreported as an implementation. The local CPU,
C-VID, input, media, frame and snapshot safeguards are retained in the plan.
Known XP regressions have named corrections and test obligations. Scope and
runtime qualification limits are explicit. No whole-core correctness or fresh
EXE qualification is inferred from documentation gates or source comparison.

S1 is ready for owner review; T92 remains open, with no later S admitted.

## Verification

Each source-changing S provides source-selection proof that excluded paths and
branches are not selected; focused device regressions for the changed
subsystem; clean x64 and x86 Release builds; `ctest --preset test-x64` and
`ctest --preset test-x86`; applicable source/product boundary checks; and
separate explicitly reserved desktop results if presentation behavior changes.
Preserve user-owned `assets/binary/softpc.ini` and supplied media.

S6 additionally proves that every retained XP/local difference is ledgered,
that no excluded XP-only route is compiled, and that the routine comparison is
XP SP1 -> local. It does not claim that OpenNT comparison material has become
a runtime, build or package dependency.

## Stop Conditions

Stop for owner direction if an intended XP adoption needs a new product host,
second device implementation, unreviewed source-rights conclusion, new binary
or firmware asset, or a guest-visible semantic choice not covered by the
admitted S. The necessary-subset policy applies to every subsequent batch;
whole-file similarity never overrides exclusion decisions or permits a silent
return to OpenNT as the selected functional baseline.
