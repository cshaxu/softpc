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
