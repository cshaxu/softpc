# Pause Input Handoff

T98 S15 is owner-admitted against af55acf7. Emulator production owns the repair;
X86 IBMPC keyboard's existing enqueue adapter must also adopt status results,
with keyboard_smoke and both X86 IBMPC manifests. This exact necessary caller
expansion was reported before editing; no PC policy changes. App integration
tests adapt the changed public result type. Lib and X86 Debug remain unchanged.

## Approved replacement contract

The owner approves freeze-on-pause, explicitly replacing drain-before-PAUSED.
The executor closes admission using the FIFO mutex, stops ordinary input
delivery and parks. Accepted pending events stay in that FIFO; genuine resume
delivers one per existing safe callback, with new events appended behind them.
No new buffer, callback, thread, retry or guest execution for draining exists.
The obsolete paused input event and its allocation/signal/wait/destroy route
are removed. Normal driver delivery remains the original single-event path.

State-read completion is serviced before ordinary input at an executor callback:
a capture already completed by the driver must request pause before another
input can mutate its captured device state. Paused capture continuation keeps
admission closed and does not consume the retained FIFO. This does not change
driver image format or include host-pending events in its snapshot. Cold run
and successful state replacement retain their existing queue clear. Stop/error
close admission without injecting a final burst after driver unwind.

The atomic cutoff is the executor's queue close, not the caller's asynchronous
pause request. An enqueue ordered before cutoff succeeds and is retained;
one ordered after cutoff returns INVALID_STATE. Enqueue does not promise guest
consumption. Full queue returns LIMIT_EXCEEDED; ordinary closed-state refusal
is handled normally by the Session and X86 IBMPC adapters, without state reread
or resending any accepted prefix. Driver deliver_input/request_wake remain void.

Not included: Session's host key-release ledger while paused, exact replay of
pending host input through snapshot load, and product control-line release.
Neither this contract nor PAUSED enum authorizes external driver-state writes.

Verification extensions: real threaded executor holds 255 alternating events
across ordinary/debug pause, rejects closed pushes, retains full-capacity status
on resume, then delivers exactly one record per controlled callback. A new
event enters behind the retained prefix. The scripted real worker checks queued
input through paused capture continuation and already-completed capture, plus
existing stop/reset/debug and synchronization-failure cases. No sleep is used
as a concurrency oracle.

Resume explicitly requests the existing driver wake when the retained FIFO is
nonempty, so delivery does not depend on a new key or timer tick. The threaded
fixture counts that wake. The paused host-key-release issue is transferred to
TODO for a separate reproduced/approved contract; no stuck-key fix is claimed.

## Finite audit ledger and design

The following is the rejected implementation draft, not an approved final
contract. The owner returned this S to research/design after executor review.

1. Machine enqueue checks RUNNING before queue push acquires its mutex. The
   executor can publish PAUSED between those operations: confirmed source race.
   The queue gains one initially closed admission bit, protected with push by
   its existing mutex. Machine owns transitions; pop releases the lock before
   invoking the driver. No extra queue, lock or generation is introduced.
2. Executor currently delivers only one record per callback and also consumes
   input in its PAUSED wait. Close admission at the pause boundary, drain the
   finite accepted prefix, disable heartbeat, publish PAUSED. Delete the paused
   input-event route and its event allocation. Normal running consumption stays
   one event per safe callback, retaining existing driver wake scheduling.
3. Return lib_status from enqueue/push: OK accepted, INVALID_STATE closed,
   LIMIT_EXCEEDED full, INVALID_ARGUMENT invalid. Session's sole adapter treats
   OK/INVALID_STATE as normal handling, all other results as failure. It does
   not reread machine state to guess why enqueue failed. Callers never replay
   an accepted prefix. KVM/monitor/source-ledger contracts stay unchanged.
4. Open only on genuine initial running/resume; keep closed during initial
   paused reset/load and internal paused state-read continuation. Existing
   heartbeat continuation is not a public resume. Preserve debug leases and
   state-read completion/suppressed notifications. A raw PAUSED enum is not
   permission for external concurrent driver access.
5. Close for stop/reset/error/shutdown. Successful stop unwinding can finish
   accepted input on the executor before terminal completion; failure paths
   report ERROR rather than promise successful delivery. Cold-run clear remains
   a generation boundary, not a pause implementation. Lifecycle calls remain
   serialized by the existing public contract; stop wins over pause completion.
6. Driver deliver_input and request_wake are void. There is no hidden status
   to propagate and no new callback signature is proposed. Existing execution/
   publication/synchronization failures retain their ERROR route. Removing the
   paused-only input event removes its ignored signal-result path altogether.
7. Session FIFO extreme phase delays and product control-line release remain
   outside this bounded Machine FIFO contract. No arcade-specific logic or
   host-ledger-as-guest-ledger assumption is introduced.

Estimated production +65-100/-35-60; tests +180-260/-15-35, separate from
manifests/docs/artifacts. Expected owners: machine/input_queue.c/h, machine.c,
machine_interface.h, session/session.c; existing Machine/Session tests and
three App integration callers. Tests use controlled barriers/private fixture
inclusion, never test-only production APIs or fixed sleeps as proof.

Completion requires an explicit disposition/proof for all seven ledger items,
dual cached builds/releases, public unit/gates and relevant App input/snapshot/
debug integration. Native execution is reported separately; local tests do not
establish every downstream product's experience. No downstream checkout is read.

## Executor review: delivery withheld

The first implementation is NOT accepted for delivery. Cached dual builds and
24/24 focused cases per width passed; background suites passed 184/184 per
width. x64 desktop 5/5 and native audio 3/3 passed. x86 desktop was 4/5:
compact_console failed at stage 1/error 6 (the existing initialization symptom).
x86 native audio and final qualification were not completed. These results do
not prove the PC keyboard safety requirement below.

A task-local probe linked existing x64 machine objects and the original keyboard
path, without modifying guest/user media or mirror source. It booted generated
disposable media, then injected alternating A make/break through
softpc_platform_keyboard_scancode, the same Compat keyboard ingress used by the
driver. With no guest reads between injections, event 34 caused keyboard snapshot
capture to fail: the preceding snapshot had FIFO count 48 and output_full=1.
Reading the resulting stream returned 17 make, 16 break and one overrun code.
Restoring the saved keyboard state, then injecting and reading 100 alternating
events individually preserved every byte in order. Probe exit was zero.

This is dynamic proof of the downstream burst limitation, not a complete
Machine-pause integration reproduction. Source connects the proposed tight
drain loop to that same ingress. Original keyba.c add_codes_to_6805_buff replaces
overflowing scan sequences with an overrun marker; snapshot capture rejects
counts above its 48-byte logical capacity. Machine FIFO can hold 255 events and
deliver_input returns void, so the draft cannot establish lossless delivery.

Do not ship the tight-loop drain. The alternative of executing guest work between
accepted events also needs explicit debug/snapshot semantics: it cannot silently
advance a stopped debugger merely to drain keyboard data. Resolving the generic
delivery boundary versus SoftPC device capacity requires further design and an
approved scope decision before changing the driver contract or mirror behavior.
No new callback/backpressure framework has been added. Correct atomic admission
and status work is retained uncommitted; draft binaries are restored to the
committed baseline. This is not a completed P or a request for user testing.

## Research restart: semantic constraints

Owner approval permits reconsidering or withdrawing the entire draft; it does
not require retaining the drain-before-pause requirement at any implementation
cost. No new production changes are made in this research phase.

Distinguish three boundaries: accepted into Machine FIFO, delivered to driver,
and consumed by guest software. Current void deliver_input confirms neither
device capacity nor guest consumption. The isolated keyboard probe proves a
burst limitation, not that the baseline avoids all overrun or that a complete
pause integration reproduction has occurred.

Source-proven constraints:

- Baseline executor consumes one input before testing pause, then may consume
  another from input_event in the paused wait. This establishes late-delivery
  possibility, but not a complete guest-consumption guarantee.
- vm_driver_take_debug_stop observes stop_pending AND result_ready. Continuing
  guest instructions merely to drain input would require a changed debugger
  contract; it cannot be assumed equivalent to ordinary pause.
- vm_driver_snapshot_observe captures/writes the image, finishes the capture,
  marks the result ready, then calls the Emulator executor callback. Draining
  new device input there can change live state after capture. This is source
  ordering evidence, not a newly reproduced snapshot failure.
- Paused state-read continuation temporarily runs toward a capture boundary;
  it is not an ordinary resume and must not reopen ordinary input admission.
- Original keyboard progress depends on guest/controller reads, not simply on
  the number of safe callbacks. One input per callback avoids the new immediate
  burst but by itself cannot prove losslessness.

Research compares (1) baseline, (2) rejected tight drain, and (3) normal-cadence
drain without asserting that the third is already viable. A simpler alternative
to evaluate is pausing delivery while retaining pending FIFO entries for genuine
resume. That changes the original no-late-delivery requirement and requires an
explicit owner decision, including how host-pending input relates to save/load
and cold reset. It must not be silently implemented as the original contract.

Completion of research requires a single explicit contract for pause/debug,
continuation, resume, stop/reset and snapshot; every accepted event must have a
defined disposition. No extra queue, larger keyboard buffer, product callback or
epoch mechanism is authorized merely to preserve the draft.

## Revised implementation checkpoint

The approved freeze contract is implemented. Production C/H versus af55acf7:
six files +63/-26 (net +37). Test C/H: ten files +200/-20 (net +180).
Counts use git diff --numstat restricted to tracked src/test C/H, excluding
README, manifests, governance records and EXEs. Changed shared trees are
src/emulator, test/emulator, src/x86-ibmpc and test/x86-ibmpc. The latter pair
only adapts status handling. Three App integration callers now compare OK
explicitly. No root test infrastructure, Lib, X86 Debug, App production,
mirror, configuration or guest-media change.

Actual-path self-review finds one driver delivery call, after queue pop unlocks;
no delivery in parked waits, no tight drain at pause/stop, no new queue/thread/
callback. Resume wakes the driver for retained input. Real input callers were
enumerated with rg emulator_machine_enqueue_input over src/test; all use status
semantics explicitly. The queue close/push mutex owns admission ordering.
Downstream consumers must adapt this public return type; treating OK as a bool
would invert acceptance. No downstream repository was read or modified.

Final production build succeeds for x64/x86 with cache and both package EXEs
are refreshed. Build logs have no warning/error diagnostics. Final background
tests pass 184/184 on each width. x86 also passes desktop 5/5 and native audio
3/3. Final x64 desktop passes its three Lib cases but both package cases fail
at initial prompt stage 3/error 0; native audio fails all three cases. A session
query during these failures reports Disc. This is an environmental observation,
not a proven explanation of all historical package intermittency. Reconnection
and explicit x64 desktop/audio rerun are required before P delivery.

The earlier x64 run caught a test-only expected-status omission: new completed
capture action 8 correctly returned OK but the existing shared assertion allowed
OK only for action 6. The assertion now names both completed-capture scenarios;
final x64/x86 whole background suites pass. No product timeout/assertion is
weakened. Superseded run failures remain disclosed, not counted as final passes.

No S15 commit/push or acceptance is claimed at this checkpoint. Scratch remains
owned by the active packet for the pending native rerun and evidence audit.

## Final qualification after reconnection

The owner reconnects. x64 desktop rerun passes four cases, but compact package
fails at stage 1/error 6 while Active. After the native group exits, the unchanged
isolated compact case passes (12.29 seconds). No assertion, timeout or product
code was changed for that rerun; this remains the existing package TODO.
All three x64 native-audio cases pass after reconnection.

Evidence before scratch retirement: x86 final-background/desktop/native_audio
reports; x64 accepted-background, reconnected-desktop/native_audio and the
explicit compact-x64 supplement. An XML audit checks every CTest registration
by name, exactly once in the effective passing set: 192 per architecture,
184 background + five desktop + three native audio, zero skips. The single
failed Active compact case is explicitly replaced only by its named unchanged
supplement, not reported as an all-green aggregate. Earlier failures above
remain part of the record. No downstream or complete manual experience claim.

Published SHA-256:

- softpc32.exe: 5337941107375D771CC3B82D75ED598CCB6F566D2E4251EE0699893CD7524BF5
- softpc64.exe: 7AB10E162F542F9F3D157E4C356E5971ADCA86DDD3E46FDDD7FF485490C903EC

User INI and untracked snapshot.debug hashes remain unchanged. Task-owned jobs
have exited; disposable evidence is retired after promotion of this record.
Only the two reusable build caches remain. S15 awaits owner acceptance after
executor delivery and actual-commit coordinator review; T98 remains open.

Downstream import scope is src/test emulator and src/test x86-ibmpc, including
manifests. There is no root test-infrastructure change. All external Machine
enqueue callers must compare lib_status explicitly: OK is accepted,
INVALID_STATE is normal refusal, other errors are failures. No old bool alias
is retained. Lib and X86 Debug trees are unchanged.
