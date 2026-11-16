# T100 S5 Compat simplification audit

Status: active investigation; baseline2464862d. Owner request:
现在准入一个新s 按照dir固定顺序审计compat 目标是精简compat代码规模 减少文件数和行数。

## Finite scope and progress

Frozen50 files,8957 physical lines (including comments/blanks), all
tracked files under src/app-softpc/compat. Source of order is literal Windows
dir /a /b /s output with directory entries filtered; no sorting. The
[file ledger](m9-t100-s5-compat-files.tsv) includes sizes,
hashes and per-file status. Frontier: file1 audio.c under review; none completed.
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
