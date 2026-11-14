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

## P4 file 42 call.c minimization (verifying)

R0015 is the sole non-excluded semantic difference: XP CALLF chooses SET_SP
versus SET_ESP using GET_OPERAND_SIZE after loading a more-privileged SS.
The existing repair calls original set_current_SP. Actual c_main CALLF dispatch
is compiled and accepts call gates; after TSS stack selection and load_SS_cache,
original spush/spush16 address the new stack using SS's address-size attribute.
A 16-bit gate entering a 32-bit stack with a different ESP high half demonstrates
the mismatch: XP updates only low SP, then spush writes via the stale high ESP.
This is source proof, not a newly reproduced guest incident. The checkpoint
verify_stack_address_width test only calls the helper and does not execute a
CALL gate; do not overstate its coverage.

Fixed XP c_stack.c already implements set_current_SP by SS width. Restore XP's
four-line if/else structure and change only GET_OPERAND_SIZE to GET_SS_AR_X.
The attribute has the two supported USE16/USE32 values; branch effects equal
the existing original helper. No new condition/path/helper/state is added.
Residual changes from +1/-4 to +1/-1, reducing original-relative diff by three
lines without a new point. Source +4/-1, net +3; tests unchanged. Counter-review
checks the newly loaded SS owns this choice and distinguishes operand item size
from stack address size. R0015's remaining one-line semantic correction awaits
owner disposition; minimization is allowed by the approved method.

Files 43-46 (call.h, cbw.c/h, ccpupig.h) have no in-scope differences against
fixed XP. CALL/CBW declarations and workers are consumed by c_main. Preserve
ccpupig.h and original includes under the owner's no-diff-growth rule; existing
#error bodies are not revisited or counted. Source review now 46/472; 426 remain.
File 47 ccpusas4.c is under investigation: added declarations/includes, snapshot
accessors, range check, BIOS privilege selection and fallback return widths.
No changes to that file are included in P4 or qualified by this batch's tests.

File 47 investigation notes (not dispositions): host_sas_init returns UTINY*
from Compat memory.c and assigns the live RAM base; lack of its prototype can
truncate a native pointer on x64. The added emm.h/host.h macros are not referenced
by this translation unit; verify declaration and preprocessing effects before
restoring these includes. Snapshot accessors are called from Compat archive.c,
and include private selectors_set/code_sel/data_sel as well as RAM/page kinds;
a caller alone does not establish why original public APIs cannot replace them.
Original c_page.c independently requests PG_U at CPL3. The existing BIOS access
test distinguishes instrumented permission checks from real ROM fallback width
execution; inspect each category's actual assertions before claiming coverage.
No changes or completed audit status for file 47 yet.

File 47 additional source evidence: phyR's original addr+typeSize+1 >= size
rejects valid terminal byte/word/dword accesses. device_contract_smoke directly
asserts phy_r16(size-2)==0x8776 and phy_r32(size-4)==0x87766554 after independently
checking host RAM bytes; distinguish these from VM argument-rejection tests.
The existing virtual-selector setter is not a substitute for raw snapshot
restoration: c_SasRegisterVirtualSelectors reconstructs two guest LDT descriptors
and always sets selectors_set TRUE. Reusing it would mutate restored guest RAM
and could not express an unregistered snapshot. This establishes a concrete
private-state requirement, not merely a dependency on a current caller.
No file 47 modification or completed-file claim yet.

### P4 final qualification

Final x86 and x64 builds publish both EXEs; each architecture passes 188/188
background and 8/8 native tests: 392/392, zero failures/skips. No new manual
guest acceptance or complete call-gate matrix is claimed. Existing source proof
and unchanged semantics justify minimization, not a new general CPU correctness
claim. Raw normalized XP diff changes from +13/-34 to +13/-31 for call.c:
three fewer diff lines, no new difference point. Production +4/-1 (net +3),
tests unchanged. This deliberately reduces mirror difference, not source size.

Published timestamps: Nov 12 2026 23:31:18 x86; 23:31:27 x64. SHA-256:

- softpc32.exe: e62fbbb4253a752efa4aecf554684e05bed185a07bd0beb2e70e837f706a438b
- softpc64.exe: 45e2d8525f3f28b91276c36ce89893da151c09a044ac17f07754a20949233fc0

Actual-change counter-review verifies USE16=0, USE32=1 and the new SS attribute
is the same source used by original set_current_SP. Only call.c production
changes, with its manifest, audit records and published artifacts. No shared
component, Compat, guest, media, configuration or snapshot changes. R0015 is
still a pending owner decision, not an automatically accepted semantic patch.
Full runner exited zero and no longer exists. Promote this evidence and remove
P4 scratch; caches remain. S1/T100 remain active at 46/472 files reviewed.

## P5 file 47 ccpusas4.c review

Point order follows the source. Existing exclusions are skipped without ledger.

| Point | Difference and disposition | Concrete evidence / counter-review |
| --- | --- | --- |
| R0016 | Added emm.h: restore XP omission | No used macro/declaration; final x86/x64 object hashes unchanged after removal. |
| R0017 | Added host.h: restore XP omission | Native calls used here are declared elsewhere; final object identity verifies removal. |
| R0018 | Added snapshot.h: pending owner | Declares the snapshot state ABI used by the two private-state accessors below; original has no equivalent private-state export. |
| R0019 | host_sas_init pointer prototype: pending owner | Actual allocator returns UTINY*, assigned as RAM base; implicit int return would truncate native pointers on x64. |
| R0020 | host_sas_term pointer prototype: pending owner | Actual provider returns UTINY* and original caller compares it with NULL; prototype restores matching C ABI. |
| R0021 | SAS capture accessor: pending owner | Captures RAM, page-type data and private virtual-selector state for existing snapshot behavior; no original getter for selectors_set/code_sel/data_sel. |
| R0022 | SAS restore accessor: pending owner | Original selector registration rewrites guest LDT and cannot express unregistered state; cannot replace raw snapshot restoration without changing saved state. Length checks bound the copies to current allocations. |
| R0023 | phyR local explanatory comment: restore XP omission | Nonsemantic; reason retained in this ledger. |
| R0024 | phyR bounds condition: pending owner | XP rejects complete end-of-RAM accesses; real device_contract assertions use independent RAM bytes and terminal WORD/DWORD reads. Subtraction also avoids addition wrap. |
| R0025 | BIOS accessor local explanatory comment: restore XP omission | Nonsemantic; permission rationale is recorded here. |
| R0026 | BIOS byte-read CPL comparison: pending owner | PG_U=2 means user; original c_page independently chooses it only at CPL3. |
| R0027 | BIOS word-read CPL comparison: pending owner | Same direct mapping permission contract, with real BIOS wrapper coverage. |
| R0028 | Protected fallback word cast IU8 to IU16: pending owner | Original biosDoInst returns EAX; IU8 discards bits 8-15 before the IU16 function return. |
| R0029 | V86 fallback word cast IU8 to IU16: pending owner | Same narrowing at the other actual ROM fallback entry. |
| R0030 | BIOS dword-read CPL comparison: pending owner | Same user/supervisor permission selection. |
| R0031 | Protected fallback dword cast IU8 to IU32: pending owner | Original IU8 discards high 24 bits before the DWORD result. |
| R0032 | V86 fallback dword cast IU8 to IU32: pending owner | Same narrowing on real-mode ROM fallback. |
| R0033 | BIOS byte-write CPL comparison: pending owner | User flag must accompany CPL3 writes, not supervisor writes. |
| R0034 | BIOS word-write CPL comparison: pending owner | Same actual access_check contract. |
| R0035 | BIOS dword-write CPL comparison: pending owner | Same actual access_check contract. |

Permission evidence is source proof plus the existing BIOS test's instrumented
translation/access wrapper assertions, not exhaustive paging verification.
Fallback-width tests execute unchanged original ROM instructions for protected
and V86 paths; only the outer translation miss is controlled. P4 passed these
tests, but no new original-negative run is claimed in this read-only review.
Snapshot encode/decode and cross-process tests cover existing accessors; private
selector necessity is additionally established by the original setter's guest
memory side effects. These are narrow state accessors, not duplicate device or
CPU logic suitable for replacing with Compat. The only native allocation owner
is Compat memory.c; original SAS still owns reads/writes and page semantics.

Four nonsemantic added lines are removed; production +0/-4, no test changes.
Remaining non-excluded display difference +69/-11, versus +73/-11 before.
Two final-width targeted builds pass and ccpusas4.c.obj is byte-identical to P4:
x86 11d088a1b15ddfa1e96b87629d7635eaac4700b64796091240c22ddede860505;
x64 db5a7afa408a3c716a37f8da41264aeb78d9ab079452d6b842c1d9b7445428da.
This is final-source object equivalence, not fresh runtime coverage.

File 48 ccpusas4.h has no non-excluded difference and is used by real CPU/SAS
consumers. Files 49-62 (cdq/clc/cld/cli/clts/cmc/cmp source/header pairs) and 63
cmpxchg.h are normalized-identical to XP. Preserve the unused instruction header
under the no-diff-growth rule; no deletion is attempted. Verify actual instruction
wiring before assigning final completion to these preliminary comparisons.

### P5 final evidence and actual-diff review

Final ccpusas4 object comparison passes on both architectures after all four
restorations. Both product builds succeed and publish fresh EXEs. Entire EXE
byte comparisons against accepted P4 find exactly seven differing bytes each.
They lie only in PE COFF TimeDateStamp (PE+8), OptionalHeader CheckSum
(PE+24+64), and the single changed NUL-terminated compiler HH:MM:SS string.
All other bytes and file lengths are identical. This validates the owner's
artifact-equivalence exception; no full runtime rerun is claimed or needed.
P4's 392/392 remains the behavior regression baseline, not a fresh P5 test run.

Published compile times Nov 12 2026: 23:43:17 x86, 23:43:20 x64. SHA-256:

- softpc32.exe: 2365a7bf86378673ce1b7d399007e8b4dfc8b8e144977043b69bb9a04d68b927
- softpc64.exe: b4ef20365db48485713a9342f028664c7df6fff752ee779d99f2061851d19719

Actual review: source +0/-4, tests unchanged, no new difference point; only
four existing added lines restored. No Compat/shared package/config/media/guest
change. Pending semantic/ABI/snapshot differences in the table are not owner
approved. Source caller review confirms original CCPU dispatch for CDQ, CLC,
CLD, CLI, CLTS, CMC and CMP, with original header and source-list wiring.
File 63 cmpxchg.h stays identical under no-diff-growth policy; no unused-header
cleanup is delivered. Ordered coverage 63/472, 409 remaining; file 64 cpu4gen.h
is investigating. Owned compilation/publication jobs exited; scratch is removed
after evidence promotion. S1 and T100 remain active.

## File 64 cpu4gen.h: GDP include restoration and declaration review

Initial analysis: the non-excluded difference is +15/-14. Two include changes
force the C-VID GDP header and inject gdp_slots.h; thirteen declarations change
return types. Current dependency alone does not establish either include needed.
Counter-review uses the pinned XP c_reg.c definitions, actual selected CCPU
callers and each Ninja dependency graph, not the historical patch comments.

| Point | Difference | Disposition and concrete evidence |
| --- | --- | --- |
| R0036 | Force ../cvidc/gdpvar.h and replace the original comment | Restore exact XP angle-bracket include and comment; the selected CCPU interface expands no GDP macro requiring this forced vocabulary. |
| R0037 | Add gdp_slots.h | Remove this include; real C-VID glue retains its own existing native-slot adapter include. No adapter is deleted or replaced. |
| R0038 | c_setCS: IUH to ISM32 | Pending owner: original definition returns ISM32, not IUH. |
| R0039 | c_setSS: IUH to ISM32 | Pending owner: same original return-type mismatch. |
| R0040 | c_setDS: IUH to ISM32 | Pending owner: same original return-type mismatch. |
| R0041 | c_setES: IUH to ISM32 | Pending owner: same original return-type mismatch. |
| R0042 | c_getCF: IBOOL to ISM32 | Pending owner: original definition returns ISM32, not IBOOL. |
| R0043 | c_getPF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0044 | c_getAF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0045 | c_getZF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0046 | c_getSF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0047 | c_getTF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0048 | c_getIF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0049 | c_getDF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |
| R0050 | c_getOF: IBOOL to ISM32 | Pending owner: same original return-type mismatch. |

The original c_reg.c explicitly avoids including cpu.h because internal/external
register macros conflict. Thus a normal separate compilation does not diagnose
these cross-translation-unit incompatible function types. Selected host types
are ISM32=long and IBOOL=int; IUH is unsigned int or native UINT_PTR for the
C-VID compilation contract. Equal 32-bit widths do not make int and long the
same C type. Keeping the declarations consistent with the original definitions
is a source/type-ABI necessity claim, not a reproduced guest failure. Changing
original providers or global typedefs instead would relocate and enlarge the
patch, and is not this minimal audit disposition. These thirteen points remain
owner decisions, not approved retention.

Both architectures have 58 actual Ninja compilation dependencies on cpu4gen.h.
All 116 were preprocessed before and after the two include restorations, using
actual compiler flags and no compiler cache. Every changed token line is a
removed unused GDP function declaration or a removed/relocated CRT thread
function declaration. No function body or expression changes. The temporary
probe restored its input before final candidate application. This comparison
is compiler/source evidence, not runtime coverage or object equivalence.

Candidate production +1/-2; tests unchanged. Remaining non-excluded XP diff
is +13/-13: three fewer diff lines and two removed difference points, no new
point. Actual native GDP adaptation remains in its existing C-VID owner.
Dual product builds and final artifact qualification pass as recorded below.

### P6 final evidence and independent actual-diff review

Both cached product builds finish successfully and publish the final header.
Across 1088 recorded object files, only the two mandatory timestamp-bearing
composition.c objects change. Both entire EXEs have exactly eight changed bytes,
all within PE COFF TimeDateStamp, OptionalHeader CheckSum and the changed compiler
HH:MM:SS literal; file lengths and every other byte match P5. This satisfies the
owner's artifact-equivalence exception. No runtime test rerun is claimed;
P4's 392/392 remains the behavior baseline through equivalent P5 and P6.

Published compiler times on Nov 12 2026: x86 23:58:31, x64 23:59:03.
SHA-256:

- softpc32.exe: a7cc9b34d313faf9d5f2ef81d8350c45c19ab14bd4aa2785cba8ecf5311a38cd
- softpc64.exe: 78c3184532cf421a4361f038812d9441fd8916da6114247d66a74289d28065b5

Actual coordinator review: only the two include restorations change production;
+1/-2 source, zero tests, XP-relative non-excluded diff +15/-14 to +13/-13.
No new difference point, no alternate GDP path, no Compat or shared-tree edits.
The C-VID glue's own slot adaptation remains unchanged; the selected CCPU
interface no longer injects it into unrelated consumers. Original-return-type
proof is separate from this equivalence test and does not claim a guest failure.
Mirror/compiler warnings are not reported as new shared-component defects.
File 64 is reviewed and restored; thirteen residual declaration decisions remain
pending owner choice. Ordered coverage is 64/472, 408 remaining; file 65
cpuint_c.h is next. S1 and T100 remain active.

## Files 65-79: original CCPU contracts and private state

Files 65-71 (cpuint_c.h, cwd/cwde C/H and c_addr C/H) are normalized-identical
to fixed XP. cpu4.h includes CpuInt_c.h; the actual c-files build includes the
instruction/address sources, and c_main dispatches CWD/CWDE through their original
headers. File 72 c_bsic.c has no non-excluded difference. Files 75-77 c_debug.h
and c_div64 C/H are normalized-identical; original DIV/IDIV call divu64/div64.
File 79 c_intr.h is normalized-identical and declares the actual interrupt entry.
No original file or original wiring is removed.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0051 | c_bsic.h adds c_getEFLAGS declaration. | Pending owner: the original internal PUSHF/c_intr/c_tsksw callers use c_bsic.h, not the external cpu4gen.h interface. c_bsic.c returns IU32 (unsigned long); without this declaration an implicit int function is called across translation units. |
| R0052 | Blank line added after that declaration. | Restore the original spacing by removing the extra line; no token change. |
| R0053 | c_debug.c adds snapshot.h. | Pending owner: type declaration for the bounded private-state accessors below; original public debug API cannot capture its private arrays. |
| R0054 | c_debug.c adds capture_debug. | Pending owner: original i_brk/d_brk contents can differ from a fresh DR7-derived table, so capturing only registers is not equivalent. |
| R0055 | c_debug.c adds restore_debug. | Pending owner: restore those private arrays/counts exactly; count bounds protect original exception loops from indexing beyond four entries. |
| R0056 | c_intr.c adds the T60 explanatory comment. | Restore original by deleting this nonsemantic task comment. |
| R0057 | c_intr.c rejects the IU16 representation of -1. | Pending owner: not redundant with c_main's filter; actual original NPX interrupt delivery also calls do_intrupt with the unchecked PIC result. |
| R0058 | Blank line added after the rejection. | Restore original spacing; retain only the two executable lines pending owner decision. |

R0051 proof boundary: an isolated original-header override compiled the real
pushf.c with each architecture's actual compiler flags, including gnu90.
Both current and original-header variants return compiler success. Therefore
this is NOT a demonstrated build failure. The source/type mismatch remains:
the original c_getEFLAGS definition is unsigned long, while an undeclared call
implicitly uses int. The separate external CPU interface cannot be included in
these internal CPU units without replacing their register macros (the original
c_reg.c explains that boundary). This is C function-type consistency evidence,
not a demonstrated Windows flag-value truncation or guest failure. Keeping one
internal declaration is the minimal candidate; no new declaration path is added.

R0053-55 counterexample is in original XP, not inferred solely from an existing
snapshot consumer: c_tsksw.c clears LOCAL_BRK_ENABLE in DR7 without calling
setup_breakpoints. check_for_inst_exception then reads the cached i_brk table
and nr_inst_break, independently of current DR7. Calling setup_breakpoints on
load would erase that state and change subsequent behavior. Existing
checkpoint_smoke verify_cpu_side_state explicitly demonstrates that distinction;
P4 passed it, but this source review does not claim a fresh dynamic run. Compat
archive.c already owns serialization and calls these accessors; it does not
reimplement breakpoint matching. No private table is moved or duplicated.

R0057 complete caller check: c_main filters acknowledged_interrupt before its
external-delivery path. However, original zfrsrvd.c TakeNpxExceptionInt raises
IRQ13, and when IF is set calls ica_intack then do_intrupt without filtering.
An asserted but masked IRQ13 does not guarantee a deliverable PIC vector.
Original ica_int_accept returns -1 when its pending scan finds no eligible IRQ,
and also on the selected non-NTVDM ISR-depth rejection path. Without the shared
check, do_intrupt treats 65535 as an IVT/IDT index and enters interrupt delivery
instead of rejecting it. INTx/INTO and exception callers use actual architectural
vectors; there is no additional product caller. Existing irq_smoke covers a real
PIC -1 result and unchanged stack/registers at do_intrupt, plus the actual stale
CPU-request loop. It does not dynamically reproduce the NPX masked-IRQ scenario;
that applicability is source proof. Moving a new guard into zfrsrvd would add a
new mirror point and is prohibited. The existing common check remains pending
owner choice; no new guard or special case is introduced.

Candidate source +0/-3, tests unchanged. c_bsic.h's non-excluded XP diff is
+2/-0 to +1/-0; c_intr.c +4/-0 to +2/-0. c_debug.c remains +45/-0.
The aggregate mirror reduction is three added lines, with no new point.
Final dual product qualification passes as detailed below; no new runtime run claimed.

### P7 final evidence and actual-diff review

Both final-source cached product builds pass and publish. All recorded objects
remain identical to P6 except the two required compiler-timestamp objects.
Entire EXE comparison finds exactly eleven changed bytes per architecture,
limited to PE timestamp/checksum and compiler date/time strings (the build
crossed midnight). Every other byte and file length equals P6. Per the approved
artifact-equivalence exception, no runtime rerun is required or claimed;
P4's 392/392 remains the runtime baseline through the equivalent deliveries.

Published compile stamps: Nov 13 2026 00:06:15 x86, 00:07:11 x64.
SHA-256:

- softpc32.exe: b0643e3f3125ff5761af7c3118bffdf4728f820191e03247651cb2aee05c0380
- softpc64.exe: 7dd8901ac40f5d56f40b10a0a630fa1e1c5529f8112b52806a5aff84515ecc08

Coordinator actual-change review confirms source +0/-3, zero test changes;
only one added blank line and one task comment/blank line are restored.
No function, error handling, snapshot layout, guest, configuration or shared
component changes. Existing semantic points R0051/R0053-R0055/R0057 remain
pending owner decisions with explicit source/probe evidence limits. Documentation
and XP mirror gates pass. Owned jobs exited; scratch is removed after promoting
these results. Source audit coverage is 79/472, 393 remaining; next file is
80 c_main.c. S1 and T100 remain active, not ready for closure.

## File 80 c_main.c: complete selected-code review, candidate P8

The review uses the fixed XP file and real CCPU/Compat/Machine callers; existing
exclusions are skipped. Initial non-excluded difference is +186/-20. Candidate
is +135/-15, reducing XP diff by 56 lines, with no new mirror difference point.
This is separate from production +10/-65 (including Compat -9) and test +34/-0.
Only c_main.c, Compat platform.c and the existing checkpoint test change code.

The table follows original/current source order. Anchors identify the actual
code blocks; added blocks have no original body and are located between their
unchanged surrounding anchors. Related blank lines and local explanation-only
comments are restored in the same point, never counted as excluded-body work.

| Point | Location / original versus local | Initial analysis and counter-review disposition |
| --- | --- | --- |
| R0059 | ntthread include condition | Pending owner: select original TLS simulation-stack implementation without enabling NTVDM behavior globally. The alternative retained branch is deliberately unavailable; no new thread implementation is added. |
| R0060 | added c_addr.h | Pending owner: internal operand/address macros call the original typed address helpers. No duplicate implementation or new API. |
| R0061 | added lifecycle.h | Pending owner: copied entry shape and lifecycle observer interface, described below. |
| R0062 | added c_seg.h | Pending owner: original reset uses load_pseudo_descr and operand helpers use the original segment contract; avoids implicit mismatched declarations. |
| R0063 | added ica.h | Pending owner: actual PIC acknowledge declaration and selected ABI. |
| R0064 | added timer.h | Pending owner: original host_timer_event declaration; no separate timer implementation. |
| R0065 | added snapshot.h | Pending owner: bounded execution-state accessors for private state. |
| R0066 | force_yoda declaration | Pending owner: real calls in invalidFunction/note_486_instruction/SAD handling require the actual void host signature; not only excluded debug code. |
| R0067 | TakeNpxExceptionInt declaration | Pending owner: selected NPX path calls the original void function. |
| R0068 | executor_event declaration | Pending owner: one host callback at original CPU safe points, no device logic copied. |
| R0069 | timer_event declaration and three calls | Restore direct original host_timer_event; remove the unnecessary Compat forwarding function. |
| R0070 | has_pending_executor_event declaration | Pending owner: source proof for leaving the original quick loop before host events can be consumed. |
| R0071 | consume_clock_tick declaration | Pending owner: host timer thread records work; original executor owns device mutation. |
| R0072 | consume_executor_wake declaration | Pending owner: input/control wake must not manufacture a guest clock tick. |
| R0073 | consume_instruction_budget declaration | Pending owner: public finite run cannot return at an arbitrary host stack location. |
| R0074 | pace_instruction declaration | Pending owner: existing continuous-run clock policy is owned outside the CPU and is not applied to finite runs. |
| R0075 | wait_for_executor_event declaration | Pending owner: blocked HLT needs original executor wake, without consuming CPU in a spin. |
| R0076 | debug observer declarations/comment | Pending owner for declarations; restore explanation-only comment. Actual policy remains machine/debug.c. |
| R0077 | interrupt_map_address after private map | Pending owner: original keyba resets this private bit map; Compat lifecycle clears pending bits at new cold run. Original reset does not clear the map; no public original clear-all operation replaces this access. |
| R0078 | capture_execution after start_trap | Pending owner: private start_trap and interrupt_map cannot be recovered solely from architectural registers. DR/TR copies travel with the same bounded state accessor. Restore the local explanatory comment. |
| R0079 | restore_execution | Pending owner: restores the same private state and discards transient quick_mode, so the next host-pointer fetch is rebuilt. Does not recreate device state. |
| R0080 | CCPU_INSTRUCTION_DELTA helper | Restore original: no necessary native-pointer narrowing; pointer difference is bytes within the current decode page, not a pointer value. |
| R0081 | UPDATE_INTEL_IP len type/expression | Restore original int len and DIFF_INST_BYTE. Page-boundary decoder resets p_start; original int can represent this bounded delta on both supported builds. |
| R0082 | UPDATE_INTEL_IP_USE_OP_SIZE, USE16 | Restore original entire-sum mask. Local mask placement changes actual wrap behavior; dual-architecture negative/positive evidence below. |
| R0083 | same macro, USE32 | Restore original DIFF_INST_BYTE; assignment to original IU32 EIP supplies the architectural width. |
| R0084 | local softpc_ccpu_entry | Pending owner: carries only the saved fetch/HLT phase and trap bit. Remove inline task comment. |
| R0085 | exception setjmp condition and dereference | Pending owner: existing TLS jmp_buf pointer must be dereferenced to the buffer expected by the compiler; preserve original setjmp lifetime inside ccpu. |
| R0086 | take_entry before NEXT_INST | Pending owner: restored HLT must remain halted, not fetch the following instruction; fetch resume skips duplicate boundary work. Remove task comment. |
| R0087 | debug_begin at DO_INST | Pending owner: debugger can edit CS:EIP/code while parked; original cached p/p_start must be invalidated by NEXT_INST, with quick_mode cleared so a pending TF trap is not skipped. Remove explanation-only comments. |
| R0088 | debug_retired after POP SS | Pending owner: this original early NEXT_INST path bypasses ordinary retirement; removing it loses instruction counts and trace-stop completion. |
| R0089 | debug_retired after MOV SS | Pending owner: same distinct original early path, without changing interrupt inhibition. |
| R0090 | debug_retired after IRET | Pending owner: original IRET jumps directly to NEXT_INST; do not move retirement across the original interrupt/trap rules. |
| R0091 | RESUME_HALT and HLT checkpoint | Pending owner: precise saved HLT/trap phase and outer-depth capture; original BOP recursion can remain active. Remove task comment. |
| R0092 | added HLT SIGIO block | Restore original: selected async_handlers has only the terminating entry; full source/test search finds no other SIGIO producer. |
| R0093 | HLT clock/wake consumers | Pending owner for consumers; use original host_timer_event and remove local explanation. Without consumption a wake returns to waiting without delivering input/devices. |
| R0094 | HLT empty-map/quick-event wait | Pending owner: wait only when neither original interrupt map nor quick-event work can progress; actual event wait is owned by Compat. |
| R0095 | debug_retired after STI | Pending owner: original interrupt-inhibition jump bypasses normal retirement. |
| R0096 | normal debug_retired | Pending owner: single observer for ordinary retired instructions; four early paths above do not reach it. |
| R0097 | quick-mode host-event escape | Pending owner: original quick loop jumps straight to DO_INST, bypassing mailbox consumption and indefinite stop/timer/input progress otherwise. Remove local comment. |
| R0098 | added normal SIGIO block | Restore original for the same verified no-producer reason as R0092. Keep original event enum/definitions and original PIC implementation unchanged. |
| R0099 | normal clock/wake consumers before traps | Pending owner: deliver queued host/device changes before original trap/IRQ tests. NEXT_INST also has direct-entry callers, so simply retaining only one location would change this order or omit entry work. Use original timer callback. |
| R0100 | PIC acknowledged_interrupt/filter block | Minimize existing repair: original IU16 variable/assignment can carry valid 0..255 vectors or the converted -1 sentinel; compare with (IU16)-1 just as do_intrupt does. Remove the added signed temporary, redundant copy and explanation. Remaining filter pending owner: reject before setting EXT or performing interrupt-boundary work, not only after calling do_intrupt. |
| R0101 | NEXT_INST clock/wake consumers | Pending owner: entry, restart and inhibited-interrupt paths reach this label without passing the normal checks. No second queue or device state owner. Use original timer callback; remove comment. |
| R0102 | NEXT_INST pace call | Pending owner: existing host timing contract; the hook does not implement time policy in the mirror. Remove comment. |
| R0103 | finite budget exit | Pending owner: requests the existing outer unwind only at a safe boundary, and lets pending hardware reset win. Removing it makes finite machine_run unbounded. |
| R0104 | RESUME_FETCH/checkpoint | Pending owner: saved phase resumes before original decode setup; does not repeat pacing/budget consumption. Remove comment. |
| R0105 | ccpu_new_code_page IP delta | Restore original expression, completing removal of the delta helper rather than leaving a divergent replacement. |
| R0106 | init TLS condition | Pending owner: initializes the original TLS stack used by selected simulate/exception paths. |
| R0107 | c_cpu_clearHwInt | Pending owner: original CPU vector declares this operation; actual PIC deassertion calls it. Original standalone body is missing, while c_cpu_interrupt only sets bits. Remove added explanation; retain narrow bit clear. |
| R0108 | lifecycle_enter in c_cpu_simulate | Pending owner: counts real nested simulation frames for safe snapshot capture; no second CPU executor. |
| R0109 | simulation setjmp condition/dereference | Pending owner: same selected original TLS buffer contract as R0085. |
| R0110 | lifecycle_leave after simulation return | Pending owner: balances normal original unsimulate, including nested BIOS/BOP returns. |
| R0111 | c_cpu_continue TLS condition | Pending owner: exception resumes the active original C frame, not a new interpreter path. |
| R0112 | c_cpu_unsimulate TLS condition | Pending owner: returns through the original matching TLS simulation frame. |

### Counter-review evidence and limits

The original CPU executes nested host_simulate calls from BIOS callbacks; native
C stack frames cannot be serialized as a fresh-run continuation. Existing
checkpoint_smoke covers depth-two capture deferral, normal nested BOP return,
outer HLT continuation, saved trap versus changed TF, fetch continuation and
large finite budgets. Private start_trap is sampled on instruction entry and
need not equal current TF when HLT is paused. This establishes why architectural
register restoration alone is insufficient. These tests are existing evidence;
new final-source qualification is reported separately below when complete.

The mirror has five retirement call sites, not five retirement owners: POP SS,
MOV SS, IRET and STI each bypass the ordinary end-of-instruction observer. All
share machine/debug.c's existing observer. Resume refetch needs quick_mode false
because the original quick loop skips trap/event checks. No debug policy is moved
into the CPU. The host timer/control producers only post atomic pending work;
removing their consumption or quick escape leaves a non-branching instruction
stream unable to observe host input/stop/timers. HLT needs the same consumption
before its auto-reset event wait. This is supported source-flow proof, not a
claim that every scheduling interleaving has been replayed.

The old timer wrapper's comment described a macro that is not present in the
selected translation unit. Full source/test search finds only c_main's three
calls and the wrapper itself. Existing original host_timer_event calls and its
timer.h declaration are already selected; the wrapper body is exactly that call.
Direct use removes nine Compat lines without moving device logic into c_main.

Original ICA async_handlers in the selected configuration contains only its
terminator. Its sole CPU_SIGIO_EVENT producer is inside the iteration over real
entries, so cannot post an event. Full source/test search finds no other caller
posting that event or modifying the table. The two added consumers have no
supported source. The original enum/cpu_interrupt case remain unchanged; this is
not a new feature toggle or re-audit of excluded device branches. Snapshots made
by the supported producer cannot contain an organically posted SIGIO bit;
forged/transplanted private snapshot state is not a supported input claim.

The PIC returns -1 or a vector from its byte-sized base plus a 0..7 line. The
existing IU16 vector therefore uniquely represents rejection as (IU16)-1.
The minimized filter still runs before EXT/SYNCH_TICK/do_intrupt; no normal
interrupt, callback, mask or EOI rule changes. R0057's common guard is still
needed by the separate original NPX path; neither existing guard is replaced
with an extra delivery route.

### Real-CCPU IP restoration experiment

A task-local native probe reuses the actual machine objects, device mock owners
and original firmware already used by checkpoint_smoke. It installs only its
own instruction bytes into disposable emulated RAM and uses a task-owned blank
boot fixture; no user guest binary/media is changed or recompiled. The first
setup omitted the required boot medium and failed machine creation in all four
variants; that setup result is not CPU evidence. The corrected fixture runs the
actual CCPU for one JNZ instruction (two budget boundaries), not an extracted
arithmetic substitute. One linked variant uses the current CCPU; the other
replaces only the five IP-macro changes with exact XP expressions in an isolated
c_main object. Both reuse the same remaining production library.

Per architecture, 24 cases cover starts 0xFFE, 0xFFF, 0xFFFC, 0xFFFD, 0xFFFE,
0xFFFF; taken/not-taken JNZ with zero displacement; default 16-bit and 0x66
operand override. Current x86 and x64 each produce 2/24 mismatches: default
16-bit not-taken cases at FFFE/FFFF yield 10000/10001 instead of 0/1. Original
expressions produce 0/24 mismatches on each architecture. Page crossings,
taken paths and 32-bit override cases agree. Pointer subtraction is a bounded
decode-byte count; the selected page decoder advances EIP and resets p_start at
page crossings, so there is no evidence requiring the added narrowing helper.
This does not claim coverage of every branch encoding, protected-mode paging,
or all CPU flags. The existing checkpoint test gains the same 24 cases with
explicit expected next-IP values, independent of implementation formulas.

P8 final-source qualification on 2026-11-13 completed both cached builds and
both publications. x86 and x64 each pass 188 background and eight native
integration tests: 392/392 fresh passes, all six command exits zero. The
checkpoint regression includes the 24 new real CCPU cases per architecture;
existing IRQ, snapshot, debug, package, Console/Window and audio tests pass.
Runtime behavior changes, so no artifact-equivalence exemption is used.
These tests do not claim fresh manual DOS/Win3.1/Win95 desktop acceptance.

Coordinator actual-diff review confirms the changes restore original expressions
and direct callbacks, preserve checkpoint/debug/budget and invalid-vector
contracts, introduce no new mirror diff point, and leave public packages,
user configuration, media and snapshots untouched. The existing mirror manifest
is refreshed for c_main.c only. Reviewed residuals remain owner decisions,
not approved retention. File coverage is 80/472; next is c_main.h.

Published artifact SHA-256:

- softpc32.exe: `137d17620d0012bf2f7ad0710dd9d59ea94a7246b08591856b76d273da98b832`.

- softpc64.exe: `f3df955dd5046d539d6ecf7340686f96030a743926bccc4f1dc79ef3b4daeea6`.

## P9 ordered source comparison: files 81-87

Compared each complete file against the same fixed XP commit and path using
Git show and normalized-line unified comparison, in frozen order:

- 81 c_main.h, 82 c_mem.h, 87 c_oprnd.h: no in-scope differences.
- 83 c_mul64.c, 84 c_mul64.h, 85 c_neg64.c, 86 c_neg64.h:
  entire normalized files identical.

Existing excluded content remains outside this audit and receives no new
accounting. No source, test, manifest or executable changes are needed.
Coordinator review reconciles all seven file dispositions with the actual
comparison. This documentation-only progress does not rerun or replace P8's
392/392 runtime evidence. Coverage is 87/472, not a completed S or T.

File 88 c_page.c remains investigation, not a retention decision. Initial
trace identifies added yoda.h and successful-operand debug notifications.
The original check_D calls remain in the file; the only selected definition
found by whole-src search is the PROD no-op macro in yoda.h. The new host
observer records linear address, width and copied values only during a live
debug instruction; original guest DR checks have separate architectural
exception semantics. Existing command_provider_smoke exercises read/write
watch overlap, trace values, byte/dword/stack and reverse-buffer x87 operands.
These call traces guide the next restoration comparison; they alone do not
qualify all added notifications for retention or complete this file's audit.

## File 88 c_page.c: original stubs and operand observation

The executor compared the entire file to the fixed XP counterpart, omitting
existing excluded bodies from consideration. Non-excluded XP difference is
+22/-6 before this batch and +20/-6 after: two fewer lines and one removed
point, no new point. Production change is +0/-2; no test source changes.

Ordered point review (XP original lines; current lines after restoration):

| ID | XP/current anchor | Initial finding and disposition |
| --- | --- | --- |
| R0113 | after 29 / before 30 | Added yoda.h. Restore original absence: real original check_D stub already links; macro replacement is unnecessary. |
| R0114 | before 435 / before 434 | Added T56 explanation. Restore original absence; no tokens or behavior require it. |
| R0115 | before 435 / 434-435 | Access observer declaration. Pending owner: required typed call to the host observation owner; not original guest DR machinery. |
| R0116 | before 435 / 436 | Read observer declaration. Pending owner: returns the existing operand value while recording it once; signature matches machine/debug.c. |
| R0117 | before 445 / 447-449 | Read-buffer starting pointer, width and address. Pending owner: original loop increments buffer, decrements address and consumes count; final observation needs original values. |
| R0118 | before 462 / 467 | Bulk read observation. Pending owner: after successful original read, reversed x87 buffer is reported without another device access. |
| R0119 | 484 / 490 | Byte direct-physical return. Pending owner: records byte-width successful operand. |
| R0120 | 488 / 494 | Byte supervisor return. Pending owner: records same operand when no cached physical address is supplied. |
| R0121 | 512 / 518 | DWORD direct-physical return. Pending owner: preserves full four-byte data and observation width. |
| R0122 | 516 / 522 | DWORD supervisor return. Pending owner: same contract for uncached path. |
| R0123 | 540 / 546 | WORD direct-physical return. Pending owner: preserves two-byte observation and original returned value. |
| R0124 | 544 / 550 | WORD supervisor return. Pending owner: same contract for uncached path. |
| R0125 | before 562 / 568-570 | Write-buffer original values. Pending owner: original reverse-memory loop consumes pointer/count/address; snapshot of inputs is needed for one completed observation. |
| R0126 | before 582 / 591 | Bulk write observation. Pending owner: reports original reverse-buffer layout only after the write completes. |
| R0127 | before 613 / 623 | Byte write observation. Pending owner: records one byte after either original write branch. |
| R0128 | before 643 / 654 | DWORD write observation. Pending owner: records four bytes after either branch. |
| R0129 | before 673 / 685 | WORD write observation. Pending owner: records two bytes after either branch. |

### Real-path comparison

Both architectures use the actual CCPU compile command from the existing Ninja
tree, an isolated c_page object inserted before the normal machine archive,
and the existing command-provider integration test plus all its real machine,
Emulator and X86 objects. Device mocks remain the test's existing PCM/MIDI
fixtures. No production build object, guest binary or user media is modified
by these comparisons. All variants compile and link successfully.

- Current source: command-provider test passes on x86 and x64.
- Remove only added yoda.h: passes on both architectures. Preprocessing retains
  original check_D calls; nm confirms they bind the linked check_D symbol.
  Whole-src symbol search and fixed-XP stubs.c inspection confirm the original
  PROD CCPU empty implementation, not an invented Compat replacement. The
  earlier narrow search for a parenthesis immediately after check_D missed
  its original IFN0 definition; no missing-symbol claim is made.
- Restore original operand functions and remove their added observer locals,
  declarations and calls: both runs reach the real watchpoint sequence but
  fail the existing bounded wait for PAUSED after g. x86 exits 0xc0000409,
  x64 exits 3; both log the same WaitForSingleObject(..., 10000) assertion.
  This is an observed missing watch completion, not an external timeout or
  a proof of all debugger/CPU cases. Current/no-header controls complete.

### Independent source review

The coordinator traced original c_oprnd.h callers: byte/word/dword operand
macros, and x87 buffer reads/writes for qword, extended, environment and state
operands. The original physical-cache and supervisor alternatives are both
real routes; moving hooks below them would include non-operand internal memory
traffic and lose the common original linear address. Original c_debug.c
checks guest DR enable/type/address and sets guest DR6; it neither supplies
host operand values nor invokes the host's watch state. Reusing it as the
host debugger would change guest debug-register/exception ownership.

machine/debug.c has the single active-debug owner: observations are accepted
only during the current instruction, retirement commits a watch hit, and the
existing executor rendezvous stops before the next instruction. Debugger
inspection does not count as guest access. Read hooks return the already-read
value; write hooks inspect the supplied value, not a second device read.
On supported Windows x86/x64, their unsigned long contracts match the original
32-bit IU32/LIN_ADDR representation. No second tracing state or queue is added.

Existing command_provider_smoke covers byte/dword reads and writes, word
watch overlap, implicit stack and reverse x87 operands with copied-value and
width assertions; those tests qualify the current implementation, while the
isolated negative comparison proves that deleting the whole observer route
breaks actual watch completion. It does not dynamically isolate every hook
or every paging/fault case. Restoring declarations or relocating this same
logic to another mirror point would not reduce the required semantics.
The minimal evidenced residual is the existing observation path (+20/-6),
submitted for owner decision; it is not automatically approved retention.

P10 final-source cached x86/x64 build and publication complete successfully.
Each architecture passes 188 background and eight native integration tests:
392/392 fresh passes, all six qualification command exits zero. The empty-stub
call replaces a no-op macro, so code bytes change; no artifact-equivalence
exemption is claimed. No new tests or generalized test framework are added.
No fresh manual DOS/Win3.1/Win95 session validation is claimed.

The ordered comparison also completes files 89-91: c_page.h has no in-scope
difference, and c_prot.c/c_prot.h are entirely identical after LF normalization.
No change is made to those files. File 92 c_reg.c is the next investigation;
its public-register snapshot copy must be evaluated separately from the
private CPU execution/debug-state bridges already reviewed. No automatic
retention is inferred from a snapshot caller or earlier design comment.

P10 coordinator actual-change review: only the two disclosed production lines
are removed; the current manifest refreshes c_page.c only. Observation state,
CPU/device semantics, guest media, user configuration and snapshots are unchanged.
Qualification includes real native package, audio and desktop tests.
Published artifact SHA-256:

- softpc32.exe: `f5b1f24e5b87d929e590dae14c4836e9a08b21652350d6c1f5da475425b473e1`.

- softpc64.exe: `967f98fa7075f969a05947a6818d00215dfea519e6c14f6cf60bc85e0e9d8ee3`.

## File 92 c_reg.c: use original accessors from the archive owner

Complete fixed-XP comparison finds +69/-0 non-excluded lines before this
batch. The final candidate has zero non-excluded difference, including original
formatting and final newlines. Existing excluded bodies receive no accounting.

| ID | Original/local anchor before restoration | Initial finding and final disposition |
| --- | --- | --- |
| R0130 | after Pigger_c.h / added stdio.h | Restore original absence. Actual x86/x64 isolated objects with and without the header are byte-identical. Original printf produces a compiler builtin-declaration warning; no compile or ABI failure is observed. |
| R0131 | same include block / snapshot.h | Delete: original register owner no longer knows the host snapshot schema. |
| R0132 | original EOF / five-line snapshot explanation | Delete: no original content or independent semantic requirement. |
| R0133 | original EOF / capture_registers | Remove the added mirror entry. Existing archive capture uses the original typed register accessors directly. |
| R0134 | original EOF / restore_registers | Remove the added mirror entry. Existing archive restore uses the same original setters in the same order. |

### Coherent restoration and independent counter-review

Initial caller inspection alone would have justified retaining the two mirror
snapshot entries. Counter-review rejects that conclusion: unlike c_main's
private continuation or c_debug's cached breakpoint table, this entire copy
already uses externally available original accessors. No private register
storage is accessed. Its placement in the mirror is not necessary.

The existing Compat archive owner now executes those ordered accessor calls
inside its existing capture/restore functions. Both added mirror functions,
their public declarations in compat/ccpu/snapshot.h, and the corresponding
calls are removed. No replacement helper or second entry is created. The
existing compat/ccpu/abi.h receives one-to-one declarations for the original
segment/descriptor/CPL accessors; native Windows long is the original ISM32,
unsigned long is IU32, unsigned short is IU16. Actual original definitions,
not the historical generated vector's mismatched return declarations, are the
authority. All six segment accessor families are declared through the existing
macro; no new dispatch table or framework is introduced.

This is not equal-complexity relocation: two function boundaries and their
snapshot-specific API disappear, while the pre-existing archive owner invokes
original APIs directly. Total production is +62/-75 (net -13); test source
is unchanged; mirror difference is reduced by 69 lines to zero. The original
register implementation is retained rather than duplicated. No new mirror
point or line is introduced, and no public shared package changes.

Mechanical token comparison verifies that capture and restore statements are
identical after substituting state-> with archive->registers. and ignoring
formatting. The CPL-zero preparation, CR0/CR2/CR3 order, all general registers,
EIP/flags, descriptor tables, TR, six hidden segment states and final CPL
restoration keep their order. Original selector-only setters are retained;
architectural c_setCS/c_setSS loaders are not substituted because they would
reload descriptor state. Snapshot schema and serialized bytes are unchanged.
The existing snapshot test captures non-default EAX/EIP/CR3, alters state,
restores the archive and verifies registers, memory, TLB/debug/FPU/device state.
No user snapshot or guest binary is rewritten.

The stdio-only isolated build uses identical source pathname and actual Ninja
compile arguments for each pair, and succeeds on both architectures:

- x86 objects both SHA-256
  7c4b65208f1c947035b067f6b0ac06369de44cde35821d17d78a0b068fc3ac4a.
- x64 objects both SHA-256
  ed5833096d755f1ee53a91daf0201825eeebd7293ab12e813bb764508de88891.

This proves only header-removal object equivalence, not the complete archive
restoration. P11 final-source cached x86/x64 builds and publication pass. Each
architecture passes 188 background and eight native integration tests, for
392/392 fresh passes; all six qualification commands exit zero. No exemption
is claimed for the whole change. Restoring original printf retains its original
mirror warning, within the owner exception; this is not a zero-warning mirror
build. No new shared-package or changed Compat declaration warning is observed.
No fresh manual DOS/Win3.1/Win95 session validation is claimed.

Ordered files 93-97 also complete comparison: c_reg.h, c_seg.c, c_stack.c and
c_stack.h have no in-scope difference; c_seg.h is entirely normalized-identical.
No edits are needed for them. File 98 c_tlb.c is next and not yet audited.

P11 coordinator actual-diff review confirms one archive owner, direct original
accessor calls, exact original non-excluded c_reg.c, no remaining removed API
callers in src/test, unchanged snapshot layout, and no user asset mutation.
Published artifact SHA-256:

- softpc32.exe: `438b8428418003da1cabee4ac83f09b5d58aed30a12d6cdf21daac97dfd44ce7`.

- softpc64.exe: `1a899975f5cd30742e2daafe790f37d0ae02f6b26b0185e759d3eb5961cecd77`.


## P12: file 98 TLB snapshot boundary

Fixed-XP comparison of c_tlb.c finds only the added snapshot include, two
snapshot functions and their following #line 124 outside previously excluded
bodies. The line directive preserves original subsequent diagnostics and is
counted as an actual local addition, not excluded-body bookkeeping.

| Point | Original/current location | Executor conclusion and counter-review |
| --- | --- | --- |
| R0135 | include block / snapshot.h | Original has no snapshot schema. Retain provisionally as the declaration for the private-state copy boundary; the coherent original-flush alternative fails on both architectures. Pending owner decision. |
| R0136 | after FAST_TLB arrays / capture_tlb | Remove redundant fixed-size argument and null/size guard. Remaining capture accesses original private arrays unavailable through original external accessors. Pending owner decision on the residual. |
| R0137 | same block / restore_tlb | Remove the same redundant argument/guard; retain actual copies and validity/next-set checks. A flush is not equivalent restoration. Pending owner decision on the residual. |
| R0138 | before original line 124 | Retain line-control for the added snapshot block; its removal would change subsequent diagnostic locations. Pending owner decision with that block. |

Counter-review distinguishes this file from c_reg.c: the original tlb,
next_set, page_index and page_address are LOCAL storage, with no original
full-state export/import accessors. test_tlb operates architectural test
registers and mutates cache state; it is not an equivalent snapshot API.
Moving the same implementation elsewhere or exposing the arrays would not
remove complexity. Original flush_tlb invalidates cached translations, causing
a subsequent lookup to observe page tables rather than saved cache contents.

The isolated comparison uses actual Ninja flags and the existing checkpoint
fixture/link path on each architecture. Control builds and runs pass. The
alternative removes the added mirror snapshot entries, omits archive capture
of TLB state and invokes original flush_tlb in place of archive TLB restore.
Both alternatives compile/link successfully, but fail the existing assertion
at checkpoint_smoke.c:552: lin2phy(0x400123u, 0) == 0x3123u. x86 exits
3221226505; x64 exits 3. The fixture changes the page-table entry after restore,
so original cached translation must survive. Source predicts a fresh lookup
would produce 0x4123; the probe did not print that actual value, so only the
failed old-mapping assertion is claimed as dynamic proof. These are SoftPC
results, not sibling-project evidence or manual guest reproduction.

The only production callers are archive.c capture/restore. Its allocator
creates exactly SOFTPC_CCPU_FAST_TLB_PAGE_COUNT bytes, matching original
NR_PAGES (1048576); allocation failure returns before capture, dispose clears
the archive, and stream reading uses the same allocation and fixed read size.
Restore rejects invalid archives and null page-index storage before calling
the bridge. Both old length arguments were the same literal constant, not
an external capacity. Removing that argument and duplicate guards leaves one
allocation owner and does not remove payload-value validation. Variable SAS
buffer sizes are different and are not changed.

Production diff is +9/-17 (net -8): mirror +4/-12, archive +2/-3,
snapshot declarations/comment +3/-2. Test code is unchanged. Relative to
fixed XP, c_tlb.c changes from +67/-0 to +59/-0: eight fewer difference lines,
no new mirror point. Counts normalize CRLF and omit only existing excluded
bodies/their line-control; the new snapshot block's #line 124 remains counted.
Snapshot serialization/layout and original TLB algorithms are unchanged.

Files 99 c_tlb.h and 101 c_tsksw.h are entirely normalized-identical to fixed
XP; file 100 c_tsksw.c has no in-scope difference. They require no edits.
P12 final-source cached x86/x64 builds/publication pass. Each architecture
passes 188 background plus eight native integration tests: 392/392 fresh
passes, all six qualification commands exit zero. Existing original-mirror
warnings remain permitted; this is not a zero-warning mirror build. No fresh
manual DOS/Win3.1/Win95 session validation or exhaustive TLB-state proof is
claimed. The existing checkpoint regression exercises the real original cache.

Coordinator actual-diff review confirms only the fixed-capacity private TLB
argument/checks are removed, original copying order and validation remain,
all real callers are updated, mirror CRLF is preserved, and no shared package,
guest, INI, media or snapshot is modified. Manifest and documentation gates
pass. This delivers the simplification without approving residual retention.
Ordered review is 101/472; file 102 c_xcptn.c is next.


P12 published artifact SHA-256:

- softpc32.exe: `73c16db7ed529ed4a0924a946dd6b92e1262faa9bebe99298307a5673b4ea5a7`.
- softpc64.exe: `77de7e0549332033cfb4ecd467a297fd146cb41f94c7a49f465d8292053934cc`.


## P13: file 102 original exception declarations and no-op call

File 102 c_xcptn.c has two in-scope differences. R0139 is the added yoda.h
include after fault.h; R0140 is the removed local host_getenv declaration in
check_exception_env. Both are restored to fixed XP. The selected content is
now exact original text; existing excluded bodies are untouched and uncounted.

Executor review follows real exception paths: benign/contributory/IDT/double
fault, divide and page fault all use the original environment check. With PROD,
the added yoda.h changes force_yoda calls into empty macros. Without that
addition, they call the existing original empty force_yoda in stubs.c, which
already belongs to the production CCPU object set. No new stub or host callback
is required, and exception register/control behavior is unchanged.

Counter-review specifically tests the removed declaration rather than assuming
its char-pointer spelling conflicts with modern getenv. Current selected
insignia.h expands IPT1 to (), not a typed parameter list. host_def.h maps
host_getenv to getenv and includes the CRT declaration. Restoring the original
local declaration compiles successfully on both architectures, and produces
byte-identical isolated objects against control with identical pathname/flags:

- x86 pair: bdad0939599e72cae0cbff8589e671068c290b482509921f553a61a6ac3da05a.
- x64 pair: 5cac435b118583f2dc7bafe6b9f7692b7a239d538b965cc0a844ed0944dc047a.

The no-yoda isolated variant also compiles on both architectures; its objects
change because original no-op calls return. This is not used to claim full
artifact equivalence or omit regression. No compiler failure proves either
historical deviation necessary. Tests of unsupported ANSI configurations are
not required to justify changing the actual selected original path.

Production is +1/-1 (net zero), tests unchanged. Fixed-XP in-scope difference
falls from +1/-1 to zero: two fewer lines and two restored points. No pending
retention decision remains for this file. Original CRLF, exception handlers,
monitor environment behavior and guest assets are preserved.

Ordered files 103-118 also complete comparison. c_xfer.c and c_xtrn.c have no
in-scope difference. c_xcptn.h, c_xfer.h, c_xtrn.h, daa.c/h, das.c/h, dec.c/h,
div.c/h, enter.c/h and evidfunc.h are entirely normalized-identical. No code
edits are made to them. File 119 evid_c.h is the next diff-bearing file and
requires caller/type investigation; reading its diff is not completed audit.
Final-source cached x86/x64 builds/publication and all six qualification
commands pass: 188 background and eight native tests per architecture,
392/392 fresh passes. Actual-diff coordinator review confirms exact selected
original content, existing empty stub linkage, no replacement implementation,
no shared-package change and no guest/media/configuration mutation. Manifest
and documentation gates pass. Original mirror warnings remain permitted;
no new manual guest session verification is claimed. Review is 118/472.


### Next-file investigation: evid_c.h (119), not yet restored

The six changed callback fields are pointer-to-IU32 versus pointer-to-IUH;
they are not embedded integer storage. Both forms have native pointer size
and alignment. Actual Ninja dependency records on x86/x64 identify the sole
compiled consumer as Compat cvidc/gdp_state.c. Its use is offsetof for these
fields and sizeof the aggregate, not dereference through their pointee types.
Source search confirms that it includes the CCPU header through the selected
include order; the separate C-VID header is not this file's consumer contract.

An isolated compile uses that real consumer, actual Ninja flags and the same
header-override pathname for current and exact XP contents. Both succeed and
objects are byte-identical per architecture:

- x86: c23f08bcb762237882f4178194abcf5ff5ec4ef75be756f3ab9b2f18b05d0e3a.
- x64: 21fc70faf79c3218fef6e637cc4d06a8c959101918e229cfcc8eb524b48789a2.

This disproves the historical comment's claimed need to change these pointer
pointee types just to store native-width addresses in this selected aggregate.
Candidate: restore the entire header to XP (+6/-10 production, mirror diff
-16 to zero), keeping existing wiring. No change is implemented in P13;
final-source rebuild/publication and artifact comparison still belong to the
next delivery. No previously excluded body or guest asset is involved.


P13 published artifact SHA-256:

- softpc32.exe: `461149983ad018d866e48a7e5f15faeca1276cef7c9de0177ab2bf762325d3ef`.
- softpc64.exe: `f5afec8d7b1351cbc5690f9664b2e2b2069cb999a5a11a1537bea2b7df26c000`.


## P14: file 119 original pointer declarations

The preceding isolated investigation is now implemented and final-source
verified. Restore evid_c.h entirely to fixed XP: R0141 restores the two string
read pointer declarations and removes their erroneous native-width comment;
R0142 restores the three marker pointer declarations; R0143 restores the byte
read pointer declaration. These are pointer pointee types, not 32-bit storage
for pointer values. The sole actual compiled consumer uses offsets/aggregate
size. No cast, helper, new header or alternative layout is introduced.

Executor review and separate actual-diff counter-review confirm exact original
header bytes after CRLF normalization, unchanged member order/offsets/native
pointer widths, and unchanged gdp_state consumer code. Do not generalize this
result to scalar pointer storage or generated-rule dereferences elsewhere.
The file's former SOFTPC-PORT-051 marker is removed with the restored content;
its current manifest disposition changes to XP_SUBSET_IDENTICAL. Historical
ledgers remain historical, rather than being rewritten as current XP counts.

Production +6/-10 (net -4), tests unchanged. Fixed-XP diff falls from +10/-6
to zero: 16 fewer difference lines, no added point. File 120 fault.h is also
normalized-identical to XP and needs no change. Ordered review is 120/472;
file 121 fpu.c is next and not yet audited.

Cached x86/x64 builds of the final source and publication both pass. Final
production gdp_state objects are byte-identical to P13, with the same hashes
recorded in the preceding isolated proof. Entire published EXEs have identical
lengths and differ by exactly eight bytes each. Every differing byte belongs
to COFF TimeDateStamp (PE+8), OptionalHeader CheckSum (PE+24+64), or the single
changed NUL-terminated compiler time literal at the same file offset:

- x86 offset 2435392: 01:16:35 to 01:24:49.
- x64 offset 2289664: 01:16:44 to 01:25:00.

All remaining bytes are identical. This proves the approved artifact-equivalence
exception; no fresh P14 runtime run is claimed. P13's fresh 392/392 full tests
remain the behavior regression baseline. Manifest initially rejected the stale
PRESERVED_LOCAL_PATCH identity after exact restoration; updating that row to
XP_SUBSET_IDENTICAL fixes the metadata, and the gate passes. No production
change is hidden behind that metadata correction. Documentation and diff checks
pass; user assets and shared packages are unchanged.

P14 published artifact SHA-256:

- softpc32.exe: `2a4bbd58c733e0cbbe4352b0aae15360b835f1826db0835575ef5220bc03f979`.
- softpc64.exe: `3203d4d1383ada135f180c8832246c7cfa7173315fa2163df0926d3650917d7b`.


## P15: file 121 FPU original-accessor restoration

The complete file comparison identifies these in-scope points; existing
excluded bodies and their line-control are not re-audited or counted.

| Point | Difference | Executor and counter-review disposition |
| --- | --- | --- |
| R0144 | added stdio.h | Restore original absence; actual isolated objects are identical with the original cast restored too. |
| R0145 | added snapshot.h | Remove after deleting the added mirror snapshot entry points. |
| R0146 | CVTFPHI64 representation copy | Residual necessity demonstrated independently on x86/x64: restoring pointer punning fails the existing exact large-integer instruction assertion. Pending owner decision, not automatic retention. |
| R0147 | FIST M64I rounding | Residual necessity independently demonstrated on x86/x64: original unrounded conversion fails 3.5 nearest-even storage. Pending owner decision. |
| R0148 | getNpxStackReg pointer-difference cast | Restore original expression; no ABI/behavior need shown, and isolated objects are identical. |
| R0149 | added delayed-NPX declarations | Remove from mirror with snapshot functions; actual archive consumer keeps exact typed declarations of the existing bridge. |
| R0150 | added capture_fpu | Delete: archive uses original register/stack accessors and original exported state directly. |
| R0151 | added restore_fpu | Delete: same original accessor path, without a replacement helper or snapshot-specific mirror API. |

Independent restoration probes use actual CCPU compiler flags and the actual
command-provider test link, with a single replacement fpu object preceding
the production archive. Current and declaration/cast-restored controls pass
on both architectures. Declaration/cast variants have identical objects:

- x86: 149ef6ce516600027dd3e622c09e5e3cf6157c9e11d78bc9898f3637a2345790.
- x64: e14265b866e23cb0b18627ae0946a99ad1ea04a0e6ca9c5c5791554a2f6ce520.

Restoring only original CVTFPHI64 representation reads compiles/links, then
fails command_provider_smoke.c:508, stored == 0x0000123400001234ull, at both
widths. Restoring only M64I original conversion compiles/links, then fails
line 693 with mode=0, input=3.5, width=8, first stored byte 00 instead of 04.
Both x86 negative processes exit 3221226505; both x64 negatives exit 3. Current
controls pass the existing 228 conversion cases and surrounding instruction
checks. No inference of complete x87 correctness follows from this finite set.

Counter-review rejects retaining snapshot wrappers just because they currently
have callers. Original getNpxStackRegs/setNpxStackRegs copy the complete native
FPSTACKENTRY array; get/setNpxControlReg and get/setNpxStatusReg already own
control, rounding and TOS updates. Other serialized flags/addresses are
original exported variables. FSAVE/FRSTOR are not substituted: they are guest
instructions with reset/conversion/exception effects. Existing archive capture
and restore call the direct state-copy accessors instead.

The archive includes the original native stack type, not a guessed layout or
new type alias. It maps serialized value bits/tags through one temporary native
stack array; restore first reads existing stack storage to preserve padding
outside those fields, matching the removed implementation. No snapshot schema,
serialized byte order, guest format or existing padding policy changes. It
introduces no new allocation, state owner, helper or execution route. Original
control/status setter order and delayed-NPX bridge remain unchanged.

The isolated accessor-only candidate compiles/links and passes both the real
command-provider and checkpoint tests at both widths. Final tests extend the
existing snapshot checkpoint with eight distinct FPU stack values, clear them,
restore the image, and verify all values/tags through original accessors.
This is actual archive/CCPU coverage, not a duplicate serializer test.

Files 122-137 are also compared in frozen order: in.c, into.c and intx.c have
no in-scope differences; the remaining files are normalized-identical. No
changes are made to them. File 138 iret.c is next and not yet fully audited.
Final cached x86/x64 builds/publication and all six full-qualification
commands pass: 188 background plus eight native tests per architecture,
392/392. Post-qualification counter-review removes two unnecessary T60
comments and restores the original conversion comment at its relevant call;
only formatting/comments change. The snapshot test then strengthens tag
coverage using alternating signed values with nonzero negative tags.
Both final builds and both revised checkpoint tests pass.

Final fpu objects are byte-identical to the fully qualified ones. Each entire
EXE has exactly eight differing bytes, exclusively within PE COFF timestamp,
OptionalHeader checksum and one compiler time literal; all other bytes and
file lengths are identical. This applies the artifact-equivalence rule to
that final nonsemantic refinement rather than claiming a second fresh full
runtime run. The changed test itself is freshly rerun on both architectures.
No manual DOS/Win3.1/Win95 session or exhaustive x87 conformance is claimed.

Production +71/-82 (net -11): archive +68/-2, removed declarations 0/-2,
mirror +3/-78. Test source +17/-0. Fixed-XP in-scope diff falls from +84/-6
(90 changed lines) to +8/-5 (13), a reduction of 77 mirror difference lines.
No new mirror point is added. The only residuals are the independently proven
representation and rounding repairs; their necessity evidence is ready for
owner decision, not silently recorded as approval.

Coordinator actual-diff review checks original accessor types, single archive
ownership, original setter order, unchanged serialization and padding policy,
no removed-entry references anywhere in src/test, preserved mirror CRLF and
unchanged shared packages/media/configuration. Original mirror warnings remain
within the approved exception; no changed Compat/test warning is observed.
Manifest/documentation gates and diff checks pass. Source review is 137/472.

Final artifact/object evidence:

- x86: final FPU object `073fb41b8b71331ed7692c69f312eb1723c9f1c997bcc46d436f64928cf7ad76`; EXE `9597f6ec038e388439dd168ff84e38346bcad4593b5714e2fcd14ec65da82959`; compiler time 01:33:27 to 01:41:19, at offset 2435392.

- x64: final FPU object `458b0ae88430995c70061aa88d2207b1238160fea6bd606b9dc1bd8d25e4c2ac`; EXE `9c74596b0f8d63b6c14ea2e6e8227372bdd641089e4b9f6ea99563a11eb0788c`; compiler time 01:33:42 to 01:41:38, at offset 2289664.


## P16 file 138: IRET original operand-width restoration

T100-R0152 is the sole in-scope iret.c difference: original XP lines 316-319
select SET_SP/SET_ESP by operand size; the local replacement selects by the
returned SS B bit through set_current_SP. Initial analysis considered the
previous CALL repair analogous. Counter-review rejects that assumption:
restoring a stack pointer from an IRET frame is not selecting a new CALL stack
from a TSS. The actual decoder dispatches IRET in the supported CCPU build;
there is no duplicate Compat implementation to substitute.

Intel SDM 325383-078US, December 2022, volume 2A, IRET operation,
RETURN-TO-OUTER-PRIVILEGE-LEVEL (3-541/3-542), explicitly restores the popped
32-bit value to ESP. Reference:
https://cdrdv2-public.intel.com/671110/325383-sdm-vol-2abcd.pdf
This does not depend on the returned SS B bit. The 2003 manual's omitted
SS/ESP restoration in its pseudocode is not used as contrary evidence.
The current manual and the original 386 presentation differ in detail around
16-bit returns; this audit does not claim real-hardware validation of upper
ESP bits after a 16-bit IRET.

Isolated actual-machine probes execute CF or 66 CF through softpc_machine_run,
with a real GDT and ring-0-to-ring-3 return frame. The incoming stack is 16-bit
with ESP=ABCD0900. The saved pointer is 12345678 for IRETD, 5678 for IRET.
Both x86 and x64 give the same results:

| Operand size | Returned SS size | Before restoration | Original XP |
| --- | --- | --- | --- |
| 16 | 16 | ABCD5678 | ABCD5678 |
| 16 | 32 | 00005678 | ABCD5678 |
| 32 | 16 | ABCD5678 | 12345678 |
| 32 | 32 | 12345678 | 12345678 |

Every case reaches CS=000B, SS=0013, EIP=00000600 and CPL=3. The initial
probe omitted the disposable floppy fixture and failed machine creation;
it supplied no CPU evidence and was corrected before the above comparisons.
The isolated build uses existing production compiler/link settings and a
single replacement IRET object, not a substitute CPU or modified guest binary.

Restore all four original lines, without a new helper or condition. The
32-bit/16-bit-stack case demonstrates a regression in the local replacement;
no supported necessity was established for retaining it. The original 16-bit
path is restored under the original-first rule rather than asserting exhaustive
hardware conformance. Existing V86, same-level and real-mode bodies are unchanged.

The existing machine checkpoint suite gains four real-instruction cases,
checking control transfer and low SP in every case, and the full saved ESP
for both IRETD cases. It deliberately does not freeze an unverified high-half
hardware expectation for 16-bit IRET. The same new test is also linked to the
pre-restoration object for a negative control: x86 exits 3221226505 and x64
exits 3, both at c_getESP() == 0x12345678u in the new test. This is not a BIOS/Windows
reproduction or an exhaustive protected-mode/VM86 instruction test.

Frozen files 139-181 are checked in order: jmp.c, lgdt.c, lidt.c, lldt.c,
lxs.c and mov.c have no in-scope differences; the rest are normalized-identical.
No changes are made to them. File 182 ntstubs.c is the next substantive review.
The historical CALL/IRET/RETF same-class claim does not constitute necessity
proof; RETF remains subject to its own later frozen-order audit.

Production +4/-1 (net +3) restores original source. Mirror in-scope diff falls
from +1/-4 to zero: five fewer difference lines and one fewer difference point.
Test source +42/-0; no shared package, Compat, ABI, snapshot or media change.
Final cached x86/x64 builds and publication pass. Both full suites pass:
188 background plus eight native desktop/audio tests per architecture, 392/392.
The exact final checkpoint test passes at both widths; the pre-restoration
negative controls fail at its intended ESP assertion. Documentation and mirror
manifest checks pass. No manual guest session or exhaustive hardware test is
claimed.

Coordinator actual-diff review confirms exact XP operand-width syntax, unchanged
surrounding IRET sequencing and line mapping, no new production path or owner,
preserved mirror CRLF and owned-test LF, and no changes to shared packages,
configuration or guest assets. Compiler warnings are confined to existing mirror
headers; no added test/owned-code warning is observed. Historical claims that
CALL, IRET and RETF required the same repair are superseded for this IRET point.

The original retained file remains required by the actual instruction decoder.
No additional mirror difference is introduced and no owner exception is sought.
Task scratch is removed after owned jobs terminate. Sequential review is 181/472;
ntstubs.c (182) remains unfinished, and earlier qualified residual decisions
remain pending. S1 is not closed.

- x86 EXE SHA-256 `7f799ae3d6f7c4bda7fdfd0024479e969608532058c8d54e58a5645fb42c6901`; Nov 13 2026 01:52:13.

- x64 EXE SHA-256 `7fad72fe61c90eaf3251a473de60cfc6488669f7209083115de24314107996e2`; Nov 13 2026 01:55:32.


## P17 file 182: ntstubs original include restoration

The actual softpc-ccpu-core target compiles ntstubs.c with CCPU/CPU_40_STYLE
and without PIG. Its Video object is populated by the existing Compat bootstrap;
its CPU/SAS bridge symbols remain required by the production archive. It cannot
be deleted as an unused file. This is not duplicate host device implementation.
All file differences outside skipped bodies were reviewed in source order:

| Point | Original/local difference | Analysis and counter-review |
| --- | --- | --- |
| R0153 | evidgen.h changed to ../cvidc/evidgen.h | Restore the original spelling: the existing target already searches base/cvidc, and no competing evidgen.h exists. No CMake change is needed. |
| R0154 | added stdio.h and separator | Restore original absence: both actual compilers accept the original builtin printf call and produce byte-identical objects. The recovered declaration warning is a mirror warning, not proof of broken supported behavior. |
| R0155 | Gdp definition changed to extern | Both original-restoration links fail with duplicate Gdp from ev_glue.c. Original ev_glue is the active CCPU/C-VID owner; introducing flags or another mirror change merely to relocate the difference is not a reduction. Residual pending owner decision. |
| R0156 | copyROM stub omitted | Restoring it duplicates the original rom.c definition at both widths. That file supplies real ROM discovery and cannot be omitted. Residual pending owner decision. |
| R0157 | initialise_npx stub omitted | Restoring it duplicates the original fpu.c definition at both widths; the original FPU implementation is required. Residual pending owner decision. |
| R0158 | npx_reset stub omitted | Same actual duplicate-symbol proof against original fpu.c. Residual pending owner decision. |

The three missing functions are no-ops in both original owners. This is not a
claim that the stubs' behavior is wrong: exactly one original definition must
be linked. They are not replaced by new Compat implementations. Moving their
omission to fpu.c/rom.c would add new mirror points, so that candidate is rejected
without requesting an exception. Removing two redundant gap lines in the
existing omission reduces its diff; existing conditional/line directives remain.

Actual isolated build/link tests use the production command lines and the
real checkpoint executable link. Independent Gdp restoration fails with one
duplicate-symbol diagnostic per architecture. Restoring the three functions
fails for all three named symbols on each architecture. Both links exit 1.
Current and original-include candidates compile/link and run checkpoint on
x86/x64. The first object hashes used distinct source filenames and therefore
were not equivalence evidence; repeating with the same ntstubs.c basename gives
byte-identical objects at each width. Final in-tree builds confirm that identity.

Final cached x86/x64 builds and publication pass. Both complete EXEs retain
exact lengths and differ from P16 only inside the COFF timestamp, PE checksum,
and one compiler time string at the same offset. Every other byte is identical.
The approved artifact-equivalence exception therefore reuses P16's 392/392
qualification; no fresh full runtime run is claimed. The focused isolated
checkpoint comparisons were run on both architectures in this batch.

Production +1/-5 (net -4); tests unchanged. Non-excluded XP-relative diff falls
from +6/-13 (19 lines) to +1/-12 (13 lines): six fewer difference lines, with
the two include/declaration points removed and no new point added. The remaining
extern and three omitted definitions have necessity evidence, not owner approval.
The original printf declaration warning is explicitly restored under the mirror
warning exception; shared/owned source warnings are not relaxed.

Coordinator review checks actual source diff, original active definition owners,
unchanged symbol signatures, supported include resolution, final object and EXE
identity, preserved CRLF and no guest/configuration/shared-package changes.
Manifest/documentation checks and build-residue cleanup accompany delivery.
Sequential source review is 182/472; ntthread.c (183) is next and not yet audited.

- x86: object `8ab6d6e33c2363ae92c940f2c72ebcce390cc42ef579684ecb3bb40776b7a20e`; EXE `1804d7c4c8afec0c6c8e53e499dd50a24d3d6ae27e99f600500607acf89c9931`; 9 metadata-only changed bytes; compiler time 01:52:13 to 02:05:58.

- x64: object `0fff0b1f2de64767e6d73e56b94311eae43eb4a671de358749a475c4880b6183`; EXE `c25162c4ae7fc41b6d017bde91d69dc43a0977d9a52bc6544a25fb711b5aaaa7`; 9 metadata-only changed bytes; compiler time 01:55:32 to 02:06:09.


## P18 file 183: original thread unwind reuse

ntthread.c is compiled in softpc-ccpu-core. The selected c_main simulate,
exception and unsimulate paths call its original TLS-frame functions through
SOFTPC_CCPU_TLS_SIMSTACK. Machine executor enter/leave uses the corresponding
original thread entry/exit, and Compat lifecycle invokes the added outer exit.
The file is live and has no replacement Compat simulation-stack owner.

| Point | Initial analysis | Counter-review/disposition |
| --- | --- | --- |
| R0159 | Allocation diagnostic %d became %zu, with a three-line note. | The size is the fixed ThreadSimBuf object, not an unbounded input. Failure injection into the real function reports 2564 bytes on x86 and 10256 on x64 with both formats, matching the allocator request. Restore exact original line; no broader portable-format claim. |
| R0160 | Added outer-exit implementation repeats in_C/longjmp logic. | Coherent original-only exit reaches the wrong host frame in the actual nested BIOS test. Retain outer-frame selection, but set level=1 and invoke original ccpu386Unsimulate, which owns decrement, in_C and longjmp. Remove duplicated mechanics and verbose local commentary. Residual necessity pending owner decision. |
| R0161 | BADID failure returns typed null rather than bare return. | Actual original-body injection returns 0x31 instead of a valid frame/null on both architectures. Keep explicit failure value, remove the long added explanation; residual pending owner decision. |
| R0162 | Missing-TLS failure returns typed null rather than bare return. | Actual original-body injection returns 0x39 on both architectures. Same limited failure-result evidence and disposition. |

The pointer results are isolated-helper evidence, not proof of safe application
failure recovery. c_main passes the returned frame directly to setjmp and does
not check null. Thus the retained null returns do not make TLS setup failure a
supported recoverable path; no such claim is made. No new failure handling or
lifecycle redesign is included. The native diagnostic format restoration is
bounded to the two actual Windows builds; it is not a generic C varargs rule.

Original-only exit comparison is coherent: remove the added outer function and
adapt its actual lifecycle caller to original c_cpu_unsimulate, then link the
real checkpoint executable with those two objects. Both compile/link, but x86
exits 3221226505 and x64 exits 3 at nested_bios, c_getEIP() == 0x604u. The nested
HLT stop incorrectly resumes the host BIOS frame instead of escaping it.
Unmodified and original-unwind-relay variants pass the same complete checkpoint
suite on both architectures. This covers actual CCPU recursion, snapshot timeout
cleanup, reentry and finite-budget paths, rather than a duplicate longjmp model.

The reduced outer function retains its existing BADID/absent-frame checks. On
its owning executor, choosing level 1 followed by the original decrement selects
frame 0; there is no callback, wait or cross-thread mutation between selection
and unwind. Original BOP/device returns continue to pop their ordinary current
frame. No signature, TLS schema, owner, new helper or alternate execution path
is introduced; the actual unwind implementation is now the original one.

The isolated failure harness replaces malloc only in the actual ntthread object,
uses real Windows TLS, and never changes guest or production allocator behavior.
Current and original-format diagnostics both match the fixed allocator request
on both widths. BADID and allocated-but-empty TLS exercise the two actual failure
branches separately. The results are not a low-memory desktop reproduction.

Files 184-193 were compared in frozen order: out.c has no in-scope difference;
the others are normalized-identical. File 194 popf.c remains the next review.
Production +4/-20 (net -16), tests unchanged. Fixed-XP diff drops from +41/-3
(44 lines) to +24/-2 (26), 18 fewer difference lines and one fewer point.
Existing #error exclusions receive no new accounting. Final-source cached builds and publication pass for x86/x64. Each width
passes all 188 background and 8 native desktop/audio tests: 392/392 total.
No manual guest session or broader TLS failure recovery is claimed.
Coordinator review confirms the actual source diff, original single unwind,
unchanged tests/interfaces, preserved mirror CRLF, manifest and documentation
gates, and no guest/configuration/shared-package changes. Build logs contain
no warning diagnostics in this incremental run.

- P18 softpc32.exe SHA-256: `44bdca5c28e44f1de6a501a2ca7b4324bf9f97a22e5effc18acd691786cff255`.

- P18 softpc64.exe SHA-256: `b4f2d57f707fb5b3afb2c6ad9a721615fa8b5b18ce47370c15bd4ec2c9326f29`.

## P19 file 194: restore original POPF declarations

popf.c is an actual softpc-ccpu-core input. POPF/POPFD retain their original
stack, flags and note_486_instruction calls. The latter is defined by original
c_main.c with a char pointer argument; no callback or alternative Compat path
is involved. The only differences are added stdio.h (R0163) and an added
extern declaration plus blank line (R0164). Initial rationale is modern
declaration completeness; counter-review finds no actual ABI difference: the
existing fixed-format sprintf call and pointer diagnostic call compile to
byte-identical objects after exact whole-file XP restoration on both supported
architectures. Original implicit declaration warnings are mirror diagnostics,
not evidence of incorrect execution or a relaxed shared-library warning rule.

Restore both additions without replacing them elsewhere. Production +0/-3,
tests unchanged; XP-relative +3/-0 becomes exactly +0/-0, eliminating two
points. Preserved CRLF is the repository mirror policy; normalized contents
match the fixed XP original and selected-subset hash. No added interface or
behavior is introduced. Files 195-205 compare normalized-identical in frozen
order. File 206 ret.c is next and has not yet completed semantic review.

Final cached x86/x64 builds/publication pass. Both entire EXEs have unchanged
length; every changed byte is within the PE timestamp/checksum or the single
compiler time literal at its unchanged offset. All other bytes are identical.
Under the owner artifact-equivalence exception, P18 392/392 qualification is
reused; no fresh runtime regression is claimed. Coordinator review inspected
the actual three deleted lines, original diagnostic signature, both object
identities, exact normalized XP restoration, final full EXE equivalence,
manifest/documentation gates and preserved CRLF. No shared code, guest,
configuration or test source changed.

- P19 softpc32.exe `f3ed3f718d26b22107b5781220d9043fe4e2276822ce11cb326b215e1106448e`; 7 metadata-only changed bytes; compiler time 02:13:10 to 02:22:40.

- P19 softpc64.exe `c72114528f2329d979483cacb41f17433236e1ccbf17300c9b453f697cb8898b`; 8 metadata-only changed bytes; compiler time 02:16:31 to 02:22:55.

- Final x86 popf object `b573573185adb0e88da23fee1e10d33db9b1f29b39a08bcfa99effdeca4043a0` matches the original-candidate object.

- Final x64 popf object `2c52185d0987683aaae21427b25c2d04fa7c35113e6cf23af5abf5ed25258558` matches the original-candidate object.

## P20 file 206: original RETF operand-size restoration

R0165 replaces original GET_OPERAND_SIZE/SET_SP/SET_ESP with set_current_SP.
The actual decoder calls RETF in softpc-ccpu-core; there is no replacement
Compat return implementation. Initial rationale analogized a TSS stack switch
to restoring a return frame. Counter-review separates the saved operand width
from the target stack address size: restoring a 32-bit saved ESP must not discard
its upper half just because the target SS.B is zero. The existing final
change_SP/byte_change_SP still owns parameter adjustment by stack address size.

Intel SDM 325383-078US, December 2022, RET RETURN-TO-OUTER-PRIVILEGE-LEVEL,
volume 2B pages 4-568 through 4-570, restores tempESP from the operand-width
frame and then assigns ESP; subsequent immediate adjustment uses stack address
size. Source: https://cdrdv2-public.intel.com/671110/325383-sdm-vol-2abcd.pdf .
The full official PDF was fetched and its RET section read, not inferred from
the already-audited IRET behavior. The 16-bit high-half hardware behavior is
not claimed from this test; original code is restored under original-first
policy rather than retaining an unproven target-stack-width replacement.

The existing checkpoint suite now runs IRET, RETF and RETF 4, each with two
operand widths and two target SS sizes. Actual CPU decoding transfers ring 0
to 3 through a real GDT and stack frame. Assertions check CS/SS/CPL/EIP, low
SP for 16-bit operands, and full saved ESP for 32-bit operands, plus the
immediate adjustment for RETF 4. It reuses the existing fixture, not a new
CPU or guest binary. Pre-restoration x86 exits 3221226505 and x64 exits 3 at
the full-ESP assertion: both builds compile/link but the current RETF loses
the saved upper half. These are instruction tests, not a Windows reproduction.

Restore the four original lines, preserving the whole surrounding original
path. Production +4/-1 (net +3); tests +16/-12 (net +4), by git diff --numstat.
XP in-scope diff +1/-4 becomes zero: five fewer lines and one fewer point,
with no new condition, helper or error policy. Existing #error bodies are not
accounted. Files 207-264 have no in-scope differences (normalized identical
except sgdt.c, shld.c, shrd.c and sidt.c); file 265 zfrsrvd.c is next.
Final-source cached x86/x64 builds and dual-EXE publication pass. Each
architecture passes 188 background plus 8 native desktop/audio tests: 392/392.
Both final checkpoint tests pass the 12 return configurations. No fresh manual
DOS/Windows session or exhaustive fault/16-bit-upper-half hardware coverage
is claimed. Coordinator review checked actual source/test diff, preserved
parameter-adjustment path, fixed XP identity outside existing exclusions,
CRLF, negative controls, final tests, manifests and documentation gates.
No shared-package, Compat, snapshot schema, user configuration or media changed.

- P20 softpc32.exe SHA-256 `532546145416597f2ffdfd3dc5e7d76f4c32cc2b398e26fa1a4eead04fdb23c2`.

- P20 softpc64.exe SHA-256 `4fdb4cda47e445bf88fbb03e08e96302b021b54a583aeec748c48bbba42fbab3`.

## P21 file 265: reserved FPU declarations and private snapshot state

zfrsrvd.c is compiled by softpc-ccpu-core and actual original c_main dispatches
ZFRSRVD with the integer x87 opcode index. Its original functions own address
decoding, environment loads and delayed exception delivery; Compat is not a
replacement implementation. Every non-excluded hunk was reviewed separately.

| Point | Initial analysis | Counter-review and disposition |
| --- | --- | --- |
| R0166 | Added c_addr.h supplies d_mem/limit_check declarations. | Parameters are existing integral values and pointers, with no float/default-promotion or pointer-return truncation. Default integral promotions preserve the small input values; address_add is a 32-bit result assigned to the existing 32-bit offset. Actual object differences are call preparation scheduling, not evidence of a broken ABI. Restore original absence, qualify actual executable. |
| R0167 | Added c_intr.h supplies do_intrupt declaration. | x64 object is identical; x86 disassembly differs only in ordering two writes of the same arguments before the call. No necessity established; restore. |
| R0168 | Added intx.h declaration. | Both objects byte-identical without it; restore. |
| R0169 | Added snapshot.h include. | Its archive types are not used by this file; both objects byte-identical without it; restore. |
| R0170 | Added FLDENV prototype and blank. | Both objects byte-identical without it; restore original absence. |
| R0171 | ZFRSRVD old-style definition rewritten to prototype form. | Actual unsigned opcode index and signatures remain compatible under the supported build; both objects identical after exact restoration. |
| R0172 | Added private delayed-NPX state accessors. | Existing NpxExceptionEIP is private and distinct from NpxFIP; coherent removal loses saved exception delivery across snapshot restore. Retention evidence pending owner decision. |

Declaration candidates were compiled independently under both production
command lines, then the combined original-declaration candidate was compiled.
Only c_addr.h at both widths and c_intr.h on x86 changed objects. This is not
a generic claim that undeclared functions are portable to every compiler; no
new compiler support or shared warning relaxation is introduced. No declaration
is moved to Compat or another mirror file to conceal an equal difference.

The snapshot regression extends existing verify_snapshot_archive by ten lines:
DoNpxException saves 11223344, current FIP then becomes 55667788, capture occurs;
a later exception saves 99AABBCC and its pending flag is cleared. Actual restore
recovers pending/current FIP; TakeNpxExceptionInt then restores 11223344. Both
architectures pass using the actual original functions and normal snapshot
image path. No private accessor is directly asserted by the test.

The negative is coherent: remove both added mirror accessors, omit their
archive getter/setter use, preserve ordinary pending-flag restoration through
the original global. Both variants compile/link the real checkpoint executable,
but x86 exits 3221226505 and x64 exits 3 at the delivered FIP assertion. Thus
the evidence is wrong subsequent exception IP, not merely undefined symbols.
Original TakeNpxExceptionInt mutates pending state and injects an interrupt;
using it to observe the private IP would change capture behavior. Creating a
second owner or altering the static original state adds mirror difference
points and is not a minimization candidate. The 17-line existing access remains
a qualified residual, not approved retention. No archive format changes.

Production +2/-7 (net -5); tests +10/-0. Non-excluded XP diff +24/-2 (26 lines)
becomes +17/-0: nine fewer lines and six removed points. Existing #error bodies
are skipped without bookkeeping. File 266 matches normalized XP; 267 com.c is
next. Final-source cached dual-width builds and publication pass. All 392 distinct
tests pass after the single x86 verifier retry documented below: each width
has 188 background and 8 native tests. Both actual final checkpoint suites
pass, including the added delayed-NPX snapshot case. No manual guest session
or exhaustive x87 opcode/fault validation is claimed. Coordinator review
inspected the actual production/test diff, fixed XP signature restoration,
individual declarations and ABI call sites, coherent negative controls,
final-source tests, preserved CRLF, manifests and documentation gates.
No user configuration/media, shared package or snapshot schema changed.

P21 qualification note: the first x86 background run passed 187 tests but
emulator.verifier-negative reached its existing 60-second budget (59.90 s
reported). After that CTest process ended, the same test passed alone in
23.48 s under the unchanged budget. No test source/configuration or compiler
settings changed for the retry. Native x86 passes 8/8. This is an observed
aggregate-timeout/retry result, not a proven scheduler root cause or a first-pass
392/392 claim. The original timeout result is retained in this evidence.

- P21 softpc32.exe SHA-256 `e5c60f60ae2c402eb3e6fbb3ebca0acd4e2eabd9032988d75facb9aa8c8b390a`.

- P21 softpc64.exe SHA-256 `ae2a1ba32a8407b09ae01a5d98de6eb0beaf0dc6a0b4091b86bfe4f18e5d09c1`.

## File 267 com.c: investigation in progress after P21

All in-scope differences were enumerated against fixed XP after masking existing
#error bodies from comparison; those bodies receive no ledger entries. Actual
code is the original UART controller, backed by Compat host_com_ioctl.
The latter receives lib_iptr and writes an int through the supplied address.
Windows x64 long is 32-bit; original casts therefore cannot carry the address
of a normal high-address local. Actual production-command comparisons compile
and link the existing serial_smoke executable with current/original-cast com
objects, not a replacement controller or a changed guest.

| Architecture/candidate | Actual serial test result |
| --- | --- |
| x86 current | Pass |
| x86 all three original long casts | Pass |
| x64 current | Pass |
| x64 all three original long casts | Access violation, exit 3221225477 |
| x64 only com_flush_input cast restored | Access violation, exit 3221225477 |
| x64 only com_inb follow-up-input cast restored | Pass; current test does not establish branch coverage |
| x64 only modem_change cast restored | Access violation, exit 3221225477 |

The middle call requires a previously data-ready RX byte and non-loopback
read. Source tracing identifies that precise condition; no dynamic claim is
yet made for it. Compat receive storage starts empty and has no live native
serial receive thread, so its availability must not be fabricated as an observed
normal product input. Existing snapshot restore can carry receive data.

Other candidates: duplicate recv_char/do_wait_on_send declarations; whitespace
and directive-comment changes; copied copyright notice; snapshot include and
controller/callback serialization. Snapshot state is private original UART
registers, interrupt reasons, pacing and callback identity; existing snapshot
tests exercise controller capture/restore, but that alone is not a coherent
original-restoration negative. Complete state/callback necessity and exact
nonsemantic restoration are still under investigation. No file-267 production
change has been made and file 267 is not counted as audited or delivered.

File 267 RX follow-up evidence: extend the existing checkpoint device-archive
fixture using its existing serial host state. Finish original first-read flush,
restore one byte (51h) into the finite host RX state, invoke original
com_recv_char, then read COM1 data via inb. This reaches data-ready/non-loopback
follow-up input query without inventing a live external serial driver. Current
checkpoint passes both x86/x64. Linking the same test with only that second
com.c cast restored to long causes x64 exit 3221225477. Together with the
previous independent flush/modem tests, all three pointer casts have direct
x64 negative evidence. This is a restored-state/control-path test, not proof
that today's virtual endpoint receives live physical serial traffic.

The changed checkpoint test is pending complete-file qualification and has not
been delivered. The UART snapshot/callback and original formatting review
remains active; no P22 or file-267 completion is claimed.

## P22 file 267: original UART formatting and duplicate declarations

Completed-file review uses fixed XP SP1, the actual original UART compilation
and callers in Compat serial, the original event queue and device archives.
The excluded bodies remain untouched and receive no accounting. Existing
#line directives are byte-for-byte unchanged. The remaining selected differences
are the relocated historical notice, snapshot header/state/callback extension,
and three pointer-width corrections. No Compat substitute or new path is added.

Initial analysis considered removing the snapshot extension. Counter-review:
UART registers, interrupt causes, critical counters, pacing and flush mask are
private controller state. Original reads consume RX/status/interrupt state;
replaying writes invokes host output and scheduling, so register replay cannot
replace direct restoration without different behavior. Existing device archive
regression mutates/restores these actual fields. The added receive regression
restores finite host RX bytes 51h/52h, invokes original com_recv_char, reads 51h
through inb, and captures the remaining original queued receive callback.
Both widths pass. A coherent archive variant omitting serial callback encoding
and decoding rejects that actual capture (x86 3221226505, x64 3, assertion at
pending archive capture). This is a real controller/queue/archive negative,
not undefined-symbol evidence. No live external serial endpoint is claimed.
The initial isolated archive compilation used the wrong quoted snapshot.h;
that probe was discarded, original quote-search ordering restored, and both
negative controls were then built and executed successfully.

Three retained pointer conversions have independent x64 access-violation
controls, exit 3221225477: flush input, data-ready read follow-up and modem
query. x86 original-cast/current checks pass. These and the snapshot residual
are evidence-qualified existing differences pending owner decision, not newly
approved additions. The historical notice remains under source policy.

Remove duplicate recv_char/do_wait_on_send declarations and the added long
explanatory comment; exact XP formatting/directive suffixes are restored.
Before/after objects compiled from the actual production commands are byte
identical on both architectures. Both directive lists (#line and #error) are
unchanged; no claim of reauditing excluded bodies is made. Exact original
whitespace restoration intentionally brings back one tab-only line at local
2172, reported by git diff --check; it is not stripped again to create a new
XP formatting difference. Original #endif NTVDM suffixes also remain original.

Selected-scope SequenceMatcher accounting (autojunk=False, #line omitted,
existing excluded bodies masked symmetrically): +156/-19 in 33 differing blocks
becomes +126/-3 in 6 blocks: XP diff decreases 46 lines and 27 points.
Tracked production is +18/-32 (net -14); checkpoint test +19/-0.
Qualification and delivery are pending; no full-pass claim yet.

P22 complete selected-point ledger. Ranges are physical lines in pinned XP
and pre-P22 local source; insertion/deletion anchors are explicit. These are
non-excluded differences only. Counter-review is the final column.

| Point | XP range | Before range | Initial analysis | Counter-review/disposition |
| --- | --- | --- | --- | --- |
| R0173 | before 15 | 15-15 | Historical notice relocated | Retain existing notice under source policy; no source-ownership change. |
| R0174 | before 58 | 57-57 | Added snapshot declaration dependency | Required by the private state extension below; evidence-qualified pending decision. |
| R0175 | before 71 | 73-73 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0176 | before 88 | 91-91 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0177 | 126-126 | before 97 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0178 | before 290 | 255-380 | Private UART snapshot/callback extension | Actual controller archive plus queued-callback negative; strip added long comment, retain minimal existing extension pending decision. |
| R0179 | before 307 | 399-401 | Duplicate callback prototypes | Restore original absence; both final objects identical. |
| R0180 | before 680 | 644-644 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0181 | 698-698 | 663-663 | Pointer widened from original long | Independent actual x64 access violation on original cast; pending decision. |
| R0182 | 752-752 | 707-710 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0183 | 754-754 | before 713 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0184 | before 1155 | 777-777 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0185 | 1180-1180 | before 783 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0186 | 1200-1200 | 790-790 | Pointer widened from original long | Independent actual x64 access violation on original cast; pending decision. |
| R0187 | 1313-1322 | before 866 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0188 | before 1325 | 869-871 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0189 | 1538-1538 | 977-977 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0190 | 1548-1550 | before 988 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0191 | before 1553 | 991-993 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0192 | 1743-1743 | 1128-1128 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0193 | before 1768 | 1143-1143 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0194 | 1769-1769 | before 1146 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0195 | 1932-1932 | 1251-1251 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0196 | before 2060 | 1269-1269 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0197 | 2116-2116 | before 1275 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0198 | before 2313 | 1350-1350 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0199 | 2353-2353 | 1376-1376 | Pointer widened from original long | Independent actual x64 access violation on original cast; pending decision. |
| R0200 | before 2763 | 1641-1641 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0201 | before 2798 | 1671-1671 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0202 | 2799-2799 | before 1674 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0203 | before 2837 | 1700-1700 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0204 | before 3550 | 2180-2181 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |
| R0205 | 3554-3555 | before 2187 | Whitespace or directive suffix differs | Restore exact XP; both final objects identical. |

P22 final-source qualification: x86 and x64 cached builds/publication pass;
each passes 188 background and 8 native desktop/audio/package tests (392 total).
Mirror/documentation gates pass. No retry was required this batch. Build warnings
are confined to original mirror sources, including two restored XP #endif
suffix warnings; no shared/Compat/test warning was introduced. No manual
DOS/Windows guest or physical serial endpoint acceptance is claimed.

Coordinator actual-diff review confirms the existing single UART/state owner,
no new production branch/API, private callback lifetime unchanged, unchanged
#error/#line directives, CRLF mirror and LF owned source/docs. The original
whitespace exception to diff --check is identified above. No unrelated file,
configuration, media, guest, snapshot or shared-package change is included.
Ordered reviewed files now 267/472; next is base/comms/printer.c (268).
Evidence-qualified existing residual decisions remain open; S1/T100 not closed.

- P22 softpc32.exe SHA-256 `dd201db674a275cff565549fade15f8a8e79944794037d44db1ef5988cf9fd17`.

- P22 softpc64.exe SHA-256 `780d2da88e82747241eb07c46431c403ebd9d64345c4288f4cdd5268eabf57e8`.

## File 268 printer.c investigation after P22

Actual root CMakeLists.txt includes base/comms/printer.c. The supported
compiled body owns original output/control/status registers, READY/OUT/OUTA
state and delayed ACK event handles. Compat parallel.c owns the host sink and
buffers; it does not replace these controller transitions. Original port reads
can advance ACK state and writes can schedule output, so replay is not a neutral
replacement for private controller state restoration.

Complete selected diff enumeration against fixed XP finds the relocated
historical notice, snapshot include and controller/callback extension, plus
whitespace and directive-suffix changes. Existing #error bodies are skipped
without records. Nonsemantic restoration/minimization remains unfinished;
production printer.c is unchanged and file 268 is not counted completed.

The existing real checkpoint test emits 33h through actual LPT I/O, creates a
delayed original ACK, captures the device archive, resets the event queue and
restores/dispatches it. Coherent removal of parallel callback encode/decode
from archive dispatch makes capture fail on both widths (x86 3221226505,
x64 3). Current passes. However omission of controller restore originally
still passed: this test had not changed its controller state after capture.

Add three test lines: write 00h to LPT data after capture; after archive restore,
read the actual data port and assert 33h. Current x86/x64 both pass. With only
archive controller restoration omitted, both now fail precisely at that data
assertion (x86 3221226505, x64 3). The same updated test still rejects omitted
callback mapping at capture. Tests use actual original port/queue/archive code,
not a replacement printer or fabricated callback. This establishes a real
snapshot-state contract, not physical printer or manual DOS/Windows evidence.
No production/public API, guest, configuration, media or snapshot change made.
The three-line regression remains local pending complete-file qualification.

## P23 printer.c complete-file candidate

Before selected XP diff +100/-21 in 32 points; candidate +74/-0 in
3 points, a reduction of 47 diff lines and 29 points. Both actual production
objects are byte-identical before/after. All #line/#error directives remain
byte-identical; original historical notice is preserved. The added long
explanation is removed, original whitespace/directive suffixes restored.
No new production path or change to guest behavior. Qualification pending.

Each non-excluded point below records original/pre-P23 ranges and counter-review
disposition. Initial hypotheses and actual archive controls appear above.

| Point | XP range | Before range | Counter-review/disposition |
| --- | --- | --- | --- |
| R0206 | before 69 | 69-69 | Existing historical notice retained under source policy. |
| R0207 | before 109 | 106-106 | Private state extension declaration; residual pending owner decision. |
| R0208 | before 199 | 182-182 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0209 | 208-208 | before 191 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0210 | before 212 | 194-194 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0211 | 216-216 | before 200 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0212 | before 246 | 229-305 | Actual archive negatives prove controller/callback necessity; remove long comment, remaining extension pending owner decision. |
| R0213 | before 264 | 324-324 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0214 | 287-287 | before 332 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0215 | before 296 | 340-340 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0216 | 297-297 | before 343 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0217 | before 323 | 371-371 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0218 | 350-350 | before 379 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0219 | 368-368 | 398-398 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0220 | 370-370 | 401-401 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0221 | 394-394 | 428-428 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0222 | 397-397 | 432-432 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0223 | before 410 | 443-443 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0224 | 419-419 | before 449 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0225 | 481-481 | 515-515 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0226 | 483-483 | 518-518 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0227 | 563-563 | 568-568 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0228 | before 646 | 618-618 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0229 | 659-659 | before 627 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0230 | 719-719 | 682-682 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0231 | 721-721 | 685-685 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0232 | before 741 | 706-706 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0233 | 752-752 | before 715 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0234 | 780-780 | 745-745 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0235 | 782-782 | 748-748 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0236 | 793-793 | 761-761 | Restore exact XP whitespace/directive suffix; both objects identical. |
| R0237 | 795-795 | 764-764 | Restore exact XP whitespace/directive suffix; both objects identical. |

P23 final-source verification: both cached x86/x64 build/publication pass;
the changed actual checkpoint test passes at both widths. Mirror and final
packet/documentation gates pass. Both final EXEs have identical length and
all bytes equal to P22 outside PE COFF time, PE checksum and exactly one
same-offset HH:MM:SS literal. Each EXE changes eight bytes total. Thus the
owner's artifact-equivalence exception applies: P22's 392 passing full tests
remain the full-regression baseline; P23 does not claim a fresh full-suite run.

Coordinator actual-diff review confirms production +21/-26 (net -5), test
+3/-0; original controller and Compat host retain distinct ownership, no
new branch/API/callback/state and no mirror complexity moved into Compat.
Original declaration order required for callback addressing stays. Existing
snapshot extension and declaration remain evidence-qualified pending owner;
original notice remains protected. CRLF mirror, LF own files, manifest and
diff --check pass. Excluded bodies and all #line directives remain untouched.
No guest/configuration/media/snapshot/shared-component modification.
Sequential reviewed files: 268/472; next base/comms/printer_.c (269).

- P23 x86 EXE SHA-256 `9a0356dff13c94bcbcc09c880130f2e0fa3725d5581753d7231105612a2c0397`; literal 03:02:48 -> 03:17:51, offset 2435904.

- P23 x64 EXE SHA-256 `3f03d1d26534d98da4e2699aa1b0257788672eb7f2aa05417368cb4e2fe7dc88`; literal 03:06:13 -> 03:18:05, offset 2289664.

## P24 files 269-271: original printer BIOS declarations and equality checks

File 269 printer_.c remains production BIOS code: root CMake includes it and
Compat device_bop installs printer_io in BIOS[BIOS_PRINTER_IO]. Its original
host_lpt_dos_open/close external declarations were removed despite matching
existing Compat definitions. Restore them, together with original whitespace.
Declarations do not create a call or enable an excluded NTVDM branch; no
removal/rewiring of other files is justified. Actual compiler objects before/
after are byte-identical on x86 and x64. Historical notice stays under policy.
All existing #line/#error lines remain byte-identical; excluded bodies skipped.
Selected XP diff +3/-6 in six points becomes +1/-0 in one point: eight fewer
diff lines and five fewer points. No new implementation or test added.

| Point | XP range | Before range | Counter-review/disposition |
| --- | --- | --- | --- |
| R0238 | before 25 | 27-27 | Preserve existing historical notice under source policy. |
| R0239 | before 66 | 67-68 | Restore exact original whitespace; no behavior difference, identical objects at both widths. |
| R0240 | 69-69 | before 74 | Restore exact original whitespace; no behavior difference, identical objects at both widths. |
| R0241 | 73-73 | before 78 | Restore exact original whitespace; no behavior difference, identical objects at both widths. |
| R0242 | 202-204 | before 194 | Restore original external declarations. They match existing Compat definitions and introduce no reference/call; identical objects at both widths. |
| R0243 | 227-227 | before 198 | Restore exact original whitespace; no behavior difference, identical objects at both widths. |

R0244: file 270 rs232_io.c has only its relocated original historical notice
outside skipped bodies/line controls. Retain existing notice under source
policy. Original BIOS implementation is compiled by root CMake and registered
at BIOS[BIOS_RS232_IO]; no implementation difference is present.

File 271 evidfunc.c is equal to fixed XP after line-ending normalization
and selected-scope comparison. Root CMake compiles this original C-VID table.
No changed point or excluded-body entry is invented. Subsequent evidfunc.h /
evidgen.h were previewed solely to identify the next ABI investigation; they
are not counted audited. File 274 preview equality is also not advanced past
those intervening unfinished files. Qualification/delivery pending.

P24 final-source cached dual build/publication, actual BOP smoke and mirror/
documentation gates pass. Actual diff review confirms production +6/-2 (net
+4) restores original text; no test changes. This reduces XP diff, rather than
optimizing local line count. CRLF, manifest and diff --check pass. No change
to shared components, guest/configuration/media/snapshot or Compat behavior.
Both EXEs match P23 byte-for-byte except COFF timestamp, PE checksum and one
same-offset HH:MM:SS literal (x86 nine changed bytes, x64 eight). Approved
artifact-equivalence exception reuses P22's 392 full passes through P23;
P24 claims fresh BOP tests, not a fresh full suite. Ordered review is 271/472.
Next is evidfunc.h (272), whose generated-rule ABI needs actual investigation.

- P24 x86 EXE SHA-256 `223a91e6f04ab23e4c56a3ded0e9174e3f4a2fbbacc0bef5ef4d55baf73aab4a`; compiler time 03:21:48.

- P24 x64 EXE SHA-256 `89806253b1c4c53d5a464c70257fac2f4f0337ed6b86b2ff1e5e2d3f76046f04`; compiler time 03:21:58.

## P25 file 272 evidfunc.h: restore original mark declarations

R0245, XP lines 641-657 / pre-P25 local 641-659: original seventeen default-int
mark declarations (including four repeated declarations) were replaced by a
six-line explanation and thirteen explicit IUH/four-argument declarations.
Initial claim was generated-rule ABI necessity (MVDM-HOST-DIV-121).
Counter-review rejects that claim as evidence for this particular edit.

Real compiled consumer evidfunc.c includes its adjacent cvidc/evidfunc.h and
initializes original EVID_MARK_POINTERS globals with function addresses. It
contains no mark calls or return conversions. ga_mark.h still supplies the
same original table field contracts. Generated sevid001.c defines the actual
IUH rules independently. ev_glue.c uses angle-bracket evidfunc.h and the actual
include search chooses base/ccpu386/evidfunc.h, not this adjacent C-VID header.
Whole production/test/tool caller scan found no additional direct consumer of
these changed declarations. Merely matching generated definitions therefore
does not establish that this local declaration rewrite fixes actual dispatch.

Coherent original restoration in the actual evidfunc.c compilation succeeds
at both widths. Isolated copies preserve the original source/header filenames
and use actual production flags/include paths. Before/after object bytes are
identical on x86 and x64. Original incompatible-pointer initializer warnings
are reported; they are original mirror diagnostics, not a new shared/test
warning or an assertion of universal compiler portability. No table cast,
helper, Compat adaptation or warning-suppression option is added.

Restore the complete header exactly to fixed XP (CRLF repository policy),
including original duplicate declarations. The current normalized hash equals
the pinned subset hash. XP diff +19/-17 becomes zero: 36 lines/one point removed.
Production +17/-19 (net -2); no test changes. Retire this current local patch;
historical DIV-121 records remain historical and are superseded by this result.
Qualification is running; full-file delivery is not yet claimed.

P25 final-source verification: cached dual build/publication and actual VGA
frame regression pass on both architectures; mirror and documentation gates
pass. Final EXEs equal P24 byte-for-byte except PE COFF time/checksum and one
same-offset compiler time literal (eight changed x86 bytes, six x64 bytes).
Approved equivalence exception reuses P22's 392 full passes through P23/P24;
no new full-suite or manual guest validation is claimed. Actual VGA test is
fresh final-source evidence, not a substitute for the equivalence proof.

Coordinator review checked the actual original header diff, real table/definition/
include consumers, byte-identical production objects, final binary equivalence,
CRLF/provenance and diff --check. No new test-only production mechanism,
Compat rearrangement, guest/configuration/media/snapshot or shared change.
Ordered review reaches 272/472; next base/cvidc/evidgen.h (273).

- P25 x86 EXE SHA-256 `f9c3ce422eb46a1a98cbdc1d42cf470355dd444a936a42d30d45b0feb99f19fd`; compiler time 03:26:02.

- P25 x64 EXE SHA-256 `c53bfb696fb81908d8ead25e5cee73ebb289742ddb97f21fed23f9a73a12704f`; compiler time 03:26:18.

## P26 file 273 evidgen.h: original vector contract investigation/restoration

The local header replaced eight IHP callback-carrying fields with three added
callback aliases, added a long rationale and two direct read/write-pointer
function declarations. No consumer uses the aliases outside this header.
IHP is already a native pointer, so its historical spelling alone is not
proof of 32-bit truncation. The generated getter/setter transport remains
native-width. Do not confuse this change with prior proven GDP slot defects.

Actual consumers include adjacent vglfunc.c/sascdef.c, ntstubs.c, facade.c
and egacpu.h users. Original egacpu.h maps SetReadPointers/SetWritePointers
to direct names; actual calls pass only readset 0-3 or no arguments and do
not consume returns. Direct declarations therefore need actual compiler/ABI
proof, not the assertion that every modern declaration is necessary.

Ninja's actual dependency database identifies 28 existing objects per
architecture depending on this header. Their before hashes were captured;
the complete exact-XP header, not a partial broken rewrite, is used for final
cached rebuild. No Compat casts, typedef moves or alternate header were added.
XP diff +21/-8 in six points becomes zero (29 lines/six points removed).
Production +8/-21, no tests changed. Current manifest equals pinned subset.

| Point | XP range | Before range | Initial analysis/counter-review |
| --- | --- | --- | --- |
| R0246 | 6-5 | 6-15 | Remove added comment and callback aliases together with their field uses; no independent consumers exist. |
| R0247 | 27-29 | 37-39 | Restore original IHP fields: pointer-width representation is unchanged, actual dependent-object/EXE proof required. |
| R0248 | 39-39 | 49-49 | Restore original IHP fields: pointer-width representation is unchanged, actual dependent-object/EXE proof required. |
| R0249 | 66-68 | 76-78 | Restore original IHP fields: pointer-width representation is unchanged, actual dependent-object/EXE proof required. |
| R0250 | 78-78 | 88-88 | Restore original IHP fields: pointer-width representation is unchanged, actual dependent-object/EXE proof required. |
| R0251 | 92-91 | 102-104 | Restore original absence of direct declarations; real calls have no pointer arguments/returns and bounded readset values 0-3. Verify actual generated code, not hypothetical ABI need. |

The next file 274 evid_c.h is normalized identical to pinned XP; no local
point exists. Its ordered completion is recorded only after file 273 is
verified. Build/EXE comparison is underway; this record alone is not a pass.

P26 final-source verification: both cached builds and publication pass. All
28 actual dependent objects at each architecture retain identical hashes;
both EXEs equal P25 byte-for-byte except COFF timestamp, PE checksum and one
same-offset HH:MM:SS literal (seven changed x86 bytes, six x64 bytes).
Actual VGA frame regression passes on both architectures. Mirror/documentation
and diff --check pass. Build warnings remain confined to original mirror
sources; no new shared/Compat/test warning or suppression introduced.

Approved artifact-equivalence exception reuses P22's 392 full-regression passes
through subsequent proven-equivalent deliveries; P26 does not claim a fresh
full suite or manual guest validation. No universal function/data-pointer
interchangeability claim is made beyond the actual supported Windows x86/x64
build/ABI and unchanged generated code. The old DIV-062 explanation is retired
as a current necessity claim; historical records remain historical.

Coordinator actual-change review confirms exact XP header restoration, all six
points removed with no moved aliases/casts or additional production paths.
Production +8/-21, no test changes. CRLF source and manifest match pinned XP.
No shared package, Compat source, guest, configuration, media or snapshot edit.
Sequential review reaches 274/472; next base/cvidc/ev_glue.c (275).

- P26 x86 EXE SHA-256 `fe2ac6d03f68eed7ad41d3b5a1635e2c7f1b3c6705dbfa171e9762447716bc1e`; compiler time 03:29:45.

- P26 x64 EXE SHA-256 `e3d0c0aebec5c6d8542c2eaa866afd1ed87659f5a0ac6e0949baec9349125d4f`; compiler time 03:30:16.

Actual dependency object paths (same set on both widths; all unchanged):

- `CMakeFiles/softpc-ccpu-core.dir/src/app-softpc/softpc.new/base/ccpu386/ntstubs.c.obj`
- `CMakeFiles/softpc-ccpu-core.dir/src/app-softpc/compat/ccpu/facade.c.obj`
- `CMakeFiles/softpc-ccpu-core.dir/src/app-softpc/softpc.new/base/cvidc/ev_glue.c.obj`
- `CMakeFiles/softpc-ccpu-core.dir/src/app-softpc/softpc.new/base/cvidc/vglfunc.c.obj`
- `CMakeFiles/softpc-ccpu-core.dir/src/app-softpc/softpc.new/base/cvidc/sascdef.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/ega_mode.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/ega_prts.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/ega_vide.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/gvi.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/gfx_updt.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/v7_ports.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/video.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/v7_video.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/vga_mode.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/vga_prts.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/ega_read.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/ega_dmmy.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/vga_vide.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/ega_writ.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/egawrtm0.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/egwrtm12.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/base/video/cga.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/host/src/nt_ega.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/host/src/nt_graph.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/softpc.new/host/src/nt_vga.c.obj`
- `CMakeFiles/softpc-machine-devices.dir/src/app-softpc/compat/dib_surface.c.obj`
- `CMakeFiles/softpc-vga-frame-smoke.dir/test/app-softpc/unit/machine/chain2_rule_smoke.c.obj`
- `CMakeFiles/softpc-vga-frame-smoke.dir/test/app-softpc/unit/machine/vga_frame_smoke.c.obj`

## File 275 ev_glue.c investigation after P26

Enumerated current/fixed-XP differences: stdio/inttypes includes, three header
selection changes, GDP slot include/allocation, two diagnostic formats,
CHAIN2 range bridge/selection, two BIOS writer parameter changes. Existing
#error body is skipped without an entry. No production edit has been made.

Actual command-line isolated compilation and actual VGA test link provide:

| Candidate | x86 | x64 |
| --- | --- | --- |
| Current | Compile and actual VGA pass | Compile and actual VGA pass |
| Remove added stdio | Object identical | Object identical |
| Restore quoted evidfunc.h | Object identical | Object identical |
| Restore quoted Evid_c.h | Object identical | Compiles, object differs |
| Restore quoted j_c_lang.h | Object identical | Object identical |
| Restore original BIOS byte parameter types | Compiles, object differs | Compiles, object differs |
| Restore original BIOS word parameter types | Compiles, object differs | Compiles, object differs |
| Restore original diagnostic formats/remove inttypes | Compiles, object differs | Compiles, object differs |
| Remove CHAIN2 bridge; case 4 selects existing Unchained functions directly | Actual VGA passes | Actual VGA passes |

The first isolated link attempt assumed a separate CCPU archive which the
actual link does not contain; it terminated before testing. Corrected probe
uses the actual combined libsoftpc-machine.a and overrides only ev_glue's
object. Current and direct-marker executions above are the corrected actual
results, not the abandoned link attempt. Each source retains original local
quoted-header search paths, and no test/guest source is altered.

These results are not yet complete necessity conclusions. In particular the
CHAIN2 bridge cannot be retained just because it was previously accepted:
its replacement by existing Unchained markers passes both actual VGA suites.
Need inspect exact dirty footprint, odd/even offsets, zero/edge counts and
bank bounds before deciding removal; tests may allow excess marking. Real
producer gfx_updt selects case 4 for CHAIN2. If removal is justified, coupling
that selection to original case 2 must be assessed against original XP diff,
not leave an unnecessary alias path merely to preserve current tests.

GDP include/create are a separate representation issue: original direct
SubrRing pointer slots at offsets 8/12/16 are four bytes apart, whereas pointers
are eight bytes on x64. Simply restoring malloc while retaining slot APIs is
not a coherent original control. Original setup_global_data_ptr and
setup_vga_globals need a paired raw-layout comparison; this remains unfinished.
No claim of dynamic GDP failure is made in this investigation record.

BIOS original narrow argument types can change truncation relative to the
current ULONG callback contract; inspect actual caller values before deciding.
Quoted Evid_c.h x64 differences also require semantic review, not automatic
retention based solely on changed object bytes. File 275 remains under audit,
reviewed count 274/472; no P27, publication or full-file completion claimed.

### File 275 extended controls (no production edit)

The paired GDP control uses the actual ev_glue setup_global_data_ptr and
setup_vga_globals bodies. Its original variant restores both host_malloc(64 KiB)
and original direct GDP macros together, not a malloc/slot-API mismatch. It
checks all three ring pointers against the actual SubrRingBuffer. x86 current
and original both pass (1/1/1); x64 current passes, original returns 1 with
ptr/low/high = 0/0/1. Original offsets 8/12/16 overlap eight-byte pointers.
This proves the narrow initialization ABI failure, not guest execution.

Extended actual VGA executable controls independently restore the BIOS byte
and word parameter declarations, printf formats and quoted Evid_c header.
Byte, word and printf restorations pass on both architectures. Quoted Evid_c
passes x86 but x64 exits 3221225477. The two headers differ in the native-width
video-base member; the GDP adapter and other consumers use the CCPU aggregate.
Counter-review: this single-header failure is not yet proof against every
coherent aggregate restoration; inspect that boundary before qualifying it.

Source counter-review of direct Unchained marking found that its string marker
marks through (offset+count)>>2 inclusively, whereas the bridge rounds the
last included address. Zero count also writes a bitmap cell although total
remains zero. Existing passing tests check total for zero count and permit
excess high bounds, so they do not prove identical dirty footprints. These
findings require real caller/capacity analysis, not an automatic retention
claim. Actual host allocation is 0x20000 bytes; the source scan found it in
host/src/nt_graph.c. File 275 remains unfinished at sequential count 274/472.

## File 275 P27 restoration and counter-review (verification running)

Before: ev_glue.c in-scope XP diff +53/-8 = 61 lines, 11 contiguous
SequenceMatcher non-equal ranges. After: +3/-2 = 5 lines, three ranges.
Delta -56 lines/-8 contiguous points. Existing exclusions are omitted from
this comparison, without individual accounting. IDs below split independent
changes sharing a range; they are not the mechanical contiguous-point count.

| ID | Original/current point | Executor disposition | Independent counter-review |
| --- | --- | --- | --- |
| R0252 | After host_def include: added stdio/inttypes | Remove both additions | stdio-only object identical; diagnostic restoration passes actual VGA both widths; original mirror warnings remain unsuppressed. |
| R0253 | XP line 28 evidfunc include | Restore exact quoted original | Actual consumer selects original declarations and vector addresses; both objects identical in control. |
| R0254 | Added gdp_slots include | Evidence-qualified pending owner decision | Pair with R0263; original fixed-offset initialization actually corrupts x64 ring pointers. |
| R0255 | XP line 35 Evid_c include | Evidence-qualified pending owner decision | CCPU aggregate is 240 bytes x64, original CVID 232; route_reg1 offsets 168/160, wrstate 236/228. x86 both 156 bytes. Existing GDP owner requires one matching aggregate width. Single-header x64 VGA control access-violates; no claim of all-family restoration testing. Rewriting the other original aggregate/users adds differences and is not an admitted reduction. |
| R0256 | XP line 36 j_c_lang include | Restore exact quoted original | Both objects identical; shared jcode register declarations remain native width. |
| R0257 | XP line 503 mode-0 diagnostic | Restore original format/argument | chain index is actual mode selector 0..2; cevid_verbose remains false in production, no surviving production setter found. No evidence of a required product behavior fix. |
| R0258 | XP line 514 mode-1 diagnostic | Restore original format/argument | Same supported domain; both actual VGA controls pass. No claim of general varargs type compatibility. |
| R0259 | Added CHAIN2 bridge before setMarkPointers | Remove helper and three wrappers | Original markers cover all affected groups. Extra inclusive string-end mark is bounded, not lost drawing; no necessary bridge semantics established. |
| R0260 | Added mark selector 4 | Restore original switch | Production gfx_updt selects existing 2 instead; no alias/second marker path remains. |
| R0261 | XP line 709 BIOS byte signature | Restore IU8/IU32 | Real video.c callers pass getAL()/byte ch; valid values already fit. Actual VGA passes both widths. Original callback declaration mismatch is not concealed. |
| R0262 | XP line 719 BIOS word signature | Restore IU16/IU32 | Real caller combines BL:AL; values fit 16 bits. Actual VGA passes both widths. |
| R0263 | XP line 792 GDP allocation | Evidence-qualified pending owner decision | Paired original allocation/macros control x64 ptr/low/high=0/0/1 versus current 1/1/1; x86 both pass. No partial malloc/slot mismatch used as proof. |

CHAIN2 boundary reasoning: original v7_get_banks masks non-CHAIN4 bank to
0/1; set_banking translates this to bitmap bias 0/0x4000. A 64-KiB source
range ends at most at bitmap cell 0x8000 including the original string
marker's extra endpoint, inside host_init_screen's 0x20000-byte allocation.
Byte and word markers cover the same required quarter-address groups. The
string marker may dirty one extra group and reports original byte-count work;
this can cause conservative redraw/full-refresh threshold timing, not missing
writes. ega_graph_update returns when dirty_total is zero and clips dirty
lines to the displayed extent. Zero-count bitmap marking alone does not
request painting. We do not claim byte-identical internal bookkeeping.

Coupled gfx_updt edit changes only existing SetMarkPointers(4) to (2); its
XP-relative diff size/point count does not increase. This is not a full audit
of that later ordered file. Existing actual VGA regression now also covers
end-of-aperture byte/word addresses, both bank offsets, string lengths 1..16
clipped to the aperture, every required dirty group, outer sentinel cells,
zero dirty total, and saved argument registers. No test-only production API.
Production +7/-51 (net -44), tests +20/-13 (net +7). Four functions and the
additional selector are removed rather than relocated to Compat.

Only the already-existing GDP include/allocation and matching aggregate
selection remain, pending owner disposition; no new mirror difference is
introduced. Manifest reflects final candidate bytes. Final dual build/full
regression/publication and delivery results follow after their actual exit.

Following identical files, checked directly against the fixed XP commit:

- 276 base/cvidc/fpu_c.h: normalized identical, SHA-256
  bc4960c41d182fc7da103a32f1229b5e5f725fee14808988129ca0763bb4892a.
- 277 base/cvidc/gdpvar.h: normalized identical, SHA-256
  d88750cd024f1ac99e06560593e07cf4712a8aec043cb052b738739f9fc4dad7.

No restoration, new point or caller change is needed for these two files.

### P27 final-source verification and actual-change review

Both cached ninja-ccache-tests builds exit 0 and publish the final EXEs.
Each architecture passes all 188 background tests and all eight explicitly
selected desktop/native-audio tests: fresh 392/392, no retry or previous-run
substitution. VGA tests include the actual selected rule writers, bank/end
boundaries and composed pixel/mode regressions. Package tests pass in the
active Console session. This is automated regression evidence, not a fresh
manual Win3.1/Win95 media session or exhaustive register-state proof.

Published compiler timestamps: x86 Nov 13 2026 03:44:12; x64 Nov 13 2026
03:47:44. Relative to P26, EXEs decrease 676 and 673 bytes respectively;
these are substantive artifacts, so the equivalence exemption is not used.
Mirror gate and documentation gate pass. Warning scan finds only original
mirror code/header diagnostics, not new shared/Compat/test-source warnings;
no suppression was introduced. git diff --check passes.

Coordinator actual-diff review: exact original restores except the three
qualified residuals; the only coupled production change selects existing
marker 2; no new function, alias, branch, allocation owner or Compat path.
Tests extend the existing owner fixture and retain actual rule calls. No user
configuration, snapshot, media, firmware, shared package or sibling changes.
In-scope mirror reduction is 56 lines/eight contiguous diff points; the
producer selector replacement adds no new XP difference. Final reviewed
sequence is 277/472; next is 278 base/cvidc/j_c_lang.c. R0254/R0255/R0263
and earlier evidence-qualified items remain pending owner decision.

Published softpc32.exe SHA-256: 3780db1c2a480c1c320a841e29ce5c1861ddf33912d6598b364a7fdb85cd464a.

Published softpc64.exe SHA-256: 12b71510ce0058132c5b3c9cc16463ab7692f69692b3aed9409759af19f540c1.

All owned probe/build/test processes were terminal before cleanup. The exact
active scratch child was previewed and removed; Clean-Build -Check passes.
Reusable Ninja/ccache roots remain. P27 is a complete file delivery, not S1
closure or approval of the evidence-qualified residuals.

## File 278 j_c_lang.c / P28 candidate

Only three in-scope ranges differ: added stdio/inttypes and reason comments,
bit-position diagnostic format, length/start diagnostic format. XP diff
+8/-2 = 10 lines/three contiguous points becomes zero by exact restoration.
Production +2/-8, no test/source relocation or added helper. Entire file now
matches fixed XP after CRLF normalization.

| Point | Executor analysis | Counter-review |
| --- | --- | --- |
| R0264 | Remove five-line added includes/comments after host_def | Actual compiler accepts original; no caller-visible computation or return changed. Original diagnostic declarations warn; no suppression or false warning-free claim. |
| R0265 | Restore original bit-position diagnostic | No actual supported requirement for widened invalid-input text established; this is not a computation repair. |
| R0266 | Restore original length/start diagnostic | Same; restore original rather than retain speculative modernization. |

Repository C source scan outside this file finds 432 mask call expressions in
14 C-VID source files and no calls to its six rotation helpers. This is a
source inventory, not proof that every expression executes. Generated rules
and the shared r0..r31 definitions are actual CCPU object inputs; the file is
not unused. Uncalled original helpers remain: deleting them would increase
mirror diff. No behavior is copied to Compat.

Isolated actual-compiler controls append a bounded test main to each complete
original/current function body. Every start in 0..width-1 and length in
0..width-start is checked bit by bit, including zero/full-width masks:
560 pairs each on x86, 2144 each on
x64. Both variants pass on both widths; invalid start=width and len=width+1
also return zero and print the same tested values. This does not claim all
invalid native-width diagnostic values produce identical text, nor validate
unused rotation semantics. Current/original diagnostic difference for extreme
invalid arguments is consciously restored under the original-first policy.
Final dual build/publication and full regression follow; no old results are
substituted for final-source qualification.

Adjacent file-279 read-only preparation: j_c_lang.h adds one seven-line
CrulesRuntimeError declaration/comment block outside existing exclusions.
Actual CVID C source scan finds 2465 direct literal-string statement calls in
31 files; no other use outside the original ev_glue provider is found in that
scope. All discard the result. The provider is void(char *) and only prints
its argument. This is not yet a full necessity disposition or header change;
compile/object ABI controls belong to the next complete-file delivery. Do not
count file 279 reviewed or alter it while P28's build/tests are running.

### P28 final verification and actual-change review

Cached x86/x64 build and publication pass. Each final architecture passes
188 background tests and eight desktop/native-audio tests: fresh 392/392,
no retry or equivalence exemption. Mirror/documentation gates and diff check
pass. The only compiler warnings in this build are original mirror printf
declarations; no new flags or suppression. Test and non-mirror production
code are unchanged.

Coordinator review checks all three actual diff ranges against fixed XP,
not only the candidate summary: the full normalized file is identical with
SHA-256 f8e31bc5d63f810f73da165084a5a498a8261e48ff20daf7efdc1393216f2f48.
No calling interface, register storage, bit computation, original helper,
Compat implementation or lifecycle is added/removed. Diagnostic wording is
restored to XP; arbitrary malformed arguments are not claimed text-equivalent.
No guest/configuration/snapshot changes. Production +2/-8 (net -6), tests
+0/-0; mirror diff -10 lines/-3 contiguous points, with no new pending
retention decision in this file. Reviewed sequence reaches 278/472; next
279 j_c_lang.h remains unreviewed pending its actual ABI controls.

softpc32.exe: compiler timestamp Nov 13 2026 03:55:23; SHA-256 9ffecd3156f419201c06d67a304c3293b9013b55351464907b156ba4f16149a9.

softpc64.exe: compiler timestamp Nov 13 2026 03:59:05; SHA-256 ed1f58f20315aa297469d25d76faf79e7c731fc1bf66dbd77b80d3153fe5b9d3.

P28 owned processes were terminal before the exact task scratch was previewed
and removed. Clean-Build -Check passes; only reusable caches remain. This is
a verified complete-file delivery, not S1 closure.

## File 279 j_c_lang.h / P29 candidate

R0267 covers the only in-scope difference: seven added lines after rolb's
original prototype, declaring CrulesRuntimeError and asserting an ABI need.
Remove that whole addition; retain all original declarations and established
excluded-body scaffolding unchanged. Production +0/-7, no tests changed;
in-scope XP diff +7/-0, one point, becomes zero. This is not whole-file raw
identity because existing exclusions remain; they receive no new accounting.

Executor: 2465 actual source call expressions across 31 C-VID files pass
literal string pointers as standalone statements. They do not consume a
return value. Original ev_glue supplies void(char *) with the same pointer
parameter. No Compat/test use is found. Original implicit-call rules do not
narrow pointer arguments through default promotion, and the ignored return
does not provide a necessary conversion. Historical marker wording alone
cannot establish an ABI repair.

Counter-review: test compiler acceptance and emitted code, not source intent
alone. Normalize Ninja's CRLF dependency dump before parsing blocks; assert
actual source object identities. Final capture finds 35 real consumers per
architecture: 31 sevid units, sinit011/012/013 and ev_glue. Their before hashes
and P28 published EXEs are captured before the header edit. Both cached builds
and whole-object/artifact comparison are running. Only exact object identity
and EXE differences restricted to validated timestamp/PE metadata may reuse
P28's fresh 392-pass full result; otherwise run a fresh full suite.

Following file 280 pigger_c.h is normalized identical to the fixed XP commit,
SHA-256 da8c15abd4725222119197bdbfe4c9abcfd89dd1f281f0573b573a196a2ab180.
No modification or new audit point is required for it.

### P29 final verification and actual-change review

Both cached builds/publications pass. All 35 actual consumer objects per
architecture are byte-identical before/after (70/70). P28 and P29 EXE sizes
are equal: x86 3515785, x64 3115113 bytes. Each EXE differs at nine bytes only,
within PE COFF timestamp, PE optional-header checksum and one same-offset
validated HH:MM:SS literal. A separate review zeros exactly those ranges and
compares every remaining byte again: both pass. No executable section or
arbitrary changed byte is excluded. Literal offsets/times: x86 2435392,
03:55:23 -> 04:06:38; x64 2289120, 03:59:05 -> 04:07:10 (Nov 13 2026).

Fresh actual VGA tests pass x86/x64 (6.51/5.41 seconds). The owner-approved
artifact-equivalence exception reuses P28's fresh 392/392 full result; no
fresh P29 full-suite run is claimed. Mirror, documentation and diff gates
pass. Recompiled original generated-rule/header warnings remain; no new
suppression or non-mirror/test warning is introduced.

Coordinator actual-diff review: only seven added declaration/comment lines
are removed; no argument/register/field conversion or resource path changes.
Original diagnostics and call expressions remain. Zero in-scope XP difference
is not a claim of whole raw-header identity: established excluded content
remains untouched. R0267 is restored/verified, not retained for approval.
Following identical pigger_c.h needs no edit. Sequential review is 280/472;
next 281 base/cvidc/sas4gen.h. Production +0/-7, tests +0/-0; XP difference
reduced seven lines/one point. No configuration, snapshots, media, guests,
shared packages or external repositories are modified.

Published softpc32.exe SHA-256: d7b0fe27e918573f06326f5b70952bc4c6a51ee5323bbfc2e094c9c6a120c6ec.

Published softpc64.exe SHA-256: 0e0aa4eb3d777201633c7aafe4906d7d21f314dbebf4369a93fabd216d040698.

P29 owned processes were terminal; exact task scratch was previewed and
removed. Clean-Build -Check passes. Reusable caches are preserved; no pending
probe or unfinished verification is represented as completed.

## P30 file 283 sevid000.c audit and minimization (verification in progress)

Files 281 sas4gen.h and 282 sascdef.c are reviewed in frozen order. For 281,
recursive comparison preserves exact selected text/conditional directives,
skipping complete existing #error bodies and their line controls without
accounting for those exclusions. File 282 is normalized identical to pinned
XP. Neither needs a production edit.

File 283 compares with the fixed XP SP1 counterpart. The initial 412 contiguous
non-equal ranges (+514/-527, 1041 diff lines) are individually mapped in
[m9-t100-sevid000-points.tsv](m9-t100-sevid000-points.tsv). Ranges refer to XP
and the pre-P30 source at b12197bf3e654fba973ce0e11df44289a22f4e7c;
multiple evidence groups may cover one range. No exclusion-body entries exist.
Reversing the precisely enumerated ABI expressions for comparison, then
comparing retained tokens, leaves only the layout, continuation and DWORD
repairs below; there is no unclassified semantic remainder.

| Point | Initial analysis and proposed disposition | Independent counter-review / evidence |
| --- | --- | --- |
| R0268 | Restore all 35 native-address dereferences exactly, including original trailing whitespace where the whole line matches. | 26 sources are VGA read-plane pointer slot 1284 plus offset; eight are native string destination LocalIUH[0], saved from r2; one is screen base slot 1400 plus offset. ev_glue read_str_fwd passes native dest through jccc_parm1. None is a GDP field offset. Existing adapter returns those valid host addresses unchanged. Original dereference candidate passes real VGA executable in both architectures. No new helper or alternate path. |
| R0269 | Existing GDP slot access/include: evidence-qualified residual, pending owner decision. | GDP is the existing opaque slot owner, not a flat original byte array. Earlier R0254/R0263 paired original allocation plus original initialization already demonstrates native-width pointer overlap on x64; restoring only this consumer would be an invalid partial experiment. This file consumes the same 0/8/12/16 tracing slots and native VGA fields. Replacing the entire owner/family is not a justified reduction within this file. No claim that a coherent whole-family restoration was dynamically run here. |
| R0270 | Existing native-width zeroed LocalIUH storage: evidence-qualified residual, pending owner decision. | XP allocation counts four bytes per IUH; supported x64 rules use eight. E.g. two-slot local offset 1 is already beyond an original eight-byte allocation. Original allocation passes x86 but x64 fails with heap-corruption termination. Sized nonzero malloc also fails x64; deliberate 0xa5-filled allocation reproduces access violation. Source confirms partial IU32 stores followed by IUH loads (e.g. Chain4 byte reader LocalIUH[0]); high halves must be initialized. Both malloc controls pass x86, so no x86-specific need is claimed. |
| R0271 | Six existing local goto continuations: evidence-qualified residual, pending owner decision. | Original recursive dispatch enters crules with fresh CopyLocal pointers initialized null, while using the saved LocalIUH allocation; leaving that activation frees it and restores null, losing the enclosing activation. Restoring the exact six calls fails the real VGA executable with access violation in both architectures. Current local transfers pass without adding another call/ownership path. |
| R0272 | Existing CHAIN2 layout correction: evidence-qualified residual, pending owner decision. | Existing production planes retain the A0 gap: physical 4*(address & ~1)+(address & 1). Original shift/stride/alignment restoration fails BIOS pixel-read colour assertion in both architectures. Original read expressions cannot consume the actual selected layout. This control preserves GDP, allocation and continuation adaptations; it is not proof of a fresh Win3.1 guest reproduction. |
| R0273 | Preserve both CHAIN2 DWORD fixes but minimize their original-relative scope. | Exact original +4 second WORD offset returns 0x66552211 instead of 0x44332211. Original final composition returns 0x00002211. Both fail real rule assertions in x86/x64. New candidate restores the original extraction/register sequence and changes only final combination to low WORD plus r20 shifted 16; r20 was just assigned the second WORD. Its preceding extraction bounds r20 to 16 bits. Offset stays +2. Both architectures pass the actual VGA suite; no widened read or changed guest layout. |
| R0274 | Existing Unchained DWORD +2 correction: evidence-qualified residual, pending owner decision. | Restoring +8 fails the real DWORD assertion in both architectures. Existing tests place independent bytes at +2/+3 and +8/+9, across four planes and several offsets; the second WORD is two guest bytes later. No additional rule implementation is introduced. |

Controls compile the actual source body with each architecture's real Ninja
compile command and link its object before the machine archive in the real
VGA test command. No test-only production entry or guest modification is used.
Early overlapping harness runs shared a relative disposable disk pathname;
file-open assertion failures from those runs were discarded as harness
interference. A subsequent single runner executes every variant serially in
its own architecture/variant working directory. Final isolated results:

| Variant | x86 | x64 |
| --- | --- | --- |
| Native-address restoration | pass | pass |
| Final minimal candidate | pass | pass |
| Original four-byte-slot allocation | pass | heap corruption 0xc0000374 |
| Native-size malloc | pass | access violation 0xc0000005 |
| Native-size malloc filled 0xa5 | pass | access violation 0xc0000005 |
| Original continuation calls | access violation | access violation |
| Original CHAIN2 layout | colour assertion | colour assertion |
| Original Unchained DWORD offset | DWORD assertion | DWORD assertion |
| Original CHAIN2 DWORD offset | 0x66552211 assertion | same assertion |
| Original CHAIN2 DWORD combination | 0x00002211 assertion | same assertion |

Source/ABI reasoning, actual rule-test evidence and guest reproduction are
separate: no new DOS/Win3.1/Win95 interactive session is claimed. The existing
suite includes original BIOS/controller paths; this is not proof of every
hardware mode/register combination. Tests are unchanged.

Proposed production diff is +53/-45 (net +8) in one mirror source. XP-relative
diff is +492/-497 = 989 lines, 408 points: **52 fewer diff lines, four fewer
points**, despite the source line increase. Original calculation statements
are restored, not moved into Compat. No shared component, configuration,
media, snapshot or external tree changes. Final cached dual build/publication,
full background/native suites and actual-change acceptance remain pending.

### P30 final verification and actual-change review

Final production source exactly matches the minimal candidate exercised by the
isolated x86/x64 VGA controls; normalized SHA-256
f6d4200da8381889e4a361eea441b0f34edb4abba062d47b7f5e46b71c8c7d57.
The source retains CRLF. Both cached builds/publications pass without compiler
warnings in this batch. Published timestamps: x86 Nov 13 2026 04:25:31;
x64 Nov 13 2026 04:29:07. No equivalence exemption is used.

Fresh qualification: x86 188/188 background and 8/8 native pass. x64 first
background run passes 187/188; emulator.verifier-negative reaches its existing
60s timeout (60.47s). Its unchanged, isolated recheck passes in 22.71s under
the same 60s budget; all 8/8 x64 native tests then pass. Thus all 392 required
tests have passing final observations, with one explicit retry, not a clean
first-pass full-suite claim. The verifier copies/configures Emulator fixtures;
it neither compiles nor executes the changed mirror. Aggregate timeout is
observed; a deeper host scheduling cause is not proven. No timeout widening,
product change or test change is made to obtain the recheck pass.

Coordinator review of actual changes confirms all 35 addresses retain their
native widths and origins, with no new ownership or dispatch path. Both DWORD
expressions mask by the low-WORD load and shift unsigned IU32 r20 by 16; the
original preceding extraction and WORD overwrite establish its bounded value.
No argument, guest address layout, selected mode or allocation contract changes.
The 412-range evidence map covers the initial diff; final XP difference is
+492/-497, 408 points. Source +53/-45 is not presented as a source reduction.
Required existing residuals remain qualified pending owner decision, not
silently approved. No sibling or shared-package edits are included.

Plain git diff --check reports restored original trailing tabs. All 26 such
added lines were checked against exact XP text (two retain the already-required
GDP ABI expression); this is required original formatting, not new style churn.
A command-local core.whitespace=-blank-at-eol check passes for other whitespace
errors, and the separate exact-original check passes. No Git configuration or
repository whitespace policy is changed. Mirror and documentation gates pass.

Published softpc32.exe SHA-256: 40c6363bc958382cc6122a8e99497e2cc66c055f10080dc72b1449641076480a.

Published softpc64.exe SHA-256: 9208689e5bd9414c1565ff6baf1172e7c38a816a437b5345e20182a1b482a019.

Sequential reviewed scope is 283/472. Next is 284 base/cvidc/sevid001.c.

P30 actual process inventory confirms no remaining task-owned probe or
qualification job. Exact scratch cleanup was previewed, applied and checked;
Clean-Build -Check passes. Reusable caches and user assets are preserved.

## P31 file 284 sevid001.c audit and restoration

Pinned XP and pre-P31 HEAD 130619e4838d482feb7cb04ff490f1057cb64456 are
compared in full. Initial +467/-470 = 937 diff lines, 381 changed ranges;
[point map](m9-t100-sevid001-points.tsv) assigns every initial range. No
existing excluded body is reopened. This compiled rule file remains required:
read/mark/write/fill/move tables use its S2118-S2149 implementations.

| Point | Executor disposition | Independent counter-review |
| --- | --- | --- |
| R0275 | Restore 31 native-address wrappers to XP dereferences. | String destinations retain full native r2; latch addresses derive from read-plane pointer slot 1284 plus physical offset; backward move destinations derive from screen pointer slot 1400, and RAM sources retain native r3. These are addresses, not GDP offsets. No truncation or replacement helper is introduced. |
| R0276 | GDP slot mapping: qualified residual, pending owner decision. | Raw XP offsets describe the original 32-bit GDP layout; current native pointer/IUH fields require mapped native storage. Existing mapping remains; this file has 528 slot calls. Shared GDP layout proof from preceding audit applies, not a claim of a new failure probe for every slot. |
| R0277 | Native-size zeroed locals: qualified residual, pending owner decision. | Actual original four-byte allocations pass x86 but crash x64; native-size allocations poisoned with 0xa5 also crash x64. Generated partial-width writes subsequently feed full-width address/register reads. 31 calloc sites remain. |
| R0278 | Restore four original continuation calls and immediate returns. | Unlike sevid000, no nested same-translation-unit caller into these string readers is present. ev_glue enters through the selected reader table; same-file non-tail calls only connect Chain4 marking to Unchained marking, not these readers. Tail calls release the active locals once and return immediately. No supported-path necessity for the four gotos was established. Both architectures pass current and restored actual-body controls and the expanded independent oracle. Earlier file-specific continuation failures do not justify these patches. |
| R0279 | CHAIN2 backward physical addressing: qualified residual, pending owner decision. | Restoring the complete original layout while retaining ABI fixes fails both architectures: mode0 even backward string expects 0x37 but reads 0x5a. Current odd/even plane stride and latch alignment match scalar reads and independently located plane bytes. No new layout or dispatch is added. |

The expanded temporary fixture executes the actual registered backward readers,
not a replacement implementation: modes 0/1, offsets 0x108/109/110/111,
counts 0/1/2/7, both destination directions and three repetitions. Independent
plane bytes and a complete 64-byte destination oracle verify 384 combinations
per executable, including untouched surrounding bytes. Current and final
restoration pass all combinations in x86 and x64. Existing VGA tests also pass
in each executable. Temporary fixtures expose no new production API.

All variants use actual Ninja compile/link commands and separate disposable
working directories, run serially. Original-layout failure exits with assertion
failure (x86 0xc0000409; x64 3); original allocation and poisoned native-size
allocation fail x64 with 0xc0000005. These are actual rule-test results, not
DOS/Win3.1/Win95 interactive reproduction or exhaustive hardware coverage.

Final production diff: +37/-33 (net +4), one mirror file; tracked tests unchanged.
XP-relative diff: +448/-447 = 895 lines, 366 points. Reduction: **42 diff lines
and 15 points**. Four extra source lines restore original two-line continuations;
source line count is not the minimization measure. No Compat relocation, shared
package change, guest, configuration, media or snapshot change. Normalized source
SHA-256: 588f4fdc95430046870d7db87719625904fa970f1a806765d6aba440cd2ee389.
Cached dual build/publication and complete regression are in progress.

P31 qualification exposed a coupled stale gate: standalone_source_boundary
required those four sevid001 gotos unconditionally. Remove its one file/list
entry after the actual-call-chain and runtime evidence above; retain the
other files' continuation checks. Gate configuration -1 line; no new exception
framework or production path. Initial x86 gate failure is retained as a real
first-run failure, not silently relabelled success.

### P31 final verification and actual-change review

Final source byte identity matches the restored-continuation candidate tested
in both architectures; CRLF retained. All 23 added trailing-whitespace lines
exactly match pinned XP. Command-local core.whitespace=-blank-at-eol diff check
passes; no global Git configuration or formatting policy changes.

Both cached builds publish fresh EXEs. The original switch-unreachable warning
at sevid001.c:1053 appears in both unchanged-source controls and final builds;
it is not introduced by this patch. No warning cleanup is used to add mirror
differences. x86 timestamp Nov 13 2026 04:46:47; x64 04:50:24.

Full qualification: x64 188/188 background and 8/8 native first pass. x86
186/188 background first pass: the stale continuation gate fails and the
unchanged emulator.verifier-negative reaches its existing timeout (58.95s).
After removing the stale gate entry, serial rechecks pass: gate 3.70s,
verifier 26.29s under the same budget. x64 gate is also rechecked (3.49s).
x86 native 8/8 passes. All 392 tests have passing final observations, with
explicit initial failures and rechecks; no claim of first-pass 392/392.
No deeper host cause of the aggregate verifier timeout is proven.

Coordinator actual-diff review checks every changed expression and all four
tail calls against XP, caller ownership, probe-source identity and the complete
381-range map. Other files' continuation guards remain intact. Existing GDP,
local-storage ABI and CHAIN2 layout residuals remain evidence-qualified pending
owner decisions. No public component, user input asset or sibling repo changes.
Production +37/-33, tests +0/-0, gate +0/-1; documentation/provenance are separate.
Mirror and documentation gates pass. Sequential review is 284/472; next file
285 base/cvidc/sevid002.c, not an assertion of whole-task completion.

Published softpc32.exe SHA-256: ae1e9746e1c9a252c0813735f0be270cfbca7a0368ec7723433bf9b7f027392f.

Published softpc64.exe SHA-256: d129f69fd3bf99c38ea965043a9727b621b45dcb41c0ef50bdbc77a62ab65075.

P31 process inventory confirms all task-owned jobs have exited. Exact scratch
cleanup was previewed/applied; Clean-Build -Check passes. Reusable caches and
user assets are preserved.

## P32 file 285 sevid002.c audit and restoration

Full fixed-XP comparison against pre-P32 bdd0364f8f127d3e55e0cbbf4f4d9e2a9007e3cb:
initial +629/-628 = 1257 diff lines, 478 ranges. The complete
[point map](m9-t100-sevid002-points.tsv) classifies every range. Reversing only
GDP/address adaptation spellings and allocation adaptation makes all remaining
non-whitespace tokens match XP. No extra device behavior patch or excluded-body
audit is involved. The file defines S2150-S2181 and remains integrated through
simple_evid/dith_evid; ev_glue selects dith_evid when the original V7 foreground
dither mode is enabled by ega_writ. This is not a test-only file.

| Point | Executor disposition | Independent counter-review |
| --- | --- | --- |
| R0280 | Restore 42 native-address wrappers exactly to original dereference syntax. | Sources trace to VGA_wplane (GDP1288), VGA_rplane (1284), screen_ptr (1400), or existing native RAM-copy arguments and their offsets. These are not GDP offsets. The existing wrapper returns valid host addresses unchanged; mapped GDP field access itself stays intact. No width, layout, marker or copy algorithm is changed. |
| R0281 | 669 GDP slot calls and their include: evidence-qualified residual, pending owner decision. | Pinned original offsets describe 32-bit GDP fields; native pointer/IUH layout differs. Existing field mapping uses the actual structure offsetof and native field width. Prior GDP ABI evidence remains applicable; no claim of individually fault-injecting every mapped access here. |
| R0282 | Native allocation capacity at 32 sites and zeroing at three: evidence-qualified residual, pending owner decision. | Original four-byte capacities cannot hold native IUH-indexed locals on x64. Focused actual V7 dither controls expose x64 heap corruption with original capacities. S2154/S2160/S2181 save LocalIUH[1] through IU32 and later read it through IUH in the RAM-source branch, requiring defined upper bytes. This three-site zeroing argument is source/ABI proof, not a claimed dynamic failure from the byte-write/fill probe. |
| R0283 | Restore original malloc semantics at the other 29 sites, retaining native capacity. | Per-function local-slot inventory finds no partial-width-store/native-width-read overlap in these functions. Original full-width pointer locals and same-width scalar accesses remain unchanged. Zero initialization is not retained merely because neighboring files need it. Current/restored candidate and nonzero-filled native allocations pass focused byte-write/fill controls in both architectures. No allocation wrapper is added to production. |

The [32-site allocation inventory](m9-t100-sevid002-allocations.tsv) records
function, capacity and mixed-width slots. Capacity adjustment remains a local
ABI patch, not a replacement allocator. Source restoration is +68/-68 (net 0)
in one mirror source, tests unchanged. XP-relative +594/-593 = 1187 lines,
457 ranges: **70 fewer diff lines and 21 fewer points**. Restoring malloc
semantics does not increase either metric. Mirror source remains CRLF.

Initial real VGA tests passed even with original-sized allocation, so those
passes could not establish this file's allocation safety. A task-local fixture
then invokes the actual registered V7 mode0 byte-write/fill functions with
explicit latches, logical masks, foreground latches, plane selection and plane
mask. Two addresses and counts 0/1/2/7 verify ten cases with an independent
four-plane byte oracle and untouched surrounding memory. It restores state
before existing VGA tests. The first two fixture drafts omitted required latch
and plane-mask setup and failed on unchanged code too; these were discarded as
fixture errors, not production failures. The corrected unchanged-code preflight
passes before restored variants are compared. Each process has its own scratch
working directory and uses actual Ninja flags and the real machine archive.

| Final corrected actual-body control | x86 | x64 |
| --- | --- | --- |
| Current production | pass | pass |
| Address-only restoration | pass | pass |
| Final 42-address/29-malloc restoration | pass | pass |
| Original four-byte allocation | pass | heap corruption 0xc0000374 |
| Native-size allocations filled 0xa5 | pass | pass |

The last row exercises the focused byte-write/fill path; it does not exercise
all RAM moves or disprove the three mixed-width slot requirements. No new
interactive guest reproduction or exhaustive dither-mode qualification is
claimed. Existing source-width behavior of RAM parameters is unchanged; these
controls do not qualify arbitrary high-address RAM move arguments. No guest,
configuration, media, snapshot, shared package or sibling repository changes.
Final normalized source SHA-256:
ecb98d97732d0b46914aa4a41d1ca48ac4c2a5735b65499977b6888313278638.
Cached dual build/publication and full regression are in progress.

### P32 final verification and actual-change review

Final tracked source is byte-identical to the minimal candidate exercised by
the corrected x86/x64 actual-rule probes. All 35 restored trailing-whitespace
lines exactly match XP; CRLF remains intact. Command-local whitespace checking
with blank-at-eol excluded passes without changing Git configuration. Production
+68/-68 (net 0), tests +0/-0; no build/gate code changed. Provenance and task
records are counted separately. All 478 initial ranges are classified, with
32 allocation sites separately inventoried.

Coordinator actual-diff review confirms 42 address wrappers removed without
changing arithmetic or native GDP field loads. The other 29 allocation changes
restore malloc behavior while preserving native slot count/size; the three
mixed-width cases remain zeroed. Other mixed reads narrow already-written values
rather than requiring uninitialized upper bytes. Existing RAM-source width and
all original rule algorithms remain unchanged. No equivalent implementation or
complexity is moved into Compat, and no new production helper/API is introduced.
Remaining ABI patches are qualified evidence, not automatically approved.

Both cached builds publish fresh EXEs: x86 Nov 13 2026 05:08:04, x64 05:11:37.
Each emits the existing sevid002 switch-unreachable warning at line 849, also
present in the unchanged-body control; no new warning is introduced. Fresh full
qualification passes x86 188/188 background + 8/8 native and x64 188/188 + 8/8,
**392/392 first run**, with no retry or budget change. Mirror/documentation gates
pass. No new interactive DOS/Win3.1/Win95 session is claimed beyond the actual
paths covered by those tests and focused controls.

Published softpc32.exe SHA-256: 38c19f049b8be7b5e1a14913456738fbf28ef192f01654627588466c0f6e9371.

Published softpc64.exe SHA-256: 49bcc1429f7431eb7caa8c71cb94f6aeb3e702ef705aeb19bf9970b9146d544f.

Sequential reviewed scope is 285/472; next file 286 base/cvidc/sevid003.c.
S1 and the whole audit remain open.

P32 actual process inventory confirms no task-owned job remains. Exact scratch
cleanup was previewed/applied, and Clean-Build -Check passes. Caches and user
assets are preserved.

## P33 file 286 sevid003.c audit and restoration

Pinned XP SP1 is the same fixed revision as admission; normalized original hash
19f87d7cdd00a65c725dea946a413238d30e1328d3d78e648f8830b958c602a5.
This compiled file implements S2182-S2213, reached through original evidfunc
write tables and ev_glue's dith_evid selection. ega_writ selects those tables
for the supported V7 dither setting. The file is needed, not test-only, and no
Compat duplicate replaces these device operations.

The [initial range inventory](m9-t100-sevid003-points.tsv) classifies all 557
non-equal ranges; the [allocation inventory](m9-t100-sevid003-allocations.tsv)
records each of 32 entry allocations. Existing excluded bodies are not counted.

| Point | Initial finding | Independent counter-review and disposition |
| --- | --- | --- |
| R0284 | 55 native memory dereferences wrapped in softpc_gdp_rule_address. | Addresses derive from native video plane pointers or native move/copy arguments, not fixed GDP offsets. The helper returns those pointers unchanged; restore original dereference spelling and formatting. Retain GDP field lookup at each pointer's source. |
| R0285 | GDP access and required header differ from original fixed 32-bit storage. | Native IUH/IHPE and callbacks require actual native-width field storage; direct original offsets do not describe that layout. Keep the existing ABI mapping, with no new helper or duplicate device behavior. Qualified residual, not owner-approved retention. |
| R0286 | 32 native-slot allocations replace original byte capacities; the pre-audit local tree used calloc at every site. | Original capacities assume four-byte locals. On x64 the original-size control corrupts the heap. Five entry bodies save LocalIUH[1] through IU32 and later read IUH in their RAM branch: S2186, S2194, S2199, S2205, S2208. Keep their existing zeroing and native capacity. This prevents reading unspecified upper storage but does not establish arbitrary high-address RAM-source correctness. Qualified residual. |
| R0287 | Remaining 27 entries zero their whole local storage without demonstrated need. | Native pointer locals are fully written before native reads; other accesses consume defined narrow values. Restore malloc semantics, retaining only native-size capacity. Do not apply this to the five mixed-width entries. |

Outside these mechanical categories, reversal of the ABI adaptations leaves
no token difference from XP. No device algorithm, continuation, stride, guest
address, callback selection or lifecycle is changed. No guest or other tree is
modified. Existing input-width limitations are not redesigned in this audit.

Production +82/-82 (net 0); tracked tests +0/-0. XP-relative diff changes from
+718/-717 = 1435 lines / 557 ranges to +672/-671 = 1343 lines / 521 ranges:
**92 fewer diff lines and 36 fewer points**. Metrics use line SequenceMatcher
with autojunk disabled, consistently with the preceding file. Native-width
malloc restoration does not add XP-relative points or lines. All 46 newly
restored trailing-whitespace lines exactly match XP; source retains CRLF.

Task-local tests compile the actual sevid003 body with the actual Ninja flags
and replace its archive definition in the existing VGA test executable. An
additional local fixture calls original registered V7 dither mode2 word and
mode 3 byte/word write/fill entries: two address parities, single writes and fill
counts 0/1/2/7. Thirty cases check independent four-plane foreground bytes and
all surrounding memory. Latches, masks, plane selection and plane pointer are
set explicitly and restored before existing VGA/CHAIN2 checks. Each control
runs in a distinct disposable directory. No test-only production API is added.

| Actual-body control | x86 | x64 |
| --- | --- | --- |
| Unchanged production | pass | pass |
| Address-only restoration | pass | pass |
| Final 55-address / 27-malloc candidate | pass | pass |
| Original four-byte allocation capacity | pass | heap corruption 0xc0000374 |
| Candidate malloc storage poisoned with 0xa5 | pass | pass |

A first task script stopped before compilation because its insertion anchor
used the wrong header spelling. Correcting that task-only script did not change
production or the fixture oracle. The corrected unchanged-production preflight
passes before candidate comparisons. Poisoning covers the exercised write/fill
paths, not every copy/move branch; the five retained zeroing cases have source
ABI evidence rather than a claimed exhaustive dynamic RAM-move proof.

Final normalized source hash:
029fe47a57faae30a9687245b25389950d965870c1f44ec7fbfbc2d824090df5.
Tracked source exactly matches the tested minimal candidate. Cached dual
build/publication and full qualification are running; this paragraph does not
claim those results or P33 delivery complete.


### P33 final verification and actual-change review

Coordinator review checks the actual 82 replacement lines against the original
expressions and the tested candidate. The 55 pointer restores leave GDP source
loads and address arithmetic intact. The 27 malloc restores retain native slot
capacity. Five mixed-width local reads still retain zeroing. No out-of-file
behavior, shared package, test API, device algorithm or continuation is changed.
After reversing only retained ABI forms (529 fixed and 225 dynamic GDP accesses,
plus allocation capacities/zeroing), whitespace-stripped content equals XP.
The 557-range inventory accounts for the complete initial non-excluded diff.

Production +82/-82 (net 0); tests unchanged. Source remains CRLF and owned
records LF. All 46 restored trailing-space lines exactly match XP; command-local
whitespace checking passes without changing Git configuration. Final source
identity equals the candidate used in all actual-body controls. These tests
are bounded rule evidence, not exhaustive V7 mode/RAM-move coverage or a new
interactive DOS/Win3.1/Win95 reproduction.

Fresh cached builds and publication pass without warnings. x86 and x64 each
pass 188/188 background and 8/8 native tests: **392/392 first run**, no retries
or timeout changes. Both compiler timestamps are Nov 13 2026. Mirror and
documentation gates pass; cleanup/process verification follows before delivery.

Published softpc32.exe: 05:24:56; SHA-256 7be919924a411d5f3bc630841d0c6ba0efaeb9c9974c4b5c5bcf26f2ac634c6c.

Published softpc64.exe: 05:28:33; SHA-256 c3b0fa073a077c1224b04b6e67cf814c77855ae87ff3357ee820ae3d6f76cc07.

Sequential audited scope is 286/472; next file 287 base/cvidc/sevid004.c.
S1 remains active, and qualified ABI residuals remain owner decisions.

P33 actual process inventory confirms all task jobs have exited. Exact scratch
cleanup was previewed/applied and Clean-Build -Check passes. Reusable caches,
configuration, guest media and snapshots are preserved.

## P34 file 287 sevid004.c audit and restoration

Original normalized SHA-256 is
2475f9a09749173b9d4dde8b947d20af221ffaed4b0fb1db68ed336270e28959,
verified against the pinned XP SP1 commit. This compiled file defines S2214-
S2245, including V7 backward moves, graphics-controller port handlers and
Generic writes/fills. ev_glue selects dith_evid for V7 dither and gricvid_evid
for supported unchained/chain4 rotation. These are original device operations,
not duplicate Compat behavior or a removable test-only file.

The [initial range map](m9-t100-sevid004-points.tsv) accounts for all 566
non-equal ranges; the [allocation inventory](m9-t100-sevid004-allocations.tsv)
covers all 32 local allocations. Existing #error exclusions are not recorded.

| Point | Initial finding | Counter-review and disposition |
| --- | --- | --- |
| R0288 | 53 native-address accesses wrapped in softpc_gdp_rule_address. | Actual sources are native planes, move/copy pointers and the expanded-data table pointer (GDP field 1328). These are addresses rather than fixed GDP offsets. Restore original accesses, retaining field mapping at the pointer source and unchanged arithmetic. |
| R0289 | Fixed original GDP offsets adapted to native field storage. | Native pointer, callback and IUH layout does not match original 32-bit storage. Keep existing mapped field access and its header; no new state/helper/device implementation. Evidence-qualified residual, not automatic owner approval. |
| R0290 | Native capacities and blanket zeroing replace original malloc byte sizes. | Native slots require their current width; the x64 original-capacity control corrupts the heap. Six entries have narrow stores followed by full IUH reads: S2219/S2222/S2226/S2229 LocalIUH[1], S2238/S2241 LocalIUH[0]. The first group passes RAM source arguments; the latter uses a guest offset in a native address expression in the chained branch. Retain their existing zeroing. This does not qualify arbitrary high-address RAM inputs. |
| R0291 | Other 26 allocations were also changed to calloc. | No required upper-byte initialization is established for those local accesses. Restore malloc semantics, preserving native capacity and all original control flow. Do not apply that restoration to the six mixed-width cases. |

Reversing only the known ABI forms leaves the entire token stream identical to
XP. A first analysis unnecessarily ran character-level matching on equal streams;
it was stopped after direct equality had already proved the result, then the
corrected short analysis completed. No build/test was killed by that action.

Production +77/-77, net 0; tracked tests unchanged. XP-relative diff decreases
from +710/-709 = 1419 lines / 566 ranges to +665/-664 = 1329 lines / 536 ranges:
**90 fewer diff lines, 30 fewer points**. Address-wrapper occurrences and changed
source lines differ because some lines contain multiple accesses. No new mirror
point, helper, second route, device behavior or relocated Compat complexity.

### Actual-body controls and newly exposed original entry defect

The temporary fixture links actual sevid004 bodies with the existing VGA test
using each architecture's real compiler/link options. Four Generic byte/word
writes have independent plane-byte expectations. Six mode1 backward-word moves
check untouched boundaries and compare complete output bytes with unchanged
production. The first backward oracle incorrectly assumed ordinary memcpy
semantics; it failed unchanged code and was withdrawn. The differential result
proves preservation, not complete hardware correctness of the original move.

The first unchanged-production preflight exposed GenericByteFill(count=1)
crashing after its nested GenericByteWrite. A task-only diagnostic in the original
body records LocalIUH non-null before the call and zero after it. GenericByteWrite
calls its continuation as a new crules activation; that activation initializes
CopyLocalIUH to zero, frees the active local storage and restores zero. Fill
then dereferences its lost locals. Both original/current and candidate binaries
reproduce access violation 0xc0000005 on x86 and x64. This is not introduced by
the restoration, and it is not silently counted as a passed test.

The registered chain is gricvid_evid -> GenericByteFill -> GenericByteWrite ->
continuation -> return to Fill. Glue_writes registers fill_byte_ev_glue, and
ccpusas4 stores its address in b_fill_ptrs. However, the currently compiled
c_sas_fills/fillsw/fillsdw loop over individual stores; the source scan finds no
consumer of read_b_fill_ptrs beyond its definition. No actual guest-triggered
failure is claimed. This original same-file nested-entry defect is recorded in
the pending original-defect diff decisions below, not repaired by adding
a forbidden new mirror patch during this original-first diff audit.

| Actual-body control | x86 | x64 |
| --- | --- | --- |
| Current: four write oracles + six backward comparisons and existing VGA tests | pass | pass |
| Address restoration, same controls | pass | pass |
| Minimal address/malloc restoration, same controls | pass | pass |
| Original four-byte allocation capacity | pass | heap corruption 0xc0000374 |
| Candidate malloc allocations filled with 0xa5 | pass | pass |
| Current GenericByteFill count=1, separate process | access violation | access violation |
| Candidate GenericByteFill count=1, separate process | same access violation | same access violation |

Normal controls do not exhaust every mode or RAM-move path; zeroing retention
uses explicit source/ABI evidence. No guest, media, INI, snapshot or sibling is
modified. Temporary probes introduce no production API. Final normalized source
hash: 3b637469838126481b3f0520eb52b7ad3c81325ad1784d2b659dfa2c5cbd0ff3.
Final source matches the tested candidate. Full cached dual qualification is
running; the result and cleanup must be reconciled before delivery.

P34 same-file nested-call scan finds GenericByteFill/GenericByteMove_Fwd
calling GenericByteWrite, and GenericWordFill/GenericWordMove_Fwd/
GenericDwordWrite calling GenericWordWrite. Only the first fill path has the
recorded dynamic failure; the other callers require the pending diff-point review,
not a claim that all have been dynamically reproduced or are guest-reachable.


### P34 final verification and actual-change review

Actual-diff counter-review confirms 77 replacement lines only: 53 address
accesses on 51 lines and 26 allocation statements. No pointer arithmetic, GDP
field selection, continuation or original device behavior changes. All 566
initial ranges are accounted for; six zeroed mixed-width local cases remain.
No complexity is moved to Compat. Qualified residuals are not owner approval.
All 44 restored trailing-space lines match XP exactly. Mirror CRLF and owned
LF are preserved; command-local whitespace checking passes. Final source is
identical to the tested candidate. Production +77/-77 (net 0), tests unchanged.

Both cached builds publish fresh Nov 13 2026 EXEs without warnings. Registered
qualification passes x86 188/188 background + 8/8 native and x64 188/188 + 8/8:
**392/392 first run**, no retries or budget changes. That full-suite result does
not erase the separately reproduced GenericByteFill defect: four current/
candidate negative controls still fail identically. No new interactive guest
reproduction, exhaustive VGA qualification or batch-call integration is claimed.
Mirror/documentation gates and source identity checks pass.

Published softpc32.exe: 05:47:08; SHA-256 775187098e72035b49813fb6f9f64c57e54aa2d43bfd10b9c587df64e971255a.

Published softpc64.exe: 05:50:44; SHA-256 a9d691e6b1daf761435b97f60594ad3ee06b779b7a0b202c158e8747bb53e9c1.

Sequential audited scope is 287/472; next file 288 base/cvidc/sevid005.c.
S1 remains active. Cleanup and actual-process checks precede delivery.

P34 actual process inventory confirms no task-owned worker remains. Exact task
scratch cleanup was previewed/applied; Clean-Build -Check passes. Caches and
user configuration, guest media and snapshots are preserved.

## P35 file 288 sevid005.c audit and restoration

Original normalized SHA-256 is
8afc078a1048d820dde939729c6c79fc61550a5e947e14d1c9aec87ecea8e87d,
matching the pinned XP SP1 manifest. This compiled file supplies S2246-S2277:
Generic moves and selected Unchained mode-0 writes, fills and copies. The
unchained_evid table registers these functions, including S2250/S2262/S2275;
existing ev_glue selection and production machine integration remain intact.
This is not a test-only file or a duplicate Compat device implementation.

The [range map](m9-t100-sevid005-points.tsv) classifies all 506 initial unequal
ranges; the [allocation map](m9-t100-sevid005-allocations.tsv) covers 32 entries.
Existing excluded bodies are not inventoried.

| Point | Initial finding | Counter-review and disposition |
| --- | --- | --- |
| R0292 | 26 native-address accesses use the GDP address wrapper. | Sources are caller RAM pointers, native read/write planes and copied pointer locals, not fixed GDP offsets. Restore original dereferences and original formatting; retain native field mapping at the pointer source. No arithmetic changes. |
| R0293 | Fixed GDP offsets use mapped native fields. | Original fixed 32-bit layout cannot represent the current native pointer/callback/IUH fields. Preserve existing mapping and header, pending owner decision; no new helper or path. |
| R0294 | 32 local allocations use native capacities and calloc. | Original four-byte capacities fail the x64 actual-body control with heap corruption. S2275 LocalIUH[1] stores IU32 then supplies a full IUH RAM-source argument to S2276; retain its zeroing and native capacity. This does not validate arbitrary high-address input pointers. |
| R0295 | Remaining 31 allocations also use calloc. | Local access review finds no corresponding partial-store/full-native-read dependence. Restore malloc, retaining native capacity. Poisoned allocations pass the selected actual-body controls, not exhaustive coverage of every move/write variant. |

After reversing only known ABI forms, the complete token stream matches XP.
The actual change is 57 replacement lines: 26 address accesses and 31 malloc
statements; production +57/-57, net 0; tracked tests unchanged. XP-relative
diff decreases from +690/-689 = 1379 lines / 506 ranges to +664/-663 = 1327
lines / 499 ranges: **52 fewer diff lines and seven fewer points**. No added
mirror point, state, second route or relocated Compat implementation.

### Actual-body verification

Task-local fixtures use each architecture's real compiler/link commands and
actual S2250/S2262 bodies through unchained_evid[0]. Ninety cases combine masks
0/0xf0/0xff, plane enables 0/5/15, odd/even destinations, byte writes and fill
counts 0/1/2/7. Independent plane-byte expectations verify the masked data/latch
result and untouched bytes. Existing VGA/CHAIN2 assertions also execute.

| Variant | x86 | x64 |
| --- | --- | --- |
| Unchanged current | pass | pass |
| Address restoration | pass | pass |
| Minimal address/malloc restoration | pass | pass |
| Original four-byte local allocation capacities | pass | heap corruption 0xc0000374 |
| Candidate malloc storage prefilled 0xa5 | pass | pass |

These controls do not establish every specialized mode, RAM move, guest behavior
or an exhaustive hardware oracle. The previous GenericByteFill defect remains
a pending original-defect diff decision; passing registered tests does not repair it. No guest,
configuration, media, snapshot or sibling repository is changed.

Final normalized source SHA-256:
c7d92bfba8047658fb8a13cde0b552370524be1c80441b32db127d62d21b499d.
The installed source is byte-identical to the tested minimal candidate.
Full qualification and final actual-diff review follow before delivery.

### P35 actual-change counter-review

All 57 replacement lines were inspected and mechanically classified: 26 native
address dereferences plus 31 allocation statements. No arithmetic, branch,
continuation, callback selection or device algorithm changed. All 26 added
trailing-space lines exactly match XP. Mirror CRLF, evidence LF and source
identity checks pass. GDP field mapping remains at native pointer sources;
removing address wrappers does not remove fixed-field translation.

Unmodified and candidate isolated compiles both emit the same existing
sevid005.c:742 -Wswitch-unreachable warning on x86 and x64. This is preserved
mirror code, not a new warning or justification for an extra mirror patch.

### P35 final qualification

Cached x86/x64 builds publish both EXEs. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first run**, with no retries or test
budget changes. Both builds have the one unchanged mirror warning documented
above, and no new warnings. Documentation/mirror gates, source identity and
command-local whitespace checks pass. No new interactive guest validation or
exhaustive device qualification is claimed.

Published softpc32.exe: Nov 13 2026 06:01:49;
SHA-256 e33e13b8b1d29a61e5171a8989a0c2c69fb634ef745be3d5c8f439ea96fb8b55.
Published softpc64.exe: Nov 13 2026 06:05:24;
SHA-256 9e5a7b7939d597a73fb1cd7fd8c95906679f938b6a386d4a7e2032dbb54bfc55.

Sequential audited scope is 288/472; next is file 289 base/cvidc/sevid006.c.
S1 remains active. Qualified retained ABI differences await owner disposition,
but ordered auditing does not pause for approval. Final process verification
and exact scratch cleanup precede delivery.

P35 actual process inventory finds no remaining task worker. Exact scratch
cleanup was previewed/applied and Clean-Build -Check passes. Reusable caches,
user configuration, guest media and snapshots remain intact.

## P36 file 289 sevid006.c audit and restoration

Original normalized SHA-256 is
067262f7ca9ebbb4577264d2872db835b6e6ec45c3f55db777140471b636ee64,
matching the pinned XP SP1 manifest. The actual compiled file defines S2278-
S2309: Unchained moves/copies and WORD writes. unchained_evid registers the
entry functions; their existing copy helpers remain in the same original
implementation. No duplicate Compat device path or test-only file is removed.

The [range map](m9-t100-sevid006-points.tsv) covers 605 initial unequal ranges;
the [allocation map](m9-t100-sevid006-allocations.tsv) covers 32 allocations.
There is no new accounting for existing exclusions.

| Point | Initial finding | Counter-review and disposition |
| --- | --- | --- |
| R0296 | 85 native-address dereferences have GDP address wrappers. | Copy inputs, native read/write plane bases (GDP 1284/1288), and their local pointer arithmetic produce host addresses, not fixed GDP field offsets. Restore original dereferences and formatting, retaining the fixed-field mapping at pointer sources. |
| R0297 | Original fixed GDP offsets use native-field mapping. | Original 32-bit field layout is not current native pointer/callback/IUH storage. Preserve existing mapping/header as an evidence-qualified residual pending owner decision; no new helper/state. |
| R0298 | 32 allocations differ in capacity/zeroing. | Original four-byte capacities corrupt the x64 heap in actual-body controls. Eleven UnchainedByteMove entries store LocalIUH[1] as IU32 and later pass it as IUH RAM-source pointer. Preserve their native capacity and zeroing; this is not proof of support for arbitrary high-address RAM-source input. |
| R0299 | Other 21 allocations use blanket calloc. | Local storage review finds no equivalent partial-store/full-IUH-read requirement. Restore malloc with native capacity. Poisoned candidate allocations pass the selected controls; not every specialized move is dynamically covered. |

All eleven retained cases are named individually in the allocation inventory:
S2278, S2280, S2283, S2285, S2288, S2290, S2293, S2295, S2298, S2300, S2303.
Reversing only the existing ABI forms makes the entire token stream identical
to XP. No unrelated semantic patch is hidden in the file.

Actual diff is 106 replacement lines: 85 address accesses and 21 allocations.
Production +106/-106, net 0; tracked tests unchanged. XP-relative diff falls
from +794/-793 = 1587 lines / 605 ranges to +713/-712 = 1425 lines / 542 ranges:
**162 fewer diff lines, 63 fewer points**. No new mirror point, branch, helper,
second device implementation or complexity moved into Compat.

### Actual-body controls

Task-local test compilation uses each architecture's actual compile/link
commands. S2305 is called through unchained_evid[0].w_write; S2279 is exercised
directly with the original rule-register arguments. Eighteen WORD-write cases
combine masks 0/0xf0/0xff, plane enables 0/5/15 and odd/even destinations.
Forty-eight four-plane copy cases combine both existing source branches,
three masks, odd/even destinations and lengths 0/1/2/7. Independent byte
expectations check copied data, latch/memory masks and untouched regions.
Existing VGA/CHAIN2 assertions also execute in each process.

| Variant | x86 | x64 |
| --- | --- | --- |
| Unmodified current, 66 cases plus existing VGA tests | pass | pass |
| Address restoration | pass | pass |
| Minimal address/malloc restoration | pass | pass |
| Original four-byte allocation capacities | pass | heap corruption 0xc0000374 |
| Candidate malloc storage prefilled 0xa5 | pass | pass |

These controls do not qualify all eleven outer move entries, all specialized
mode combinations or guest-triggered batch reachability. The original nested
continuation defect documented in P34 remains a pending original-defect diff decision. No new
interactive guest reproduction, guest/configuration/media/snapshot change,
production testing API or sibling change is claimed.

Final normalized source SHA-256:
eceedf41812475259448a4fa8a417810b92e8a8e41c8c36d6f6c5e6cc762e5f4.
Installed source is byte-identical to the tested minimal candidate. Actual
counter-review verifies the 106 changed lines, unchanged arithmetic and
callbacks, and exact XP formatting for restored lines. Mirror CRLF is kept.
Full qualification and final evidence reconciliation follow before delivery.

### P36 final qualification and review

Cached dual builds and publication complete. x86 and x64 each pass 188/188
background plus 8/8 native tests: **392/392 first run**, no retries or budget
changes. Each build retains the same original sevid006.c:1065 unreachable-switch
warning present in unmodified actual-body controls; no new warnings. No warning
suppression or extra mirror patch is introduced.

The complete actual diff remains 106 replacements, no algorithm/callback
changes. All 81 restored trailing-space lines match XP exactly. Source identity,
mirror CRLF, documentation/evidence LF and command-local whitespace checks pass.
All 605 initial ranges are classified; qualified residuals are not automatically
approved. Documentation and mirror gates pass. No exhaustive VGA or new
interactive guest validation is claimed.

Published softpc32.exe: Nov 13 2026 06:15:39;
SHA-256 3d96799eccf894e8a14390c308a35e36f01670309d55829b6f35329452cfed5c.
Published softpc64.exe: Nov 13 2026 06:19:17;
SHA-256 2d1af296ff41c74a1e92da8f5e7f18b6552d704273ed3b7d822d5fd2e8cd99e2.

Sequential audited scope is 289/472; next file 290 base/cvidc/sevid007.c.
S1 remains active without a routine approval wait. Exact process verification
and scratch cleanup precede delivery; caches and user assets are preserved.

P36 actual process inventory confirms all task workers exited. Exact scratch
cleanup was previewed/applied and Clean-Build -Check passes. Reusable caches,
user configuration, guest media and snapshots remain intact.

## P37 file 290 sevid007.c audit and restoration

Original normalized SHA-256 is
a0d23a551a0a1d70d0df184bb86fab7f870d6d155ac6a9dc5936716c448e06c7,
matching the fixed XP SP1 manifest. This compiled file supplies S2310-S2341:
Unchained WORD writes, fills, moves and their original copy helpers. The
unchained_evid table registers the public rule entries; their normal selection
and all caller wiring remain unchanged. No duplicate Compat device code or
test-only mirror file is removed.

The [range map](m9-t100-sevid007-points.tsv) covers all 717 initial unequal
ranges; the [allocation map](m9-t100-sevid007-allocations.tsv) covers 32 entries.
Existing excluded bodies are not recorded.

| Point | Initial finding | Counter-review and disposition |
| --- | --- | --- |
| R0300 | 62 native-address accesses use GDP address wrappers. | Inputs are native source pointers and native read/write plane bases plus original offsets, not GDP field identifiers. Restore original dereferences/formatting; retain field translation where the native pointer is obtained. |
| R0301 | Fixed GDP fields use native mapping/header. | Original 32-bit field layout differs from native IUH, pointer and callback layout. Preserve the existing adapter as an evidence-qualified residual pending owner disposition, without new helpers or device logic. |
| R0302 | 32 allocations use native capacities and calloc. | Original four-byte capacities corrupt the x64 heap in actual-body controls. S2330/S2333/S2335/S2338/S2340 store LocalIUH[1] as IU32 then read it as IUH for RAM-source arguments; retain their zeroing. No arbitrary high-address RAM-source support is claimed. |
| R0303 | Other 27 entries also use calloc. | Local access review finds no corresponding upper-byte dependency. Restore malloc with native capacity; selected poisoned-storage controls pass. No claim that all specialized entry paths were dynamically exercised. |

Reversing the known ABI forms leaves the complete token stream identical to XP.
Actual diff is 89 replacement lines: 62 address accesses and 27 allocations.
Production +89/-89 (net 0), tracked tests unchanged. XP-relative diff drops
from +992/-991 = 1983 lines / 717 ranges to +934/-933 = 1867 lines / 683 ranges:
**116 fewer diff lines, 34 fewer points**. No new mirror point, algorithm,
branch, state or relocated Compat complexity is introduced.

### Actual-body controls and corrected fixture assumption

The task fixture compiles with actual x86/x64 build commands and links real
rule bodies. S2317 runs through unchained_evid[0].w_fill: 72 combinations of
three masks, three plane enables, odd/even addresses and lengths 0/1/2/7 have
independent repeated-WORD and untouched-region expectations. S2334 is called
directly for 48 four-plane WORD-copy cases: two source branches, three masks,
two destination parities and four lengths. Existing VGA/CHAIN2 assertions run
in every successful control.

The first unchanged x86 preflight failed the copy oracle, not the fill checks.
A diagnostic recorded mask=0, RAM=false, count=1, second byte/plane0:
actual 7 versus expected 59. The fixture had assumed the source-plane latch
advanced per byte. The original WORD-copy body reuses one plane group for both
bytes and advances it per word. That assumption was withdrawn, not repaired
in production or silently reported as a passed hardware oracle.

Final copy checks retain independent RAM-source/full-mask results and untouched
boundaries. All 48 complete 4096-byte output images are also compared byte-for-
byte against unchanged production per architecture. Thus partial-mask video
source cases prove preservation, not an exhaustive VGA hardware specification.
The task does not establish a new guest-visible defect from the initial oracle.

| Final control, 120 cases plus existing VGA tests | x86 | x64 |
| --- | --- | --- |
| Unchanged current | pass | pass |
| Address restoration | pass | pass |
| Minimal address/malloc restoration | pass | pass |
| Original four-byte capacities | pass | heap corruption 0xc0000374 |
| Candidate malloc filled with 0xa5 | pass | pass |

These results do not cover every specialized move or establish guest-triggered
batch reachability. The P34 original nested-call defect stays a pending original-defect diff
decision. No guest, configuration, snapshot, media or sibling is changed; no new
production test interface is introduced.

Final normalized source SHA-256:
ab931c53f7465b3e72943a76a0d045544ae3e814503aca97755fbf6b44d61f39.
Installed source is byte-identical to the tested minimal candidate. Actual
counter-review confirms unchanged field selection/arithmetic/callbacks and
mirror CRLF. Full qualification follows before delivery.

### P37 first regression result and controlled retry

The first x86 background run finishes 187/188: emulator.verifier-negative
hits its existing 60-second budget (CTest displays 58.88 seconds). The other
187 tests pass, including Types and runtime boot. The fixture left at timeout
contains the expected private-path negative include ../session/control.h; no
corpus assertion failure is reported. That script launches repeated CMake
negative checks and does not execute the modified C-VID body.

After the first runner terminates, the same test runs alone with its unchanged
60-second budget and passes in 25.81 seconds. No source change, timeout change
or suppression is used. This establishes a completed retry, not the exact host
scheduling cause or a claim that the first aggregate run passed. x86 native
qualification then passes 8/8. x64 qualification follows independently.

Both architectures' successful copy controls produce identical 196608-byte
captures, SHA-256 bbb57146913550f0e8ceea7faf72f3d6266a57561648226d99e63b0f51a1fcb7.
Address/minimal/poison candidates match their unchanged baseline bytes.

### P37 final qualification and actual-change review

x64 aggregate testing also finishes 187/188, with only emulator.verifier-negative
hitting its unchanged 60-second budget (60.30 seconds displayed). It then passes
alone in 25.35 seconds without any source/budget change. x64 native tests pass
8/8. Across architectures all 392 registered tests have passing evidence, with
**two aggregate timeouts and two successful targeted retries**. This is not a
first-run all-green claim. Exact host scheduling cause is unproved; the shared
test's aggregate timing margin is recorded separately in TODO for investigation,
not repaired or weakened within this mirror-restoration scope.

Both cached builds/publications succeed and retain only the same original
sevid007.c:849 unreachable-switch warning seen in baseline compiles. No new
warning, suppressed check or product timeout adjustment is introduced.
Final source equals the focused-tested candidate. All 89 replacement lines
remain the declared 62 native accesses and 27 allocation statements; all 58
restored trailing-space lines match XP exactly. Fixed-field translation,
original arithmetic, callbacks and device control flow stay unchanged.
Mirror CRLF, evidence LF, source identity and command-local whitespace checks
pass. Documentation/mirror gates pass. No complete hardware or new interactive
guest qualification is claimed. Qualified ABI residuals await owner decisions.

Published softpc32.exe: Nov 13 2026 06:30:35;
SHA-256 ea1055a9367779823024cf2e12f807fbba3d1d93225bc2f3145d00eaa520ebe0.
Published softpc64.exe: Nov 13 2026 06:35:10;
SHA-256 d36d3352a1879d908f481115b39f2ae983dcf9544d4a9d09d8922ba0c357080a.

Sequential audited scope is 290/472; next file 291 base/cvidc/sevid008.c.
S1 remains active. Process verification and scratch cleanup precede delivery;
user assets and reusable caches remain intact.

P37 actual process inventory confirms no task worker remains. Exact scratch
cleanup was previewed/applied and Clean-Build -Check passes. Reusable caches,
configuration, guest media and snapshots are preserved.

## P38 file 291 sevid008.c audit and restoration

Pinned XP normalized SHA-256:
011fc67217c7a176da5ef1555db4416f343f53df2c6af059d5a51c3421712ff3.
Initial local normalized SHA-256:
53b23c5956355e74d95eb7462391a012cca19e4dc823f0f7204d1c206e1db33b.
The [565-range inventory](m9-t100-sevid008-points.tsv) covers every initial
changed range. The [32-allocation inventory](m9-t100-sevid008-allocations.tsv)
records native capacity and mixed-width slots.

Initial review: R0304 restores 76 native-address expressions; these addresses
already refer to native pointers, not GDP offsets. R0305 retains actual GDP
field translation. R0306 retains native-sized local capacities and seven
zeroed move allocations: S2343/S2345/S2348/S2350/S2353/S2355/S2358 write
LocalIUH[1] as IU32 and subsequently consume its complete IUH representation.
R0307 restores malloc at the other 25 sites. Necessary residuals remain
qualified owner decisions, not automatic approval. No extra approval wait
blocks subsequent ordered work.

Counter-review: reversing only the enumerated ABI adaptations restores the
complete XP token stream. No new device arithmetic, branch or callback is
introduced. The replacement preserves 32 native capacities, including seven
mixed-width zeroed slots. Production is +101/-101, net zero; tests unchanged.
XP-relative scope falls from +758/-757, 565 ranges to +686/-685, 505 ranges:
144 fewer diff lines and 60 fewer points, not a source-line reduction claim.

Actual-body controls run unchanged, address-only, minimal, original-capacity
and poisoned-malloc variants on x86/x64. Each successful variant completes
48 independent four-plane WORD-copy cases, 90 DWORD result-preservation cases
and the existing 96 CHAIN2 checks. Candidate captures match the unchanged
baseline byte-for-byte. Poisoned candidate locals pass with the seven necessary
calloc sites retained. Original four-byte allocation capacities pass x86 but
fail x64 with heap corruption 0xc0000374; all other variants pass both widths.
These controls establish the selected cases, not every specialized rule.

### Original DWORD write and fill defect

The unchanged actual body and candidate both write 11 22 at the starting
address and 33 44 at +8/+9, leaving +2/+3 untouched when asked to write
0x44332211. Fill count one shows the same mismatch. S2360 and S2372 pass
start+8 to their second WORD operation; this is present in pinned XP, not
introduced by restoration. The preservation controls intentionally distinguish
equal existing output from correct contiguous DWORD semantics.

evidfunc.c registers these generated entries. However Glue_writes registers
byte/word operations only; write_dword_ev_glue has no production caller in
the repository search. Current CCPU c_sas_storedw reaches phy_w32, which uses
two phy_w16 calls at start and start+2. c_sas_fillsdw loops those stores.
Thus no current guest-triggered instance of the generated DWORD fault has
been established. Repair remains a pending diff-point decision before enabling these
entries; this reduction-only audit does not add a new mirror patch.

Final source SHA-256 (LF normalized):
6d4f22abde30c30c376aaa90600675ba1fdbe6e42716fac42e51dd8bd4301ae9.
Installed source is byte-identical to the tested minimal candidate and remains
CRLF. Cached dual builds and full qualification are running; P38 is not yet
a completed delivery.

### P38 final qualification and review

Both cached builds and publications succeed. Each architecture passes 188/188
background tests plus 8/8 native audio/desktop/package tests: 392/392, first
run, without budget changes or suppressed checks. Both build logs contain no
warnings. Actual-change review classifies all 101 changed lines as the stated
76 native accesses and 25 allocation statements; tested candidate identity,
CRLF mirror format and manifest checks pass. No full specialized-rule matrix
or fresh interactive guest reproduction is claimed.

The 368640-byte DWORD preservation captures match on both architectures:
SHA-256 1264a241a1eed501b4f531ad3209b8c5f07a0c024ef001535f98fc7b84e3add7.
This preserves the demonstrated original defect too; it is not proof of
correct DWORD semantics. That issue is recorded in the pending original-defect diff decisions below.

Published softpc32.exe: Nov 13 2026 06:48:38;
SHA-256 7d5856c16267622101b84e411b179689e0c26622ac4de8bebcd0f33cbe91b809.
Published softpc64.exe: Nov 13 2026 06:52:16;
SHA-256 8e2062b508c81b6eaa76a5e84abe2538f20b2edf15364188d642480cfeb28c3d.

Sequential audited scope is 291/472; next file 292 base/cvidc/sevid009.c.
S1 remains active, with qualified ABI decisions distinguished from delivered
restorations. No routine approval is awaited to continue.

P38 process inventory confirms no task worker remains. Exact disposable scratch
cleanup was previewed/applied and Clean-Build -Check passes. The two reusable
caches, configuration, guest media and snapshots remain intact.

## P39 file 292 sevid009.c audit and restoration

Pinned XP normalized hash is
1aeffcd2a58d19189c680ef1b36016c97fc27f3ada9d1545122ea603e50030ee;
pre-audit local hash is
ab2f1a3bfa3b19a55d380db933fef21ede34e29f296c81996073a5a38c5c99b2.
The [342-range map](m9-t100-sevid009-points.tsv) covers the complete initial
diff. The [allocation inventory](m9-t100-sevid009-allocations.tsv) covers all
32 functions. No excluded body is introduced into this accounting.

Initial analysis: R0308 native accesses refer to framebuffer storage or the
native expansion table, not fixed GDP offsets. R0309 actual GDP accesses
still require field translation. R0310 original local capacities use four
bytes per slot, insufficient for native IUH on x64. Unlike previous move
files, all 32 local blocks here use matching narrow reads/writes or initialize
native slots before use; no narrow-store/native-read slot needs zeroing.
R0311 restores malloc while retaining the native capacities.

Counter-review: reversing the enumerated ABI forms gives the entire pinned
XP token stream. The supported Ninja core compiles this file; evidfunc.c
selects its write-mode 1 BYTE/WORD and mode 2 BYTE entries through the existing
memory-handler route. No alternative device implementation is introduced.
The actual candidate changes 40 lines: eight native address expressions and
32 malloc statements. Tests and shared packages remain unchanged.

The all-nine-address restoration would reduce XP diff lines 889 -> 871 but
increase changed ranges 342 -> 343 by splitting a previous range at line 2780.
It is rejected under the owner's no-increase rule, without requesting an
exception. Keeping only that existing expansion-table access wrapper yields
342 ranges and 873 diff lines (+437/-436): 16 fewer lines, no extra points.
The remaining wrapper is constrained by that rule, not falsely justified as
an ABI necessity. Native-address restoration alone and combined malloc
restoration have the same diff metrics. Production is +40/-40, net zero.

Actual-body controls use the existing VGA fixture and original rule functions:
432 independent pixel cases cover mode 1 BYTE/WORD and mode 2 BYTE writes,
all 16 nibble values, three masks, three plane-enable patterns, alternating
address parity, full 4096-byte output and untouched boundaries. Original,
minimal and 0xa5-poisoned malloc variants pass on x86 and x64, along with the
existing 96 CHAIN2 cases. Restoring original four-byte allocation capacities
passes x86 and corrupts the x64 heap (0xc0000374). No permanent test-only
production API is added. These controls do not claim a complete specialized
DWORD fill/move matrix or a guest-triggered reproduction of those entries.

Installed candidate SHA-256, normalized LF:
fe021542acfc229b7530ca438bf5bcb349340cebe3c6dd4c9bcd7b70ae4f228e.
It is byte-identical to the tested minimal source and retains CRLF. Full
cached dual-build/publication/regression qualification follows before delivery.

### P39 final qualification and actual-change review

Both cached builds and publications succeed. Each architecture passes 188/188
background tests plus 8/8 native audio/desktop/package tests: 392/392, first
run, without budget changes or suppressed checks. Both builds retain the same
sevid009.c:537 unreachable-switch warning also present in the unchanged
actual-body controls; no new warning is introduced. Actual review classifies
all 40 changed lines as eight native addresses and 32 malloc statements.
Tested candidate identity, mirror CRLF, manifest/documentation gates and
command-local whitespace checks pass. No specialized-rule full matrix or new
interactive guest qualification is claimed. Qualified necessary ABI residuals
remain owner decisions, distinct from the explicit no-increase constraint.

Published softpc32.exe: Nov 13 2026 07:02:06;
SHA-256 1ca4901c6ff21b9bc7dd4549aedd68c7d8ac3f5af4c5a7dc0989a10b0be7a025.
Published softpc64.exe: Nov 13 2026 07:05:46;
SHA-256 a41377f174e0018730440b93f9e957d57a6a40e674da995f039fd9606a091824.

Sequential audited scope is 292/472; next file 293 base/cvidc/sevid010.c.
S1 remains active; ordered work does not wait for routine approval.

P39 actual process inventory confirms no task worker remains. Exact scratch
cleanup was previewed/applied; Clean-Build -Check passes. Reusable caches,
configuration, media and snapshots are preserved.

## P40 file 293 sevid010.c audit and restoration

Pinned XP normalized hash:
777f007854e409742de11bd83d4e4b7bc6df0fbde8729d88d8706e9f83dc91cf.
Initial local normalized hash:
f1caddd84ebef15e549e317578e9d741a412aa9e1b9c5091b3e490d2251b8cf8.
The [629-range map](m9-t100-sevid010-points.tsv) accounts for the initial diff;
the [32-allocation map](m9-t100-sevid010-allocations.tsv) separates capacities
and mixed-width locals. No existing excluded body is counted.

Initial review: R0312 native pointers refer to plane data or expansion-table
entries, whereas R0313 actual GDP fields still require translation. R0314
native local capacities remain necessary; S2413/S2416/S2418/S2421/S2431/
S2434/S2436 also retain zeroing because LocalIUH[1] receives IU32 then is
consumed as native IUH in the RAM-move branch. R0315 restores malloc for the
other 25 allocations. The original rule arithmetic and callback order remain.

Counter-review: reversing the enumerated ABI adaptations reproduces the
entire pinned XP token stream. The selected core compiles this file and the
existing evidfunc tables select its mode-2 operations. Actual source changes
are exactly 92 native-address expressions and 25 allocations: +117/-117,
net zero. Tests and public components are unchanged. Qualified necessary ABI
residuals are not automatic owner approvals.

Restoring all 93 address expressions would yield 632 changed ranges instead
of 629, violating the no-increase rule. Candidate selection also checks line
count: a different retained wrapper reduced range count but increased lines
to 1887 and was rejected. The selected candidate leaves only the existing
framebuffer-store wrapper at current line 3228. Using the same full-file
SequenceMatcher method as previous batches, XP diff changes from +865/-864,
629 ranges to +857/-856, 608 ranges: 16 fewer diff lines and 21 fewer points.
Repeated generated statements affect matching; these are diff metrics, not
a claim that 16 source lines were deleted. The remaining wrapper is retained
under the explicit no-increase constraint, not described as necessary ABI.

### P40 actual-body controls and fixture correction

The first unchanged-source test failed at BYTE fill with mask=0, planes=0,
count=1: offset 1024 contained 17 instead of the fixture's expected 90.
Inspection showed the fixture set plane_enable_mask (GDP1324), but the
per-plane fill dispatch checks plane_enable (GDP1320). This was an incomplete
fixture setup, not evidence of a production defect. The corrected fixture
saves, sets and restores both representations coherently; the initial failure
is not hidden as a passing run.

With corrected setup, unchanged, minimal and poisoned-malloc actual bodies
pass x86/x64. Each runs 1440 independent full-buffer pixel cases: mode-2
BYTE/WORD write and BYTE/WORD fill, 16 nibbles, three bit masks, three plane
sets, alternating parity, zero/one/two/seven fill counts and untouched bounds.
Existing CHAIN2 checks also pass 96/96. Restoring original four-byte allocation
capacities passes x86 but corrupts the x64 heap (0xc0000374). Poisoning the
25 restored malloc blocks with 0xa5 passes with the seven mixed-width calloc
blocks retained. This does not establish every specialized move variant or
a new interactive guest reproduction. No test-only production API is added.

Final source hash, LF normalized:
ea9c578eb771cc49995a7fd74b1491cec9e02630c1ec9621201a9b1c559be95c.
Installed source is byte-identical to the tested candidate and retains CRLF.
Full cached build/publication/regression qualification follows before delivery.

### P40 final qualification and actual-change review

Both cached builds and publications succeed. Each architecture passes 188/188
background tests plus 8/8 native audio/desktop/package tests: 392/392 on the
first full run, without changing budgets or suppressing checks. Both builds
retain the original sevid010.c:909 unreachable-switch warning, also present
in unchanged control compiles. No new warning is introduced. Actual review
classifies all 117 replacements as the declared 92 native accesses and
25 malloc statements; tested-source identity, CRLF, mirror/documentation
gates and command-local whitespace verification pass. This does not turn
the initial incorrect fixture run into a pass or claim complete move coverage.
The reduced differences and qualified remaining ABI decisions stay separate.

Published softpc32.exe: Nov 13 2026 07:17:58;
SHA-256 db168a14ed63b693c0ef5b037acba3ceae5c6289936256587cf98257d42b0239.
Published softpc64.exe: Nov 13 2026 07:21:34;
SHA-256 b8ecd3705e5a061571343693b2bd1e38241bf208742030d0dac59aeb3726021c.

Sequential audited scope is 293/472; next file 294 base/cvidc/sevid011.c.
S1 remains active; no routine approval is awaited.

P40 process inventory confirms no task worker remains. Exact disposable scratch
cleanup was previewed/applied and Clean-Build -Check passes. Reusable caches,
configuration, guest media and snapshots remain intact.

## Pending original-defect diff decisions

Owner moves these discoveries out of TODO into this audit's pending diff-point
list. D001-D006 identify proposed differences, not existing local patch ranges;
they do not inflate the measured XP diff or the restored-point counts. None
is repaired or approved for a new mirror patch. Continue ordered auditing
without waiting; the no-increase constraint remains in force.

| Decision | Source and proposed difference | Evidence and reachability | Status |
| --- | --- | --- | --- |
| D001 | base/cvidc/sevid004.c, GenericByteFill nested continuation: a potential change to preserve caller locals across GenericByteWrite. Exact minimal patch is not yet established. | P34 actual bodies fail with 0xc0000005 on x86/x64 before and after restoration; nested continuation loses LocalIUH. Current CCPU fills use individual stores; no guest batch consumer established. Related nested callers were source-scanned, not dynamically proven faulty. | Pending diff decision; original behavior unchanged. |
| D002 | base/cvidc/sevid008.c, S2360/S2372: potential second-WORD offset correction from +8 to +2, with multi-DWORD fill ordering still to verify. | P38 unchanged/candidate actual bodies put 33 44 at +8/+9 instead of +2/+3 on both architectures. Current CCPU DWORD stores use two WORD operations at +0/+2; no guest use of these generated DWORD entries established. | Pending diff decision; original behavior unchanged. |

| D003 | base/cvidc/sevid023.c, Chain4DwordFill (S2844 demonstrated): proposed correction of multi-element WORD-pass overlap. Minimal implementation is not yet established. | P54 actual bodies on x86/x64 request four copies of 0x44332211 but produce 11 22 33 44 33 44 33 44 33 44 followed by six untouched bytes. Current, candidate and poisoned candidate agree. Pinned XP uses the same two count-wide WORD fills at offsets +0/+2. c_sas_fillsdw loops over c_sas_storedw; no production caller of fill_dword_ev_glue was found. No guest symptom established. | Pending diff decision; original behavior unchanged. |

| D004 | base/cvidc/sevid025.c, CHAIN4 mode-2 WORD/DWORD writes: plane-selection discrepancy; a possible correction would select the addressed plane's expanded byte rather than always the low byte. No patch is admitted. | P57 real setWritePointers selects all four mode-2 variants. On x86/x64, offset 0x100, WORD 0x0201 writes ff 00, whereas the per-plane expansion expectation is ff ff; DWORD 0x08040201 writes ff 00 00 00 rather than ff ff ff ff. Current, restored and poisoned-restored bodies agree. Pinned XP stores the low byte after lookup expansion. write_word_ev_glue is in the production video write vector. This is a source/fixture discrepancy, not a reproduced guest failure or completed hardware conformance investigation. | Pending diff decision and hardware/guest applicability verification; original behavior unchanged. |

| D005 | base/system/timer.c, issueIREQ0 existing-active-event branch: clamp the newly computed zero int_delay to one before assigning timer_multiple_delay; isolated candidate adds two lines, no production edit admitted. | P115 uses real Machine/PIT ports, two heartbeats and actual quick-event dispatch. Both current architectures reach divisor=1, mode=2, delay=0, active=1, more=65525, then exit 0xc00000fd. The original active-event calculation is unchanged; the existing first-event guard does not cover it. Two-line isolated candidate passes all nine mode/divisor combinations on each architecture. | Pending new diff decision; not implemented, not TODO, no observed real-guest crash claimed. |

| D006 | base/video/egwrtm12.c, ega_mode2_chn_b_write easy even-address path: proposed plane-2 enable test 8 to 4. | P126 actual Machine/BIOS mode 3 and GC mode 2 select the original BIOS writer. With sequencer plane mask 4, no logical operation and value 4 at offset 0, both architectures leave plane 2 at 00 rather than ff. The original XP condition tests bit 8 while writing plane 2; logical and WORD paths test bit 4. This line is not an existing local diff. | Pending new diff decision; no production patch or full mask matrix, no guest symptom claimed. |

The P34/P38 sections retain the detailed controls and caller evidence. This
transfer does not claim a guest symptom, a completed fix, or authority to add
a new diff. The unrelated verifier-negative timing issue remains in TODO.

## P42 file 294 sevid011.c audit and restoration

Initial review: R0316 restores 52 native-address expressions; R0317 retains
actual GDP field translation. R0318 retains native local capacities and zeroing
for S2439/S2461/S2464/S2466/S2469, which store LocalIUH[1] as IU32 and read it
as native IUH in the RAM-source branch. R0319 restores malloc for the other
27 allocations. Original algorithms and callback order are unchanged.

Counter-review: reversing the enumerated ABI forms reproduces the pinned XP
full token stream. CMake's selected C-VID sources compile this file; evidfunc
registers its mode-2/mode-3 entries. The [482-range map](m9-t100-sevid011-points.tsv)
and [32-allocation inventory](m9-t100-sevid011-allocations.tsv) account for the
initial differences and remaining mixed-width locals. Qualified ABI residuals
remain pending owner disposition, not automatically approved.

Exactly 79 lines change: 52 native address expressions and 27 allocation calls,
production +79/-79, net zero. Tests are unchanged. With the same full-file
SequenceMatcher method, XP diff falls from +617/-616, 482 ranges to +570/-569,
457 ranges: 94 fewer diff lines and 25 fewer ranges. Neither metric increases.

Unchanged, minimal and poisoned-malloc actual bodies pass on x86/x64. Each
executes 1440 independent mode-3 BYTE write/fill pixel cases across data,
bit masks, plane enables, address parity, zero/one/two/seven fill counts and
untouched full-buffer bounds, plus existing CHAIN2 checks (96/96). The fixture
sets both plane-enable bitmap and expanded mask. Original four-byte allocation
capacities pass x86 but corrupt the x64 heap (0xc0000374). Only the 27 restored
malloc blocks are poisoned with 0xa5; the five mixed-width calloc blocks remain.
This does not prove every specialized move or a new interactive guest scenario.
No test-only production API or new device fix is introduced.

Installed source is byte-identical to the tested candidate, retaining CRLF.
Final LF-normalized SHA-256: ab1a94696436969ea900903837978d8e6c30b7dff8afc29296522ff365ade9a2.
Dual cached build/publication and full regressions remain to be completed;
file 294 is not yet counted as delivered.

P42 also finds the D002 source pattern in sevid011.c S2441-S2448: original
and current mode-2 DWORD write/fill wrappers pass start+8 to the second WORD
operation. This extends D002's source-review scope, not its dynamic proof:
these eight entries were not exercised by the mode-3 BYTE oracle. No new
mirror fix or TODO is introduced, and no guest-triggered symptom is claimed.

### P42 final qualification and actual-change review

Both cached builds and publications succeed without warnings. Each architecture
passes 188/188 background tests plus 8/8 native audio/desktop/package tests:
392/392 on the first full run, with unchanged test budgets. Actual-change review
confirms precisely 52 address and 27 allocation replacements, no new behavior,
no shared-component or test-source changes. Candidate byte identity, CRLF,
manifest provenance, mirror/documentation gates and command-local whitespace
checks pass. Specialized moves and guest-driven DWORD paths are not newly
qualified by the selected mode-3 oracle; D002 remains a pending decision.

Published softpc32.exe: Nov 13 2026 07:35:42;
SHA-256 c809c876d1c199b252e1364e96a07efc1e7d4e5e3f6d60a79cae1d24316d5305.
Published softpc64.exe: Nov 13 2026 07:39:23;
SHA-256 f8f1ebe135a2c9e4b26f18300f76e7570b6d3ae662887a6fc9be4fa605370018.

Sequential audit is 294/472; next is file 295 base/cvidc/sevid012.c.
S1 remains active. Qualified residual decisions are not closed by these tests.

P42 process inventory confirms no task worker remains. Exact scratch cleanup
was previewed and applied; Clean-Build -Check passes. Reusable caches,
configuration, media and snapshots remain unchanged.

## P43 file 295 sevid012.c audit and restoration

Initial review: R0320 restores 62 native plane/table address expressions;
R0321 retains actual GDP field translation. R0322 retains native local
capacities, with zeroing only for S2479/S2482/S2484/S2487/S2501: their
LocalIUH[1] receives IU32 and is consumed as native IUH for RAM-source input.
R0323 restores malloc in the other 27 entries. No device algorithm changes.

Counter-review: reversing the identified ABI forms reproduces the full pinned
XP token stream. CMake's selected C-VID target compiles this file, and evidfunc
registers its mode-3 operations. The [550-range map](m9-t100-sevid012-points.tsv)
and [32-allocation inventory](m9-t100-sevid012-allocations.tsv) account for the
original differences. Necessary ABI residuals remain qualified owner decisions,
not implicitly approved. The source changes exactly 89 lines (+89/-89, net zero):
62 native addresses and 27 allocation calls. No test source is changed.

The same full-file SequenceMatcher measurement gives XP diff +695/-694,
550 ranges before, +638/-637, 509 ranges after: 114 fewer diff lines and
41 fewer ranges. Both required metrics decrease.

Actual unchanged, minimal and poisoned-malloc bodies pass on x86/x64. Each
runs 1440 independent mode-3 WORD write/fill cases with different low/high
bytes, three masks, three plane sets, address parity, zero/one/two/seven
counts and full-buffer untouched bounds; existing CHAIN2 tests pass 96/96.
Both plane-enable bitmap and expanded mask are set coherently. Original
four-byte capacities pass x86 but produce x64 heap corruption (0xc0000374).
The 27 restored malloc blocks also pass when filled with 0xa5; five mixed-width
calloc blocks remain. These controls do not qualify every specialized move
or constitute new interactive guest evidence.

D002 source review extends to S2489-S2496 in this file: pinned XP and local
mode-3 DWORD write/fill wrappers pass start+8 to the second WORD operation.
Those entries were not exercised by this WORD oracle, so no new dynamic or
guest reachability claim is made. Record this in pending diff decisions only;
no new repair patch or TODO is introduced.

Pinned XP normalized SHA-256:
ba9e4272cc22e491cac6362596c5bcecb5629309f1b90a1d986844bbfdfc1c10.
Initial local normalized SHA-256:
983045c2221014ddb6e02a473bef176eafd7761616054e95cfc4d3d6f79302d5.
Final normalized SHA-256: 092ecf4a6541efda1f3d1019adda2e556f8a941701cdb17ba915cd353f1a7566.
Installed source is byte-identical to the tested candidate and remains CRLF.
Full cached dual builds/publication and regressions follow before delivery.

### P43 final qualification and actual-change review

Both cached builds/publications succeed without warnings. Each architecture
passes 188/188 background tests plus 8/8 native audio/desktop/package tests:
392/392 on the first full run, with no timeout changes or suppressed checks.
Actual-change review verifies exactly 62 native address and 27 allocation
replacements. Installed bytes match the tested candidate; CRLF, manifest
provenance, mirror/documentation gates and command-local whitespace checks pass.
No test or public component source is changed. The selected WORD oracle does
not claim complete specialized-move coverage or guest-triggered DWORD proof.

Published softpc32.exe: Nov 13 2026 07:50:27;
SHA-256 d8f1f3ff0f7d9585e1d9525f14a1c201fece2bdc065795d8012ad194ac89d2ae.
Published softpc64.exe: Nov 13 2026 07:54:16;
SHA-256 c4d8f9d4f1a04e1aab365d25e205bf65337a324c38016318324b202059db28e9.

Sequential audited scope is 295/472; next is file 296 base/cvidc/sevid013.c.
S1 remains active; qualified residual decisions are not closed by passing tests.

P43 process inventory confirms all task workers exited. Exact scratch cleanup
was previewed/applied and Clean-Build -Check passes. Reusable caches,
configuration, media and snapshots are preserved.

## P44 file 296 sevid013.c audit and restoration

Initial review: R0324 restores 81 native source/plane address expressions;
R0325 retains actual GDP field translation. R0326 retains native capacities
and zeroing in 12 outer move entries: S2504/S2506/S2509/S2511/S2514/S2516/
S2519/S2521/S2524/S2526/S2529/S2531. Their LocalIUH[1] receives IU32 and is
consumed as native IUH in the RAM-source branch. R0327 restores malloc for
the other 20 allocations. Original move direction and callback order remain.

Counter-review: reversing the enumerated ABI forms reproduces the complete
pinned XP token stream. Selected C-VID CMake sources compile this file;
evidfunc registers its backward mode-0 moves. The [581-range map](m9-t100-sevid013-points.tsv)
and [32-allocation inventory](m9-t100-sevid013-allocations.tsv) account for
initial differences. Necessary residuals are qualified pending owner decisions,
not automatically approved. Exactly 101 lines change (+101/-101, net zero):
81 native-address expressions and 20 allocation calls. Tests remain unchanged.

Using the same full-file SequenceMatcher metric, XP diff changes from
+753/-752, 581 ranges to +676/-675, 506 ranges: 154 fewer diff lines and
75 fewer ranges. No new mirror patch is introduced.

### P44 actual-body controls

Unchanged, minimal and poisoned-malloc bodies pass on x86/x64. Each executes
1440 captures across 12 real copy helpers (six four-plane, six one-plane),
RAM/video sources, three masks, zero/one/two/seven counts, address parity and
four one-plane shifts. Forty-eight four-plane copy cases additionally use an
independent full-buffer expected result with untouched-byte checks. The other
captures establish preservation, not complete hardware correctness. Existing
CHAIN2 tests also pass 96/96. The original RAM-source helper traversal is
preserved; no stronger backward-order contract is inferred from its name.

All successful capture files are byte-identical, 5898240 bytes, SHA-256
1904bd411833cd4b91d1bf22591cd2871ac5308a81cb13c53f5e3898f011be39.
Restoring original four-byte capacities passes x86 but corrupts the x64 heap
(0xc0000374). Poisoning only the 20 restored malloc blocks with 0xa5 passes;
the 12 mixed-width calloc blocks remain. No complete outer-move matrix,
arbitrary high-address RAM input or new interactive guest reproduction is
claimed. No test-only production interface is added.

Pinned XP normalized SHA-256:
b6c22a09b7cb906143dac06d291bad67fed63044c54a31914bf0dc7c52102c09.
Initial local normalized SHA-256:
c6e673b16d8135717cb2d6f1f1fc34d13d685f0cfc84b5c198ba77525ff8071d.
Final normalized SHA-256: d949492f6d9d965047a81d007ba07b4034f868ac5263e667cd3ab3a7d42fa8bd.
Installed source matches the tested candidate byte-for-byte and retains CRLF.
Full cached dual build/publication/regression qualification follows.

### P44 final qualification and actual-change review

Both cached builds/publications succeed without warnings. Each architecture
qualifies 188 background tests plus 8 native audio/desktop/package tests,
392/392 after the isolated retries documented below, with unchanged budgets.
The initial aggregate timeout is retained and is not called a first-run pass.
Actual review confirms
exactly the 81 native address and 20 allocation changes, with no shared-component
or test-source changes. Source/candidate byte identity, CRLF, provenance,
mirror/documentation gates and command-local whitespace checks pass. The
capture controls establish preservation for the exercised helpers, not complete
original algorithm correctness or interactive guest coverage. Necessary ABI
residuals remain qualified pending decisions.

Aggregate verifier-negative timeout architectures: x86.
Only that test was retried alone at its unchanged 60-second budget; each retry passed.
The existing TODO timing debt remains, with no root cause asserted from this retry.

Published softpc32.exe: Nov 13 2026 08:05:02;
SHA-256 9f9b7120a7701716c3d9a03626c64efd31b2f6dfd4f4fd5676fda9392eaf37ac.
Published softpc64.exe: Nov 13 2026 08:09:12;
SHA-256 105712780c8106aaa190949600f7b595a0c03d18d5d74326a8bb3c55a1a9fd76.

Sequential audited scope is 296/472; next is file 297 base/cvidc/sevid014.c.
S1 remains active; pending decisions are not closed by the passing tests.

P44 timing evidence: x86 aggregate verifier-negative timed out at 60.01 seconds;
the isolated same-budget retry passed in 28.31 seconds. x64 aggregate passed
in 57.29 seconds. All other registered tests passed in their initial runs.
This does not establish the scheduling root cause; existing timing debt remains.

P44 actual process inventory confirms all task workers exited. Exact scratch
cleanup was previewed/applied; Clean-Build -Check passes. Reusable caches,
user configuration, media and snapshots remain intact.

## P45 file 297 sevid014.c audit and restoration

Initial review: R0328 restores 111 native source/plane address expressions;
R0329 retains actual GDP field translation. R0330 retains native capacities
and zeroing in eleven outer WORD move entries: S2534/S2536/S2539/S2541/
S2544/S2546/S2549/S2551/S2554/S2556/S2559. Their LocalIUH[1] receives IU32
and is read as native IUH in the RAM-source branch. R0331 restores malloc
for the other 21 allocations. Original algorithms and callback order remain.

Counter-review: reversing all enumerated ABI forms reproduces the pinned XP
full token stream. The selected C-VID target compiles this file and evidfunc
registers its backward WORD moves. The [678-range map](m9-t100-sevid014-points.tsv)
and [32-allocation inventory](m9-t100-sevid014-allocations.tsv) account for
initial differences. Qualified remaining ABI differences still await owner
disposition. Exactly 132 lines change (+132/-132, net zero): 111 address
expressions and 21 allocation calls. Test sources are unchanged.

The same full-file SequenceMatcher metric gives XP diff +897/-896, 678 ranges
before, +794/-793, 591 ranges after: 206 fewer diff lines and 87 fewer ranges.
No new mirror repair, helper or second path is introduced.

Unchanged, minimal and poisoned-malloc actual bodies pass x86/x64. Each runs
1248 captures across eleven real WORD copy helpers (six four-plane, five
one-plane), RAM/video sources, three masks, zero/one/two/seven counts, address
parity and four one-plane shifts. Eight zero-mask RAM cases independently
assert latch output and untouched full-buffer bytes. Other captures prove
preservation, not correctness of every original traversal or video-latch rule.
Existing CHAIN2 checks pass 96/96. No complete outer-move matrix or new guest
reproduction is claimed. No test-only production API is introduced.

All successful captures match byte-for-byte: 5111808 bytes, SHA-256
20bd537e29f50e6a5d177c84ad58c48f3266e7c5d9a5f842f88ab93971dbfff7.
Original four-byte allocation capacities pass x86 but corrupt the x64 heap
(0xc0000374). Poisoning only the 21 restored malloc blocks with 0xa5 passes;
eleven mixed-width calloc blocks remain. No arbitrary high-address input
contract is inferred from direct helper tests.

Pinned XP normalized SHA-256:
ae9fb6f14e10f79e859ab8777348f9cfa87451c87047e9f2e26ee0bc7ee896b1.
Initial local normalized SHA-256:
fe2a542ad98b757c99f6bb413c60a38046b852305a8e89fba91ac65ef2c0ed8e.
Final normalized SHA-256: 44008656d3d7efb4e4b48872bdc900c8df463b67a5318134bd3c4d7768be70c8.
Installed source equals the tested candidate bytes and retains CRLF.
Full dual cached build/publication/regression qualification follows.

### P45 final qualification and actual-change review

Both cached builds/publications succeed. Each architecture passes 188/188
background tests plus 8/8 native audio/desktop/package tests after the isolated
retry recorded below: 392/392 qualified with unchanged budgets. Initial
aggregate timeout evidence is retained, not called a first-run pass.
Both builds emit the original
sevid014.c:1065 unreachable-switch warning; unchanged actual-body control
compiles emit the same warning on both architectures. No new warning appears.
Actual review verifies exactly the 111 address and 21 allocation replacements,
no test/public-component changes, final-source identity, CRLF and local-only
manifest hash changes. Mirror/documentation gates and command-local whitespace
checks pass. Captures prove selected helper preservation, not all original
WORD traversal semantics or interactive guest coverage. Qualified ABI residuals
remain pending owner decisions.

Aggregate verifier-negative timeout architectures: x64.
Only the affected test was retried alone with its unchanged 60-second budget.
Existing timing debt remains; retry success does not establish its root cause.

Published softpc32.exe: Nov 13 2026 08:20:54;
SHA-256 9f36852f3f776a5077e4e72c1e3d9017ee6a073c1f0e3777435797dcd2e0997e.
Published softpc64.exe: Nov 13 2026 08:24:35;
SHA-256 3719736797fc1ad26159547c27c0997f45c4a60ee0cfadd042eed085c5255b77.

Sequential audited scope is 297/472; next is file 298 base/cvidc/sevid015.c.
S1 stays active; no routine approval is awaited for ordered work.

P45 timing evidence: x64 aggregate verifier-negative timed out at 60.36 seconds;
its isolated unchanged-budget retry passed in 26.29 seconds. All other tests
passed on the first aggregate run. No timing-cause or interactive guest claim
is inferred from the retry.

P45 actual process inventory confirms all task workers exited. Exact disposable
scratch was previewed and removed; Clean-Build -Check passes. Reusable caches,
user configuration, media and snapshots remain intact.

## P46 file 298 sevid015.c audit and restoration

Initial review: R0332 restores 82 native source/plane/table address expressions;
R0333 retains original GDP offset to native-field translation. R0334 retains
native allocation sizes and zeroing in eight outer BYTE/WORD moves:
S2576/S2579/S2581/S2584/S2586/S2589/S2591/S2594. Their LocalIUH[1] is
written through IU32 and read through IUH in the RAM-source arm. R0335 restores
malloc in the other 24 allocations. This changes 106 lines (+106/-106, net zero).
No production helper, alternative route, shared component or test source changes.

Counter-review: reversing the enumerated ABI forms reproduces the fixed XP
full token stream, not just selected hunks. CMake selects sevid015.c through its
C-VID source set; evidfunc registers the BYTE/WORD/DWORD moves. Source-derived
buffer addresses originate in native source, plane and expansion-table pointers;
GDP operands retain their separate field mapping. Original copy loops, callback
order and continuation behavior are not rewritten. The
[547-range map](m9-t100-sevid015-points.tsv) and
[32-allocation inventory](m9-t100-sevid015-allocations.tsv) account for all
initial differences. Existing ABI residuals remain pending owner decisions.

XP diff initially +715/-714 (1429 lines), 547 ranges; candidate +639/-638
(1277 lines), 519 ranges: 152 fewer diff lines and 28 fewer ranges. Counts use
full-file SequenceMatcher with autojunk disabled, independently of source LOC.

Actual-body unchanged/minimal/poison controls pass on both architectures.
Each captures 960 cases across eight BYTE/WORD copy helpers, RAM/video,
three masks, counts 0/1/2/7, parity and four single-plane shifts. 240 zero-count
cases independently assert that the full destination buffer remains untouched;
nonzero cases establish preservation, not an exhaustive hardware oracle.
Existing CHAIN2 controls pass 96/96. No complete outer-move matrix or new
interactive guest reproduction is claimed. Poison fills the 24 restored malloc
sites with 0xa5, retaining the eight mixed-width calloc sites.

All successful capture files contain 3932160 bytes with SHA-256
ef1cadca78517b78c17595a47f926814d76ca37cc0c2450b8ffb5fb803b31ff2.
Original four-byte allocation sizes pass x86 but fail x64; the exact process
result is recorded with final qualification below. Allocation safety does not
authorize altering any original device algorithm. Source capacities remain
native-width, and the installed source is byte-identical to the tested candidate.

Pinned XP normalized SHA-256:
761881d731c670ab688a5e3391f3162b78fd7518634a59595cce70169a0960d8.
Initial local normalized SHA-256:
9b7999beb1ae848be29f24c3a9e0c9c0a8813bb72a7ae9f7972a33a876a54081.
Final normalized SHA-256: 297355ae73793093a24a5a2e4e10a088e08835d40f46d362c1279f1ef19393ca.
CRLF and original line count remain. Full dual cached qualification follows.

### P46 final qualification and actual-change review

Both cached x86/x64 builds and EXE publications succeed. Both retain the
sevid015.c:969 unreachable-switch warning also emitted by unchanged control
compiles; there is no new warning.
Each architecture completes all 188 background and eight native audio/desktop/
package tests. 392/392 pass on the first full run.
Actual review confirms only the 82 address and 24 allocation replacements,
CRLF, tested-source identity, local-only manifest hash change and no new
production/test interface. Original-capacity x64 failure is 0xc0000374 (heap
corruption). Mirror, documentation and whitespace gates pass. Focused captures
do not claim complete guest or outer-move coverage; existing residuals still
need owner disposition.

x86: 186/188 Test #172: emulator.verifier-negative ................................   Passed   44.78 sec
x64: 186/188 Test #172: emulator.verifier-negative ................................   Passed   38.78 sec

Published softpc32.exe: Nov 13 2026 08:35:35;
SHA-256 72bc0d5225675cbbf779f470efaaef797d2514d3c23e62086bb09e2f81f4b104.
Published softpc64.exe: Nov 13 2026 08:39:14;
SHA-256 26c54375a3a7094352d2dc3537641801f72e326690ddab77eccf1f69db835a88.

Sequential audited scope is 298/472; next file 299 base/cvidc/sevid016.c.
S1 remains active; no routine approval is awaited.

P46 actual process inventory confirms all task workers exited. Exact disposable
scratch was previewed and removed; Clean-Build -Check passes. Reusable caches,
user configuration, guest media and snapshots remain intact.

## P47 file 299 sevid016.c audit and restoration

Initial review: R0336 restores 69 native buffer/table address expressions;
R0337 retains GDP field mapping. R0338 retains native capacities and nine
mixed-width calloc sites: eight mode-3 outer moves use LocalIUH[1] as IU32/IUH,
and S2629 uses LocalIUH[8] as an IU32 counter and native pointer offset.
R0339 restores the other 23 malloc sites. R0343 restores four original private
enum entries and their four static wrapper/pointer declarations (24 lines).
Whole-repository symbol search finds no consumers of these L25_18/19/20/21
entries outside their own former definitions. Their removal is not necessary
to retain selected marker dispatch; restoring the dormant private scaffolding
passes both actual-body VGA controls and does not create a public entry path.

Counter-review finds substantive differences beyond the ABI patterns:
R0340 retains selected byte/string dirty dispatch; R0341 retains two CHAIN2
address A0-gap shift corrections; R0342 retains two local continuation gotos.
Each was independently restored in an isolated actual-body control, preserving
the other current patches and native GDP bindings. These are qualified existing
residuals, not new patches or automatically approved retention.

The [534-range map](m9-t100-sevid016-points.tsv) and
[32-allocation inventory](m9-t100-sevid016-allocations.tsv) reconcile initial
differences. Production +116/-92 (net +24) restores original scaffolding rather
than adding logic. XP diff falls +656/-717 (1373 lines), 534 ranges, to
+593/-630 (1223 lines), 471 ranges: 150 fewer diff lines and 63 fewer ranges.
No public component, test source or build interface changes.

Actual-body controls link the existing complete VGA regression against each
isolated sevid016 object. Unchanged, minimal, scaffold-restored and poisoned
malloc controls pass on x86/x64. Original four-byte capacities pass x86 but
fail x64 with 0xc0000374. Poison touches only the 23 restored allocations;
nine mixed-width calloc sites remain. This does not exhaust every generated
helper or arbitrary input combination.

Coherent original dirty restoration fails the selected EGA dirty-low assertion
on both architectures (vga_frame_smoke.c:1561). An earlier dirty control omitted
native GDP translation and is discarded, not counted as evidence. The corrected
control restores both complete original blocks and generated entries with GDP
bindings maintained. Isolating only the string marker additionally fails the
zero-length dirty-low sentinel on both architectures. The current candidate
passes zero/one/two/seven-byte string checks. These are actual function-body
tests, not a new guest reproduction or a claim that CCPU uses every bulk entry.

Restoring only the A0-gap shifts fails existing mode-64 incremental/full-frame
assertions on both architectures. Restoring only the two nested continuation
calls causes 0xc0000005 on both. Neither failure justifies changing other
original algorithms. Existing source/caller evidence distinguishes scalar CCPU
routes from generated bulk routes and their direct regression coverage.

Pinned XP normalized SHA-256:
779010fc9ac72e8ffc0c760d434588526e6fb417b467917a43828d3870826794.
Initial local normalized SHA-256:
1cbd0cfaa1edf813d9be70d8774969384cc9f40fbb6bd95a3000c5ae6d74473c.
Final candidate normalized SHA-256: 744a3a3e227d2ff1648e50e0156e67f729ca49af206458b187dfe36921879a8f.
Installed source equals the tested scaffold candidate and retains CRLF.
Full dual cached qualification follows.

### P47 final qualification and actual-change review

Both cached x86/x64 builds and publications succeed. Each architecture completes
188 background plus eight native audio/desktop/package tests. 392/392 qualify after correcting and rerunning the source-boundary gate on both architectures; the initial gate failures remain recorded.
The original unreachable-switch warning remains (now line 1029 after original
scaffolding restoration); unchanged controls emit the same warning. No new
warning appears. Source identity, CRLF, actual change accounting, local-only
manifest hash, mirror/documentation gates and whitespace checks are verified.
No guest reproduction or exhaustive helper coverage is inferred from these
tests. Qualified existing ABI/device residuals remain pending owner decisions.

The initial full runs each passed 187/188 background and 8/8 native tests.
The sole failure was the root gate's blanket prohibition of retired L25 names,
which also rejected dormant private declarations. Its two-line adjustment
rejects actual case/goto/label control flow while allowing the original private
scaffolding; the existing inline-CGA and 28 selected-marker checks remain.
Executing the actual check fragment accepts the declaration fixture and rejects
case, goto and label negative controls. Both complete source-boundary tests
pass after correction. No product source changed after its full build/test run;
only this affected gate needed rerunning. Root validation script +2/-2 is
reported separately from mirror production +116/-92; test sources are unchanged.

x86: 186/188 Test #172: emulator.verifier-negative ................................   Passed   49.62 sec
x64: 186/188 Test #172: emulator.verifier-negative ................................   Passed   43.15 sec

Published softpc32.exe: Nov 13 2026 08:57:15;
SHA-256 5c1fb1f40677d5cb42f5d7261539a837d46d47f2091db4452bc456c44f102fcd.
Published softpc64.exe: Nov 13 2026 09:00:59;
SHA-256 e4f9872e6be4dcc8823cd11e10cf57cda389d75499279f1034403a968764ac16.

Sequential audited scope is 299/472; next file 300 base/cvidc/sevid017.c.
S1 remains active; no routine approval is awaited.

P47 process inventory confirms all owned workers exited. Exact disposable
scratch was previewed and removed; Clean-Build -Check passes. Reusable caches,
user configuration, guest media and snapshots remain intact.

## P48 file 300 sevid017.c audit and restoration

File 300 is a selected C-VID production translation unit. evidfunc registers
its CHAIN2 word/mode-1/mode-2 entries; existing CCPU scalar stores and the
generated compound-entry tests are distinguished, not assumed equivalent
guest consumers. This file is not eligible for retirement.

The [367-range map](m9-t100-sevid017-points.tsv) reconciles the initial XP
differences; the [32-allocation inventory](m9-t100-sevid017-allocations.tsv)
records native slots and mixed-width uses. No excluded body is counted.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0344 | GDP accesses use mapped native fields instead of original offsets. | gdp_state.c maps original field offsets and callback/pointer widths to native VGA storage; coherent semantic controls preserve these bindings. Necessary ABI residual, pending owner choice. |
| R0345 | Ten address wrappers surround native buffer/table addresses. | Source traces them to RAM input locals, the existing video buffer or expansion table, not GDP offsets below 4096. Restore exact original dereferences; positive and poisoned-allocation controls pass both architectures. |
| R0346 | All 32 local allocations were zeroed and native-sized. | Restore 29 malloc calls; retain native capacity. S2635/S2647/S2653 slot 8 is first assigned through IU32 then read through IUH as a buffer offset, so three calloc sites remain. Original capacities corrupt the x64 heap; x86 passes. |
| R0347 | Five original inline CGA dirty blocks were replaced by the existing selected byte/string markers. | Restore all original blocks coherently with GDP mapping and private declarations: both architectures fail getVideodirty_low()==offset>>2. Existing selected-marker repair remains a qualified decision. |
| R0348 | Four mode-1/2 address shifts retain the plane A0 gap. | Existing VGA regression alone passes old shifts, so it is insufficient evidence. An independent physical-address oracle exercises both modes and eight addresses each; candidate passes 16 cases per architecture, original shifts write address 0x100 at physical 0x200 instead of 0x400 and fail. Retain qualified repair. |
| R0349 | Seven continuations use local goto instead of recursively invoking crules. | Exact original call restoration causes 0xc0000005 on both architectures; local activation storage must survive nested compound operations. Retain qualified repair, without claiming each continuation independently triggered. |
| R0350 | Ten private L25 enum/function/pointer groups were removed together with old dirty control flow. | Restore 60 original declaration lines; no active old case/goto/label is restored. Private dispatch identities have no external/persistent consumer; candidate passes both architectures. |

Production +99/-39 (net +60), tests unchanged. This is original-source
restoration, not new functionality. XP diff falls +479/-652 (1131 lines),
367 ranges, to +471/-584 (1055 lines), 353 ranges: 76 fewer diff lines and
14 fewer ranges. Existing original device defects remain pending diff
decisions; no new mirror repair or TODO is introduced.

Actual-body controls compile the selected translation unit with production
flags and link it explicitly into the existing VGA regression. Unchanged,
address/allocation candidate, poisoned candidate, and private-scaffold candidate
pass on x86/x64. Original four-byte local capacities pass x86 and fail x64
with 0xc0000374. Coherent original dirty logic fails the existing EGA dirty-low
assertion; original continuation calls fail with 0xc0000005 on both widths.

The added task-local physical oracle uses the actual CHAIN2 mode-1/2 writers,
known latch/mask state and independent interleaved-plane offsets, checking the
whole plane allocation for unintended writes. It restores modified controller
state afterward. Both current candidate runs pass all 16 cases. Old address
shifts fail at mode 1/address 0x100 on both architectures. This distinguishes
physical correctness from the existing compound-entry replay/decomposition
checks, which did not reject the original shifts. No new guest reproduction,
exhaustive arbitrary-mode proof or full product qualification is inferred.

Pinned XP normalized SHA-256:
c03486571fa8f0954a4a63a22563faebc5732ce5f6bd9133ef6829916ded5a58.
Initial local normalized SHA-256:
117de357265e6f407b5a69169a157bfaa1161bb4f645d286f5c34d95437ca170.
Installed candidate normalized SHA-256:
b6bf4d4f36c5b8131bff1228b3e836e77fae36888e0ebf2667d7ddcaca2f66e4.
Production bytes equal the tested scaffold candidate and retain CRLF. Full
dual cached build/publication and regression are running; file 300 is not yet
counted as delivered. No public component, user configuration, media or
snapshot changes are part of this batch.

### P48 final qualification and actual-change review

Both cached builds and dual EXE publications succeed. Every architecture
completes 188 background and eight native audio/desktop/package tests.
392/392 qualify after the unchanged-budget targeted timeout retries recorded
below; this is not a first-run all-green result. The first isolated x86 verifier attempt ended inconclusively with process
status 0x40010004 after 26.78 seconds and no CTest completion; its terminated
process was verified absent before the recorded second isolated run.
Full regression does not
erase the separately demonstrated original-code negative controls.

Actual diff review verifies exactly ten original native-address expressions,
29 allocation-kind restorations with native capacities intact, and 60 exact
original private declaration lines. Tested-source identity, mirror CRLF,
local-only manifest hash, mirror/documentation gates and whitespace checks pass.
The original unreachable-switch warning persists (original control line 741,
final line 801 after the 60 original lines); no new warning is introduced.
There is no production/test API or public-component change.

- x86 build: exit 0, 16.17 seconds.
- x86 background: exit 8, 227.16 seconds.
- x86 native: exit 0, 30.05 seconds.
- x64 build: exit 0, 21.15 seconds.
- x64 background: exit 8, 231.16 seconds.
- x64 native: exit 0, 30.06 seconds.
- x86 emulator.verifier-negative isolated retry: exit 0, 22.67 seconds; existing test budget unchanged.
- x64 lib.types-layout-selftest isolated retry: exit 0, 58.65 seconds; existing test budget unchanged.
- x64 emulator.verifier-negative isolated retry: exit 0, 19.2 seconds; existing test budget unchanged.

Aggregate verifier records:

x86: 186/188 Test #172: emulator.verifier-negative ................................***Timeout  60.09 sec
x86: 187/188 Test #149: lib.types-layout-selftest .................................   Passed  174.99 sec
x64: 186/188 Test #172: emulator.verifier-negative ................................***Timeout  60.16 sec
x64: 187/188 Test #149: lib.types-layout-selftest .................................***Timeout 180.06 sec

Published softpc32.exe: Nov 13 2026 09:17:36;
SHA-256 329df4bd8e37ebe65baf6a5f2a4e9a928ff6b5e1fd9e5ac911bf53e41ac76818.

Published softpc64.exe: Nov 13 2026 09:22:09;
SHA-256 06d1f4838eba13c94585ae06e171d4f09685487f6dc7cdad0689802eb212ff8b.

Sequential audited scope is 300/472; next file 301 base/cvidc/sevid018.c.
Qualified residuals await owner decisions; S1 remains active.

P48 process inventory confirms all owned workers exited. Exact disposable
scratch was previewed and removed; Clean-Build -Check passes. Reusable caches,
user configuration, media and snapshots remain intact.

## P49 file 301 sevid018.c audit and restoration

The selected production C-VID unit supplies CHAIN2 mode-2/mode-3 generated
entries registered by evidfunc.c. CCPU scalar paths and direct compound-rule
regression consumers are distinguished; no new guest reachability is claimed.
The [384-range map](m9-t100-sevid018-points.tsv) reconciles all initial
differences, with no excluded-body bookkeeping. The
[allocation inventory](m9-t100-sevid018-allocations.tsv) covers all 32 sites.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0351 | GDP offsets differ from native field/pointer layout. | Existing gdp_state field mapping supplies the supported native ABI; semantic controls retain this binding. Qualified residual, pending owner decision. |
| R0352 | Ten wrappers translate already-native RAM/video-buffer addresses. | Trace confirms native input local 6 and GDP video-buffer field 1292, not small GDP offsets. Restore exact original dereferences; current/candidate/poisoned controls pass both widths. |
| R0353 | All 32 allocations were zeroed and widened. | Restore 28 malloc calls; preserve native slot capacity. S2665/S2671/S2683/S2689 slot 8 is initialized through IU32 and subsequently read through IUH as a buffer offset, requiring zero upper storage. Original capacity restoration corrupts x64 heap; x86 passes. |
| R0354 | Five inline CGA dirty blocks use the selected marker instead. | Coherent original blocks plus private scaffolding and GDP translation fail the EGA dirty-low assertion on both architectures. Qualified residual; no new marker path. |
| R0355 | Two mode-3 address shifts preserve the A0 gap. | Independent physical oracle checks eight addresses and the entire plane buffer. Candidate passes; original shifts put address 0x100 at physical 0x200 instead of 0x400. Qualified residual on both widths. |
| R0356 | Seven local continuations replace nested crules calls. | Exact original call restoration causes 0xc0000005 on both widths. Source shows local activation ownership; grouped dynamic proof does not claim every continuation independently triggered. |
| R0357 | Ten private L25 declaration groups were removed with dirty bodies. | Restore 60 exact original enum/function/pointer lines. No old case/goto/label is restored; no external or persisted dispatch identity exists. Actual candidate passes both widths. |

Production +98/-38 (net +60); test sources unchanged. XP-relative diff falls
+503/-670 (1173 lines), 384 ranges, to +495/-602 (1097 lines), 368 ranges:
76 fewer diff lines and 16 fewer ranges. No new device repair, API, secondary
path or public-component change is introduced.

Actual-body controls use production compilation flags and the existing full
VGA regression linked with an explicit replacement sevid018 object. Current,
candidate and candidate malloc filled with 0xa5 pass on both architectures.
Original four-byte capacities pass x86 and fail x64 with 0xc0000374. Original
dirty logic fails vga_frame_smoke.c:1561 getVideodirty_low()==offset>>2;
original continuation calls fail with 0xc0000005 on both architectures.

The task-local mode-3 physical oracle sets/restores existing latch/plane-enable
state, calls actual CHAIN2 byte writers with zero input, and checks all plane
bytes against independent interleaved offsets. Candidate passes eight addresses
per width; original shifts fail at address 0x100. This is not a guest
reproduction or exhaustive arbitrary-state qualification. Original defects
remain pending diff decisions, not new TODOs or unauthorized fixes.

xp normalized SHA-256: 870093e49a0bf12cc26b0b21e8bd0d13cd2d084d0685b3968950351592b37f0a.
before normalized SHA-256: 74bfb545e3b09f4dedcf63181fdb808be229082effc8fffab2b4a768ed5c936e.
candidate normalized SHA-256: 7f11e41102c8e83fe3b29eb96bb678d736781d2986c4b2d5d46c79ac7a913c73.

Installed bytes equal the tested candidate and retain CRLF. Full cached dual
qualification is pending; file 301 is not yet counted as delivered.

### P49 final qualification and actual-change review

Both cached x86/x64 builds and dual EXE publications succeed. Each architecture
completes 188 background and eight native audio/desktop/package tests.
392/392 pass on the first full run.
Actual diff review confirms exactly ten restored native-address expressions,
28 allocation-kind restorations with native capacities intact, and 60 exact
original private declaration lines. Final source equals the tested candidate;
mirror CRLF and local-only manifest hash are verified. The original
unreachable-switch warning remains (control line 801, final line 861); no new
warning appears. Mirror/documentation gates and whitespace checks pass.
No public component, test source, media, INI or snapshot is changed. Existing
evidence-qualified ABI/device residuals remain pending owner decisions.

- x86 build: exit 0, 10.08 seconds.
- x86 background: exit 0, 136.12 seconds.
- x86 native: exit 0, 25.03 seconds.
- x64 build: exit 0, 13.09 seconds.
- x64 background: exit 0, 153.14 seconds.
- x64 native: exit 0, 25.03 seconds.

Aggregate fixture records:

x86: 186/188 Test #172: emulator.verifier-negative ................................   Passed   24.60 sec
x86: 187/188 Test #149: lib.types-layout-selftest .................................   Passed   85.42 sec
x64: 187/188 Test #172: emulator.verifier-negative ................................   Passed   22.37 sec
x64: 188/188 Test #149: lib.types-layout-selftest .................................   Passed   65.43 sec

Published softpc32.exe: Nov 13 2026 09:39:52;
SHA-256 9f7c2189b740c1875153fe9f17457fcede7281fd8c1865222cd77594da7962f9.

Published softpc64.exe: Nov 13 2026 09:42:43;
SHA-256 0a605a7f965df1bd817c19293bf31592f03fb5aa9261051a5af449873b1c96df.

Sequential audited scope is 301/472; next file 302 base/cvidc/sevid019.c.
S1 remains active; no routine approval is awaited.

P49 process inventory confirms all owned workers exited. Exact disposable
scratch was previewed and removed; Clean-Build -Check passes. Reusable caches,
user configuration, guest media and snapshots remain intact.

## File 302 sevid019.c investigation in progress

No production change or P50 delivery is claimed yet. The scratch candidate
restores 40 native-address expressions, 20 malloc calls, 168 original private
declaration lines and the shift literal at former DIV-123. That shift's
immediately preceding assignment fixes its count to zero; the widened literal
has no demonstrated necessity there. Exact original whitespace reconciliation
and remaining point review are still required. Initial XP difference is
+726/-1120, 486 ranges; this intermediate candidate is +688/-917, 432 ranges.
These are candidate metrics, not delivered reductions.

Actual production-flag translation-unit replacements linked into the existing
VGA fixture pass on x86/x64 for unchanged code, candidate and candidate malloc
storage poisoned with 0xa5. All include 96/96 independent Copy DWORD fill cases.
Original four-byte allocation capacity passes x86 but faults with 0xc0000005
on x64. Restoring the 28 layout constant lines fails actual VGA readback at
vga_frame_smoke.c:1516 on both architectures. Removing the WORD empty-fill
guard faults on both; removing the DWORD empty guard or odd-tail repair fails
the independent fill oracle on both. No guest-triggered reproduction is claimed.

Restoring all 16 continuation calls passes the pixel assertions on both
architectures, so those assertions alone do not justify retention. A separate
actual-body allocation/free counter supplies the missing ownership evidence:
both architectures allocate 672566 blocks in this unit. Candidate frees all
672566; restored continuations free only 672462, leaving 104 allocations.
The original continuation enters a fresh crules activation with CopyLocalIUH
zero; its cleanup loses the enclosing same-file caller's saved local pointer.
This is an observed resource leak, not a claimed crash in this file. Counters
are task-only instrumentation; no production test API is introduced.

All three probe runners completed. Remaining work includes selected-marker,
DWORD rotation and mixed-width local proof reconciliation, exact original
restoration review, point registration and the required final-source dual
build/publication/full regression. Sequential delivered coverage remains
301/472. Task-owned scratch remains solely for this active investigation.

### File 302 final-source review and qualification pending

The exact candidate is now applied: production +228/-63 (net +165), test
sources unchanged. XP difference changes +726/-1120 (1846 lines, 486 ranges)
to +687/-916 (1603 lines, 432 ranges): 243 fewer diff lines, 54 fewer ranges.
The added source is original private scaffolding, not additional runtime logic.
The point map covers all 486 initial ranges; the allocation inventory names
all 32 local allocations, with 20 malloc restorations and 12 zeroing residuals.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0358 | GDP slot access differs from the original byte-offset carrier. | Native field offsets and pointer-sized fields differ on x64; keep existing mapped GDP access pending owner decision. Direct RAM addresses are reviewed separately. |
| R0359 | Forty native-address wrappers treat already resolved pointers as possible GDP offsets. | The addresses derive from plane/native RAM buffers and the original native pointer carriers; restore exact original dereferences. Final-source and poisoned-malloc controls pass both architectures. |
| R0360 | All local arrays use native-sized calloc. | Twenty arrays have no partial-store/full-native-load overlap and recover malloc with native capacity. Twelve have narrow stores followed by native loads (Copy value/address slots or backward source-offset slot 8); retain initialization pending decision. Native capacity is required: original four-byte allocation faults on x64. Poisoning all formerly zeroed arrays also faults on x64; it does not individually prove every retained array. |
| R0361 | Fourteen original inline CGA dirty blocks were replaced by selected markers. | Coherent restoration includes original L25 labels and intervening address computation, retaining native GDP mapping. Both architectures fail the actual EGA dirty-low assertion at vga_frame_smoke.c:1561. Retain selected byte/word/string marker wiring, including backward span conversion, pending decision. |
| R0362 | Twenty-eight address/stride constants preserve odd/even plane gaps. | Exact original constant restoration fails actual VGA readback at vga_frame_smoke.c:1516 on both architectures; preserve layout fixes pending decision. |
| R0363 | Sixteen continuation calls became same-activation gotos. | Pixel tests alone pass with original calls. Actual-body counters independently show 104 leaked allocations on each architecture with original calls versus zero with the candidate. Retain ownership fixes pending decision, without claiming this file crashes. |
| R0364 | Copy WORD/DWORD fills have empty-count protection, full-DWORD rotation and a byte-sized odd tail. | Separate original restorations of each category fail: WORD empty access faults; DWORD empty, rotation and tail fail the 96-case independent plane oracle. Both widths reproduce. Preserve existing fixes pending decision; no new bulk caller is enabled. |
| R0365 | Original private L25 enum/function/pointer declarations were omitted. | Restore 168 exact original lines. No active case/goto/label or old CGA accounting is reintroduced by these declarations. Both widths compile and run. |
| R0366 | DIV-123 widens literal 1 at a shift-count-zero test. | The preceding assignment fixes the count to zero. Restore the complete exact original line and remove the three-line explanation. Both final-source controls pass; no new behavior or mirror point is added. |

These generated entry points remain in the existing C-VID controller tables;
no duplicate Compat implementation or removable compilation unit was found.
Native RAM and controller-rule tests exercise registered batch entries, but
this does not establish ordinary guest use of every bulk entry. No guest,
media, configuration, snapshot or sibling repository changed.

Single-person executor review verifies every source delta is an exact original
private block, direct address, allocator spelling with unchanged native capacity,
or the exact shift line. Counter-review checks that original private declarations
do not reactivate removed CGA marker branches. Native-width GDP/local ownership,
selected-marker and plane-layout differences remain explicit; passing modified
tests is not their only justification. Mixed-array evidence is bounded as stated.

Final LF-normalized SHA-256:
9ea9816db5c005e129a6f82e684e1dd62b3318a9336e88583c4869f6e57a77d5.
CRLF source equals the final tested candidate. Mirror manifest verification
passes. Full cached dual build/publication and registered regression are now
running; file 302 is not counted delivered until they and final review finish.

### File 302 counter-review refinement: restore five direct continuations

Further same-file call analysis splits R0363 rather than treating all sixteen
sites as one necessary repair. S2697, S2699, S2700, S2702 and S2703 have no
same-file nested callers. Their L21_137/153/155/161/163 continuations can use
the exact original tail-return call without losing an enclosing local array.
The other eleven sites occur in the same-file move-wrapper chains and retain
their ownership fix pending decision. This supersedes the all-sixteen
retention suggestion above; a group-level leak test alone was insufficient.

Restoring only those five calls passes the actual VGA fixture on both widths.
Independent allocation/free instrumentation records 672566/672566, zero
residual blocks, on x86 and x64, with 96/96 Copy DWORD fill cases. Restoring
all sixteen still loses 104 allocations as recorded above. Each newly restored
two-line block matches pinned XP exactly. No production instrumentation remains.

Final production change is now +238/-68 (net +170). XP difference is
+682/-906 (1588 lines), 428 ranges: **258 fewer diff lines, 58 fewer ranges**
than the file's initial state. Final LF-normalized SHA-256:
5c7a9b564215ecc9734da69ada668e2146c92e1326ed8dcec1a996dfc5049d18.
The prior candidate completed 392/392 with no retries (x86 build/background/
native 10.09/140.11/26.04 seconds; x64 11.09/145.10/25.03). That suite is
not reused as proof of the refined final source. After all prior workers
exited, the five original calls were applied and a fresh complete cached
dual build/publication/regression was started. Delivery remains pending.

### P50 final qualification and actual-change review

Both final-source cached builds and fresh publications succeed. Each
architecture initially passes 187/188 background and 8/8 native tests. The
sole failure is the old source gate requiring all sixteen continuation gotos.
After the full run ends, its list is narrowed to the eleven entries with
same-file nested callers; both exact gate reruns pass. Qualification is
**392/392**, not a first-full-run pass. No budget changes. The original
switch-unreachable warning remains the sole build warning, shifted from
line 981 to 1149 by original declarations; no new warning appears.

- x86 build: 10.08 seconds, exit 0.
- x86 background: 132.09 seconds, exit 8.
- x86 native: 26.03 seconds, exit 0.
- x64 build: 11.1 seconds, exit 0.
- x64 background: 142.1 seconds, exit 8.
- x64 native: 25.03 seconds, exit 0.
- x86 corrected source gate: 5.98 seconds, exit 0.
- x64 corrected source gate: 6.85 seconds, exit 0.

Counted tracked production change is sevid019.c +238/-68; test C/H sources
are unchanged. The coupled root gate changes +3/-2: one explanatory comment
and removal of five proven direct-only entries from its continuation list.
Other files' continuation requirements and the 28-marker gate are unchanged.

Final-source identity, CRLF, actual-change review and mirror/documentation
gates pass. This completes file 302 delivery qualification, not the whole S1
audit or the owner decisions. No new interactive DOS/Win3.1/Win95 validation
is claimed beyond the registered integration suite and actual-body controls.

Published softpc32.exe: embedded dates Nov 13 2026; times 10:06:59;
SHA-256 5110100ac34582fa2b06786c7fe351331fffda2bbd4ac99f85e74cd70832321d.

Published softpc64.exe: embedded dates Nov 13 2026; times 10:09:48;
SHA-256 25e6f6a924543d4a8690b16420c6ca2907472c0c24ee559259e8f70fbf252396.

Sequential audited scope is 302/472; next file 303 base/cvidc/sevid020.c.
S1 remains active; no routine approval is awaited.

P50 process inventory confirms all owned workers exited. Task scratch was
previewed and removed through Clean-Build; its closure residue check passes.
Reusable Ninja/ccache roots, user configuration, media and snapshots remain.

## P51 file 303 sevid020.c audit and restoration

Initial XP difference is +648/-700 (1348 lines), 503 ranges. The final
candidate is +637/-667 (1304 lines), 493 ranges: **44 fewer diff lines,
10 fewer ranges**. Production +53/-31 (net +22), test sources unchanged.
The root continuation gate drops the one restored direct-only call (+1/-1).
The range map accounts for all 503 initial ranges; the allocator inventory
records 32 arrays, 19 malloc restorations and 13 retained zeroing arrays.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0367 | GDP byte-offset accesses use native field mapping. | Keep existing ABI mapping pending decision: native offsets/widths differ on x64, and real generated callers consume those fields. Not equivalent to the already resolved pointers below. |
| R0368 | Nine native RAM/plane address accesses use GDP-address wrappers. | Follow plane-buffer and native source-pointer assignments; restore exact original dereferences. Candidate and poisoned-allocation actual-body controls pass both widths. |
| R0369 | All local arrays are native-sized and zero initialized. | Restore malloc for 19 arrays without partial-store/full-native-load overlap. Keep 13 arrays: backward ModeX slot 8 and twelve Chain4 writer slot-0 addresses are written narrow then read native for address calculation. Original four-byte capacities corrupt the heap on x64; no failure is claimed for x86 capacities. |
| R0370 | Two backward Move entries replace original CGA inline marking. | Coherent original restoration, with native GDP mapping preserved, makes dirty_high 0x100 instead of the actual EGA group range around 0x3e..0x41 at offset 0x104. Both widths fail the corrected actual-entry oracle. Retain selected marker/span conversion pending decision. |
| R0371 | Ten Copy backward address/stride constants retain plane gaps. | Exact original constant restoration leaves physical storage for guest address 0x101 untouched at start 0x104; both widths fail the independent constant-byte Copy oracle. Retain existing layout fixes pending decision. |
| R0372 | Six continuation calls were made same-activation gotos. | S2730 has no same-file nested caller: restore its exact L21_251 tail call. S2726 is called by S2728, and S2729 by S2730/S2731: retain the other five pending decision. Actual-body counters on both widths give candidate 290 allocations/290 frees; restoring all calls gives 290/254, 36 residual allocations despite passing pixels. |
| R0373 | Original private L25 declarations were omitted. | Restore 24 exact original enum/function/pointer lines without reintroducing any active CGA accounting or case/goto/label. |
| R0374 | DIV-123 widens a literal shifted by zero. | The immediately preceding assignment sets the count to zero; restore the full exact original test line and remove its three-line explanation. No concrete necessity remains. |

The real C-VID compile unit and controller tables remain the single path;
this file also contains original Chain4 writers/fills, not duplicate Compat
device logic. No file deletion is justified. Native source pointers remain
native; GDP adaptation is not restored indiscriminately. No guest, media,
configuration, snapshot or sibling repository changes.

Both widths pass unchanged/candidate actual VGA fixture and poisoned candidate
malloc controls, including 96/96 Copy DWORD fill assertions. A task-only
helper adds sixteen backward-entry cases: exact dirty/physical-address checks
for Copy, bounded EGA group marking for ModeX WORD, whose existing nested
writers also mark their individual writes. An initial helper incorrectly
required a simple contiguous WORD lower bound for ModeX; it failed unchanged
behavior and was withdrawn, not counted as a production defect. The corrected
candidate passes on both widths; original dirty and layout controls fail as
described separately. These probes do not establish every Chain4 mode or
interactive guest use of legacy bulk entries.

Actual-change review confirms all insertions are exact original declarations
or the direct continuation block. Other changes are direct native addresses,
malloc spelling with unchanged native capacity, and the original shift line.
Root gate adaptation removes only L21_251 from this file's required-goto list;
the other five and all unrelated files' checks remain. Final CRLF source equals
the tested candidate. LF-normalized SHA-256:
cb95f3bb73e2bc49547e29328ea7c6179165f250773975d1ed67534985ec6196.
Mirror verification passes. Full cached dual build/publication and regression
are running; delivered scope remains 302/472 until completion and review.

### P51 final qualification and actual-change review

Both cached builds/publications succeed without warnings. Each architecture
passes 188/188 background and 8/8 native tests: **392/392 first full run**,
without retries or budget changes. Final-source identity, CRLF, actual-change
review, mirror and documentation gates pass.

- x86 build: 9.09 seconds, exit 0.
- x86 background: 135.09 seconds, exit 0.
- x86 native: 25.03 seconds, exit 0.
- x64 build: 11.09 seconds, exit 0.
- x64 background: 137.08 seconds, exit 0.
- x64 native: 25.03 seconds, exit 0.

No new interactive guest validation or exhaustive Chain4 qualification is
claimed. The retained differences remain qualified owner decisions.

Published softpc32.exe: embedded dates Nov 13 2026; times 10:21:56;
SHA-256 1820f999c16e35ac50469d5e0f8015a474e1d901560847cfc42854bd91ad998e.

Published softpc64.exe: embedded dates Nov 13 2026; times 10:24:45;
SHA-256 f221049454e08003dec13ad7348f7e9481496d8a60439c6a8c677f6192567138.

Sequential audited scope is 303/472; next file 304 base/cvidc/sevid021.c.
S1 remains active; no routine approval is awaited.

P51 process inventory confirms no owned worker remains. Task scratch was
previewed and removed; Clean-Build closure residue verification passes.
Only reusable caches remain. User configuration, media and snapshots are intact.

## File 304: sevid021.c (P52)

Pinned XP comparison has 583 non-equal ranges, +741/-740 lines (1481 total).
The candidate restores 50 native-address dereferences and malloc in 18 of
32 local arrays. Production +68/-68, net zero; permanent test code unchanged.
XP difference becomes 533 ranges, +691/-690 lines (1381 total): reduction
of 50 ranges and 100 added/deleted lines, not merely a source line reduction.
The [point map](m9-t100-sevid021-points.tsv) covers all initial ranges;
the [allocation inventory](m9-t100-sevid021-allocations.tsv) names all 32 arrays.

| Point | Initial executor analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0375 | GDP accesses use an explicit field mapping rather than original byte offsets. | Retain pending decision. The actual GDP owner is an opaque slot aggregate, and native pointer fields differ in width; arithmetic into that aggregate cannot substitute for its field storage. Both immediate and register-selected offsets are audited separately from already resolved pointers. |
| R0376 | Fifty native byte accesses use the GDP address adapter. | Every operand comes from native local slots 6, 7, 9 or 10: RAM source or native read/write plane base plus offset. They are not small GDP offsets. Restore each complete original XP line; source comparison verifies exact equality. |
| R0377 | All 32 local arrays use native capacity and zero initialization. | Restore malloc in twelve ByteMove wrappers and six CopyByte4Plane helpers, whose partial stores are not later consumed as native-width values. Retain zero initialization in six CopyBytePlnByPln helpers (slot 7) and eight WordWrite entries (slot 0): an IU32 address store precedes an IUH address load. Poisoned removal of these initializations faults on x64; original four-byte capacities corrupt the x64 heap. Both controls pass on x86, which has no corresponding upper half. Native capacity and the 14 mixed-width initializations remain owner decisions. |

The compilation unit belongs to the real C-VID target. SetWritePointers in
ev_glue.c selects chain4_evid for CHAIN_4 with zero rotate, then selects a
mode/state entry. evidfunc.c registers this file's ByteMove and WordWrite
entries; the Copy helpers are called from the generated entries. There is
no duplicate Compat implementation to replace and no unused-file deletion.
No new guest use of every legacy bulk entry is claimed.

The unchanged, candidate and poisoned-malloc candidate bodies each pass the
existing VGA fixture on x86/x64. A task-local extension additionally invokes
160 Chain4 entry cases: all twelve ByteMove variants, eight WordWrite variants,
four address residues and two plane masks, with repeated full-output checks.
The first two variants have independent byte-position/mask expectations;
other variants establish restoration equivalence, not exhaustive hardware
correctness. All six runs give the same output hash 0306c195.

A second task-local extension invokes all twelve Copy helpers directly with
both RAM and video-source flags and four address residues. This exercises
the native source/read-plane branches without inventing a missing bulk-read
callback. Together with the preceding entries it yields 256 cases per run,
with full-output hash 83654095 for current/candidate/poisoned candidate on
both widths. The first two helper variants again have independent byte-position
expectations. This is actual generated-body evidence, not a new guest program
or proof that every entry has an active guest bulk caller.

Original-capacity control exits 0 on x86 and 0xc0000374 on x64. Removing the
remaining zero initialization with poisoned malloc exits 0 on x86 and
0xc0000005 on x64. These are actual-body controls, not predictions from
uninitialized storage. No new mirror patch follows from unrelated original
behavior; such discoveries remain pending diff decisions rather than TODO.

Counter-review verifies all 50 restored address lines exactly match XP,
the other 18 changes only restore malloc while preserving native capacity,
and no condition, continuation, calculation, callback or device path changes.
Final CRLF source equals the tested candidate. LF-normalized SHA-256:
a2f9ecb65cbdaa804bf53af922b9dbc788337053f247046114afbb3baf33fa77.
The original switch-unreachable warning remains at line 1113 in both current
and candidate builds on both widths; it is not a new warning.
Full qualification is in progress; delivered scope remains 303/472.

### P52 final qualification and actual-change review

Both cached builds/publications succeed. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first full run**, without retries
or budget changes. The sole reported compiler warning is the unchanged
original switch-unreachable statement at line 1113, present before and after
on both widths. No new warning is observed.

- x86 build: 9.08 seconds, exit 0.
- x86 background: 133.09 seconds, exit 0.
- x86 native: 25.03 seconds, exit 0.
- x64 build: 11.09 seconds, exit 0.
- x64 background: 135.11 seconds, exit 0.
- x64 native: 25.03 seconds, exit 0.

Published softpc32.exe: embedded dates Nov 13 2026; times 10:38:51;
SHA-256 5db7c3abf9b5a25d2e4f6206bf8681a5c7af4aa05cede24460c44d7bc504db98.

Published softpc64.exe: embedded dates Nov 13 2026; times 10:41:38;
SHA-256 a5da2e60dd59989046007197a0e10f67182ce6d080c5ac15aa44a537458b519f.

Final source identity, CRLF, point reconciliation and actual-change review
pass. No guest/configuration/media/snapshot changes and no new production
path. No new interactive guest acceptance is claimed. Sequential audited
scope is 304/472; next file 305 base/cvidc/sevid022.c. S1 remains active;
qualified residual decisions are not owner-approved retention or closure.

P52 process inventory confirms all owned workers have exited. Task scratch
was previewed and removed through Clean-Build; closure residue verification
passes. Only reusable caches remain. User configuration, media and snapshots
are unchanged.

## File 305: sevid022.c (P53)

Pinned XP comparison has 614 ranges, +774/-773 lines (1547 total). Restore
21 native address dereferences and malloc in 28 of 32 local arrays.
Production +49/-49, net zero; permanent test code unchanged. XP difference
becomes 593 ranges, +753/-752 lines (1505 total): 21 fewer ranges and 42
fewer added/deleted lines. Every initial non-equal range, including potential
delete-only ranges, is reconciled by the [point map](m9-t100-sevid022-points.tsv).
The [allocation inventory](m9-t100-sevid022-allocations.tsv) lists all 32 arrays.

| Point | Initial executor analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0378 | Original GDP byte-offset accesses use explicit native field mapping. | Retain pending decision. The current GDP owner is opaque slot storage, not the original contiguous 32-bit layout; real generated callers require native field widths. Distinguish field lookup from dereferencing an already resolved pointer. |
| R0379 | Twenty-one byte accesses use the GDP address adapter. | Restore exact XP lines. Registers load native local slots 6/7/9/10, sometimes minus one for the second byte. Source assignments originate in native RAM pointers or native plane bases plus offsets; none is a small GDP field offset. Actual current/candidate/poison output agrees on both widths. |
| R0380 | All 32 arrays use native capacities and calloc. | Restore malloc in 28 arrays with no partial-store/native-load overlap. Keep calloc in S2790-S2793 slot 0: IU32 guest address assignment is followed by IUH address arithmetic. Poisoning these four arrays faults on x64. A separate capacity control keeps zero initialization but uses original four-byte slots; x64 heap corruption proves the width requirement independently. Both controls pass x86. These four initializations and native capacity remain owner decisions. |

The file remains in the actual C-VID compile target and the chain4_evid
controller table. It supplies four WordWrite entries, twelve WordFill entries
and their four-plane helper, eight WordMove entries and seven CopyWord helpers.
CopyWordPlnByPln delegates to existing CopyByte helpers in the preceding unit;
CopyWord4Plane uses native pointer locals. There is no duplicate Compat device
implementation to replace, nor a basis to remove the file. No continuation,
device algorithm, branch, callback, guest, media or configuration is changed.

Task-local actual-body controls replace only this unit's object and extend
the existing VGA fixture at its initialized controller boundary. Ninety-six
cases cover four WordWrite variants, twelve WordFill variants and eight
WordMove variants, each at four address residues, with repeated complete-plane
output. Current, candidate and poisoned-malloc candidate pass on x86/x64;
all six runs give hash 528a38a5. The existing VGA assertions also pass.
These are restoration-equivalence/replay tests, not an independent oracle
for every hardware mode, video-source callback or guest bulk consumer.

Two separate negative controls use the same fixture: removal of the remaining
four initializations exits 0 on x86 and 0xc0000005 on x64; original four-byte
capacities with calloc zeroing preserved exit 0 on x86 and 0xc0000374 on x64.
No timeout/retry is counted as failure evidence and no new original defect
patch is introduced. Existing original-defect decisions remain outside TODO.

Actual-change review verifies exactly 49 line replacements. All 21 address
lines match pinned XP byte-for-byte apart from repository CRLF; the other
28 restore malloc without changing the needed native capacities. The final
source equals the tested candidate; normalized-LF SHA-256:
cf2f6ffe21bbc067f1bd08f0f92c15711f635a8cdb3dd02f0efbc281d7888b37.
Full cached dual-architecture qualification is running; delivered scope
remains 304/472 until it completes and is reviewed.

### P53 final qualification and actual-change review

Both cached builds/publications succeed. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first full run**, without retries
or changed budgets. No new warning: the original switch-unreachable statement
at line 801 appears in current and candidate on both widths.

- x86 build: 9.09 seconds, exit 0.
- x86 background: 132.08 seconds, exit 0.
- x86 native: 25.03 seconds, exit 0.
- x64 build: 12.09 seconds, exit 0.
- x64 background: 131.12 seconds, exit 0.
- x64 native: 25.03 seconds, exit 0.

Published softpc32.exe: embedded dates Nov 13 2026; times 10:51:22;
SHA-256 c58a156d88a0fa6f72ec22106c5b9de23081233867ca9b56dd207f46380ef9c2.

Published softpc64.exe: embedded dates Nov 13 2026; times 10:54:09;
SHA-256 160144d0f9ee54d54e5d1f630d6d4abddb00668de8267abebbbf3f1372d0f4fa.

Final source identity, CRLF and actual-change review pass. No permanent test
source or public component changes. No new interactive guest acceptance is
claimed. Sequential audited scope is 305/472; next file 306 base/cvidc/sevid023.c.
S1 remains active; qualified residuals still await owner decisions.

P53 process inventory confirms no owned worker remains. Task scratch was
previewed and removed via Clean-Build; closure residue verification passes.
Only reusable caches remain. User configuration, media and snapshots are intact.

## File 306: sevid023.c (P54)

Pinned XP comparison contains 393 ranges, +510/-509 lines (1019 total).
Restore 23 native address dereferences and all 32 malloc calls, retaining
native allocation capacity. Production +55/-55, net zero; no permanent test
code change. Candidate XP diff is 370 ranges, +487/-486 lines (973 total):
23 fewer ranges and 46 fewer added/deleted lines. All initial ranges are
reconciled in the [point map](m9-t100-sevid023-points.tsv); all 32 arrays are
listed in the [allocation inventory](m9-t100-sevid023-allocations.tsv).

| Point | Executor analysis | Independent actual-diff review and disposition |
| --- | --- | --- |
| R0381 | Native GDP field mapping and its declaration include. | Retain pending owner decision: the GDP owner uses native slot storage rather than original contiguous 32-bit offsets. Resolved address dereferences are separate from field lookups. |
| R0382 | Twenty-three resolved native addresses routed through GDP address mapping. | Restore exact XP lines. CopyWord helpers operate on native source/plane pointers already loaded through GDP or native local slots. Both widths preserve output under actual-body controls. |
| R0383 | Thirty-two zeroed native local arrays. | Restore malloc in every array: no partial-store/native-load intersection was found in these function bodies. Poisoned malloc controls pass both widths. Keep native capacity pending decision: restoring original four-byte capacities produces x64 heap corruption (0xc0000374), while x86 passes. |

The file is compiled in the C-VID target and selected through chain4_evid.
It contains CopyWord helpers, five WordMove variants, twelve DwordWrite
variants and eleven DwordFill variants. There is no duplicate Compat device
implementation or basis for whole-file retirement. Original DwordWrite uses
second WORD offset +2, unlike the previously recorded Unchained case; its
correct offset is unchanged. No algorithm, continuation or guest changes.

Task-local actual-body tests use the existing initialized VGA fixture and
replace only this unit's object. There are 112 cases per run: twelve DWORD
write variants, eleven DWORD fill variants and five RAM-source WORD move
variants, each at four address residues, with repeated full-plane comparison.
Current, candidate and poisoned-malloc candidate pass on both widths; all
six hashes equal ec525591. Existing VGA assertions also pass. An independent
four-byte write oracle covers the first two write variants; other variants
provide restoration equivalence, not exhaustive hardware correctness.
Original-capacity controls pass x86 and fail x64 with heap corruption; no
observation timeout is treated as evidence.

The fill diagnostic demonstrates D003 above, not a new regression: four
requested DWORDs occupy only ten bytes because the high-WORD pass overwrites
the low-WORD pass. Pinned XP contains the same count-wide calls at +0/+2.
Full src search finds fill_dword_ev_glue only at its definition; the current
CCPU c_sas_fillsdw loop instead invokes c_sas_storedw and advances by four.
No real guest use of the defective bulk entry or guest symptom is claimed.
The discovery stays in pending diff decisions, not TODO, with no new patch.

Review confirms exactly 55 replacements: 23 address lines match pinned XP
exactly, and 32 calloc calls become malloc with native capacities unchanged.
Source equals the tested candidate, retains CRLF, normalized-LF SHA-256:
a0b4b10edf25a6a35915c9fe8ea4f47b49a490a01952b6c1e945e860cb39b3e9.
Full cached dual-architecture qualification is running; delivered scope stays
305/472 until qualification, review and delivery complete. S1 remains active.

### P54 final qualification and actual-change review

Both cached builds/publications succeed. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first full run**, no retries or
budget changes. No compiler warnings in this unit's current/candidate controls
or either full incremental build.

- x86-build: 9.09 seconds, exit 0.
- x86-background: 134.1 seconds, exit 0.
- x86-native: 25.03 seconds, exit 0.
- x64-build: 10.09 seconds, exit 0.
- x64-background: 131.1 seconds, exit 0.
- x64-native: 25.03 seconds, exit 0.

Published softpc32.exe: embedded dates Nov 13 2026; times 11:04:50;
SHA-256 91979831c2c853e3f99dd89b180151d02a59eb43c8b5fdc805acf1d4f4463202.

Published softpc64.exe: embedded dates Nov 13 2026; times 11:07:38;
SHA-256 fdd7efb990c9211f86f5d64afe5b9e880712eadf53fbaf0be4f37d6cbb695971.

Final source identity, CRLF, actual-diff review and gates pass. No permanent
test source or public component changes. No new interactive guest acceptance
is claimed. Sequential audited scope is 306/472; next file 307
base/cvidc/sevid024.c. S1 remains active; residual decisions are not closed.

P54 process inventory confirms no owned worker remains. Exact task scratch
was previewed and removed through Clean-Build; residue verification passes.
Reusable caches, user configuration, media and snapshots are unchanged.

## File 307: sevid024.c (P55)

Pinned XP comparison contains 400 contiguous ranges, +498/-497 lines (995
total). Restore ten native address dereferences and malloc in 25 of 32 local
arrays. Production +35/-35, net zero; permanent test code unchanged. Final
XP diff is 406 ranges, +488/-487 lines (975 total): twenty fewer added/deleted
lines, with six additional contiguous ranges caused solely by restoring
intervening original lines. No new differing location is introduced.
All original ranges appear in the [point map](m9-t100-sevid024-points.tsv);
all arrays appear in the [allocation inventory](m9-t100-sevid024-allocations.tsv).

The executor initially conflated contiguous range count with independent
mirror differences and tried restoring only two address expressions. That
candidate gave 398 ranges/991 lines but unnecessarily retained eight redundant
address adapters. The owner explicitly confirms restoring original code takes
priority over range fragmentation. All ten address lines now match pinned XP
exactly. The no-new-unjustified-difference requirement remains intact.

| Point | Executor analysis | Actual-diff counter-review and disposition |
| --- | --- | --- |
| R0384 | GDP native field mapping and declaration include. | Retain pending decision. The existing GDP owner is native slot storage, not a contiguous original 32-bit object. No second device implementation is added. |
| R0385 | Ten resolved native addresses routed through GDP address mapping. | Restore all ten exact XP lines. Two byte reads use native source locals 6/9; eight IU32 reads use sr_lookup (native field 1328) plus a bounded nibble-derived offset. These are host addresses, not GDP offset identities. Restoring eight of these splits existing ranges, not semantic differences. |
| R0386 | All 32 arrays use calloc and native capacities. | Restore malloc in 25 arrays with no partial-store/native-load overlap. Keep initialization in seven arrays: S2867/S2871/S2877-S2880 slot 0 and S2870 slot 7. These narrow assignments feed native address expressions. Poisoning all seven causes x64 access violation; original four-byte capacities with retained calloc zeroing independently cause x64 heap corruption. Both controls pass x86. Native capacities and the seven initializations remain owner decisions. |

The actual C-VID target and chain4_evid table retain this file: the final
mode-0 DWORD fill, twelve mode-0 DWORD moves, mode-1 byte/word/dword operations
and CopyByte helper, four mode-2 byte writes/fills and the first byte move.
There is no duplicate Compat device implementation to replace or basis for
whole-file deletion. Device algorithms, continuation calls, modes, guest
media/configuration and frame ownership are unchanged.

Task-local actual-body controls extend the initialized VGA fixture and
replace only this compilation unit. Each run executes 132 cases at four
address residues: twelve mode-0 DWORD moves plus final DWORD fill, nine
mode-1 operations, four mode-2 byte writes/fills plus the first move, and
direct S2870 calls with both source flags. Inputs are nonuniform, full planes
are compared across repeated runs. Current, full restoration and poisoned
malloc restoration pass on x86/x64; all six outputs hash to 9cdffc1d and the
existing VGA assertions pass. This is restoration/replay equivalence, not an
independent hardware oracle for every mode or proof of guest bulk consumers.

The earlier two-address candidate and its poisoned-malloc variant also pass
both widths with the same hash. Negative controls on that candidate remove
all seven retained initializations (x64 0xc0000005) or restore original slot
capacities while preserving remaining calloc zeroing (x64 0xc0000374); x86
passes both. These controls establish collective necessity, not independent
dynamic proof for each of the seven arrays. No timeout counts as evidence.

Actual-change review confirms 35 replacements only: ten exact original lines
and 25 malloc restorations with native capacity unchanged. Final source equals
the tested full candidate, preserves CRLF; normalized-LF SHA-256:
c7ce2e2a4ed8a5f308c8b3bb08649b99d2c3beaf834493878070d0f1df5d529e.
Full dual-architecture qualification is running. Delivered scope remains
306/472 until qualification, review and delivery complete.

### Follow-up required by the range-count clarification

A scan of prior range-increase dispositions identifies two existing wrappers
retained solely because of the earlier counting interpretation: file 292
sevid009.c line 2780 (R0308), and file 293 sevid010.c line 3228 (R0312).
Current source confirms both remain. Revisit them in the next complete P
under this S before advancing to file 308; do not put them in TODO or claim
the earlier interpretation proves necessity. Historical P39/P40 evidence
remains unchanged as history. This is an explicit correction revisit, not a
change to the frozen order of previously unaudited files. No production
change to those files is made during P55 qualification.

### P55 final qualification and actual-change review

Both cached builds/publications succeed. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first full run**, without retries
or changed budgets. Only the original switch-unreachable warning at line 651
remains, present in current and restored controls on both architectures.

- x86-build: 9.08 seconds, exit 0.
- x86-background: 135.1 seconds, exit 0.
- x86-native: 25.04 seconds, exit 0.
- x64-build: 12.09 seconds, exit 0.
- x64-background: 131.09 seconds, exit 0.
- x64-native: 25.15 seconds, exit 0.

Published softpc32.exe: embedded dates Nov 13 2026; times 11:19:03;
SHA-256 cccc805e88b0c07d4f81c384a15bc07ea366be4cd6b1f585d311b392e36a4c11.

Published softpc64.exe: embedded dates Nov 13 2026; times 11:21:52;
SHA-256 fccf9195df11c3a6beceb7ddf9091914ffd5d46ce80d0c95a3db6be230e9aaac.

Final source identity, CRLF and actual-change review pass. No permanent
test code or public-component changes. No new interactive guest acceptance
is claimed. Sequential audited scope is 307/472; next file 308
base/cvidc/sevid025.c after the recorded P39/P40 correction revisit. S1 remains active; residual decisions are not closed.

P55 process inventory confirms no owned worker remains. Task scratch was
previewed and removed via Clean-Build; residue verification passes. Persistent
caches, user configuration, media and snapshots remain untouched.

## P56 correction revisit: files 292/293, sevid009.c and sevid010.c

Owner's P55 clarification requires correcting two prior range-count decisions.
This revisits R0308/R0312 without renumbering or advancing the frozen file
frontier (307/472). No excluded bodies or already proven ABI residuals are
re-audited. P39/P40 remain historical records; their counting-based retention
of these two wrappers is superseded by this correction.

| Existing point | Restored location | Actual source proof |
| --- | --- | --- |
| R0308 | sevid009.c line 2780, XP line 2779, S2405 byte write | r21 is native sr_lookup (GDP field 1328) plus a bounded nibble index times four. Restore the original IU32 dereference; it is not a GDP offset identity. |
| R0312 | sevid010.c line 3228, XP line 3227, S2425 word write | r20 is native VGA_wplane (GDP field 1288) plus the guest offset expanded by four and advanced to the second word. Restore the original IU32 store, matching the preceding store's existing path. |

Exactly two lines change (+2/-2, net zero), each matching the pinned XP
line verbatim. Both files now contain no softpc_gdp_rule_address call.
No allocation, initialization, algorithm, continuation, public interface,
permanent test source or guest input changes. Native GDP field mappings and
previously justified allocation adaptations are unaffected.

### Metrics: fixed original-line correspondence versus heuristic matching

These two files have exactly the XP line count plus the native GDP include
at line 3. Removing that include for comparison establishes a fixed one-to-one
original line correspondence throughout each file; this is not a semantic
normalization or removal of local patch content from the metric. Count the
include itself separately as one added line.

| File | Anchored changed-line pairs | Anchored added+deleted lines, include counted | SequenceMatcher ranges | SequenceMatcher added+deleted lines |
| --- | --- | --- | --- | --- |
| sevid009.c | 436 -> 435 | 873 -> 871 | 342 -> 343 | 873 -> 871 |
| sevid010.c | 774 -> 773 | 1549 -> 1547 | 608 -> 632 | 1713 -> 1547 |

Actual restoration removes two changed locations/four replacement-diff lines.
The apparent heuristic reduction of 168 lines includes 164 lines of matching
realignment in repetitive generated code; it is not claimed as additional
cleanup. Likewise, the 25 added contiguous ranges are not new differences.
Original anchored content, changed lines and metrics remain independently
checkable against the fixed XP commit. Historical heuristic totals are retained
as such, not silently rewritten into anchored totals.

### Actual-body verification

The existing initialized VGA fixture is extended only in task scratch. Current
and candidate versions of both actual compilation units are explicitly linked
before the archive. Real unchained_evid entries select S2405 byte write and
S2425 word write. Each of four runs (x86/x64, current/candidate) passes 576
independent full-plane pixel cases: 16 low-nibble values (complementary high
nibble for WORD), three masks, three plane-enable patterns, two address
parities and both operations. Assertions cover the second WORD's destination
and untouched bytes, not merely successful return or repeated output. Existing
VGA assertions also pass. This tests these address restorations, not every
VGA mode or new interactive DOS/Win3.1/Win95 acceptance.

Original switch-unreachable warnings at lines 537 and 909 appear before and
after on both architectures. No new warning is introduced. Both production
files equal their tested candidates and retain CRLF. Normalized-LF hashes:

- sevid009.c: 6578f99fc8aab76d6ae111b7bee5aa7246fbcdceacc8907e7ef53a2dc51aeaee.

- sevid010.c: 97ce0562d31d38ff53032150d1eb37fa6ac30ec2e6ac41a9726002a983a285f8.

Cached dual builds, publication and full qualification are running. The
correction is not delivered until qualification and actual-diff review pass.

### P56 final qualification and actual-change review

Both cached builds/publications pass. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first full run**, no retries or
budget changes. Only the existing original switch warnings remain unchanged.

- x86-build: 9.08 seconds, exit 0.
- x86-background: 132.09 seconds, exit 0.
- x86-native: 25.03 seconds, exit 0.
- x64-build: 12.1 seconds, exit 0.
- x64-background: 150.13 seconds, exit 0.
- x64-native: 28.03 seconds, exit 0.

Published softpc32.exe: embedded dates Nov 13 2026; times 11:29:30;
SHA-256 59ad3d1655f6ba9d7de9380bd0c9e5be46941898992b07c83a892d9d8c08f6cc.

Published softpc64.exe: embedded dates Nov 13 2026; times 11:32:16;
SHA-256 dee06db2ac1d3f9931d2d4421e47f462a4c94bc61e9d156351055a8abe274b2b.

Actual-change review verifies exactly two XP-original line restorations,
matching the tested files. Public components, permanent tests, INI, media and
snapshots are unchanged. Both P39/P40 counting-only retentions are corrected.
The frozen frontier remains 307/472; resume at file 308 sevid025.c. S1 and
qualified residual decisions remain open; no new guest acceptance is claimed.

P56 process inventory confirms all owned jobs exited. Task scratch was
previewed and removed via Clean-Build; residue verification passes. Reusable
caches, configuration, media and snapshots are preserved.

## File 308: sevid025.c (P57)

Initial review covers the complete retained compilation unit in frozen order.
Pinned XP is a2f6d7c93aa4f11efc51147566d5961c59a8ef89, corresponding
Source/XPSP1/NT/base/mvdm/softpc.new/base/cvidc/sevid025.c. Its supported
C-VID object and evidfunc table entries remain production-linked; Compat does
not implement a duplicate of these rules. Whole-file retirement is inapplicable.
No existing exclusion body is inventoried.

The [initial range map](m9-t100-sevid025-points.tsv) covers 558 non-equal
ranges. The [allocation inventory](m9-t100-sevid025-allocations.tsv) covers all
32 local arrays, S2886-S2917. Complete line correspondence, allowing only the
existing GDP include at local line 3, classifies every initial changed line:
634 GDP accesses, 66 native-address accesses, 32 allocations and the include.
There is no unclassified algorithm, continuation or formatting difference.

| Point | Initial review | Independent counter-review and disposition |
| --- | --- | --- |
| R0387 | GDP mapped-field access and declaration. | Existing native GDP storage is not the original packed 32-bit layout. Keep the field mapping pending owner decision; restoring unrelated native-address dereferences does not require restoring this different representation. |
| R0388 | 66 address adapters around already resolved native pointers. | Trace lookup loads to native sr_lookup plus nibble*4 and plane/source accesses to native pointers plus offsets. Every restored line equals XP exactly. All address-adapter calls disappear from this file; no new helper or relocated logic. |
| R0389 | All 32 arrays use calloc and native capacity. | Restore malloc in 26 arrays. Six arrays write an IU32 slot and later read it as IUH: S2886/S2890 slot 7 and S2893-S2896 slot 0. Retain their zero initialization and all native capacities pending decision. Combined poisoned no-zero and original-capacity controls fail on x64; source review identifies each retained mixed-width slot. |

Production changes +92/-92, net zero; permanent tests are unchanged. Of those
92 lines, 66 restore exact XP address expressions and 26 remove unnecessary
zero initialization while retaining necessary native allocation size. XP-relative
heuristic and fixed-correspondence counts agree: +733/-732 (1465) before,
+667/-666 (1333) after, a reduction of 132 difference lines. Contiguous ranges
558 -> 556 are descriptive, not the reason for retention or restoration.

### Actual-body controls and an original discrepancy

The scratch fixture extends the existing initialized VGA unit, explicitly
linking current or candidate sevid025 before the production archive. Real
setWritePointers selects CHAIN4 mode 2 with states 8/9/14/15, rotate zero and
no dithering; assertions verify the selected vector, and calls use that vector.
It covers four address parities, WORD/DWORD write/fill/move, byte move, and
six directly called copy helpers with both source flags. The first DWORD move
belongs to this file; other variants are not falsely counted as its coverage.
Each run has 148 cases and repeats the full 4096-byte result for determinism.

Current, candidate and candidate-with-malloc-filled-0xa5 pass on both widths,
all with result hash 5bb43715. Existing VGA assertions also pass. Removing the
six retained zero initializations under poisoned allocation fails x64 with
0xc0000005; restoring 4-byte allocation capacity while preserving the six
calloc choices also fails x64 with 0xc0000005. Both controls complete on x86.
These are actual-body ABI controls, not a substitute for hardware correctness
or exhaustive proof of all possible register states.

The initial independently calculated per-plane pixel assertion fails on the
unchanged file as well as the candidate. Diagnostic runs isolate D004 above:
expanded bits for planes 1-3 are not selected by the generated stores. The
lookup table and low-byte stores are in pinned XP too. Later equivalence runs
print these mismatches instead of counting them as pixel-correctness passes;
they preserve the original result rather than quietly changing the oracle to
make it green. The recorded 148 cases prove restoration equivalence and
allocation behavior, not correction of D004. No new original-bug patch, TODO
item, guest symptom claim or sibling-project change is made.

Normalized-LF candidate SHA-256:
0df51b6df36f911d300cf25b6456866f556115e6badf0e54e24499e64536a5de.
The production file equals the tested candidate and retains CRLF. Cached
build/publication and full regression are running; the sequential frontier
remains 307/472 until final qualification and delivery.

### P57 final qualification and actual-change review

Both cached builds and publications pass. Each architecture passes 188/188
background and 8/8 native tests: **392/392 first full run**, no retries or
budget changes. Current and candidate isolated compiles have no warnings.

- x86-build: 14.18 seconds, exit 0.
- x86-background: 141.09 seconds, exit 0.
- x86-native: 26.03 seconds, exit 0.
- x64-build: 15.09 seconds, exit 0.
- x64-background: 133.1 seconds, exit 0.
- x64-native: 22.03 seconds, exit 0.

Published softpc32.exe: embedded Nov 13 2026 11:46:36; SHA-256 0f2a53f05b58ca003eeef04e7e1a7797af115db57f63b67199a2835a7acd1baa.

Published softpc64.exe: embedded Nov 13 2026 11:49:37; SHA-256 9a1aa24234b92131a144f5da4661c8010e2b9772bcff4e9e092c47c1c6258a39.

Actual-change review matches all 92 changed production lines to the tested
candidate: 66 exact XP address restorations and 26 malloc restorations with
necessary native capacity. No production algorithm, public component, permanent
test, INI, media or snapshot change. XP export hash matches frozen provenance.
Mirror and documentation gates pass; no new guest acceptance is claimed.
The sequential frontier advances to 308/472; next file 309 sevid026.c.
S1 and qualified residual decisions remain open.

P57 process inventory confirms all owned jobs exited. Exact task scratch was
previewed and removed via Clean-Build; residue verification passes. Reusable
caches, user configuration, media and snapshots are preserved.

## File 309: sevid026.c (P58 in qualification)

Pinned XP: a2f6d7c93aa4f11efc51147566d5961c59a8ef89; XP raw SHA-256 ea77886c01c66fe91c2021b870a1f0f100735decfdc3d88be4f2ebb893cc2fa2.
Initial 540 ranges; anchored original-relative diff 1245 lines. Candidate 515 ranges; 1195 lines, 50 fewer. Production +47/-47: 25 exact XP address expressions; 22 malloc restorations retain native-size allocation. Ten zero-initialized allocations and 32 native-size capacities remain pending decisions.
Actual VGA body fixture: mode 3 byte/word write, fill and move across four table variants and four parities; mode 2 three dword move entries; six helper entries with two source flags and four parities. Current/candidate/poisoned candidate pass x86 and x64: 156 cases per run, hash 61d0054f. Repetition and full-plane equivalence, not independent hardware conformance. Combined removal of ten zero initializations passes x86 but fails x64 with 0xc0000005; this does not attribute failure to every allocation. Source paths separately show partial-width writes before native-width reads.
Current mirror normalized SHA-256 88ec8914b47be3b4269692f30f202ad0af769f82b01f16a3de8e7ac44083bd3d. No guest reproduction asserted. Final build, tests, actual diff and publication remain pending.

Initial difference ranges and individual allocation dispositions are recorded in [point map](m9-t100-sevid026-points.tsv) and [allocation inventory](m9-t100-sevid026-allocations.tsv). Original GDP field mapping is retained pending owner decision; native-address and allocation changes are limited to this file. Source and dynamic evidence do not establish every guest configuration.

### P58 final qualification

Both cached builds and publications pass. Each architecture passes 188 background and 8 native tests: 392/392, first run. Mirror and documentation gates pass. Final source matches the tested candidate. No guest acceptance is claimed.

- x86-build: 11.09 seconds, exit 0.
- x86-background: 138.09 seconds, exit 0.
- x86-native: 26.03 seconds, exit 0.
- x64-build: 14.09 seconds, exit 0.
- x64-background: 137.1 seconds, exit 0.
- x64-native: 26.03 seconds, exit 0.

softpc32.exe: Nov 13 2026 12:04:02, SHA-256 4eef0ab34e9bfbcbc2acd2e9c694dae2083688c435b8e8da3d6e45407a1ce6f2

softpc64.exe: Nov 13 2026 12:06:57, SHA-256 4f3df6391feb222fc3756e38e95100270cf62cd06a3b3dd63107f83ae019ab4c

P58 owned jobs exited; task scratch was previewed and removed via Clean-Build. Residue check passes; caches, configuration, media and snapshots remain intact.

## File 310: sevid027.c (P59 in qualification)

Pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89, raw SHA-256 2ba72a0b38f90c4371e02a2ead62b0ba338d061ea63f114adb3b18fb6d4d52b1. Initial fixed correspondence finds 491 GDP mapping lines, 41 native-address wrappers, 32 allocation lines and the GDP include; no other changed lines. Initial 449 ranges are classified in [point map](m9-t100-sevid027-points.tsv); [allocation inventory](m9-t100-sevid027-allocations.tsv) records all 32.

R0393 retains GDP native layout pending owner decision. R0394 restores 41 exact XP dereferences. R0395 restores 28 malloc calls while retaining native capacity; four partial-width-write/native-width-read calloc calls remain pending owner decision. Production +69/-69. XP difference 1129 -> 1047 lines, reduction 82; ranges 449 -> 408.

Current, candidate and poisoned candidate each pass 56 RAM-source actual-body cases on x86 and x64, hash e5c42581. These exercise mode 0 backward byte move entries and four copy helpers, plus one mode 3 word move. Repeated full-plane comparison proves equivalence for these cases, not independent hardware correctness. Removing four retained zero initializations under poisoned allocation passes x86 but faults x64 (0xc0000005); this combined negative does not individually prove all four. Source review shows partial writes followed by native reads.

Video-source backward test cannot complete: actual fixture GDP backward string-read callback is NULL, and unchanged current and candidate both fault. Source search finds no setter call and backward glue functions only as definitions; CCPU backward move stubs call c_sas_not_used, while forward CCPU moves use scalar access. No guest failure or all-indirect-call impossibility is claimed. No new patch is added. This video-source path is not included in passing coverage.

Normalized LF final candidate SHA-256 16e804f7faa65927c40d05a297dd79bde17f4508c5077c6d892d3590c0703cac. Builds and full regression are running; frontier remains 309/472 until delivery.

### P59 final qualification

Both cached builds and publications pass. Each architecture passes 188 background and 8 native tests: 392/392 first run. Final source matches tested candidate. Video-source callback remains an unverified boundary, not a passed test or confirmed guest defect.

- x86-build: 13.09 seconds, exit 0.
- x86-background: 140.09 seconds, exit 0.
- x86-native: 26.03 seconds, exit 0.
- x64-build: 12.09 seconds, exit 0.
- x64-background: 140.09 seconds, exit 0.
- x64-native: 26.03 seconds, exit 0.

softpc32.exe: Nov 13 2026 12:19:56, SHA-256 0495315d3845a08fa96cdb4956a87747c81958ac0cf7d70eb07ca2f5ba8b5d53

softpc64.exe: Nov 13 2026 12:22:56, SHA-256 d0819099715403bd0c6033db9476c5099f841392d599789942d1bd9c0c3acb7c

P59 owned jobs exited; task scratch was previewed and removed via Clean-Build. Residue check passes; caches, configuration, media and snapshots remain intact.

## File 311: sevid028.c (P60 in qualification)

Pinned XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89, raw SHA-256 5dcaf32b871e4d86717642fc17962a3d54092206ea293f1df521c28a674adb61. Initial fixed correspondence finds 622 GDP mapping lines, 62 native-address wrappers, 32 allocation lines and the GDP include; no other changed lines. Initial 572 ranges are classified in [point map](m9-t100-sevid028-points.tsv); [allocation inventory](m9-t100-sevid028-allocations.tsv) records all 32.

R0396 retains GDP native layout pending owner decision. R0397 restores 62 exact XP dereferences. R0398 restores 30 malloc calls while retaining native capacity; two partial-width-write/native-width-read calloc calls remain pending owner decision. Production +92/-92. XP difference 1433 -> 1309 lines, reduction 124; ranges 572 -> 510.

Current, candidate and poisoned candidate each pass 68 RAM-source actual-body cases on x86 and x64, hash 90163a59. These exercise 12 mode 0 backward word entries, three backward byte entries and two direct copy helpers. Repeated full-plane comparison proves equivalence for these cases, not independent hardware correctness. Removing both retained zero initializations under poisoned allocation passes x86 but faults x64 (0xc0000005); this combined negative does not individually prove both. Source review shows partial writes followed by native reads.

Video-source backward callback remains the unverified boundary documented in P59; it is not included in passing coverage. Normalized LF candidate SHA-256 1cea245a945bf9cbddd800344d6893235b4556447a767deb70e4cf83d9f8b75f. Builds and full regression are running; frontier remains 310/472 until delivery.

### P60 final qualification

Both cached builds and publications pass. Each architecture passes 188 background and 8 native tests: 392/392 first run. Final source matches tested candidate. Video-source callback remains an unverified boundary, not a passed test or confirmed guest defect.

- x86-build: 11.09 seconds, exit 0.
- x86-background: 150.09 seconds, exit 0.
- x86-native: 25.03 seconds, exit 0.
- x64-build: 14.1 seconds, exit 0.
- x64-background: 141.1 seconds, exit 0.
- x64-native: 25.03 seconds, exit 0.

softpc32.exe: Nov 13 2026 12:32:03, SHA-256 431841c12d5859311d2e8b0c1fe7e0d927e40360ec85acd9546044e4b89f3a74

softpc64.exe: Nov 13 2026 12:35:10, SHA-256 cdbe6b20be7f8c0eaeb5300759050ae6f6514e3b8a308461258be075a5f8858a

P60 owned jobs exited; task scratch was previewed and removed via Clean-Build. Residue check passes; caches, configuration, media and snapshots remain intact.

### P61: P59/P60 video-source callback investigation

Owner rejected advancing with an unexplained callback boundary. File 312 work
was held without production edits. This follow-up supersedes the unexplained
boundary classification above; it does not add direct legacy bulk calls to
the passing test count.

Source proof at e1b0ca18:

- Both supported architectures select CCPU, C_VID and CPU_40_STYLE in root
  CMake. In pinned XP ega_read.c, assignments to bwd_str_read_addr occur in
  the alternative !CPU_40_STYLE path. The selected original path calls
  SetReadPointers instead. Current ev_glue.c setReadPointers installs the
  c_ev_read_ptr table and read_byte_addr, not GDP offsets 1332/1336.
- GDP aggregate storage starts zeroed. Its offset mapping identifies 1332
  and 1336 as fwd_str_read_addr and bwd_str_read_addr. Their presence in the
  generated getter/setter vector is not a production setter invocation.
- Production symbol search finds the three backward glue wrappers only as
  definitions. The active Glue_writes registration supplies forward moves;
  ccpu386's move-pointer arrays are populated but their read macros have no
  consumers. CCPU MOVSB/MOVSW/MOVSD in c_main.c directly use F_X/P_Y operands;
  c_oprnd.h selects vir_read/vir_write and updates SI/DI according to DF.
  Thus the actual instruction path does not dispatch the legacy bulk wrapper.
- The earlier direct bulk probe bypassed that instruction path. With
  srcInRAM=0, sevid027.c calls GDP offset 1336, which is NULL under this
  configuration. The fault predates P59/P60 restoration. It is not a Window
  frame-publication callback and not evidence of a guest MOVS failure.

Dynamic proof: a disposable variant of the existing vga_frame_smoke fixture
uses the actual machine create/reset, original BIOS mode 13h dispatch, and
softpc_machine_run. It writes a small diagnostic instruction sequence to
scratch guest RAM, without changing firmware or original guest binaries.
REP MOVSB, MOVSW and MOVSD each copy four elements between disjoint video
offsets 0x100 and 0x400 with DF=0 and DF=1. Every destination byte and completed
CX is asserted. GDP callback 1336 is asserted NULL before and after every
case; both callback values are printed after mode initialization.

Cached x86 and x64 production objects were linked with only the disposable
fixture object replaced. Compile, link and execution all exit 0: six cases
per architecture, twelve total. Forward final SI/DI are 104/404, 108/408,
110/410; backward values are ff/3ff, fe/3fe, fc/3fc. Both callbacks are NULL
on both architectures. This independently proves the selected scalar path
can copy video data while the legacy callback remains unbound.

Counter-review: no initializer was lost in P59/P60; the pinned XP conditional
choice explains its absence. No new initializer, fallback, source branch or
mirror patch is justified by this probe. The direct legacy video-source
bulk entry remains unusable in the current configuration and must not be
treated as a supported production entry by future callers without separate
integration work. The six instruction cases cover mode 13h, disjoint ranges
and read/write mode 0, not all video modes, overlap, paging or real DOS/Windows
sessions. The earlier RAM-source equivalence results retain their stated scope.

No production, permanent test or published EXE changes; this is a source and
focused dynamic investigation, not another full-suite run. The frozen frontier
remains 311/472. Existing residual decisions remain open.

P61 owned-process inventory confirms all probes exited. Clean-Build preview,
exact task-scratch removal and residue check pass. Documentation gate and
actual documentation diff review pass; configuration and guest assets are untouched.

## File 312: sevid029.c (P62 in qualification)

Pinned XP raw SHA-256 0b7477892fb8e55fddd49bdf7712ca0e61d86a40b09879e026e6fb5684d85428.
Complete fixed-line correspondence identifies 497 GDP mapping lines, 52 native
address wrappers, 32 allocations and the GDP include; no other changed lines.
The [point map](m9-t100-sevid029-points.tsv) and
[allocation inventory](m9-t100-sevid029-allocations.tsv) record these differences.
R0399 retains native GDP layout pending owner decision. R0400 restores 52 exact
XP native address expressions. R0401 restores 29 malloc calls, retaining native
capacity; three zero initializations remain qualified decisions. Their local
slot 7 is assigned through IU32 and then read through IUH for address addition.
Other mixed-width mentions include a full-width initialization and do not
justify preserving calloc merely because two widths occur in one function.

Production +81/-81. Anchored XP added+deleted difference 1163 -> 1059, reduction
104; SequenceMatcher ranges 456 -> 436. Normalized final candidate SHA-256
9e383214bf702c6b739891198273b541edb3a20b4aa87521e501a03f1a0fbb78.

Disposable actual-body fixture calls all 32 generated entries with RAM-source
flag and four destination parities. Original production machine initialization
and BIOS mode 13h precede calls; each case initializes source and video bytes,
then hashes the complete 256 KiB plane image. Current, candidate and malloc
poison candidate pass all 32 processes per architecture, 128 parity cases per
variant. All corresponding output hashes match. This proves these restoration
cases equivalent, not independent hardware conformance or every guest mode.
Legacy video-source callback scope is governed by the completed P61 investigation,
not silently counted as passing here.

Removing the three remaining zero initializations under poisoned allocation
passes all 32 x86 entry processes but causes x64 access violations, including
direct helper entries S_3026, S_3030 and S_3034. Each helper has its own allocation
and the identified partial-write/native-read slot. Other failing callers inherit
those helper faults; their allocations are not thereby qualified for calloc.
No new production repair or initialization path is introduced.

Final cached build, publication and full regression remain in progress.
The sequential frontier stays 311/472 until complete P62 delivery.

### P62 final qualification

Both cached builds and publications pass. Each architecture passes 188 background
and eight native tests, 392/392 first run. Final production source equals the
actual-body candidate. No original guest or desktop acceptance is inferred
from the function-level equivalence probes.

- x86-build: 13.02 seconds, exit 0.
- x86-background: 141.43 seconds, exit 0.
- x86-native: 25.05 seconds, exit 0.
- x64-build: 15.75 seconds, exit 0.
- x64-background: 141.65 seconds, exit 0.
- x64-native: 24.82 seconds, exit 0.

softpc32.exe: Nov 13 2026 12:54:56, SHA-256 11c1c86a0f1619727f7b0e085f09d87cc5655b42888cade475d44a24eae75aa4.

softpc64.exe: Nov 13 2026 12:57:55, SHA-256 2518ac9c3b50a9228a471dd44b5fa5f280d9feaba05208df030fd2dca21d9096.

Independent actual-change review reconciles all 81 production changes with the
tested candidate: 52 exact original dereferences, 29 malloc restorations with
required native capacity. There is no new algorithm, public-component change,
permanent test, INI, media or snapshot change. Original trailing whitespace on
restored lines is deliberately preserved, not reformatted. Manifest and
documentation gates pass. Frontier advances to 312/472; next is sevid030.c.
S1 and qualified residual owner decisions remain open.

P62 process inventory confirms all owned jobs exited. Exact task scratch was
previewed and removed using Clean-Build; residue verification passes. Reusable
caches, configuration, media and snapshots remain intact.

## File 313: sevid030.c (P63 in qualification)

Pinned XP raw SHA-256 2b5337bc2a577e43d54e060d901c775af8e9b1e91918e0913c79d50a9cbd54dd.
Full fixed-line correspondence identifies 329 GDP mapping lines, 34 native
address wrappers, 23 allocations and the GDP include; no other changed lines.
The [point map](m9-t100-sevid030-points.tsv) and
[allocation inventory](m9-t100-sevid030-allocations.tsv) cover those differences.
R0402 retains native GDP layout pending owner decision. R0403 restores 34 exact
XP native-address expressions. R0404 restores 21 malloc calls with native
capacity retained. S_3050 and S_3054 retain zero initialization: their local
slot 7 receives an IU32 offset, then is read as IUH for native address addition.
Full-width initialization in other mixed-width slots does not justify calloc.

Production +55/-55; anchored XP added+deleted diff 773 -> 705, reduction 68;
SequenceMatcher ranges 333 -> 299. Final normalized candidate SHA-256
32448fead96be5ac96d5419c60c373bef4b04a43b63e024679c79ce33c054ca9.

Actual-body probe uses original machine initialization and mode 13h, all 23
generated entries, four destination parities and two bit-protection masks.
The eight direct copy helpers also exercise both source flags; the public
bulk entries use RAM source only. Nonzero latches and set/reset values make
mode 3 output observable. Each invocation starts from the same patterned
256 KiB plane image. Current, candidate and poisoned-malloc candidate complete
23 isolated processes per architecture, with 248 cases per variant, and
corresponding complete-plane hashes agree. This is restoration equivalence,
not independent hardware conformance or all guest modes. The unbound legacy
video-source callback remains outside supported bulk-entry coverage, for the
specific source and dynamic reasons established in P61.

With only the two remaining zero initializations removed and malloc poisoned,
all x86 entries pass but direct S_3050 and S_3054 calls fail on x64 with
0xc0000005. Dependent byte/word/dword entry failures inherit those helper faults;
they do not justify retaining zero initialization in other allocations. No
new initializer, callback connection, algorithm or production test API is added.

Final cached build, publication and full regression are pending. The frozen
frontier remains 312/472 until complete P63 delivery.

### P63 final qualification

Both cached builds and publications pass. Each architecture passes 188 background
and eight native tests, 392/392 first run. Final source equals the actual-body
candidate. The function-level comparisons do not establish every guest mode.

- x86-build: 9.47 seconds, exit 0.
- x86-background: 137.97 seconds, exit 0.
- x86-native: 24.78 seconds, exit 0.
- x64-build: 12.4 seconds, exit 0.
- x64-background: 142.73 seconds, exit 0.
- x64-native: 24.75 seconds, exit 0.

softpc32.exe: Nov 13 2026 13:06:46, SHA-256 5ecd0d3d0462fd4f20f3706941e0b09a2e09c7907aa28f4b52aa791e1b7e6555.

softpc64.exe: Nov 13 2026 13:09:38, SHA-256 ac9d5c9cf721ac8fd3c374b03944cf99c9481c32a0d1140c3cd59da27b3adb17.

Actual-change counter-review reconciles all 55 production changes to the tested
candidate: 34 exact original dereferences and 21 malloc restorations retaining
native capacity. No new algorithm, public component, permanent test, INI, media
or snapshot change. Original whitespace is preserved on restored mirror lines.
Manifest and documentation gates pass. Frontier advances to 313/472; next is
sinit011.c. S1 and evidence-qualified residual owner decisions remain open.

P63 process inventory confirms all owned jobs exited. Task scratch was previewed
and removed with Clean-Build; residue verification passes. Reusable caches and
user configuration, media and snapshots are preserved.

## File 314: sinit011.c (P64 verified delivery)

Pinned XP raw SHA-256: 57ebd03b4e94c8ef89556cfd8d19d5963d5680f66757635fc0b8cfed24847771.
Removing only the added GDP include aligns every remaining line with XP:
420 GDP access replacements and 20 allocation replacements; no other changed
line or excluded body occurs. This initial classification covers the entire file.

R0405: the include and 420 accesses use the existing original-offset-to-native
GDP mapping. The actual state is the Compat-owned indexed allocation, with VGA
members at native VGAGLOBALSETTINGS offsets, not one original flat 32-bit GDP.
Restoring raw offsets would access the wrong object; no new mapping is added.
The generated getters remain registered in VideoVector by vglfunc.c. Existing
video consumers include snapshot capture in vga_prts.c and dirty accounting in
gfx_updt.c; this file cannot be classified as an unused test-only component.

R0406: all 20 entries allocate one IUH local. Eight pointer getters fully write
that native-width slot before reading it: rplane, wplane, scratch, sr_lookup,
fwd_str_read_addr, bwd_str_read_addr, video_copy and mark_byte. Restore their
original malloc operation, retaining sizeof(IUH) capacity for x64. The twelve
scalar entries write IU32 and then load IUH for the generated function return.
Their zero initialization is an evidence-qualified mixed-width ABI residual:
the generated function body returns IUH and must not read an indeterminate
upper half after the host-width expansion. The typed VideoVector consumers
return IU32: a high-half mismatch
alone is not evidence that those consumers malfunction.

Focused experiment compiles the actual complete generated source with cached
production flags and links the existing VGA fixture dependencies, replacing
only its disposable main and this generated object. The real GDP allocator and
mapping are used, not duplicate mapping stubs. All 20 getters receive four
values; pointer cases also carry nonzero upper bits on x64. Current, candidate
and candidate with malloc memory filled with A5 pass 80 full-return assertions
each on both architectures. A negative variant also removes the remaining
twelve zero allocations: x86 passes, x64 fails at latches with
A5A5A5A587654321 instead of 87654321. This proves the generated native return
depends on zero initialization, not a guest video failure. No guest binary is
modified, and this isolated proof is not a real DOS/Windows session test.

Candidate production delta is +8/-8. Anchored XP differing lines remain
881 (one include plus 440 replaced lines counted on both sides): malloc is
closer to the original operation, but native capacities still differ. No
mirror diff-line reduction is claimed. Final normalized source SHA-256 is
a19fb94bbc6826d8831a31e6bf5a4bf2dd941fe9110da00353171845c4d0c755.
Dual-width build/publication and full regression are running in owned scratch.
File 314 is not delivered, the verified frontier remains 313/472, and no
residual decision is reported approved.

P64 final qualification: x86 build 9.38s, background 139.71s, native 23.45s;
x64 build 14.39s, background 139.41s, native 23.83s. Each architecture passes
188 background and eight native tests on the first run: 392/392 total.
Mirror manifest and documentation gates pass. Production is exactly the tested
candidate, +8/-8; no permanent test-source change. The line inventory is
[m9-t100-sinit011-points.tsv](m9-t100-sinit011-points.tsv).

Published softpc32.exe: Nov 13 2026 13:19:19, SHA-256
f835b53aba6f48e5c88ea52c70ec2e23015daeb3755e92484602a58b89e8c23e.
Published softpc64.exe: Nov 13 2026 13:22:12, SHA-256
95fb7998ec5326886b5d7b829960c32da9beef6b0bb03e207ded2d886544b8bd.

Independent actual-diff counter-review confirms only eight allocation-operation
restorations, with original whitespace and native capacity preserved. GDP
mapping and mixed-width initialization remain evidence-qualified owner
choices; no new helper, path or interface was introduced. The native-return
negative test must not be described as a guest failure through typed IU32
consumers. Configuration/media/snapshots are unchanged. Verified frontier is
314/472, next file 315 sinit012.c; S1 remains active.

P64 owned-process inspection confirms all probes and qualification jobs exited.
Clean-Build preview, exact scratch removal and residue check pass. Only the
two reusable cache roots remain; no raw logs or probe binaries are retained.

## File 315: sinit012.c (P65 verified delivery)

Pinned XP raw SHA-256: 6f0868fde324613c1d55c0bece49d42978ee2e9b37c30ca3d48a5e3a025009ef.
After removing the one GDP include, every line has a fixed XP correspondence:
672 GDP replacements and 32 allocation replacements, no other change. No
excluded bodies are counted. R0407 covers the include and native GDP accesses;
R0408 covers the local allocations. Exact locations are recorded in
[m9-t100-sinit012-points.tsv](m9-t100-sinit012-points.tsv).

The 32 entries comprise five pointer getters, nine IU32 getters, five byte
getters and thirteen setters. Each allocates one local slot. Pointer getters
and setters fully initialize the native slot before reading it: restore
18 malloc operations, retaining sizeof(IUH). Byte getters only read/write
IU8 within the slot, so restore all five original malloc(4) lines exactly,
including original whitespace with the required mirror CRLF. Nine IU32 getters
write a low word and load IUH at return; their zero initialization remains an
evidence-qualified native-width residual, as distinguished from typed IU32
consumer behavior in P64. Native GDP mapping remains necessary because current
GDP is indexed storage and native VGAGLOBALSETTINGS members, not the original
flat 32-bit byte layout. No duplicate Compat accessor implementation replaces
these generated entries; vglfunc.c registers them in the existing VideoVector.
Video read-mode and V7 bank consumers remain in ega_read.c/ega_prts.c.

Focused proof uses complete actual source bodies and the real GDP allocator
and mapping, linked with cached production dependencies. All 32 entries receive
four values, including pointer upper bits on x64; getters assert their result,
setters assert the stored field. Byte getter assertions use their actual byte
contract. Current/candidate/poisoned-malloc variants each pass 128 assertions
on x86 and x64. Removing the nine remaining zero allocations also passes x86,
but x64 returns A5A5A5A587654321 rather than 87654321 at read_shift_count.
This demonstrates indeterminate high return bits, not a reproduced guest
failure through the typed scalar consumers. The first disposable harness
mistakenly supplied zero as setter v1, producing identical current/candidate
failures at the first setter; corrected argument delivery passes. No production
change was made to accommodate that fixture mistake.

Candidate +23/-23 restores 23 allocation operations, including five entire XP
lines. Anchored XP added+deleted diff is 1409 -> 1399, a reduction of ten lines;
source line count is unchanged. Final normalized source SHA-256 is
c80398bd17c057fb0a33a19f6be7c262247ae02bd10ce1df71ca33324c82bdd9.
Exact-final-source probes and dual-width full qualification are running.
Verified delivery frontier remains 314/472; no S closure or residual approval
is claimed.

P65 final qualification results (seconds): x86-build 8.88, x86-background 138.85, x86-native 25.35, x64-build 13.92, x64-background 141.62, x64-native 24.42.
Each architecture passes 188 background and eight native tests, all first run:
392/392 total. Final-source focused probes, mirror and documentation gates pass.
Actual-diff counter-review reconciles all 23 changed production lines and no
other production/test changes. Five byte allocations match XP verbatim except
required mirror line endings; 18 native-sized allocations restore malloc.
SequenceMatcher with autojunk=False reports XP ranges 97 -> 92 and
+705/-704 -> +700/-699, agreeing with the anchored ten-line reduction.

softpc32.exe: Nov 13 2026 13:30:13, SHA-256 9d156dbe60a4904fb0a1b25f124476f4e6b3f7e5402cf573724b7b22f487ab39.

softpc64.exe: Nov 13 2026 13:33:06, SHA-256 eebbcff13ac9018062db744ab8ff6b508b2e2ffdf9bafaf7785e18b50c6d2e82.

The nine mixed-width zero initializations and GDP mapping remain qualified
owner decisions, not approved retention. No guest session acceptance or
full video-mode matrix is inferred from getter/setter probes. Verified frontier
advances to 315/472; file 316 sinit013.c is next. S1 remains active.

P65 owned-process inventory confirms all probes and test jobs exited.
Clean-Build preview, task scratch removal and residue check pass. No probe
binary or raw log is retained; reusable caches and user assets are preserved.

## File 316: sinit013.c (P66 verified delivery)

Pinned XP raw SHA-256: 384a566f06caa49330ad57b385fcc8dfe489efefb319e4673e30d7a95831ada4.
One added include accounts for the line shift; all remaining lines align with
XP. Complete classification: 689 GDP access replacements, 29 allocations,
three ordinary-address wrappers, no other changed lines or excluded bodies.
Locations are in [m9-t100-sinit013-points.tsv](m9-t100-sinit013-points.tsv).

R0409: retain the existing native GDP mapping as an evidence-qualified owner
decision. Actual GDP indexed storage and native VGAGLOBALSETTINGS members do
not implement the original flat 32-bit byte layout. The 26 field setters are
registered by vglfunc.c; no duplicate Compat setter substitutes for them.
This file remains required and cannot be retired as test-only code.

R0410: restore all three original dereferences. Each address comes from
j_modeLookup plus a table index, not an encoded GDP offset. The original
symbol is initialized to a native object's address in ev_glue.c. The removed
wrapper would return that same native address; it does not repair the table
indexing. Restore original syntax and whitespace, not an equivalent rewrite.

R0411: restore all 29 malloc operations, preserving native capacities. The
26 setters write their full IUH local from v1 before reading it, including
byte setters. Write-pointer selection initializes its local from the table
base before branching. Read/mark selectors initialize both native locals
from the argument and table base before use. No allocation in this file needs
zero initialization. Single-slot capacities remain sizeof(IUH); two-slot
capacities remain 2u * sizeof(IUH), avoiding x64 undersized allocations.

Production wiring was independently checked against pinned XP: selected
CPU_40_STYLE/C_VID egacpu.h undefines SetWritePointers/SetReadPointers/
SetMarkPointers to the C functions in ev_glue.c. The original ev_glue.c note
explicitly says generated selection was replaced by C and placeholder symbols
remain for linking. Current C_Video registers field accessors but not the
three S_0441/S_0442/S_0443 selectors. Symbol search finds those generated
entries only in their own definitions/address exports. This is original
wiring, not a new local bypass or a newly supported legacy interface.

Focused actual-body proof: compile the complete current/candidate/poisoned
candidate file with production flags and real GDP mapping, linked to cached
production dependencies. Each width passes 26 setters times four values =
104 storage assertions (native pointer high bits included on x64). Separately,
controlled aligned table storage exercises old selectors: 96 write combinations
(dither, chain, write mode, rotate, write state), 12 read selections and four
mark selections. Resulting GDP-field fingerprints match all three variants
within each width. All compile/link/process exits are zero. The 112 selector
cases are isolated restoration comparisons with controlled tables, not proof
that the unregistered legacy selectors work with current production tables.
No guest binary is changed; no DOS/Windows-mode acceptance is inferred.

Candidate production +32/-32; XP +722/-721 -> +719/-718, reducing six diff
lines. SequenceMatcher autojunk=False ranges 174 -> 171; fixed line alignment
agrees. Normalized final SHA-256:
05a717cff69ad8740cc64784eaefff2bd4d00b43603966b502f16e9a15334e15.
Full dual-width qualification is running. Verified frontier stays 315/472;
S1 and owner decisions remain open.

P66 qualification times (seconds): x86-build 8.87, x86-background 139.56, x86-native 23.46, x64-build 10.8, x64-background 138.66, x64-native 24.78.
Each architecture passes 188 background and eight native tests first run:
392/392 total. Mirror and documentation gates pass. Final source is byte-for-byte
the tested candidate and uses CRLF throughout. Counter-review checks every
changed line: 29 malloc operations and three exact original native dereferences;
no other production or permanent test-source change. No public component,
user configuration, media or snapshot is modified.

x86 current/candidate/poison focused output SHA-256 (identical within width): 4fb4562283ec22ca11d759384e3e1f66b93fe9ac3b95a74edfbfa4ed381ab2fc.

x64 current/candidate/poison focused output SHA-256 (identical within width): 718b69c3ba7165f6154de3e5feebe5f2e7010a44450eb4131152891654c20bb5.

softpc32.exe: Nov 13 2026 13:40:34, SHA-256 5f07d82496aaad29eb6d0efa2c0d7c4e416a911def25163ead4375bc40a2c2bb.

softpc64.exe: Nov 13 2026 13:43:26, SHA-256 68283ce57b80536cb7ae74f4e336ae62d2f552711ccc677ee4d7919b969b9f27.

Production +32/-32; anchored XP diff decreases by six lines. Native GDP and
capacity residuals remain qualified owner decisions. Isolated legacy-selector
comparisons do not establish a new supported production path. Verified frontier
advances to 316/472; next file 317 univer_c.h. S1 remains active.

P66 owned-process inspection confirms all jobs exited. Clean-Build preview,
exact task scratch removal and residue check pass. Reusable caches remain;
temporary probes/logs and disposable binaries are removed.

## Files 317-318: univer_c.h and vglfunc.c (P67 read-only qualification)

File 317, univer_c.h: fixed XP SHA-256
6aa718d4f8fcd4e0774c21eef3516b6fa37c1c2d1fecc1433c739cedef9a3c1d.
Entire normalized text equals XP; local CRLF follows mirror policy. No code
restoration or diff decision is needed. Repository source/test/CMake search
finds no include consumer. Whole-file removal would introduce a new difference
against an otherwise identical original file, prohibited by the owner's
no-increase clarification; retain the original file, not a claimed runtime
dependency. No excluded-body accounting is introduced.

File 318, vglfunc.c: fixed XP SHA-256
d1adc782d8a812db646ccf71abc9e59a394086ae2bbba236bbb407e7c80de36b.
Only one addition exists: local source lines 6-10, five lines (+5/-0).
R0412 is an evidence-qualified declaration adaptation pending owner decision.
It undefines IFN1 and redefines it to a typed parameter list for this translation
unit; the three-line comment is its existing required local reason marker.
All original declarations, vector entries and their order remain unchanged.

Real consumer/compile proof: root CMake includes vglfunc.c in the C-VID core.
Its C_Video vector registers the generated getters/setters examined in the
preceding files. The selected insignia.h IFN1 is a K&R function-definition
macro, expanding to a parameter-name list followed by a parameter declaration;
that expansion is not a valid standalone extern declaration. Restoring the
entire fixed XP vglfunc.c, rather than a broken subset of its declarations,
fails with the actual x86 and x64 production compiler flags. Both report
"expected declaration specifiers before ';' token" at XP line 46, the first
setter declaration. Current unchanged vglfunc.c compiles on both architectures.
All four jobs terminate normally: XP exits 1/1, current exits 0/0.

Counter-review: changing the global IFN1 would affect actual old-style function
definitions outside this declaration-only unit; it is not a smaller coherent
restoration. The current local adaptation preserves the existing single vector
and introduces no second path, lifecycle or guest behavior. Retain its five
lines for the owner's decision with this concrete compilation evidence.
No new source/test/manifest/EXE changes; no new full-suite or guest validation
is claimed. P66 remains the published 392/392 executable baseline.
Verified ordered audit frontier advances to 318/472; next file 319 diskbios.c.
This is not S1 closure or approval of the retained difference.

P67 compiler processes have exited. Owned scratch was previewed and removed
with Clean-Build; residue and documentation checks pass. Worktree review
confirms documentation/inventory changes only, with production and assets intact.


## File 319: diskbios.c (P68 read-only qualification)

Pinned XP raw SHA-256:
779cb7418924632a9ec3ef7a5b5dd1a6d5c7bf88ba9afc8cf69b66e10876d98d.
Full-file comparison outside the skipped exclusions/line controls identifies
only the relocated historical notice and the two read_dasd register assignments.
No excluded bodies receive new accounting. No production restoration is justified.

| Point | XP/current location | Disposition and evidence |
| --- | --- | --- |
| R0413 | Historical notice, current line 22 | Preserve existing notice under source policy, as in preceding audited files. |
| R0414 | XP 1663-1664/current 905-906 | Existing capacity-return correction is evidence-qualified, pending owner decision. XP divides/remainders by 256 and casts both results to UCHAR; current code splits the sector count into two 16-bit registers with 65536. |

Real production path: root CMake includes diskbios.c. disk_io reads AH and
its command table selects read_dasd for 0x15. getdpb reads the guest disk
parameter table; read_dasd computes the original reserved-cylinder capacity
and returns AH=3, CF=0 and CX:DX. No Compat capacity implementation replaces
this path. The audited difference changes only the two register assignments,
not geometry, connection policy, the reserved cylinder or media access.

Focused proof reuses the actual device_contract_smoke disk case and cached
production objects. A disposable copy restores both assignments together to
exact XP; current and restored copies compile/link with each architecture's
actual production flags. Test fixture and all other linked objects are the same.
The initial invocation omitted the required disk argument: all four executions
failed the argc assertion before exercising BIOS. Those failures are invalid
capacity evidence and were corrected, not counted as product failures.

With the correct argument, current x86 and x64 return 0. Both verify all three
capacity pairs (0000:0484, 0001:0000, 000f:bc10), AH/CF and absent-drive dispatcher
semantics through BIOS[BIOS_DISK_IO], without real media or guest modification.
Both exact-XP controls fail the first CX assertion at fixture line 1542
(x86 exit 3221226505, x64 exit 3). For that 1156-sector case XP's source expression
produces CX=4 and DX=132, rather than CX=0 and DX=1156. These numerical original
values are source calculation; the dynamic negative stops at the CX assertion
and does not prove execution of the other two negative capacity cases.

Counter-review: restoring only the UCHAR casts or only the divisor would not
restore the original, and would not provide a correct 16-bit register pair.
The coherent two-line control demonstrates a real supported BIOS contract
failure. Preserve the existing two-line correction for owner disposition;
no new wrapper, special case, test API or production patch is introduced.
This focused BIOS test is not a DOS/Win3.1/Win95 boot or complete disk regression.

Production/test/build/manifest/EXE delta is zero; XP-relative diff delta is zero.
The reviewed active semantic replacement remains +2/-2, plus the one-line
historical notice. No whole-file excluded-content metric is added. Published
EXEs and the last full 392/392 qualification remain P66; no new full-suite run
is claimed. Ordered frontier advances to 319/472; next file 320 fdisk.c.
S1 remains active and qualified residual decisions remain pending.

P68 authoritative process inspection confirms the owned jobs exited. Clean-Build
preview, exact scratch removal and residue check pass. Documentation gate and
actual diff review pass; only the three audit/state documents change.


## File 320: fdisk.c (P69 read-only qualification)

Fixed XP raw SHA-256:
bd7218dcec4af7db766845baa45356ab9c5f812e0159c603904a897ad785c2e4.
Complete comparison outside existing skipped exclusions/line controls finds
one include at current line 236 and an appended 119-line block at 1896-2014.
No original retained controller algorithm, declaration or expression is changed.
Active additions are +120/-0, unchanged by this audit. No excluded-body accounting.

| Point | Location | Evidence-qualified residual, pending owner decision |
| --- | --- | --- |
| R0415 | current 236, 1896-2014; XP insertion before 1922 | Existing HDD snapshot types, stable callback/command encoding and private controller-state capture/restore. |

Production chain: machine/snapshot_image.c calls the CCPU archive; compat/ccpu/
archive.c invokes device archive capture/restore, whose archive.c directly calls
both HDD state functions and dispatches both HDD callback codecs. The root build
includes the original fdisk controller. Its private drives[2], pseldrv, taskfile,
sector buffer/index, fixed/digital registers and activecmd are the actual device
state, not a duplicate model in Compat. The archive only stores and serializes
that state. Removing this extension loses access to the private state; deleting
its callers as well removes HDD checkpoint coverage rather than restoring an
equivalent supported snapshot path. Exporting the original locals or recreating
the device in Compat would add different mirror changes or a second authority.

The added fields are consumed by hdd_drive_fields and hdd_fields in archive.c.
Each copied drive scalar, all ten taskfile bytes, 256 sector words, registers,
sector index and selected-drive/command IDs map back to the same original owner.
Pointers are encoded rather than serialized as process addresses: pseldrv maps
to -1/0/1, and the ten-entry command table includes null and the original command
functions. This table is not interchangeable with the original dispatch table:
its stable IDs/order differ and it includes diagnose. Restore validates indices
before changing state. No extra device command execution is introduced.

The two callback IDs refer to actual delayed events: rsector schedules
fdisk_pause at current line 769; irq schedules disk_int_call_back at
1346. The former resumes activecmd. The archive's shared event codec calls the
HDD encoder/decoder; these are not test-only entry points. Codecs reject unknown
pointers/IDs and do not invent a fallback. Their necessity is established by
this source/caller/private-state proof, not by pretending the focused checkpoint
test dynamically exercises every delayed HDD callback or in-flight command.

Focused dynamic evidence: the existing checkpoint_smoke CTest passes on x86
and x64 (1.69 and 1.66 seconds including CTest). Its HDD section changes taskfile
ports, captures state, overwrites ports, restores and compares the taskfile.
All ten active-command IDs round-trip through the actual capture/restore
functions; -1 and 10 are rejected. The full existing fixture also runs its
Machine checkpoint path. This does not prove all disk geometries, every partial
sector transfer, every delayed callback timing or DOS/Windows guest sessions.
No new permanent test or production API is added.

Counter-review: all non-excluded additions serve the existing snapshot contract;
no exact XP restoration preserving that contract was identified. Merely deleting
snapshot functionality, moving the same state access elsewhere or replacing
stable IDs with process pointers is not a valid simplification. Retain R0415
for owner disposition, not automatic approval. No code/test/build/manifest or
EXE changes; XP-relative diff delta zero. P66 remains the published full-suite
392/392 baseline; this batch adds only the two focused test results.
Ordered frontier 320/472; next file 321 fla.c. S1 remains active.

P69 actual-source counter-review and documentation gate pass. Owned process
inventory confirms the runner exited. Clean-Build preview, exact task-scratch
removal and residue check complete; only audit/state documents are delivered.


## File 321: fla.c (P70 read-only qualification)

Pinned XP raw SHA-256:
f43c7274bc3e3a943bd26cbf5f7f721c6c36ff87276a4dd303c34d2053efd803.
The complete retained-code comparison finds only the historical notice,
snapshot include, one callback forward declaration and the snapshot extension.
Original FDC commands, interrupt scheduling and NDMA implementation are intact.
Existing excluded bodies and line-control scaffolding are skipped without new
accounting. Active additions total +99/-0; audit delta is zero.

| Point | Current location / XP anchor | Disposition |
| --- | --- | --- |
| R0416 | 46 / before XP 46 | Preserve historical notice under source policy. |
| R0417 | 78, 107, 224-319 / XP 80, 108, 233 insertion anchors | Existing FDC snapshot declaration, callback codec and private-state access; evidence-qualified, pending owner decision. |

Production proof: root CMake compiles fla.c. The Machine snapshot path reaches
compat/ccpu/archive.c then compat/devices/archive.c, which invokes FDC capture
and restore and serializes every field using fdc_fields. The command/result
blocks, SIS slots, status, command/result counters, NDMA buffer/count/sector size,
drive selection and interrupt-line state are the original controller state.
The archive is storage, not another controller. The buffer has 8192 bytes on
the selected original path; the copy bound is that actual capacity. Raw DOR is
captured/restored through its all member without reinterpreting bit order.
Restore validates lengths, selectors and finite flags before mutating state.
No independent interrupt, replay or device command is generated by restore.

The callback is also live: fla_atomicxqt schedules fla_int_call_back through
add_q_event_i after a successful interrupt-requiring command when the gate
permits it. The existing queue codec calls this file's encoder/decoder and
stores SOFTPC_DEVICE_QUEUE_FDC_INTERRUPT rather than a native code address.
The function is LOCAL and defined later (current 831); the added declaration
is needed where its address is used earlier. Removing only that declaration
would leave an undeclared identifier, not restore a coherent original snapshot
path. Moving the extension to a new mirror insertion point merely to save this
line is not admitted as a new difference. Removing all snapshot access and its
callers would remove supported controller preservation, not equivalent behavior.
No duplicate Compat implementation can replace this original state owner.

Dynamic evidence is the unchanged-source P69 checkpoint_smoke execution on both
architectures, not a new run: source and test trees are unchanged from 19d81f0f.
The FDC section writes DOR and SPECIFY bytes through actual ports, captures,
resets DOR, restores and verifies DOR/current-command/command-count. Both full
checkpoint fixtures returned success. This proves those transitions only;
all NDMA payloads, queue callback timing and every malformed archived field
were not individually exercised. Necessity of remaining field and callback
access is source/caller proof, not a claim of exhaustive runtime coverage.

Counter-review finds no original device algorithm replaced by the extension,
no second device-state authority, and no safely removable retained difference
under the existing snapshot contract and no-new-diff constraint. R0417 remains
an owner decision. No production/test/build/manifest/EXE change or new full
regression; published P66 and its 392/392 result remain the last full baseline.
Verified ordered frontier 321/472; next file 322 floppy.c. S1 remains active.


## Files 322-323: floppy.c and floppy_i.c (P71 read-only qualification)

Fixed XP raw SHA-256:

- floppy.c: 0d477d6dcea01fb9af2976729fc14b56d02ee3f34d37f38075bbd6b1bc023815.
- floppy_i.c: 9feb3e43eefeefa09f803897c34be6a975deeef96296fb5437d60bc5e49427f1.

Complete per-file original/current text comparison and contextual review find
no retained-code differences outside already excluded bodies and their existing
conditional/line-control scaffolding. Diff alignment fragments were checked
against their enclosing original and current conditions, rather than mistaken
for additional selected-code deletions. Those exclusions are skipped without
new accounting, point IDs or retention decisions, as required by the owner.
This is retained-subset equivalence, not whole-file byte identity with XP.

Both files are production inputs explicitly listed in root CMake. They retain
the original floppy BIOS implementation and interrupt entry. Their removal
would remove production code, and no removal or alternative Compat path is
proposed. No source restoration is needed in this pair.

Counter-review: neither an original-body exclusion nor its line-control boundary
is used to claim a new restoration or diff reduction. Production, test, build,
manifest and EXE changes are all zero; mirror diff delta zero. This is a read-only
source comparison, with no new compilation or dynamic tests. P66 remains the
published executable/full 392/392 regression baseline; P69 remains the latest
focused dual-width checkpoint result. No broader guest assertion is made.
Verified frozen frontier advances to 323/472; next file 324 gfi.c. S1 and the
previous evidence-qualified owner decisions remain open.


## Files 324-325: gfi.c and gfi_mpty.c (P72 read-only qualification)

Pinned XP raw SHA-256:

- gfi.c: 88321cdfcc0bf9063814bd6a01d95dec9205ec382f2e3f3cd33fb76915740e25.
- gfi_mpty.c: ed31f747a67892278b2f9a27ef63361617b8deaffa4d51459f71983eb554d125.

Full-file retained-text comparison finds only one relocated original historical
notice per file (current gfi.c line 29, gfi_mpty.c line 23). R0418 and R0419
preserve those notices under source policy. Each is +1/-0 outside already
skipped exclusions/scaffolding; this audit changes neither. No excluded-body
accounting or new runtime-difference decision is introduced.

Both files are root-CMake production sources. gfi_fdc_command routes commands
through the original per-drive table; SPECIFY reaches each drive. gfi_init
installs gfi_empty_active for the drive slots. gfi_mpty.c supplies that original
empty server and populates command, drive/motor, density, type and change
callbacks. Compat gfi_image.c also invokes gfi_empty_active on its media detach
path. This is a live no-medium/default implementation, not an unused stub;
it does not imply that the product lacks floppy hardware. Host image I/O and
the empty backend implement distinct medium states, not duplicate controllers.
Original behavior and registration are retained without extra Compat changes.

Counter-review checks the only additions against the original notice text and
source policy. No remaining functional expression or controller algorithm
diff exists outside skipped regions; no restoration is needed. Source/test/
build/manifest/EXE delta and XP-diff delta are zero. No new build or test is
claimed for this read-only comparison. P66 remains the published 392/392 full
regression baseline. Frozen frontier 325/472; next file 326 base/inc/base_def.h.
S1 and earlier evidence-qualified residual decisions remain open.


## Files 326-329: original base headers (P73 read-only qualification)

Fixed XP raw SHA-256:

| File | SHA-256 | Whole-file disposition |
| --- | --- | --- |
| base_def.h | a862bc34af1b24820530e2bf4e5651f4ad81eccb86856a869316f6691e7075d0 | Retained content equals XP outside existing exclusions/scaffolding. |
| bios.h | 333eecd7ab1e4a48da56894256670a0a256eb0393df963fe96454c26d55a2a0f | Retained content equals XP outside existing exclusions/scaffolding. |
| build_id.h | 6b140e5550335468ba615ea7f911da2b425bc2c7a1b467ea9260c5920ca8e970 | Entire normalized text equals XP. |
| cga.h | 55eb38e415d7582b80a8bb52ff0dc935fdc7b232bbcd2bce5e768287ba318cb0 | Retained content equals XP outside existing exclusions/scaffolding. |

Each file was compared in frozen order across its full original/current text.
Apparent bios.h deletion fragments belong to existing excluded branches and
were checked in context; no selected declaration restoration is missing.
Existing excluded bodies and boundaries receive no new point IDs or accounting.
No new residual decision exists in this group.

Actual consumers remain: host/inc/insignia.h includes base_def.h; original BIOS
and Compat device_bop/gfi_image users include bios.h; base/bios/build_id.c uses
build_id.h; original mouse.c and video/cga.c include cga.h. These are retained
original declaration/constants dependencies, not dead test-only files.
The original build_id.h date fields encode the legacy BIOS build ID, not the
standalone product's fresh compiler timestamp. No version rewrite is warranted.

Counter-review finds no non-excluded local type, macro, declaration or formatting
adaptation to restore. Preserve mirror CRLF; normalized equality is not a claim
of identical raw LF/CRLF bytes. No source/test/build/manifest/EXE change, no
XP-relative diff reduction, no new compilation or dynamic test result. P66 is
still the published 392/392 full baseline. Frozen frontier advances to 329/472;
next file 330 base/inc/ckmalloc.h. S1 and pending decisions remain open.


## File 330: ckmalloc.h (P74 qualification in progress)

Pinned XP raw SHA-256:
9f05409d3a7414620614b1b4c0f050b0ec2a81dfb792a0f3ced264a684804f3f.
The entire header differs only by a three-line rationale and stdlib.h include.
R0420 restores exact XP text, retaining repository-required CRLF. Production
+0/-4; XP-relative +4/-0 becomes zero, eliminating all four diff lines and the
one local adaptation. Manifest local identity is updated; historical audit
records remain historical, while the current divergence row records retirement.

All include consumers were inspected: nt_graph.c, com.c, rom.c, cga.c,
ccpusas4.c (two includes) and fpu.c include host_def.h earlier. That header
already includes stdlib.h before defining host_malloc/host_calloc. rom.c also
includes malloc.h and ccpusas4.c directly includes stdlib.h. Removing the
ckmalloc include therefore does not remove allocator declarations at any real
consumer. The comment's historical MSVC justification does not establish a
current necessary difference. No host header change or new dependency is added.
Future host_def.h auditing must recheck coherent declarations if its own include
is considered for restoration; this file's removal does not predetermine that
later decision or authorize an implicit-int allocator call.

Executor review confirms exact XP equality after newline normalization and a
four-line deletion only. No helper, macro rewrite, allocation behavior, guest,
configuration, snapshot or permanent test changes. Cached dual-width builds,
publication and full regressions are running on this final source. The verified
frontier stays 329/472 until delivery; no passing status is inferred from launch.


### P74 final qualification

Final-source cached builds and publication succeed: x86 9.06 seconds, x64
8.96 seconds. Each architecture passes 188 background and eight native tests
first run: 392/392 total. Background times are 139.25/141.98 seconds; native
times are 24.95/24.65 seconds. Full regression includes component manifests,
source/dependency gates, snapshot tests and native package/audio/desktop paths.
No allocator implicit-declaration error appears in either build log.

Published softpc32.exe: Nov 13 2026 14:07:35; SHA-256
9b6250038470c07fc17c075b46fabec5538f7c55fae4f2c330123b331236e36d.
Published softpc64.exe: Nov 13 2026 14:10:28; SHA-256
97c87c5dca86d180cfac38eececd48b3f475ffeb960ca40165cf243c4970eec8.

Coordinator counter-review of the actual diff confirms the production change
is exactly four deleted lines and the final normalized header equals fixed XP.
The single header adaptation is retired in the current divergence record;
manifest XP/raw/subset fields are preserved and local hash now equals XP.
No test or other production source change, no asset/configuration/media/snapshot
change except the two required published EXEs. No new retained decision.
Verified frontier advances to 330/472; next file 331 base/inc/cmos.h. S1 and
previous evidence-qualified decisions remain open; this is not task closure.

P74 process inventory confirms all owned jobs exited. Clean-Build preview,
exact disposable scratch removal and residue check pass; reusable caches stay.
Final documentation gate and actual diff review pass before commit.


## Files 331-334: CMOS, configuration and CPU headers (P75 read-only)

| File | Fixed XP raw SHA-256 | Disposition |
| --- | --- | --- |
| cmos.h | ed45d50c623bbb896bf3c9f00b494663730f1b4829514d75657bfcc2ba6fce35 | Retained text equals XP outside existing exclusions/scaffolding. |
| cmosbios.h | d2f031fec3938ebda4c42c2a372579b3448383ee76af71a1e20ee563af5cd3f4 | Entire normalized text equals XP. |
| config.h | 24f83045f8fd71e60da8a83f4d670794d8306090d56c21c4d2637166ff7f3196 | Retained text equals XP outside existing exclusions/scaffolding. |
| cpu4.h | 556863f1abb245ad9850a22e3ab9c9732f0ebb29fb5c3179faf32f1b873bec18 | Retained text equals XP outside existing exclusions/scaffolding. |

All four complete files were compared in frozen sequence. config.h's apparent
standalone deletion is inside its existing NTVDM exclusion, confirmed against
both surrounding originals and local directives. No excluded-body accounting
or new decision point is introduced. There is no non-excluded local change to
restore in these files.

Production consumers include original system/cmos.c (cmos.h, cmosbios.h and
config.h), diskbios.c (cmos.h/config.h), Compat platform.c (config.h), and
host_def.h's selected CpuH mapping to cpu4.h. These are live original interfaces.
No duplicate replacement or file deletion is justified. Existing original
constants, types, declarations, register mappings and notices remain unchanged.

Counter-review confirms source-only equivalence, not a new dynamic device proof.
No source/test/build/manifest/EXE changes, no new build/test execution, and zero
XP-diff delta. P74 remains the freshly published 392/392 full regression baseline.
Ordered frontier 334/472; file 335 cpu_vid.h has real retained ABI/declaration
differences and is not counted complete. It requires separate coherent caller
and provider verification. S1 and previous residual decisions remain open.


## File 335: cpu_vid.h (P76 qualification in progress)

Pinned XP raw SHA-256:
5df752f3e978f2e9b3bf0df4acd843148584b711b438933dca4d0d5f77ce542f.
The retained differences are writer IPT arguments/comment, reader IPT arguments/
comment, and nine explicit provider declarations with their reason comment.
Existing excluded bodies/scaffolding remain untouched and receive no new ledger.

R0421 restores exact XP arguments in EVID_WRT_POINTERS b_write/w_write and
removes the one-line T60 explanation. R0422 restores READ_POINTERS b_read,
w_read and str_read exact XP arguments and removes the three-line ABI rationale.
Selected insignia.h defines IPT1/IPT2/IPT3 as (), so these changed parameter
spellings do not become typed prototypes under the actual supported builds.
The restored argument spelling is not a claim that a future ANSI configuration
has correct typed parameters; that configuration is currently excluded.

Source-to-compiler proof uses actual Ninja compiler/options for ev_glue.c,
ega_read.c, ega_writ.c and ccpusas4.c. Private current/candidate include roots
supply cpu_vid.h without changing other headers or production files during the
probe. On x86 and x64, all eight current/candidate preprocessor output pairs
(-E -P) are byte-identical; every preprocessing command exits zero. Thus this
restoration preserves actual selected declarations and implementation tokens.

R0423 retains the nine explicit glue declarations as an evidence-qualified
owner decision. Actual ega_read.c Glue_reads and ega_writ.c Glue_writes tables
use these function identifiers; ev_glue.c provides them. Removing the whole
added declaration block in a separate candidate causes syntax compilation to
fail on both architectures: the three read and six write/fill/move identifiers
are undeclared in the actual table initializers. All four negative compiles
exit 1. This is concrete compilation proof, not a claimed guest crash.
The original unprototyped IPT table fields do not themselves declare provider
function names; retaining those fields and removing providers is not equivalent.

Counter-review of the candidate confirms five restored original lines and four
removed comment lines: production +5/-9. The five restored replacements remove
ten XP added/deleted lines; four deleted added comments remove four more,
reducing anchored XP differences by 14 lines. No runtime function, public API,
state, dispatch path, original guest or permanent test changes. Existing skipped
conditional scaffolding remains unchanged. Candidate normalized SHA-256:
690cb9668f24f45405734f67d517e5ecda4bf0cdce0f71be8c1fce4daa234a38.
Full cached builds, dual publication and regression are running on the applied
candidate. Verified frontier remains 334/472 until this delivery completes.


### P76 final qualification

Both cached final-source builds and EXE publication pass (x86 14.54 seconds,
x64 21.91 seconds). Each architecture passes 188 background and eight native
tests first run: 392/392 total. Background times 141.65/136.24 seconds; native
24.90/24.85 seconds. This is full repository regression, not a new claim that
legacy bulk video-source callbacks are production-supported (P61 scope remains).

Published softpc32.exe: Nov 13 2026 14:17:59; SHA-256
92336d1f4193700b832f5919e783179225d12e3f5eecaf228291b98d9e348595.
Published softpc64.exe: Nov 13 2026 14:21:00; SHA-256
8f924c8c9fd7c40b1f417d4ee7f72b302c64849dd2d167ff203095a33c23ec55.

Final header matches the preprocessor-tested candidate after LF normalization,
with CRLF preserved in the mirror. Actual diff review confirms only the five
original IPT lines and four removed comment lines: production +5/-9, permanent
tests zero, anchored XP added/deleted differences reduced by 14. No extra source
or behavior change. Manifest local hash and current divergence explanation are
updated; historical metrics are not silently presented as current XP counts.
R0423 remains a qualified owner decision, not approved retention.
Verified frontier advances to 335/472; next file 336 base/inc/debug.h.
S1 remains active; no completed-file delivery implies whole-audit closure.

P76 authoritative process inspection confirms all owned jobs exited. Clean-Build
preview, exact scratch removal and residue check pass. Final documentation gate
and actual source/manifest/artifact review pass before commit.


## Files 336-337: debug.h and dma.h (P77 qualification)

Pinned XP raw SHA-256:

- debug.h: 388f486956e939fd24e9f2b4ba37f71dc8b7506ece928fb878bb68c30e4db618.
- dma.h: 88f5c9d2dbfdb620fcd6ae34346298e0ce31da9132e32edd480648138764660c.

Complete debug.h retained text equals XP outside existing exclusions/scaffolding.
It remains used by the original controller code (including at_dma.c). No new
difference point or restoration is needed; this is not whole-file raw equality.

R0424: dma.h adds the dmaGetAdaptor pointer-return declaration and its one-line
T93 reason after the existing excluded declaration branch, plus a separating
blank. Non-scaffolding addition +3/-0 is unchanged. Existing excluded bodies
and diagnostic line controls are not separately counted or reopened.

Actual production use: nt_sbdsp.c includes dma.h and SbInitializeDMA calls
dmaGetAdaptor, then indexes the returned controller array for the selected DMA
channel. The retained at_dma.c accessor returns the address of its original
adaptor state. This is access to the existing device owner, not a second DMA
model in Compat. XP exposes the declaration only under NTVDM, which is not the
selected build; the selected DSP integration requires its declaration here.
Removing the declaration and also deleting the DSP's controller access would
remove supported audio DMA behavior, not preserve the product by restoring XP.

Compiler proof uses the actual nt_sbdsp.c Ninja compile flags on x86 and x64,
with private current and restored-declaration-scope dma.h include roots.
-Werror=implicit-function-declaration is added explicitly to turn the observed
missing declaration into a decisive compile result; it is not represented as
the unchanged production warning policy. Current passes both (exit 0).
Removing the added declaration/reason, while preserving excluded scaffolding,
fails both (exit 1): implicit declaration of dmaGetAdaptor and assignment from
int to DMA_ADAPT *. Original diagnostic location is nt_sbdsp.c:2288. The pointer
return contract is required on both widths; no guest crash or observed runtime
pointer truncation is claimed from this compilation experiment.

Counter-review: existing original controller access avoids a separate DMA
implementation. Relocating the same declaration into another source adds a new
mirror difference rather than removing the necessary contract. Retain R0424
pending owner decision. No production/test/build/manifest/EXE change and no XP
diff reduction. No new full regression or audio session run; P76 remains the
published 392/392 full baseline. Frozen frontier 337/472; next file 338 dsktrace.h.
S1 and previous owner decisions remain open.


## Files 338-339: disk trace and EGA CPU header investigation (not delivered)

Pinned XP raw SHA-256:

- dsktrace.h: 587ad1ebae181ea51d1373f7c23f3580bf21d63bd8f9e7d141e72eecf2273d60.
- egacpu.h: 17b5cebb9c603a24f179d19df0966026877ea20e70bcc17a1cc2880a0f99bf56.

Retained dsktrace.h matches XP outside existing exclusions/scaffolding and is
included by diskbios.c/fdisk.c. No source change is required. It is not yet
advanced in the delivered frontier while the adjacent file is investigated.

The egacpu.h retained differences include a VGAGLOBALSETTINGS forward tag,
the selected globals pointer type and reason comment, and the evidgen include
path/location plus reason comment. Whole-hunk suppression when a hunk contains
#error would hide the selected pointer adaptation; contextual inspection of
the full file identified it explicitly. Excluded bodies themselves are skipped.

Current repository contains only base/cvidc/evidgen.h; the historical competing
CCPU header described by the include comment no longer exists. Private-header
experiments with the actual ev_glue.c and vga_prts.c build flags on x86/x64
show: current, exact original include spelling/location, and removal of the
forward tag each compile successfully (twelve successful syntax checks).
Restoring VGA_GLOBALS * instead produces an incompatible-pointer diagnostic
at ev_glue.c:766, where the original generated GLOBAL_VGAGlobals is assigned;
both widths exit 1 with explicit -Werror=incompatible-pointer-types. vga_prts.c
passes that variant because its only globals use is a null check. This is a
compiler/type proof, not a dynamic guest failure. The extra diagnostic flag
is disclosed and is not claimed as the unchanged production warning policy.

These separate experiments support combining include/tag reductions while
keeping the actual provider pointer type for owner disposition. No production
file has yet been changed, and combined final-source/full regression has not
run. Frozen delivered frontier remains 337/472. The active task owns temporary
probes until the complete-file delivery; no investigation is reported closed.


### P78 applied candidate and difference decisions

R0425 removes the redundant VGAGLOBALSETTINGS forward tag and separator;
the selected member declaration supplies the same struct tag, and both actual
consumers compile without the extra declaration on both widths.
R0426 retains the actual generated globals pointer type and local reason as an
evidence-qualified owner decision. The original incompatible VGA_GLOBALS pointer
would not describe GLOBAL_VGAGlobals; adding a cast or changing the original
provider to hide the mismatch is not an equivalent smaller single-owner fix.
R0427 restores the exact XP evidgen.h include spelling and original position
before C_VID, deleting the obsolete duplicate-header explanation. The current
repository has only the correct generated C-VID header, already on the selected
include path. No include directory or other production file is modified.
Existing #error bodies and scaffolding remain untouched and unaccounted.

Combined production change is +2/-5 (net -3). XP added/deleted difference
reduction is seven lines: removed forward tag/separator plus restoration of
the original include/separator and removal of its replacement/comment. The
excluded-body contribution is unchanged and is not introduced as a new metric.
Final normalized candidate SHA-256:
c566977f387b59d68fcf7c0d144ec814cf824bcdeea72a3ed43ef57142b7757a.
Manifest local hash is updated; baseline hashes remain fixed. Full cached
build/publication/regression is running on the combined final candidate.
Frozen delivered frontier remains 337/472 until verified delivery.


P78 first combined x86 build failed at the two softpc-vga-frame-smoke sources:
its private include path omitted base/cvidc, so the restored original evidgen.h
include was not found. Both actual test consumers share this one target. Root
CMake now adds that existing dependency directory to the target (one added
build-configuration line); production include paths already contain it.
This is disclosed coupled test build wiring, not a new mirror fallback or
source implementation. The first failed build is not a passing qualification.
Full dual-width qualification restarts after this final configuration change.


### P78 final-source delivery qualification

After the disclosed initial test-target include failure was corrected, both
cached builds and publications complete successfully. Each architecture passes
188 background and eight native tests: 392/392 in the final qualification run.
Stage exit codes and elapsed seconds: [["x86-build", 0, 10.09], ["x86-background", 0, 135.47], ["x86-native", 0, 25.52], ["x64-build", 0, 26.12], ["x64-background", 0, 146.78], ["x64-native", 0, 24.89]].

- softpc32.exe: Nov 13 2026 14:30:51; SHA-256 97888789d3eeeb4affea72732635ffd25bb3f15b4b4f5740a3942cdb1838a316.
- softpc64.exe: Nov 13 2026 14:33:42; SHA-256 b1fb577bf87bd92e910b80fa9c7d45f6b67905c7ba7f6e3ca1e47f828b5ff279.

Counter-review confirms exact XP include spelling/location and removal only of
the redundant struct tag. The current generated carrier type remains R0426,
pending owner decision; no cast, new initializer, fallback or device logic is
introduced. The only coupled root build change gives the existing VGA fixture
its actual header dependency. Production +2/-5, net -3; permanent test source
+0/-0; build configuration +2/-1, net +1. Anchored XP diff decreases seven lines.
Existing exclusions are unchanged and not counted. Final mirror bytes match the
focused final candidate, and manifest local identity matches. No guest/media/INI
or shared-component changes. Full regression does not claim exhaustive guest
video modes; P61 explains the separate unsupported legacy bulk callback path.

Frozen delivered frontier advances to 339/472. Next is file 340 base/inc/egagraph.h.
S1 and qualified residual owner decisions remain open.

P78 final actual-diff and documentation-governance reviews pass. Authoritative
process inventory confirms the qualification worker exited. Task-scratch cleanup
preview, exact removal and residue check pass; reusable caches are preserved.


## P79: files 340-341 retained-content comparison

Fixed XP raw SHA-256:

- base/inc/egagraph.h: 376143cae84f8d1965d28e4d82273539fe2a541cf80cf97927eb7223efb60f12.
- base/inc/egamode.h: 8c3ac82156131dbec6b892bda14ba2fcae4e883e18777f124960181e429eca45.

The full newline-normalized comparisons show no retained-content differences
outside existing exclusions and their surrounding scaffolding. No whole-hunk
suppression is used to infer equivalence. egagraph.h retains its display-state,
palette/font/underline, split/panning state macros, enumeration and update
contracts. egamode.h retains the display selector and EGA/VGA declarations.
Existing #error bodies/scaffolding are skipped without additional accounting.
Mirror CRLF is preserved; raw upstream hashes are not newline-normalized hashes.

Actual production includes remain: ega_mode.c/ega_prts.c/vga_mode.c/vga_prts.c
and Compat video.c/platform.c consume these declarations; original nt_graph.c
also includes both. These are not test-only unused files. No duplicate behavior
implementation exists in these declaration-only headers to relocate or delete.
Counter-review finds no selected declaration or macro-body edit concealed by
an adjacent excluded branch. Preserve both files exactly as currently retained.

No production, permanent test, build, manifest or EXE changes; no XP diff
reduction is claimed. No fresh runtime/full-suite run: P78's 392/392 published
baseline remains unchanged. Frozen delivered frontier is 341/472, next file
342 base/inc/egaports.h. S1 and existing evidence-qualified owner decisions remain open.

### Next-file investigation, not a delivered disposition

The non-excluded egaports.h addition is the ega_get_pel_panning declaration
and its T60 comment. Actual calls occur in nt_ega.c painting and gfx_updt.c
neighbour marking; vga_prts.c owns the implementation, reading attribute
panning and split cancellation. Those consumers alone do not prove the added
behavior necessary. The next complete-file investigation must examine coherent
restoration and actual panning evidence before qualifying a residual. File 342
remains unreviewed in the delivered ledger; no production edit has been made.

P79 documentation gate and actual three-file diff review pass. No background
job was started; task-owned comparison scratch was previewed and removed,
and the build residue check passes. No new approval or closure is inferred.


## P80: file 342 egaports.h investigation and candidate

The only retained addition outside existing excluded scaffolding is the pel
panning query declaration and its historical explanation. Actual production
nt_ega.c painters and gfx_updt.c call it; vga_prts.c owns the attribute-controller
query. The query derives the scanline's shift from the existing register and
split-screen cancellation, without owning a second state. Root CMake compiles
all three implementation files in the original-device object target.

The real existing VGA fixture passes unchanged on x86/x64. A disposable copy
of vga_prts.c changes only the query result to zero, is compiled using the
actual Ninja command, and replaces its member in a private copy of the machine
archive. It links to the same fixture/cached other objects. Both copies fail
at vga_frame_smoke.c:501: after actual 3C0h writes set pan=1, visible pixels
must equal unpanned pixels 1..8. x86 exits 3221226505 and x64 exits 3 following
the assertion; the original current fixture exits 0 on each. Windows crash
reporting completed; no observation timeout is counted as a failed assertion.
This is an isolated no-panning behavior counterexample, not a claim to have
restored every original T60 source change or reproduced a real guest session.

The passing existing fixture also checks three EGA painters, shifts 0..7,
bank/wrap variants and split cancellation against unpanned pixel sequences.
Those scopes do not establish every video mode. The actual port-write test,
provider register read and actual output assertions establish a consumed
panning capability, rather than merely the existence of adapter callers.

Private-header removal of the added declaration yields the specific implicit
query-declaration diagnostic in nt_ega.c and gfx_updt.c on both widths, with
explicit extra -Werror=implicit-function-declaration. Current-header nt_ega.c
passes that same check. gfx_updt.c also has an unrelated setMarkPointers
implicit declaration under the extra flag with BOTH headers, so its exit code
alone is not attributed to this query. The normal build policy is unchanged.

R0428: retain the single query declaration as an evidence-qualified owner
decision. Moving it into another mirror file does not reduce the declaration
surface and adds a new point. Removing the real panning effect breaks the
observable pixel contract. Counter-review distinguishes declaration necessity
from exhaustive proof of every existing panning patch; implementation files
retain their later ordered reviews.
R0429: remove the unnecessary one-line T60 explanation, restoring XP absence.
Production +0/-1, net -1; XP added/deleted diff decreases one line. No test
source or build configuration change. Existing exclusions/scaffolding remain
untouched and unaccounted. Final normalized SHA-256: b1a4bc2d7f1c8311fd3d8983ed83d6d1ab9caedccbb305c2ce2ffeb4f4e263b3.

Candidate applied; full final-source qualification is pending. Delivered
frontier remains 341/472 until P80 delivery. No residual is owner-approved.


### File 343 egaread.h retained-content review

Pinned XP raw SHA-256:
e84f967733ed9e18df9e372a4a2bbf942d4984a685cf12b3410eca07ba973097.
Complete normalized comparison shows only existing excluded-body scaffolding;
all retained read declarations and READ_STATE fields match XP. Actual consumers
include ega_read.c, ega_prts.c, vga_prts.c, ega_vide.c, vga_vide.c and v7_ports.c.
This is a live declaration owner, not test-only behavior. No restoration or
new residual point is required. Existing exclusions are skipped without ledger
entries. Delivery of files 342-343 remains pending P80 final qualification.


### P80 final-source qualification

Both cached builds and publications pass; x86 and x64 each pass 188 background
and eight native tests, 392/392 first final-source run. Stage results:
[["x86-build", 0, 18.47], ["x86-background", 0, 138.05], ["x86-native", 0, 24.83], ["x64-build", 0, 22.88], ["x64-background", 0, 139.33], ["x64-native", 0, 24.46]].

- softpc32.exe: Nov 13 2026 14:44:14; SHA-256 01d81afaf70f82898d4d5fb7525c64f764ae3e2809de967082524c4b1aa60f6f.
- softpc64.exe: Nov 13 2026 14:47:15; SHA-256 a517c31bf8e6e8881793b11b35ec81f799f29808d11da751001db001cbe4e40d.

Actual-change review confirms production +0/-1, test source +0/-0, build
configuration +0/-0; XP-relative diff decreases one line. No device algorithm,
register state, callback, shared component, media or INI is changed. The
manifest matches final tested source. R0428 remains a qualified owner decision.
Frozen delivered frontier 343/472; next file 344 base/inc/egavideo.h.
S1 remains active; neither that next file nor existing owner decisions is closed.

P80 documentation gate and actual eight-file diff review pass. Authoritative
process inventory finds no live task-owned probes/builds. Scratch cleanup
preview, exact removal and residue check pass; reusable caches remain.


## P81: file 344 egavideo.h investigation and restoration candidate

Pinned XP raw SHA-256:
7c3428b6b211f9682749de07d41c699e3c1f8d13c13aff59b8b84195dd0c639b.
The only retained addition is the ega_read_dot declaration, its two-line
explanation and separator. Actual symbol search finds the function definition
in ega_vide.c and its single call in video.c. The latter already declares
extern void ega_read_dot(int, int, int) immediately before vd_read_dot; that
precise declaration is original XP video.c:2637. No caller needs the new header
copy. Existing exclusions/scaffolding are unchanged and not newly accounted.

R0430 restores original absence of all four added header lines. No declaration
is moved and no Compat replacement is introduced. Every actual direct consumer
was compiled with current/restored headers and otherwise identical actual Ninja
flags on both widths: Compat platform.c/video.c/v7_pointer.c and original
ega_vide.c/video.c/v7_video.c/vga_vide.c. All 28 compile operations succeed and
all fourteen object pairs are byte-identical. Tests do not directly include
this header; the header is not transitively included by another active header.
The mouse_io.h occurrence is an original explanatory comment, not an include.

Object-pair identities (architecture, source, current, restored, equal):
- ["x86", "src/app-softpc/compat/platform.c", "edc02e2aa8319d1c940696091a36a18fab85f4902eac37a323f015ed8185f21a", "edc02e2aa8319d1c940696091a36a18fab85f4902eac37a323f015ed8185f21a", true]
- ["x86", "src/app-softpc/compat/video.c", "ba29eab142031287a0692a45d8b3f98f44485fad245ba4591190ef7efc7f48a2", "ba29eab142031287a0692a45d8b3f98f44485fad245ba4591190ef7efc7f48a2", true]
- ["x86", "src/app-softpc/compat/v7_pointer.c", "90663464c9146e02b7fe61ca40e8e0b2145f7147ca5e713b7c98d62a9bb5cf4a", "90663464c9146e02b7fe61ca40e8e0b2145f7147ca5e713b7c98d62a9bb5cf4a", true]
- ["x86", "src/app-softpc/softpc.new/base/video/ega_vide.c", "e461cfb47deb229fdb15bd8a6e298a34c16cdf1ff16951df6df08a9a9917838d", "e461cfb47deb229fdb15bd8a6e298a34c16cdf1ff16951df6df08a9a9917838d", true]
- ["x86", "src/app-softpc/softpc.new/base/video/video.c", "82d6cf9bd9bc50471f3d793f37867948d7d94eeeabcaf0ca1df1b7e83fea056f", "82d6cf9bd9bc50471f3d793f37867948d7d94eeeabcaf0ca1df1b7e83fea056f", true]
- ["x86", "src/app-softpc/softpc.new/base/video/v7_video.c", "125da31ca13711e174fe7c4b3708c0279cc3ccdfa6d5d5e8f71b19c2d43f964e", "125da31ca13711e174fe7c4b3708c0279cc3ccdfa6d5d5e8f71b19c2d43f964e", true]
- ["x86", "src/app-softpc/softpc.new/base/video/vga_vide.c", "7f70de8c0dfad16b3bf756c467ee8c9326d6993bb838e414628c435df6118da6", "7f70de8c0dfad16b3bf756c467ee8c9326d6993bb838e414628c435df6118da6", true]
- ["x64", "src/app-softpc/compat/platform.c", "3809881f84e2217e14bb955b0e139622414b909ffefc462d2815ed47e1065cc8", "3809881f84e2217e14bb955b0e139622414b909ffefc462d2815ed47e1065cc8", true]
- ["x64", "src/app-softpc/compat/video.c", "eed76e86a47a2653a1adb1cf36dfc788464d69be6ed67c3f5074caeef2866d11", "eed76e86a47a2653a1adb1cf36dfc788464d69be6ed67c3f5074caeef2866d11", true]
- ["x64", "src/app-softpc/compat/v7_pointer.c", "aa927ba35f1cc8540e9811cdcd2d7d4667fe33c2dc6d9d616b31635d02524295", "aa927ba35f1cc8540e9811cdcd2d7d4667fe33c2dc6d9d616b31635d02524295", true]
- ["x64", "src/app-softpc/softpc.new/base/video/ega_vide.c", "ed9e568095052b0e1bec41ad8c016940ca95d6ddda3d7e4ddcc96b3f3b835601", "ed9e568095052b0e1bec41ad8c016940ca95d6ddda3d7e4ddcc96b3f3b835601", true]
- ["x64", "src/app-softpc/softpc.new/base/video/video.c", "0ecfcdb33270187c0704833880a820ba32dbf571e06bc6a2b256c7f45ab8c9dc", "0ecfcdb33270187c0704833880a820ba32dbf571e06bc6a2b256c7f45ab8c9dc", true]
- ["x64", "src/app-softpc/softpc.new/base/video/v7_video.c", "eab6297ec326f31f0742e57e256e479b89866f3f343fbcfe92d42d75d1b929ee", "eab6297ec326f31f0742e57e256e479b89866f3f343fbcfe92d42d75d1b929ee", true]
- ["x64", "src/app-softpc/softpc.new/base/video/vga_vide.c", "b8a949fc5c37e994964d9126fa3c140fd00542beecfd8d0eff6931fe018eb783", "b8a949fc5c37e994964d9126fa3c140fd00542beecfd8d0eff6931fe018eb783", true]

Counter-review confirms this is a duplicate declaration, not a change to the
BIOS dot service, its argument passing or provider. Production +0/-4, net -4;
XP added/deleted difference decreases four lines. Final normalized SHA-256:
6377e7acb5dfdf78e5f563f287fd4ebd1ac4d0414dff91b22edcdf25effb12a4. No permanent test/build change.

Candidate applied; cached dual-width build/publication and executable
comparison are pending. Full-suite omission requires actual artifact-equivalence
proof apart from required timestamp fields. Delivered frontier remains 343/472.


### P81 final-source and artifact-equivalence qualification

Cached build/publication results: [["x86", 0, 12.26], ["x64", 0, 14.59]].

- softpc32.exe: 3463049 bytes; 8 changed bytes, zero unexpected differences. Old stamp 14:44:14 Nov 13 2026; new stamp 14:53:37 Nov 13 2026; final SHA-256 f1f8b91be48ae9d59dfd2fea2f8a1d9360ae96e7d809b50e93aade927a1f2a0a.
- softpc64.exe: 3082345 bytes; 8 changed bytes, zero unexpected differences. Old stamp 14:47:15 Nov 13 2026; new stamp 14:53:49 Nov 13 2026; final SHA-256 1e5cf3e4f6028b62492481a4fadd62b5a4555ceab4a8e5d34c27857e9c04a5c5.

Comparison checks equal file length and PE header location, then every byte.
Only COFF TimeDateStamp (PE+8, four bytes), OptionalHeader CheckSum (+64, four
bytes) and the eight-byte compiler time at the matching unique date/time literal
are permitted to differ. Every byte outside those precisely identified fields
is identical. The date is unchanged (Nov 13 2026); fresh times are verified.
This is actual artifact equality apart from required build metadata, not merely
a source-equivalence claim. Per the owner's equivalence exception no full suite
is rerun; P80's 392/392 is the runtime baseline, not a new P81 test count.

Final mirror matches the restored header used in the object comparisons.
Production +0/-4, permanent tests and build configuration +0/-0; XP-relative
diff decreases four lines. No guest/media/INI/shared-component change. Complete
selected-content comparison leaves no non-excluded local difference in this
header. R0430 is restored, with no new owner-retention decision. Counter-review
confirms the original function definition and caller-local prototype remain.
Frozen delivered frontier 344/472; next file 345 base/inc/emm.h. S1 remains active.

P81 mirror-manifest and documentation gates pass. Actual eight-file diff review
passes. Both owned workers exited; scratch preview, removal and residue check
pass. Reusable caches are preserved, with no unrelated worktree changes.


## P82: files 345-348 ordered review and restoration candidate

Fixed XP raw identities:
- base/inc/emm.h: eb018c490a67e91b62af865f0ff6f75a1d4af9a08cd33186e8b74867ee0b7e8a.
- base/inc/equip.h: 8121528b389fa0ce6653bda8ccb6a042282d6c018cd7badf8bc72deb1b0c364e.
- base/inc/error.h: a262dcddcb4059e50a24c7795e09f493c9ba6e576ca0e7d904c3e60e6da08e51.
- base/inc/fdisk.h: 7bb7f6bcad81fe7bc980e61689cb29d069abc0eeea1337a0e81b0c918c69a044.

R0431 concerns emm.h's three host-storage declarations changed from long to
IHP, with a three-line native-pointer rationale. Whole source/test search finds
no implementation or caller for host_allocate_storage, host_free_storage or
host_reallocate_storage. Both actual machine archives also contain no defined
or unresolved symbol with these names. The only active direct header consumer
is original reset.c. Keeping a historical pointer adaptation for nonexistent
consumers is not justified by generic x64 concerns.

Restore the exact three XP declarations and remove the added explanation.
Actual reset.c compiled with current/restored headers under identical Ninja
flags yields identical objects on both widths (four successful compilations):
- ["x86", "2c63ec66bb03e09f86987d33eebb900b91ff516159e224da75ff91596f85dd1d", "2c63ec66bb03e09f86987d33eebb900b91ff516159e224da75ff91596f85dd1d", true]
- ["x64", "ab53371ca25a897711f81e1efb7b310f90e05a6e94d5e07ce0f52a0e80a97788", "ab53371ca25a897711f81e1efb7b310f90e05a6e94d5e07ce0f52a0e80a97788", true]

Counter-review: no active allocator or ABI is changed. The original header and
include remain; deleting an unused original declaration/file would increase
mirror differences and is not proposed. No Compat implementation substitutes
for these functions. Existing #error exclusions are skipped without accounting.
Production +3/-6, net -3; XP added/deleted diff decreases nine lines.
Final normalized emm.h SHA-256: 52bb00c12434a96968c185329dc88f0cf5a963dd3666fcb2c98c9fad1f83bedf.

Complete retained-content comparisons for equip.h and error.h show no changes
outside existing excluded scaffolding. Actual production consumers include
reset/equipmnt/floppy/ega_vide for equipment and diskbios/cmos/video code for
error contracts. fdisk.h matches XP after newline normalization and is consumed
by diskbios/fdisk/fla/reset. These live declarations do not duplicate Compat
behavior. No modifications or new residual decisions are required for 346-348.

Final-source cached build/publication and EXE comparison are pending. The
owner's no-repeat-test exception applies only after actual artifact-equivalence
verification; delivered frontier remains 344/472 until P82 delivery.


### P82 final-source delivery qualification

Cached build/publication results: [["x86", 0, 8.02], ["x64", 0, 9.51]].

- softpc32.exe: 3463049 bytes, 7 differing bytes. Prior stamp 14:53:37 Nov 13 2026; current 14:57:16 Nov 13 2026; SHA-256 acb67db7b7d3c97e22719922718baa10e8dabe6a91eab6e5144397a914f7b35a.
- softpc64.exe: 3082345 bytes, 6 differing bytes. Prior stamp 14:53:49 Nov 13 2026; current 14:57:24 Nov 13 2026; SHA-256 3c6c252918e3c8bf32021f5cd4d47bc29de219e980252f901b0a62c7b90b2164.

Both complete EXEs compare identical outside the explicitly located COFF
TimeDateStamp, PE optional-header CheckSum and eight-byte compiler-time literal.
Lengths, PE positions and unique date/time-literal positions match. No other
byte differs. The owner-approved equivalence exception therefore omits a repeat
full runtime suite; P80's 392/392 remains the runtime baseline, not a P82 count.
Both final mirrored header bytes and declaration scope match the tested variant.

Actual-change review: only emm.h changes in production, +3/-6 (net -3), while
XP-relative added/deleted difference decreases nine lines. Test/build source
+0/-0. R0431 is restored rather than retained for approval. Other reviewed
headers have no retained-content difference. No guest, configuration, media,
shared component or device behavior changes. Delivered frontier 348/472;
next file 349 base/inc/fla.h. S1 and prior owner decisions remain open.

P82 manifest/documentation gates and actual eight-file diff review pass. Both
owned workers exited; scratch preview, exact removal and residue check pass.
Reusable caches and unrelated assets remain unchanged.


## P83: files 349-354 retained-content audit

Pinned XP raw identities:
- base/inc/fla.h: 2343c83bb8ff48b88b30bb9ed003bf4f922546971bd178b378a389b3abd34b80.
- base/inc/floppy.h: e12f73f715763b9a860e9e2b6128b8e08ab930aacb8917bfd7951334c9f803ed.
- base/inc/ga_defs.h: e8c8d47ff6f355dfc2ca34055ee428718d67afb5951b25e23a5d7050d4cc9f10.
- base/inc/ga_mark.h: 9f510835e14e6d1293512b3e2cf8358270fb58f36cef08d87fbc5d3627f11489.
- base/inc/gendrvr.h: 013b5d77b7d28a921f52eecdd2d00ae687576d3419e007901050d37bd7b5e599.
- base/inc/gfi.h: 407735b7f27f596377711cd1976b60d252b83abe75ce956a1103c042e25320ea.

Complete newline-normalized comparisons: fla.h, ga_defs.h and ga_mark.h match
XP outright. floppy.h, gendrvr.h and gfi.h differ only at existing excluded-body
scaffolding. Each complete difference was inspected with retained context;
no whole-hunk suppression hides a neighboring selected declaration change.
Existing exclusions are skipped without new accounting or approval points.

Actual production wiring:

- fla.h: original reset/floppy/fla/gfi and Compat gfi_image consumers.
- floppy.h: original reset/floppy/floppy_i declarations and constants.
- ga_defs.h: original video/gfx_updt/ega_read/ega_prts includes.
- ga_mark.h: generated evidfunc/ev_glue and original EGA/update consumers;
  the mark-table structures and declarations remain original.
- gendrvr.h: original reset.c include remains, but the retained header has no
  active driver declarations under the selected excluded branches. It is not
  described as a functioning driver. Removing the file would require another
  mirror include deletion, prohibited by the owner's no-increase clarification.
- gfi.h: original controller/BIOS/CMOS and actual Compat image bindings.

Counter-review distinguishes historical comments (ga_defs names a tester) from
actual production includes. No duplicate Compat implementation is demonstrated
by these headers, and no new deletion/rewiring is justified. Preserve all six
files. No production/test/build/manifest/EXE change; no XP diff reduction or
fresh runtime suite is claimed. P82's published equivalent EXEs remain, with
P80's 392/392 runtime baseline. Frozen delivered frontier 354/472; next file
355 base/inc/gfitest.h. S1 and prior owner decisions remain open.

P83 documentation gate and actual three-file diff review pass. No background
worker was started. Comparison scratch preview, exact removal and build
residue check pass; no unrelated changes are present.


## P84: files 355-358 ordered review and restoration candidates

Fixed XP raw identities:
- base/inc/gfitest.h: f831f93a2ac5fab542c5300e41194662482fd9302fe5a69472ee477c08477c9d.
- base/inc/gfx_upd.h: c0faaed46835b8d769bed149cf4d4b60715f721ea9a8d14fe93d7fd1ab5298cc.
- base/inc/gispsvga.h: 2efdde6a1f36b5dcf088ed8690d9f15295b36d4d5189bb87948d89da1d5d85ed.
- base/inc/gmi.h: 01ec314cb52fc3051e7c6135be6262f1cc0791ef2fcb0af4c442e3a26b709c08.

gfitest.h matches XP after newline normalization; original gfi.c includes it.
Its historical tester name is not grounds to delete an original include.
gispsvga.h differs only at existing excluded scaffolding and remains included
by original rom.c; no active GISP implementation is claimed. No changes to
either header. All existing #error exclusions/scaffolding remain untouched
and are skipped without new accounting.

R0432 restores the one original STREAM_IO comment before the NEC_98 conditional
in gfx_upd.h. It is outside the excluded body; no macro continuation or token
is affected, and the existing line-control directive remains. Full comparison
finds no other retained-content change in that header.

R0433 restores the original MEM_HANDLERS member declarations in gmi.h's selected
body and removes the three-line historical explanation. Existing outer branch
scaffolding remains. Current supported !ANSI IPT0..IPT4 macros all expand to (),
so the added parameter vocabulary supplied no concrete prototype under either
actual architecture. Restoring the original declarations does not alter the
selected table's types, layout or call arguments. The original b_move also uses
IPT4 and therefore has the same expansion. No new pointer cast/provider/helper.

Actual Ninja-command preprocessing with -E -P compares current/restored headers
for ev_glue.c, ccpusas4.c, ega_writ.c and the VGA fixture on each architecture.
All sixteen operations pass; all eight complete preprocessed pairs match bytes:
- ["x86", "src/app-softpc/softpc.new/base/cvidc/ev_glue.c", "73d55916aa1289620aa441a763543e0aaa63d7bfe8efdaefd80b7e5d7753f075", "73d55916aa1289620aa441a763543e0aaa63d7bfe8efdaefd80b7e5d7753f075", true]
- ["x86", "src/app-softpc/softpc.new/base/ccpu386/ccpusas4.c", "709e1c18542cc538a781aaf76517c580536e719a8c4b9d20b742dfb73920b440", "709e1c18542cc538a781aaf76517c580536e719a8c4b9d20b742dfb73920b440", true]
- ["x86", "src/app-softpc/softpc.new/base/video/ega_writ.c", "10d72bb728465c90bc0a33a8e4cebb35cae0334f77ebed0e6435cc495f13dd1d", "10d72bb728465c90bc0a33a8e4cebb35cae0334f77ebed0e6435cc495f13dd1d", true]
- ["x86", "test/app-softpc/unit/machine/vga_frame_smoke.c", "141276a2e4e528377a4b0612f829b64fc3d85aba4c2868b9c0f6c52c675237d7", "141276a2e4e528377a4b0612f829b64fc3d85aba4c2868b9c0f6c52c675237d7", true]
- ["x64", "src/app-softpc/softpc.new/base/cvidc/ev_glue.c", "04dff869bc4fed10d1d8c677c94df3572b4481ca461ecb3958c81a2e0a2240fb", "04dff869bc4fed10d1d8c677c94df3572b4481ca461ecb3958c81a2e0a2240fb", true]
- ["x64", "src/app-softpc/softpc.new/base/ccpu386/ccpusas4.c", "1bb0f89c1bd669146a4e7fe1c04916618d48c77b46cae3397993099af92ebcb9", "1bb0f89c1bd669146a4e7fe1c04916618d48c77b46cae3397993099af92ebcb9", true]
- ["x64", "src/app-softpc/softpc.new/base/video/ega_writ.c", "6c7138fc801088a5aca61a4980aa49558586ea442ad2566e7008d79dd15e7ffc", "6c7138fc801088a5aca61a4980aa49558586ea442ad2566e7008d79dd15e7ffc", true]
- ["x64", "test/app-softpc/unit/machine/vga_frame_smoke.c", "a7f3dec3e632306af37edc3a97aa3e8c9f08cc8e291e983304a60331cfc1b7c6", "a7f3dec3e632306af37edc3a97aa3e8c9f08cc8e291e983304a60331cfc1b7c6", true]

Counter-review distinguishes the old ANSI-related rationale from actual selected
macro behavior. No consumer-specific exception is introduced. Complete final
cached build/publication and EXE-equivalence proof are pending before delivery.
Final normalized header identities: {"base/inc/gmi.h": "1298cfa1304729c5619328360e02aa6d28766f11e603ad4a53893dba11db515c", "base/inc/gfx_upd.h": "3450e83aee27dc45a79a013fdf85b5aefc95ee8a578590f36c732c1465a26e88"}.
Delivered frontier remains 354/472 pending final qualification. File 359 gvi.h
has only been inventoried; no completed disposition is claimed.


### P84 final-source and artifact-equivalence qualification

Cached build/publication results: [["x86", 0, 22.8], ["x64", 0, 31.85]].

- softpc32.exe: 3463049 bytes, 9 changed bytes; prior stamp 14:57:16 Nov 13 2026, final 15:03:04 Nov 13 2026; SHA-256 436d680ba23d25005c61b47de7a0b211003cd140aba04303c0eb8d00510aef08.
- softpc64.exe: 3082345 bytes, 8 changed bytes; prior stamp 14:57:24 Nov 13 2026, final 15:03:27 Nov 13 2026; SHA-256 b131b9116b5d662479ee457035b89db3e0b02d541c1bc2b81afa78ab67d64156.

Complete EXE byte comparison permits only located COFF TimeDateStamp,
OptionalHeader CheckSum and the eight compiler-time bytes at the unique date/time
literal. Lengths, PE offsets and literal positions match. No bytes outside
those fields differ. Final source is the proven declaration candidate plus
one literal original comment. Under the owner's artifact-equivalence exception,
no repeat full runtime suite is run; P80's 392/392 remains the runtime baseline.

Actual-change review: production +8/-11, net -3; no test/build source changes.
XP-relative diff decreases nineteen lines: eighteen from restoring seven
original member lines in place of eight adapted lines plus three comment lines,
and one from restoring the omitted original comment. Existing excluded-body
scaffolding is unchanged and not included in that reduction. No new mirror
point, state owner, fallback, provider, calling path or shared-component change.
R0432/R0433 are restored, with no retention decision added. Manifest/gate
verification remains separate from the artifact proof.

Frozen delivered frontier 358/472; next file 359 base/inc/gvi.h. S1 and prior
qualified owner decisions remain open. User configuration/media are untouched.

P84 mirror/documentation gates and actual nine-file diff review pass. Both
owned workers exited; scratch preview, exact removal and residue check pass.
No unrelated worktree changes or retained temporary probe files remain.

## File 359: gvi.h (P85)

Pinned XP raw SHA-256 4c49d9dc3c0107f5090e446ec72d59ec466c5d3b22e0c7a47f52bd0aad404c62.
The complete normalized file comparison finds one non-exclusion addition before
XP line 491: local lines 292-294, the reason comment, get_chain2_screen_ptr
macro and diagnostic line restoration. Existing excluded bodies/scaffolding
are skipped without new accounting. All remaining selected text matches XP.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0434 | Added CHAIN2 display-offset conversion, three added lines including reason and line control. | Qualified residual pending owner decision. Actual nt_cga.c and gfx_updt.c consumers need the selected odd/even plane layout; restoring the original direct offset in both consumers fails the actual text painter oracle on x86 and x64. This is coupled to the independently demonstrated layout requirement, including R0362, not proof merely from current caller dependence. |

Source chain: the existing BIOS/C-VID writes and unchained readback retain
character and attribute lanes in the four-plane backing. Text update offsets
advance four units per cell, while successive backing cells are eight bytes
apart. The added conversion maps offset 12 to 24 and offset 13 to 25;
blind doubling would incorrectly move the attribute lane. nt_text gathers
four-byte host cells with an eight-byte source step; gfx_updt uses the same
conversion for initial addresses and cursor attributes. All production macro
references occur in those two files. Zero-offset calls remain deliberately
consistent with this address contract; replacing them individually provides
no coherent simplification. No duplicate Compat implementation is substituted.

Focused dynamic proof on the unchanged P84 production baseline: the existing
vga_frame_smoke executable passes on x86 and x64, exit 0. A task-local header
variant maps get_chain2_screen_ptr directly to the original get_screen_ptr.
Both actual consumer objects are recompiled with their real Ninja arguments,
inserted into a copied machine archive, and linked to the unchanged fixture.
Both compile/link successfully, then fail the identical full-cell comparison
at vga_frame_smoke.c:390. x86 exits 3221226505 and x64 exits 3; both processes
exit naturally. The oracle independently selects raw source bytes by cell,
including nonzero partial-region origin, multiple pitches and row counts.
This is an actual painter regression, not a new Win3.1 guest reproduction.
The earlier actual BIOS/C-VID handoff proof supplies the independent layout
basis; this probe does not claim a whole-layout XP rollback was performed.

The local reason marker explains a qualified semantic difference and the
#line restores the original diagnostic location; removing either is not an
exact restoration of a separately unnecessary adaptation. The pending option
is retain this three-line addition together with the established layout
contract, or owner-directed coherent restoration of that contract. No new
mirror patch, fallback, dispatch or permanent test API is added.

Production/test/build source changes: zero. XP-relative diff reduction: zero;
qualified residual: +3/-0, awaiting owner decision. Existing published EXEs
are unchanged; no new build/publication or full regression is claimed. P84
remains the published artifact baseline, P80 the 392/392 full-runtime baseline.
Frozen audited frontier advances to 359/472; next is file 360 base/inc/host.h.
S1 and the pending difference decisions remain open.

P85 actual three-file documentation review and mirror/documentation gates pass.
The worker and owned children exited; task-scratch preview, exact removal and
residue check pass. No configuration, guest asset or published binary changed.

## File 360: host.h (P86)

Pinned XP raw SHA-256 3b5397efd06f83fbe2c31766d250ae807409e174e673812d08b5ad8ae916a716.
Full normalized comparison finds one selected difference: original line 50
versus local lines 52-55, the memset4 declaration and DIV-090 explanation.
All other differences are existing exclusions/line scaffolding, skipped
without new accounting. The original include wiring remains required.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0435 | DIV-090 changes IU32/ULONG parameters to unsigned int to agree with host_def.h and Compat's provider. | Restore the exact original declaration and remove the two-line rationale. The supported insignia.h branch expands IPT3 to (), so neither version contributes parameter types. The actual host_def.h prototype and provider remain unchanged. Twenty full preprocessed pairs are identical; the claimed active declaration mismatch does not exist. |

The only production calls are ega_vide.c's two memset4 calls. The original
host_def.h declaration remains a concrete unsigned-int prototype. This audit
does not widen its types, alter the implementation, enable ANSI, remove any
include or introduce a replacement wrapper. No duplicate device implementation
is moved into Compat. Other files' differences are not declared audited here.

All ten direct C consumers were obtained by scanning the current source tree:
rom_basc.c, nt_graph.c, cga.c, ega_vide.c, egawrtm0.c, vga_vide.c, video.c,
ica.c, timer.c and time_day.c. For each architecture, both header versions
were preprocessed with that consumer's actual Ninja compiler flags, with only
output/dependency flags replaced by -E -P. Forty compiler runs succeed; all
twenty complete-output pairs match byte for byte. This is stronger than merely
matching the memset4 line, but is not a dynamic memset4 behavior test.

Final-source cached builds and publication succeed: x86 7.89 seconds, x64
9.66 seconds. Both logs contain 89 warning lines, all from preserved mirror
files; no non-mirror warning or build failure was found. Whole EXE byte
comparison permits only the exact PE COFF timestamp, optional-header checksum
and uniquely located compiler-time literal; file lengths and all other bytes
match. Both EXEs change seven bytes, within those fields only. Under the
owner-approved artifact-equivalence exception no full suite is rerun; P80's
392/392 remains the runtime baseline, not a new P86 test count.

- x32: Nov 13 2026 15:15:10; SHA-256 5d1d22b0e6b6db214097f51a2fe8e27e768a598db5f77c95ad03a4f8a59e8df9; previous 436d680ba23d25005c61b47de7a0b211003cd140aba04303c0eb8d00510aef08.
- x64: Nov 13 2026 15:15:17; SHA-256 33d3a3cbcafe4e868f47f2c7814c68af17c9646f0fa48a4ad442acc200221d39; previous b131b9116b5d662479ee457035b89db3e0b02d541c1bc2b81afa78ab67d64156.

Actual production diff +1/-4, net -3; test/build source changes zero.
XP-relative selected diff decreases five lines (+4/-1 becomes +0/-0).
R0435 is restored, not a new pending retention decision. Existing exclusion
scaffolding and original declarations are unchanged. Mirror manifest local
hash is refreshed; original/subset provenance hashes remain fixed.
Frozen frontier 360/472; next file 361 base/inc/host_cfg.h. S1 and prior
qualified owner decisions remain open.

P86 actual eight-file review and mirror/documentation gates pass. Both owned
workers exited; scratch preview, exact removal and residue check pass. No
unrelated configuration/media changes remain.

## Files 361-362: host_cfg.h and host_com.h (P87)

File 361 host_cfg.h: the complete normalized file equals pinned XP SHA-256
cc396afb209e97d88fd9ef66a180bbecc442f56562de6cb24b6971a172e45a07.
Its original config.h include remains. The empty guarded body is original,
not a reason to create a new include-removal diff. No point or code change.

File 362 host_com.h: pinned XP raw SHA-256
efcdcc996c542a3c726e192bd1d0c1f4b8979d223bd037f7fe1ebe580ea7340f.
Full-file comparison finds two selected differences outside the existing
excluded bodies/scaffolding, which are skipped without new accounting.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0436 | Original line 77's long argument in the documentation example changed to intptr_t. | Restore the exact original comment text. The entire example is inside the original block comment, not a function definition or executable signature. |
| R0437 | Original line 197's LONG argument changed to intptr_t and gained a DIV-068 marker. | Restore the exact original declaration and remove the marker. Current supported IPT3 expands both declarations to an empty parameter list. All actual consumer preprocessing is identical; changing this header does not narrow any runtime pointer. |

Actual pointer-bearing calls in com.c still pass intptr_t, and serial.c's
provider still receives lib_iptr; HOST_COM_INPUT_READY and HOST_COM_MODEM
still dereference that native-width argument. No caller or provider is changed.
This is not a rollback of the independently necessary x64 pointer transport.
The header is retained by three production C consumers (com.c, platform.c,
serial.c) and two fixtures (serial_smoke.c, checkpoint_smoke.c).

For all five consumers, both versions were preprocessed under their actual
Ninja compiler arguments on each architecture. Twenty compiler runs succeed;
ten complete -E -P output pairs match byte for byte. The candidate header is
then applied unchanged. Independent review confirms both changes restore exact
XP text, preserve the include guard and excluded branches, add no API or path,
and leave no selected difference in this file.

Final cached builds/publications pass: x86 9.75 seconds, x64 11.24 seconds.
Each build log contains 271 warning lines, all within preserved mirror files;
there is no non-mirror warning or build error. Full EXE comparison confirms
identical lengths and all bytes outside the exact COFF timestamp, PE checksum
and unique compiler-time literal. Changed byte counts: x86 six, x64 five.
The owner-approved artifact-equivalence exception omits repeat full testing;
P80's 392/392 remains the full-runtime baseline, not a new P87 run.

- 32-bit publication: Nov 13 2026 15:19:05; SHA-256 f1dd971c8f74fd79bf7b215d0253841716ea8009eafe0ee6e6656b30448efd6f.
- 64-bit publication: Nov 13 2026 15:19:14; SHA-256 4d67c0edf40e58725e5d1341a92ba2374261f6e66811ca90b68f6b0e9b3e6189.

Actual production +2/-3, net -1; test/build source changes zero. XP-relative
selected diff decreases five lines (+3/-2 becomes +0/-0). R0436/R0437 are
restored; no retention decision is added. Manifest local hash is refreshed,
with original/subset provenance fixed. Frozen frontier 362/472; next file
363 base/inc/host_gwi.h. S1 and prior pending decisions remain open.

P87 actual eight-file review and mirror/documentation gates pass. The owned
worker exited; scratch preview, exact removal and residue check pass. No
configuration, media or unrelated worktree changes were made.

## Files 363-366: original host headers (P88)

Full normalized pinned-XP comparisons and actual include scans complete this
ordered group. Existing excluded bodies and their established scaffolding are
skipped without new accounting; no exclusion point or count is introduced.

| File | Fixed XP raw SHA-256 | Selected-content disposition and actual wiring |
| --- | --- | --- |
| 363 base/inc/host_gwi.h | 7f5fd9d64ecf10f700c774e367740d8c761a6345e73ed1352288a39b33a73526 | Complete normalized content equals XP. Original includes remain in error.h, keyboard.h and gfx_upd.h. The comment-only file is not a reason to introduce include-removal differences. |
| 364 base/inc/host_lpt.h | 71e4e9fd3253ecaad3b8209b90d8bf7497bdfc6bad67890d9534d4512c902459 | Selected declarations, constants and comments match XP; remaining differences belong to existing exclusion scaffolding. Includes occur in original reset.c, printer.c, printer_.c, Compat parallel.c and the printer/checkpoint fixtures. No new native-printer capability is claimed. |
| 365 base/inc/host_sas.h | 00c5ab25083ff6a19ce2ad461c43bb37adbd2aeb241865cc9b3a2c4847ea8d2e | Non-excluded content matches XP; original sas.h still includes the file. No HOST_SAS dispatch implementation is claimed active merely because the header exists. |
| 366 base/inc/hunter.h | deb671235433d87e347299b07832c1b9a8261fb0827005baf5fdd0252983e5b5 | Non-excluded content matches XP; original reset.c still includes the header. The HUNTER implementation remains excluded; retaining the original include does not enable it. |

Independent counter-review: keep the original files and include wiring, with
no duplicate Compat path or gratuitous declaration cleanup. Removing these
files would require new include-removal differences and violate the owner's
no-increase rule. Whole-file diff review found no selected residual requiring
an owner decision in these four files.

Production/test/build changes zero; XP diff change zero. This is a read-only
source audit and documentation delivery, not a new runtime/build qualification.
P87's dual-EXE publication and P80's 392/392 runtime baseline remain unchanged.
Frozen frontier 366/472; next file 367 base/inc/ica.h. Its preliminary scan
finds an added CPU_40_STYLE guard around PIC interrupt-name macros. The CCPU
enum uses the same CPU_HW_INT name for value two, whereas the PIC macro is
zero; actual restoration still requires a bounded check. This observation is
not a completed file audit or an approved retention decision. S1 remains open.

P88 actual three-file documentation review, mirror/documentation gates and
build residue check pass. No task processes or scratch were created.

## File 367: ica.h (P89)

Pinned XP raw SHA-256 c7d4feeec4957ee0a9cbc60aebfb77936686df9c4109fbcf808e182ff89b8e6f.
The complete comparison identifies two non-exclusion differences. Existing
excluded bodies/scaffolding are skipped without new accounting. Selected
original declarations, data structures and interrupt constants otherwise match.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0438 | Local lines 91-106 add a CPU_40_STYLE guard and two line-control directives around original lines 87-98. | Qualified residual pending owner decision. Removing just the added guard/scaffolding makes actual c_main.c fail to compile on both architectures: CPU_HW_INT becomes macro zero and duplicates CPU_HW_RESET's switch case. Current controls compile successfully. |
| R0439 | Original final Ctrl-Z line is absent. | Qualified format/compiler residual pending owner decision. Restoring byte 0x1a with its newline after the retained header makes actual c_main.c fail on both architectures with stray '\32' in program. Current controls compile successfully. This byte is outside the excluded GISP body, so it is not silently skipped with that body. |

The active root targets select CPU_40_STYLE/CCPU. cpuint_c.h defines the
CPU_INT_TYPE enum with CPU_HW_RESET=0 and CPU_HW_INT=2. c_main.c includes
the CPU interface before ica.h, then c_cpu_interrupt switches on those enum
names. The original PIC header's CPU_HW_INT macro means bitmap bit zero,
not the CPU enum. Restoring it substitutes zero into the later hardware case
and collides with reset. Renaming the CPU enum or rewriting consumers would
create additional differences; no such alternate repair is made. The original
CPU_40_STYLE callback block later in ica.h is separate and remains unchanged.

Probe correction: the first task-local candidate used an unrestricted text
replacement and accidentally removed that later block's endif too. Its results
are rejected. The corrected candidate removes only the first added guard and
its two line-control lines; its exact four-line deletion was reviewed before
rerunning. Both corrected unguarded builds fail only with duplicate case value,
not unterminated conditional structure. The Ctrl-Z candidate is separate and
retains all guards. Six final compiler runs use the real Ninja c_main.c commands:
current x86/x64 exit 0, corrected unguarded x86/x64 exit 1, Ctrl-Z x86/x64 exit 1.
All jobs exit naturally. These are compile-contract proofs, not guest runtime
reproductions or a new full regression run.

The existing residuals are +4/-0 guard/line control and +0/-1 EOF line. No
source reduction or XP diff reduction is claimed. Production/test/build changes
zero; published EXEs remain P87, full runtime baseline remains P80's 392/392.
No initializer, compatibility alias, fallback or new permanent probe API is
introduced. Frozen frontier 367/472; next file 368 base/inc/idetect.h. S1 and
these evidence-qualified owner decisions remain open.

P89 actual three-file documentation review and mirror/documentation gates pass.
Both probe workers and their children exited; scratch preview, exact removal
and residue check pass. No unrelated source, configuration or asset changed.

## Files 368-369: idetect.h and ios.h (P90)

File 368 idetect.h: pinned XP raw SHA-256
b95cbdd0bc63b85852f0bd71132f0045cae86b5e571dba24816f6f3c6a696c6b.
Full-file comparison finds no selected difference outside the established
NTVDM exclusion/scaffolding. Original consumers include idetect.c, keyboard,
video, disk and communications code; Compat platform.c uses the original idle
contract. Retain the original header/include wiring, with no new point.

File 369 ios.h: pinned XP raw SHA-256
5cc2d50b2027c42f9fae436f4767507c503bd1eeeef4b26be647f005f4dd9b8d.
The only selected difference is line 23: adding SOFTPC_STANDALONE to the
original OR expression selects the existing 0x10000 table instead of 0x400.
All other differences are established excluded bodies/scaffolding, skipped
without new accounting. The remainder of the selected interface matches XP.

| Point | Initial analysis | Independent counter-review and disposition |
| --- | --- | --- |
| R0440 | One-line product profile addition selects a full 16-bit I/O dispatch table. | Qualified residual pending owner decision. Coherent restoration of the original expression aliases an unregistered high port onto actual COM1; both architecture controls distinguish them, while restored configurations fail the same assertion. No new I/O implementation is required. |

Source chain: ios.c owns and initializes both adapter tables, registers actual
ports, and masks indices by PC_IO_MEM_SIZE-1. Original CCPU in.c/out.c use the
same mask directly; these are the only C units containing this size macro.
COM1 is registered at 03F8-03FF. There is no registration of 07FD. The UART's
adapter_for_port and low-three-bit register decode accept the aliased address,
so the smaller dispatch map incorrectly invokes the COM1 status handler for
that unregistered port. This is distinct from a device deliberately registering
an alias; the table mask imposes it on every device. Historical profile claims
alone were not accepted as proof.

Focused evidence: a disposable extension of the existing serial_smoke fixture
creates/resets the actual machine and invokes actual CCPU IN8 at 03FD and 07FD.
The expected high result is IO_EMPTY_PORT_BYTE_VALUE (FF), independently of
table size; the registered COM1 status is 60. Current x86/x64 controls both
report low=60, high=FF and complete the remaining fixture with exit 0.
For the counterexample, the exact original header condition is restored in a
private include directory. Actual ios.c, in.c and out.c objects are recompiled
with their real Ninja flags and replaced together in a copied machine archive;
all compile/link steps succeed. Both variants report low=60, high=60 and fail
the same explicit isolation assertion. x86 exits 3221226505, x64 exits 3;
all processes exit naturally. No product source, original guest or published
binary is changed. Disposable fixture files remain task-local.

Proof boundary: this exercises the actual CPU I/O worker and UART dispatch,
not a new real DOS/Windows reproduction, an exhaustive port matrix, or a guest
instruction-decoder test. It demonstrates the full-map contract's observable
failure under coherent original restoration; it does not prove every historic
PC should lack hardware aliases. Defining NTVDM/GISP_SVGA/SFELLOW merely to
select 64 KiB would also change unrelated machine profiles, not simplify the
current port adaptation. No alternative flag, duplicate table or fallback is
introduced.

Production/test/build source changes zero; XP diff change zero. The qualified
residual is +1/-1 awaiting owner choice. Published EXEs remain P87; P80's
392/392 is the unchanged full-runtime baseline. Frozen frontier 369/472;
next file 370 base/inc/j_c_lang.h. S1 and pending decisions remain open.

P90 actual three-file documentation review and mirror/documentation gates pass.
The worker and children exited; scratch preview, exact removal and residue
check pass. No user configuration, media or unrelated files were changed.

## Files 370-376: register/device headers (P91)

Each full normalized file was compared with its pinned XP counterpart, in
frozen order. Existing excluded bodies/scaffolding are skipped without new
accounting. No selected semantic or nonsemantic adaptation was found.

| File | Fixed XP raw SHA-256 | Actual retained relationship |
| --- | --- | --- |
| 370 base/inc/j_c_lang.h | d98ec1630242cda361ad334193c0919d651ae673d3fd1e997784e77db229cc7a | Original generated C-VID register declarations/little-endian offsets, included by sevid/sinit units, ev_glue.c and chain2_rule_smoke. Non-excluded content matches XP. |
| 371 base/inc/keyba.h | b946d6fa9a2e4af545b2499d49042047155c628007c272449a19658d9f97897d | Original controller prototypes/function pointers used by keyba.c, keybd_io.c, reset.c and Compat keyboard.c. Non-excluded content matches XP. |
| 372 base/inc/keyboard.h | dc63ed1ca7c58c80f0869427a09df6799e96ae4191413a7eecfe9ab751700fa6 | Complete normalized file equals XP. Original keyboard/BIOS and Compat consumers remain. |
| 373 base/inc/mouse.h | 5def51c2afcf05668fcd9516fad0f1c91802c000d6d6cfac1b1a842ca54af1f4 | Original mouse.c/reset.c and mouse/checkpoint fixture includes remain; selected constants/interfaces match XP. |
| 374 base/inc/mouse_io.h | bd398b6b37123919be5aa7f9807b45c715d4f0caac8aa04ec2ea5602ca5a112f | Original mouse.c and reset.c still include it. The former same-name implementation's retirement does not make the header/include disposable. Non-excluded content matches XP. |
| 375 base/inc/ppi.h | f4e0c66e1dd7ab4e6976fde1c5a996613d18a6430ec1718147ce0d9b226844dd | Original ppi.c, keyba.c, keybd_io.c and reset.c declarations; selected content matches XP. |
| 376 base/inc/printer.h | c15f20e8729b12e3a903c7bc8bc818e72baccfcac1372ad493e7af9c534d128a | Original printer.c, printer_.c and reset.c includes, declarations and BIOS constants match XP outside existing exclusion scaffolding. |

Independent counter-review: no point requires restoration or retention approval
in this group. Original declarations and includes are preserved; do not remove
whole headers or unused-looking declarations at the cost of additional XP
differences. A textual include does not independently prove every historical
function declared in a header is an active runtime capability. No such claim is
made, and excluded implementations are not reopened.

Production/test/build/EXE changes zero; XP diff change zero. This read-only
source audit does not claim another runtime test or rebuild. Published baseline
remains P87 and full-runtime baseline P80's 392/392. Frozen frontier 376/472;
next file 377 base/inc/quick_ev.h has snapshot interface additions requiring
separate actual-caller/ownership investigation. It is not marked audited.
S1 and earlier qualified pending decisions remain open.

P91 actual three-file documentation review, mirror/documentation gates and
build residue check pass. No task processes, scratch or unrelated edits exist.

## File 377: quick_ev.h (P92)

Pinned XP raw SHA-256:
`5f1c17105d08a7604cc5cc5101ad2276b1825a46200d28e36cd52d81debb5874`.
The complete selected-header comparison has two insertion ranges before XP
lines 21 and 29. Existing NTVDM exclusion/scaffolding is skipped without new
accounting. All other selected text matches the pinned XP file.

| Point | Original/local scope | Initial analysis | Independent counter-review | Disposition |
| --- | --- | --- | --- | --- |
| R0441 | XP before line 21; previous local lines 21-25 | Five added explanation lines do not declare state or affect preprocessing. | They are an ordinary complete comment, not a notice, directive, macro continuation or required line-control marker; the implementation already explains the same archive boundary. | Restore absence exactly: +0/-5 production and five fewer XP-relative added lines. |
| R0442 | XP before lines 21 and 29; final local lines 21-46 and 55-63 | Two snapshot record types, two callback types and three snapshot entry declarations support actual pending-event archival. | Mere current references would not suffice. The original private queue and original add API cannot enumerate or reconstruct the saved state; see source proof below. No separate Compat scheduler replaces it. | Qualified pending owner decision, +35/-0 relative XP; not automatically approved. |

Actual compilation/call chain: root CMake includes base/system/quick_ev.c in
softpc-machine. Machine snapshot_image.c capture/restore calls Compat CCPU
archive.c, then Compat devices/archive.c. The latter measures queue capacities,
captures records, serializes them with explicit u32 fields, and restores them
with stable callback-ID decoding. All three added APIs have production calls.
The structures are also used by the actual quick_ev.c provider, not merely tests.

Source necessity proof: the original Q_EVENT node and both queue heads, hash
tables, next-handle counters and tic countdown belong to quick_ev.c's private
state. The XP public interface supplies add/delete/dispatch/init, but no readout
or restore operation. add_event allocates a new handle and changes queue timing;
it immediately calls the callback when time is zero. Replaying adds therefore
cannot restore the original handles/countdowns and would allow callbacks during
load. Removing the declarations together with their provider/caller wiring would
remove pending-event restoration from an actual supported snapshot operation;
moving a copy of queue internals into Compat would duplicate the state owner.
This is source/contract evidence, not a claim that a deliberately uncompilable
header alone proves necessity or that every possible redesign has been tested.

Field mapping was checked end to end: time_from_last/original_time/event_type
retain the original queue scheduling representation; handle/param retain device
references and callback argument; callback_id replaces process-local function
addresses. hash_next_index and the two 16-entry heads retain the original
hash-list ordering as indices rather than native pointers. next handles, counts
and entry totals cover allocator position, countdown and payload bounds.
Capture and restore read/write every field; device_write_queue/device_read_queue
serialize every field. No unused field or unused entry was found. The original
queue remains the only scheduler; archive records are copies, not another live
queue. This header audit does not claim the later quick_ev.c file's complete
implementation has already passed its ordered diff audit.

Fresh focused dynamic proof: existing checkpoint_smoke invokes
verify_event_queue_archive through its main. It captures two quick events and
one tick event, rejects an invalid late callback without destroying the live
queue, clears and restores the queues without running callbacks, checks saved
countdown, and then dispatches the expected event order. The same executable
also exercises the existing snapshot integration assertions. Both current x86
and x64 tests pass (1/1 each, 1.67 s and 3.18 s). No new test/source fixture or
user snapshot was created or changed. These tests do not exhaust hash collision,
all callback types, malformed archives or real Windows guest sessions; the
field/ownership necessity above is source proof rather than such a claim.

P92 actual production change is +0/-5, test change zero. Selected XP differences
fall from +40/-0 to +35/-0; excluded scaffolding is unchanged. Both original
insertion ranges remain, illustrating why range count alone is not progress.
Manifest local hash is refreshed; XP/subset hashes remain unchanged.

Cached background builds finish successfully: x86 17.27 s, x64 21.20 s. The
985/988 warning lines all originate in preserved softpc.new, with none outside
that mirror. Published EXE lengths are unchanged. Full-file comparison to P87
finds 8 changed bytes in x86 and 6 in x64, all within verified compiler time,
PE timestamp/checksum fields. No other byte changes. Compiler stamps are
Nov 13 2026 15:38:52 / 15:39:11. Published SHA-256:

- x86: `4e872186efa35a0bb8afb220f05980f0100cd92c067c5a0aa284fe3cf9c751f8`.
- x64: `29ec4cfabddf7c2deb7537c905b2fce7c0cafb4fb94eb1d7dd2d72029f3b6072`.

The owner-approved artifact-equivalence exception applies: no full-suite rerun
is claimed; P80's 392/392 remains the latest full-runtime evidence. The focused
snapshot results above are fresh. Frontier advances to 377/472 after this
complete header audit; next is file 378 base/inc/rom.h. S1 and qualified owner
decisions remain open.

P92 independent actual eight-file diff review, mirror and documentation gates,
and whitespace check pass. CIM confirms the owned runner and child jobs have
exited. Clean-Build preview/apply removes only the task scratch; residue check
passes. No INI, media, snapshot, unrelated source or sibling repository changed.

## Files 378-380: ROM, serial and RTC headers (P93)

Read-only full-file comparison against the pinned XP revision, in frozen order:

| File | XP raw SHA-256 | Selected-content result and actual consumer |
| --- | --- | --- |
| 378 base/inc/rom.h | 8beb30e866d660cb7c7d02b6364ea092f8e8974dfaef6e19c3f9a7c5de17d1ec | Complete LF-normalized identity, including notices, original macintosh condition and ROM entry declarations; included by original rom.c, reset.c and ccpusas4.c. |
| 379 base/inc/rs232.h | 54cf3d4437b813e87124d70cd54ae193b89700e3096236d99b73dcf791cd1d75 | Selected UART bitfields, register/port/timeout constants, serial entry declarations and adapter_for_port match XP; original com.c, rs232_io.c and reset.c include it. Differences are existing exclusions and their surrounding line/control scaffolding, not a claim of whole-file identity. |
| 380 base/inc/rtc_bios.h | 5351e07637a4925311775332104c0617411780a7a7977eea3daa8c2162a80bf5 | Complete LF-normalized identity, including notices, BIOS offsets, TIME_DEC and DOUBLE_TIME; included by original floppy.c, cmos_bis.c and tape_io.c. |

Initial review used full normalized comparisons. The first rs232.h display was
truncated by its excluded bodies, so it was not accepted as complete evidence;
a second bounded opcode listing and selected local spans were inspected. That
review explicitly checked selected sibling content sharing replacement ranges
with excluded code, including the UART ID/status fields, FIFO-adjacent register
constants, parity constants and serial-port mapping. No selected declaration or
runtime macro difference was found. Existing directive comments/spacing and
blank/line-control scaffolding around excluded bodies remain under the approved
skip boundary; no new points or exclusion ledger were created.

Counter-review: root CMake explicitly compiles all the named original consumers.
Their includes are retained, and their original declarations do not duplicate
a new Compat implementation. The header's historical platform declarations do
not imply that all historical backends are supported; none is newly enabled.
No whole-file retirement or include removal is justified or attempted.

Production/test/build/EXE changes zero; XP-relative diff change zero. This is
source evidence, with no new compile or runtime claim. P92 remains the published
artifact and focused checkpoint baseline; P80's 392/392 remains the full-runtime
baseline. Frontier 380/472; next file 381 base/inc/sas.h is unreviewed. All earlier
qualified owner decisions and S1 remain open.

P93 actual three-file documentation diff review, mirror/documentation gates,
whitespace and build-residue checks pass. No task scratch or background jobs
were created. The initial frozen file hashes remain unchanged.

## File 381: sas.h (P94)

Pinned XP raw SHA-256:
`ac96517e65ef664fac67ec56220c1ac6d060e72acd58339347ef21484c45f4c3`.
Full-file opcode comparison identifies the following non-excluded differences;
existing exclusion bodies and line/control scaffolding are unchanged and receive
no new accounting. Broad inclusion of sas.h does not make every declaration used.

| Point | XP/current scope | Initial analysis | Independent counter-review | Disposition |
| --- | --- | --- | --- | --- |
| R0443 | XP before line 103; added blank and sas_overwrite_memory declaration | No actual call uses the added public name. | Whole src/test/tools C/H search finds only declarations, a generated function type, c_sas_overwrite_memory provider and ntstubs wrapper definition; none needs this added header declaration. Preserve original generated types, implementations and non-CPU40 declaration. | Restore original absence, +0/-2 production. |
| R0444 | XP line 109; CPU40 Length_of_M_area | PHY_ADDR matches the actual storage definition; IHPE is pointer-width. | Both pinned XP ccpusas4.c and zfrsrvd.c already define/import PHY_ADDR. Changing those to match the old header would add new mirror differences, while globally narrowing IHPE would break its pointer-carrier role. Actual compiler counterprobe confirms the local mismatch, detailed below. | Retain only IHPE-to-PHY_ADDR token change as qualified pending owner decision, +1/-1 relative XP; restore original spacing and trailing comment. |
| R0445 | XP line 117; non-CPU40, non-GISP declaration | Current PHY_ADDR replacement has no supported CPU40 consumer. | Both sys_addr and PHY_ADDR expand to IU32; exact XP spelling/comment is sufficient even in this retained inactive branch. No new conditional exclusion is added in this audit. | Restore exact XP sys_addr declaration, +1/-1 production and two fewer XP-relative diff lines. |

base_def.h defines PHY_ADDR and sys_addr as IU32, while current insignia.h
uses unsigned long for IU32 and UINT_PTR for IHPE. The original CCPU's
Length_of_M_area storage is a guest physical byte count, not a native pointer.
The active root configuration selects CPU_40_STYLE for x86 and x64. sas.h has
actual original BIOS, device, video and CCPU includes; root CMake compiles those
consumers. Compat has no duplicate definition that should replace the original
storage owner.

Counterprobe uses each real Ninja ccpusas4.c compile command, changing only
output/dependency paths and prepending a task-private sas.h with the selected
Length_of_M_area declaration restored to IHPE. Other local necessary changes
and exclusions remain unchanged. Current final header compiles; the original
selected declaration conflicts with the unmodified original PHY_ADDR storage.
An initial runner incorrectly expected x86 success on equal bit widths. Actual
x86 diagnostic reports unsigned int versus unsigned long, so that assumption
was rejected; no build or EXE publication had begun in that first run. After
confirming process exit, the expectation was corrected and both architectures
were rerun. Equal widths do not make distinct C types compatible. This is
compile/source proof, not a runtime memory-corruption reproduction.

Production change +2/-4, net -2; tests unchanged. Excluding existing scaffolding,
selected XP added/deleted diff falls from +4/-2 to +1/-1, four fewer diff lines.
The surviving line is reduced to one type-token deviation; restored comments
and spacing do not falsely count as removal of that surviving line difference.
Historical sas.h register counts are superseded by this precise scope.

Final counterprobe: current compile exits 0 on both architectures; the original
IHPE declaration exits 1 on both. x86 reports unsigned long versus unsigned int;
x64 reports unsigned long versus unsigned long long. No runtime fault is claimed.
Both cached builds then succeed, x86 32.11 s and x64 45.46 s. Their 4107/4160
warning lines all originate in the preserved mirror, none outside softpc.new.
Published EXEs have unchanged lengths; full-byte comparison to P92 finds only
8/7 bytes changed inside verified compiler-time and PE timestamp/checksum fields.
Fresh compiler stamps are Nov 13 2026 15:45:36 / 15:46:12. SHA-256:

- x86: `aabbb8a911ac347f20b259aa1a181b686880132febcaefb89cadfc0af011efbd`.
- x64: `fd8e52be68a9f7dde709685ccc521bb48b58412c9c9f69654d3af2169793ea0e`.

Artifact equivalence invokes the approved test-omission rule: no new runtime
suite is claimed. P92 checkpoint tests and P80's 392/392 remain the respective
focused/full runtime baselines. Final header CRLF and local manifest hash are
preserved/updated; upstream and selected-subset hashes are unchanged. Frozen
frontier is 381/472, next file 382 base/inc/sasp.h. S1 remains active and R0444
awaits owner disposition, not automatic approval.

P94 actual eight-file diff review, mirror/documentation gates and whitespace
check pass. CIM confirms task processes exited; exact task scratch preview,
removal and residue check pass. User INI/media/snapshots are untouched.

## Files 382-383: SAS dispatch and Sound Blaster headers (P95)

File 382 base/inc/sasp.h is completely LF-normalized identical to pinned XP,
raw SHA `861eb41563531cab6d3b0011161188ff1dbeefa985af1074afaa2ff436a7d696`.
Actual compiled ccpusas4.c includes it and defines/calls SasSetPointers. Preserve
the original declaration and file; no new patch point or deletion is warranted.

File 383 base/inc/sndblst.h XP raw SHA:
`35b5225c531deb47037fbe676a9057c427137b46c988863e15701eb3fa74a353`.
This header has no excluded body to skip. Full raw-XP comparison, actual C/H
consumer search and original host/src/nt_sb.c signature review establish:

| Point | Original/local scope | Initial analysis | Independent counter-review | Disposition |
| --- | --- | --- | --- | --- |
| R0446 | AUTO_BLOCK_SIZE moved from XP line 138 to local hardware constants | Same value, no order-dependent consumer in the header. | Actual DSP use appears after the complete include; original position works. | Restore original position exactly. |
| R0447 | XP lines 30-54, excluding SbTerminate return; coupled nt_sb.c initialization signature | USHORT/IU16 and VOID/void are available through insignia.h. Initial BOOL-only header restoration conflicts with current int provider. | Do not use a broken partial restoration as necessity proof: XP provider also returns BOOL. insignia.h defines BOOL as UINT, not Windows signed BOOL; restore provider and header together and adapt the one test's explicit extern/wrapper types. Every current return is TRUE, FALSE or equality, and production callers test truth value. | Restore original aliases, spelling and original four-line SbInitialize signature. Test extern/wrapper become unsigned int to match the actual macro expansion. |
| R0448 | XP line 42 SbTerminate return VOID; current IBOOL | Real teardown returns PCM/MIDI close failure. | Machine reset/destroy and platform reboot consume that failure; a void coherent restoration would require deleting error propagation, allowing reset continuation after failed teardown. Existing failure fixture checks exactly that stop boundary. | Qualified pending owner decision, +1/-1. No new callback or recovery path. |
| R0449 | Added 0x36 MIDI_READ_TIMESTAMP_POLL_WRITE_POLL_UART among XP MIDI commands | Actual DSP dispatcher consumes unsupported UART data rather than interpreting it as later PCM commands. | Device fixture explicitly exercises 0x36 and every following byte, checks retained parser state, no PCM/MIDI output, reset and snapshot state. Removing both definition and dispatch handling would return to command interpretation. This is source-path evidence plus current fixture coverage, not a fresh original-bug replay. | Qualified pending owner decision, +1/-0. |
| R0450 | XP lines 126-152, 158-169 and 194-205 | Previously omitted original constants/comments/SYNTH_DATA and relocated MPU constants have no justification for deletion. | Search confirms restored unused declarations do not install native FM, alternate MPU ports or a second output path. The used MPU constants retain identical values; their original location is after the same completed include. | Restore omitted text and original definition positions exactly. |

Production consumers are compiled nt_sb/nt_sbdsp/nt_sbmid/nt_sbmxr/nt_sbfm,
base/system/at_dma and Compat platform; tests include mixer and device contracts.
The coupled nt_sb.c edit restores only the original initialization signature;
its later frozen file is not marked audited. No function body, lifecycle branch,
command dispatch, guest state, audio API or device selection changes.

Verification correction: initial header-only BOOL restoration fails compiling
nt_sb.c due to unsigned-versus-signed prototype mismatch. That failed candidate
was not published or treated as necessary retention. After confirming runner
exit, the original provider signature and corresponding test declarations were
restored coherently. Final x86 build succeeds, but complete EXE comparison finds
non-metadata byte differences, so the artifact-equivalence exemption is rejected.
Disassembly confirms SbInitialize's generated register allocation/instructions
changed following the signed-to-original-unsigned return type restoration;
source branches and 0/1 returns are unchanged. Full dual-architecture runtime
qualification is required rather than asserting byte or runtime equivalence.

Scope/counts: production +63/-15 (net +48), tests +2/-2. Restoring original
text intentionally increases source lines while reducing mirror deviations.
With full-file difflib SequenceMatcher(autojunk=False), header XP diff falls from
+16/-60 to +2/-1 (73 fewer lines). The large mixed nt_sb.c alignment reports
+163/-777 before and +166/-777 after, despite exact signature restoration.
Combined heuristic change is 1016 to 946, 70 fewer diff lines. Fixed original
signature correspondence removes the old one-line replacement of four original
lines, i.e. five actual signature-diff lines; anchored reduction is 78 overall.
No new XP-relative location or semantic modification is introduced. Both metrics
are reported instead of mistaking heuristic realignment for restored code.

Final verification: both cached builds succeed and publish final-source EXEs.
Final successful x86/x64 build logs contain 89/267 warning lines, all within
preserved softpc.new. No non-mirror warnings occur. Complete qualification passes
392/392: x86 188/188 background (140.14 s) plus 8/8 desktop/native-audio (24.48 s);
x64 188/188 background (133.21 s) plus 8/8 desktop/native-audio (24.77 s).
The SB DMA, commands/framing, archive, mixer and platform-failure fixtures pass
on both architectures in these full runs. This includes current behavior tests,
not an original-bug negative replay or every real Windows guest audio workload.
Desktop was observed Active; no user intervention, guest/media/configuration
mutation or native session workaround was needed.

Final compiler stamps and EXE SHA-256:

- x86 Nov 13 2026 15:51:51:
  `b41bbaf225196cc2cc932959a72308a799e015a27fbf8b03240c0c0f1a1388a2`.
- x64 Nov 13 2026 15:53:05:
  `8724c86ca6d86108eb257771851237368ea3e0075cb7f57446aba805da21f743`.

Independent complete-header check removes exactly the two declared residuals
from a private in-memory comparison and obtains byte equality with normalized
pinned XP. No other header deviation is hidden in the comparison. Both local
manifest hashes are updated without changing raw/subset provenance hashes.
The coupled provider signature and test prototype diff have been reviewed;
public shared components remain unchanged. Frontier becomes 383/472, next file
384 base/inc/spcfile.h. R0448/R0449 remain evidence-qualified owner decisions;
S1 is not closed and later nt_sb.c whole-file audit remains outstanding.

P95 independent actual ten-file diff review and mirror/documentation/whitespace
gates pass. CIM confirms all task jobs exited; exact scratch preview, removal
and residue check pass. No unrelated worktree or user asset changes remain.

## Files 384-388: selected header identity and compiler EOF proof (P96)

Pinned XP SP1 remains a2f6d7c93aa4f11efc51147566d5961c59a8ef89. Full-file
comparisons normalize CRLF to LF only. Existing exclusion bodies and their
line scaffolding are skipped without new accounting. No source is edited.

| Order | File in base/inc | XP raw SHA-256 | Selected-content result and actual consumer |
| --- | --- | --- | --- |
| 384 | spcfile.h | 231bd47a01f69ea8cb30203be63045871e5f88d5686cbac7b680892b170e6bcc | Entire normalized file identical; production cmos.c includes it. |
| 385 | suplib.h | cc55070b5361e5401fd71a43d74859d3163154c5ea5b9f38a671cd4a4a895553 | Identical outside existing exclusions; host_inc.h includes it for production sascdef.c. |
| 386 | tape_io.h | 5dc7f546ac3e60e55e947c3314ccb5f60215cebd06d16ff69c96af6a3bb501d6 | One selected difference, R0451 below; actual BIOS tape_io.c and floppy implementation consume declarations. |
| 387 | timer.h | d8a61535bf3b189fa4939c9a01e088eb94849fd5b7ab39cdb0f0fbeaedd6b6dc | Identical outside existing exclusions; timer.c, video, CPU and Compat consumers retain original declarations. |
| 388 | timestmp.h | 8ddc2b6b7823ad249acb359dbf4bfcd77333c2cd13b5ce75f7f20d6ac231b4c0 | Entire normalized file identical; quick_ev.c and Compat platform.c include it. |

Both existing Ninja build trees contain production compile targets for cmos.c,
sascdef.c, tape_io.c, timer.c and quick_ev.c. These are not test-only headers.
No duplicate Compat implementation replaces any of these header declarations.

### R0451: tape_io.h original standalone Ctrl-Z

XP line 118 contains a standalone 0x1a byte after cassette_io's declaration;
the current file omits it. Selected XP-relative difference is +0/-1. Initial
analysis considered exact restoration rather than assuming EOF formatting was
automatically exempt from the original-first rule.

Dynamic counter-proof uses each architecture's actual Ninja tape_io.c GCC
command, removing dependency-output options and redirecting only the resulting
object. A private copy of the current header restores exactly the original
terminal Ctrl-Z line; a preceding include directory selects that copy. No
source branch, macro choice or function body is changed. Current-header
compilation exits 0 on x86 and x64. Restored-header compilation exits 1 on
both, with tape_io.h:118:1: error: stray '\32' in program. Both runs also
retain pre-existing mirror macro-redefinition warnings; these are not the
reason for the negative result.

Counter-review: this is the actual supported production translation unit,
not an artificial consumer or hypothetical parser incompatibility. Retaining
the existing deletion is evidence-qualified, pending owner decision; no new
workaround is proposed. It establishes compiler necessity, not new guest
runtime behavior. All other selected content matches pinned XP.

### P96 verification and delivery scope

Production +0/-0, tests +0/-0; no change to existing mirror diff totals or
published artifacts. Probe objects never replace cached production objects.
No full rebuild or runtime suite is claimed for this documentation-only
delivery; P95's final-source 392/392 remains the applicable runtime baseline.
Frontier advances to 388/472, next file 389 base/inc/timeval.h. R0451 remains
pending; the S and T remain active. Actual source comparisons and probe
construction/results were reviewed independently before ledger advancement.

P96 CIM inventory confirms the owned probe has exited. Exact scratch cleanup
preview, removal and residue check pass, as do documentation and whitespace
gates. The actual three-file documentation diff preserves frozen order and
initial hashes; no production, test, asset or unrelated changes are present.

## File 389: timeval.h original host-time contract (P97)

Pinned XP raw SHA-256 is
47909596ce2ff91bb96b86333efd13b639ef7a4240b6c8bbca14923eed7a7b2d.
Existing NTVDM exclusion is skipped. The only selected difference is R0452:
XP line 57 declares time_t host_time(time_t *); local lines 57-60 replaced
that with long host_time(long *) and three rationale-comment lines.

Initial necessity claim was that every reached caller required 32-bit seconds.
Actual caller inspection contradicts it: cmos.c sync_rtc_to_host_time stores
the result in time_t bintim, and time_day.c get_host_time uses time_t
SecsSince1970. Both pass that same type to host_localtime. Neither supplies
a non-null argument to host_time. The sole provider is Compat platform.c,
where an unnecessary (long)time(NULL) conversion created the narrower ABI.
There is no original device requirement for this narrowing. The similarly
named snapshot host_time array contains nine broken-down calendar fields,
not this function's return storage; snapshot schema and callers stay unchanged.

R0452 disposition: restore the exact XP declaration and remove the local
marker/comment; coherently change the existing native provider to
time_t host_time(time_t *location) { return time(location); }. This removes
the conversion and duplicated CRT pointer-write operation rather than moving
the mirror patch elsewhere. Native CRT remains the single wall-clock source.
No new interface, conditional, fallback, device state or shared-package change.

Independent counter-review distinguishes an artificially conflicting partial
header restoration from the coherent implementation. Preserving the current
Compat long signature would prove only that the two declarations disagree,
not that the mirror adaptation is necessary. Both real callers already use
the original time_t contract. Full final-header comparison leaves only the
previous exclusion/line scaffolding; no selected residual awaits approval.

Production +3/-9 (net -6): mirror +1/-4, Compat +2/-5. Selected XP-relative
diff drops from +4/-1 to zero, five fewer difference lines. The existing
platform_failure_smoke fixture adds 17 lines for an injected CRT time source
and return/writeback checks. A value 4294967297 is used when time_t exceeds
32 bits, otherwise 1234567890; NULL output and -1 propagation are checked.
The injection is reset before the pre-existing machine tests, which retain
the actual clock. No timing sleeps or wall-clock ordering assertions are used.
These checks do not claim complete guest calendar support beyond 2038 or
all native localtime failure behavior. Build/runtime qualification follows.

### P97 final-source qualification

Cached builds complete: x86 10.12 seconds; x64 13.33 seconds. Full background
regression passes 188/188 per architecture (140.36 / 140.26 seconds); serial
native desktop/audio tests pass 8/8 each (24.56 / 24.67 seconds), 392/392 total.
The updated platform-failure fixture passes on both architectures. Build logs
contain 623 x86 / 624 x64 warning diagnostics, all in preserved mirror paths;
none originate in Compat or test changes. These are not warning-free mirror
builds. No metadata-equivalence exemption was used; full tests ran.

Fresh published EXEs on Nov 13 2026:

- x86 16:09:15: dc81bfe41978046b34261943e3a808856707a3599ca01a451a2484a89242984c.
- x64 16:12:09: 5ce1558a09494d8d4891076a7252760b56b643a5638ebcc7c14110678b6a860f.

Current production calls and pointer types were scanned across src/test.
The mirror manifest updates only local identity; pinned raw/subset hashes
remain unchanged. Actual source review confirms no archive change, no shared
component change and no unrelated provider edits. This is source/call-chain,
controlled time injection and existing full-suite evidence, not new manual
DOS/Windows guest-date validation. Frontier advances to 389/472, next 390
tmstrobe.h; S1 remains active with earlier qualified owner decisions pending.

P97 actual ten-file diff review passes. CIM confirms task runner 11728 and
its jobs exited. Exact scratch preview/removal and residue check pass;
final mirror, documentation and whitespace gates pass. No user configuration,
media, snapshot or unrelated worktree modification is included.

## Files 390-393: original header whitespace restoration (P98)

Pinned XP SP1 remains a2f6d7c93aa4f11efc51147566d5961c59a8ef89. Existing
exclusions and their scaffolding are skipped without additional accounting.

| Order | base/inc file | XP raw SHA-256 | Result / production consumer |
| --- | --- | --- | --- |
| 390 | tmstrobe.h | 59ae1702556022757b133c494b2dcfb140b55efbed72daa5c70a8e939e368d3a | Normalized whole-file identity; cmos.c and timestrb.c. |
| 391 | trace.h | 6d772999cfb9162e2567989be13d4b040c3f09ec1df2972930affc071359ff6a | Selected-content identity; ica.c and other compiled original controllers. |
| 392 | vgaports.h | c9e1221af93b3bad79d396ea0c7c517ec9d826cb44245227153c4fb3d4d4a0de | R0453 restores three whitespace lines; actual ega_mode.c, v7_ports.c and host nt_munge.c include it. |
| 393 | vga_dac.h | d10f721bd10a3844c94398a51dc3587cfbcd97caf4023f35c95ec79ae6b9adad | Normalized whole-file identity; production vga_vide.c. |

### R0453: exact original selected whitespace

Initial selected difference: local vgaports.h lines 184, 436 and 453 had
removed trailing whitespace from the original EXTN_CONTROL not_used bitfield
line, the blank line opening struct crt_controller, and the blank line before
horizontal_total. No syntax, bit width, layout or register meaning changed.
No necessity supports this formatting deviation. Restore those exact bytes,
preserving CRLF, form-feed bytes, all conditional structures and #line values.

Independent full-file byte/line comparison after restoration leaves only
existing exclusion and line-control scaffolding. Comparisons use LF as the
line delimiter; form feeds must not be interpreted as additional source lines.
No original bitfield defect was found or changed. No Compat implementation
duplicates these declarations. Production +3/-3, tests +0/-0; selected
XP-relative difference +3/-3 becomes zero: six fewer difference lines,
not six fewer source lines. No new residual decision is required.

### P98 final-source verification

Cached x86 build completes in 9.29 seconds; x64 in 12.94 seconds. Both EXEs
are published with fresh Nov 13 2026 timestamps:

- x86 16:18:46: be153dcab45a872e17d6268b5a5599bc73ee6a235ab2de7bde94b78da1a813e1.
- x64 16:18:55: e739454bc2e3e2a7856cf4b4bb66e6de0baf5fee7f4099246d14c4064835f332.

Both complete EXEs equal their P97 predecessors after masking only PE
timestamp/checksum (PE+8/PE+88, four bytes each) and the eight-byte compiler
time at the unique unchanged date/time literal location. Lengths and PE
offsets also match. No other bytes are ignored. Under the owner's artifact
equivalence exception, no repeated full runtime suite is run; P97's 392/392
remains the full-runtime baseline. This batch establishes no new behavior.

Ordinary git diff --check reports exactly the three restored mirror trailing-
whitespace lines. They match pinned XP and are intentional original-first
restoration, not newly invented formatting. The remaining tracked diff must
pass the ordinary whitespace check. Mirror identity gate passes. Frontier
advances to 393/472, next file 394 video.h; later dependency reads do not
constitute completed file audits. S1 and qualified earlier decisions stay open.

P98 actual eight-file diff review is complete. Documentation gate and the
non-mirror whitespace check pass; the three exact-XP whitespace diagnostics
are explicitly accounted for above. CIM confirms all owned jobs exited;
scratch preview, cleanup and residue check pass. No user assets or unrelated
changes are included.

## Files 394-397: video stream declarations and remaining headers (P99)

Pinned XP SP1 remains a2f6d7c93aa4f11efc51147566d5961c59a8ef89. Existing
excluded bodies and their conditional/line-control scaffolding, including
adjacent scaffolding spacing, are skipped without additional accounting.

| Order | base/inc file | XP raw SHA-256 | Result / actual consumer |
| --- | --- | --- | --- |
| 394 | video.h | 72fda2adc7a26d137b7ca5b4ac03d35712e4bdcd628d56c633faacc391597257 | R0454 reduces the selected patch; R0455 qualifies four remaining declarations. Original video.c and ega_vide.c consume the interface. |
| 395 | virtual.h | 57ae800ca06293867949f1e7bc6130ba8a93f226c95a111956933bdeff19a21e | Normalized whole-file XP identity; production reset.c. |
| 396 | xt.h | bc57c977dfd0ba3262fa8aef0a55ce6bab8ce8d1255dfff57dd8587299cfa02e | Identity outside existing exclusions; actual BIOS, controllers and Compat use the original definitions. |
| 397 | yoda.h | 73dd5141b0b8e3f9ff139283ee6dd78c5ecc1d5b51cff46a6437c0154ee955ff | Identity outside existing exclusions; production ica.c includes it. |

### R0454: remove nonessential selected comment and restore declaration spelling

Remove video.h's three-line detached-presentation comment immediately before
the added stream declarations, and restore the original two spaces between
boolean and stream_io_enabled. The explanation is captured in R0455 rather
than retained as unnecessary mirror text. Final declarations use exact XP
spelling. Production +1/-4, net -3; tests +0/-0. Selected patch +7/-0 becomes
+4/-0, a three-line mirror-diff reduction. Existing line scaffolding is unchanged.

### R0455: stream I/O declarations outside NTVDM

Original XP video.h lines 352-355 declare stream_io_buffer,
stream_io_dirty_count_ptr, stream_io_buffer_size and stream_io_enabled inside
NTVDM. Current final lines 294-297 expose those four declarations to the
selected non-NTVDM build. Initial analysis questioned whether this was merely
an adapter-created dependency that should disappear with coherent restoration.

Source evidence contradicts that interpretation: pinned XP video.c lines
2751-2756 reference all four unconditionally inside vd_write_teletype, after
the earlier NTVDM local-declaration block has ended. The original function is
registered in video.c and ega_vide.c service tables and called by the original
write-string implementation. Current selected code preserves that block.
Compat video.c supplies disabled initial state (FALSE, NULL pointers, zero
capacity); whole-src/test symbol search finds no subsequent enable assignment.
This is not a second display implementation, nor a working stream backend.

Actual dual-architecture countercompilation uses each Ninja production
video.c command with only dependency/object outputs redirected. A private
header copy removes these four added declarations, leaving all existing
conditional choices and original consumer code unchanged. Current-header
compiles exit 0 on x86/x64; restored declaration visibility exits 1 on each:
stream_io_enabled, stream_io_dirty_count_ptr, stream_io_buffer_size and
stream_io_buffer are each undeclared at original lines 2751/2752/2754.

Independent counter-review: a header-only compile failure would not suffice
if a coherent original caller restoration removed the references. Here the
references themselves are original XP selected code. Deleting that original
block would add a new mirror behavior difference; moving the same declarations
to another header would merely relocate the adaptation. Neither is a justified
original-first reduction. Four original-spelling declarations remain the
minimal existing patch, evidence-qualified and pending owner decision, not
automatically approved. This proves compilation need, not live stream-I/O
behavior, and does not complete the later video.c whole-file audit.

### P99 qualification

Final-source cached builds complete in 15.62 seconds (x86) and 19.06 seconds
(x64). Fresh published EXEs on Nov 13 2026:

- x86 16:22:39: 28905a8241a2b2422597c2db0f7d97a2d897e292ad1a15bdf1456dfea0676944.
- x64 16:22:54: f5d338e1dfb98e67c82258fd8b9183e4ad140f85418faa2c87fd0105e03c5024.

Each full EXE matches P98 after masking only PE timestamp/checksum and eight
compiler-time bytes at the same unique date/time literal. Lengths and PE
offsets match; no code/data bytes are otherwise ignored. The authorized
equivalence exception applies: no repeated runtime suite; P97's 392/392
remains the full-runtime baseline. The compile counterprobe is separate from
that baseline and never overwrote production objects. Frontier is 397/472;
next file 398 base/keymouse/keyba.c. S1 and earlier qualified decisions remain
open; no whole-task completion is claimed.

P99 actual eight-file diff review and mirror/documentation/whitespace gates
pass. CIM confirms probe and build jobs exited; exact scratch preview,
cleanup and residue check pass. Configuration, guest assets and snapshots
are untouched. Frozen initial hashes and file order are preserved.

## File 398: keyba.c keyboard behavior and archival (P100)

Pinned XP raw SHA-256:
6096d9f805d832c0f939e511e0f2d56e37a4eee48187376b6c6465844923a5a2.
The file is compiled in both production device targets. Existing exclusions,
conditional/line-control scaffolding and adjacent scaffold spacing are skipped.
The selected differences are reconciled below; later keyboard files are not
implicitly audited.

| Point | Initial local / XP anchor | Disposition |
| --- | --- | --- |
| R0456 | Local 46 / original SCCSID notice at XP 47 | Original notice retained outside the excluded SCCSID body under source policy; +1/-0, qualified pending owner choice. |
| R0457 | Local include 81 and implementation beginning 2349 / XP before 83 and 3099 | Existing snapshot declaration dependency and keyboard state/callback codecs; qualified pending owner choice. Implementation span is 118 physical lines including separators, plus the include. |
| R0458 | Local 2087 / XP before 2791 | code_to_send=input_port_val; +1/-0, qualified by actual command-path negative controls. |
| R0459 | Initial local 2514-2516 / XP 3173-3174 | Restore the original IRQ comment and following blank; delete the three-line replacement explanation. No behavior change. |
| R0460 | Initial local 2517 / original call inside JOKER at XP 3176 | Existing IRQ1 clear remains in selected read-port path; one selected call, qualified by actual controller/PIC negative controls. |

### Actual supported behavior, rather than historical retention claims

For R0458, original cmd_to_8042 declares automatic code_to_send without an
initializer. Its C0 case marks the value valid without assigning the input
port value. Current one-line assignment fills that missing value; other
command cases retain original assignments and dispatch. The real bop_smoke
fixture sends C0 to the actual 8042 and reads port 60, requiring the initialized
input-port value BFh. This is not a test-only implementation of the controller.

For R0460, original selected kbd_inb consumes output_contents and clears
output_full/status but the explicit PIC clear resides only under JOKER. In
the actual non-JOKER configuration, a masked IRQ1 can remain pending after
the byte is consumed. irq_smoke sends the keyboard identification command,
observes IRR bit 1, consumes FAh from port 60, then requires bit 1 to clear.
The retained call uses the existing PIC clear operation, not a synthesized
EOI or a second keyboard queue. The explanatory comment need not differ
from XP, so R0459 restores its original text and blank line.

Private counterprobes compile the actual keyba.c with its Ninja compiler
flags, replacing only the source/object output paths. Each resulting explicit
object is linked before the existing archive into existing real test fixtures;
cached production objects and archives are never overwritten. Current-body
bop/irq/checkpoint tests exit 0 on both architectures. Independently removing
only C0 assignment fails bop_smoke line 90 (input_port == 0xbf); removing
only the selected IRQ clear fails irq_smoke line 79 ((irr & 0x02) == 0).
Both x86 negatives exit 3221226505; both x64 negatives exit 3. Assertion
messages establish the intended failures, not merely nonzero exit status.
These four negative controls prove bounded command/PIC regressions, not that
the patches explain a particular historical DOS/Windows user incident.

### Snapshot necessity and limits (R0457)

Production ownership is Machine snapshot_image -> CCPU archive -> device
archive -> keyboard capture/restore and callback codecs. device archive's
keyboard_fields serializes the captured fields, including FIFO, held keys,
scan-set/key-type arrays, command/response latches, modifiers/repeat counters,
output/status and refill handle. Original keyba.c keeps the FIFO indices/data,
key-type arrays and make/break lookup state private. Original port commands
cannot enumerate or restore these states without consuming input, scheduling
new work or changing the controller. Removing the archive hook therefore
cannot be replaced by an equivalent replay through original public commands.

The two encoded callbacks are real original delayed do_int and allowRefill
entries. Numeric IDs reconstruct their addresses rather than persisting host
pointers. The anomalous sequence pointer is encoded as an original break-table
index and resolved after init_key_arrays selects the restored scan set. FIFO
free capacity is reconstructed from count; temporary translation scratch is
cleared, not persisted as a host pointer. Capture happens at the existing
executor safe boundary, not midway through synchronous scan-code construction.
No duplicate keyboard device lives in Compat.

The real checkpoint fixture captures pending FIFO/command state, mutates via
ports, restores and compares a fresh semantic capture; rejects FIFO count 49;
checks anomalous pointer encoding and restores a queued keyboard interrupt
through the device/event archive. irq_smoke also restores scan sets 1-3 and
checks actual make/break paths. These pass on both architectures. This does
not claim every malformed field or every allowRefill timing was dynamically
enumerated. Restore is not a transactional standalone device API: it may
select key arrays before a later validation fails. Production staged-image
restoration resets the machine and returns failure without resuming guest
execution if any device restore fails; no rollback guarantee is invented.
Necessity is established by private-state ownership and original API limits,
with these focused dynamic checks, not by link references alone.

### P100 restoration and final verification

Production +2/-3 (net -1); tests +0/-0. Exact XP comparison using LF-delimited
SequenceMatcher with autojunk disabled shows added-diff delta -3 and
deleted-diff delta -2: five fewer mirror difference lines. Existing exclusions
are unchanged and not newly counted. R0456/R0457/R0458/R0460 remain qualified
owner decisions; R0459 is restored, not retained as a patch.

Final cached builds complete in 8.70 seconds x86 / 11.81 seconds x64. Final
bop/irq/checkpoint tests pass 3/3 each. Fresh EXEs on Nov 13 2026:

- x86 16:29:14: ec10a6f5027323a979cbbb3dff0e842ba8943f73b5cca47d1e97a178009a5e62.
- x64 16:29:25: 1b9dc218a5b3f5df4a0ae92194ebc77b063cc286ac85bc5e4b94eb65991fe8e7.

Both entire EXEs match P99 after masking only PE timestamp/checksum and
eight compiler-time bytes at the same unique date/time literal. No other
bytes are ignored; lengths and PE offsets match. No repeated full suite under
the authorized equivalence exception; P97's 392/392 remains the full-runtime
baseline. Frontier advances to 398/472, next 399 base/keymouse/keybd_io.c.

P100 actual eight-file change review, mirror/documentation/whitespace gates
and cleanup residue check pass. CIM confirms all owned probes/build/tests
exited before exact scratch removal. No user configuration, media, snapshot
or unrelated changes are included. S1 remains active.


## File 399: keybd_io.c (P101)

Pinned XP raw SHA-256
c0e79e4d4015be8a57ecfcc8e335e44c54ee497c9b634e5d85fc1f4332c999e2.
Full LF-delimited correspondence against the pinned XP file was inspected,
including deletion anchors that SequenceMatcher separated from their existing
#error replacement. Those deletions belong to the existing NTVDM/MONITOR
and final NTVDM excluded bodies, not omitted selected implementation.
Existing exclusions, line scaffolding and adjacent scaffold spacing are
skipped without new bookkeeping.

R0461: local line 49 preserves the original Insignia notice from XP line 50
outside the excluded SCCSID body. It adds one comment line and no runtime
behavior. Source policy requires preserving historical notices; removing this
line would remove that notice from the retained source. This is an
evidence-qualified owner decision, not automatic approval.

All remaining selected implementation is unchanged from XP. CMakeLists.txt
line 191 includes this file, both persistent Ninja trees contain its real
softpc-machine-devices object rule, and BIOS reset.c calls keyboard_init at
line 788. It is not a test-only or snapshot-only file. No duplicate Compat
implementation or replacement is introduced by this review.

Independent counter-review rechecked the complete non-equal ranges and their
original/current surrounding conditions. No selected semantic restoration is
needed; no unused-file retirement is justified. Source +0/-0, tests +0/-0,
XP-relative diff change zero. This delivery changes only audit documentation;
no build, publication or runtime test is claimed. P100 artifacts and P97's
392/392 runtime baseline remain unchanged. Frontier 399/472; next file 400
base/keymouse/mouse.c. Qualified owner decisions remain open; S1 is active.


## File 400: mouse.c (P102)

Pinned XP raw SHA-256
d449af5953821629584958ecb29217ac7114b246169d4d0e6526c6ed2f67ec64.
Complete LF-delimited correspondence includes original burst-comment/body
removals as selected differences; they are not hidden among excluded bodies.
Existing #error exclusions and line scaffolding receive no new accounting.

| Point | Original/current anchors | Disposition |
| --- | --- | --- |
| R0462 | XP SCCSID notice / local 30 | Preserved historical notice, +1 comment line; source-policy qualified owner decision. |
| R0463 | XP 104-106, 369-405, 420, 434, 456, 512 / local quick_ev include, 97, 102-120, mode/send/reset timer wiring | Original finite burst replaced by one existing quick-event timer and enable/HOLD-aware IRQ delivery; qualified owner decision. |
| R0464 | XP insertion before mouse_inb / local snapshot include, 122-187 | Private InPort state and callback archival; qualified owner decision. |
| R0465 | XP mouse_inb entry / local 190 | +1 default-return assignment prevents uninitialized CPU input; qualified owner decision. |
| R0466 | XP reset and mouse_init / local 389-399, 473-475 | Clear stale movement/latches and initialize cold controller state; qualified owner decision. |
| R0467 | Three added comment sites before timer declaration, rescheduling and cold init | Four nonsemantic lines removed exactly; no replacement commentary or code. |

### Initial analysis and actual consumers

The actual Machine mouse ingress calls mouse_send at machine.c:280. BIOS
reset calls q_event_init at reset.c:631 and mouse_init at 926. mouse_init
registers the original adapter handlers for ports 23Ch-23Fh. No parallel
Compat device implementation is substituted.

R0463 source proof: XP explicitly uses loadsainterrupts=5, decrements it on
eligible mode writes, and queues 100 PIC interrupts each time. It neither
schedules the selected 30/50/100/200 Hz period nor recurs after the five bursts.
The original comment itself describes this Windows-specific workaround.
Coherent restoration would remove the single quick-event timer and its
associated snapshot callback/handle, restoring finite bursts, not an
alternative functioning periodic source. Current timer tick clears its
consumed handle, tests enabled rate, suppresses IRQ during HOLD, then schedules
one next event. HOLD-only writes preserve that handle; disabling/rate changes
cancel it. Data IRQ requires mode bit 3; constant timer-high requires timer
enable bit 4. Restoring original unconditional mouse_send/constant-high IRQ
would generate interrupts even when the respective source is disabled.
These are concrete source/contract differences, not a claim that every old
Windows driver fails. No new driver-specific branch is justified.

R0464: archive.c captures/restores the controller and serializes all thirteen
fields, including selected register, alternating ID phase, held data/status,
current and previous buttons, accumulated deltas, mode and timer handle. Its
callback codec connects the actual recurring quick event to the existing
queue archive. Original port reads cannot enumerate this private state
without changing ID phase/selected register; HOLD writes latch and clear
movement, while mode writes schedule interrupts. Replaying ports cannot
replace exact state restoration without side effects. Existing original
ports therefore do not provide a coherent substitute for this extension.

R0465: mouse_inb originally leaves the output untouched for unsupported
internal register selections and other registered ports. The actual CCPU
IN8 in ccpu386/in.c declares an uninitialized IU8 temp, calls the registered
handler and assigns temp to its destination. support/ios.c inb also forwards
the caller pointer without a default. Original restoration thus permits an
indeterminate guest result; the one assignment is a bounded default, not
another controller path. This is source proof, not a fresh historical guest
reproduction or a claim about all bus electrical behavior.

R0466: original reset clears only mode, burst count, address and IRQ. Pending
movement and held data survive it. Current reset also clears accumulated and
latched data and sets last buttons to the current buttons; cold initialization
clears buttons and discards the obsolete queue handle before reset. Because
q_event_init already ran, deleting an old handle during cold init could act
on a newly allocated queue entry. No second reset owner or event loop exists.

### Independent counter-review and verification

Counter-review considered restoring the complete timer/burst design rather
than deleting isolated scheduling statements and blaming broken references.
The original finite burst cannot satisfy recurring rate-selected interrupts;
retention is evidence-qualified but still awaits the owner's decision.
Snapshot retention rests on private state and original port side effects,
not merely on existing adapter calls. Nonsemantic explanations are removed.

Existing actual-controller mouse and checkpoint tests pass 2/2 on each
architecture before and after the comment restoration. They exercise guest
IRQ delivery through CCPU/PIC, movement latching, four scheduled periods,
recurrence across HOLD, timer cancellation, queue/controller archival and
restored register reads. Checkpoint verifies held data, selected register and
invalid ID-phase rejection. The first attempted filter selected no tests;
that invocation is not counted. Correct path-qualified filters selected the
four reported tests. No new permanent fixture or production test API is added.

Coverage limits: this batch does not repeat real Windows sessions, inject
quick-event allocation failure, or enumerate every malformed snapshot field.
Default invalid-port initialization and the cold-reset ordering are established
by the concrete source chain above, not newly claimed dynamic negative tests.

Production +0/-4, tests +0/-0; XP-relative added-diff delta -4 and deleted-diff
delta 0: four fewer mirror difference lines. Final cached builds succeed in
9.79 seconds x86 and 13.35 seconds x64. Fresh Nov 13 2026 EXEs:

- x86 16:39:26: 0cd080f36f9a0aa0487ad1e98efa2b86b58d47e87c8bf7b785a6479dd4a7a374.
- x64 16:39:37: a69dd8b015309179d3f9643e80d031d352a8d457d433e543b489e2ec02aa8bf9.

Entire files equal P100 after masking only PE timestamp/checksum and eight
compiler-time bytes in the unique same-position Nov 13 literal. No other
bytes are ignored. Final focused tests pass 4/4; full tests are not repeated
under the owner's artifact-equivalence exception. P97's 392/392 remains the
full runtime baseline. Frontier 400/472; next 401 base/keymouse/ppi.c.
S1 remains active, with R0462-R0466 qualified pending owner decisions.

P102 final actual eight-file diff review, mirror/documentation/whitespace
gates and residue check pass. CIM confirms both test/build jobs exited before
exact scratch cleanup. No user configuration, media, snapshot or unrelated
change is included. Full-file SequenceMatcher confirms the four-line added-
diff reduction with unchanged deleted-diff count; exclusions were not changed.


## File 401: ppi.c (P103)

Pinned XP raw SHA-256
e0c63cb92be4c82da7037f1263bf897e74c4089ee5b9806162389bec800dfe5a.
Complete original/local correspondence inspected; existing #error bodies and
line scaffolding are skipped without additional accounting. No production
restoration is justified by this file review.

| Point | XP/current anchors | Evidence-qualified disposition |
| --- | --- | --- |
| R0468 | XP SCCSID notice / local 17 | One historical notice comment preserved outside exclusion; source policy, pending owner decision. |
| R0469 | XP SPKRDATA state and 140-150, 170 / local 69-74, 144-147, 166 | Remove duplicate speaker-edge state/legacy calls and select original post-gate HostPpiState; pending owner decision. |
| R0470 | XP ppi_init after register assignment / local 199 | One gate_2_was_low reset assignment; deterministic dual-architecture negative controls, pending owner decision. |
| R0471 | XP snapshot additions absent / local include 46 and 204-229 | Private PPI state capture/restore, pending owner decision. |

### Initial source proof and coherent-restoration review

Root CMake compiles both ppi.c (line 189) and original host/src/nt_sound.c
(line 221). BIOS reset.c:724 calls ppi_init; registered port 61h writes invoke
ppi_outb. Original nt_sound.c HostPpiState consumes both bit 0 (T2State) and
bit 1 (PpiState); PlaySound requires the correct combination and FreqT2.
The selected original timer_gate runs before this full-state notification.

The legacy non-NTVDM branch observes only changes to bit 1 and passes no
port value to host_enable_timer2_sound/host_disable_timer2_sound. Restoring
that whole branch would lose independent bit-0 updates to the actual original
host sound owner. An implementation of those callbacks cannot recover the
missing transitions merely by translating its no-argument call. Reimplementing
this state elsewhere would duplicate original HostPpiState rather than restore
one original path. Compat audio.c's remaining disable wrapper is a BIOS reset
sink and clears pending onset; it is not another PPI port interpreter. The
SOFTPC_STANDALONE condition is an existing host-selection difference, not a new
product-specific branch added by this audit.

For R0470, static initialization only establishes the first process lifetime's
gate state. A prior port-61h write with bit 0 high leaves gate_2_was_low false;
original ppi_init clears the register but not that history. A subsequent high
write then misses the required timer_gate rising transition. Resetting the
history alongside the register is therefore a supported repeated-reset fix.

For R0471, archive.c:159/228 calls the capture/restore pair and its wire map
483-486 stores all three fields. ppi_inb toggles bits 4/5 before returning;
ppi_outb masks written state to four bits and triggers timer and sound edges.
The public ports cannot inspect/restore the private register phase and gate
history without mutation. Direct archival restores them and notifies the same
original HostPpiState owner. The derived speaker-data field is part of the
existing serialized snapshot contract; this audit does not silently change it.

### Dynamic proof, counter-review and delivery

Existing machine sound, checkpoint and original sound-state tests pass 3/3 on
each architecture, six total. Their mock output boundary does not prove native
audio-device availability. The actual source-chain coverage includes CCPU port
writes, original PPI/PIT state, sound delivery and snapshot roundtrip.

A private PPI object differs only by removing the added ppi_init gate reset.
It is linked before the normal device archive, without changing persistent
production objects or archives. Both original sound tests then fail at the
fast-frame-count assertion, but that timing-bearing result alone is not used
as causal proof. A disposable version of the existing checkpoint fixture adds
ppi_init after its real high-gate state restore, then captures the state and
asserts register zero and gate_2_was_low==1. Actual current PPI passes on x86
and x64. The single-line-restored objects both fail exactly the gate assertion
(line 714): x86 exit 3221226505, x64 exit 3. All private compiles and links pass.
No fixed sleep is used for this direct-state comparison.

Independent counter-review verified the complete original host-call choice,
not an artificially unresolved-link restoration. Existing snapshot consumers
alone do not establish necessity; the side effects of the original port API
above do. No Compat state machine, helper or public API is added.

No new Windows guest session, native-device playback or exhaustive malformed
snapshot validation is claimed. Production +0/-0, tests +0/-0, mirror diff
delta zero. No production build or republication is required for this read-only
source audit; P102 dual EXEs remain unchanged. P97's 392/392 remains the full
runtime baseline. Frontier 401/472; next 402 base/support/ios.c. S1 remains
active and R0468-R0471 await owner disposition.


## File 402: ios.c (P104)

Pinned XP raw SHA-256
b7a07d41a99d14c6d8444135c1f5230e0bfd283f15b973be7afbfb8b8c5ddbbc.
Complete original/local correspondence and every replaced-range boundary were
reviewed. Existing #error bodies are skipped without new bookkeeping. The
final GetExtIo adapter deletions belong to the existing NTVDM exclusion;
they are not missing selected dispatcher functions.

R0472: XP lines 196-199 place the empty-port byte assignment inside an
unconditional block, with its original default-value comment. Current source
had omitted the opening brace, comment and closing brace. Restore those three
original lines exactly and adjust the adjacent line-control marker from 198
to 196. The selected path neither declares block-local variables nor jumps
across this block; restoration changes no expression, return value or call.
The NTVDM exclusion is unchanged.

Initial analysis and independent counter-review agree that this is a genuine
selected-body difference, not excluded content to skip. Restoring it removes
an unnecessary adaptation. All remaining selected code matches XP. CMakeLists
line 163 includes the file, BIOS reset.c:692 calls io_init, and actual CCPU
in.c/out.c dispatch through its adapter tables. No unused-file deletion,
Compat duplicate or additional virtualized-I/O path is introduced.

Production +4/-1 (net +3), including the line-control replacement; tests +0/-0.
Exact LF-delimited SequenceMatcher comparison with autojunk disabled shows
XP-relative added-diff unchanged and deleted-diff reduced by three lines.
No exclusion body is changed or newly counted. There is no residual selected
patch requiring an owner decision for this file.

Final cached builds pass in 10.30 seconds x86 / 11.31 seconds x64 and publish
both EXEs. Their unique Nov 13 2026 compiler-time literals are:

- x86 16:48:28: 177b8ac8e89a642d59de593f05052eb82dfb56b436aff09841bb8c0e2f5d6010.
- x64 16:48:39: bcba006d00ce63e42f2b16eb61c5f615ffd23f6b68d06ee827b182f0fdd9c9aa.

Entire EXEs match P102 after masking only PE timestamp/checksum and eight
compiler-time bytes at the unique same-position date/time literal. No other
bytes or length changes are ignored. No runtime tests are repeated under the
owner's artifact-equivalence exception; P97's 392/392 remains the full runtime
baseline. This is build/artifact equivalence evidence, not a new guest session.
Frontier 402/472; next 403 base/support/time_day.c. S1 remains active.


## File 403: time_day.c (P105)

Pinned XP raw SHA-256
64bb35da9f3e9b66a3660f9042adcb9be676641839955406930c2fffdad0d7a4.
Complete non-equal ranges and their boundaries were inspected. The apparent
final UpDateInProgress deletion is within the existing NTVDM exclusion;
existing #error bodies and line scaffolding receive no new bookkeeping.

R0473: local line 45 preserves the original notice from XP line 48 outside
the excluded SCCSID body. One comment line, no executable change; source
policy establishes its preservation reason. Qualified pending owner decision.
All other selected implementation matches XP, with no reducible adaptation.

Actual consumers: root CMakeLists.txt:175 builds time_day.c, reset.c:783 calls
time_of_day_init, and compat/device_bop.c:84 installs time_of_day as the BIOS
time service. Original get_host_time uses time_t and host_localtime; the
coherent original host_time interface was restored in P97 and remains time_t
in Compat. This is original BIOS behavior plus host time binding, not a
standalone duplicate or a test-only file.

Independent counter-review checked every replacement boundary, including
selected branches adjacent to excluded NTVDM/PROD alternatives. No selected
brace, declaration, timestamp calculation or time-service body was omitted.
Production +0/-0, tests +0/-0, XP-relative diff delta zero. This is a source
and consumer audit; no new runtime test or build is claimed. P104 published
EXEs and P97's 392/392 runtime baseline remain unchanged. Frontier 403/472;
next 404 base/system/at_dma.c. S1 remains active, with owner decisions open.


## File 404: at_dma.c (P106)

Pinned XP raw SHA-256
d756863ccac611e83298aaf22aa74c5a2db18a499e631b64a0b0060cffecbd3a.
Complete diff and replaced-range boundaries were inspected, including selected
content adjacent to existing exclusions. The moved dmaGetAdaptor body is a
real local host-selection difference, not hidden by the NTVDM exclusion.
Existing #error bodies and line scaffolding are not newly accounted.

| Point | Original/current difference | Disposition |
| --- | --- | --- |
| R0474 | sndblst.h moved before debug.h | Exact XP include order restored. |
| R0475 | GLOBAL DMA_ADAPT adaptor changed to static | Exact original GLOBAL declaration restored; full-src search finds no conflicting global definition. |
| R0476 | Two channel-position query conditions | Correct decoded-channel comparison remains; restore original braces, statement indentation and operand ordering around the necessary condition change. Qualified owner decision. |
| R0477 | Decrement wrap boundary omits offset zero | Existing two +1 corrections remain, qualified by actual transfer negative controls. |
| R0478 | Decrement copy helpers and their size arguments | Existing unit-aware reverse guest/forward host transfer remains, qualified by actual transfer negative controls. |
| R0479 | Original dmaGetAdaptor exposed outside NTVDM | Existing original accessor body remains for actual DSP initialization; qualified owner decision. |
| R0480 | DMA snapshot include and capture/restore extension | Existing state serialization remains; qualified owner decision. |
| R0481 | Five added explanatory comment lines | Removed without replacement; exact original text retained elsewhere. |

### Actual paths and source proof

Compat gfi_image.c calls dma_request for actual diskette data/ID transfers.
The original controller owns its current/base address/count, mask, mode,
request, page and flip-flop state. No Compat DMA engine replaces it. The
original sound DSP initializes its controller pointer through dmaGetAdaptor
(nt_sbdsp.c:734), then QueryDMA/SetDMACountAddr operate on that same controller.
Removing the accessor's selected definition while preserving the admitted
original DSP would remove its original state access. Adding another adapter
or changing DSP access merely relocates the same difference and is not used.

R0476: SbDmaChannel is a channel number; byte-controller address/count ports
are respectively 2*channel and 2*channel+1. XP compares those port numbers to
channel and channel+2. Channel 1 address port 2 therefore does not trigger
its needed progress sample. The existing correction compares the same decoded
channel used immediately afterward to index current_address/current_count;
the first_last check samples only once for the two-byte read. No second sound
position authority or new polling path is introduced.

R0477: with decrementing unit offsets, offset zero remains transferable before
wrapping. The first segment contains offset+1 units, not offset units. This
holds for either byte or word controller because dma_request selects the unit
size before do_transfer scales it to bytes.

R0478: original do_transfer explicitly documents that guest address decreases
but device buffer always advances. Original read helper instead increments
guest source and decrements the device destination; the write helper decreases
individual bytes, not whole word-channel units. Current helpers use existing
SAS physical range access, decrement guest by size, advance host by size and
preserve bytes inside each unit. The declarations and callers carry the same
existing do_transfer size; no alternate transfer path is added. Original XP
auto-init address/count restoration is already present, not another local fix.

R0480: archive.c captures/restores both controllers and page state through the
existing extension. Ports cannot enumerate base versus current versus temporary
register arrays, and reading status/bytes mutates status or first_last. Port
replay would not restore all in-flight state. The original GLOBAL declaration
does not justify moving this equal serialization logic to Compat; it stays
beside its existing state owner. Restore validates first_last per controller,
not an atomic rollback of a malformed second controller; the actual Machine
restore failure does not resume guest execution. No broader transactional API
claim is made.

### Independent counter-review and negative controls

All three private negative variants compile and link actual DMA objects before
the existing device archive, with private fixture working directories. No
persistent object or library is overwritten. Both architectures fail at the
same contract-specific assertion:

- Restore original two query conditions: device_contract sb-dma, line 1230,
  DspNumberDmaQueries == 1 fails.
- Restore original two decrement-wrap expressions: device_contract dma,
  line 1415, actual memory differs from independently calculated expected data.
- Restore original helper signatures/call arity and both helper bodies as one
  coherent unit: device_contract dma, line 1488, returned data differs.

x86 negative exits are 3221226505; x64 exits are 3. Compile/link all return zero.
An earlier probe omitted the required mode argument and over-replaced a helper
boundary; those harness failures were corrected and are not defect evidence.
The retained tests independently cover both controllers, increment/decrement,
read/write/verify, five start offsets, guards, terminal count, partial transfer,
ROM write protection and auto-init/non-auto-init. Sound-port tests cover
channels 1/3, address/count low/high reads, other channels and disabled DSP.
Checkpoint tests cover base address/count and page roundtrip, not every invalid
archive field or all real guest/device timing.

Counter-review restores unnecessary visibility/formatting changes rather than
using historical approval as proof. Necessary transfer semantics are not
rewritten; new defects or expanded device capability are not introduced.
Final verification and delivery evidence follows below.


### P106 final verification and diff accounting

Production +8/-11, net -3; tests +0/-0. Exact XP comparison with LF delimiters
and SequenceMatcher autojunk disabled shows added-diff delta -9 and deleted-
diff delta -6: fifteen fewer mirror diff lines. Existing exclusions are
unchanged; no new source adapter or helper is added. Restored original GLOBAL
visibility and source syntax produced non-metadata EXE differences, so the
artifact-equivalence exemption was not used for that delivery build.

Full regression passes on both architectures: 188/188 background tests plus
8/8 desktop/native-audio tests each, total 392/392. Background elapsed times
are 135.97 seconds x86 and 137.21 seconds x64; native sets take 25.05 and
23.50 seconds. No detached-session failure or skipped native test is hidden.

The final fifth comment removal was made after both architecture builds had
finished. A final cached rebuild/publication passes in 9.14 seconds x86 and
12.87 seconds x64. These final whole EXEs match the corresponding full-tested
EXEs after masking only PE timestamp/checksum and eight time bytes in the
unique same-position Nov 13 date/time literal. Final DMA, SB-DMA and checkpoint
regressions also pass 3/3 each (6/6). Final published Nov 13 2026 artifacts:

- x86 16:59:30: b0d23c01cc85f04b39c864c006f02c78fafcf390ee9f211b1520e23b01b75d8c.
- x64 16:59:41: c3a5a57fb30fccd81d59c19e564acec52e7248ae591c3d49695ddc19eb51b0e4.

Mirror CRLF and manifest identity are preserved. No guest, configuration or
snapshot is modified. P106 becomes the full runtime/artifact baseline.
Frontier advances to 404/472, next 405 base/system/cmos.c. R0476-R0480 remain
qualified owner decisions, not automatic approvals; S1 remains active.

P106 actual eight-file review, mirror/documentation/whitespace gates and
cleanup residue check pass. CIM confirms all owned build/test/probe processes
exited before exact task-scratch removal. No unrelated worktree changes are
included; the frozen initial hashes and file order are unchanged.


## File 405: base/system/cmos.c (P107)

Pinned XP raw SHA-256 ffcc54503743e559d4b6180f5c3a6215fb6d4ea140a466b66e5e40b87bde7647.
The complete selected-file comparison checks all differing ranges, including
selected edges adjacent to excluded bodies; existing exclusions are not counted.
CMakeLists.txt compiles this controller into the original device group. Original
reset.c calls cmos_post; no independent Compat CMOS register owner exists.

- R0482, current include line 40 and disk selection at 963, XP 1041-1044:
  the added cmos/port.h and replacement disk selection report attached HDDs.
  Original unconditionally declares C: type 3, adding D: type 4 when configured.
  Actual config_inquire already returns the mounted paths, but its original
  caller never asks whether C: exists. The FDC-only production configuration
  has no HDD; this is unrelated to the always-present A: drive without media.
  Coherent original disk selection in the real controller, linked into the
  existing fdc_smoke fixture, fails CMOS_DISK == 0 on x86 and x64. The current
  fixture passes, and dual_media_smoke checks C: 0x30. Residual source scope is
  one include plus one replacement line against four original lines (+2/-4).
  Counter-review: this is host media presence, not duplicate CMOS state. Moving
  original disk policy into another new path would not reduce complexity.
  Qualified pending owner decision; no new patch is added. D-only topology is
  source-reviewed, not dynamically claimed by these fixtures.
- R0483, snapshot include at 49 and 118-line extension after selected source:
  archive.c captures/restores the private CMOS bytes, selected index, conversion
  mode, clock state and periodic event handle, and maps three actual queued RTC
  callbacks. Their original owners are static in this file. Reading register C
  acknowledges interrupt state; ports neither enumerate the private tick count
  and callback handle nor safely reconstruct a suspended queue. Removing these
  methods leaves real archive consumers unlinked; replacing them with port
  replay does not preserve the same state. Moving equal serialization into
  Compat by exposing private globals is not a reduction. Qualified source/ABI
  necessity, pending owner decision. Existing checkpoint_smoke verifies a CMOS
  byte across capture/mutation/restore; it does not exhaust RTC phase/mode
  combinations. Restore is not a transactional public API: a rejected archive
  is not resumed. No claim that every malformed archive was dynamically tested.
- R0484, XP343/current315: restore the exact original ULONG conversion.
  The actual config provider returns pointer-encoded constant VGA adapter 5;
  the extra ULONG_PTR cast changes neither width's result. A mirror compiler
  warning alone does not justify retaining a semantic-free adaptation.
- R0485, XP1040/current962: restore the original fixed-disk comment verbatim.
- R0486, XP1054/1056/current973/975: restore original comparison parentheses.
  Investigation initially found the original precedence misclassifies correct
  extended-memory bytes as BAD_XMS when the total KiB exceeds 255. A disposable
  observation in the actual cmos_error body proves that internal classification:
  current comparison passes an after-POST zero-error assertion; coherent original
  comparison fails it on both widths. This is NOT proof of a guest-visible fault.
  Counter-review follows cmos_error in the selected PROD branch: it writes the
  correct capacity bytes and recomputes the checksum, including when those bytes
  were already correct. Trace text is not emitted in this configuration. Other
  error bits and repairs remain independent. The only proven difference is
  redundant internal correction, so no supported observable-contract necessity
  has been established for the local parentheses. Original-first restoration
  therefore takes precedence; do not list the prior internal assertion as a
  required product regression. Existing CMOS tests check the actual register
  values after all four low/high corruption combinations.

Initial proof uses private replacement controller objects before the original
archive, not a copied device implementation. All variant compile/link steps
exit zero. Current observation passes both architectures. Original-memory
observation fails audit_xms_error == 0 (fixture line 300); original-disk fails
CMOS_DISK == 0 (existing fixture line 235). x86 exits 3221226505, x64 exits 3.
The initial two-line candidate passed four focused tests per architecture and
was artifact-equivalent to P106 except build metadata, but it is not the final
source: R0486 was subsequently restored and receives fresh qualification.

Final candidate changes source +4/-4 (net zero), permanent tests unchanged.
Four original lines restored reduce XP-relative added/deleted diff by eight
lines; no new difference point, helper or implementation path is introduced.
Full final-source dual-width build/publication and regression are running.
Frontier remains 404/472 until final evidence and delivery.

### P107 final qualification and counter-review

Both final cached builds and publications succeed. x86 passes 188 background
in 142.52 seconds and eight native tests in 23.92 seconds; x64 passes 188
background in 139.61 seconds and eight native tests in 24.79 seconds: 392/392.
The final CMOS register-value regression passes after original parentheses
are restored. Initial observation-only negative assertions are not acceptance
requirements and are removed with their disposable probes.

Final Nov 13 2026 compiler timestamps and published SHA-256:
- x86 17:10:02, cfbcef3a9638ad46db5f1d296311b900a0972f2be6c6010383ac21c81b9358e7.
- x64 17:12:59, ce43561599d3862e199b73eb130e1a091aa20f37ba2876cd5e737b679d0feb1a.

Independent review of the actual final diff confirms four exact original-line
restorations, source +4/-4, permanent tests unchanged, and XP-relative delta
-4 added/-4 deleted lines. The original comparison's internal false-positive
classification is explicitly retained rather than pretending it was fixed.
Only the topology and snapshot residuals are source/dynamically qualified
pending owner decisions. No guest binary, user configuration, media, snapshot,
shared component or Compat implementation changed. Existing exclusions have
no new ledger entries. Manifest/documentation and whitespace checks pass.
No exhaustive RTC phase, all HDD topologies or real DOS/Windows session claim
is made from these tests. Ordered file audit frontier is now 405/472; next is
406 base/system/dummy_nt.c. S1 remains active, not closed.

P107 process inventory confirms both owned jobs exited. Exact scratch cleanup
and build residue check pass; reusable Ninja and ccache roots are preserved.


## File 406: base/system/dummy_nt.c (P108)

Pinned XP raw SHA-256 002d33c97e599892fc8e78d5185ffafa0ec0d721e9c8cb249754ee56a0b7405e.
Complete file comparison confirms every selected original declaration and the
empty dummy_int body, including its semicolon, are unchanged. Existing excluded
bodies and line-control scaffold receive no new accounting.

R0487, local line 16 / original SCCSID notice at XP17: the historical notice
is preserved as one comment outside the excluded SCCSID body. Source policy
requires retention of that provenance; removing it is not a harmless cleanup.
This +1-line residual is source-policy-qualified pending owner decision.
Counter-review confirms it adds no executable behavior or replacement stub.

CMakeLists.txt:168 includes the file in the original device group. Both actual
Ninja trees contain its device object. compat/device_bop.c:65 registers the
original dummy_int as BIOS[BIOS_DUMMY_INT], defined as 01h in original bios.h.
The BOP dispatcher calls the registered handler. An intentionally empty original
BIOS implementation is still the needed single implementation; deleting the file
would require replacement wiring, not eliminate an unused production path.
This is source/build-registration proof, not a claim of a newly traced guest BOP.

No production, tests, configuration, manifest, guest assets or EXEs changed.
No new build/runtime test is necessary or claimed for this documentation-only
review. P107 remains the exact artifact and full 392/392 regression baseline.
The actual documentation diff and gates are reviewed before delivery. Frontier
406/472; next 407 base/system/ica.c. S1 and qualified owner decisions stay open.


## File 407: base/system/ica.c (P109 investigation; not complete)

Pinned XP raw SHA-256 3809cd6df0b6f665227735a6004f830b083670f07c0d1aa46f9ff8112a646066.
The complete-file comparison identifies selected differences in the historical
notice, snapshot/EOI declarations, delayed-IRQ mask, master-INTR withdrawal,
CCPU IRET-hook policy and private snapshot extension. Excluded bodies are not
new audit entries. No source restoration is delivered in this investigation;
the frozen file remains UNREVIEWED until its complete point review qualifies.

Initial source findings, not automatic retention approvals:
- The actual nt_sbdsp.c transfer path calls host_DelayHwInterrupt. Compat eoi.c
  owns the delayed-release event and DelayIrqLine; ica_scan_irr uses the combined
  mask while preserving IRR accounting. Existing device_contract irq cases
  exercise both adapters, repeated requests, release and cancellation. Removing
  that mask is not justified merely because the XP !NTVDM branch lacked it.
- CCPU c_main.c receives hook_address from ica_intack but does not consume it;
  its original comment states this explicitly. Actual keyboard_init and
  timer_init call ica_iret_hook_control. The current CCPU no-op avoids enabling
  the unconsumed hook protocol. The complete coherent restoration consequences
  and master-INTR withdrawal negative control still need final review.
- ICW1 register clearing remains the original implementation. AutoInitEoiHook's
  count<0 path returns without restarting DMA; do not infer the neighbor's
  timer EOI failure or add cancellation calls without this project's evidence.

### P109 pending local-extension decision L001: PIC initialization phase

This is a defect in the existing local snapshot extension, not in XP PIC
execution and not a newly admitted patch. SWPIC_outb owns static sequence[2]
with states -1, 0, 2, 3, 4. Snapshot capture/restore serializes adapter registers
and IRET bookkeeping but omits sequence. devices/archive.c's in-memory record
and wire field map also omit it; ccpu/archive.c calls that device archive.
Thus no other archive layer restores the controller's command interpretation.

A disposable variant of existing device_contract_smoke creates/resets the real
Machine and uses the actual original PIC port functions and current PIC snapshot
functions. For each adapter and after ICW1, ICW2 and ICW3:
1. Capture the partially initialized controller.
2. Complete the remaining ICWs without restoration as a control; capture the
   correct final register state and assert the requested vector base.
3. Perform normal PIC POST initialization, restore the saved partial state,
   send the same remaining ICWs, and compare final base, mask and mode.

Both architecture fixture compilations and links succeed against unchanged
production archives. All six controls per architecture produce the requested
base, mask 0 and mode 17. All six restored cases per architecture diverge:
- Master after ICW1: expected base/mask/mode 56/0/17; actual 0/1/1.
- Master after ICW2 or ICW3: expected 56/0/17; actual 56/1/1.
- Slave after ICW1: expected 112/0/17; actual 0/1/1.
- Slave after ICW2 or ICW3: expected 112/0/17; actual 112/1/1.
The final failures==0 assertion at disposable fixture line 311 intentionally
fails: x86 exit 3221226505, x64 exit 3. This is twelve actual-controller
reproductions with matching no-restore controls, not twelve passing tests.

Source cause: POST leaves sequence at fully initialized (0). Restore copies
registers but leaves that phase untouched, so ICW continuation writes on the
data port are interpreted as OCW1 mask changes. CPU instruction-boundary
snapshot access does not itself guarantee an entire multi-instruction ICW
sequence has completed. The probe does not execute a guest snapshot command
or reproduce a DOS/Win3.1/Win95 user symptom; those are unverified product cases.
It does establish failure of the actual device capture/restore contract.

Minimum proposed direction is to include the existing per-adapter phase in
that same archive: retain one state owner, make its storage accessible to the
existing capture/restore implementation, serialize/validate the phase and
restore it without port replay or a second initialization state machine.
This would introduce a new mirror difference and change archive schema; exact
patch size and format disposition require an owner decision. Under the current
no-increase audit rule no repair, fallback or compatibility layer is added.
L001 is a pending diff-point decision here, not TODO and not a claim that all
snapshot differences are correct. Existing source/artifacts remain P107.

P109 is a bounded investigation delivery. Production/test source diff is zero;
no fresh production build/publication or full-suite pass is claimed. The prior
P107 392/392 suite did not cover this partial-ICW snapshot case. Ordered frontier
remains 406/472; file 407 investigation stays active.

P109 owned probe process exited; exact scratch cleanup and build residue check
pass. Documentation gate and actual diff review pass. No user assets changed.


## File 407: base/system/ica.c (P110 complete-file review)

This completes the remaining P109 investigation without resolving its separately
qualified L001 owner decision. CMakeLists.txt selects CCPU, CPU_40_STYLE, PROD
and EOI_HOOKS on both widths; the actual device target includes ica.c. Full
selected-file comparison, including live code at excluded-range edges, finds
these difference points. Excluded bodies/scaffolding have no new ledger entries.

- R0488, current132 / XP133: one relocated historical notice. Source-policy
  necessity, no executable effect; qualified pending owner decision.
- R0489, snapshot header at165 and capture/restore extension after original
  selected body: actual device archive needs private PIC counts, ISR nesting,
  register and IRET bookkeeping. Port reads do not enumerate those fields;
  writing ports to replay initialization cannot restore arbitrary in-service
  state. Removing functions breaks real archive linkage; exposing all private
  state to move equal code into Compat does not simplify ownership. Pending
  qualified retention, with the specifically proven missing initialization
  phase L001 explicitly unresolved. No claim of complete snapshot correctness.
  The callback pointer rejection is a deliberate capture failure instead of
  serializing a native address; no parallel callback reconstruction is added.
- R0490, EOI header at166: the original host notification calls are selected by
  EOI_HOOKS, while original NTVDM declarations are not. The actual Compat EOI
  boundary declares these callbacks and the delayed mask used by R0491. This
  is a needed declaration, not duplicate PIC state. Pending source/ABI decision.
- R0491, current scan_irr / XP493: original uses iretHookActive only; local
  expression also masks DelayIrqLine. Original nt_sbdsp transfer completion
  uses host_DelayHwInterrupt. The host records a request in the actual PIC while
  delaying delivery until the existing quick event clears its mask. Restoring
  the original expression in the actual controller immediately makes that
  queued IRQ eligible; both architectures fail the existing assertion that
  scan_irr has no deliverable IRQ before release (fixture157). Current cases
  pass on both adapters, including release, accumulation and cancellation.
  Keep the one-line residual pending owner decision; remove its nonoriginal
  one-line explanatory comment without changing execution.
- R0492, master withdrawal after original XP1410: added else-if clears
  ica_cpu_int and host INTR when the only pending master request disappears.
  Original clears IRR but leaves the master output flag asserted. A private
  fixture using real PIC initialization, raise, withdrawal, capture and re-raise
  proves current output ownership clears then reasserts. Coherent removal of
  only the added branch leaves cpu_interrupt true after withdrawal and fails
  fixture289 on both architectures. Actual devices withdraw via ica_clear_int;
  the unchanged slave path recursively withdraws the master cascade. The test
  proves the PIC output flag contract, not a captured real guest lockup. Pending
  qualified retention; remove the added three-line explanation only.
- R0493, IRET control / XP1899-1904: original enables per-line IRET hook masks;
  selected CCPU adaptation is no-op. keyboard_init and timer_init really request
  hooks. Original c_main.c and zfrsrvd.c receive hook_address but never consume
  the return-hook protocol; original c_main explicitly documents that absence.
  Restoring the full original function body in a private controller, then using
  the actual keyboard registration/intack/EOI sequence, delivers IRQ1 three
  times but rejects the fourth: vector -1 instead of 9, hook=2 throughout.
  Current implementation delivers all eight requests with no hook on both
  widths. This is the unbalanced ISR-depth protection, not an invented caller
  obligation to invoke a hook the CCPU never uses. Pending qualified retention;
  remove only the added two-line explanatory comment. Existing branch scaffold
  remains, with no new compile-time configuration or fallback.

Every private production variant compiles and links with exit zero. Current
three probes pass on x86/x64 (six passes). Three original-restoration controls
per architecture fail their stated assertions: x86 3221226505, x64 3. The
IRET control logs deliveries 0/1/2 at vector9 and delivery3 at -1 on both widths.
No guest program, firmware, media or permanent test source is modified; this
uses one replacement real controller object before the existing archive and
one disposable variant of the existing device fixture, not a second PIC model.

Independent counter-review: delayed delivery and IRET completion are distinct
owners; neither mask can replace the other. ICW1 clearing remains original;
neighbor timer cancellation behavior is not transplanted. Snapshot state is
needed but L001 prevents claiming its completeness. The only final production
edit is six removed comment lines (+0/-6), permanent tests +0/-0; XP-relative
added lines decrease by six, deleted lines unchanged. No new difference point
is introduced. Points R0488-R0493 and L001 await owner disposition.

Both cached builds/publications pass. Each architecture passes three focused
CTest entries (device_contract_eoi, device_contract_irq, checkpoint_smoke):
six total. Whole published EXEs match P107 after excluding only the PE build
timestamp/checksum and the eight compiler time bytes; all other bytes match.
The owner equivalence exception applies: no new full or native-suite run is
claimed, and the P107 392/392 baseline remains applicable to unchanged code.
This does not qualify the user's changed package configuration.

Final x86 compiler timestamp 17:26:39 Nov 13 2026, SHA-256 2fe2d88d177eef0a0c452835da7a29735cf8d85f4eadd3bfbede6d1c816382ac.

Final x64 compiler timestamp 17:27:24 Nov 13 2026, SHA-256 02ef43877bffc5ff0d839fb0d266f5fe91e570fc7cd317f217da79edcb283fc1.

The user changed assets/binary/softpc.ini before this batch. The build/probe job
verified its bytes unchanged throughout; its existing change accompanies the
completed batch under the package-configuration rule. No agent edits to it.
File 407 now has a complete investigation disposition; frontier 407/472, next
408 base/system/idetect.c. S1 remains active with qualified decisions open.

P110 owned-process inventory is quiescent. Exact temporary-directory cleanup,
build residue check, manifest/documentation gates and final diff review pass.


## File 408: base/system/idetect.c (P111)

Pinned XP raw SHA-256 7bd4b2f03ca2befbf1807a8aa31b5f8a897e4c0165330bb438373ba4f03d5e00.
Complete file comparison finds exactly two non-excluded selected-code changes;
its original historical notice is already intact. Existing excluded bodies,
line-control scaffold and associated formatting are skipped without accounting.

R0494, original XP501/504, local379/382: idle_set originally assigns
(word)minperiod and (word)minpoll after the positive-value guards. Local code
removed the casts. Original base_def.h defines word as IU16, while this selected
!NTVDM implementation keeps the thresholds as int. Thus the removed casts can
change values above 65535; this is not automatically a mechanical x64 repair.
Whole source/test symbol and macro search finds only the definition, declaration
and IDLE_set macro, with no actual threshold-setting caller. No supported use
requires an expanded range. Restore both original statements exactly; do not
invent a new clamp or preserve unproven behavior as a compatibility contract.

Counter-review follows the real retained execution path: CMakeLists.txt includes
idetect.c; original keyboard code calls IDLE_poll/waitio/init, timer.c calls
IDLE_tick, and original video/disk/comms paths clear the activity flags.
Compat platform.c only toggles IDLE_ctl and resets IDLE_init on continuous-run
entry. The original detector decides when to call host_release_timeslice;
that existing host callback waits for the executor event and preserves finite
instruction budgets. Neither that integration nor original default thresholds
(12 ticks, 10 failed polls) changes. No competing idle implementation is added.

After restoration all selected original code matches XP. Source +2/-2, tests
unchanged, XP-relative added/deleted diff reduces by four lines. No residual
retention decision belongs to this file. Removing the whole file would remove
real keyboard/timer idle behavior and is not justified by the unused setter.

Final-source dual-width cached builds/publication and full regression are in
qualification. x86 EXE is not metadata-equivalent, so the owner exception is
not used merely because no actual setter consumer was found. No historical
full-suite result is substituted for this final-source run. Frontier remains
407/472 until verification and delivery; no guest/configuration changes.

### P111 final verification and review

Both final cached builds and publications succeed. x86 passes 188 background
in 143.70 seconds and eight native tests in 14.82 seconds; x64 passes 188
background in 149.18 seconds and eight native tests in 16.12 seconds: 392/392.
Both EXEs have actual nonmetadata changes from restored conversion code; no
artifact-equivalence test omission is claimed. Full-suite success is regression
evidence, not a new requirement that applications supply oversized idle limits.

Final x86 timestamp 17:31:50 Nov 13 2026, SHA-256 9730b9470aad50ee90170b7ba3579f60035b65113dbd283eb6f2c00db6461895.

Final x64 timestamp 17:34:39 Nov 13 2026, SHA-256 48df2431b5dd271920dbac1794bad2a631b9b50104c3573dd3a23e647e093f35.

Independent review verifies the two exact original casts, source +2/-2,
permanent tests unchanged and XP diff delta -2 added/-2 deleted lines. No new
helpers, Compat implementation or semantic special cases. Mirror CRLF and
normalized manifest identity remain correct. User INI is byte-unchanged by the
job; no media/snapshot/guest modifications. Documentation/manifest and actual
diff checks pass. Frontier advances to 408/472, next 409 base/system/illegalp.c.
All earlier qualified decisions, including L001, remain open; S1 is not closed.

P111 owned process has exited; exact disposable cleanup and build residue
checks pass. Reusable cache trees are preserved.


## File 409: base/system/illegalp.c (P112)

Pinned XP raw SHA-256 2f67c567c269779994da89f615de5dbcd103892d545e65f652730463702f4804.
Whole-file original/current comparison finds no selected-code difference.
Existing excluded bodies, line-control scaffold and adjacent scaffold spacing
are skipped without new point records or counts. The original historical
notice remains intact; no residual retention decision is needed.

Actual integration: root CMake compiles illegalp.c into softpc-machine-devices
and supplies its existing Compat system error declaration include directory.
Compat device_bop.c registers illegal_op_int at BIOS[0x06]; the common device
BOP dispatcher obtains and calls that table entry. The original routine reads
the saved CS/IP, reports the invalid operation through host_error, adjusts the
saved instruction address on continuation and calls unexpected_int. No copied
Compat implementation replaces it.

Counter-review distinguishes the other functions: illegal_op is an original
empty function in PROD, not an invented local stub. Whole src/test search finds
illegal_dvr_bop's direct test caller in bop_smoke.c, but no production service
registration for that particular entry. This does not make the file unused:
BIOS[0x06] remains a real consumer. Removing original functions or includes
would introduce differences without reducing an existing patch. Keep them.

Production/test/build changes: zero; XP-relative diff delta: zero. This is
source/caller proof, not a fresh guest invalid-op or driver-mismatch experiment.
No build, publication or runtime rerun is warranted for this documentation-only
delivery; P111 remains the unchanged dual-EXE and 392/392 regression baseline.
No scratch, configuration, media or snapshot changes. Frontier advances to
409/472, next 410 base/system/quick_ev.c; prior pending decisions remain open.


## File 410: base/system/quick_ev.c (P113)

Pinned XP raw SHA-256 e2ca6ab27c1e8b474902bda1c6c8a4b06e634cce062fa7ca07daac7b1e8658f4.
Full comparison outside existing excluded bodies/scaffolding finds the
relocated historical notice and the snapshot extension between delete_tic_event
and the original calibration functions. Original selected event insertion,
deletion, dispatch and calibration code is unchanged. Exclusions receive no
new accounting.

R0495, local59 / original60: one relocated historical copyright notice remains
necessary under source policy after the excluded SCCSID body. Qualified
retention decision, +1 original-relative line; not a runtime modification.

R0496, original insertion boundary1026 / local1002 onward: the snapshot
extension owns capture, measurement and restoration of private quick/tick
queues. Actual consumers are Compat devices/archive.c capture/restore, reached
by Compat ccpu/archive.c; queue records are serialized by device_write_queue
and device_read_queue. Callback translation covers actual timer, CMOS, floppy,
hard-disk, keyboard, serial, parallel, mouse, Sound Blaster and delayed-EOI
callbacks. Native pointers are not serialized. Existing quick_ev.h declarations
were separately audited in P92, not taken as proof by themselves.

Source necessity: the original queues, hash chains, handles and tick counter
are private to this translation unit. The original public add/delete APIs do
not capture these states, cannot prescribe restored handles/hash order, and
add_event immediately executes a zero-delay callback. Replacing reconstruction
with those calls would not preserve restore-without-dispatch semantics. Removing
the extension breaks the actual archive consumers; moving these same private
operations to Compat would not remove complexity or improve ownership.

Counter-review of the implementation: capture translates callbacks and checks
hash membership against the ordered list; restore constructs temporary lists,
validates callback IDs and hash links, cleans temporary allocations on failure,
and only replaces live queues after both lists succeed. The original free-list
cleanup helper is reused. Measurement enables actual-size archive allocation.
No second event dispatcher or timer policy is added. These are concrete source
reasons for the retained extension, not a claim that every malformed archive
or allocation-failure combination has been dynamically covered.

R0497 removes the six-line added explanatory comment before the extension;
its rationale is preserved here. Source +0/-6; permanent tests unchanged;
XP-relative added lines decrease by six, deleted lines unchanged. The remaining
extension is 230 added lines, including its separators, and remains a qualified
owner decision under R0496. No original selected statement is changed.

Existing checkpoint_smoke exercises real Machine/reset and the real quick/tick
queues: capture, late invalid callback restore failure preserving live state,
successful restore without executing callbacks, then ordered dispatch. Existing
queue_archive_smoke checks hash-chain membership including an orphan after a
valid prefix. These cover distinct observable contracts, not exhaustive guest
snapshot behavior. Final-source dual-width builds and these tests are running;
frontier remains 409/472 until this delivery is verified.

### P113 final verification and independent review

Both cached builds and publication complete successfully. Whole-EXE comparison
to P111 is identical on each architecture after masking only PE timestamp and
checksum (PE+8 and PE+88, four bytes each) and the eight compiler-time bytes
at the unique unchanged Nov 13 date/time literal location. All other bytes,
lengths and that literal position match. Under the owner-approved equivalence
exception the full suite is not repeated; P111 remains the 392/392 full-runtime
baseline. Fresh checkpoint_smoke and queue_archive_smoke both pass on both
architectures: 4/4 (x86 1.86 seconds, x64 4.05 seconds total).

Each build emits 178 diagnostics marked warning, all located in the preserved
mirror tree, not the public components. No warning cleanup is mixed into this
audit and this delivery is not claimed warning-free.

x86: compiler timestamp 17:45:15 Nov 13 2026, SHA-256 bb40ba3f8a9edd203281d193cb87e40363b566cb1b6d075270aed7657d6520ea.

x64: compiler timestamp 17:45:31 Nov 13 2026, SHA-256 e92b69169daa2702cd0eda7d7f945a0efe3d5fc8290f2b067fd5efa00fa2c912.

Independent actual-diff review confirms exactly six removed comment lines,
no executable-code change, no new path, no moved implementation, and the
normalized local manifest update. Mirror CRLF remains intact; documentation
uses LF. User INI is unchanged and guest/media/snapshot files are untouched.
The owned background process exited. Frontier advances to 410/472; next 411
base/system/rom.c. R0495/R0496 and earlier qualified decisions remain open;
no task closure or complete snapshot correctness is claimed.


## File 411: base/system/rom.c (P114)

Pinned XP raw SHA-256 dce7d1cfce5bbe7f8c27cbebe1f5a4294f882a8d148618cdb0a78aa485de62a4.
Whole-file review outside existing excluded bodies/line-control scaffold finds
three selected difference groups. Original notices are already intact.
Existing exclusions receive no new accounting.

R0498, XP257 / initial local239-240: restore the original ULONG conversion of
config_inquire(C_GFX_ADAPTER, NULL) and remove the added portability comment.
The actual Compat platform.c provider returns SOFTPC_VGA_ADAPTER, constant 5,
encoded through void *. No pointer is dereferenced or needs to survive the
conversion. Original consumers in reset.c, cmos.c and keyba.c already use the
original ULONG form. A native-pointer-width conversion is not necessary for
this scalar domain; a warning alone does not establish changed semantics.

R0499, XP585 / initial local442: retain the removal of the original UCHAR cast
from setCS((addr & 0xf0000) >> 4), qualified pending owner decision. The actual
reset.c path calls search_for_roms, which checks signatures/checksums, pushes
the original FE00:095A return trampoline, sets CS/IP and invokes host_simulate.
For an option ROM at C8000, the entry is C000:8003. The original eight-bit cast
turns C000 into 0000, selecting physical 08003 instead of C8003. This is a
concrete segment-address error, not a host x64 representation requirement.
The existing device_contract_rom test installs a generated 512-byte valid ROM
whose entry stores its actual CS at physical 0500 and returns through the
original trampoline; expected bytes are 00 C0. It uses the real scanner and
CPU, not an arithmetic-only mock. No original guest binary or firmware asset
is modified. Current-source proof does not imply all option-ROM layouts or
all protected-mode scenarios were tested. Residual XP difference is +1/-1.

R0500, XP799/828/851/871 / initial local618-620/649-650/674-675/687:
restore original IU32 endLinAddr, original strlen-based calculation/loop and
cur_loc update, removing the added size_t/stringLength adaptation and comment.
Whole src/test symbol search finds only display_string and clear_string in
this original file (the latter calls the former), with no external production
or test consumer. No supported caller requires a host-size-length expansion
or cached strlen. Restoring the complete original group avoids retaining a
partial adaptation. Original empty copyROM/update_romcopy and other functions
are kept; no extra deletion differences are introduced.

Counter-review: root CMake still compiles rom.c, original ccpusas4.c SAS initialization loads ROM through
rom_init/read_video_rom, while reset.c calls the scanner. The embedded-resource host
provider remains the host boundary, not a competing ROM initialization
implementation. Necessary CS correction is preserved; no renderer or Compat
rewrite substitutes for original logic. Source +5/-10, tests unchanged;
XP-relative diff decreases by 15 added/deleted lines, not merely net five
source lines. Remaining selected difference is solely R0499.

Final-source cached dual-architecture build/publication and regression are in
progress. x86 has nonmetadata executable changes, so full regression is run;
no source-equivalence claim substitutes for actual artifact comparison.
Frontier remains 410/472 until verification and complete delivery.

### P114 final-source verification and independent review

Both final cached builds/publications succeed and both EXEs have nonmetadata
changes, so the complete regression is run on each architecture. x86 passes
188 background tests in 141.97 seconds and eight native tests in 15.82 seconds;
x64 passes 188 background in 137.64 seconds and eight native in 15.56 seconds:
392/392. Both real device_contract_rom tests pass. This verifies the current
retained CS correction through generated-ROM execution; original failure is
proved here by the exact address truncation, not a newly run original-code
negative or a claimed DOS/Windows guest symptom.

x86: compiler timestamp 17:49:06 Nov 13 2026, SHA-256 5b4f9c57becd81c3c91dd1668a298e8c04321f4cf5eff124a5588afbc5b41de2.

x64: compiler timestamp 17:51:55 Nov 13 2026, SHA-256 3476ba2577d9cb62f124076b03f62a82f1237fe268be896bd1a3d9c3b5f5023c.

Independent review confirms the five exact XP statement restorations and
removal of the two corresponding added comments. Source +5/-10, tests zero;
raw-baseline comparison delta is -10 added/-5 deleted, reducing mirror diff
by 15 lines. Only the CS conversion remains as a selected-code difference.
All other mismatches belong to already excluded bodies/scaffolding and receive
no new records. No replacement implementation, fallback or new mirror point.

Build warnings: x86 89 and x64 90, all originating in preserved mirror paths;
this is not a warning-free claim. The original scalar conversion adds the
expected x64 pointer-width diagnostic without changing its actual value domain.
No warning suppression or unrelated original-code cleanup is added. Mirror
CRLF, normalized manifest and documentation LF are preserved. User INI is
byte-unchanged, guest/media/snapshot files untouched, owned job exited.
Frontier advances to 411/472; next 412 base/system/timer.c. R0499 and earlier
qualified decisions remain open. S1 remains active.


### P115: file 412 timer.c behavior-difference investigation

This is an investigation delivery while file 412 remains in review. Pinned XP
raw SHA-256 4d640fafdc5d7b3daa698e531f567225aebfb66f4eea7edb1f6417e8d105084b.
Production, permanent tests, manifest and published EXEs are unchanged.
Frontier stays 411/472; no later file is advanced.

Three independent disposable timer.c variants restore one existing behavior
change at a time. Each is compiled with its actual Ninja production command
and linked before the real machine archive into the unchanged existing PIT
or sound fixture. No original guest/firmware is rebuilt or modified.
All six variant compiles and links succeed. Current production controls pass
both fixtures on both architectures (four control executions).

- Remove the first-event zero-delay guard: real pit_smoke exits 3221225725
  (0xc00000fd, stack overflow) on x86 and x64. With a one-clock counter,
  SYSTEM_TICK_INTV/n truncates to zero; original add_event immediately invokes
  zero-delay callbacks, and timer_multiple_ints recursively requeues itself.
- Restore the original estimate/wrap behavior coherently (remove estimated
  flag and suppression together): pit_smoke fails irq_requests == 0 at line46
  on both architectures, x86 exit3221226505, x64 exit3. The test only latches
  and reads the real PIT while no executor/heartbeat advances time; original
  guess-driven wraps manufacture IRQ work.
- Restore TIMER2 POST gate to GATE_SIGNAL_RISE: sound_smoke fails the actual
  delivery-count assertion at line177 on both architectures, same respective
  assertion exit codes. Current LOW gate passes. This is the existing
  machine/PPI/audio-fixture contract, not a claim of hearing a native device.

The first two retained differences must not be presented as exhaustive PIT
correctness. The adjacent active-event update still assigns
SYSTEM_TICK_INTV/(n+1) to int_delay and can lower timer_multiple_delay to zero.
The existing fixture stops after checking that two heartbeats requested IRQ;
it does not then dispatch the queued callbacks.

#### D005: active backlog can reintroduce zero delay

A disposable copy of pit_smoke keeps the real create/reset, port writes and
nine mode/divisor combinations. After its two real time_tick calls, it records
the actual PIT snapshot, sets the existing CPU quick-event countdown to zero
and calls the actual dispatch_q_event. No alternate timer/queue is introduced.
Both current production libraries pass the three mode0 cases, then print:

    before dispatch divisor=1 mode=2 delay=0 active=1 more=65525

Both then terminate with 3221225725 (0xc00000fd). This is dynamic proof of
current device/queue failure, not merely an inference from a suspicious divide.
It is not a recorded DOS, Win3.1 or Win95 guest crash; the probe deliberately
advances the queue through the existing countdown/dispatch interface.

The isolated candidate inserts exactly these two statements immediately after
the active-event int_delay calculation, before the existing comparison:

    if (int_delay == 0)
        int_delay = 1;

It preserves the original comparison and scheduling path. Both architectures
compile/link successfully and complete all nine real mode/divisor/dispatch
cases: eighteen total. Mode2 and mode 3 divisor1 now report delay1, active1,
more65525 and return from dispatch. No production change or new permanent
test is made: these two lines would introduce a new XP-relative point, barred
by the current no-increase policy. D005 belongs to pending diff decisions,
not TODO. Existing first-event protection remains necessary but incomplete.

Remaining file work: reconcile selected formatting against XP, complete
snapshot/state/callback evidence and minimize only permitted existing
differences, then deliver the complete-file audit with required builds/tests.
Do not promote this investigation into file completion or inherited approval
of all timer extensions. P114 remains the unchanged full 392/392 and dual-EXE
baseline. All three owned probe processes have exited; results are recorded
here before disposable cleanup. No user configuration/media/snapshot changes.


## File 412: base/system/timer.c (P116)

Complete-file follow-up to P115. Fixed XP raw SHA-256 remains
4d640fafdc5d7b3daa698e531f567225aebfb66f4eea7edb1f6417e8d105084b.
Existing excluded bodies and line-control/conditional scaffolding are skipped
without new bookkeeping. Changes are confined to selected original formatting
and added comments, not timer behavior.

| Point | Original/current difference and disposition | Evidence and independent review |
| --- | --- | --- |
| R0501 | Relocated historical notice at initial local34 / XP35, one added line; qualified pending. | Source policy requires preserving the notice outside the excluded SCCSID body. No code behavior. |
| R0502 | Snapshot include at initial local69 and appended snapshot/callback operations; qualified pending. | Actual devices/archive capture/restore and callback codecs consume them; private PIT states and actions cannot be reconstructed from public port reads/writes. Header plus final 358-line extension span, not a claim that excluded original debug code was newly deleted. |
| R0503 | Initial local2041-2071 / XP2313-2338: restore exact original indentation and remove three added comment lines, retaining the two first-event zero-delay guard lines. | P115 restoring the guard away causes stack overflow on both architectures in real pit_smoke. D005 separately proves that the retained guard is incomplete for already-active backlog; necessary does not mean sufficient. |
| R0504 | Initial local2093-2094,2130-2136,2166-2173 / XP2495,2528-2533,2566-2568: retain estimated flag and wrap suppression; restore original surrounding statement/comment indentation and remove two added explanatory lines. | P115 coherent original variant fails no-IRQ-from-read assertion on both widths. Original counter updater remains the single calculation path; no persisted state, caller special case or new clock. |
| R0505 | Initial local2572 / XP3026: retain TIMER2 POST gate LOW instead of RISE, +1/-1, qualified pending. | P115 original variant fails actual sound fixture delivery at line177 on both widths; current controls pass. Original PPI gate transition requires consistency with its low reset register. No new sound path. |
| R0506 | Remove six-line added snapshot explanation before the extension enums. | No executable behavior; rationale belongs in this record. Existing snapshot code and format stay unchanged. |

R0502 necessity and scope: original timer.c owns three COUNTER_UNIT records,
the selected current counter, latched read state, current mode, state/action
functions, activation phase and pending interrupt bookkeeping. Port reads
consume/alter latches and cannot enumerate these private fields. The snapshot
implementation maps the finite original state/action functions to IDs; it does
not serialize process code pointers. Source scan of actual state, prior/wait,
gate and action assignments finds the functions represented by those mappings.
The encoder/decoder also maps timer_no_longer_too_soon and timer_multiple_ints
for the actual quick-event queue. It is not a duplicate timer implementation.

Counter timestamps are captured as elapsed microseconds and rebased against
the existing per-counter getTime function on restore (getIdealTime for counter0,
getHostSysTime for the others). The existing archive serializer explicitly
writes these fixed-width fields. Restore republishes channel2's existing
waveform through host_timer2_waveform; no second waveform generator is added.
Moving the private serialization wholesale to Compat would not reduce the
implementation and would expose state without removing its necessity.

Existing checkpoint_smoke programs channel0, captures it, reprograms it,
restores and checks mode, access mode, initial count, state and wait action;
it rejects an invalid state ID. This is direct coverage of the actual snapshot
entry, not exhaustive coverage of every mode/action/error combination. Restore
can partially modify counters before returning failure; this record does not
claim transactionality or permission to resume a failed archive. Existing
P115 D005 remains an explicitly proven open defect. No all-PIT-correct claim.

Actual source diff +38/-49 (net -11), permanent tests unchanged. The raw fixed
XP comparison falls from +682/-813 to +639/-781: -43 added/-32 deleted,
75 fewer difference lines. These totals are used only for the delta; existing
exclusions are not enumerated or added to this audit. All selected original
formatting recovered here is exact XP text; necessary retained new guard lines
use the surrounding original indentation. No new mirror point or D005 patch.

### P116 final-source verification and independent review

Both cached builds and publication succeed. Complete EXEs match P114 after
masking only PE timestamp/checksum (PE+8/PE+88, four bytes each) and eight
compiler-time bytes at the unique unchanged Nov 13 literal location. Lengths,
literal positions and every other byte match. Thus the owner-approved artifact
equivalence exception applies; the full runtime baseline remains P114 392/392.
Fresh PIT, sound and checkpoint tests pass 3/3 on each architecture, 6/6 total
(x86 2.62 seconds, x64 7.68 seconds). This does not include or fix D005.

x86: compiler timestamp 18:04:17 Nov 13 2026, SHA-256 b95a9dca256e757608037f286c83126bcbcca2f354b3822dd708b4befee9a7dd.

x64: compiler timestamp 18:04:30 Nov 13 2026, SHA-256 dc25d22833c329bcbe4ededf8e1d25ce1711be5cc6443e57cc2fd909ffef6e7a.

A preliminary scratch-script generation assertion stopped before creating
the runner; the attempted launcher exited without running a build. Its absence
was verified before the actual runner was generated and launched. No live
job was restarted or killed. Final verification uses only the completed run.

Actual diff review verifies retained behavior, CRLF mirror bytes, manifest
identity, and no user INI/media/snapshot changes. Frontier advances to 412/472,
next 413 base/system/timestrb.c. Qualified residuals remain owner decisions;
D005, L001 and earlier decisions are open. S1 is not closed.

P116 emits 89 existing mirror-path warnings per architecture, none outside
the mirror; no warning-free claim. Owned process exit, exact scratch cleanup,
residue check, manifest/documentation gates and actual diff review pass.

## File 413: timestrb.c (P117)

Pinned XP raw SHA-256 897275811dc989e7676598292c0811dbe0cc648d17837665ec443ba608e55dca.
The complete file comparison finds no selected-code difference. Existing
excluded bodies and their line-control scaffolding are skipped without new
accounting. The original historical notice and empty dummy_alarm remain.

Actual production wiring: the root device source list includes timestrb.c;
CCPU c_main.c calls host_timer_event, whose Compat platform implementation
calls time_strobe. The selected original function invokes time_tick,
rtc_tick, dispatch_tic_event and ica_check_stale_iret_hook, and preserves the
original video-off counter, clear and refresh behavior. The unused original
empty function is not a reason to introduce a new mirror difference.

Independent counter-review distinguishes this unchanged file from timer.c's
D005 finding: this file adds no timer repair and does not resolve that pending
decision. No production, test, manifest, binary, configuration or guest change.
No new build/runtime test is needed for this documentation-only disposition;
P116 published artifacts and P114 full 392/392 baseline remain unchanged.
Frontier advances to 413/472; next is 414 base/system/unexp_nt.c. Its terminal
Ctrl-Z difference has been inspected but is not yet a completed disposition.
S1 remains active, with all prior pending decisions still open.

## File 414: unexp_nt.c (P118)

Pinned XP raw SHA-256 e4858f4cf27151e479ba6c977c897f5f0f2396b93cb80bd4617f743201488f94.
The entire selected interrupt handler matches XP. Root CMake includes the
translation unit; Compat device_bop registers unexpected_int at BIOS_UNEXP_INT,
and original illegalp.c also calls it. Original ISR inspection, interrupt
masking, cascade preservation, master/slave command-port EOI and BIOS cause
storage remain unchanged. Existing excluded bodies/scaffolding are skipped
without new bookkeeping.

R0507: retain the relocated original historical notice, +1 line, qualified
pending owner decision under the source policy; no executable behavior.
R0508: XP ends with a standalone Ctrl-Z byte after the final function newline.
The local file replaced that byte with a blank line. Remove that replacement
blank line: source +0/-1, XP added/deleted diff decreases by one added line.
The necessary residual is deletion of the original Ctrl-Z, +0/-1, qualified
pending owner decision. No new mirror location or logic is introduced.

Initial analysis considered restoring the exact original byte. Independent
counter-proof compiles this actual translation unit using each supported
Ninja GCC command, redirecting object/dependency outputs to task scratch.
Current-source controls compile successfully on x86 and x64; restoring only
the terminal byte fails both with error: stray '\32' in program at line 96.
The unchanged original handler is therefore not rewritten to solve a compiler
format issue. This establishes build necessity, not guest runtime coverage.

Final cached builds and publication pass: x86 8.17 seconds, x64 8.94 seconds.
Whole-EXE comparison against P116 finds identical lengths and bytes after
masking only PE timestamp/checksum and eight time characters of the unique
same-position Nov 13 2026 compiler literal. The final date is unchanged.
Both artifacts are metadata-equivalent; the approved exception applies and
no new full runtime run is claimed. P114 remains the 392/392 full baseline.

x86-artifact: 18:13:17 Nov 13 2026, SHA-256 8fd85fd0fd7efd90cff337e17a14050c67c9e42816084d3f410068ef44d7ce44.

x64-artifact: 18:13:25 Nov 13 2026, SHA-256 93791fd680b32a8775c882e02e45e36dbd440a01c743f82dbd86c46ebfbef8f4.

Actual diff review confirms only the extra terminal blank line was removed;
CRLF is retained and the local manifest hash matches. No tests, Compat,
configuration, media or snapshots change. Frontier 414/472; next 415 cga.c.
All earlier pending decisions, including D005 and L001, remain open.

Both builds emit 89 existing mirror-path warnings, none outside the mirror.
The owned runner exited successfully and its PID is absent. Task scratch was
previewed, removed and residue-checked. Manifest/documentation gates and
actual diff review pass; no warning-free or fresh full-runtime claim.

## File 415: cga.c (P119)

Pinned XP raw SHA-256 568555cd33693f885beaa8768178ae85cdebb141738c41d7fd31237ed501f3b4.
Full-file comparison finds only two selected declaration changes beyond the
existing excluded bodies and line-control scaffolding, which are skipped
without new accounting. The original historical notice is already intact.

R0509 restores both bios_ch2_byte_wrt_fn and bios_ch2_word_wrt_fn definitions
from added IPT2 parameter vocabulary to exact original (). Under the actual
supported non-ANSI macro branch IPT2 expands to (), so the added spelling
provides no parameter-type contract. It does not repair argument width or
calling convention. Source +2/-2; XP diff reduced by two additions and two
deletions, four difference lines. No selected residual difference remains.

Actual consumers are the original video.c BIOS text writes; cga_init installs
simple_bios_byte_wrt/simple_bios_word_wrt, and the selected C-VID SetBiosWrites
installs write_bios_byte/write_bios_word. Root CMake includes cga.c and gvi.c
calls its initialization/termination. The header's existing IPT2 declaration
has the same active expansion and needs no coupled edit. No Compat substitute
or new dispatch route is introduced.

Counter-review uses the real x86/x64 Ninja GCC preprocessing commands before
and after exact restoration. Complete outputs differ only by the space before
() at these two definitions; all other bytes match. The initial raw comparison
correctly detected those spaces. A first normalization attempt also touched
unchanged extern declarations and was rejected; the final comparison matches
only the two definition lines and passes both architectures. Each failed
probe process exited before correction; none was killed or restarted live.
No failed preliminary probe is counted as passing evidence.

Final cached builds and dual-EXE publication pass. Whole-file EXE equivalence
against P118 masks only PE timestamp/checksum and eight compiler time bytes
at the unique same-position Nov 13 2026 literal; every other byte and length
matches. The approved equivalence exception applies: no fresh full runtime
run is claimed; P114 remains the full 392/392 baseline.

x86-build: 8.92 seconds, exit 0.

x86-artifact: 18:17:41 Nov 13 2026, SHA-256 239394f2d7f438f9d37b71aef5987a7454d5701caf7807af54de73dce05808ff.

x64-build: 10.11 seconds, exit 0.

x64-artifact: 18:17:50 Nov 13 2026, SHA-256 b7fbe5f8457db0e27000d1cc7f5ca12fa844ee51739d051ef42718c9c0949467.

x86: 89 existing mirror warnings; none outside the mirror.

x64: 89 existing mirror warnings; none outside the mirror.

Actual production diff is exactly the two original declarations; mirror CRLF
and manifest identity are preserved. No tests, other source, user INI, media
or snapshots change. Frontier 415/472, next 416 base/video/egawrtm0.c.
Earlier qualified decisions remain open; S1 is not closed.

P119 owned runner exited and its PID is absent. Exact scratch cleanup and
residue check pass, as do mirror/documentation gates and actual diff review.

## File 416: egawrtm0.c (P120)

Pinned XP raw SHA-256 59d1f4149c4717d951e6d70953cd9b0a79c19db3ae3108072260032db1af0303.
Complete-file comparison finds four selected address/stride replacements;
existing excluded bodies and their scaffolding are skipped without accounting.
The original notice, declarations and other selected code remain unchanged.

Actual wiring: ega_write_routines_update selects mode0_copy_handlers or
mode0_gen_handlers and assigns the BIOS byte/word callbacks to this file's
functions. CPU_40_STYLE SetWritePointers selects the separate CPU instruction
writers, but does not replace those BIOS callbacks. SetBiosWrites exists in
ev_glue.c but has no production caller; its mere definition does not make the
assigned BIOS functions unreachable. video.c invokes the callbacks for its
applicable non-V86 text-write paths. This is not proof of every V86 path.

Source reasoning: odd/even physical storage preserves the plane address A0,
so byte address a maps to 4*(a & ~1)+(a & 1). Original division by two loses
a plane address bit. For an odd-address WORD, the second byte advances to
the next even address, eight physical bytes beyond the aligned base rather
than four. The selected layout is independently qualified by earlier C-VID
and text-painter evidence, including R0272, R0362 and R0434.

| Point | Exact selected difference | Independent recovery test |
| --- | --- | --- |
| R0510 | ega_copy_b_write: (offset >> 1) << 2 becomes (offset & ~1u) << 2. | Byte 0x11 at offset 2 must read back through C-VID. |
| R0511 | ega_copy_w_write: same aligned-address replacement. | WORD 0x3322 at offset 6 must read back as 22,33. |
| R0512 | ega_copy_w_write: odd WORD second-byte displacement 4 becomes 8. | WORD 0x5544 at offset 9 must read back as 44,55. |
| R0513 | ega_mode0_chn_b_write: same aligned-address replacement. | Actual graphics-controller set/reset selects generic callback; enabled planes at offsets 12/13 must read back ff. |

A disposable variant of existing vga_frame_smoke initializes and resets the
real machine and invokes original BIOS mode 3. It asserts the actual selected
callback identity, calls that callback, and reads via the existing C-VID
reader, comparing its byte result. Graphics-controller ports select generic
mode instead of patching function pointers or inventing a private writer.
Four private copies of the actual production translation unit each restore
exactly one original difference. Each is linked ahead of the unchanged machine
archive with the same fixture, preserving all other production objects.
No original guest binary, media, production file or permanent test is modified.

The first setup assertion found multiple reset expressions in the fixture and
exited before compilation. The corrected setup targets main's reset explicitly;
the exited process was confirmed absent before the actual run began.

Both architectures compile and link all five production variants and their
fixture successfully. Current controls pass both architectures. Each of the
four separate original restorations fails its intended readback assertion on
both architectures: eight expected negative failures. x86 assertion exits
3221226505; x64 exits 3. All observed values below are low bytes; the original
reader's wider return register is not the byte contract being asserted.

| Variant | Expected bytes | Restored original observed bytes |
| --- | --- | --- |
| Byte mapping | 11 | 20 |
| WORD mapping | 22,33 | 20,07 |
| Odd WORD displacement | 44,55 | 44,20 |
| Generic mapping, first offset | ff | 20 |

Current controls also pass generic offset 13 (odd plane), not only offset 12.
The actual selected BIOS pointer identities are asserted before each family.
These are real controller/BIOS callback and C-VID readback tests, not a new
Win3.1 reproduction, all-mode matrix or proof of every fill/move path.

Independent review: the complete original restoration at each point breaks
its actual selected consumer; retaining these existing four replacements is
therefore evidence-qualified pending owner decision R0510-R0513. Minimal
residual is the existing four changed lines (+4/-4 against XP), with no new
helper, branch, duplicate writer or broader change. Production/test source
delta is zero; mirror-diff reduction this delivery is zero. Earlier original
bulk-entry issues retain their separate decisions, not silently fixed here.

This delivery changes documentation only. Published P119 EXEs remain untouched;
P114 remains the full 392/392 runtime baseline. No full rebuild or regression
is claimed for this source investigation. Frontier advances to 416/472; next
417 follows the frozen file ledger. S1 and all qualified decisions remain open.

P120 actual diff review confirms only evidence, frontier and CURRENT changed.
Owned runner exited and its PID is absent; task scratch preview/removal and
residue check pass. Documentation gate passes. Configuration/media are intact.

## Files 417-418: ega_dmmy.c and ega_mode.c (P121)

Reviewed in frozen order, completing 417 before inspecting 418. Complete
pinned-XP comparisons find no selected-code differences in either file.
Existing excluded bodies and line-control scaffolding are skipped without
new bookkeeping; original notices and formatting remain intact.

| Order | File | Pinned XP raw SHA-256 | Actual production integration |
| --- | --- | --- | --- |
| 417 | base/video/ega_dmmy.c | 82c69f46436fd64c47d0e62b479f684084375160bcaf9ed1a6b513ac2d91df16 | Root device object; get_ega_switch_setting returns original zero and is called by ega_ipstat0_inb in ega_prts.c to fill the switch-sense bit. |
| 418 | base/video/ega_mode.c | f4c062454b54800c2979b0acd9d2ae75726dbdc0169443b7f5ed0379d879da7b | Root device object; EGA/VGA port initialization invokes ega_mode_init; original mode tables, screen-plane selection and update routing remain unchanged. |

Independent counter-review: a function being an original stub is not a reason
to delete it or replace it with a Compat implementation. File 417 has a real
consumer and exactly preserves XP behavior. File 418's unchanged original
selection logic needs no new patch merely because other video files contain
qualified differences. Neither file acquires a new decision point.

Production, tests, mirror diff and published EXEs are unchanged. This is source
and integration inspection, not new runtime proof. No rebuild/test run is
needed for the documentation-only update; P119 artifacts and P114 full 392/392
baseline remain. Frontier 418/472; next 419 base/video/ega_prts.c. S1 remains
active and earlier qualified decisions remain open. No scratch or job created.

## File 419: ega_prts.c (P122)

Pinned XP raw SHA-256 a8e0f61aa9e9424b1cda069e814d7e04b761e9ad8342424e3602457cc00eda5f.
Complete-file comparison, including mixed diff ranges and deletion-only
fragments, finds only the added reset-state getter outside existing exclusion
and line-control scaffolding. Those excluded bodies/scaffolding receive no
new accounting. Original cursor calculations and other selected register
handlers remain exactly XP; no new repair is inferred from their presence.

R0514: ega_snapshot_seq_reset, seven added lines including reason comment and
separators, is qualified pending owner decision. This accesses the actual
static EGA sequencer.reset.as.abyte; it does not create a second state owner.
Actual chain: Compat devices/archive.c captures video_controller through
softpc_device_snapshot_capture_video_controller in vga_prts.c, which invokes
this getter. Both EGA and VGA port initialization register ega_seq_reset as
the reset write receiver. The receiver writes this private EGA storage and
updates ASYNC_RESET/SYNC_RESET display-disable bits.

Independent counter-review of original alternatives: the ordinary original
vga_seq_inb register-zero case always returns 3, regardless of the receiver's
current stored value. The VGA structure's reset field is a different object.
Neither can capture the actual pending reset state. Reading visible disable
bits alone also does not preserve the stored byte's complete value. Making
the EGA storage global or duplicating it in Compat increases the ownership
surface rather than coherently restoring an existing capture mechanism.
The current narrow getter is the minimal existing state-access adaptation;
no new fallback, synchronization, or archive representation is proposed.

Focused verification runs the unchanged production-linked vga_frame_smoke
executable on both architectures: x86 7.18 seconds, x64 5.29 seconds, both
exit 0. Its existing reset loop writes each value 0..3 through ega_seq_reset,
captures and asserts that byte, changes the live reset, restores the archive,
and checks the resulting disable bits. This proves the current capture/restore
contract. The failure of substituting fixed original port readback for values
0..2 is source proof, not a newly executed negative build. The fixture is not
an actual guest snapshot acceptance or all-register/invalid-archive matrix.

Production/test source and mirror diff delta are zero. No permanent test or
published EXE changes; P119 remains published, P114 retains full 392/392
baseline. No new full build/runtime run is claimed. Frontier 419/472, next
420 base/video/ega_read.c. R0514 and earlier decisions remain open; S1 active.

P122 owned runner has exited and its PID is absent. Exact scratch cleanup,
residue check, documentation gate and actual three-document diff review pass.
User INI/media/snapshots remain unchanged.

## File 420: ega_read.c and coupled cpu_vid.h restoration (P123)

Pinned XP ega_read.c raw SHA-256 c9a284cca09e307fe4f6da47f2d696c0f4ec378d1d88159c84a5e94ccd419127.
Full comparison finds three removed original local declarations and one added
trailing space; other differences are existing excluded bodies/scaffolding,
which are skipped without new accounting. The selected CPU_40_STYLE path uses
SetReadPointers; old alternate bulk callback wiring retains the P61 disposition.

R0515 restores the exact three original read-provider declarations beside
Glue_reads, and removes the corresponding three added declarations from
cpu_vid.h. The old deletion's now-unneeded #line 385 is removed because the
original lines 382-384 are back. R0516 restores the original Glue_reads
initializer-line whitespace. No selected residual remains in ega_read.c.

Initial isolated restoration fails x86/x64 with conflicting int() versus
IU32/void declarations. That proves only an incompatibility with our own
header additions, not that original local declarations are necessary to omit.
Counter-review therefore removes the three added header reader declarations
in a private include root while restoring original local declarations.
Actual ega_read.c and ev_glue.c compiler commands all succeed on x86 and x64,
four coherent builds. Repository search finds no other production consumer
of these three provider identifiers; existing VGA tests declare their byte
reader locally. The original table receives the same actual provider addresses;
provider definitions and table call types are unchanged. No runtime adapter,
new type, second dispatch route or substitute implementation is introduced.

This supersedes R0423's retention conclusion for its three reader declarations.
P76's isolated removal lacked the original local declarations and therefore
was incomplete evidence for their retention. Its six writer declarations
remain a separate qualified decision, subject to their own consumer review.
The earlier result is preserved as history, not treated as current proof.

Final production delta: +4/-5 (net -1); tests unchanged. XP raw comparison
before/after: ega_read.c +77/-344 -> +75/-340, cpu_vid.h +20/-13 -> +17/-13;
combined reduction is nine added/deleted difference lines (-5 added/-4 deleted).
These whole-file delta metrics do not create new exclusion records. Current
mirror CRLF and required diagnostic mapping are retained; manifests updated.

A preliminary whitespace-only build preceded the coherent-restoration result.
Final delivery was rebuilt from the complete two-file source changes, not
inferred from that preliminary build. Final x86 build 15.15 seconds and x64
18.63 seconds pass. Whole-EXE comparison masks only PE timestamp/checksum and
eight compiler time characters at the unique same-position Nov 13 2026 literal;
all remaining bytes and lengths match. The final artifacts are metadata-
equivalent to the preliminary artifacts and P119. The approved test-omission
exception applies; P114 remains the full 392/392 runtime baseline. No fresh
full-suite or all-guest validation is claimed.

x86-final-artifact: 18:32:22 Nov 13 2026, SHA-256 8a1b6a4ad3fc4ae986d8e50d0d3b55f558f2a2cc8e8bfc8262e4a57c13d54e7b.

x64-final-artifact: 18:32:37 Nov 13 2026, SHA-256 e84f3a7737552f5ec42be71fa98b1c4be760e4e62239835d2451db61f430ef48.

x86: 1013 mirror-path warnings, none outside the mirror.

x64: 1063 mirror-path warnings, none outside the mirror.

Original implicit-int declarations cause pointer-initializer warnings in the
mirror; this is explicitly not a warning-free build. Actual diff review finds
only original restoration and removal of the coupled header additions. No
user configuration, media, snapshots or permanent tests change. Frontier
420/472; next 421 base/video/ega_vide.c. Other qualified decisions remain open.

P123 owned probe/build processes exited and their PIDs are absent. Exact scratch
cleanup and residue check pass. Mirror/documentation gates and final diff
review pass; final-source evidence, not the preliminary build, owns delivery.

## File 421: ega_vide.c (P124)

Pinned XP raw SHA-256 96981fb8adfd00b298c1ab0486d17e328639b11d49f2030036ebdd1e37d5d131.
Complete-file comparison identifies one selected addition: thirteen lines in
recalc_text, including its hardware/source explanation. Existing excluded
bodies and associated conditional/line-control scaffolding are skipped without
new accounting. No other selected declaration or executable change is present.

R0517 concerns the VGA V86 branch reading the current virtual CRTC height.
Actual original BIOS font handlers call recalc_text for user and built-in
font heights. The supported SPC386/no-GISP_CPU path enables support/ios.c
virtual I/O; inb/outb enter IOVirtualised, which executes the ROM I/O request
in the guest context. Physical get_screen_height cannot represent a different
VM's virtual CRTC. The addition preserves the original EGA low-resolution case,
non-V86 fallback, existing vertical multiplier and row-rounding convention.
It saves/restores the CRTC index and combines both VDE overflow bits.

Counter-review considers coherent removal of the entire branch, not a broken
partial rewrite. A private actual ega_vide.c copy restores that exact original
selection and links ahead of the unchanged machine archive. Both controls and
restorations use the existing vga_frame_smoke fixture. Its actual recalc_text,
SAS and BIOS virtual-I/O paths run with only the nested guest executor replaced
by controlled virtual registers. The 36 combinations cover VGA native/V86 and
EGA V86, two extents, three font heights and both multipliers; assertions check
rows, font height, page length and index restoration before later CRTC writes.

Historical evidence remains distinct: T99 S37 recorded actual Win3.1 VDD
execution from an existing snapshot, with a probe-issued BIOS font request.
Virtual 400-line CRTC versus physical 481-line calculation yielded 60/30 rows
instead of 50/25 for 8/16-dot requests; the repaired probe returned 50/25.
This is not a new Win3.1 run in P124 and not proof that a particular unmodified
guest application naturally issued those requests. See the existing S37
original-device audit evidence; no guest binary or media is changed here.

Final dynamic results: private original copies compile/link successfully on
both architectures. Current controls pass (x86 7.53 seconds, x64 5.62 seconds).
Both original restorations fail the expected BIOS row-count assertion at
vga_frame_smoke.c:2004, not a build or fixture-start failure. x86 exits
3221226505 after 13.29 seconds; x64 exits 3 after 10.18 seconds. The controlled
matrix is component integration proof, not a repeat of the historical VDD run.

R0517 is evidence-qualified pending owner decision: retain the existing
thirteen-line branch (+13/-0 versus XP). Its condition, index preservation,
register reads, overflow assembly and original scaling/rounding each serve
the single current-VM computation; no new helper or special guest version is
introduced. No safe minimization or other selected difference was identified.
Production/test delta and mirror-diff reduction are zero. No full rebuild,
published EXE replacement or full runtime rerun is required for this audit-only
delivery; P123 publication and P114 full 392/392 baseline remain unchanged.
Frontier 421/472; next frozen file 422. All qualified owner decisions remain
open, including R0517; S1 is not closed.

P124 owned runner exited and its PID is absent. Scratch preview/removal and
residue check pass. Actual diff review confirms documentation-only delivery;
documentation gate passes and user configuration/media/snapshots are intact.

## File 422: ega_writ.c (P125)

Pinned XP raw SHA-256 7afd11d2b5d392f74d0a6ddb1b97aee587a6808ea305d09f4c8a9d3477a06cbb.
Complete-file comparison finds the six omitted original local C-EVID writer
declarations. Other differences are existing excluded bodies and associated
conditional/line-control scaffolding, skipped without new accounting.

R0518 restores the six exact XP declarations and removes the now unnecessary
nine-line added writer declaration block from cpu_vid.h. Production registration
remains the original Glue_writes table in ega_writ.c; providers remain ev_glue.c.
No new callback wiring, conversion or runtime path is introduced. In the actual
supported non-ANSI macro selection IPT2/IPT3/IPT4 expand to empty argument lists;
the original fill_word declaration's IU8 spelling is preserved, not modernized.
Two existing direct test consumers of byte/word writes receive local private
extern declarations. No production header extension is retained solely for tests.

This coherent restoration supersedes the remaining writer part of R0423/P76.
The old isolated deletion result did not restore the original local declarations
and was insufficient evidence for retention. Together P123 (three readers) and
P125 (six writers) remove that entire added provider declaration block. The
remaining cpu_vid.h differences are existing selection/exclusion scaffolding;
no selected provider declaration residual remains pending there.

Actual delta: production +6/-10 (net -4), tests +2/-0 (net +2). XP-relative
raw added/deleted counts: ega_writ.c +46/-336 becomes +45/-330; cpu_vid.h
+17/-13 becomes +8/-13. The combined reduction is 16 difference lines
(-10 added/-6 deleted), not a claim based on heuristic hunk count. Excluded
bodies are unchanged and not newly accounted. Both mirror files retain CRLF.

Final cached builds pass: x86 14.69 seconds, x64 18.47 seconds. The actual
rebuilt VGA frame fixture passes x86 in 6.97 seconds and x64 in 5.64 seconds.
Build warning lines number 1009 and 1059 respectively, all in softpc.new;
no public-component warning is present. Published artifacts:

- x86: 51a4c9a4d0554b0a190d75b68367995af192254da93f8e06e0e1c892031bffaf,
  18:39:13 Nov 13 2026.
- x64: 75ec3fa9e12b7e565aaa69d0812b4eb8a896e247ec8622837b858a2bc1befceb,
  18:39:35 Nov 13 2026.

Each full EXE matches the P123 published baseline after masking only PE
creation timestamp/checksum and the unique eight-character compiler time;
compiler date and literal offset remain identical. Under the owner's confirmed
artifact-equivalence rule, full runtime tests are not repeated; P114's 392/392
remains the full-suite baseline. This is not a new desktop or guest verification.
Frontier 422/472; next frozen file 423. Other qualified decisions remain open.
Runner PID 25912 is absent and exit.txt records zero; no active job is replaced.

P125 scratch preview/removal and residue check pass after owned process exit.
Mirror/documentation gates and actual source/test/artifact diff review pass.
User configuration, snapshots and guest media remain untouched.

## File 423: egwrtm12.c (P126)

Pinned XP raw SHA-256 c8719090315333503c86824121a6342dca3749903a677b001faa75005b35e838.
Full-file comparison finds ten selected changed lines; existing excluded bodies
and associated scaffolding are skipped without new accounting. Root CMake
includes the file in softpc-machine-devices. Original ega_write_routines_update
selects its mode-1/mode-2 byte/word routines into bios_ch2_*_wrt_fn. Original
video.c invokes those callbacks for non-V86 CHAIN2 BIOS text writes. Compat
and Machine do not provide duplicate implementations of these routines.

Each difference is evidence-qualified pending owner decision, +1/-1 per point:

| Point | Current line / XP line | Necessary behavior shown by independent restoration |
| --- | --- | --- |
| R0519 | 298 / 353 | Mode-1 byte offset preserves plane address A0. |
| R0520 | 332 / 387 | Mode-1 word start preserves plane address A0. |
| R0521 | 340 / 395 | Mode-1 odd WORD second byte reaches next plane-0 cell (+8). |
| R0522 | 346 / 401 | Same next-cell step for plane 2. |
| R0523 | 631 / 686 | Mode-2 byte offset preserves plane address A0. |
| R0524 | 702 / 757 | Mode-2 word start preserves plane address A0. |
| R0525 | 717 / 772 | Mode-2 logical odd WORD advances plane 0 by +8. |
| R0526 | 729 / 784 | Same logical next-cell step for plane 2. |
| R0527 | 767 / 822 | Mode-2 simple odd WORD advances plane 0 by +8. |
| R0528 | 773 / 828 | Same simple next-cell step for plane 2. |

The ten original-line variants compile the actual source body, each reverting
only its named line, then link ahead of the unchanged production machine archive.
An additional variant restores all ten together, avoiding a conclusion based
only on incomplete restoration. A disposable copy of the existing VGA fixture
uses actual machine create/reset and BIOS mode 3, sets the original sequencer/GC
ports, and asserts callback identities before invoking those selected writers.
It covers mode 1 and mode 2, no logical function and AND, byte offset 2, even
WORD offset 6 and odd WORD offset 9. Latches/values are all ones and planes
start zero. Expected positions are explicit cells 8/10, 24..27 and 33/35/40/42,
not a copy of the implementation's offset expression. The selected four-byte
plane layout and the original controller read/write path already have separate
P120/P61 evidence; no new layout or renderer is introduced here.

Both current controls pass (x86 0.19 s, x64 3.10 s). All twenty independently
restored bodies compile/link successfully and fail the corresponding byte,
even-WORD or second-byte plane assertion. x86 assertion exits are 3221226505;
x64 exits are 3. Both full ten-line restorations also fail the first mode-1
byte-address assertion. No build error is counted as a behavior failure.
An initial private fixture used undeclared c_setAX and failed compilation;
that setup was corrected to the existing c_setAH/c_setAL interfaces only after
the first runner exited. This setup failure is excluded from the proof count.

A separate one-case mask probe confirms D006 on both architectures: plane mask
4, mode 2, no logical operation, value 4 at offset zero yields plane2=00.
Pinned XP contains the same bit-8 test; it is a newly recorded original defect,
not a reason to add an unapproved patch. The four-byte layout does not explain
away a wrong enable bit, and the ten retained address corrections do not fix it.
No all-mask, all-mode, overlap, paging or real DOS/Windows session claim is made.

No safe reduction of these selected differences is identified. Production,
permanent test and artifact deltas are zero; XP diff reduction is zero. The
existing ten changed lines remain for owner review, not automatic approval.
No full rebuild or full runtime rerun is needed for this audit-only delivery;
P125 artifacts and P114 full 392/392 baseline remain. Frontier 423/472; next
frozen file 424. S1 and all qualified owner decisions remain open.

P126 runners exited with their PIDs absent. Scratch preview/removal and residue
check pass. Documentation gate and actual diff review pass; delivery changes
only the three task evidence/status files. User media/configuration are intact.


## File 424: gfx_updt.c (P127)

Pinned XP raw SHA-256 3d8218d31fa1c9845900b431e5c25d1a6df6fa6ed61fb42d084a93ad3d323a4e.
The file is compiled into softpc-machine-devices on both architectures. The
original update selector and host timer dispatch its text/graphics painters;
original mode selection also calls set_mark_funcs. This is the original
producer/update machinery, not a replacement in Session or KVM.
Existing excluded bodies and their conditional/line-control scaffolding are
skipped without new accounting. The frozen frontier remains 423 until delivery.

### Restoration and remaining decisions

R0529 restores original spelling by removing added explanatory comments and
three trailing spaces. R0530 minimizes the existing ega_text_update guard:
remove four local copies and unsupported 160-column/200-row/32-pixel limits,
restore the original lines_per_screen expression, and retain only nonzero
stride, actual backing-store spans and the actual dirty-array capacity.
The latter capacity is sizeof(dirty)/sizeof(dirty[0]), 768 in the selected
configuration; it is not a new public text-size limit. This removes restrictions
which reject real CRTC-generated geometry. Zero stride cannot be restored
unconditionally: the actual original divide faults after a CRTC offset write.
The remaining guard is qualified pending owner decision, not automatically
approved. No extra update path or compatibility mode is added.

Other selected residuals, grouped by one shared representation/algorithm rule:

| Point | XP source interval | Residual and source justification |
| --- | --- | --- |
| R0531 | 310-565, 1868-2374 | Dirty scanning, painting and clearing preserve byte alignment within four-byte dirty groups; paired V7 rows retain the shared-group residue. Converting offsets to groups early drops bytes at row boundaries. Clear extent rounds up to include the final intersecting group. |
| R0532 | 596-602, 645-650, 831-1158, 1195-1234, 1517-1650, 2493-2748 | CHAIN2 text, split/wrap, both CGA banks and scrolling use the same existing interleaved-cell mapping and stride. WORD pointers advance four elements; DWORD pointers advance two. Keep zero-offset get_chain2_screen_ptr calls consistent with the mapping, as required by the owner; an isolated equal-base substitution is not an independent simplification. |
| R0533 | 673 | remove_old_cursor supplies the fifth paint argument, height=1. The actual host paint contract has five arguments; the original four-argument call exposes an indeterminate height. |
| R0534 | 1868-2374 | Panning marks the preceding group read by the output pixel dependency, including bank wrap; ascending traversal does not repeatedly propagate newly marked predecessors. Wrapped split paints both portions before clearing shared marks. Split heights use the correct host/scanline units and separately painted boundary rows are cleared once. These use the original dirty bitmap and paint dispatch, not a second renderer. |
| R0535 | 3830 | CHAIN2 graph mode selects the existing EGA bitmap marker. The original assert-only case leaves stale text/CGA marking when the actual controller changes mode. |

The selected ranges above include all changed text/dirty/paint/marker bodies;
there is no additional selected algorithm silently classified as formatting.
The grouped negative tests below prove the relevant behavioral dependency;
they are not represented as an independent negative for every repeated line.
The source review checks each repeated address/clear rule against its caller
and representation. R0531-R0535 remain evidence-qualified owner decisions.

### Actual-body comparisons

Private variants compile the real gfx_updt.c body with the production command
and link its object ahead of the unchanged machine archive. Current controls
pass on both architectures. Restoring the dirty group to original fails the
existing route-0 full-mark residue assertion (stride 80, residue 1, address
320). Restoring the complete text-mapping group, including both CGA banks
through XP line 1650, fails the independent incremental colour matrix in modes
04/05 on both widths. An earlier partial text restoration stopped at 1625;
it is superseded by this complete group and is not the final justification.

A focused real BIOS mode-3 to V7 mode-63 transition followed by c_sas_store at
0xa0100 produces bitmap[0x40]=1, low=high=64 with the selected marker. Restoring
the original assert-only branch leaves bitmap[0x40]=0, low=INT_MAX, high=-1
although dirty_total increments. Both originals fail the bitmap assertion;
current controls pass. This closes a gap in the older fixture which installed
mark pointers directly and therefore could not validate mode-selection wiring.

The actual remove_old_cursor body calls a five-argument recording painter.
Current height is 1 on both widths; original four-argument bodies yield
17651156 (x86) and -1082140016 (x64) in these runs and fail the height assertion.
Those incidental values are not a stable ABI promise; the missing argument is
the defect. A first undeclared probe entry failed compilation and was corrected
before this comparison; it does not count as behavior evidence.

### Geometry restoration evidence

Original BIOS mode 3, CRTC protection unlock, horizontal-display register 160
and offset register 81 produce 161 columns, stride 324 and 25 rows. The old
local guard paints nothing; the minimized guard paints 25 rows on both widths.
CRTC font height 1 and vertical-display/overflow writes produce 400, 768 and
769 rows. The old 200-row guard rejects the first two; the candidate paints
400/768 and rejects 769, preserving the actual dirty-array bound. These are
original controller and updater calls, not direct replacement geometry setters.

Writing CRTC offset zero then running the original two-tick mode update yields
stride/length zero. Current guarded execution returns; restoring the original
unguarded calculation faults with 0xc0000094 on both widths. The initial probe
omitted the ticks and therefore did not establish zero stride; that setup
failure is excluded. The initial wide-column probe omitted the CRTC protection
unlock and similarly did not establish the requested width; it is excluded.

The permanent regression extends vga_frame_smoke with the CRTC geometries and
actual backing-store boundaries.
Its first placement after the virtual-CRTC test inherited that test's vertical
multiply flag: observed 800 lines, stride 648 and length 32400 instead of
400/324/8100. Reset did not undo the test's direct flag assignment. The new
regression runs before those state-mutating probes and restores BIOS mode 3
on exit. Sequential runs also erase an old cursor through a separate two-byte
paint: 400 full rows plus one cursor erase is 401 callbacks, not a row defect.
The assertion counts full-row paints separately; it does not disable cursor
behavior or change production to accommodate a test.

No new guest session, every malformed snapshot combination, all paging modes
or complete graphics hardware conformance is claimed. The guard is not presented as a validator for arbitrary corrupted snapshot state.
Final-source dual-architecture build/full regression is in progress.

### Counter-review: reject the first capacity candidate

The first candidate retained screen_length <= 2*EGA_PLANE_DISP_SIZE while
relaxing 200 rows to the actual dirty-array capacity. Actual V7 register F6
bit 6 doubles that display span, but nt_graph.c still allocates video_copy at
0x20000 and EGA_planes at 4*EGA_PLANE_SIZE. A controlled private fixture gives
the updater larger scratch buffers and observes writes past those real limits,
without performing an uncontrolled heap overrun.

BIOS mode 3, extension unlock, F6=40h, CRTC offset=127 and 400 one-scanline rows
produce stride 508, screen_length 203200 and display bank 131072. The first
candidate writes 72128 bytes beyond video_copy's real capacity on both widths;
the pre-audit guard writes none. The first candidate is rejected, despite its
passing 188/188 background suites on both widths. Those passes are not final
qualification. A private-probe compilation initially used incorrect allocation
wrapper names; correcting only the probe to lib_allocate/lib_release establishes
the actual-body comparison.

The length check must include 2*screen_start and use the real backing size,
not the selectable display-bank span. A second counter-review also covers
visible columns wider than the row stride: 160 columns, stride 256, 512 rows
have screen_length 131072 but the last visible row extends another 64 bytes.
Checking the full-copy extent alone permits those writes; checking the final
visible-row extent as well rejects them. The two checks share the existing
failure return, with no additional painter, allocation or retry mechanism.
The candidate correction and final qualification are still being completed.


The final-row private comparison explicitly compiles both candidate bodies,
rather than mixing the concurrently rebuilt x86/x64 archive revisions. Both
architectures observe 64 out-of-capacity bytes with the full-copy-only guard
and zero with both extents checked. Its first diagnostic accidentally retained
stride 508; that run is not last-row evidence. Final private full VGA fixtures
with the seven planned geometry cases pass x86 (7.48 s) and x64 (7.07 s).
They establish focused candidate behavior, not final published qualification.

Counter-review of the guard uses the actual accesses: full copies touch
2*screen_start + screen_length cached bytes; dirty scanning's final visible
row ends at 2*screen_start + screen_length - stride + 2*columns. The interleaved
source uses four times that byte domain and is allocated four times as large.
The original CRTC fields bound these arithmetic inputs; this is not a claim
that arbitrary externally corrupted internal values are accepted safely.
Both spans must fit EGA_PLANE_SIZE, independently of the bank wrapping control.
The original nonzero-stride check precedes division, and dirty-record capacity
bounds the number of rows. No new allocation, producer state or hidden retry is
introduced. Earlier arbitrary column/font limits have no role in those bounds.

### P127 final-source qualification status (not delivered)

Actual production delta +9/-34, net -25; permanent test delta +70/-0.
XP-relative added/deleted diff delta is -29/-4, 33 fewer difference lines.
Existing exclusions are unchanged, not separately re-counted. Mirror CRLF and
non-mirror LF are preserved. Final normalized source SHA-256 is
d177cfcb603265f074c76c7a4145251639a16afa9044fc97dae46e71d5b4619a;
final test SHA-256 is
9cc84ae3374bd3b3fd9ad1f9d3478542d347423768289fa604113767120ba913.

Final cached builds pass x86 (9.71 s) and x64 (12.81 s); final VGA fixtures pass
5.91/5.57 s. Background suites pass 188/188 on each width, 137.21/140.27 s.
Both build logs have 89 warnings, all located in mirror source/headers, none
in shared components or autonomous test source. This is not a warning-free
mirror claim. Actual source/test diff, XP manifest and documentation gates pass.

Native suites pass only 2/8 on each architecture: Window retirement/modal pass;
audio lifecycle, SB MIDI, native MIDI, both package variants and Console display
fail. Audio failures are device-open assertions; package initialization fails;
Console display fails its 80*25 write assertion. The actual executing neko
session (ID 3) is Disc at both observation points. These are observed failures,
not passes or conclusively all explained by disconnection; a connected rerun
is required. The owner has been asked to reconnect. Full result is 380/392,
not complete acceptance, and no P127 commit/push/frontier advance is claimed.

Published candidate artifacts (not accepted delivery):
- x86-artifact: 19:36:48 Nov 13 2026; SHA-256 78d5b8cc2319f6ba53a642ee5908ad92cdd106bfeee1bb7c172231b4a94088c8.
- x64-artifact: 19:39:31 Nov 13 2026; SHA-256 a34feecb86871c2250091263c92db6909dc3b3795634926f9c73d7ebddc6aa1a.

Final delivery runner exited with code 1 because native tests failed. Process
inventory confirms no owned Python/compiler/CTest task remains. Scratch stays
under the active packet's build/t100-s1 pending connected qualification; it is
not archived evidence. Source investigation for this file is recorded, but file
424 remains undelivered and the frozen delivered frontier remains 423/472.

### P127 connected revalidation and delivery review

Owner requested retry and the executing neko session was Active. The native
CTest suites then pass 7/8 on each width (43.75/15.18 s): audio lifecycle,
SB MIDI, native MIDI, both package routes and both Window tests pass.
Console display alone still fails its initial 2000-character write assertion.
The owner reported another disconnection during this interval; after a fresh
Active observation, isolated Console CTest runs also fail. Do not attribute
this residual solely to disconnection or claim CTest 392/392.

A disposable instrumented copy of the actual Console fixture records its
setup before broker creation. x86 requests 120x60, but observes 42x14 before
console_broker_create; native fill reports 588. The backend retains that same
geometry, so this first failure precedes broker operation and does not execute
the changed VGA object. The instrumented x64 fixture passes without production
changes, while the original CTest launch remains sensitive. No shared code,
assertion, system setting or user configuration was changed.

The exact original test binaries pass with CREATE_NEW_CONSOLE and
STARTF_USESHOWWINDOW/SW_HIDE set at process creation. Fixed three-run sequences
pass on both architectures (x86 0.43/0.42/0.53 s; x64 0.41/0.40/0.40 s).
A further direct run uses the exact WORKING_DIRECTORY registered in
CTestTestfile.cmake and passes x86/x64 (0.44/0.47 s), with no changed arguments.
The binary SHA-256 values are:
- x86: 1f7587e3f20858edb4a2ccd1bdedacdda197e87e4e08ca4d9328eed9cba36c86.
- x64: 933a32e7d962d568ab0456ffbbee1da5cda5169228fe33fa343f134ade0e9328.

Running CTest itself in a new hidden Console, including its documented
interactive-debug option, does not fix the child's initial geometry. Thus the
qualification consists of 376 passing background CTest cases, 14 passing native
CTest cases and two original native Console binaries passing in explicit hidden
Console processes. All required test bodies ran unchanged; this is not a claim
that the default CTest launch is fixed. Its host-startup geometry limitation is
recorded here separately from mirror behavior, with no unrelated shared patch.
An exploratory headless-host launch also exited zero but is not needed for or
counted in the acceptance evidence.

Coordinator review re-read the actual production/test diff, the original-first
request and failure controls. Restored text reduces XP-relative difference;
retained guards correspond to proven division, array and memory-span needs.
The two unsafe intermediate candidates were rejected before delivery. There is
one original update/paint path, no alternate renderer or generated build input.
Final binaries, source and fixture hashes match their verified final builds.
Qualified R0530-R0535 remain owner decisions; D001-D006 and L001 stay open.
This completes file 424's delivery scope, not S1 or T100. Frontier 424/472;
next frozen file is 425 base/video/gvi.c.

P127 owned-process inventory is empty. Exact scratch preview/removal and build
residue check pass; reusable caches and user configuration/media remain intact.
XP manifest, documentation gate and final actual-diff checks pass.


## File 425: gvi.c (P128)

Pinned XP raw SHA-256 6f70695bc6cdb8db4bef8c42580b7a672cd601ea9d8da4d08c4dd8e1bda40a03.
A full-file line comparison, ignoring existing #line scaffolding and replacing
only already excluded bodies at their #error locations, finds no selected
residual difference. No excluded branch is re-audited, counted or newly logged.
The full selected declarations, globals, recalc_screen_params, gvi_init and
gvi_term match XP spelling and behavior. The original conditional structure
around host_init_adaptor/host_clear_screen also remains intact.

Root CMake line 205 includes the file in the production device target; both
Ninja trees contain its real compilation edge. Original BIOS reset.c calls
gvi_init, and gvi.h's original geometry setters call recalc_screen_params.
There is no reason to remove the file or replace the original geometry code
with Compat. An absence of direct gvi_term callers does not make the complete
translation unit unused or justify a new mirror deletion.

Executor comparison and coordinator counter-review agree: no source, test,
manifest, build or binary change is needed; no new residual decision is opened.
Production/test delta is zero and XP diff reduction is zero. P127 verified
artifacts and its precisely qualified native-launch evidence remain unchanged;
no new build or runtime verification is claimed for this documentation-only P.
Frozen frontier advances to 425/472, next file 426 base/video/v7_ports.c.
S1, T100, existing owner decisions and original-defect entries remain open.

## File 426: v7_ports.c (P129)

Pinned XP raw SHA-256 7abc5b3922bff2321225493b5aa91f1edb866d3e345cb4a208964cccc306b875.
Full selected-file comparison finds exactly one residual: R0536, XP line 679
in draw_v7ptr. Existing #error exclusions and necessary #line are skipped
without new bookkeeping. No other selected spelling or semantic change exists.

XP tests get_seq_chain4_mode() && get_chain4_mode(); current code tests
get_seq_chain4_mode() && get_256_colour_mode(). This is +1/-1 relative to XP,
with no new source change in P129. The original halving and drawing path remain.
Root CMake includes v7_ports.c in both production device builds. Actual pointer
register writes invoke draw_v7ptr, which calls the selected paint_v7ptr callback.

Executor analysis traces SR4 to vga_seq_mem_mode: changing CPU chain4 changes
EGA_CPU.chain, get_chain4_mode and the original read/write routines. V7 FC owns
the separate sequential_chain4 display state; the colour state is separate too.
Thus CPU addressing can change while the packed display interpretation stays
the same. Using CPU chain4 to decide pointer coordinates then moves the pointer
without changing its position registers or display interpretation.

Counter-review tested coherent restoration of the only selected difference,
not a stub or a new alternate renderer. Private current/restored translation
units use each architecture's actual Ninja production compilation command and
the existing VGA fixture link, overriding only the v7_ports object. Both compile
and link. Current runs pass (x86 6.18 s, x64 7.29 s); exact XP-condition runs
both fail the same existing assertion at vga_frame_smoke.c:1432:
surface[6 * width + 8] == 0xff. Failure exits are 3221226505 and 3 respectively;
neither is counted as a successful test or an interactive timeout.

The fixture enters V7 modes 66-69 through the real BIOS, changes SR4 through
outb, checks CPU address selection independently, then writes original pointer
position registers with X=16 and Y=6. Its pixel oracle expects display X=8.
Both current full-fixture controls cover the complete mode/origin/SR4 sequence;
the restored negative runs stop at their first failed pixel assertion, so they
do not prove failure at every later matrix member. This is actual original-body
and controller-path evidence, not a new interactive Win3.1/RDP reproduction.

R0536 is evidence-qualified for owner keep/restore decision, not automatically
approved. No redundant Compat implementation or smaller equivalent XP
restoration was identified. Source/test delta and mirror-diff delta are zero;
no shared code, user asset or published EXE changes. P127's exact build and
native-launch qualification remains the artifact baseline. P129 needs no
product rebuild or unrelated full regression. Owned probe work has finished;
concise evidence is promoted before scratch removal and documentation gates.
Frontier is 426/472; next file 427 base/video/v7_video.c. S1/T100 remain active.

## File 427: v7_video.c (P130 verification in progress)

Pinned XP raw SHA-256 d7a42d240c51ad83b58182a86ea7073f748810c0a07dc17d20ba9a3ab36b3651.
Full selected-file comparison finds one semantic group, R0537, in
v7vga_extended_set_mode at XP lines 293-296. Existing exclusions are skipped
without new bookkeeping. The current patch moved mode extraction before
validation and added an explanation. XP validated raw BL before extracting
the low-seven-bit mode number. The production device target includes this
file on both widths; ega_video's BIOS function 6F dispatch selects this function
for AL=5. Original standard-mode compatibility calls also reach it.

Executor analysis: is_bad_vid_mode already masks bit 7, but is_v7vga_mode does
not. Raw BL=E7 therefore fails the original extended-mode validation instead
of selecting mode 67 while preserving memory. The original later ega_info
update and screen-clearing condition already implement the preserve flag.
No additional state or Compat device implementation is needed.

Coherent exact restoration of this whole group was compiled and linked into
the real existing VGA fixture using each Ninja production command. Current
controls pass on x86/x64 (7.01/7.31 s). Exact XP restoration fails both at
vga_frame_smoke.c:1800, Currently_emulated_video_mode == 0x67, after a real
BIOS request with BX=E7 (exit 3221226505/3). Earlier same-mode requests alone
can conceal rejection, so the different-mode assertion is material evidence.
This does not claim a new interactive Win95 boot or guest reproduction.

The minimal candidate restores XP assignment placement and blank lines and
removes the added comment. Only the V7 predicate masks its argument:
is_v7vga_mode((getBL() & 0x7F)). Double parentheses are required by the original
macro's unparenthesized parameter uses; the macro itself is not changed.
c_getBL simply reads GET_BL with no mutation. The unchanged bad-mode predicate
already performs the same mask. BL is not modified by these validations, and
its original unmasked value still supplies the preserve flag later.

Independent counter-review confirms one original setter/clear path, no guest
special case, helper, alternate path or public-component change. Private
minimal-version fixture runs pass both widths (7.89/7.41 s), including every
existing V7 text/graphics mode, preserve/clear behavior, different-mode request,
banking and invalid FF rejection. No new test duplicates are necessary.
R0537's remaining +1/-1 is evidence-qualified pending owner keep/restore choice.

Production worktree delta is +3/-3, net zero; test delta is zero. Relative to
fixed XP selected source, before +3/-3 becomes +1/-1: two added and two deleted
difference lines removed, total mirror-diff reduction four lines. Final
LF-normalized source SHA-256 is
116df4a0f269bd7a0af14b6cb23469061e4b83a44e39607c95490377a4c96fcd.
Manifest is synchronized. Final dual-width builds, publication and regressions
are in progress; file 427 is not yet counted delivered.

Final-source verification subsequently completed its first pass. Cached builds
pass x86/x64 in 8.98/10.74 s; final VGA fixture passes in 6.85/5.74 s. Background
suites pass 188/188 on each width (143.08/138.39 s including runner overhead).
Each build emits 89 warnings, all in retained mirror source/headers; no shared
or autonomous-source warning was found. The EXE code section changes, so the
metadata-only exemption is not used. Publication verifies fresh compiler stamps:
- x86 Nov 13 2026 20:13:06; SHA-256
  53c0027cf9fe07eee45f04b6b33b87da5a952529d767d8260b988961369651ab.
- x64 Nov 13 2026 20:15:54; SHA-256
  4ad33dbde1326b8f7cbd26d8dbc35150d102406cb92f17a2bf3ad4383cabaf62.

Native suites are only 2/8 per width: both Window tests pass; audio lifecycle,
SB MIDI, native MIDI, both package variants and Console display fail. Actual
session inventory repeatedly reports neko session 3 Disc. Audio fails opening
the device, package reports cannot compose machine, and Console fails its
initial write contract. These are not reclassified as passes, nor is the current
patch blamed or absolved solely by a disconnected-session observation. An
online retry of the affected cases remains required. The separate P127 Console
launch qualification is historical evidence, not this batch's successful run.
The background runner has completed; no interrupted process or timeout is
counted as a test result. Source/manifest and documentation gates pass. P130
remains uncommitted pending native qualification; scratch is retained only for
the active packet, and file 427 does not advance the delivered frontier.

P130 follow-up independently exercises the unchanged Console binaries with
CREATE_NEW_CONSOLE and STARTF_USESHOWWINDOW/SW_HIDE, using their exact registered
CTest working directories and no argument changes. Both pass (x86 0.44 s,
x64 0.38 s). Their SHA-256 identities are respectively
1f7587e3f20858edb4a2ccd1bdedacdda197e87e4e08ca4d9328eed9cba36c86 and
933a32e7d962d568ab0456ffbbee1da5cda5169228fe33fa343f134ade0e9328.
This is fresh P130 execution evidence, not inheritance from P127; it does not
repair or count as a passing default CTest Console launch. The session is still
Disc. Five native cases per width (audio lifecycle, SB MIDI, native MIDI and
both package variants) still require online retries before P130 delivery.

Owner subsequently authorizes temporary desktop-verification deferral and
continuation of other work. This supersedes the preceding delivery blocker,
not its failed evidence. P130 delivers under that explicit limitation; online
audio lifecycle, SB MIDI, native MIDI and both package variants per width
remain pending until the owner reports desktop availability. No test is removed
or reclassified as passing. Default CTest Console launch remains separately
qualified by the exact unchanged binary's explicit-Console execution above.
Coordinator actual-diff review confirms only original-order restoration and
the one necessary V7 argument mask; no second path, guest change or shared
API change. Final source, manifest and published hashes match the recorded
verification. Owned jobs have exited; scratch can be removed after promotion.
Frontier advances to 427/472, next file 428 base/video/vga_mode.c. S1/T100 and
the outstanding owner decisions stay open; this is not complete acceptance.

### P131: online qualification supplement and test-scope clarification

P130 is committed/pushed as b766bd5c. On owner retry, actual session 3 is
rdp-tcp#0 Active. Without rebuilding or changing sources, the five deferred
tests pass on both widths: audio_lifecycle_smoke, sb_midi_native_smoke,
native_midi_output and both package_smoke variants. Runner times are
x86 5.72 s and x64 5.36 s. Earlier disconnected failures are retained above.
Together with P130's 188 background cases, two Window cases and explicit-hidden-
Console execution per width, all required bodies have now passed. Default
CTest Console startup is not claimed repaired. No new guest-session observation
or source modification is inferred from these test results.

Owner clarifies that this V7 BIOS predicate change did not itself require native
audio/desktop testing; those cases were scheduled as full-regression policy.
For subsequent batches that do not affect those paths, defer native desktop,
audio and package checks until final acceptance rather than block ordered work.
The proposal and CURRENT reflect that authorization. Final native acceptance
remains required. P131 changes documentation only; frontier remains 427/472,
with file 428 vga_mode.c under investigation and no edit to it yet.

## File 428: vga_mode.c (P132)

Pinned XP raw SHA-256 56895920930e023f46ecebe22639b0462c920b4b4a25a0908fb8137b22d6e1e8.
Full selected-file comparison finds added explanations and three changed
conditions in choose_vga_display_mode. Existing excluded bodies and required
line scaffolding are skipped without new records. Root CMake compiles this
file in both production device targets; vga_init installs the chooser and
actual controller changes select it. No duplicate Compat mode selector exists.

R0538 removes two added explanatory lines and compacts the packed-display
condition. Its additional V7VGA wrapper was local, not original XP structure.
All supported device builds explicitly define V7VGA; gvi.h defines the queried
state independently. Removing this added wrapper restores the original single
if layout without deleting any original conditional or changing supported
preprocessing. No non-V7 build support is newly claimed or introduced.

Three remaining semantic points are evidence-qualified, pending owner choice:

| Point | XP line | Necessary residual and evidence |
| --- | --- | --- |
| R0539 | 487 | Packed V7 display retains four-byte address units while CPU chain4 is off. Exact restoration fails actual BIOS/SR4 display stride at fixture line 1380 on x86/x64. |
| R0540 | 492 | Planar 256-colour display uses per-plane stride independently of CPU odd/even. Restoring the original condition doubles the stride and fails line 1762 on both widths. |
| R0541 | 519 | Planar wrap uses the same per-plane address units. Restoring only this condition fails the wrap oracle at line 1764 on both widths. |

Private variants compile the actual source with the real Ninja command and
override only its object in the existing VGA fixture link. Current controls
and comment-only cleanup controls pass on both widths; each individual
restoration fails as above. Coherent restoration of all three conditions also
fails at line 1380 on both widths, so the result does not depend solely on
isolated, potentially incoherent restoration. Negative runs stop at their
first failed assertion; they do not establish failure for every later case.

Counter-review additionally checks register reachability rather than relying
only on the fixture's direct state matrix. A private copy of the actual fixture
enters BIOS mode 13, unlocks V7, clears FC sequential flags, writes SR4=02,
GC5=50, GC6=07, CRTC offset=28 and start=C200. Both widths report colour=1,
memory_chained=1, chain4=0, sequential_chain4=0, actual stride=80 and start=49664.
Current code derives stride=80, length=16000, wrap=1. Restoring only R0540
derives stride=160, length=32000. Restoring only R0541 retains stride=80 and
length=16000 but reports wrap=0, although 49664+16000 exceeds 65536. Current
register-fixture controls pass; both negative variants fail on both widths.
The first private fixture compilation lacked its original relative-header
search directory; adding that original directory fixes the experiment setup.
That setup failure is not a production defect. No tracked test is modified.

Final change is production +1/-7, net -6; tests +0/-0. Fixed-XP selected diff
changes from +9/-3 to +3/-3: six fewer added lines, unchanged deleted lines.
All three necessary conditions remain in the original chooser; no new state,
helper, product special case or alternate renderer is introduced. Final source
LF-normalized SHA-256 is
f2d8484de4ef2fc4f545c537174fe2869a2b43d47649058ffd6c98ac54359629.

Final cached builds/publications pass x86/x64 in 10.09/10.88 s, with zero build
warnings. Final VGA fixtures pass in 6.71/5.24 s. Both EXEs compare exactly to
their P130 predecessors after masking only the PE COFF timestamp, optional
header checksum and the single NUL-terminated compiler-time literal. No code,
data, layout or other bytes differ. Thus the owner-approved equivalent-artifact
exception applies: no redundant full or native regression. P130/P131's precise
test qualification remains the baseline; no new interactive guest run is claimed.

- x86 compiler stamp Nov 13 2026 20:49:47; EXE SHA-256
  b51aa658eecf222d4846c65352918937e09185ec45c54061a86d03982e015bae.
- x64 compiler stamp Nov 13 2026 20:50:04; EXE SHA-256
  4c06f759544a6e85e25e2e37e173afcc363f90d400126ad8c1c8818d15190c0a.

Executor and coordinator actual-diff review confirm the fixed source baseline,
complete selected-file comparison, real caller path, negative controls and
unchanged production semantics of the cleanup. Manifest is synchronized.
Owned jobs have completed; promoted evidence permits scratch cleanup.
Frontier 428/472; next file 429 base/video/vga_prts.c. Owner decisions and
S1/T100 remain open.

## File 429 investigation stage: vga_prts.c

Intermediate record below is superseded by the P133 final qualification.

This record is not a completed-file disposition or a P delivery. Frontier
remains 428/472. Fixed XP raw SHA-256 is
57a3318256a0b9392329fd4afada8b9926b32345c4262f08c3f3324b65d291f2.
Selected differences comprise snapshot declarations/capture/restore, cursor
shape and disable-register storage, underline refresh, line-specific pel
panning, and attribute-register-only refresh. Existing HUNTER exclusions must
be removed from comparison alignment before assessing the appended snapshot
block; a raw sequence matcher incorrectly aligns that block with excluded
HUNTER functions. No excluded content is re-audited or counted.

Private real-object probes compare current code, exact original cursor shape,
exact original cursor-start register handling, coherent restoration of both,
and a reduced candidate. All use actual Ninja production compilation and the
existing VGA fixture link. Current controls pass x86/x64 in 6.75/6.77 s.
All three original restorations fail the cursor register/visibility oracle at
fixture line 788 on both widths (x86 3221226505, x64 3). The reduced candidate
restores the original reversed/equal-shape blocks and original end-bound and
assert expressions, while retaining initial disable/reversed guards and the
inclusive final height. It passes in 6.16/6.34 s. The reversed branch remains
unreachable behind the earlier VGA hiding guard; equality now uses the original
one-line cursor branch. This restores original text without restoring the
incorrect special treatment of end=0. No tracked production edit yet.

Snapshot source review finds actual archive.c capture/restore consumers and
write-only/internal state that ordinary port reads cannot reproduce. The V7
snapshot getter duplicates 28 register reads, but only four need values different
from the original read handler: A5 and F3 are masked, F9 returns a subfield, FE
is masked. A private candidate reuses vga_extn_inb for the remaining reads and
preserves the sequencer index around it, retaining those four raw-state cases.
The original selected register set does not include side-effecting F0/F2 reads.

A private copy of the actual VGA fixture assigns all 28 archived fields each
byte value 0..255, calls the real snapshot capture, asserts all captured values
and preserved sequencer/attribute indices, then restores its setup state. This
is 7,168 value assertions per run, not an interactive guest reproduction.
Current and reduced candidates pass x86 (6.10/5.96 s) and x64 (6.84/6.86 s),
including the existing complete fixture thereafter. A preparation attempt used
the wrong fixture-function signature and stopped before launching; corrected
preparation uses its actual machine parameter. No production failure is inferred.

Separate negative probes have now completed on both widths. Restoring the
original underline register path fails produced-frame assertion line 911;
restoring split-panning refresh fails dirty assertion line 507; restoring pan
register refresh fails dirty assertion line 496. Removing only the added
line-specific split behavior fails the existing resolved-pixel comparison.
That last probe isolates the helper's semantics; it is not described as coherent
restoration of all coupled painter changes. All negative runs exit through
assertion failure (x86 3221226505, x64 3), not timeout. This file still requires
complete snapshot-path review, final diff minimization and ordinary batch
qualification. Private candidates are not published or committed product
implementations. Task-owned scratch remains for that continuing work.

## File 429: vga_prts.c (P133)

Complete selected-source review uses the pinned XP file above. Comparison
parses conditional branch paths, omits each existing excluded body from both
sides, then compares retained text after removing line-control directives.
This avoids aligning the appended snapshot ABI against the excluded HUNTER
body. No excluded branch receives a new ledger entry or count.

The implemented candidate changes production +35/-57 (net -22), tests 0/0.
Selected XP diff is +233/-46 before, +188/-23 after: 68 fewer added/deleted
diff lines, independently of source net size. Original whitespace around the
NEC_98 boundary is restored with the corresponding original line mapping.
The final LF-normalized source SHA-256 is
8074e00b5a6415b514f33aa402874b64b4e042a1b91bc7f3be1f00015fa6af69.

| Point | Original location / local boundary | Executor disposition and concrete evidence |
| --- | --- | --- |
| R0542 | XP 577-630 and added snapshot block | Restore original reversed/equal cursor blocks, block-bound and assertion spelling; remove added explanations, duplicate V7 getters and redundant initialization. Original reversed block remains unreachable behind the necessary VGA guard. No executed second cursor path is introduced. Reduced candidate passes both actual VGA fixtures. |
| R0543 | XP 364, 455; appended snapshot ABI | Qualified pending owner decision: archive.c captures/restores live controller state on the executor, including V7 registers, C-VID latches, controller indices and DAC phase. Original vga_int_1C is a guest-memory BIOS service: it saves five sequencer registers, 25 CRTC registers, 20 attributes, nine GC registers and optional BIOS/palette state; it omits V7 and live latch/phase state and changes indices while capturing. It cannot replace the complete snapshot contract. Keep the existing copied-state ABI, using original port handlers for restoration and 24 of 28 V7 reads. A5/F3/FE port reads mask raw bits and F9 reads only a subfield, so these four raw reads remain. |
| R0544 | XP 577-630 | Qualified pending owner decision: VGA disable/reversed/out-of-font visibility, zero-end shape and inclusive normal end. Coherent original shape/register restoration fails actual fixture visibility; existing 8/14/16/32-height shape assertions pass the reduced candidate. Original equal-range branch produces the required one scanline. No guest-interactive claim follows from this fixture. |
| R0545 | XP 1480-1490 | Qualified pending owner decision: storing the complete start register preserves disable-only writes. Original code compares only start bits and reads the previous disable flag, losing the new disable bit. Exact original register block fails the same real write/visibility oracle on both widths. |
| R0546 | XP 1674 | Qualified pending owner decision: underline-only writes request existing refresh. Original path fails the produced-frame oracle on both widths. No separate drawing path is added. |
| R0547 | XP 2717 | Qualified pending owner decision: line-specific panning is consumed by original-host painters in nt_ega.c and the original dirty updater in gfx_updt.c. Split mode requires zero panning below the split. Removing this helper alone breaks those real callers; the isolated raw-only helper probe fails resolved-pixel comparison. This is source/coupled-contract plus fixture evidence, not a claim that all coupled painter changes were restored in that probe. |
| R0548 | XP 2802, 2836 | Qualified pending owner decision: split-panning and horizontal-panning register changes request the existing refresh when VRAM itself is unchanged. Separate exact-original restorations fail actual dirty assertions on both widths. |

Snapshot reset review distinguishes required derived-state initialization from
dead writes. vga_init already initializes DAC_data_mask and DAC_data_bits;
DAC phase/address temporaries have no intervening consumer before the final
saved assignments (their only readers/writers are DAC port handlers, which
register replay does not call). The temporary sequencer index 6 is overwritten
by the first V7 replay iteration before any consumer. These five statements
are deleted. Protection clearing and zero-state/chain consistency remain:
change-only original handlers otherwise retain unrelated live derived state.
Separate removal probes fail EGA_CPU.set_reset at fixture line 1856, character
height at 1323 and double-pixel width at 1318, respectively, on x86 and x64.
All failures are assertions, not timeouts. The small declaration additions
have actual copied-state/memset/reset-state consumers; no original header
supplies the added private state declarations.

The final candidate preserves all 7,168 raw-value checks and both selector
indices in the private real-body fixture, followed by the complete existing
VGA tests: x86 7.66 s, x64 7.00 s, both pass. An initial run overlapped another
probe using the same disposable media filename and failed its file-open
assertion; that run is invalid qualification, not a product defect. It was
rerun serially after confirming the other PID exited. Initial non-elevated
background launches exited without producing logs; authoritative PID queries
confirmed termination before the explicit hidden Python launches. No live
job was restarted on a timeout inference.

Coordinator counter-review inspected the actual patch and original caller
chains: restored text does not restore the wrong zero-end behavior; unsigned
end-minus-one underflow cannot reach the restored block test because zero
is either hidden or handled by equality first. Reused V7 reads preserve the
index and exclude F0/F2 side-effect reads. Raw four-register cases remain for
complete copied state. No Compat implementation was substituted for original
logic, no new helper/API or public-component change is introduced, and the
existing snapshot format/layout is unchanged. Necessary explanations move to
this evidence; the port-ABI reason marker remains in source.

At candidate freeze, cached builds/publication and background regressions
were pending; the results below complete that qualification. Native desktop,
audio and package qualification remains deferred to final acceptance under
the owner's unaffected-path authorization.

### P133 final qualification and delivery

x86: cached build/publication 9.20 s; final VGA fixture 7.02 s; background **188/188** in 142.43 s.
x64: cached build/publication 12.66 s; final VGA fixture 5.42 s; background **188/188** in 138.62 s.

Both builds report 89 warnings, all in mirror sources/headers; none in public or autonomous code. Mirror, documentation and whitespace gates pass.

- softpc32.exe: Nov 13 2026 21:09:40, SHA-256 7f410a64adbc479fe7db913d6d66c340137755f65da95dbd5bd07897e6816a5a.
- softpc64.exe: Nov 13 2026 21:12:19, SHA-256 c2688195129f767cb265fcb5d674f669a696f35d4a5d669d1bb9a1e7597d28a6.

Final-source identity, CRLF mirror policy and pinned XP raw identity were rechecked. Actual changed-path review confirms only this mirror source, manifest/provenance/task records and the two published EXEs change; no test, public-component, Compat, INI or guest-medium edit. Native checks are explicitly deferred, not counted as passes. No new interactive DOS/Windows guest reproduction is claimed. Existing qualification limits and pending owner decisions remain. Completed owned jobs and promoted evidence permit scratch cleanup before commit. Frontier 429/472; next file 430 base/video/vga_vide.c. S1/T100 remain open.


## File 430: vga_vide.c (P134)

Pinned XP raw SHA-256 is
9828bd882bcc99021605becf586fd2d9c71b43d1b3e2bbfc80f6f6e06cc3a267.
Whole-file comparison after line-control normalization finds that every
remaining replacement is exactly one existing #error body. Selected source
matches XP; no new point or excluded-body accounting is created. This is
selected-source equality, not raw-byte equality. Root CMake includes the file
in softpc-machine-devices; ega_vide.c calls vga_func_12 and dispatches
vga_int_1C. Required BIOS behavior has no replacement Compat implementation.
Executor and counter-review agree no edit/retirement is warranted. Production
and tests unchanged; P133 final-source binaries and qualification still apply.
File review complete; delivered frontier remains 429 until this documentation
batch is pushed. Next file 431 base/video/video.c.


## File 431: video.c (P134)

Fixed XP raw SHA-256:
90bf0cf3f869903e6b24e0402c8b29c7d89b50a1c3ceb77e92e06f121f73a40f.
Root CMake compiles this required BIOS implementation in softpc-machine-devices.
Original EGA/VGA BIOS dispatch reaches its text and CGA pixel operations; no
Compat substitute exists. Compare the complete file by conditional branch
paths, omit existing excluded bodies and line directives, and normalize only
conditional-directive comments for scaffolding comparison. Remaining selected
changes are the BDA text-aperture expression with eleven uses, two odd/even
pixel offsets, explanatory comments and misplaced blank lines. No excluded
body is counted or reopened.

| Point | XP location | Initial analysis and counter-review |
| --- | --- | --- |
| R0549 | Blank placements throughout; added explanations at 398, 2539, 2559 | Restore original blank placement and remove added explanations. Four existing line directives are adjusted to keep original diagnostic anchors (113, 170, 3433, 3939); no new directive or conditional framework is introduced. Production +17/-23, tests unchanged. |
| R0550 | Macro at 398; text origins/bounds at 1223, 1476, 1554, 1556, 1741, 1972, 2098, 2267, 2381, 2938, 3107 | Qualified pending owner decision: nine text origins and two clear limits derive from the existing caller BDA table. Original cached bounds are set by physical mode programming in ega_vide.c, whereas original BIOS dispatch reads the active caller's BDA and can already use virtualized SAS access. No new state/VM-version branch exists. Coherently restoring all eleven uses and deleting the macro fails the actual BIOS B8000 character oracle on both widths. Both bounds must remain consistent; the current fixture also checks full-width clearing. |
| R0551 | 2539, 2559 | Qualified pending owner decision: both XOR and replacement CGA writes preserve the existing odd/even plane address spacing before OR-ing the low plane bit. Original shift halves the physical group offset. Coherently restoring both expressions fails BIOS write-dot/read-dot comparison on both widths; production CHAIN2 layout evidence is established in the earlier audited writers. No alternative drawing implementation is introduced. |

R0550's supported runtime context is also documented in the prior
[S33 guest investigation](m9-t99-s33-windowed-dos-addressing.md): actual Win31
VM=1 BIOS writes had BDA mode 3 with the physical cache A0000; changing the
address source restored typed text. That is historical guest evidence, not a
new interactive run. This audit independently rereads the actual cache-setting,
text-only dispatch/SAS paths and executes fresh original-object controls.
The current focused fixture deliberately supplies stale physical bounds and
does not claim to emulate a complete paged V86 environment. V7 extended text
uses original BDA mode 1 plus its extension latch, with the same B8000 aperture;
original mode programming and graphics-address callers remain unchanged.

Private probes compile the actual device command and replace only video.c's
object in the existing VGA fixture link. Current controls pass x86/x64. All
three original candidates (text only, both pixel expressions, both groups)
fail by assertion: text at line 1475, pixel/combined at 1453. The latter matrix
covers BIOS CGA modes 4..6, x=0..19 and y=0..2; it does not establish every
arbitrary CGA coordinate or all V86 graphics behavior. Existing XOR uses the
same address calculation; no separate XOR-only negative is claimed.
- x86-video-current-run: exit 0, 6.03 s.
- x86-video-text-original-run: exit 3221226505, 10.76 s.
- x86-video-pixel-original-run: exit 3221226505, 11.97 s.
- x86-video-original-run: exit 3221226505, 11.75 s.
- x64-video-current-run: exit 0, 6.91 s.
- x64-video-text-original-run: exit 3, 11.36 s.
- x64-video-pixel-original-run: exit 3, 36.11 s.
- x64-video-original-run: exit 3, 11.26 s.
- x86-video-minimal-run: exit 0, 6.43 s.
- x64-video-minimal-run: exit 0, 6.89 s.

All private jobs were sequential against their task-owned media file and
completed before final qualification. The reduced source changes no executable
expression: it removes comments/blank differences and preserves diagnostic
mapping. Selected XP diff falls from +31/-24 to +14/-13: 28 fewer diff lines,
not merely the six-line net source reduction. Final normalized SHA-256:
a2db040e40056f6a4e268681d00091bd474974a47ff2e4e71b43b221d3b43139.
Final tracked builds/publications and focused tests are pending. Full regression
may be omitted only if each EXE is demonstrably equal after the permitted
PE timestamp/checksum and unique compiler-time masks. Native qualification
remains deferred to final acceptance under the owner's unaffected-path rule.

### P134 final verification and actual-diff review

Final x86/x64 cached builds and publication pass in 9.40/10.14 s; rebuilt VGA
fixtures pass in 6.31/4.98 s. The exact published files are byte-equivalent to
P133 after masking only PE COFF timestamp, optional-header checksum and the
single NUL-terminated compiler HH:MM:SS string. No instruction/data/layout or
other byte is ignored. The owner-approved equivalence exception therefore
avoids repeating full background/native suites; P133 188/188 per width remains
the applicable background baseline. Final native acceptance remains pending.
Both builds have 89 original-mirror/header warnings and zero outside the mirror.
Mirror, documentation and whitespace checks pass.

- softpc32.exe: Nov 13 2026 21:23:34, SHA-256 ef980f3d133b509407764892343225aac49eae8d274ffaaf694532d105f41269.
- softpc64.exe: Nov 13 2026 21:23:50, SHA-256 c5833bf9ddf8b62a8cd6599ecc4c18282d906c65c73e34bdb8e1e01711dae43b.

Coordinator counter-review checks the actual source patch, the fixed original
comparison and probe oracles. No semantic site, public tree, Compat, guest,
INI, media or snapshot is changed. Diagnostic anchors are verified against
original matched retained lines, not guessed offsets. Retained R0550-R0551
remain evidence-qualified owner decisions, not automatic approvals. Final
mirror bytes equal the tested reduced candidate and retain CRLF. Owned jobs
are terminal; promoted evidence permits scratch cleanup. Completed delivered
frontier is 431/472, next file 432 base/video/video_io.c. S1/T100 remain open.


## File 432: video_io.c (P135)

Pinned XP raw SHA-256:
24ada0ef848aed12dba720b680338ca1f1c71b4a7b0c3485d7fbe850ad21b833.
Root CMake compiles this file in softpc-machine-devices. The original BIOS
entry table calls video_io, and bop_smoke asserts BIOS[BIOS_VIDEO_IO] is that
exact entry, distinct from EGA's 0x42 dispatch. Its check_video_func guard,
IDLE_video call and video_func dispatch match XP. No Compat replacement is
needed; deleting this live implementation is not justified.

R0552 restores only blank placement: one blank moves from before NEC_98 to
the selected else body, and two blanks move from before the excluded NTVDM
arm to after it. Existing line directives change 97->96 and 109->107 so the
original define/function/comment positions remain 97/99/109. No excluded
body is audited, restored, counted or given a new point. Executor comparison
and independent counter-review find no remaining selected code difference.
Normalized selected XP diff falls from +3/-3 to +0/-0: six fewer diff lines.
Tracked production changes +5/-5 include the two line-control adjustments;
test changes are zero. The retained file is not claimed byte-identical to XP.

Final source LF-normalized SHA-256:
7e300b8b5956ffb0525dce0f76c990b21af103db8e21a8fd99492102c54a2fad.
The complete final comparison, excluding existing error bodies and line
controls, equals the original; no semantic residual needs an owner decision
for this file. Cached final-source builds/publication and the real BOP/VGA
fixtures qualify the cleanup. Applicable equivalence/full-test disposition
follows below. Earlier pending decisions and final native acceptance remain.

### P135 final-source qualification

- x86: cached build/publication 8.86 s; BOP and VGA **2/2** in 7.48 s; 89 mirror/header warnings, none outside the mirror.
- x64: cached build/publication 12.96 s; BOP and VGA **2/2** in 7.50 s; 89 mirror/header warnings, none outside the mirror.
- softpc32.exe: Nov 13 2026 21:27:05, SHA-256 a709ae6b5215c21da4b0a72931846ee11ff02d7c54fb4d9811cfc12f5e597002.
- softpc64.exe: Nov 13 2026 21:27:21, SHA-256 75f9a7b042f7cf3743cac305cac137ec1a3bcb4afda47a44e4a8b60dbdef3180.

Both EXEs are byte-equal to P134 after masking only PE timestamp/checksum and
the one compiler-time literal. The owner equivalence exception skips duplicate
full regression; P133 background 188/188 per architecture remains applicable.
Native final acceptance is still pending. Manifest, documentation and diff
checks pass. Actual changed-path review confirms only original whitespace,
line controls, provenance/task records and fresh EXEs changed. No public tree,
Compat, guest, INI or snapshot changed. Completed owned jobs permit scratch
cleanup. Frontier 432/472; next file 433 host/inc/cfpu_def.h. S1/T100 and all
earlier qualified owner decisions remain open.


## File 433: cfpu_def.h (P136)

Pinned XP raw SHA-256:
5e1fff78ae51d0548295fc84d7396318e0d201be908eb70ea13d26682495d6cd.
Actual consumers are original ccpu386/fpu.c and zfrsrvd.c plus Compat's CPU
archive.c. The Compat cfpu_def.h wrapper includes this mirror and supplies
native fenv bindings; it does not duplicate the mirror's FPU implementation.
FPSTACKENTRY remains original and is used by snapshot get/setNpxStackRegs.
Deleting the mirror or its wrapper would remove a live, distinct responsibility.

R0553 restores the original absence of the extra FPU_I64 comment. The existing
two-line host-ABI reason marker remains as required by Coding Rules; it covers
these adjacent layout adjustments. Production +0/-1, tests unchanged; the
XP-relative selected diff decreases by exactly one added comment line. No
existing error exclusion or line-control framework receives new accounting.
The #line 46 anchor after the integer fields remains correct after deletion.

R0554 is the evidence-qualified, pending owner decision for six little-endian
representations: FP32 and FP64HI sign/exponent/mantissa bitfields, FP80SE
sign/exponent bitfields, FP64 words, FPU_I64 words and FP80 mantissa words.
fpu.c reads these representations directly in LoadValue, CVTFPHI64 and
extended conversion; wrong offsets cannot be cured by its existing byte-order
macros. CVTI64FPH computes high_word * 4294967296.0 + low_word. The selected
original BCD tables initialize low then high (their final entries are {1,0}
and {10,0}); restoring high-first would turn these into 2^32 and 10*2^32.
This is source proof of the integer requirement, not a claim of a new guest
BCD playback or arithmetic trace. No new Compat arithmetic is introduced.

Private comparison compiles the existing x87_layout_smoke with its real fenv
wrapper and only a substituted mirror header. Current, each of the six
individual XP type restorations, and all six coherently restored compile/link
successfully on x86 and x64. Current runs pass (0.11/0.22 seconds). Every
restored variant fails its corresponding assertion on both widths: FP32 at
line 67, FP64HI at 69, FP80SE at 73, FP64 at 58, FPU_I64 at 61, FP80 at 59;
coherent original first fails at 58. Negative exits are 3221226505 on x86 and
3 on x64, with explicit assertion diagnostics, not timeouts. The offset
assertions establish the representation boundary; source consumers above
establish why those offsets matter. These probes do not prove exhaustive
x87 opcode, rounding, exception, guest or snapshot compatibility. The existing
fixture additionally exercises native rounding and exception bindings.

Remaining selected content matches the fixed XP counterpart apart from the
six representations and required reason marker. No original guest, public
component, Compat implementation, configuration, media or snapshot changed.
Final LF-normalized source SHA-256: 5e5f4421f951f009607a426822c9617c242d612a36391c68972fa3d6269cce17.

### P136 final-source qualification

- x86 cached build/publication: 12.87 s; x87 fixture 1/1 in 0.18 s.
- x64 cached build/publication: 14.19 s; x87 fixture 1/1 in 0.18 s.
- softpc32.exe SHA-256: f7fcc9cbc4eed7cb897f6e7f6578741135e324400263f55564075bc2c76bc11d.
- softpc64.exe SHA-256: ad9eec8f276872dc5b2909a444fa7cebcf7cf234fc2caa05febb4f3da0cbd2c5.

Both delivered EXEs equal P135 after masking only PE COFF timestamp/checksum
and the unique NUL-terminated compiler HH:MM:SS literal. All other bytes and
lengths are equal. Under the owner exception, full regression is not repeated;
P133 background 188/188 per architecture remains applicable. Final native
acceptance is pending, not passed. Coordinator review checks the actual one-line
source deletion against XP, the unchanged six layouts, exact negative oracles
and current passing fixture; required ABI differences remain owner decisions.
Frontier 433/472 after this delivery; next file 434 host/inc/error.h. S1/T100,
earlier pending owner decisions and original-defect decisions remain open.

P136 gates pass: XP manifest, documentation governance and git diff whitespace.
Each incremental build emits four existing mirror fpu.c sprintf-declaration
warnings and none outside the mirror. Both owned jobs are terminal; scratch
is removed after promoting the evidence above. No native test pass is claimed.


## Files 434-435: error.h and host.h (P137)

Compared complete files with the fixed XP commit, in frozen order.
File 434 host/inc/error.h has only existing excluded ANSI, BUILDING_CPU_TOOL
and non-PROD bodies plus their necessary line controls. All remaining lines,
including original macros and declarations, match XP. These excluded bodies
receive no new point or accounting. Fixed XP raw SHA-256 is
938a9e29bcf81a76de3c15c02ac6dbdc2d1f976702d5bcd43160352135c7a672;
current LF-normalized SHA-256 is
8acb0a52f075ab97ebb0e582442db9d9f5d5ed5a28612702693960c7c1f926f8.

File 435 host/inc/host.h matches XP after CRLF/LF normalization throughout:
10801b826f2438763d70217c1a5883cdfb9bb25f080bea61bda3857c088dc1d0.
No source restoration or new retention decision exists for either file.
Ninja's VALID dependency records for the actual rom_basc.c object explicitly
include both mirror headers on x86 and x64. CMake builds rom_basc.c in
softpc-machine-devices; rom_basic calls host_error(EG_NO_ROM_BASIC, ERR_CONT,
NULL). Other actual video and device source consumers also include them.
The Compat system/error.h overlay selects the distinct original Base enum
for illegal-instruction handling; it is not a replacement implementation of
these host declarations. There is no basis to retire these live mirror files.

Executor whole-file comparison and coordinator review agree: production +0/-0,
tests +0/-0, XP diff unchanged. Only audit status/evidence change. No rebuild,
publication or test rerun is required for this read-only audit batch; P136
published EXEs and x87 results, P133 full background results and explicitly
pending native final acceptance remain the existing evidence, not new passes.
Documentation and whitespace gates qualify these records. No scratch was
created. Frontier 435/472; next file 436 host/inc/host_cpu.h is under investigation
and is not marked audited. S1/T100 and earlier owner decisions remain open.


## File 436: host_cpu.h (P138)

Fixed XP raw SHA-256:
2a5b8bc3a6d6cccdb2639f777be34f62b595ffbfe5c7ab7656fd41b194d3270c.
The actual cpu4.h includes this header, then generated cpu4gen.h. The latter
redefines selected CCPU setters as direct c_set* calls with their actual typed
prototypes. Whole src/app-softpc search finds setAX_func/setCS_func only in
this header's declarations/macros, not vector definitions or assignments.
The previous reason marker claiming these unused vector declarations were
necessary CCPU call-ABI contracts is not supported by current consumers.

R0555 restores the complete original common WRITE declaration block, including
original spelling, separate lines and trailing blank. Removes the local
MVDM-HOST-DIV-102 marker and now-unnecessary #line 419; the preceding #line 383
and restored original text already establish that original next location.
Generated CCPU declarations and actual calls remain unchanged. Production
+32/-16 (net +16), tests unchanged. Selecting current supported conditions and
omitting existing excluded bodies/framework and line controls gives XP-relative
+16/-33 before, +1/-1 after: 47 fewer diff lines, despite more source lines.
No excluded body receives new accounting. The retained original unused
vector declarations are not a second implemented dispatch path.

R0556 remains evidence-qualified pending owner decision: reg.byte.low precedes
high for the actual little-endian hosts. generic_outw assigns temp.X then
sends low to the first port and high to the next. Actual VGA vga_gc_outw also
assigns temp.X, takes low as register index and high as register value.
Original high-first storage reverses those meanings. The header cannot be
removed because these original I/O/video consumers use its reg union.
Original nibble/bit members and all other selected contents are unchanged.

A private scalar probe compiled with the real device object's flags/header
chain assigns reg.X=0x0104. Current prints low=4 high=1 and returns 0 on both
widths; original byte order prints low=1 high=4 and returns 1 on both.
A separate actual-controller probe recompiles complete vga_prts.c against only
the private header and links the existing vga_frame_smoke, without replacing
machine initialization or rendering logic. Current controls pass x86/x64 in
7.14/7.17 s. Original-byte variants compile/link but terminate with access
violation 3221225477 in 10.90/11.21 s. No timeout is counted as a failure.
The scalar probe proves the exact swapped values; source proves their index/
data use. The broader negative proves unsafe behavior with the real module,
not a captured stack trace identifying the first faulting instruction. No
claim of real DOS/Win3.1/Win95 manual reproduction or exhaustive register-path
coverage follows from these tests.

### P138 final-source qualification

Both cached builds publish fresh EXEs: x86 32.96 s, x64 48.03 s. Actual selected
focused tests are VGA and BOP, 2/2 per architecture in 7.64/7.23 s including
runner overhead. The attempted device_contract_smoke regex names no registered
test; it is not counted as tested (actual device cases have individual suffixes).
Both EXEs are byte-equal to P136 after masking only PE timestamp/checksum and
unique compiler HH:MM:SS literal; no code/data byte is ignored. The owner
exception skips duplicate full suites. P133 background 188/188 per width
remains applicable; native final acceptance is still pending.

- softpc32.exe SHA-256: 18c1e79a9650d4f4fdc6d63b320a185d394bd0db51d56ffa78406c92a271b603.
- softpc64.exe SHA-256: 2e4fb8e12f8d86e1c83cd16fe1c17564d876285d28f612503d87f47581c317eb.
- Final source LF-normalized SHA-256: 789d83449a610fd4a9ea9fdffb11eec491845a1b578cf27fd93ee2931fa3c036.

Build logs contain 5179/5233 warning occurrences, all within mirror paths
(including original macro redefinitions); no outside-mirror warning is present.
Coordinator review checks original correspondence, removal of the unjustified
ABI reason, actual consumer routing, retained byte-order evidence and artifact
equality. Public components, Compat, guest, configuration, media and snapshots
are unchanged. Manifest/documentation/whitespace checks qualify this delivery.
Completed owned jobs permit scratch cleanup. Frontier 436/472; next file 437
host/inc/host_def.h. S1/T100 and retained owner decisions remain open.


## File 437: host_def.h (P139)

Fixed XP raw SHA-256:
539e747884cfff6a355b40eb45b6c170f4ab5fcf8ef574461912deb4ba6a8101.

R0557 restores the original absence of two extra declaration blocks: stdlib.h
and host_simulate(void), including their DIV-070/DIV-057 rationale comments.
It also restores four original NEC_98 else/endif comments without changing
conditions or excluded bodies. The two original header omissions (NTVDM and
CPU_30_STYLE definitions) are separately qualified below. Production +4/-20
(net -16), permanent tests unchanged. With existing excluded bodies replaced
by the same placeholders and line controls omitted, complete-file XP diff is
+21/-7 before and +1/-3 after: 24 fewer diff lines. The remaining +1/-3 is
heuristic alignment of two missing defines around original blank lines; it is
not a third semantic change. No excluded body is re-audited or re-accounted.

The actual selected insignia.h imports Windows headers before host_def.h.
Current x86/x64 SDK combaseapi.h already includes stdlib.h; preprocessing
shows the char-pointer getenv declaration before each observed call even with
both added blocks absent. Allocator declarations likewise remain available;
this rechecks the dependency noted when ckmalloc.h was restored in R0420.
For host_simulate, cpu4.h/host_cpu.h provide the original vector macro and
its declaration, while direct callers retain original explicit declarations
or host.h. Compat's existing provider and vector binding are unchanged.
There is no need to introduce a new include, Compat adapter or second path.
This evidence concerns the actual supported toolchains, not every hypothetical
SDK or future non-Windows port.

Private headers separately remove stdlib, host_simulate, both, or neither.
For each architecture, real Ninja compile flags are used on c_bsic, ccpusas4,
com, quick_ev, video, keybd_io, floppy, floppy_i, rom, cga and fpu. All 176
preprocess/syntax commands succeed. Each of the 22 combined syntax comparisons
adds no warning/error message relative to its current control. Removing line
markers, whitespace and the one retired declaration from the preprocessed
comparison leaves identical text for all 22 current/combined pairs. This is
supporting declaration evidence, not a replacement for final EXE equality or
runtime regression. No guest/SDK source is modified by these private probes.

R0558: retaining omission of NTVDM is an evidence-qualified owner decision.
The product selects independent CCPU machine/device/Compat targets, not an NTVDM
host. Restoring the original define in a private host_def.h makes the actual
ccpusas4 compile fail on both widths, including the existing sas.h NTVDM and
NTVDM-without-X86GFX guards. R0559: omission of CPU_30_STYLE is likewise qualified;
all three product targets select CPU_40_STYLE, and restoring CPU_30_STYLE in
its original header position fails the actual compile on both widths, including
existing xt.h and host_cpu.h guards. All four isolated configurations return
compiler status 1. These demonstrate conflict with the actual selected source
subset; they do not claim original XP NTVDM or CPU_30 itself is defective.
No excluded implementation is evaluated or admitted by this configuration test.

Final source LF-normalized SHA-256:
bc242fbbed1df4eaa9ea94191ebfdd4e419c4b9750b8918b57b8d4db8e6f00f6.
The final diff contains only the specified original restoration. No public
component, Compat implementation, guest, user configuration, media or snapshot
is changed. The old declaration-preservation rationales are superseded for
this current toolchain; historical records remain historical.

### P139 final-source qualification

- x86: cached build/publication 148.75 s; VGA, BOP, checkpoint and x87 **4/4** in 9.12 s including runner overhead; 5271 warning occurrences, all in mirror paths.
- x86 EXE SHA-256: d73d1213d60059a6f656f2fefd251e61b7d1b4d68db53f01c11f84e64bc5de28.
- x64: cached build/publication 206.12 s; VGA, BOP, checkpoint and x87 **4/4** in 10.99 s including runner overhead; 5326 warning occurrences, all in mirror paths.
- x64 EXE SHA-256: 9a5859a188c5cee2229a9d289d746597a533fbc90cec7497173b1973019a223c.

Both EXEs equal P138 after masking only PE COFF timestamp, optional-header
checksum and the unique NUL-terminated compiler HH:MM:SS literal. All other
bytes and lengths are equal. No full-suite repetition is needed under the
owner exception; P133 background 188/188 per width remains applicable.
Native final acceptance remains pending. No manual guest verification is
claimed for this source-neutral restoration. Mirror, documentation, whitespace
and newline checks pass. Coordinator actual-diff review confirms only the
specified declarations/comments, manifest, audit records and fresh EXEs change.
Owned probes/builds have exited; their evidence above permits scratch cleanup.
Frontier 437/472; next file 438 host/inc/host_gen.h. R0558/R0559 and earlier
owner decisions remain open, as do S1 and T100.


## Files 438-443: host general/graphics/event headers (P140)

All six files were compared completely, in frozen order, with the fixed XP
commit. Existing error bodies and their line controls receive no new accounting.

| Order/file | Fixed XP SHA-256 | Disposition |
| --- | --- | --- |
| 438 host_gen.h | 9205e86dd8709b64a4ef3200e0008f83905c621b76e176293a5ceae003063759 | Entire normalized file equals XP; unchanged. |
| 439 host_gfx.h | a3343f1d4c4f53c0b579a3c6fff0a8ae7e8ab7f76f8e2721d9964fac36d9da66 | Entire normalized file equals XP; unchanged. |
| 440 host_hfx.h | 176270b4fb330aa0cdfa43d40748a1e5a1a9f0c11d112152c23991281f4b873d | Only selected difference is relocated historical notice, R0560. |
| 441 host_inc.h | 3d2660aac331347656c79c2039eb961ebe4b72ac10bd683103975252b03981fe | Restore original blank, R0561. |
| 442 host_qev.h | 75412a3456ce5b2f766b989355d2774b09ce5802b4773f9044d7b7b38beb731a | Original LONGLONG handle typedef, normalized XP identity; unchanged. |
| 443 host_rrr.h | 8f6b062e0ae80a201bf3c09ef5ef387d8a00cae944056fc3d57e7542e6c3e5c5 | Restore original two wide-character declarations, R0562. |

Ninja's actual dependency records on each width include all six: host_gen
193 objects, host_gfx 11, host_hfx 2, host_inc 1, host_qev 21 and host_rrr 4.
These counts are dependency observations, not counts of runtime consumers.
Concrete paths include xt.h -> host_gen (and Compat memory.c uses original
NOWRAP_PROTECTION), video modules -> host_gfx, quick_ev.c -> host_hfx,
sascdef.c -> host_inc, quick_ev.h -> host_qev, and original nt_ega/nt_graph/
nt_cga/nt_vga -> host_rrr. The HFX header has no selected runtime definitions;
its original include still exists. Removing the include/file would introduce
a new unjustified mirror difference and is prohibited by the owner's rule.
No implementation is moved to Compat or duplicated.

R0560 retains one historical copyright/version notice outside the excluded
SCCSID body. Removing it would remove the retained source's notice; source
policy qualifies its necessity. It remains a pending owner decision, not an
automatic approval or new bookkeeping for the excluded body.
R0561 inserts the original blank after selected !NTVDM else, with existing
#line 73 changed to 72 so the following include remains original line 73.
No declaration or include target changes. Selected XP diff -1 becomes zero.
R0562 restores wchar_t wszHideMouseMenuStr[] and wszDisplayMouseMenuStr[];
whole production/test search finds neither original nor replacement names
outside these two declarations. There is no definition, assignment or caller
requiring the changed names/types. The original Windows header chain supplies
wchar_t; actual dual builds and equivalent EXEs verify restoration on supported
architectures. Selected XP diff +2/-2 becomes zero. No font or text rendering
behavior is changed and no guest encoding scenario is claimed tested here.

Production total +4/-3 (net +1), permanent tests unchanged. XP-relative diff
falls by five lines: one missing blank plus two replaced declarations.
host_inc final LF-normalized SHA-256:
ec65fdb111e8059794282a322468232e476989142759824d50f4d037556cc52d.
host_rrr final LF-normalized SHA-256:
424e265d2b3e8f1fdc831a340fb9d50dd31d8c354910ea7588c1217d2391c201.
Other four source hashes remain unchanged. Existing error branches are neither
restored nor interpreted as active product support.

### P140 final-source qualification

- x86: cached build/publication 11.73 s; VGA/BOP/quick-time **3/3** in 7.76 s including runner overhead; 180 warning occurrences, all inside mirror paths.
- x86 EXE SHA-256: 09c292b3f910138c059c894ff22f7659df5393831e62faca35a5e07c1b98e652.
- x64: cached build/publication 12.18 s; VGA/BOP/quick-time **3/3** in 7.23 s including runner overhead; 180 warning occurrences, all inside mirror paths.
- x64 EXE SHA-256: 9f777ad62a4fe456b0b4e13baf730050ac637a3bccc1e95e52cc0d0406109cf7.

Both EXEs equal P139 after masking only PE COFF timestamp/checksum and unique
NUL-terminated compiler-time literal, with all other bytes and lengths equal.
Owner equivalence policy skips repeated full suites; P133 background 188/188
per width remains applicable. Native final acceptance remains explicitly
pending. Mirror/documentation/whitespace checks pass; source CRLF and document
LF are preserved. Coordinator actual-change review confirms only the blank,
its diagnostic anchor and original declarations are restored; all other
production files, public trees, Compat, configuration, media and snapshots
remain untouched. Owned jobs are terminal and scratch can be removed after
this evidence promotion. Frontier 443/472; next file 444 host/inc/insignia.h.
R0560 and earlier owner decisions, S1 and T100 remain open.


## File 444: host/inc/insignia.h (P141)

Complete selected-file comparison uses pinned XP SP1, raw SHA-256
fabcbf3571d0a56826b18f57bb734412295c237c586cf888d9cf06a5425c8973.
Existing excluded bodies/line controls receive no new accounting.

- R0563: original IHPE is unsigned int (XP line 77), whose comment promises
  an integer the size of IHP. Current UINT_PTR plus its reason comment is
  +3/-1 relative to XP. Actual sevid000.c global function-address initializers
  require native pointer width. Private original-IHPE restoration makes the
  actual x64 translation unit fail at line 134 with "initializer element is
  not constant"; x86 compiles. This is concrete supported-build ABI evidence,
  not a claim that an original 32-bit XP product was defective.
- R0564: original IUH remains unsigned int for ordinary CCPU. Only existing
  CVIDC_RULE_WORD selection adds a native pointer carrier (+7/-0). Root CMake
  selects it on x64 for sinit/sevid, j_c_lang, ev_glue and vglfunc. Actual
  j_c_lang rule registers and C-VID local/return pointers require that width;
  removing it globally from the private header truncates a real heap pointer.
  No universal widening or second implementation is introduced here.
- R0565: restore the original single blank before the final warning pragma,
  deleting one extra blank. No token, declaration or layout changes.

The private experiment derives the real sevid000.c Ninja command for each
architecture, prepends only a task-owned header variant, and checks current,
original IHPE, original IUH and both originals. All eight scalar probes build.
All four x86 consumer compilations and heap round trips pass (both types 4).
On x64 current is 8/8 and round-trips heap pointer 000002378F482440 unchanged;
IHPE-original is 4/8 and truncates 000002443E532440 (result bit 1);
IUH-original is 8/4 and truncates 0000026EFB982440 (result bit 2);
both-original is 4/4 and truncates 000002CB54BF2440 (result bits 3).
Current and IUH-only x64 actual consumer compilation succeed; IHPE-only and
both-original fail. The coherent both-original variant was checked as well as
individual deltas. These are compiler and pointer-representation proofs, not
an exhaustive guest/video matrix or a new desktop reproduction.

Executor proposes retaining R0563/R0564; independent counter-review confirms
the real source users and source-specific CMake scope, and rejects interpreting
x86 success as x64 evidence. Both remain qualified pending owner decisions.
Restoring neither now is automatic approval. No duplicate Compat behavior is
introduced or transferred. This header remains directly needed by the core.

Production +0/-1, permanent tests unchanged. Selected XP difference decreases
from +11/-1 to +10/-1: one fewer diff line, two semantic residual sites.
Final LF-normalized source SHA-256:
01c8a4488fc13ee4d3ceb6d9c181a0b5bcb489fcd1f8f8f2c0e0e66ee7313761.

### P141 final-source qualification

- x86: cached build/publication 144.97 s; VGA/checkpoint **2/2** in 8.45 s including runner overhead; 5271 warning occurrences, all in mirror paths.
- x86 EXE SHA-256: a08de712acaeb770093f123b9cdb5c6db62d8addcfccae6780d6d4dce7335e3c.
- x64: cached build/publication 213.24 s; VGA/checkpoint **2/2** in 8.51 s including runner overhead; 5326 warning occurrences, all in mirror paths.
- x64 EXE SHA-256: 439e14009a0529f3a6643ef4338b36a46b527865f620610f391fed249c62081c.

CHAIN2 rule assertions are linked into and invoked by vga_frame_smoke; there
is no separate chain2_rule_smoke CTest registration, so it is not counted as
a third test. Both EXEs equal P140 after masking only PE COFF timestamp,
optional-header checksum and the unique NUL-terminated compiler-time literal.
All other bytes/lengths equal. The owner exception avoids repeated full suites;
P133 background 188/188 per width remains applicable. Native final acceptance
is pending, not passed. No desktop path changed in this blank restoration.
Final gates and actual-change review accompany delivery; only the blank,
manifest, audit records and fresh EXEs change. S1/T100 and owner decisions
remain open. Frontier 444/472; next file 445 host/inc/ntcheese.h.


## Files 445-451: host graphics and event headers (P142)

Complete comparisons in frozen order use the fixed XP SP1 commit. Five files
are byte-identical after CRLF/LF normalization; nt_cga has no selected-body
difference. Existing error bodies/line controls are skipped, not counted.

| Order/file | Fixed XP SHA-256 | Disposition |
| --- | --- | --- |
| 445 ntcheese.h | 80575edceb22f7daf924bb605d2dfb7d3a081352018677986679f8b7f1d9df84 | Normalized full XP match; unchanged. |
| 446 nt_cga.h | 7c186c4861f27b438804c15974f96c4c89cc142a31a27ebc71ec56544489f372 | Selected content matches; unchanged. |
| 447 nt_cgalt.h | af4f0a1c5806c8f653cf31b57d20904f145c772e7dc5f7977c9b1ae56614b187 | Normalized full XP match; unchanged. |
| 448 nt_det.h | 06f2e58d129de66771e2256af934cec7c7440082298fe7e637497717e80ef463 | Normalized full XP match; unchanged. |
| 449 nt_ega.h | 5681131039a17c68b93adcd73e61e4092166581ab3846c7772d6b63254c36cdb | Normalized full XP match; unchanged. |
| 450 nt_egalt.h | 8c21868a5bf708c885193be91e2a12528df63e0f2ce09825ded09d98453bc57b | Normalized full XP match; unchanged. |
| 451 nt_event.h | a380a809c97fb7c1d2cd15b156ce7e0812d7c0f69034544fa098cb5cf1eca2c9 | Restore original declaration, R0566. |

Real retained includes are nt_graph -> ntcheese/nt_cga/nt_ega/nt_event/nt_det,
nt_cga.c -> nt_cga/nt_cgalt/nt_det, and nt_ega.c -> nt_ega/nt_egalt.
These carry original graphics dimensions, lookup tables and host declarations;
they are not independently removable unused files. No Compat duplicate is
created or moved, and original include wiring is preserved.

R0566 removes DIV-099's three-line modern-C rationale and restores the exact
XP declaration `extern PointerAttachedWindowed;` (XP line 58). Full source/test/
tool search finds no reference or definition outside this declaration. The
comment's historical nt_event.c provider is not a current retained provider.
Implicit int is valid in the actual mirror target's GNU90 language selection;
the current supported build does not require a modern typed declaration here.
Private current/original header variants were separately compiled through the
real nt_graph.c Ninja commands, x86 and x64, four successful syntax checks.
The only variant edit was this complete declaration/comment restoration.
No compiler error or extra implicit-int diagnostic occurred in either width.
This establishes current compilation, not hypothetical future C dialect support.

Executor restores rather than retaining a patch on the strength of its old
comment. Counter-review checks the language flags, actual include route and
absence of runtime use; no caller/layout change is involved. The final selected
body now matches XP. Production +1/-4 (net -3), permanent tests unchanged;
XP-relative +4/-1 becomes zero, five fewer diff lines. No new difference site.
Final LF-normalized source SHA-256: 6939e6c2448496c7fd629e2ca3c9f939beab5f072fa9010c0c04ef09f99654d9.

### P142 final-source qualification

- x86: cached build/publication 8.66 s; VGA **1/1** in 6.74 s including runner overhead; 91 warning occurrences, all inside mirror paths.
- x86 EXE SHA-256: 9df563e238cb851b5bf2ef02d0422bae8b7bc89a7efb26b0e96a8f696a4e4b64.
- x64: cached build/publication 10.01 s; VGA **1/1** in 5.56 s including runner overhead; 91 warning occurrences, all inside mirror paths.
- x64 EXE SHA-256: 066ec216837f42ccc6444b7c1eaba27066ec7eb5dab450334dd432095f8c1389.

Both EXEs equal P141 after masking only PE COFF timestamp, optional-header
checksum and the unique NUL-terminated compiler-time literal; every other byte
and length is equal. No repeated full suite under the owner's equivalence
exception; P133 background 188/188 per width remains applicable. Native final
acceptance remains pending. No new desktop behavior or guest validation is
claimed. Final mirror/documentation/whitespace and newline checks accompany
delivery. Actual-change review confirms only this declaration/comment, manifest,
audit records and fresh EXEs change. Frontier 451/472; next file 452
host/inc/nt_fulsc.h. Earlier owner decisions, S1 and T100 remain open.


## File 452: host/inc/nt_fulsc.h (P143)

Complete comparison uses fixed XP SP1 raw SHA-256
9bb9026596879d81d4a1e2a7e28210877f9ab707c2cb6c83f8bd11443a206104.
Existing excluded bodies and their framework/line controls are not re-audited
or entered as new decisions. Retained production nt_graph.c and VGA fixture
both include this header; its original constants/types/declarations remain
needed, with no whole-file deletion or replacement Compat implementation.

R0567 restores `IMPORT VOID SwitchToFullScreen(BOOL);` at original line 186.
Full src/test/tools search before restoration finds no definition or call;
there is no competing declaration requiring omission. The preceding source
already maps InitTToG to line 185. Replacing the omission's #line 187 with the
original declaration keeps CheckForFullscreenSwitch at line 187. This does not
enable an NTVDM host fullscreen operation or change public KVM behavior.

R0568 restores the second original blank preceding the final X86GFX condition.
The boundary's #line 258 becomes 257; two original blanks then put the next
condition at original line 259, unchanged. Existing excluded contents remain
untouched. No standalone implementation, extra conditional or helper is added.

Executor restores both unproven deviations. Counter-review checks complete
original/current text, original line anchors, actual include users and absent
SwitchToFullScreen consumers. Compilation and final EXE equivalence below
support current restoration; they are not new fullscreen runtime verification.
Production +3/-2 (net +1), permanent tests unchanged; two missing original
lines restored, two fewer selected XP diff lines. Existing exclusion line
controls are not claimed as newly removed functional patches.
Final LF-normalized source SHA-256: 917f6dccb55130fa060ff1af0136d4cb5470a274a9082117c41fbb156706a10c.

### P143 final-source qualification

- x86: cached build/publication 9.23 s; VGA **1/1** in 7.62 s including runner overhead; 91 warning occurrences, all in mirror paths.
- x86 EXE SHA-256: f482b0acabe9088caccda663434ee2db9e2e322b69305118300fbdb557a2b76f.
- x64: cached build/publication 10.65 s; VGA **1/1** in 5.72 s including runner overhead; 91 warning occurrences, all in mirror paths.
- x64 EXE SHA-256: bba8806ff39457844f1280be0e4b220528dbcb995c38dfab3a75429bd19736ca.

Both EXEs equal P142 after masking only PE COFF timestamp, optional-header
checksum and unique NUL-terminated compiler-time literal; all other bytes and
lengths equal. Under the owner's exception no repeated full suite is required;
P133 background 188/188 per width remains applicable. Native final acceptance
remains pending. Final manifest/documentation/whitespace checks and actual-diff
review accompany delivery. Only this header, manifest, audit records and fresh
EXEs change. Frontier 452/472; next file 453 host/inc/nt_graph.h. S1/T100 and
earlier owner decisions remain open.


## File 453: host/inc/nt_graph.h (P144)

Complete comparison uses fixed XP raw SHA-256
42d0ad0e3d43ea838cddc769f8535a0728e3838d327716704d551bc1ff7b2eaa.
Existing excluded bodies/framework/line controls are skipped without new
accounting. Actual consumers include original nt_cga/nt_ega/nt_graph/nt_munge/
nt_vga, the Compat nt_graph.h include bridge, video/DIB adapters and VGA tests.
SCREEN_DESCRIPTION, paint vectors and scanline contracts remain original;
no second schema, host state or rendering implementation is introduced.

| Point | Original anchor | Disposition |
| --- | --- | --- |
| R0569 | after V7VGA prototypes, before XP NEC_98 condition | Restore original adjacency by removing one added blank. |
| R0570 | XP 494 | Restore missing blank, with line control 495 -> 494 so high_stretch3 remains logical line 495. |
| R0571 | XP 500-501 ega_colour_hi_munge | Qualified pending owner decision: seventh int carries scanline-specific pel pan; original has six arguments. |
| R0572 | before added group declaration | Remove redundant T60 explanation; no behavior or declaration removed. |
| R0573 | after XP 501 | Qualified pending owner decision: declare the shared interleaved-plane panning helper. |
| R0574 | XP 540 | Restore original extern boolean host_stream_io_enabled, remove omission comment and redundant #line 542. |

R0574 has no production/test/tool reference or definition before restoration.
base_def.h already maps boolean to IBOOL; restoring an unused extern requires
no provider, storage or NT stream implementation. The original !NEC_98 branch
is selected and retained. The declaration is logical line 540, as in XP.
Earlier content-exclusion rationale does not prove the declaration harmful.

For R0571/R0573, original-device nt_ega medium/high painters call the munging
function with pan derived for each scanline. The low painter calls the group
helper directly. nt_munge owns the helper and both masked/unmasked munging
loops use it; it shifts each plane byte independently and wraps the next group
at the existing plane-display extent. This is existing device rendering, not
a Compat duplicate. Restoring only the six-argument declaration conflicts with
the seven-argument definition/callers; that alone is not the retention proof.
The P80/R0428 actual-renderer experiment already compiled a private coherent
no-pan query variant and linked the actual fixture: x86 and x64 failed the
pixel assertion after real 3C0h pan=1 writes, while current passed. Current
verify_panning_pixels covers all three painters, shifts 0..7, bank/wrap and
split cancellation against independently shifted unpanned pixels. This batch
reruns that actual VGA fixture on both widths. These results qualify the
existing pan contract, not every implementation patch or a fresh guest-session
reproduction. nt_ega/nt_munge implementation files retain their later ordered
reviews; no claim of fully restoring their coupled XP bodies is made here.

Executor removes unsupported omissions/nonsemantic additions and proposes
retaining just the consumed pan declarations. Counter-review checks real
port-to-pixel evidence rather than treating existing callers as sufficient.
R0571/R0573 remain pending owner choice, not automatically accepted.
Production +3/-5 (net -2), permanent tests unchanged. Selected XP diff decreases
by five lines (two blanks, one explanation, and replacement of an omission
comment by its original declaration). Remaining selected pan declaration diff
is +2/-1. No new difference site or public component change.
Final LF-normalized source SHA-256: f53d0e2a588499bc9e2aa48b847d9b65383764e029aa6e6308c57951ee36ff5f.

### P144 final-source qualification

- x86: cached build/publication 14.22 s; VGA **1/1** in 7.33 s including runner overhead; 269 warning occurrences, all inside mirror paths.
- x86 EXE SHA-256: 9852a737516840eded6610590b217c808fb4ffef9827c2a012d18170d7f22800.
- x64: cached build/publication 18.17 s; VGA **1/1** in 5.51 s including runner overhead; 269 warning occurrences, all inside mirror paths.
- x64 EXE SHA-256: 4d008289f1732344439954d67b555eb43db5acee4ca91d0f2bc5259322cf70d2.

Both EXEs equal P143 after masking only PE COFF timestamp, optional-header
checksum and unique NUL-terminated compiler-time literal; every other byte and
length is equal. Owner equivalence exception avoids repeated full suites;
P133 background 188/188 per width remains applicable. Native final acceptance
is pending. Final manifest/documentation/whitespace checks and actual-change
review accompany delivery. Frontier 453/472; next file 454 host/inc/nt_mouse.h.
S1/T100, R0571/R0573 and previous owner decisions remain open.


## File 454: host/inc/nt_mouse.h (P145)

Complete byte comparison with fixed XP SP1, after CRLF/LF normalization,
is identical. Fixed original SHA-256:
12424b51bffd9bf47973e39afb7a83994f0597042bd64138fd4f5e42323ef772.
There is no selected difference, exclusion record or new decision point.
Original nt_graph.c includes it at physical line 62. Its retained declarations
remain original; names of no-longer-retained mouse providers do not justify
new removal differences. No duplicate implementation is present in this header.

Executor and counter-review agree: preserve the complete original file and
include wiring. Production/test/artifact diff is zero. No rebuild or runtime
rerun is justified for a read-only identical-file audit. Published binaries
remain P144; that batch's dual cached build and VGA passes remain unchanged.
Documentation/whitespace gates and actual documentation-only review accompany
this delivery. Frontier 454/472; S1/T100 and pending decisions remain open.

### File 455 investigation, not a completed disposition

nt_sb.h is a mixed header, not an identical file. Full original/current text
and all direct include sites were read. Real consumers are nt_sb, nt_sbdsp,
nt_sbmid, nt_sbmxr, nt_sbfm, Compat platform and sound fixtures. The preliminary
restoration groups requiring investigation are:

- Original mmsystem typedefs/externs and currently omitted unused prototypes;
  do not assume an unused declaration requires a native host implementation.
- VOID/BYTE/DWORD spelling and original declaration layout: candidate exact
  restorations. BOOL needs coherent provider review: insignia maps BOOL to
  unsigned UINT, whereas SetSpeaker and bHighSpeedMode currently use signed
  IBOOL. A declaration-only conflicting candidate is not necessity proof.
- Original external BasePort/MpuBasePort versus current static storage in
  nt_sb.c: evaluate the original wiring, not automatic retention of privacy.
- Current BufferMidi returns lib_status, checked in VsbByteOut; SbGetStatus/
  SbService also reach actual Compat executor status/service sites. DMA helpers
  operate on the existing controller through SB_DMA_INFO. These are real
  consumers, but their presence alone does not establish minimality/necessity.
- DBG's selected empty macros, historical comments and original reporting
  macro versus existing excluded REPORT_SB_MODE body require separate treatment;
  do not accidentally enable an excluded diagnostic path.

No nt_sb.h edit or retention decision has been made. The next complete-file
work must test coherent restoration, classify every selected difference and
finish that file before advancing its ledger. This record does not close the
investigation or pre-approve its existing device/host adaptation.

### File 455 private compilation experiments (not a P delivery)

Task-owned variants now provide concrete evidence beyond the initial inventory.
No tracked production/test source or published EXE has changed.

1. Restore XP's mmsystem include, MM API typedefs and function-pointer externs
   before the current selected header. Actual Ninja commands for 16 source/target
   combinations per architecture, current versus restored, give 64/64 successful
   syntax compilations. They include all five original sound modules, Compat
   platform, real device-contract fixture, mixer/FM/MIDI fixtures and native MIDI
   fixture compilation. This is not native MIDI playback verification.
2. A fuller private candidate restores original prototypes/layout, unused host
   declarations and selected empty debug macros. It keeps existing diagnostic
   exclusions and current additional service/DMA contracts; BufferMidi retains
   its status return. Coherent private providers restore BasePort/MpuBasePort
   external WORD storage, bHighSpeedMode BOOL and SetSpeaker BOOL together.
   Of 64 syntax compilations, 62 pass; the two failures are solely the actual
   device-contract test's repeated IBOOL bHighSpeedMode declaration.
3. Removing that redundant test declaration lets the restored owning header
   supply the type. An initial copied-fixture run failed because its relative
   fixture_cleanup.h include no longer resolved in the scratch directory;
   this is a probe relocation error, not product evidence. Preserving the real
   test directory as an -iquote search root corrects that probe. Current and
   candidate then compile on both architectures, 4/4.

These results support further exact restoration, not a completed ABI/runtime
proof. The fuller candidate is still private and requires original provider
format reconciliation, remaining-contract necessity review, final diff accounting,
link/runtime regressions and publication. In particular, existing consumers of
new status/DMA/lifecycle calls are a map of the next investigation, not approval
to retain all current adaptations. File 455 remains unfinished and no later
file has been advanced. The active packet retains its bounded scratch for this
ongoing experiment; it is not a completed batch's permanent log archive.

### File 455 coherent worktree qualification (investigation still open)

The previously private candidate is now in the worktree, with exact original
WORD port storage, initialization-time BasePort assignment, BOOL high-speed
flag and multiline BOOL SetSpeaker signature restored together. The device
fixture removes its conflicting redundant flag declaration and uses the header.
Port dispatch is connected only after SbInitialize assigns BasePort and completes
DMA initialization; moving the default from static initialization does not expose
an uninitialized production port handler. This is source-chain evidence, not a
claim that all initialisation failures were manually exercised.

Both cached builds and publication succeeded (x86 13.20 s; x64 20.01 s).
Both EXEs differ beyond the permitted timestamp/checksum normalization, so the
equivalence exception was not used. Each architecture passed eight focused
sound tests and all 188 background tests (149.90 s and 140.32 s). After the owner
reconnected and the console session was verified Active, each also passed four
serial native tests: audio_lifecycle_smoke, sb_midi_native_smoke and both package
variants (7.50 s and 7.62 s). Unchanged Lib desktop tests remain pending final
acceptance under the owner-approved changed-path selection; these eight native
passes do not represent the entire desktop suite or a manual guest music test.

Published candidate SHA-256:

- x86: e506f763514e2aa1037a37f123883fbdfb7f70bf019e019edf1e3abdef94b9e4
- x64: 6421a17afaf5f7b6b1667eedddf8cb37210931bca04120798e611e369555814c

Counter-review of remaining declarations found that XP's public/sdk/inc/vddsvc.h
at the pinned commit defines VDD_DMA_INFO with WORD addr/count/page and BYTE
status/mode/mask: the same six fields as local SB_DMA_INFO. XP nt_sbdsp.c owns
DSP_MODE internally and declares its DMA helpers there. Current cross-file
production uses of SbInitializeDMA and DspCloseOutput are confirmed in nt_sb.c;
DSP_MODE and several other helper declarations also have direct fixture users.
This establishes actual ownership/call sites, not necessity of putting every
helper in nt_sb.h. Coherent original type/declaration placement and the remaining
status/service contracts still require review before file 455 can be completed.
No later file is advanced, no retention decision is approved by these passing
tests, and this candidate is not yet a complete P delivery.

### File 455 internal declaration restoration

The next source review establishes that DSP_MODE, PauseDMA, ContinueDMA,
StartDspTransfer and DmaDataToDsp have no cross-file production consumer.
DSP_MODE and the original PauseDMA/ContinueDMA declaration blocks are restored
inside nt_sbdsp.c. DmaDataToDsp needs an internal forward declaration because
StartDspTransfer calls it earlier; its current success result remains unchanged.
StartDspTransfer is defined before its production callers and needs no added
forward declaration. These five declarations/types are removed from nt_sb.h.
The existing direct device fixture declares the same enum and its two tested
functions locally; no test-only production entry or second implementation is
introduced. Cross-translation-unit enum values and function types are unchanged.

Counter-review confirms the remaining external production callers of
SbInitializeDMA (nt_sb.c initialization), DspCloseOutput (nt_sb.c teardown),
SbService/SbGetStatus (Compat platform service/fault reporting), SbSetRunning
(Compat executor activation), mixer gain/application functions (original mixer
and DSP/MPU owners), and MIDI parser service/reset calls (original port owner).
This is distinct from asserting that all their internal implementations are
minimal; the later whole-file DSP/MIDI audits remain in their frozen order.

Final declaration-restoration builds pass on x86 (12.97 s) and x64 (17.45 s),
with focused sound tests 8/8 on each. Both binaries match the immediately prior
fully tested candidate byte-for-byte after masking only the PE timestamp,
checksum and unique compiler-time literal. The owner's equivalence exception
therefore applies to repeating its 188/188 background and 4/4 related native
tests. This evidence does not waive final native desktop acceptance.

- x86 SHA-256: 8e0cbaef4de13d242d505f84ba6e9577b323b6345d8e4fec9c29826bd933c94f
- x64 SHA-256: 9ee287108b42eb013b8c715a3cb2b58a684bcb0b95a40b6ad250d12cb3bb7dd8

File 455 remains local pending reconciliation of the residual type/status
necessity decisions and exact original-relative diff accounting. Passing this
round does not mark the file complete or authorize moving to file 456.

### File 455 final counter-review and diff accounting

The internal-declaration migration above was rejected after measuring the whole
coupled difference: it reduced nt_sb.h by ten XP-diff lines but increased
nt_sbdsp.c by eleven relative to the prior candidate, a net increase of one.
Its source changes and extra test declarations have been reversed. This corrects
the earlier inference that restoring their location would necessarily reduce
the mirror difference. The later DSP whole-file review may find a coherent
original ordering that avoids that increase; none is claimed here.

Likewise the expanded SetSpeaker signature is not retained at the current
non-original function position: it added three XP-diff lines. Only the BOOL
parameter type is corrected to agree with the restored original header; current
one-line layout and parameter spelling stay unchanged. The redundant PCM include
is removed from nt_sb.h: actual PCM producers already include that contract.
The MIDI include remains for lib_status and the actual nt_sb.c host calls.

Using SequenceMatcher with autojunk disabled over LF-normalized fixed-XP and
HEAD/current files, with the two previously excluded header bodies omitted on
both sides, the net XP added+deleted difference decreases by **328 lines**:
nt_sb.h -325, nt_sb.c -3, nt_sbdsp.c unchanged. No excluded body is assigned
a point or new accounting entry. This measures mirror difference, not source
length. Tracked production changes are +324/-56 (net +268); the device fixture
changes +1/-1. Added source is principally restored original declarations.

The x86 final build and eight focused tests pass (12.86 s, 0.94 s). The first
x64 link was blocked by the running repository softpc64.exe (PID 19188), not by
a compile error. After closing that exact instance under existing owner
authorization, x64 build and eight focused tests pass (7.84 s, 0.81 s).
Both final binaries are equivalent to the already fully qualified candidate
under the strict timestamp/checksum-only normalization. Final publication hashes:

- x86: 8714984c762d60b38e559dcf25ce5bc06203cf35a1a462c003fd4556fc1480a2
- x64: 91783edec40524944ab15bd98ef429088c662583523c2c056ebf1195af5bf89c

Remaining decisions must distinguish declaration necessity from implementation
approval. BufferMidi's status reaches MidiResult in VsbByteOut; the real parser
fixture observes IO_ERROR, paused rejection and retained-message service. The
original void API cannot report those outcomes to the port owner. SbService and
SbGetStatus are checked by platform.c before executor progress; SbSetRunning
connects the same executor's activation to PCM/MIDI suspension. Original native
worker/event contracts cannot be silently substituted for this executor-only
device contract. Mixer gain callbacks connect the existing register owner to
stream-local PCM gain and optional MIDI host gain, rather than creating another
mixer. These are concrete supported contracts, not proof that every later
provider implementation line is necessary.

The DMA structure retains the original six field widths without introducing
an NTVDM SDK dependency: neither supported MinGW include tree supplies the
original vddsvc.h type, and no retained repository header defines it. Its
QueryDMA/SetDMACountAddr users access the original controller fields; replacing
it with a native handle or guest pointer is not an equivalent restoration.
An exact original type-name/declaration restoration still needs a coherent
provider-level check, rather than a same-layout observation alone. File 455 is
therefore **not yet marked complete**: this remaining original-type alternative
must be resolved before recording its residual decision and delivering P146.

## File 455 host/inc/nt_sb.h (P146)

The original-type alternative is now resolved: restore VDD_DMA_INFO and
PVDD_DMA_INFO, the original six WORD/BYTE fields, original DMA function return
and parameter types, all provider uses and the existing fixture declarations.
There is no SB_DMA_INFO compatibility alias. The declaration remains local
because the supported SDKs do not provide vddsvc.h; this does not import any
NTVDM service implementation. Its field order and storage widths are unchanged.
This coherent restoration adds four header-diff lines but removes 22 provider
diff lines, a further net reduction of 18. Unlike the rejected internal
declaration relocation, it reduces the complete coupled mirror difference.

| Point | Initial analysis | Counter-review / disposition |
| --- | --- | --- |
| R0575 | Original MM include, typedefs, handle/port externs, unused prototypes and selected empty macros were removed/reformatted. | Actual consumers compile with original declarations. Restore them; unused declarations do not require recreating native workers. Existing excluded bodies remain unexamined. |
| R0576 | Original WORD port globals and BOOL declarations conflicted with current signed/private providers. | Restore providers coherently, including initialization-time BasePort assignment and the single owning high-speed flag declaration. Remove the fixture's redundant conflicting declaration. Header-only failure is not used as retention evidence. |
| R0577 | BufferMidi differs from XP VOID by returning lib_status. | Qualified pending owner decision: VsbByteOut consumes the status; the real parser and host-failure fixtures verify rejected input, IO_ERROR and retained-message service. The original void contract cannot convey these outcomes. This is supported-contract/source proof plus current-path tests, not a claim that the original NTVDM product failed. |
| R0578 | Additional status/service, suspension, reset, completion and mixer declarations plus MIDI adapter include. | Qualified pending owner decision: sole-executor platform service checks SbService/SbGetStatus and activation calls SbSetRunning; original port/mixer owners call the parser and output owners. Native-worker replacement would change the supported executor/host ownership contract. Retain declarations for those existing paths; provider whole-file implementation review is still outstanding. Remove the redundant PCM include instead of retaining both transitively. |
| R0579 | SB_DMA_INFO renamed and compressed the original SDK record. | Restore original VDD_DMA_INFO/PVDD_DMA_INFO and corresponding provider types; tests use the unique restored declaration. Local placement remains a qualified owner decision because supported SDKs lack the original type and actual controller query/update calls require its layout. No guest/native handle conversion or second DMA controller is introduced. |
| R0580 | DSP_MODE and internal forward declarations sit in the added header tail rather than their original provider positions. | A coherent private relocation compiled and was binary-equivalent but increased total XP diff by one line, so reject it under the owner's no-increase rule. The existing declarations supply types before real production calls; retaining their location is a qualified constrained decision, not a claim that header ownership is inherently required. No extra test enum or declarations remain. |

Every remaining selected header difference is accounted for by these declaration,
status, SDK-layout and placement groups. Necessary original line mappings and
the established exclusions are unchanged in meaning and receive no new points.
The two provider files have only coupled edits here; their complete-file audits
have not been advanced. Historical comments removed from the rewritten header
do not constitute new executable behavior or approval of the prior adaptation.

Final validation: cached x86/x64 builds 13.94/25.10 s; focused sound tests 8/8
per architecture (0.99/1.44 s). Both EXEs again match the already qualified
candidate under the strict timestamp/checksum normalization. Thus the preceding
188/188 background and 4/4 related native tests per architecture remain
applicable; unchanged Lib desktop tests remain pending final acceptance.
Mirror/documentation gates and actual source/test diff review pass. No source
changes after these builds are part of this delivery.

- Final x86 SHA-256: 550b98b814cf0f2750f1c96c3c9c00044672d77e7dc8ad3ec9f0383bd894d139
- Final x64 SHA-256: 61a447b595e7fc89dcf140a85862c84bdd111ec174c15cef078e3629bd6f7f4d
- Production +347/-75, net +272; test +12/-12, net zero. This is restored source.
- XP-relative added+deleted diff **decreases 346 lines**: header -321,
  nt_sb.c -3, nt_sbdsp.c -22. Existing exclusion bodies are skipped.

File 455 is audited with restored and qualified-pending dispositions. Frontier
455/472, with 17 files not yet completely audited. These qualified points and
earlier owner decisions remain open; S1/T100 are not closed.

## File 456 host/inc/nt_uis.h (P147)

Pinned XP normalized SHA-256 is
2e766aed26843dd76c1eb34cb752cfeaca73f55ac1314f6f6a5b8c5d1389ca2a.
The actual consumers are machine/input.c, compat/keyboard.c, original
host/src/nt_graph.c and the existing keycode fixture. A full src/test symbol
search before restoration found no remaining declaration, definition or caller
of the omitted names. Thus there is no conflict or unsupported runtime service
enabled merely by restoring their declarations/constants.

| Point | Initial analysis | Counter-review / disposition |
| --- | --- | --- |
| R0581 | BiosKeyToInputRecord prototype and aNumPadSCode extern removed. | Restore exact original declarations; neither is called by current production. An unused declaration is not a request to restore the original provider or NT service. |
| R0582 | ED_DOSTITLE 335 removed. | Restore original resource constant/comment; no current consumer or collision found. |
| R0583 | ED_INITGRAPHICS 345 removed. | Restore original constant; no current consumer. Do not reinterpret the skipped DBCS body as an enabled resource registration. |
| R0584 | Original blank before the second DBCS condition removed. | Restore formatting; its excluded body stays untouched. The following original source position is 117, so the existing line-control boundary follows that original position. |
| R0585 | ED_FORMATSTR0-3 removed and an extra blank inserted instead. | Restore all four original constants and exact surrounding spacing; no current consumers. |

After excluding the already-excluded bodies and their line controls, the entire
remaining file is exactly equal to fixed XP, not just the restored hunks. The
selected added+deleted difference decreases from ten lines to zero. Working-tree
production delta is +10/-2 (net +8); no test source changes. There is no residual
selected semantic difference or new owner decision for this file.

The owner has returned to the desktop and explicitly requests complete tests.
This batch therefore runs both cached builds and all 188 background plus eight
native tests per architecture even if EXE equivalence permits a smaller run.
Final validation passes: x86 cached build 10.40 s, background 188/188 in
142.32 s, native 8/8 in 13.52 s; x64 build 22.89 s, background 188/188 in
149.99 s, native 8/8 in 14.17 s. Total 392/392, including actual PCM/MIDI,
both package cases, Lib MIDI, Window retirement/modal and Console broker tests.
No native test remains deferred for this final-source baseline. These automated
tests do not claim every guest/manual scenario or resolve pending diff decisions.
Both EXEs are equivalent to P146 under the strict timestamp/checksum mask,
but the owner-requested full tests were nevertheless executed.

- x86 SHA-256: 3dc3065e7c50773401061471a9283eeb5a52c1aa3ed4ed2f312961214ba740cb
- x64 SHA-256: 44887d25f83ebe5b47119ca86894fd20707fa9948aedca59678346535c004dd6

Actual-diff counter-review, manifest and documentation gates pass. Each build
has 91 warnings, all attributed to retained mirror files; no warning outside
the mirror is observed. Only the identified declaration/constants/spacing and
source-line boundary change; no keyboard conversion implementation changes.
Frontier 456/472. The remaining 16 files and earlier qualified decisions keep
S1/T100 open; no task closure is implied by full regression success.

### File 457 nt_cga.c investigation (not a completed P)

Fixed-XP comparison is now complete at the selected-text inventory level.
Existing excluded bodies are omitted using matching conditional paths, without
new IDs or exclusion accounting. Differences comprise the local BIGWIN undef,
DIB contract include, spacing/line boundaries, text pointer/gather/stride,
empty-region and allocation guards, a trailing trace comma, and the CGA
odd/even pointer/increment mapping. No tracked production edit has been made.

Task-owned copies use actual cached Ninja compiler flags and the actual VGA
fixture linker inputs, replacing only the CGA object before the original static
library. Neither cache objects nor published EXEs are overwritten. The current
variant builds, links and passes the VGA fixture on both architectures.

- Restoring the original trace comma fails both compilations: sub_note_trace4
  receives seven arguments but its actual selected definition takes six.
- Removing the local BIGWIN undef fails both compilations: host_def.h defines
  BIGWIN and selects the established unsupported profile. This does not reopen
  or account for the excluded implementation.
- Coherently restoring the original text source pointer and both original copy
  branches builds/links, but both runs fail the existing text-stride byte-content
  comparison at vga_frame_smoke.c:392.
- Coherently restoring the CGA pointer, 3/1 increment and XOR-2 alternation
  builds/links, but both runs report mode 04/05 incremental/full mismatches and
  fail colour_resolution_failures == 0 at vga_frame_smoke.c:2445.
- Removing either the empty-region guard or the allocation guard passes the
  unmodified VGA fixture on both architectures. This alone does not prove either
  deletion safe. In particular verify_text_stride preallocates every size itself,
  bypassing the production obligation under investigation.

An additional private fixture changes the requested dimensions to 100x5 and
calls the actual nt_text before any test-side prepare, then queries actual
surface dimensions. Current CGA passes on both widths; the no-allocation
variant fails columns == 100 && rows == 5 && stride == 100 on both. Thus the
producer allocation call has direct evidence, not merely a historical rationale.
The copied fixture has no new production entry or alternate broker/renderer.
The observed negative exits are assertion failures (x86 fast-fail 3221226505,
x64 3), not observation timeouts or manually reproduced guest failures.

The empty-region guard's necessity and exact reducible formatting remain under
review. Its old bulk-copy-underflow comment does not describe the current gather
loop, which executes no cell copies for zero height. Actual caller preconditions
and null/negative cases must be checked before disposition. File 457 remains
unreviewed in the convergence ledger; no later file has been advanced. These
probe results are not a delivered source change or approval of residual patches.


## File 457 host/src/nt_cga.c (P148)

The earlier investigation is resolved by actual empty-transition evidence:
request 100x5 while the existing surface has the saved dimensions, then call
nt_text with height zero. Current code preserves the old surface on x86/x64;
removing the empty guard replaces it and fails the saved-dimensions assertion
on both. This is premature surface replacement, not the obsolete bulk-copy
underflow rationale. A positive-height call then creates the requested 100x5
surface. Both checks now extend the existing verify_text_stride fixture; no
production test entry, second rendering path or test-side prepare is added.

Actual selected compilation is CCPU/C-VID with host_def.h defining BIGWIN;
nt_graph.c's selected text and CGA painter tables call these original functions.
The C-VID dirty producers in gfx_updt.c supply the source offsets and repaint
rectangles; compat/dib_surface.c owns the dynamically sized host text surface.
Its allocation is not a duplicate guest text renderer. All selected differences
are reconciled below; existing excluded bodies are not inventoried.

| Point | Initial analysis | Counter-review / disposition |
| --- | --- | --- |
| R0586 | Local BIGWIN undef and its explanatory paragraph. | Remove the non-original paragraph. Undef remains qualified pending: coherent removal selects the unsupported host profile and fails both actual compile commands. No excluded body is reopened. |
| R0587 | dib_surface.h include. | Qualified pending with R0591: declares the actual host allocation contract used here; restoring original fixed Console allocation cannot supply the supported dynamic surface. |
| R0588 | Selected spacing and mismatched source-line boundaries. | Restore exact original spacing and line positions, including EOF. No executable expression changes. |
| R0589 | CHAIN2 text source pointer, cell gather, physical source pitch and dynamic destination pitch replace original bulk-copy branches. | Qualified pending: restoring the original pointer and both original branches together builds but fails text byte comparisons on both architectures. Existing matrix covers 40/80/100/132 columns, padded pitch and partial/full updates. Remove only the extra local gather comment. |
| R0590 | Empty/null/nonpositive text guard. | Qualified pending: empty transition before allocation must preserve the existing surface; both negative variants fail, both current variants pass. Source inspection additionally shows negative height would not terminate the decrement-to-zero loop and null source is not readable; these are source proofs, not injected runtime cases. Remove the stale underflow rationale, keep the single existing guard. |
| R0591 | Geometry validation and text surface prepare. | Qualified pending: direct positive-height production call must resize without fixture preparation; removing the guard fails the actual queried 100x5 dimensions on both architectures. Nonpositive dimensions are invalid for allocation and negative origins are invalid for the destination indexing; no separate workaround is introduced. Remove the misleading claim that copying remained original. |
| R0592 | Removed trailing trace comma. | Qualified pending: exact original invocation has seven arguments versus the selected six-argument macro and fails both compilers. |
| R0593 | CGA odd/even pointer plus 7/1 increments and XOR-6. | Qualified pending: coherent original pointer/3/1/XOR-2 restoration builds but fails mode 04/05 incremental versus full pixel comparisons on both architectures. Remove only the extra inline comment. |

The before/after selected XP added+deleted difference is 95 -> 74, a reduction
of 21 lines, using matching conditional paths and SequenceMatcher with autojunk
false. Line-control directives are checked against the fixed XP positions,
not treated as executable changes. Production +12/-25 (net -13), test +10/-3
(net +7). Selected residuals above are evidence-qualified owner decisions, not
automatically approved retention. No guest/manual reproduction is claimed by
these host fixture results; no original guest, media or configuration changed.

Final-source cached builds/publication pass: x86 9.23 s, x64 12.87 s. Background
regression passes 188/188 on each architecture (147.25/149.38 s). x86 native
passes 8/8. x64 native initially passes 6/8: the two package cases receive
extra characters, showing `uodapause` and `eviewpause` instead of `pause`.
This is observed input contamination, not evidence of a disconnected session
or a renderer defect. The same binaries pass both isolated reruns in 5.50 s.
Final coverage is 392/392, preserving the first-run failure disposition; this
is not a claim of a completely clean first pass. No native verification deferred.

Both published binaries are equivalent to P147 when masking only PE timestamp,
checksum and the unique compiler-time string. Full tests were still executed.

- x86 SHA-256: 63314d80a34b4fbd49506b45a8a0c0b188749aafaa50858f9b48c7537c5a4ed3
- x64 SHA-256: f30606e9c012cdf363558ccd94d0ea31dafd0cee0368ea18d8a5b8667b3db7de

Actual-diff review confirms no executable production change and no new
mirror difference. Frontier 457/472; next is host/src/nt_ega.c. Fifteen files
and qualified owner decisions remain open; S1/T100 are not closed.


## File 458 host/src/nt_ega.c (P149)

The pinned XP counterpart and every selected difference were compared in order.
Current selected differences comprise three size/bounds blocks, three panning
connections, three extra explanatory lines and a trailing blank. Existing
excluded bodies are skipped without IDs or new accounting. Their surrounding
selected structure otherwise matches the pinned original.

Actual consumers are the std_colour_paint_funcs table in nt_graph.c and
C-VID gfx_updt.c's paint_screen callbacks. EGA split/wrap updates derive source
pitch, offsets and row counts from the current device display state; the full
split update can request zero first-region rows. The host DIB has the actual
mode dimensions. In particular the original fixed high-mode width/height
ceiling does not cover V7 65h's 1024x768 planar output. These are selected
production paths, not test-only copies or alternate Compat rendering.

Private variants compile using actual cached Ninja flags and link the existing
VGA fixture with only the tested EGA object overridden before the machine
archive. All variants compile and link. Three variants restore the exact
original guard, one painter at a time. Three variants restore the original
no-panning computation, one painter at a time: low uses the original direct
word load; medium/high pass zero through the existing shared signature so no
artificial argument-count failure is used as evidence. The helper's zero-pan
branch returns the original word unchanged. Three additional diagnostic variants
retain only a nonpositive-size check ahead of the original guard, separating
empty-region failure from the remaining bounds/extended-mode requirement.
These diagnostic variants are not proposed production implementations.

Initial parallel experiment groups accidentally shared the fixture's fixed
scratch-disk filename; one x64 run failed at fopen rather than the renderer.
That result is invalid for code necessity. Both groups terminated; all twenty
runs were repeated serially in separate task-owned working directories. The
serial results supersede the contaminated run, not erase its disposition:

- Current fixture: x86 and x64 pass.
- Each original low/medium/high guard: both widths exit with access violation
  0xC0000005; current equivalents pass the same complete fixture.
- Original guards plus nonpositive-size protection: low/medium fail the actual
  destination-byte boundary assertion (fixture line 712); high fails the dirty
  publication assertion after the real V7 65h BIOS/BOP mode setup (line 2279).
- No-pan low/medium variants fail the independently shifted pixel comparison
  (line 561); high fails the pixel comparison after the actual 3C0h pan write
  (line 512). Both architectures reproduce all three failures.
- Assertion exits are x86 fast-fail 0xC0000409 or x64 3. These are observed
  assertions/access violations, not timeout diagnoses. No manual guest
  reproduction or universal safety claim is made.

| Point | Original / local anchor | Executor and counter-review disposition |
| --- | --- | --- |
| R0594 | Extra T60 explanations in the three standard painters | Restore original by removing all three lines; they add no behavior and no longer justify a mirror difference. |
| R0595 | XP low guard near 606 vs current nt_ega_lo_graph_std | Qualified pending. Original guard fails empty-region and destination bounds cases. Source and destination spans must match the actual DIB and plane capacity; the existing checks precede division/indexing and the do/while loops. No separate clamp or second render path is introduced. |
| R0596 | XP low word load near 639 | Qualified pending. Direct original word load loses required pixel panning in the low painter; same fixture fails with only this coherent change. |
| R0597 | XP medium guard near 1048 | Qualified pending for the same actual buffer/loop contract as R0595, independently reproduced in the medium painter. |
| R0598 | XP medium munge call near 1076 | Qualified pending. Original no-pan semantics through the current ABI fail independently expected pixels; a declaration mismatch is not the proof. |
| R0599 | XP high guard near 1282 | Qualified pending. Besides empty and bounds safety, original fixed limits reject actual V7 extended planar rendering. High-mode negative variant loses the required dirty publication after BIOS mode 65h. |
| R0600 | XP high munge call near 1305 | Qualified pending. Actual register-write/pixel test fails without this per-scanline panning argument. |
| R0601 | EOF selected blank after line control | Restore original spacing by removing the extra blank; existing excluded body and line control remain intact. |

Counter-review distinguishes arbitrary-input boundary assertions from actual
mode/register paths: negative origins/pitch and overflow-sized requests have
fixture/source safety evidence, not proof that a particular guest emits every
such request. The existing predicate ordering prevents invalid division and
pointer formation; positive span clipping limits each row and source advance.
No unchanged helper implementation is declared fully audited here; nt_munge.c
retains its later ordered review. Residual decisions are not auto-approved.

Final source changes are nonsemantic only: production +0/-4, tests unchanged.
Selected XP added+deleted diff is 49 -> 45 (-4), using matching conditional
paths and SequenceMatcher with autojunk false. Final-source validation and delivery evidence follows.


Final-source x86/x64 cached builds pass (11.42/14.24 seconds), with zero warning
lines in these incremental build logs. Both artifacts are equivalent to P148
under only the PE timestamp/checksum and unique compiler-time mask. This does
not claim an unrelated clean build has no historical mirror warnings.

- x86 SHA-256: 6f9842d9a9834f83111f4bf2ba114289501a32b0a35367ab17f3727ac8ad7177
- x64 SHA-256: 151706b8c15da3ad905e501661487d44ebef98c52872d0b8a8bc360927bf4bd6

x86 background initially passes 187/188 in 159.08 seconds: checks/xp_mirror
rejects my incorrect manifest column update. The fixed XP subset hash is restored
unchanged and only local_sha256 is updated by column name. The actual CTest gate
rerun passes 1/1 in 0.79 seconds; this metadata correction changes no compiled
source or artifact. x86 native passes 8/8 in 15.39 seconds. x64 background passes
188/188 in 143.99 seconds and native passes 8/8 in 14.76 seconds. Final coverage
is 392/392 with the first-run failure explicitly retained, not a clean-first-pass
claim. Native desktop/audio/package coverage is not deferred.

Actual-diff counter-review confirms only the four identified nonsemantic lines
are removed, no test source changes or added production path, all original
notices preserved, mirror CRLF preserved, and the pinned XP subset hash intact.
Manifest, documentation and whitespace gates pass. Completed file frontier is
458/472; host/src/nt_graph.c is next. Fourteen files and qualified owner decisions
remain; S1/T100 are not closed.


### File 459 nt_graph.c investigation (not a completed P)

Complete selected-difference inventory is established against pinned XP, skipping
existing excluded bodies without new IDs/accounting. The selected differences
are: DIB contract include; V7 DAC-width declaration; removed stream state/function;
five static forward declarations; resetWindowParams; three removed fullscreen
checks; selected formatting; host update begin/end; packed-painter selection and
width; soft_reset guard; and the two palette conversion loops.

Current production sc is the original static SCREEN_DESCRIPTION object, hence
ScreenState starts WINDOWED (zero). Whole src/test search finds no production
write to this field or whole sc assignment/copy; compat/video.c only reads it.
The archive stores controller/device state, not the host SCREEN_DESCRIPTION.
Only two VGA fixture functions assign FULLSCREEN. Their historical failure under
original guards is therefore not evidence of a supported production requirement.
Private fixture copies exercise the actual WINDOWED state instead. Restoring
all three original guards and the uncalled original stream global/function
passes on x86/x64. Production tests are revised to assert the actual host state,
retaining clear/cursor assertions without manufacturing FULLSCREEN.

The same actual Ninja compiler/linker inputs build isolated complete nt_graph
objects, replacing the archive member, and run in separate scratch directories.
An initial copied fixture lacked the original directory for relative includes;
compilation failed before testing. Adding that original include search directory
resolved the harness error; it is not evidence against original code.

- Restoring the original five external forward declarations fails both compilers
  because the original definitions are static. The minimal static declarations
  remain a candidate for owner approval; no implementation linkage is changed.
- Restoring resetWindowParams exactly passes both VGA fixtures and both actual
  snapshot transaction/cross-process fixtures. Restoring the original soft_reset
  early return separately passes the same tests. No concrete necessity for the
  extra reset writes or deleted startup guard was established; restore both.
- Restoring the original CHAIN4 conjunction fails expected geometry at fixture
  line 1388; removing packed-width selection fails 640x400 at line 2302, both
  architectures. These are actual controller/mode paths, not declaration errors.
- Restoring nt_start_update/nt_end_update together fails produced-frame evidence
  at the cursor-output assertion, line 800, on both. Source tracing reaches the
  existing DIB update depth and V7 pointer clear/draw integration; no duplicate
  device implementation was found in Compat.
- Original fixed DAC multiply-by-four passes the old fixture. A separate actual
  C1/3C8/3C9 port-write fixture covers 6-bit and 8-bit DAC values in both planar
  and packed palette branches. Current passes both widths; original fails the
  8-bit red component assertion (line 2447). The source C1 path sets DAC_data_bits
  to 8 and preserves byte values, so multiplying them by four is not equivalent.
  This missing coverage is added to the existing VGA fixture, with original DAC
  control/value and mode restored afterward; no test-only production interface.

Candidate changes restore the unproved removals and original reset body,
remove non-original explanations, and restore selected spacing/diagnostic line
positions. Retained semantic candidates are the DIB update contract, five static
forward declarations, CHAIN4 painter selection, packed width and DAC conversion.
No guest or user configuration/media/snapshot changes. This is host fixture and
source evidence, not a claim of fresh real Win3.1/Win95 manual verification.

Cached full qualification is running. The x86 build preceded the final
nonsemantic blank/line-control correction, so it requires a final-source build
and relevant verification before delivery. No file-ledger advancement or P150
commit is claimed yet; the last delivered frontier remains 458/472.


## File 459 host/src/nt_graph.c (P150)

The preceding investigation is complete. Actual-diff counter-review reconciles
all selected points as follows; retained items require owner choice, not an
inferred blanket approval. Existing excluded bodies/framework receive no point.

| Point | Original/current anchor | Disposition and counter-review |
| --- | --- | --- |
| R0602 | Include list: dib_surface.h | Qualified pending with R0609: the declared paired host-update contract is exercised by the actual original video callbacks, not a second renderer. |
| R0603 | Globals: DAC_data_bits | Qualified pending with R0614; reduce to one extern int declaration. Remove the local V7 conditional wrapper and explanation: an unused external declaration needs neither implementation nor a runtime guard. No broad extra header is introduced. |
| R0604 | host_stream_io_enabled | Restore exact original definition. No actual production reader was found; the separate disabled guest stream_io state is not this host global. Restoration does not enable streaming. |
| R0605 | Forward declarations: set_screen_sizes/check_win_size/select_paint_routines/dummy_paint_screen/init_lut | Qualified pending: exact original external declarations conflict with the original static definitions on both real compilers. Keep only the matching static declaration correction. |
| R0606 | resetWindowParams | Restore the exact original body, indentation and comment, removing extra flush/palette/mode/cursor cache resets. Separate original restoration passes both VGA and snapshot transaction/cross-process tests; no necessity was established for the additional writes. |
| R0607 | nt_clear_screen fullscreen check | Restore exact original check inside its existing original condition. Production never changes sc.ScreenState from static WINDOWED; an artificial test-only FULLSCREEN value does not justify deleting native policy. |
| R0608 | nt_graphics_tick and EOF selected spacing/comments | Restore original spacing/directive comments and diagnostic positions. Preserve existing excluded bodies without new accounting. |
| R0609 | nt_start_update/nt_end_update | Qualified pending: restoring the pair to original behavior fails actual produced-frame evidence on both widths. Existing host depth and V7 pointer integration remains the single rendering transaction. |
| R0610 | make_cursor_change fullscreen check | Restore original check and source-line position. Fixture now validates actual WINDOWED presentation, retaining cursor visibility/shape assertions. |
| R0611 | nt_set_paint_routine CHAIN4 conjunction | Qualified pending: coherent original condition fails real controller-programmed geometry on both widths (fixture line 1388). Sequencer selection remains independent of transient memory access chaining. |
| R0612 | check_win_size soft_reset guard | Restore original guard/spelling. Separate VGA and snapshot transaction/cross-process probes pass on both widths; no necessary production behavior difference was established. |
| R0613 | Packed painter width | Qualified pending: original width calculation fails actual 640x400 expected output (fixture line 2302) on both widths. Retain the selected painter's source-byte width; remove added explanatory prose. |
| R0614 | set_the_vlt packed and planar DAC conversion | Qualified pending: actual C1/3C8/3C9 writes prove 8-bit components must not be multiplied by four. Both branches and 6/8-bit cases are now covered by the existing fixture. Remove the added explanation, retain one conversion rule. |
| R0615 | graphicsResize fullscreen check | Restore exact original check; same actual WINDOWED host contract as R0607/R0610. |
| R0616 | host_stream_io_update | Restore the exact uncalled original function and original selected tail spacing. No new wiring or native Console streaming consumer is enabled; this avoids retaining an unsupported source omission. |

Production delta is +35/-41 (net -6); test delta +34/-14 (net +20). The important
metric is XP-relative added+deleted diff: **70 fewer selected difference lines**,
not the net source count. Comparison uses matching conditional paths and
SequenceMatcher(autojunk=False); unchanged excluded material is not re-audited.
The remaining code has no added helper, fallback, recovery state or second
renderer. No unrelated public component, configuration, media or snapshot changed.

The initial whole-file restored candidate passes both complete suites:
x86 cached build 13.23 s, background 188/188 in 143.79 s and native 8/8 in
15.80 s; x64 build 10.99 s, background 188/188 in 146.37 s and native 8/8 in
15.13 s. Total 392/392, including native audio/desktop/package coverage.
These EXEs differ materially from P149 because original code/guards/reset body
are restored; equivalence to P149 is not claimed or used to omit full testing.

Final review then corrects the cursor/soft_reset source-line correspondence,
one extra blank and the unnecessary declaration wrapper. Final cached builds
pass (x86 9.38 s, x64 10.79 s); final VGA, snapshot transaction, snapshot
cross-process and mirror gate pass 4/4 on each width (9.53/8.73 s). Final EXEs
are equivalent to the already fully qualified candidate under only the PE
COFF timestamp/checksum and unique compiler-time mask. The intermediate x86
line-control-only rebuild also passed equivalence and its two focused checks;
no stale-source build is presented as final-source proof.

- Final x86 SHA-256: b8a97800380d3531aba96abc1a2b37a0c31fc000d9d760cca0d0d3e78c7bb18d
- Final x64 SHA-256: 30a8cc0416fd6d8e0ef65b7f613349ecb0af54d609073e64100c856884defd36

Each final incremental build reports 91 warnings, all in retained mirror code;
none outside the mirror. Manifest, documentation and whitespace checks pass.
Mirror CRLF and own-code/docs LF are preserved. Owner decisions R0602/R0603,
R0605/R0609/R0611/R0613/R0614 remain open. Completed frontier 459/472, next
host/src/nt_keycd.c; thirteen files and earlier decisions keep S1/T100 open.

## File 460: host/src/nt_keycd.c (P151 investigation)

The actual device object is linked into softpc-machine and consumed by
machine/input.c: vm_keyboard_inject_machine_event constructs a private
KEY_EVENT_RECORD, masks the supplied scan code to eight bits, and calls
KeyMsgToKeyCode. The original tables are the single mapping implementation.
Whole src/test search finds no caller of BiosKeyToInputRecord and no duplicate
BIOS-buffer-to-host-input conversion in Compat; nt_uis.h retains its declaration.

Private copies use the actual Ninja device compiler flags and replace only
nt_keycd's archive member at link time for the existing keycode fixture.
Both x86 and x64 compile, link and pass the fixture with the original
NULLCHARSCAN/aNullCharScan and aNumPadSCode restored verbatim. Their prior
omission has no demonstrated necessity. Restoration does not introduce a
caller or change key delivery.

Restoring the original three NT includes fails at missing nt.h on both
architectures. Restoring the complete BIOS function together with its original
tables, while keeping the existing Windows include, compiles but cannot link:
RtlOemToUnicodeN and NT_SUCCESS are unresolved on both widths. This is a
selected-host dependency omission, not proof of an original BIOS algorithm bug.
No native RTL replacement, new import wiring or duplicate converter was added.

The two original greater-than table guards are tested separately with actual
KeyMsgToKeyCode and -fsanitize=bounds -fsanitize-undefined-trap-on-error.
Each architecture accepts both guarded current calls (regular index 133,
extended index 84), while each restored original call terminates with
0xC000001D at the compiler-inserted bounds trap. These are isolated synthetic
boundary inputs, not a recorded desktop keyboard incident. The ordinary
production caller's eight-bit mask does not independently bound either table.
The first experimental sanitizer link required unavailable libubsan; that
attempt supplies no runtime evidence. Trap instrumentation avoids that missing
runtime. Separate processes ensure the first failure does not hide the second.

Executor proposal: restore original standalone tables and nonsemantic spacing;
retain the existing two >= repairs and the necessary NT-dependent omission as
qualified owner decisions. Current full qualification remains in progress;
no completed frontier advance or delivery is claimed by this entry.

## File 460 host/src/nt_keycd.c (P151)

All selected differences are reconciled against pinned XP. Existing excluded
bodies remain untouched and uncounted. No duplicate Compat implementation or
new product input path is introduced.

| Point | Original/current anchors | Executor analysis and counter-review disposition |
| --- | --- | --- |
| R0617 | XP 10-12; current before windows.h | Qualified pending: omission of nt.h/ntrtl.h/nturtl.h. Coherent original include restoration fails at unavailable nt.h on both supported toolchains. Remove the non-original standalone summary; retain accurate original line mapping. Coupled NT conversion omission is R0621. |
| R0618 | XP 575-697; current original BIOS conversion tables | Restore all original declarations, data and spacing verbatim. Original tables compile/link and existing mapping tests pass on both architectures. Unused data alone does not justify keeping this 123-line omission. |
| R0619 | XP 713/788/818; current 432/459/477 | Qualified pending: one local explanation and two >= repairs, five selected diff lines total. Original > admits indices equal to 133/84-byte arrays. Both distinct boundaries trap in actual original function on both widths; current guards return zero. No second mapper or caller workaround. |
| R0620 | Selected spacing adjacent to declarations/conditions | Restore exact original blank lines, including the two found in final review. Necessary diagnostic anchors are adjusted accordingly; no executable behavior changes. |
| R0621 | XP 828-995; current EOF | Qualified pending: original BiosKeyToInputRecord omission (168 lines). Restore its tables and function together in the isolated object: actual fixture linkage fails on RtlOemToUnicodeN and NT_SUCCESS on both widths. Full original headers are also unavailable (R0617). No production caller or equivalent Compat converter exists. Retention concerns the selected host ABI, not a claimed defect in the original function. |

Independent counter-review checked every non-excluded diff opcode, including
adjacent selected content, original source mapping and actual consumer scope.
Final selected residuals are exactly three missing NT includes, the five-line
bounds repair, and the missing BIOS conversion function. Existing exclusions
receive no new identifiers or accounting. There is no new semantic difference;
R0617/R0619/R0621 remain owner decisions, not approved retention.

Production source: +128/-5, net +123; tests: +0/-0. Original-relative
added+deleted selected diff decreases 127 lines. This is restored original
source, not a source-line reduction. Measurement uses normalized line endings,
ignores necessary line directives, and verifies unchanged excluded regions
cancel from the before/after delta. No exclusion-body totals are recorded.

### P151 verification and delivery

Cached builds pass: x86 9.53 s and x64 13.07 s. Background full suites pass
188/188 on each width (188.12/148.38 s); native Console/Window, audio and package
suites pass 8/8 on each width (16.34/15.78 s). Total 392/392; no native test is
deferred and no original failure is hidden by retry.

Final exact blank-line restoration is followed by cached rebuilds (9.07/9.76 s)
and 4/4 focused checks per width (keycode, runtime input continuation, mirror
and mirror selftest; 3.51/3.85 s). Both final EXEs equal the fully qualified
candidate after masking only PE COFF timestamp, PE checksum and the unique
NUL-terminated compiler-time literal. No other byte is ignored. All four
incremental build logs contain zero warnings; this does not assert warning-free
recompilation of every cached original source.

- Final x86 SHA-256: 8938e3be62c517ea3cd101d98d514430a39b6fcfa3a98dc6f8ace24cad07934d
- Final x64 SHA-256: 0608c054bf12f2ef2f4b88fc7d07850b0ee4a02b90a2d864430e2e6f53209bfe

Manifest and documentation gates and whitespace checks pass. Mirror CRLF and
own-document LF are preserved. The task scratch is removed after promoting
these results and verifying all owned jobs have exited. Completed frontier
460/472; next host/src/nt_munge.c. Twelve files and outstanding owner decisions
keep S1/T100 open.

## File 461 host/src/nt_munge.c (P152)

The complete file has six diff ranges, all belonging to one horizontal-panning
adaptation: +25/-1 against fixed XP (26 diff lines). No formatting-only or
unproven semantic residue was found. No source/test/build change is made.
The original BIGWIN implementations remain identical to XP.

Actual production is nt_graph.c painter selection -> nt_ega.c low/medium/high
painters -> the shared group helper or ega_colour_hi_munge. The sole helper
consumers are the low painter and both original munging branches; medium/high
painters pass their scanline's panning amount. vga_prts.c's existing
 ega_get_pel_panning returns zero for the configured split region, otherwise
horizontal_pel_panning & 7. No new state, device policy or alternate renderer
is introduced here. Compat has no duplicate plane-to-pixel implementation.

| Point | XP/current anchors | Initial analysis and independent counter-review |
| --- | --- | --- |
| R0622 | XP before conapi.h; current 25-26 | Two includes are qualified pending with the panning unit. Removing egaports.h fails compilation at EGA_planes/EGA_PLANE_DISP_SIZE; removing vgaports.h fails at extensions_controller in the bank-size macro, on both widths. They are direct data dependencies, not gratuitous includes. |
| R0623 | XP before ega_colour_hi_munge; current 45-59 | Qualified pending: shared 15-line group reader. Zero pan returns the original word; nonzero pan combines corresponding plane bytes with the next group, wrapped at the existing V7 display-bank boundary. Actual low painter and original medium/high paths fail pixel assertions when the whole original no-pan unit is restored. |
| R0624 | XP 46/55; current 63/72 | Qualified pending: pan argument and local panned word. The argument comes from existing per-scanline controller state; the local word lets the original lookup-table code consume shifted bytes without changing guest VRAM. Coherent restoration includes the original declaration and callers, so argument mismatch is not evidence. |
| R0625 | XP before 65; current 83-85 | Qualified pending: feed the panned word to the original all-planes-enabled loop. Medium/high matrix with mask 0xf passes current and fails complete original restoration on both widths. |
| R0626 | XP before 109; current 130-132 | Qualified pending: same feed in the original masked-plane loop. Medium/high matrix with mask 0x5 passes current and fails original restoration on both widths. It reuses original masking/LUT logic rather than implementing a second renderer. |

### P152 coherent restoration and verification

Task-local experiments compile the entire exact original nt_munge.c with the
actual device compiler flags. Its original six-argument declaration is restored
in a private header and all selected nt_ega.c consumers are coherently restored:
low directly loads the original word, medium/high remove the panning argument.
The existing Compat nt_graph.h mutex adapter remains intact. Link overrides
only these two device objects in the existing VGA fixture; each run has its
own disposable directory and the runs are serial.

An initial probe accidentally bypassed that Compat header and failed fixture
compilation at assert4. This harness result is not original-code evidence. The
corrected harness preserves the actual wrapper and both complete variants
compile/link on both architectures.

Five independent matrix runs per implementation and architecture select low,
medium, high with mask 0xf, and medium/high with mask 0x5. Each matrix covers
pan 0-7, both bank sizes, normal/wrapping addresses, split on/off and both rows.
The fixture compares shifted output against independently captured unpanned
adjacent output. All ten current runs pass. All ten original runs reach and
fail that pixel comparison (private fixture line 557); x86 returns 0xc0000409,
x64 returns 3. Neither prototype nor linkage failure is counted as runtime
necessity. Four separate include-removal compiles establish R0622 dependencies.

These are actual original renderer/controller-state fixtures, not a new guest
or a new desktop incident. Mask 0x5 and 0xf exercise the two branches; no claim
is made that all possible masks, every display mode or all native scaling paths
were exhaustively tested. Prior P149 real-port panning proof remains separately
identified; it is not substituted for this full-original-unit experiment.

Independent review re-enumerates all six diff ranges and all real consumers.
The residual is one coherent panning capability; R0622-R0626 remain qualified
owner decisions, not automatic retention approval. There is no justified
restoration or smaller existing difference to deliver in this file.

Production +0/-0, tests +0/-0, XP diff delta 0. No production files or build
inputs changed; no redundant EXE rebuild/publication is performed. Published
EXEs are byte-identical to P151 (x86 8938e3be62c517ea3cd101d98d514430a39b6fcfa3a98dc6f8ace24cad07934d;
x64 0608c054bf12f2ef2f4b88fc7d07850b0ee4a02b90a2d864430e2e6f53209bfe).
P151's 392/392 qualification applies unchanged; this is reused evidence, not a
new full-suite run. Documentation/mirror/whitespace gates pass. Task probes
are removed after evidence promotion and verified exit. Frontier 461/472;
next host/src/nt_sb.c. Eleven files and outstanding owner decisions keep S1/T100 open.

## File 462 host/src/nt_sb.c (P153)

The complete file is compared with fixed XP, including all selected declarations,
function bodies and whitespace. The original has no excluded body in this file.
Restore the original MM pointer globals, global types/linkage/defaults, forward
declarations, InitializeIoAddresses, LoadWinmm, DeInstallIoHook and debug tail.
Restore original port-switch structure, alternative-port case labels, diagnostic
calls and comments wherever the admitted host boundary does not require a change.
The alternative MPU ports are not registered by the live initializer; restoring
case labels does not add a second exposed device. No restored unused function is
newly called, and no native WinMM loader or NTVDM lifecycle is activated.

| Point | Original/current anchors | Executor analysis and counter-review disposition |
| --- | --- | --- |
| R0627 | XP includes 1-13; current 1-10 | Qualified pending: three unavailable NT headers and nt_reset.h/softpc.h omitted; existing lifecycle/snapshot dependencies retained. Selected host headers do not supply NTVDM kernel control. Earlier dual-toolchain nt.h failure remains applicable; the original device-control restoration below independently fails on its actual NT types/constants. |
| R0628 | Original globals, declarations and independent functions | Restore exact original definitions, original scalar types/linkage/defaults, initialization-address helper, loader, disconnect helper and debug tail. Current SbInitialize assigns IRQ/DMA defaults before use; both-width device/reset/archive tests pass. No real consumer requires their former omission or private spelling. |
| R0629 | Current 73-109 snapshot functions | Qualified pending: archive.c prepares the device before PIC restoration and restores state afterward. MPU mode and pending ACK are part of the serialized state. Removing the two assignments breaks the same actual-port snapshot probe on both widths; invalid saved mode is rejected. This qualifies the boundary, not every downstream archive implementation. |
| R0630 | Current 112-125 status aggregation | Qualified pending: platform readiness/service and machine reset/destruction consume errors. Suppressing MIDI failure storage fails the existing faulted-output closure assertion. The actual CCPU cancellation-context probe distinguishes ordinary INVALID_STATE from exit cancellation; removing that distinction fails on both widths. Unsupported guest message reporting remains separate from a native output failure. |
| R0631 | Current 127-151 mixer/service/running handoff | Qualified pending: mixer writes reach MidiApplyMixer; executor service reaches SbService; heartbeat handoff reaches SbSetRunning. Existing gain assertions and pause/resume note restoration pass; deleting the running handoff fails the expected 33-message restoration on both widths. No new timer, thread or alternate driver callback is added. |
| R0632 | Current SbPorts and SbInitialize | Qualified pending: selected ios.c owns void-returning table registration, unlike original ConnectPorts' boolean contract. Restoring original ConnectPorts fails compilation at use of void on both widths. Removing preflight collision checks allows foreign ownership to be overwritten and fails !SbInitialize(). Removing the new-lifetime reset fails TimeConstant reset. Optional unsupported MIDI remains absent rather than faking a device; the existing real-port test checks this. |
| R0633 | XP SbTerminate; current 219-258 | Qualified pending: existing status-returning close and original-port disconnection replace NTVDM shutdown. Actual machine destroy/reset consumers propagate failure. Existing close-failure test proves machine/RAM remains owned until successful disposal; repeat termination and adapter removal pass. Original SbCloseDevices depends on unavailable NtVdmControl state, not merely an absent prototype. |
| R0634 | XP 278-486 missing InitDevices/SbCloseDevices/ConnectPorts/InstallIoHook | Qualified pending: paired original device functions fail on VDM_PM_CLI_DATA, PM_CLI_CONTROL_* and VdmPMCliControl in both selected compilers. ConnectPorts independently fails against the actual void registration ABI. InstallIoHook depends on that contract. Reintroducing kernel VDM control, a second output lifetime or a changed global I/O ABI is not an original-first local restoration. |
| R0635 | XP port handlers' lazy initialization and DisplaySbMode calls | Qualified pending: lazy InitDevices would reintroduce R0634; selected REPORT_SB_MODE declarations/implementation are excluded. Original read/write handler logic, diagnostics, spelling, aliases and default cases otherwise return verbatim. Existing exclusions themselves receive no new ledger entry. |
| R0636 | Current UART write and MPU reset/mode switch | Qualified pending: Lib-owned output status replaces HMidiOut ownership; complete-message BufferMidi errors are observed. MPU reset clears the existing parser and resets the same output; original Close/OpenMidiDevice would establish the incompatible native lifetime. Removing parser reset fails existing subsequent note-restoration expectations. Original intelligent-mode no-op/default semantics remain unchanged. |
| R0637 | Current ResetAll 689-692 | Qualified pending: parser, MPU mode/ACK and MIDI status reset precede original FM/mixer/DSP reset. Restoring exact original ResetAll fails machine reset after an injected MIDI fault on both widths. Original reset ordering and all original comments/diagnostics are restored. |
| R0638 | XP SbReinitialize 836-935 omitted | Qualified pending: no actual caller; its original command parser invokes the same unavailable SbCloseDevices/InstallIoHook and NTVDM VDMForWOW ownership. It is not duplicate Compat device logic. No fabricated parser replacement or new runtime configuration path is introduced. |
| R0639 | Selected formatting and local prose throughout | Restore original whitespace, descriptions, debug tail and EOF; remove non-original explanatory prose. No new semantic point is introduced. Original EOF blank lines are deliberately retained rather than manufacturing a formatting difference to satisfy Git's generic EOF heuristic. |

The actual device protocol remains in the mirror. Compat provides PCM/MIDI host
transport, not a second SB port decoder. Restoring unused self-contained original
code is allowed; restoring a second active host ownership path is not justified.
All 24 remaining original-relative line ranges map to the qualified points above.
Qualified residuals are pending owner decisions, not approved retention. Provider
internals in nt_sbdsp.c/nt_sbmid.c remain later files in the frozen order; passing
these caller tests is not a claim to have completed their audits.

### P153 controlled proofs and qualification

The unchanged starting implementation and restoration candidate each pass actual
sb-dma, sb-commands, sb-archive and snapshot-transaction fixtures on both widths:
16 successful runs. Separate actual-flag compiles establish the original host
contract failures above. Six independent withdrawal variants compile and link,
then fail the existing sb-commands assertions on both widths: admission checks,
lifetime reset, original ResetAll, MIDI error storage, MPU parser reset and
running handoff. These are controlled necessity probes, not original NTVDM
product reproduction or claims that every original audio implementation is wrong.

An additional task-private fixture uses the actual port functions to save UART
mode with an unread ACK, reset, restore, resume and emit a note. It also rejects
an invalid saved mode and tests ordinary versus CCPU-exit INVALID_STATE, followed
by delivery of the retained message after cancellation clears. The first probe
incorrectly omitted the resume handoff; the second omitted CCPU enter/leave when
testing exit cancellation. Their failures are harness errors, preserved during
investigation and not evidence of a production defect. Corrected final candidate
passes on both widths. Removing MPU restoration fails the restored status-port
assertion; removing cancellation distinction fails the status assertion, on both
widths. The tests add no production interface and are not represented as new
permanent regressions or an entire snapshot/audio-state proof.

Cached builds pass (x86 10.65 s; x64 11.71 s). Both full background suites pass
188/188 (147.43/146.27 s), and native audio/Console/Window/package suites pass
8/8 (15.83/15.87 s): 392/392. No native coverage is deferred. Incremental build
logs contain zero warnings; cached untouched mirror sources were not all rebuilt.
Final private probes use the same production source as this full qualification.

Production +415/-37, net +378; tests +0/-0. Normalized exact XP comparison with
SequenceMatcher(autojunk=False) changes +164/-776 to +147/-381: original-relative
added+deleted diff falls from 940 to 528, a reduction of 412 lines. Contiguous
ranges decrease from 49 to 24; ranges are not semantic points. This is restored
original code, not a claim that production line count decreased.

- Published x86 SHA-256: 5a2a3fba1107482af28324f9afd85d66498c8da584fa2813f64abc02484216b4
- Published x64 SHA-256: bd4a68abce11cbcfd28c0e9d1ff795442a0ada305d626cd16db5c8a54a5a5b13

The original EOF is byte-identical CRLF with two final blank lines. Plain Git
diff --check flags only those restored blanks; the check with the command-local
core.whitespace=-blank-at-eof passes without changing repository configuration.
No production change is made to suppress this non-semantic original formatting.

Mirror and documentation gates pass. Actual-diff counter-review checks the
original correspondences, all 24 residual ranges, real consumers and fixture
limitations above. Task jobs have exited; scratch is removed after evidence
promotion. Frontier 462/472; next host/src/nt_sbdsp.c. Ten remaining files and
qualified owner decisions keep S1/T100 open.

## File 463 host/src/nt_sbdsp.c investigation (not delivered)

Started after P153 from the fixed XP SP1 counterpart, without consulting OpenNT.
Production remains unchanged and the completed frontier remains 462/472. The
task-private candidate restores eight complete original functions: DspReadStatus,
DspReadData, DspResetWrite, GetSamplingRate, SetDMACountAddr, QueryDMA,
SetDMAStatus and GenerateInterrupt. Their existing header declarations already
use the original SDK types. Restorations recover original diagnostics, scalar
spellings and equivalent high/low-byte operations rather than a new algorithm.

Both before/candidate objects compile and link with actual x86/x64 Ninja flags.
For each variant and width, sb-dma, sb-commands and sb-archive pass: twelve
successful fixture runs. The eight-function candidate reduces XP-relative
added/deleted diff by 58 lines; unchanged excluded regions cancel from this delta
and receive no accounting. Private test EXEs are not byte-identical when masking
only PE timestamp/checksum, so no artifact-equivalence claim is made. Full
product qualification has not been run for this uninstalled candidate.

The wider file audit remains active. Original WaveOut header/ring/thread ownership,
the current single-executor transfer/completion path, physical DMA address access,
pause/resume, parser framing, snapshot state and original unused functions require
separate coherent review. Compat sb_pcm.c owns Lib stream transport and unsigned
8-bit to signed 16-bit sample conversion; it contains no duplicate SB parser or
DMA controller. That source observation alone does not qualify all current DSP
adaptations as necessary. No remaining difference is approved by this entry.

Further coherent restoration recovers original globals and prototypes (except
the live fixed DspBuffer representation and the already shared DSP_MODE type),
plus twelve otherwise uncalled original helpers: FindWaveDevice, OpenWaveDevice,
SetWaveOutVolume, ResetWaveDevice, CloseWaveDevice, TestWaveFormat, SetWaveFormat,
GetWaveOutPosition, WaitOnWaveOutIdle, WriteBurst, PrepareHeaders and
UnprepareHeaders. No caller is added and no WinMM output is opened. Both widths
compile/link and pass the same three device selectors. A subsequent candidate
places the restored groups at their original relative positions and restores
valid original windows.h/ios.h/ica.h inclusion; its six runs also pass. This
aligned private candidate reduces XP diff by 832 lines versus the untouched
production file, with excluded regions unchanged. It is not yet final source.

Four controlled variants of that aligned candidate compile/link on both widths
and fail the existing sb-commands fixture at their corresponding observations:
original increment-only small-block direction fails the DMA-address assertion
(line 1102); original increment-only table-munge direction fails the expected
second byte (1126); replacing physical SAS accesses with logical accesses fails
the paged ADC byte (1116); removing the host running handoff fails the paused
output assertion (914). These are eight failed negative runs, with the aligned
candidate passing on both widths. The first two restore original arithmetic;
the latter two isolate current boundary obligations and are explicitly not a
complete original NTVDM backend experiment. They support direction, physical
memory and pause obligations, not every detail of the present implementation.
All probe jobs exited. Production source and published P153 EXEs remain unchanged;
remaining file-level review and eventual full qualification are still required.

The next refinement restores original ContinueDMA including its single Sleep(0)
yield, original parser diagnostics/type spelling, the original MidiOutPoll name
at the unchanged numeric value, original unreachable parser cases, reset layout
and unused native scalar resets. The physical DMA address helper retains only
its actual host-address translation difference. Original PauseDMA presentation
returns, while its native GetWaveOutPosition branch remains separate from the
Lib stream owner. Existing exclusion placeholders/line directives retain their
sequence and content. Each intermediate candidate compiles/links and passes all
three device selectors on both widths. No new guest code is used.

Four more withdrawal variants compile/link and fail on both widths: removing
ResetState initialization allows the previous lifetime's reset pulse to finish
(fixture 805); original command dispatch fails retained MIDI framing (972);
advancing by requested rather than accepted PCM prefix fails the seven-sample
backpressure assertion (444); resetting the consumed prefix to zero fails the
subsequent archive capture (1705). These eight negative runs are distinct from
the earlier direction/physical-memory/pause probes. Existing completion tests
also observe the 93 ms event deadline, both DMA directions, auto-init, lag,
stream epoch mismatch, progress failure and cancellation through real q-events.

The assembled candidate is now installed for P154 qualification, not delivered.
Production delta is +1178/-340; XP-relative diff decreases by 1222 lines with
unchanged exclusions cancelling. Cached x86 build passes (9.48 s); the owned
background full x86/x64 job is still running at this entry. Final source review
found indentation left behind when removing local comments; a whitespace-only
correction is prepared privately and will require final rebuild/qualification
before delivery. No production source is edited while that job is running.
Frontier remains 462/472, and no full-file acceptance is claimed yet.

The installed first candidate completes cached builds and all 392 tests:
x86 build 9.48 s, background 150.37 s, native 16.98 s; x64 build 19.62 s,
background 150.62 s, native 15.99 s. The owned process is confirmed exited.
This qualifies that candidate, not later final source.

Further original-first review restores the prepared indentation correction,
SbGetDMAPosition diagnostics/comments and finite Sleep(0) yields, plus the
original DspVirtualIncx assignment at block preparation. DESCENT remains
excluded: the restored variable only selects finite host yields on supported
paths, not guest DMA arithmetic. It does not add an archived guest-state field.
Original wait-for-worker looping and DspLock use are not conflated with those
finite yields: current production has no native DSP thread to advance that
wait and does not initialize that native lock. These remaining ownership
changes still require file-level evidence reconciliation and owner decisions.
A fresh full background qualification is running against this revised source;
no new pass or delivery is claimed before its result.

A separate private candidate restores complete original PauseDMA, including
GetWaveOutPosition. Both architectures compile/link and pass sb-dma,
sb-commands and sb-archive (six fixture runs). The original helper makes no
output-open call; an absent WaveOut handle gives its existing failed-query
return without changing guest state. Its omission is not justified merely by
the Lib output owner. This restoration is prepared but not installed while
the full qualification job runs. Frontier remains 462/472.

The next installed revision also passes 392/392: x86 build 15.81 s,
background 146.28 s, native 15.90 s; x64 build 11.85 s, background 147.28 s,
native 16.03 s. Process exit is independently confirmed. A further private
candidate restores ExitAutoMode's original declaration, documentation,
diagnostics and NextWaveOutPos reset while retaining the registered-hook
ownership guard; its six dual-width device selector runs pass. Complete
PauseDMA and that ExitAutoMode restoration are then installed, with SetSpeaker's
original declaration/documentation and the original relative function order.
Final delivery qualification must cover this source, not the preceding pass.
Earlier numeric diff reductions in this investigation are intermediate claims;
delivery will recompute one reproducible Git comparison against the frozen XP
file and the immutable pre-P154 file. No excluded-body count will be recorded.

Metric correction for the installed delivery candidate: the immutable before
file exactly matches HEAD after CRLF normalization, and the original hash is
identical to the manifest's fixed XP raw hash. Normalize the three files to LF
and compare with git -c core.autocrlf=false diff --no-index --numstat, using the
same XP input for before/after. The XP-relative added+deleted delta is -1624
lines; unchanged excluded regions cancel and receive no separate accounting.
Tracked production is +1251/-333 (net +918), no test-source changes. Earlier
investigation reductions, including 1222, are superseded by this reproducible
comparison and must not be reused as final metrics. The increase in source size
restores original bodies; it is not an increase in mirror differences.

## File 463 host/src/nt_sbdsp.c P154

Fixed XP identity and the immutable before file have been verified. This review
covers selected differences only. Existing exclusion bodies and directives are
not decision points. Source positions below refer to the installed delivery
candidate; XP positions refer to the fixed original.

| Point | Location | Initial analysis and counter-review |
| --- | --- | --- |
| R0640 | XP includes 11-26; current 12-26 | Restore available windows.h/ios.h/ica.h in original relative order. Qualified pending: unavailable NT headers and NTVDM host includes are replaced by actual SAS, EOI, q-event, snapshot and PCM declarations. The PCM declaration must precede the core's const macro; external data ownership is not writable. Original Sim32 NTVDM pointers are not standalone guest physical addresses. |
| R0641 | XP globals 31-141; current 31-145 | Restore original scalar types, declarations, native globals and macros. Qualified pending: fixed 65536-byte DspBuffer, existing transfer/host state, and DSP_MODE declared in nt_sb.h. Original GenerateHdrs reallocates and indexes a native WaveOut burst ring; the current copy is one complete 16-bit DMA block, archived by value and submitted by accepted prefix. Restoring only the pointer declaration would be an invalid partial experiment and is not used as proof. |
| R0642 | Current 149-223; XP StartDspDmaThread/StopDspDmaThread and AutoInitEoiHook | Qualified pending: one executor services transfer, EOI registration and terminal status. The complete original owner creates a second thread, mutates DMA/PIC from it and waits on native events. The current runtime's pause, capture and teardown quiesce its executor, not that original extra worker. No second active backend is introduced. Existing service/status callers are nt_sb.c and the machine runtime; they are not themselves sufficient proof without the thread and state-ownership analysis. |
| R0643 | Current 224-302 and 1502-1527; XP SetSpeaker/native format owner | Qualified pending: Lib PCM format, gain, lifecycle and host-running handoff. Original SetSpeaker changes a native device's WaveOut volume and has no Lib stream. Current PCM consumes signed 16-bit copied samples; Compat only converts the DSP's unsigned bytes and transports them, not a duplicate DSP parser/DMA implementation. Withdrawal of the running handoff fails the paused-output assertion on both widths. Original SetSpeaker declaration and documentation are restored. |
| R0644 | Current 303-387; XP DspThreadEntry interrupt-delay and native completion | Qualified pending: executor q-event completion uses the accepted sample prefix, stream consumption/epoch and a cancellable event identity. Original owner waits on WaveOut semaphores/events in its separate thread; putting those waits on the executor prevents that executor from advancing guest/device work or servicing pause. Existing real-q-event tests cover the 93 ms deadline, cancellation, lag, progress error and epoch mismatch. This establishes the boundary, not real hardware timing equivalence. |
| R0645 | Current 388-450; XP DmaDataToDsp 1323 onward and DspProcessBlock 2344 onward | Qualified pending: guest physical DMA addressing, 16-bit wrap/decrement, copied block and accepted-prefix submission. XP converts Sim32 host pointers through ULONG and increments host addresses. That is not an x64-safe guest-memory representation and cannot express the current nonidentity guest paging contract. Original-direction withdrawals fail small-block/table-munge assertions; physical-to-logical access withdrawal fails the paged ADC assertion; advancing queued count by requested rather than accepted samples fails backpressure. The original virtual-increment assignment/diagnostic is restored. |
| R0646 | Current parser declarations 453 onward; XP enums and function-local parser state | Restore original enum names, values and SDK scalar spelling. Qualified pending: UnsupportedMidiUart remains the final state; blockSize and table-munge partial state are file-owned for capture/restore. Original function-static partial bytes cannot be inspected/restored by the device archive. Numeric MidiOutPoll=12 and UART=13 remain unchanged. |
| R0647 | Current 598-743; XP has no corresponding device archive | Qualified pending: capture/restore and callback encoding preserve copied PCM, DMA/parser state, consumed prefix and cancellable event identity. A restored native handle or thread would refer to the old host lifetime, not archived guest state. Removing consumed-prefix preservation fails archive capture on both widths; that withdrawal changes both pause and restore assignments and is not presented as a restore-only experiment. Existing archive tests cover partial parser commands and resumed sample ownership. |
| R0648 | Current SbInitializeDMA and original host preparation/cleanup omissions | Qualified pending: channel/controller selection is used by current original DMA APIs without allocating an NTVDM native WaveOut owner. GenerateHdrs, PrepareWaveInitialization and CleanUpWave are coupled to allocation/free of the original pointer ring, native semaphore/event handles and extra thread. Their coherent original owner also uses ULONG-truncated host pointers. Unrelated independent original native helpers are restored instead of treating every WaveOut reference as grounds for omission. |
| R0649 | DspReadStatus/DspReadData/DspResetWrite/GetSamplingRate/SetDMACountAddr/QueryDMA/SetDMAStatus/GenerateInterrupt | Exact original function restoration, including original diagnostics, types, byte operations and formatting. Both before and initial restored candidates compile/link and pass all three device selectors on each width. Original no-op command cases and diagnostics throughout DspWrite are restored too. |
| R0650 | WriteCommandByte and DspWrite MIDI states | Qualified pending: unsupported DSP MIDI consumes its payload without reinterpreting it as PCM commands; supported MIDI remains MPU-401. Original command dispatch withdrawal fails the framing assertion. The original MidiOutPoll name returns; its BufferMidi call remains omitted because that would activate the distinct unsupported DSP MIDI output route. No new decoder or consumer is added. |
| R0651 | DspWrite small blocks, TableMungeBuffer and GetDMATransferAddress | Qualified pending: direction-aware address changes and physical SAS access. Both original increment-only withdrawals fail the corresponding address/data assertions; logical access fails nonidentity paging. GetDMATransferAddress retains its original ULONG declaration/description and only the physical-address boundary changes. No alternate DMA controller is introduced. |
| R0652 | ResetDSP | Restore original layout, comments and native scalar resets. Qualified pending: executor stop/clear replaces the original native-thread/event handshake; ResetState is reset for a new device lifetime. Removing the added reset-pulse clear makes an old lifetime's partial reset finish in the next lifetime, failing the actual port fixture on both widths. No claim is made about every reset race. |
| R0653 | PauseDMA/ContinueDMA and twelve independent native helpers | Restore ContinueDMA and independent original helper bodies. Qualified pending: omit PauseDMA native position query. Actual production never calls LoadWinmm, leaving GetPositionProc null. Corrected active Single-DMA pause fixture fails with access violation 0xc0000005 on both widths when the actual original body is restored; current passes. The earlier candidate accidentally matched a prototype and is not evidence for restoring this body. No new native output owner is activated. |
| R0654 | SbGetDMAPosition | Qualified pending: readiness uses the current active transfer, not HWaveOut; no wait for an absent worker or uninitialized native critical section; address progress handles decrement and 16-bit wrap. Restore original diagnostics, comments and finite yields. DspVirtualIncx only controls finite host yields in supported branches and is not guest DMA arithmetic; no snapshot layout is changed for it. |
| R0655 | ExitAutoMode | Restore original structure, declaration, diagnostics and native scalar reset at original relative position. Counter-review rejects the outer registration guard: RemoveEOIHook already checks IRQ and callback identity and preserves other callbacks. Restore the original unconditional removal; only clearing the archive-visible registration flag remains qualified pending. Both widths pass all three private selectors with the guard removed. |
| R0656 | Selected nonsemantic content and original relative order | Restore original types, descriptions, whitespace and independent bodies; remove non-original prose. Restore SetSpeaker/ExitAutoMode placement rather than introducing duplicated original text elsewhere. Existing exclusions remain untouched. Three existing line-control values must follow the newly restored original statements: 2230 for the closing brace, 2268 for the position diagnostic, and 2921 before ExitAutoMode. No new exclusion entry is created. |

Independent counter-review checks actual source dependencies rather than relying
on tests alone: whole-original backend restoration requires the NTVDM native
thread/ring/pointer owner, while supported runtime pause and snapshots require
executor quiescence and copied guest physical state. The coherent original owner
is not dynamically reimplemented as an experiment, and no original NTVDM product
failure is claimed. Narrow withdrawal results qualify only their named boundary.
The remaining entries are owner decisions, not automatic retention approval.

Full qualification of the installed final candidate is still running. This
section does not advance the delivered frontier or claim P154 committed.

The final counter-review finds redundant EOI ownership checking in ExitAutoMode:
RemoveEOIHook already rejects wrong IRQ/callback identity and safely rejects an
absent hook. The original unconditional call plus the existing archive flag
clear passes all six private device selectors. The earlier assertion that the
outer guard itself was necessary is withdrawn; R0655 records the correction.
Three existing #line values are corrected to follow the restored XP statements,
without adding directives or touching excluded bodies. This candidate is staged
privately until the currently running full qualification exits.

The pre-guard-removal delivery candidate passes all 392 tests: x86 build
9.89 s, background 151.68 s, native 15.72 s; x64 build 13.39 s,
background 152.11 s, native 15.61 s. Both its job and the EOI private probe
are confirmed exited. The final two-line guard reduction and three line-control
corrections are now installed; a fresh final-source qualification is required.
No failed test was suppressed and no desktop test was omitted.

Important correction: the earlier PauseDMA private restoration selected its
prototype, not its function body. Statements above that complete PauseDMA was
restored, installed or proved safe are withdrawn. Production retained its
query omission throughout; no null-function-pointer call was published.
LoadWinmm has no production caller, so GetPositionProc stays null. A corrected
private fixture pauses/continues immediately after a Single transfer is armed.
The same fixture and build flags compile/link both actual-body variants:
x86/x64 original query each exits 0xc0000005; current each passes. This is a
concrete supported-path counterexample, not just a native-header mismatch.
R0653 now records the qualified omission. The existing completion fixture will
retain this two-line regression. A brace-aware same-class review confirms all
21 other claimed complete restored functions exactly equal their XP bodies;
it distinguishes function definitions from prototypes and covers the eight
initial functions, twelve independent helpers and ContinueDMA.

### Final qualification and delivery

Final source: production +1257/-341 (net +916), test +2/-0. The frozen XP
comparison decreases by 1628 added/deleted diff lines using the normalized
Git method above. Earlier provisional figures do not supersede this result.
Restored source growth is not mirror-diff growth. Existing exclusions are
unchanged; only necessary line-control values follow the recovered statements.
No new production path, dependency, runtime switch or guest modification exists.

Both final-source cached builds and all 392 tests pass:

| Architecture | Build | Background | Native desktop/audio/package |
| --- | --- | --- | --- |
| x86 | 10.01 s | 188/188, 146.21 s | 8/8, 16.31 s |
| x64 | 11.10 s | 188/188, 151.72 s | 8/8, 16.20 s |

The two-line active-DMA pause regression was then added to the existing
completion matrix. Rebuild only the affected test target on both widths and run
app-softpc/unit/machine/device_contract_sb_commands: 1/1 each. Published EXEs
remain byte-identical to those qualified by the complete suite. No build warning
or error appears in these final build logs. No native test is deferred.

Published SHA-256:

- softpc32.exe: `78fce56f93f8bd9a57ca4e9af383bb9b00dbabb15abf03ced6333dc2b3181f78`.
- softpc64.exe: `65d10a71600d4864ce0ed83dc4290935b8a68862d6806dea258848528c1dec52`.

Actual-diff review verifies all 21 complete original function bodies, remaining
selected ranges against R0640-R0656, real producer/consumer ownership, physical
address and partial-transfer withdrawals, and the corrected PauseDMA negative.
Tests do not prove every NTVDM host behavior or guest application. Native owner,
physical-memory, lifecycle and archive differences remain qualified owner
choices; no retention approval is inferred from passing tests. All task jobs are confirmed exited; task-owned scratch is removed and the
build residue check passes. File frontier advances
to 463/472 on P154 delivery; next host/src/nt_sbfm.c. Nine files and qualified
owner decisions keep S1/T100 open.


## File 464 host/src/nt_sbfm.c investigation (not delivered)

The fixed XP raw hash matches its manifest. Production is unchanged after P154.
Source-body brace review finds a real semantic difference: XP FMDataWrite updates
timer flags inside AdlibRegister == AD_MASK (04h). Note-register writes may flush
its native batch, but are not inside that status-update condition. Current code
updates flags for B0h..BDh too; the current exhaustive fixture encodes that same
broadened behavior. Its green result is not evidence for retaining the difference.
The local comment claiming that note-register behavior matches XP is incorrect.
No guest failure is inferred from this source fact alone.

Next coherent experiment will restore original selected declarations, reset/read/
select/write structure and the inactive native batch helper, retaining existing
exclusions and the actual archive boundary. FMActive starts false and production
has no OpenFMDevice caller; the original batch helper's false branch clears its
position and succeeds without native WriteFile. Source review must distinguish
this reachable no-hardware path from the NTVDM kernel/direct-I/O open/close path.
No new synthesizer, port owner or runtime route is admitted. The fixture's
expected transitions must follow the original AD_MASK guard rather than copying
the current expanded condition. No file completion or P155 qualification is
claimed yet; delivered frontier remains 463/472.


## File 464 host/src/nt_sbfm.c P155

The fixed XP normalized raw hash d2ee18de413c7b2b0c12314bc1316afb040d9339d12087853bbc4361fe845607
matches the manifest. Actual production handlers remain VsbByteIn/VsbByteOut and
ResetAll in nt_sb.c; the device archive calls capture/restore of selected register
and Status. No native FM open/close caller is added. The unit target compiles this
actual mirror source, not a duplicate implementation.

| Point | Location | Initial analysis and counter-review |
| --- | --- | --- |
| R0657 | XP includes 11-20 | Restore windows.h and original host_def.h/insignia.h order. Qualified pending: omit unavailable nt.h/ntrtl.h/nturtl.h/softpc.h and retain the actual archive declaration. Exact whole-original compilation fails at nt.h on both supported toolchains; no external source is made a build dependency. |
| R0658 | XP globals 22-41 and ResetFM 49-59 | Restore original globals, SDK types, native batch declarations, timer bookkeeping and exact ResetFM body. FMActive remains false: neither the original native open routine nor another production writer is wired. Therefore batch position/timer bookkeeping do not acquire an independent state owner. |
| R0659 | XP FMStatusRead 61-103 and FMRegisterSelect 105-111 | Restore original types, declarations and selected formatting. Existing excluded input body is untouched. Reads retain the original Status semantics and do not clear flags. |
| R0660 | XP FMDataWrite 113-176 | Restore original selected function structure, including batching and AD_MASK-only status updates. The former simplification treated note registers B0h..BDh as timer-control writes. Its explanatory comment and corresponding fixture expectation were incorrect, not evidence of an intentional required device behavior. No user symptom is claimed as its consequence. |
| R0661 | XP FMPortWrite 184-214 and FindFMSynthDevice 216-262 | Restore original selected helper bodies. FMPortWrite with FMActive false clears Position and succeeds without WriteFile. FindFMSynthDevice has no new caller; its original callback declarations resolve from nt_sb.c in production. The isolated FM fixture supplies the two unused callback pointer objects, leaving them null, rather than linking an unrelated whole machine or activating MIDI hardware. No FM output backend is introduced. |
| R0662 | Existing capture/restore additions | Qualified pending: selected register and Status are the guest-observable archived state. Restore validates the eight reachable status encodings before mutation; all selected byte values are legal. The exhaustive fixture proves archive round-trip and invalid-state rejection. Newly restored batch storage is inactive host submission bookkeeping: with FMActive false, flushing cannot change Status except through the original AD_MASK branch and has no host side effect. |
| R0663 | XP OpenFMDevice/CloseFMDevice 264-360 | Qualified pending omission: these call NtVdmControl for direct I/O and kernel/user ADLIB emulation, using VDM_ADLIB_DATA and NTVDM service constants. Complete original bodies, with the restored original globals/helper declarations and ordinary Windows headers, fail compilation on both widths at the unavailable NTVDM types/constants. This demonstrates a native host boundary, not a runtime failure of the XP product. Adding those declarations alone would not provide the interpreter's missing NTVDM kernel service. No new stub, alternate lifecycle or host capability is invented. |
| R0664 | Nonsemantic differences and original EOF | Restore original descriptions, comments, spacing and trailing blanks; remove local prose asserting the wrong note-register behavior. Existing exclusions remain skipped. The existing native-debug line-control directive follows its restored original position and resumes at original line 209; no new excluded body is created. |

Controlled comparison uses actual Ninja compile/link flags and the actual source
on x86/x64, with no guest or asset modification. Each width runs three variants:
old implementation plus old fixture expectation passes; old implementation plus
the XP AD_MASK expectation fails the status assertion at fixture line 39;
restored implementation plus that same expectation passes. These are four passes
and two deliberate failures, all compiling/linking successfully. The corrected
matrix checks both low-bit reset states, four prior flag combinations, every
register and byte value (524288 transitions), repeat reads and snapshot round-trip.
The new expected guard is derived from original scope/braces, not from the
modified implementation's condition. The old assertion's success is explicitly
not treated as correctness evidence.

Separate production-flag compiles of the exact whole XP file fail at nt.h on
both widths. Compiles of the coherent native open/close bodies with ordinary
Windows declarations fail at VDM_ADLIB_DATA and related service definitions.
These four negative builds qualify only the native boundary; they do not justify
omitting ordinary no-hardware behavior or independent original helpers.

Installed candidate: production +171/-26 (net +145), tests +4/-1 (net +3).
Using the same normalized Git comparison before/after, XP-relative added/deleted
diff decreases by 189 lines. Existing excluded bodies cancel and receive no
accounting. The first cached x86 build passes; full x86/x64 qualification is
running. Remaining choices are qualified owner decisions, not approved retention.
Delivered frontier remains 463/472 until P155 verification, cleanup and push.

### Final verification and P155 delivery

Final-source cached builds and all 392 tests pass:

| Architecture | Build | Background | Native desktop/audio/package |
| --- | --- | --- | --- |
| x86 | 11.21 s | 188/188, 143.38 s | 8/8, 16.15 s |
| x64 | 12.92 s | 188/188, 142.63 s | 8/8, 15.84 s |

Additional controlled archive withdrawals compile/link on both widths. Omitting
selected-register restoration fails the next-write assertion (fixture line 47);
omitting Status restoration fails the immediate restored-state assertion (44).
These four deliberate failures qualify the archive fields in R0662, not every
possible future archive format. The restored fixture passes the full matrix.
No desktop verification is deferred; no real OPL audio capability is claimed.

Independent actual-diff review confirms the frozen before file equals HEAD,
fixed XP identity, CRLF mirror format, exact complete ResetFM/FMRegisterSelect/
FindFMSynthDevice bodies, and original selected FMDataWrite/FMPortWrite structure.
The production reference scan finds only HFM and native open/close declarations
outside this file, no caller that activates native FM. Original batch data remains
host-inactive; no additional archived guest state is needed for that buffer.
Production +171/-26 (net +145), tests +4/-1 (net +3); XP diff falls by 189 lines.
Mirror and documentation gates plus actual-diff checks pass. Task jobs are
checked exited and scratch removed before commit. Qualified owner decisions
remain R0657/R0662/R0663; restored behavior is not declared a new OPL implementation.

Published SHA-256:

- softpc32.exe: `01a3c46ada247b30e97bd0a37b7179bffef697c3b1952c2ca1baad9fffa309a0`.
- softpc64.exe: `39c06e1784703f6621b0990b7eb7d0010bd46b8f30225c6ccf5a2341c13c6e4c`.

P155 advances the delivered frontier to 464/472; next host/src/nt_sbmid.c.
Eight files and qualified owner decisions keep S1/T100 open.

## File 465 host/src/nt_sbmid.c investigation (not delivered)

The fixed XP counterpart is read from the pinned commit, not a sibling working
tree. Current production compiles the file in softpc-machine-devices. The actual
MPU UART data path is SbIOWrite -> BufferMidi; SbService drains SendMidiRequest;
SbSetRunning invokes MidiSetRunning. The SB archive captures/restores the MIDI
parser and logical voices. Compat sb_midi owns Lib MIDI output, cancellation,
volume and native lifetime. These consumers establish scope, not necessity of
every local deviation. No production or permanent test changes are installed.

An isolated experiment includes the complete, unchanged XP nt_sbmid.c body.
Only platform declarations/diagnostics and the native short-message sink are
provided by a private fixture. REPORT_SB_MODE remains disabled. Both compiler
widths build and execute. To isolate parser behavior from pointer truncation,
the fixture reserves its buffer at 0x20000000 and initializes the original ring
fields as InitializeMidi does; it does not run native device initialization.
Input 90 3C 64 produces zero sends; an explicit SendMidiRequest still produces
zero. Completing running-status bytes 3D 65 sends only the preceding 00643C90.
A following F8 sends 0000653D; another explicit drain still leaves F8 queued.
Both architectures produce the same trace. This proves original tail retention
at the native-call boundary, not a real synth's interpretation of running status
or a historical guest symptom.

A separate x64 invocation reserves the buffer at 0x120000000. Reservation is
asserted successful before calling the original parser; its ULONG-based pointer
arithmetic then exits with 0xc0000005. This isolates the pointer-width defect
from the tail observation. The negative exit is expected evidence, not a passing
test. No real MIDI device, guest, media, configuration or published EXE is used.

Counter-review: these two failures do not justify the whole replacement parser,
voice continuation or all omitted original host helpers. Full-file reconciliation,
coherent smaller-restoration experiments, archive/lifetime withdrawal evidence
and XP-relative metrics remain unfinished. No point is declared qualified solely
because current tests pass. Frontier remains 464/472; R0665 is still the next
unallocated decision ID. Task-owned probes remain in build/t100-s1 only while
this investigation is active; their actual processes have exited.

### File 465 point review and restoration candidate

| Point | Original/current difference and first review | Independent counter-review and disposition |
| --- | --- | --- |
| R0665 | XP includes windows.h and defines the ring constants, CMDHDR, LONG parser counters and native declarations. Local storage uses a bounded copied-message array and extra pending fields. | Restore original include, constants, type, independent declarations and LONG counter spelling/initialization. Keep the array/pending fields as qualified message-boundary choices: the high-address probe proves original pointer arithmetic unsafe, but does not by itself justify every field. |
| R0666 | XP OpenMidiDevice, SetMidiOutVolume, ResetMidiDevice, CloseMidiDevice, MidiCopyMessages and DetachMidi were absent. | Restore all six complete original bodies. None has a current production caller; the preserved native declarations are initialized inactive. Current output stays on the existing Compat/Lib path. Original flaws in these inactive helpers are not silently fixed. |
| R0667 | XP InitializeMidi allocates/owns the ring, native message buffers and native device; current SbInitialize opens Compat/Lib and resets the parser. | Whole original activation cannot simply replace the current path: both-width original probes show tail retention, and even a pointer/tail-corrected original ring fails the real Lib message validator for running status, interrupted SysEx and undefined common messages. Native reset waits for queued headers before issuing cancellation; this is different from current cancellable lifetime. The omission remains a qualified owner choice, not proof that no redesign could reuse more original code. |
| R0668 | Current capture/restore additions serialize parser prefixes, pending messages and logical voices; XP has no archive implementation. | Withdrawing prefix restoration fails the actual existing round-trip fixture on both widths. The state validator prevents restored out-of-range indices and invalid message prefixes; existing negative archives are checked before mutation. The evidence supports the copied state contract, not blanket approval of its implementation or future archive formats. Qualified owner choice. |
| R0669 | PruneMidiNotes, RememberMidiMessage, RestoreMidiNotes and MidiSetRunning add pause/reset and voice continuation absent from XP. | Removing note tracking fails expected restored notes; omitting host reset on pause fails the reset assertion, both widths. Supported current user behavior includes approximate continuation. Keep these as qualified owner choices; no claim of sample-exact synth envelopes or host polyphony. |
| R0670 | ResetMidiParser initializes current message/pedal state without opening a second host device. | Current MPU reset/initialization callers are traced in nt_sb.c. This is coupled to the retained copied-message/archive contract; original InitializeMidi is not an equivalent parser-only reset. No new entry is added. Qualified owner choice. |
| R0671 | SendMidiRequest returns status, sends complete messages including the final one, keeps failed output pending and accounts for notes. XP's void sender excludes LastCommand and ignores native submission errors. | Tail retention is dynamically proved against the unchanged original body. Removing current failure propagation fails the exact host-error fixture on both widths. Restore original function layout and description; complete-message/status behavior remains a qualified owner choice. |
| R0672 | BufferMidi differs in real-time interleaving, incomplete-message handling, SysEx bounds/EOX and materialized running status. Its unchanged command length calculation and diagnostics were reformatted/removed. | Restore the original signature layout, descriptions, matching length calculation, running-status diagnostic/control structure and final byte-store/increment statements. Removing materialized status or oversize tracking fails both-width fixtures. The corrected-original-ring probe isolates further incompatibility after fixing pointer/tail defects. Keep necessary parser adaptations as qualified owner choices, not a newly approved wholesale rewrite. |

The actual existing sb_midi_smoke is compiled with each architecture's Ninja
fixture flags. Baseline and the first restoration candidate pass on both widths.
Six controlled withdrawals per width fail at their intended observable assertions:
note tracking (message bytes), prefix restore (message existence/length), host
reset on pause (reset count), host error propagation (LIMIT_EXCEEDED), materialized
running status (message existence/length), and SysEx overflow rejection
(UNSUPPORTED). All variants compile/link; these are twelve intentional assertion
failures, not failed release tests. This evidence is limited to those contracts.

A further coherent private original-ring experiment corrects pointer width and
the ready-tail boundary in both copy/send loops. Its native sinks call the real
audio_midi_short_pack/audio_midi_sysex_validate implementation, without opening
audio. Both widths accept 90 3C 64; they reject original packed 3D 65 without a
status, F0 01 and 02 F7 fragments around F8, and F4. This is a boundary comparison,
not a proposed patch or a guest reproduction. No new production path is added.

Original SetMidiOutVolume is also exercised with controlled MMRESULT values.
MMSYSERR_NOERROR leads to zero set calls and unchanged saved volume; an error
leads to one set call and adoption of the output value. Both widths agree. This
is an original error-check inversion, recorded with this diff decision rather
than TODO; the restored inactive function is not patched. Current Compat volume
logic already distinguishes OK/UNSUPPORTED and retains cancellation ownership.

The larger candidate restores all six independent original helpers and passes
the existing MIDI fixture on both widths. Its seven null native callback-pointer
definitions belong only to the isolated test link; production obtains these from
nt_sb.c. They do not open a synthesizer. Source growth is intentional original
restoration; the normalized XP-relative added/deleted diff decreases by 407
lines. Excluded bodies remain untouched and receive no new accounting.
The smaller candidate passed 392/392 but does not qualify the larger candidate.
The larger candidate's first full build caught missing native callback-pointer
objects in the separate sb_midi_native_smoke fixture. The same seven declaration
objects were added there, with no behavior mocks or new test paths. Both owned
fixtures use LF; the mirror stays CRLF. This link failure is not described as a
host MIDI failure. Its evidence is retained until final delivery.

Final source is installed: production +438/-37 (net +401), tests +16/-0 across
the two existing MIDI fixtures. Independent brace-aware review verifies all six
restored bodies exactly match the fixed XP source, whose normalized SHA-256 is
75b271b759c1da2b940dbc54c5c227b10546167b49c0f1394b385392b41b603d.
The existing SbInitialize -> ResetAll -> ResetMidiParser chain initializes the
active parser before use; restoring original static initializers does not replace
that lifecycle. The restored native helper chain has no existing activation
caller; internal calls among those original helpers do not activate a device.
Final-source dual-architecture qualification is in progress. No full-file
delivery/frontier advance is claimed until it passes and the P is pushed.

## File 465 host/src/nt_sbmid.c P156

Final-source cached builds and all 392 tests pass:

| Architecture | Build | Background | Native desktop/audio/package |
| --- | --- | --- | --- |
| x86 | 7.22 s | 188/188, 151.87 s | 8/8, 20.26 s |
| x64 | 12.53 s | 188/188, 149.9 s | 8/8, 15.89 s |

Actual production diff is +438/-37 (net +401); test fixture declarations are
+16/-0 (net +16). XP-relative added/deleted diff decreases by 407 lines. This is
original code restoration, not a claim that fewer source lines mean less diff.
The complete six-body comparison, real caller scan, fixed XP hash, CRLF/LF
verification and final source identity have been independently checked. No
shared eight-tree, guest, configuration, media or snapshot changes are included.
No native test is deferred. No new real-guest MIDI symptom or audible fidelity
claim is made from isolated parser/native-call evidence.

R0666 is restored; R0665 and R0667-R0672 retain qualified owner decisions.
Original inactive helper defects are not new repairs. The first incomplete
native fixture link is superseded by the complete final build, not hidden as a
desktop failure. The earlier smaller candidate's passes remain separate evidence.
Task jobs were confirmed exited and disposable scratch removed before commit;
the build-residue check passes. Neither final test log reports skipped tests.

Published SHA-256:

- softpc32.exe: `31dfefd2b2ce6f797121506fa77cf4668915b6e75a4a6541236cac108fbed5cf`.
- softpc64.exe: `bf63631f5412b2c8b01f55130f5860aba8e66b7121d907548a2d3465d335953a`.

P156 advances the delivered frontier to 465/472; next host/src/nt_sbmxr.c.
Seven files and qualified owner decisions keep S1/T100 open.

Build warning scope: x86 emits no warning; x64 emits twelve pointer-conversion
warnings, all in the restored original inactive nt_sbmid.c helpers/macros across
its compile consumers. No public component or owned fixture warning is present.
These original bodies are not safe for an unreviewed future x64 activation; their
lack of an actual activation caller is checked, not assumed from passing tests.
The owner's mirror-warning exemption applies; no suppression or new original
pointer patch is introduced to make this audit look warning-free.

## File 466 host/src/nt_sbmxr.c review (qualification pending)

The fixed XP file and current complete body are compared in frozen order. The
production device object owns CT1335 registers; SbIOWrite calls MixerAddrWrite/
MixerDataWrite and SbIORead calls MixerDataRead. SbApplyMixer routes the current
register-derived gain to MIDI and to an active PCM stream. Stream creation and
archive reconstruction also obtain gain from these registers; Compat does not
own a second mixer. Current native HWaveOut/HFM/HMidiOut are inactive: their
opening functions have no production activation callers. Restored native handle
branches therefore do not start another output path.

| Point | Original/current difference and first review | Independent counter-review and disposition |
| --- | --- | --- |
| R0673 | Windows include, original local prototypes, BYTE fields/parameters and VOID spelling had been replaced/removed. | Restore exact original declarations and spelling. Current widths/ABI are unchanged, confirmed by both-width fixture builds and behavior. No rationale remains for these substitutions. |
| R0674 | The local mixer archive copies selected register and four byte values; XP has no archive functions. | Withdrawing those restorations fails the saved selector/value assertion on both widths. Invalid selector rejection and absence of premature host application remain covered. Existing SB snapshot callers need this contract. Qualified owner choice, not a claim that XP had snapshots. |
| R0675 | Three setters replaced original native branches with SbApplyMixer. | Restore all original branch bodies, declarations and comments; retain the one existing apply call in each setter. Withdrawing those calls fails reset's three-application assertion on both widths. Original native handles are not the current Lib stream objects; no second activation or opaque-handle alias is introduced. The three apply lines remain a qualified owner choice. |
| R0676 | MixerGain/MixerGetVoiceGain/MixerGetMidiGain combine master and device registers and special-case decoded zero. | Original-body probes show direct zero writes yield FFFEFFFF and independent master/voice writes overwrite each other. Withdrawing composition fails current combined-gain assertions. However, withdrawing the zero special case unexpectedly passes: unsigned 32-bit composition and final 16-bit conversion already produce zero. Exhaustive before/after proof confirms equivalence, so remove that unnecessary branch instead of qualifying it for retention. The remaining gain functions are a qualified owner choice for the current single output contract. |

The exact original function bodies run with controlled native sink callbacks on
x86/x64, without opening host devices. Master, voice and MIDI zero each produce
FFFEFFFF. Master 2 followed by voice 4 produces 7FFF7FFF; reversed order produces
1FFF1FFF. These are native-call values, not acoustic measurements or a new claim
of hardware-perfect CT1335 calibration. They show why independent original writes
cannot substitute unchanged for the existing composed output contract.

The first withdrawal experiment expected a failure without the zero guard but
observed a pass; that expectation was rejected, not concealed. Independent tables
of eight master/FM and four voice levels, using 64-bit expected multiplication,
then check every pair of raw register bytes through the actual before and after
functions on each architecture: 131072 voice/MIDI output assertions per run,
524288 total across two versions and two widths. Each verifies IU32 is 32 bits;
all pass. A separate explicit 32-bit arithmetic enumeration also finds zero
differences. This is evidence for removing the branch, not for retaining it.
The source argument is bounded to these callers: positive decoded gains are at
least 8191 and at most 57343. A zero factor becomes UINT32_MAX; multiplication
by a positive supported factor wraps to 2^32 minus that factor, whose quotient
by 65535 is 65536 and whose final IU16 value is zero. Two zero factors wrap to
one, also yielding zero. Positive-factor products fit IU32. This is not a generic
identity for arbitrary future gain ranges or a wider intermediate type.

The existing mixer fixture passes for baseline, restored and guarded variants on
both widths. Withdrawing host application, composition or archive restoration
fails the intended assertions on both widths (six controlled failures); every
variant compiles/links. Test additions supply four original native handle/callback
declarations only, initialized inactive; no test-only production API is added.

Installed candidate restores every original XP line in its original order:
normalized XP comparison has zero deletions and 48 local added lines, versus
67 additions/65 deletions before; mirror diff decreases by 84 lines. Production
diff is +69/-23 (net +46), tests +5/-0. Remaining additions are the archive include/
functions, three apply calls and the current gain calculation. No excluded-body
accounting is introduced. Final-source dual builds/publication and full regression
are running; delivered frontier remains 465/472 until P157 completes.

Independent final-diff check verifies fixed XP normalized SHA-256
12d2a9c58306ab4ba7686e80afeeabd429c61a926ad2d816e38ba98978b0a890,
48 additions/zero deletions against that original, exact installed/candidate
identity, mirror CRLF and owned-test LF. Both cached build logs contain no
warnings or errors; this is not a claim about recompiling unrelated old mirror
objects without cache.

## File 466 host/src/nt_sbmxr.c P157

Final-source cached builds and all 392 tests pass:

| Architecture | Build | Background | Native desktop/audio/package |
| --- | --- | --- | --- |
| x86 | 9.33 s | 188/188, 143.82 s | 8/8, 15.63 s |
| x64 | 12.39 s | 188/188, 142.61 s | 8/8, 15.95 s |

Final actual-diff review confirms all original XP lines are preserved, the native
handle branches remain inactive in production, and the existing Lib gain/archive
path remains the sole active one. Production +69/-23 (net +46), tests +5/-0;
XP-relative diff reduced by 84 lines. No public eight-tree, guest, configuration,
media or snapshot-format change is included. R0673 is restored; R0674-R0676 retain
qualified owner decisions. The proven-redundant zero branch is removed, not sent
for approval as necessary. Tests establish present contracts and exhaustive gain
equivalence, not hardware/acoustic fidelity or all future arithmetic changes.
No final test was skipped. Mirror/documentation/diff and scratch cleanup checks
pass; all task jobs were confirmed exited and disposable scratch removed before
commit. Reusable Ninja/ccache trees and user assets remain intact.

Published SHA-256:

- softpc32.exe: `4a96e3782d9204b872b5543f6b4c15c770da4a31c0d816f25a3c7d48d44ae3bd`.
- softpc64.exe: `e775b29e3faf5fd8e8e71cb33238aad427c7826ac2a426fbe1fd82dd8055b5d9`.

P157 advances the delivered frontier to 466/472; next host/src/nt_sound.c.
Six files and qualified owner decisions keep S1/T100 open.

## File 467 host/src/nt_sound.c review (qualification pending)

The complete non-excluded file is compared with fixed XP normalized SHA-256
553c2d22e29b0634c5d2e40423c55b420578c3dd839776c9c32ed4dd8d91a7f9.
It is compiled into softpc-machine-devices. Original ppi.c calls HostPpiState;
timer.c calls host_timer2_waveform; Compat platform's periodic service calls
PlayContinuousTone. The original PPI/Timer2 frequency and gating algorithms
remain the sole state owner. Compat audio receives frequency/duration requests
and supplies PCM to Lib; it does not duplicate the PPI calculation. Machine
driver start/stop owns that audio worker. InitSound has a declaration and test
consumer but no production call; its test is not evidence of a product shutdown
requirement. No production assignment opens hBeepDevice or sets BeepCloseCount.

| Point | Initial review | Independent counter-review and disposition |
| --- | --- | --- |
| R0677 | Four NT-private headers are omitted at XP lines 1-4. | Coherent full pinned original compilation with the actual sound fixture's x86/x64 flags fails at missing nt.h. The selected desktop toolchains do not provide the original NTVDM build environment. This qualifies the header omission; it does not by itself prove every changed runtime statement necessary. |
| R0678 | The local undef prevents the Windows PlaySound macro rewriting the original local function name. | Removing just this adaptation fails both-width compilation: PlaySoundA's SDK declaration conflicts with the original one-argument function. Qualified owner choice; no renaming or second API added. |
| R0679 | Original lock declarations, Beep handle/count and native cleanup statements were removed, with extra includes, comment and formatting. | Restore original declarations, variables and cleanup statements exactly; remove unnecessary includes/comment and sink-adjacent blank. Existing helper fixture and additional real-handle cleanup probes pass both widths. Inactive handle cleanup does not activate a second native output. |
| R0680 | Two empty host_ica_lock/unlock macros replace calls into the old NT host synchronizer. | Actual selected ica.c also uses no-op macros; the repo has no callable host definitions. Removing these macros with the original declarations restored produces unresolved host_ica_lock/unlock symbols on both widths. The single executor owns the state transitions; the separate PCM worker uses its own request synchronization. Qualified existing ABI decision, not evidence that arbitrary future threads can mutate this state safely. |
| R0681 | OpenBeepDevice, NT session checks and LazyBeep's native request are replaced by the existing tone sink. | Original nonzero-session behavior directly calls synchronous Beep with the original producer's INFINITE duration. That is not the current return-after-submit contract. The Session-0 branch instead needs NtCreateFile/NtDeviceIoControlFile and NT-private types; no matching initialized native owner exists here. Keep this existing endpoint difference qualified for owner choice. Restore all independent cleanup rather than retaining its blanket deletion. InitSound's missing SessionId guard is a native-ABI omission, not a proven user-visible shutdown fix. |

The original full-source compile probe fails at nt.h on both widths; it is not
a runtime reproduction. The current/candidate actual-body fixture passes on
both widths. Independent withdrawals of the PlaySound undef and the lock adapter
fail compilation/linking on both widths for the distinct reasons above.
The restored cleanup probe creates real disposable event handles: InitSound(FALSE)
closes and clears one; PlayContinuousTone retains another at countdown 1 and
closes/clears it at 0. Both architectures pass GetHandleInformation checks after
closure. These controlled states prove cleanup statements, not native Beep-device
activation. No persistent regression is needed for an inactive original restore;
the existing production-state fixture is unchanged.

Microsoft's Beep contract states that it synchronously waits for sound completion:
https://learn.microsoft.com/windows/win32/api/utilapiset/nf-utilapiset-beep
A real host Session-3 probe calls only Beep(440,250): x86 succeeds after 262.452 ms,
x64 after 258.813 ms. GetLastError was printed as 126 after success and is not a
failure result; it has no interpretation on successful Beep. The actual original
PPI function fixture independently proves a 1000 Hz/INFINITE output request.
Together with the documented API contract this establishes the synchronous
handoff mismatch by source reasoning. No INFINITE native Beep was executed;
no 49-day block or audible guest reproduction is claimed. A future redesigned
native backend is not ruled out, but is not an exact restoration or this audit.

Restoration is production +23/-11 (net +12), permanent tests unchanged;
XP-relative added/deleted diff falls by 30 lines. Existing exclusions and line
controls are untouched and receive no new accounting. No new host state owner,
SDK shim, output path or public component change is introduced. The original
equal-tick PPI expiration remains unchanged, including its existing fixture.
Final-source dual builds/publication and all background/native suites are running;
delivered frontier remains 466/472 until P158 verification and push complete.

## File 467 host/src/nt_sound.c P158

Final-source cached builds, publication and all 392 tests pass, with no native
deferral:

| Architecture | Build | Background | Native desktop/audio/package |
| --- | --- | --- | --- |
| x86 | 8.79 s | 188/188, 148.53 s | 8/8, 16.44 s |
| x64 | 10.68 s | 188/188, 143.63 s | 8/8, 15.69 s |

Actual-diff counter-review confirms +23/-11 production (net +12), no permanent
test edits and 30 fewer XP-relative diff lines. Restored cleanup has no native
opener or countdown initializer in production, so it cannot activate a second
audio path. Existing PPI/Timer2 state and Compat/Lib output ownership remain.
R0679 is restored; R0677/R0678/R0680/R0681 are evidence-qualified owner choices,
not approved retention. Native Beep timing proves only the tested finite call;
the INFINITE mismatch is source/API reasoning, not an infinite-duration test.
No guest, configuration, media, snapshot or public eight-tree change is included.

Published SHA-256:

- softpc32.exe: `824fa85389d434f09e91be3c49c765de1a7ea11cd107c88135fa1955685dd590`.
- softpc64.exe: `ee9245d6c0c479c702cb7353fbe3ae5f0c7ac16ccf4cd211fff4a63e75bd60e3`.

P158 advances the delivered frontier to 467/472; next host/src/nt_vga.c.
Five files and qualified owner decisions keep S1/T100 open.

Final mirror, documentation and whitespace gates pass. Both cached build logs
have no warnings/errors; this is not a clean rebuild of every old mirror object.
All task processes were confirmed exited. Concise evidence was promoted before
removing the task's disposable probes/logs; build residue verification passes,
with only the reusable Ninja/ccache trees retained. User assets are unchanged.

## File 468 host/src/nt_vga.c review (qualification pending)

The fixed XP normalized file SHA-256 is
75fe56ba5e12147d8ba08cb6bfaf02e8aae1d819f3a16ad335d3354ab1521e68.
All non-excluded differences are four standard-painter validation/clipping
blocks, four added explanations and an extra EOF blank. Existing exclusions
and necessary line controls are not re-audited or counted.

Actual binding is softpc-machine-devices -> nt_graph.c std_colour_paint_funcs
-> its original selected paint_screen -> gfx_updt.c refresh/split/wrap/dirty
callbacks. Compat provides the DIB and invalidation endpoint, not duplicate
VGA pixel conversion. EGA_PLANE_SIZE is 0x20000 in this V7 build: four-plane
capacity is 512 KiB, not 256 KiB. Packed offsets are bytes; medium/high offsets
are four-byte plane groups. The medium output doubles source rows. Original
vga_split_graph_update can submit a zero-height first region when the screen
split is zero; it does not filter that call before the painter's do/while loop.
This is source-path evidence, not a claim that a guest was observed programming
every boundary value used in the fixture.

| Point | Original / local anchor | Executor review and independent counter-review |
| --- | --- | --- |
| R0682 | Four T60 explanation lines and trailing selected blank | Restore original absence, production +0/-5. No executable statement or exclusion framework changes. |
| R0683 | XP nt_vga_graph_std around 84-102 | Qualified owner choice: original guard/clipping does not reject empty/negative regions or constrain backing-source span. Exact whole-function restoration crashes on the zero-height fixture; adding only a nonpositive-size check then fails destination-byte assertions for negative x. Current passes. Overflow-sized clipping remains a boundary contract, not a claimed guest reproduction. |
| R0684 | XP nt_vga_med_graph_std around 439-461 | Qualified owner choice: current validation/clipping uses source groups/rows before vertical doubling. Whole original function crashes on zero height; nonempty-only variant fails the negative-x destination oracle. The existing original conversion loop remains unchanged. |
| R0685 | XP nt_vga_hi_graph_std around 840-856 | Qualified owner choice: original function crashes on explicit empty input. Nonempty-only restoration still crashes on the fixture's final plane-group offset with height 768; the current source-span clip prevents the next out-of-range row. This latter case is a controlled boundary probe, not a traced Win3.1 failure. |
| R0686 | XP nt_v7vga_hi_graph_std around 1350-1358 | Qualified owner choice: original fixed height 400/width 640 ceiling rejects the actual BIOS/BOP-selected 640x480 packed mode. Both original and nonempty-only restoration fail dirty publication after host_flush_screen in that mode. Actual-size clipping supports the existing 640x480/720x540/800x600 paths without a second painter or enlarged virtual card. |

Private variants use the real cached Ninja compile flags and link the existing
VGA fixture with one overridden full nt_vga.c object before the machine archive.
Each standard function is independently replaced by its exact XP body; excluded
bodies are not restored. Four additional diagnostic variants add only an empty
size return ahead of each original body. They are experiments, not proposed
production paths. Baseline passes on x86/x64; all sixteen restored diagnostic
runs compile/link but fail:

- Standard/medium/high original bodies: access violation 0xC0000005 on both.
- Standard/medium nonempty-only: destination byte assertion at fixture line 706
  (x86 0xC0000409, x64 exit 3).
- High nonempty-only: access violation 0xC0000005 on both.
- V7 original and nonempty-only: dirty publication assertion at line 2330 after
  the real mode-67h BIOS/BOP sequence (x86 0xC0000409, x64 exit 3).

The tests run serially in separate task-owned working directories; no fixed
fixture image is shared with another experiment or the background qualification.
Additional fixture-local diagnostics identify zero-height, negative-x and final
source-group requests at the failures, instead of inferring causes from process
exit codes alone. They do not change production or add a testing API.
The fixture exercises arbitrary negative/overflow arguments as well as real
mode/register paths; those evidence scopes are explicitly distinct. No manual
guest reproduction, universal hardware fidelity or safety of all other painters
is claimed. Existing unsupported-branch content receives no new ledger.

The installed reduction is production +0/-5, no permanent test changes, and
five fewer XP-relative diff lines. Final-source cached dual builds/publication
and all 392 tests are running. The delivered frontier remains 467/472 pending
P159 qualification and push; retained boundary blocks require owner choice.

## File 468 host/src/nt_vga.c P159

Final-source cached builds/publication and all 392 tests pass:

| Architecture | Build | Background | Native desktop/audio/package |
| --- | --- | --- | --- |
| x86 | 10.75 s | 188/188, 149.48 s | 8/8, 17.35 s |
| x64 | 13.35 s | 188/188, 142.47 s | 8/8, 15.23 s |

Actual-diff review verifies five selected nonsemantic lines removed and no
executable change: production +0/-5, tests unchanged, XP diff reduced by five
lines. R0682 is restored; R0683-R0686 remain qualified owner decisions. All
eighteen primary actual-body runs and twelve diagnostic reruns completed without
timeout; their controlled failures are distinguished from final green tests.
No public component, guest, configuration, media or snapshot change is included.

Published SHA-256:

- softpc32.exe: `1496556dc83c0acc7352feddac0dbe6f8b04ce11a8b8ac07faef03c82fa2eaaf`.
- softpc64.exe: `025e424f2ea39700a8d33fe23574f5562b0d0db6ce6b63370839f57d6ea35b1b`.

The source portion of P159 reaches 468/472. The final four fixed resource
reviews follow below; qualified owner decisions still prevent S1/T100 closure.

## Files 469-472 fixed resources P159

Each resource is read in frozen order and compared byte-for-byte with its same
path at fixed XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89. Byte hashes also match
the original manifest and the frozen initial inventory. No resource is modified,
rebuilt, replaced or newly imported.

| Order | Resource | Bytes | Byte SHA-256 | Result |
| --- | --- | --- | --- | --- |
| 469 | roms/bios1.rom | 28420 | 1ce33544249fef59865b91308faf60f354563cf854f80eb196d47c968899a574 | Exact XP identity; no diff. |
| 470 | roms/bios4.rom | 8191 | b09fdff1a500971d54233378ce9c36f1b6a58bf577e4a544da03a0d78d7e0d6f | Exact XP identity; no diff. |
| 471 | roms/cmos.ram | 64 | 97fca7adb543bdac4e571185a3f37c17a30a6ad937e0817048d965a850edddd9 | Exact XP identity; no diff. |
| 472 | roms/v7vga.rom | 18832 | 970f105cd9e42ee56f07aae695bac89786d3455ab9d4c1ea9a1d1643b1e8f6f0 | Exact XP identity; no diff. |

Production references are checked separately from identity. firmware.rc embeds
bios1.rom/bios4.rom/v7vga.rom as RCDATA 101/102/103; the product and machine
fixtures compile that resource file. Original rom.c selects the three filenames
and calls Compat host_read_resource, which maps them to these embedded IDs.
They are actual fixed firmware consumers, not runtime dependencies on XP checkout.

cmos.ram is different: it is an unchanged 64-byte original resource, but it is
not embedded by firmware.rc, accepted by host_read_resource or opened by any
current production path. The filename remains an original host_def.h macro;
cmos.c initializes the RTC/CMOS and POST derives equipment/memory from current
machine state. No claim of a runtime cmos.ram consumer is made. Removing an
identical original file would introduce a new whole-file difference, prohibited
by the owner's later no-new-diff clarification. It therefore remains unchanged;
no compatibility loader or new use is invented to justify its presence.

The ordered file-level audit now covers 472/472 frozen files. This means each
file has a recorded disposition, not that all semantic differences are approved
or that S1/T100 is closed. Next work is reconciliation of restored deliveries,
qualified pending owner decisions and the unchanged-original-defect decisions.
The resource-only check changes no executable input; P159's final-source dual
build/publication and 392/392 regression evidence remains applicable. No additional
build is needed for these documentation-only dispositions.

P159 final mirror/documentation/whitespace gates pass. Both cached build logs
contain no warning/error lines. All owned probe and qualification processes
were confirmed exited, their bounded evidence promoted, and disposable task
scratch removed. Build residue checks pass; reusable caches remain. No skipped
native tests or unrelated worktree changes are hidden by this delivery.

## P160 reconciliation: inventory and delivery integrity

This is a bounded accounting check at source delivery
0722fb2ca998cdaa6bbcae7cd47aa518cb9dd95f, not approval of residuals or S1 closure.

- All 472 frozen order/path/initial-hash triples equal the first committed
  inventory at P1 7494ca3b9b8496a2f3abb943657eab754ffc42e5. Orders are exactly
  1 through 472 and paths are unique. The current git-tracked mirror path set
  equals that inventory: no unrecorded addition, deletion or order change.
- Git history after the admitted starting baseline contains every P1-P159
  delivery identifier. Presence establishes delivery traceability, not the
  correctness of every historical claim.
- Searching the progress report AND its m9-t100 supporting records finds all
  R0001-R0686 identifiers. R0390-R0392 were absent from the main narrative but
  present in sevid026's original point map; no new IDs or exclusion records
  are created. Identifiers count audit groups, not current surviving hunks.
- P58 6417136c480d1076770412eeabbfa4afd23f7aa0 covers those three groups:
  R0390 GDP mapping remains qualified pending owner decision; R0391's 25
  address expressions were restored; R0392 restores malloc in 22 allocations
  while retaining native capacity and ten mixed-width zero initializations
  pending owner decision. The initial map has 483/25/32 ranges respectively.
  Its 540 stale "pending actual-body proof" cells now point to the existing
  final P58 evidence; original ranges and point assignments are unchanged.
  P58's combined negative control is not individual dynamic proof of every
  retained allocation. The separate allocation inventory preserves that limit.
- Current published executable byte hashes equal the P159 final qualification
  hashes recorded above. No executable source or test changed in this
  reconciliation; the final-source 392/392 evidence remains applicable.

Remaining reconciliation must resolve each point's latest disposition and
reviewable evidence, including mixed restored/pending groups, D001-D006 and
L001. Historical provisional wording does not override later qualification;
qualification does not mean owner approval. This check neither proves every
residual necessary nor closes unresolved design or original-defect decisions.

## P161 final-decision reconciliation, positions 1-72

The [owner review view](m9-t100-owner-decisions.md) reconciles the first 72
frozen positions with current bindings and P1-P6 final evidence. Thirty-four
point IDs remain qualified recommendations, not approvals: R0006/R0008/R0010,
R0014/R0015, sixteen SAS points and thirteen cpu4gen.h declaration points.
Withdrawn bswap.h retirement R0013 is explicitly not a pending change.
Artifact-equivalent P5/P6 deliveries are not misreported as fresh full tests.
Positions 73-472 and D001-D006/L001 still need final-decision reconciliation;
the earlier file-level audit remains complete, but S1/T100 is open.

Current-source review finds the R0008 EOI line still carries an unnecessary
T99 explanatory comment. Remove only that inline text, keeping the corrected
ICA1_PORT_0 token and all other bytes/line endings. The root source target
compiles this file and the existing device_contract fixture exercises idle,
slave IRQ11, master IRQ5 and spurious cascade with ISR/mask/BDA assertions.
No new fixture or guest modification is needed for comment removal.

Production +1/-1, tests +0/-0; normalized source text is 42 bytes shorter.
The same fixed-XP SequenceMatcher comparison has unchanged added/deleted
line totals: removing text on an already changed line does not reduce the
line-based mirror diff. No new point, exclusion record, path or behavior is
introduced. The local manifest hash changes; pinned original/subset hashes do
not. Builds/publication and full regression are running; final results follow.

### P161 final qualification

| Architecture | Build | Background | Native |
| --- | --- | --- | --- |
| x86 | 8.26 s | 188/188, 156.93 s | 8/8, 17.39 s |
| x64 | 10.18 s | 188/188, 154.42 s | 8/8, 15.91 s |

All 392 final-source tests pass without retries or skips. Cached build logs
contain no warning/error diagnostics; this is not a fresh warning audit of
unchanged original source. Publication refreshed both timestamp-bearing EXEs.

- softpc32.exe: `2e4ae95bfab0722015b81c30cd321c799acecd85acfc5a7932440ec906b90333`.
- softpc64.exe: `90612e4da6ac3d8376b7595216d124411b1304dc238e5848d7eedb0e33378660`.

Actual-change review confirms only the inline comment changed in production,
with the same CRLF and source line count; manifest changes only its local hash.
The new review view is bounded to positions 1-72 and preserves all thirty-four
qualified decisions as unapproved. No test, public component, configuration,
media, snapshot or guest binary changed. A supplementary documentation-gate
invocation reported its script temporarily unavailable; a subsequent same-path
read and gate pass succeeded. No cause is inferred for that transient lookup.
Final mirror/documentation/whitespace checks are rerun after the jobs finish.

P161 final gates pass. The owned runner is confirmed exited; temporary logs
and script were promoted above and removed through Clean-Build. Residue check
passes, retaining only reusable caches. S1/T100 remains open.

## P162 final-decision reconciliation, positions 73-118

The owner review view now reconciles positions 1-118. This documentation-only
delivery adds 70 pending IDs to the previously reconciled 34 (104 total):
five in c_bsic/c_debug/c_intr, 46 in c_main, fifteen in c_page and four in
c_tlb. c_main grouping is checked against its complete R0059-R0112 range
minus the eight restored IDs, with no missing or extra decision IDs. Repeated
include IDs in the explanation are not counted twice. This is point-group
accounting, not current hunk counts or owner approval.

Current-source review confirms the separate original NPX caller of do_intrupt,
private debug caches independent of current DR7, executor quick/normal/fetch
entry differences, five debug retirement call sites, one host observer owner,
and private TLB storage. Git confirms c_main and c_page remain at their P8/P10
source revisions. The source/test scan finds no remaining removed register
snapshot API: R0130-R0134 are fully restored, not pending. R0139/R0140 are also
restored. P7 artifact equivalence and P10/P12 actual negative comparisons are
reported with their original limitations; no new runtime probe is claimed.

No production, test, manifest, configuration or EXE changes occur in P162.
The latest P161 dual build/publication and 392/392 final-source tests therefore
remain applicable. Both EXEs are byte-identical to HEAD P161; no unnecessary
build or timestamp-only replacement is performed for document changes.
Documentation/mirror/whitespace gates pass. No scratch is created. The next
position is 119; remaining positions and D001-D006/L001 are not closed.

## P163 final-decision reconciliation, positions 119-266

The owner review view now covers positions 1-266. Ten further pending IDs are
reconciled, for 114 cumulative: R0146/R0147, R0155-R0158, R0160-R0162 and
R0172. P14/P15/P16/P19/P20/P21 restorations are explicitly distinguished from
those remaining choices. In particular IRET/RETF stack-width replacements and
FPU snapshot wrappers are restored, not retained adaptations.

Current-source review checks original FPU stack accessor calls from the archive,
CVTFPHI64 representation copying and M64I rounding, Gdp/no-op symbol owners,
outer unwind relaying to ccpu386Unsimulate, both null-return branches, operand
size at the original IRET/RETF return blocks and private delayed exception IP.
P15/P18 remain the latest corresponding source revisions before this cleanup.
P21's isolated verifier retry and ntthread's unchecked setjmp consumer are
retained as explicit limits, not hidden behind a general safety claim.

### P163 final evidence

Remove only the remaining explanation above ccpu386UnsimulateOuter; keep all
function logic and the original single unwind implementation. Production
+0/-1, tests +0/-0. Fixed-XP comparison confirms added diff lines decrease by
one, deleted lines unchanged. The residual is +23/-2; no new point or excluded
body record. The manifest updates only ntthread.c's local hash. Mirror CRLF
and all other production bytes are preserved.

Both cached full-target builds succeed and publish fresh EXEs. Complete-byte
comparison against P161's published artifacts finds equal lengths and only
metadata changes, listed below. PE offsets are validated from each DOS header;
permitted fields are COFF TimeDateStamp and OptionalHeader CheckSum. Each
changed compiler literal is a complete NUL-terminated time at the same offset.
Every other byte is identical. This is the approved artifact-equivalence
exception: no fresh runtime tests are claimed, and P161's final-source 392/392
is reused. No changed test requires a focused rerun.

- x86: build 9.3 s; 3478046 bytes; changed offsets [136, 137, 216, 217, 2394691, 2394692, 2394694, 2394695]; compiler time 03:08:38 to 03:21:12 at 2394688; SHA-256 `db128de49f8724bd2e1caef4c6f3917e6683abea39c55df7759b66bd165a3ba2`.
- x64: build 10.66 s; 3095494 bytes; changed offsets [136, 137, 216, 217, 2262243, 2262246, 2262247]; compiler time 03:11:41 to 03:21:22 at 2262240; SHA-256 `1f766670b79795c346546fe95ba5cdbc05ba108a6e3695fd83be419363a642e6`.

Actual-diff review confirms one comment-only production deletion, no guest,
media, configuration, snapshot, Compat or shared-component changes. Cached
build logs contain no warning/error lines; this is not a fresh warning audit
of unchanged mirror units. The build runner is confirmed exited. Final gates
and scratch cleanup follow; owner approval and S1/T100 closure remain pending.

P163 final mirror/documentation/whitespace gates pass. Task scratch was previewed
and removed after runner exit; build residue check passes. No live job or
unrelated change is removed.

## P164 final-decision reconciliation, positions 267-282

The owner decision view now covers positions 1-282, adding ten pending semantic
IDs for UART/LPT private archive state and pointer-width host arguments, plus
GDP allocation and aggregate selection. The cumulative semantic count is 124;
four historical notice-policy items are separately identified, not added to
that count. Restored points and evidence limitations remain explicit. Next is
position 283; D001-D006/L001 reconciliation and owner approval remain pending.

### P164 final evidence

Restore only ev_glue.c's include-line comment to exact XP wording. Preserve
angle-bracket selection and all executable tokens. Production +1/-1, tests
+0/-0; normalized source is 27 bytes shorter. XP added/deleted diff-line total
is unchanged because the necessary header selection still differs on that line.
Only this file's local manifest hash changes; mirror CRLF is preserved.

Cached full-target builds and dual publication pass: x86 9.23 s, x64 11.52 s.
Complete comparison against P163 finds equal sizes and no changes outside PE
COFF timestamps/checksums and same-offset, validated compiler time literals:

- x86: 3478046 bytes; six changed bytes; time 03:21:12 to 03:25:31 at 2394688;
  SHA-256 `1c4f19f8f9840a86fdb796ebec035fea8c45704d0f25edde7a3ba7d92f65fe1c`.
- x64: 3095494 bytes; seven changed bytes; time 03:21:22 to 03:25:40 at 2262240;
  SHA-256 `f4fb0f42bf7b7e91db40b2ca44fdda2341fd164ee4c820736b29c3acfb493a05`.

Each incremental build reports 11 original ev_glue.c printf declaration warnings
and no errors. These are preserved mirror declarations, not new shared-code
warnings or an assertion that a clean rebuild is warning-free. Actual diff
review confirms no behavior, tests, Compat, shared component, guest, media,
snapshot or user configuration changes.

The owner reports returning to the desktop and permits complete verification.
Session 3 is Active; a hidden, owned runner executes both background suites and
serial native desktop/audio/package suites against these published artifacts.
Each stage has a 900-second budget within the packet's two-hour limit. Fresh
results below replace reliance on the metadata-equivalence test exception.

Fresh full regression: x86 background 188/188 (150.62 s), native 8/8
(15.60 s); x64 background 188/188 (147.11 s), native 8/8 (15.69 s).
Total 392/392, no retries, failures or skips. Both runner PIDs have exited.
Native results include real audio, Console/Window and package tests; these
are automated regression results, not new manual guest acceptance.

P164 final mirror/documentation/whitespace gates pass. Task-owned scratch was
previewed and removed after runner exit; build residue check passes. Reusable
Ninja/ccache roots and user assets are preserved. S1/T100 remain open.

## P165 final-decision reconciliation, positions 283-288

Reconcile sevid000-sevid005 against P30-P35, their per-range/allocation records,
current production bindings, GDP field owner, actual DWORD/continuation bodies
and native allocation sites. All six normalized source hashes still equal the
final candidates named in those delivery records. Current calloc/malloc counts
are respectively 31/0, 31/0, 3/29, 5/27, 6/26, 1/31. No production edits are
needed by this reconciliation; existing limitations are not silently approved.

The decision view adds 17 qualified semantic IDs, cumulative 141 through file
288. Original address wrappers and unnecessary zeroing/tail-continuation changes
are identified as restored, not pending. The view explicitly distinguishes the
six sevid000 continuations from the four restored sevid001 continuations,
source-only mixed-width proof from bounded dynamic controls, and original
GenericByteFill failure from guest reachability. D001-D006/L001 remain pending
final reconciliation, not TODO or new fixes.

Executor and coordinator review agree that this P changes only three task
records. No source, test, EXE, configuration, guest, snapshot or shared component
is modified. P164's fresh 392/392 applies to the identical executable sources;
no fresh compilation, publication or runtime test is claimed for documentation.
Next reconciliation position is 289; S1/T100 are not closed.

P165 documentation/mirror/whitespace checks and build-residue check pass.
No disposable scratch or live job is created by this document-only delivery.

## P166 final-decision reconciliation, positions 289-300

Reconcile sevid006-sevid017 with P36-P48 and the superseding P56 corrections.
Current normalized hashes match their latest tested candidates. Direct source
checks confirm no softpc_gdp_rule_address calls in these twelve files, and
calloc/malloc counts 11/21, 5/27, 7/25, 0/32, 7/25, 5/27, 5/27, 12/20,
11/21, 8/24, 9/23, 3/29. The mixed-width allocation entries match their named
inventories. Actual CCPU phy_w32 uses two WORD stores at +0/+2 and DWORD fill
loops those scalar stores; generated DWORD faults are not mislabeled current
guest triggers. Current continuation guards retain only the demonstrated
same-file nested cases, allowing restored private declarations.

The decision view adds 30 semantic IDs, cumulative 171 through file 300.
The two wrapper retentions in P39/P40 are explicitly superseded by P56, not
carried forward as necessity claims. Anchored versus heuristic diff counts,
source-only zeroing proof, bounded preservation controls, independent pixel
oracles, old gate failures and timeout retries remain distinguishable.
No new source, test, product, mirror or asset change is needed in this batch.

Executor/counter-review inspect these current facts and historical final
controls rather than treating a prior initial proposal as delivered evidence.
This delivery changes only task records and uses P164's fresh 392/392 for
unchanged production. No new compilation/publication or runtime test is claimed.
Next is position 301; D001-D006/L001 and owner decisions remain unclosed.

P166 documentation/mirror/whitespace gates and build-residue check pass. No
new task scratch or worker exists. Only the three reviewed task records change.

## P167 final-decision reconciliation, positions 301-311

Current hashes for sevid018-sevid028 match their latest final candidates in
P49-P60. Actual source confirms the refined P50 five original tail calls,
P51 direct-only tail restoration, GDP read-byte binding and the per-file
calloc/malloc counts recorded in the owner decision view. All eleven files
have no softpc_gdp_rule_address calls. No source changes result from this check.

Reconcile 32 pending semantic IDs, cumulative 203 through file 311. The record
uses P50's final eleven-continuation disposition, not its superseded sixteen-
site proposal. Allocation/free counters distinguish observed leaks from crash
claims. P61 resolves P59/P60's legacy video-source callback boundary with the
selected compilation path and actual CCPU REP MOVS evidence; the old unexplained
limitation is not carried forward. Original D003/D004 remain decisions, not
new fixes or hidden failures erased by equality tests. R0390-R0392 map through
the P58 point ledger and its final qualification, without inventing new IDs.

Executor/counter-review classify this P as three task-record changes only.
P164's fresh 392/392 applies to identical production/test artifacts. No new
build, publication or runtime proof is claimed. Next reconciliation position
is 312; S1/T100 and owner decisions remain open.

P167 documentation/mirror/whitespace gates and build-residue check pass. No
new scratch or background worker was created. Actual diff is task records only.

## P168 final-decision reconciliation, positions 312-330

Reconcile final generated rules/accessors, disk controller state and first base
headers using P62-P74 final evidence. Current normalized hashes of the five
generated files match their final candidates. Source confirms allocation counts,
local vglfunc IFN1 adaptation, BIOS CX:DX assignments, HDD/FDC snapshot functions
and live no-medium gfi wiring. Git history confirms the reviewed disk/vglfunc
bodies predate their audit controls and ckmalloc's latest edit is P74 restoration.

The owner view adds fourteen semantic IDs (217 cumulative) and four separate
notice-policy items (eight cumulative), reaching file 330. Scalar getter native
return failure is not described as typed-IU32 consumer or guest malfunction.
Legacy selectors are not advertised as supported production interfaces. HDD/FDC
private-state access remains distinct from archive serialization, and actual
checkpoint coverage is separated from source-only callback/state proof.
Floppy no-medium behavior is not confused with missing hardware. R0420 is a
completed restoration, not a retention choice or new change in this batch.

Actual-change review: only three task records change. No source/test/EXE,
configuration, guest, media or snapshot changes. P164 fresh 392/392 applies to
identical production; no fresh build/publication/runtime run is claimed for
this document-only delivery. Next is file 331. S1/T100 remain open.

P168 documentation/mirror/whitespace gates and build-residue check pass. No
new worker or disposable scratch exists; final diff contains task records only.

## P169 final-decision reconciliation, positions 331-358

Reconcile P75-P84 with current source and later coupled P123/P125 restoration.
A current-hash check deliberately rejects P76 as cpu_vid's final identity;
Git history identifies the later coherent reader/writer restoration. Current
cpu_vid contains no added provider block; original local declarations exist
in ega_read/ega_writ. R0423 therefore is restored, not a qualified retention.
This corrects the incomplete isolated-removal evidence instead of propagating
its superseded conclusion into the owner's approval list.

The other six restored/retained header hashes match their recorded final
candidates. Current searches confirm local ega_read_dot declaration, unused EMS
storage declarations, the DMA pointer declaration and panning/provider types.
Only R0424/R0426/R0428 remain pending: three new IDs, cumulative 220 through
file 358. Eight historical notice items remain separate. No existing excluded
body is recorded. Coupled VGA-test include wiring and private test declarations
are disclosed, not new public paths. Original compilation warnings and extra
probe warning flags are distinguished from runtime/guest failure claims.

Actual-change review finds task records only. No production/test/EXE change or
new scratch/worker. Current production retains P164's fresh 392/392 evidence;
no new compilation, publication or runtime run is claimed. Next file is 359;
S1/T100 and owner decisions remain open.

P169 documentation/mirror/whitespace gates and build-residue check pass. Final
diff contains only the three reviewed task records; no unrelated changes.

## P170 final-decision reconciliation, positions 359-369

All eleven current normalized files match their P90 committed contents.
Current source confirms the CHAIN2 address macro consumers, original restored
host declarations, CCPU/PIC macro collision and actual I/O dispatch masking.
P85-P90 evidence is reconciled with those final sources, not rerun or promoted
to broader guest proof. Rejected malformed P89 probe results remain rejected.
R0434/R0438/R0440 add three pending semantic IDs; R0439 is separately identified
as a compiler/format residual. Cumulative residual IDs are 224: 223 semantic
and one compiler/format, with eight historical notice items still separate.
R0435-R0437 remain completed restorations. Original include wiring stays intact.

Executor and counter-review find only three task-document changes. No source,
test, EXE, configuration or media changes; no scratch or worker was created.
P164 fresh 392/392 remains applicable to identical artifacts, not a newly run
suite. Next position is 370; D001-D006/L001 reconciliation and owner decisions
remain open. S1/T100 are not closed.

P170 mirror/documentation/whitespace gates and build-residue check pass. Actual
diff review confirms only the three task records and no unrelated modifications.

## P171 final-decision reconciliation, positions 370-393

Normalized current contents of all twenty-four headers match the committed P98
state. Actual provider/caller searches confirm queue capture/restore wiring,
PHY_ADDR storage, Sound Blaster teardown consumers and unsupported UART parser,
and Compat's restored time_t provider. The initial time_day path search used
the wrong directory; file inventory located base/support/time_day.c rather
than treating a missing search path as evidence of no consumer.

Reconcile P91-P98 final evidence, including coherent BOOL provider/header/test
restoration, original time_t reinstatement and exact whitespace restoration.
Retained R0442/R0444/R0448/R0449 add four semantic IDs; R0451 adds a compiler
EOF residual. Totals: 229 residual IDs (227 semantic, two compiler/format),
plus eight historical notice items. Compile proofs, current-behavior tests,
original-negative controls and actual full suites remain distinguished.

Executor/counter-review find three task records only. No production, tests,
EXEs or user assets changed; no scratch/job was created. P164 fresh 392/392
applies to unchanged artifacts; no new runtime result is claimed. Next file
394 and D001-D006/L001 remain to reconcile. Owner decisions and S1/T100 remain
open, not closed by the accumulated file coverage.

P171 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Final changes are limited to the three task records.

## P172 final-decision reconciliation, positions 394-401

All eight current normalized files equal their P103 committed contents.
Current actual-source searches confirm teletype stream references and disabled
storage, keyboard command/PIC operations, device archive calls, Machine mouse
ingress and BIOS reset order, mouse timer/clearing, and original PPI sound
notification after the timer gate. No later superseding edit is hidden.

Reconcile P99-P103 evidence with precise limits: keyboard C0/IRQ and PPI gate
have actual-source original-negative controls; mouse defaults/reset and finite
burst replacement rely on identified source contracts and current tests rather
than a claimed fresh original guest reproduction. Snapshot ownership evidence
is not reduced to mere references. Notice policy is separate from behavior.
Eleven semantic IDs and four notices are added: 240 residual IDs (238 semantic,
two compiler/format) and twelve historical notice items through position 401.
R0454/R0459/R0467 remain delivered restorations, not approval candidates.

Executor/counter-review changes only three task records. No code, tests, EXEs,
configuration or media changed; no job/scratch created. P164's fresh 392/392
remains applicable without claiming a new run. Next is position 402; original
D001-D006/L001 and owner decisions remain open. S1/T100 are not closed.

P172 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task documents are changed.

## P173 final-decision reconciliation, positions 402-408

All seven current normalized files match their P111 committed versions. Actual
source checks confirm DMA query/channel/global ownership, CMOS topology and
original memory comparison, PIC masks/withdrawal/private ICW phase and restored
idle setter casts. P104-P111 final deliveries supersede preliminary incomplete
qualification text. CMOS's internal-only negative is expressly not treated as
a product necessity; its original expression was restored and fully tested.
PIC L001 remains a proven incomplete archive contract, not erased by full tests.

Twelve semantic residual IDs and three notice-policy items are reconciled:
252 residuals (250 semantic, two compiler/format), fifteen notices through 408.
L001 is a proposed new repair, not included as an existing residual patch.
R0472/R0474/R0475/R0481/R0484-R0486/R0494 remain completed restorations.

Executor/counter-review changes only three task documents. No source/tests,
EXEs or user assets changed; no worker/scratch created. P164 fresh 392/392
remains the current runtime evidence, not a new run in this P. Next is 409;
remaining files and final D001-D006/L001 decisions keep S1/T100 open.

P173 actual three-document diff review, mirror/documentation/whitespace gates
and build-residue check pass. No unrelated changes are present.

## P174 final-decision reconciliation, positions 409-415

All seven normalized current files match their P119 committed versions. Actual
source checks confirm temporary event-queue construction precedes live replacement,
ROM segment width, original restored length operations, PIT first-event guard
and still-unprotected active-backlog divide, and registered original interrupt
handlers. P112-P119 final evidence is reconciled, not rerun.

Six semantic IDs, one compiler/format ID and three notices are added: 259
residual IDs (256 semantic, three compiler/format), eighteen notices through 415.
D005 remains a reproduced current device/queue failure and isolated candidate,
not an implemented repair. The current suite does not cover that dispatch case.
R0497/R0498/R0500/R0506/R0509 and the extra R0508 blank remain restored.

Executor/counter-review limits actual changes to three task documents. No code,
tests, EXEs, configuration or guest assets change; no scratch/job exists.
P164 fresh 392/392 applies to identical artifacts, without a new runtime claim.
Next is file 416; final D001-D006/L001 and owner decisions keep S1/T100 open.

P174 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three task documents change.

## P175 final-decision reconciliation, positions 416-423

All eight current normalized files match their committed P126 versions. Current
source confirms actual BIOS writer selection distinct from CPU SetWritePointers,
EGA reset getter/archive use, virtual recalc_text branch, original local reader/
writer declarations and absent added provider declarations in cpu_vid.h.
The no-match header search is expected restoration evidence, not a tool failure.

Reconcile P120-P126 final evidence, including individual and whole-group writer
negative controls, controlled virtual-I/O matrix versus historical guest probe,
and fully superseded R0423 declaration retention. Sixteen semantic IDs are added:
275 residuals (272 semantic, three compiler/format), eighteen notice items.
D006 remains a separately reproduced original plane-enable defect, not fixed by
retained address corrections or hidden by suite success.

Executor/counter-review changes three task records only. No source/tests/EXEs,
configuration or media change, and no scratch/job created. P164 fresh 392/392
remains applicable without a new runtime claim. Next is file 424; final original
defect decisions and owner retention choices keep S1/T100 open.

P175 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. The three task records are the complete changed-file set.

## P176 final-decision reconciliation, positions 424-427

All four normalized current files match the P131 committed versions. Actual
source confirms the final two backing-span checks and dirty capacity, pointer
colour condition and one-argument V7 validation mask. Reconcile P127-P131 final
results without treating intermediate unsafe guards or disconnected tests as
passed. Default CTest Console launch and exact-binary explicit-Console execution
remain distinct; P164 is the later fresh 392/392 current-source evidence.

Eight semantic IDs are added: 283 residuals (280 semantic, three compiler/format),
eighteen notices. Grouped dirty/mapping proof is not claimed as independent
negative execution of every repeated line. R0529/R0530/R0537 actual minimization
is preserved; no new renderer or extra limit is introduced by this review.

Executor/counter-review changes only three task records. No source, test,
EXE or user asset changes; no worker/scratch created. Next is file 428 and
final D001-D006/L001 reconciliation. All owner decisions and S1/T100 remain open.

P176 actual three-document diff review, mirror/documentation/whitespace gates
and build-residue check pass. No unrelated modifications are present.

## P177 final-decision reconciliation, positions 428-431

All four normalized current files match final P134 committed contents. Actual
source confirms mode conditions, four necessary raw V7 reads and reuse of
original port handling, cursor guards and original BIOS integration. Reconcile
P132-P134 final proofs, not intermediate candidates or invalid overlapping
scratch-fixture results. Controlled matrix, real register path and historical
guest probe are distinct evidence; native deferral is not retroactive success.

Eleven semantic IDs are added: 294 residuals (291 semantic, three compiler/format),
eighteen notices. R0538/R0542/R0549 reductions remain delivered; no new state,
renderer, callback, source/test or public API is added in this reconciliation.
Current source artifacts retain P164 fresh 392/392 evidence without a new run.

Executor/counter-review changes only three task documents. No source, tests,
EXEs, user configuration/media or snapshot change; no job/scratch created.
Next is 432 and final D001-D006/L001 reconciliation. S1/T100 remain open.

P177 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task documents change.

## P178 final-decision reconciliation, positions 432-437

All six normalized current files match final P139 committed contents. Current
headers confirm retained actual FPU/byte representations and restored original
vector declarations, with redundant host declarations/profile defines absent.
P135-P139 final probes and deliveries distinguish unused declaration typing
from required representation and selected-subset contracts. No hypothetical
future toolchain support or exhaustive FPU behavior is inferred.

Four residual IDs are added: 298 total (295 semantic/ABI/configuration, three
compiler/format), eighteen notices. R0552/R0553/R0555/R0557 remain restorations;
source-line increases in original blocks are not mistaken for added mirror diff.

Executor/counter-review changes only three task documents. No code/tests/EXEs,
configuration, media or snapshots change; no scratch/job created. P164 fresh
392/392 remains the current runtime evidence, not a new run. Next file 438 and
final D001-D006/L001 reconciliation remain; S1/T100 and owner decisions stay open.

P178 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Final changes are restricted to the three task records.

## P179 final-decision reconciliation, positions 438-452

All fifteen normalized current files match P143 committed contents. Current
whole-tree searches confirm restored unused declarations have no implementation
or caller; source/CMake confirms native IHPE and scoped C-VID IUH with ordinary
CCPU unchanged. P140-P143 final evidence distinguishes declarations, actual
compiler failures and real heap truncation, including coherent both-type control.

Two ABI IDs and one historical notice are added: 300 residual IDs (297 semantic/
ABI, three compiler/format), nineteen notices. R0561/R0562/R0565-R0568 remain
completed restorations; original include wiring is retained without claiming
unused historical APIs are implemented. No new pointer representation or path.

Executor/counter-review changes only three task documents. No source/tests,
EXEs, user configuration/media or snapshots change; no job/scratch created.
P164 fresh 392/392 remains applicable without a new run. Next is file 453;
remaining files, D001-D006/L001 and owner choices keep S1/T100 open.

P179 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task records change.

## P180 final-decision reconciliation, positions 453-454

Both normalized current headers match P145. Actual declarations, nt_ega callers,
nt_munge provider and VGA pixel assertions were reread. P80's negative is an
isolated no-pan query experiment, not restoration of all coupled implementation
patches. P144's no-definition observation is superseded by P150/R0604: current
host_stream_io_enabled has its original declaration and definition, no reader.
This correction prevents stale investigative observations becoming final facts.

R0571/R0573 add two semantic decisions: cumulative 302 residual IDs (299 semantic/
ABI, three compiler/format), nineteen notices. R0569/R0570/R0572/R0574 remain
restorations; nt_mouse.h remains original. No excluded bodies are counted.

Executor/counter-review modifies only the three task documents; no production,
tests, artifacts, user configuration/media or snapshots change. No job/scratch
is created. P164 fresh 392/392 remains applicable, not a new test run. Next is
455; remaining files and D001-D006/L001 still require final reconciliation.
S1/T100 and all qualified owner decisions remain open.

P180 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Final changes are restricted to the three task records.

## P181 final-decision reconciliation, positions 455-458

All four normalized current files match P149. Current sound declarations,
controller types, platform status/activation callers, CGA producer guards and
mapping, EGA bounds/panning and text/pixel tests were checked against final
P146-P149 evidence. Rejected intermediate declaration migrations are not final
changes; the later DSP header-owned type remains in place.

Seventeen residual IDs are added: 319 cumulative (315 semantic/ABI, four
compiler/format), nineteen notices. Original restorations and actual XP diff
reduction are distinguished from source growth. Scratch contamination, package
input contamination and corrected manifest metadata remain explicit historical
limits, not necessity proof or invented first-pass success.

Executor/counter-review changes only the three task records. No source/tests,
EXEs, configuration, media or snapshots change; no job/scratch created. P164
fresh 392/392 remains applicable, not a new run. Next is 459 and remaining
D001-D006/L001 reconciliation; S1/T100 and owner decisions remain open.

P181 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task records change.

## P182 final-decision reconciliation, positions 459-461

All three normalized current files match P152. Current graphics update/DAC,
original fullscreen checks, keyboard caller/array guards and both plane-munging
branches were checked against P150-P152 final evidence. Unused stream state and
original reset/guards stay restored. Artificial FULLSCREEN fixture state is
not production necessity; original keyboard traps and complete coherent
no-panning restoration are distinguished from partial interface failures.

Fifteen residual IDs are added: 334 cumulative (329 semantic/ABI, five compiler/
format), nineteen notices. No excluded-body accounting. All owner choices stay
open. Tests establish their stated boundaries, not every keyboard/video mode
or a fresh guest session.

Executor/counter-review changes only three task records. No production/tests,
EXEs, configuration, media or snapshots change; no owned jobs or scratch.
P164 fresh 392/392 remains applicable, not a new run. Next position 462 and
D001-D006/L001 remain; S1/T100 are not closed.

P182 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Final changes are restricted to the three task records.

## P183 final-decision reconciliation, positions 462-463

Both normalized current sources match P154. Current device archive/status,
executor handoff, actual physical DMA/consumption and pause/EOI ownership were
checked against final P153/P154 evidence. The prototype-only PauseDMA experiment
is explicitly invalid; corrected real-function negatives and the permanent
active-DMA regression are retained. Redundant outer EOI removal guard remains
restored to original, not proposed for retention.

Twenty-six residual IDs are added: cumulative 360 (355 semantic/ABI, five
compiler/format), nineteen notices. Whole-original host ownership is a source
analysis, not a dynamically recreated native backend. Narrow withdrawals and
full suite results retain their actual scopes; no owner approval is implied.

Executor/counter-review changes only the three task records. No source/tests,
EXEs, configuration, media or snapshots change; no job/scratch created. P164
fresh 392/392 remains applicable without a new run. Next position 464 and
D001-D006/L001 remain; S1/T100 stay open.

P183 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task records change.

## P184 final-decision reconciliation, positions 464-466

All three normalized current files match P157. Current FM scope and fixture,
MIDI native activation/parser consumers and mixer arithmetic were checked against
P155-P157 final evidence. Original FM scope corrects both prior code and its
incorrect expected behavior. The mixer zero guard is proven redundant and stays
removed. Original MIDI tail/pointer failures do not qualify every replacement
field; corrected-ring, withdrawal and ownership evidence stay separate.

Thirteen residual IDs are added: 373 cumulative (368 semantic/ABI, five compiler/
format), nineteen notices. Inactive original volume defect remains a diff-decision
finding, not a new patch or TODO. No original helper activation is claimed.

Executor/counter-review changes only three task records. No production/tests,
EXEs, configuration, media or snapshots change; no job/scratch created. P164
fresh 392/392 remains applicable, not a new run. Next position 467 and remaining
D001-D006/L001 decisions keep S1/T100 open.

P184 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Final changes are restricted to the three task records.

## P185 final-decision reconciliation, positions 467-472

Current normalized tone/VGA files and four byte-exact resources match P159.
Resource hashes are freshly checked; actual embedded mappings and the absent
cmos.ram consumer are distinguished. Tone submission/native Beep reasoning,
painter complete-body probes and actual extended-mode evidence retain their
scope limits. No excluded body is counted or resource removed.

Eight residual IDs are added: cumulative 381 (375 semantic/ABI, six compiler/
format), nineteen notices. All 472 file dispositions are reconciled. D001-D006/
L001 and final accounting still require work; file coverage is not retention
approval, S1/T100 closure or a fresh complete runtime qualification.

Executor/counter-review changes only three task records. No production/tests,
EXEs, configuration, media or snapshots change; no job/scratch created. P164
fresh 392/392 remains applicable without a new run. Owner decisions stay open.

P185 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task records change.

## P186 proposed-defect decision reconciliation

Current generated GenericByteFill continuation, second-WORD +8, overlapping
WORD-fill passes, CCPU scalar fill route, active PIT int_delay, plane2 enable
condition and PIC private sequence/wire map were checked against the prior
D001-D006/L001 evidence. Earlier read commands used two incorrect path guesses;
actual ccpu386 and compat/devices paths were located and inspected instead.
No absent-path output is treated as absence of a production consumer.

All seven decisions now have scope/limits in the owner view. D001/D003 minimal
patches, D002 fill ordering, D004 hardware applicability and L001 exact schema
change remain unproved, not marked implemented. D005/D006 have concrete minimal
candidate directions but no new-diff authorization. Original inactive MIDI
findings remain visible with their existing decisions, not silently dropped.

Existing counts remain 381 residual IDs and nineteen notices; proposed repairs
are not added to that total. P164's passing suite does not cover away these
negative probes. Only three task documents change; no code/tests/artifacts,
configuration, media, snapshots or scratch change. Final accounting and owner
review remain; S1/T100 are open.

P186 actual diff review, mirror/documentation/whitespace gates and build-residue
check pass. Only the three reviewed task records change.

## P187 final identity and restored-ID accounting

Fresh checks verify all 472 P1 order/path/initial-hash triples and current tracked
mirror paths, plus unique uninterrupted P1-P186 delivery identifiers. Current
source/test/tools/build/assets match P164; both published EXE byte hashes match
its fresh 392/392 qualification. No rebuild or new runtime result is claimed.

A final-view identifier scan found 23 restored groups described collectively but
not explicitly indexed; their exact IDs/dispositions now appear in the owner
view. They are restorations, not new residuals, exclusions or new decisions.
The source delta from task start is separately recorded: mirror 123 files
+5922/-3721; owned src four files +137/-29; tests nine files +336/-48; build/tools
two files +8/-7. These are not XP diff-reduction figures. Pending file labels,
residual groups, notices and proposed repairs are explicitly separate counts.

Only three task records change. Fixed-XP aggregate accounting and the compact
owner decision package remain. No code/tests/artifacts or user assets change;
no job/scratch created. S1/T100 and owner decisions remain open.

P187 final checks confirm explicit R0001-R0686 coverage and ordered section
coverage 1-472 without gaps/overlaps. Actual diff review, mirror/documentation/
whitespace gates and build-residue check pass. Only three task records change.
