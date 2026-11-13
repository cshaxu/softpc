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
mode3 byte/word write/fill entries: two address parities, single writes and fill
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
list. D001-D003 identify proposed differences, not existing local patch ranges;
they do not inflate the measured XP diff or the restored-point counts. None
is repaired or approved for a new mirror patch. Continue ordered auditing
without waiting; the no-increase constraint remains in force.

| Decision | Source and proposed difference | Evidence and reachability | Status |
| --- | --- | --- | --- |
| D001 | base/cvidc/sevid004.c, GenericByteFill nested continuation: a potential change to preserve caller locals across GenericByteWrite. Exact minimal patch is not yet established. | P34 actual bodies fail with 0xc0000005 on x86/x64 before and after restoration; nested continuation loses LocalIUH. Current CCPU fills use individual stores; no guest batch consumer established. Related nested callers were source-scanned, not dynamically proven faulty. | Pending diff decision; original behavior unchanged. |
| D002 | base/cvidc/sevid008.c, S2360/S2372: potential second-WORD offset correction from +8 to +2, with multi-DWORD fill ordering still to verify. | P38 unchanged/candidate actual bodies put 33 44 at +8/+9 instead of +2/+3 on both architectures. Current CCPU DWORD stores use two WORD operations at +0/+2; no guest use of these generated DWORD entries established. | Pending diff decision; original behavior unchanged. |

| D003 | base/cvidc/sevid023.c, Chain4DwordFill (S2844 demonstrated): proposed correction of multi-element WORD-pass overlap. Minimal implementation is not yet established. | P54 actual bodies on x86/x64 request four copies of 0x44332211 but produce 11 22 33 44 33 44 33 44 33 44 followed by six untouched bytes. Current, candidate and poisoned candidate agree. Pinned XP uses the same two count-wide WORD fills at offsets +0/+2. c_sas_fillsdw loops over c_sas_storedw; no production caller of fill_dword_ev_glue was found. No guest symptom established. | Pending diff decision; original behavior unchanged. |

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
