# T100 S5 Compat simplification audit

Status: active investigation; baseline2464862d. Owner request:
现在准入一个新s 按照dir固定顺序审计compat 目标是精简compat代码规模 减少文件数和行数。

## Finite scope and progress

Frozen50 files,8957 physical lines (including comments/blanks), all
tracked files under src/app-softpc/compat. Source of order is literal Windows
dir /a /b /s output with directory entries filtered; no sorting. The
[file ledger](m9-t100-s5-compat-files.tsv) includes sizes,
hashes and per-file status. Frontier: file1 audio.c source review complete, candidate awaiting owner decision;49 files pending.
No production/test/configuration/artifact changes in admission P1.

## File1 audio.c: preliminary source findings

Current372 lines. Owns PC speaker tone-to-PCM host presentation, not SB PCM/MIDI
or original PPI/PIT state. CMakeLists.txt:256 compiles it; machine/driver.c invokes
start/shutdown; original nt_sound.c submits tones and reads the clock; reset.c
calls host_disable_timer2_sound. Therefore the file cannot simply be deleted.
Reading audio.h and the inward Base contract does not advance their file review.

- Two unused extern declarations (T2State, PlaySound) are local cleanup candidates:
  neither identifier has a use in this file. Expected production0/-2.
- softpc_speaker_stop duplicates the task's cancellation event for the outer
  wait. base_sync_wait_any includes cancel_task->cancellation itself and gives it
  precedence; shutdown successfully requests cancellation before signaling the
  extra stop event. Candidate: remove the extra handle/create/rollback/destroy,
  its event-index branch, and use wake plus the existing task cancellation.
  Expected net reduction roughly15-25 source lines, zero new abstractions and
  zero file reduction. This estimate is not a tested diff. Existing PCM
  cancel_wait remains essential to unblock stream waits that do not watch the
  task event. Active synthesis and finite-tone completion still need review
  before declaring the whole shutdown simplification verified.
- Preserve onset retention, generation supersession and accepted-frame phase
  accounting pending concrete equivalence proof. Short gate pulses must not
  disappear before first submission; finite and continuous tones have distinct
  completion semantics. No deletion merely because both involve PCM.
- Existing first-tone, first-delivery, audio_failure and machine sound fixtures
  exercise the actual production implementation with controlled sinks. The
  failure fixture references stop directly and must adapt if that resource is
  removed; do not delete assertions solely to make a new layout pass. Native
  audio lifecycle is a separate device-dependent integration test.

No fresh build/test or claim of a confirmed runtime defect. Next investigation:
finish file1's ownership/cancellation/call-path review and give the exact minimal
candidate, then proceed in ledger order. Cross-file aggregation remains pending;
no total file/line reduction is promised before evidence.

## P2: file1 audio.c minimum retained responsibility

Owner directs sequential file reviews and a report of the minimum retained
parts. This report finishes file1 only; related files were read for evidence,
not marked reviewed. No source/test modification or new runtime test.

| Required part | Why it remains |
| --- | --- |
| GetPerfCounter and host_disable_timer2_sound | Original nt_sound.c reads timing; reset.c stops the original PPI tone. These are host contracts, not duplicate device logic. |
| Frequency validation and single request handoff | Original LazyBeep submits stop, continuous frequency and one-millisecond CLICK; asynchronous rendering must copy the request and synchronize it. |
| One onset record | Existing first-delivery/failure fixtures explicitly exercise start then gate-off before worker execution. Removing it can erase a real short tone. |
| Generation check | Finite completion must not erase a later request; continuous frequency changes reset phase. It is one existing supersession token, not an extra queue. |
| One speaker worker, wake event, request mutex and PCM stream | Lib transports PCM but does not synthesize a frequency request. Moving this product synthesis into Lib or blocking the guest executor is not equivalent reuse. |
| Square-wave phase and accepted-prefix handling | enqueue may accept only a prefix. Advancing phase by requested rather than accepted frames changes subsequent samples; silence/gate-off flushing must not discard accepted data. |
| Finite and continuous rendering semantics | Original PlaySound calls LazyBeep(CLICK,1) as well as INFINITE tones. Finite countdown/checked flush is not a test-only capability. |
| Task cancellation plus PCM cancel_wait | The former wakes base_sync_wait_any; the latter interrupts a different blocking wait. Both are required despite deleting the duplicate stop event. |
| Join before release and error reporting | Driver retains admission/context after shutdown failure. Neither a failed join nor a failed stream destroy permits freeing dependencies. Ordinary cancellation stays silent. |
| Existing non-Windows branch | No platform-support removal is admitted. Its broader behavior is not changed to obtain line reductions. |

### Exact candidate (memory-only construction)

1. Delete unused T2State/PlaySound externs and now-useless PlaySound undef:3 lines.
2. Delete softpc_speaker_stop and all creation, rollback, signaling and destruction
   wiring. Wait only on the existing wake event plus task cancellation; remove
   event-index storage/branch. Shutdown's extra wake signal is also redundant
   after successful task cancellation. The inner synthesis does not consume
   either event; retain stop_request and PCM cancel_wait for that path.
3. In start and shutdown, replace write_request(0,0)+cancel_onset with the
   existing stop_request. It performs the same field writes and one generation
   increment while holding one lock instead of two. No new helper or state.
4. Remove the second frequency!=0 check immediately after its zero branch has
   already broken out of the loop. Duration handling itself remains.

A deterministic in-memory edit of current audio.c produces **+5/-28,net-23**:
372 ->349 physical lines, file count unchanged. This is a source candidate count,
not a compiled or delivered patch. No mirror diff, public API or new abstraction.
No larger deletion is currently established as equivalent. In particular do not
merge SB PCM/MIDI with PC speaker merely because all ultimately output sound.

### Evidence and implementation checks

Current Base sync.c inserts the task cancellation event at wait index0 and returns
CANCELLED before considering ordinary events. shutdown returns immediately if
request_cancel fails; on success the redundant stop event cannot add a distinct
stop result. Worker handle/resource creation precedes task start. The actual
nt_sound.c PlaySound body proves one-millisecond finite tones are production use.
The original request/gate-off/phase and failure fixtures provide verification
points, not a newly executed pass in this review.

Implementing the candidate must adapt the existing audio_failure fixture's
resource-count assumptions and fake wait result to the one-event layout, while
retaining first-tone, finite/continuous, backpressure, flush/error and failed-join
checks. Add/retain real Base cancellation coverage for an idle speaker and PCM
cancel_wait coverage for a blocked producer. Run cached x86/x64 affected tests
and the repository-required delivery suite/publication after approval. Original
warning/manifest boundaries remain. A static cancellation proof does not replace
that runtime qualification. Test line delta is not yet measured.

Next ordered file is audio.h (13 lines). No source changes are approved or
implemented merely by completing this audit entry.
