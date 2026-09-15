# HLT and audio failure handling

## S20 admission

Original owner request: 批准hlt和音频失败处理；请你准入一个s任务 编译测试提交推送。

M9 T59 S20 continues from clean main c4e87f3. S19 delivery and review remain
in [history](../history/M9-T59-S19-machine-paused-wait-failure.md); T59 stays open.

HLT must never conceal missing/failed native wake resources with sleep/retry.
Compat validates timer/event readiness before CPU execution. Failed HLT wait
unwinds through the existing outer CCPU return; the machine run returns its
existing IO_ERROR, which the unchanged VM/Common path already reports.
Retain the original timer owner and normal infinite HLT wait.

Audio retains synchronous 40 ms Beep slices. A failed playback ends that burst
and returns to the existing event wait, not an immediate retry. Distinguish
zero-time wait timeouts from failures; stop/cancel remain normal termination.
No audio backend replacement, CPU pacing change, frame-sampling change,
Lib/Common/MVDM change, public product API, media or INI mutation is admitted.

## Finite convergence ledger

| Member | Result | Evidence |
| --- | --- | --- |
| Initialization | Failed event/timer creation prevents CPU run | Injected native creation failures, cleanup and recovery |
| HLT | Missing/failed wait ends run with IO_ERROR; no polling | Injected wait failure and real CCPU unwind; normal wake regression |
| Audio | Failed Beep cannot spin; stop/cancel/wait failure leave worker | Scripted playback/event results without real audio or Sleep |
| Similar paths | All wait/playback failure branches in the two implementations accounted for | Source sweep and tests |
| Delivery | Dual-width build/full tests, fixed EXEs and clean pushed P1/P2 | Hashes, production/test counts and actual-change review |

Scope: Compat platform/machine/audio implementation and private declarations,
product unit tests/build registration and task records. Use existing build trees;
tests create/remove only their own tiny disposable fixture. No traces, new
external data or new media inputs. Count tracked production/test C/H against
c4e87f3 separately from CMake/docs/binaries. Stop for a required CPU semantic or
shared-corpus redesign. Automatic verification does not imply owner acceptance.

## Implementation and bounded sweep

Compat retains the event/timer lifetime. Its private readiness query rejects
failed initialization before CPU execution. A failed HLT wait sets one executor
fault latch and uses the existing CCPU outer return; machine_run then returns
IO_ERROR through the existing VM driver path. Timer shutdown clears the latch.
No new exception, callback, retry, state machine or public product API is added.

Audio keeps the existing worker and 40 ms Beep call. Failed playback breaks
the burst and returns to the blocking stop/wake wait; a new tone notification
may start a new burst. Post-slice stop/wake/cancellation use one wait_any instead
of two separate waits. Only TIMED_OUT continues playback; other failed waits
or failed wake resets leave the worker. Audio does not introduce a VM fault or
a new diagnostic channel. Existing task destruction joins this same worker.

Sweep: rg -n 'WaitForSingleObject|base_sync_.*wait|Beep|CreateEvent|CreateTimerQueueTimer|event_reset'
src/compat/audio.c src/compat/platform.c. HLT's sole native wait, event creation
and timer creation are checked. Audio's sole playback call, blocking wait,
post-slice wait and both wake-reset sites have explicit exits. Audio startup
already unwinds failed event/task creation; shutdown retains its existing join.
CPU pacing, sampling, signal-delivery policy and native timer teardown are not
redefined by this bounded failed-wait/playback repair. No additional retry path
was found in the searched scope.

platform_failure_smoke injects failed event/timer creation, verifies partial
cleanup and successful initialization, and tests a normal wake. It then runs
real CCPU CLI/HLT code: an injected WAIT_FAILED must exit with IO_ERROR after
one wait; another run is rejected before CPU entry. Destruction clears the
resources and latch. audio_failure_smoke scripts seven playback/wait/reset
cases without sound or Sleep, including normal continuation after timeout.
Both fixtures include the production implementation so native-call injection
does not add production hooks or expose worker internals in a public API.

Tracked C/H diff against c4e87f3: platform.c +22/-6, platform.h +1/-0,
machine.c +4/-1, audio.c +7/-7: production +34/-14, net +20. New product tests
are platform_failure_smoke.c (+82) and audio_failure_smoke.c (+59), net +141.
CMake registration adds 15 lines; total code/build net +176, excluding docs
and EXEs. Lib/Common/MVDM/VM and user configuration/media are unchanged.

## Executor verification

Both tests-x64/tests-x86 builds passed. Full CTest: x64 96/96 (80.56 s),
x86 96/96 (65.50 s). The native-injection tests use the same GNU90 legacy
headers/options as Compat; existing legacy warnings remain, so this is not a
claim that the preserved CPU/Compat corpus passes strict warning-as-error.
All four shared manifests, documentation governance and whitespace checks pass.
No manual listening, Linux runtime or owner acceptance is claimed.

Fixed outputs: softpc32.exe 3,484,990 bytes, SHA256
5E6BDEE6FF6708DD2913BD16A768F308DDF4E4024CC942F8D8D559CE6B9FE004;
softpc64.exe 2,844,098 bytes, SHA256
F7E7C3EF6357C541A9F162727EF6C45AE0060FA99B81F21EDBC516FB2C27F8CC.
Existing build trees/evidence remain; regenerated types-layout fixture children
are removed after tests. The disposable platform test image is removed by its
test. No new trace, media or build tree is retained. All five ledger members
are verified for executor P1 delivery; actual-commit review follows separately.
