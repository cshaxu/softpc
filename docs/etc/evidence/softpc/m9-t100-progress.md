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

## P2 ordered investigation (not delivered)

File 8, base/bios/slave_bs.c: complete source review; original-relative
in-scope diff +2/-1 becomes +1/-1 after removing the relocated SCCSID comment
(R0007, nonsemantic restoration, local and not yet built/delivered).
R0008 remains an owner decision: D11_int writes EOI 0x20 to ICA1_PORT_0
rather than XP ICA1_PORT_1. The actual BIOS dispatch installs D11_int in
slots 72/73/74/76/77. Original SWPIC_outb treats odd ports as IMR writes,
whereas even-port OCW2 command 0x20 invokes ica_eoi. Thus the original write
changes the mask to 0x20 and leaves the active slave ISR uncleared. This is
source proof, not a newly run negative probe. Existing device_contract_smoke
verify_unused_irq covers idle, slave IRQ11, master IRQ5 and spurious master2;
P1's final run passed that test. No current P2 test result is claimed.
Counter-review: this file is a real BIOS consumer, not duplicate Compat logic;
restoring the original port cannot be justified by absence of a GUI symptom.
The retained one-line correction still requires owner disposition.

File 9, base/bios/tape_io.c: investigation ongoing, not complete. R0009
restores the two unused original xms A20 declarations; R0011 restores the
original JAPAN conditional end comment. Both are local nonsemantic changes.
R0010 (descriptor limit pointer type) and R0012 (INT15/C0 segment) remain
investigations, not admitted retention decisions. The descriptor caller's
source_limit/target_limit are sys_addr and the current compatibility helper
uses original CPU40 read_descriptor_linear; inspect coherent old-helper ABI
before disposition. For C0, reset.c installs INT15 at F000:F859 and CCPU BOP
dispatch does not itself replace CS. Therefore the current comment claiming
the handler necessarily executes in the guest caller's segment is not proof.
Trace the ROM entry and execute the actual INT15 path before deciding.
No guest, user configuration or snapshot changed. No P2 build started yet.
Eight of 472 files source-audited; 464 remain, including file 9. Pending
owner decisions and local unverified restoration are not closed work.

### File 9 completed source review and focused proof

R0012 restored XP getCS() and removed the unsupported local explanation.
The fixed in-tree bios4.rom is loaded at F000:E000. INT15 enters F000:F859;
AH=C0 follows comparisons at F85A/F865/F86A/F871 to BOP bytes C4 C4 15 at
F87A, without a far transfer or change of CS. CCPU dispatch keeps that CS.
An added sequence in the existing machine_smoke INT15 boot fixture executes
INT15/C0 through this ROM and stores AX, ES, BX and flags in guest RAM.
Both x86 and x64 rebuilt machine_smoke and passed: ES=F000, BX=E6F5,
AH=0 and CF clear after entering with CF set. The original AH88 memory-size
assertion still passes. This is actual generated-test guest execution, not a
changed original guest binary or a direct cassette_io stub. No new manual
DOS/Windows acceptance is claimed.

R0010 is now evidence-qualified pending owner decision, not unknown:
XP tape_io.c:95-96 declares limit as word* (IU16*), but its original local
source_limit/target_limit and their passed addresses are sys_addr (IU32).
Original CPU40 read_descriptor_linear decodes a 20-bit limit, including
page granularity expansion to 32 bits. The Compat facade only delegates that
original decoder and copies its results. Fixed XP base contains no definition
of retrieve_descr_fields, only this declaration and two calls. Narrowing the
adapter to match the old prototype would either leave half of the caller's
32-bit object unwritten or truncate real descriptor limits; retaining the
adapter and reverting only the prototype creates incompatible pointer types.
The existing device_contract descriptor tests exercise limits through FFFFFFFF
and surrounding guards; P1 passed them. Counter-review distinguishes an ABI
mismatch from a claim of an observed customer fault. Minimal residual +1/-1
is the corrected sys_addr* declaration; no extra helper or state is proposed.

File 9 in-scope residual +6/-5 becomes +1/-1 (10 fewer XP-relative diff
lines). The original declarations and end comment were restored exactly;
existing excluded bodies were not reopened or entered into the ledger.
Nine of 472 files source-audited; 463 remain. P2 full dual build/publication
and regression is running in the active task scratch; focused success is not
full qualification or delivery. No P2 commit or closure yet.

### Following read-only checks during P2 regression

Files 10-25 (aaa through arpl, source/header pairs) are byte-identical after
required line-ending normalization to fixed XP. Files 26-31 (bound, bsf, bsr
pairs) have no in-scope differences. Original c_main directly calls or selects
these instruction workers; their declarations are included there and the CMake
CCPU source list compiles their definitions. These files remain necessary;
Compat does not provide duplicate instruction workers.
File 32 bswap.h is under consumer investigation: CPU_486 selects its include,
but SPC486 implementation branches are excluded and BSWAP opcodes use Int6.
Do not equate an included declaration with an actual implementation consumer.
Preliminary read-only comparisons of later files do not advance ordered audit
completion beyond file 31. Thus 31/472 audited, 441 remaining.

P2 source changes are +4/-6 (net -2), tests +13/-3 (net +10). Raw normalized
XP-relative diff added/deleted delta is -6/-4, ten fewer diff lines across the
two changed files. The residual display ignoring required line controls goes
from slave +2/-1 to +1/-1 and tape +6/-5 to +1/-1; do not confuse its eleven-line
reduction with the raw comparison's ten. Existing excluded bodies receive no
new ledger or counts. Actual diff review confirms the only runtime restoration
is INT15/C0 getCS; EOI and descriptor corrections are unchanged pending decisions.

### P2 final qualification and review

Final source passes x86 188/188 background plus 8/8 native and x64 188/188
background plus 8/8 native: 392/392, no failures or skips. Both focused actual
INT15 ROM tests also passed. Documentation, XP manifest and whitespace gates
pass. Builds published fresh timestamps Nov 12 2026 23:12:02 (x86) and
23:12:09 (x64). Published SHA-256:

- softpc32.exe: 7e8781e7c8d68311a2a79aff04d441ad2f0e0e49adddfd9dbd742ecce8b2723f8
- softpc64.exe: c9f5c98fdd40819165f3b340f320fffa5c567136443ccfd767d73fe1302be2ad

Executor review of actual diff: two mirror files, one existing machine test,
manifest/audit/state records and two published EXEs only. No public package,
Compat implementation, configuration, firmware or snapshot changes. This
restoration adds no runtime branch or duplicate path. The test extends one
existing boot fixture rather than adding a parallel BIOS fixture. Previously
qualified residuals R0006/R0008/R0010 remain owner decisions, not approval.
Runner completed with exit zero and is no longer live. Disposable P2 evidence
is removed after promotion here; caches are retained. P2 delivery does not
close S1 or T100. Source review stands at 31/472; file 32 remains investigation.

## P3 file 32 whole-file retirement (local, verifying)

R0013: bswap.h is identical to XP but has no necessary production declaration
consumer. It declares only BSWAP(IU32*); repository source/test scans find no
call or address-taking reference. c_main includes it under CPU_486 (enabled),
but opcodes 0FC8-0FCF use the original Int6 path under the supported SPC386
selection. The SPC486 implementations are already excluded; they are not
reopened or counted. bswap.c is absent and there is no Compat implementation.
Thus adding the declaration is not implementing a supported instruction.

Owner's explicit whole-file-and-wiring deletion supplement authorizes retiring
this file. Delete its 16 lines and its sole include, replacing the include with
an empty line to preserve all following diagnostic locations. This is a coupled
edit in c_main.c, not completion of that later file's audit. The remaining
CPU_486 conditional and its other headers are untouched. Counter-review:
removing an unreferenced declaration cannot change opcode dispatch; do not
accidentally enable original SPC486 code or claim increased CPU support.
Current mirror inventory becomes 471; frozen audit universe remains 472 with
one explicit deletion disposition. This is not claimed as a mirror-diff
reduction: the retained c_main file gains one original-relative replacement
line pair, and the removed identical file is reported separately.
Production +1/-17 (net -16), tests unchanged. Dual builds and regressions run
before delivery; object hashes before the change are captured for comparison.

Files 33-40, bt/btc/btr/bts source and header pairs, have no in-scope differences
against fixed XP. CMake includes the original CCPU source list; c_main includes
their headers and selects their functions for bit opcodes. They are necessary
original instruction implementations, with no duplicate Compat path. No edit.
40/472 source-audited; 432 remain. File 41 c-files is under investigation:
its nine omitted entries require coherent source-selection review, not blind
restoration of references to retired files. No new approval of those omissions
is implied by their historical presence.

### File 41 source-selection decision

R0014, c-files, original-relative +0/-9: omitted bswap.c, ccpupig.c,
cmpxchg.c, c_getset.c, invd.c, invlpg.c, nop.c, wbinvd.c and xadd.c.
Fixed XP inspection shows six instruction bodies are entirely SPC486-owned;
ccpupig and c_getset implement the PIG comparison harness. Neither selection
is supported here. NOP is an empty original worker and original c_main's
0x90 case already breaks without calling it. No corresponding Compat
instruction/harness implementation exists to replace with these originals.
The build reads c-files directly; restoring only entries would reference absent
files. Coherent restoration would also restore unnecessary whole source files,
contrary to the owner's explicit whole-file retirement policy. These are
source-selection omissions, not fixes to original algorithms. Minimal remaining
option is the nine omitted lines; owner disposition remains pending, not inferred
from prior retirement approvals. No new runtime negative test is claimed.
Counter-review explicitly inspected original c_getset's PIG purpose instead of
assuming its name denoted the product's register-access implementation.
File 41 completes source review; 41/472 audited, 431 remain.

P3 dual builds succeeded. Before/after c_main object hashes are identical:
x86 46fa193708f6d2a64c1d4c441a2a4b6fdd802b69a4497d253a62c7275b2372f8;
x64 03e80b78a3fdbdab7ece859a53d2843b0ca0b89e20be192e8712597807c914fb.
This proves equality of these compiled objects, not a new manual guest test.
Already started full regression continues; no cancellation on equivalence.

### Owner rejection: withdraw file 32 retirement

Owner rejects removing bswap.h and its include because that increases mirror
diff. Restore both files and the current manifest exactly to P2 HEAD. File 32
is retained XP-identical by explicit owner direction; no deletion is delivered.
Earlier P3 retirement descriptions above are withdrawn candidate history, not
current disposition. The original 472-file current inventory is restored.
No P3 source/test change remains. The already-running candidate qualification
is not final-source proof for the restored tree; it is allowed to finish rather
than killing a live test. P2 already qualified this exact source/test state.
Do not retire an identical header merely to remove an unused declaration when
its include removal adds mirror diff. Other audit progress is preserved.

Final owner clarification prohibits any increase in mirror diff points or lines;
do not request an exception. Discard increasing candidates and continue. This
supersedes the earlier retirement rationale and increase-approval language.
The withdrawn candidate run finished 392/392 with exit zero; this is recorded
only as withdrawn-candidate evidence, not qualification of a new delivery.
Both published EXEs are restored byte-for-byte to accepted P2 along with all
source/test/manifest contents. Only audit progress and owner-rule documentation
remain changed. No code or binary retirement ships in this continuation.
