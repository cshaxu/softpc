# M9: XP SP1 SoftPC Mirror Baseline Upgrade

## Admission checkpoint (2026-10-15)

Owner admits T92 after accepting and closing T91. Only S1, the source audit
and migration design, is active; CURRENT owns its executable packet. The
following import batches remain planned, not silently executed by admission.
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

Upgrade the repository-owned selected SoftPC mirror from its OpenNT/NT4
baseline to the frozen XP SP1 SoftPC baseline:

```text
XP SP1: winxpscodes/Source/XPSP1/NT/base/mvdm/softpc.new @ a2f6d7c93
Target: src/app-softpc/softpc.new
OpenNT: opennt-src-2/nt/private/mvdm/softpc.new @ 5e4619ab6
```

At closure, the ordinary mirror review and divergence gate is **XP SP1 ->
`src/app-softpc/softpc.new`**. OpenNT remains a frozen lineage and migration
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
four outcomes. The choice is based on the selected IBM PC/AT projection and
the smallest maintainable XP-to-local diff, not on its A/B text label.

| Outcome | Meaning | Required proof |
| --- | --- | --- |
| **XP file** | Adopt the XP SP1 file unchanged because its selected profile semantics are wanted and it has no local adaptation to retain. | Selected-source proof, dual-width build, focused subsystem regression. |
| **XP file + local patch** | Use the XP file as the mirror basis and reapply only necessary standalone/x64/port-ABI changes as explicit, reviewable XP-to-local diffs. | Exact local patch rationale, ownership proof, dual-width and focused behavior proof. |
| **XP hunk + current remainder** | Retain the current file only when profile-pruning or structural separation makes a whole XP file less simple; take every accepted XP hunk explicitly. | Hunk map showing excluded branches and no lost selected-machine behavior. |
| **Do not introduce** | Omit a path or XP change that is solely foreign hardware or product-host support and has no selected standalone value; classify regional IBM PC changes separately. | Explicit exclusion record and build-source proof. |

The default is **XP file** or **XP file + local patch** when that produces the
smaller, more legible final XP-to-local difference. “Do not introduce” applies
to a file that is useless to the standalone selected profile; it is not a
reason to retain OpenNT implementations of useful IBM PC/AT hardware.

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

Adopt the approved XP basis for low-risk representation, CMOS, printer,
GFI/FDC, disk/floppy and BIOS candidates. Prefer whole XP files plus explicit
standalone patches where that is simpler than retaining an OpenNT body. Keep
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

Perform the nontrivial three-way migrations for reset, keyboard/mouse, PIC,
ROM and timer by subsystem. Preserve required standalone topology in explicit
local patches. `nt_keycd.c` and `keyba.c` include Pause/NumLock/Ctrl-Alt-Del and
scan-set table proof; timer
includes zero-delay quick-event protection; controller work includes IRQ and
boot/media proof.
The seven selected keyboard tables have 60 changed elements. Preserve the
four new key identities (14/56/94/107), verify all 256 translation inputs,
and repair XP's omitted set-3 reverse[0x5d]=14 entry in the existing table.
Test per-key type commands and reset as well as make/break; do not silently
import a new key that cannot receive its own attributes.

### S5 — XP-first C-VID and video basis

Migrate selected VGA/EGA/CGA/V7 source and declarations to the XP basis,
excluding product presentation routes while retaining the standalone copied
frame/surface path through the appropriate local owner. Verify CGA, EGA, VGA
and V7 mode, cursor, port and frame behavior on both host widths.

### S6 — XP baseline gate and OpenNT lineage freeze

Make the XP manifest and XP-to-local destination ledger the routine mirror
gate. Freeze OpenNT-to-XP and OpenNT-to-local reports as migration lineage
evidence. A new future XP reference revision requires a separately admitted
audit; it cannot silently redefine the mirror.

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
mouse BX truncation, preserves the keycode bounds correction and fills one set-3
reverse-table slot. These are in addition to retaining existing local patches,
not replacements for them. Sound-card attachment remains the larger independent
engineering batch; absent OPL/MIDI backends and unresolved source admission must
be resolved in its implementation brief, not hidden behind stubs.

S1 is a source audit and upgrade recommendation, not acceptance of the later
runtime implementation. Its complete path universe is recorded in the linked
retained/unretained ledgers. The final decision is to proceed by subsystem,
preserve local fixes, carry explicit XP bug corrections, and stop before any
source copy lacking admission or any sound implementation lacking the above
backend decision. Audit completion does not waive either stop condition.

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
admitted S. A failed whole-file migration falls back to the explicit XP-hunk
plus-current-remainder outcome; it does not silently retain OpenNT as the
mirror basis.
