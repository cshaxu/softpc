# M9: Neutral PCM Audio Stream

## Original Owner Request

> 同意，先建立中性的src/lib/audio test/lib/audio，完成功能补全，后续再引入xp的声卡功能；至少功能有了，就可以让nes原版引入了。请你帮我准入一个T任务实现这个，先做proposal，S1设计、切分任务并提交推送收口，我验证后再准入后续S任务构建。

## Scope And Baseline

SoftPC baseline is `2b17749a` (accepted T80); the worktree was clean.
Admission is T81, S1 design only. CURRENT owns execution status.
No production code, tests, artifacts, INI or media change in S1.

The present product has PIT/PPI PC-speaker tone output through
`core/softpc.new/host/src/nt_sound.c` and `core/compat/audio.c` (Beep).
It has no general PCM stream or attached Sound Blaster implementation.
XP's nt_sb/nt_sbdsp/nt_sbfm/nt_sbmid are research references only: they
depend on NTVDM/WinMM and do not establish standalone device compatibility.

MyNES `b48e57f` is the owner-approved canonical source import for revised S7. It has
the same shared-corpus provenance and no runtime/build dependency is created:
SoftPC copies the checked-in source and tests, then proves their hashes. Its
private FIFO, worker and explicit finite-tail flush replace SoftPC's uncommitted
S7 implementation; platform failure remains terminal and Linux remains an
honest unsupported leaf.

## Product Result And Non-goals

Provide a reusable PCM playback stream in `src/lib/audio`, covered by
`test/lib/audio`. NES can import the canonical Lib/Common source/test packages
unchanged and connect its existing sample producer through its own adapter.
This is source portability, not binary compatibility with NNES's provisional
audio API; its adapter may need a narrow call-site/sample-type adjustment.

No guest instruction timing, APU, sound-card registers, DMA, IRQ, OPL/MIDI
synthesis, decoding, mixing, volume UI, resampling or device enumeration.
No Common/Machine audio transport, KVM audio frame or audio executor.
No changes to App, Common, x86, snapshots or configuration. S2--S4 do not
change Core or the existing speaker; S5 alone changes the Compat presentation
endpoint while preserving the original speaker state machine and mirror.
Windows playback is required. Linux is an honest UNSUPPORTED placeholder,
not a sleep-and-discard success path; real Linux playback is a separate task.
Win3.x/Win95 sound-card playback is explicitly not this T's exit criterion.

## Ownership And Structure

- Audio owns a stream's copied samples, output handle, fixed buffer lifetime,
  validation, native submission/reset/close and their status translation.
- Types owns only scalar and external SDK declaration wrappers. Add `lib_i16`
  and the necessary `types/win32/audio.h` declarations, not audio policy.
- Audio may depend on Types and public Base synchronization if actually needed;
  it cannot import another component's platform implementation. WinMM linkage
  belongs to Audio, never Types or all consumers.
- Root `audio/stream_interface.h` is the public boundary; `stream.h` is private.
  Platform implementations in `audio/win32` and `audio/linux` expose the same
  private function shapes. No native/private/internal suffix or wrapper object
  whose only purpose is owning another pointer.
- Audio owns one bounded copied PCM FIFO and one output worker per stream. The
  worker is the only caller of the selected platform leaf; this is the one
  Audio delivery path, not a product mailbox or a second producer. The native
  completion callback only signals Audio-private readiness; it never invokes
  product code. `wait_writable` exposes FIFO admission as a neutral blocking
  operation, so callers need not poll. `cancel_wait` is the sole concurrent
  operation and only interrupts that wait. Host playback proceeds independently.
- Caller serializes all operations on one stream, including destruction, except
  `cancel_wait` may wake one concurrent `wait_writable`.
  Separate streams are independent. Producer/control synchronization belongs
  to the caller; native completion does not invoke caller code.

## Proposed Public Contract

Use `lib_status` throughout and opaque `lib_audio_stream`. Provisional names
below are finalized in S2 without adding redundant aliases.

| Operation | Contract |
| --- | --- |
| create(options, out_stream) | Validate format, open native output and prepare fixed storage; success means ready, failure leaves output null. Initially active. |
| enqueue(stream, samples, frame_count, out_accepted_frames) | Copy an ordered prefix without waiting for playback; accepted count is authoritative. Never retain caller memory. |
| query(stream, out_queued_frames, out_writable_frames) | Report copied FIFO occupancy and immediately writable FIFO capacity; not a hardware sample-position or latency clock. |
| wait_writable(stream) | Block until the copied FIFO accepts at least one frame; a successful `cancel_wait` wakes it with INVALID_STATE rather than modifying PCM. |
| cancel_wait(stream) | The sole operation permitted concurrently with `wait_writable`; it wakes that wait without changing queued PCM. |
| set_active(stream, active) | False discards queued audio using the same reset path as clear; true enables new submissions. Inactive enqueue returns INVALID_STATE. Repeated calls are idempotent. |
| clear(stream) | Discard all pending native blocks, retaining active/inactive state. No old queued block can be submitted after successful return. |
| destroy(inout_stream) | Reset, unprepare, close and release; success nulls pointer, null is idempotent. Failure retains owned resources for safe cleanup, never frees buffers still owned by the OS. |

PCM uses interleaved signed 16-bit samples (`lib_i16`); a frame contains one
sample per channel. Mono and stereo; initial supported rates 22050, 44100 and
48000 Hz. No implicit conversion. Unsupported formats/devices fail explicitly;
invalid counts/pointers and size multiplication overflow are rejected before
modification. Mono 48 kHz serves NES without imposing NES concepts on Lib.

Audio accepts copied ordered prefixes into one fixed private FIFO. Its worker
submits practical private batches to the four-slot WinMM leaf; the producer sees
FIFO admission, never slot accounting. `flush` is the explicit finite-sound
boundary: it delivers a remaining sub-batch tail in order before returning.
`clear` is the explicit discard boundary. No dynamic growth or caller-memory
retention exists.

Full capacity returns LIMIT_EXCEEDED with zero accepted. Partial capacity
accepts a prefix and returns OK. If native submission fails after earlier
blocks succeeded, return the failure plus that accepted prefix, never invite
replay of it. Zero-length enqueue is a no-op with zero accepted (null data
allowed only for zero length). Invalid arguments accept nothing.

Submission/reset/wait/cleanup errors are never translated into successful
silence. An output failure closes new admission and remains observable through
query/enqueue until destruction; no automatic reopen/retry. Cleanup can still
be attempted. OS playback completion means buffer ownership returned, not
proof that a physical speaker was audible. Successful clear cannot retract
samples already heard or eliminate downstream hardware latency.

Win32 uses waveOut with fixed prepared buffers and no application callback.
Native completion flags govern reuse. Reset/close ordering and partial prepare
failure must be proven with an injected backend and a real native smoke.
No unchecked reuse/unprepare/free while a block is still submitted.
If the bounded FIFO/worker design cannot meet those contracts, stop and revise
the design with the owner rather than adding a second queue or hidden thread.

## Ordered S Tasks And Estimated Diffs

Estimates are planning bands, not quotas. Report actual tracked additions,
deletions and net separately for production, tests/build and docs at each S.

| S | Work and boundary | Expected change | Exit proof |
| --- | --- | --- | --- |
| S1 | This design, task split, admission and design review only | 3-4 docs; code +0/-0; docs about +150..220/-5..15 | Governance gate, original-request and actual-diff review, push; stop for owner |
| S2 | Public/private stream contract, shared validation, fake backend tests, C11 build/DAG/manifest integration | 8-12 source/build/test files; production +140..240/-0..10; tests/build +180..300/-0..15 | All contract cases below deterministic on x86/x64; incomplete native implementation explicitly unsupported; no product hookup |
| S3 | Win32 fixed-slot playback, Types declarations, Linux placeholder and native/failure tests | 6-10 files; production +180..300/-0..30; tests/build +150..250/-0..15 | Actual PCM playback, capacity/reuse/reset/cleanup proofs; native failures observable; no polling worker |
| S4 | Four-package isolated copy acceptance, remaining lifecycle tests, docs and release artifacts | 4-8 files; production +0..30/-0..30; tests/build +60..140/-0..20 | Strict dual C11, isolated Lib/Common and full background regressions, unchanged copy hashes |
| S5 | Use the finished stream at the existing SoftPC PC-speaker presentation boundary | 3-6 Core Compat/test/build files; production +100..180/-40..90; tests/build +80..160/-0..20 | Win3.1/DOS PC-speaker handoff, state/clear/shutdown proof and audible owner test |
| S6 | Close the Common Session Window-creation admission gap: a missing Window may be created only after a RUNNING completion; paused state may retain but never synthesize a Window | 2-3 Common/test/docs files; production +2..8/-0..4; tests +20..50/-0..10 | State matrix proves INIT, STOPPED, RESET_COMPLETED/PAUSED and ERROR never create a missing Window for either display mode; existing paused Window retention remains unchanged |
| S7 | Owner-revised before an executor commit: adopt the MyNES `b48e57f` Audio source/test corpus and reconnect the one SoftPC Compat PC-speaker producer through its FIFO/flush contract | 10-14 Lib/Core/test/build/docs files; production +20..140/-80..220; tests +20..120/-20..140 | Exact shared-corpus code hash comparison except the documented upstream-stale README and derived manifests, finite-tail/failure proof, dual-width strict C11/background regression and audible `AUDIO.COM` acceptance |
| S8 | Investigate and repair owner-observed first-use PC-speaker silence/discontinuity after S7; retain one Compat producer and one Audio worker | 2-5 Core/test/docs files; production +5..50/-0..30; tests +30..120/-0..20 | Deterministic fresh/repeated/reset tone handoff proof, dual-width strict C11/background regression and package artifacts |

The owner has approved automatic sequential admission of S2--S5. Each
code-changing S builds x86/x64 EXEs, runs focused and
background regressions, refreshes only the two package EXEs, commits/pushes
and waits for the owner. The EXEs do not gain a new guest sound card; a small
standalone audio test executable under ignored build space demonstrates PCM.
S5 is the only task that alters product code. It replaces neither guest PIT/PPI
nor the original `nt_sound.c` state machine: that code continues to issue the
existing frequency/stop request, and Core Compat synthesizes a bounded PCM
square-wave block. It retains one Compat worker because a sustained speaker
tone needs a product-time source to generate future samples; Audio's separate
worker owns only host PCM delivery.
Frequency zero, gate-off and shutdown clear unsounded native PCM. No sound card,
DMA, IRQ, OPL/MIDI or guest multimedia route is added. An audible test must be
explicit, bounded and separate from silent CI tests.

## Finite Acceptance Ledger

Frozen coverage unit is each row below, not an assertion about all audio APIs.
Each row must have a passing test/evidence or an owner-approved scope revision.

| Requirement | Owner / proof receiver |
| --- | --- |
| Exact component/test placement, no product/native dependency leaks | S2 DAG/manifest and static negative tests |
| PCM units, mono/stereo/rates, overflow, null, zero and short input | S2 deterministic contract tests |
| Bounded capacity, prefix acceptance, order, copied lifetime, completion reuse | S2/S3 fake plus native tests |
| Inactive rejection, idempotence, clear of native pending data | S3 deterministic submission/reset ordering and native smoke |
| Open/prepare/write/reset/unprepare/close failure and partial ownership | S3 injected failures; no successful silent fallback |
| One Audio FIFO/worker, no product-side platform path or audio-driven machine pacing | S7 changed-path ownership audit |
| Linux honest placeholder with identical private ABI | S3 compile/check and explicit UNSUPPORTED test where runnable |
| Independent four-directory import, strict C11 on both widths | S4 isolated copy/build/test; no sibling or product paths |
| Existing SoftPC behavior retained, both deliverable EXEs | Each implementation S background regression and artifact hashes |
| Audible PCM, caller reuse instructions, known platform limits | S4 bounded native tone test and owner acceptance |
| SoftPC PC Speaker through the completed Lib stream | S5 Core state proof plus Win3.1/DOS owner acceptance |
| Non-running presentation never synthesizes a missing Window | S6 Common Session state matrix on both display modes |
| No periodic PCM readiness polling; completion wakes a bounded PC Speaker producer | S7 native completion and Compat worker proofs plus owner audible test |

All six shared manifests and dependency gates must remain valid. Use the
existing background presets (exclude desktop); do not pop windows or request
interactive input for automated tests. Test fixtures are generated PCM, not
guest media or copyrighted music. Do not alter user INI or assets/media.
Linux compile/runtime and downstream NES integration are reported separately;
Windows tests alone cannot establish either. S4 copy proof means importability,
not a claim of already running the downstream application.

## S7 Revised Canonical-Import Record

The canonical comparison baseline is MyNES `b48e57f`. Audio C/H/platform/build
paths and Audio C tests are copied verbatim; only each corpus manifest is
derived for this repository. SoftPC deliberately retains its accurate Audio
README rather than importing MyNES's stale calls-based completed-slot wording:
the canonical implementation has one copied FIFO and one delivery worker.

Compat remains the sole SoftPC producer and uses only the neutral stream API:
enqueue, FIFO-writable wait/cancellation, clear and lifecycle. It neither sees
WinMM vocabulary nor owns delivery. Tone gate-off clears unsounded PCM rather
than flushing it, preserving the existing immediate-stop product semantics;
`flush` remains available to finite PCM producers. Original `nt_sound.c` is
unchanged.

Verification is dual-width strict C11/package builds, focused fake/native Audio
and Compat failure suites, full background regression, component DAG, manifest
and documentation gates. The remaining acceptance is the owner's audible
`AUDIO.COM` check; no user INI or media is tracked by this task.

## S8 First-Tone Lifecycle Repair

P1 restored eager neutral-stream creation before the speaker task and removed
the lazy-create mutex wrapper. Its lifecycle proof passed, but owner testing
reproduced the first-run silence unchanged; that initialization race was not
the causal defect and P1 is retained only as a tested baseline.

P2 follows the unmodified original sound contract instead. `LazyBeep(CLICK,
1)` deliberately invokes its native Beep endpoint before resetting its cache;
the original PPI calibration uses these short requests until it derives a
stable continuous frequency. Compat incorrectly converted every sub-10ms
request to silence. It now transfers one coherent frequency/duration request
to its sole speaker producer. Infinite requests retain bounded PCM generation;
finite requests generate their exact PCM duration and use the existing neutral
Audio `flush` to deliver an otherwise sub-batch tail. A short mutex protects
only the two-field cross-thread handoff; it is not an output gate, retry state
or second producer. The original mirror, public Audio ABI, MyNES-shared Audio
corpus and native worker stay unchanged.

The focused fake proof covers a 100Hz, 1ms request becoming one 48-frame PCM
submission followed by one flush. Background regression passes x64 114/114
(151.94 s) and x86 114/114 (154.18 s); both package EXEs are refreshed.
Owner-visible audible confirmation remains required. User media stays
untracked.

P3 removes the standalone-only PPI speaker-data helpers from the preserved
`nt_sound.c`.  Standalone PPI writes now select the original NTVDM complete
`HostPpiState(value)` transition after the existing timer-gate operation.  The
immediate gate-only update remains necessary for an unchanged PPI speaker-data
bit and is retained as Compat host support; it is no longer a mirror-local
implementation.  The reset stop callback likewise lives in Compat.  This is a
net OpenNT mirror reduction: the PPI selection/snapshot adjustment is `+8/-6`,
while `nt_sound.c` is `+0/-30` relative to the preceding accepted baseline.
Focused state, machine/PPI and Audio failure tests pass on both widths.  The
owner still performs the audible fresh-run confirmation before S8 closes.

P4 corrected the cold-reset Timer2 gate to match the original PPI reset state;
it remains a valid initialization invariant, but the owner reproduced silence
afterward, so it is not the first-use playback cause.  The resulting trace of
the owner's first `AUDIO.COM` run proves the complete existing path: Timer2/PPI
produces a 439Hz request, Compat's sole worker produces PCM, the Audio FIFO
accepts it, and the first `waveOutWrite` succeeds.  This rules out a second
producer, guest timing, and a missed gate transition as the remaining cause.

P6 first established that a neutral Win32 endpoint requires an initial reset,
but placed it before header preparation.  The owner then reported that a cold
boot was still silent while a later warm reset worked, proving that this was
not equivalent to the existing `clear` path.  P7 moves the same reset to after
all headers are prepared and before the stream is published, exactly matching
the reset state MyNES establishes after its completed stream creation.
Later `AUDIO.COM` executions already follow a prior `clear/reset`, which
explains the owner-observed first-versus-later distinction.  This is a Lib
endpoint invariant, not a SoftPC first-tone exception: no public API, worker,
polling loop, guest timing, or preserved-mirror source changes.  The Win32
fake backend proves create/reset failure cleanup and normal initial reset; the
real threaded Compat proof covers a fresh first request reaching its first PCM
submission on both widths.  Physical audibility remains an owner desktop
check because managed automation has no usable `waveOut` device.

T closure requires separate original-request/ledger/changed-path audit and
owner acceptance. XP sound-card implementation remains future separately
admitted work; it is not silently folded into the existing mirror-rebase queue.
