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
