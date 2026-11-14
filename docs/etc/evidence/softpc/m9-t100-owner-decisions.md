# T100 owner decision reconciliation

This is the review view of the [ordered audit](m9-t100-progress.md), not a new
retention authority. CURRENT owns task status. Existing exclusions are outside
this review and receive no entries. Existing A/B residual retention choices
remain unapproved until recorded in S3 below. S1's new-repair recommendations
are historical; S2 decisions supersede them.

## S3 approval progress

**Superseded by owner revocation:** all retention approvals and automatic
delegation recorded below are withdrawn. Current approved count is zero.
The tables and P2-P15 narratives are historical traceability, not permission
to retain any point. The owner requires fresh review of all current differences,
including former approvals, with category block/line/point totals. This is not
an instruction to revert production code. P15's limited classification does
not satisfy that whole-domain request.

Owner subsequently authorizes [P16 text restoration](m9-t100-s3-p16.md), not
semantic retention: 176 text hunks in 37 files are restored. Their relationship
to historical IDs is recorded separately, including text within R0344/R0345 and
R0619. These restorations are not new semantic points or revived approvals.

Owner subsequently closes [S3](../../../history/M9-T100-S3-mirror-review.md).
Outstanding full reclassification and retention decisions remain with the open
T100 proposal; this closure supplies no additional semantic approval.

S3 starts at `62474beb` after owner closure of
[S2](../../../history/M9-T100-S2-candidate-review.md). Its remaining existing
residual ledger is the 381 S1 IDs below (375 semantic/ABI, six compiler), with
nineteen notices tracked separately. No approval is inferred from S2 closure.
S2 repairs D004/D005/D006 and TEST01 are accepted; L001/D001-D003 were declined
this round. Those outcomes do not authorize or reject another ID in their files.

Review order remains the frozen 472-file order; restored/identical files and
#error-excluded bodies are not reviewed again. Recorded and next decisions:

| File position / ID | Difference | Recommendation | Owner decision |
| --- | --- | --- | --- |
| 6 / R0006 | Omit two reset calls to the retired optional DOS software mouse driver. | Retain product-boundary omission; hardware mouse initialization remains. | Owner approves retention, S3 P2 |
| 8 / R0008 | Slave PIC EOI targets command rather than mask port. | Retain one-token port correction. | Owner approves retention, S3 P2 |
| 9 / R0010 | Descriptor limit output declaration uses sys_addr rather than word. | Retain declaration matching the actual 32-bit result/callers. | Owner approves retention, S3 P2 |
| 41 / R0014 | Omit nine retired/unselected CCPU source entries from c-files. | Retain existing selected-source list; this is a product selection, not a runtime defect repair. | Owner approves retention, S3 P3 |
| 42 / R0015 | CALLF chooses SP/ESP using destination SS address size. | Retain the one-condition correction; operand width is not stack address width. | Owner approves retention, S3 P3 |
| 47 / R0018, R0021, R0022 | Snapshot declaration and private SAS capture/restore accessors (+1/+27/+28, no deletions). | Retain access to existing owner state; original selector registration mutates LDT and cannot substitute for restoration. | Owner approves retention, S3 P4 |
| 47 / R0019, R0020 | Pointer-return prototypes for host_sas_init/term (+1/-0 each). | Retain declarations matching the actual provider and pointer-using callers. | Owner approves retention, S3 P4 |
| 47 / R0024 | Physical read span bounds (+1/-1). | Retain correct end-of-RAM check with subtraction avoiding addition wrap. | Owner approves retention, S3 P5 |
| 47 / R0026, R0027, R0030, R0033, R0034, R0035 | BIOS access user-permission selection (+1/-1 each). | Retain CPL==3 rather than CPL!=3 for PG_U. | Owner approves retention, S3 P5 |
| 47 / R0028, R0029, R0031, R0032 | WORD/DWORD fallback return casts (+1/-1 each). | Retain IU16/IU32 rather than IU8, preserving the returned value width. | Owner approves retention, S3 P5 |
| 64 / R0038-R0041 | Four segment setter return declarations, IUH to ISM32 (+1/-1 each). | Match original c_reg.c providers without changing the function bodies. | Owner approves retention, S3 P6 |
| 64 / R0042-R0050 | Nine flag getter return declarations, IBOOL to ISM32 (+1/-1 each). | Match original c_reg.c providers; type consistency, not a claimed reproduced guest fault. | Owner approves retention, S3 P6 |
| 73 / R0051 | Internal c_getEFLAGS declaration (+1/-0). | Actual provider returns IU32 and internal callers require the matching prototype. | Retained under owner small-diff delegation, S3 P6 |
| 74 / R0053 | Include snapshot.h (+1/-0). | Actual capture/restore definitions require the shared state declaration; this does not approve their larger bodies. | Retained under owner small-diff delegation, S3 P6 |
| 74 / R0054 | Capture private breakpoint state (+20/-0 including separator). | Registers alone cannot reproduce original cached breakpoint state. | Owner approves retention in the owning mirror file, S3 P7 |
| 74 / R0055 | Restore private breakpoint state (+24/-0 including separator). | Preserve cache/count state with bounds on the original four-entry arrays. | Owner approves retention in the owning mirror file, S3 P7 |
| 78 / R0057 | Reject PIC -1 sentinel in do_intrupt (+2/-0). | Original NPX caller lacks c_main's filter; real PIC/interrupt test checks unchanged stack/registers. | Retained under owner small-diff delegation, S3 P6 |
| 80 / R0059 | Select original TLS simulation header (+1/-1). | Selected SOFTPC_CCPU_TLS_SIMSTACK build uses the original TLS/setjmp path; no second interpreter. | Retained under owner small-diff delegation, S3 P7 |
| 80 / R0060-R0068 | Nine headers/typed declarations (+1/-0 each). | Actual address, lifecycle, segment, PIC, timer, snapshot, Yoda, NPX and executor consumers require their declarations. | Retained under owner small-diff delegation, S3 P7 |
| 80 / R0070-R0075 | Six host event/budget/pacing/wait declarations (+1/-0 each). | Current original-loop calls match actual Compat platform.c providers. | Retained under owner small-diff delegation, S3 P7 |
| 80 / R0076 | Two debug observer declarations (+2/-0). | Actual begin/retired calls match machine/debug.c providers. | Retained under owner small-diff delegation, S3 P7 |
| 80 / R0077 | Return private interrupt-map address (+6/-0 including separator blanks). | Actual cold-run lifecycle clears the original map through this accessor; original timer-bit clear does not clear all pending bits. | Owner approves retention, S3 P8 |
| 80 / R0078 | Capture private execution state (+16/-0 including separator). | Preserve DR/TR, pending interrupt map and sampled start_trap. | Owner approves retention, S3 P8 |
| 80 / R0079 | Restore private execution state (+17/-0 including separator). | Restore saved state and discard transient quick_mode before refetch. | Owner approves retention, S3 P8 |
| 80 / R0084 | Local saved-entry variable (+1/-0). | Carries the one-shot saved HLT/fetch phase and sampled trap bit. | Owner approves retention, S3 P9 |
| 80 / R0085 | TLS exception selection and jmp_buf dereference (+2/-2). | Selected TLS provider returns jmp_buf pointer; setjmp stays within the live CPU execution frame. | Owner approves retention, S3 P9 |
| 80 / R0086 | Consume saved entry before ordinary NEXT_INST (+11/-0). | Resume HLT waiting or saved fetch phase without executing the following instruction prematurely or repeating boundary work. | Owner approves retention, S3 P9 |
| 80 / R0087 | Debug return refetch (+5/-1, replacing a blank line). | A debugger may change CS:EIP, code or flags while parked; invalidate quick mode and refetch. | Owner approves retention, S3 P10 |
| 80 / R0088, R0089 | POP SS and MOV SS retirement notifications (+1/-1 each, replacing blanks). | Original inhibited-interrupt early jumps bypass the ordinary debug retirement site. | Owner approves retention, S3 P10 |
| 80 / R0090 | IRET retirement notification (+1/-0). | Original IRET early jump likewise bypasses ordinary retirement. | Owner approves retention, S3 P10 |
| 80 / R0091 | HLT resume label and checkpoint (+2/-0). | Resume saved halt phase and expose the existing lifecycle checkpoint while halted. | Owner approves retention, S3 P11 |
| 80 / R0093 | Consume host clock/control events while halted (+4/-0). | Wake alone does not deliver pending device/control work; keep execution on the executor. | Owner approves retention, S3 P11 |
| 80 / R0094 | Wait when interrupt map and quick-event work are empty (+3/-0). | Existing Compat event wait avoids idle spinning without blocking pending quick-event work. | Owner approves retention, S3 P11 |
| 80 / R0095 | STI retirement notification (+1/-0). | Original STI inhibited-interrupt jump bypasses ordinary retirement. | Owner approves retention, S3 P12 |
| 80 / R0096 | Ordinary instruction retirement notification (+2/-0 including separator). | Shared debug observer counts completed ordinary instructions; early jumps do not also pass here. | Owner approves retention, S3 P12 |
| 80 / R0097 | Escape quick mode for pending host events (+2/-0). | Otherwise the original direct DO_INST shortcut bypasses host clock/control consumption. | Owner approves retention, S3 P12 |
| 80 / R0099 | Consume clock/control events before original trap/IRQ checks (+6/-0 including two blanks). | These checks must see events produced at this boundary; direct NEXT_INST entry still needs its own consumption point. | Owner approves retention, S3 P13 |
| 80 / R0100 | Guard the complete PIC acknowledgement block (+7/-4). | Reject -1 before EXT, SYNCH_TICK and interrupt entry; four original operations are only reindented. | Owner approves retention, S3 P14 |
| 80 / R0102 | Existing Compat pacing call (+2/-0 including separator). | Actual host pacing provider owns timing; removing the instruction-boundary call bypasses that policy. | Retained under owner small-diff delegation, S3 P14 |
| 80 / R0104 | RESUME_FETCH label and checkpoint (+2/-0). | Saved fetch resumes after pacing/budget work and before decode setup; observer retains existing depth/phase ownership. | Retained under owner small-diff delegation, S3 P14 |

Owner says "批准" to the first three-point retention batch and "好 批准这两"
to R0014/R0015, then approves R0018-R0022 after the complete code and individual
diff sizes are presented. Those five total +58/-0; the function counts include
their trailing separator blanks. Owner then approves the eleven boundary,
permission and fallback-width points (+11/-11), followed by R0038-R0050.
Owner additionally delegates strongly evidenced points of at most two changed
lines. Count added plus deleted lines per existing point, never net growth or
artificially split functions. R0051/R0053/R0057 meet that rule after individual
provider/caller/state checks. Owner then approves R0054/R0055 after reviewing
the full diff and why private-state access belongs in c_debug.c. S3 P7 also
retains seventeen qualifying c_main.c header/declaration/TLS points (+18/-1)
under the same delegation. Owner then approves R0077-R0079 (+39/-0) after
reviewing their code, consumers and the mutable-pointer encapsulation limit.
Owner then approves R0084-R0086 (+14/-2) after reviewing the actual code,
TLS declaration and one-shot Compat resume consumer. All three count as explicit
approvals, including the qualifying one-line R0084. Owner then approves
R0087-R0090 (+8/-3 including replaced blank lines), all recorded as explicit
approvals. Owner then approves R0091/R0093/R0094 (+9/-0).
Owner then approves R0095-R0097 (+5/-0), expanding automatic retention to
existing points with added plus deleted lines strictly less than five (at most
four). Evidence remains required; do not split IDs or count net growth. This
supersedes the earlier two-line threshold without authorizing new repairs.
Owner then approves R0099 (+6/-0) and R0100 (+7/-4). P14 independently
checks and retains R0102/R0104 under the expanded delegation (+2/-0 each).
Progress: 76/381 retained (54 explicit and twenty-two delegated), zero
restoration decisions, 305 pending;
notices stay separate.
No source changes follow from retaining existing code. Existing evidence below
stays the single detailed inventory. Next manual review is R0101 (+6/-0,
including two separators) and R0103 (+7/-0 including separator). Together
with R0102/R0104 these partition the existing seventeen-line NEXT_INST
addition without dropping or double-counting blanks. Machine run enables a
nonzero finite budget; the Compat consumer decrements it and the existing
executor callback performs the requested outer unwind. The reset bit retains
priority. Pacing already uses a clock-check/sleep/yield loop in Compat; retaining
its mirror call does not claim that the provider is polling-free. Source
evidence, not a fresh dynamic run, supports this review. R0098 was
already restored in S1. R0095-R0097 counts include one separator blank and
exclude the existing exclusion-related #line.
R0092 was already restored in S1. HLT checkpoint, pending clock/control
consumption and event wait are source-checked against pinned XP and Compat
providers; no new dynamic qualification is claimed.
R0080-R0083
were already restored in S1. Approved counts include pinned-XP separator lines. The map accessor
returns a mutable pointer, not a bounded clear operation; its actual consumer
only clears the map. Do not claim this API shape is the sole possible design.
Archive capture/restore callers and original CPU private state are rechecked.
No fresh dynamic run is claimed for approval bookkeeping and no larger block
is approved merely because its include/declaration is covered by delegation.

## S3 purpose re-review

Owner requests a combined purpose review including already approved points.
P15 changes no approvals or production code. Purpose, necessity, and placement
are separate questions: a current caller proves use, not that XP was wrong or
that the present API shape is minimal. Prior approval is not an exemption from
this review. Existing excluded bodies remain out of scope.

All 76 approved IDs receive one primary category below. Mixed motivations are
not erased: R0085 combines selected-host TLS wiring with a pointer-type fix;
R0061 supports lifecycle and snapshot; representation fixes may expose original
undefined behavior without proving a fault in the original XP build.

| Primary purpose | Count | Approved IDs |
| --- | --- | --- |
| Original behavior correction | 15 | R0008, R0015, R0024, R0026-R0035, R0057, R0100 |
| Declaration / host ABI consistency | 23 | R0010, R0019-R0020, R0038-R0050, R0051, R0060, R0062-R0064, R0066-R0067 |
| Product snapshot capability | 13 | R0018, R0021-R0022, R0053-R0055, R0065, R0078-R0079, R0084, R0086, R0091, R0104 |
| Product host debugger capability | 7 | R0076, R0087-R0090, R0095-R0096 |
| Standalone executor / host lifecycle and timing | 16 | R0059, R0061, R0068, R0070-R0075, R0077, R0085, R0093-R0094, R0097, R0099, R0102 |
| Selected source / product boundary | 2 | R0006, R0014 |

Mechanical expansion confirms 76 distinct IDs, no overlap, missing ID or ID
outside the approved table. These are point counts, not diff-line weights.
R0101 remains pending host-event wiring; R0103 remains pending finite-run product
contract wiring. Neither becomes approved by reclassification.

The full existing 381-ID evidence ledger contains the same purposes plus
compiler-only acceptance and mixed video/audio integration responsibilities:

- Original behavior: PIC/permission/width/bounds, DMA, ROM segment, controller
  reset, VGA cursor/dirty/addressing and generated DWORD read corrections.
  Examples R0414/R0476-R0478/R0499/R0503-R0505/R0544-R0545. S2's delivered
  D004-D006 are separate accepted repairs, not extra IDs in the 381 count.
- Host representation: native GDP slots, local storage, pointer carriers,
  little-endian FPU/port layout and matching types. Examples R0254/R0263,
  R0269-R0270, R0554/R0556/R0563-R0564. A modern x64 failure does not prove a
  32-bit XP product defect. Native-width zeroing and local-continuation repairs
  have different reasons even when they share a generated file.
- Snapshot extension: private CPU/TLB/queue/controller state and stable callback
  identity. Examples R0135-R0138/R0172/R0415/R0417/R0442/R0496/R0502/R0543.
  This adds product capability; it does not repair XP's lack of that capability.
  R0489 remains incomplete for the separately declined L001 ICW-phase case.
- Host debugging: operand observation R0115-R0129 and the already classified
  begin/retired hooks. Guest debug-register emulation and host debugger service
  are different consumers; neither should be described as the other's duplicate.
- Standalone integration: outer unwind, executor-owned devices, NT service
  replacement, PCM/MIDI/tone endpoints and teardown/status contracts. Examples
  R0160/R0490-R0493/R0627-R0638/R0640-R0655/R0680-R0681. Audio groups can contain
  parser defects, snapshot extensions and host ownership changes simultaneously;
  do not describe their whole blocks as either one original bug or a thin shim.
- Coupled video contract: CHAIN2 plane layout, BIOS writers, dirty markers and
  painters must agree (R0272/R0340-R0341/R0434/R0510-R0513/R0532/R0535).
  Original-mode bugs and repairs needed to reconcile selected implementations
  require distinct evidence; a consumer change alone does not settle blame.
- Product selection: omitted retired sources, selected CPU/host profile and
  unavailable NT activation (R0006/R0014/R0558-R0559/R0621/R0663).
  This is a supported-product choice, not proof of an original algorithm bug.
- Compiler-only six IDs remain R0439/R0451/R0508/R0592/R0605/R0678; historical
  nineteen notice items are separate policy records, not functional repairs.

Boundary questions retained for the next point review: R0077 exposes a writable
pointer for a clear-only consumer; R0102 invokes an existing clock/sleep/yield
pacing loop; R0103's budget is consumed at an entry boundary, not proved exact
retired-instruction counting; status/lifecycle and snapshot completeness must
be proved separately. None authorizes a new refactor or revokes a prior decision.

Evidence scope: P15 re-reads the full-ledger purpose summaries and groups and
reconciles every approved ID. It does not claim a fresh source-level necessity
audit or new negative/runtime tests for all 381 IDs. The existing detailed
source/test evidence and limits below remain authoritative per point. Group
classification is not blanket retention approval of the 305 pending IDs.

## Coverage and decision rules

The frozen universe remains the [472-file inventory](m9-t100-files.tsv).
File-level auditing and final file-decision reconciliation cover all 472 files.
At S1 delivery, D001-D006 and L001 were unimplemented proposed repairs,
separate from existing residuals. P188 records final accounting; neither file coverage
nor reconciliation approves residuals or closes S1/T100.

An existing residual and a proposed new repair are distinct decisions. A failed
original-body probe proves only the exercised contract; a passing current test
does not approve the remaining patch. Source/ABI evidence is identified as such.
Do not infer approval from delivery of other restorations in the same file.

## Compact owner decision packet

This section and the detailed evidence below preserve the S1 review snapshot;
its admission/closure wording is historical. S3 progress above and CURRENT
own current decisions and task status.

The delivered restoration baseline is P164; P165-P189 reconcile evidence and
decisions without changing production, tests or published executables. All 472
frozen file positions are covered. The task-start-to-current normalized XP
added-plus-deleted comparison decreases by 7328 lines across 123 changed files;
mirror source itself grows by 2201 lines. Restoring original code explains why
those two metrics move in different directions. Full qualification is P164's
fresh x86/x64 392/392 with no retries/skips, including native tests.

### A. Existing functional and ABI differences: recommendation to retain

The 375 semantic/ABI IDs below are existing differences, not 375 newly written
fixes. The following is a navigation summary, not a replacement ID ledger or
permission to retain unlisted changes. Each linked section gives original/local
behavior, actual consumers, evidence and its limits. Evidence can be a concrete
ABI/source requirement or a controlled original/current failure comparison;
neither a missing symbol after partial restoration nor a passing suite alone
establishes necessity.

| Reason for retaining existing differences | Representative decisions and complete evidence sections | Consequence of coherent restoration where proved |
| --- | --- | --- |
| Native pointer widths and generated-rule calling conventions | R0563/R0564, [host representation](#positions-438-452-host-graphicsevent-headers-and-native-pointer-types); [generated interfaces and rules](#positions-271-274-generated-interfaces-restored) through position 318 | x64 pointer truncation, incompatible rule calls or lost nested state; x86 and x64 conclusions are distinguished. |
| Current executor, shutdown and observation boundaries | [c_main](#position-80-c_mainc-execution-integration), [operand observation](#positions-81-97-operand-observation-and-restored-register-implementation), [TLS unwind](#positions-182-193-selected-symbols-and-original-tls-unwind) | Guest execution can bypass queued host work, fail to stop at the admitted boundary, or lose required debug/return information. No new worker or duplicate device owner is proposed. |
| Snapshot access to original private device/CPU state | [TLB](#positions-98-118-private-tlb-state-and-original-exception-paths), [FPU](#positions-265-266-delayed-fpu-exception-state), [UART/LPT](#positions-267-270-uart-lpt-and-source-notices), [device headers](#positions-370-393-device-snapshot-and-native-time-headers), [DMA/CMOS/PIC](#positions-402-408-io-dma-cmos-pic-and-idle-detection), [event/PIT](#positions-409-415-queue-rom-pit-and-original-interrupt-handlers) | Required private state cannot be restored through the remaining original public API alone. L001 explicitly prevents claiming complete mid-ICW PIC restoration. |
| Demonstrated original register/address/permission errors | R0008, R0015, [SAS](#positions-47-63-sas-and-following-ccpu-files), [disk BIOS](#positions-319-325-disk-bios-private-checkpoints-and-empty-media), [DMA](#dma-r0476-r0480), [BIOS video writers](#positions-416-423-original-bios-video-writers-and-virtual-text-height) | Wrong EOI port, access width/permission, address reload or display-memory mapping reappears in the specified source paths/probes. This is not proof that every customer symptom came from them. |
| One coherent video representation and published update transaction | [display addressing](#positions-359-369-display-addressing-and-hostpicio-headers), [geometry/dirty/V7](#positions-424-427-update-geometry-dirty-mapping-and-v7-display-state), [VGA](#positions-428-431-vga-selection-controller-state-and-bios-addressing), [painters](#positions-455-458-sound-declarations-and-cgaega-painters) through position 468 | Address/stride, palette, cursor or produced-frame assertions fail in the named controls. Empty/invalid geometry tests are distinguished from real controller-programmed modes. |
| Standalone host binding and original device ownership | [selected symbols](#positions-182-193-selected-symbols-and-original-tls-unwind), [profile](#positions-432-437-bios-entry-and-host-representationdeclarations), [sound/DSP](#positions-462-463-sound-blaster-owner-and-dsp), [FM/MIDI/mixer](#positions-464-466-fm-midi-and-mixer), [tone](#positions-467-472-tone-vga-and-fixed-firmware) | XP NT-private services and asynchronous ownership are not the supported host contract. Restored independent original helpers remain; Lib endpoints are bindings, not a replacement guest device. |

Two policy-constrained cases should not be mistaken for universal technical
necessity: R0006 omits calls into the explicitly retired optional DOS mouse
driver; R0580 retains existing shared sound declaration placement because the
tested relocation increased XP diff by one, prohibited by the owner's rule.
Neither recommendation authorizes restoring a retired subsystem or changing
the no-increase policy. Original unused files/declarations stay when deleting
them would create new differences (for example bswap.h and cmos.ram).

### B. Existing compiler differences: recommendation to retain separately

Exactly six IDs are compiler/format decisions: R0439 (ica.h), R0451 (tape_io.h)
and R0508 (unexp_nt.c) omit the terminal Ctrl-Z rejected by actual compiler
commands; R0592 removes the extra trace argument comma; R0605 aligns static
forward declarations with static definitions; R0678 undefines the SDK
PlaySound macro conflicting with the original function. These have actual
dual-width compilation evidence, not claimed guest behavior benefits.

### C. Historical notices: preserve, no license decision inferred

The nineteen notice-policy IDs are R0173, R0206, R0238, R0244, R0413, R0416,
R0418, R0419, R0456, R0461, R0462, R0468, R0473, R0487, R0488, R0495, R0501,
R0507 and R0560. They are separate from the 381 functional/compiler IDs.
Technical cleanup does not authorize deleting these historical notices or
asserting redistribution rights. Existing excluded bodies receive no entries.

### D. New repairs: not part of existing-difference retention approval

See [the seven proposed decisions](#proposed-new-repairs-d001-d006-and-l001)
for actual failing controls and limits. The recommended next choices are:

- D005: authorize the isolated two-line PIT backlog correction and permanent
  regression; current first-event protection does not cover an active backlog.
- D006: authorize the one-bit EGA plane-enable correction and mask/path tests.
- L001: authorize design/implementation of PIC initialization-phase snapshot
  state in the existing archive. Include its schema consequence; exact patch
  size is not established, so this is not approval of an unseen final diff.
- D001-D003: keep unimplemented unless the owner admits further repair design.
  Actual generated-body failures exist, but current bulk guest consumers and
  complete minimal fill fixes are not established. Do not activate an unused
  batch path merely to create a consumer.
- D004: investigate hardware interpretation before a repair decision. The
  production-wired selector/fixture discrepancy is proved; the expected
  hardware behavior is not yet fully established.

Approving A/B retains only the documented existing differences; it does not
approve D, erase remaining proof limits, or declare all guest behavior correct.
Rejecting an A/B point requires coherent restoration and appropriate rebuild/
regression before that alternative can be delivered. S1/T100 remain open for
the owner's disposition; no approval is inferred from this packet's push.

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

## Positions 416-423: original BIOS video writers and virtual text height

All eight normalized current files match their final P126 committed versions.
Current ega_write_routines_update still selects BIOS callbacks for modes 0-2;
CPU_40_STYLE SetWritePointers does not replace them. Defining SetBiosWrites
without a production caller does not make those BIOS writers unreachable.
Current EGA reset getter, virtual-height branch and original local provider
declarations also match the final evidence. No new runtime result is claimed.

### R0510-R0513: mode-0 BIOS CHAIN2 address mapping

Four existing changed lines (+4/-4) retain the independently established plane
layout: aligned byte address preserves plane A0; an odd WORD's second byte
advances eight physical bytes, not four. R0510 is copy-byte mapping, R0511
copy-WORD start, R0512 odd-WORD next cell, R0513 generic mode-0 byte mapping.

P120 initializes the actual machine and BIOS mode 3, asserts selected callback
identity, changes GC ports to select generic behavior, invokes the real callback
and reads back through C-VID. Each individual original-line restoration compiles
and links but fails the intended assertion on both architectures: byte expected
11 observed 20; even WORD 22,33 observed 20,07; odd WORD 44,55 observed 44,20;
generic first byte ff observed 20. Current controls also cover odd generic
plane selection. These are BIOS/controller/C-VID integration proofs, not all
V86, bulk fill/move or a newly reproduced Win3.1 session. Retention remains
coupled to the earlier independently evidenced layout R0272/R0362/R0434.

ega_dmmy.c and ega_mode.c have no selected differences. The original zero
switch-setting stub has a real status-port consumer; deleting it would add
unnecessary wiring changes.

### R0514: actual EGA sequencer-reset readout

The existing seven-line getter reads the private reset byte written by the
registered EGA reset receiver. VGA archive capture calls it. Original VGA
register-zero readback returns constant 3 and the VGA reset member is another
object; neither represents pending EGA reset state. Display-disable bits alone
also do not retain the whole byte. Exposing/copying state elsewhere would
expand ownership rather than remove adaptation.
P122 current VGA fixtures write reset values 0..3, capture, alter, restore and
check disable bits on both widths. Fixed-readback failure for 0..2 is source
proof, not a newly executed negative or all-register guest snapshot validation.
The existing getter remains pending, with no added fallback or state owner.

### R0515/R0516/R0518: coherent declaration restorations, no residual

P123 restored three original reader declarations in ega_read.c and removed
their additions from cpu_vid.h, plus original initializer whitespace: nine
XP difference lines removed. P125 restored six local writer declarations and
removed the remaining header block: sixteen more difference lines removed.
Two existing direct tests received private extern declarations (+2 test lines).
The selected IPT macros already erase argument lists; original spellings,
including fill_word IU8, were not modernized.

The initial isolated restoration conflicted with our added header declarations.
Coherent restoration compiled actual provider/consumer objects and retained
original Glue_reads/Glue_writes and unchanged providers. Final EXEs were
metadata-equivalent; rebuilt VGA fixtures passed. This supersedes all of
R0423's old retention conclusion, already removed from the earlier header
section. Neither these restorations nor that superseded claim is counted as
a new residual here. Mirror implicit-int warnings were disclosed, not suppressed.

### R0517: current-VM virtual CRTC height

The thirteen-line VGA/V86 addition uses the existing virtual I/O route for
CRTC VDE/overflow, saves/restores the index and keeps original multiplier/row
rounding plus EGA/non-V86 behavior. Actual BIOS font handlers call recalc_text;
physical height alone cannot describe another VM's virtual CRTC.
P124's actual recalc_text/SAS/BIOS route with controlled nested guest registers
covers 36 native-VGA/V86-VGA/V86-EGA extent/font/multiplier combinations; current
passes and full original-branch restoration fails row count on both widths.
Separately, historical S37 actual Win3.1 VDD snapshot evidence used a probe-
issued font request: virtual 400 versus physical 481 produced 60/30 instead
of 50/25 rows. That is not a new guest run or proof that an unmodified guest
application issued those exact requests. Retention is pending, +13/-0.

### R0519-R0528: mode-1/mode-2 BIOS CHAIN2 writers

Ten existing +1/-1 replacements preserve the same layout, with no new writer:
R0519/R0520 mode-1 byte/WORD start; R0521/R0522 odd-WORD next cell for planes
0/2; R0523/R0524 mode-2 byte/WORD start; R0525/R0526 logical odd-WORD next
cell for planes 0/2; R0527/R0528 simple odd-WORD next cell for planes 0/2.
P126 actual machine/BIOS/ports select and assert callbacks. Independent explicit
cell locations, not a copied offset formula, cover byte 2, even WORD 6, odd
WORD 9, no logic and AND. Twenty individual original-line variants plus two
whole-ten-line restorations compile/link and fail their expected assertions;
current controls pass. The initial undeclared fixture setter was corrected
before these results and is not counted as defect evidence.

D006 is separate: original mode-2 no-logical-operation code tests the wrong
plane enable bit. A mask-4/value-4 offset-zero probe observes plane2=00 on both
widths. The address fixes do not repair it; no unapproved new patch is added.
This does not establish all masks/modes/overlap/paging or guest-session coverage.

Sixteen semantic IDs are added: cumulative 275 qualified residuals (272
semantic, three compiler/format); eighteen notices unchanged. D006 remains a
separate proposed repair. This reconciliation changes no source/tests/EXEs;
P164 fresh 392/392 remains applicable, without approving pending residuals.

## Positions 424-427: update geometry, dirty mapping and V7 display state

All four normalized current files match their final P131 committed versions.
Current guard expressions use real backing spans and dirty-array capacity;
V7 pointer scaling uses display colour state, and mode validation masks only
the argument. P127-P131 final deliveries, including failed/intermediate test
results and later revalidation, are distinguished below.

### R0530: minimized text geometry guard

P127 removed arbitrary 160-column/200-row/32-pixel limits and four copied locals,
restoring original row calculation. The retained guard checks nonzero stride,
full-copy extent 2*start+length, final-visible-row extent
2*start+length-stride+2*columns, and sizeof(dirty)/sizeof(dirty[0]) rows. Both
byte spans must fit actual EGA_PLANE_SIZE, not the selectable display-bank span.
Source pixels occupy four times that byte domain in a four-times allocation.
This is not a public text-size policy or arbitrary-corrupted-snapshot validator.

Real BIOS/CRTC tests prove 161-column output and 400/768 rows work; 769 rows
exceed the actual dirty records. Zero offset plus real update ticks reaches
zero stride; original unguarded division faults on both widths. Two rejected
intermediate guards matter: display-bank capacity permitted 72128 bytes beyond
real video_copy; checking full-copy alone missed 64 bytes at the last visible
row. Enlarged private scratch observed those writes without an uncontrolled
heap overrun. Final both-span checks prevent them. Earlier background passes
on an unsafe candidate were NOT accepted as final-source qualification.
The regression orders state-mutating probes correctly and counts full rows
separately from the legitimate extra old-cursor erase callback.

### R0531-R0535: one original update path, grouped evidence

- R0531 preserves dirty byte residue within four-byte groups through scan,
  paint and clear, including paired V7 rows and the last intersecting group.
  Original-group restoration fails the real route-0 full-mark residue oracle
  on both widths (stride 80, residue 1, address 320).
- R0532 retains one CHAIN2 mapping across text, split/wrap, both CGA banks and
  scrolling. WORD/DWORD pointer steps reflect the same interleaved layout;
  zero-offset calls retain the owner's uniform pattern. Complete group
  restoration through both CGA banks fails the incremental modes 04/05 colour
  matrix on both widths. Earlier partial restoration was superseded.
- R0533 supplies missing fifth paint argument height=1 when erasing the old
  cursor. The actual body with a five-argument recording painter returns 1;
  original four-argument calls expose indeterminate heights and fail both
  widths. Incidental observed garbage values are not stable ABI semantics.
- R0534 marks panning predecessor dependencies including bank wrap; traversal
  avoids recursive propagation of newly added marks, paints wrapped portions
  before clearing shared marks, and uses correct split scanline/host units.
  It uses the existing bitmap and painter. Grouped tests and per-rule source
  checks are not independent negatives for every repeated changed line.
- R0535 selects the existing EGA marker on CHAIN2 graphics mode. Actual BIOS
  mode 3 to V7 mode 63 then c_sas_store(a0100) marks bitmap[40] and low/high=64.
  Original assert-only restoration leaves bitmap clear and invalid bounds
  despite dirty_total incrementing. Both widths fail that assertion. Unlike
  the older fixture's direct marker installation, this verifies mode wiring.

R0529 restored spelling/comments; R0530's minimization reduced XP difference
by 33 lines overall (+9/-34 production, +70 test). No second renderer, retry,
extra allocation or producer state is introduced. The six residuals remain
pending, not approved by a successful regression.

gvi.c has no selected residual. Original initialization and geometry setter
consumers require it; absent direct termination callers do not justify deletion.

### R0536: V7 pointer coordinates follow display interpretation

The single +1/-1 changes draw_v7ptr's second condition from CPU chain4 to
256-colour display, preserving sequential-chain4 first condition and original
halving/drawing. SR4 CPU addressing can change independently of packed display.
P129 actual BIOS modes 66-69, SR4 writes and pointer registers use X=16/Y=6
with expected pixel X=8. Current full fixtures pass; exact XP-condition
restoration fails the first pixel oracle on both widths. This is a real
controller/body test, not a fresh interactive RDP or Win3.1 reproduction and
not failure evidence for every later negative-matrix case.

### R0537: minimal V7 preserve-mode validation

Original raw BL=E7 fails is_v7vga_mode even though the existing later code
supports mode 67 plus preserve bit. The minimized patch leaves original
assignment placement/blank lines and masks only the predicate argument.
Double parentheses protect the original macro's unparenthesized parameter;
getBL is a non-mutating read and the preserve flag remains intact downstream.
Coherent original restoration fails actual different-mode BIOS request on both
widths; a same-mode request alone could hide rejection. Minimal controls pass
mode/preserve/clear/bank/invalid-FF cases. Four XP difference lines were removed;
remaining +1/-1 is pending, with no helper or guest-specific path.

### Exact qualification history, not retroactive passes

P127 final background suites passed but disconnected native cases failed.
After reconnection fourteen native CTest cases passed; Console still failed
initial geometry before broker creation or the changed VGA object. The exact
unchanged Console binaries passed explicit hidden-new-Console launches using
the registered working directories. Thus that batch qualified all required
bodies, not default-launch CTest 392/392 or a repaired Console launcher.
P130 V7 predicate code changed EXEs: full background and Window tests passed,
explicit Console tests passed, five native cases per width were owner-deferred.
P131 connected reruns then passed those ten cases without source changes.
P164 subsequently ran fresh full 392/392 without retry/skip on current artifacts.
These outcomes do not erase earlier observed failures or rejected candidates.

Eight semantic IDs are added: cumulative 283 residuals (280 semantic, three
compiler/format), eighteen notice items unchanged. This reconciliation edits
only task records; code/tests/EXEs are unchanged, and owner decisions stay open.

## Positions 428-431: VGA selection, controller state and BIOS addressing

All four current normalized files match final P134 committed contents. Source
checks confirm packed/planar conditions, four raw V7 snapshot cases with all
other reads delegated to original handlers, cursor disable/reversed guards and
original BIOS implementation. P132-P134 final results qualify the following
existing residuals without adding new patches or new runtime claims.

### R0539-R0541: display units independent of CPU addressing

R0539 retains four-byte packed V7 address units while CPU chain4 is off.
R0540 keeps planar 256-colour per-plane stride independent of CPU odd/even;
R0541 uses the same units for wrapping. Three individual original-condition
restorations and one coherent all-three restoration compile/link, then fail
actual stride/wrap assertions on both architectures. Negative execution stops
at the first failure, not proof of all later matrix members.

Register reachability was separately checked: BIOS mode 13, V7 unlock, FC/SR4/
GC/CRTC writes produce colour=1, chain4=0, sequential_chain4=0, stride 80,
start 49664. Original R0540 yields stride 160 instead of 80; original R0541 yields
wrap 0 even though 49664+16000 exceeds 65536. This supplements direct-state
matrix coverage with original register paths. R0538 removed two explanations
and a local-only V7 wrapper; supported targets already define V7VGA. Six XP
difference lines were removed, leaving three +1/-1 conditions. No non-V7
support or alternate chooser is claimed. Final VGA tests passed and EXEs were
metadata-equivalent, with no new full-suite run in P132.

### R0543: complete copied controller state with original port reuse

The BIOS vga_int_1C service cannot substitute for this archive: its guest-memory
record omits V7/live latches/DAC phase and mutates indices. Existing archive
captures these private fields on the executor and restores through original
port handlers. The reduced getter reuses vga_extn_inb for 24 of 28 V7 registers,
preserving selector index. A5/F3/FE mask raw bits and F9 exposes a subfield, so
four direct raw reads remain; side-effecting F0/F2 are not selected.
Private actual-body fixtures check all 28 registers at every byte value (7,168
assertions/run) and preserved indices, then run the full VGA fixture on both
widths. This is not an exhaustive guest snapshot or malformed-archive proof.

R0542 removed duplicate getters and five dead initialization writes: vga_init
already sets DAC mask/bits, phase/address values have no intervening reader,
and the temporary selector is overwritten before use. Required protection and
zero-state/chain initialization remain because change-only original handlers
otherwise preserve unrelated derived state; independent removal probes fail
set-reset, character-height and double-pixel assertions. Snapshot schema is
unchanged. This is reduced same-owner serialization, not a Compat substitute.

### R0544-R0548: cursor, underline and panning

R0544 retains VGA disabled/reversed/out-of-font hiding, zero-end correctness
and inclusive normal end. R0542 restored original equal/reversed blocks and
bounds/assertion spelling: reversed remains unreachable behind hiding, and
equal range correctly produces one scanline. Current 8/14/16/32-height fixtures
pass; coherent original shape/register restoration fails actual visibility.
R0545 stores the entire cursor-start byte, so a disable-only write is not lost
by comparing only start bits then reading the old disable flag. Its separate
original-register negative fails on both widths.
R0546 requests existing refresh on underline-only writes; original restoration
fails the produced-frame assertion. R0547 provides line-specific panning,
zero below the split, to original host painters and dirty updater. The isolated
raw-only helper negative fails pixels; it is expressly not coherent restoration
of every coupled painter change. R0548 refreshes split/horizontal-pan register
changes even with unchanged VRAM; separate original negatives fail dirty tests.
No second drawing or cursor-clock path is introduced.

P133 reduced selected XP difference by 68 lines (production +35/-57, tests 0).
Actual final VGA fixtures and 188 background tests per width passed. One private
run collided on scratch media; it was discarded and rerun serially after the
other process exited. Native tests were explicitly deferred then, not passed;
P164 later supplies the fresh full 392/392 current-source baseline.

### R0550/R0551: active BIOS caller text aperture and CGA pixels

vga_vide.c has no selected residual and retains the original BIOS state service.
video.c remains the original BIOS implementation. R0550 derives nine text
origins and two clear bounds from the active caller's BDA table, not physical
mode cache. Coherent original restoration of all eleven uses and removal of
the macro fails the actual B8000 character oracle on both widths; clearing
bounds are tested consistently. Historical S33 Win3.1 VM=1 evidence observed
BDA mode 3 with physical A0000 and restored typing after selecting the BDA
aperture. It is not a new interactive run here. The current focused stale-cache
fixture is not a full paged-V86 model; original graphics callers stay unchanged.
R0551 preserves odd/even plane spacing for both XOR and replacement CGA pixel
writes. Coherently restoring both expressions fails BIOS write/read-dot tests
on both widths (modes 4..6, x 0..19, y 0..2). There is no separate XOR-only negative
or claim covering every coordinate/V86 graphics combination.
R0549 restores blanks/comments and verified diagnostic anchors, reducing XP
difference by 28 lines (source +17/-23, tests 0). Final VGA tests pass and whole
EXEs match P133 except precisely identified metadata, so no redundant full
runtime test was claimed. All coupled source identity checks use final builds.

Eleven semantic IDs are added: cumulative 294 residuals (291 semantic, three
compiler/format), eighteen notices. This reconciliation changes only task
records; owner choices and original-defect decisions remain open.

## Positions 432-437: BIOS entry and host representation/declarations

All six normalized current files match their final P139 committed versions.
Current headers confirm original vector declarations, little-endian FPU/word
layout and absence of the removed host_def additions/profile definitions.
P135-P139 final-source evidence is reconciled without rerunning tests.

### Completed restorations and original required headers

R0552 restores video_io.c blank placement and diagnostic anchors, removing six
XP difference lines (production +5/-5, tests 0). The real BIOS table retains
video_io, distinct from EGA dispatch; guard, idle notification and dispatch
are original. Final BOP/VGA fixtures pass both widths and EXEs were equivalent
except exact build metadata. No selected residual remains.
Host error.h selected text and host.h whole normalized contents are original.
Actual rom_basc dependency records contain both; original host_error binding is
not a duplicate of Compat's distinct Base-error declaration overlay. No file
retirement or new include-removal difference is justified.

R0553 removes one redundant FPU_I64 comment while retaining the required ABI
reason marker. R0555 restores the complete original common-WRITE vector block,
removing the unsupported DIV-102 rationale and obsolete line anchor. Actual
cpu4gen.h selects typed direct CCPU setters; setAX_func/setCS_func have no vector
storage/assignment consumer. Extra typing did not govern those calls. This
restoration adds 16 source lines but removes 47 XP difference lines; it creates
no second implemented dispatch path. Final BOP/VGA fixtures pass; EXEs differ
only in metadata. A regex selecting no device tests was not counted as coverage.

R0557 removes redundant stdlib.h and host_simulate declarations/rationales and
restores original directive comments: 24 XP difference lines removed (source
+4/-20, tests 0). Actual supported Windows header ordering supplies standard
prototypes before calls; original CPU/host headers supply host_simulate routing.
Eleven real consumers across both architectures and four private header variants
produce 176 successful preprocess/syntax commands; 22 combined comparisons add
no warnings/errors. This concerns current toolchains, not every future SDK or
port. Final VGA/BOP/checkpoint/x87 tests pass four per width; whole EXEs are
metadata-equivalent. Existing original-mirror warnings were disclosed.

### R0554: six actual little-endian FPU representations

FP32, FP64HI and FP80SE bitfields, FP64 words, FPU_I64 words and FP80 mantissa
words are read directly by original conversion code. Byte-order macros cannot
repair wrong struct offsets. CVTI64FPH computes high*2^32+low and original BCD
tables initialize low/high; reversing {1,0}/{10,0} changes those values to
2^32/10*2^32. Compat's header wrapper provides fenv bindings, not alternate FPU
arithmetic; original FPSTACKENTRY and snapshot register access stay intact.

P136 compiles current, six individually restored XP layouts and all-six
restoration with the actual wrapper on both widths. Current passes and each
restoration fails its corresponding offset/representation assertion; coherent
original first fails FP64. Source consumers establish why the layout matters.
These are representation/ABI proofs plus native binding tests, not exhaustive
x87 opcodes, rounding/exception combinations, guest arithmetic or snapshots.
The six layouts form one existing residual decision, not six newly added APIs.

### R0556: low/high port bytes

reg.byte.low must precede high on these hosts. Original generic_outw emits low
to the first port; VGA vga_gc_outw interprets it as index and high as data.
Actual-flags scalar probe sets 0x0104: current low=4/high=1, original layout
low=1/high=4 on both widths. Recompiled real VGA objects against the original
layout also access-violate in the existing fixture. The scalar assertion and
source chain prove the exact swap; the broader fault is not a stack-traced
first instruction or a real guest reproduction. Original nibble/bit members
remain unchanged. This residual is separate from the restored unused vectors.

### R0558/R0559: selected host and CPU profiles

Omission of original NTVDM and CPU_30_STYLE defines remains two pending
configuration decisions. Actual targets select independent CCPU/Compat and
CPU_40_STYLE. Individually restoring either definition in its original header
position fails real ccpusas4 compilation on both widths at existing unsupported
combination guards. This proves conflict with the admitted subset, not a defect
in original XP NTVDM/CPU30 or a reason to reopen excluded bodies. Reusing another
profile to silence the conflict is not a semantic-free restoration.

Four semantic/ABI configuration IDs are added: cumulative 298 residual IDs
(295 semantic, three compiler/format), eighteen notices unchanged. No source,
tests or EXEs change in this reconciliation. P164 later supplied current full
392/392; earlier deferred native checks are not retroactively claimed as passes.
All owner decisions remain open.

## Positions 438-452: host graphics/event headers and native pointer types

All fifteen current normalized files match their final P143 committed versions.
Current whole-tree searches find the restored menu strings, attached-window and
fullscreen symbols only at their original declarations. CMake/source checks
confirm the source-specific C-VID word override and ordinary CCPU IUH remaining
unsigned int. P140-P143 final evidence is reconciled below.

### Original files and delivered restoration

host_gen.h, host_gfx.h and host_qev.h are normalized-XP-identical; the original
quick-event handle is LONGLONG, not a newly invented widening. host_hfx.h has
no selected runtime definition, but original include wiring remains required
under the no-new-diff rule. Actual dependency observations establish includes,
not a claim that every historical declaration has runtime consumers.

R0561 restores host_inc.h's original blank and diagnostic anchor. R0562
restores wchar_t wszHideMouseMenuStr/wszDisplayMouseMenuStr declarations.
Neither original nor replacement names have definition/call/assignment consumers;
no font/encoding capability depended on the local adaptation. Combined XP
difference reduction is five lines, production +4/-3, no test change. P140
VGA/BOP/quick-time tests passed three per width; EXEs were metadata-equivalent.

ntcheese.h, nt_cgalt.h, nt_det.h, nt_ega.h and nt_egalt.h are wholly normalized
XP matches; nt_cga.h selected text matches. Original host graphics includes
remain. R0566 restores exactly extern PointerAttachedWindowed and removes its
three-line modern-C rationale. No provider/consumer exists in the retained
repository; actual GNU90 mirror compilation accepts it. Four real nt_graph
syntax controls pass without extra implicit-int diagnostics. Five XP diff
lines are removed (source +1/-4), without a runtime/layout change. P142 final
VGA tests passed and EXEs were metadata-equivalent.

R0567/R0568 restore original SwitchToFullScreen(BOOL), the terminal blank and
proper existing line anchors in nt_fulsc.h. No implementation/call was found;
this does not enable NTVDM fullscreen or alter KVM. Two missing XP lines are
restored (source +3/-2). Final VGA tests passed, and complete EXEs differ only
in identified build metadata. No fullscreen interaction test is claimed.

### R0563: IHPE carries native pointers

The original comment requires an integer the size of IHP, but XP's unsigned
int is only four bytes on the supported x64 host. The existing UINT_PTR plus
reason is +3/-1. Actual sevid000 global function-address initializers fail x64
compilation under original IHPE; current compiles, x86 original also compiles.
This is a host ABI requirement, not a defect claim about a 32-bit XP product.

### R0564: native word only for generated C-VID rules

Ordinary CCPU IUH remains unsigned int. The existing +7-line CVIDC_RULE_WORD
selection is scoped by x64 CMake to sinit/sevid, j_c_lang, ev_glue and vglfunc.
Those real rule registers, locals and returns carry pointers. This is not
universal guest-word widening, a second rule engine or a new definition added
by this audit.

P141 uses real consumer flags with current, each original type and both restored
coherently. All x86 builds/heap roundtrips pass with four-byte types. On x64,
current eight-byte carriers roundtrip real heap addresses; original IHPE,
original IUH and both truncate their respective carriers. IHPE-only/both also
fail actual sevid000 compilation; IUH-only compiles but loses the pointer.
Thus compile success alone is insufficient. Scalar roundtrip and consumer
proofs do not imply exhaustive video/guest/snapshot coverage.

R0565 removes one extra blank, not either justified carrier. Final VGA and
checkpoint tests passed on both widths; CHAIN2 assertions are inside VGA, not
a separately registered third test. Both EXEs were metadata-equivalent. Current
full runtime qualification is the later P164 fresh 392/392, not an invented
native pass at P141. R0563/R0564 remain owner decisions.

R0560 is one historical notice-policy item; notice total becomes nineteen.
Two ABI IDs are added: cumulative 300 residuals (297 semantic/ABI, three
compiler/format). No excluded body is counted. This reconciliation changes
only task documents, with code/tests/EXEs unchanged and S1/T100 still open.

## Positions 453-454: graphics contracts and original mouse header

Both current normalized headers match P145. nt_mouse.h remains a complete
normalized XP match with its original include wiring; unused declarations do
not justify additional removal differences.

R0569/R0570/R0572/R0574 are delivered restorations: original blank adjacency,
line anchor, removal of redundant explanation and restoration of the original
host_stream_io_enabled declaration. Together P144 reduced XP differences by
five lines (production +3/-5). P144's historical observation of no definition
is not the current state: P150/R0604 subsequently restored the original global
in nt_graph.c. Current whole-tree search finds that declaration and definition,
but no reader. Neither restoration enables the separate guest stream backend.

R0571/R0573 remain two qualified owner decisions: the seventh per-scanline pan
argument to ega_colour_hi_munge and the common ega_colour_panned_group
declaration (+2/-1 selected header difference). Actual low, medium and high
painters consume these contracts; nt_munge combines corresponding plane bytes
and wraps the next group at the existing display-bank extent. Mere caller or
prototype agreement is not the necessity proof.

P80's actual port-to-pixel control makes the existing query return zero in a
private real-provider object and links the actual VGA fixture. Both widths
fail the independently expected pixel shift after 3C0h pan=1, while unchanged
fixtures pass. This is an isolated no-panning counterexample, not coherent
restoration of every implementation difference or a fresh guest reproduction.
The current fixture also checks all three painters, shifts 0..7, bank/wrap
and split cancellation; later implementation points retain their own review.
P144 reran actual VGA tests on both widths; both EXEs were metadata-equivalent.
The later P164 fresh 392/392 remains the current full runtime qualification.

Two semantic IDs are added: cumulative 302 residuals (299 semantic/ABI and
three compiler/format), nineteen historical notices. R0571/R0573 remain
unapproved. This reconciliation changes no source, test or artifact.

## Positions 455-458: sound declarations and CGA/EGA painters

All four normalized current files match P149. Final dispositions supersede the
private/incomplete P146 experiments; no rejected declaration migration remains.

### Sound and user-interface headers

R0575/R0576 restore original MM declarations, unused prototypes, WORD port
storage, initialization-time BasePort assignment and owning BOOL declarations
coherently. The test's conflicting redundant declaration is removed. R0579
restores VDD_DMA_INFO/PVDD_DMA_INFO and provider types, with original WORD
addr/count/page and BYTE status/mode/mask; no SB_DMA_INFO alias survives.
P146 reduces total coupled XP difference by 346 lines (header 321, nt_sb 3,
nt_sbdsp 22), despite production +347/-75; tests +12/-12. Restored declarations
do not recreate native workers or require unused providers.

Four residual IDs remain pending:

- R0577: BufferMidi returns lib_status to the actual VsbByteOut owner. Parser
  and host-failure tests observe rejection, IO_ERROR and retained-message
  service; original VOID cannot convey these supported outcomes. This is
  current-contract proof, not an original NTVDM failure claim.
- R0578: service/status, suspension, completion and mixer declarations connect
  the sole executor and original device owners. Current platform.c checks
  SbService/SbGetStatus and calls SbSetRunning; the redundant PCM include was
  removed. Declaration need does not approve all provider implementation lines.
- R0579: local placement of the original DMA SDK record remains necessary for
  the supported SDKs lacking vddsvc.h and the actual controller query/update
  contract. The original type name/layout are already restored.
- R0580: existing shared DSP_MODE and internal helper declaration placement is
  a constrained pending choice. The coherent relocation compiled and produced
  equivalent binaries but increased total XP difference by one, so it was
  reversed under the owner's rule. This is not architectural necessity proof.
  Later DSP review still retains this header-owned type; no test enum copy remains.

P146 final focused sound tests pass 8/8 per width, with strict artifact
equivalence to its candidate qualified by 188 background and four related
native tests per width. Its then-deferred Lib desktop tests are not claimed
as P146 passes. P147 subsequently passes the whole 392-test suite.

R0581-R0585 restore nt_uis.h's original unused declarations/resource constants
and spacing; its selected text now matches XP, reducing ten XP difference
lines to zero (production +10/-2). No keyboard conversion changes or resource
service is enabled by restoring unused names.

### CGA producer and text surface

R0588 restores spacing/line positions; unnecessary explanations in other groups
are removed. P148 reduces XP difference 95 to 74 (production +12/-25, tests
+10/-3). Seven residual IDs remain pending:

- R0586: BIGWIN undef selects the supported subset; removal fails both actual
  compiler commands. No excluded implementation is reopened or counted.
- R0587/R0591: host allocation declaration and actual text-surface preparation.
  Without preparation, a real nt_text call before fixture allocation fails
  queried 100x5 dimensions on both widths.
- R0589: CHAIN2 pointer, gather and source/destination strides. Coherent original
  pointer plus both original copy branches fail byte comparisons on both widths;
  current tests cover 40/80/100/132 columns, padding and partial/full updates.
- R0590: empty/null/nonpositive guard. Removing it makes an empty transition
  replace the existing surface, failing dimensions on both widths. The old
  bulk-copy-underflow rationale is removed. Null/negative cases have source
  safety arguments, not separately injected runtime proof.
- R0592: removed trace comma is a compiler/format decision: original invocation
  supplies seven arguments to the selected six-argument macro on both compilers.
- R0593: coherent original CGA pointer/3-1 increment/XOR-2 restoration fails
  mode 04/05 incremental/full pixel comparisons on both widths. Current mapping
  uses 7-1/XOR-6; this is not a second painter.

### EGA standard painters

R0594/R0601 remove four nonsemantic lines, reducing XP difference 49 to 45.
Six IDs remain pending: R0595/R0597/R0599 guard each low/medium/high painter's
actual surface/plane spans; R0596/R0598/R0600 connect per-scanline panning.
Each original guard produces access violation in the actual fixture on both
widths. Adding only nonpositive protection still fails low/medium destination
boundary assertions and high-mode publication after real V7 65h BIOS setup.
No-pan low uses the original word load; medium/high pass zero through the
existing signature, avoiding an artificial declaration mismatch. Each fails
independently expected pixels on both widths. Boundary tests do not establish
that every invalid input is emitted by a guest.

An initial scratch-file collision is invalid renderer evidence; twenty serial
runs in separate directories supersede it. P148's extra-character package
failures passed isolated reruns; P149's wrong manifest-column update was fixed
and its gate rerun. Neither batch is labeled clean-first-pass. Later P164
fresh 392/392 supplies current full qualification without retries or skips.

These four files add seventeen residual IDs: cumulative 319 (315 semantic/ABI,
four compiler/format), nineteen notices. All remain owner choices. This
reconciliation changes only documents, not source, tests or artifacts.

## Positions 459-461: host graphics, keycode and plane conversion

All three normalized current files match P152. Current source/caller checks
confirm the final dispositions below; earlier incomplete experiments are not
used as final-source results.

### Host graphics

R0604/R0616 restore the original unused stream global/function. R0606 restores
resetWindowParams rather than retaining extra flush/palette/mode/cursor resets;
R0612 restores the soft_reset startup guard. Separate original restorations pass
actual VGA and snapshot transaction/cross-process fixtures. R0607/R0610/R0615
restore fullscreen checks: production sc stays statically WINDOWED, and the
fixture's invented FULLSCREEN state was not proof of a product requirement.
Current fixture asserts the actual WINDOWED state. R0608 restores spacing and
source positions. P150 removes 70 XP difference lines (production +35/-41,
tests +34/-14), not merely six source lines.

Seven residual IDs remain pending:

- R0602/R0609: DIB include and paired update calls. Restoring both original
  update bodies loses actual produced-frame evidence on both widths. Existing
  host update depth and V7 pointer clear/draw form one rendering transaction.
- R0603/R0614: the single DAC_data_bits declaration and packed/planar conversion.
  Real C1/3C8/3C9 writes distinguish 6-bit scaling from 8-bit components; original
  unconditional multiply-by-four fails the 8-bit assertion on both widths.
  The previously passing fixture missed this case; both branches are now covered.
- R0605: five static forward declarations match the original static definitions.
  Original external declarations fail actual compilation on both architectures;
  this is a compiler/linkage decision, not new exported functionality.
- R0611: painter selection independent of transient memory-access CHAIN4;
  original conjunction fails controller-programmed expected geometry on both.
- R0613: selected packed painter's source-byte width; original calculation fails
  actual 640x400 expected output on both widths.

P150's substantive restored candidate passes 392/392. Final line-position,
blank and declaration-wrapper adjustments are rebuilt and pass VGA, snapshot
transaction/cross-process and mirror checks, four per width, with strict EXE
metadata equivalence to that fully qualified candidate. This is automated host
and snapshot evidence, not a fresh manual Win3.1/Win95 session.

### Keycode conversion

R0618 restores all original BIOS conversion tables; R0620 restores blank lines.
P151 production +128/-5 removes 127 XP difference lines, tests unchanged.
Actual machine/input.c still masks scan codes to eight bits and calls the
original KeyMsgToKeyCode tables; this mask does not bound their 133/84 entries.

Three residual IDs remain pending:

- R0617: three unavailable NT headers omitted. Coherent original includes fail
  at nt.h on both supported toolchains; this is a host dependency boundary.
- R0619: two >= table guards plus their local explanation (five diff lines).
  Actual function calls with regular index 133 and extended index 84 return
  zero currently; each original > variant traps separately on both widths
  under compiler bounds instrumentation. These are synthetic boundary tests,
  not recorded physical keyboard incidents. The failed libubsan-link attempt
  supplies no evidence; the trap-instrumented runs do not need that runtime.
- R0621: omitted original BiosKeyToInputRecord. Full function and tables restored
  together compile but fail actual fixture linkage at RtlOemToUnicodeN and
  NT_SUCCESS on both widths. No current production caller or Compat duplicate
  exists. This does not establish an original algorithm defect or justify a
  second converter/native import path.

P151 complete suites pass 392/392; final blank restoration is rebuilt and
passes keycode, runtime continuation and both mirror gates on each width.
Final EXEs are strictly metadata-equivalent to the qualified candidate.

### Plane-to-pixel panning

R0622-R0626 are five IDs belonging to one coherent capability, not five
independent renderers. Current nt_munge.c differs +25/-1 across six ranges;
P152 made no source/test change. The includes provide plane storage/bank
extent, the shared reader combines corresponding bytes of adjacent groups,
the per-scanline argument comes from existing register state, and a local word
feeds the original LUT/masking branches without mutating guest VRAM. Zero pan
returns the original word. No duplicate Compat converter was found.

Unlike an isolated zero-pan query experiment, P152 restores the complete exact
original nt_munge.c, original six-argument declaration and all selected nt_ega
consumers together. The real Compat header adapter remains intact. Five
independent matrices per width exercise low, medium/high mask 0xf, and
medium/high mask 0x5, with pan 0-7, both banks, wrap, split and two rows.
All ten current runs pass; all ten original runs fail independently shifted
pixel assertions. Four include-removal compilations establish the direct data
dependencies. An initial adapter-bypass compile failure is discarded as harness
error. Neither argument mismatch nor that failed harness counts as necessity.
These tests do not cover every mask, video mode or native scaling path.

Fifteen residual IDs are added: cumulative 334 (329 semantic/ABI, five
compiler/format), nineteen notices. All remain pending owner decisions.
P164's later fresh 392/392 remains current runtime qualification. This batch
changes only task records; no source, test, executable or user asset changes.

## Positions 462-463: Sound Blaster owner and DSP

Both normalized current files match P154. Current status, archive, executor,
physical DMA, pause and EOI paths were inspected against final P153/P154
records. The large earlier rewrite is not accepted merely because tests pass;
restored independent original code and retained ownership changes are separate.

### Device owner

R0628/R0639 restore MM globals, original types/linkage/defaults, declarations,
InitializeIoAddresses, LoadWinmm, DeInstallIoHook, diagnostics, port-switch
structure and debug tail. Restored unused helpers are not newly called;
alternative port case labels do not register those ports. P153 production
+415/-37 removes 412 XP difference lines (940 to 528); tests unchanged.

Eleven residual IDs remain pending:

- R0627/R0634/R0638: unavailable NT/VDM headers and coupled original device
  initialization/shutdown/registration/reconfiguration owner omitted. Actual
  original code fails both compilers on VDM_PM_CLI_DATA and PM_CLI control;
  original ConnectPorts also requires a boolean where actual registration is
  void. No second kernel-control/output/configuration path is introduced.
- R0629: archive preparation/restoration and MPU mode/unread ACK. Actual-port
  save/reset/restore/resume probe fails both widths without MPU assignments;
  invalid saved mode is rejected. Not a proof of every archive field/path.
- R0630: status aggregation and distinction between ordinary INVALID_STATE and
  actual CCPU exit cancellation. Removing error storage or that distinction
  fails separate current-contract assertions on both widths. The cancellation
  probe explicitly enters/leaves CCPU; its earlier missing context was a harness
  error, as was the first snapshot probe's missing resume handoff.
- R0631: existing mixer/service/running handoff. Deleting it loses the expected
  33-message resume restoration on both widths; no extra worker/timer is added.
- R0632: port admission and new-lifetime initialization. Withdrawal overwrites
  foreign port ownership or retains old TimeConstant, failing the actual tests.
  Optional unsupported MIDI remains absent, not a falsely present device.
- R0633: status-returning termination consumed by machine reset/destruction.
  Close failure preserves machine/RAM ownership; repeat termination and port
  removal are exercised. The original kernel shutdown is not this lifecycle.
- R0635: lazy native InitDevices/report calls omitted, while original handler
  protocol/diagnostics/defaults are restored. Dependencies are the already
  established host boundary; excluded diagnostic bodies are not inventoried.
- R0636: parser/status/output-owned MPU reset. Removing parser reset breaks
  subsequent note restoration. Original native Close/OpenMidiDevice would
  create a different owner; intelligent-mode no-op behavior remains original.
- R0637: parser, MPU mode/ACK and status reset precede original device reset.
  Exact original ResetAll fails machine reset after injected MIDI failure on
  both widths; original reset order/comments/diagnostics remain restored.

P153's before/candidate device/archive controls pass sixteen runs. Six separate
withdrawals fail their named assertions on both widths. Full final qualification
passes 392/392; native coverage is not deferred. These are controlled host/device
fixtures, not a reproduced failure of the original NTVDM product.

### DSP transfer and host ownership

R0649/R0656 restore original independent functions, scalar spelling, descriptions,
no-op cases, placement and formatting. P154 production +1257/-341 (net +916)
removes 1628 XP difference lines. Tests add two lines for active-DMA pause.
The final brace-aware review verifies 21 complete original function bodies,
not merely their prototypes. Fifteen residual IDs remain pending:

- R0640/R0648: actual SAS/EOI/event/archive/PCM declarations and controller
  preparation replace unavailable NTVDM/native-ring ownership. Available original
  includes and independent native helpers are restored. Sim32 host pointers
  through ULONG are not x64-safe guest physical addresses.
- R0641/R0642: one complete 65536-byte DMA block and sole-executor transfer/EOI
  state, rather than original WaveOut ring plus worker. Runtime pause/capture/
  teardown quiesce this executor; they do not quiesce an added native worker.
  Whole-original backend ownership was analyzed in source, not dynamically
  rebuilt as a second backend. Current callers alone do not prove necessity.
- R0643/R0644: Lib format/gain/running ownership and cancellable q-event completion
  track accepted samples, consumption and epoch. Original event waits in its
  own worker cannot be inserted into the sole executor without preventing guest
  progress/pause service. Running-handoff withdrawal fails paused output on both;
  current tests cover deadline, cancellation, lag, progress error and epoch
  mismatch, not real-hardware timing equivalence.
- R0645/R0651: physical DMA, wrap/decrement and accepted-prefix submission.
  Increment-only withdrawals fail small-block/table-munge data; logical access
  fails nonidentity-paging ADC; requested-count advance fails backpressure.
  Original virtual-increment assignment/diagnostics and ULONG declaration return.
- R0646/R0650: original parser names/values restored; final unsupported UART state
  consumes payload without treating it as PCM commands. MPU remains the supported
  MIDI route. Partial command/table state is file-owned for archive access;
  original function-static state cannot satisfy that capture contract.
- R0647: copied PCM/parser/DMA and consumed-prefix archive. The negative removes
  both pause and restore assignments, not restore alone; it fails capture on
  both widths. Native handles/threads cannot represent a restored host lifetime.
- R0652: executor reset/clear plus new-lifetime ResetState clear. Removing the
  latter completes an old lifetime's partial reset in the new one and fails real
  port tests on both. This does not prove every reset race.
- R0653: original PauseDMA position query omitted; ContinueDMA and independent
  helpers are restored. LoadWinmm has no production caller, so GetPositionProc
  is null. The earlier allegedly safe restoration changed a prototype and is
  invalid evidence. Corrected active Single-DMA function-body restoration crashes
  both widths with 0xc0000005; current passes. No such crash was published.
- R0654: active-transfer position rather than native HWaveOut/worker readiness,
  with decrement/wrap. Original finite yields/diagnostics return; DspVirtualIncx
  controls those yields, not guest DMA address arithmetic or snapshot layout.
- R0655: only archive-visible EOI registration flag clear remains. Original
  unconditional RemoveEOIHook is restored: it already checks IRQ/callback
  ownership. The rejected outer guard was redundant, not necessary recovery.

Final P154 builds and all 392 tests pass after the guard/line corrections.
The two-line active-DMA test addition then passes its affected target on both
widths, without changing either fully qualified EXE. No hypothetical original
backend execution or broad guest/audio guarantee is inferred.

Twenty-six IDs are added: cumulative 360 (355 semantic/ABI, five compiler/
format), nineteen notices. P164's later fresh 392/392 remains the current full
qualification. This document-only reconciliation approves no residual and
changes no source, test, binary, configuration or media.

## Positions 464-466: FM, MIDI and mixer

All three normalized current files match P157. Actual FM register transitions,
MIDI caller/activation boundaries and mixer arithmetic were rechecked against
P155-P157 final evidence. No restored inactive native helper is newly activated.

### FM register behavior

R0658-R0661/R0664 restore original globals/types, ResetFM, register/read/write
structure, independent helpers and formatting. In particular original AD_MASK
scope returns: note registers B0h-BDh must not update timer flags. The old
simplification and its agreeing test expectation were wrong, not required
behavior. P155 production +171/-26 and tests +4/-1 reduce XP diff by 189 lines.

Three IDs remain pending: R0657 omits unavailable NT headers while retaining
archive declarations; R0662 archives selected register and Status; R0663 omits
native open/close requiring NtVdmControl/VDM_ADLIB_DATA. Whole-original and
coherent open/close compilations fail both toolchains on those dependencies;
that does not justify omitting independent original no-hardware behavior.
FMActive remains false, so original batching/flush has no host write side effect.
No new FM backend or OPL sound capability is claimed.

On each width old code/old expectation passes, old code/original-scope expectation
fails, and restored code/corrected expectation passes. The independent transition
table covers 524288 register/data/status transitions, reads and archive roundtrip.
Withdrawing either archived selector or Status separately fails both widths.
Final P155 builds/full suites pass 392/392; this is not acoustic evidence.

### MIDI parser and output

R0666 restores six complete independent original helpers; original declarations,
constants, types, scalar spelling and matching parser statements are restored
within other groups. P156 production +438/-37, tests +16/-0 reduce XP diff by
407 lines. Inactive native pointer objects in isolated fixtures are link supplies,
not a second synthesizer or production output path.

Seven residual IDs remain pending:

- R0665: bounded copied messages/pending state instead of original ring pointer
  storage. An actual original-body probe at confirmed x64 address 0x120000000
  crashes through ULONG arithmetic. This does not justify every replacement field.
- R0667: omitted native InitializeMidi activation. Original ring at low address
  on both widths retains the ready tail: complete 90 3C 64 and explicit drain
  send nothing; next message releases only its predecessor. A coherent private
  pointer/tail-corrected ring still fails real Lib validators for materialized
  running status, SysEx interrupted by F8, and F4. This is not a proposed patch
  or proof that no further redesign could reuse original code.
- R0668: parser prefix/pending/voice archive and validation. Prefix withdrawal
  fails actual roundtrip both widths; invalid archives are checked before mutation.
- R0669: approximate voice continuation and pause reset. Note-tracking and
  host-reset withdrawals fail their independent assertions on both widths;
  sample-exact envelopes/polyphony are not promised.
- R0670: parser-only reset reached from current MPU initialization/reset. Original
  native device/ring initialization is not an equivalent reset-only operation.
- R0671: status-returning complete-message send includes final message and keeps
  failed output pending. Error-propagation withdrawal fails host-error assertions
  on both widths; original description/layout are restored.
- R0672: real-time, partial-message, SysEx and materialized-status contract.
  Status-materialization and oversize-tracking withdrawals fail both widths;
  original matching length calculation/control structure/store statements return.

Six controlled withdrawals per width compile/link then fail intended assertions.
A separate original SetMidiOutVolume probe finds inverted error handling: success
makes no set call, error makes one. This remains an unpatched inactive-original
finding with its diff decision, not TODO or evidence that active gain is broken.
Native helper internal calls do not constitute a production activation caller.
The first incomplete native fixture link is a build setup failure, not a host
MIDI outage. Final larger candidate passes all 392 tests. Twelve x64 warnings
in restored inactive original helpers remain under the mirror exemption; these
helpers are not qualified for an unreviewed future x64 activation.

### Mixer

R0673 restores original declarations/types. Every original XP line is present
in original order; only 48 additions remain (zero deletions), reducing XP diff
by 84 lines. Production +69/-23, tests +5/-0. Three IDs remain pending:
R0674 selector/register archive, R0675 three host-apply calls, R0676 composed
gain. Independent withdrawal of each contract fails both-width assertions.
Original native handle branches are restored but inactive; registers remain the
only mixer authority and Compat has no duplicate mixer.

The R0676 zero guard was not necessary: its withdrawal unexpectedly passed,
then independent gain tables with 64-bit expectations verified every raw-byte
pair across both implementations/widths (524288 assertions). IU32 wrap and final
IU16 conversion already yield zero for supported decoded factors; positive
products fit IU32. The branch is deleted, not submitted for retention. This
identity is limited to present gain ranges/types, not arbitrary future math.
Original native callbacks show zero writes yield FFFEFFFF and master/voice
writes overwrite one another; these are call values, not acoustic calibration.
Final P157 builds/full suites pass 392/392.

Thirteen IDs are added: cumulative 373 (368 semantic/ABI, five compiler/format),
nineteen notices. All residuals remain owner decisions. P164's later fresh
392/392 remains current runtime evidence; this batch changes only documents.

## Positions 467-472: tone, VGA and fixed firmware

The two normalized source files and four byte-exact resources still match P159.
Current tone calls, painter bounds and resource wiring were checked separately
from this source identity. Existing excluded bodies receive no accounting.

### Tone endpoint

R0679 restores original lock declarations, inactive Beep handle/count and native
cleanup, removing unnecessary includes/prose. Actual disposable event-handle
probes confirm InitSound(FALSE) closes/clears, and countdown closes only at zero,
on both widths. Production does not open the handle or initialize the countdown;
this is cleanup proof, not activation of a second output path. P158 production
+23/-11 reduces XP difference by 30 lines; permanent tests unchanged.

Four residual IDs remain pending:

- R0677: unavailable NT-private headers. Full original source fails at nt.h on
  both toolchains; that does not qualify every changed runtime statement.
- R0678: PlaySound undef prevents the SDK macro from conflicting with the original
  one-argument function. Withdrawal fails both compilers; compiler/name decision.
- R0680: existing empty host_ica_lock/unlock adapters match selected ICA and the
  sole executor owner. Removing them yields unresolved symbols on both widths;
  the PCM worker has separate request synchronization. No general thread-safety
  guarantee follows for future callers.
- R0681: tone submission endpoint instead of NT session/device ownership.
  Original producer emits INFINITE duration, while nonzero-session Beep waits
  synchronously; Session-0 requires unavailable NT-private services/owner.
  Historical finite Beep(440,250) probes take about 262/259 ms on x86/x64.
  The infinite-duration mismatch is source/API reasoning, not an executed
  infinite call or audible guest test. InitSound has no production caller;
  its test does not prove a user-visible shutdown requirement.

Original PPI/Timer2 frequency and gating remain the sole device state authority;
Compat converts/transports requests through Lib. P158 final builds and all
392 tests pass without native deferral. A future redesigned native backend is
not ruled out, but is not an exact restoration within this audit.

### VGA painters

R0682 removes five nonsemantic lines; production +0/-5 and XP diff -5, tests
unchanged. R0683-R0686 remain four boundary/geometry decisions for standard,
medium, high and V7 packed standard painters. Actual device callbacks call the
original conversion loops. This V7 build has EGA_PLANE_SIZE 0x20000, hence
512 KiB over four planes; packed offsets are bytes and planar offsets groups.

Each complete original function is restored independently with real object/
fixture linkage on both widths. Standard/medium/high fail zero-height cases;
adding only a nonpositive guard still fails standard/medium negative-x output
assertions and high final-group source bounds. V7 original and nonempty-only
variants fail dirty publication after actual BIOS/BOP mode 67h (640x480), since
original 640x400 ceilings reject the mode. Current clipping follows actual mode
and backing capacity without a second painter or larger virtual card.

All eighteen primary runs and twelve diagnostic reruns complete; local probes
identify failing request arguments rather than guessing from exit codes.
Original split-update can submit an empty first region: source-path proof is
separate from arbitrary negative/overflow boundary fixtures. No particular
Win3.1 failure or universal painter safety is claimed. Final P159 builds and
all 392 tests pass; no executable statements changed in its restoration.

### Fixed resources

Current byte hashes match the P159 fixed-XP comparisons and initial inventory:
bios1.rom (28420 bytes), bios4.rom (8191), cmos.ram (64), v7vga.rom (18832).
firmware.rc embeds BIOS1/BIOS4/V7 as 101/102/103; original rom.c names reach
Compat host_read_resource's same mapping. No external checkout is needed at
runtime. cmos.ram has no current loader/embedding consumer; only its original
filename macro remains. It stays byte-identical because deleting it creates a
new whole-file difference forbidden by the later owner clarification, not
because a fictional runtime consumer is needed. No ROM/guest resource changed.

Eight IDs are added: cumulative 381 (375 semantic/ABI, six compiler/format),
nineteen notices. All 472 file decisions are now reconciled, but original/local
extension defect decisions and final accounting remain. All residual choices
are unapproved. P164 fresh 392/392 remains the current full runtime result;
this batch changes only records, with no source/test/artifact change.

## Proposed new repairs: D001-D006 and L001

These are separate decisions, not existing residual IDs or delivered fixes.
Current source still contains the affected behavior. The no-new-diff rule
prevents silently adding these repairs. No item is moved back into TODO;
unknown minimal patches and unproved reachability remain explicit limitations.

| Decision | Current evidence and scope | Proposed disposition before implementation |
| --- | --- | --- |
| D001 | sevid004 GenericByteFill calls GenericByteWrite then continues using shared LocalIUH. P34 actual bodies crash on both widths before/after restoration. Current CCPU fills use individual stores; no guest batch consumer established. Related nested sites were source-scanned, not dynamically proved faulty. | Keep unimplemented. A minimal caller-local preservation patch is not yet established; do not add a second write path or claim guest impact. |
| D002 | sevid008 S2360/S2372 second WORD starts at +8. P38 actual bodies place 33 44 at +8/+9, not +2/+3, on both widths. sevid011 S2441-S2448 share the source pattern but lack equivalent dynamic proof. Current guest DWORD video access uses existing WORD operations rather than these generated DWORD entries. | Potential +8 to +2 correction requires multi-DWORD fill-order verification; a scalar correction alone does not prove the fill. No new patch or guest symptom claim. |
| D003 | sevid023 S2844 fills two overlapping count-wide WORD runs at +0/+2. P54 four requested 44332211 values produce 11 22 33 44 33 44 33 44 33 44 and six untouched bytes on both widths; current/restored/poisoned variants agree. Current c_sas_fillsdw loops through scalar stores; no production fill_dword_ev_glue caller established. | Minimal whole-DWORD fill repair remains undesigned. Keep separate from D002 offset repair; no unproved activation of the batch API. |
| D004 | sevid025 CHAIN4 mode-2 WORD/DWORD expansion writes low-byte results to subsequent addressed planes. Real selector tests yield ff 00 for 0201 and ff 00 00 00 for 08040201, versus the per-plane fixture expectation, on both widths. WORD vector is production-wired. | Hardware/guest applicability is not fully verified. Treat as a source/fixture discrepancy requiring that investigation before approving an addressed-plane-byte repair; no universal hardware defect claim. |
| D005 | timer.c active-event backlog computes zero int_delay and lowers timer_multiple_delay to zero. P115 real Machine/PIT ports, two heartbeats and actual quick-event dispatch reach divisor1/mode2/delay0/active1/more65525, then stack overflow on both widths. | Concrete isolated two-line candidate clamps int_delay to one before the existing comparison; all nine mode/divisor cases pass per width. Recommend a separately authorized minimal repair plus permanent regression. Current first-event guard is necessary but incomplete. No real guest crash was observed. |
| D006 | egwrtm12 easy even-address mode-2 byte path tests plane-enable bit8 while writing plane2. Real Machine/BIOS mode3 and GC mode2, mask4/value4/offset0 leave plane2=00 instead of ff on both widths. Logical/WORD counterparts use bit4. | Concrete candidate changes 8 to 4 in the existing condition. Recommend a separately authorized minimal correction with mask/path regression. Current evidence is one controlled case, not a full mask matrix or guest reproduction. |
| L001 | Local PIC snapshot extension omits SWPIC_outb's per-adapter sequence phase. P109 actual controller snapshots after ICW1/2/3 on master/slave: all six uninterrupted controls pass per width, all six restored cases diverge in base/mask/mode. POST leaves phase0, so remaining ICWs become OCW1 mask writes. | Add the existing phase to the same archive, validate and restore it without port replay or another state machine. Requires new mirror difference and archive schema/format decision; exact patch size is not established. Do not call current PIC snapshot complete. |

For L001 the expected master/slave bases are 56/112, mask0/mode17. After ICW1
restore the actual base is0, mask1/mode1; after ICW2/3 base is correct but
mask1/mode1 persists. This is twelve failures with twelve matching controls,
not twelve passes. Instruction-boundary capture does not ensure a multi-write
ICW transaction has completed. No guest-issued snapshot scenario was run.

For D005 the private probe deliberately advances the existing quick-event
countdown/dispatch boundary; it is not a parallel timer or a record of normal
guest timing. The tested two-line correction is not installed. For D006 the
retained CHAIN2 address corrections do not fix the unrelated wrong enable bit.

Additional inactive-original MIDI ring/volume findings remain with R0665/R0667
and R0666's restored native helper discussion. They are not silently counted as
D007 or implemented: the active copied-message path is distinct and the original
helpers have no activation caller. They remain visible to owner review.

This reconciliation adds no residual ID, patch, test or new runtime result.
381 existing residual IDs and nineteen notice items remain separately counted.
No pending defect is erased by the final P164 suite passing 392/392: the temporary
negative probes above exercise cases outside that permanent passing suite.

## Final accounting: identity and restored-ID index

At P186 HEAD, a fresh check compares all 472 frozen order/path/initial-hash
triples to P1 7494ca3b9b8496a2f3abb943657eab754ffc42e5. They are unchanged;
orders are exactly 1-472 and the current tracked mirror path set is identical.
Every P1-P186 delivery identifier occurs exactly once in Git history after the
admitted baseline. Delivery presence proves traceability, not correctness.

The final view previously described these restored groups collectively without
spelling every ID. They are explicitly indexed here, not reclassified as pending:

| File | Already restored IDs | Disposition |
| --- | --- | --- |
| sevid007.c | R0300 / R0303 | Exact native dereferences; malloc in 27 entries without mixed-width upper-byte dependence. |
| sevid008.c | R0304 / R0307 | Exact native expressions; malloc at the 25 corresponding sites. |
| sevid009.c | R0308 / R0311 | Native dereferences including P56's final lookup restoration; ordinary allocations restored. |
| sevid010.c | R0312 / R0315 | Native dereferences including P56's final second-WORD store; ordinary allocations restored. |
| sevid011.c | R0316 / R0319 | Native expressions and 27 ordinary malloc calls. |
| sevid012.c | R0320 / R0323 | Native expressions and 27 ordinary malloc calls. |
| sevid013.c | R0324 / R0327 | Native expressions and ordinary malloc calls. |
| sevid014.c | R0328 / R0331 | Native expressions and ordinary malloc calls. |
| sevid021.c | R0376 | Fifty original native byte-access lines. |
| sevid022.c | R0379 | Twenty-one original native byte-access lines. |
| sevid023.c | R0382 | Twenty-three original resolved-address expressions. |
| sevid024.c | R0385 | Ten original native source/table accesses. |
| sevid025.c | R0388 | Sixty-six original native-address expressions. |
| sevid027.c | R0394 | Forty-one original dereferences. |
| sevid028.c | R0397 | Sixty-two original dereferences. |

These 23 IDs complete explicit R0001-R0686 coverage in this view, counting
written ranges as their individual identifiers. Identifier presence is not a
claim that every ID is a current residual or has individual dynamic proof.
Remaining mixed-width/native capacity and GDP contracts retain their separately
listed pending IDs and proof limits; no excluded-body IDs are created.

The inventory has 111 file statuses containing PENDING and 361 other statuses.
Those are historical file-disposition labels, not 111 approved patches or an
independent count of necessary hunks. The reconciled 381 residual IDs represent
audit groups (375 semantic/ABI and six compiler/format), with nineteen notices
and seven proposed repairs separate. Some groups contain both restored and
retained portions; source ranges are not independent defects.

### Current tested-source and artifact identity

Git diff from P164 21e56aa077334ed5db04a3ab1d64e026bcc2656f through P186 is
empty for src, test, tools, root CMake files and assets. Current EXE bytes are:

| Artifact | Bytes | SHA-256 |
| --- | --- | --- |
| softpc32.exe | 3478046 | 1c4f19f8f9840a86fdb796ebec035fea8c45704d0f25edde7a3ba7d92f65fe1c |
| softpc64.exe | 3095494 | f4fb0f42bf7b7e91db40b2ca44fdda2341fd164ee4c820736b29c3acfb493a05 |

They match P164's fresh x86/x64 188 background plus eight native tests each,
392/392 without retry/skip. This is reused unchanged-source evidence, not a
new test run. Temporary negative probes identify gaps outside that suite.

### Task-start source delta, not XP mirror-diff reduction

Reproducible command: git diff --numstat from
ac9d8dc85a6d9b0cc66d0a6649f60e8903406f18 to P186, by tracked path scope.

| Scope | Changed files | Added | Deleted | Net source lines |
| --- | ---: | ---: | ---: | ---: |
| Mirror source tree | 123 | 5922 | 3721 | +2201 |
| Other production src | 4 | 137 | 29 | +108 |
| Tests | 9 | 336 | 48 | +288 |
| Tools and root build configuration | 2 | 8 | 7 | +1 |

The four non-mirror production files are Compat ccpu/abi.h, ccpu/archive.c,
ccpu/snapshot.h and platform.c. Build/check changes are CMakeLists.txt and
standalone_source_boundary.cmake. No public eight-tree changes are included.
These counts deliberately exclude docs and EXEs. They do not measure remaining
or reduced XP diff: restored original code can increase source length.

### P188 fixed-XP aggregate comparison

Across the 123 changed mirror files, the normalized Git/Myers comparison with
the fixed XP baseline decreases by **7328 added-plus-deleted diff lines**.
The other 349 frozen files are unchanged since task start and contribute zero
to this delta. Of the changed files, 122 reduce that metric; ev_glue.c is neutral.
No changed file increases it. This is a reproducible heuristic comparison,
not a count of independent semantic differences or the sum of anchored local
restoration counts.

| Mirror directory | Change in XP added-plus-deleted diff lines |
| --- | ---: |
| base/bios | -55 |
| base/ccpu386 | -286 |
| base/comms | -101 |
| base/cvidc | -3266 |
| base/inc | -176 |
| base/keymouse | -9 |
| base/support | -3 |
| base/system | -136 |
| base/video | -162 |
| host/inc | -431 |
| host/src | -2703 |
| Total | -7328 |

Reproduction inputs are task-start ac9d8dc85a6d9b0cc66d0a6649f60e8903406f18,
current source (unchanged from P164), and XP SP1
a2f6d7c93aa4f11efc51147566d5961c59a8ef89 under
Source/XPSP1/NT/base/mvdm/softpc.new. Enumerate the frozen inventory; compare
each changed file against that same original, before and after. Normalize CRLF
to LF without changing other bytes; omit line directives for this metric.
Parse conditional branches by parent, expression, occurrence and branch arm,
preserving physical line positions through comments and directive continuations.
Remove already-excluded body spans from corresponding comparison inputs, then
run git diff --no-index --numstat --no-renames --diff-algorithm=myers. Subtract
the two added-plus-deleted totals. Exclusion bodies receive no IDs, inventory,
or claimed restoration benefit; this comparison is not an exclusion audit.

Automatic structural correspondence succeeded for 113 changed files. The other
ten were checked against original/current directive context: reset.c, c_main.c,
cpu_vid.h, egacpu.h, gmi.h, ica.c, vga_mode.c, cfpu_def.h, host_cpu.h and
nt_graph.c. Existing local-only guards have no invented XP counterpart; their
real framing differences remain visible. Renamed conditions and shifted
occurrences are matched to their actual original location. For reset.c's
restored floppy branch, the formerly excluded body is omitted from both
comparison sides rather than counted as restoration gain. These ten files
contribute -280; the other 113 contribute -7048.

Global heuristic matching is not interchangeable with the earlier fixed-range
proofs: for example, nt_sbdsp.c contributes -1376 here, while P154 records 1628
restored difference lines in its anchored comparison; nt_sb.c contributes
-390 here versus P153's local -412. Other P batches also touched those files.
Do not replace the per-point evidence with this aggregate or add local numbers
to it. The aggregate measures task-start to final source using one method;
the individual records establish which original code was restored and why.

This accounting does not approve the 381 residual IDs, nineteen notice items
or proposed D001-D006/L001 changes. No source, test or EXE changed in P188;
P164's 392/392 result remains applicable and is not a newly executed test run.

## Verification boundary

The recheck reads current CMake bindings, BIOS/PIC/descriptor callers,
CALL/stack-size operations, SAS/archive providers, CPU return declarations
and definitions, plus the existing BOP/device/BIOS fixtures. It reconciles
these claims against P1-P13 final evidence. The continuation also traces
current interrupt callers, executor/lifecycle/debug ownership, operand
observers, archive register access and private TLB storage. It does
not create a new runtime result from a source search, nor turn three qualified
recommendations into owner approval. The compact decision packet and P189
audit-delivery completion review preserve those approval boundaries; owner
disposition remains outstanding.

## Subsequent authorized repairs

S2 P2 implements owner-approved D005 and D006; see the
[repair evidence](m9-t100-s2-repairs.md). The S1 findings above remain historical;
other repair candidates are not implicitly approved or implemented.
