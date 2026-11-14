# T100 owner decision reconciliation

This is the review view of the [ordered audit](m9-t100-progress.md), not a new
retention authority. CURRENT owns task status. Existing exclusions are outside
this review and receive no entries. All choices below remain unapproved.

## Coverage and decision rules

The frozen universe remains the [472-file inventory](m9-t100-files.tsv).
File-level auditing is complete; final point-by-point reconciliation is not.
This view currently reconciles positions 1-72 only. Positions 73-472, original
defect decisions D001-D006 and local-extension defect L001 still require their
final review here. Their existing evidence and pending status remain in force.

An existing residual and a proposed new repair are distinct decisions. A failed
original-body probe proves only the exercised contract; a passing current test
does not approve the remaining patch. Source/ABI evidence is identified as such.
Do not infer approval from delivery of other restorations in the same file.

## Positions 1-9: BIOS

Positions 1 and 5 have delivered nonsemantic restorations R0001/R0002.
Positions 2, 3, 4 and 7 have no pending in-scope differences. In position 6,
R0003-R0005 were restored in P1; in positions 8-9, R0007/R0009/R0011/R0012
were restored in P2. R0008's redundant inline explanation is removed in P161;
the command-port correction remains unchanged and pending. No new point is
created for trimming that existing difference.

### R0006: reset.c, two optional DOS mouse-driver calls

- Original: XP lines 910-911 call `mouse_driver_termination()` and
  `mouse_driver_initialisation()` during reset. Local: both calls are omitted;
  the current in-scope residual is +0/-2.
- Actual path: root CMake compiles reset.c; Compat device_bop.c installs it as
  BIOS_RESET. Original `mouse_init()` still initializes the InPort device.
  The optional Insignia DOS software driver is separate from that hardware.
- Evidence: the software-driver definitions/state were retired; production
  does not install its BIOS_MOUSE BOP entries. Current bop_smoke asserts those
  entries are NULL and BIOS_RESET is the original reset function. Restoring
  only the calls leaves unresolved definitions. Restoring the whole subsystem
  would reverse its explicit retirement, not fix an original reset bug.
- Recommendation: retain the two-call omission as the selected product
  boundary. There is no duplicate Compat DOS driver to replace. This is source
  and binding evidence, not proof that the optional original driver could
  never be reintegrated. An alternative owner choice would require deliberate
  full-subsystem restoration and renewed lifecycle/snapshot qualification.
- Delivery/proof: P1 final-source dual build and 392/392; P159 full regression
  remains applicable to this unchanged path. No new interactive guest proof.

### R0008: slave_bs.c, D11_int slave PIC EOI

- Original: `outb( ICA1_PORT_1, 0x20 );`. Local: `ICA1_PORT_0` instead;
  in-scope residual +1/-1. P161 removes the local comment only: no additional
  diff-line reduction and no behavior change are claimed.
- Actual path: BIOS dispatch entries 72h/73h/74h/76h/77h call original D11_int.
  Original SWPIC_outb interprets the odd port as the interrupt mask register
  and the even port's 20h command as a non-specific EOI.
- Evidence: for slave IRQ11 in service with mask f7h, D11 masks it to ffh;
  the original second odd-port write would replace the mask with 20h and
  leave slave ISR bit 3 uncleared. This follows the actual controller source.
  Existing verify_unused_irq exercises idle, slave IRQ11, master IRQ5 and
  spurious cascade, asserting ISR, masks and BDA results. Its current pass is
  not described as a fresh negative run of the original line.
- Recommendation: retain the single port correction. Reverting restores the
  demonstrated register semantics error; this does not establish a particular
  DOS or Windows user-visible failure.
- Delivery/proof: P2 final qualification and unchanged P159 full regression;
  P161 final-source qualification is recorded separately when complete.

### R0010: tape_io.c, descriptor limit output type

- Original declaration: `word *limit`; local declaration: `sys_addr *limit`;
  in-scope residual +1/-1. No replacement descriptor decoder was introduced.
- Actual path: original cassette_io INT15/MOVE_BLOCK passes addresses of
  `sys_addr` source_limit/target_limit objects. Compat retrieve_descr_fields
  delegates to original CPU40 read_descriptor_linear and copies its results.
- Evidence: the original decoder can return a 32-bit limit after granularity
  expansion. The current function definition and callers use that width;
  restoring only the old declaration makes the cross-unit contract disagree.
  Changing the helper to write 16 bits instead would leave the upper half of
  the caller's object untouched and lose limits above ffff. Current fixture
  cases verify ffff, 12345 and ffffffff with neighboring guard values.
- Recommendation: retain the one-line declaration correction. This is an ABI
  and source data-flow finding, supported by current decoder assertions; it
  is not a claimed customer fault or fresh original-helper runtime experiment.
- Delivery/proof: P2 final dual build and 392/392; unchanged path included in
  P159 full regression. P2 also restored original INT15/C0 getCS through an
  actual ROM-entry test; that separate restoration is not pending here.

## Positions 10-46: initial CCPU files

Positions 10-31 and 33-40 have no pending in-scope differences. Position 32
bswap.h is XP-identical and retained by explicit owner direction: R0013's
attempted deletion was withdrawn, not delivered or pending approval. Positions
43-46 likewise have no pending in-scope differences. None of these files is
submitted as a retained local semantic patch.

### R0014: c-files, nine omitted source entries

- Original includes bswap.c, ccpupig.c, cmpxchg.c, c_getset.c, invd.c,
  invlpg.c, nop.c, wbinvd.c and xadd.c. The local list omits them: +0/-9.
- Actual path: root CMake reads this list to form the CCPU source target.
  Current selection includes SPC386, not SPC486 or the PIG harness.
- Evidence: the original six additional instruction bodies belong to SPC486;
  ccpupig/c_getset belong to PIG. Original NOP is empty and the selected 90h
  opcode handler already performs no operation without calling that worker.
  There is no duplicate Compat implementation of these omitted workers.
- Recommendation: retain the source-subset list. Restoring only list entries
  references absent files; coherent restoration also entails restoring those
  retired files. This is a product source-selection decision, not proof that
  compiling the complete original project is inherently impossible or unsafe.
  P3's earlier broad deletion rationale is superseded by the later owner ban
  on new mirror differences; it is not used to justify new deletion here.
- Proof: original-source and CMake consumer review recorded in P3; no dynamic
  failure of a complete expanded source set is claimed. The residual remains
  an owner choice, distinct from the approved preservation of bswap.h.

### R0015: call.c, privilege-changing CALL stack address size

- Original CALLF selects SET_SP/SET_ESP using GET_OPERAND_SIZE after loading
  the destination SS. Local uses GET_SS_AR_X instead: +1/-1 after P4's
  restoration of the original four-line if/else shape.
- Actual path: c_main dispatches CALLF; call.c obtains the TSS stack, calls
  load_SS_cache, then pushes through the original stack implementation.
- Evidence: original c_stack.c uses the SS B bit for stack addressing. A
  16-bit gate entering a 32-bit stack cannot update only the low SP and then
  correctly push through an unrelated stale high ESP. Operand item width and
  stack address width are different quantities. Current source still uses
  the selected SS attribute, with original branch assignments unchanged.
- Recommendation: retain the one-condition correction. This is concrete
  source/data-flow evidence. Existing verify_stack_address_width tests the
  stack helper, not an executed CALL-gate transition; P4 explicitly makes no
  full call-gate matrix or observed guest-failure claim.
- Delivery/proof: P4 minimized the earlier helper substitution and passed
  dual build/publication plus 392/392. The current path is unchanged since
  that delivery and included in P159 qualification.

## Positions 47-63: SAS and following CCPU files

Position 47 ccpusas4.c retains sixteen qualified point IDs, grouped below by
contract. P5 restored R0016/R0017 (unused includes) and R0023/R0025 (local
explanations). Positions 48-63 have no pending in-scope differences, including
the unchanged cmpxchg.h preserved under the no-new-diff policy.

| Pending points | Original/local difference and recommendation | Evidence and precise limit |
| --- | --- | --- |
| R0018, R0021, R0022 | Retain the snapshot declaration include and SAS capture/restore accessors. XP has no corresponding accessors. | Compat ccpu/archive.c captures/restores RAM, page types, wrap mask and private selectors_set/code_sel/data_sel. The original selector-registration API rewrites guest LDT entries and always registers selectors; it cannot reproduce unregistered saved state without mutating restored RAM. Current accessors operate on the original owner's fields, not a second device model. Existing archive round trips are supporting evidence, not proof of every snapshot state. |
| R0019, R0020 | Retain matching UTINY* prototypes for host_sas_init/host_sas_term; XP has no declarations here. | Compat memory.c returns pointers; SAS assigns the init result as its live RAM base and compares term's result with NULL. An implicit int call is not the provider's C ABI and can truncate a 64-bit allocation address. This is actual provider/caller type evidence, not a claim every allocation necessarily lands above 4 GiB. |
| R0024 | Retain phyR's subtraction-based complete-span bound in place of the original addr+typeSize+1 bound. | Original rejects valid last WORD/DWORD locations. Existing device_contract assertions compare terminal reads with independently set RAM bytes. Current code first rejects addr beyond RAM, then subtracts safely; no new read path. Current regression does not establish all possible paging states. |
| R0026, R0027, R0030, R0033, R0034, R0035 | Retain six BIOS read/write checks selecting PG_U when CPL equals 3 instead of when it differs. | Original paging code defines PG_U as user access and independently uses CPL3. bios_access_smoke checks the requested permission via controlled translation wrappers across privilege/page/read-write/width combinations. These are not exhaustive real page-table or V86 proofs. |
| R0028, R0029, R0031, R0032 | Retain IU16/IU32 fallback return casts instead of IU8 in WORD/DWORD BIOS readers. | biosDoInst returns EAX; IU8 discards the upper result bits. Existing width fixture forces the outer miss, then executes original ROM fallback instructions in protected/V86 cases. This is distinct from the instrumented permission checks and does not modify original guest binaries. |

The file's P5 residual was +69/-11 outside existing skipped scope. P5's four
nonsemantic removals had byte-identical compiled objects on both architectures;
whole EXEs differed only in recorded timestamp/checksum bytes. Therefore P5
used the authorized artifact-equivalence exception, not a fabricated fresh
392-test run. P4 provided that prior regression; P159 later ran the complete
suite again. Current source/provider/fixture review confirms the contracts
above still exist. All sixteen IDs remain owner choices, not automatic approval.

## Positions 64-72: external CPU declarations

P6 restored R0036/R0037, removing the forced GDP include and unused native-slot
header from cpu4gen.h. The original include spelling/comment remain. Positions
65-72 have no pending in-scope differences.

### R0038-R0050: cpu4gen.h return types

- R0038-R0041 change c_setCS/c_setSS/c_setDS/c_setES declarations from IUH to
  ISM32; R0042-R0050 change c_getCF/PF/AF/ZF/SF/TF/IF/DF/OF from IBOOL to
  ISM32. The residual is thirteen replaced declarations, +13/-13.
- Original c_reg.c definitions return ISM32 for all thirteen. Current source
  recheck confirms those providers and the matching local declarations. The
  selected Windows types distinguish long, int and the native-width unsigned
  type; equal byte width alone does not make function return types compatible.
- Original c_reg.c intentionally does not include the external CPU header,
  because its internal register macros conflict. Separate compilation therefore
  need not diagnose the inconsistent original declarations. This is C type/ABI
  evidence, not a claimed reproduced Windows flag truncation or guest failure.
- Recommendation: retain the declaration corrections rather than changing
  original providers or global fundamental types. No new helper or alternative
  register path is needed. All thirteen IDs remain separately unapproved.
- P6 preprocessed the 116 actual dependent compilations while restoring the
  unrelated includes. Final object/EXE equivalence qualified that cleanup,
  with only timestamp-bearing objects changing; it did not dynamically prove
  each return-type mismatch. P159 later supplied complete regression coverage.

## Verification boundary

The recheck reads current CMake bindings, BIOS/PIC/descriptor callers,
CALL/stack-size operations, SAS/archive providers, CPU return declarations
and definitions, plus the existing BOP/device/BIOS fixtures. It reconciles
these claims against P1-P6 final evidence. It does
not create a new runtime result from a source search, nor turn three qualified
recommendations into owner approval. The next position is 73.
