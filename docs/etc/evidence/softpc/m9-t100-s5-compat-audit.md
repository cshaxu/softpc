# T100 S5 Compat simplification audit

Status: active investigation; baseline2464862d. Owner request:
现在准入一个新s 按照dir固定顺序审计compat 目标是精简compat代码规模 减少文件数和行数。

## Finite scope and progress

Frozen50 files,8957 physical lines (including comments/blanks), all
tracked files under src/app-softpc/compat. Source of order is literal Windows
dir /a /b /s output with directory entries filtered; no sorting. The
[file ledger](m9-t100-s5-compat-files.tsv) includes sizes,
hashes and per-file status. Current progress is recorded in the latest P entry
below; earlier frontiers and approval requests are historical.
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

## P3: approved audio.c simplification

Owner: "帮我做修改，老规矩五件号。" Baseline eb141ce7. Only file1
implementation is admitted; the remaining49 ordered reviews remain pending.

Removed the redundant stop event, its creation/rollback/release and extra shutdown
signals. The existing Base task cancellation owns outer-wait termination; the
existing PCM cancel_wait still interrupts PCM backpressure before joining. Reuse
stop_request in start/shutdown to update the request and onset under one lock.
Unused original-sound declarations and the already-proven frequency check are
removed. Onset, generation, finite duration, accepted-prefix phase, native error
reporting and failed-join/stream ownership remain unchanged.

Actual production delta: one file +5/-29,net-24 (372 ->348 lines). The extra
deleted line versus P2's estimate is the blank after removed declarations.
Tests: three files +15/-17,net-2. No files, interfaces, mirror differences,
shared packages, snapshot format, media or configuration changes.

Existing failure fixture now requires one wake event, checks task cancellation
before PCM cancellation before join, retains failed-join retry/ownership, and
rejects redundant shutdown event signals. Existing real-Base first-tone fixture
also starts/stops an idle speaker. Removed unused sound stubs in both real-Base
fixtures. No new test executable or production test API.

Similar-issue sweep: searched Compat for request_cancel/cancel_wait/speaker_stop
and the affected fixtures for T2State/PlaySound. The speaker is the only Compat
Base task cancellation owner; SB MIDI cancel_wait serves its separate native
output and is retained. Original sound declarations/definitions elsewhere have
real callers and are not removed. No later file is marked reviewed.

Verification completed on both architectures using existing Ninja/ccache trees:

| Architecture | Background (unit, gates, integration) | Native desktop/audio/package | Fresh EXE timestamp |
| --- | --- | --- | --- |
| x86 | 188/188 | 8/8 | Nov 15 2026 20:34:35 |
| x64 | 188/188 | 8/8 | Nov 15 2026 20:37:43 |

392/392 total. Both full build logs contain no compiler warning/error; package
post-build validation verified timestamps. The existing PCM stream fixture also
checks blocked native waits/cancellation; the speaker fixture checks its calling
order with injected PCM, not real-device backpressure timing. Native lifecycle
passed separately. No new manual guest listening claim.

Actual-diff coordinator review: removed stop event is never consumed by the
inner synthesis path; task cancellation is ordered first by Base wait_any.
PCM cancellation and join-before-release remain. The stopped request fields and
generation are unchanged, now written under one existing lock. No public API,
second path, new state/lock/helper, polling or ownership regression identified.
Review covers this bounded delivery, not all49 remaining Compat files.

Frozen inventory keeps original lines/hashes; file1 status advances to delivered
awaiting owner acceptance. S5/T100 remain open. Disposable logs are summarized
here and removed after the owned runner finishes; reusable caches remain.

Published softpc32.exe SHA-256: `1d5350eff04e64cf6b4cff2e2fd91552f6e18d99076e379b71129ec8ac76b628`.

Published softpc64.exe SHA-256: `75060d81f370be37a0c3e5535a54f2f9040e456a975d5fb7eb70fac370178380`.

## P4: file2 audio.h audit

Owner requests the next ordered file after accepting the P3 delivery. Baseline
de96a91e. audio.h is13 lines and has two declarations; both are necessary.

| Item | Actual consumer and disposition |
| --- | --- |
| Header guard | Retain: ordinary standalone declaration boundary. |
| types_interface.h | Retain: directly owns lib_status, with no mirror/Windows dependency. |
| softpc_platform_audio_start | driver.c vm_create starts the speaker after machine creation and before driver publication. Real integration and machine sound fixtures also call it. |
| softpc_platform_audio_shutdown | vm_create rollback and vm_destroy both check its result before releasing the machine/admission. Tests include failed join and repeated lifecycle. |

Do not delete/merge this header or relocate its declarations to mirror headers:
it separates host-worker lifecycle from original-device tone requests. Removing
it merely relocates declarations or forces callers to unrelated headers.
The tone callback declared by original nt_sound.c is a distinct device-host ABI;
there is no reason to add its mirror ULONG types to this neutral lifecycle header.

One comment is too broad: shutdown can join the worker successfully and then
fail stream destruction; in that case the worker is already NULL. Thus "A failed
destroy retains the live worker" is not true for every failure. Minimum proposal
is a two-line comment replacement: failure retains unreleased resources, and
the VM owner must retain admission until shutdown succeeds (+2/-2, no behavior
change). No production edit is made in this audit.

Consumer sweep found platform.c includes audio.h but uses neither declaration;
its Types dependency is already explicit on line1. Candidate: remove that one
include when reviewing platform.c (-1 line); do not mark that file reviewed now.
No other redundant direct inclusion established.

Read-only source/caller evidence, no new build/test. Existing P3 test evidence is
not represented as a new run. Source/test delta0; two of50 files reviewed,48
remain. Next fixed file: conapi.h.

## P5: audio.h ownership wording and unused include

Owner approves P4's two candidates and requests the usual full delivery. Baseline
7a743d5e. Correct audio.h's shutdown comment: failure retains unreleased
resources, not necessarily a live worker. Remove platform.c's unused audio.h
include; platform.c directly includes Types and calls neither audio lifecycle
entry. Do not advance platform.c's full-review status.

Production: two files +2/-3,net-1. No test changes or new tests: no executable
behavior changed. No mirror/public-package/API/schema/media/config changes.
The existing shutdown order remains task cancellation, PCM cancellation, join,
stream destruction and final resource release. Existing error returns remain.

Self-review compares the actual two hunks and all direct header consumers:
audio.c, driver.c, machine sound and native lifecycle tests retain required
includes. platform.c does not acquire a transitive dependency after removal.
Header comment covers both failed join and post-join stream-release failure.
No new abstractions, paths or error handling.

Verification: cached Ninja/ccache full builds and publication completed for
x86/x64. Each architecture passed188/188 background and8/8 native tests:
392/392 total, including desktop/audio/package and existing audio ownership
regressions. Runner exit0. Fresh package timestamps validated by post-build
check: x86 Nov15 2026 20:48:15; x64 Nov15 2026 20:51:13.

Each build emitted178 diagnostics, all from unchanged softpc.new mirror headers
(host_cpu.h endif labels and cpu4gen.h macro redefinitions); zero non-mirror
warnings and no build error. Mirror warnings are not cleaned in this task.
Documentation gate and actual-diff review passed. No new guest manual-test claim.
Scratch removed after runner termination; caches retained. File2 delivered for
owner acceptance,48 files still pending; platform.c remains pending full review.

P5 softpc32.exe SHA-256: `a13e4b002193c8e8898fe56b8cecdab9d001f20ebdf82fbe7eba2ebe925396f0`.

P5 softpc64.exe SHA-256: `034d4eafbdb53b044802d68d806cd4944e4dffbfd785985fbc5f1766a1799b1a`.

## P6: continuous ordered audit and first reduction batch

Owner authorizes autonomous, evidence-backed simplification of all Compat.
Baseline8ce56701. No mirror/shared-package/configuration/media/schema changes.
The following ordered source review records minimum responsibilities, not a
claim that every guest/platform scenario has been dynamically exercised.

| Order | File | Minimum responsibility and disposition |
| --- | --- | --- |
| 3 | conapi.h | Retain original nt_graph/nt_cga/nt_ega/nt_vga API redirection to the detached surface. Real host Console calls are not equivalent. |
| 4 | device_bop.c | Retain original BIOS table registration; consolidate the separate unsupported fast-BOP endpoint here. Test-only dispatch wrapper is a follow-up removal candidate. |
| 5 | dib_surface.c | Retain surface ownership, monochrome conversion, update transaction and body-ready generation. Delete unconsumed palette history; preserve existing trace output from the current palette. |
| 6 | dib_surface.h | Remove unused history declaration; retain actual original painter and machine interfaces. |
| 7 | edl_fast_bop.c | Merge unchanged failure behavior into device_bop.c and remove the source/build entry. Original CCPU still needs the symbol. |
| 8 | eoi.c | Retain original PIC/SB callback registration and delayed IRQ ownership; cancellation and serialized IDs are distinct obligations. |
| 9 | eoi.h | Retain declarations consumed by original PIC/SB and archive glue. |
| 10 | gfi_image.c | Retain raw-medium adapter under original FLA/GFI/FDC. Use existing 512-byte format invariant for buffers/checks; unify identical motor callbacks. |
| 11 | graphics_console_compat.c | Retain logical Console geometry and original painter adaptation; replacing it with the current presentation surface conflates distinct state. |
| 12 | hdd_media.c | Retain original fixed-disk transport and CHS addressing. Remove impossible failure rollback when initializing the explicitly empty second slot. |
| 13 | hdd_media.h | Retain narrow media attachment declarations used by platform and snapshot. |
| 14 | keyboard.c | Retain original keyboard callback table and key translation. Empty callbacks have different signatures and are not merged. |
| 15 | media_snapshot.c | Retain medium identity, overlay payload and transactional validation. Remove repeated path-null check after unconditional validation. |
| 16 | media_snapshot.h | Retain copied-media archive boundary used by machine snapshot. |
| 17 | memory.c | Retain SAS RAM lifetime and physical-access adapter; direct memory copying would bypass original SAS semantics. |
| 18 | mirror_types.h | Retain original insignia declarations and removal of its pre-ANSI const macro. |
| 19 | nt_graph.h | Retain selected single-executor graphics mutex/header adaptation; no second rendering owner. |
| 20 | parallel.c | Retain partial-write buffering and snapshot restrictions. Unify identical thresholds and remove branch with identical result; preserve direct-access state. |
| 21 | platform.c | Retain clock/pending-event distinction and joined lifecycle. Delete never-read interrupt counter; actual CCPU interrupt remains. |
| 22 | platform.h | Retain machine-facing lifecycle/media/video declarations; no independent implementations. |
| 23 | sb_midi.c | Retain native-output ownership, cancellation coordination and optional device-wide volume restoration. It is not PC speaker synthesis. |
| 24 | sb_midi.h | Retain original SB MIDI host-boundary declarations. |
| 25 | sb_pcm.c | Retain unsigned-8 to signed-16 conversion, gain and completion/cancellation adapter; raw Lib stream calls are not equivalent. |
| 26 | sb_pcm.h | Retain original DSP host-output declarations. |
| 27 | serial.c | Retain UART host endpoint and immediate byte-output semantics. COM/LPT merging would conflate different buffering, flush and archive contracts. |
| 28 | snapshot_stream.c | Retain explicit little-endian field encoding and callback failures; raw struct serialization is not an equivalent reduction. |
| 29 | snapshot_stream.h | Retain shared byte-stream contract for CPU/device/media archives. |
| 30 | v7_pointer.c | Retain original-controller pattern compositing, clipping and generation-checked save-under. Follow-up: replace restore's inner byte loop with existing memory copy per row. |
| 31 | video.c | Retain original font banks, underline/cursor state and presentation rebuild; no alternate video-device authority. |
| 32 | bios/host_def.h | Retain selected original-reset LIM header override; removing it changes original reset compilation. |
| 33 | ccpu/abi.h | Retain original accessor widths/signatures at the CCPU boundary; avoids leaking original CPU macros into driver. |
| 34 | ccpu/archive.c | Retain captured CPU/RAM/TLB/FPU state and explicit encoding. Follow-up: private field functions redundantly recheck nonnull embedded state after public validation. |
| 35 | ccpu/archive.h | Retain archive ownership used by machine save/load/checkpoint. |
| 36 | ccpu/cfpu_def.h | Retain native floating-point environment adaptation required by original FPU. |
| 37 | ccpu/facade.c | Retain original video installation and address/descriptor accessor glue; no duplicate CPU implementation. |
| 38 | ccpu/lifecycle.c | Retain original simulation-depth authority, TLS checkpoint and atomic exit handoff. No second depth counter to remove. |
| 39 | ccpu/lifecycle.h | Retain actual original CCPU and driver lifecycle declarations. |
| 40 | ccpu/snapshot.h | Retain fixed-width copied CPU state, distinct from padded native structs and on-disk encoding. |
| 41 | cmos/port.h | Retain fixed-disk topology query consumed by original CMOS. |
| 42 | cvidc/gdp_rule_access.h | Retain generated-rule address translation boundary for native pointer width. |
| 43 | cvidc/gdp_slots.h | Retain uniform generated GDP macro overrides; partial removal would restore fixed-base accesses for some fields. No new generator/framework. |
| 44 | cvidc/gdp_state.c | Retain stable separately allocated native slots and original-offset mapping to shared VGA fields. Reallocating a flat byte array would invalidate retained pointers. |
| 45 | cvidc/gdp_state.h | Retain GDP ownership and slot declarations consumed by original C-VID glue. |
| 46 | devices/archive.c | Retain callback IDs, ordered device restore and fixed-width field map. Follow-up: queue write traversal can use the read side's existing quick-then-tick traversal shape. |
| 47 | devices/archive.h | Retain opaque device archive ownership; do not expose legacy pointer-bearing state. |
| 48 | devices/snapshot.h | Retain explicit controller transfer records and archive hooks; native layouts are not portable snapshot bytes. |
| 49 | keymouse/cpu4.h | Retain original keyboard's interrupt-map accessor and selected reset bit boundary. |
| 50 | system/error.h | Retain original Base error-header lookup despite colliding NT header guard; no independent error framework. |

P6 source changes: +27/-70,net-43 and one source file removed. Build source list
-1 line; existing FDC fixture +7 lines checks every byte-sized N except2 returns
no-data before retaining normal format/readback. Each transfer/format stack
buffer falls from8192 to512 bytes. Removed palette storage is512 bytes; trace
sequence/output remains. No whole-program performance claim.

Proof: transfer already rejects N!=2; format's previous N<=6 then128<<N==512
admits only2. DMA count is16-bit, so ceil((count+1)/512) is always1..128.
The two GFI motor callbacks have identical signatures/body and only invocation
consumers. Empty HDD attach resets the slot and returns1 before I/O. Media path
is checked before copying. LPT thresholds are both1020. The interrupt counter
has no read. Repository search finds no palette-history consumer. Fast-BOP
symbol, argument, diagnostic and abort are unchanged after relocation.

P6 validation completed: cached x86/x64 builds and publication pass; each
architecture passes188 background plus8 native tests,392/392 first run, no
retries/skips. Background elapsed185.89s/190.11s; native24.58s/24.05s. Both
builds emit358 warnings, all unchanged mirror headers, zero non-mirror warnings.
Published timestamps: x86 Nov15 2026 20:59:59; x64 Nov15 2026 21:03:24.
Actual-diff self-review confirms the evidence above, no additional state or
production path, no weakening of meaningful I/O failure handling. Documentation
and whitespace checks pass. This does not claim new manual guest verification.

### Remaining bounded implementation plan

P7 will finish the four follow-up rows after P6 delivery. No second dispatcher,
test helper, archive interpreter or renderer will be added:

- device_bop.c: delete the test-only dispatch function. Existing bop_smoke and
  vga_frame_smoke invoke the original BIOS table directly; absent-selector tests
  assert the table entry is null. Guest dispatch remains in original CCPU.
- v7_pointer.c: replace the restore-only inner byte loop with one memory copy
  per clipped row; retain generation, dimensions, saved row stride and dirty
  rectangle. Paint's mask/XOR operation still requires its pixel loop.
- ccpu/archive.c: remove ten private embedded-state null checks. Public read/
  write validation dominates every sole call; all serialized-content checks and
  callback failures remain. Do not unify capture and restore order.
- devices/archive.c: queue writing uses the same quick-then-tick iteration
  shape already used when reading. Remove private map argument rechecks whose
  only calls pass embedded archive fields, static maps and validated callbacks.

Expected production reduction roughly35-45 lines, no added files, no schema/
mirror/shared change. Existing BOP/VGA/frame/queue/archive and transaction
regressions will run in the complete dual-architecture suite. The692 generated
GDP overrides were structurally checked: every definition is paired with its
undef and uses the sole slot allocator, with no exceptional body. Uniform ABI
mapping remains; removing unused names alone would leave mixed representations.

P6 softpc32.exe SHA-256: `5ee5049e057cec3dc61c2162a5ca62a5d589bd95abff02b8ad3dc3e245d378bb`.

P6 softpc64.exe SHA-256: `a18a2b79461fc72e187cc19f08abffd676aeba352e4b367482be2c679cd6c8d5`.

## P7: complete the bounded simplification plan

Baseline086a8cc2. P6 coordinator review confirmed the actual17 changed paths,
qualification and unchanged mirror/shared trees. P7 implements the four recorded
follow-ups; source +9/-47,net-38; existing tests +67/-69,net-2; no added file.

- BOP dispatch has no production caller. Delete its13 lines, leaving original
  BIOS registration and CCPU invocation. Two existing fixtures call BIOS entries
  directly and check missing entries as null. Existing result, register and
  memory assertions remain; no new test-only helper or production API.
- V7 clear copies each clipped row from saved background. Destination stride
  remains aligned surface width; source stride remains32. Left/top are clipped
  nonnegative, right/bottom are inclusive and bounded; source/destination are
  separate allocations. Generation and size guards still reject stale save-under.
  Existing VGA tests check movement, one-pixel bottom/right clipping, rebind,
  full-body repaint, enable transitions and monochrome compositing.
- All ten removed CPU null checks are private functions called exclusively with
  embedded fields of the publicly validated archive. Device map functions only
  receive embedded fields, static field tables and validated stream callbacks.
  Public argument validation, wire-content validation, I/O failure and allocation
  cleanup are unchanged. No broad policy of removing error handling is implied.
- Queue serialization retains quick entries followed by tick entries, with the
  same seven fields per entry. It now uses the already-existing read traversal
  shape. Current archive/checkpoint tests retain queue ordering, restore and
  truncated/corrupt image checks. No format conversion or asset edit is needed.

Similar-issue scan: deleted history/dispatch/counter/source-file names have no
remaining source/test/build references. GPU slot mappings, original no-op hooks,
clock wake/cancel semantics and explicit snapshot field maps remain necessary
boundaries. No evidence-backed mirror restoration was identified in this pass;
mirror diff is unchanged, not counted as a reduction.

All50 frozen paths now have source-review dispositions;49 files remain,8851
physical lines versus8957. Whole-S source delta against2464862d is +43/-149,
net-106. This is measured footprint reduction, not a claim of mathematical
minimality or new manual DOS/Windows guest acceptance. P7 qualification is complete.

### P7 qualification and final review

Cached dual builds and publication pass. Each architecture passes188 background
and8 native tests,392/392 first run, without retries/skips. Background elapsed
x86 160.14s/x64 180.79s; native24.08s/24.02s. Includes all public-component
units, gates, App units/integration, native audio, Console and both package cases.
Each build has89 warnings from unchanged mirror headers, zero non-mirror
warnings and no error. Fresh published timestamps: x86 Nov15 2026 21:09:42;
x64 Nov15 2026 21:12:34.

Executor actual-diff review: all removed checks are dominated by public entry
validation; failure returns and cleanup remain. V7 copy bounds and disjoint
buffers are proven above. BOP table registration assertions and actual handler
result assertions remain. Snapshot queue wire order and widths are unchanged.
Source/test LF, documentation governance and whitespace checks pass. Frozen
inventory reconciles exactly: only edl_fast_bop.c removed, no unlisted new file.
Shared packages, mirrors and user configuration/media/snapshots are unchanged.

Whole S5: production +43/-149 (net-106), tests +89/-86 (net+3), build wiring
+0/-1. The current continuous P6/P7 run accounts for net-81 production lines;
the earlier accepted P3/P5 account for25. No mirror diff reduction is claimed.
All identified safe candidates are delivered; further reductions have no proven
behavior-preserving design from this audit. Preserve the recorded minimum
responsibilities rather than replace them with a generic framework. S5 is ready
for owner acceptance; T100 remains open. Raw scratch is removed after the runner
exits, with only reusable Ninja/ccache roots retained.

P7 softpc32.exe SHA-256: `2726a0cc1adda0f68266e0a6b4245f38db138f1c270491f9daa031cba1dae7c8`.

P7 softpc64.exe SHA-256: `3b81d36cff48b27f05fd0dd1b07131d6ec67316e3836990ca1ee40b7f3586618`.
