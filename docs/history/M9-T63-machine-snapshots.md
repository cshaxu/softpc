# M9 T63: machine snapshot implementation record

## S7 P8: executor-owned state transfer

The VM driver owns the sole safe-boundary capture and stopped restore
transaction behind Common's two opaque state-transfer callbacks. It does not
add a Common lifecycle state or alter Lib. A focused real-driver smoke proves
running-read/paused, corrupt-stopped-write/stopped and
valid-stopped-write/paused before ordinary resume and stop.

## Scope and admission

Owner approved the revised [snapshot proposal](../proposals/m9-machine-snapshots.md):
read only while running, write only init/stopped, success ordinary paused;
VM owns safe-stop selection and a single 1-second monotonic deadline. Lib is
unchanged; Common may add only the two state operations and necessary wiring.
S2 remains active. This record is partial proof, not snapshot acceptance.

## S3 P3: corrected existing-Storage decision

The initial S3 stop claim was too broad. `lib_storage_medium_open(path,
LIB_STORAGE_MEDIUM_READONLY)`, `lib_storage_medium_byte_count`, and
`lib_storage_medium_read_at` already form a bounded, chunked reader for an
arbitrary snapshot file. The snapshot path must not use `read_owned`.

The owner explicitly accepts ordinary `TRUNCATE` writer publication: a failed
save may truncate or partially replace a prior same-path snapshot. Consequently
the initially proposed temporary-file/commit extension is withdrawn. Lib and
test/lib remain unchanged; App retains file ownership, VM/Compat retain state
format and media ownership. This only corrects the boundary decision; it does
not claim that a snapshot container or state serializer exists.

## P2 pre-audit: producer capture barrier

Baseline cce6fa9. Counted scope: compat/platform.c, compat/platform.h and the
existing test/unit/platform_failure_smoke.c. Estimated production addition
25-40 lines; test addition 40-70. No original mirror/Common/Lib edits.

The original timer callback increments pending ticks and signals an existing
executor event. Snapshot capture must stop/join that producer without invoking
host_timer_shutdown, which also closes the wake event and resets failure state.
The same original 50 ms timer creation is reused, not a second clock. Pending
ticks remain accepted device work, not disposable timing noise.

The new executor-owned set_clock_running operation disables and synchronously
joins only the producer, or enables it with the original due/period. Failure
returns false; the caller must not capture or resume as though it succeeded.
Disabling twice is harmless. Enabling does not synthesize missed ticks. The
ordinary pause path is unchanged; later snapshot wiring must use this barrier.
Executor readiness still requires a live timer: enable before resuming CPU.

The deterministic fake completes one final callback during the delete/join.
Checks cover preserved pending ticks/event, idempotent stop, delete failure,
creation failure and no catch-up. Existing real HLT failure proof remains.

Actual production: +29/-7 = +22 in two C/H paths; tests +56/-1 = +55 in one
path. Original mirror diff: zero. Fixed EXEs rebuilt for regression, but do
not expose save/load yet. Full delivery verification is recorded below.

## Execution-boundary findings (not yet a restoration proof)

| Entry | Evidence | Required disposition |
| --- | --- | --- |
| Post-retirement host event | c_main.c, before start_trap/DR/IRQ handling | Not a plain pre-fetch snapshot point; pending inter-instruction work cannot be dropped. |
| NEXT_INST | c_main.c, setup IP/RF/TF and decoding entry | Candidate canonical entry; prove initialization/reentry does not reset relevant state or duplicate quick ticks. |
| HLT loop | c_main.c advances IP before waiting | Preserve halted execution phase; resume must wait for eligible interrupt rather than execute following instruction. |
| Debug begin | vm/debug.c before decoding | Keep exact stepping/breakpoints; not proof of outermost invocation. |
| Nested BIOS | keybd_io INT15, floppy/floppy_i, video callbacks | Wait for natural return under VM deadline; never serialize native stack or forcibly unwind for a successful save. |
| Native HLT wait | compat/platform.c waits indefinitely on executor event | Deadline needs an explicit bounded wait while preparation is active; normal idle wait remains event-driven. |
| Timer producer | compat/platform.c callback | P2 provides isolated producer join; device/queue serialization is not yet implemented. |

Blocking-call sweep also finds synchronous medium read/write in gfi_image.c
and hdd_media.c, and synchronous trace writer calls in vm/trace.c. These cannot
be interrupted by checking a deadline between CPU instructions. Consequently
the agreed 1-second deadline is observed at execution checkpoints, not a hard
wall-clock return guarantee during an OS file operation. Audio waits are on
the separate speaker worker, not the CPU executor. No second I/O implementation
or Lib cancellation API is introduced to hide this distinction.

The original setjmp-based simulation stack is not serialized. MVDM hook changes
and their original-diff estimate must precede the next code part. S2 still owes
the selected mutable-state ledger, canonical CPU/HLT reentry proof, timeout
coverage and end-to-end request wiring design. No claim of cross-process
restore, file safety, or complete device capture follows from these tests.

## P2 delivery verification

- Both `tests-x64` and `tests-x86` build presets completed successfully and
  refreshed the fixed packages. Existing original-source compiler warnings
  remain; this is not a warning-free build claim.
- Full `test-x64`: 101/101; full `test-x86`: 101/101. Focused x64 platform
  failure/lifecycle checks: 2/2. Documentation governance and diff checks pass.
- Lib/Common/MVDM and shared tests have zero diff from P1. No INI/media edits.
- x86 SHA256: `9FABCD2A39DE89FB713CD05BCF6D4C5F116B561BCA7777152DBA9F94A6853855`.
- x64 SHA256: `FC43874F81FE5BE17AA95893496D15E400E754290F598BB1BED03803C2F0F711`.
- S2 is not closed: the isolated timer barrier is only one required piece of
  the safe-stop and recovery proof; no save/load command is available yet.

## P3 pre-audit: precise CPU checkpoint observation

Baseline 1f9e6e5. Scope: compat/ccpu/lifecycle.c/.h, original c_main.c,
test/unit/checkpoint_smoke.c and its root CMake wiring. Estimate 40-60 production
lines and 100-140 test lines; original diff only two observation calls/comments,
no new branch interpreting product state. Common/Lib are not changed.

The existing TLS recursion count is authoritative. An executor-bound observer
receives that count and the original CPU's fetch-versus-halted phase. There is
no second CPU or guest breakpoint. No observer means no product behavior change.
If stop is requested during observation, the existing outer-exit mechanism
runs before another instruction; it is not used to claim a nested save succeeded.

The test runs real CCPU bytes with a disposable 512-byte image in the selected
build tree (15-second CTest timeout; test removes it on success). A test-owned
BOP calls the nested original CPU, retains a local return IP, and completes
only after BOP FE returns normally. Observation must distinguish depth two from
depth one; outer HLT must expose already-advanced IP. Stop there must not run
the following INC AX. The injected BOP and observer are removed before teardown.
This establishes observation, not complete saved-state restoration or timeout.

## P3 implementation review

Actual counted production C/H paths: lifecycle.c +19/-0, lifecycle.h +8/-0,
original c_main.c +4/-0: total +31/-0. The original-source delta is only two
calls and their reason comments. One new unit test is +87/-0; root CMake wiring
is +6/-0, counted separately. This is below the pre-audit estimate because the
existing recursion ledger and outer-stop mechanism were reused.

Similar-path sweep (`rg` for lifecycle enter/leave/checkpoint/observe,
host_simulate, NEXT_INST, DO_INST and quick_mode across Compat/VM/c_main):
the original c_cpu_simulate remains the only recursion ledger entry/exit;
host_simulate and its function slot both call it. The new observer is TLS and
executor-bound. No observer is installed by the product yet. Quick execution
still uses the existing pending-executor-event escape to NEXT_INST; this is
not a claim that the observer runs on every quick instruction. HLT has its own
observation because its wait does not retire another instruction. Stop uses
the existing outer exit, while the successful nested test requires natural
BOP FE return. Debug begin/retired hooks and their callbacks are unchanged.

Canonical restore still needs separate proof: c_cpu_simulate performs
SYNCH_TICK, and NEXT_INST initializes RF/TF and operand/address modes. Those
effects must not be accidentally duplicated when installing saved state.
The new test does not claim to prove that remaining restoration contract.

## P3 delivery verification

- Both fixed package builds completed. Full x64 suite: 102/102; full x86:
  102/102. Focused x64 checkpoint/platform-failure/lifecycle: 3/3.
- Documentation governance and diff checks pass. Both owned checkpoint images
  were removed by the tests. No configuration or user media was changed.
- Lib/Common and their shared tests remain byte-for-byte unchanged from P2.
- x86 SHA256: `B6D279A508B69D15B19C76A6A294D180653842E9E41EC7BFA8DCEA2AC1B79F11`.
- x64 SHA256: `94E37355B8293C715293ED37146DEBF6DDD224F81CAB7ECF4F2F4975BB15A7A6`.
- S2 remains active; this P completes checkpoint observation only. The selected
  state ledger, restore-entry and VM deadline proof remain before S2 closure.

## P4 pre-audit: reconstruct the CPU continuation

Baseline d52a901. Estimated production churn 60-90 lines in the same lifecycle
C/H and c_main.c; extend checkpoint_smoke by about 100-150 lines. No Common/Lib
change. Reuse the real CPU loop, exception frame and HLT loop, not a new decoder.

The selected CCPU has PROD and no PIG/SFELLOW/SYNCH_TIMERS (root target definitions
and source define sweep). SYNCH_TICK on simulate entry is therefore empty.
FETCH capture precedes RF/TF and host-IP preparation, so resume must enter there
without first consuming another host tick/pacing step. HLT capture follows IP
advancement and must resume the original wait, retaining start_trap separately
from current EFLAGS (a paused debugger may previously have edited flags).
Decoder pointers are rebuilt, never persisted. HLT continuation sets equal null
decode pointers so the original post-instruction IP adjustment does not repeat.

The internal copied continuation is phase plus pending trap. An executor-only
resume call validates it at depth zero, establishes the usual original CPU
entry, and consumes it once before dispatch; nested/exception entries cannot
reuse it. Ordinary run, debug and reset do not select this entry. Tests will
prove unchanged IP/quick counter on reentry, no second HLT execution, and the
STI shadow using a real pending PIC IRQ whose handler observes the next INC.
This proves continuation reconstruction using existing machine state, not yet
cross-process restoration of the rest of the machine or a complete snapshot.

## P4 implementation and path review

The observer now copies `{halted, trap}`. FETCH always supplies zero trap;
HLT supplies the original `start_trap`, not an interpretation of current TF.
Compat validates this finite representation before entering at depth zero.
The borrowed resume descriptor is consumed exactly once; normal nested entries
and exception reentries cannot replay it. c_main reuses its original two labels:
FETCH immediately before phase observation/setup; HLT immediately before wait.
No guest instruction, interrupt dispatch, port behavior or ordinary pause was
reimplemented. Caller still owns the existing run-exit cleanup and clock.

Focused real-CCPU proof covers natural nested BOP return; unchanged HLT IP and
quick count after two separate reentries; pending trap surviving an intervening
TF edit; rejected invalid/recursive entry; then an ordinary run (no leftover
entry), saving immediately after STI, injecting real PIC IRQ0, and reentering.
The IRQ handler stores AX and must see the INC following STI. A premature IRQ
would store 0x10 instead of the required 0x11. The final CLI/HLT stops normally.
The fixture remains the same disposable 512-byte image; no external media.

The first test rebuild exposed missing legacy include paths, then base_def.h's
non-ANSI `const` macro. The test takes the same include paths as irq_smoke and
undefines that macro after legacy headers. No mirror header was changed; an
old executable's early test pass was discarded. Fresh rebuilt proof passes.

Actual P4 production C/H: +49/-6 = +43 across three paths; mirror alone +16/-2
= +14 in c_main.c. Test +116/-3 = +113 in checkpoint_smoke.c; CMake +3/-0.
Counts exclude documents/packages. The estimated production churn was larger
than required because no new CPU simulation wrapper or execution loop was added.

Continuation-state inventory for the selected c_main object was cross-checked
with `nm --defined-only` B/D symbols and source declarations. GR/SR/CR/DR/TR,
IP/CPL/FLAGS/SAR/STAR, SasWrapMask, cpu_interrupt_map and cpu_heartbeat belong in
the S4 state payload; start_trap belongs to the HLT continuation proved here.
CCPU_WR/BR alias GR and CCPU_M aliases owned RAM; these are rebound, not saved
addresses. p/p_start/pg_end and operand decode scratch are regenerated at FETCH
or unused until HLT retires. The source's PIG-only single_instruction_delay is
absent from the selected object. This is a CPU-entry inventory checkpoint, not
the complete S2 all-file mutable-state ledger or its closure claim.

## P4 delivery verification

- Fresh fixed x86/x64 builds completed, both full suites 102/102; original
  source compiler warnings remain outside this proof. Fresh focused x64
  checkpoint test passed before full regression.
- Lib/Common/shared tests and user INI/media unchanged. Disposable checkpoint
  fixtures removed on success. Documentation governance and diff checks pass.
- x86 SHA256: `91BA0F02331162EFB18A9F3E6FF905499032188FAF379E0337F2776667155307`.
- x64 SHA256: `A7F23DAA4EFCA0658F4E1EEE274A580FF9DDA76C5FB4B5E71B37D1CFE32AFAE8`.
- S2 remains active for deadline/combined barrier and the full selected-state
  inventory. There is still no public save/load or complete state serializer.

## P5 pre-audit: VM capture deadline and producer barrier

Baseline a19b890. New vm/snapshot.c/.h owns only the pending capture operation,
its one-second monotonic deadline, copied CPU entry and joined clock barrier.
Estimate 80-110 production lines, 100-140 test lines plus CMake wiring. Lib and
Common are unchanged; no additional MVDM hook is required. The later state
encoder and driver read slot will consume this same VM implementation, not a
second capture state machine or additional Common preparation API.

Checkpoints come from the original CPU observation, not a polling thread. While
waiting, the original 50 ms producer continues to wake a halted CPU; only a
successful depth-one capture stops it. Timeout/failure does not force nested
unwind or export data. The operation's caller must use ordinary pause for a
failed request; P5 proves the boundary result, S7 wires Common's pause fact.
Finalization restarts only a successfully joined producer. Failure remains
explicit; it cannot be treated as permission to run without a clock.

A deterministic standalone test includes the actual VM implementation with
clock/timer test functions, as existing failure probes do; it has no product
or media dependency. Checkpoint_smoke also uses this operation against the real
timer, requesting from inside a nested BIOS callback and completing after its
natural return. Existing 15-second owned-fixture limits apply.

## P5 implementation and proof scope

VM snapshot owns IDLE/WAITING/READY/FAILED for this one operation, not machine
state. begin records counter/frequency only on success; a second begin fails.
checkpoint completes once: elapsed >= one frequency unit is timeout even at
depth one; nested entries keep waiting; only a timely outer entry joins the
clock and copies the CPU phase. No stack return or lifecycle event occurs here.
finish enables a joined clock before clearing state; failed activation retains
ownership and returns IO_ERROR. There is no automatic retry loop.

The fake test proves 999/1000 ms, nested HLT timeout, clock query/frequency
failure, producer stop/start failure, one completion and idempotent finish.
Time spent after READY is not the boundary deadline. The real checkpoint test
requests while inside a nested BOP, completes only after natural return, and
joins/restarts the actual producer. Its second program never returns from
nested HLT: the real monotonic deadline reports failure and the test itself
issues stop for cleanup. No successful snapshot is claimed on that forced exit.
An initial test-only iteration cap fired before one second on the original
HLT busy path; it was removed in favor of the agreed time-only criterion.
The failed test's owned image was removed and the rebuilt test then passed.

The P2 HLT wait concern is resolved without another wait API: while WAITING the
existing 50 ms producer remains active; it is joined only after a depth-one
boundary succeeds. The real nested-HLT test covers this deadline delivery.
OS scheduling and synchronous external I/O still have the previously stated
limits; this does not claim a hard real-time preemption guarantee.

Actual production +81/-0 in vm/snapshot.c/.h. Tests +142/-1 = +141 across the
extended checkpoint test and new snapshot_boundary_smoke; CMake +7/-0.
MVDM, Lib and Common unchanged in this P. Driver/Common operation wiring and
ordinary-paused completion on timeout remain S7 work, not an extra public API.
The combined CPU/timer boundary is now exercised, but the full selected-state
ledger and serialized device state remain incomplete; S2 cannot yet close.

P5 verification: both test/package builds completed; full x64 and x86 suites
each passed 103/103. The previous failed checkpoint entry in LastTestsFailed
was stale: the completed x64 LastTest contains 103 passes and zero failures.
Both owned checkpoint media files are absent after the successful runs.
No changes to Lib/Common/shared tests, user INI or original mirror in P5.
Package SHA-256:

- x86: `9DE0913E287AB619C68F15FF4B0AB8928628F32C1F7BBA9B33DB282852B88999`
- x64: `4026853D69925AB4E856C364E3ABDC45C33713F4D019C856E504F86BD31C7EC9`

## S2 P6 CPU/FPU/RAM inventory and translation proof

Before: no production edit. Extend checkpoint_smoke by approximately 30 lines,
using the original c_tlb interface, and add its private include directory.
This refines the admitted state inventory; it does not implement S4 early.
P5 actual-commit review passed five focused tests on each width, HEAD equaled
origin/main and the worktree was clean before P6.

### Frozen inventory slice

At 3373006 both built machine archives have the same 88 named B/b/D/d symbols
in these five objects: c_main=41, c_tlb=4, c_xcptn=9, fpu=32, memory=2.
Reproduce with `nm -A --defined-only` on each fixed build's
libsoftpc-machine.a, selecting those object names and symbol classes, excluding
dot-prefixed compiler sections; strip the x86 external leading underscore for
name comparison. Object symbols are a cross-check, not a heap/field inventory
or proof that every D symbol is mutable. This slice does not cover the other
selected device, SAS, C-VID, VM and Compat objects; S2 remains open for them.

The receiver for all saved fields below is S4's fixed CPU/FPU/RAM section;
each original file retains ownership and needs only its narrow state port.
Validation/roundtrip proof belongs there, not in Common/Lib. Save means copied
fixed-width values, not structure padding or address bytes.

| Owner / exact symbols | Disposition and fields |
| --- | --- |
| c_main: CCPU_GR[8], CCPU_CR[4], CCPU_DR[8], CCPU_TR[8], CCPU_IP, CCPU_CPL, CCPU_FLAGS[32] | Save exact original scalar register state, including the per-bit flag representation. Do not use register setters that execute MOV_CR and flush translation state during commit. |
| c_main: CCPU_SR[6] | Save selector, ar_dpl, ar_e, ar_r, ar_w, ar_c, ar_x, base and limit for each hidden segment. Loading selectors through debug would reread possibly changed descriptors and is not a restore. |
| c_main: CCPU_SAR[2], CCPU_STAR[2] | Save LDTR/TR selector, ar_super, base, limit; GDTR/IDTR base and limit. No descriptor-memory lookup during installation. |
| c_main: CCPU_MODE[3] | Save operand/address mode and POP displacement. FETCH recomputes the first two; retaining the three small values also covers HLT before original retirement. |
| c_main: cpu_heartbeat, cpu_interrupt_map, SasWrapMask, PigSynchCount | Save scalar counter/pending bits/wrap mask. Host wall-clock absolute values are separate. PigSynchCount's selected SYNCH_TICK is inactive but its scalar need not become a special serialization case. |
| c_main: start_trap | Save through the already-proven HLT continuation, not current TF; FETCH uses zero. |
| c_main: CCPU_BR, CCPU_WR, CCPU_M | Rebuild aliases to this process's CCPU_GR and SAS allocation. Never serialize these addresses. |
| c_main: m_isreg, m_la, m_la2, m_off, m_pa, m_pa2, m_seg, modRM, ops, p, p_start, pg_end, save_id, segment_override | Instruction operand/decode scratch; no active instruction at FETCH or inside the HLT wait. Original FETCH/DO_INST creates the next operands. P4 HLT reentry sets p=p_start=NULL so completion cannot advance IP again; FETCH remaps host pointers. |
| c_main: in_C, took_absolute_toc, took_relative_jump, quick_mode | Execution bookkeeping, not another guest CPU. Rebuild at the existing entry/phase; original FETCH clears branch hints, and entry starts outside quick mode. Selected SYNCH_TIMERS is absent. Preserve the P4 interrupt-shadow/HLT proof when wiring restore. |
| c_main: simulate_level | Rebuild through fresh original simulation entry. The source TLS jump buffers are also rebuilt; they are not heap/serialized state even though optimized object enumeration may omit them. |
| c_main: pf_table, first.2, want_yoda.1 | Fixed parity table and host initialization/debug request state. Keep native initialization; no Yoda request imported. Environment-controlled CPU policy must be checked as compatibility, not copied as host pointers. |
| c_tlb: tlb[4][8], next_set[8] | Save each la, pa, v, d, mode and the eight replacement positions. Even invalid entries can be observed through original test_tlb validity matching; do not silently drop them. |
| c_tlb: page_index[1048576], page_address[128] | Save copied byte/u32 tables. They contain physical addresses, not host addresses. Original test_tlb writes do not build the same fast mapping as lin2phy, so simply regenerating all valid entries is not established as equivalent. Fixed data avoids inventing a cache rebuild protocol. |
| c_xcptn: CCPU_save_EIP, doing_contributory, doing_double_fault, doing_fault, doing_page_fault, EXT | Save scalar exception bookkeeping. No exception handler C stack is active at the admitted outer boundary. Do not assume all these persistent flags are zero merely because execution reached FETCH. |
| c_xcptn: first_exception, show_exceptions, trap_exceptions | Host diagnostics/environment state, not guest payload. Current host diagnostic setup remains in force. |
| fpu: NpxControl, NpxStatus, NpxFCS, NpxFDS, NpxFEA, NpxFIP, NpxFOP, NpxLastOff, NpxLastSel, NpxException | Save exact scalar control/status/last-operation state. NpxException is not inferred from status. |
| fpu: FPtype, NPX_ADDRESS_SIZE_32, NPX_PROT_MODE, POPST, DOUBLEPOP, UNORDERED, REVERSE, DoAPop, tag_or, tag_xor | Retain these bounded decoder/FPU scalar values in the fixed state port rather than infer which last-operation flags happen to be dead. No function/pointer or second owner is required. |
| fpu: FPTemp, IntelSpecial, FPRes | Save their scalar payload/tag representation alongside the stack; do not convert via guest FSAVE, which executes FINIT and can normalize unevaluated values. |
| fpu: FPUStackBase, TOSPtr | Allocate/reuse the original eight-entry stack, save its eight values/tags and TOS as index 0..7, then rebind TOSPtr. FPSTACKENTRY contains an 8-byte FPH plus space used for a 10-byte unevaluated BCD/R80 representation and a tag. Encode the meaningful representation, not host IUH padding/struct size. |
| fpu: npxRounding | Rebuild from saved NpxControl using the existing rounding operation on the executor thread. Host sticky flags require separate capture/restore review below. |
| fpu: NpxDisabled | Configuration compatibility value. Loading must not silently change the configured FPU availability. |
| fpu: BCDHighNibble, BCDLowNibble, FPConstants | Fixed conversion/constant tables, not modified machine state. Keep compiled values. |
| fpu: FpatanTable, first.0 | Rebuild original initialized pointer table/allocation guard. Source FPUpload and constant pointers optimized away in this build remain fixed aliases, not missing payload. |
| compat/memory: softpc_ram, softpc_ram_size | Save allocation size and actual backing bytes through a narrow owner port, then rebind the address. Account for the original extra 0x2000 protection allocation separately from visible size. Physical debug bus reads/writes are not suitable: they dispatch SAS_VIDEO and can change latches/dirty state. |

Source-only and heap cross-checks: c_reg.h defines all nine segment fields and
the system-register fields above; cfpu_def.h defines the variable-width host
FPSTACKENTRY. getNpxStackRegs copies physical slots, setNpxTagwordReg does
nothing, and getNpxStatusReg updates the cached TOS field. Consequently the
existing debugger accessors do not constitute a complete, non-mutating snapshot
port. The current fenv mapping uses ISO C rounding/sticky flags; S4 must capture
the needed flag values before encoder/helper work can change the executor's
floating-point environment, not serialize a native fenv_t blob.

### New evidence changes the restoration plan

The real-core test builds a PDE/PTE mapping to physical 0x3000, resolves it,
changes only the PTE to 0x4000, and resolves again. It still resolves to 0x3000;
only explicit flush_tlb changes it to 0x4000. This passes on x64 and x86.
The proposal's blanket permission to discard TLB was therefore incorrect and
has been narrowed. This is a refinement of exact state fidelity, not a new
feature or shared API. Later S4 must preserve this test's pre-flush behavior
across an actual save/load, including dirty/access modes and replacement order.

Additional discovered state belongs to the remaining S2 ledger: SAS page-type
map and BIOS-virtualization selectors, and environment policy such as
SHIFTROT_OF_UNDEF and SAS out-of-range junk_value. They cannot be waved away as
ordinary RAM or guessed solely from current hardware config. This P does not
claim they are audited/restored; the next inventory slice owns them.

The first x86 direct build invocation omitted the preset's compiler runtime
PATH and failed to preprocess without a C diagnostic. Repeating with the
existing tests-x86 build preset succeeded unchanged; both new focused tests
passed. No toolchain/source workaround was added.

The adjoining zfrsrvd.c also owns NpxIntrNeeded and NpxExceptionEIP: its
DoNpxException delays delivery until a following NPX instruction, so those two
fields must travel with FPU state, not be cleared as host notifications.
Ax_regptr/NpxInstr are copied scalar decode context to classify with that
owner. c_debug's d_brk/i_brk and counts implement guest DR breakpoints, whereas
c_xtrn's interface_abort is a native exception-return stack. Neither is the
product debugger's plan. Their complete disposition is retained for the next
CPU-adjacent slice, not hidden by the five-object count above.

After: production C/H +0/-0, original mirror +0/-0; checkpoint test +25/-0;
CMake +1/-0. Both full builds succeeded and both full suites passed 103/103
(x64 58.78 s; x86 57.97 s). All 88 symbol names have ledger entries; that
name check is only completeness for the declared slice, not serialization
proof. Documentation gate and diff whitespace checks passed. Both package
hashes remain exactly P5: no production or firmware change to package.
Owned 512-byte checkpoint fixtures were removed by both successful tests.

### S2 P7: queued-event restore design audit (documentation only)

Before: continue the admitted S2 state inventory, not S5 serialization. Inspect
quick_ev.c and all source callers of add_q_event_i/t, add_tic_event and
add_q_ev_int_action. Estimated production/test churn zero; update the existing
proposal and this ledger only. No Lib/Common/guest-media changes.

Q_EVENT contains callback, time_from_last, original_time, handle, param,
event_type and three native links. Save the first six semantic fields with a
stable callback identifier and list order; rebuild links/hash/free allocation.
Save next_free_handle, tic_next_free_handle, tic_event_count and the actual CPU
quick counter separately. add_event copies the current counter into the old
head only on insertion: the stored head delay alone is not current remaining
time. Equal deadlines insert after existing nodes; this order must survive.

Do not reconstruct through add_event: zero delay calls the callback immediately,
and nonzero insertion allocates a new handle. Device fields such as
refillDelayedHandle, rtc_periodic_event_handle and printer out/outa handles
would then refer to different events. Preserve their numeric handles together
with the queue, and perform no dispatch while restoring. The original file's
dispatch path removes a node before invoking its callback; a snapshot must not
be taken from inside that callback just because the node is absent.

The raw source call sweep found keyboard do_int/allowRefill; CMOS periodic,
alarm and host-time synchronization callbacks; timer delay/multiple IRQ;
floppy and fixed-disk delay; printer out/outa; serial next_batch/recv_char/
do_wait_on_send. These are candidates, not a claimed selected-build whitelist:
conditional compilation and callback parameter meanings still require the
device-by-device S2 ledger. quick_ev.c also owns select_int_action plus an
associated action queue; no external source caller of add_q_ev_int_action was
found. Its empty/unused disposition must be proved before excluding it.

Calibration fields and host timestamps are not yet classified by this slice.
In particular, restarting calibration must not silently alter an already
pending event deadline. This is outstanding S2 work, not an implicit choice to
reset all scheduler state. S2 remains active and save/load are not exposed.

After: production/test/build files unchanged; only the two existing task
documents changed. Source review and documentation/whitespace gates passed.
No rebuild claimed for this documentation-only refinement; fixed EXEs and the
P6 dual-width 103/103 test baseline remain unchanged.

### S2 P8: prove quick/tick queue semantics and classify scheduler state

Before: continue S2, production C/H and mirror changes zero; add an estimated
60-90 lines to the existing checkpoint smoke, no new test target or API.
Use original queue operations and a stopped host timer, not a fake scheduler.
The owner's follow-up asks whether state operations participate in lifecycle
and whether all event queues must be drained. The proposal clarifies existing
ownership: Common owns operation admission and final PAUSED notification;
external input is bounded at capture, but internal device events are preserved,
not forced to execute. Ordinary pause does not replace the safe-point request.

Evidence: selected x64 preprocessing uses the actual devices target defines
(CCPU, CPU_40_STYLE, CPU_486, C_VID, PROD, SPC386, V7VGA, WDCTRL_BOP,
SOFTPC_STANDALONE and pointer width), include response and keyba's own quote
include. The initial diagnostic incorrectly applied that quote include to all
files and flooded macro warnings; it was discarded and repeated with the
per-file option only. No generated preprocessed file became a build input.
The source sweep includes whitespace before the call's opening parenthesis;
this catches fdisk's disk_int_call_back, missed by the P7 literal-call search.

| Original callback owner | Selected queued callbacks and parameter meaning |
| --- | --- |
| keyba.c | do_int: copied scancode; allowRefill: zero; preserve refillDelayedHandle |
| cmos.c | rtc_periodic_event, rtc_alarm, sync_rtc_to_host_time: zero; preserve periodic handle and alarm handle |
| timer.c | timer_multiple_ints: remaining interrupt count; timer_no_longer_too_soon: zero, present in selected source although its hack_active path stays false with CPU_40_STYLE |
| fla.c | fla_int_call_back: zero |
| fdisk.c | fdisk_pause, disk_int_call_back: zero |
| printer.c | lpr_state_out_event/outa_event: adapter index; preserve both handle arrays |
| com.c | recv_char/do_wait_on_send: adapter index; next_batch is absent in selected preprocessing |

These fourteen callback names form the selected source's direct device call
set, not an instruction to persist native function pointers. The archive's
undefined-symbol sweep identifies these same seven caller objects (the
optimized timer object needs only add_q_event_t). select_int_action is defined
inside quick_ev.c but has no production add_q_ev_int_action caller; its action
queue remains initial/empty. A future unexpected callback is a capture failure,
not something silently omitted. Product debug callbacks are not q/tic events.

Scheduler mutable-symbol ledger (25 named B/b/D/d symbols, excluding compiler
dot sections; counts are a bounded object audit, not a full-device claim):

| Fields | Snapshot disposition |
| --- | --- |
| next_free_handle, tic_next_free_handle, tic_event_count | Save values, alongside actual CPU quick counter and ordered active node records |
| q_list_head, q_list_tail, tic_list_head, tic_list_tail, q_ev_hash_table, tic_ev_hash_table | Rebuild links with new addresses; retain list order and hash-chain order using bounded node indices |
| q_free_list_head, tic_free_list_head | Empty/rebuild allocator resources; no queued work lives in free nodes |
| DisableQuickTickRecal, q_ratio_initialised | Reestablish initialized CCPU invariant; init_q_ratio forces disable true and no production assignment reenables it |
| usecPerIJC, jumpRestart | Remain initial unsigned -1 in this selected path; calc uses time/10 and count*10, not host calibration |
| ideal_q_rate, real_q_rate | Initialized/equal values, not used by the crude conversion while usecPerIJC is -1; quick_tick_recalibrate takes its disabled early return |
| firstfew.4, ijc_calib.1, ijc_recount.2, ijc_tstamp.3, previous_tstamp | Unused calibration branch/host sample; no active pending deadline derives from them in selected CCPU |
| firstcall.0, int_act_qhead, int_act_qident | Initial unused action-queue state: no production registration caller |

q_ratio_history/head source-only averaging state is also unused (AVERAGED is
not selected), and optimized out of the named data-symbol set. No new saved
host timestamp protocol is needed for this scheduler. RTC wall-time sampling
is a separate owner: sync_rtc_to_host_time reschedules itself every 200 ticks
and calls Compat host_time/host_localtime. Saving CMOS bytes alone will not
isolate that later host sample; the S5 time-base implementation must preserve
the approved restored-clock semantics, not treat calibration exclusion as
permission to lose RTC state. Its host_tm pointee also needs copied fields.

The original allocator wraps handles at 0xffff without checking active
collisions. Hash-chain order therefore cannot be inferred from deadline order
for every possible live queue. Retain original hash adjacency as node indices
in S5, rather than fixing original allocation semantics or inventing a new
handle namespace. This refines P7's "rebuild hash" into a lossless rule.

The real-queue test proves: initialized CCPU conversion; equal quick/tick
deadline order; inserting while the CPU counter is partially consumed;
cancelling a later quick/tick event by its original handle; zero-delay callbacks
execute immediately and return handle zero. Host timer is joined for this
bounded queue test and restarted before disposal; no Sleep and no guest media
changes. This is scheduler contract proof, not a serialization roundtrip.

After: production C/H +0/-0; original mirror +0/-0; one product test C file
+68/-0. No build/shared-corpus edits. Full x64/x86 builds succeeded and full
suites passed 103/103 (59.58 s / 59.74 s). Both widths have the same 25 named
scheduler data symbols and every name has a ledger entry. Documentation and
whitespace checks pass; temporary checkpoint media was removed by both tests.
Package hashes remain P5 unchanged. S2 remains active: device-local state and
remaining CPU-adjacent/video state still need complete inventory, then the
admitted payload stages must implement and prove actual cross-process restore.

## S2 P9: CPU side-state proof

Before: one product test, approximately 35-45 added lines; no production,
original mirror, Lib or Common changes. This bounded proof extends S4's state
requirements; it does not close S2's remaining device/video inventory.

The defect class under audit is treating derived CPU data as safely
reconstructible. The real CCPU test now demonstrates both cases:

- `c_debug.c`: `nr_inst_break`, `nr_data_break`, `i_brk` and `d_brk` are saved
  semantic data (addresses, identifiers, ranges and types), not native pointers.
  `c_tsksw.c` clears local DR7 enables without `setup_breakpoints`; the only
  production calls rebuilding the table are in `mov.c`. The test uses that
  same DR7 clear primitive and demonstrates that a cached breakpoint still
  sets DR6; rebuilding the table removes it. This is not a full TSS-switch
  test or a CPU bug fix. Load must preserve the actual table, not repair it.
- `zfrsrvd.c`: `NpxIntrNeeded` and private `NpxExceptionEIP` must be saved.
  `DoNpxException` copies FIP; later `TakeNpxExceptionInt` restores that older
  IP even when current FIP differs. The test disables IF to inspect deferred
  delivery without entering an ISR; the following test resets the machine.
  FPU register export alone cannot preserve this pending exception.

Similar-issue sweep: searched all selected mirror sources for
`setup_breakpoints`, inspected the task-switch clear and both NPX functions,
and retained the earlier real TLB proof. No snapshot encoder or register
setter is substituted for the original implementation in these tests.

After: one test C file +35/-0; production and original mirror +0/-0.
Both full builds succeeded, with x64/x86 full suites 103/103 (58.76/60.74 s).
Final review aligned the test mask with original LOCAL_BRK_ENABLE (0x155,
including LE); both rebuilt focused suites then passed 2/2. Both fixed package
hashes remain P5 unchanged. No temporary checkpoint image remains; no Lib,
Common, configuration or media change. Proposal carries the two additional
state obligations. S2 remains active; this is not snapshot roundtrip proof.

## S2 P10: SAS and CPU-adjacent ownership audit

Before: production/test churn zero. This is the next finite CPU-adjacent
inventory slice after P9, covering the selected CCPU SAS, lazy compatibility
values, external-interface unwinds and per-thread simulation backing. It does
not claim any device/video payload implementation.

| Owner / exact state | Snapshot disposition |
| --- | --- |
| `c_bsic.c`: `cold`, `shiftrot_of_undef` | Save the two lazy-policy fields, or reject a container whose recorded `SHIFTROT_OF_UNDEF` policy differs. The value changes guest OF behavior; do not silently reevaluate a changed host environment after restore. |
| `c_debug.c`: counts plus `i_brk` / `d_brk` | Save semantic entries; P9 proves these cannot be rebuilt from DR7. |
| `zfrsrvd.c`: `NpxIntrNeeded`, `NpxExceptionEIP` | Save with FPU state; P9 proves deferred delivery needs the copied older FIP. `Ax_regptr` and `NpxInstr` are per-instruction temporaries and do not survive the admitted boundary. |
| `c_xtrn.c`: `interface_abort`, `interface_active`, `interface_error` | Rebuild inactive. They are a native `setjmp`/`longjmp` scope used only by six synchronous segment setters. The admitted FETCH/HLT boundary is outside that call; no native continuation can enter a file. |
| `ccpusas4.c`: RAM allocation/length, `memory_type`, `SasWrapMask`, selectors and `selectors_set` | Save visible RAM, allocation size and the complete page-type map, wrap mask and virtual-selector scalars. Recreate the original guard allocation but do not encode its unobservable padding bytes; rebind RAM aliases on restore. Do not call `c_SasRegisterVirtualSelectors`: it writes LDT bytes and would mutate captured RAM; its small local state port installs the saved values directly. |
| `ccpusas4.c`: `Sas`, `cSasPtrs`, read/write/fill/move tables, `temp_func`, `Start_of_M_area` | Rebuild native pointers/vectors from the selected implementation and restored video binding; never serialize function or process addresses. The video owner must re-install the semantic memory handler before execution resumes. |
| `ccpusas4.c`: `scratch`, `currentLength`, Video scratch pointer | Rebuild a fresh 64 KiB scratch allocation and reconnect Video. All selected uses are temporary ROM/font/string transfer storage; contents are not guest state. A request above the initialized size is already an original fatal “VGA will be broken” path, not a normal persistent buffer. |
| `ccpusas4.c`: `phyR` lazy `first` / `junk_value`; `biosDoInst` lazy `first` / `bodgeAdjustment` | Preserve parsed out-of-range-read policy (or reject a differing compatibility profile). `bodgeAdjustment` is deterministically recomputed only after validating identical ROM bytes; tracing is host diagnostics and remains host-local. `biosDoInst`'s C locals are excluded because nested simulation must naturally return before capture. |
| `ntthread.c`: TLS id, jmp buffers, thread list and level | Recreate for the new executor thread. P4 proves resumption starts with a fresh original simulation entry; host thread/TLS addresses and jmp buffers are not machine payload. |
| `ntstubs.c`: CPU/Video/SAS function vectors and diagnostics | Reinstall normal selected bootstrap vectors. No selected caller establishes an independent persistent payload here; do not serialize addresses or diagnostic pointers. |

`gmi_define_mem` has five selected video callers (`cga.c`, `ega_trcr.c`,
`ega_writ.c`). Its tables are not a second video-state owner; S6 must restore
the video owner's selected handler exactly once after all scalar/video state
is installed. This avoids saving function addresses while preserving the
subsequent SAS dispatch.

After: documentation only, +0/-0 production/test/original-mirror lines.
The caller sweep used `rg` over selected C/H sources for SAS scratch,
`gmi_define_mem`, `c_SasRegisterVirtualSelectors`, and the native-interface
symbols. It found no other selected caller of the six `call_cpu_function`
segment-setter paths. The unresolved receiver is explicitly S4/S6, not an
implicit reset/reinitialization during load. S2 remains active.

## S2 P11: controller state inventory

Before: documentation-only selected-object inventory. No code, ABI, build,
Lib, Common, configuration or media change. The frozen source universe is the
actual `softpc-machine-devices` target's selected original/Compat C files and
the x64 archive's B/b/D/d symbols (the x86 names must match before S2 closes).

| Owner / selected mutable state | Receiver and disposition |
| --- | --- |
| `ica.c`: two `ADAPTER_STATE` records, `sequence`, `iretHookActive`, `howoften` | S5 saves every semantic PIC register/count/delay/priority/ISR-aging value. Each action callback becomes a fixed callback ID plus parameter; `ica_*_func` and the static async-handler function table rebuild. This preserves pending/in-service IRQ rather than reissuing ports. |
| `timer.c`: three `COUNTER_UNIT` records, phase/local counters, backlog and timer scalars | S5 saves counter registers and semantic state-machine phase IDs, never host-time/getTime or state-function pointers. It rebases host-time baselines at restore without manufacturing guest ticks. `active_int_event` is reconciled against the saved q/tic nodes, not re-added. |
| `cmos.c`: CMOS bytes/index/register selection, alarm/period fields and RTC queue handle | S5 saves all guest-visible registers and scheduling fields. BCD/hour conversion pointers and `host_tm *` rebuild. Its periodic host-time sync is a separate virtual-clock issue: loading CMOS bytes alone is insufficient, so S5 must define restored RTC time-base behavior and reconnect exactly the saved queue handle. |
| `at_dma.c`: full `DMA_ADAPT` and `lim_active` | S5 saves register, address/count/page/mask/flip-flop state; it does not replay DMA port writes. No native pointers occur in the adaptor record. |
| `fdisk.c`, `fla.c`, `floppy.c`, `gfi*.c`, Compat GFI/media | S5 saves command/result/taskfile/status/sector buffers, selected drive, DMA/NDMA transfer progress and pending queue handles. Function tables, image leases and host paths are rebuilt/validated through the separately saved media section. An incomplete disk command must remain incomplete. |
| `ppi.c`, `keyba.c`, `keybd_io.c`, `mouse.c`, `mouse_io.c` | S6 owns these input-controller registers/FIFOs/typematic/keyboard state, mouse deltas/buttons/mode and pending action handles. Host input callback pointers and UI source state are rebuilt and never stored. |
| `audio.c`, `nt_sound.c`, `parallel.c`, `serial.c`, `printer*.c` | Guest-programmed sound/port state is classified with S5; native task/event/handle/audio resources rebuild. Per product contract, attached external serial/parallel/printer endpoints reject save rather than claim rollback of external effects. |

The `COUNTER_UNIT` and `ADAPTER_STATE` fields contain native function pointers;
the future format therefore needs explicit fixed phase/callback identifiers,
not structure-byte dumps. The quick/tic P7/P8 ledger remains their only queue
owner. This table adds no competing scheduler or controller state source.

After: documentation only, production/test/original-mirror +0/-0. The sweep
enumerated actual selected-device target sources and archive mutable symbols;
the next proof must repeat the symbol set on x86 and turn each table row into
narrow owner ports and fault-injected roundtrip tests. S2 remains open for
video and remaining selected devices.

## S2 P12: video and input-state receiver audit

Before: documentation-only inspection of the selected video objects,
`compat/cvidc/gdp_state.c`, and P11's keyboard/mouse object inventory. No
production/test/ABI change.

| State family | Receiver and strict rule |
| --- | --- |
| VGA/CGA/EGA registers and planes | S6 saves EGA/CGA controller registers, DAC/palette/index/latches, bank/chain/mode scalar state, text/font and plane bytes, cursor/scroll/split state, plus exact C-VID scalar state needed by future writes. It does not infer these by replaying port writes. |
| `EGA_CPU`, `EGA_GRAPH`, `CGA_GLOBALS`, C-VID GDP slots | Encode semantic scalar fields and RAM-relative offsets only. `plane_offset`, mapped-plane addresses, screen/regen pointers and all generated function pointers are rebuilt against the newly allocated plane/RAM/GDP storage. No host pointer or `struct` byte dump is legal. |
| C-VID GDP storage | `compat/cvidc/gdp_state.c` deliberately mixes slot widths and contains direct host pointers. S6 requires a fixed audited field list: per-slot scalar/offset/semantic-ID encoding, with unknown slot use rejecting capture. It must not serialize the slot table wholesale merely because it is separately allocated. |
| Dirty and presentation resources | DIBs, `PCDisplay`, native paint functions, host drawing buffers, dirty queues, and frame-generation resources rebuild. Restore marks a complete redraw rather than depending on saved dirty rects; Window/KVM receives the existing complete-frame publication after Common's normal paused fact. |
| Handler tables and geometry helpers | EGA/CGA/VGA read/write/mode/copy/mark function tables and host drawing LUTs rebuild from restored semantic mode/register state. `gmi_define_mem` then installs the corresponding SAS handler once; neither C-VID nor video may retain an old process address. |
| Keyboard/mouse | P11's complete controller/FIFO/delta/button/typematic state is the guest payload. KVM/host input sources, logical Console state, mouse capture, source identity, host cursor and UI mailboxes remain excluded; they re-enter by the existing post-load UI route. |

The source sweep found two independent plane/RAM representations: guest plane
bytes and controller semantics must both travel, while DIB/Window pixels must
not. It also found `gdp_state`'s slot allocation is an ABI adaptation, not a
snapshot format; a generic memory image would include pointers and make
cross-process restore invalid. The exact fixed GDP field map and video-port
tests are S6 implementation work; no partial generic encoder is admitted.

After: documentation only, +0/-0 production/test/original-mirror lines. This
completes the selected video/input *receiver classification* but not S2's full
device ledger or S6 payload proof. S2 remains active.

## S2 P13: remaining selected host/BIOS/media inventory

Before: documentation-only audit of all remaining B/b/D/d records in the
selected device archive, plus their ownership source. No production/test/ABI
change.

| Owner / state | Snapshot disposition |
| --- | --- |
| `reset.c`: `soft_reset`, reset buffer and working function vectors | Only stable reset-in-progress data can reach capture; normal save admission rejects control transitions. Save a genuinely live semantic reset field if one exists at the safe boundary; rebuild all function vectors. Do not serialize reset's temporary buffer or make an implicit reset part of load. |
| `diskbios.c`, `gfi.c`, Compat `gfi_image.c`, `hdd_media.c` | Save guest disk BIOS/controller semantic state under S5 and validate/recreate media leases/paths from S3's media section. GFI and IOS function tables are host bindings and rebuild; no descriptor, `FILE*`, lease pointer or path buffer enters the state payload. |
| `com.c`, `printer*.c`, `rs232_io.c`, Compat serial/parallel | Save only any guest UART/LPT register state explicitly covered by S5. A configured live external endpoint rejects capture; native COM/LPT handles, retry tasks and callbacks are excluded. This preserves the approved no-external-world-rollback contract. |
| `ios.c`, `nt_keycd.c` | Rebuild I/O dispatch tables and static scan-code mappings from normal selected bootstrap. They are function/table wiring, not device registers. |
| `time_day.c`, `cmos.c`, `timer.c` | Save BIOS timer-vector/timestamp and RTC/CMOS semantics; rebase host time only through a single documented restored virtual time base. Current code samples host wall time, so S5 cannot call the ordinary initialization path after RAM restore without changing guest time. |
| `idetect.c` | Rebuild idle/poll counters as host responsiveness policy. They neither represent a guest peripheral nor a scheduled guest event; the original machine resumes from the saved device/CPU state. |
| `rom.c`, BIOS bootstrap/equipment helpers | Rebuild ROM loader cursors and fixed ROM metadata after container ROM fingerprint validation. ROM bytes/guest RAM state are already separately represented; no resource-reading continuation may remain at capture. |
| Compat `dib_surface`, `graphics_console_compat`, `video`, `v7_pointer`, original `nt_graph`/`nt_sound` | Rebuild host pixels, dirty/cursor background, painting/audio state and task handles. Guest video and PPI/timer state are saved elsewhere; complete-frame publication gives presentation one clean reentry point. |
| Compat `platform.c` | P2/P5 owns timer producer, safe-boundary deadline and copied CPU entry. Executor callback/event, native timer handle, errors, tracing, pacing origin and product strings remain host-local/rebound. No snapshot work adds a second executor. |

The only selected caller family that may retain a host-visible irreversible
effect is serial/parallel/printer; it is therefore a deliberate capture
rejection, not missing serialization. The current original `time_day.c` and
`cmos.c` both consume host time, so their restored time-base needs one S5
design/roundtrip proof; this ledger explicitly prevents an accidental
post-load host-clock jump.

After: documentation only, production/test/original-mirror +0/-0. This sweep
now classifies every selected archive object family by receiver, but S2 remains
open: the frozen named-symbol table has not yet been written as a single
per-object convergence ledger, and no S4-S6 serializer exists.

## S2 P14: selected archive convergence index

Before: P6--P13 classified every selected *family*, but left the final
cross-check distributed across those records.  This documentation-only pass
freezes the actual x64 archive object universe as a finite index.  It neither
adds a serializer nor treats every writable ELF/COFF section as guest state.

The reproducible input is `libsoftpc-machine.a`, generated by the existing
`tests-x64` preset.  For every `*.c.obj`, enumerate only named `B/b/D/d`
symbols, strip the x86 leading external underscore for the width comparison,
and ignore the compiler's anonymous `.bss` and `.data` section symbols.  An
instruction translation unit with only those anonymous section symbols has no
separately addressable mutable state to encode; its instruction implementation
is rebuilt by loading the selected program image.

| Frozen object set | Count / disposition | Receiver |
| --- | --- | --- |
| CPU core: `c_main`, `c_tlb`, `c_xcptn`, `fpu`, `memory`, `c_bsic`, `c_debug`, `c_xtrn`, `ccpusas4`, `zfrsrvd` | 10 owner units.  Registers, hidden descriptors, TLB, FPU/deferred-NPX, SAS page map/RAM and semantic breakpoint data are payload; decode/JMP/TLS/function aliases are rebuilt. | S4; exact P6/P9/P10 tables. |
| CPU instruction leaves, including `aaa` through `zfrsrvd` except the named core owners above, plus `j_c_lang` | No named guest-state object in the leaf units.  `j_c_lang`'s `r0..r31`/`rnull` are generated rule-entry addresses. | Rebuild selected instruction/rule tables; never encode addresses. |
| Scheduler: `quick_ev`, `timer`, `ev_glue`, `timestrb`, `time_day` | Active queue records, counters, IDs and guest-visible timer/RTC state are payload; queue links, callback/function pointers, calibration samples and host timestamps rebuild/rebase. | S5; P7/P8 and P11/P13. |
| Interrupt, DMA, storage controllers: `ica`, `at_dma`, `cmos`, `fdisk`, `fla`, `floppy`, `gfi`, `diskbios`, `com`, `printer`, `rs232_io`, `serial`, `parallel`, `ppi`, `audio` | Guest register/FIFO/transfer state and saved queue handles are payload.  Native endpoint/task handles, function tables and external-world effects rebuild or make capture fail. | S5; P11/P13. |
| Keyboard and mouse controllers: `keyba`, `keybd_io`, `mouse`, `mouse_io` | Controller/FIFO/typematic/delta/button/queued semantic state is payload; host sources/callbacks are excluded. | S6; P11/P12. |
| Video semantic owners: `cga`, `ega_mode`, `ega_prts`, `ega_read`, `ega_vide`, `ega_writ`, `egawrtm0`, `egwrtm12`, `vga_mode`, `vga_prts`, `vga_vide`, `v7_ports`, `v7_video`, `v7_pointer`, `gvi`, `gfx_updt`, `video` | Guest registers, planes, latches, palette, cursor/split/bank and RAM-relative semantic state are payload.  DIBs, dirty regions, host pixels and all generated draw/read/write vectors rebuild. | S6; P12/P13. |
| C-VID rule/vector owners: `ev_glue`, `evidfunc`, `sevid000..sevid030`, `sinit011..sinit013`, `sascdef`, `gdp_state` | C-VID scalar slots/offsets are S6 payload only after the fixed field map is audited.  `j_*`, `Gdp`, `cSasPtrs`, function tables and rule entry addresses are process bindings.  Any unknown slot use rejects capture. | S6; P10/P12. |
| Original/Compat host presentation: `nt_cga`, `nt_ega`, `nt_graph`, `nt_keycd`, `nt_sound`, `dib_surface`, `graphics_console_compat`, `keyboard`, `mouse_instance`, `platform`, `v7_pointer`, `hdd_media`, `gfi_image` | Guest-visible state belongs to the controller/video/media owners above.  Native windows, painting buffers, cursor backing, key maps, timer/thread handles, media leases and audio resources rebuild after a full-frame publication. | S3/S5/S6; P11--P13. |
| Bootstrap/configuration: `reset`, `rom`, `ios`, `ntstubs`, `ntthread`, `stubs`, `build_id`, `idetect` | ROM/config/build identity is container compatibility input; immutable tables and vector wiring rebuild.  Native scopes/TLS, loader cursors, diagnostic and idle-policy data are not payload.  A live semantic reset transition cannot pass the safe-boundary admission. | S3/S4/S5; P10/P13. |

The list is deliberately an index, not a generic structure copier.  It closes
the former ambiguity between (a) an object that is a receiver for an existing
S4--S6 payload, (b) a process-local binding to rebuild, and (c) a host/external
condition that rejects capture.  There is no fourth, silently omitted class.
The first cross-width comparison found compiler-shape differences, not a new
state owner: x64 names four lifecycle TLS variables under `emutls_v.*`, while
x86 emits the same four names directly plus a `.tls$` section marker.  x86
also emits `LocalIUH`/`LocalFPH` in the generated C-VID rule units.  Those
variables are rule-call local stacks saved only to support nested rule calls;
the admitted safe boundary is outside every C-VID invocation, so they rebuild
empty with the fresh call stack.  Finally, x86 retains `com:LCRFlushMask`:
it is a deterministic lazy cache initialized from the fixed line-control bit
definition, so it rebuilds from normal COM initialization.  These are explicit
rebuild items, not width-dependent payload.  The normalized receiver set is
therefore the same on both widths; implementation must test this rule instead
of assuming raw symbol spellings are byte-identical.

After: documentation only, production/test/original-mirror +0/-0.  The x64
archive scan reports 110 objects with 1,934 named B/b/D/d symbols after
anonymous section symbols are excluded.  The x86 scan reports 2,004 names;
the count delta is the 68 generated-rule local names, one deterministic COM
cache and one TLS section marker; the four lifecycle names have alternate
compiler spellings on the two widths.  Every remaining
symbol belongs to one receiver above or to the instruction-leaf rebuild set.
No production build, package or media changes are claimed.  S2 remains active
for the already-admitted safe-boundary completion review and the eventual
payload round-trip work.

## S2 P15: boundary and ledger closure review

The S2 exit review is deliberately narrow: it proves the state **cutoff** that
later serializers consume; it does not claim that a snapshot file, a public
Common operation or a restored machine exists.

`softpc_snapshot_begin` records one monotonic deadline.  The original CCPU
observer sees every selected FETCH and HLT boundary.  A nested simulation only
continues waiting; a timely outer FETCH/HLT boundary first joins the timer
producer, copies the phase required for fresh-stack reentry, and reaches
READY.  It neither serializes a C stack nor calls a second CPU loop.  The
timeout path does not force an unwind or emit a payload.  The later S7
transaction, rather than this internal seam, owns the user-visible ordinary
PAUSED completion and must retain the joined producer while encoding.

The real `checkpoint_smoke` covers natural nested-BOP return, HLT reentry,
interrupt shadow and preserved quick-event counter; its timeout half proves
that a permanently nested HLT fails through the monotonic deadline without a
successful capture.  `snapshot_boundary_smoke` independently proves deadline
edges, one completion, clock query/frequency failure and timer stop/start
failure.  P6--P14 provide the finite state-owner/index evidence needed so a
later S4--S6 implementation cannot silently substitute stack, function
address, native timer or host resource bytes for a semantic receiver.

Final verification rebuilt both package test trees and passed the complete
suite: x64 103/103 and x86 103/103.  The x86 `LastTestsFailed.log` retains an
older machine-smoke entry despite the current run's 103 successful records;
it is build-local stale CTest metadata, not a current failure.  Both owned
checkpoint fixture files are absent.  Documentation governance and whitespace
checks pass.  This closure commit changes only task documents: production
C/H +0/-0, tests +0/-0, original mirror +0/-0, Lib/Common +0/-0.

P15 supplies S2's closure evidence.  The following committed admission may
activate S3's independent pre-audit of the fixed binary container and
media-base transaction; it must not expose a partial command or weaken the
running-only capture contract.

## S3 P2: container and media transaction pre-audit

No implementation is admitted by this record.  The audit fixes the required
ownership and identifies one blocking shared capability before a partial file
format can escape.

The existing binary writer and `lib_storage_medium_read_at` are useful: Compat
owns the FDD/HDD leases and can scan an overlay's effective pages after S2 has
stopped new guest media writes.  Direct/readonly media can therefore record
only path, mode, size and a VM-local SHA-256 fingerprint.  Overlay media can
open a separate readonly base lease at the same path, compare every 4-KiB
effective page, and write only unequal page indexes plus bytes.  This requires
no Lib page-enumeration API and does not create a second runtime overlay.

Loading still validates all bases before changing the init/stopped machine;
Compat will later prepare and owner-commit replacement leases.  Existing live
serial/parallel/printer rejection remains the correct external-world rule.

The blocker is public Storage file I/O.  It supplies only whole-file
`read_owned` and a writer that truncates its destination.  It has no streaming
reader and no same-directory temporary/atomic replacement publication.  Thus
a large RAM-plus-overlay image would require one unbounded allocation, and a
disk-full or write failure can destroy the prior snapshot.  App, VM and Compat
must not bypass that boundary with Win32 calls or a private file layer.

S3 therefore stops before container code until the owner decides whether to
admit the proposal's minimal neutral Storage reader and atomic-replace writer.
No production, test, Lib, Common, original-mirror, package, configuration or
media file changed in this audit.

## S5 P1: private controller and queue archive

S5 adds one private `compat/devices` archive owner beneath the existing CCPU
archive.  It stores fixed-width semantic state only: DMA registers/pages,
PIC pending/in-service state, PIT counter phase and elapsed age, CMOS/RTC
register state, FDC command/result/NDMA state, HDD taskfile/sector state, and
the q/tic queue records required to continue those controllers.  It does not
expose an App command, Common API, file format, media export, or a public
pointer to the archive.

Queue callbacks are mapped by a small, audited semantic ID table.  Timer,
RTC, FDC and HDD callbacks restore by ID; an unrecognised queued callback
causes capture to fail rather than serializing an address or silently omitting
work.  Queue restoration constructs both replacement lists first.  Only after
both validate does it discard the live lists and install the replacements;
therefore an invalid late callback cannot destroy the current queue.  Restore
never calls normal enqueue APIs, so a zero-delay record cannot execute while
being restored.

PIT state uses state/action IDs and elapsed phase, not function pointers or
absolute host timestamps.  Restore rebinds the original state-machine
functions and rebases the phase on the current host clock.  CMOS similarly
copies `host_tm` values rather than its pointer and rebuilds conversion
functions.  PIC rejects an active unrepresentable callback instead of storing
it.  New `compat/devices` is a Compat taxonomy owner for these archive-only
adapters; original controller ownership and normal port behavior remain in
the mirror.

Focused checkpoint proofs now cover: failed capture on an unknown callback;
queue ordering, cancellation, handles, zero-delay non-execution and failed
restore retaining the old queue; CMOS state through the composed archive; and
a non-default PIT mode/count/state/action round-trip.  This is still not a
user snapshot: S6 owns the remaining video/input and selected device payload;
S7 later owns the Common state operations and final recovery transaction.

## S5 P2--P3: unrepresentable continuations and controller proof

HDD capture now has the same representability contract as the other archive
receivers.  A selected-drive pointer outside the two semantic drive slots, or
an active command without a fixed command ID, rejects capture before the
archive becomes valid.  It cannot report a successful private capture that a
later restore must reject.

The checkpoint smoke also drives a pending PIC IRQ/mask, DMA channel 2/page
state, FDC `SPECIFY`, and HDD taskfile registers through their existing port
paths.  For each it
captures, deliberately changes live state, restores, then recaptures and
compares the affected semantic fields.  This proves the selected S5 state
hooks reinstall live controller state; it does not expose a snapshot command
or claim coverage of S6 keyboard/mouse/video state or external endpoints.

## S5 closure review

The independent review covered `9f7bcbf..c3cba33`.  The private payload has no
raw controller struct, host pointer, function address, native handle, stack
or `jmp_buf`: q/tic callbacks cross the boundary only through the eight fixed
semantic IDs.  Restore builds both queue replacements before replacing either
live queue, so a late invalid callback leaves the live queues intact; it never
uses an ordinary enqueue API, including for zero-delay entries.  PIC active
callbacks and HDD continuations outside their reviewed identities reject
capture rather than being silently lost.

The selected S5 receiver set is PIC, PIT/RTC, DMA, FDC, HDD and q/tic.  The
callback sweep also found keyboard, communications and printer queue producers;
they have no accepted ID in this slice and therefore make private capture fail.
Their guest payload or external-endpoint policy remains the explicit S6
receiver work, not a second archive path.  No App command, file container,
Common API or Lib code was introduced.

Actual counted production C/H changes are `+1,558/-1` (net `+1,557`): Compat
private archive `+340/-1`, preserved MVDM port-ABI hooks `+1,218/-0`.
Focused checkpoint and boundary tests pass on x64/x86; final sequential full
CTest passes `103/103` on both widths.  The S5 package code is in
`assets/binary/softpc32.exe` and `assets/binary/softpc64.exe` from P2; P3/P4
only changed test and historical evidence.  The worktree was clean after P4.
Lib and Common remain unchanged by this P.

## S6 P2: PPI latch and speaker reconstruction

PPI is the first implemented S6 receiver. Its fixed-width archive contains
only the guest port latch, the Timer 2 gate edge baseline, and the speaker-data
edge baseline. It deliberately does not save the PIT state a second time,
replay `timer_gate()`, or copy audio task/event/buffer/native-handle state.
PIT remains the S5 receiver. On restore PPI reinstalls the two edge baselines
and asks the existing host sound implementation to enable or disable speaker
output, rebuilding that host resource from restored guest state.

`checkpoint_smoke` drives PPI port `61h`, captures it, changes it, restores it,
and recaptures the three semantic values. This is a private archive proof only:
there is no snapshot command, container, App path, Common API, or Lib change.
The next S6 receivers remain keyboard, mouse and video.

## S6 P3: InPort hardware mouse archive

P3 admits only the finite Microsoft InPort hardware receiver in `mouse.c`.
The fixed-width private state contains the unconsumed relative deltas and
button state, latched data/status registers, last-button edge baselines,
mode/address selection, finite startup-interrupt count, alternating ID state
and diagnostic-transfer state. Restore assigns these values directly. It does
not replay port writes, send an IRQ, retain an input source, serialize a
callback or preserve any host cursor resource.

`checkpoint_smoke` drives the original InPort through its normal hold and
diagnostic protocol, captures it, changes/reset the live hardware state,
restores it and compares every archived field. It then consumes the restored
one-shot diagnostic byte. This proves both latent movement/register state and
the diagnostic continuation survive without an I/O side effect.

The DOS INT 33h driver in `mouse_io.c` remains explicitly deferred: its guest
callback, cursor backing and `MOUSE_CONTEXT` need a separate fixed-width map;
P3 does not claim a complete mouse-driver snapshot. No App command, snapshot
container, Common API or Lib code is added.

## S6 P4: 8042 keyboard-controller archive

P4 archives the finite 8042 controller rather than host keyboard ownership.
Its fixed-width state records the logical unread FIFO order, scan-set and
translation state, typematic/key-down and held-key state, controller command
latches/status/output, and the finite anomalous key sequence by an index into
the scan-set-derived break table.  Restore rebuilds those tables from the
saved scan set and deliberately clears transient host-call scratch pointers.

The two reviewed delayed keyboard callbacks are now semantic queue IDs:
`do_int(scancode)` preserves its copied scancode parameter through the generic
queue archive, while `allowRefill()` also retains its cancellation handle in
the controller state.  No callback address, host input source, native reader,
or host keyboard focus is serialized. `keybd_io.c`'s C-stack re-entry depth is
not snapshot state: the VM safe boundary requires it to be zero before archive
capture.

`checkpoint_smoke` proves direct state capture/mutation/restore and invalid
FIFO rejection, then creates a real delayed keyboard interrupt through
`host_key_down`, archives it through `softpc_device_archive`, restores the
queue, and dispatches it. This is still private restoration proof only: no
snapshot command, file format, Common API, or Lib code has been introduced.

Actual production changes are `+166/-4` across the private archive, its
fixed-width state contract, and the narrow original keyboard-controller hook;
the focused test adds `+38/-0`. Both full x64 and x86 CTest runs pass
`103/103`. Fixed packages are `softpc32.exe`
`CEBE79236A96DB0C50CABBB72D1135DAA9AC06B79A7498C2902E8AFBEC22B538` and
`softpc64.exe`
`B7B058DB87AB3FD59063124019A10CADAB0A47CC8AE2488F3DC995B9B330F027`.

## S6 P5: DOS INT 33h driver archive

P5 adds the missing DOS mouse-driver receiver without using the historical
`MOUSE_CONTEXT` or `MM_INSTANCE_DATA` memory layouts as a wire image.  The
private fixed-width map records installed state, cursor/motion/sensitivity,
graphics and text cursor backing, guest handler segment:offset and masks,
saved callback registers, video semantics and the driver-visible interrupt
rate/revision.  It deliberately excludes the instance handle, all host-width
or derived EGA addresses, guest scratch pointers and host cursor callbacks.

An inactive driver is a valid archive state: restoring it terminates an
installed live instance.  Restoring an installed archive creates the normal
driver instance when required, rebuilds EGA-derived bindings from the saved
video mode, and only then restores the semantic field map.  This keeps guest
cursor erase and callback continuation data intact without retaining a
process-local address.

`checkpoint_smoke` proves absent-state capture, installed-state capture,
automatic instance reconstruction on restore, byte-for-byte semantic
recapture, and restoration back to the absent state.  It remains a private
VM/Compat proof: no snapshot file, command, Common API or Lib code is added.

Actual production C/H change is `+480/-2`: Compat-private archive/state map
is `+113/-1`; the preserved original mirror receives the narrow `+367/-1`
port-ABI hook.  The focused checkpoint proof is `+27/-0`.  Sequential full
CTest passes `103/103` on x64 and x86.  Fixed packages are `softpc32.exe`
`4E93AC27F4B0A4D000C44452FDD5DDF6F405E36286A0D6581C3F290F49CF6409` and
`softpc64.exe`
`658543D40F2D84AB933C9B7116D2CAA88379B9A5D24E18237EE86AE6632C91C0`.

## S6 P6: video plane and DAC archive

P6 adds the first safe video receiver: all four selected V7 EGA/VGA plane
banks (including programmable fonts) and the 256 three-component DAC entries.
They are fixed-width guest data, not a legacy controller-structure image.
Restore copies those bytes back and requests a normal complete renderer
refresh; it does not serialize a DIB, a Window surface, dirty bookkeeping,
function vectors, or host pointers.

The controller registers, latches, bank/chain/mode values, cursor and
split/scroll state remain deliberately pending. They cannot be folded into
this receiver because their legacy structures contain bitfield layout and
derived process addresses. C-VID GDP remains separately pending because its
slot carrier mixes scalar fields with pointers and generated vectors.

`checkpoint_smoke` changes both endpoints of the plane allocation and a DAC
entry, captures, mutates, restores, and compares a fresh fixed-width capture.
The production archive also includes the receiver, so later whole-archive
tests exercise its placement. This is still private VM/Compat proof only: no
snapshot file, command, Common API, or Lib code is added.

Actual production change is `+64/-1` across the private archive/state
contract and Compat video boundary; focused checkpoint proof is `+27/-0`.
The final x64 and x86 full suites each pass `103/103`. The first x86 full run
had one timing-sensitive `softpc-package-smoke` failure at its monitor
pause-stop-start stage; its immediately repeated package, checkpoint and
documentation set passed `4/4`, and the final complete repeat passed. P6 does
not claim an unrelated package timing repair.
Fixed packages are `softpc32.exe`
`28DF09513FE461C0A33ACFBC00279C64761BEF1B7E298D269AEAAE268F8E39C1` and
`softpc64.exe`
`F1D1C6DD8AFD35596CEBC3AD82C0156F77F5B53B08136BF028D230943BEA39DF`.

## S6 P7: video-controller and C-VID reconstruction audit

The P7 source audit rejects two tempting but invalid shortcuts.  The EGA/VGA
controller structures in `ega_prts.c` use compiler-dependent bitfield
layouts, and the C-VID GDP side table contains native pointers, generated rule
entry addresses and function vectors alongside scalar values.  Neither can be
copied as a structure or as a GDP allocation image.

The audited value boundary is narrower and reproducible: the EGA/VGA/V7
register files, selected indices, attribute flip-flop and DAC cursor phase are
guest-visible scalar bytes; C-VID's ordinary four-byte latch and V7 foreground
latch affect the next memory operation and are scalar payload.  Plane/scratch/
screen pointers, string-read routes, mark/read/write vectors, `sr_lookup`,
dirty bookkeeping, DIBs and host drawing resources are process bindings or
host presentation state.  They must be rebuilt by the existing controller
register handlers followed by one full refresh.

This record does not implement a controller receiver and does not claim that
all GDP slots are serializable.  It instead freezes the required restore
ordering and retains the task stop condition: any active slot not provably
payload or rebuild remains a capture rejection, not an implicit reset.  No
production/test/original-mirror/Lib/Common files changed.

## S6 P8: video-controller register replay

P8 turns the P7 map into a private archive receiver.  The fixed-width state
contains sequencer, CRTC, graphics, attribute and selected V7 register bytes;
their selected indexes; attribute flip-flop/source byte; DAC cursor phase; and
the C-VID ordinary/V7 foreground latches.  It excludes every legacy bitfield
carrier, GDP allocation, generated C-VID vector, pointer, host DIB and dirty
record.

The narrow `vga_prts.c` port-ABI hook starts from the original controller
baseline, replays the original register handlers in dependency order, restores
the two latch scalars through existing `getVideo*`/`setVideo*` accessors and
requests one normal refresh.  No guest port I/O is replayed and no second video
state owner is introduced.  `checkpoint_smoke` alters real VGA ports, captures,
destructively changes the controller, restores, recaptures the complete fixed
map and compares it byte-for-byte; it also proves an invalid DAC phase rejects.

Actual code changes are production `+235/-0` across the private archive/state
contract and the preserved-mirror hook, and focused test `+33/-0`.  Full x64
and x86 CTest each pass `103/103`, followed by both package-smoke variants at
each width.  Lib and Common remain unchanged.  The refreshed packages are
`softpc32.exe` `3D2AF884E3D623280F50806192AE72216A86ADC53102614B24665CA74300A803`
and `softpc64.exe` `460919575D43A2C3915B0DECB18D62E2840FA4D935FD02C5863A5B6E5E96FC6F`.

## S6 P9: serial and parallel virtual-endpoint archive

P9 separates two concerns that cannot be conflated: the selected UART/LPT
controllers and their no-file standalone host carriers are finite machine
state, while configured output paths and host files are external effects.
The fixed map saves controller registers, UART interrupt state, virtual serial
RX/TX state, parallel registers, virtual printer buffer and delayed callback
handles. Queue callbacks are translated only to semantic receive/send and
printer OUT/ACK IDs. A configured serial or printer output path makes capture
fail before a file path, `FILE` pointer, handle or external output can enter
the archive.

The narrow preserved-source hooks only transfer original state through the
fixed map; Compat owns the virtual host queues and buffer. No Lib/Common or
product API changed. `checkpoint_smoke` proves destructive serial/parallel
round trips, printer delayed-callback archive/restore, and rejection of both
configured output endpoints. Production changes are `+469/-4` across six
paths; the focused test is `+62/-1`. Full x64/x86 CTest each pass `103/103`,
and both package-smoke variants pass at each width. The refreshed packages are
`softpc32.exe` `07C23E12C8CB3328DB6F54637FCB8273BAB18292CF77E3004685E196E7D1305E`
and `softpc64.exe` `C013E98E34D442A3F2D90149E1C2FD4A6CB8BD91F9C0E81683BFC416665180E4`.

## S7 P5: CCPU/SAS canonical stream slice

S7 begins its VM-owned stream implementation without exposing a partial
snapshot operation. The private CCPU archive now encodes and decodes the
already-reviewed CCPU register, execution, debug, TLB and FPU maps together
with SAS scalar state, RAM, page-type bytes and the fast-TLB index. Integers
are explicit little-endian values; only byte arrays are copied verbatim. No
legacy structure image, padding, pointer, callback or native handle crosses
the stream boundary.

Decode leaves the archive invalid until the separately owned device section
exists, so this cannot accidentally restore a core-only machine. The existing
checkpoint smoke captures a real archive, round-trips the new byte slice, and
checks the affected semantic maps and complete dynamic byte arrays. It also
rejects a mismatched declared SAS size before allocating archive backing.
Focused checkpoint tests and final full CTest each pass `103/103` at x64 and
x86. The refreshed packages are `softpc32.exe`
`8BEDA6DF9D689E8F1755E8DA6FB87AC4685CB868555C5F055EA2F44DD6A8EC9D`
and `softpc64.exe`
`91F61AF4E0BB8C61E40024250444350E0E74396722C19219BCC1D883C108E771`.
This P is internal only: it adds no App command, snapshot file, media
serialization, or public product behavior.

## S7 P6: device canonical stream slice

The private device archive now encodes every reviewed semantic device field in
the common little-endian callback stream. It does not write archive structs,
padding, callback pointers, queue links or host resources. Pending quick/tick
records use fixed `u32` fields; their counts are rejected before allocation if
they exceed the 65,536-record format limit. Decode builds a new archive and
only marks it valid after the complete stream succeeds.

`checkpoint_smoke` captures a real device archive, decodes it, then demands
byte-for-byte equality after re-encoding. Focused checkpoint smoke passes at
both host widths; final full CTest passes 103/103 at x64 and x86. The refreshed
packages are `softpc32.exe`
`01EA56DEA53EC1E964317BE79439CFF133D29A7A608800966AC08C7DE2925FA7`
and `softpc64.exe`
`609C4C216F4D73951F071B11746B6A04BEADF2B864CDB2739F4DA25E071AA051`.
This internal P adds no file, media, VM driver or product command.

## S7 P7: complete private image container

The initial private image container wrote independently owned core and device
streams with bounded lengths and the outer CCPU resume entry. The current S10
layout is deliberately simpler: it uses fixed stream order rather than a
version, magic, count or semantic identifiers, while retaining the bounded
length checks required to reject malformed input.

The checkpoint smoke now captures a live image, serializes the full container,
decodes it and compares complete RAM bytes plus the resume entry. This P stays
below the Common driver contract and contains no App file command, media
section or user-visible save/load operation. Focused checkpoint proof passes
at x64 and x86.

## S9 P7: actual VGA sequencer reset owner

Owner reported that correct restored geometry still produced a black Window.
The supplied image reproduced a 640x480 all-zero output with 294736 nonzero
VRAM bytes and 255 nonzero palette entries. The original painter returned
early because `display_disabled` was 3 (both sequencer reset bits asserted).
VGA delegates register-zero writes to `ega_seq_reset`, whose private EGA
sequencer is separate from the VGA structure. The archive incorrectly read
VGA's unused zero field. A diagnostic correction to the saved reset value
produced 37343 nonzero pixels from the same image; that forced value was
removed, not shipped.

Capture now obtains the real register through one read-only mirror hook.
Restore still calls the original reset receiver. Revision 4 rejects prior
images because their true reset value was not captured; no guessed repair or
legacy reader is retained. The mirror change is six hook lines, one import
and one corrected read. No Lib/Common change is required.

The similar-issue sweep checked VGA's imported EGA sequencer and graphics
handlers: selected VGA initialization replaces sequencer entries 1-4 and
graphics entries 0-8 with VGA receivers; reset alone retains the EGA owner.
Tests round-trip all four reset combinations, require display enabled after
ordinary restored VGA state, and boot a tiny real mode-13h colored framebuffer
before save. Fresh-process restore must retain nonzero pixels. This closes the
previous test gap where only a valid frame and PAUSED state were asserted.
The diagnostic ran with overlay media and did not alter owner media or image.
Final verification: x64 and x86 each pass 105/105 CTests; bidirectional
cross-width graphics save/load also passes. Production (three C paths) is
+9/-2, tests (two C paths) +60/-2, excluding documentation and packages.
Owner Win3.1 acceptance remains pending; S9 is not closed.

## S9 P8: complete restored scan lines

Owner reported a thin strip at the top followed by black after mouse movement.
The supplied revision-4 image reproduced height 480, line stride 1024, but
screen length 61440: exactly 60 rows. The restore cleared CRTC state, then
`vga_init` installed its boot-time character height 8. Replaying a saved
maximum-scan-line value of zero took the original unchanged-register path,
leaving 8 instead of 1. Restore now starts that derived value consistently
with its cleared register before the original handlers execute.

The same baseline audit found `ega_write_init` reset the CPU to unchained
while display chain flags could retain their prior values. That doubled the
restored line stride in the V7 round-trip probe. The same restore boundary
now resets those two flags to match the initialized CPU. No normal register
write logic or public API changes. The owner image then reports length
491520 and pixels in all 480 rows. Revision 4 and owner files are unchanged.

The V7 regression starts from BIOS-programmed state and compares character
height, line stride and full screen length after restoration. The full
transaction fixture now requires every pixel to equal its boot program's
color, rather than accepting a single nonzero pixel. Temporary probes and
owner file paths were removed from production/tests. Production is +8/-0 in
one mirror C file; tests are +19/-1 in two C files.
Final x86 and x64 full suites each pass 105/105; bidirectional cross-width
complete-frame save/load passes. Both fixed packages were rebuilt. Owner
acceptance remains pending and S9 stays active. Existing owner deletions of
obsolete bisect packages are preserved separately from this repair.

## S9 P9: restore executable VGA/V7 state, not only its first picture

Owner reports a correct initial restored frame followed by horizontal
stretching and corruption on mouse/key input. The actual Win3.1 diagnostic
reproduced the stretch after a fresh-process load. Before input chain4 was
enabled with stride 1024; afterward sequencer memory mode had changed from
0x0e to 0x03, disabling chain4 and reducing stride to 256.

The snapshot saved V7's extension-enable latch but replayed it as a port
command. The original receiver accepts EA/AE commands, not the latch value 1.
Consequently the restored extensions remained locked, the selected F6 index
became 06, and subsequent V7 pointer writes aliased ordinary VGA indexes.
Restore now assigns that semantic latch before restoring the selected index.
No ordinary port handler, mouse mapping or KVM scaling behavior is changed.

The same receiver/readback audit found three obsolete shadow fields: sequencer
map mask, graphics read map and bit mask are actually owned by C-VID getters.
Capture now uses those getters, matching original INB behavior. The DAC pixel
mask is likewise not the DAC component-depth mask; capture/restore now use the
pixel-mask accessors, while V7 DAC control rebuilds component depth. Version 5
rejects earlier images with lost masks; no guessed defaults or compatibility
reader are added. The original image/media/INI are not rewritten.

Replay also clears the derived write-state lookup/set-reset/enable/RAM fields
before VGA initialization, and the zero-clock/shift display flags before
register replay. Previously zero-register replay skipped change-only handlers
and left set/reset=15 and XOR enabled despite saved zero values. Tests now
check the live values, actual post-restore plane writes in all four write
modes, selected read planes, full/partial masks, and V7 pointer-port routing.
The boot fixture polls a real keyboard event after resume and repaints every
pixel, so fresh-process/cross-width checks exercise subsequent CPU video writes.

The bounded disposable diagnostic boots the existing Win3.1 media in overlay,
waits for DOS, types WIN, saves after graphics startup, then supplies ten
relative mouse moves and Tab/Right key transitions. A fresh process loads the
same image and supplies the identical inputs. Both final frames are 640x480;
their RGB difference bounding box is empty (pixel-identical). Diagnostic code,
raw frames and snapshot are temporary build-owned evidence, not product/test
fixtures or dependencies. The shared Lib/Common corpora remain unchanged.

Final-source x64 full regression passes 105/105 (111.49s), and the fresh-process
keyboard-redraw test passes 30 consecutive final-source runs (22.18s), in
addition to 15 preceding diagnostic runs. Initial development runs did time
out waiting for the repaint; this is recorded rather than treating those runs
as passes. The final fixture disables IRQ1 while polling port 64h/60h so BIOS
and the fixture do not compete to consume the input. Both cross-width
save/load directions pass including actual post-resume repaint. Final x86
full regression passes 105/105 (100.89s); documentation governance and diff
whitespace checks pass. S9 remains open for the owner's fresh-image test.

Against P8, two production files are +17/-7 (net +10); two test files are
+103/-14 (net +89), excluding documentation and EXEs. No alternative renderer,
input path, Lib/Common API, or compatibility reader was introduced. The
temporary Win3.1 diagnostic code and owned image/frame captures were removed;
only fixed package EXEs are refreshed. Pre-existing owner deletions under
`assets/binary/t63-bisect/` are left separate from this corrective P.


## S10 admission: complete FDD/HDD overlay snapshots

Owner explicitly rejects T63 closure while overlay content is absent.
The attempted closure and T64 admission were documentation-only and never
committed; they are withdrawn. Lib optimization remains queued.

S9 display/input fixes are manually accepted. T63 remains open because
the original one-binary media requirement is not delivered. S10 must audit
all media owners, implement overlay archive/restore through existing Storage,
and prove fresh-process restoration of both disk content and machine state.
No new Common API or Lib change is pre-approved. See the active packet and
the proposal's S10 ledger.

S10 pre-implementation audit confirms the missing media section and reset's
destructive reattachment of overlay leases. The additional omitted host state
is GFI's current cylinder. Existing controller capture covers HDD registers,
not disk contents. The proposal now enumerates the media owners, DIRECT
sharing constraint, installation order and estimated implementation/test scope.
No production code or new successful snapshot proof is claimed at this point.

## S10 implementation: FDD/HDD overlay payload

The original media gap is now implemented in Compat plus the VM-private image
container. A fixed MEDIA payload archives the complete effective overlay
delta for every fixed owner slot. It never copies DIRECT/READONLY disk bytes.
VM stages/validates media before reset and Compat installs prepared overlay
leases after reset but before device/CPU restoration. The real post-reset
external lease is revalidated for DIRECT/READONLY. FDD current cylinder is
included. No Lib/Common/MVDM change was made.

Focused x86/x64 media and snapshot tests passed, followed by full x86 106/106
(121.24 s) and x64 106/106 (133.11 s). Owner manual acceptance remains
required; this record does not close S10 or T63.
