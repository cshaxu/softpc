# M9: XP SP1 SoftPC Mirror Baseline Upgrade

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
`NEC_98`, `JAPAN`, NTVDM/DOS/WOW/VDD/PIF/CSR host routes, and non-x86 machine
families are explicit exclusions. A false compile-time guard is never by
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
remains useful evidence about the old three-way comparison: in particular it
identifies normal-AT `keyba.c` ScanCode `0x45` handling as a real candidate,
and distinguishes representation cleanups from PC-98, Japan and NTVDM work.
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
| **Do not introduce** | Omit a path or XP change that is solely PC-98, Japan, non-x86 or NTVDM-product support and has no selected standalone value. | Explicit exclusion record and build-source proof. |

The default is **XP file** or **XP file + local patch** when that produces the
smaller, more legible final XP-to-local difference. “Do not introduce” applies
to a file that is useless to the standalone selected profile; it is not a
reason to retain OpenNT implementations of useful IBM PC/AT hardware.

## Known Semantic Starting Points

The refreshed ledger is authoritative; the following old-audit conclusions are
starting hypotheses only.

| Area | Current conclusion to verify in S1 |
| --- | --- |
| CMOS, printer, GFI/FDC value narrowing | Existing machine semantics with XP representation/typing cleanups; likely XP-file or XP-hunk cases, not new guest features. |
| Disk, floppy and BIOS media paths | IBM PC/AT hardware source is a candidate for XP-first adoption, while image/direct/readonly/overlay ownership remains a local standalone patch. |
| `keyba.c` | XP normal-AT handling distinguishes Pause from NumLock for ScanCode `0x45`; adopt XP as file basis if the local input patch can be reapplied cleanly, then prove Pause, NumLock and Ctrl-Alt-Del. |
| Reset, PIC, ROM, timer and C-VID/video | XP-first three-way migration candidates. Existing x64 ABI, fixed-ROM/media, quick-event, IRQ and copied-frame constraints must be explicit local patches and regression criteria, not reasons to keep OpenNT as baseline. |
| Original `nt_*` host endpoints | XP NTVDM host implementations are excluded as host implementations. A self-contained selected-machine algorithm may be moved only with an explicit Compat/Machine owner and proof. |
| PC-98, Japan, non-x86 | Explicitly excluded, whether they appear as a path, branch or declaration. |

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
dual-media operation as applicable.

### S3 — XP-first input, reset, IRQ, ROM and timing basis

Perform the nontrivial three-way migrations for reset, keyboard/mouse, PIC,
ROM and timer by subsystem. Preserve required standalone topology in explicit
local patches. `keyba.c` includes Pause/NumLock/Ctrl-Alt-Del proof; timer
includes zero-delay quick-event protection; controller work includes IRQ and
boot/media proof.

### S4 — XP-first C-VID and video basis

Migrate selected VGA/EGA/CGA/V7 source and declarations to the XP basis,
excluding product presentation routes while retaining the standalone copied
frame/surface path through the appropriate local owner. Verify CGA, EGA, VGA
and V7 mode, cursor, port and frame behavior on both host widths.

### S5 — XP AT ISA Sound Blaster source and standalone attachment

Evaluate XP-only Sound Blaster paths as an IBM PC/AT capability batch, not as
PC-98 work and not as an unrelated greenfield device. Bring accepted
XP-derived device-model files into the XP mirror, while explicitly excluding
VDD/`NtVdmControl`, NTVDM DMA/port hooks and other product-host material.
Attach the resulting device through repository-owned standalone boundaries for
ISA port decode, DMA, IRQ, DSP, FM/AdLib, mixer and audio output. The device
model remains XP-derived; only its host attachment is standalone-owned.

### S6 — XP baseline gate and OpenNT lineage freeze

Make the XP manifest and XP-to-local destination ledger the routine mirror
gate. Freeze OpenNT-to-XP and OpenNT-to-local reports as migration lineage
evidence. A new future XP reference revision requires a separately admitted
audit; it cannot silently redefine the mirror.

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
