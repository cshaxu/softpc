# T100 owner decision reconciliation

This is the review view of the [ordered audit](m9-t100-progress.md), not a new
retention authority. CURRENT owns task status. Existing exclusions are outside
this review and receive no entries. All choices below remain unapproved.

## Coverage and decision rules

The frozen universe remains the [472-file inventory](m9-t100-files.tsv).
File-level auditing is complete; final point-by-point reconciliation is not.
This view currently reconciles positions 1-415 only. Positions 416-472, original
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

## Positions 267-270: UART, LPT and source notices

Four historical notice items are separately identified: R0173 in com.c,
R0206 in printer.c, R0238 in printer_.c and R0244 in rs232_io.c. They remain
under source/notice policy, not as functional repair recommendations. They are
not included in the semantic pending-ID count and are not erased by restoring
unneeded technical comments. There is no claim of new ownership or licensing.

### R0174/R0178/R0181/R0186/R0199: com.c

- Retain snapshot declarations and private controller/callback encoding
  (R0174/R0178). UART registers, interrupt reasons, pacing and pending callback
  identity belong to the original controller. Register replay consumes RX or
  triggers host output/scheduling, so it is not neutral restoration.
- Existing archive tests restore finite host RX bytes 51h/52h, call original
  com_recv_char and read 51h through the real port. Coherent omission of serial
  callback encode/decode rejects capture of the remaining receive event on both
  architectures. This is controller/queue/archive proof, not a live physical
  serial receive test; the current host endpoint has no live receive thread.
- Retain all three pointer-width ioctl arguments (R0181 flush, R0186 data-ready
  follow-up, R0199 modem). Current host_com_ioctl receives lib_iptr and writes
  an int through the address. Windows x64 long truncates it. Each original-cast
  variant independently access-violated on x64. The follow-up case required
  the restored RX fixture above; its earlier passing test did not reach that
  branch and is not counted as proof. Original/current x86 controls passed.

P22 restored the other R0175-R0205 formatting/declaration differences. Combined
remaining selected scope is +126/-3 including the protected notice. It passed
fresh 392/392; no physical UART or manual guest acceptance is claimed. Current
source and archive dispatch retain the same single controller/host ownership.

### R0207/R0212: printer.c

Retain the private controller-state/callback extension and its declaration.
Original port access can advance ACK state or schedule output; replay cannot
replace copying the actual READY/OUT/OUTA and delayed-event state.

P23's actual LPT test writes 33h, captures with a pending ACK, changes the data
port to 00h, restores, then reads 33h. Omitting controller restoration fails
that last assertion on both widths; omitting callback mapping fails capture.
The earlier test without the post-capture mutation could not prove controller
restoration and is not used as evidence. No physical printer is claimed.

P23 restored the remaining R0208-R0237 text differences and removed the long
added explanation. Residual +74/-0 includes the notice. Final objects/EXEs were
equivalent; the changed checkpoint test was freshly run, with full regression
inherited from P22. P161 later passed all 392. Current original controller and
Compat host sink remain distinct, without a second device implementation.

Positions 269-270 have no semantic patch beyond the notices: P24 restored
R0239-R0243, including original host_lpt_dos_open/close declarations. These
matching declarations do not introduce a call or enable an unsupported branch.
Original BIOS printer/serial functions remain wired by device_bop.

## Positions 271-274: generated interfaces restored

Position 271 evidfunc.c and position 274 evid_c.h have no pending differences.
P25 fully restored R0245 in evidfunc.h, including original duplicate mark
prototypes; actual consumer stores function addresses and does not use the
changed return contract. Before/after objects and complete EXEs were equivalent.
Original initializer warnings are disclosed, not called new execution failures.

P26 fully restored R0246-R0251 in evidgen.h: removed added aliases/rationale and
unused direct declarations; restored original IHP fields. IHP already has native
pointer width. All 28 actual dependent objects per architecture were unchanged,
and complete EXEs were metadata-equivalent. This does not prove universal
function/data-pointer interchangeability outside supported Windows builds.
No aliases/casts were moved to Compat. None of these seven restored IDs remains
an owner retention choice.

## Position 275: ev_glue.c GDP and matching aggregate

P27 removed the CHAIN2-only bridge, three wrappers and selector 4, restoring
original markers and selecting existing 2 from gfx_updt. Original string marking
can conservatively include one extra dirty group; real bank/aperture bounds keep
it inside allocated storage and painting clips to the display. Passing tests
alone were insufficient until that source-boundary review. No claim of identical
internal dirty bookkeeping is made. R0259/R0260 are restored, not pending.
R0252/R0253/R0256-R0258/R0261/R0262 were also restored.

Three existing differences remain recommended, selected residual +3/-2:

- R0254/R0263: GDP slot include and native allocation owner. A paired original
  allocation plus original direct-layout initialization control passed all three
  ring pointers on x86, but x64 preserved only the last: offsets 8/12/16 overlap
  eight-byte pointers. Current initialization preserved all three. This is a
  real initialization ABI proof, not a full guest run or a deliberately broken
  malloc/adapter mixture. Restoring only one consumer would be incoherent.
- R0255: select the existing CCPU Evid_c.h aggregate via angle brackets.
  Actual x64 CCPU/CVID sizes were 240/232 with corresponding later-field offsets
  168/160 and 236/228; x86 both were 156. The quoted-header x64 VGA control
  access-violated, while x86 passed. This is not a dynamic proof against every
  possible coherent whole-family rewrite; making another aggregate and its
  consumers differ merely to move this patch is not an admitted reduction.
  P164 restores the adjacent comment to exact XP wording while retaining the
  angle-bracket selection; no semantic or diff-line-count change is claimed.

Current source still selects existing marker 2 for the relevant producer paths,
uses one GDP creation/slot owner and has no extra CHAIN2 marker bridge. P27's
fresh 392/392 and independent controls qualify the delivered reduction. The
three remaining IDs are not automatically approved by those passes.

## Positions 276-282: original helpers and declarations

Positions 276-277 and 280-282 have no pending in-scope differences. P28 fully
restored j_c_lang.c (R0264-R0266): original includes and diagnostics, retaining
all mask/rotation helpers. Bounded actual-body mask tests passed; arbitrary
invalid native-width diagnostic strings and unused rotation semantics were not
claimed equivalent or exhaustively correct.

P29 restored R0267 in j_c_lang.h. Its actual CrulesRuntimeError consumers pass
literal strings and ignore results; all 70 dependent objects (35 per width)
were unchanged after removing the added declaration/comment. Full EXEs were
metadata-equivalent and actual VGA tests were rerun. No new compiler support,
warning suppression or testing-only API was introduced. All four IDs are
restored, not retained recommendations.

## Positions 283-288: generated video rules

The six current source hashes match the final candidates recorded in P30-P35.
Root CMake still includes sevid*.c in the production CCPU object group;
evidfunc tables and ev_glue select the original readers, writes, dither and
Generic rules. These are not removable test-only implementations. The initial
range maps and allocation inventories linked in P30-P35 provide original/current
locations; they are not new exclusion records or separate approval items.

### R0269-R0274: sevid000.c

Six semantic IDs remain recommended, with distinct evidence:

- R0269: mapped GDP fields use the same native owner as R0254/R0263. Original
  32-bit field spacing cannot hold native x64 pointers. That paired owner proof
  applies; no whole-family original restoration was dynamically run here.
- R0270: 31 local allocations retain native capacity and zeroing. Original-sized
  allocations heap-corrupt on x64; native-sized malloc and poisoned allocations
  access-violate. Generated partial IU32 stores followed by IUH loads explain
  the required initialized upper bytes. The original controls pass x86.
- R0271: six local continuations retain goto. Restoring original recursive
  dispatch frees the enclosing locals and loses their saved context; actual
  VGA controls access-violate on both architectures. This is file-specific.
- R0272: CHAIN2 reads retain the selected physical plane layout. Coherent
  original layout restoration fails actual BIOS pixel colour assertions on
  both widths; this is not a new Win3.1 guest reproduction.
- R0273: both DWORD readers retain second-WORD offset +2 and explicit low/high
  WORD combination. Original offset produces 66552211 instead of 44332211;
  original combination produces 00002211. Both independent controls fail on
  both widths. P30 restored the original intermediate extraction sequence.
- R0274: Unchained DWORD retains +2 instead of +8. Tests distinguish bytes at
  +2/+3 from +8/+9 across planes and offsets; original offset fails both widths.

R0268's 35 address wrappers were restored. Residual XP scope is +492/-497;
P30 removed 52 diff lines. P30's x64 verifier timeout/retry is recorded, not a
first-pass result. P164 subsequently passed the complete current 392/392.

### R0276/R0277/R0279: sevid001.c

Retain mapped GDP fields, 31 native zeroed local allocations, and CHAIN2
backward addressing. Original-size or poisoned local storage crashes x64;
original backward layout reads 5a instead of expected 37 on both widths.
The actual registered-reader oracle covers 384 combinations per executable,
including both read modes, address parities, counts and destination directions.
It is bounded rule evidence, not exhaustive hardware or guest validation.

R0275's 31 address wrappers and R0278's four tail continuations were restored.
Unlike sevid000, these readers have no enclosing same-file caller requiring
preserved locals after the tail return. The stale gate requiring those gotos
was removed in P31; other continuation checks remain. Residual +448/-447,
42 fewer XP diff lines. P31's gate failure and verifier retry remain disclosed.

### R0281/R0282, R0285/R0286, R0289/R0290, R0293/R0294

These are the eight pending IDs for sevid002 through sevid005, respectively:
one GDP mapping and one native local-storage group per file. Original-size
actual-body controls heap-corrupt on x64 and pass x86. Existing native fields
remain the sole GDP owner. No device algorithm is duplicated in Compat.

| File | Native local allocations | Zeroing retained only at | Restored malloc sites | Residual XP + / - |
| --- | --- | --- | --- | --- |
| sevid002.c | 32 | S2154, S2160, S2181 | 29 | 594 / 593 |
| sevid003.c | 32 | S2186, S2194, S2199, S2205, S2208 | 27 | 672 / 671 |
| sevid004.c | 32 | S2219, S2222, S2226, S2229, S2238, S2241 | 26 | 665 / 664 |
| sevid005.c | 32 | S2275 | 31 | 664 / 663 |

Retained zeroing follows identified IU32 stores subsequently read as IUH.
This is source/ABI evidence for defined upper storage, not proof that arbitrary
high-address RAM arguments survive the original narrow input path. That limit
must not be hidden by the successful write/fill tests or poison controls.

R0280/R0284/R0288/R0292 restored native-address dereferences; R0283/R0287/
R0291/R0295 restored unnecessary zeroing. P32-P35 removed respectively 70,
92, 90 and 52 XP diff lines without moving behavior elsewhere. Focused controls
cover selected V7 dither, Generic writes, backward moves and Unchained masked
writes/fills. The backward-move comparison in P34 proves preservation, not a
complete independent hardware oracle. All four delivered full suites passed.

P34 also reproduced the original nested GenericByteFill continuation failure
on both widths. The compiled scalar SAS fill path does not call its registered
bulk entry. It remains a separate original-defect decision; passing registered
tests neither repairs it nor proves a guest-triggered crash. D001-D006/L001
still require final reconciliation. No new repair is admitted here.

Seventeen semantic IDs are added to the review view (141 cumulative), separate
from the four historical notice-policy items. These recommendations remain
unapproved. This reconciliation changes documentation only; current production
hashes and P164's fresh full-regression evidence remain applicable.

## Positions 289-298: Unchained generated rules

For sevid006-sevid015 the remaining semantic decisions are exactly the existing
GDP mapping and native local-storage groups below. All native-address wrappers
have been restored. Current source hashes match the delivered candidates in
P36-P46, except sevid009/sevid010 which match their later P56 correction.
The linked per-file range/allocation inventories in those records supply the
original/current locations and individual mixed-width uses.

| File | Pending GDP / local-storage IDs | Retained calloc / restored malloc | Final XP added / deleted lines |
| --- | --- | --- | --- |
| sevid006.c | R0297 / R0298 | 11 / 21 | 713 / 712 |
| sevid007.c | R0301 / R0302 | 5 / 27 | 934 / 933 |
| sevid008.c | R0305 / R0306 | 7 / 25 | 686 / 685 |
| sevid009.c | R0309 / R0310 | 0 / 32 | 436 / 435 |
| sevid010.c | R0313 / R0314 | 7 / 25 | 774 / 773 |
| sevid011.c | R0317 / R0318 | 5 / 27 | 570 / 569 |
| sevid012.c | R0321 / R0322 | 5 / 27 | 638 / 637 |
| sevid013.c | R0325 / R0326 | 12 / 20 | 676 / 675 |
| sevid014.c | R0329 / R0330 | 11 / 21 | 794 / 793 |
| sevid015.c | R0333 / R0334 | 8 / 24 | 639 / 638 |

All ten actual-body original-capacity controls pass x86 and heap-corrupt x64.
The native GDP owner/layout proof is shared, not individually repeated for every
slot. Zeroing is confined to identified partial IU32 stores followed by IUH
reads; calloc elsewhere was restored to malloc. This does not fix or qualify
arbitrary high-address RAM inputs. The corresponding address-restoration and
unnecessary-zeroing IDs R0296/R0299 through R0332/R0335 are not retention choices.

P39/P40 initially kept one address wrapper each solely to avoid increasing
contiguous diff ranges. P56 supersedes those decisions: both original lines
were restored, and current files contain neither wrapper. Fixed original-line
correspondence proves four replacement-diff lines removed; heuristic matching
also realigned 164 lines in sevid010. That realignment is not additional restored
code. Table totals for these two files use P56's anchored count, including the
GDP include; no counting-based retention remains.

Evidence limits remain file-specific:

- P36 exercises 18 WORD writes and 48 copy cases; P37 exercises 72 WORD fills
  and 48 copies. P37's partial-mask video-source copies prove preservation
  against baseline, not the discarded per-byte-latch oracle.
- P38's 48 copies and 90 DWORD preservation cases also preserve the original
  second-WORD +8 defect in S2360/S2372. P42/P43 find the same source pattern
  in S2441-S2448 and S2489-S2496; their BYTE/WORD oracles do not dynamically
  prove those DWORD entries. Current phy_w32 calls phy_w16 at +0/+2 and
  c_sas_fillsdw loops scalar stores. No current guest trigger of those generated
  DWORD faults is established; they belong to the separate original-defect list.
- P39's 432 pixels, P40/P42/P43's 1440 pixel cases each, and P56's 576 pixels
  independently check selected real registered writers/fillers and boundaries.
  The corrected P40 fixture sets both plane-enable representations.
- P44/P45/P46 helper captures establish bounded preservation. Their independent
  subsets check 48 copies, eight zero-mask RAM cases and 240 zero-count cases,
  respectively. They are not a complete hardware oracle or outer-move matrix.

P37 had two verifier timeouts and successful same-budget retries; P44 had an
x86 retry, P45 an x64 retry. Other listed final deliveries passed their full
suites without such retries. P164 freshly passed the entire current 392/392.
Historical timeouts and withdrawn fixture assumptions are not rewritten as
initial passes or new production defects.

## Positions 299-300: CHAIN2 write rules

Five pending IDs per file remain; declarations/address syntax/extra zeroing
restored in P47/P48 are not additional decisions.

| Responsibility | sevid016.c | sevid017.c |
| --- | --- | --- |
| Native GDP fields | R0337 | R0344 |
| Native locals and required zeroing | R0338: 9 calloc, 23 malloc | R0346: 3 calloc, 29 malloc |
| Selected dirty markers | R0340 | R0347 |
| A0-gap address corrections | R0341: two shifts | R0348: four shifts |
| Local continuations | R0342: two gotos | R0349: seven gotos |

Original local capacities heap-corrupt x64. Retained zeroing includes S2629
and S2635/S2647/S2653 slot 8, where an IU32 counter later becomes an IUH buffer
offset, in addition to the eight sevid016 outer-move pointer cases. Current
source confirms these allocation counts and the required continuation targets.

Coherent original dirty restoration, retaining native GDP bindings, fails the
EGA dirty-low assertion on both architectures. P47 separately restores the
string marker and fails the zero-length sentinel. The discarded control that
omitted native GDP mapping is not evidence. Existing selected markers remain
one path; no additional marker implementation is proposed.

Original sevid016 shifts fail mode-64 incremental/full-frame assertions. The
ordinary regression did not reject original sevid017 shifts; its independent
physical oracle did: mode 1/address 100h wrote physical 200h instead of 400h.
Sixteen mode/address cases pass with the current source per architecture.
This is bounded actual-rule evidence, not a fresh guest reproduction.

Restoring the original continuation calls access-violates on both widths.
The evidence covers the restored groups, not an isolated failure proof for each
goto. Keeping the same activation preserves enclosing local ownership.
R0343 and R0350 restored 24 and 60 original private declaration lines, without
reactivating obsolete case/goto/label paths. The gate was narrowed in P47 to
permit declarations while still rejecting that old active control flow.
R0336/R0339/R0345 and the malloc portion of R0346 were also restored.

Residual XP scopes: sevid016 +593/-630; sevid017 +471/-584. Source growth from
restoring declarations is not added mirror difference. P47's initial stale-gate
failures and P48's aggregate timeout/retry results remain in their delivery
records; neither is a first-run full-pass claim. Current source hashes match
the tested final candidates. No new mirror patch is added by this review.

These twelve files add 30 pending semantic IDs, bringing the review view to
171 through position 300, plus the four separate notice-policy items. Approval
and the final original-defect reconciliation remain outstanding.

## Positions 301-303: remaining CHAIN2 ownership and layout

Current sevid018-sevid020 hashes equal the final P49-P51 candidates, including
the refined continuation restoration in P50. Existing range and allocation maps
provide original/current locations. No initial proposal supersedes its final
counter-review. Sixteen semantic IDs remain across these three files:

| File | Pending IDs | Final XP added / deleted | calloc / malloc |
| --- | --- | --- | --- |
| sevid018.c | R0351, R0353-R0356 | 495 / 602 | 4 / 28 |
| sevid019.c | R0358, R0360-R0364 | 682 / 906 | 12 / 20 |
| sevid020.c | R0367, R0369-R0372 | 637 / 667 | 13 / 19 |

GDP and native local-storage evidence retain the established separation:
original field spacing differs from native fields; original local capacities
fail x64. Zeroing remains only where identified narrow writes feed native
loads. Whole-group poisoning is not individual dynamic proof for every array.
R0352/R0359/R0368 native dereferences are restored. R0357/R0365/R0373 restore
60/168/24 private declaration lines without old active CGA control flow.
R0366/R0374 restore original shift-by-zero literals and remove their explanations.

Selected-marker decisions R0354/R0361/R0370 have coherent original-body
negative controls with native GDP mapping preserved. The first two fail the
EGA dirty-low assertion on both widths. The backward Move control in sevid020
marks dirty_high=100h instead of the bounded EGA group span around 3eh-41h.
Its initial simple-WORD lower-bound fixture failed unchanged code and was
withdrawn; the corrected original/candidate comparison is the evidence.

Layout decisions R0355/R0362/R0371 retain the A0 gap. The independent mode-3
oracle in P49 catches guest address 100h stored at physical 200h instead of
400h. P50 original constants fail VGA readback. P51's backward Copy oracle
finds guest address 101h storage untouched. These are actual-rule tests, not
claims of every bulk entry being reached by a guest.

Continuation decisions are not blanket replacements:

- R0356 retains seven calls as same-activation gotos; grouped original-call
  restoration access-violates on both architectures.
- R0363 retains eleven, not sixteen. P50 restored the five direct-only tail
  calls L21_137/153/155/161/163. Current final controls allocate/free
  672566/672566; restoring all sixteen leaves 104 allocations on each width,
  despite passing pixels. This is a demonstrated leak, not a claimed crash.
- R0372 retains five, not six. S2730's L21_251 tail call was restored. Remaining
  same-file chains lose 36 allocations with all original calls: 290/254 versus
  290/290 with the final candidate, on both widths.

R0364 retains the existing Copy WORD/DWORD empty-count guards, full-DWORD
rotation and byte-sized odd tail. Separate original restorations fault or fail
the 96-case independent fill oracle on both widths. No new bulk caller is
introduced. P50's initial gate failure was corrected to require only the eleven
necessary gotos; P49/P51 passed full suites directly. P164 now passes 392/392
on unchanged final production, without erasing those earlier failure records.

## Positions 304-311: Chain4 rules and legacy callback boundary

These eight files retain only GDP mapping and native local-storage groups.
All resolved native-address wrappers are restored. Current hashes equal final
P52-P60 candidates and the exact calloc/malloc counts below are verified:

| File | Pending GDP / local-storage IDs | calloc / malloc | XP added+deleted lines |
| --- | --- | --- | --- |
| sevid021.c | R0375 / R0377 | 14 / 18 | 1381 |
| sevid022.c | R0378 / R0380 | 4 / 28 | 1505 |
| sevid023.c | R0381 / R0383 | 0 / 32 | 973 |
| sevid024.c | R0384 / R0386 | 7 / 25 | 975 |
| sevid025.c | R0387 / R0389 | 6 / 26 | 1333 |
| sevid026.c | R0390 / R0392 | 10 / 22 | 1195 |
| sevid027.c | R0393 / R0395 | 4 / 28 | 1047 |
| sevid028.c | R0396 / R0398 | 2 / 30 | 1309 |

R0390-R0392 are recorded in sevid026's point map, not missing decisions:
R0391's 25 addresses are restored; R0392 restores 22 malloc calls while retaining
native capacities and ten mixed-width initializations. P160 replaced stale
qualification-pending text with P58 final-evidence references; no new IDs arise.

The per-function allocation inventories distinguish native capacity from
zeroing. P52/P53/P55/P57 separately test original capacity and poisoned removal
of remaining zeroing; x86 controls pass while x64 faults. P54 has no remaining
zeroing. P58-P60 record combined no-zero failures and source-level capacity/data
flow evidence; do not invent an independent original-capacity runtime result
where their record does not supply one. Each retained initialization has the
named narrow-write/native-read use, not a general claim that calloc is safer.

The selected actual-body cases (256/96/112/132/148/156/56/68 respectively)
primarily prove restoration equivalence. Independent pixel subsets and their
limits remain in P52-P60. P54's four-DWORD fill occupies ten bytes, preserving
original D003. P57's independent plane oracle exposes original D004; subsequent
148-case equality checks do not become pixel-correctness proof. These defects
remain separate decisions; no new repair or guest symptom is claimed here.
P55's all-ten-address restoration follows the owner's range-count correction,
so splitting ranges is not a reason to retain redundant wrappers.

P61 supersedes the unexplained video-source callback limitation in P59/P60:

- The selected CPU_40_STYLE original path installs c_ev_read_ptr/read_byte_addr;
  the legacy forward/backward GDP string callbacks remain null. Generated
  setters existing in a vector are not production setter invocations.
- CCPU MOVSB/MOVSW/MOVSD use scalar operands; move-array read macros have no
  active consumers. Direct legacy video-source bulk calls bypass that path and
  fault on the null callback. The condition predates the restorations.
- An actual machine/BIOS mode-13h probe ran REP MOVSB/MOVSW/MOVSD with both DF
  directions: six cases per width, destination bytes, CX and SI/DI checked,
  callbacks null before/after. Those twelve passes prove this selected path;
  they do not cover overlapping ranges, paging or all DOS/Windows video modes.

No lost initializer, new fallback or additional callback path is warranted.
The unsupported direct legacy bulk entry must not be advertised as usable
without separate integration. This boundary is explained, not an unfinished
mystery carried forward. No new guest binary or production API was introduced.

All final P52-P60 full suites passed. This reconciliation adds 32 semantic
pending IDs, cumulative 203 through file 311, with the four historical notices
still separate. Source/test/EXE bytes are unchanged from P164 qualification;
original-defect decisions and owner approval remain outstanding.

## Positions 312-318: final rules and generated accessors

Current sevid029/sevid030 and sinit011/012/013 normalized hashes still match
P62-P66 final candidates. The actual calloc/malloc counts are 3/29, 2/21,
12/8, 9/23 and 0/29. Existing per-file point/allocation inventories give the
original/current locations. Ten semantic IDs remain across those five files:

| File | Pending GDP / local-storage IDs | Final XP added+deleted lines |
| --- | --- | --- |
| sevid029.c | R0399 / R0401 | 1059 |
| sevid030.c | R0402 / R0404 | 705 |
| sinit011.c | R0405 / R0406 | 881 |
| sinit012.c | R0407 / R0408 | 1399 |
| sinit013.c | R0409 / R0411 | 1437 |

R0400/R0403 restore 52/34 native dereferences. Remaining zeroing in sevid029
is confined to S3026/S3030/S3034 slot 7; sevid030 to S3050/S3054 slot 7.
Poisoned removal fails those direct helpers on x64 while x86 passes. Faults in
dependent callers do not justify zeroing their own arrays. The 128/248-case
full-plane controls prove restoration equivalence, with the P61 legacy callback
boundary preserved, not exhaustive hardware or guest-mode conformance.

The generated accessors remain in VideoVector, not replaced by Compat:

- sinit011 restores eight pointer-getter malloc operations, retaining native
  capacity. Twelve scalar getters write IU32 then return IUH, retaining zeroing.
  P64's real-GDP 80-assertion control tests native pointer high bits too.
- sinit012 restores 18 native-sized malloc operations and five complete original
  malloc(4) byte-getter lines. Nine IU32 getters retain zeroing. P65 tests all
  32 entries with four values, including actual setter storage. Its initial
  incorrect setter-v1 fixture is not counted as production failure.
- Both scalar no-zero controls return A5A5A5A587654321 instead of 87654321
  on x64. This proves an indeterminate native return, not failure through
  typed IU32 VideoVector consumers, which discard the upper bits.
- sinit013 restores all 29 malloc operations and R0410's three native-table
  dereferences. No zeroing remains. Full native locals precede reads; native
  capacities remain. P66 verifies 104 setter assertions per width.

The three old generated selectors in sinit013 are not registered in current
C_Video. The selected original CPU_40_STYLE wiring already routes selection
to ev_glue C functions. P66's 112 controlled-table comparisons are isolated
restoration proof, not support for the old selectors with production tables.
No new selector implementation or initialization path is introduced.

Position 317 univer_c.h equals normalized XP and needs no decision. Its lack
of include consumers does not authorize deleting an otherwise original file
under the owner's no-new-diff rule. R0412 in position 318 vglfunc.c remains
recommended: five added lines locally make IFN1 valid for extern declarations.
Coherent whole-XP-file compilation fails both actual compiler configurations
at the first setter declaration; current source compiles. Changing global IFN1
would affect original function definitions elsewhere. All original vector order
and registrations remain; no alternate accessor path is added.

P62-P66 full suites passed; P67 adds actual compiler controls without changing
source. Current accessor/function hashes and vglfunc history confirm those
final sources remain installed. Passing getter tests do not approve retention
or establish a guest failure. These eleven IDs await owner choice.

## Positions 319-325: disk BIOS, private checkpoints and empty media

Three semantic IDs remain, distinct from four historical notices:

- R0414, diskbios.c: two assignments return sector count as 16-bit CX:DX,
  replacing XP's divide/remainder by 256 plus UCHAR narrowing. Actual AH=15h
  dispatch verifies 0000:0484, 0001:0000, 000f:bc10 and AH/CF/absent-drive
  behavior on current x86/x64. Coherent two-line XP restoration fails the first
  CX assertion. XP's first-case 4:132 result is source calculation; the negative
  test stops before checking later cases. Geometry/reserved-cylinder policy is
  unchanged. Residual semantic replacement is +2/-2.
- R0415, fdisk.c: +120/-0 accesses original private HDD state and encodes stable
  command/callback identities for the existing snapshot path. The archive
  serializes, rather than reimplements, that controller. Taskfile mutation and
  restore, ten command-ID round trips and invalid-ID rejection have actual
  checkpoint evidence; every in-flight transfer/callback timing does not.
  Removing all callers would remove supported preservation, not restore an
  equivalent original route. Replaying ports would execute device side effects.
- R0417, fla.c: the FDC snapshot include, callback declaration/codec and private
  state extension preserve the original controller's command/result/NDMA and
  interrupt state. The early callback address use requires its declaration.
  P69's same-source checkpoint run verifies DOR/SPECIFY mutation and restoration;
  all 8192-byte NDMA payloads and callback timings are not dynamically covered.
  The file's +99/-0 total includes its historical notice. No second controller
  or synthetic interrupt is introduced by restoration.

Current source still contains the same register assignments, private snapshot
functions and real archive consumers. Git history confirms these production
files have not changed since their respective audit evidence. R0413/R0416/
R0418/R0419 are historical notice-policy items (diskbios/fla/gfi/gfi_mpty),
not additional semantic repair decisions. Together with the earlier four,
there are eight notice items separately identified.

Positions 322-323 floppy.c/floppy_i.c retain original selected content; no
pending point exists outside skipped exclusions. Positions 324-325 gfi.c and
gfi_mpty.c differ only by the notice. Their empty-medium implementation is live:
gfi_init installs it and Compat image detach calls it. The product still has
floppy hardware without a mounted image. The empty backend and image I/O are
distinct medium states, not redundant device implementations to delete.
No new media, guest or snapshot-format change follows from this review.

## Positions 326-330: original base declarations

base_def.h, bios.h and cga.h have no pending selected-content difference;
build_id.h equals normalized XP. Their include users remain. Its legacy BIOS
date fields are not the product's fresh build timestamp and are not rewritten.

R0420 in ckmalloc.h is fully restored by P74: the redundant stdlib include and
three-line rationale were removed, eliminating all four XP diff lines. Every
real consumer already obtains allocator declarations before this header.
The current header's last code change is that restoration; later host_def
review must still preserve coherent declarations. No pending retention ID
is created by a restored header or an existing excluded branch.

This batch adds fourteen semantic IDs, cumulative 217 through position 330,
and four notice-policy items, cumulative eight. Production/test/EXE bytes are
unchanged; P164's fresh 392/392 remains applicable. D001-D006/L001 and owner
approval still require final reconciliation; no task closure is claimed.

## Positions 331-358: base/video declarations

The original-content-only dispositions in P75/P77/P79/P80/P82/P83/P84 remain
applicable for positions 331-334, 336, 338, 340-341, 343, 346-348, 349-355 and
357. Existing exclusions are skipped, not new decision entries. These retained
headers have actual original includes; gendrvr.h/gispsvga.h do not thereby
supply active excluded drivers. Removing original files and their includes
would introduce new differences and is not an admitted cleanup.

### R0421-R0423: cpu_vid.h is restored, not pending

P76 restored five original IPT argument lists and removed four rationale lines.
Actual selected non-ANSI IPT macros expand to empty argument lists, with eight
current/restored preprocessor pairs byte-identical. This does not qualify the
excluded future ANSI configuration.

P76 initially retained nine added glue-provider declarations (R0423), based on
isolated removal failing table initializers. P123/P125 supersede that conclusion:
the coherent restoration restores three original declarations beside Glue_reads
and six beside Glue_writes, and removes the added header block. Current source
and its last two commits confirm this final form. No selected provider addition
remains in cpu_vid.h. The former test proves missing declarations in an
incomplete restoration, not necessity of the local header patch.

The coherent compiler controls and complete EXE metadata-equivalence checks
passed. Two existing direct writer tests gained private declarations in P125;
the rebuilt VGA fixture passed both widths. Those coupled test declarations do
not justify retaining a production-only-for-tests API. Original provider/table
calling paths remain unchanged. R0421/R0422/R0423 are restored, with no owner
retention choice. Later file 420/422 accounting must reference these same
restorations rather than count an additional header approval item.

### Three remaining semantic IDs

- R0424, dma.h: retain dmaGetAdaptor's pointer-return declaration (+3/-0 with
  existing reason/separator). Actual nt_sbdsp SbInitializeDMA indexes the
  original controller array returned by at_dma. XP's declaration sits in an
  unselected NTVDM branch. Private-header controls pass current and fail removal
  on both widths with explicit extra -Werror=implicit-function-declaration.
  That flag is disclosed; this proves the missing pointer contract, not an
  observed guest crash or unchanged production warning-policy failure.
- R0426, egacpu.h: retain the actual generated VGAGLOBALSETTINGS pointer type
  and existing reason. Restoring VGA_GLOBALS* produces the provider-assignment
  mismatch in ev_glue on both widths under extra incompatible-pointer Werror;
  vga_prts's null-only use still compiles. No cast or second carrier is proposed.
  R0425's redundant forward tag and R0427's relocated include/comment were
  restored. P78's initial VGA-test build lacked the existing cvidc include
  directory; adding that single target dependency fixed the build. Final dual
  full regression passed. This is disclosed root build wiring, not a fallback.
- R0428, egaports.h: retain one panning query declaration. Its provider reads
  original attribute/split state, not another state owner. A no-pan actual-body
  control fails port-written pan=1 pixel assertions on both widths. This proves
  the consumed panning capability, not a coherent restoration of every panning
  implementation patch. Header removal emits the specific undeclared-query
  diagnostic under an extra flag; gfx_updt also fails for unrelated setMarkPointers
  with either header, so its exit code alone is not supporting evidence.
  R0429 removed the unnecessary T60 comment; implementation files retain their
  separate ordered decisions.

Current egacpu/egaports hashes match P78/P80 final candidates; dma's source and
actual DSP/controller consumer chain remain. These three IDs require owner
approval; successful regression does not supply it.

### Completed declaration/comment reductions

- R0430, egavideo.h: the added ega_read_dot declaration/comment was removed.
  The actual single caller already has the original local declaration. All
  fourteen current/restored consumer-object pairs match, and EXEs differ only
  in metadata. Four XP diff lines removed; no provider behavior changed.
- R0431, emm.h: three unused storage declarations restored from IHP to original
  long and the rationale removed. No implementation/caller or machine-archive
  symbol exists; reset consumer objects match on both widths. Nine XP diff
  lines removed. This does not truncate an active allocation pointer.
- R0432, gfx_upd.h: original STREAM_IO comment restored outside the excluded
  branch. R0433, gmi.h: original MEM_HANDLERS member declarations restored;
  selected IPT macro expansion is unchanged. Eight preprocessed pairs and full
  metadata-adjusted EXEs match. Combined nineteen XP diff lines removed.

Current hashes for these four headers match the final P81/P82/P84 candidates.
Their artifact-equivalence exception is documented, not described as fresh
runtime testing. P164 subsequently ran the full current 392/392 without retries.
No original BIOS date or inactive header is rewritten for cosmetic uniformity.

This group adds only three pending semantic IDs: cumulative 220 through file
358. Historical notice items remain eight. Production/test/EXE bytes are
unchanged in this reconciliation; original-defect review and owner choices
remain outstanding.

## Positions 359-369: display addressing and host/PIC/I/O headers

Current normalized contents of all eleven files equal their P90 committed
versions. Source searches confirm the actual CHAIN2 consumers, CCPU enum/switch
collision and three I/O table-size consumers. P85-P90 proofs below remain
bounded historical evidence; this reconciliation did not rerun those probes.

### R0434: gvi.h, CHAIN2 display offset conversion

The three-line addition (macro, rationale and diagnostic line restoration)
remains pending. It preserves the lane bits while mapping cell offsets to the
four-plane backing: 12 becomes 24, 13 becomes 25. Current consumers are exactly
nt_cga.c and gfx_updt.c, including cursor attributes and partial-region starts.
P85 rebuilt both real consumers with the macro mapped to original direct
get_screen_ptr; current controls pass and both architectures' restored variants
fail the independent full-cell painter oracle. This supplements the earlier
BIOS/C-VID layout proof, including R0362; it is not a new Win3.1 reproduction
or proof from a caller's mere dependence on the macro. Zero-offset uses keep
the same address contract rather than splitting the pattern for no benefit.
Choice: retain with the coupled layout contract or direct its coherent
restoration; do not approve/delete this declaration in isolation.

### Completed restorations and unchanged original headers

R0435 (host.h) restores the original memset4 IPT3 declaration and removes the
rationale. Under the selected macros both declarations expand to (); the
concrete host_def.h prototype/provider remain unchanged. Twenty complete
preprocessor pairs match. R0436/R0437 (host_com.h) restore the original comment
example and LONG declaration; ten complete consumer preprocessor pairs match.
Actual serial caller/provider native-width pointer transport is unchanged.
Both deliveries built and published dual EXEs with only identified metadata
byte differences; their testing exception is not a fresh runtime result.

host_cfg.h and host_gwi.h are wholly normalized-XP-identical. host_lpt.h,
host_sas.h, hunter.h and idetect.h have no selected residual. Their original
includes remain; inactive implementation or comment-only content is not a
reason to add include-removal differences. No exclusion bookkeeping is added.

### R0438/R0439: ica.h, distinct compiler contracts

R0438 retains four added guard/line-control lines around the original PIC
interrupt-name macros. CCPU defines CPU_HW_RESET=0 and CPU_HW_INT=2; the PIC
macro CPU_HW_INT=0 would replace the later c_cpu_interrupt switch case and
collide with reset. P89 actual c_main compiler commands pass for current
headers and fail with duplicate case on both widths when only that added
guard is removed. The original later CPU_40_STYLE callback block is unchanged.
The initial malformed probe removed another endif and was rejected; only the
corrected bounded candidate supplies evidence. No guest crash is claimed.

R0439 is a separate format/compiler residual, not a runtime semantic fix:
XP's terminal Ctrl-Z line is absent. Restoring its byte after the retained
header makes both real compiler commands fail with stray byte 0x1a. It is
outside the excluded body, so cannot be ignored as an exclusion. Proposed
retention of this one deleted line remains an explicit owner choice.

### R0440: ios.h, complete port-address dispatch

The one-line SOFTPC_STANDALONE addition selects the existing 0x10000 table
instead of 0x400; no alternate dispatch implementation is introduced. ios.c
and CCPU in.c/out.c mask the index by table size. P90 coherently recompiled all
three with the original header expression. Actual machine/CCPU IN8 controls
read registered COM1 status 0x60 at 0x03fd and empty 0xff at unregistered
0x07fd; restored variants read 0x60 at both and fail the isolation assertion
on both architectures. This is not a guest decoder test or an exhaustive
hardware-alias model. Defining another product profile solely to enlarge the
table would change unrelated behavior. Retain this existing +1/-1 adaptation
or direct coherent restoration of the port contract; owner decision pending.

This group adds three semantic IDs and one format/compiler ID. Cumulative
qualified residual IDs: 224 (223 semantic, one format/compiler), plus eight
separate historical notice items. R0435-R0437 are restored, not pending.
Production/test/EXE bytes are unchanged; P164's fresh 392/392 remains applicable.

## Positions 370-393: device, snapshot and native-time headers

All twenty-four current normalized files equal their final P98 versions.
Current providers/callers confirm the queue archive APIs, physical memory count,
Sound Blaster teardown/parser contract and restored native time_t function.
This is final-source reconciliation of P91-P98, not another runtime test run.

### Original selected headers and completed restorations

No pending selected difference exists in j_c_lang.h, keyba.h, keyboard.h,
mouse.h, mouse_io.h, ppi.h, printer.h, rom.h, rs232.h, rtc_bios.h, sasp.h,
spcfile.h, suplib.h, timer.h, timestmp.h, tmstrobe.h, trace.h or vga_dac.h.
Original include wiring remains; in particular retiring mouse_io.c did not
justify removing the original mouse_io.h includes. A retained declaration does
not claim all historical implementations are active. Exclusions get no entries.

- R0441 removed five redundant quick_ev.h explanation lines; the separately
  justified API below remains. R0443 removed the unused added overwrite-memory
  declaration; R0445 restored the original inactive sys_addr spelling. R0444's
  spacing/comment were restored while its single type token remains pending.
- R0446/R0447/R0450 restored sndblst.h constants, positions, omitted text and
  aliases, together with nt_sb.c's original BOOL initialization signature.
  Header-only restoration was correctly rejected as a partial mismatch;
  coherent restoration also adapted the fixture's unsigned declarations.
  Anchored XP diff reduction is 78 lines (heuristic total 70, due to repetitive
  provider alignment). Source +48 is not increased mirror difference. EXE
  instructions changed, so P95 actually ran all 392 tests, not the metadata
  exemption. No parser or lifetime body was changed by that restoration.
- R0452 restored timeval.h's original time_t interface and made Compat's sole
  provider return time(location), removing the long conversion and duplicate
  writeback. Both real callers already use time_t. The separate snapshot
  broken-down calendar array is unaffected. P97's injected-time test checks
  wide values where supported, NULL output and -1; full 392/392 passed. This
  does not establish every guest's post-2038 calendar behavior.
- R0453 restored three exact-XP whitespace lines in vgaports.h, removing six
  difference lines without changing bitfields. Dual published EXEs differ only
  in precisely identified metadata. The three intentional mirror whitespace
  diagnostics were accounted for; this is not an unreported failed gate.

### R0442: private event-queue snapshot contract

The remaining +35 header lines define two record types, two callback types
and measure/capture/restore declarations. All are used by the original queue
provider and Compat archive capture/serialization/load. The original queue,
hash lists, handles and countdowns are private; original add/delete APIs cannot
read them or reconstruct handles/timing without potentially running a zero-time
callback. This is source/ownership proof for snapshot behavior, not merely a
missing-declaration compiler failure. Copying the scheduler into Compat would
create a second owner. Every record field has a capture/restore/serialization
use; callbacks are persisted as stable IDs rather than process addresses.
P92 checkpoint controls cover two quick events and one tick event, invalid late
callback rejection preserving the live queue, callback-free load, countdown and
subsequent order. They do not exhaust hash collisions or guest sessions.
Retention remains pending; later implementation review is a separate position.

### R0444: physical memory length type

Only IHPE to PHY_ADDR differs on the selected Length_of_M_area declaration,
+1/-1. Original ccpusas4.c owns PHY_ADDR storage; zfrsrvd.c imports the same.
Restoring the pointer-width header type conflicts on both architectures:
x86 unsigned int versus unsigned long, x64 unsigned long long versus unsigned
long. P94 corrected its initial mistaken expectation that equal widths imply
compatible C types, then reran actual compiles. Current controls pass. This is
compile/ABI evidence, not a reproduced runtime overwrite. Do not widen the
original storage or globally narrow pointer carriers to avoid this local fix.

### R0448/R0449: Sound Blaster teardown and parser state

R0448 retains SbTerminate's IBOOL return instead of VOID, +1/-1. The current
provider returns actual PCM/MIDI close status; Machine destroy/reset and
platform reboot consume failure. Returning void coherently would remove the
existing stop-on-failed-teardown contract; the platform failure fixture checks
that boundary. No new recovery callback is proposed.

R0449 retains the 0x36 UART command definition, +1/-0. Existing nt_sbdsp.c uses
it to enter UnsupportedMidiUart and consume following bytes as UART data,
instead of misinterpreting them as later PCM commands. Current device tests
cover the command, following bytes, retained state, reset and snapshot. This is
source plus current-behavior evidence, not a fresh original-negative replay.
Both are existing residuals awaiting owner approval, not approved by P95's
successful signature restoration or by the latest full suite.

### R0451: tape_io.h compiler EOF

The only selected difference omits XP's terminal Ctrl-Z line, +0/-1. P96 used
the actual production tape_io.c compiler commands: current passes both widths,
restoring that byte fails both with stray byte 0x1a. Existing macro warnings
were not the failure cause. This is a format/compiler residual, separately
pending like R0439, with no claimed guest behavior change.

This group adds four semantic and one compiler/format ID: cumulative 229
qualified residual IDs (227 semantic, two compiler/format), plus eight separate
historical notice items. Sources/tests/EXEs are unchanged in this reconciliation;
P164's fresh 392/392 still applies, with all owner choices remaining open.

## Positions 394-401: video declarations and keyboard/mouse/PPI

All eight current normalized files match their committed P103 versions.
Current source confirms original teletype references, disabled stream storage,
keyboard command/IRQ changes, real Machine mouse ingress, reset ordering,
recurring mouse event and PPI gate/HostPpiState wiring. P99-P103 evidence is
reconciled below without inventing new runtime results.

### R0455: original stream declarations in the selected build

Four declarations in video.h remain added outside NTVDM, +4/-0. The actual
original vd_write_teletype body references all four after its NTVDM-local
block, so removing them fails actual video.c compilation on both widths.
Compat supplies FALSE/NULL/zero initial state; no later enable assignment was
found. This is not an implemented stream backend. Deleting the original body
would introduce another behavioral difference, while moving declarations would
only relocate the adaptation. R0454 already restored spelling and removed the
three unnecessary explanation lines. virtual.h, xt.h and yoda.h have no
selected pending differences. Their original include wiring remains.

### Keyboard: R0457/R0458/R0460

R0457 retains snapshot integration (118 implementation/separator lines and its
include). FIFO, scan sets, held/key-type arrays, response latches, repeat state,
refill handle and anomalous sequence pointer are private. Original port APIs
consume/mutate input and schedule work, so cannot provide equivalent capture
and restoration. Callback IDs encode actual do_int/allowRefill entries;
anomalous pointer state uses a break-table index, not a saved host address.
The existing archive is invoked at the executor safe boundary. Checkpoint and
IRQ tests cover queued state, invalid FIFO count, pointer reconstruction and
scan sets 1-3. A late validation failure can already have selected key arrays;
production staged restore resets/fails without resuming, not transactional
per-device rollback. No exhaustive refill timing claim is made.

R0458 adds the missing code_to_send=input_port_val assignment in C0. Original
C0 marks the automatic value valid without assigning it. Actual bop fixture
expects BFh at port 60; individually reverting the assignment fails that same
assertion on both architectures, while controls pass.

R0460 keeps existing PIC IRQ1 clearing on consumption of port 60 outside the
original JOKER selection. Actual IRQ fixture sees pending masked IRQ1, reads
FAh and requires IRR bit 1 cleared; individual original restoration fails both
widths. This clears a pending request, not a fabricated EOI or second queue.
R0459 restored original comment/blank text, removing five mirror diff lines.
These are bounded controller/PIC negatives, not proof of a specific old guest
incident. keybd_io.c otherwise has no selected implementation difference.

### Mouse: R0463-R0466

- R0463 replaces original five finite bursts of 100 interrupts with the one
  rate-selected recurring quick event and enable/HOLD-aware delivery. Original
  bursts cannot sustain 30/50/100/200 Hz; no-argument or partial restoration is
  not used as proof. Timer tick consumes its handle, suppresses IRQ during HOLD
  and schedules one successor; rate changes/disable cancel it. Data and timer
  IRQ enables remain separate. Existing actual-controller tests cover periods,
  HOLD recurrence, cancellation and CCPU/PIC delivery. They are not a new
  coherent-original runtime negative or universal Windows driver proof.
- R0464 archives thirteen private InPort fields and the actual timer callback.
  Reading ports changes ID phase; HOLD latches/clears movement; mode writes
  schedule interrupts. Replay therefore cannot replace side-effect-free state
  restoration. Checkpoint tests exercise held data, selection, ID validation
  and queue/controller roundtrip, not every malformed field.
- R0465 initializes the byte returned by mouse_inb even for unsupported internal
  selections. Actual CCPU IN8 otherwise passes an uninitialized IU8 to a handler
  which may leave it untouched. This is concrete source proof of indeterminate
  input, not a fresh guest or exhaustive electrical-bus experiment.
- R0466 clears stale accumulated/latched data on reset, aligns prior/current
  button state, and initializes cold controller state. BIOS already called
  q_event_init before mouse_init; the obsolete handle must be discarded rather
  than deleting a possibly reused queue entry. Reset ordering is source proof,
  with current mouse/checkpoint behavior tests, not new allocation-failure or
  exhaustive timing tests.

R0467 removed four redundant comment lines. The device itself remains the
original port handler receiving Machine mouse_send; Compat has no parallel
mouse implementation. All four residuals still require owner choice.

### PPI: R0469-R0471

R0469 selects original HostPpiState after timer_gate, removing duplicate legacy
speaker-edge handling. Original host sound uses both bits 0 and 1; the legacy
branch only notified bit-1 changes through no-argument callbacks, losing bit-0
transitions. Reconstructing those elsewhere would duplicate the state owner.
The remaining Compat disable wrapper serves BIOS reset, not PPI decoding.
This is an existing selection patch, not a newly introduced product branch.

R0470 resets gate_2_was_low alongside ppi_register on repeated initialization.
Otherwise an earlier high-gate write leaves the next high transition missing.
P103's initial sound timing failure was not taken as decisive proof: a direct
state fixture restores a high gate, calls actual ppi_init, captures state and
checks zero register plus low-gate history. Current passes both widths;
single-line original restoration fails the same gate assertion on both.

R0471 captures/restores private register phase and gate history; original reads
toggle status and writes mask/change state and trigger edges. Archive restore
notifies the same HostPpiState. Three fields remain in the existing serialized
contract; this audit does not silently alter that schema. Six current sound,
checkpoint and sound-state tests passed in P103 with mocked audio, not evidence
of native-device availability or a new Windows session.

### Counts and separate notice policy

R0456/R0461/R0462/R0468 each preserve one historical notice outside its excluded
SCCSID block. These four policy decisions are separate from runtime patches;
no excluded body is counted. Historical notice items total twelve.
This group adds eleven semantic IDs: cumulative 240 qualified residual IDs
(238 semantic, two compiler/format). Current production/test/EXE bytes remain
unchanged; P164's full 392/392 applies. No pending choice is treated as approval.

## Positions 402-408: I/O, DMA, CMOS, PIC and idle detection

All seven normalized current files match their final P111 committed versions.
Current source confirms original GLOBAL DMA ownership, decoded channel query,
CMOS topology binding, PIC delay mask/master withdrawal, private ICW sequence
and restored word conversions. P104-P111 final evidence supersedes intermediate
investigation or incomplete qualification statements in that history.

### Completed restorations and retained original stubs

R0472 restores ios.c's original empty-port block/comment and line control,
removing three mirror difference lines. Its selected implementation otherwise
matches XP. R0474/R0475 restore DMA include order and GLOBAL visibility;
R0481 removes five extra comment lines. Together with original formatting at
R0476, P106 removes fifteen XP difference lines. The visibility edit changed
EXE code, so full 392/392 was run; the later final comment removal received
fresh publication, exact metadata-only equivalence to tested EXEs and six
focused tests. No claim relies on an intermediate source version.

R0484/R0485/R0486 restore CMOS's original ULONG cast, disk comment and memory
comparison parentheses: eight XP difference lines removed. A disposable test
showed the original precedence creates internal BAD_XMS false positives, but
selected PROD cmos_error merely writes the already-correct capacity/checksum,
without emitted trace or demonstrated observable regression. That internal
assertion is not a product requirement and did not justify retaining the patch.
Final original-parentheses sources passed full 392/392. This is a deliberate
original-first restoration, not a claim that the original expression is ideal.

R0494 restores two word casts in idle_set, removing four XP difference lines.
Whole caller search found no threshold-setting consumer requiring values above
65535. The real idle detector/defaults and executor host wait remain active;
an unused setter is no reason to delete the whole component. Both EXEs changed
outside metadata and received full 392/392, not an equivalence exemption.

dummy_nt.c retains the exact original empty dummy_int, registered in the real
BIOS BOP table. Replacing/deleting an active original no-op would add needless
wiring differences. time_day.c retains original time_t behavior and the P97
coherently restored host binding. Neither has a selected behavior residual.

### DMA: R0476-R0480

- R0476 corrects two query conditions to compare the decoded channel number,
  not an address/count port against channel/channel+2. first_last still samples
  once per two-byte register read. Restoring both original conditions fails
  actual SB-DMA query-count assertion on both widths; current tests cover
  channels 1/3, other channels, low/high reads and disabled DSP.
- R0477 counts offset zero before decrement wrap: offset+1 transferable units.
  Restoring the two original boundaries fails independent memory data assertions
  on both widths. Controller unit size is handled by the existing transfer.
- R0478 preserves the original documented direction contract: decrement guest
  addresses by transfer unit, advance device memory and preserve bytes inside
  word units. Coherent original helper bodies/signatures/call arities fail real
  transfer assertions on both widths. This is not an isolated mismatched call.
  Current fixture covers both controllers, increment/decrement, read/write/
  verify, five starts, guards, terminal count, partial/ROM and auto-init cases.
  XP already restores auto-init address/count; that is not another local fix.
- R0479 exposes the original dmaGetAdaptor outside NTVDM to original DSP DMA
  initialization. QueryDMA/SetDMACountAddr use that same controller. Replacing
  the accessor with a new engine or alternate access would relocate complexity.
- R0480 archives controller/page state with its owner. Ports mutate status or
  flip-flops and cannot enumerate base/current/temporary arrays. Global storage
  visibility is not a reason to relocate equal serialization into Compat.
  Existing checkpoint covers address/count/page roundtrip, not all malformed
  fields. Failed staged restoration does not resume, without per-device rollback.

The corrected P106 private probes all compile/link and fail their specific
contract assertion; initial harness mode/boundary mistakes are not evidence.
None of these tests is a fresh exhaustive guest/device timing experiment.

### CMOS: R0482/R0483

R0482 reports attached HDD topology through one include and replacement line,
+2/-4. Original always reports C: type 3; actual FDC-only fixture restoring that
policy fails CMOS_DISK==0 on both widths, while current and dual-media cases
pass. This does not mean the product can lack its physical floppy controller;
empty A: media and absent HDD are different. D-only is source-reviewed only.
R0483 archives private CMOS bytes/index/mode/clock/handle and three real callback
IDs. Register C reads acknowledge interrupts and port replay cannot recover the
private queue state. Existing checkpoint proves a byte roundtrip, not all RTC
phases. These are qualified residuals, not an approved new RTC implementation.

### PIC: R0489-R0493, with L001 explicitly unresolved

R0489 archives private register, nesting/count and IRET state. Original ports
cannot reconstruct arbitrary in-service state. Native callback pointers are
rejected, not silently serialized. However the existing extension omits the
private ICW sequence[2]. P109's six cases per architecture compare completing
ICWs directly versus POST/restore/continuation: all twelve restored cases fail
base/mask/mode while no-restore controls succeed. That proven contract defect
is L001, not a passing test or a known guest incident. Its proposed same-owner
phase serialization changes mirror/schema and still awaits separate decision.
R0489 necessity does not establish snapshot completeness.

R0490 supplies the actual Compat EOI declarations selected by EOI_HOOKS and
the delayed mask. R0491 combines DelayIrqLine with the original scan mask so a
request from real DSP completion is retained but not delivered before its
release event. Original expression restoration fails the existing before-release
assertion on both adapters/architectures. This is not the IRET completion mask.
R0492 withdraws master INTR when its final pending request disappears; real
PIC raise/withdraw/capture/re-raise controls pass, and reverting the added
branch leaves cpu_interrupt asserted on both widths. Slave withdrawal already
recurses through the master. This proves output ownership, not a guest lockup.
R0493 makes IRET-hook control a no-op for the selected CCPU which receives but
never completes that protocol. Restoring the entire original function yields
three IRQ1 deliveries then vector -1 on the fourth; controls deliver all eight
on both widths. No invented obligation is placed on the CPU to consume hooks.

P110 removed only six extra explanation lines. ICW1 itself remains original;
neighbor timer cancellation behavior is not transplanted. Six focused tests
passed and EXEs were metadata-equivalent to the full-tested baseline. These
results cannot erase the separately reproduced partial-ICW archive defect.

### Counts and verification boundary

R0473/R0487/R0488 are three historical notice-policy items, separate from runtime
patches; notice total becomes fifteen. This group adds twelve semantic IDs,
for 252 qualified residual IDs (250 semantic, two compiler/format). L001 is
not included as an existing retention patch; its new repair decision remains
separate. This reconciliation changes no source/test/EXE. P164's fresh 392/392
remains applicable to unchanged artifacts, with all owner decisions open.

## Positions 409-415: queue, ROM, PIT and original interrupt handlers

All seven normalized current files match their final P119 committed versions.
Current source confirms queue replacement only after temporary reconstruction,
full option-ROM segment assignment, initial-event zero-delay protection and the
still-unprotected active-backlog calculation. These reconcile P112-P119 final
proofs; no runtime result is newly generated by this document-only pass.

### R0496: event-queue snapshot implementation

After R0497 removed six explanatory lines, the existing extension is +230
lines including separators. It measures/captures/restores private quick/tick
lists, hash chains, handles and countdowns for the real device archive. Original
add/delete cannot prescribe those values and zero-delay add immediately invokes
callbacks; replay is not restore-without-dispatch. Stable callback IDs cover
actual devices rather than persisting code addresses. Moving equal private
operations to Compat would not reduce implementation or preserve ownership.

Capture checks list/hash membership. Restore builds and validates both temporary
lists, cleans them on failure using the existing helper, then replaces live
queues only after both succeed. This retains one scheduler, with no new timer
policy. P113 checkpoint and queue-archive tests passed on both widths, covering
late-invalid callback preserving live state, callback-free load/order and an
orphan hash member after a valid prefix. They do not prove every allocation
failure or malformed archive combination. R0496 remains pending alongside its
separately audited header R0442, not two competing implementations.

### R0499: ROM initialization segment width

Only removal of XP's UCHAR conversion remains selected, +1/-1. At ROM C8000,
the scanner must call C000:8003; truncating CS to eight bits selects 0000:8003.
The retained test builds a minimal valid 512-byte ROM, executes it through the
real scanner/CPU and FE00:095A trampoline, and checks actual CS bytes 00 C0
stored at physical 0500. P114 full 392/392 includes that test on both widths.
Original failure here is established by address arithmetic/source, not a fresh
original-negative run or a captured Windows symptom. All option-ROM layouts
and protected-mode scenarios are not claimed.

R0498/R0500 restored scalar ULONG and original unused display_string length/
loop code, removing fifteen mirror difference lines. Existing empty ROM stubs
remain original. No caller required the host-size expansion; warning-only
pointer casts do not establish a needed adaptation for constant adapter 5.
Nonmetadata EXE changes received full tests rather than an equivalence exemption.

### PIT: R0502-R0505, with D005 still open

R0502 retains the snapshot include and 358-line extension span. Original PIT
owns counters, latches, modes, state/action functions, phase and pending IRQ
state; port access mutates latches and cannot enumerate these. Finite function
IDs and callback IDs avoid native pointers. Elapsed timestamps are rebased
against each existing getTime source; channel 2 republishes through the existing
waveform owner. No separate counter or waveform generator is introduced.
Checkpoint verifies programmed count/mode/state/action roundtrip and invalid
state ID, not all transitions. Counter restore can partially modify before
failure; production does not resume failed load, without per-counter rollback.

R0503 retains two first-event minimum-delay lines. Removing them from actual
PIT causes real pit_smoke stack overflow on both architectures: integer division
yields zero and original add_event recursively invokes the callback. R0504
retains estimated-read wrap suppression in the existing updater; coherent
original restoration manufactures IRQ work during latch/read without elapsed
heartbeat and fails the no-IRQ assertion on both widths. R0505 keeps channel-2
POST gate LOW consistent with the original PPI reset register; original RISE
fails actual sound delivery-count tests on both widths. Those sound tests use
the output fixture, not proof of native playback.

D005 remains separate: active-backlog int_delay=SYSTEM_TICK_INTV/(n+1) can still
be zero. P115 actual controller/queue dispatch after two heartbeats reproduces
stack overflow on both widths at mode 2/divisor 1, despite the existing guard.
The isolated two-line minimum-delay candidate completes eighteen mode/divisor
cases, but introduces a new mirror point and is NOT applied. It awaits owner
decision, is not TODO, and is not hidden by current full-test success.

P116 restored surrounding original formatting and removed added comments
(including R0506), reducing XP difference lines by 75 with behavior unchanged.
Final dual EXEs were metadata-equivalent and six focused tests passed; those
six did not include/fix D005. Necessary residuals are not claimed sufficient.

### Original handlers, compiler EOF and completed CGA restoration

illegalp.c selected code is original and illegal_op_int is registered at BIOS
0x06; its PROD empty helper and test-only subordinate entry do not make the
whole file unused. timestrb.c retains original time_tick/rtc_tick/tick queue/
IRET-check and video refresh wiring through host_timer_event. Neither adds a
patch or resolves D005.

unexp_nt.c's original interrupt handler remains unchanged. R0508 removes only
XP's terminal Ctrl-Z, +0/-1; the extra replacement blank was restored away.
Both actual production compiler commands reject a restored 0x1a with stray-byte
error and pass current source. This is compiler evidence, not guest behavior.
R0509 restored two CGA callback definitions to original (), removing four XP
difference lines. Active IPT2 already expands to (); corrected preprocessing
comparison and whole EXE metadata-only equivalence confirm no added type
contract was lost. Preliminary overbroad comparison normalization was rejected.

R0495/R0501/R0507 are three separate historical notice-policy items (eighteen
total). Six semantic IDs and one compiler/format ID are added: cumulative 259
residuals (256 semantic, three compiler/format). Existing exclusions are not
counted. Production/test/EXE bytes remain unchanged; P164 fresh 392/392 applies.
D005 and all owner retention decisions remain unapproved.

## Verification boundary

The recheck reads current CMake bindings, BIOS/PIC/descriptor callers,
CALL/stack-size operations, SAS/archive providers, CPU return declarations
and definitions, plus the existing BOP/device/BIOS fixtures. It reconciles
these claims against P1-P13 final evidence. The continuation also traces
current interrupt callers, executor/lifecycle/debug ownership, operand
observers, archive register access and private TLB storage. It does
not create a new runtime result from a source search, nor turn three qualified
recommendations into owner approval. The next position is 416.
