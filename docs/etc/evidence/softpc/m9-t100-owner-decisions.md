# T100 owner decision reconciliation

This is the review view of the [ordered audit](m9-t100-progress.md), not a new
retention authority. CURRENT owns task status. Existing exclusions are outside
this review and receive no entries. Existing A/B residual retention choices
remain unapproved until recorded in S3 below. S1's new-repair recommendations
are historical; S2 decisions supersede them.

## S4 current file-by-file classifications

This section records the fresh 121-file manual review, in frozen directory
order. Historical IDs below identify locations, not inherited approvals.
After each file is presented, append its point classifications, XP-relative
added/deleted lines, evidence limits and explicit owner disposition here.
Owner now explicitly approves all points already reviewed in files 1-11
(through ccpu386/c_main.c): "之前所有diff点位记得标记为批准".
This supersedes their earlier recommendation/pending status, not their evidence
limits. Restored/deleted points remain restored/deleted; they are not reinstated.
Owner subsequently approves file 12 (c_page.c), all 15 points R0115-R0129.
Owner subsequently approves file 13 (c_tlb.c), all four points R0135-R0138.
Owner subsequently approves file 14 (fpu.c), both points R0146-R0147.
Owner subsequently approves ntthread.c R0160-R0162 at review 16; its P9
items were already approved. Review 15 retains the dispositions recorded below.
Owner approves vglfunc.c R0412 and diskbios.c R0414. Diskbios directive-only
differences are restored under P30, not retained approval candidates.
Unreviewed points remain pending until explicitly approved.
Existing #error bodies and separately counted line controls remain excluded.

| Review / order | File under base/ | Points | Actual purpose | + / - | Fresh disposition / evidence limit |
| --- | --- | --- | --- | --- | --- |
| 1 / 6 | bios/reset.c | R0006 | Product boundary: retired software mouse-driver lifecycle calls | 0 / 2 | Retain approved by owner; hardware mouse initialization remains. |
| 2 / 8 | bios/slave_bs.c | R0008 | Original device defect: slave PIC EOI command port | 1 / 1 | Retain approved by owner; actual BOP/PIC path checked; fixture read, not rerun. |
| 3 / 9 | bios/tape_io.c | R0010 | Selected CPU interface declaration: descriptor limit sys_addr pointer | 1 / 1 | Retain approved by owner; provider/caller agree; no demonstrated runtime truncation. |
| 4 / 41 | ccpu386/c-files | R0014 (nine entries) | Product build subset: six SPC486 workers, two PIG workers, unused NOP | 0 / 9 | Retain approved by owner; actual CMake consumer and XP guards checked. Not an algorithm repair. |
| 5 / 42 | ccpu386/call.c | R0015 | Original CPU defect: destination SS determines stack address width | 1 / 1 | Retain approved by owner; source proof; no fresh executed call-gate transition test. |
| 6 / 47 | ccpu386/ccpusas4.c | R0018, R0021, R0022 | Snapshot: private SAS state declaration/capture/restore | 56 / 0 | Retain approved by owner; archive callers checked; not exhaustive snapshot-state proof. |
| 6 / 47 | ccpu386/ccpusas4.c | R0019, R0020 | Host pointer ABI: typed RAM init/term declarations | 2 / 0 | Retain approved by owner; pointer-return provider verified; x64 truncation risk, not every allocation reproduced. |
| 6 / 47 | ccpu386/ccpusas4.c | R0024 | Original memory defect: complete physical read span bound | 1 / 1 | Retain approved by owner; terminal-span source example; no fresh test run. |
| 6 / 47 | ccpu386/ccpusas4.c | R0026, R0027, R0030, R0033-R0035 | Original permission defect: six CPL3 user-access predicates | 6 / 6 | Retain approved by owner; controlled existing fixture is not exhaustive paging/V86 proof. |
| 6 / 47 | ccpu386/ccpusas4.c | R0028, R0029, R0031, R0032 | Original data-width defect: four WORD/DWORD fallback casts | 4 / 4 | Retain approved by owner; original IU8 truncation verified; existing tests not rerun. |
| 7 / 64 | ccpu386/cpu4gen.h | R0038-R0041 | C declaration consistency: four segment setter returns | 4 / 4 | Retain approved by owner; original ISM32 providers verified; not a reproduced guest failure. |
| 7 / 64 | ccpu386/cpu4gen.h | R0042-R0050 | C declaration consistency: nine flag getter returns | 9 / 9 | Retain approved by owner; equal storage width does not make C types identical. |
| 8 / 73 | ccpu386/c_bsic.h | R0051 | C declaration completeness: internal EFLAGS getter | 1 / 0 | Retain approved by owner; actual IU32 provider/callers checked; no proven EFLAGS truncation. |
| 9 / 74 | ccpu386/c_debug.c | R0053-R0055 | Snapshot: private instruction/data breakpoint caches | 45 / 0 | Retain approved by owner; DR7 alone cannot preserve observed cache state; fixture read, not rerun. |
| 10 / 78 | ccpu386/c_intr.c | R0057 | Interrupt delivery boundary: reject PIC no-vector sentinel | 2 / 0 | Retain approved by owner; independent NPX caller checked; no traced guest NPX incident. |

Classification totals for these ten files only:

| Purpose | Added | Deleted | Added + deleted |
| --- | ---: | ---: | ---: |
| Product/source-subset selection | 0 | 11 | 11 |
| C interface declaration consistency/completeness | 15 | 14 | 29 |
| Host pointer ABI adaptation | 2 | 0 | 2 |
| Snapshot integration | 101 | 0 | 101 |
| Original behavior / interrupt-boundary corrections | 15 | 13 | 28 |
| Total | 133 | 38 | 171 |

These reconcile with the first ten rows of the frozen count ledger: 41 Git
text hunks. Hunks, grouped rows and individual semantic locations are not
interchangeable counts. No production changes or new dynamic verification are
claimed. The following entry advances the presentation frontier; the ten-file category totals above remain scoped to those ten files.

### Review 11 / order 80: ccpu386/c_main.c

Frozen selected-content diff: 29 text hunks, +136/-16. Owner now approves
all surviving points after P6-P9; restored points stay removed. Current
selected-content diff is +110/-16. The following table is the original review
classification; subsequent P6-P9 dispositions supersede removed entries. The current 11-file total
is 70 text hunks, +269/-54 (323 added+deleted lines). The table below classifies
all remaining historical point IDs without counting shared includes twice.
It does not assign text hunk boundaries to semantic categories or replace the
frozen count ledger with historical anchored line estimates.

| Points | Actual classification | Current evidence / disposition |
| --- | --- | --- |
| R0059, R0085, R0106, R0109, R0111, R0112 | Selected-host execution ABI: original TLS simulation stack; two jmp_buf pointer dereferences | ntthread.c supplies jmp_buf pointers. Six NTVDM condition extensions select the existing TLS path without enabling all NTVDM. Owner-approved retention; no exhaustive longjmp proof. |
| R0060, R0062-R0064, R0066-R0067 | C declaration consistency/completeness | c_addr/c_seg/ica/timer headers and force_yoda/TakeNpxExceptionInt declarations; actual original providers, no new algorithms. Owner-approved retention. |
| R0061 | Lifecycle interface declaration (also used by snapshot continuation) | Shared include for the existing lifecycle owner; counted once. Owner-approved retention with represented hooks. |
| R0065, R0078-R0079, R0084, R0086, R0091, R0104 | Snapshot state and continuation | Save DR/TR, interrupt map and sampled trap; restore HLT/fetch phase rather than restart at arbitrary instruction boundary. Existing archive/lifecycle consumers; owner-approved retention. |
| R0068, R0070-R0075, R0093-R0094, R0097, R0099, R0101-R0102 | Executor integration: queued clock/control, HLT wait, quick-loop escape, pacing | platform.c owns the queue/wait/timing functions. Three consumption positions have distinct incoming paths. Owner approves preserving required integration; do not claim pacing is polling-free or proven minimal. |
| R0103 | Finite-run product capability | NEXT_INST consumes budget, preserves reset priority and requests outer exit. Entry-boundary consumption is not proof of exact retired-instruction counting. Keep this limitation explicit. |
| R0076, R0087-R0090, R0095-R0096 | Host debugger capability | debug.c begin/refetch plus five retirement edges (POP SS, MOV SS, IRET, STI, ordinary). Separate from guest DR snapshot state. Owner-approved retention; no full instruction-edge matrix executed this round. |
| R0077 | Private-state exposure for product integration | Writable interrupt-map address used by lifecycle clear and Compat keymouse/cpu4.h for original keyboard wiring. Earlier clear-only rationale is incomplete. Do not infer smallest possible API solely from callers. |
| R0107 | PIC deassertion integration | c_cpu_clearHwInt clears the existing CPU_HW_INT_MASK; platform.c consumes it. Owner-approved retention; not a second IRQ model. |
| R0108, R0110 | Lifecycle nesting ownership | Enter/leave hooks track original nested simulation frames for safe outer return/checkpoint. Owner-approved retention with existing owner. |
| R0100 | Interrupt-boundary correction | Reject PIC sentinel before EXT/SYNCH_TICK/interrupt delivery; callee guard is later. Owner-approved retention. |

Fresh inspection covered current source differences, ntthread pointer providers,
lifecycle.c, platform.c, machine/debug.c and the keymouse map adapter. Historical
checkpoint/runtime evidence is supporting material only; no fresh build, dynamic
negative control, desktop or guest validation occurred. The pointer accessor,
pacing implementation and budget precision require separate owner discussion;
they are not silently declared optimal or approved by this presentation.
Next file: order 88, ccpu386/c_page.c, verified against the frozen list.
110 files remain to present. No production changes.

### Review 12 / order 88: ccpu386/c_page.c

Fresh fixed-XP comparison confirms +20/-6 selected-content lines, 14 text
hunks and 15 historical point IDs R0115-R0129. Excluded bodies receive no
review entries. All surviving changes serve host DEBUG operand observation;
none is a paging repair or snapshot feature. Owner explicitly approves
retention of all 15 points after their individual explanation.

| Points | Purpose and exact size | Evidence / limitation |
| --- | --- | --- |
| R0115-R0116 | Two observer declarations, +3/-0 | Signatures match machine/debug.c; no extra memory read. |
| R0117, R0125 | Preserve original bulk pointer/count/linear address, +6/-0 | Original loops mutate all three; callback needs the initial values. |
| R0118, R0126 | Completed bulk read/write notifications, +2/-0 | Original x87 callers use reversed buffers; reversed=1 preserves address/value association. |
| R0119-R0124 | Six scalar read result wrappers, +6/-6 | BYTE/WORD/DWORD, cached-physical and supervisor routes; helper returns its input unchanged. |
| R0127-R0129 | Three completed scalar write notifications, +3/-0 | Shared placement after either original write branch avoids duplicate notifications. |

Current CMake explicitly selects c_page.c for production. Original c_oprnd.h
calls these operand functions for ordinary and x87 operands. machine/debug.c
owns the observation state: accepts records only in an instruction, records
copied values, and retirement requests a stop after a watch hit. Guest DR
checks in c_debug.c instead update guest architectural debug state; using them
as the host observer would conflate independent debugger ownership.

The S1 file-88 comparison recorded both architectures compiling with the whole
observer route removed, then failing command-provider's wait for PAUSED after
a watch run; current/no-yoda controls passed. This historical negative control
proves a real consumer of the route, not every individual hook or fault edge.
Fresh source review of the existing command-provider fixture confirms WORD
read/write overlap, returned data/width and debugger-inspection non-triggering
assertions. This round does not rerun tests or claim exhaustive BYTE/DWORD,
x87, cross-page fault or partial-write coverage. No newly established safe
restoration candidate; no source, test, binary or guest change.

Progress: 12/121 files presented, 109 remain. Files 1-11 retain the owner's
explicit approval; this file is now also owner-approved. Next: order 98, ccpu386/c_tlb.c.

### Review 13 / order 98: ccpu386/c_tlb.c

Fresh fixed-XP comparison confirms two selected text hunks, +58/-0. This is
snapshot integration, not an original paging defect. The associated non-exclusion
line control adds one diagnostic line (+59/-0 when included), explaining the
historical count. Excluded bodies and their anchors are not audited or booked.

| Point | Size | Purpose and retention basis |
| --- | --- | --- |
| R0135 | +1/-0 | snapshot.h supplies the typed archive schema and declarations used here. |
| R0136 | +25/-0 | Capture original private tlb entries, replacement next_set and FAST_TLB lookup arrays; 24 function lines plus one separator. Actual archive.c caller owns storage. |
| R0137 | +32/-0 | Restore those same original arrays; reject nonboolean valid/dirty and out-of-range next_set. 31 function lines plus one separator. Actual archive.c propagates failure. |
| R0138 | +1/-0 diagnostic only | Existing #line associated with the inserted block; not part of the +58 selected-content total. |

Current c-files includes c_tlb.c; CMake consumes that list. Original storage
is LOCAL, and original flush_tlb invalidates rather than restores it. Original
test_tlb manipulates architectural test registers/cache entries, not a complete
state import/export contract. Replacing the bridge with external raw-array
ownership or moving the same code does not establish a complexity reduction.
Field copies separate the archive schema from native TLB_ENTRY representation;
whole-struct memcpy is not established as equivalent.

Historical S1 P12's coherent flush-only replacement compiled on x86/x64 but
failed the real checkpoint assertion expecting saved mapping 0x3123; controls
passed. Fresh source review confirms the fixture demonstrates 0x3123 remaining
cached after a page-table change, 0x4123 after flush, and 0x3123 after restoration.
This round does not rerun it. It proves the cache-restoration requirement, not
every field independently or every malformed archive. In particular restoration
writes entries before all validity checks finish: rejection is not transactional
rollback. Do not claim otherwise or expand this review into an unapproved repair.

Owner approves retention of all four points. No new safe
restoration candidate established. No production/test/binary changes. Frontier
13/121; 108 remain. Next: order 121, ccpu386/fpu.c.

### Review 14 / order 121: ccpu386/fpu.c

Fresh fixed-XP comparison confirms +8/-5 selected-content lines (13 changed
lines), five text hunks, two semantic points. Excluded bodies are not audited
or counted. No snapshot-specific mirror additions remain in this file.

| Point | Size | Classification and retention basis |
| --- | --- | --- |
| R0146 | +6/-4, four hunks | Host C representation correctness: CVTFPHI64 copies FPH bytes into a real FPHOST object before reading exponent, mantissa and sign. Original casts dereference double storage as a different aggregate type. Same original conversion algorithm and representation remain; no numerical recast or new path. |
| R0147 | +2/-1, one hunk | Original integer-conversion behavior: FIST M64I uses existing npx_rint before CVTFPHI64, matching M16I/M32I's existing rounding owner. Reuses FPRes and keeps original overflow/exception/store handling. |

Real production selection includes fpu.c in the CCPU direct-port tail.
CVTFPHI64 is consumed by original integer/conversion operations; FIST's M64I
branch is exercised by guest FISTP m64 in the command-provider fixture.
Current cfpu_def.h selects double FPH and FP64 FPHOST. The representation
copy removes this particular incompatible-type access; it does not establish
that all other original FPU type punning is safe or that every host format works.

S1 P15 independently restored each point using actual compiler/link flags:
R0146 restoration failed stored == 0x0000123400001234ull on both architectures;
R0147 restoration failed nearest-even input 3.5 / width 8 (first stored byte
00 rather than 04) on both. Current controls passed 228 conversion cases and
surrounding checks. These are historical per-point negative controls, not a
fresh run. Fresh inspection confirms the current test still uses actual guest
instructions, four rounding modes, integer widths and output/status assertions.
The observed failure byte is not generalized into an unmeasured complete value.
No exhaustive x87, NaN/exception or all-compiler correctness is claimed.

Owner approves retention of both points. No production,
test or binary change, and no newly established equivalent restoration.
Frontier 14/121; 107 remain. Next: order 182, ccpu386/ntstubs.c.

### Review 15 / order 182: ccpu386/ntstubs.c

Fresh fixed-XP comparison confirms +1/-12 selected-content lines, two text
hunks and four semantic points. These are selected-build symbol ownership,
not guest algorithm repairs or snapshot features. Adjacent excluded bodies
are skipped; their proximity must not hide the three omitted external stubs.

| Point | Size | Retention basis |
| --- | --- | --- |
| R0155 | +1/-1 | Gdp becomes extern; original ev_glue.c defines it under the selected CCPU branch and uses it for C-VID. Avoid a second definition of the same object. |
| R0156 | +0/-3 function lines | Omit empty copyROM definition; selected original rom.c already defines the same empty function. ccpusas4.c calls it. |
| R0157 | +0/-3 function lines | Omit empty initialise_npx definition; selected original fpu.c already defines it. |
| R0158 | +0/-3 function lines | Omit empty npx_reset definition; selected original fpu.c already defines it. |

The contiguous three-function deletion also removes two inter-function blank
lines: +0/-11 for that block, giving +1/-12 overall. Separators are counted
once, not presented as independent semantic patches.

CMake explicitly selects ntstubs.c. Its Video object is populated by existing
Compat facade.c from C_Video; original SAS/CPU bridge symbols remain. Whole-file
deletion is not justified. The retained original definitions of all three
functions are themselves no-ops: no claim that omission enables a real reset
or ROM-copy algorithm. No new Compat replacement is introduced.

Historical S1 P17 independent Gdp restoration failed production-equivalent
links on x86/x64 with duplicate Gdp. Restoring the three functions produced
all three duplicate-symbol diagnostics on both architectures. Current controls
compiled, linked and passed checkpoint. This round freshly verifies actual
source ownership/callers and fixed-XP diff, without rerunning links or tests.
Moving omissions to fpu.c/rom.c relocates differences and adds mirror sites;
no simpler equivalent alternative is established. Recommend all four for
retention, pending owner. No source/test/binary changes. Frontier 15/121,
106 remain; next order 183, ccpu386/ntthread.c.

### S4 P15: owner-selected static original stubs

Owner chooses retaining the three original ntstubs.c empty bodies with static
linkage rather than deleting them. This supersedes the review-15 deletion
recommendation for R0156-R0158. Restore original function names, bodies and
spacing; add only static to each definition. No references, macros, alternate
implementations or build flags change. External rom.c/fpu.c owners remain.
The preceding excluded-branch line anchor is corrected to #line 91 so copyROM
is again at original logical line 92; no exclusion-body bookkeeping is added.

Current selected diff is +4/-4 (8 lines): R0155 +1/-1 unchanged and each of
R0156-R0158 +1/-1. Former +1/-12 (13 lines) therefore decreases by five mirror
difference lines, despite source growing +12/-1 (net +11 including line anchor).
Frozen file-list counts remain unchanged. The static choices are owner-approved;
R0155 retains its existing review disposition and is not silently approved.

Both cached release builds/publication pass. Both ntstubs objects are exactly
byte-identical to pre-change objects. Complete EXEs retain their sizes; x86
has nine differing bytes and x64 ten, all confined to PE COFF timestamp,
OptionalHeader checksum and the existing compiler time string at the same
location. No other byte changes. The approved artifact-equivalence exception
applies: no new runtime tests are claimed. P9's 392/392 remains historical
runtime qualification. Existing mirror macro/printf warnings persist; no
nonmirror warning or error is introduced. No new state, consumer or runtime
path; static unused original bodies generate no additional release object code.

### Review 16 / order 183: ccpu386/ntthread.c

Fresh full fixed-XP comparison: +36/-3 (39 changed lines), five current text
hunks and five semantic points. Frozen list +23/-2 predates approved P9's
query/teardown changes; keep that historical list unchanged.

| Point | Size | Classification / evidence |
| --- | --- | --- |
| S4 P9 TLS detachment | +4/-1 | Lifetime safety: clear TLS before free; failed detachment reports error and retains the still-referenced object, while independent thread-list cleanup continues. Owner-approved implementation. |
| S4 P9 depth query | +9/-0 | Single original nesting authority: read ThreadSimBuf.level without push/pop; lifecycle checkpoint/resume/exit consumers replace the deleted Compat counter. Owner-approved implementation. |
| R0160 | +21/-0 | Product outer-exit integration: select level 1 and call original ccpu386Unsimulate, which alone decrements, sets in_C and longjmps. Needed to exit all nested BIOS frames to executor. Recommend retention. |
| R0161 | +1/-1 | Defined pointer return on BADID in ccpu386ThrdExptnPtr; replace nonvoid bare return with null. Recommend retention with caller limitation below. |
| R0162 | +1/-1 | Same defined result when TLS lookup supplies no stack. Recommend retention with the same limitation. |

Actual CMake includes this file; machine worker teardown calls exitthread;
lifecycle.c uses depth query at checkpoint/resume/exit and calls outer unwind;
c_main.c uses the exception pointer for setjmp. No extra nesting state or
second unwind implementation is added. Private depth is not snapshot encoding.

Historical S1 P18's ordinary-unwind substitution failed actual nested checkpoint
EIP 604 assertions at both widths; controls passed. Original exception-pointer
bare-return injections produced 0x31 / 0x39 at both widths rather than a defined
pointer. This proves the return defect, NOT safe caller recovery: c_main still
passes *ccpu386ThrdExptnPtr() directly to setjmp without checking null. No claim
that memory exhaustion or TLS failure is fully handled; no new repair is made.

S4 P9 tests/probes already covered real nesting 1/2/1/0, outer exit, HLT resume,
zero after teardown, same-level exception jump and failed TLS clear retaining
allocation without freeing reachable storage. Full 392/392 and snapshot
interoperability evidence is historical here, not a new run. A native TLS-clear
failure may leak until process exit; the code intentionally avoids a dangling
pointer without a recovery manager. Fresh review confirms these current paths;
it does not prove all allocation/longjmp failure combinations or smallest
possible diagnostic formatting. No new equivalent restoration identified.

No production/test/binary change. Frontier 16/121, 105 remain. Next: order 265,
ccpu386/zfrsrvd.c. Owner now approves R0160-R0162; P9 items retain their
existing explicit approval. Moving to this file alone does not approve R0155.

### Review 17 / order 265: ccpu386/zfrsrvd.c

Fresh fixed-XP comparison confirms one selected hunk +17/-0, historical point
R0172, entirely snapshot support for private delayed-NPX exception IP. Excluded
bodies and associated anchors are not audited/counted. Earlier restored
R0166-R0171 remain restored, not resurrected for review.

| R0172 subpart | Size | Retention basis |
| --- | --- | --- |
| softpc_ccpu_snapshot_delayed_npx_eip | +6/-0 including separator | Read the original LOCAL NpxExceptionEIP without delivering an interrupt or changing state; archive.c capture is the actual caller. |
| softpc_ccpu_snapshot_set_delayed_npx | +11/-0 including separator | Restore saved private IP and pending flag via archive.c, preserving original delayed-delivery behavior and original state ownership. |

Original DoNpxException snapshots NpxFIP into NpxExceptionEIP. The later
TakeNpxExceptionInt restores that saved value, clears pending and injects an
interrupt. Current FIP and saved fault IP are distinct states; existing original
functions are not side-effect-free archive accessors. CMake selects zfrsrvd.c,
and original CCPU dispatch consumes its NPX handlers. No guest instruction,
second FPU or new snapshot format is added by these accessors.

Historical S1 P21 coherent removal of both accessors/archive calls retained
ordinary pending-flag restoration but failed real restored exception-FIP
assertions on both architectures. Controls passed. Fresh fixture inspection
confirms saved IP 0x11223344, current FIP 0x55667788 at capture, later exception
0x99aabbcc, then real delivery after restore recovering 0x11223344. This round
does not rerun tests. This proves the saved-IP restoration capability, not all
x87 fault opcodes or that every helper spelling/placement is uniquely minimal.
Owner approves retention of R0172. No production/test/binary changes.
Frontier 17/121; 104 remain. Next: order 267, comms/com.c.

### Review 18 / order 267: comms/com.c

Fresh fixed-XP comparison confirms +126/-3 (129 changed lines), six selected
text hunks. Five functional IDs plus one historical notice ID remain; excluded
bodies/anchors receive no entries. Earlier restored formatting/prototype points
remain restored. Current production CMake explicitly selects this controller.

| Point | Size | Purpose / basis |
| --- | --- | --- |
| R0173 | Restored in P20 | Owner rejects relocating content out of an excluded SCCSID body. Remove the extracted comment; retire this retention candidate. |
| R0174 | +1/-0 | compat/devices/snapshot.h declares the schema/constants for the private-state bridge. |
| R0178 | +121/-0 | Snapshot controller/callback bridge: declarations/separators 5, encode 15, decode 8, capture 44, restore 49 lines. Details below. |
| R0181 | +1/-1 | com_flush_input passes input_ready address as intptr_t rather than Windows long. |
| R0186 | +1/-1 | com_inb data-ready/non-loopback follow-up passes input_ready address at native pointer width. |
| R0199 | +1/-1 | modem_change passes modem_status address at native pointer width. |

The 121-line block retains original UART ownership: snapshot copies existing
private registers, interrupt causes, control/handshake flags, baud index,
critical counters, flush mask and transmit pacing. Restore validates critical
count and baud-index bounds, then copies back without replaying I/O. Original
port reads consume RX/status state and writes can cause output/scheduling;
register replay is not equivalent. Callback encoding maps existing recv_char
and do_wait_on_send function identities to stable archive IDs, and decoding
returns those original functions, not new device callbacks or serialized host
addresses. Actual compat/devices/archive.c consumes all four entries.

Fresh reads confirm host_com_ioctl takes lib_iptr and dereferences its output
argument as int*. Windows x64 long truncates addresses. Historical S1 P22
independently reverted each of the three casts and obtained x64 access violations;
x86 original/current controls passed. R0186 required restored host RX 51h/52h,
real com_recv_char and inb; its earlier non-covering passing test is not evidence.
Current checkpoint fixture still mutates/restores controller/host states and
asserts returned data and equality. No live physical serial input is claimed.

Historical coherent omission of serial callback encode/decode rejected actual
capture with a queued receive event on both architectures. This supports queue
identity persistence; it does not independently prove every captured field or
send callback. Snapshot restore tests, source side-effect analysis and real
callers support the controller copy. Not all UART settings, malformed states,
physical endpoints or manual guests were verified. No new runtime run this
round. Recommend retaining the five functional points. Owner subsequently rejects
the relocated notice in P20; it is not a retention exception. No new safe restoration identified, source/test/binaries unchanged.
Frontier 18/121; 103 remain. Next: order 268, comms/printer.c.

### S4 P19: com.c notice and snapshot minimization counter-review

Owner requests explanation and a minimal design only; no production change is
authorized or made by this investigation. Fixed XP places the exact Insignia
text in static char SccsID[] inside #ifdef SCCSID. Current selection excludes
that body with #error and preserves the text in an unconditional comment.
Deleting only the comment changes no compiled tokens, but removes this file's
remaining historical copyright identification. Literal restoration of the
conditional declaration is a separate excluded-body policy change, not merely
removal of the extra line. Source policy requires explicit owner direction for
notice changes; this report does not infer a license or runtime necessity.

The 121-line block was rechecked against its actual state and caller schemas:
5 declaration/separator lines, 23 callback encode/decode lines and 93 capture/
restore lines. Native ADAPTER_STATE contains legacy register unions/bitfields
and ints, whereas the archive uses fixed-width scalar fields. Whole-struct
memcpy is not established as representation-equivalent. Existing device wire
maps know archive offsets/widths, not native UART field mappings, so reuse would
require a new transfer mechanism rather than remove an existing duplicate.

Both transfer directions map the same 23 port fields plus critical counters,
flush mask and pacing; they do not introduce another live UART controller.
Deleting fields has no equivalence proof. Moving mapping into Compat requires
exposing private original storage or adding a bridge; moving equal code does
not establish a total-diff reduction. X-macros or offset tables can shorten
text but add a mapping abstraction and validation obligations; not recommended
for this bounded block under the owner's simplicity priority. Combining restore
validation with copying changes its existing check-all-before-mutate behavior.
Removing null guards/return contracts merely shifts preconditions to callers;
no need to change those contracts to save a few lines has been established.

Conclusion: no meaningful, behavior-preserving simplification established that
also reduces the mental model without new abstraction, ABI coupling or source
sites. Do not label every one of the 121 lines independently proven minimal;
retain the known evidence/coverage limits. Cosmetic line compression is not a
recommended repair. No new builds/tests; source/caller review only. Review
frontier remains com.c, no automatic retention approval or next-file advance.

### S4 P5: c_main.c necessity counter-review

Owner requests proof of necessity rather than consumer existence. No source is
restored by this review. A passing baseline suite is not an ablation proof.
Fresh source reads distinguish the following conclusions:

- Declarations (R0060/R0062-R0064/R0066-R0067): actual typed providers and
  selected calls establish C interface requirements. This is not proof of a
  reproduced runtime failure from every removed declaration.
- TLS selection (R0059/R0085/R0106/R0109/R0111/R0112): the current lifecycle
  outer-unwind uses ntthread frames; restoring only conditions splits the
  simulate/continue/unsimulate owner. Two setjmp expressions receive pointers
  to buffers and require matching argument types. An alternative coherent
  original static-stack implementation has NOT been proven impossible; it
  would require a separate lifecycle redesign, not removal of these lines.
- Snapshot/HLT (R0061/R0065/R0078-R0079/R0084/R0086/R0091/R0104 and nesting
  R0108/R0110): callers preserve private interrupt/trap state, halt/fetch phase
  and native nesting depth. Registers alone do not reproduce these states.
  Existing checkpoint tests cover real nested BIOS execution and retained HLT.
  Historical original-only outer-unwind comparison failed both architectures
  (S1 progress, original-only exit comparison); that proves the coupled unwind
  requirement, NOT independent necessity of every current line or TLS choice.
- Executor hooks (R0068/R0070-R0072/R0075/R0093-R0094/R0097/R0099/R0101):
  removing all consumption disconnects host work. Original quick_mode jumps
  directly to DO_INST; the added predicate provides an exit for pending host
  events. HLT needs its own consumption/wait. Ordinary-boundary versus direct
  NEXT_INST placement preserves different ordering. This is control-flow proof;
  there is no fresh per-site ablation matrix proving the three placements are
  the only minimal arrangement.
- Debug hooks (R0076/R0087-R0090/R0095-R0096): begin/refetch serves edits while
  paused; four early retirement edges bypass the ordinary site. Removing these
  loses existing observation/counting. No new all-instruction dynamic matrix.
- Map access (R0077): keyba.c:2148 writes the reset bit through the Compat
  macro; lifecycle clear is called from machine create/reset. Restoring just
  the function deletion breaks actual access. This proves an access requirement,
  not that a writable pointer is the uniquely minimal interface. Changing it
  needs coordinated callers and may introduce other mirror differences.
- Deassertion (R0107): existing c_cpu_interrupt only sets bits; platform.c
  consumes c_cpu_clearHwInt to clear the hardware bit. It cannot substitute
  for clear-all initialization or keyboard reset.
- PIC guard (R0100): rejection occurs before EXT/SYNCH_TICK; do_intrupt's later
  guard cannot undo those operations. No independent new negative control of
  this caller's side effects was run.
- Pacing (R0074/R0102): continuous-run setup enables a nominal 1,000,000
  counted boundaries/second, checking every 1024, with sleep/yield waiting.
  Original device heartbeat remains separate. Removing declaration/call would
  reduce three added mirror lines (including separator) but removes a real speed
  policy. No evidence establishes that this exact limiter is required for CPU
  correctness or is the simplest timing design. Reclassify as product-policy
  decision, not proven indispensable bug repair or safe equivalent restoration.
- Budget (R0073/R0103): cold entry goes through NEXT_INST before first fetch;
  budget one requests outer exit before any instruction executes. The existing
  observe_budget test explicitly expects initial decrement; the two-boundary
  JNZ historical probe executes one instruction. It is a safe-point budget,
  not evidence of precise retired-instruction count. Removal makes finite runs
  lose their termination mechanism, so is not equivalent restoration. Exact
  instruction semantics require a separate decision, not a new patch here.

Result: no unconditional behavior-preserving restoration candidate established.
It is also NOT established that every retained line is minimal or independently
negative-tested. Pacing is an explicit policy choice; budget naming/semantics,
map-interface shape and event-site minimization remain qualified decisions.
No new compilation/runtime test is claimed; no source, guest or media changed.
This report updates evidence strength, not owner approval or file frontier.

### S4 P6: owner-approved instruction-rate limiter removal

Owner: 好。去除这个限速器，减少其diff。 This authorizes removal of
R0074/R0102, not retention of other c_main points. Delete the declaration and
call/separator in c_main.c; delete the complete Compat limiter, four private
state variables, two constants, run-enable setup and idle-time compensation.
The existing platform failure fixture retains the signaled idle wait/count
checks and removes only the obsolete pacing-origin assertion/local block.

Production: c_main.c +0/-3 and Compat platform.c +0/-82, total +0/-85.
Test: platform_failure_smoke.c +2/-6, net -4. No public component, guest,
configuration, firmware, medium or snapshot schema change. Existing event
consumption, clock producer, finite-budget checks, HLT/keyboard event wait and
sound run-state notification remain on their existing paths. Continuous runs
are no longer capped by the removed instruction-rate policy; no claim of
unchanged guest speed or a measured performance multiplier is made.

The frozen S4 table is historical and is not silently rewritten: c_main's
selected diff changes +136/-16 to +133/-16 (three fewer added+deleted lines).
Independent normalized full-file Git Myers comparison with fixed XP gives
+284/-541 before and +281/-541 after, confirming the same reduction even with
excluded text retained. Original exclusion bodies are unchanged and uncounted
in the selected-code result. The current mirror manifest records the new body.
All removed limiter symbols have zero remaining src/test references; UART
transmit pacing and original quick-event timing are unrelated and untouched.

Dual cached builds/publication pass; full regression passes 392/392 on first
run (188 background plus eight native per architecture), no skips or retries.
Artifact hashes and verification boundaries are in the S4 evidence. Removed
R0074/R0102 are resolved by this explicit owner instruction; other points remain
unapproved. No real guest desktop performance comparison is claimed.

### S4 P7: owner-approved pending-event dispatch consolidation

Owner approves retaining the interrupt-map accessor and consolidating pending
event implementation in Compat. This is host/executor-boundary simplification,
not an original guest-CPU bug repair or approval of all other file differences.

Fixed XP c_cpu_interrupt only sets request bits; c_cpu_reset/c_cpu_init do not
clear the map, and clear_any_thingies clears only its timer bit. The accessor
serves unchanged original keyba reset-bit assignment and Machine cold-run
clearing before/after hardware reset. Replacing the keyboard write with an
original interrupt call would still need cold clearing and add a keyba diff.

The three dispatch positions remain: HLT loop, ordinary pre-PIC boundary and
NEXT_INST. Initial entry, STI, MOV/POP SS, IRET and debugger refetch can reach
NEXT_INST without ordinary interrupt checks. The ordinary path still traverses
both sites. One Compat function consumes one clock then one wake; no drain
loop, queue, state, retry or lock is added. Original single-bit timer signaling
cannot replace counted ticks or advance-free control wake. Both old consume
exports and declarations are removed, with no compatibility aliases.

Production relative to P6: c_main.c +4/-16, platform.c +10/-18, platform.h
+1/-0; total +15/-34, net -19. Tests +53/-2, net +51. Selected mirror diff
falls from +133/-16 to +121/-16: twelve fewer XP diff lines, not nineteen.
Frozen S4 tables stay unchanged; current manifest is updated. Excluded bodies
and line-control directives are untouched.

The existing platform fixture checks simultaneous pending kinds, one-tick
consumption, clock-before-wake observation, remaining ticks, empty dispatch and
real CCPU entry whose clock callback exits while wake remains pending. Existing
idle and HLT failure checks remain. Final run evidence is in the S4 audit.


### S4 P8: owner-approved finite-budget removal

Owner approves removing the c_main finite-budget difference and using existing
DEBUG trace/execution breakpoints for tests. This supersedes R0103's earlier
finite-run classification and the budget declaration within R0070-R0075.
The production driver always passed zero; nonzero callers were test-owned.
The old count measured NEXT_INST boundaries (including initial pre-fetch),
not completed instructions. It was not the product DEBUG trace counter.

Remove the mirror declaration and seven-line check, Compat counter/active flag
and idle bypass, and the machine_run budget parameter. Production remains a
continuous run stopped by its existing lifecycle. Tests bind the existing
machine/debug state, submit existing execution plans, and turn its completed
result into the existing stop request; there is no new instruction counter or
production API. Boot fixtures use existing execution breakpoints where their
completion PC is known; other instruction checks reuse TRACE. Actual DEBUG
command-provider integration remains covered by the full suite.

Baseline is P7 9ead43ec. Production +9/-45 (net -36), tests +106/-127 (net -21),
including the new 45-line test-local execution fixture. Fixed-XP full-file diff
changes +269/-541 to +261/-541: eight fewer added-plus-deleted lines, all removed
local additions. Selected-content diff is +113/-16 after P6/P7/P8; frozen S4
statistics remain unchanged. No #error body or public package is modified.
Qualification and actual-diff review are recorded in the S4 audit evidence.


### S4 P9: owner-approved original simulation-depth authority

Owner accepts P8 and approves implementing the investigated depth unification,
including the independent unused declaration cleanup. This expressly accepts
an architectural reduction rather than claiming fewer total mirror diff lines.
R0108/R0110 enter/leave calls are removed. The actual original ThreadSimBuf.level
in ntthread.c becomes the only nesting counter; a read-only query serves Compat
checkpoint, resume and exit checks. Ordinary BOP return and outer stop retain
their distinct existing original unwind paths. Snapshot encoding is unchanged.

The added ntthread query is nine lines including its comment/separator; safe
TLS detachment changes four lines and removes the original free line (five
added/deleted diff lines). Failure reports the native error and retains the
still-referenced allocation while independent thread registration is removed. No automatic retry or new lifetime manager is introduced. c_main loses
the two hooks and P8's leftover declaration: -3. Total fixed-XP mirror diff thus
increases eleven lines; depth unification alone increases twelve, while the
independent declaration cleanup removes one. This approval is limited to this
measured tradeoff, not inherited approval of all c_main or ntthread differences.

Relative to accepted P8 5d3f3b86, five production files are +18/-25 (net -7),
three existing test files +24/-5 (net +19). Tests replace fake depth entry with
real execution and observe original depth during nesting, HLT resume, outer stop
and worker teardown. Evidence and full delivery results belong to the S4 audit.
The later ntthread file's complete manual review is not claimed by this coupled
investigation; the ordered review frontier remains c_main, file 11 of 121.


## S3 approval progress

**Superseded by owner revocation:** all retention approvals and automatic
delegation recorded below are withdrawn. The revoked S3 approval count is zero; fresh S4 decisions are recorded above.
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
failure or malformed archive combination. Owner approves R0496 in P60, with
the four unused measure assignments removed: current extension +226/-0. Its
separately audited header R0442 is not a competing implementation.

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

## S4 P20: correction of excluded-branch notice treatment

Owner explicitly rejects P19's recommendation: a wholly excluded SCCSID body
is replaced by #error; its contents must not be extracted into an external
comment. Delete only the relocated com.c line, leaving the conditional
framework, exclusion and necessary diagnostic anchor intact. R0173 is retired
from current retention review, not treated as a separate excluded-body audit
item. P19 remains historical evidence of the corrected recommendation.

Production +0/-1, no test changes. Selected XP diff becomes +125/-3 (128 lines),
one fewer than the frozen +126/-3. Five functional review candidates remain
pending. Same reasoning applies when later files contain extracted excluded
content; historical notice classification alone cannot authorize its retention.
This direction concerns extracted excluded content, not unrelated original
notices or any licensing claim. No change to the UART snapshot implementation.

## S4 P21: remaining com.c evidence and concrete reduction candidate

Scope is the current file's remaining five functional points, not all unreviewed
mirror files. No source changes or new dynamic tests. R0181/R0186/R0199 each
have independent historical x64 negative controls and live pointer consumers.
R0174 supplies actual snapshot types/constants; its necessity is a compile/data
contract, not a guest-failure claim. R0178's receive callback route has coherent
dual capture-failure controls; send callback and individual state fields do not
each have independent negative controls. Existing real-state mutation/restore
assertions and original-field consumers support them, without exhaustive proof.

Fresh source sweep finds all 23 copied port fields still referenced outside the
snapshot block. In particular had_first_read controls the one-time RX flush;
com_baud_ind selects delay-table entries; cached DTR/RTS/OUT states participate
in transition detection; the separate interrupt reasons feed arbitration.
Reference counts alone are not proof that every field is irreducible. Restore
range checks precede mutation; merging validation and copying would change
failure behavior. No equivalent field deletion or native-struct memcpy has
been established.

A genuine small reduction supersedes P19's no-established-reduction result:
move the existing added snapshot block to file end, after all original
variables and declarations, and remove its three added forward declarations
(recv_char, do_wait_on_send, tx_pacing_enabled). The original declarations and
definition already provide them before that location. Keep all four added
functions otherwise identical, original source order intact and no new #line
needed after appended functions. No interface/schema or build-policy changes.
This is one relocated added block, not extra patches to original functions.

In-memory candidate only: 121 added block lines become 118. Expected selected
XP diff +125/-3 -> +122/-3, reduction three lines. Raw SequenceMatcher changes
+442/-2590 -> +425/-2576 because block movement changes matching alignment;
that raw 31-line apparent reduction is NOT 31 lines of simplification and does
not count excluded-body content. No candidate was written or compiled. Dual
compile/link plus object/full-artifact comparison is required before claiming
equivalent output; existing checkpoint regression applies if output differs.

Do not compress mappings into macros/tables or combine statements merely to
reduce printed lines. The 93 capture/restore lines map different representations;
this review establishes no larger safe reduction under unchanged behavior and
snapshot schema. Present the three-declaration candidate for owner decision;
retain evidence limits rather than declaring all remaining lines independently
proven minimal. Review frontier unchanged.

## S4 P22: remove only added duplicate UART declarations

Owner approves the P21 candidate and explicitly limits cleanup to added mirror
diff: original duplicate declarations remain allowed and untouched. Relocate
the added snapshot block to file end, remove only added recv_char,
do_wait_on_send and tx_pacing_enabled forward declarations, and reuse the
original declarations/definition. All four snapshot function bodies remain
unchanged. An exact source comparison reconstructs the new file as only this
move-minus-three-lines; no other original text or exclusion directives change.

Production one file +118/-121 (net -3), tests unchanged. Added block 121 ->118;
selected fixed-XP diff +125/-3 -> +122/-3 (128 ->125 changed lines). Frozen
file list remains historical. R0174 and the remaining R0178 body retain their
pending evidence-qualified dispositions; the three pointer corrections are
unchanged. No snapshot schema, ABI or runtime ownership change.

Same-file similar-issue search covers added declarations and all uses of the
three names. The surviving forward declarations and tx_pacing definition are
original, precede the moved functions, and each removed declaration has its
original provider. This is not a blanket cleanup of original mirror style or
other unreviewed files.

Cached dual full builds and publication pass. Complete EXE comparison has
nonmetadata differences, so artifact-equivalence exemption is not used. An
isolated before-source compile at each width uses the actual production flags;
old/new symbol tables confirm changed function positions. For example x64
com_recv_char moves 0x2530 ->0x1da0 while snapshot encode moves 0x1da0 ->0x2300.
No behavior proof is inferred from layout alone; full runtime qualification is
recorded in the P22 audit evidence.


## S4 P23: com.c accepted; printer.c review 19 / order 268

Owner says "好 批准 下一个文件" after P22 delivery: com.c reviewed retained
points R0174/R0178/R0181/R0186/R0199 and P22 cleanup are accepted. Removed
notice/declarations remain removed. Advance presentation to printer.c only.

Fresh comparison against pinned XP SP1 finds three nonexcluded text points,
+74/-0 (line-control directives excluded), matching the frozen row:

| Point | Current anchor | Size | Purpose and disposition |
| --- | --- | --- | --- |
| R0206 | printer.c:69 | +1/-0 | Extracted SCCSID notice outside excluded branch; same issue as com.c P20. Recommend removal under owner's existing excluded-content rule; not a runtime capability. |
| R0207 | printer.c:106 | +1/-0 | Snapshot declaration include, consumed by four functions below. Retain recommendation pending review. |
| R0212 | printer.c:228-299 | +72/-0 | Private parallel controller snapshot and semantic event callback mapping, including two added forward declarations. Retain capability; two declarations can be avoided by moving extension after original definitions within PRINTER scope. |

The actual root CMake compiles printer.c; host/inc/host_def.h defines PRINTER.
Original selected lpr_state_outa_event/lpr_state_out_event definitions are at
current lines 374/380 (XP 351/357). The added declarations are not original
mirror duplicates. Relocation must remain within the original PRINTER guard;
no original definition or original duplicate declaration should be removed.
Candidate reduction: one extracted notice plus two added declarations, reducing
selected XP +74/-0 to +71/-0. No production modification in this review.

The original controller owns output/control/status, READY/OUT/OUTA, event handles
and retryErrorCount. Compat parallel.c instead owns host output buffering and
resource state. archive.c actually calls both layers and registers the two
callback mappings. Reading status advances ACK/state; writing control can schedule
output, so port replay cannot restore the private state without side effects.
Moving these static arrays to Compat or duplicating them is not a simplification.

Current checkpoint fixture first mutates/restores controller and host state,
then writes 33h, captures a pending printer ACK, changes data to 00h, clears the
queue, restores archive, asserts actual data-port 33h and dispatches the event.
Historical S1 P23 independent controls showed missing callback mapping fails
capture and missing controller restore fails the data assertion on both widths.
Those negative controls were read, not freshly rerun. Current P22 full 392/392
qualification includes this checkpoint test. No physical printer, exhaustive
ACK-state/timing matrix or manual guest print-job proof is claimed.

Source review finds no original-device bug repair here: remaining functional
diff implements snapshot capability, not printer emulation replacement. Four
function bodies need no new abstraction or second path. No new runtime test or
source change for this documentation-only review. Frontier 19/121, 102 remain;
next is order 269 comms/printer_.c. Candidate cleanup awaits this file's review.


## S4 P24: approved mirror-wide same-issue cleanup

Owner approves printer.c cleanup and requests the same added-diff classes be
resolved across the mirror, with explicit point/line reduction accounting.
Original mirror duplicates remain untouched. The finite 472-file sweep,
candidate dispositions, before/after per-file counts and qualification are in
[P24 evidence](m9-t100-s4-audit.md#p24-mirror-wide-added-diff-cleanup).

The 18 remaining extracted notice points R0206, R0238, R0244, R0413, R0416,
R0418, R0419, R0456, R0461, R0462, R0468, R0473, R0487, R0488, R0495, R0501,
R0507 and R0560 are removed under the owner's explicit excluded-content rule.
R0173 was already removed by P20. This supersedes the historical notice-retention
recommendations above, without changing original unconditional notices or making
any license/distribution claim. Existing excluded bodies receive no new entries.

Three added-function placement groups are cleaned: two declarations in printer.c,
one in fla.c, and one in nt_sbdsp.c. Original callback definitions, snapshot
bodies, state ownership, failure checks and dependencies remain. Required cross-
file declarations and the DspCompleteBlock scheduling cycle are retained with
specific evidence, not characterized as redundant.

Result: 21 cleanup points (18 notice points and three declaration groups),
22 added lines removed. Counting declarations individually gives 22 removed
line-level sites. Full textual hunk counts are not semantic point counts.
Selected differences in the 19 changed files: +1712/-1072 -> +1690/-1072.
Git source movement: +99/-121, net -22; tests unchanged. Seven files whose only
selected difference was the removed notice now match XP outside exclusions:
printer_.c, rs232_io.c, gfi.c, gfi_mpty.c, keybd_io.c, dummy_nt.c and host_hfx.h.
Frozen inventory statistics are not rewritten. The ordered semantic frontier
remains review 19; unrelated points in later files are not deemed reviewed or
approved by this same-issue sweep.


## S4 P26: current printer.c per-function counter-review

At accepted P24 source, printer.c has two selected text blocks, +71/-0:
R0207 include +1; R0212 extension +70 (65 function lines and five separator
lines). No original printer-function body is changed by these surviving points.
All are snapshot integration, not original printer bug repairs.

| Current source anchor | Lines | Purpose / retention evidence |
| --- | ---: | --- |
| 105 snapshot.h include | 1 | Direct authoritative state types and capture/restore declarations consumed here; no second schema. |
| 806 encode_parallel_callback | 14 | Map two original static ACK callback addresses to stable archive IDs; actual archive encoder consumes it. Historical coherent removal fails capture at both widths. |
| 821 decode_parallel_callback | 9 | Resolve IDs back to these same original callbacks after queue reset/restore. Current real archive regression restores and dispatches the pending event; no raw persisted function pointer. Historical mapping-removal control tests the pair, not decode alone. |
| 831 capture_parallel_controller | 17 | Copy actual private output/control/status, READY/OUT/OUTA, two queue handles and retry count. Actual archive capture calls it; current fixture changes controller state after capture before comparing restore. |
| 849 restore_parallel_controller | 25 | Validate all adapters before mutation, then restore the saved private controller state. Historical independent omission fails actual data-port 33h assertion after changing it to 00h, on both widths. |

The original callback definitions remain at lines 301/307 and schedule the
second ACK event using the original queue. Status-port reads can consume ACK
progress and cancel queue events; writes can schedule output. Therefore replay
of I/O is not an equivalent restoration. Compat parallel.c owns host buffering,
not these static controller variables; moving their ownership out or adding a
second state copy would not remove the capability requirement. Native layout
also differs from fixed archive types (controller state byte versus int32,
queue handle versus int64), so one raw memcpy is not an equivalent replacement.

Current checkpoint_smoke.c lines 751-767 checks controller/host round-trip;
775-790 captures an actual pending ACK, changes data, resets the queue, restores,
reads 33h and dispatches. Final P24 392/392 includes these cases. This review
freshly reads code and tests; S1 P23 failure controls remain historical and are
not represented as freshly rerun. No independent ablation of every field,
negative check, each callback stage or physical printer is claimed. Existing
restore validation does not claim exhaustive hostile-snapshot validation.

Recommendation: retain all five functional pieces. The include plus four
functions form one existing snapshot path; no remaining same-class duplicate
forward declarations or extracted notice. No further behavior-preserving
reduction is established by this review. No production changes or runtime rerun.


## S4 P27: printer approved; ev_glue.c ordered review

Owner explicitly approves retaining all five printer.c pieces in P26 (R0207,
remaining R0212). R0206 and the removed added declarations stay removed.
Skip frozen reviews 20/21, printer_.c and rs232_io.c: P24 removed their only real
selected differences. Next substantive file is frozen review 22 / order 275,
base/cvidc/ev_glue.c. Compare current source to pinned XP: three text points,
+3/-2, unchanged since the frozen count. Existing excluded body is not audited.

| ID / current anchor | + / - | Purpose | Evidence / limit |
| --- | --- | --- | --- |
| R0254 line 30 gdp_slots.h include | 1 / 0 | Native-width GDP access contract | setup_vga_globals uses mapped VGAGlobals and three SubrRing slots. Original fixed offsets 8/12/16 overlap eight-byte pointers. Must be assessed with R0263. |
| R0255 line 35 angle-bracket Evid_c.h | 1 / 1 | One existing CCPU aggregate layout | Current compile include order selects base/ccpu386 before base/cvidc; quotes instead select the adjacent C-VID header. video_base_ls0 is a pointer while video_base_lin_addr is IU32. Historical actual x64 VGA control crashes when only quoted selection is restored. |
| R0263 line 793 softpc_gdp_create | 1 / 1 | Construct the same GDP owner consumed by mapped slots | Original 64 KiB allocation is not this slot carrier. Historical coherent raw-allocation plus direct-macro restoration fails two of three ring pointer checks on x64; x86 passes. |

Fresh source inspection checks actual dual compile flags/include order, original
ring offsets, current slot mappings/create/destroy, EGA_CPU.globals assignment,
and machine teardown's softpc_gdp_destroy_global. The owner stores native-width
slots separately and has matching destruction; it is not an extra tracing or
CHAIN2 implementation. Original Gdp/j_Gdp declarations/definitions remain.

Historical S1 extended controls used actual initialization bodies, restoring
both allocation and original direct macros together: x86 current/original all
three pointers match; x64 current 1/1/1, original ptr/low/high 0/0/1. This proves
initialization ABI overlap, not a full original guest run. Header measurements:
x64 CCPU/CVID aggregate size 240/232, later offsets 168/160 and 236/228; x86
both 156. Quoted-header actual VGA test passes x86, x64 exits 3221225477.
Those negative controls were not rerun this review. Current P24 392/392 includes
current video tests; it does not prove every generated rule or hypothetical
coherent rewrite. No evidence justifies restoring one consumer inconsistently,
or duplicating/changing the aggregate family merely to remove an include diff.

Recommendation: retain the three points as native ABI/structure ownership
adaptation, pending owner approval. No production changes or fresh runtime run.
Next real-diff file is order 283 base/cvidc/sevid000.c. This review does not
approve that generated-code family or assert its every patch is minimal.


## S4 P28: approved ev_glue.c and remove zero-diff files from current list

Owner explicitly approves retaining ev_glue.c R0254, R0255 and R0263, then
requires removing all no-longer-existing differences from the current list.
The current real-difference Markdown inventory now contains 114 files and
P25's reconciled per-file counts. Remove the seven zero-real-diff rows:
printer_.c, rs232_io.c, gfi.c, gfi_mpty.c, keybd_io.c, dummy_nt.c and host_hfx.h.
Original directory ordinals remain; frozen TSV evidence is historical, not a
pending task list. Existing excluded bodies receive no new entries. This does
not approve unreviewed differences or remove files merely because retention
has been approved. Next real-diff file remains order 283 sevid000.c.
Documentation-only; no source/build/runtime changes. Documentation and
whitespace checks pass, with no disposable build work.


## S4 P29: approved vglfunc.c and reviewed diskbios.c

Owner explicitly approves R0412 in order 318 base/cvidc/vglfunc.c: +5/-0,
local IFN1 declaration adaptation. Status: approved retention. Owner directs
skipping the intervening generated cvidc files; they remain unreviewed and
unapproved in this S4 review.

Order 319 base/disks/diskbios.c is compared again with fixed XP SP1
a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Exclusion bodies and necessary line controls receive no accounting.
Three remaining textual points total +4/-4:

| Current / XP physical line | Size | Classification and disposition |
| --- | --- | --- |
| 293 / 441 | +1/-1 | NTVDM opening directive loses the original trailing True for all NT VDMs comment. No behavior or demonstrated retention need; recommend restoring original directive text. |
| 295 / 452 | +1/-1 | NTVDM closing directive changes the tab before its comment to a space. Formatting only; recommend restoring original whitespace. |
| 904-905 / 1663-1664 | +2/-2 | R0414 corrects BIOS AH=15h capacity split from UCHAR/base-256 to full CX:DX/base-65536. Recommend retain, pending owner approval. |

The first two points are surviving directive text, not accounting for the
excluded body. No source is changed by this review. If restored later, only
R0414 (+2/-2) would remain. R0413's extracted notice was already removed by
P24 and is not a current decision.

Current source confirms BIOS disk_io command table entry 0x15 -> read_dasd,
getdpb geometry, unchanged reserved-cylinder calculation and AH=3/CF=0.
Current device_contract_smoke verify_disk invokes BIOS[BIOS_DISK_IO] and
asserts capacity pairs 0000:0484, 0001:0000 and 000f:bc10, plus absent-drive
rejection without changing untouched registers. No Compat capacity substitute.
Historical S1 P68 used identical fixtures/linked objects with both assignments
restored together: current x86/x64 passed, XP controls both failed the first
CX assertion. XP's 1156-sector result CX=4/DX=132 is source calculation; the
negative test stops at CX. These historical controls were not rerun here.
Current review proves source/caller consistency, not a fresh guest boot or
exhaustive BIOS test. P24 full 392/392 remains historical regression evidence.
Documentation-only P; no production, tests, EXEs, media or snapshot changes.
Next file after owner review is order 320 base/disks/fdisk.c.


## S4 P30: approved R0414 and bounded nonsemantic sweep

Owner approves R0414 retention (+2/-2), authorizes restoring diskbios.c's
nonsemantic directive differences, and extends that cleanup class to all 114
current real-diff files. R0414 is approved; no other semantic decision is
inferred from this batch. The current inventory removes five newly zero-diff
files and updates all changed counts. Restored formatting points are no longer
pending decisions; frozen counts and older narratives remain historical.

The [P30 audit](m9-t100-s4-audit.md#p30-nonsemantic-restoration-across-114-files)
records coverage, restored groups, retained semantic-context indentation,
verification and delivery. Source bodies under existing #error remain outside
scope. The generated cvidc family is scanned for this formatting class only;
its outstanding semantic review has not been approved or completed.


## S4 P31: fdisk.c current per-piece review

Order 320 base/disks/fdisk.c, fixed XP SP1 comparison: two real text blocks,
+120/-0, all R0415 snapshot integration. Existing exclusions/line controls are
outside accounting. Original retained device algorithms remain unchanged.

| Current physical lines | Added | Purpose and actual receiver |
| --- | ---: | --- |
| 236 | 1 | compat/devices/snapshot.h: shared archive schema and callback IDs. |
| 1896-1910 | 15 | Encode original fdisk_pause / disk_int_call_back to stable event IDs; device archive event codec consumes it. |
| 1912-1921 | 10 | Decode those IDs to the same original callbacks; unknown IDs return NULL. |
| 1923-1926 | 4 | Stable active-command ID table: null plus nine original handlers; no new command implementations. |
| 1928-1937 | 10 | Map activecmd pointer to archive ID, reject unknown pointer. |
| 1939-1947 | 9 | Map checked archive ID to original handler. |
| 1949-1979 | 31 | Capture original private drives/taskfile/sector buffer/registers/index/selected-drive/activecmd state. |
| 1981-2014 | 34 | Validate pointer IDs and restore that same private state without executing device I/O. |
| Between added items | 6 | Separating blank lines; included in +120, not another behavior. |
| Total | 120 | No deleted original retained lines. |

Fresh source tracing: machine/snapshot_image.c -> compat/ccpu/archive.c ->
compat/devices/archive.c -> capture/restore_hdd; archive's event codec also
calls both HDD callback codecs. hdd_drive_fields and hdd_fields serialize all
listed scalar/array fields. The original rsector schedules fdisk_pause, which
calls activecmd(CONTINUE); irq schedules disk_int_call_back. These are real
production users, not test-only exports. Register reads have side effects
(including interrupt deassertion), so port replay is not an equivalent archive.

Original dispatch[] is keyed by command high nibble and contains duplicates;
wd2010 separately selects diagnose/setparams/bad. It is not the ten-entry
snapshot ID mapping. Direct substitution changes persisted IDs and cannot
represent every current activecmd; raw pointers are not portable archive data.
The table is identity encoding, not a duplicate command-execution path.
Keeping the narrow accessors here preserves ownership of private device state.
Exporting original locals or moving their implementation to Compat would
introduce different mirror changes/ownership, not recover original semantics.

Current checkpoint_smoke lines 683-705 writes taskfile ports, captures, changes
them, restores and compares taskfile bytes. It round-trips all ten active-command
IDs and rejects -1/10. Both architectures passed this existing fixture in P30's
392/392 full suite. Historical S1 P69 also passed focused x86/x64 checks;
this P does not rerun them or claim a new removal-negative control. No evidence
here proves every drive field, partial sector transfer or delayed callback
interleaving individually, nor every DOS/Win3.1/Win95 snapshot scenario.
Callback/private-state necessity is source/caller evidence; tests prove the
stated register and ID behaviors. Keep those evidence strengths distinct.

No independent format/declaration or dead addition was found for restoration.
The original has no equivalent snapshot interface. Removing all additions and
callers removes HDD checkpoint capability, rather than preserving behavior.
Recommendation: retain R0415 pending owner approval; not a claim that every
helper spelling is uniquely minimal. No production/test/manifest/EXE change.
Documentation/manifest/whitespace checks pass. Next manual file after owner
decision is order 321 fla.c; skipped generated files remain unapproved.


## S4 P32: owner approves fdisk; fla.c current per-piece review

Owner explicitly approves retaining R0415 in fdisk.c. Its P31 pending decision
is now resolved as approved; no source change follows from that approval.

Order 321 base/disks/fla.c, pinned XP SP1 comparison: three real text blocks,
+97/-0, all R0417 FDC snapshot integration. Existing #error bodies and necessary
#line scaffolding are excluded without new accounting. Retained original
controller commands, NDMA logic and interrupt scheduling are unchanged.

| Current physical lines | Added | Purpose and actual receiver |
| --- | ---: | --- |
| 77 | 1 | Include shared snapshot state schema and stable event IDs. |
| 224-255 | 32 | Capture original private command/result blocks, four SIS slots, 8192-byte NDMA buffer, registers, flags and transfer counters. |
| 257-299 | 43 | Validate archive bounds/selectors/flags before writes, then restore the same private controller state without issuing I/O. |
| 1179-1188 | 10 | Encode original fla_int_call_back as SOFTPC_DEVICE_QUEUE_FDC_INTERRUPT; reject other callbacks/null output. |
| 1190-1196 | 7 | Decode that stable ID to the same original callback, otherwise return NULL. |
| Between added items | 4 | Separating blank lines included in the total. |
| Total | 97 | No deleted retained original lines. |

Fresh source evidence: root CMakeLists.txt includes fla.c in production.
Machine snapshot_image -> Compat CCPU archive -> device archive reaches
capture_fdc/restore_fdc (archive.c lines 157/226). Its fdc_fields table at 442
serializes every captured field. The archive event codec at 66/92 calls both
FLA callback codecs. The selected non-NTVDM original fla_atomicxqt schedules
fla_int_call_back with add_q_event_i at 861; that original callback at 810
asserts the hardware interrupt. Thus neither the state helpers nor callback
codecs are test-only. Stable IDs are needed instead of persisted code pointers.

These additions preserve the controller, not the mounted image: command/result
phase, pending sense results, partial NDMA data and delayed IRQ are not encoded
by disk contents. Reading/writing device ports is not an equivalent state copy:
reads consume result/data bytes and writes execute commands or reset state.
The original private variables remain the sole live device state; archive data
is a saved representation. Moving helpers into Compat would require exposing or
moving those locals and introduce different mirror changes. Raw struct dumping
cannot replace explicit wire-field mapping across ABI/layouts. No original
snapshot interface exists to substitute for these additions.

Current checkpoint_smoke.c lines 672-682 writes DOR and SPECIFY through actual
ports, captures, resets DOR, restores and asserts DOR/current-command/command-
count equality. This unchanged fixture passed on both architectures in P30's
392/392 run. Historical S1 P70 cited the older P69 test execution; it is not a
new removal-negative control. This review performed no fresh runtime tests.
Tests do not individually prove all NDMA bytes, SIS slots, malformed fields or
pending callback timing, nor all DOS/Win3.1/Win95 snapshot interleavings. Those
remaining fields/codecs have current source/caller evidence, not exhaustive
dynamic proof.

P24 already removed the extracted notice R0416 and redundant added callback
forward declaration by placing the codecs after the original definition.
Neither is an outstanding diff or retention decision. No further independent
formatting, duplicate declaration or unused addition can be restored to XP
while preserving the existing snapshot contract. Recommendation: retain R0417,
pending owner approval. No claim that every helper spelling is uniquely minimal.
Next real-diff file is order 335 base/inc/cpu_vid.h; intervening zero-real-diff
files need no new manual review. Skipped generated files remain unapproved.
Production/tests/EXEs unchanged; documentation, manifest and whitespace gates
are the verification for this documentation-only delivery. S4/T100 stay open.


## S4 P33: owner approves fla; cpu_vid.h conditional residue

Owner approves retaining fla.c R0417. P32's pending decision is resolved as
approved. P32 documentation was committed/pushed after explicit publication
approval; no production change was made.

Order 335 base/inc/cpu_vid.h compared in full with the fixed XP SP1 file.
Current inventory: two text blocks, +3/-0, representing one semantic review
item: an added conditional wrapper around the unchanged READ_POINTERS b_read
and w_read declarations. Current lines 87/91/93 are the added #if/#else/#endif.
Its #error body and line controls are not separately counted. The original
!CPU_40_STYLE excluded branch remains out of scope without new accounting.

XP declares both IU32-returning function pointers unconditionally. Local
selected declarations are now exactly XP after P76. The old selected signature
and provider declaration patches R0421-R0423 were already restored by P76,
P123 and P125; they must not be relabelled pending or approved for retention.
However the wrapper itself is not in pinned XP, so the earlier broad statement
that only exclusion scaffolding remained did not resolve this real +3 residue.

Current CMakeLists.txt lines 257/261/264 and 462 explicitly select CPU_40_STYLE
and C_VID. Under this supported configuration, removing the wrapper preserves
the two fields, their order, return type and argument macros exactly. No x64,
snapshot or device behavior depends on this added branch. It merely rejects
other macro combinations, and no necessary consumer contract was found to
justify that extra mirror restriction. No OpenNT source was consulted.

Recommendation pending owner approval: restore the original unconditional
pair of declarations by removing this added wrapper as a whole, including its
now-orphaned #error and immediate #line 90. Keep the original surrounding
structure and other existing excluded branches/line controls. This is removal
of an added non-XP wrapper, not restoration or auditing of excluded code.
Expected production +0/-5 physical lines; real XP diff +3/-0 -> 0/0, two text
blocks removed. The file would then leave the current real-diff inventory.
No source edit, build or runtime test has yet been performed. This is exact
selected-source reasoning, not dynamic evidence. An implementation must verify
the actual dual build inputs and artifact equivalence under the existing rules.

Documentation-only review gates apply here. Next file after the owner decision
is order 337 base/inc/dma.h. S4/T100 remain open.


## S4 P34: owner-authorized conditional-wrapper sweep

Owner authorizes restoring cpu_vid.h and cleaning all equivalent cases across
the real-diff file list. The [P34 audit](m9-t100-s4-audit.md#p34-redundant-non-xp-conditional-wrappers)
records all 109 files' bounded scan, twelve candidate-file dispositions, eight
changed paths and ten removed wrappers. cpu_vid.h and gmi.h now have no real
retained-content diff and are removed from the current actionable list.
The P33 restoration candidate is resolved. R0421-R0423 and R0433 remain restored,
not retention requests. Real type, endian, snapshot and host wiring changes
remain visible and subject to their per-file review; this batch does not approve
them wholesale. Existing original excluded bodies receive no new accounting.
Next manual file remains order 337 dma.h; S4/T100 remain open.


## S4 P35: owner accepts P34; dma.h review

Owner accepts P34 and asks to continue with dma.h. This accepts the delivered
wrapper cleanup, not the remaining unrelated semantic differences in those files.

Order 337 base/inc/dma.h: one retained text point, +3/-0, R0424. At current
physical lines 497-499 the addition is one reason comment, the declaration
DMA_ADAPT *dmaGetAdaptor(void); and a separating blank line. Existing #error
bodies and necessary #line are excluded without new accounting. No DMA register
layout, bitfield, transfer macro or constant differs outside those exclusions.

Pinned XP declares DMA_ADAPT *dmaGetAdaptor(); at original line 575 inside
NTVDM. The current standalone build does not define NTVDM. Exposing this
existing pointer-returning accessor to the retained original DSP is sound/DMA
integration and a compiler type contract, not a new DMA engine or snapshot API.

Fresh production tracing: compat/platform.c:669 -> SbInitialize in nt_sb.c ->
SbInitializeDMA at nt_sb.c:191 / nt_sbdsp.c:744 -> dmaGetAdaptor. nt_sbdsp.c
includes dma.h and stores the result in pDmaAdp, then selects pDcp from the
original controller array. at_dma.c:878 returns &adaptor, the original single
DMA owner. DSP QueryDMA, GetDMATransferAddress and SetDMACountAddr read/update
that owner's address/count/page/mode/status (current nt_sbdsp.c:1947-2097).
Both original source files are actual root-CMake production inputs. Whole-tree
search finds one declaration, one definition and one direct call; no duplicate
Compat declaration/accessor offers an equivalent restoration.

Historical S1 evidence (P77) used the actual nt_sbdsp compile flags plus explicit
-Werror=implicit-function-declaration: current header compiled on both widths;
removing this added declaration/reason failed both with implicit declaration
and int-to-DMA_ADAPT-pointer assignment. That added warning-as-error flag is
part of the control, not the unchanged production policy. It proves the missing
pointer-return declaration, not an observed guest crash or pointer truncation.
This P performs no fresh compiler or runtime experiment. P34 rebuilt both full
test targets and proved artifact equivalence; P30's full 392/392 remains the
historical runtime baseline. sound_smoke has real controller/DSP DMA coverage
with a mock audio platform, not fresh desktop or audible playback evidence.

Counter-review: deleting only the declaration loses the return-type contract;
removing its caller removes working DSP controller access. Defining NTVDM just
to expose it selects unrelated unsupported host paths. Moving a duplicate extern
into nt_sbdsp.c or Compat merely relocates the diff and weakens ownership. Keeping
the declaration in the original DMA owner's header is the narrow coherent seam.
The one-line reason is the required local provenance explanation, and deleting
its separating blank is not a functional simplification. No safe equivalent
XP restoration identified. Recommend retaining R0424, pending owner approval.
No production/test/manifest/EXE changes. Documentation/manifest/whitespace gates
apply. Next real-diff file is order 339 egacpu.h, now +4/-1 after P34; S4/T100
remain open.


## S4 P36: owner approves DMA declaration; egacpu.h review

Owner explicitly approves retaining R0424. The P35 pending decision is resolved.
Order 339 base/inc/egacpu.h currently has one retained text block, +4/-1, R0426:
three explanation lines plus struct VGAGLOBALSETTINGS *globals replace the
original VGA_GLOBALS *globals at XP line 153 (current physical lines 112-115).
Existing exclusions and necessary line controls are not counted or reopened.
P34 already removed its redundant C_VID wrapper and shortened the stale comment.
R0425's forward tag and R0427's changed include placement/spelling were restored
in P78 and are not outstanding differences.

This is a C declaration/type correction at the existing video boundary, not a
new renderer, snapshot API, state owner or pointer-width expansion. Both old
and new declarations are native pointers of the same size. The pointed-to
structures are different C types: the original VGA_GLOBALS view uses ULONG,
function-pointer fields and a shorter member set, while the generated
VGAGLOBALSETTINGS carrier has the selected generated field types and members.

Fresh production source proof: machine/machine.c:153 calls setup_vga_globals;
ev_glue.c:766 assigns EGA_CPU.globals = &(GLOBAL_VGAGlobals). The pinned XP
source contains the same assignment at line 765, so this is not invented wiring.
The actual GDP macro in compat/cvidc/gdp_slots.h:632 yields a struct
VGAGLOBALSETTINGS object; pinned XP's base/ccpu386/evid_c.h also defines that
generated struct at line 59. The declared member should describe that object.
Whole-repository search finds the assignment and the snapshot precondition
null check in vga_prts.c:3163 as direct EGA_CPU.globals uses; no direct field
access through the old pointer is claimed as a proven runtime failure.

Historical P78 compiler controls used actual ev_glue/vga_prts commands on both
architectures. Restoring the original VGA_GLOBALS pointer produced an
incompatible-pointer diagnostic at the assignment and exit 1 with explicitly
added -Werror=incompatible-pointer-types; current type passed. vga_prts still
passed with the old type because it only checks NULL. This is precise compiler
contract evidence, not observed corruption or a claim that production compiler
flags necessarily stop at that warning. P78 also completed a historical full
392/392 run. P34's latest cached builds and whole-artifact equivalence passed;
this read-only P does not rerun experiments or claim fresh runtime acceptance.

Counter-review: reverting the field restores a known mismatched declaration.
Adding a cast at the original assignment merely hides that mismatch and moves
or adds a mirror patch. Changing the generated provider or deleting the field
would expand scope and disturb the existing initialization/null-check contract.
No second carrier, conversion or conditional path is needed. The current one-
line type correction plus required reason is the narrow truthful declaration.
No further safe original restoration was identified under the current type-
correctness standard. Recommend retain R0426, awaiting owner approval; do not
classify it as a proved guest-display bug repair or snapshot-only capability.

Documentation/manifest/whitespace gates apply; production/tests/EXEs unchanged.
Next real-diff file is order 342 base/inc/egaports.h. S4/T100 remain open.


## S4 P37: owner approves generated carrier; egaports.h review

Owner explicitly approves retaining R0426; the P36 pending decision is resolved.
Order 342 base/inc/egaports.h has one real text block, +1/-0, R0428:
extern int ega_get_pel_panning IPT1(int,line), physical line 29. Fresh pinned
XP SP1 comparison confirms no other retained-content difference. Existing
excluded bodies and necessary line controls are skipped without accounting.
R0429's historical explanatory comment was already removed in S1 P80.

Purpose: declaration supporting the VGA horizontal pixel-panning repair, not
snapshot state, a second renderer or a second register owner. Current provider
vga_prts.c:2605 reads attribute_controller.horizontal_pel_panning (low three
bits), with the existing split/pan-mode rule returning zero below the split.
Actual nt_ega.c consumers at 621, 1065 and 1301 use the per-line result to
compose panned plane groups. gfx_updt.c:1863 uses the same query for dirty
neighbour marking. Root CMake includes all three files (203, 210, 218).
Whole-source search finds one declaration and one implementation, not a
redundant Compat declaration that could replace this one.

Current fixture source corroborates actual behavior: verify_panning_refresh
writes 3C0h to set pan=1 and compares rendered pixels with unpanned pixels
1..8. verify_panning_pixels covers three painters, shifts 0..7, bank/wrap
variants and split cancellation. These are source observations this P, not
newly executed tests. Historical S1 P80 used the same fixture and actual
Ninja compile commands with an isolated provider returning zero: both widths
failed the port-written pixel assertion; unchanged providers passed. This
proves the consumed panning effect, not a coherent XP restoration of all
coupled implementation patches or a freshly observed guest failure.

Historical declaration-removal controls emitted the specific implicit-query
diagnostic in both consumers/architectures under an explicit additional
-Werror=implicit-function-declaration. Current-header nt_ega passed that check;
gfx_updt also failed for unrelated setMarkPointers with BOTH headers, so its
exit code alone is not evidence against removal. Do not claim that the normal
production compiler policy necessarily rejects the original header.

Counter-review: simply deleting the declaration loses the explicit cross-file
function contract; moving it into a consumer or Compat relocates the same diff
and duplicates ownership. Returning zero removes behavior with an actual pixel
counterexample. The current single declaration is the smallest shared contract;
no further equivalent XP restoration identified. The provider and painter
implementation differences retain their own later ordered reviews. Recommend
retain R0428, awaiting owner approval. S1 P80's full 392/392 run is historical;
this documentation-only review does not rerun it or claim fresh guest coverage.

Production/test/manifest/EXE changes: none. Documentation, mirror-manifest and
whitespace gates apply. Next real-diff file is order 359 base/inc/gvi.h;
order 358 gmi.h has no real difference after P34 and remains excluded from
the actionable list. S4/T100 remain open.


## S4 P38: owner approves panning declaration; gvi.h review

Owner explicitly approves retaining R0428; P37's pending decision is resolved.
Order 359 base/inc/gvi.h has one real retained block, R0434, +2/-0: the
reason comment and get_chain2_screen_ptr macro at physical lines 292-293.
The adjacent #line 491 restores the XP diagnostic position and is excluded
from current real-diff accounting. Historical P85's +3 count included this
line; it is not an additional semantic point or a new source reduction.
A fresh complete comparison against pinned XP SP1 confirms every other
change is an existing excluded body or diagnostic line control, not a new
selected-content difference. Those exclusions are not reopened or inventoried.

Category: CHAIN2/odd-even display layout correctness, not snapshot machinery
or an x64-only compilation adaptation. The original get_screen_ptr remains
unchanged. The additional macro maps the group address to the four-plane
backing while preserving the low two lane bits: offsets 0,1,4,5,12,13 become
0,1,8,9,24,25. Blind multiplication by two would map attribute 13 to 26,
which is incorrect. The operation derives an address, with no additional
state, allocation, rendering path or guest modification.

Fresh source tracing finds all production references in nt_cga.c and
gfx_updt.c only. nt_text at nt_cga.c:228 translates the initial offset,
then copies four-byte cells at an eight-byte source step. gfx_updt applies
the same address contract to dirty-region origins, wrapping/split paths,
cursor attributes (1204), and selected display operations. These are real
production files; no equivalent Compat-owned mapping is available to remove
this declaration. Keeping zero-offset uses in the same pattern preserves
one addressing contract even though their numerical result is unchanged.

Current verify_text_stride in vga_frame_smoke.c uses widths 40/80/100/132,
multiple pitches, full and partial regions, and one/three rows. Its expected
buffer is independently assembled from source cell indices with an eight-byte
stride, not through this macro. Partial origin x=3 exercises logical offset
12 and backing offset 24. Historical S1 P85 recompiled BOTH actual consumers
with their real Ninja flags and the macro changed to original direct
get_screen_ptr; both architectures compiled/linked, then failed the same
full-cell assertion. Unchanged controls passed. This is dynamic real-painter
proof, not merely evidence that new callers require a new symbol. P85 also
links the independent BIOS/C-VID layout evidence including R0362. It is not
a fresh guest reproduction or a coherent reversal of every coupled layout
patch. This P rechecks source and historical records, not a new runtime run.

Counter-review: deleting the macro breaks its callers; mapping it directly
to get_screen_ptr restores the demonstrated wrong address. Changing the
original generic accessor would affect non-CHAIN2 consumers. Inlining the
conversion across the two files increases repetition and diff. The one-line
reason records why lane bits must be preserved; no redundant explanation or
format-only difference was found. Recommend retain R0434 with its established
layout contract, pending owner approval. Later implementation files retain
their own separate ordered reviews.

Production/tests/EXEs unchanged. Documentation, mirror and whitespace gates
apply. Next actionable file is order 367 base/inc/ica.h; intervening files
have no current real diff. S4/T100 remain open.


## S4 P39: owner approves CHAIN2 accessor; ica.h review

Owner explicitly approves retaining R0434; P38's pending decision is resolved.
Fresh complete pinned-XP comparison of order 367 base/inc/ica.h finds three
real text blocks, +3/-1, covering R0438 and R0439. Existing excluded bodies
and diagnostic line controls are skipped without new accounting.

R0438: two text blocks, +2/-0, add #ifndef CPU_40_STYLE and its closing endif
inside XP's existing #ifndef CPU_30_STYLE. The two associated #line directives
are not counted. All enclosed original PIC definitions remain unchanged.
Fresh source proof: actual CMake targets define CPU_40_STYLE and CCPU (257,
261,264,462). cpuint_c.h:35-37 defines CPU_HW_RESET=0 and CPU_HW_INT=2.
c_main.c includes its CPU interface before ica.h and c_cpu_interrupt's switch
uses both enum names (4456,4465). The PIC header's CPU_HW_INT macro denotes
bitmap bit zero instead. Without the guard it substitutes zero into the later
hardware-interrupt case, colliding with reset. This is a compiler/interface
namespace repair, not new PIC behavior, snapshot support or a proven guest
runtime failure. c_main retains its own CPU_HW_INT_MASK bitmap definition.

Historical S1 P89's corrected bounded removal used real Ninja compiler
commands: current headers passed on both widths; removing ONLY this guard
and its line controls failed both with duplicate case value. A first malformed
probe that also removed a later endif was explicitly rejected and supplies no
evidence. The later original CPU_40_STYLE callback block remains unchanged.
Renaming enum consumers or undefining names in individual users moves/increases
diff; changing the old bitmap value conflates two distinct contracts. Keep the
two-line guard. Combining it into XP's outer expression would replace an
original line and obscure the retained original conditional structure; it does
not improve total added/deleted line count.

R0439: one EOF block, +1/-1 under current counting. XP ends with a literal
Ctrl-Z byte (0x1a) line outside GISP_CPU; current source replaces it with an
empty line after diagnostic restoration. Historical P89 restored the actual
byte and newline while retaining guards: both real c_main compile commands
failed with stray byte 0x1a; current controls passed. Therefore restoring the
Ctrl-Z is not safe under the supported toolchain. This is a format/compiler
adaptation, not a functional interrupt repair.

Counter-review finds the replacement empty line has no semantic necessity.
Recommend deleting only that trailing blank, retaining normal newline after
#line 263 and preserving CRLF. It would change R0439 to +0/-1 and the file
total to +2/-1, reducing one real added diff line without restoring the rejected
byte or altering code. No source change is made pending owner decision. This
clarifies historical +4 guard/scaffolding and +0/-1 EOF figures: current
accounting excludes #line but counts the actual replacement blank.

This P rechecks current source and historical controlled compiler evidence;
no new compile/runtime experiment, guest test or full-suite pass is claimed.
Documentation, mirror and whitespace checks apply. Production/tests/EXEs
unchanged. R0438 retention, R0439 byte removal and its one-line cleanup remain
pending owner decision. Next real-diff file is order 369 base/inc/ios.h;
idetect.h has no current real diff. S4/T100 remain open.


## S4 P40: owner approves PIC contracts; EOF cleanup and ios.h review

Owner approves R0438/R0439's necessary changes and explicitly directs removal
of ica.h's replacement EOF blank. Removed exactly one trailing CRLF blank;
retained #line 263's terminating CRLF, the name-collision guard and absence of
Ctrl-Z. Production +0/-1; tests unchanged. Current mirror manifest updated.
Real ica.h diff changes +3/-1 to +2/-1, still three text blocks. Current
inventory changes to 107 files, 15,716 blocks, +24,669/-24,033; frozen evidence
is unchanged. This is one fewer real added diff line, not one fewer point.

Order 369 base/inc/ios.h: fresh complete pinned XP SP1 comparison finds one
retained difference at line 23, R0440, one block +1/-1. Add
 defined(SOFTPC_STANDALONE) ||
to the existing NTVDM/GISP_SVGA/SFELLOW condition. The original 0x10000 and
0x400 branches and their implementations remain unchanged. Existing excluded
bodies/line controls are skipped without accounting. Category: standalone
I/O address-space integration with an observable aliasing defect if restored;
not snapshot, x64 ABI, new device implementation or duplicate dispatch path.

Fresh callers: base/support/ios.c owns both adapter tables and masks dispatch
and registration with PC_IO_MEM_SIZE-1. Original CCPU in.c:55/74 and out.c
use that same mask directly. com.c registers COM1 at 03F8-03FF using ios.h's
original ranges and decodes the selected UART register. Restoring the condition
in this product selects 0x400 entries: 07FD & 03FF equals 03FD. Thus a port
not registered as COM1 can hit COM1's status handler. This is global dispatch
aliasing, distinct from a device intentionally registering a hardware alias.

Historical S1 P90 extended the real serial fixture to create/reset the machine
and call actual CCPU IN8 at 03FD and 07FD. Current x86/x64 controls returned
60/FF and passed; a coherent private original-header restoration rebuilt ALL
three affected original C objects (ios.c, in.c, out.c) with real Ninja flags,
then linked the same fixture. Both compiled and linked, returned 60/60, and
failed the isolation assertion. Expected high-port FF was independent of the
table implementation. This is an actual CPU-I/O-worker/UART counterexample,
not a freshly run guest, decoded guest instruction or exhaustive port matrix.
Current source matches the recorded failure mechanism. No new dynamic test
of ios.h is claimed in this P; ios.h remains unmodified.

Counter-review: using NTVDM/GISP_SVGA/SFELLOW merely to select the table changes
unrelated profile branches. Enlarging tables without matching CPU masks is
inconsistent. Moving this selection into new tables, wrappers or Compat adds
complexity; unconditional 0x10000 would discard original branch behavior and
structure. The existing single condition term selects original implementation
consistently. No safe equivalent restoration found. Recommend retain R0440,
pending owner approval. Later implementation differences retain their own
ordered reviews. Next real-diff file: order 377 base/inc/quick_ev.h.

Cached tools/build.cmd --no-pause completed x86/x64 package builds with exit
0; both EXEs published. Full before/after byte comparisons are identical after
normalizing ONLY PE TimeDateStamp, PE CheckSum and the unique compiler time
string. No code/data/debug difference remains. Runtime tests are skipped under
the owner's complete-artifact-equivalence exception, not claimed passed.
Existing original-mirror compiler warnings were emitted; no warning-free claim.
Documentation, mirror manifest and git diff --check pass. Initial sandbox
background launch exited without starting the script; the authorized hidden
background rerun completed normally. No timeout/termination is test evidence.
Disposable copies/logs are removed after promoting this result.

Published Nov 14 2026:

- softpc32.exe: 22:49:29; SHA-256 e1ba6e7fc52d85b7aef13991e649e8a98dd5497d97677b20ca2bbcbc3b41a480.
- softpc64.exe: 22:49:47; SHA-256 7cbae63b28ee6ce4cecd703ccd0a8d33ec6c3190c4ec6934d583fd60f6a8395e.


## S4 P41: owner approves full I/O map; quick_ev.h review

Owner explicitly approves retaining R0440; P40's pending decision is resolved.
Fresh complete pinned XP comparison of order 377 base/inc/quick_ev.h confirms
one retained semantic point, R0442, in two insertion blocks: +26/-0 at physical
21-46 (two record types, two callback types and spacing), +9/-0 at 55-63
(measure/capture/restore declarations). Total +35/-0. All other retained text
matches XP; existing excluded-body/line controls are skipped without accounting.
R0441's five redundant explanation lines were already removed in S1 P92 and
are not a current decision. No additional formatting-only restoration found.

Category: supported snapshot capability, not an original CPU/device bug repair
or an x64-only compiler adaptation. Actual root CMake includes quick_ev.c;
Machine snapshot_image.c and Compat CCPU/device archives reach the added API.
Current compat/devices/archive.c:144 measures entries, 149 captures, 235
restores. Its queue writer/reader at 686-750 serializes all record members
explicitly as u32 values rather than copying native structs or function pointers.
Original quick_ev.c remains sole owner of the live queues and scheduler.

Fresh field/ownership verification: quick_ev.c capture (1001 onward) reads
relative/original time, handle, parameter and event type from original nodes;
encode converts the function pointer to a stable callback ID. Hash head and
next indices represent the original linked lookup chains without persisting
addresses. State records retain next-handle positions, quick/tick countdowns
and entry counts. Restore uses the reverse decoding and existing private
queue state. None of these members or declarations is unused. Snapshot record
arrays are archival copies, not a second active scheduler.

Necessity: XP's interface offers add/delete/init/dispatch, but not enumeration
or restoration of private heads, hashes, allocation handles and countdowns.
Original add_event allocates handles and adjusts timing; its zero-time path
calls the callback immediately. Replaying add calls cannot preserve the saved
handle/countdown contract or guarantee callback-free load. Exposing private
nodes or reproducing the scheduler in Compat would weaken ownership and expand
diff. Moving these declarations merely relocates an interface still required
by both provider and archive; it does not eliminate its need. This is source
proof for the supported snapshot contract, not proof from missing declarations
alone or a claim that no alternative architecture could ever be designed.

Historical S1 P92 ran the existing checkpoint fixture on both architectures,
1/1 each. Its verify_event_queue_archive (currently 973 onward) captures two
quick events plus one tick event; rejects an invalid late callback while
preserving the live queue; clears/restores without running callbacks; checks
the saved countdown; then verifies event order 41,42,51. Current source still
contains those assertions. This review does not rerun them or claim a fresh
Windows guest session, exhaustive hash collisions or every malformed archive.
Later quick_ev.c implementation differences remain separately reviewable.

Recommend retain R0442, pending owner approval. No safe equivalent XP restore,
unused API, duplicated declaration or redundant added comment identified.
Production/tests/EXEs unchanged. Documentation, mirror and whitespace gates
apply. Next actionable file is order 381 base/inc/sas.h; intervening headers
have no current real diff. S4/T100 remain open.


## S4 P42: owner approves event archive; sas.h review

Owner explicitly approves retaining R0442; P41's pending decision is resolved.
Fresh complete pinned XP comparison of order 381 base/inc/sas.h confirms
one retained difference, R0444, +1/-1 at current line 106 / XP line 109:
extern IHPE Length_of_M_area becomes extern PHY_ADDR Length_of_M_area.
Original spacing and trailing comment are preserved. Existing excluded bodies
and necessary line controls are skipped without accounting. R0443's unused
added overwrite-memory declaration and R0445's inactive sys_addr substitution
were already restored in S1 P94; neither remains an outstanding point.

Category: C declaration/definition consistency and native-width port ABI,
not a snapshot feature or independently reproduced guest memory bug. Fresh
pinned-XP reads confirm base/ccpu386/ccpusas4.c:135 already defines
GLOBAL PHY_ADDR Length_of_M_area = 0, and zfrsrvd.c:40 imports PHY_ADDR.
Current corresponding locations are 134 and 41. The actual storage owner
therefore is original, not a newly introduced Compat definition. ccpusas4.c
assigns the requested guest memory size and uses the variable for guest-memory
bounds; it is a physical byte count, not a host pointer. CPU_40_STYLE is the
supported x86/x64 profile. base_def.h:95 defines PHY_ADDR as IU32; insignia.h
uses unsigned long for IU32 and UINT_PTR for IHPE.

Historical S1 P94 controls used actual Ninja ccpusas4.c compile commands,
changing only the selected declaration via a private header. Current controls
passed both widths; original IHPE failed both with conflicting variable types:
x86 unsigned int versus unsigned long; x64 unsigned long long versus unsigned
long. The initial expectation that equal x86 widths would suffice was rejected
and corrected before the final paired runs. Equal size does not make distinct
C integer types compatible. This is compiler/ABI proof, not a runtime overwrite
reproduction. This P rechecks original/current source and retained evidence;
it does not claim new compiler experiments or new guest/runtime acceptance.

Counter-review: restoring IHPE leaves the known conflicting declaration.
Widening original storage or changing zfrsrvd would add mirror modifications;
globally narrowing IHPE breaks its host-pointer-carrier purpose. Removing this
shared declaration or scattering replacements across users loses a coherent
header contract. The existing one-token replacement describes the actual
original storage; no further safe equivalent XP restoration identified.
Recommend retain R0444, pending owner approval. Later implementation-file
changes remain subject to their separate ordered review.

Production/tests/EXEs unchanged. Documentation, mirror and whitespace gates
apply. Next actionable file is order 383 base/inc/sndblst.h; sasp.h has no
current real diff. S4/T100 remain open.


## S4 P43: owner approves memory type; sndblst.h review

Owner explicitly approves retaining R0444; P42's pending decision is resolved.
Fresh full pinned XP comparison of order 383 base/inc/sndblst.h finds exactly
two real differences, two blocks +2/-1, with no excluded body in this header.
R0446/R0447/R0450's original constants, declaration aliases/signature and
omitted text were already restored by S1 P95; no remaining formatting-only
difference was found. Their historical restoration is not a new approval of
later implementation files.

R0448, line 42, +1/-1: SbTerminate return VOID becomes IBOOL. Category: host
resource lifecycle/error propagation. Current nt_sb.c:219-257 stops DSP work,
disconnects ports and attempts both PCM and MIDI close, then returns whether
both statuses are OK. Production callers machine/machine.c:140 and 503 stop
reset/destroy on failure; compat/platform.c:646 marks reset failure and requests
executor exit during BIOS reboot. No application callback, new recovery path
or second teardown operation is introduced by this declaration change.

The platform_failure fixture injects a failed termination at the platform
boundary, verifies machine run reports I/O error, and verifies subsequent reset
does not call sound initialization. This proves caller failure handling, not
an induced native audio device-close failure. Device tests also exercise real
successful/repeated SbTerminate calls. Historical P95 ran these fixtures in
both complete architecture suites, 392/392 total. This P rechecks source and
records, not fresh runtime evidence. Restoring VOID alone mismatches the
provider; coherently discarding its status removes an actually consumed error
contract. A separate error query would add state/API instead of simplifying.
Recommend retain R0448; do not claim every native teardown failure is covered.

R0449, line 121, +1/-0: define
MIDI_READ_TIMESTAMP_POLL_WRITE_POLL_UART as 0x36 alongside original 34/35/37.
Category: DSP command framing for an unsupported MIDI UART mode, not new MIDI
sound support or snapshot functionality. Actual nt_sbdsp.c:1091 uses it with
the other UART commands to enter UnsupportedMidiUart; that state's data-write
case at 920 consumes bytes without treating them as new DSP commands. Reset
returns the existing parser to normal command processing. Removing just the
symbol breaks its switch case; removing the handling as well reintroduces
interpretation of following data bytes as commands.

Current verify_sb_midi_framing at device_contract_smoke.c:956 explicitly includes
0x36. It writes all 256 subsequent byte values through actual ports, verifies
parser retention, unchanged PCM/DMA/device state, no PCM/MIDI output or IRQ,
and reset plus capture/restore behavior. Historical P95's full dual suites
passed this coverage. It is current-behavior proof plus a source-path argument,
not a newly executed original-negative counterprobe or a new guest reproduction.
No source evidence makes the missing constant redundant. Inlining literal
0x36 only moves the difference and abandons the original named-command style.
Recommend retain R0449 with the existing framing implementation, pending owner
approval. That implementation still has its separate later-file audit.

Production/tests/EXEs unchanged. Documentation, mirror and whitespace gates
apply. Next actionable file is order 386 base/inc/tape_io.h; intervening
headers have no current real diff. S4/T100 remain open.


## S4 P44: owner approves SB contracts; tape_io.h review

Owner explicitly approves retaining R0448 and R0449; P43's pending decisions
are resolved. Fresh complete pinned-XP comparison of order 386 tape_io.h
finds only R0451 outside the established JAPAN excluded body/line control:
one block +0/-1, removal of the final literal Ctrl-Z (0x1a), XP line 118.
Byte-level comparison confirms equality after omitting that existing excluded
body and its necessary #line and deleting only the original terminal byte.
The raw XP byte has no trailing newline; current source ends with the normal
CRLF after extern void cassette_io(). There is no replacement blank to remove,
unlike ica.h. No other declaration, comment, spacing or behavior differs.

Category: compiler/legacy text-EOF adaptation, not snapshot, x64 ABI width or
original guest bug repair. The header is production-connected: actual BIOS
tape_io.c:45, floppy.c:134, floppy_i.c:61 and Compat device_bop.c include it.
Root CMake includes those original implementation files. The original
cassette_io service also handles AT INT 15 extensions and is registered in
the actual BIOS BOP table; the tape filename does not make it unused.

Historical S1 P96 used the real architecture-specific Ninja tape_io.c GCC
commands, redirecting output and selecting a private header with the original
terminal byte restored. Current controls exited 0 on x86/x64; restored-byte
controls exited 1 on both with tape_io.h:118:1 stray '\32' in program.
Existing macro warnings occurred separately and did not cause the failure.
This is production translation-unit compiler proof, not a guest behavior or
new runtime result. The current P rechecks bytes/callers and historical
records; it does not rerun the controlled compiler experiment.

Counter-review: exact restoration returns a demonstrated compile error.
Replacing the byte with a comment or adding preprocessing would add new
content to solve a one-byte legacy EOF issue; deleting the header would
remove used declarations. Current omission is already minimal and preserves
all retained original source. Recommend retain R0451, pending owner approval.
No further equivalent restoration or cleanup candidate identified.

Production/tests/EXEs unchanged; documentation, mirror and whitespace checks
apply. Next actionable file is order 394 base/inc/video.h; intervening files
have no current real diff. S4/T100 remain open.


## S4 P45: owner approves tape EOF; video.h review

Owner explicitly approves retaining R0451; P44's pending decision is resolved.
Fresh complete pinned-XP comparison of order 394 base/inc/video.h finds only
R0455 outside existing excluded bodies and diagnostic line controls: four
IMPORT declarations at current lines 293-296, one block +4/-0. They name
stream_io_buffer, stream_io_dirty_count_ptr, stream_io_buffer_size and
stream_io_enabled, using exact original XP spelling. Their original XP
locations (352-355) are inside NTVDM; the current selected copies are outside
that excluded block. R0454's redundant comment and spelling difference were
already restored in S1 P99 and are not outstanding points.

Category: standalone linkage/declaration boundary preserving original video
code, not snapshot or a new functional stream backend. Fresh pinned XP reads
confirm vd_write_teletype references all four at original video.c:2751-2756
OUTSIDE NTVDM. The earlier NTVDM block only declares a local function pointer.
Current video.c:2242-2247 preserves the original stream test/body. The function
is installed in original video.c and ega_vide.c service tables and called by
original write-string logic. Root CMake builds both the original video module
and Compat video.c. This dependency was not manufactured by a new consumer.

Current Compat video.c:27-30 defines NULL buffer and dirty-count pointers,
zero buffer size and FALSE enabled. Whole src/test symbol search finds no
assignment enabling this stream. stream_io_update remains an empty host seam;
no active stream output, second display implementation or safety of externally
enabling these disabled globals is claimed. The separately named
host_stream_io_enabled in nt_graph is not this variable.

Historical S1 P99 recompiled actual video.c with each architecture's real
Ninja command and a private header removing only these four declarations.
Current controls passed; restored declaration visibility failed on x86 and
x64 with all four names undeclared at original lines 2751/2752/2754. This is
compiler proof coupled with original-consumer source proof; it is not merely
an uncompilable partial rollback of an invented caller. This P verifies
current/original sources and historical evidence, not fresh runtime tests.

Counter-review: enabling NTVDM to expose the originals changes unrelated host
branches. Moving the declarations elsewhere merely relocates the adaptation.
Removing the original selected runtime block would add a new mirror deletion
and require a separately considered coupled change; it does not restore XP
or reduce the existing four-line boundary. Defining replacement macros would
change the original variable contract. Current declarations are the narrow
existing seam with disabled host storage. No safe equivalent restoration or
remaining nonsemantic cleanup found. Recommend retain R0455, pending owner
approval. The later video.c implementation audit remains separate.

Production/tests/EXEs unchanged. Documentation, mirror and whitespace checks
apply. Next actionable file is order 398 base/keymouse/keyba.c; intervening
headers have no current real diff. S4/T100 remain open.


## S4 P46: owner-directed disabled-stream removal

Owner rejects the need to preserve the unused stream body merely to justify
four declarations. Explicit authorization deletes the six-line stream branch
from vd_write_teletype and restores video.h's four added declarations to XP
absence. Follow-up authorizes deletion of nt_graphics_tick's unused seven-line
STREAM_IO branch and the now-unreferenced empty Compat stream_io_update.
Owner subsequently approves the exact displayed runtime-branch deletion.
R0455 is resolved by removal, not retained or pending. Its historical evidence
only proved that retaining the original body required declarations; it did not
prove that retaining both was the best product subset. P45's recommendation
is superseded by this owner-directed simplification.

P46-A, base/video/video.c original 2751-2756: omit the unused runtime branch;
retain normal teletype processing, add one local reason and #line 2757.
P46-B, host/src/nt_graph.c original 1007-1013: omit the unused STREAM_IO runtime
branch, retain its original #ifndef NEC_98 framework, one reason, and existing
#line 1015. These are explicitly approved NEW mirror omissions, not claimed
restorations. They are not #error exclusions: !NEC_98 is selected, whereas
ScreenState==STREAM_IO is a runtime comparison. #error there would break the
supported build. The same distinction applies to stream_io_enabled in video.c.

Actual active chain: Compat host_timer_event calls host_graphics_tick; the
original macro calls working_video_funcs->graphics_tick. Compat host_init_screen
selects nt_video_funcs, whose slot is nt_graphics_tick. Preserve this live
function's mode-change delay, palette update, calc_update, cursor update and
shared flush_count/TICKS_PER_FLUSH. Source search finds static-storage-duration SCREEN_DESCRIPTION
sc with zero-initialized ScreenState; WINDOWED=0 and STREAM_IO=2. No production
ScreenState write selecting stream exists. Four separate stream globals were
FALSE/NULL/zero with no enabling assignment. Snapshot rebuild selects the
original mode and redraws; it does not restore a stream ScreenState. No source
claim of a working stream backend is made.

Removed Compat's four disabled globals, their three-line explanation/separators,
and empty stream_io_update. Retain separately named original
host_stream_io_enabled and host_stream_io_update: neither is the removed function,
and neither needs a new unrelated mirror deletion in this bounded change.
Whole src/test search finds no exact removed stream symbol remaining. No guest,
media, configuration, shared component or snapshot schema is changed.

Actual production changes in four files: +3/-28, net -25 (one added #line is
included in this production count). Tests unchanged. Relative to XP, excluding
necessary #line: remove video.h +4/-0; add video.c +1/-6 and nt_graph.c +1/-7.
Thus real mirror additions decrease 2, deletions increase 13: total changed
mirror lines INCREASE 11, while source lines decrease 25. Owner explicitly
approves this tradeoff. New omissions are two points; obsolete header point is
removed. Text-block total increases one, not a defect count.

Current inventory: 254 identical, 112 exclusion-only, 106 real-diff files;
15,717 blocks, +24,667/-24,046. Original 472 identities/frozen counts unchanged.
Before/after zero-context Git comparisons with only #line stripped confirm
both implementation-file deltas and one new block each; unchanged exclusions
cancel in those deltas. Current manifest hashes updated for the three mirrors.

Final-source dual cached builds and full test-target builds passed. Both EXEs
were published. Their sizes/code differ from the previous baseline, so the
artifact-equivalence test exemption was rejected. Full runtime verification
passed first run: x86 188 background (198.66s) plus eight native (25.53s),
x64 188 background (219.85s) plus eight native (24.37s): 392/392, no retries,
skips or failures. Both package variants, Console/Window and native audio
are included. Mirror, documentation and whitespace checks pass. P46 preserves current review frontier:
next manual file remains order 398 keyba.c; later video/nt_graph differences
outside these explicitly authorized omissions are not deemed reviewed.

P46 delivery review: checked all four actual production diffs against owner
scope; no second path, new state, polling, guest change or shared API change.
The selected outer conditional structure and diagnostic line mapping remain.
Existing VGA frame tests exercise the real host timer and assert WINDOWED;
no redundant new fixture was added for unreachable stream removal. The suite
is automated regression, not a fresh manual DOS/Win3.1/Win95 acceptance claim.

Published package SHA-256:

- softpc32.exe: 692b3b724dc3914cc5a045110402fccdbef8fe546cf504606203a3c9eb95145c
- softpc64.exe: 7eb5645d7fbce87b81f39fafe91e45d6892e1d3a992d1b5bd3a1b1a543c21f78

Publication verified fresh embedded compiler times Nov 14 2026 23:21:51 and
23:22:02 respectively (build-host clock). Both final-source builds and all test
target builds exited zero. Task-owned temporary logs/probes are removed after
promoting these results; reusable Ninja and ccache trees remain.

## S4 P47: keyba.c ordered review

Owner accepts P46 (1c8567a5). Fresh full-file comparison against pinned XP
SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89 confirms order 398:
four real text blocks, +121/-0, excluding existing excluded bodies/line control.
XP raw SHA-256: 6096d9f805d832c0f939e511e0f2d56e37a4eee48187376b6c6465844923a5a2.
Earlier R0456 notice and R0459 comment differences are gone.

| Decision | Current anchor | Blocks and lines | Purpose |
| --- | --- | --- | --- |
| R0457 | 80, 2346-2463 | 2; +119/-0 | Snapshot include (+1) and state/callback implementation (+118). |
| R0458 | 2085 | 1; +1/-0 | Assign input_port_val before publishing the C0 response. |
| R0460 | 2513 | 1; +1/-0 | Clear IRQ1 request when port60 consumes its output byte. |

R0457 is a snapshot extension, not an original keyboard defect. Compat device
archive capture/restore and callback codecs are real production consumers.
The original FIFO, held-key/scan-set/repeat state and response latches are
archived. Private do_int/allowRefill and refill handle require access here.
Original ports mutate state and cannot replace capture/restore. Callback IDs
and anomalous break-table indices avoid persisting host addresses. Moving the
block into Compat would require exposing private implementation or including
source, rather than restoring XP and reusing an equivalent original API.
Checkpoint tests cover queued state, invalid FIFO count, pointer reconstruction
and archived IRQ; irq_smoke covers scan sets 1-3. Per-device restore is not
transactional: existing staged machine restoration owns failure handling.

R0458: XP C0 marks automatic code_to_send valid without assigning its value.
The added line supplies the actual input port, initialized to BFh in the
ordinary colour-adapter configuration. bop_smoke 88-90 sends C0 at port64 and
requires BF at port60. Historical S1 P100 separately restored this line on
both architectures; both failed this assertion while patched controls passed.

R0460: original clear is inside JOKER, excluded in supported builds. The added
selected call uses original ica_clear_int after byte consumption. It is not
EOI or a second queue. irq_smoke 65-80 observes masked pending IRQ1, reads FA
from port60, then requires IRR bit1 cleared. Historical P100 removal controls
failed this exact assertion on both widths. Fresh compile-command inspection
confirms keyba.c in both production device targets without JOKER/NTVDM.

Fresh evidence is source/baseline/caller/test inspection. The four negative
executions are historical P100 evidence, not newly rerun. P46's fresh 392/392
regression includes these unchanged current tests; it does not prove every
refill timing or a particular historical guest incident. No remaining
nonsemantic cleanup or equivalent restoration found. Recommend retaining all
three decisions, pending owner approval. Source/tests/EXEs unchanged;
documentation, mirror and whitespace gates pass. Next manual file is order
400 base/keymouse/mouse.c, not implicitly reviewed.


## S4 P48: mouse.c ordered review

Owner approves keyba.c R0457/R0458/R0460; P47 pending decisions are resolved.
Fresh comparison of the entire mouse.c to fixed XP SP1 confirms 14 blocks,
+115/-34 outside established exclusions/line control. XP raw SHA-256:
d449af5953821629584958ecb29217ac7114b246169d4d0e6526c6ed2f67ec64.
R0462 and R0467 obsolete added notices/comments are already removed.

| Decision | Added/deleted | Purpose and current sites |
| --- | --- | --- |
| R0463 | +41/-34 | Periodic/enable-aware IRQ: quick_ev include, timer handle/helpers, mode transitions, reset cancellation, data IRQ gating and cold handle invalidation. |
| R0464 | +67/-0 | Snapshot include and 66-line callback/state archive block, lines121-186. |
| R0465 | +1/-0 | Default output byte at mouse_inb entry, line190. |
| R0466 | +6/-0 | Four reset-state lines plus two cold-init lines (buttons and reset call). |

Line attribution splits shared Git hunks without double counting: removal of
the old loadsainterrupts reset and cold timer-handle invalidation belong to
R0463; the replacement reset-data assignments belong to R0466. Total is
+115/-34. Blocks remain textual units, not fourteen independent decisions.

R0463 is a functional device timing/enable correction, not x64 portability.
XP explicitly limits five bursts and requests 100 interrupts per eligible
mode write. Its own deleted comment calls this a Windows 1.02/2.03 workaround.
It cannot provide sustained rate-selected recurring events. Current helpers
use the existing quick-event queue with one handle; HOLD suppresses IRQ but
keeps recurrence, and changed rate/disable cancels the old event. Data-enable
bit3 and timer-enable bit4 gate their respective sources. No extra thread,
polling loop or parallel device exists. Restoring the complete original
scheme restores the finite burst policy; restoring isolated lines is not a
valid coherent-original negative. mouse_smoke 153-220 checks four actual
scheduled periods, recurrence through HOLD, cancellation and PIC state;
earlier assertions deliver guest input through CCPU/PIC. No fresh complete
original-scheme negative or exhaustive old Windows driver test is claimed.

R0464 is a snapshot extension. Actual archive.c capture/restore and callback
codec callers serialize thirteen InPort fields. Static registers, ID toggle,
prior buttons and timer callback are owned here. Port reads toggle ID phase;
HOLD latches/clears deltas; mode writes schedule interrupts. Therefore replay
through original ports is not an equivalent state archive. Existing checkpoint
and mouse tests cover held data, selection, invalid ID phase, callback and
controller/queue roundtrip. Moving the block to Compat would need extra
exposure of private state/callbacks, not merely restore unused mirror code.

R0465 prevents undefined guest input: current original CCPU IN8 uses an
uninitialized IU8 temporary and invokes this handler; unsupported selectors
formerly left it untouched. The one default assignment makes that path
bounded/deterministic. This proves the need for defined output, not that FFh
is a measured electrical value for every undocumented InPort register.
mouse_smoke has unsupported-selector sentinel coverage; no exhaustive hardware
bus proof is claimed.

R0466 clears accumulated movement and latched data/status on reset, aligns
previous/current buttons, and cold-initializes buttons through the same reset
path. Otherwise pre-reset movement remains eligible for later latching.
BIOS reset calls q_event_init before mouse_init; the stale timer handle must
be invalidated, not deleted from a newly initialized queue. Current machine
mouse_send is the real producer, and mouse_init registers the original ports.
No alternative Compat controller was found. Existing movement/reset and
checkpoint tests exercise the current path; no new exhaustive reset timing
negative is claimed.

Recommend retaining all four; no equivalent nonsemantic restoration found.
Fresh work is source/XP/caller/test inspection; P46's unchanged production
baseline passed full 392/392, and historical P102 ran focused mouse/checkpoint
checks on both widths. These are not newly repeated guest-session acceptance
or allocation-failure tests. Source/tests/EXEs unchanged; mirror/documentation/
whitespace gates apply. Next manual file is order401 base/keymouse/ppi.c.


## S4 P49: R0463 coherent-original guest control

Owner asks to prove whether R0463 is needed, following the historical origin
review. git -S mouse_period identifies ee62ad01 (T85 S1 P2, 2026-09-28).
That task combined device semantic completion and a compatible Win1.01 driver;
its successful guest acceptance did not isolate the timer replacement from
the driver correction. The original need cannot be inferred from that joint
success. T85 closure and proposal remain historical, not rewritten.

### Controlled experiment

Production baseline 81ac2c8c; x64 cached compiler/link settings. Two task-owned
mouse objects are explicitly linked before the existing device archive into
identical probes. No cached object/archive or shipped EXE is overwritten.
The probe reuses existing runtime restart fixture helpers and real Machine
input/frame APIs. Guest writes use LIB_STORAGE_MEDIUM_OVERLAY. No guest binary
is edited, no snapshot is loaded, and no Computer Use is involved.

Control A retains current behavior. Control B restores the complete R0463
active behavior: five bursts, 100 IRQ requests per eligible mode write, original
unconditional input IRQ and constant-high request, original mode assignment,
no periodic scheduling or mode-based cancellation. Reset rearms five bursts.
Unrelated R0465 default and R0466 reset-data corrections remain identical.
Private timer/archive helpers remain linkable but inert in B: no active path
schedules their callback and cold runs never restore an archive. This isolates
R0463 without inventing a snapshot-format failure as evidence against XP.
Both objects log bounded mode writes; B additionally logs burst exhaustion.

Read-only media identities, unchanged before/after all runs:

- Existing DOS622/Win1.01 10MiB image:
  d7cc97f17ab1fc557213d09a9c98a3c50a9cab0c13942ff3ddeda05c50cd37d8.
- Repository Win3.1 installed image:
  2de4b03a68b1e1ac23d482c3a3402726a853a8520b7acc370db694053dffa3a4.

Win1.01 installed modules are combined in WIN100.BIN; no loose MOUSE.DRV was
found. Do not claim fresh identity verification against the historical 3358-byte
standalone driver. The same entire image/installed driver is used in both
arms, and its live InPort mode writes are observed (11h/31h). Win3.1 uses
09h/29h. Neither driver is replaced during the experiment.

Sequence: cold boot, wait for rendered DOS prompt, type WIN through Machine
FIFO, allow initialization, capture published RGB frame, deliver identical
relative movement, reposition to a deterministic screen corner, move to File,
press/release button and capture results. Capture delays are observation
windows, not predicates proving success. Frame contents and original-mode
traces supply the actual outcome. Six paired runs (12 processes) cover initial
Win1.01 movement/file selection, repeat Win1.01, Win3.1, targeted File-menu
clicks on each guest, and held-button Win1.01 menu capture. All exit zero.
Initial Win3.1 click hit a non-menu location; only the later correctly targeted
click is counted as menu proof. First probe compile failed on a void function
using a returning assertion macro; corrected before any execution.

### Results and limitations

- Both guests boot and move their visible pointers in both arms.
- Win1.01 B logs remaining burst count 4,3,2,1,0; movement/selection and the
  final menu observation occur after exhaustion. Not merely unconsumed startup
  bursts. Held-button File menu expands in both arms; full PPM bytes match,
  SHA-256 334b886edbcce70ed7bb02fc5369ff95719c9e4e6b438cd42e4d951c2426139f.
- Win3.1 File menu expands after click in both arms; full PPM bytes match,
  SHA-256 ea2913c86661488fa210a78975f0eda9150f85716f30977e010ea8b43e17d7f3.
- First Win1.01 pair matches all four captured stages. Some later intermediate
  Win1.01 images differ by 54 pixels on row67, x5..111, the file-selection
  outline; menu pixels match without masking. Do not claim all captures match.
- Snapshot restore, x86, all mouse drivers, long-duration use, Win95 and
  quantitative hardware timing equivalence were not exercised by this probe.
  Audio is the existing runtime test's mock; no native audio claim.

Final probe source SHA-256:
e2d10e4291df7609c595d6efc79bf78543c88bb625064c76674c5026c4fcda7e.
Current instrumented object source:
9a81f246e8d0dea124e6019e4a8dbe1e5c11136a8a920a54fb37478868d44ce9.
Original-behavior instrumented source:
483c045a242e954d15041e8beb26b591b2b8d9530511273f24976e74a73afeed.
Final probe EXEs: current 5cf49f5d92f1d8eaea138412225febb324fdbc32920b733a660e7a2622e8c94e;
original 79174c9865a65c7c004c31fe92a66f5a4f3455f4032a921a0b60f33be2f2b10b.

Conclusion: no demonstrated guest necessity for R0463. Its single-controller,
existing-queue design is architecturally coherent, but that alone does not
justify retention under the owner's original-first policy. Supersede P48's
blanket retention recommendation: R0463 is a restoration candidate, pending
owner decision and coherent snapshot/event adaptation planning. No production
restoration is silently applied. R0464/R0465/R0466 remain separate decisions.
Only documentation changes; product/media hashes unchanged. Promote this
bounded evidence and remove task scratch after jobs exit; no raw guest image,
capture or probe binaries are committed. S4/T100 remain open at mouse.c.


## S4 P50: restore original InPort bursts and current snapshot

Owner approves coherent R0463 restoration after P49's real-guest comparison,
requires no legacy snapshot recognition in production, and separately authorizes
one-off conversion of the existing package snapshot. R0463 is restored, not an
approved retained patch. R0464 now archives the original remaining burst budget;
R0465 default input and R0466 reset-data behavior stay unchanged.

### Actual change and accounting

- Restore XP's five eligible bursts per reset, each requesting 100 PIC events,
  and original constant-high/input IRQ behavior. Delete periodic scheduling,
  callback codecs, queue identifier and added enable/HOLD delivery gating.
- Keep one original counter/state owner. Archive its remaining count with the
  existing device serializer; validate 0..5. No second timer or compatibility
  reader. The only supported format is SCP4; malformed-format tests use generic
  corruption/truncation, not historical format definitions.
- Production four files: +45/-73, net -28. Tests three files: +38/-78, net -40,
  measured with `git diff --numstat 8b35181c -- src test`. No public-eight change.
- mouse.c against pinned XP, excluding excluded bodies/line controls:
  14 blocks +115/-34 becomes 5 blocks +59/-0: nine fewer text blocks and
  90 fewer changed mirror lines. Remaining selected content is snapshot +52,
  default input +1, reset/cold-init +6. Global real-diff inventory remains
  106 files, now 15,708 blocks, +24,611/-24,012.
- Preserve XP CRLF and its original trailing tab on the restored burst condition.
  Normal `git diff --check` flags this exact original whitespace; it is not
  reformatted into a new mirror deviation. Owned-source whitespace is checked
  normally; the mirror check excludes end-of-line blank complaints only.

### One-off asset conversion

The owner's existing `assets/binary/snapshot` is converted offline with a
scratch-only tool using the current canonical image/device reader and writer.
Its retired timer handle is zero; its one quick/two tick entries have callback
IDs 2,5,4, below the removed identifier. Therefore no event is removed or
renumbered. The asset's saved mode is 9. The retired model had no original
remaining burst budget; conversion initializes that newly restored state to
its original reset value five without issuing interrupts or changing the PIC.

Exact byte comparison proves two changed bytes out of 18,700,025: offset 3
(format identity), offset 17,842,181 (zero retired handle to budget five).
CPU, RAM, all other device state, queue entries and saved media bytes are
unchanged. Saved HDD mode is overlay; no INI or guest image is modified.
Original SHA-256: f519bf14842856aefd7209cac6449443ed1d6aab21358d3d6a1279303350519f.
Converted SHA-256: 9c9e6116e4235c57d48f14a6267bb4b080942b0046a163344146e4a35dd2460f.

Both x86/x64 probes use the actual current Emulator state-write, VM driver,
media preparation and executor restore path. Each consumes all 18,700,025 bytes,
returns OK, reaches PAUSED and reads back budget five. Final probes additionally
confirm both PIC masks/IRR/ISR and all 16 pending request counts match the image.
Audio is the existing
mock fixture; this proves real state loading, not fresh visual/manual or native
sound acceptance. The one-off converter is deleted at delivery, not a product
legacy API or persisted old-format definition.

### Verification and actual-diff review

Initial background runs fail the two new PIC assertions on both architectures.
The burst fixture wrongly assumed lowering a PIC line cleared all accumulated
requests; original SWPIC_clear_int decrements one. Correct its assertion to
compare the accepted-count delta (+100 for each of five writes, then zero).

The corrected checkpoint comparison identifies a real existing archive-order
fault, not a second fixture error. An isolated trace shows PIC restore at100;
VGA controller replay changes it to98; every later restore leaves98. VGA and
InPort share adapter1/line1. Replaying CRTC11 twice with bit4 clear invokes
original ica_clear_int twice. Periodic timer replacement previously left no
burst queued at capture, so that fixture did not expose this loss.

Minimal repair in the existing Compat archive path: move its one PIC restore
from the front to the final return, after device register replay. No duplicate
restore, compensation, new state, mirror change or runtime path. Device restores
run on the existing stopped/paused executor; no guest instruction intervenes.
Queue/EOI/SB restoration remains in its existing order; their state is not
rederived from the PIC. The same failing checkpoint assertion must now pass.
Review also covers serial/parallel/device restore calls that can affect IRQs;
placing the shared PIC restore last uniformly preserves saved interrupt state.

Existing guest IRQ, movement/HOLD registers and archive checks remain; rate
selections1..4, exhaustion, invalid budget and absence of recurring events are
covered. The assertion failures and isolated negative trace are retained as
results, not silently reclassified as successful first runs.

Final cached dual builds and publication pass. Final full regression passes
392/392 without retries/skips in that final run: x86 background188/native8
(187.45s/24.56s), x64 background188/native8 (205.54s/24.20s).
Both focused checkpoint reruns and converted-asset executor probes pass.
These final results do not erase the earlier assertion failures above.
Mirror manifest and documentation gates pass. No public package or new
owned-code compiler warning is introduced; preserved mirror warnings are not
claimed eliminated. Final embedded compiler timestamps are Nov15 2026
00:34:26 (x86), 00:34:33 (x64).

Executor and coordinator actual-diff reviews confirm one original InPort
state owner, one current-format reader, one device/PIC restore path, no added
compatibility state or mirror exception. The saved asset differs in exactly
the two reviewed bytes. User INI/media are unchanged. Task probes/logs are
promoted to this concise evidence then removed before commit.
Current artifact SHA-256:
- x86: 40d94eca40568899935fa32780c98f1048f602ea2135c710a5a6274222f57306.
- x64: b3bddab000df773a060e3b2e7ad46afdc611b8458e5a18b4b1f82fc7cc6db10c.

Similar-issue sweep covers all production/test references to the retired mouse
callback, event ID and state field; no consumer remains. The unrelated original
MOUSE_TIMER_INTERRUPTS_PER_SECOND constant in mouse_io.h is retained. Enum followers are
intentionally renumbered under the new format and no old reader is retained.
Existing audio queue archive tests exercise current IDs. Retained snapshot
capture/restore checks the original budget rather than creating new policy.
No additional mirror restoration, guest patch or public package change is made.
P49 remains the separately identified historical real-guest comparison;
this delivery does not claim every guest/driver's timing was validated.
S4/T100 remain open awaiting owner acceptance of P50.

## S4 P51: confine R0466 data reset to cold initialization

Owner request: 审计466 去试着找到过硬证据; after the evidence and the
four-line cold-only proposal, owner authorizes 批准 老规矩五件套.
P50 is accepted. This is a bounded implementation, not closure of S4/T100.

### Evidence and decision

The prior audit used five isolated controller variants on both x86 and x64,
linked to the actual Machine reset, original BIOS/controller and CCPU IN8.
All ten runs completed. The fixture supplied movement/buttons, issued guest
reset, then repeated public cold reset without new host input. Current code
and cold-only initialization returned clean state. Removing all six R0466
lines left latched X=11, Y=249 (-7), status=108 readable through IN8 after cold
reset. Removing only four guest-reset assignments left pending movement 9,4
readable after the new HOLD. Removing only the two init lines also retained
old state. These are controlled production-function/port-path observations,
not a manually reproduced Windows cursor symptom or hardware reset oracle.

The actual production chain is machine/machine.c softpc_machine_reset ->
original BIOS reset -> mouse_init. Public cold start reuses process-static
controller state. Guest port writes instead reach mouse_outb through the
original registered adapter. No authoritative hardware or guest-negative
proof was found for extending the latter reset to clear data. Therefore the
four assignments are restored out of guest reset; cold initialization alone
zeros buttons/prior buttons, pending movement and latches, then reuses the
original reset for mode, address, interrupt and burst budget. No new state,
callback, API, mode condition, compatibility path or snapshot format appears.
R0466 is owner-approved as this narrower standalone cold-run boundary; it is
not classified as a proven XP hardware-reset defect. P48's blanket reset-data
retention rationale is superseded. R0464/R0465 remain separate decisions.

### Implementation and focused regression

Production mouse.c +3/-5 (net -2); existing mouse_smoke.c +30/-0.
The regression uses real public input, registered ports and Machine cold reset:
old latch survives guest reset, pending movement becomes the next HOLD data,
then cold reset clears status/X/Y both before and after HOLD despite prior
pending movement and button transitions. Existing archive/burst and real CPU
IRQ-handler checks remain. No test-only production interface or new fixture.

Similar-issue sweep: rg over production mouse_init/mouse_outb and mouse-related
snapshot/reset tests confirms BIOS reset is the cold initialization caller,
registered mouse_outb is the guest path, and snapshot restore remains direct
state restoration. No second reset-data owner is introduced or changed. Other
controllers and excluded bodies are outside this bounded restoration.

XP-relative retained difference: R0466 +6/-0 becomes +4/-0; mouse.c five
blocks +59/-0 becomes four blocks +57/-0. The file count remains 106; current
global aggregate becomes 15,707 blocks, +24,609/-24,012. Excluded bodies and
necessary line controls are not counted. The frozen S4 ledgers stay historical.
Source CRLF and owned-source LF are preserved. User INI/media/snapshot and all
public-eight trees are unchanged.

Cached dual build and publication completed. Both background suites pass
188/188, including mouse, checkpoint and XP manifest checks. Each native suite
passes 7/8: audio, MIDI, package and Window checks pass; console_broker_display
fails at line 431 (written == 80 * 25). An unchanged x86 single-case rerun fails
at the same assertion. A scratch-only diagnostic copy records size=31,30 and
written=930; an additional sample proves size=31,30 immediately after the test's
prepare_output_surface, before broker creation. This is independent of the
changed mouse path; it does not establish the exact native resize cause.
No shared-source or test-standard change is authorized or made to bypass it.
Current aggregate is 390/392, not full qualification. Owner was asked to enlarge
the active RDP desktop before another native validation. Commit/push and
acceptance remain pending that qualification; task-owned scratch is retained
only while this active packet needs it.

The initial direct manifest check caught use of a raw CRLF hash; corrected to
the existing LF-normalized text hash before both full suites passed that gate.
Production remains CRLF. No product/compiler fix was needed for this ledger
error. Known original mirror macro-redefinition warnings remain; no new owned
source warning was found. Whitespace checks pass.

Actual-diff self-review confirms exactly one cold-state initialization owner,
no guest-reset override and no new ABI/snapshot/guest/public-tree change. The
selected guest-reset body matches fixed XP byte-for-byte after line-ending
normalization. Existing asset snapshot SHA-256 is unchanged from P50.
Published EXEs: x86 Nov15 2026 08:05:27,
85f1357675431a28fc0f21069e1a92fb3a3a2aef7ee79c303678fb5b18a2e9b4;
x64 Nov15 2026 08:11:00,
3b74c6fd950d8a0ffc85fe906eed1ebca032660dc128feb73b89a6935965262b.

Owner follow-up: rdp桌面无法扩大 这是你集成测试的缺陷 请你在桌面宽度不合适的时候 正确设定预期好吗.
The native fixture now clears its queried buffer capacity, places its history
marker within that capacity, fits the ordinary switch frame to the visible
extent and bounds visible-row assertions by the captured columns (+11/-5).
It does not skip large-frame extent tests, remove retained-frame checks, or
change the requirement to preserve content before redraw. Its manifest follows.
Attempting to re-expand fixture capacity alone was experimentally rejected:
subsequent native metadata operations still shrink it. No such workaround remains.

With the size assumptions corrected, an isolated diagnostic observes the
specific native call SetConsoleScreenBufferInfoEx changing 31x30 to 31x29,
with the first cell changing from 'h' (history) to 'M' (Monitor). Growing the
buffer later cannot recover that line. This is not a smaller visible-width
expectation. Microsoft Console's SetConsoleScreenBufferInfoExImpl in
https://github.com/microsoft/terminal/blob/main/src/host/getset.cpp#L647
interprets the input viewport as exclusive, while the queried Ex rectangle is
inclusive. A scratch-only wrapper adding one to Right and Bottom preserves
size/content and completes the full existing Console test on x86 and x64.
This is a candidate native API-boundary conversion, not an RDP-specific branch.
No external source was imported; no formal public production edit is made yet.
Owner approval was requested for the two-line public production correction,
which exceeds the just-authorized fixture-only correction. Do not report the
candidate's success as qualification of the unmodified production backend.

### P51 follow-up research: remove compensation, retain frame preparation

Owner asks whether the earlier restoration code should be withdrawn, then
requests investigation only. No formal public production edit is made.
History eaf05bff (T86 S1 P4) added capacity re-expansion after a successful
metadata call left one fewer row; its historical evidence does not isolate the
endpoint error or prove a separate need after correcting that error.

Controlled native comparisons use the unchanged current integration fixture
with its owner-authorized narrow-desktop expectations and task-local backend
copies. Exclusive-endpoint conversion with old repair passes x86/x64. Conversion
with every metadata size mismatch rejected fails both widths during palette
application: saved=80x25, actual=80x10, temporary viewport=20x10. Thus rejecting
all mismatches is not correct for a call that is about to draw a complete frame.
Conversion with capacity rejection only for restore_cursor (actual handoff),
and no re-expansion inside apply_display, passes the full native Console test on
both widths. Palette updates already invalidate old frame cells and subsequently
call the existing ensure_text_surface before drawing; that is the single needed
frame-capacity owner. Actual handoff must reject lost capacity rather than grow
it back and report content restoration. Keep cursor placement, final viewport
restoration and final handoff capacity checks.

The existing unit stub was reused in isolated dual-architecture observations:
- Normal restore: both variants return OK, zero resize calls, 120x60 capacity.
- Injected metadata truncation to 120x29: old compensation returns OK after one
  resize and one cursor operation; proposed handoff returns IO_ERROR (5), zero
  resize/cursor operations, leaving the failure observable.
- Palette truncation to 80x24: proposed helper returns to its existing caller;
  ensure_text_surface grows once to the new 80x25 frame, instead of restoring
  obsolete 80x30 capacity. No new path, state or recovery callback.
The complete existing Console unit fixture with corresponding contract updates
also passes x86/x64, exit zero. The redundant resize-fails-after-compensation
case is removed; real frame-growth failure tests remain.

Candidate production diff +7/-11 (net -4, including endpoint comment). Unit
fixture excluding scratch include-path changes +6/-16 (net -10); already
prepared native fixture +11/-5 (net +6). Console source plus tests total net -8,
excluding manifests/docs. Candidate tests are isolated evidence, not final
full-package qualification. No all-host guarantee or new guest acceptance is
claimed. Both apply_display consumers were rechecked: select_output requests
cooked handoff restoration (true); raw selection passes false and retains its
existing complete-frame restoration; write_text_frame_bound requests palette application then
uses existing surface preparation. Recommendation is this narrowed removal,
not wholesale deletion of display restoration or rejection of palette resizing.

### P51 approved implementation and final review

Owner: 批准执行 老规矩五件套. The researched backend and fixture changes
are now formal source. No additional public API or state is introduced.
The restore_cursor check applies to cooked native-history restoration;
raw selection already uses false and restores its retained complete frame.
That existing raw path is unchanged, not a newly introduced recovery route.
The metadata endpoint conversion is common to both callers. New-frame
capacity remains owned by ensure_text_surface, with its existing failure checks.

Similar-issue scan: rg for set_console_screen_buffer_info_ex and
console_broker_apply_display across src and test/lib finds one production
native setter, its Types declaration alias, two production helper call sites,
and the existing unit/native test wrappers. No second production setter or
unconverted metadata path exists. Dedicated SetConsoleWindowInfo continues
to receive inclusive endpoints. Bounds from fit_viewport keep the converted
exclusive endpoints within the signed native buffer dimensions.

Actual-change self-review: no replay/retry/state is added, saved cooked content
is not declared recovered merely because capacity was regrown, and raw frame
preparation remains unchanged. Existing tests retain real resize failure,
no-redraw content/cursor retention, palette growth and wide-buffer coverage.
The narrow desktop fixture derives its initial capacity/visible extent from
the actual host rather than treating 80 columns as a desktop prerequisite.
Final build/test results and artifact identities follow below.

Final qualification: cached x86/x64 configuration, test/product builds and
package publication succeed. Each architecture passes all 188 background tests
and all eight native desktop/audio/package tests: 392/392, with no retries or
skips in this final formal-source run. Earlier 390/392 and candidate outcomes
above are investigation history, superseded by this complete qualification.
The final incremental build log has no warning/error diagnostics; this is not
a claim that all unchanged original mirror sources were freshly recompiled.

Accounting against P50 HEAD 13987f6b (git diff --numstat):
- Production: mouse.c +3/-5 and Console backend +7/-11; total +10/-16, net -6.
- Tests: mouse +30/-0, Console unit +6/-16, native +11/-5;
  total +47/-21, net +26. No new fixture or production test API.
- Three manifest entries follow the changed source/tests; mirror inventory
  changes only for the two-line reduction already recorded above.
- No build configuration changes or source import. Shared tree changes are
  limited to src/lib and test/lib; no downstream synchronization is claimed.

Published compiler timestamps: x86 Nov 15 2026 08:59:46,
x64 Nov 15 2026 09:03:59; package checks verified the freshly built timestamp.
softpc32.exe SHA-256: f02b74596c6d4e09d3876c4193be3be8804837f8f78760a0001d25c9fa8d840f.
softpc64.exe SHA-256: 0372e5c192577a04180d228a146869a69724d51cc8067024383acf7b2a611e04.
snapshot SHA-256: 9c9e6116e4235c57d48f14a6267bb4b080942b0046a163344146e4a35dd2460f.

The snapshot identity equals P50; user INI and media are unchanged. Public
source/test manifests, Types/dependency/naming/test-boundary and XP mirror
gates pass in both suites. Review covers the actual five changed C files and
both apply_display consumers, not only test summaries. No new manual guest
acceptance or exhaustive Console-host/version coverage is claimed.
S4/T100 remain open; this P waits for owner acceptance.

## S4 P52: PPI file review

Owner accepts P51 and requests the next file, ppi.c, with all differences,
size, purpose, strong evidence and restoration candidates. P51 is accepted;
this review does not approve ppi.c retention or a snapshot schema change.
Fixed source is XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89,
base/keymouse/ppi.c; current source is unchanged from 17bccc35.
Existing #error bodies and necessary #line controls are excluded without a
new exclusion ledger. R0468's extracted notice is already gone.

Six actual text blocks, +27/-13 (net +14), form three semantic groups:

| Point | Current physical line / change | Size | Purpose and disposition |
| --- | --- | --- | --- |
| R0471a | 45, snapshot.h include | +1/-0 | Declaration owner for private PPI archive access; retain with that capability. |
| R0469a | 68, remove SPKRDATA_was_low | +0/-1 | Remove the state of the unused legacy speaker-notification route. |
| R0469b | 141, remove legacy speaker edge body | +0/-11 | Avoid invoking the bit-1-only path before timer_gate. |
| R0469c | 162, extend original HostPpiState selector | +1/-1 | Use original complete post-gate host sound route in the supported standalone build. |
| R0470 | 195, reset gate history | +1/-0 | Cold/repeated initialization consistency; retain recommendation. |
| R0471b | 200-224, capture/restore | +24/-0 | Snapshot capability; two lines associated with derived speaker_data_was_low are not independently needed. |

R0469 total +1/-13 is host-boundary selection, not a claim that original XP's
NTVDM configuration was defective. Both cached compile commands compile this
file with SOFTPC_STANDALONE=1 and without NTVDM/NEC_98; CMakeLists.txt lists
ppi.c and original nt_sound.c. Port 61h reaches registered ppi_outb. Its
selected HostPpiState runs after timer_gate; nt_sound.c:243 records bit 0 and
bit 1, and PlaySound:173 uses both with FreqT2. Original non-NTVDM callbacks
only occur on bit-1 edges and have no port-value argument. A 3->2 transition
changes bit 0 without a bit-1 edge: the legacy-only route cannot notify this
host owner of that changed bit. Compat audio.c:29 retains disable only for
BIOS reset.c:636. Restoring the legacy body/selector without changing the
sound host architecture loses this full-state notification; implementing another
interpreter in Compat would duplicate the original owner. The empty non-NTVDM
frameworks intentionally retain selected-content omissions, not #error branches.
No source-based case for an equivalent local restoration was found.

R0470 is a repeated cold-initialization boundary, not a new device reset
protocol. BIOS reset.c:724 calls ppi_init. A prior bit-0-high write leaves
gate_2_was_low false; XP ppi_init zeros ppi_register but leaves that history.
The next high write then skips timer_gate(RISE). S1 P103 records paired actual
PPI objects: current passes direct register/gate assertions on x86/x64;
removing only this line fails the gate assertion on both. The timing-bearing
sound failure in that investigation was not its decisive proof. Current
sound_smoke still checks reset register/gate and runs two real guest sound
program executions separated by Machine reset. P51 full tests passed these
fixtures; no fresh ablation is claimed by P52.

R0471 is snapshot capability, not an original defect. archive.c:154/222 uses
both accessors; ppi_inb toggles 0x30 and ppi_outb masks the register, changes
gate history and triggers side effects. Those ports cannot non-destructively
capture and restore the same private phase. Restore notifies existing original
HostPpiState after restoring the fields; archive restores PIT before PPI.
Current checkpoint_smoke:706-713 exercises the actual roundtrip after port
writes. This establishes the capability's need, not every serialized field.

Fresh whole-src/test search finds speaker_data_was_low only in this capture,
its range check, the struct, wire map and roundtrip assertion. Capture derives
it from register bit 1; restore does not consume it for device behavior.
Therefore its assignment and the extra validation operand lack independent
runtime necessity. Recommend a separately approved coherent removal of the
field, wire entry and assertion with the corresponding two mirror lines; keep
register/gate capture and HostPpiState. That reduces real XP additions by two,
not restoration of the complete snapshot block. It changes serialized layout;
do not leave an uninitialized wire byte, add old-format recognition, or silently
invalidate/convert the user snapshot in this read-only audit. Current schema
and asset remain unchanged. Prior blanket necessity reasoning for this field
is superseded; an existing format alone is not semantic necessity evidence.

Evidence boundaries: fresh pinned-source/caller/compile-command review, S1
P103 historical negative controls, and accepted P51 full 392/392 are distinct.
P52 adds no new runtime test, manual guest claim, production diff or binary.
All three groups await owner disposition. Next real-difference file is order
404 base/system/at_dma.c; ios.c has no real diff and is not reintroduced.

## S4 P53: PIC phase and PPI snapshot correction

Owner approves repairing L001 and removing the R0471 redundant field together,
retains R0469/R0470, requires current-only snapshot loading and verified one-off
conversion of assets/binary/snapshot, dual EXEs/publication/full regression,
commit/push/self-review and acceptance. This supersedes the earlier L001 deferral;
it does not approve other unreviewed mirror points or close S4/T100.

Plan and finite verification scope: original PIC sequence[-1,0,2,3,4] stays the
sole command-phase owner, moved from function-static to file-static in ica.c.
Existing capture/restore copies that field per adapter; the existing preflight
loop rejects all other values before either adapter is mutated. No port replay,
new state machine, new accessor or public component changes. R0471 removes its
derived speaker_data_was_low field from the struct, wire map, capture and range
check, retaining actual register/gate state and original HostPpiState delivery.
SCP5 is the sole supported format; no SCP4 reader, detection or compatibility
branch is added. Tests use the existing checkpoint byte stream and actual
archive serialization/controller functions: both adapters at all three partial
ICW boundaries and completion, power-on phase and invalid values -2/1/5.

Asset proof before conversion: original 18,700,025-byte SHA-256
9c9e6116e4235c57d48f14a6267bb4b080942b0046a163344146e4a35dd2460f.
The current field maps were evaluated in an isolated layout probe; queue has
one quick and two tick entries, device section begins at 17,831,642. Master
PIC has base80/mask152/ISR1/mode0x0211; slave has base88/mask156/ISR0/mode0x0a11.
SWPIC_outb's ICW1 clears mode to low four bits; only ICW4 sets ICA_MPM (0x10)
and completes sequence=0. Both saved controllers have that bit, establishing
completed initialization rather than guessing from successful file decoding.
PPI bytes are 0,1,1; the removed field is exactly the inverse of register bit1.

The one-off task tool inserts signed-zero phase fields (four bytes per adapter),
removes the one derived PPI byte, updates section length by +7 and format to
SCP5. Reversing exactly those edits reproduces the complete original file
byte-for-byte; CPU/RAM/media and other device/event bytes are unchanged.
Converted length 18,700,032, SHA-256
ff7c9db73fcb4d1aacfa11b0b11d9f8863aa5c4fe97ca0382cacc83b962cb44f.
The converter/old copy are task-only scratch, removed at delivery. No guest or
INI is edited, and no new source/binary/medium is imported.

Mirror accounting uses fresh fixed-XP comparison after excluding existing
#error bodies and line controls; prechange results match the current ledger.
- ica.c: 5 blocks +109/-7 -> 7 blocks +119/-13 (16 more changed lines).
- ppi.c: 6 blocks +27/-13 -> 6 blocks +25/-13 (two fewer changed lines).
- Total: 106 files, 15,709 blocks, +24,617/-24,018; net increase in mirror
  added-plus-deleted lines is 14. No new differing mirror file. This is distinct
  from tracked production net lines and includes relocation of original code.
R0469/R0470 are approved retained; R0471 retains only the necessary schema.
L001 is now an approved local snapshot-completeness repair, not an XP PIC bug.

Initial fixture run passed the new PIC cases and then failed an unchanged
assertion expecting format character '4'. It is corrected to '5'; generic
invalid-format rejection remains. Early isolated layout probes required C90
loop syntax and metadata-only compilation to avoid unrelated archive links;
those probe failures are not production defects or acceptance results.

Both architectures restore the converted real asset through Machine's state-write
executor path, reaching PAUSED with a published frame and saved PIC phase0,0,
base80,88, mask152,156 and ISR1,0, then stop/destroy successfully. This proves
archive/runtime loading, not new manual DOS/Win3.1/Win95 desktop acceptance.
Both also run the same checkpoint fixture linked to an isolated current PIC
object with only phase capture/restore removed. Both negatives fail the resumed
PIC base comparison (x86 exit 0xc0000409, x64 exit3); the repaired fixture passes.
The negative runner regards that assertion as expected, not a regression pass.

Bounded similar-issue sweep: rg sequence/ICA_MPM/ICA_ICW1_MASK in ica.c confirms
one original phase variable and unchanged ICW transitions; capture/restore are
its only new accesses. The existing restore preflight validates both adapters
before mutation. Archive's existing U32 field transport copies signed bits,
including -1; no new signed codec is needed. rg speaker_data_was_low/SCP4/
34504353 across src and test finds no obsolete field or legacy-format code.
PPI retains its original register and independent gate history; HostPpiState
still replays those values. This is not a completeness claim for all devices.

Final qualification: cached x86/x64 test builds, focused checkpoint tests,
package publication, 188 background plus eight native tests per architecture
all pass: full 392/392, no retry or skip in this final run. The earlier stale
format assertion failure is recorded above and was corrected before this run.
Published compiler timestamps: x86 Nov 15 2026 09:36:03; x64 Nov 15 2026 09:41:11.
EXE SHA-256:
- softpc32.exe: 32a41f1d9961500b27ab1f11642c3f40457a9ab84dc8b88c450ce908f7e91e0e.
- softpc64.exe: 27ed5edeebcb86e720e079a8b16bd8d1b58511f2162c234802bae339a2ed72e0.

Actual tracked diff against dfe955eb (git diff --numstat, src/test only): five
production files +20/-15, net +5; one existing test +61/-2, net +59. This adds
no fixture framework. Three artifact paths are snapshot and both EXEs; no INI,
media, guest, shared-package code or build script changes. Mirror files retain
CRLF, owned source/test retain LF; normalized source hashes match the manifest.
Existing preserved-mirror compiler warnings remain; no new owned/public warning
was introduced. Full governance/manifest tests and whitespace checks pass.

Coordinator actual-change self-review checks the approved scope and each source,
test, schema and asset diff: original phase storage is relocated rather than
duplicated, original ICW transitions stay unchanged, invalid restore returns
before mutation, allocation ownership remains in the existing archive path,
and PPI replay derives speaker output from the real register. Retaining R0469
and R0470 does not imply they are XP bug fixes. L001 repairs a local snapshot
omission. No new correction candidate or extra production path is introduced.
P53 is delivered for owner acceptance; S4/T100 remain open at the current audit
frontier rather than automatically moving to the next mirror file.

## S4 P54: at_dma file review

Owner accepts P53 (908f6803), then requests the next real-difference file,
order404 base/system/at_dma.c. P53 and its snapshot conversion are accepted;
S4/T100 stay open. Fresh comparison uses fixed XP SP1 a2f6d7c9 and current
908f6803, omitting excluded bodies and line controls, retaining conditional
framing and whitespace. All 16 text blocks reconcile to +115/-19:

| Point/current line | Text blocks | Added/deleted | Category and evidence |
| --- | ---: | ---: | --- |
| R0476, 192/223 | 2 | +2/-2 | Correctness: decode address/count port into channel before sampling SB DMA progress. |
| R0477, 743/746 | 2 | +2/-2 | Correctness: decrement wrap includes offset zero, so offset+1 units precede wrap. |
| R0478, 67/70/936/947/964/967/976/979 | 8 | +24/-13 | Correctness: existing helpers take unit size, decrement guest address and advance device buffer by a unit. |
| R0479, 874-882 and original position | 2 | +7/-2 | Integration: original dmaGetAdaptor made selected for DSP, plus unnecessary position change. |
| R0480, 46/1037-1114 | 2 | +80/-0 | Snapshot: header +1; existing controller/page capture/restore +79. |

R0478's complete mechanical breakdown: to-memory declaration +2/-1,
from-memory declaration +2/-2, two callers each +1/-1, two definitions each
+2/-2, two loop bodies each +7/-2. No other retained difference remains.
R0473-R0475 are already restored/removed, not newly pending points.

Production CMakeLists.txt:166 compiles at_dma.c for both widths. Compat
GFI's data/ID paths call dma_request (gfi_image.c:139/146/188). Original DSP
SbInitializeDMA calls dmaGetAdaptor at nt_sbdsp.c:747, then QueryDMA and
SetDMACountAddr operate on that exact controller, not a duplicate engine.
Archive capture/restore and wire map use the DMA snapshot extension at
archive.c:150/217/377/833/863. Port reads toggle first_last and can acknowledge
status; port replay cannot enumerate original base/current/temporary arrays.
Snapshot fields preserve original state, not additional device authority.

R0476 example: channel1 address/count ports2/3 must sample once on low-byte
read; XP compares address port to1 and count port to3, so address progress is
missed. Current test verify_sb_dma_read_ports covers channels1/3, direction,
low/high reads, other channels and disabled DSP. R0477: offset0 still permits
one transfer before wrap; XP splits zero units. R0478: byte reads originally
advance guest and retreat destination; word writes originally step individual
bytes. Both conflict with do_transfer's original documented unit/direction.
Existing verify_dma_transfers independently derives expected memory/device
bytes for two controllers, both directions, three transfer types, five starts,
guards and ROM protection. verify_dma checks completion and auto-init. XP
already restores auto-init address/count; that is not another local diff.

Historical S1 P106 actual-object negative controls restored each of the three
repair groups coherently: both widths failed respectively SB query-count,
DMA memory-data and returned-data assertions, while current fixtures passed.
This round freshly verifies source/callers and unchanged relevant assertions;
it does not relabel those negative executions as new. Accepted P53 full392/392
includes current DMA, SB-DMA and checkpoint tests. Checkpoint covers base/count/
page roundtrip; no exhaustive corrupt-archive or real-guest timing claim.
DMA restore's per-controller validation can partially mutate before rejecting
controller2; Machine's failed restore does not resume execution. No transactional
rollback contract or new recovery mechanism is proposed here.

Counter-review finds one restoration candidate in R0479: XP places its NTVDM
frame/accessor before the Internal functions heading; current moves both after
that heading. Exposure outside the excluded NTVDM body remains necessary, but
this location change is not. An in-memory, source-unmodified comparison moving
the frame/accessor back and restoring original surrounding whitespace reduces
16 blocks +115/-19 to 15 blocks +112/-17: five fewer changed lines. This is a
proposal, not implemented or dynamically validated. Retain the actual accessor,
three repairs and snapshot capability; do not delete production functionality
or move identical serialization into Compat merely to hide mirror differences.
All five groups await owner disposition in this fresh audit; next file is cmos.c.
No source/test/EXE/snapshot changes or runtime runs in P54.

## S4 P55: restore DMA accessor placement

Owner approves P54 findings and the usual dual build/publication/full regression,
commit/push/self-review delivery. R0476-R0480 necessary functionality is approved;
restore only R0479's unnecessary accessor/frame relocation. Original XP positions
are verified directly: accessor starts at859, internal-functions heading866,
do_transfer872. Preserve CRLF and exclusions, correct accessor line control to859
and reset to864 before original blank lines/heading. No DMA expression, signature,
function body, snapshot layout, guest or shared component is changed. No new test
is needed for source placement; run existing DMA/SB-DMA/checkpoint coverage in
full regression. Scratch ownership and budgets remain build/t100-s4, two hours
with fifteen-minute no-progress limit; background CMD only.

Actual fixed-XP excluded-body comparison verifies P54's estimate: 16 blocks
+115/-19 becomes 15 blocks +112/-17, five fewer changed lines. Current global
inventory is 106 files, 15,708 blocks, +24,614/-24,016. Existing excluded bodies
are not newly audited/bookkept. Non-comment/non-line-control token comparison
before/after is identical (including the remaining conditional directives).
Bounded placement review confirms the original heading is at XP866 and
original function entry at859; do_transfer stays at872. The only extra line
control restores heading diagnostics after moving the selected accessor.
No new declarations, state, callbacks, path, tests or public API are introduced.

Final cached x86/x64 test builds and package publication pass. Full tests pass
392/392 first run (188 background plus eight native per architecture), no skips
or retries; DMA, SB-DMA, checkpoint, package and native Console tests included.
Published compiler timestamps: x86 Nov 15 2026 09:54:19; x64 Nov 15 2026 09:58:46.
Existing preserved-mirror compiler warnings remain; no owned/public code changed.
- softpc32.exe SHA-256: 7921ca65d4bd30a0429ecf9d92d53ebf4819b1a4091a0a58d9f9e2122e077502.
- softpc64.exe SHA-256: fcff886d6229eae965b64f96e4e76126a00471b090e026c853016f82da588e6c.

Actual tracked production diff against cf30439b: one file +9/-9, net0; test
code unchanged. This is distinct from the five-line reduction against XP.
Coordinator self-review checks the actual source diff, conditional ownership,
line mapping, selected accessor caller and approved retention of R0476-R0480.
Only existing declaration/body placement and line controls change; one original
DMA state owner and original DSP access remain. No second path, new framework,
resource lifetime change or additional repair is present. The bounded similar-
issue review rechecks this file's full retained-content differences: no other
unnecessary location change is established; other files remain at their existing
audit dispositions. Snapshot SHA-256 remains
ff7c9db73fcb4d1aacfa11b0b11d9f8863aa5c4fe97ca0382cacc83b962cb44f;
INI/media are unchanged. Final normalized manifest and CRLF checks pass.
P55 awaits owner acceptance; S4/T100 remain open.

## S4 P56: firmware ROM provenance and resource dependencies

Owner directs discarding both diagnostic reversions: current 16-bit EIP wrap
semantics and direct CCPU timer dispatch remain unchanged. A diagnostic
worktree checkout exposed two independent build defects: the three embedded
ROMs inherited a text/CRLF checkout rule, and windres did not report its
RCDATA inputs to Ninja. A ROM change could therefore leave existing launcher
or test resource objects stale.

The gate marks `roms/*.rom` binary, verifies `bios1.rom`, `bios4.rom` and
`v7vga.rom` at configure time against the existing pinned XP manifest, and
adds all three inputs as `OBJECT_DEPENDS` of `firmware.rc`. Generated Ninja
rules confirm each firmware resource object, including all self-contained
machine/integration tests, explicitly depends on all three ROMs. No ROM bytes,
guest media, INI, CPU semantics or timer behavior are changed.

## S4 P57: CMOS file review

Owner advances from delivered P55 to the next file. Preserve the intervening
P56 firmware-provenance work at current HEAD29c41ecb; this review does not
alter or requalify it. Order405 base/system/cmos.c is compared against pinned
XP SP1 a2f6d7c9. Excluded bodies and line controls are omitted, including the
nested NTVDM rtc_init tick assignments: they are not extra live differences.
The inventory reconciles to four blocks +122/-4, two semantic groups:

| Point/current location | Blocks | Added/deleted | Classification |
| --- | ---: | ---: | --- |
| R0482a, cmos/port.h include at40 | 1 | +1/-0 | Host configuration declaration. |
| R0482b, fixed-disk selection at963 | 1 | +1/-4 | Report configured HDD topology instead of always claiming C. |
| R0483a, snapshot.h include at49 | 1 | +1/-0 | Snapshot declaration ownership. |
| R0483b, appended callback/state functions at1177-1294 | 1 | +119/-0 | Snapshot capability, including separators. |

Fresh caller proof: CMakeLists.txt:181 selects this original device owner;
original POST calls cmos_post. platform.c:232 reports configured HDD paths,
returning C type3 in the high nibble and D type4 in the low nibble. It owns no
CMOS register state. The always-present floppy controller is unaffected.
No HDD means type0, C only0x30, C+D0x34; D only0x04 is source-derived here.
Original XP unconditionally initializes disk=0x30, then optionally0x34, so it
cannot represent no configured HDD. Historical S1 P107 actual-controller
original-restoration negative fails fdc_smoke CMOS_DISK==0 on both widths.
Current fdc_smoke:235 retains that independent assertion and :237 expects
CMOS_DISKETTE0x40; dual_media_smoke:63 checks0x30. Do not conflate no HDD with
no floppy hardware or claim a fresh manual guest reproduction.

R0483 four function bodies (including GLOBAL line): callback encoder17 lines,
decoder12, capture30, restore56; together115 plus four separating blank lines
make the119-line appended diff. The three callbacks are actual originals:
rtc_periodic_event uses add_q_event_t; rtc_alarm and sync_rtc_to_host_time use
add_tic_event. archive.c:63/87 encodes/decodes their identities; :151/219 calls
capture/restore. The existing archive wire map carries state, not native function
or struct-tm pointers. CMOS bytes, selected index, conversion modes, alarm/IRQ,
tick count, calendar/time, periodic interval and event handle stay with their
original owner. Restore rebuilds cmos_register and conversion-function pointers.
Reading registerA clears UIP and reading registerC clears interrupt flags;
port replay is not equivalent to restoring private state and pending callbacks.
Moving equal serialization to Compat would expose private internals without
removing the capability's complexity. Returning to XP removes required archive
entry points; retaining the snapshot feature requires this access.

Evidence limit: checkpoint_smoke:482/603/635 proves byte0x5a survives capture,
mutation to0x42 and restore through the real archive; it does not exhaust every
RTC phase/mode/host-time combination. P55 full392/392 is historical qualification,
not a newly executed result at P56. Restore rejects invalid state and has no
transactional per-device rollback guarantee; a failed Machine restore does not
resume the guest. No claim of exhaustive malformed-snapshot coverage is made.

Counter-review: remaining statements all have current consumers/state owners;
no semantically redundant formatting, conditional relocation or added wrapper
was found in the retained diff. R0484 cast, R0485 comment and R0486 parentheses
were already restored in S1 P107 and are not pending retention items. In
particular the old parentheses concern caused redundant internal correction,
not a proven guest fault, and is not reintroduced. Recommend retaining R0482
and R0483, awaiting owner decision. No source/test/EXE/snapshot changed or
runtime tests rerun. Next real-difference file is ica.c; intervening zero-diff
files remain excluded from this review list.

## S4 P58: PIC file review

Owner approves R0482/R0483 retention and requests next file. CMOS points are
now approved. Review order407 base/system/ica.c at49f0729f against pinned XP
SP1 a2f6d7c9, preserving P56 work. Exclude existing error bodies and line controls.
Seven retained diff blocks reconcile to +119/-13:

| Current point | Added/deleted | ID/category |
| --- | ---: | --- |
| Headers at164-165 | +2/-0 | R0489 snapshot declaration +1; R0490 host EOI declaration +1. |
| Phase storage at214-222 | +7/-0 | L001 snapshot repair: relocate original static sequence for owner capture. |
| scan_irr at431 | +1/-1 | R0491 delayed-delivery integration. |
| Removed old function-static phase declaration | +0/-6 | L001 relocation, not a second phase owner. |
| SWPIC_clear_int at1244-1249 | +6/-0 | R0492 withdraw stale master CPU interrupt output. |
| ica_iret_hook_control at1680-1682 | +3/-6 | R0493 selected CCPU protocol adaptation. |
| Snapshot functions at1983 onward | +100/-0 | R0489 plus already accepted L001 phase capture/preflight/restore. |

Fresh production proof: CMakeLists.txt:178 compiles this controller with
EOI_HOOKS/CPU_40_STYLE/CCPU selected at275; archive.c:147/244 captures/restores
PIC state through these functions. Original IRR/ISR/mask/base/priorities,
request counts, nested ISR bookkeeping and IRET state are not reconstructible
from port replay. Capture rejects native callback pointers rather than storing
process addresses. Preflight rejects bad master/phase/depth before mutation.
The phase remains the original static sequence, now file-visible only. L001
was approved/delivered/accepted in P53: actual serialization at each partial
ICW boundary and completion on both adapters, invalid-phase checks, actual asset
loads on both widths, and missing-phase negatives failing resumed PIC base.
Those are P53 evidence, not new executions in this read-only review. Historical
L001-unresolved text is superseded by P53; do not resurrect that deferral.

R0490 supplies the actual host_EOI_hook and DelayIrqLine declarations without
including NTVDM native headers. R0491 adds the existing delayed mask to original
scan eligibility, keeping request accounting intact. nt_sbdsp.c:1497 invokes
host_DelayHwInterrupt; compat/eoi.c:60 queues release and raises the real PIC
request, :16 releases the mask and rescans. Without the added OR, requests are
eligible immediately. Existing verify_delayed_irq covers both adapters, IRR
presence before release, accumulated requests, release and cancellation. S1 P110
original-expression negatives fail the before-release assertion on both widths.
No extra timer, polling loop, queue or state owner is introduced by this line.

R0492 completes the master counterpart of the original slave withdrawal path:
when the final request disappears and scan returns no candidate, clear both
ica_cpu_int and host CPU INTR. Actual keyboard and DSP paths call ica_clear_int;
the unchanged slave code withdraws the master cascade. Historical P110 real-PIC
raise/withdraw/capture/re-raise probes pass with current code and fail with only
this six-line branch removed on both widths: cpu_interrupt remains asserted.
This proves output-state correctness, not a captured manual guest lockup.

R0493 keeps the original API but makes registration a no-op: keybd_io.c:1700
and timer.c:2550 request hooks, while selected c_main.c:4226 explicitly says
CCPU does not use IRET hooks; c_main/zfrsrvd receive hook_address but do not
complete the hook protocol. Historical P110 full-original-body negatives using
real keyboard registration/intack/EOI deliver vector9 three times then -1 on
fourth; current controls deliver all eight on both widths. This is a required
CCPU integration boundary, not a claim that XP's full native host is broken.
Implementing a new CPU hook lifecycle would enlarge scope and differences.

Counter-review recommends retaining all necessary groups. No inert conditional
relocation, redundant declaration or behavior-neutral restoration is established
in this file. Original PIC initialization logic is unchanged; phase snapshot
repair is already accepted, not an additional request. The two masks serve
distinct delay versus IRET purposes; this review does not delete original
bookkeeping or alter snapshot schema. Six superfluous explanation lines were
already removed in S1 P110. Current branch/comments do not establish another
safe restoration. Pending fresh owner disposition: R0489-R0493; L001 remains
accepted. No source/test/artifact/runtime change in P58; current full tests are
historical evidence only. Next real-difference file is quick_ev.c.

## S4 P59: quick-event file review

Owner approves R0489-R0493 retention; mark those points approved, L001 already
accepted. Next order410 base/system/quick_ev.c atf2de4e11 is compared against
pinned XP SP1 a2f6d7c9. Existing error bodies/line controls excluded: one live
addition block +230/-0, R0496 snapshot capability, not scheduler bug repair.
Original add/delete/dispatch/calibration bodies are unchanged. R0495 notice and
R0497 extra explanation removals are historical, not pending differences.

| Current function location | Lines including LOCAL/GLOBAL | Purpose |
| --- | ---: | --- |
| snapshot_capture_list,1001-1050 | 50 | Copy ordered event records and preserve hash-chain membership as indices. |
| q_event_snapshot_capture,1052-1078 | 27 | Validate buffers, capture both lists, handles and live countdowns. |
| q_event_snapshot_measure,1080-1097 | 18 | Count records before existing archive allocates capacity. |
| snapshot_restore_list,1099-1168 | 70 | Decode callbacks/build nodes and validate links; free partial allocations on failure. |
| q_event_snapshot_restore,1170-1229 | 60 | Prepare both queues before replacing live queues, retain counters and handles. |
| Separating blank lines | 5 | Part of the addition block; total230. |

CMakeLists.txt:179 selects the original scheduler. archive.c:139/144/229 calls
measure/capture/restore; callback IDs are encoded/decoded by existing device
owners. These records are archival copies, not a second scheduler. Original
add_event immediately invokes a zero-delay callback (quick_ev.c:334), allocates
new handles and adjusts timing; replay cannot preserve saved handles/countdowns
or guarantee callback-free load. Original init is reused for cleanup instead
of a parallel disposal implementation. Restore stages quick and tick lists,
then exchanges state only when both succeed; late failure preserves live queues.
Private heads/hash chains/countdowns are not exposed through original add/delete.
Relocating equal code into Compat would require exposing those internals.

Current checkpoint verify_event_queue_archive captures two quick events and one
tick event, rejects a late invalid callback with live counter/state preserved,
restores without callbacks, then observes order41,42,51. Original zero-delay
execution is separately asserted in verify_event_queue. queue_archive_smoke
calls the actual helper in the same compilation unit: valid hash prefix,
collision handles1/17, orphan after a valid prefix and empty-list orphan.
Historical S1 P113 passed both-width checkpoint/queue fixtures. These prove the
listed paths, not every allocation failure, malformed graph or guest timing.
This round rereads current assertions/callers; it does not claim new executions.

Concrete reduction candidate: all current measure consumers are archive.c:139
(reads only quick_entries/tick_entries before calling capture) and mouse_smoke.c:
172 (reads only quick_entries). Header adds no promise that measure captures
other fields. Its last four assignments duplicate capture's work with no reader:
state->next_quick_handle = next_free_handle;
state->next_tick_handle = tic_next_free_handle;
state->quick_count = host_q_ev_get_count();
state->tick_count = tic_ev_get_count();
Delete only these four measure assignments; retain state zeroing, count loops,
null check and all actual capture/restore assignments. Same signature/schema,
no snapshot conversion, new helper, second path or change to event dispatch.
Expected real mirror diff +226/-0 (four fewer lines), still one block. Production
expected +0/-4, tests need no new mirror API. Existing archive/mouse/checkpoint
regression should verify the unchanged consumer behavior if owner approves.

Counter-review does not remove capacity validation, hash membership checks,
callback rejection or staged cleanup merely to reduce size: each protects the
existing archive/ownership contract. Count-only measure and complete capture
have distinct callers/purposes; removing measure entirely would require a new
allocation or retry interface. Recommend retain R0496 capability and narrow this
unused output work. No production/test/EXE/snapshot modified. Prior blanket
necessity statements for every measure assignment are narrowed by this fresh
caller audit. Approval remains pending. Next real-difference file is rom.c.

## S4 P60: remove unused queue measurement outputs

Owner approves retaining R0496 capability and deleting only measure's four
unused handle/countdown assignments, with dual cached build/publication/full
regression and commit/push/self-review. Baseline cefa3fae. Capture remains the
sole complete archive producer; signature, state struct and wire format do not
change. Production expected +0/-4, tests unchanged, real XP diff +226/-0.
Existing archive/mouse/checkpoint/queue tests cover the actual count consumers.
No new test-only API or zero-field assertion is added.

P60 owns only build/t100-s4-p60 (two-hour job, fifteen-minute no-progress budget).
The concurrent Win95 probes use build/t100-s4/win95-driver-bisect/softpc64.exe;
initial inventory finds PIDs13640/26964 there and no shared Ninja build job.
Do not stop those processes, touch their assets or clean build/t100-s4. Existing
package fixture filters windows by its own child process ID and terminates only
its own process handle. Reusable Ninja trees are idle before P60 starts.

Actual source audit: git diff --numstat from cefa3fae is +0/-4 in quick_ev.c;
no test code changes. Byte comparison after LF normalization confirms all
content outside measure is identical; the working mirror retains CRLF. Its
normalized SHA-256 matches the updated XP manifest. Real mirror totals become
106 files, 15708 blocks, +24610/-24016; only four added lines disappear, with
no new block or file. R0496 body is approved. The only production measure
caller reads quick_entries/tick_entries before complete capture; mouse smoke
reads quick_entries. No second archive path or legacy format is introduced.

Published SHA-256:
- softpc32.exe: 17ba89e348309326ef7ac3097d17180a81d87ee97ea390fe6b94c784aaea5e35
- softpc64.exe: e9f23efe6224cc17d20d117d55f984b2fd7931488663faa55a0164f2834da0e4

Package snapshot and user INI hashes are unchanged from P60 start. Compiler
timestamps verified by publication are Nov 15 2026 12:34:29 (x86) and
Nov 15 2026 12:40:05 (x64).

Qualification: both cached Ninja/ccache configurations, full builds and package
publication passed. x86: 188/188 background (185.57s), 8/8 native (24.57s).
x64: 188/188 background (213.33s), 8/8 native (25.17s). Total 392/392, first
run, no retry or skip. Native coverage includes audio/MIDI, both package modes,
Window retirement/modal and Console display. Full gates include documentation,
manifest, component boundaries and source policy. No warning originates outside
the preserved mirror; existing mirror warnings remain outside this cleanup.

Coordinator actual-diff review confirms precisely four unused assignments
removed, unchanged capture/restore and call signatures, no snapshot/INI change,
no new state, branch, API, allocation or test framework. Existing consumer
regressions suffice; this delivery does not claim exhaustive malformed-archive
or allocation-failure coverage. Only own scratch is retired; the concurrent
Win95 investigation directory remains protected, so global build residue is not
claimed clean. S4/T100 remain open; P60 awaits owner acceptance.

## S4 P61: accepted P60 and ROM file review

Owner accepts P60 eb8e706d and requests the next real-difference file, literal
order411 base/system/rom.c. Read-only source review against pinned XP SP1
finds three retained-content blocks, +1/-3; excluded bodies and line controls
are not re-audited/accounted. No production/test/artifact change is made.

| Point | XP / current anchor | Size | Purpose and disposition |
| --- | --- | --- | --- |
| R0499 | XP585 / current442, do_search_for_roms | +1/-1 | Original defect repair: remove UCHAR truncation of CS; recommend retain, pending owner decision. |
| Original opening scope omission | XP788 / current before608, display_string | +0/-1 | Deleted ordinary opening brace; no necessary behavior or ABI purpose. Recommend restore. |
| Original closing scope omission | XP869 / current before681, display_string | +0/-1 | Matching deleted closing brace; restore together, never individually. |

Actual production path is original reset.c calling search_for_roms, then
signature/checksum validation, original FE00:095A return setup, setCS/setIP and
host_simulate. Both cached compile commands include rom.c in machine-devices
with CCPU/CPU_40_STYLE; this is not a test-only implementation. setCS maps to
c_setCS, whose original parameter is IU16. At ROM C8000 the entry must be
C000:8003 (physical C8003). XP's UCHAR cast converts C000 to 00, selecting
0000:8003 (physical 08003). This is a guest segment truncation, not host-pointer
adaptation, snapshot support, or a product-specific workaround.

Fresh existing-fixture runs on x86 and x64 both pass device-contract rom using
P60-built binaries. The generated checksum-valid 512-byte ROM executes
mov ax,cs; mov [0500],ax; retf through the actual scanner/CPU/trampoline;
observed bytes at0500 are 00 C0. No guest or firmware asset is modified.
The original-cast negative is not rerun in P61; source arithmetic establishes
the wrong entry address, while the fresh positive proves current execution.
This is not a claim of observed Windows failure or all ROM/protected-mode cases.

The two omitted braces surround display_string's existing body. They are
outside ARCX86's conditional body: restoring them does not restore UseEmulationROM
or any excluded branch. Existing local declarations and uses remain inside the
same pair; cur_loc update remains after it. There is no necessary scope escape,
new caller, state owner or ABI rationale for the omissions. Restoring precisely
the original pair would be production +2/-0 while reducing real XP differences
from three blocks +1/-3 to one block +1/-1 (two fewer changed lines). Current
106-file aggregate is unchanged pending approval. No other nonexcluded diff
or recoverable formatting difference is found in this file.

P61 uses only build/t100-s4-p61 for serial hidden-CMD fixture execution and
final documentation gate (two-hour job/fifteen-minute no-progress budget),
then removes it. Concurrent Win95 build/t100-s4 remains protected. P60's
392/392 full result is previous qualification, not a newly repeated full run.
Next file is timer.c; do not advance its review before owner decision.

## S4 P62: restore original ROM display scope

Owner approves R0499 retention and restoration of the original display_string
brace pair; baseline 76717b54. Only two original lines are restored, preserving
CRLF and original line control. No selected statements, excluded body, API,
snapshot schema, user configuration or media change. Tests unchanged.
Real fixed-XP diff becomes one block +1/-1 instead of three +1/-3; aggregate
106 files, 15706 blocks, +24610/-24014. R0499 is approved.

P62 owns only build/t100-s4-p62, with two-hour job/fifteen-minute no-progress
budget. Win95 investigation scratch/processes are protected. Initial inventory
finds PID29264 running the published x64 EXE; do not stop it pending ownership
clarification. No shared Ninja build/test process was present. Begin with x86.

Owner then explicitly authorizes closing the occupied published EXE, confirms
investigation has copied its own executable, and reiterates standing permission
to close publication blockers. PID29264 identity was checked against the exact
published softpc64.exe path. CloseMainWindow returned false; only that process
was terminated. No investigation scratch or copied binary was changed.

Independent comparison with pinned XP (excluding error bodies/line controls)
confirms exactly one retained block +1/-1. Original braces preserve the body
scope and leave cur_loc update outside it. Existing R0499 and all statements
are unchanged; no new test duplicates this structural restoration.

First x86 full background run passed 187/188; checks/xp_mirror correctly
rejected an editing error in the manifest: the new local hash was initially
written into the subset column instead of the local column. Restored the pinned
subset hash and updated only local_sha256. No source/test change followed.
The gate rerun passes 1/1; all eight x86 native tests pass. Do not describe this
as a first-run full pass. Scope/CRLF comparison confirms exactly two brace
insertions; selected statements and R0499 are byte-identical after normalization.

Published compiler timestamps: x86 Nov 15 2026 12:53:04; x64 Nov 15 2026
12:58:32. Publication validates timestamps in both EXEs. SHA-256:
- softpc32.exe: 172b27757710041d600bc8860de8a40b48812e27dac92f85e4810d051ba77c97
- softpc64.exe: 071cafca2a2ed6acd34792bff7c879c1f63fc8999bda983290c500b4adbb962f

Snapshot and user INI hashes remain unchanged. Build warnings are confined to
preserved mirror code. No production branch, type, allocation or error path is
added. Actual source accounting is +2/-0, tests +0/-0; the desired reduction is
in fixed-XP differences (two fewer lines and blocks), not production line count.

Final qualification: x86 background 187 initial passes plus corrected mirror
gate pass, native 8/8 (25.57s); x64 background 188/188 (214.61s) and native
8/8 (25.70s), first run. All 392 distinct tests have passing results, none
skipped; x86 initial background elapsed190.94s. ROM/device, snapshot and public
component regressions are included. Both cached builds/publications succeed.
Final coordinator actual-diff review confirms only the two original braces,
correct local manifest column, unchanged upstream/subset identities and R0499,
no duplicate state/path and no snapshot/INI modification. P62 scratch is removed
after jobs exit; concurrent investigation scratch remains protected. This is
P62 delivery awaiting owner acceptance, not closure of S4/T100.
