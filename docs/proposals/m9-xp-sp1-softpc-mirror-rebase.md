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

## Proposed Serial Work

### S1 — Current XP migration ledger and source admission

Freeze the current 498-path mirror universe, the XP path universe, and exact
raw/normalized hashes. Recompute the three-way relation using the current T83
source root. For every retained mirror file and XP-only candidate, record one
destination outcome, selected-profile reachability, required local patch,
owner, and test receiver. Record external-source admission facts before the
first payload enters the tree. No runtime source changes occur in S1.

### S2 — XP-first mechanical and controller/BIOS basis

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

### S3 — XP-first input, reset, IRQ, ROM and timing basis

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

### S4 — XP-first C-VID and video basis

Migrate selected VGA/EGA/CGA/V7 source and declarations to the XP basis,
excluding product presentation routes while retaining the standalone copied
frame/surface path through the appropriate local owner. Verify CGA, EGA, VGA
and V7 mode, cursor, port and frame behavior on both host widths.

### S5 — XP AT ISA Sound Blaster source and standalone attachment

S1 has established that XP's FM path depends on a native FM device and
NtVdmControl, not a software OPL-to-PCM generator. The final S5 design must
explicitly account for that missing standalone capability rather than promise
a complete sound card through file copying alone. AT DMA already calls the XP
sound position hook in its normal path; migrate those changes with their
device/host owner, not as an independent low-risk representation change.

Evaluate XP-only Sound Blaster paths as an IBM PC/AT capability batch, not as
PC-98 work and not as an unrelated greenfield device. Bring accepted
XP-derived device-model files into the XP mirror, while explicitly excluding
VDD/`NtVdmControl`, NTVDM DMA/port hooks and other product-host material.
Attach the resulting device through repository-owned standalone boundaries for
ISA port decode, DMA, IRQ, DSP, FM/AdLib, mixer and audio output. The device
model remains XP-derived; only its host attachment is standalone-owned.
Account explicitly for the SbTerminate and SbCloseDevices obligations found
in XP's unselected terminat.c and nt_reset.c, plus initialization after reset
in nt_msscs.c and closure before host blocking in nt_event.c: attach these
obligations to the existing machine lifecycle, distinguishing device state
from host resource lifetime. Attach device teardown and host
resource closure to the existing lifecycle, without importing a second NTVDM
main/reset/worker path. Verify reset, stop, destruction and snapshot alongside
first playback; port registration alone is not complete sound-card adoption.

The MIDI/mixer audit adds explicit prerequisites: UART-output-only MPU scope,
a real MIDI output/synthesis destination separate from PCM Audio, full-message
tail delivery, native-width host pointers, bounded buffer ownership, zero-volume
correctness and independent master/channel gain. Do not preserve XP's 5-second
INQUEUE polling loop or reuse its global host-volume controls as device mixing.
The linked audit records the concrete source defects. S5 requires a bounded
device/host design before implementation; importing seven files alone cannot
satisfy the IBM PC sound capability requirement.

#### Concrete standalone design and admission boundary

Keep one guest-visible device owner in the XP-derived files. Port dispatch,
DSP command parsing, DMA transfer count/address, IRQ assertion/acknowledgement,
MPU parser and mixer registers are executor-owned. Do not put a second copy of
those fields or a command interpreter in Compat. Replace only NT host seams:
NtVdmControl/VDD registration, WinMM handles/buffers, native volume, worker
completion and wall-clock reads. Preserve original source paths and mark each
host seam in the XP divergence ledger; no whole-file standalone-only fork.

Use existing device timing delivery and quick-event infrastructure, not host
playback position as a second guest DMA clock. Audio callbacks cannot read or
advance guest RAM/DMA/IRQ. PCM delivery carries copied samples outwards; a
host completion only reports sink capacity/failure, never raises a guest IRQ.
S5 must prove this timing contract with DMA count/IRQ observations, including
host output stalled, pause/resume and restore. Actual chunk size and pacing
must be selected by that proof, not a new fixed Sleep or nominal CPU frequency.

The existing Lib stream accepts copied interleaved i16 PCM, up to 512 frames
per enqueue, at 22050/44100/48000 Hz, with authoritative accepted-frame count.
Unsigned DSP samples, ADPCM state, device sample rate and master/channel gains
are device semantics; conversion/mixing occurs before this boundary. The
private output adapter handles partial enqueue, wait cancellation and explicit
tail flush. Do not reinterpret queue acceptance as audible completion or use
Lib readiness as the guest clock. Do not change the accepted PC-speaker onset
handoff merely to attach a second source; any shared output refactoring needs
first/second playback and concurrent-speaker regression.

Machine initialization/reset/stop/destruction invoke the same device entry and
host-resource lifecycle. Snapshot captures fixed-width device/parser/sample
phase and pending-event state at the existing executor safe point; pointer,
WinMM handle, native queue and already-heard samples are not archive data.
Restore reconstructs host output and cancels stale-generation PCM. Existing
snapshot safe-point, single executor and cross-width format remain authorities.
No version compatibility is implied for a format extended with a new device.

Known file boundary: seven XP candidate files (6,097 reference lines), existing
at_dma.c/ios.h port integration, Compat audio/host seams, Machine lifecycle,
device snapshot records/codec and owner-local tests. Lib/Emulator/X86 product
APIs do not need SB-specific fields. A neutral MIDI host backend, if selected,
is a separate public-capability decision, not permission to put WinMM calls in
guest protocol code. A software OPL engine likewise needs its own reviewed
origin or implementation design; XP's native FM route supplies neither.

**S1's executable verdict is that S5 is not yet source-import-ready.** Before
admitting its implementation, the owner must select and admit the OPL/MIDI
output solution and resolve source rights. S2-S4 can be reviewed independently
after their source-admission gate. This is not deletion or deferral of sound
from T92: S5 stays required for the full capability objective, and T92 cannot
claim complete sound or close that obligation after a DSP-only demonstration.
No defensible final source-line estimate for a missing synthesis backend is
available from this XP tree. Report the known seven-file reference size and
the precise interface list above; do not invent a small implementation budget.

The S5 proof must include reset ACK/version, single and auto-init DMA, count
readback and IRQ acknowledgement, unsigned PCM/implemented ADPCM samples,
zero/master/channel gain and register readback, a one-message MIDI stream and
SysEx/realtime boundaries, FM timer/register and actual PCM output, first and
repeated playback, stop/cancel/teardown, and cross-width restore of an active
transfer. Unsupported XP operations remain explicitly unsupported; no SB16 or
recording claim is made. These tests observe ports/state/PCM, not just UI text
or a human hearing one tone.

### S6 — XP baseline gate and OpenNT lineage freeze

Make the XP manifest and XP-to-local destination ledger the routine mirror
gate. Freeze OpenNT-to-XP and OpenNT-to-local reports as migration lineage
evidence. A new future XP reference revision requires a separately admitted
audit; it cannot silently redefine the mirror.

## S1 File-Scope And Diff Accounting Checkpoint

The current audit now assigns all 86 XP-changed retained paths to batches:
S2 owns 37, S3 owns 18, S4 owns 31. The report's path table is the precise file
list, not a request to rewrite every listed file. The 74 local-only paths have
unchanged XP/OpenNT bases: preserve their accepted patches in place, with the
explicit exception of S2's two CMOS comparison corrections. The 338 identical
paths need no content edit. S6 changes reference accounting, not those bodies.
The linked 499-path unretained ledger accounts for every remaining XP path;
only the seven Sound Blaster files are new device-model import candidates.

Measured LF-normalized source-reference changes (XP minus OpenNT), grouped by
that exact 86-path table, are:

| Batch | Changed retained paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| S2 | 37 | 8,838 | 1,712 | +7,126 |
| S3 | 18 | 8,475 | 5,890 | +2,585 |
| S4 | 31 | 12,945 | 3,748 | +9,197 |

These are reference-diff measurements, not final implementation estimates:
they include guarded PC-98/regional/NT host text, and do not preserve local
patches automatically. Blind current-to-XP replacement would respectively be
+8,844/-2,267, +8,560/-7,249 and +13,163/-4,378 and would remove required
local behavior. That replacement is expressly rejected. The implementation
brief must estimate the actual accepted patch before execution, and report
actual additions/deletions/net against both the preceding local commit and XP.
Do not present the 6,097-line SB source volume as an estimate of working audio.

The precise known extra corrections beyond an XP body are intentionally small:
S2 rejects the disk CX truncation, synchronizes one descriptor-helper declaration
and two Compat lines, and transfers two CMOS comparisons; S3 rejects ROM CS and
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
