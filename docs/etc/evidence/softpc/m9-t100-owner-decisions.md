# T100 owner decision reconciliation

This is the review view of the [ordered audit](m9-t100-progress.md), not a new
retention authority. CURRENT owns task status. Existing exclusions are outside
this review and receive no entries. All choices below remain unapproved.

## Coverage and decision rules

The frozen universe remains the [472-file inventory](m9-t100-files.tsv).
File-level auditing is complete; final point-by-point reconciliation is not.
This view currently reconciles positions 1-266 only. Positions 267-472, original
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

## Positions 73-79: flags, private debug state and interrupt rejection

P7 restored R0052/R0056/R0058 (spacing and an explanatory comment).
Positions 75-77 and 79 have no pending in-scope differences.

| Pending points | Recommendation and remaining difference | Evidence / limit |
| --- | --- | --- |
| R0051 | Retain one c_getEFLAGS declaration in c_bsic.h (+1/-0). | Original definition returns IU32; internal callers do not use the conflicting external CPU macro interface. Original-header probes compiled successfully on both widths, so this is function-type consistency evidence, not a build failure or observed flag truncation. |
| R0053-R0055 | Retain snapshot.h and private debug capture/restore in c_debug.c (+45/-0). | Original task switching clears local DR7 enable bits without rebuilding cached i_brk/d_brk. Original exception checking subsequently reads those caches. Rebuilding from DR7 on restore would lose that state. Existing verify_cpu_side_state demonstrates the distinction; Compat serializes it without duplicating breakpoint logic. Restore validates counts against the original four-entry arrays. |
| R0057 | Retain do_intrupt's two-line rejection of (IU16)-1 (+2/-0). | c_main already filters its PIC result, but original NPX TakeNpxExceptionInt also calls ica_intack then do_intrupt without that filter. PIC can return -1 when IRQ13 is masked. irq_smoke verifies a real -1 result leaves stack/registers unchanged; applicability through masked NPX is source proof, not a traced NPX guest failure. The common guard is not replaced by a new caller-specific guard. |

P7 used demonstrated whole-artifact equivalence after nonsemantic removal,
not a new full runtime run. P161 subsequently passed all 392 tests. Current
source recheck confirms the private cache/task-switch distinction and the
separate NPX caller. All five IDs remain owner choices.

## Position 80: c_main.c execution integration

P8 restored R0069, R0080-R0083, R0092, R0098 and R0105. The original IP
expressions fixed actual 16-bit wrap failures in the real CCPU comparison;
these eight IDs are not pending. The redundant Compat timer wrapper was
removed, leaving original host_timer_event calls. Original unused SIGIO
consumer additions were removed after producer review, without changing PIC.

The residual after P8 is +135/-15. The following 46 IDs are grouped only for
readability; approval remains explicit for the represented differences.

| Points | Retained difference / recommendation | Why original alone is insufficient for the supported contract |
| --- | --- | --- |
| R0059, R0085, R0106, R0109, R0111, R0112 | Keep selection of original TLS simulation frames and correct jmp_buf dereference. | Existing exception/simulate/continue/unsimulate must share the same live native C frame. The selected original TLS path is reused without globally enabling NTVDM or introducing a second interpreter. Source/ABI evidence; not every longjmp interleaving has been exercised. |
| R0060-R0068 | Keep typed original/helper and integration declarations/includes. | Address/segment/PIC/timer functions retain their real original signatures; lifecycle/snapshot and executor hooks declare the integrations below. force_yoda and TakeNpxExceptionInt have selected void consumers. These declarations are justified with their represented calls, not independent new capabilities. |
| R0070-R0075, R0093, R0094, R0097, R0099, R0101, R0102, R0103 | Keep pending-work consumption, quick-loop escape, HLT wait, pacing and finite-budget boundary. | Host producers queue work; only the executor mutates devices. The original quick loop bypasses event handling and HLT cannot progress queued work without consuming it. HLT, ordinary retirement and NEXT_INST have distinct incoming edges. Finite budgets must unwind at a safe boundary; reset retains priority. Current source uses the one Compat event/timing owner, not separate queues. This is source-flow evidence supported by checkpoint/runtime tests, not an exhaustive scheduler proof. |
| R0061/R0065, R0077-R0079, R0084, R0086, R0091, R0104, R0108, R0110 | Keep private execution-state capture and lifecycle checkpoints/resume. | Original nested BIOS/BOP simulation frames cannot be serialized. Saved fetch/HLT phase and start_trap are not recoverable from registers alone; trap sampled before HLT can differ from current TF. Existing checkpoint cases cover nested capture deferral/return, HLT and fetch continuation. The interrupt-map pointer serves existing cold-run clearing, not a second device owner. R0061/R0065 are the same includes listed above, not extra IDs. |
| R0076, R0087-R0090, R0095, R0096 | Keep debug declarations, begin/refetch and five retirement sites. | POP SS, MOV SS, IRET and STI bypass ordinary retirement, requiring their own call to the same observer. Debug edits to CS:EIP/code need refetch with quick mode cleared; otherwise cached decode or skipped TF handling persists. machine/debug.c remains the policy owner. No extra debug state is added in the mirror. |
| R0100 | Keep filtering the PIC sentinel before EXT/tick/delivery. | P8 restored the original IU16 carrier and removed the redundant signed copy. -1 is uniquely represented as (IU16)-1. The common do_intrupt guard alone is too late to prevent this caller's preceding boundary effects; that guard also serves NPX. |
| R0107 | Keep the narrow c_cpu_clearHwInt bit clear. | Actual PIC deassertion needs the original CPU-vector operation; c_cpu_interrupt only sets bits and cannot replace it. The selected original standalone body was absent. No alternate IRQ delivery is introduced. |

Git confirms c_main.c still has P8 as its latest source change. Current-source
review follows the ordinary/quick/NEXT_INST branches, five retirement calls,
clock/wake consumers, reset priority and Compat lifecycle owner. P8 executed
24 IP cases per width: the previous code failed two 16-bit not-taken cases,
exact XP expressions passed all 24. That evidence qualifies the restoration,
not all retained hooks. P8's full 392/392 and later P161 regression support the
current implementation, without claiming all snapshot/debug interleavings.

## Positions 81-97: operand observation and restored register implementation

Positions 81-87, 89-91 and 93-97 have no pending in-scope differences.

### R0115-R0129: c_page.c host operand observation

P10 restored R0113/R0114 (unneeded yoda.h and explanation). Fifteen IDs remain,
+20/-6: observer declarations, saved original buffer inputs, successful bulk
read/write observations, six scalar read returns and three scalar write calls.

Recommendation: retain this existing observation route. Original c_oprnd.h
uses both cached-physical and supervisor alternatives and reversed x87 buffers.
Hooks preserve the original linear address/value and do not reread devices.
Original guest DR checking updates architectural exceptions, not the host's
watch/trace state. machine/debug.c accepts observations only inside the current
instruction and commits watch hits at retirement; debugger inspection is not
guest access. Moving hooks below these functions would observe unrelated
internal memory traffic and lose the correct operand boundary.

P10's coherent removal of the whole observation route compiled/linked on both
widths but failed the command-provider fixture's bounded wait for PAUSED after
a real watchpoint run. Current and no-yoda controls passed. This is a negative
control of the route, not individual proof of every hook or paging fault.
Current source still has P10 as its latest change and retains the same owner,
scalar widths and reverse-buffer handling. No new guest incident is claimed.

### R0130-R0134: c_reg.c, fully restored

These five IDs are restored, not submitted for retention. P11 removed the
added headers/explanation and both register snapshot wrappers. The existing
archive owner now calls original typed register accessors in the same order;
there is no remaining snapshot-register production entry in src/test. This
removed two function/API boundaries, unlike relocating a private-state copy.
Current c_reg.c has no in-scope difference. P11's complete dual regression
qualified the coherent archive adaptation; unchanged snapshot schema and
serialized data were preserved. No further owner choice is needed here.

## Positions 98-118: private TLB state and original exception paths

### R0135-R0138: c_tlb.c private snapshot boundary

Recommendation: retain snapshot.h, capture/restore and the associated original
line mapping, residual +59/-0. R0136/R0137 already lost redundant fixed-size
arguments and guards in P12; that reduction is delivered, not pending.

Original tlb/next_set/page_index/page_address are private storage. Original
flush_tlb is not restoration: it discards translations and observes current
page tables on the next lookup. P12 coherently removed these accessors and
substituted original flushing in the archive. Both architectures compiled and
linked, but failed the real checkpoint assertion expecting cached translation
0x3123. Source predicts the new table translation 0x4123; that actual value was
not printed, so it is not asserted as a measured result. Current control passed.

Current archive still allocates the fixed original page-index size and rejects
invalid/null archive storage before restore; payload-value/next-set validation
remains in the bridge. No duplicate TLB implementation or external raw cache
owner is introduced. This does not prove every possible TLB state. P12 and
P161 full regressions cover the existing implementation.

Positions 99-101 and 103-118 have no pending in-scope differences. In position
102, P13 fully restored R0139/R0140: removed the unneeded yoda.h include and
restored the original host_getenv declaration. The original PROD force_yoda
empty function links; no duplicate Compat stub was invented. Object comparison
qualified the declaration alone; full regression qualified restored no-op calls.
No pending patch remains in c_xcptn.c.

## Positions 119-137: original pointer declarations and FPU

P14 fully restored R0141-R0143 in evid_c.h. The six fields are pointers, not
32-bit storage for pointer values; the actual consumer uses offsets/size, not
the changed pointee types. Object and complete-EXE comparisons established
metadata-only change. This is not generalized to scalar native-pointer storage.
Position 120 and positions 122-137 have no pending in-scope differences.

### R0146/R0147: fpu.c representation and integer rounding

Recommendation: retain these two independently tested corrections (+8/-5
combined). P15 restored R0144/R0145/R0148-R0151: removed needless headers and
cast, and removed the FPU snapshot wrappers. The archive now calls original
get/setNpxStackRegs and control/status accessors, preserving original exported
flags, setter order, serialization and stack padding. It does not invoke
FSAVE/FRSTOR with their guest-visible reset/conversion effects.

- R0146 copies the floating representation with memcpy before bitfield reads.
  Independent exact-original restoration compiled/linked, then failed the
  actual large-integer instruction assertion expecting 0x0000123400001234 on
  both architectures. Current control passed. This is supported optimized-code
  evidence, not a claim that every possible compiler fails original punning.
- R0147 rounds M64I with existing npx_rint before conversion. Independent
  original restoration failed nearest-even 3.5 storage: first byte 00 rather
  than 04 on both architectures. Current controls passed the existing 228
  conversion cases; this is not exhaustive x87 conformance.

Current fpu.c still has P15 as its latest source change. Source/consumer
recheck confirms only the representation and rounding behavior remains local;
original native stack accessors now serve the archive directly. P15's final
nonsemantic refinement used proved EXE equivalence, while its strengthened
tag round-trip test was freshly run at both widths. P161 later ran all tests.

## Positions 138-181: IRET restored, not a retained repair

R0152 in iret.c is fully restored in P16. CALL's TSS stack selection does not
justify applying SS address size to a popped IRET operand. Actual ring-0-to-3
IRETD with a 16-bit target stack lost the saved ESP upper half under the local
helper substitution. Original operand-size selection preserves it; the same
negative/positive fixture runs on both architectures.

Current source keeps original GET_OPERAND_SIZE/SET_SP/SET_ESP in the restored
outer-privilege return block. The separate original V86 set_current_SP use is
not confused with that block or removed. Tests assert low SP for 16-bit returns
and full ESP for IRETD; they do not freeze unverified high-half hardware behavior
for 16-bit IRET. P16's full 392/392 qualified restoration. Positions 139-181
have no pending in-scope differences. No IRET patch is submitted for retention.

## Positions 182-193: selected symbols and original TLS unwind

### R0155-R0158: ntstubs.c, single original symbol owner

Recommendation: retain extern Gdp and omit the three duplicate no-op definitions
copyROM/initialise_npx/npx_reset. The combined residual is +1/-12 after P17
restored include spelling, removed the unneeded stdio include and trimmed gaps
(R0153/R0154 restored).

Current bindings retain original ev_glue.c's Gdp and original rom.c/fpu.c's
functions. Independently restoring Gdp failed both links; restoring all three
stubs failed with all three duplicate symbols at both widths. The no-op behavior
is not itself defective: exactly one original definition is required. Omitting
an active original owner or adding another local omission merely moves/increases
the difference. No substitute Compat stubs are introduced. P17 include cleanup
used final object/EXE equivalence and focused checkpoint runs, not a fabricated
fresh full suite. P161 subsequently passed the full suite.

### R0160-R0162: ntthread.c outer exit and defined failure results

P18 restored R0159's original fixed-allocation diagnostic. The three remaining
IDs are distinct choices:

- R0160: retain outer-frame selection, followed by original ccpu386Unsimulate.
  Coherent replacement with ordinary c_cpu_unsimulate returned to the wrong
  nested BIOS host frame; the actual checkpoint assertion at EIP 604 failed
  on both widths. Current and original-unwind-relay controls passed. The local
  routine now sets level=1; the original routine owns decrement, in_C and
  longjmp. There is no intervening callback/wait or cross-thread update.
- R0161/R0162: retain explicit null results in the BADID and missing-TLS branches
  of ccpu386ThrdExptnPtr. Actual original-body failure injections returned 31h
  and 39h respectively on both widths. This proves an invalid helper return,
  not safe application recovery: c_main still passes the result directly to
  setjmp without a null check. Do not approve these as a complete low-memory
  or TLS-failure lifecycle solution.

P163 removes the remaining one-line explanatory comment above the added outer
function, leaving its behavior unchanged and reducing the residual from +24/-2
to +23/-2. The recommendation concerns the remaining semantics, not that removed
comment. P18's complete checkpoint/full regression evidence covers normal
nested unwind/reentry and finite budgets, not injected whole-product failures.
Positions 184-193 have no pending in-scope differences.

## Positions 194-264: POPF and RETF fully restored

P19 restored R0163/R0164 in popf.c (unneeded stdio and diagnostic declarations).
Its real object/EXE equivalence showed no necessary supported ABI change;
original mirror warnings were not mistaken for execution faults. Positions
195-205 have no pending in-scope differences.

P20 restored R0165 in ret.c. Popped return operand width determines the restored
ESP; subsequent immediate parameter adjustment still uses stack address size.
Current code retains that original order. The combined real-instruction fixture
covers IRET, RETF and RETF 4 across two operand and two target-stack widths.
The prior RETF helper substitution failed the full saved ESP assertion on both
architectures; original restoration passed full regression. No general hardware
claim is made for upper ESP after a 16-bit return or all fault combinations.
Positions 207-264 have no pending in-scope differences. No POPF/RETF residual
is submitted for approval.

## Positions 265-266: delayed FPU exception state

### R0172: zfrsrvd.c private saved exception IP

Recommendation: retain the existing 17-line private-state accessor difference.
P21 restored R0166-R0171 (unneeded declarations/includes and prototype rewrite).
Current DoNpxException saves NpxFIP into private NpxExceptionEIP; subsequent
TakeNpxExceptionInt restores that saved IP and mutates pending/interrupt state.
It cannot be called merely to inspect the snapshot state without side effects.

The actual archive regression saves 11223344 as delayed IP, changes current FIP
to 55667788, captures, installs a later 99AABBCC exception, restores, then checks
that real exception delivery recovers 11223344. Coherent removal of both
accessors and their archive calls preserved ordinary pending-flag restoration
but failed this assertion on both architectures. This is actual delayed-IP
behavior, not only missing-symbol evidence. No second FPU owner or format change
is introduced; current archive still calls these narrow accessors.

P21's first x86 full run timed out in emulator.verifier-negative; its unchanged
isolated retry passed. That delivery is not called first-pass 392/392. P161
later passed the complete suite without retries. Position 266 is original
normalized-identical and has no pending difference. R0172 remains unapproved.

## Verification boundary

The recheck reads current CMake bindings, BIOS/PIC/descriptor callers,
CALL/stack-size operations, SAS/archive providers, CPU return declarations
and definitions, plus the existing BOP/device/BIOS fixtures. It reconciles
these claims against P1-P13 final evidence. The continuation also traces
current interrupt callers, executor/lifecycle/debug ownership, operand
observers, archive register access and private TLB storage. It does
not create a new runtime result from a source search, nor turn three qualified
recommendations into owner approval. The next position is 267.
