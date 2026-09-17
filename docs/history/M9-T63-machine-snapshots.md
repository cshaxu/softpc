# M9 T63: machine snapshot implementation record

## Scope and admission

Owner approved the revised [snapshot proposal](../proposals/m9-machine-snapshots.md):
read only while running, write only init/stopped, success ordinary paused;
VM owns safe-stop selection and a single 1-second monotonic deadline. Lib is
unchanged; Common may add only the two state operations and necessary wiring.
S2 remains active. This record is partial proof, not snapshot acceptance.

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
