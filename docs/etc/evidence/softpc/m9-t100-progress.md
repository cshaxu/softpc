# M9 T100 original-first mirror audit

CURRENT owns active status; the [approved method](../../../proposals/m9-xp-mirror-original-first-audit.md)
preserves the original request and execution contract.

## S1 admission and initial evidence

Owner confirms the method on 2026-11-12, with one correction: existing #error
exclusions require no new bookkeeping. They receive no IDs, counts or approval
records. The existing exclusion policy and correct diagnostic mapping remain.

The [file inventory](m9-t100-files.tsv) freezes all 472
files in literal `cmd /d /c "dir /a /b /s"` output order from the mirror root,
filtering directory entries without sorting. Starting commit is
ac9d8dc85a6d9b0cc66d0a6649f60e8903406f18. Initial SHA-256 values are byte hashes.
XP source reads use fixed commit a2f6d7c93aa4f11efc51147566d5961c59a8ef89,
under Source/XPSP1/NT/base/mvdm/softpc.new. No OpenNT source is consulted.

## Sequential file results

Owner correction during reset.c review: the product always has a floppy drive;
an absent floppy image means empty media, not absent hardware. Earlier commentary
claiming a supported HDD-only/no-drive topology was incorrect and is withdrawn.
`gfi_image.c` assigns a drive type even for a null media path, preserves that type
across media replacement, and snapshot restoration accepts A present/B absent.
`machine_smoke.c` expects equipment 0xc821 for both floppy and HDD boot inputs.
The original non-NTVDM branch's diskette-present behavior therefore has no proven
defect in this supported topology. The two standalone condition extensions have been restored and are not
submitted as necessary retained differences. The first candidate finished before
the final source edits; its results do not qualify that subsequent restoration.

| Position | File | Initial analysis | Counter-review and disposition |
| --- | --- | --- | --- |
| 1 | base/bios/bootstra.c | T100-R0001: one relocated SCCSID comment plus its added line directive; no remaining in-scope executable change. | XP has neither addition outside SCCSID. Remove both nonsemantic additions; the following original condition regains line 45, and existing subsequent line mapping remains. Restoration verified in P1. |
| 2 | base/bios/build_id.c | Whole normalized content equals pinned XP. | No modification; do not claim byte identity from normalized comparison. |
| 3 | base/bios/cmos_bis.c | No in-scope difference. | Retained source matches pinned XP outside skipped scope; no change. |
| 4 | base/bios/equipmnt.c | No in-scope difference. | Retained source matches pinned XP outside skipped scope; no change. |
| 5 | base/bios/mem_size.c | T100-R0002: relocated SCCSID comment, absent at that position in XP. | Remove nonsemantic addition; following SCCSID starts at original line 16; subsequent line mapping unchanged. Restoration verified in P1. |

Current sequential audit: 7/472 files examined, 465 not completed. File 8,
base/bios/slave_bs.c, is under review. File 7 rom_basc.c has no in-scope diff
and is required by the direct BIOS_BASIC production dispatch. The first six
files are the current delivery batch. No whole-file deletion is justified by
these production bindings. Existing exclusions have no new ledger entries.

## File 6 review (supersedes the initial progress snapshot above)

`base/bios/reset.c` is a required production BIOS reset implementation, compiled
into softpc-machine-devices and directly assigned to BIOS_RESET. The product
test `bop_smoke.c` verifies that exact function pointer; Compat has no replacement
reset algorithm. It cannot be deleted under the whole-file cleanup supplement.

- R0003: two pointer-width intermediate casts at XP lines 498 and 1052.
  `config_inquire(C_GFX_ADAPTER)` returns the small fixed VGA enum encoded as
  a pointer, not an address. The target LONG/ULONG is already 32-bit in Windows;
  no supported value needs high pointer bits. Restore the original casts.
  Counter-review: x64 emits two pointer-to-integer-size warnings, but the real
  encoded value fits; warnings alone do not prove behavior requires a patch.
- R0004: local indentation, blank placement, explanatory comments and erroneous
  redundant line controls. Restore original layout, remove comments explaining
  already recorded omissions. A matching-line comparison with fixed XP reports
  zero incorrect logical lines for the unchanged non-directive source after
  cleanup. This diagnostic comparison does not audit excluded bodies.
- R0005: two expanded NTVDM conditions and the displaced non-NTVDM floppy arm.
  The original ppi_get_switches selects PPI_ONE_DRIVE; the GFI adapter always
  installs A even with no medium. Restore original condition/body, including
  the previously suppressed now-selected else arm. No new configuration or
  compatibility route is justified. This corrects the withdrawn no-drive claim.
- R0006: XP lines 910-911 call mouse_driver_termination/initialisation.
  Their original definitions allocate/free the NIDDB MM_INSTANCE_DATA for the
  optional Insignia DOS mouse driver. The current product has no BIOS_MOUSE
  install/dispatch slots, uses the original InPort hardware mouse_init plus
  the guest's driver, and explicitly removed that optional state from SCP3.
  There is no duplicate Compat DOS mouse implementation to replace. Restoring
  only calls fails symbol resolution; restoring the complete original subsystem
  would reintroduce the specifically retired driver/state without a guest
  consumer. This is a product-subset boundary, not an original algorithm bug.
  Proposed minimum is the existing two-line omission; owner decision pending.
  Source/callgraph evidence only: no new standalone original-driver runtime
  comparison is claimed. The owner may instead direct full subsystem restoration.

Before this file's cleanup, the comparison outside existing exclusions and
diagnostic line controls was +24/-21; the intermediate candidate is +2/-4.
The final floppy restoration leaves +0/-2: only the two DOS mouse calls.
Final-source verification passed as recorded below.
First-candidate cached builds passed both architectures; each passed 188
background plus 8 native cases (392/392 total). These results do not qualify
the subsequent floppy-condition restoration. Scratch is now build/t100-s1, owned by this active packet.

## File 8 initial source proof

The current slave_bs.c has residual +2/-1: a relocated SCCSID comment and
ICA1_PORT_1 -> ICA1_PORT_0 in D11_int, with an inline local explanation.
BIOS dispatch slots 72h/73h/74h/76h/77h point directly to D11_int. Original
SWPIC_outb routes odd ports to ica_imr = value; even port 20h to non-specific
ica_eoi. With IRQ11 in service and mask f7h, D11 first masks to ffh. Restoring
XP's second odd-port write overwrites that mask with 20h and never clears
slave ISR bit 3. That is concrete source proof in the actual supported PIC,
not a claim of a newly reproduced runtime failure. Existing device_contract_irq
covers no IRQ, slave IRQ11, master IRQ5 and spurious cascade with explicit
ISR/mask/BDA assertions. No change to this file while the current batch runs.

## S1 P1 final-source delivery accounting

Changed production files: bootstra.c, mem_size.c, reset.c. Against the admitted
SoftPC baseline, production +28/-33 (net -5), tests +0/-0, no Compat change.
Against fixed XP, the original-relative added/deleted delta is respectively
bootstra.c -2/0, mem_size.c -1/0, reset.c -28/-26: total mirror diff reduction
57 lines. This measures raw line-diff change with LF-normalized comparison and
SequenceMatcher(autojunk=False); it creates no excluded-body inventory.
The separate in-scope display comparator elides existing exclusion bodies and
line controls: reset.c falls from +24/-21 to +0/-2. These two metrics differ
because restoration also repairs line-control/layout and reinstates the selected
original floppy arm. Do not conflate either metric with net source-line change.

Final-source cached full builds publish:

- x86: 3,519,378 bytes; SHA-256
  16d61d5da737e129b7af0f7d4b0c68e708767d7fe07b276f3f1df4a9b3799b8e.
- x64: 3,117,988 bytes; SHA-256
  2169514740492f92c222386ddb0fcc25b84c5ee618debfb6c896caf3cd205e83.

Both embedded compiler dates are Nov 12 2026. Final build logs have zero errors;
91 x86 and 93 x64 warnings are from original mirror source/headers. The additional
two x64 diagnostics are the restored original small-enum pointer casts; their
bounded value was source-reviewed. No warning suppression was added. Unchanged
shared packages were not claimed freshly warning-free from cached compilation.
All three changed files retain CRLF and have zero matching-source logical-line
mismatches versus fixed XP. No guest/configuration/snapshot or public package
source changed. Final test completion and commit identity follow below.

Final qualification: x86 188/188 background + 8/8 native, x64 188/188
background + 8/8 native: 392/392, zero failures/skips. This includes actual
machine BDA floppy/HDD boot assertions, reset/BOP, FDC, snapshot, graphics,
package, Console, audio and shared component gates. No new manual DOS/Win3.1/
Win95 interactive acceptance is claimed. Documentation, XP manifest and
`git diff --check` pass after final documentation updates.

Executor actual-diff review: only the three listed mirror paths, their hashes,
ordered audit records, active packet and two EXEs change. No product behavior
special case, duplicate Compat path, test-only API or guest edit was introduced.
P1 contains five restored audit points (R0001-R0005); R0006 is an unresolved
owner decision, not approved retention. File 7 is read-only complete; file 8
is the next incomplete file. Overall 7/472 audited, 465 remaining, zero whole
files deleted. P1 is a batch delivery, not S1 or T100 closure. The two runners
completed; their disposable logs and probes are removed after promoting results.
