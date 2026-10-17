# XP Sound Blaster Standalone Capability

## Owner request and admission boundary

The owner accepts T92 and admits T93/S1 on 2026-10-15. This task receives the
sound capability separated from the [XP core migration](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md).
[CURRENT](../states/CURRENT.md) alone owns admission and completion status.

Latest requests retained verbatim:

> 我只需要你匹配xp的方案就可以了，我在乎的是代码吻合度，不在乎是不是真的一定要听到声音

> 明白了，那你把XP实际用到的PCM、MIDI、FM宿主接口逐项核对清楚，明确lib audio需要补什么、compat负责什么，更新S1设计和后续S任务划分。听起来，如果这个项目做起来，我们会需要给lib audio改名成 lib/audio/pcm并提供额外PCM接口，而MIDI和FM同理。请你相应调研和规划S任务划分。

This supersedes the earlier software-MIDI choice, software-OPL recommendation,
instrument-bank proposal and S2-S4 budgets. No YMFM, Nuked-OPL3, TinySoundFont
or SoundFont was imported. They are not pending implementation prerequisites.
There is no new C++ target, software synthesizer or mandatory instrument asset.
Host MIDI restoration may be approximate. FM silence on a host without the
necessary capability is explicitly acceptable, as in XP's fallback.

## T93 S1 admission and design brief

Design only: audit pinned XP host calls, current Lib contracts and receiving
owners; revise the serial plan and proof requirements. No production/test
edits, EXE refresh, INI/media changes or claimed playback result in S1.
Current source baseline is accepted T92, HEAD at admission 6ac9bf0b9.
The original design proposed S2-S8. The owner's subsequent shared-quality
insertions below expand the remaining sequence through S15; CURRENT alone
records which step is active or completed.

This revision replaces contradictory draft designs rather than appending a
second active design. Preserve source findings, but do not treat previous
software-synthesis estimates as current commitments. S1's exit requires owner
review of this shared-interface and host-fidelity scope, a complete document
gate and executor delivery followed by actual-change review.

## Frozen audit universe and source evidence

XP revision: a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Paths below are relative to Source/XPSP1/NT/base/mvdm/softpc.new/.

| XP path | Reference lines | Git blob |
| --- | ---: | --- |
| base/inc/sndblst.h | 205 | 274e295145f6dbb3ef08c3318e0ecac6cb2cbf70 |
| host/inc/nt_sb.h | 351 | 8da47b0b061cce84d6ad8fb23c5710e673ad716d |
| host/src/nt_sb.c | 988 | af611f75f3a0702c46d3b66a554c4983176a23b7 |
| host/src/nt_sbdsp.c | 2962 | 2ef226261fef08f9d406a79f8877cc3490bb19f6 |
| host/src/nt_sbfm.c | 366 | 9b312c2e9a06def7d5de4b5142c1a57137a1f3a0 |
| host/src/nt_sbmid.c | 896 | 4e47c9ead86e9c0fd5728eb7ed455f4c47cbda6c |
| host/src/nt_sbmxr.c | 329 | 2ae09eb5a801dc5044bee178f1be426f9f5f1510 |

The total 6,097 lines is a reference census, not the import or local-code budget.
Additional read-only references at the same revision are
Source/XPSP1/NT/base/ntos/vdm/i386/vdmentry.c and
Source/XPSP1/NT/base/ntos/ke/i386/vdm.c for VdmAdlibEmulation.
Kernel source is not an admitted copy payload.

Finite coverage units are the 24 dynamically loaded WinMM names in
nt_sb.c:235-259, the FM native operations below, and each existing Lib stream
operation. Every unit is assigned to reuse, necessary Lib extension, Compat
translation, device owner or unused/unavailable exclusion. Completion of this
source ledger does not certify implementation or guest-driver playback.

Reproduce the census with pinned git show and git grep over the seven paths;
search each Proc invocation as well as declarations/loading. A whole
softpc.new search confirms no PauseProc/RestartProc invocations.
OpenWaveDevice has three callers, all with zero callback; OpenMidiDevice has
two callers, both with zero event. Do not infer WOM_DONE/MOM_DONE requirements
from unused prototypes. Dynamic loading before WOW32 is NTVDM host policy,
not a reason to duplicate a dynamic loader in Compat.

### PCM: every loaded WinMM entry

| XP entry | Actual use | Disposition |
| --- | --- | --- |
| waveOutGetNumDevs | FindWaveDevice enumerates outputs | Lib selects the host endpoint; no SB device enumeration in Compat. |
| waveOutGetDevCapsA | Requires 8-bit mono 11025/44100 format flags | Replace historical bitmap selection with actual PCM format admission; no WAVEOUTCAPS in public types. |
| waveOutOpen | OpenWaveDevice; TestWaveFormat uses WAVE_FORMAT_QUERY | PCM create/admission at requested rate; Compat translates the original host contract. |
| waveOutPrepareHeader | PrepareHeaders prepares each WAVEHDR | Copied PCM submission removes borrowed-native-header ownership; not a public API. |
| waveOutWrite | WriteBurst submits an 8-bit mono burst | Existing bounded copied enqueue, with Compat's lossless u8-to-i16 conversion. |
| waveOutGetPosition | GetWaveOutPosition requests TIME_SAMPLES; DSP/DMA consume it | New stream progress query in input frames; never substitute FIFO length or accepted frames. |
| waveOutUnprepareHeader | Waits while WAVERR_STILLPLAYING | No native header crosses Lib; copied-source storage can be reused after acceptance, playback completion remains separate. |
| waveOutReset | Discards queued wave data and rebases position | Reuse clear, explicitly reset the new progress epoch and cancel outstanding ranges. |
| waveOutClose | Closes after idle/header cleanup | Reuse destroy; distinguish delivery/drain from cancellation, no Sleep(1) polling. |
| waveOutGetVolume | Original/previous volume bookkeeping | Stream-local gain query if needed by retained adapter; no machine-wide volume ownership. |
| waveOutSetVolume | Speaker mute and mixer/master/voice paths | Stream-local gain set; register interpretation stays in the XP device. |
| waveOutPause | Loaded, not called | Do not add API solely for this unused entry. |
| waveOutRestart | Loaded, not called | Same; DSP PauseDMA/ContinueDMA are device state, not these functions. |

nt_sbdsp.c:GetSamplingRate retains time-constant decoding, clamps and XP's
standard-rate normalization. It can request 8000/11025 and nonstandard rates;
the existing three-rate allowlist is insufficient. SetWaveFormat's search
over up to 50,000 adjacent rates is an old host workaround, not a new public
Lib requirement. Request the decoded rate explicitly; return unsupported
rather than silently changing speed if the backend cannot serve it.

### MIDI: every loaded WinMM entry

| XP entry | Actual use | Disposition |
| --- | --- | --- |
| midiOutGetNumDevs | FindFMSynthDevice, not ordinary MIDI mapper selection | Host FM capability research; MIDI create uses the host default. |
| midiOutGetDevCapsA | Checks MOD_FMSYNTH for FM | A MIDI device type alone does not prove raw OPL access. |
| midiOutOpen | HMidiOut via MIDIMAPPER; HFM via selected FM device | Lib MIDI owns its output handle; FM cannot borrow MIDI internals. |
| midiOutShortMsg | Complete messages of up to three bytes | New MIDI copied short-message operation, explicit length. |
| midiOutPrepareHeader | Prepares four native long-message buffers | Private Lib MIDI native ownership, no public MIDIHDR. |
| midiOutLongMsg | Submits buffered long data | Copied bounded SysEx submission; validate boundaries, not an arbitrary byte-stream transport. |
| midiOutUnprepareHeader | Releases native long-message ownership | Private completion/reset cleanup. Never free while the driver owns it. |
| midiOutReset | Turns off notes and returns pending buffers | New reset/cancel operation; reset before waiting for returned buffers. |
| midiOutClose | Releases HMidiOut/HFM | Lib destroy after completion/ownership release; no five-second polling loop. |
| midiOutGetVolume | Saves/restores volume | Optional host-handle volume capability, explicit unsupported result. |
| midiOutSetVolume | FM/MIDI mixer output | Host-handle volume when supported; not MIDI CC substitution or global endpoint control. |

XP ResetMidiDevice waits on MHDR_INQUEUE before reset, using Sleep(5000).
Do not reproduce this host cleanup defect. Native completion is asynchronous
or synchronous depending on the driver; Lib owns copied buffers through both.
XP's short-message and long-message packing, running status and SysEx/realtime
handling need owner-local tests; do not copy the ULONG pointer arithmetic or
the LastCommand tail omission into a neutral library.

The GetVolumeProc/GetMidiVolumeProc conditionals in the wrapper setters and
closers treat a nonzero error as success. MMRESULT success is zero. This is a
confirmed source defect, not evidence that volume bookkeeping works in XP.
All imported uses need explicit status checks. Mixer zero underflow and
independent master/voice overwrite remain device-local fixes, not Lib policy.

### FM: actual native service, not generic MIDI synthesis

| XP operation/path | Meaning | Planned boundary |
| --- | --- | --- |
| FindFMSynthDevice + midiOutOpen | Reserves a MOD_FMSYNTH output | Insufficient by itself for standalone raw-register access. |
| NtVdmControl(VdmAdlibEmulation, ADLIB_DIRECT_IO) | Grants guest direct FM port handling | NTVDM-specific; unavailable to the standalone x64 process, not imported. |
| Ki386AdlibDirectIo | READ_PORT_UCHAR/WRITE_PORT_UCHAR | Requires privileged/hardware support, not WASAPI or generic WinMM MIDI. |
| FMPortWrite / WriteFile of SYNTH_DATA | Historical native FM port batch | No casting HMIDIOUT into a modern file handle; only a verified raw-FM backend could implement this capability. |
| FMStatusRead / ReadFile block | ReadFile block is under #if 0 | Do not build an API merely to preserve disabled code. |
| ADLIB_KERNEL_EMULATION / Ki386AdlibEmulation | Stores index/status; simulates immediate timer flags, emits no samples | Preserve XP-compatible no-host behavior in the one device owner, not in Lib. |
| ADLIB_USER_EMULATION / CloseFMDevice | Returns handling to user-mode emulation and releases host resources | Ordinary device/host lifecycle; no NTVDM dispatcher. |
| ResetFM | Clears selected register, batch position and status | Guest device reset, not an OS audio operation. |

Ki386AdlibEmulation explicitly does not output data. It starts with status 0x06
and models immediate flag changes, not a clock-accurate OPL engine. Its outer
condition includes registers 0xB0..0xBD as well as 0x04, whereas nt_sbfm.c's
user-mode status update is nested under 0x04. These are not identical handlers.
S8 must record which XP branch each test represents and retain one standalone
device handler; do not merge both into independently maintained simulations.
The selected no-host acceptance reference is the active XP kernel fallback,
including its observable index/status behavior; translating that requirement
does not admit copying the NT kernel implementation.

Read-only native enumeration on this host found one Microsoft GS Wavetable
Synth (technology 7) and no MOD_FMSYNTH (technology 4). No output was opened.
This is neither an audible test nor proof about every possible driver.

Decision: define the FM capability boundary honestly. The current Windows
standalone backend has no verified raw-FM transport; create reports
UNSUPPORTED with a null handle. Linux likewise remains explicitly unsupported.
No fake successful write/reset, software synth, privileged driver, plugin
registry or MIDI-message translation is added. A future real FM backend needs
separate admission. T93 can qualify XP's no-FM-host behavior, not physical FM
playback. Capability absence is not a machine error; a failure of an already
opened output is not silently relabelled as normal absence.

## Proposed Lib Audio layout and contract

Keep audio as the organizational group; its leaves are independent capabilities:

```text
src/lib/audio/
  pcm/{stream_interface.h,stream.h,stream.c,win32/stream.c,linux/stream.c}
  midi/{output_interface.h,output.h,output.c,win32/output.c,linux/output.c}
  fm/{output_interface.h,win32/output.c,linux/output.c}
test/lib/audio/
  pcm/...
  midi/...
  fm/...
```

Names/targets follow audio-pcm, audio-midi, audio-fm and lib_audio_pcm_*,
lib_audio_midi_*, lib_audio_fm_*; private prefixes and guards follow ownership.
S2 uses git mv, updates all callers, tests, CMake, verifier and manifests;
no permanent old-name aliases or forwarding headers. Existing PCM comprises
five files / 793 lines; relocation is not 793 lines of new logic.
Types continues to own all SDK declarations; native code lives only in each
leaf's platform directory. Lib tests do not import SoftPC or another suite.
No audio manager or common abstract stream vtable is justified.

### PCM additions, reuse and excluded APIs

Retain the bounded copied-i16 FIFO, 512-frame submission cap and WASAPI backend.
Compat's 8-bit conversion is exact: sample 0/128/255 becomes -32768/0/32512.
There is no reason to expand Lib's sample representation just for this device.
Lib must accept validated source rates beyond 22050/44100/48000, including
every XP-decoded rate (roughly 3921..44100), without altering playback speed.
Use WASAPI's existing conversion flags; actual native admission determines
support. Invalid sizes/arithmetic return invalid argument; unavailable format
is unsupported and endpoint failure is I/O error, not successful silence.
Creation must report native format/open failure before claiming readiness;
reuse the existing worker rendezvous rather than introducing a second thread.

Add a copied progress result with an epoch and 64-bit accepted, submitted and
consumed input-frame counters. FIFO query keeps its present capacity meaning.
Only accepted prefixes advance accepted; only native submission advances
submitted. Consumed means host audio-engine consumption, not proof of physical
speaker audibility. WASAPI padding is expressed in the client's frame units.
A proposed implementation uses native submitted frames minus current padding,
excluding the existing initial-silence priming, clamped to valid submitted
input. Underrun cannot advance consumed past submitted. Clear cancels rather
than completes old ranges and changes epoch; failure must remain observable.
This is a design backed by API semantics, not a tested progress implementation.

Compat uses an accepted burst's end-frame position to determine completion;
do not add both completion tokens and a redundant WAVEHDR state machine.
Native progress must continue to be observed for the final outstanding tail,
even after the producer FIFO becomes empty. Extend the existing worker's
native-event/control wait; do not add a polling observer or guest thread.
A neutral progress/failure wake sink may notify the existing executor, without
mutating guest state or synchronously reentering the stream. Detach/quiesce
the sink before destroying its context. Coalesce notifications; frames are
not queued as one event per sample.

Keep flush explicitly meaning host submission, not playback completion.
Add no public prepare/unprepare and no unused WinMM callback/flag replicas.
A caller requiring drain uses progress to its accepted endpoint plus event
notification; clear/destroy perform cancellation and release ownership.
New per-stream gain uses a neutral bounded value, with zero truly silent and
full-scale unchanged. Use a per-stream backend control, never system volume.
Do not silently turn existing set_active(false), which discards output, into
a queue-preserving pause.

One actual current gap is the 1024-frame worker batch gate: a 512-frame-or-less
burst can remain queued until more input or flush. S3 must let the existing
worker submit an available bounded prefix, without waiting for a future guest
command. There is still one FIFO and one submission path. Preserve and rerun
all accepted first-tone PC-speaker proofs; no workaround padding in Compat.

### MIDI additions and limits

New opaque output owns the native default output, bounded copied SysEx buffers,
completion notification and reset/destruction. Proposed operations: create,
send_short, send_sysex, query_capacity, wait_writable/cancel_wait, reset,
optional get/set volume, destroy. No timestamps, guest ports, DMA or synth
state in this interface. Short data carries explicit length; long submission
accepts a complete bounded message or returns capacity/unsupported status,
never silently truncates. Keep ordering across the two forms; explicitly test
short-after-long and realtime delivery. Native headers remain private.

Use WinMM MIDI with native completion events. Initial bounded design is four
long-message slots, maximum 4096 bytes per complete SysEx; this is an explicit
support bound, not a claim of arbitrary-size SysEx support. A larger guest
message is consumed by the device parser to its end and diagnosed as unsupported,
not emitted as a valid truncated command. No unbounded allocation/backlog.
The device parser owns running status and incomplete guest messages; Lib
validates complete outgoing messages, not a second guest MIDI parser.
Capacity notification must drain accepted complete device output even if no
further guest byte arrives; never wait for the next command to flush the last.

Reset stops notes, returns outstanding buffers, and permits safe reclamation.
Account for incomplete external SysEx: reset alone does not send EOX; terminate
an in-flight message when needed and propagate actual output failures.
Completion callbacks only signal owned synchronization, never call WinMM
recursively or guest code. No automatic retry/reopen after terminal I/O error.
No-device/unsupported volume is explicit. Native MIDI sound and instruments
depend on the host; Lib does not promise a uniform sound bank.

### FM additions and limits

Do not build an empty clone of all MIDI/PCM operations for symmetry.
The first concrete public operation is capability acquisition with an opaque
FM output; unsupported platform implementations return UNSUPPORTED/null.
Only admit register-batch/status/reset operations with a real backend and
documented transport, not a dormant generic plugin scheme. S4 tests the
unsupported acquisition contract and inward dependencies. S8 handles the
fallback at the XP device owner, not inside this Lib capability.

Thus the three directories are symmetric in responsibility (host capability),
not necessarily in function count. PCM and MIDI supply working transports;
FM names a truthful capability boundary without claiming that current Windows
provides raw OPL output.

## Compat, device and execution ownership

- XP-derived device files own DSP parser/registers, mixer register values,
  MPU byte parser and FM index/status. They retain original paths/notices and
  selected-source accounting. Compat does not duplicate these state machines.
- Compat owns Lib stream/output handles, u8-to-i16 conversion, copied
  submission/progress translation, host capability admission and cancellation.
  PCM frames remain at their device-selected rate; no private resampler.
- Original DMA/PIC remain the only controller owners. Host position is an
  input to the XP-derived transfer policy, not authority for Lib to change
  guest RAM/count/address or assert IRQ.
- The existing executor services device work. XP's DspThreadEntry, native
  locks/semaphores and direct cross-thread DMA mutations cannot be imported
  as a second machine executor. Replace their scheduling at the host seam,
  not the DSP command state machine.
- Reuse Compat platform's executor-wake ownership for copied progress/failure
  notification and original delayed-interrupt obligations. No fixed Sleep,
  perpetual sound quick-event or faster PIT/video heartbeat.
- Do not combine the abandoned software-synthesis timeline with XP's playback
  position model. Preserve observable transfer/readback/EOI contracts and
  separately account for unavoidable scheduling differences. Lib progress is
  not enqueue success. Original small-block and named-game hacks are not new
  Lib policies; S6 must disposition them against the one device transfer path.
- PC-speaker remains on its accepted PCM stream/onset path; OS mixing handles
  simultaneous streams. Do not invent a new software mixer for PCM plus MIDI.
- Mixer decoding/zero gain/master-channel combination belongs to the device.
  Compat forwards the resulting host gain; optional MIDI volume failures
  remain explicit and do not change guest register readback.

Ordinary/debug pause, safe-point continuation, reset, stop and restore must
silence/rebind outputs at the actual executor boundary. Source audit found
set_heartbeat(false) currently runs at executor exit, not at pause entry.
A proposed narrow prerequisite invokes that existing driver hook at pause/
actual-resume too, with an updated generic contract and ordering tests.
This is a shared Emulator behavior change, not implicitly authorized as a Lib
rename; it requires explicit approval in S6's packet before implementation.
Compat never reads Emulator private pause flags. Snapshot capture's clock
barrier must not reactivate sound while the machine remains paused.

## DMA prerequisite and retained defects

at_dma.c's transfer contract advances the device buffer while the guest address
follows DMA direction. bwd_dest_copy_from_M instead decrements the host
destination and increments the guest source. The decrement wrap split uses
offset rather than offset+1. Word channels must reverse unit order, not byte
order within a unit. Repair the original helpers/call sites, not a private
sound DMA implementation. Existing T92 auto-init fixes must survive.

dma_request reports terminal count, including masked/no-transfer cases, not
a byte-transfer success count. dma_enquire exposes count, not readiness.
S5 tests real transfer and canaries for increment/decrement, byte/word,
zero/one/ffff offsets, crossing/noncrossing, auto-init second round,
masked/disabled channels and mapped ROM. Existing decrement VERIFY tests alone
do not prove copying. A needed transfer-result extension stays with DMA.

The existing TODO remains until S5 is admitted and qualified; no unrelated
INT 1Ah oracle or null-BOP debt is silently brought into this task.

## Snapshot fidelity

Snapshot belongs to the machine/device archive, not Lib. Preserve fixed-width
DSP/parser/DMA/IRQ/FM state and pending copied sample data needed for logical
continuation; cancel old host output and rebase host position on restore.
Do not archive handles, native WAVEHDR/MIDIHDR or the WASAPI queue.

"Precise PCM snapshot" means precise emulated device/data continuation, not
rewinding sound already heard or reproducing the OS mixer's hidden buffers.
Host MIDI restoration sends reset then saved bank/program/controllers/pitch/
sustain and tracked notes; repeated note-on/envelopes, arbitrary vendor SysEx
and external hardware state cannot be restored exactly. State this limitation
in product documentation. Keep incomplete parser input in the device archive,
not a replay log of all MIDI traffic. FM no-host state is index/status and
pending device data; no nonexistent synthesizer phase is invented.

The existing snapshot safe-point and x86/x64 format rules remain authoritative.
No new version field or legacy-format reader is introduced. Lifecycle cleanup
is required in every integration S; S9 is combined qualification, not permission
to ship unsafe intermediate teardown.

## Planned serial S tasks

These estimates are engineering envelopes, not measured diffs or blanket
permission to exceed scope. C/H production and tests are separate; build,
manifests/docs and binary artifacts are counted separately at delivery.
Every implementation S refreshes its exact file/add/delete brief first.

Owner scheduling revision, 2026-10-15 (original request):

> 要求当前s完成后，优先准入新s修复；后续s任务顺延。

S6 remains active. After its complete executor delivery and actual-change
review, admit the shared-quality S7 below before the former S7/S8, now S8/S9.
This is an internal T93 sequence change, not a second active task or a new
global Queue candidate. Existing completed-S records retain their identifiers.

### S1 — host-call audit and reviewed design

This document, CURRENT and source evidence only. Production/test +0/-0.
Exit: owner reviews the PCM/MIDI/FM limits and the S2-S8 sequence. No claim of
source import, playback, executable qualification or final T closure.

### S2 — move existing PCM without behavior changes

Move the five audio files to audio/pcm and the two Lib audio tests to
test/lib/audio/pcm. Rename symbols/guards and target to their PCM owner;
update Compat audio.c and four existing App sound tests, root/shared CMake,
test registration, manifests, boundary checks and documentation.
About 15-22 code/build paths; the 793 production lines move, not duplicate.
Rename-aware production estimate +180-260/-180-260, net about zero;
test reference edits +80-160/-80-160, net about zero. No format/progress change.
Exit: no stale public alias, single PCM path, unchanged first-tone and full
existing regression. This isolates relocation from behavioral extensions.

### S3 — complete the neutral PCM contract

Extend pcm stream interface/root/private/Windows/Linux files and Types audio
declarations; add native-admission result, supported source rates, copied
progress with reset epoch, event notification, stream gain and short-tail
submission. Keep one worker/FIFO and the WASAPI backend.
Estimated production +250-450/-60-120, net +130-390; tests +300-500/-20-60,
net +240-480; roughly 7-10 production/test files plus build/manifests.

Tests: invalid/unsupported rates, 8k/11025/nonstandard rate with backend
injection, first and repeated 1/512/1024-frame bursts, accepted versus submitted
versus consumed, priming silence, underrun, clear/stale epoch, partial capacity,
gain zero/full, native attach failure and cancellation. Prove progress after
the last enqueue without polling or another producer command. Rerun native
PCM and all PC-speaker first-delivery/failure tests; fake acceptance is not
evidence of an audible endpoint.

### S4 — neutral MIDI output and honest FM capability

Add the proposed MIDI files and bounded native ownership plus Types WinMM
declarations/linkage. Add only the minimal unsupported raw-FM acquisition
boundary on current platforms; no fabricated OPL implementation.
Estimated MIDI production +300-550/-0; FM +30-70/-0; combined net +330-620.
Tests +300-500/-0; roughly 10-15 source/test paths plus build/manifests.

Tests: one complete short message immediately delivered; SysEx copy lifetime,
four-slot exhaustion, short/long ordering, synchronous and asynchronous
completion, reset during pending output, native failure and close ownership,
optional volume unsupported, FM unsupported/null. Native MIDI smoke must
distinguish device absence from a successful output call. No promise of
audible FM or a Linux backend. No SB state in Lib tests.

#### S4 implementation and qualification ledger

Frozen S4 universe: the public MIDI operations listed above, FM acquisition,
their selected platform implementations, Types/build ownership, four MIDI/FM
tests and the existing complete dual-width suite. This does not qualify the
future guest device parser, audible instruments or arbitrary host endpoints.
Completion requires each row below and the full packet qualification. The
executor has completed the following evidence; coordinator review follows
the complete executor commit and push, not this self-report.

The implementation uses four stable WinMM header/data slots, two native
auto-reset events and no worker, secondary FIFO or periodic polling. Caller
operations reap completed headers. Accepted final output already belongs to
WinMM and does not need a subsequent producer call. Ordinary short messages
wait for preceding long messages; realtime messages bypass that wait. WinMM's
own short-message call can block and cannot be cancelled by the library event.
The existing caller-serialization contract is retained.

WinMM reset returns pending headers but does not transmit EOX. The same long
submission path sends a one-byte EOX continuation only if reset interrupted
pending SysEx; no public partial-message API or second parser is added.
Native output failure is terminal; cleanup may still reclaim native ownership.
Optional MIDI device volume is explicitly not per-stream WASAPI gain.
FM has only acquisition with UNSUPPORTED/null, without a pretend backend.

| Member | Proof receiver | Current disposition |
| --- | --- | --- |
| Complete short/SysEx admission, maximum size, reserved status | midi_output smoke | Dual-width full pass plus 50 repetitions |
| Four-slot copy lifetime and exhaustion | win32_midi_output smoke | Dual-width full pass plus 50 repetitions |
| Ordinary/realtime ordering and native completion | win32_midi_output smoke | Dual-width full pass plus 50 repetitions; no audible claim |
| Reset EOX, terminal errors and close ownership | win32_midi_output smoke | Dual-width full pass plus 50 repetitions, including event creation and wait failures |
| Absent capability and optional volume | native_midi_output, unsupported_output and native mock | Both native default outputs opened/released; unsupported transport and volume branches pass |
| FM unsupported/null | midi_output smoke | Dual-width full pass plus 50 repetitions |
| Inward dependencies and Types authority | exact DAG selftest, types-layout and manifests | All pass; matrix includes every ordered MIDI/FM direct edge |
| Existing PCM and product behavior | complete shared/background/desktop suites | 154/154 per width, including all ten native desktop cases |

Pre-change estimate remains MIDI +300-550, FM +30-70 and tests +300-500.
The concrete source census is five MIDI C/H paths, two FM C/H paths and one
Types header, plus Lib build/edge declarations. Tests add four C files and
extend registration and the existing exhaustive direct-edge matrix. FM needs
fewer lines than estimated because there is no legitimate platform transport
to wrap. No mirror, Compat, Machine or Emulator production changes occur.

First focused build caught an incorrect test-only output helper name; it was
removed rather than adding a needless Types alias. The corrected initial two
tests pass on x64 and x86. The real Windows adapter itself compiled in that
first build. These are source/compiler and mocked transport proofs, not
manual hearing or guest integration acceptance.

References: Microsoft [midiOutOpen](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutopen),
[midiOutShortMsg](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutshortmsg)
and [midiOutReset](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutreset).

S4 final executor evidence (baseline `1525f4f3`):

- Production C/H: eight paths, +444/-0, net +444 (MIDI 397, FM 22,
  Types 25). Tests: four C paths, +356/-0, net +356. Build/edge configuration:
  source two files +22/-5, tests two files +8/-2, reported separately from C/H.
  Two manifests, design/contract documentation and both EXEs are also updated.
- No XP/OpenNT mirror diff, existing PCM implementation change, Compat/Machine/
  Emulator production change, user INI, medium or snapshot modification.
- Hidden full job passes both Release builds. x64 shared 94/94 (50.31 s),
  other background 55/55 (93.43 s), desktop 5/5 (14.25 s). x86 shared 94/94
  (45.70 s), other background 55/55 (72.01 s), desktop 5/5 (14.00 s).
  The three isolated message/unsupported/native-mock tests each pass 50
  repetitions on each width. Native default MIDI open/release passes separately.
- x86 Ninja again reports recovery from a prematurely ended log and rebuilds
  597 steps successfully. The executor did not purge the build cache or
  interrupt a build. x64 reuses its existing intermediates (34 steps).
- Both manifest gates, Types boundary, documentation governance and diff
  whitespace checks pass. Raw owned logs are disposable after this checkpoint;
  no trace, external sound bank or external firmware becomes a test input.
- Published x64: 3,097,633 bytes, SHA-256
  `2E95B23961221DE7CB0B5F8F3D7DF568A55F4CFFDC651ABF474173642B4ED8A2`.
  Published x86: 3,498,351 bytes, SHA-256
  `2F151B30204794604CC2DE9171BF0D658850C8BD275E4B4FD34B861D317D6855`.
- Similar-issue sweep: source searches for polling/sleep/thread creation,
  direct WinMM vocabulary and retry paths in the new audio leaves find no
  polling, extra worker or reopen path. Direct SDK calls remain Types aliases
  used only by the selected Windows implementation. PCM retains its distinct
  WASAPI worker/FIFO because PCM and MIDI are different host transports, not
  duplicate delivery routes. All new public/native failure paths have a
  disposition in the table above; no additional defect is deferred from S4.
- Evidence boundaries: compiler/static and mocked transport proofs plus real
  device open/close are established. No manual hearing, Linux runtime,
  guest MIDI parser or XP Sound Blaster integration success is claimed here.

### S5 — original DMA prerequisite repair

Change at_dma.c's existing decrement helpers/split and, only if justified,
the existing DMA result contract/header. Do not duplicate transfer logic.
Production +20-50/-10-25, net -5 to +40; tests +120-220/-0 in the existing
device_contract_smoke fixture. Roughly 2-3 code paths plus mirror manifest.
Use the complete canary/wrap matrix above. Remove the corresponding TODO only
after actual proof. Keep unrelated debt and T92 accepted repairs intact.

S5 implementation checkpoint: the existing backwards READ helper advances
guest memory but decrements the host pointer; backwards WRITE reverses bytes
rather than controller units. The decrement split also omits offset zero.
The local patch changes only the existing helpers/signatures and split: one
unit is copied through the same SAS memory authority, the host buffer advances,
and the guest unit address retreats. Selected host_def.h defines PM; the old
non-PM ROM-clipping branch is not a selected runtime path and is not expanded
or rewritten. SAS continues to reject writes to protected ROM.

The bounded sweep finds three current production calls, all in gfi_image.c,
using the beginning of a forward host buffer (read, write and format IDs).
There is no alternate DMA copy owner. The existing reload test used VERIFY
for decrement, so it could not detect the copy bug; it now performs real READ
in both directions. The new 60-case matrix covers two widths, two directions,
three transfer types and five offsets (ordinary, zero, one, FFFE and FFFF).
Each case compares a whole DMA bank and host canaries, zero-length/disabled
requests, partial transfer/TC, final registers, TC acknowledgement and masked
repeat. WRITE cases also map their private fixture bank as ROM and verify
that DMA cannot modify it. The eight prior reload combinations remain.

Before production edits the new test compiled and failed on transferred
memory, establishing a red regression. An earlier test-only compile failure
was an original-header `byte` macro collision with a fixture variable; the
variable is now `lane`. The expanded transfer/ROM/reload test passes both
widths; final qualification is recorded below. No guest audio playback or Sound
Blaster integration is claimed by these DMA checks.

Actual production delta is one file +28/-15 (net +13); test delta is one file
+107/-5 (net +102). No public header, shared component, guest-device state or
snapshot layout changes. Reconstructing the pinned XP subset from the retained
S3 include ranges reproduces SHA-256
`cae6f92e62d993a24c7f490af3ab24eebb78dcbcea434141437878166a6628c0`.
`git diff --no-index --numstat` against that subset changes from +81/-1 before
S5 to +109/-16 after S5: local mirror diff grows by 43 changed lines, while
production grows by 13 source lines. Excluded XP ranges are unchanged; no
local repair was excluded to reduce that comparison.

Qualification checkpoint: dual Release builds and all background tests pass
(94 shared + 55 other per width). x64 timings are 43.96/95.80 seconds;
x86 45.97/72.07 seconds. The x86 Ninja log reports premature-end recovery
and rebuilds 597 steps; no cache purge or competing build was requested.
The original first-tone, guest sound, media/snapshot and cross-process snapshot
regressions are included. Public eight-directory sources/tests and owner
INI/media/snapshots remain unchanged.

Native desktop qualification exposed an intermittent fixture-boundary failure:
the first x64 compact-console case and the following x86 compact-console case
reported stage=1/error=6 before sending start. The unchanged x64 five-case
confirmation then passed. Source identifies this stage as unavailable Console
code page after attempted AttachConsole; error 6 alone does not establish why
attachment failed or whether the child exited. A scratch-only native probe
successfully attached six consecutive x86 children (656-703 ms, code page 437,
child still active). A scratch copy of the package fixture with added child-exit
diagnostics passed both normal and compact routes. Those passes do not explain
the earlier failure; no production or tracked-test workaround was introduced.
The initial scratch probe compiler lookup assumed FILEPATH rather than the
actual UNINITIALIZED CMake cache key; that harness-only lookup was corrected.
The final same-process desktop attempt failed x64 compact again. Running the
diagnostic fixture pair under CTest reproduced child exit 3221225794
(0xC0000142, DLL initialization failed), AttachConsole error 5 and subsequent
GetConsoleCP error 6. This narrows the failure to native child startup, not
guest DMA execution, but does not identify the DLL or prove CTest is its cause.
The bounded follow-up runs every original desktop case in an independent
CTest process; this is test isolation, not a production retry. The unresolved
startup diagnostic is explicitly transferred to TODO rather than erased.

Final qualification passes every registered test on both widths: 94 shared,
55 other and five native desktop cases (154/154 coverage per width). Desktop
cases pass unchanged in separate CTest processes: x64 durations
6.67/6.26/0.33/0.81/1.26 seconds, x86 6.95/6.37/0.33/0.85/1.28 seconds.
This is not a claim that the earlier grouped native invocations passed or
that their startup defect is fixed. The original DMA debt is removed after
bounded copy/wrap qualification; unrelated debts remain. Local mirror,
documentation and whitespace gates pass. One manual mirror-gate invocation
used a relative source root that its script did not resolve; the absolute-root
invocation and registered full-suite gate pass.

Published Release artifacts:

- softpc64.exe: 3,097,633 bytes; SHA-256
  `244A758A607FC0BE5C22D5E7A104B37523032E58CC84F6AA1908146F4C63C8AC`.
- softpc32.exe: 3,498,351 bytes; SHA-256
  `FEAAFEFE2BD850315374B1237C4FB5FFD341D8DAE4129DF2FBBA084DA984D8D7`.

The coverage unit is the finite transfer matrix plus the prior eight reload
combinations, three current caller sites and all 154 registered cases per
width. Every member has a source disposition or executed assertion above;
no hardware/channel beyond the selected two-controller model or real guest
Sound Blaster workload is claimed. The only transferred finding is the
pre-main native package child startup failure, not a second DMA implementation.

### S6 — XP DSP/PCM/Mixer and host integration

Import necessary subsets of sndblst.h, nt_sb.h, nt_sb.c, nt_sbdsp.c and
nt_sbmxr.c, preserving source identity/notices and minimal explicit patches.
Wire original ports/DMA/PIC, Lib PCM through Compat sound host files, and
Machine lifecycle; no NTVDM VDD loader or second device thread.
Estimated retained XP source +1300-2200/-0; local production +250-450/-20-60,
net +190-430; tests +300-500/-0. Approximately 12-18 code paths.
The owner explicitly approves the narrow pause-hook implementation and requires
a subsequent public-component quality audit. Its Emulator tests add +60-100/-0;
the existing callback remains the only execution-activity hook, with no new state/API.

Before payload, publish the exact selected-range/host-seam diff for review,
especially scheduling and DMA-position mapping. Preserve source behavior where
possible; source fidelity does not require copying unsafe host pointers or
polling teardown. No standalone-only branch inside mirror source.
Tests: reset/version, speaker gate, single/auto/high-speed transfer, count and
EOI/IRQ ordering, pause/continue, zero/master/channel gain, rate changes,
first/repeated bursts, no endpoint/stalled endpoint and no use-after-free.
Compare host progress and guest observations separately. Each changed source
policy or XP workaround gets an explicit disposition, not a hidden rewrite.

#### S6 pinned-source scheduling checkpoint

Read-only recheck at XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89, against
SoftPC 44cad548 plus the owner-approved S7 planning edits. No XP body is
imported by this checkpoint and no new device test or playback is claimed.
The following concrete seams refine the pre-payload review, not its approval.

| Pinned XP source range | Observed behavior | Required receiving path/proof |
| --- | --- | --- |
| nt_sbdsp.c:1276-1321 AutoInitEoiHook | EOI can reopen for a changed rate and submits the next Auto block unless DSP-paused. | Use the existing PIC EOI callout; do not make audio completion itself start the next block. Test completion-before-EOI, delayed EOI and paused EOI. |
| nt_sbdsp.c:1345-1410 DmaDataToDsp | Captures original DMA address/count, checks mask/count, obtains guest data and retains the block size before host delivery. | Preserve guest-address versus host-pointer distinction and frozen transfer ownership. Test masked/no-transfer and snapshot state separately from accepted PCM. |
| nt_sbdsp.c:1911-1996 DMA accessors | Directly reads/writes the existing controller; restores base address/count on terminal count in auto-init. | Do not import a second DMA model or both advance through dma_request and apply the same progress again. The chosen adapter must prove exactly one count/address advance and reload. |
| nt_sbdsp.c:2555-2672 StartDspDmaThread | Creates a worker, prepares/reopens output, registers EOI and begins the transfer. | Preserve the device start/EOI policy but replace native-thread scheduling at the host seam with the existing executor. Native worker/event/header ownership stays in Lib. |
| nt_sbdsp.c:2750-2834 timeout completion | Updates DMA and raises IRQ after a deadline; only conditionally rechecks physical position when the guest has queried DMA, with a bounded rescheduling heuristic. | This is not identical to unconditional consumed-frame completion. Explicitly disposition the XP heuristic before selecting the executor deadline/progress rule; no implicit parity claim from enqueue success or a green PCM test. |
| nt_sbdsp.c:2883-2898 Auto timing | Adjusts estimated block time for small/large blocks. | Record whether retained guest scheduling or excluded old host workaround, with counter/IRQ evidence; do not silently erase it while importing the parser. |
| nt_sbdsp.c:2545-2549, 2699-2707 | Header unprepare polls; thread stop waits for native worker exit. | Replace only host ownership/wait mechanics using existing Lib completion/cancellation, not Sleep polling or another guest worker. |

The local PIC already has EOI_HOOKS callouts in ica.c:360-366 and pending-IRQ
withdrawal notification at 1271-1278; nt_eoi.h already declares the host hook. The current
Compat search finds no implementation. First assess enabling this existing
seam and adding its host owner, instead of editing PIC dispatch or adding an
independent interrupt observer. Registration/withdrawal/no-registration semantics
need actual tests; existence of the hook is source evidence only.

The local Emulator invokes set_heartbeat at run entry/exit (machine.c:562,
573), not at its pause rendezvous (391-445). The existing SoftPC receiver
also resets idle/pacing state, so invoking it at pause/resume is a behavior
change, not merely adding an audio call. The separately required owner
approval has now been granted. The focused proof must cover
ordinary/debug pause, paused snapshot safe-point continuation, reset/stop
while paused, and exactly ordered enable/disable calls before completion
facts; never call the enabled hook on a stopped/error exit. No new public
callback or duplicated pause state is proposed by this checkpoint.

#### S6 approved pause-hook implementation checkpoint

Owner explicitly approves the Emulator change and requires the same public
code-quality audit afterward. The existing executor rendezvous now disables
heartbeat before PAUSED/reset completion and enables it before continuation,
including private snapshot safe-point continuation. Stop/error unwind retains
the existing final disable. The callback contract explicitly permits repeated
disable; no heartbeat state cache, new callback or audio dependency is added.

Count against 44cad548: production machine.c +7/-4 and interface comments
+3/-0, total +10/-4 (net +6); worker regression +58/-5 (net +53), excluding
manifests. The test runs the real worker with scripted waits. It asserts the
gate at ordinary/debug pause, resume, reset, stop, failed/cancelled wait,
private snapshot continuation and completion without duplicate public facts.
Both x64/x86 focused builds and emulator.machine_wait pass. Initial runs
exposed a test-fixture expectation still assuming unsupported snapshots; the
successful capture case now has its own callback and expected result. This
is not guest DSP or whole-S6 qualification. x86 Ninja recovered a premature
log ending and rebuilt 36 affected inputs; no cache purge was performed.

Post-edit audit: all four production heartbeat calls remain in the same
Emulator worker/executor owner; the sole App registration still forwards at
the existing machine boundary. Added values use Lib Types. No include,
link dependency, public symbol, stored state, lock, queue or platform call
was added. Emulator corpus/manifest and test dependency checks pass, as do
documentation governance and diff whitespace checks. Existing broader shared
quality findings remain assigned to S7, not silently included here.

#### S6 PCM host-boundary implementation checkpoint

Compat sb_pcm.c/h now own one Sound Blaster host stream, separate from the
PC-speaker stream. The adapter converts bounded unsigned eight-bit samples
to signed sixteen-bit PCM and preserves Lib's accepted-prefix and error
results without retrying. Progress copies accepted, submitted and consumed
counts separately. Its notification only wakes the existing executor; it
does not touch guest DMA, PIC or DSP state or create a device thread.

Close unregisters the borrowed sink, clears unsounded output and destroys the
stream in that order. Failed destruction retains the stream and rejects a
replacement open; cleanup can subsequently release it. This is host-resource
ownership, not a new guest lifecycle. Machine/DSP teardown integration is
still pending and is not claimed by the adapter's isolated tests.

The owner-local sb_pcm_smoke compiles the actual adapter with injected Lib
operations. Both x64 and x86 pass after the failure fixture was corrected:
the real Lib installs a notification sink even when registration returns an
existing terminal failure, so the fixture must retain that sink too. Assertions
now verify unregister-before-clear-before-destroy, failed startup combined
with failed destruction, retained ownership, and successful later cleanup.
Existing cases cover exact sample conversion, bounded/partial submission,
queue-full and host failures, gain/active forwarding and independent progress
counters. This proves adapter contracts, not real endpoint playback or XP DSP
delivery. No Lib production change is needed for these failure cases.

#### S6 EOI host-boundary implementation

The existing ica.c EOI_HOOKS sites are enabled by the device target; one
Compat registry implements RegisterEOIHook, RemoveEOIHook and host_EOI_hook.
The original nt_eoi.h cannot be imported wholesale as its remaining declarations
require NTVDM structures and scheduling. Compat eoi.h instead declares the
three existing host ABI operations; ica.c gains only that include (+1/-0).
No PIC algorithm is copied into Compat. Registration validates 0..15 and
non-null hooks, rejects occupied slots without mutation, and removal requires
the same owner. Negative cancellation counts are dispatched, not discarded.
The mirrored subset identity stays unchanged; the include is a local patch.

Delayed IRQ is separate from EOI registration: pinned XP sets its delivery
mask before recording the pending request in the PIC. Queueing the request
only after timeout is not equivalent. The local quick-event action helper
also delays request creation and is therefore not a substitute. Its separate
action-list ownership is unnecessary here.

The selected host adaptation now uses XP's existing quick-event mechanism:
minimum 1000 delay units, the original 200-unit overhead offset, one pending
release per IRQ, and repeated calls accumulating only in the original PIC.
The existing scan combines DelayIrqLine with the retained IRET mask; no
alternate PIC or request counter is introduced. The callback releases the
actual IRQ's adapter (irq >> 3), avoiding XP's bit-mask-as-line ambiguity.
Cancellation deletes the queued release and clears eligibility suppression;
the device must separately withdraw its request, as in the original contract.
Scheduling failure leaves the mask and PIC unchanged. NTVDM locks, WOW and
kernel timers are excluded because all these operations run on the executor.
The interpreted quick-event delay is not claimed to be an exact physical
playback deadline. Device reset/stop and archive ownership still need the
DSP integration and final state-qualification work.

Both x64/x86 real-PIC tests now pass for master IRQ5 and slave IRQ13:
pending IRR before release, blocked eligibility, two accumulated requests,
delivery after queue dispatch, cancellation retaining PIC ownership, and
withdrawal followed by queue dispatch with no stale request. EOI tests still
pass in the same run. This adds 48 Compat implementation/declaration lines,
48 test lines and two CMake registration lines over the EOI checkpoint;
the additional mirror change is +2/-1 (comment plus scan expression).

The real PIC regression passes on x64 and x86: registration bounds/null,
duplicate and wrong-owner rejection, ordinary/specific/automatic EOI through
port writes and interrupt acknowledge, redundant EOI suppression, pending
request withdrawal (-1), and no callback after owner removal. This covers
master IRQ5, not all PIC modes or sound-device lifecycle. Compat adds 39 C/H
lines; mirror +1/-0; tests +56/-0; root build/test wiring +4/-1. No new public
shared API is introduced. The fixed XP mirror gate passes for all 498 bodies.
Both targeted builds finish successfully; Ninja log recovery caused additional
unchanged-input rebuilds, not an intentional clean build. No EXE publication,
full regression, DSP integration or executor delivery is claimed yet.

#### S6 DSP parser and DMA-read import checkpoint

Pinned nt_sbdsp.c has now been re-read through its complete command parser,
reset, TableMunger, rate selection, DMA accessors and worker completion branch.
The necessary parser range is 523-987 (read status/data, reset write, byte
parser and command dispatch); it must not be replaced by a new command table.
ResetDSP's guest-state assignments at 1031-1064 survive, while native event,
thread and WaveOut ownership are excluded. GetSamplingRate at 1149-1237 keeps
the existing clamp and standard-rate rounding. The exact function boundaries
must be checked against the pinned text when extracting; these ranges are
source-review locations, not a completed subset manifest.

One additional necessary source repair is confirmed in pinned at_dma.c.
Its address read tests `port == SbDmaChannel`, although controller-0 address
ports are 0,2,4,6 and the default channel is 1. Its count read tests
`port == SbDmaChannel + 2`, which happens to match channel 1's count port 3,
but is not a general channel mapping. The existing port dispatch already
derives the channel with `(port-DMA_CH0_ADDRESS)/2` or the corresponding count
base. Use that derivation for the original SbGetDMAPosition callback, only
before the low-byte read (`first_last == 0`). No alternate DMA register cache
or port observer is needed. This is source proof, not a reproduced guest fault.
Focused proof must verify address/count low-byte refresh, matching high-byte
stability, unrelated channels, and controller-1 reads without spurious calls.

Other explicit import dispositions:

- The parser's <=0x10 short-block compatibility branch currently bypasses
  PCM and generates a delayed IRQ; the size is stored as transfer length minus
  one. Tests must distinguish lengths 1,2,17,18, not silently claim tiny
  transfers exercise PCM. Any decision to remove that original behavior must
  be explicit, not hidden in a new host output adapter.
- TableMunger and the short capture placeholder dereference a guest-derived
  address as a host pointer. Preserve the guest byte transformation and use
  existing SAS memory access; never cast that address to a native pointer.
- Reset-to-exit-high-speed and ordinary reset take different original paths.
  Preserve that distinction while removing native thread/event waits; no
  shared Emulator special case belongs here.
- Worker timeout completion conditionally consults host position only when
  DMA has been queried. Enqueue success must not complete the DMA block.
  The exact remaining scheduling policy is still an implementation obligation,
  not proved by Lib PCM progress tests or the newly passing IRQ tests.

#### S6 Mixer and port-path source checkpoint

The complete 329-line pinned nt_sbmxr.c has now been read. Its retained
device portion is MixerSettings/MixerState and the existing address/data/reset
functions (28-210), with the existing setters at 212-328 receiving explicit
local host-output corrections. This is a candidate selection, not a committed
subset manifest. Original notice/header remains; the direct windows.h import
and WinMM handles/calls are host vocabulary, not guest mixer state.

Confirmed source-level dispositions:

- Master and FM decode three bits after shifting by one; Voice decodes two.
  Preserve raw stored register bytes and those masks. Do not silently upgrade
  this CT1335 model to an SB16 stereo mixer or manufacture CD audio output.
- Master/Voice/FM compute level*step-1 (241/283/319). At zero this underflows
  before channel packing. Both PCM setters then overwrite the same host volume,
  so writing Voice after Master can defeat master mute. Fix the existing
  device-side effective-gain calculation, not Lib's gain contract. One
  recomputation path must serve master/voice/speaker/reopen; no independent
  Compat mixer-register copy. Preserve/report the positive-level mapping
  explicitly rather than silently claiming a hardware-calibrated attenuation
  curve. Exact composed scaling remains part of the pre-payload patch review.
- MixerDataRead's reset case calls ResetMixer without assigning *data. The
  real port entry VsbByteIn initializes *data=0xff before dispatch, so this is
  not proof of an uninitialized public read. Tests must exercise that entry,
  including reset reads, rather than force a newly invented return byte.
- Unknown mixer indices leave the previous selection unchanged in the XP
  switch. Preserve that observable behavior unless a separate, evidenced
  correction is approved; do not add a generic register-array replacement.
- SetSpeaker in nt_sbdsp.c writes zero/off or full/on using PreviousWaveVol.
  Reopening and speaker toggles must recompute from the existing device
  controls rather than overwrite master mute. Retain the guest DSP command
  and register owners; classify this as a local output correction.

Port-path review covers nt_sb.c:411-518 (registration/removal) and 520-709
(DSP/Mixer dispatch and Adlib alias mapping). Reuse io_define_inb/outb and
io_connect_port; registration/removal must stay balanced on partial failure.
S6 must not activate the separate MPU/FM handlers prematurely: their receiver
is S8. DSP busy status in high-speed mode and the port-level default 0xff
remain original behavior. No software FM or success-only placeholder is
admitted to make the intermediate link pass.

InitDevices at 300-328 contains NtVdmControl(PM CLI), then PCM resource/device
admission; failure returns to VsbByteIn with 0xff and causes writes to return
without dispatch. Optional MIDI initialization failure instead disconnects its
ports. Exclude the NTVDM control call, but retain an explicit no-PCM-device
disposition and test it; do not claim XP no-endpoint parity merely because a
new Lib create returns UNSUPPORTED. Lazy initialization, terminal output failure
and optional capability absence must not collapse into an automatic reopen
loop. These are source requirements, not newly passing runtime tests.

Focused receiver tests therefore need actual IO dispatch for reset/index/data,
all decoded Master x Voice levels (8 x 4), speaker on/off, reserved-bit readback,
both setter orders, reset/reopen and no-endpoint behavior. Capture the exact
gain passed to the injected host interface; audibility is not the oracle.
The finite matrix is a requirement for implementation, not an executed result.

#### S6 Mixer subset implementation checkpoint

The first sound payload now imports the pinned nt_sbmxr.c device, the
sndblst.h mixer constants and nt_sb.h mixer declarations. Exact selected
ranges are in [the S6 subset ledger](../etc/evidence/softpc/t93-s6-xp-sound-subset-ranges.tsv).
Remaining sound-header ranges are deferred to DSP/S8 or excluded native ABI;
they are not presented as permanently unnecessary guest definitions.
The current mirror manifest now covers 501 bodies. Its new t93-s6 selection
label identifies this separate extraction; no build-time transformation or
external build dependency was introduced.

The existing MixerSettings, MixerState, register selection/read/write and
reset remain the only mixer state. BYTE/VOID use the existing core IU8/void
ABI instead of including windows.h. Each original setter stores its original
raw byte and calls one device-owned SbApplyMixer entry. MixerGetVoiceGain and
MixerGetMidiGain read that same state. For positive decoded levels the original
XP scales remain level*8192-1 and level*16384-1; zero now maps to zero. Master
and channel gains compose as master*channel/65535 using bounded IU32 products.
This is an explicit output correction, not a hardware-calibrated attenuation
claim. DSP speaker gating belongs in the pending SbApplyMixer implementation,
not in a new Compat register cache. No success-only production implementation
is provided to conceal that pending integration.

Changed-path accounting for this payload: three newly retained production
files +386/-0, tests +86/-0. Relative to the exact selected XP source, the
mixer is +47/-65, internal header +16/-12 and constants +0/-0: local patch
total +63/-77, net -14. The omitted header ranges are separately recorded,
not charged as local functional deletions. Root CMake/test and provenance
metadata are excluded from those C/H counts.

The actual imported mixer is compiled in its focused test target, with only
the pending device-output callback replaced by a test observer. Both x64/x86
pass all 32 Master/Voice combinations in both setter orders, all 64 Master/FM
combinations, zero mute, reserved-bit retention, defaults, CD storage without
host output, unknown-index retention and reset read/write. The expected gain
table is explicit and uses wider arithmetic than the implementation. Initial
compilation failed because the test omitted the canonical Types include;
adding it resolves both builds. This proves the imported register functions,
not actual I/O port dispatch, speaker gating, endpoint admission, DSP playback
or snapshot restoration. Those remain S6/S9 integration obligations; this
checkpoint does not close S6 or publish an EXE.

#### S6 DSP rate and DMA implementation checkpoint

The next selected nt_sbdsp.c payload retains GetSamplingRate, QueryDMA,
SetDMACountAddr, SetDMAStatus and GetDMATransferAddress, plus their existing
TimeConstant and controller references. The original dmaGetAdaptor owner
accessor is restored without the NTVDM conditional; no second controller is
introduced. The S6 range ledger supersedes S3's exclusions for exactly that
accessor/prototype. Reconstruction of the earlier S3 selections first matches
their recorded hashes, then adds the necessary XP ranges. The mirror gate now
verifies 502 retained bodies.

Host scalar aliases use the existing core types. The internal SB_DMA_INFO is
a copied observation, not an exposed NTVDM VDD service ABI. SbInitializeDMA
binds the existing controller and rejects channels outside byte-wide DMA.
GetDMATransferAddress uses the existing dma_system_address expression and
returns guest physical address, removing the native Sim32 pointer conversion
and its 32-bit truncation. The future transfer consumer must use SAS, not cast
this value to a host pointer. Native diagnostic calls are excluded, not
replaced by a second logging facility.

The real machine device fixture compiles this selected DSP source directly
until its port owner is installed. It supplies only SbDmaChannel; all DMA
register state belongs to the actual controller. x64/x86 each pass the existing
DMA regression and new DSP DMA regression (2/2). Thirteen rate cases cover
XP's standard-rate mappings, a nonstandard rate and lower/upper clamping.
Channels 1 and 3 are programmed through actual ports with all auto-init and
decrement combinations. Tests verify full address/page/count, mask/mode,
register writeback observed through ports, terminal-count auto reload versus
non-auto preservation, request/TC flags and read-to-clear semantics. This is
not yet proof of DSP command entry or asynchronous PCM/DMA progression.

This increment adds 289 lines of DSP source, nine controller accessor/comment
lines, thirteen internal declaration lines and three channel declaration lines;
the existing device test gains 66 lines. No shared component or guest asset
changes. The complete S6 delivery still requires the original parser, real
port registration, playback progression and machine lifecycle wiring.

#### S6 DSP command-state implementation checkpoint

The selected DSP payload now includes XP's read/write/reset state machines,
DspReadStatus/Data, DspResetWrite, DspWrite, WriteCommandByte and TableMunger.
The original DSP constants accompany them. Ordinary versus high-speed reset
keeps the XP distinction: ordinary reset restores TimeConstant, while the
high-speed escape retains it. ResetDSP delegates transfer quiescence to the
pending executor-owned StopDspTransfer rather than copying XP native events,
locks or its DMA thread. StartDspTransfer likewise names that pending owner.
There is no production success stub for either operation.

Native diagnostics and mode-display calls are excluded. Removing diagnostic-only
default branches leaves explicit breaks, not compiler-extension-only empty
labels. Capture probing and TableMunger use SAS writes to the guest address;
no original host-pointer cast remains. The same-issue scan of all retained
address increments finds four command/probe sites: the two short-block paths
and both TableMunger bytes now honor the existing controller decrement bit.
Playback-position/completion increments in yet-unselected XP ranges remain an
explicit part of the next transfer import, not a claim that they were fixed.

XP's short-block behavior remains deliberately observable: lengths represented
by SBBlockSize <= 0x10 raise a delayed IRQ without PCM; the zero case does not
advance DMA. This is preserved source behavior, not a newly introduced
application-name special case. Large capture remains XP-unimplemented. DSP MIDI
cases are retained where they existed; no new dispatch rule is invented before
the separately planned MIDI attachment.

This increment adds 573 DSP lines, 107 constant/header lines and 22 internal
declaration lines, plus 103 fixture lines. The selected DSP file is now 862
lines; against its exact current XP subset it is +123/-238 (net -115). Range
and identity ledgers account for the expanded source selection. The fixture
observes pending transfer, IRQ, speaker and reset boundaries only in its test
executable; actual DMA and guest-memory operations are not mocked. It covers
all 256 identification and reserved-register bytes, version/status progression,
reset sequencing, high-speed escape, command dispatch, two-byte block lengths,
short-block direction/count, capture silence and both TableMunger directions.
Initial compilation exposed a fixture variable named byte colliding with the
original core macro; it was renamed to observed. The first corrected dual-width
run passes all three DMA/DSP cases; final post-review verification likewise
passes all three on each width. No live PCM, port-installation or
whole-S6 success follows from this focused result.

Follow-up boundary coverage expands the same short-block fixture from four to
twelve cases: single/auto command, increment/decrement, and ordinary/upper/
lower 16-bit addresses. Expected addresses are explicit constants; page 2 must
remain unchanged across wrap. This changes no production code. Dual-width
background verification passes all three targeted cases on each width. This
proves the command's controller updates, not asynchronous PCM completion.

The next increment selects pinned DSP lines 1239-1274 (GenerateInterrupt).
It keeps immediate PIC delivery versus the existing delayed-IRQ host boundary;
only native type spelling and diagnostics change. The test-only interrupt
observer is removed. Commands now reach the real PIC and quick-event queue in
the fixture, with pending IRR, deferred eligibility, interrupt acknowledge and
EOI checked. Other unfinished transfer/reset host boundaries remain explicit
test observers. Dual-width verification passes all three targeted DMA/DSP
cases per width. This increment adds 37 DSP source lines and one declaration;
the fixture removes its IRQ observer and checks actual controller delivery.
No full port or PCM integration is inferred.

#### S6 DMA-position implementation checkpoint

Pinned SbGetDMAPosition's active branch advances a virtual position; despite
its introductory comment it does not call GetWaveOutPosition. Preserve its
small-block three-quarter threshold, large-block eight-sample reserve,
near-end increment of two and Single-only PreviousWaveOutPos signal. Removing
the inactive alternative and DESCENT branch is recorded in the subset ledger,
not a new guest-name condition. The executor cannot spin awaiting its own
submission, so the former bWriteBurstStarted wait becomes a readiness guard.
The original transfer-context fields remain in DSP, not Compat or Lib.

Address distance now uses the DMA direction modulo 16 bits before adding the
virtual increment; count/address still write through SetDMACountAddr. No
terminal-count completion or PCM acceptance is manufactured by this query.
The duplicate SbInterrupt declaration added with the IRQ checkpoint is removed;
sndblst.h already owns it. This increment is production +95/-1 (net +94) and
tests +49/-0. The actual DMA-controller fixture covers eight direction/size/
mode combinations, each with three readiness gates and 100 queries. Expected
final wrapped addresses/counts are fixed constants, including retained page 2.
Both architectures pass the three targeted DMA/DSP cases. This proves the
position operation; original port read-hook attachment and transfer-derived
initialization remain unfinished, not implied by directly seeded fixture state.

#### S6 transfer receiving sequence, before payload

Re-read pinned DmaDataToDsp (1322-1470), DspProcessBlock (2343-2490),
StartDspDmaThread (2555-2672), SbGetDMAPosition (2120-2283) and worker
completion (2711-2919), alongside the current PCM progress/notification
contract. The receiving sequence is one DSP-owned transfer, driven by the
existing executor; no Lib callback may mutate its controller or PIC.

1. Start retains the original Single/Auto, high-speed, pause and EOI policy.
   Capture the DMA address/count/mode/page and block bytes once through SAS.
   Physical addresses use the existing DMA address conversion, with 16-bit
   directional wrap. Do not advance the controller while copying or queueing.
2. Replace native WaveHdr/semaphore ownership with one bounded frozen block
   and its accepted-prefix cursor. Submit at most the existing PCM submission
   capacity per call; partial success advances only that prefix. A full FIFO
   returns to the executor. The existing Lib notification only wakes it; no
   sleep, busy retry or device worker is introduced. Host errors remain errors,
   never synthetic playback success.
3. Once the block is queued, retain XP's virtual-increment initialization from
   its size and preceding query count. Keep the native accepted-range origin
   separate from guest DMA progress. Lib accepted/submitted/consumed counters
   are different observations; no one of them substitutes for all three.
4. Use the existing quick-event owner for the XP completion deadline. Preserve
   the Single nominal delay, Auto small-block +1/8 and large-block -BurstSize
   adjustments. The original conditional position recheck and ten-attempt
   bound remain explicit DSP scheduling policy. A host epoch change cancels
   old observations rather than being mistaken for completion.
5. Completion writes the existing DMA registers once, applies terminal-count
   reload through the existing accessor, then raises IRQ. Auto waits for the
   existing EOI callout before copying the next block. Guest PauseDMA retains
   XP's behavior: it suppresses the next Auto block, not an invented host pause
   command. Machine pause is separately handled by the approved execution gate.
6. Reset/stop remove device EOI ownership before withdrawing pending requests,
   cancel the scheduled completion, discard unsounded host data and quiesce
   the host callback before release. Preserve ordinary-reset versus high-speed
   escape. Snapshot serialization of the new pending device work remains S9,
   not an implicit claim from S6's host tests.

Native format probing is another host-only difference: XP's SetWaveFormat
tries nearby formats through WinMM. The selected PCM endpoint accepts or
rejects the requested representable rate; no unreviewed resampler or guessed
fallback rate belongs in DSP. The existing rounded GetSamplingRate mapping
remains the guest-to-requested-rate authority. Format rejection must follow
the device's failed initialization/output path, not silently play at a wrong
rate. TimeConstant's 0xffff consumed-request sentinel must not be passed back
to GetSamplingRate; retain the established active rate until a new request.

The next implementation batch is estimated at 250-400 DSP/Compat production
lines and 100-180 focused test lines, replacing the remaining transfer
observers as its real operations become linkable. This is not a new S or a
claim that the receiving sequence has been implemented. Proof must cover
ordered partial queueing, frozen source data, count/IRQ before and after
completion, EOI gating, rate changes, cancellation and endpoint failure in
addition to the already tested command/controller functions.

The first receiving increment now implements frozen block capture and bounded
prefix submission in the existing DmaDataToDsp/DspProcessBlock path. A 65536
byte device-owned array replaces native header storage for the maximum DSP
block, with only its size and accepted cursor retained. SAS reads preserve
DMA page/direction/wrap; copying and queueing never advance DMA. The original
query-count increment calculation is retained. Full FIFO yields an incomplete
block to the executor, and a host error is returned, not converted into IRQ.
Completion scheduling, start/stop and actual port attachment are still pending.

The focused fixture uses actual guest memory and DMA plus a scripted PCM sink:
32, 700 and 65536 byte blocks in both directions, source mutation after capture,
a seven-byte accepted prefix, full FIFO, reported output error, exact remaining
bytes without duplication, and unchanged DMA registers after complete enqueue.
The first build caught legacy const-macro pollution of the host declaration;
the host header is now included before original core headers, preserving the
public signature. Corrected dual-width verification passes all three targeted
DMA/DSP cases per width. This increment adds 62 DSP lines and four internal
header lines, plus 72 test lines, with no production removals. No successful
native audio or whole-S6 result is claimed by these tests.

The completion increment now owns one cancellable time-based quick event,
separate from PCM submission readiness. It retains XP Single/Auto delay
adjustments and the conditional physical-lag check with ten-attempt bound.
Host progress errors and changed epochs latch failure instead of reporting a
successful DMA transfer. Completion writes directional address/count once,
uses the existing terminal-count reload and then raises the existing PIC IRQ.
The stalled-host flag still requires consumption by the pending start/reopen
integration; EOI rearming and actual device lifecycle are not yet attached.

The focused 24-case completion matrix covers both directions, controller
auto-reload on/off and six outcomes: ordinary timeout, queried lag then progress,
ten stalled checks, changed epoch, host failure and cancellation. It observes
real DMA/PIC effects with scripted host progress. Initial dual-width state
tests pass. Self-review then corrected scheduling from the instruction-count
entry to add_q_event_t, and added a queue-record check for EVENT_TIME and a
1025-sample/11025-Hz deadline. That new assertion initially contained a hand
calculation error (94 rather than 93 ms); the corrected rerun passes all three
focused device cases on each width.
No wall-clock accuracy or native endpoint playback is inferred from explicit
test dispatch of the queue. Increment: production +99/-0, tests +89/-0.

#### S6 failure and port-dependency review

Real start/stop failure tests now cover occupied EOI registration (the other
owner survives), unsupported host open, gain failure before a queued completion
(no success IRQ), and cancellation of a delayed pending PIC request. They pass
all three focused cases on both widths before the subsequent subset cleanup.

Port-source review found an unnecessary link dependency retained in the first
DSP extraction: SB-MIDI cases were placed in switch(DSPWriteState), but use
command values 0x30..0x38. Every reachable state assignment is in 1..11;
MidiOutPoll (12) was assigned only inside the unreachable 0x38 state branch.
The pinned source header explicitly excludes SB-MIDI. The state, its cases
and BufferMidi declaration/test override are therefore removed from the
necessary subset, not replaced by a successful empty implementation. The
MPU MIDI parser remains S8's real work; this does not remove a supported path.
A regression sends the entire 0x30..0x38 command interval then GET_VERSION,
checking that unsupported commands neither respond nor consume its next byte.
That follow-up run passes all three focused cases on both widths. Source
reachability proof and command regression are distinct; corrupted internal
enum values are not a guest API.

#### S6 DSP/Mixer port-handler checkpoint

The selected nt_sb.c read/write handlers and ResetAll now replace the final
device-protocol fixture override. Device IRQ/DMA globals also return to that
owner. The retained XP defaults are A220/I5/D1; native WinMM loading, NTVDM
trapping, diagnostic calls and later FM/MPU dispatch are not imported here.
The exact selected ranges and local type/default-binding changes are accounted
in the S6 subset ledger and mirror manifest. This checkpoint does not yet
install the handlers during product machine initialization.

The new handler regression checks reset pulse/AA response, DSP 2.01 response,
Mixer readback/default restoration, high-speed write-status refusal, blocked
DSP commands, still-accessible Mixer and reset exit from high-speed mode.
The first run caught a fixture assumption: its old ResetAll stub omitted
ResetMixer, so a later test expected the cumulative gain-call count to remain
zero. The corrected assertion compares calls immediately before/after speaker
commands while output is closed; production reset retains XP's Mixer-before-
DSP ordering. The retest passes all three focused cases on x64 (0.41 s) and
x86 (0.49 s), with the hidden job exiting zero. No end-to-end audio result
or product qualification is inferred from these handlers. This step adds
154 lines in nt_sb.c and two declarations, and removes the fixture's duplicate
device globals and ResetAll implementation. The product initialization,
executor service and pause/resume sample preservation remain S6 work.

#### S6 actual port registration checkpoint

Port attachment follow-up selects XP's previously deferred unconditional
SNDBLST_ADAPTER 36 declaration (ios.h line 642), without enabling the null
driver or importing NTVDM's spare VDD slots. SbInitialize/SbTerminate bind
and release the six selected DSP/Mixer addresses in the original I/O table.
Local io_connect_port is void, unlike XP's fallible NTVDM trap operation;
preflight checks that same table before changing any binding. No second
registry or generic recovery API is added. Repeated attachment preserves
device state, and teardown quiesces DSP work before disconnecting.

The existing port-handler test now uses real inb/outb and checks occupied-port
rejection, all six read/write owners, duplicate attach preserving Mixer,
unbinding and reattachment. All three focused cases pass on x64 (0.44 s) and
x86 (0.31 s). This is 38 added production lines in nt_sb.c plus the one
restored ios.h declaration. The three device bodies now move into the
production device archive; the smoke links those same objects instead of
compiling a private device copy. That follow-up passes all three focused
cases on x64 (0.50 s) and x86 (0.47 s); the hidden job exits zero.
Machine-lifecycle invocation is not yet implemented. Its cold-reset order
must terminate the old binding before the original I/O tables and quick-event
queues are recreated, then attach afterward; a stale attached flag must not
skip registration into newly cleared I/O tables.
The lifecycle integration must also exercise cold reinitialization after a
changed time constant and an incomplete reset pulse: ResetAll currently
preserves XP's ordinary/high-speed split, so calling it is not by itself proof
that a fresh standalone hardware lifetime has initial parser/rate state.

#### S6 machine lifetime checkpoint

Machine lifetime wiring now terminates sound before cold reset rebuilds queues
and I/O tables, then installs DSP/Mixer after original reset. Initialization
selects the full existing reset branch, and ResetDSP clears its reset-pulse
parser as well as its read/write parser. A new lifetime cannot inherit an old
time constant or finish an old half-written pulse. Guest high-speed reset
still follows the original partial-reset selection.

SbTerminate reports failed host closure while disconnecting ports. A later
cleanup can release a retained PCM handle without touching disconnected ports.
The existing machine destroy return now reports failure and preserves its
object before releasing CPU/RAM; both production callers in driver.c honor
that result and retain composition ownership. No public shared API changes.
Focused lifecycle/port regressions pass on both widths. The additional
close-failure injection proves machine destroy reports IO_ERROR without
freeing its RAM, and successful later cleanup releases the retained output.
A real machine cold reset also verifies reconstructed I/O bindings, restored
time constant and an abandoned old reset pulse. Each width passes all three
focused tests (x64 0.42 s, x86 0.31 s). The machine/driver change is +18/-4,
net +14 across three files; selected device initialization/reset changes
remain separately visible in the mirror manifest. Machine, lifecycle,
checkpoint, snapshot-boundary and restart regressions pass 5/5 per width
(x64 68.56 s, x86 64.75 s), with a zero job exit. These check existing
machine/snapshot behavior, not yet serialization of active DSP state (S9).

#### S6 executor service checkpoint

The existing platform executor event now services a pending PCM prefix after
the generic lifecycle callback, unless shutdown already requested an unwind.
An output failure requests the same original lifecycle unwind; readiness reads
the DSP's existing latched status through a narrow getter, not a second error
state. The VM return also checks readiness for its restored-CPU route, which
does not call machine_run. Normal and restored execution therefore report the
same failure instead of returning a successful stop after audio failure.

The added test submits only seven of 64 guest bytes, then services the real
executor event and checks the complete ordered payload. Its failure half
injects a write error while the actual CPU run is live and requires machine
IO_ERROR with no invented accepted bytes. A cold reset clears the device
failure through the existing owner. Dual-width verification passes all three
focused tests (x64 0.48 s, x86 0.38 s); the background job exits zero.
PCM pause/resume retention remains unfinished; no full S6 claim is made.

#### S6 pause-boundary audit checkpoint

The existing Lib inactive operation discards pending output and resets its
progress epoch; it cannot be treated as a queue-preserving pause. The device
already owns a frozen guest block, but its current accepted prefix is not a
consumed prefix. Wiring the execution gate directly to inactive/active would
therefore either lose pending samples or replay already consumed samples.
No such direct hookup is admitted as a completed implementation. Retention
must account for the current block at the device owner and preserve the
original DMA/IRQ completion path, without a second PCM queue or clock.

The Compat seam regression now injects both deactivate and activate failure,
requiring the real adapter to return I/O error rather than claiming success;
it also injects progress failure and verifies subsequent successful copied
progress. No production/shared API change is needed for this proof. The
test change is +18/-5, net +13 C lines. Both widths build and pass the focused
test (x64 0.16 s, x86 0.10 s), with a zero background-job exit. This is mocked
host error-forwarding evidence, not proof of queue-preserving pause, physical
audio playback or active-device snapshot restoration.

#### S6 paused-block retention implementation

The admitted execution gate now calls the DSP owner. It records the consumed
prefix of the existing frozen block before discarding native output, cancels
that block's completion event, and preserves the unchanged controller address,
count and transfer obligation. On resume it reuses DspServiceTransfer to enqueue
the remaining prefix and arm the original completion path. There is no second
sample buffer, device worker, clock or IRQ route. An output-local suspended
flag makes repeated park/continue idempotent; a consumed-prefix offset rebases
progress across the Lib clear epoch. Transfer mode is frozen at capture so
STOP_AUTO's final block does not lose its original completion policy.

The consumption checkpoint is copied Lib progress, not an atomic capture of
the physical sound endpoint. Already heard audio cannot be undone; samples
consumed between that observation and native cancellation may be replayed.
This is within the documented distinction between exact emulated data/state
continuation and host audible phase, not a claim of sample-exact host pause.
No DMA advancement or IRQ is inferred from cancellation or resumed enqueue.

The real machine gate regression covers seven-byte partial and complete
64-byte submission, zero/partial/all observed consumption, repeated gate
calls, a second pause after further consumption, byte-for-byte suffix order,
unchanged DMA at park/resume, and original-deadline completion. Failed native
deactivation latches the existing DSP error. The first dual-width focused
pass is 3/3 each (0.54 s x64, 0.45 s x86); because final source/test refinement
overlapped that build, a fresh serial focused plus expanded machine run is
required. That final run now exits zero: focused 3/3 per width (x64 0.53 s,
x86 0.41 s), then expanded machine 5/5 per width (x64 66.10 s, x86 63.76 s).
This still does not replace complete S6 package qualification.

#### S6 complete PCM delivery regression

The existing machine sound smoke is extended rather than adding another
parallel fixture. Its original PC-speaker checks remain. After releasing that
stream, two cold-reset runs program the real DMA and DSP ports with a known
64-byte ramp. The production DSP, Compat conversion and Lib worker remain
linked; only the final platform leaf records delivered I16 samples. Each run
must deliver all 64 frames at 11025 Hz with exact U8-to-I16 values, without a
flush or subsequent producer burst. This separates complete first-delivery
evidence from the scripted Compat outlet used by the device contract matrix.
Verification ran serially after the pause/machine job, avoiding concurrent
writers to either Ninja tree. Both builds and the smoke pass (x64 4.20 s,
x86 0.92 s), with a zero job exit. No native audibility result is inferred.

#### S6 qualification coverage ledger

The frozen universe is the admitted DSP/PCM/Mixer protocol, its original
DMA/PIC owners, host output translation, execution/lifetime integration,
the shared pause-hook change, and every currently registered root test on
both widths. MIDI/FM attachment and active sound-state serialization remain
explicit S8/S9 obligations, not implicit S6 completions.

| Surface | Focused proof and current disposition |
| --- | --- |
| DSP reset/version/parser/short probes/high speed | Real device command and I/O-table tests pass both widths. |
| DMA copy/readback/direction/wrap, Single/Auto/EOI/IRQ | Original controller/PIC tests and completion matrix pass both widths. |
| Mixer/rate/speaker gain/output ownership | Mixer matrix and device host-failure fixture pass both widths. |
| PCM conversion/partial submission/first and repeated delivery | Compat seam and real DSP-to-Lib-worker smoke pass both widths; only native output is injected. |
| Pause/resume, cold reset, teardown and output errors | Device-gate matrix and expanded machine tests pass both widths. |
| Generic executor contract | Emulator deterministic pause/continuation regression and public corpus/manifest/inward test gates pass; no outward audio dependency. |
| XP subset identity | 503-body mirror gate passes; prior ambiguous pending labels are replaced with explicit native-host exclusions or S8 MIDI/FM receivers. Included source ranges and local hashes are unchanged by that label clarification. |
| Dual package build/publication and all registered regressions | Final R3 passes 160/160 per width: 94 shared, 61 other background and five individually isolated native desktop cases; both package EXEs refreshed. |

The first qualification script mistakenly selected shared names using a slash
instead of their actual dot prefix. Its shared group was empty and the other
group selected all 155 background tests; this is not valid evidence of the
required group order. The live run is preserved, not cancelled. A dependent
job runs only if it succeeds, then repeats the shared and remaining groups in
the required order using the exact dot prefix and --no-tests=error. Do not
report the initial empty group as a passing shared suite.

The initial x64 background run finishes 154/155: the source-boundary test
correctly rejects nt_sb.h directly importing Lib Types. The header now consumes
the existing Compat PCM host contract instead; no gate is weakened or direct
mirror-to-Lib exception added. The source boundary and 503-body mirror checks
pass after repair. Its follow-up build exposes the Mixer-only fixture's missing
private App include root, which is corrected on that test target only. Both
failed runs remain evidence. A fresh full run uses the correct dot-prefixed
shared selection and rejects empty groups; dual-width qualification is still
pending, not inferred from the earlier 154 passes.

Prequalification accounting (C/H only, tracked diff plus whole new files):
Compat five paths +192/-1 (net +191); Machine three paths +21/-5 (net +16);
Emulator two paths +10/-4 (net +6); nine mirror paths +2077/-1 (net +2076);
five test paths +1287/-13 (net +1274). These are changes from the S5 endpoint,
not the mirror's local-patch distance from the selected XP source. Build,
manifests, documentation and EXEs are counted separately at final delivery.

No S6 P commit is admitted as complete until the final row, exact changed-path
accounting and actual-change review obligations are satisfied. User INI/media/
snapshots are not outputs of this qualification.

#### S6 final qualification and public-change audit

The R3 hidden qualification job terminates with exit zero. Both Release builds
complete and refresh the existing package EXEs. Each width passes all 160
registered tests: shared suites first (94/94), other background tests next
(61/61), then five native desktop cases in separate CTest processes. x64
background durations are 45.76/100.01 seconds; x86 durations are 45.62/70.37
seconds. All ten desktop cases pass, including both ordinary/compact package
flows and Window retirement/modal plus Console display restoration. No Computer
Use or user media/configuration changes are involved. Earlier failed attempts
remain recorded above; this successful run does not erase their evidence.

Published SHA-256:

- softpc32.exe: `67e8d7b83ede74bd4f5b3cd533f55d1791f1bf3f41f8fc212f03b989f8fdf979`
- softpc64.exe: `fd4b6fcec5e13982465bea67eab1303ee5cb236c41415269948e498dfbb16669`

The owner-required post-edit public audit rechecks the actual Emulator diff,
all four heartbeat call sites and the executor park/continue/error paths.
Changed production expressions use existing Lib Types constants. The sole
executor remains the callback owner; private snapshot continuation still emits
no extra RUNNING or PAUSED fact. There are no new includes, link dependencies,
public symbols, stored fields, platform calls, locks, queues or threads.
The change is +10/-4 production (net +6), +58/-5 tests (net +53), plus the two
manifests. Emulator corpus, manifest, inward test-boundary and whitespace gates
pass again after full qualification. The shared suites exercise existing gates
for all four source/test pairs. This is a bounded change audit, not a claim that
the known S7 formatting, path, naming and verifier gaps have been repaired.

PCM delivery evidence is the real DSP/Compat/Lib-worker route with the final
native leaf injected; it is not physical hearing evidence. Active DSP snapshot
serialization and MIDI/FM attachment remain S9/S8 respectively. The complete
executor delivery and subsequent actual-change review are still required before
S6 closure.

Final source accounting retains the prequalification C/H figures above:
production +2300/-11 (net +2289), tests +1287/-13 (net +1274). Root CMake is
+30/-1 (net +29), the mirror checker +1/-1; manifests, source-selection records,
task documents and two binaries are separate from those code figures.
The staged whole-change whitespace check additionally sees the newly imported
sndblst.h trailing blank line, identical to its XP selected subset. It is
retained under mirror-format fidelity rather than introducing a formatting
patch; this warning is not presented as a clean whole-change whitespace result.

The nine touched mirror files compared with their manifest-pinned XP necessary
subsets use `git diff --no-index --numstat --ignore-space-at-eol`; the selected
texts are reconstructed from the recorded upstream ranges and their normalized
hashes checked against the manifest before comparison. These totals include
pre-S6 local patches in already retained files, not just this S's new changes:

| Mirror path | Added | Removed | Local purpose |
| --- | ---: | ---: | --- |
| base/inc/dma.h | 3 | 1 | Existing controller access declaration without NTVDM guard. |
| base/inc/ios.h | 2 | 2 | Retained adapter slot and prior local source layout. |
| base/inc/sndblst.h | 11 | 11 | Existing scalar/time-width declarations, no native thread ABI. |
| base/system/at_dma.c | 122 | 27 | Earlier DMA/snapshot repairs plus selected sound readback hooks. |
| base/system/ica.c | 139 | 1 | Earlier PIC archive/IRET support plus Compat delayed-IRQ eligibility. |
| host/inc/nt_sb.h | 66 | 63 | Device declarations and existing Compat host contract. |
| host/src/nt_sb.c | 45 | 213 | Original I/O-table attachment replaces NTVDM registration. |
| host/src/nt_sbdsp.c | 646 | 1428 | Executor/quick-event transfer and copied PCM replace native DMA thread/WinMM ownership. |
| host/src/nt_sbmxr.c | 47 | 65 | Register semantics retained; stream gain replaces native device volume. |
| Total | 1081 | 1811 | Net -730 relative to the selected XP bodies. |

Excluded XP ranges are itemized separately in the S6 subset ledger; they are
not counted as local functional patch deletion. This selected-source metric
must not be substituted for the +2077/-1 mirror growth from the S5 endpoint.

#### S6 DMA-read regression detail

The two deferred XP DMA-read hooks and sound declaration include are now
selected (at_dma.c lines 50, 191-193, 220-222). Local hooks use the existing
address/count port-to-channel calculation, not XP's incorrect direct port
comparison. They run only on the controller's low-byte read; the following
high byte sees the same sampled position. Controller 1 is unchanged. This is
the existing DMA register path calling the existing DSP position operation,
not an alternate port handler or second controller.

Production adds seven lines. A 37-line regression addition exercises actual
inb/outb for channels 1/3, address/count, increment/decrement and a low-byte
carry. It also checks another channel, controller 1 and inactive DSP do not
advance the sound position. All three focused DMA/SB cases pass on each width.
The device fixture supplies the currently uninstalled sound port globals;
the product archive must acquire the real port owner before its full link is
qualified. No temporary success stub is added to production to conceal that
unfinished S6 dependency.

#### S6 transfer-control integration checkpoint

The selected XP start/stop (2554-2710), AutoInitEoiHook (1275-1321) and
ExitAutoMode (2922-2962) now feed the existing executor-side transfer helpers.
Native thread/semaphore ownership is excluded, not emulated by a second worker.
DspServiceTransfer resumes accepted-prefix submission and arms exactly one
completion after the whole copied block is accepted. The existing host wake
will call this service; production executor/port attachment is still pending.
Auto completion raises the PIC request only; the real EOI callback captures
the next block. Negative withdrawal notices, paused/reset state, an outstanding
block and a latched host failure cannot start another block. Ordinary reset
removes the EOI owner before withdrawing the PIC request, cancels the completion
and delayed IRQ, and closes output. High-speed escape clears output but retains
the admitted format. ExitAutoMode removes continuation without clearing already
accepted PCM or cancelling the last completion. Gain failure is checked before
the completion callback can overwrite it with a successful progress query.

Start/stop observer functions are removed from the device fixture. Its parser
checks now inspect actual mode/active/output state with DMA masked. A composed
DMA/PIC/host-fixture sequence covers increment/decrement, partial acceptance,
two blocks across EOI, pause/continue, controller auto-reload, exit-auto's final
block and stop cancellation. Both widths pass focused 3/3 (0.50/0.31 s). This is
not native-host playback, real guest port installation or whole-S6 qualification.
Production increment is +81/-0 C/H; test net change is +54 lines, replacing
observer assertions rather than retaining a second test path.

#### S6 output ownership and gain checkpoint

The pinned output/speaker ranges (1471-1504 and 1622-1910) now have a local
host-seam implementation. DspPrepareOutput uses the retained time constant and
rate conversion, reuses an unchanged rate, and closes before changing format.
The native device enumeration is excluded; Lib admits the requested rate or
returns its actual status. The former WinMM nearby-rate probe loop is not
copied: the selected PCM backend already owns native format conversion.
TimeConstant becomes 0xffff only after a usable stream is acquired. Failed
close prevents replacement; failed open leaves the requested rate available.
The existing stalled-output flag now clears the host queue before the next
block, matching XP's forced reset of its next-output position.

SbApplyMixer computes one stream gain from existing Mixer registers and the
speaker gate. Closed-output speaker commands are ignored as in XP. Unlike
XP's device-wide restore logic, no native global volume is read or mutated;
the admitted stream-local gain composes Master/Voice rather than replacing
either register. A host gain failure latches the existing transfer failure,
with no retry or fabricated DMA completion. The next device start/reset
integration must use these functions; they are not yet wired to guest ports.

The device contract target now links the real Mixer as well as DSP. Its new
host fixture covers initial/default gain, muted Mixer changes, speaker-on with
zero Voice, unchanged/changed rates, failed close/open and terminal gain
failure. The old speaker observer is removed. The focused DMA/SB-DMA/command
set passes 3/3 on each width (0.62/0.46 s); its command case executes the
output matrix. This does not replace the standalone Mixer register matrix.
This increment adds 53 production C/H lines and changes tests +85/-4, net +81;
one CMake source line links the real Mixer into that test target. The selected
upstream ranges remain separate from local host adaptation; mirror and
documentation gates pass. No public shared source is changed in this increment.

#### S6 guest DMA pause checkpoint

Pinned DSP lines 1505-1573 supply PauseDMA/ContinueDMA. Both mutate the existing
bDspPaused flag; neither calls native pause/reset. The Single-mode position
query only guarded Sleep(0), so both that query and host yields are omitted
on the sole executor. Machine pause remains a separate lifecycle operation.
Production adds 14 lines; tests replace the two observer stubs with actual
command-state assertions for Single/Auto, repeated pause/continue and DSP reset.
The focused DMA/SB-DMA/SB-command set passes 3/3 on each width (0.42/0.41 s).
The x86 Ninja log again required automatic recovery and 307 build steps;
no cache purge or foreground timeout occurred. No EOI continuation or host
playback qualification is claimed by these flag assertions. Test increment
is +12/-6 (net +6); production is +14/-0. Mirror and documentation gates pass.

Pinned ExitAutoMode (2922-2961) removes the EOI hook and resets the next-output
position; it does not clear already queued samples. The pending transfer
integration must preserve that distinction from reset/teardown. AutoInitEoiHook
checks bDspPaused before capturing another block, not before completing the
currently submitted block. These are source findings, not runtime proof yet.

#### S6 EOI and delayed-IRQ dependency checkpoint

Additional read-only dependency reference: pinned host/src/nt_eoi.c blob
9f4e9dae836b054e3b43a179923cc0da20fb2ab1. It is not an eighth whole-file
import. Its WOW/native monitor services, lock and timer dispatch are not an
admitted standalone payload.

The existing EOI contract explicitly defines CallCount=-1 as cancellation of
a pending interrupt. Local SWPIC_clear_int emits it only when clearing the
last pending request; normal EOI emits the remaining count. This corrects the
earlier loose label of that path as reset notification. XP AutoInitEoiHook
ignores CallCount and starts the next block unless paused. Therefore teardown
must not leave a live auto-transfer callback behind while withdrawing an IRQ.
Do not simply discard all negative notifications in a generic host dispatcher:
their documented purpose is to release a waiting device. The device transition
and callback ordering require an explicit port/PIC regression.

The original host EOI table is one 16-line registry. Register rejects an occupied
line; Remove only removes the same callback. Dispatch range-checks the line,
but Register/Remove do not. A receiving Compat owner must validate all three
entry points and null registration, preserve occupied/mismatched state, and
use the existing PIC callout. Do not add a parallel registry in the sound
device or replace existing PIC interrupt selection. Required cases: line -1/16,
duplicate registration, wrong-owner removal, ordinary/automatic/specific EOI,
pending withdrawal, stop/reset cancellation and callback removal before reuse.

XP host_DelayHwInterrupt (229-328) queues the PIC request immediately while
DelayIrqLine blocks delivery; its non-MONITOR branch schedules add_q_event_i
and later calls ica_RestartInterrupts, with 0xffffffff cancelling the delayed
entry. The current selected standalone PIC/Compat source has no DelayIrqLine,
UndelayIrqLine or ica_RestartInterrupts implementation. Thus neither copying
that function nor substituting an immediate ica_hw_interrupt preserves delay.
Resolve the bounded existing-event-queue adaptation before implementing this
host seam. Its proof must distinguish request accounting, delayed eligibility,
cancellation and stale completion after reset; no periodic sound timer or new
guest thread is authorized. This remains an open implementation-design member,
not an already-working scheduling path.

### S7 — shared contract and quality repair

Closed after reviewed executor b1934914; implementation checkpoints follow below.
Baseline is S6 executor 065ac9ed and its published EXEs. The supplied
audit is source/reproduction input, not proof of these repairs or a fresh
full-suite pass. NXVM remains read-only comparison material; no receiving
repository runtime, audit JSON or build tree becomes a dependency.

Frozen scope is the following finding-to-owner/proof ledger, including the
preceding local eight-package audit. Recheck every finding against the S6
endpoint, record already-fixed members with evidence, and do not duplicate
their repairs. Shared source/tests retain their existing owners and APIs.

| Finding | Repair owner and minimal approach | Required focused proof |
| --- | --- | --- |
| Window title/status formatting reports success after truncation | src/emulator/product/monitor.c: check the existing formatter's required length against capacity, including the initial status prefix; retain one formatting path. | test/emulator/product/monitor_smoke.c: exact fit including NUL, one byte short, tiny capacity, long name, both title outputs, empty/nonempty help rows and existing invalid arguments. |
| Entry test doubles reproduce the same incorrect success contract | test/x86-ibmpc/product/entry_smoke.c: bounded, contract-faithful doubles; do not recreate the monitor implementation. | Exercise error propagation and cleanup when title or status formatting rejects capacity, while Emulator owns real formatting tests. |
| Executable directory retains a non-root trailing separator | src/lib/base/process.c, existing process_interface.h contract: correct extraction in its existing path. | test/lib/process_directory_smoke.c: Windows/POSIX ordinary directories, drive/POSIX/UNC roots as supported by the platform contract, exact capacity, missing separator/native failure, unchanged output on failure. |
| App appends a separator unconditionally | src/app-softpc/product/config.c: review the consumer with corrected Base output; minimally avoid duplicate separators for preserved filesystem roots if needed. | Existing App config test: ordinary directory and root produce exactly one join separator, unchanged filename and capacity/error behavior; no user INI changes. |
| Console guard/comment retain old owner | src/lib/console/console_interface.h: LIB_CONSOLE guard and Console Broker ownership wording; no alias. | Extend the existing naming gate and its negative fixtures to reject the retired Console guard/owner wording. |
| Linux input retains LINKVM/linkvm spelling | src/lib/kvm-base/linux/input.h and input.c, test/lib/linux_input_contract_smoke.c: use consistent KVM_LINUX/kvm_linux names and update all references, without behavior change. | Existing Linux input contract and stale-name scan/negative naming proof; do not claim Linux runtime from a Windows compile. |
| Public CMake project names retain NXVM branding | src/lib/CMakeLists.txt and src/emulator/CMakeLists.txt: neutral project names, preserving public target names and links. | Independent package configuration and name scan; these names were not runtime dependencies. |
| Outer production code mixes NULL and LIB_NULL | Emulator and both shared X86 source packages: use existing Types vocabulary; no new aliases. | Whole-package scan excluding literals/comments and negative source-gate tests, aligned with the existing test Types rule. |
| X86 source gates skip unexpected C/H layout and incompletely inspect CMake links | Both X86 verify_corpus.cmake files: reject unowned C/H paths and apply existing inward-link rules consistently. | Existing verify_negative.cmake suites: nested/unowned C/H, mixed-case CMake links and alternate global-link forms; valid inward dependencies remain accepted. |
| Target-wide format-truncation warning downgrade | src/x86-debug/CMakeLists.txt: recheck actual diagnostics, remove an obsolete exemption or fix the precise bounded formatting site. | Strict dual-width build; do not classify a warning exemption alone as a proven runtime defect or add a broader suppression. |

The owner-supplied extension reports 40 production and 17 test formatting
sites, no further confirmed production truncation, no second shared directory
join owner and no duplicate header guards. These counts are received evidence;
S7 must reproduce a current local inventory and classify every hit rather than
claiming its earlier 94/94 test result proves these boundaries. The local audit
at 44cad548 covered 265 paths/216 C/H files and found 230 raw NULL tokens in
eight outer production files after excluding comments/strings; refresh this
mechanical count at admission rather than forcing it onto changed code.

Preliminary scope: roughly 15-22 production C/H paths, 6-10 test C/H paths,
plus existing CMake/gates/manifests. Behavior corrections should remain about
+20-45/-10-25 (net -5 to +35); Types/naming substitutions may add and delete
250-350 lines each (near-zero net). Tests estimate +120-220/-10-30
(net +90-210), gates/build +40-90/-15-35 (net +5-75). These are estimates,
not measured diffs; publish refreshed per-file figures before implementation.
No source-mirror diff, audio transport redesign, extra manager, retry/rollback
framework, new product policy or edits to NXVM are admitted here.

Verification: first demonstrate focused failing boundaries on the old code,
then fix in place and rerun the same cases. Rebuild/publish x86/x64 EXEs using
existing intermediates; run all shared suites, other unit/integration suites
and all ten native desktop cases under the existing hidden-job governance.
Report original failures and known native-startup debt without unconditional
retries or claiming grouped tests passed when they did not. Refresh affected
manifests and run documentation/whitespace checks. Preserve INI/media/snapshots.

Exit: each ledger member is fixed and verified, or shown already correct with
current evidence; no silent deferral. Report production/test/gate add-delete-net
and public-package diffs separately. Push the complete executor P, switch roles
and review actual Git changes against this ledger before admitting S8. Stop
for unbounded path semantics, new public behavior outside these contracts,
unrelated worktree changes or a proposed second implementation path. T closure
still waits for the owner.

#### S7 first implementation checkpoint

The S6 endpoint and clean worktree were rechecked before edits. The formatting
repair estimate is monitor.c +12/-6, its owner-local test +45-60/-0, and the
entry fixture +15-25/-5. The new boundary regression fails on the old monitor
at the first too-small running title (x64, normal test exit 8), proving the
missing required-length check. The same route now checks signed formatter
results against each destination capacity; no helper, extra buffer or API is
added. Actual code is +12/-6; tests +48/-0 and +20/-5 respectively. Entry tests
inject title/status errors separately and verify command disposal, leaving
real formatting semantics with Emulator. Both widths pass the two focused
tests (x64 0.29 seconds, x86 0.17 seconds). This is not S7 full qualification.

The next bounded directory change estimates process.c +15-25/-1 and App
config.c +4-6/-2. Existing root extraction remains the only path; no general
path framework is proposed. A new Lib owner-local contract fixture injects
only the native executable-path query, while the existing native process test
is retained. It covers ordinary Windows/POSIX paths, drive/POSIX/UNC and
extended Windows roots, native failure and unchanged output. Capacity tests
use the existing whole-executable-path admission limit; this task does not
introduce dynamic retry merely to accept a smaller directory-only buffer.

The old directory implementation fails the injected ordinary-directory case
(x64 CTest exit 8). The repair retains POSIX/drive roots and the server/share
boundary for UNC (including canonical extended UNC), and removes only the
ordinary-directory trailing separator. App avoids appending a second separator
at preserved roots. Actual production change is process.c +13/-0 and config.c
+5/-2; tests add the 52-line Lib contract fixture and App +19/-1. Both widths
pass all three focused directory/config tests, including the retained native
directory query (x64 0.23 seconds, x86 0.37 seconds). No INI is written.

Affected manifests are updated. An initial PowerShell-default sort violated
their ordinal ordering; the manifest gates correctly rejected it. Ordinal
ordering was restored without weakening the gate, and all affected Lib and
Emulator manifests pass. This checkpoint does not close S7: naming/Types/gate
repairs, the full same-issue inventory and full package qualification remain.

#### S7 naming and Types checkpoint

The next mechanical scope was estimated as six naming/build files, about
60 equal replacement lines, plus checks and negative proof. Console now names
its own guard and Console Broker owner. Linux input uses KVM_LINUX/kvm_linux;
standalone CMake projects use shared_lib/shared_emulator without changing
public targets. The existing naming check rejects each retired Console/Linux
spelling independently; it adds no production state or alias.

A token-aware scan of all Emulator and both X86 production C/H files excludes
comments and string/character literals. It confirms 230 raw NULL occurrences
in eight files: machine.c 87, input_queue.c 9, control.c 16, control_state.c 13,
session.c 40, ui.c 55, debugger command.c 5 and PC keyboard.c 5. These become
LIB_NULL in place, with no control-flow or data-layout change. Source gates
reject reintroduction. Both X86 gates also reject unowned/nested source paths,
mixed-case outward links and global/property link bypasses; nested CMakeLists
receive the same checks. Their new negative cases pass locally.

The target-wide Debugger format-truncation warning downgrade is removed.
Strict shared-target builds pass on both widths without a replacement
suppression; the Linux-input contract and two naming checks pass 3/3 on each
width. This Linux-input test compiles selected decoder code on Windows, not a
Linux runtime qualification. The first build script mistakenly named the
INTERFACE aggregate as a build target and failed before compilation; the
corrected run builds its actual entry test instead. The failed script result
is retained, not counted as a test failure or silently retried.

This is still an uncommitted S7 checkpoint. The complete formatting-site
disposition, independent configuration check, full shared/product/desktop
qualification, EXE refresh and actual-change review remain required.

#### S7 finite same-issue audit

The refreshed eight-directory universe is 266 files, including 217 C/H files:
source Lib 116/102, Emulator 30/21, x86-debug 15/10, x86-ibmpc 14/9;
matching tests 54/48, 22/18, 9/6 and 6/3 (all-files/C-H).
The added directory contract fixture accounts for the increase over the
received 265/216 baseline. Formatting inventory uses
`rg -n 'lib_c_v?snprintf\s*\(' <eight roots> -g '*.c' -g '*.h'`;
Types macro aliases are declarations, not invocation sites.

| Formatting owner | Sites | Disposition |
| --- | --- | --- |
| Emulator monitor | 5 | Three variable-size public title/status calls repaired and boundary-tested; two internal message/prompt calls use fixed short messages or the fixed monitor prompt. All message callers were inspected. |
| Emulator Session | 1 | Copies the NUL-terminated command-result prompt to an equally sized 64-byte field; no smaller destination is introduced. |
| x86-debug command | 15 | Three variable formatting operations validate required lengths/dynamic output allocation; twelve fixed error renderings use bounded 16/32-bit hex fields or constant ERROR text into 256-byte arrays. |
| x86-debug assembler | 8 | Six immediate renderings fit 256-byte arrays by fixed integer widths; two assembled statements separately validate total operand lengths. |
| x86-debug disassembler | 2 | Existing macros check negative result and required length against owned array/context capacity. |
| x86-ibmpc command | 9 | Fixed short messages/prompts and same-capacity debugger prompt copies; the Session destination relation is statically asserted. |
| Lib storage test | 1 | Temporary filename template plus fixed suffix fits a 64-byte array; mkstemp changes characters, not template length. |
| Emulator tests | 3 | Fixed media text, bounded fixed command vocabulary, and a printf-semantic composition capture returning the native required length. |
| x86-debug tests | 6 | Fixed register names and integer widths fit explicitly inspected local arrays; expected memory row is shorter than 128 bytes. |
| x86-ibmpc tests | 7 | Three entry doubles now reject truncation; four command-fixture calls format constant short text. |

This accounts for all 40 production and 17 test invocation sites, rather
than treating every unchecked fixed literal as a new failure path. The raw-NULL
inventory is zero production tokens after the mechanical Types repair;
remaining occurrences in comments are not C vocabulary violations. Directory
extraction has one Base owner and one App consumer; the latter's root join is
covered without touching user configuration.

Same-class gate review also found Emulator skipped root-level C/H files.
There are no such production files in this universe. A four-line guard and
three-line negative case now reject that unowned layout in the existing gate;
this refines the admitted verifier ledger without a new checker or runtime
change. Full qualification started before that static-only addition; its final
corpus/manifest/negative checks must be rerun against the final gate hashes.

#### S7 executor qualification

The complete final code passes both Release builds and 161/161 tests per
width: 95 shared first, 61 other unit/integration next, then five separately
invoked native desktop tests. All ten desktop cases pass. Final static-only
edits receive a further 8/8 corpus/manifest/negative pass per width.
Background shared/other times are x64 52.51/99.98 seconds and x86
69.45/73.67 seconds. No Linux runtime or manual listening claim is made.
Four independent Lib/Emulator configurations pass and cache their neutral
project names; they do not rebuild the production objects a second time.

Production C/H (14 paths) is +264/-242, net +22; tests C/H (five paths,
including the new 52-line contract fixture) are +140/-7, net +133.
Production build/gates are +73/-44, net +29; test build/gates +61/-4,
net +57. Counts exclude manifests, documents and EXEs. Mirror diff is zero.
The expanded x86 rebuild followed Ninja's existing premature-log recovery
warning; no cache was cleared and no CCPU source was changed.

Published SHA-256: x86
`83c36c9abdbc402aafc9e04df51236b175adc1f6db49d5ba9c65499221eb3dd7`;
x64 `2208dac4071a5f1961118106d3ff30b8a29044a809e86b661672984105d9460e`.
User INI/media/snapshots are unchanged. Executor delivery and the separate
actual-change review will create the S7 closure record; these results do
not claim the pending S8/S9 device/archive work is complete.

#### S7 reopened NXVM audit

Owner request: "先不准入S8，继续S7.请你审计NXVM项目中的最新公共八组件".
S8 is not active. This continuation changes audit/status documents only;
prior S7 implementation and qualification remain intact. No upstream import,
production/test change, build or new runtime result is claimed.

On 2026-10-16 the comparison uses SoftPC 7a523a42 and NXVM HEAD 59aea8649
plus all current working-tree content, including the new debug_result_smoke.c.
The upstream index changed from unstaged to staged during inspection; content
was checked again, so Git commit identity alone is not the audited identity.
The finite universe is the union of the four src/test pairs: 267 paths,
206 byte-identical, 58 differing shared paths, two SoftPC-only and one
NXVM-only. NXVM contains 265 paths, SoftPC 266. Relative to SoftPC the entire
eight-directory delta is +843/-771, net +72, including manifests/build/docs.

| Pair | Source same / changed / local-only | Test same / changed / local-only / upstream-only | C/H source + / - | C/H test + / - |
| --- | --- | --- | --- | --- |
| Lib | 105 / 11 / 0 | 45 / 8 / 1 / 0 | 80 / 53 | 139 / 56 |
| Emulator | 18 / 12 / 0 | 17 / 5 / 0 / 0 | 248 / 259 | 50 / 102 |
| x86-debug | 9 / 6 / 0 | 5 / 4 / 0 / 0 | 19 / 12 | 29 / 1 |
| x86-ibmpc | 5 / 8 / 1 | 2 / 4 / 0 / 1 | 30 / 65 | 75 / 21 |

Method: recursive file SHA-256 comparison; git diff --no-index --stat/--numstat
between corresponding roots. C/H counts include added/deleted files and
exclude manifests, build rules and documentation. All changed production C/H
paths were reviewed; unchanged paths were hash-compared, not re-proven line by
line. Normalizing LIB_NULL to NULL proves the input queue, Session control/
state/session and UI differences are only those token substitutions.

Import blockers and required preservation:

1. NXVM machine.c lacks the S6 executor pause disable and continuation enable
   calls to set_heartbeat. Its header lacks the gate contract, and its
   machine_wait_smoke lacks pause/capture/debug-stop coverage. SoftPC's driver
   forwards this gate to Compat and DspSetRunning; replacing it would remove
   the proven pause contract. This is a missing local repair, not evidence
   that NXVM recently introduced a new audio regression. Retain/align the S6
   three-file repair before import. No new runtime reproduction is claimed.
2. The outer production sources still use raw NULL where S7 uses LIB_NULL;
   their source gates omit the raw-NULL check. x86-debug still applies
   -Wno-error=format-truncation to the whole target. These fail SoftPC's
   admitted quality baseline even though they need not change runtime
   behavior. Preserve Types vocabulary and strict warning policy.
3. Both X86 dependency gates inspect only the root CMakeLists and lowercase
   command spelling, lacking the S7 nested/mixed-case/global/property-link
   rejection. Existing production links were not found to violate inward
   ownership; this is a regression-prevention gap, not a claim of an observed
   outward production dependency. Retain those negative-test-backed guards.
4. The upstream entry test fixes truncation in its formatting double and adds
   an oversized-name case, but lacks S7's disposal count and independently
   injected title/status failure short-circuit assertions. Keep that
   owner-local failure coverage; an oversized title does not reach the later
   status failure. Monitor boundary coverage is also narrower (single name,
   empty hotkey map), although the production truncation checks are correct.

Reasonable upstream improvements, subject to integration qualification:

- PCM completion/wake signal failure uses the existing task cancellation to
  release synchronous waiters. New deterministic clear/gain/flush/wake/space
  failure cases accompany it; no second worker or retry mechanism is added.
- KVM Base resolves shared cursor scanlines once; Console and Window retain
  their own percentage/pixel conversions. No guest-specific cursor branch.
- Monitor's fixed command metadata is consolidated. Current groups preserve
  ERROR rejection for exactly five lifecycle commands plus save/load, leaving
  debug/general and extension dispatch available. No product-name special case.
- Debug output failure is finalized by Debug itself, with borrowed static
  failure text and cancellation; Product consumes the result through one
  helper. Allocation failure and result-transfer tests accompany the change.
- Snapshot unsupported output falls back to the existing neutral Monitor
  rather than a duplicate X86 fallback function. The extension remains usable.
- The redundant app_composed_machine wrapper is removed in favor of
  emulator_product_machine; UI options gain an owner-specific name. This is
  an API migration, not a drop-in replacement: SoftPC's composition, config,
  extension and main declarations/callers need coordinated edits and tests.
- Directory-path cases now share the existing native-query test executable.
  Native discovery remains exercised; ordinary/root/UNC and failure cases
  remain. Removing the separate fixture is not itself lost native coverage.
- Retired Console/Linux naming checks move into Lib's component dependency
  checker. Their removal from verify_kvm_naming alone is not a missing check.

Read-only upstream component-DAG, naming and three outer-source gates pass.
Passing those gates does not establish the stronger missing S7 checks above.
No downstream compilation, runtime suite, desktop test or native audio test
was run in this audit; previous SoftPC qualification cannot certify the new
upstream changes. Conclusion: useful consolidation, but not ready for blind
eight-directory overwrite. Resolve the listed preservation gaps and approve
the App API migration before admitting import. S7 stays open for owner review.

#### S7 NXVM correction re-audit

The owner requests another read-only check after upstream repair. This pass
uses NXVM e86eb135ea8571c3d7230143fdfadb531a2a7ca9 with a clean worktree at
both entry and exit; Shared corrective commit is 86175d302. It supersedes the
import-blocker conclusion of the preceding working-tree audit, not its
historical findings. SoftPC production/test files remain unchanged.

The 267-path union now has 212 byte-identical paths, 52 changed common paths,
two SoftPC-only paths and one NXVM-only path. Complete eight-directory delta
is +750/-489, net +261 including build/docs/manifests. Source/test changed-path
counts respectively are Lib 11/9, Emulator 7/5, x86-debug 6/4, x86-ibmpc 8/5.

- Machine implementation is now byte-identical to SoftPC. The header retains
  the execution gate contract. The full pause/capture/debug-stop test returns,
  with an additional assertion that stop from parking does not re-enable it.
- Outer Types substitutions and strict Debug warning policy are restored.
  No target-wide format-truncation exemption remains in the compared CMake.
- Emulator and both X86 gates now inspect included .cmake files as well as
  nested CMakeLists. Their negative matrices exercise mixed-case direct links,
  global links, property-based links and raw NULL. These are stronger in scope
  than the previous root-only upstream checks.
- PC entry retains the disposal count and independent title/status failure
  short-circuit assertions; its oversized-name regression is additional.
  Directory tests also restore exact-fit/one-byte-short checks for every case.

The local read-only audit ran eight source/negative/DAG/naming checks and
verified all 257 listed content hashes across eight manifests; all passed.
These checks used disposable fixtures beneath the existing S7 build workspace,
never upstream writes. The first foreground collection did not retain terminal
output; it is not counted as qualification. A subsequent hidden run has explicit
log/error/zero-exit evidence and is the counted result. No dynamic test or EXE
build was performed here. Upstream's correction evidence reports 631 units
per width and receiver integrations, and explicitly retains an initial x86
MyNES console failure followed by unchanged-code passes; that is upstream
evidence, not SoftPC qualification or proof that the transient is repaired.

No new blocking defect was identified in the reviewed correction and retained
consolidations. Import can be admitted, but still requires the coordinated
SoftPC App API migration described above and complete receiving qualification.
Monitor's earlier exhaustive capacity matrix remains broader locally than
the upstream single-name/empty-hotkey tests; preserve its meaningful boundary
coverage during convergence, without calling this a newly found production
bug. No import or S8 admission is inferred from this audit.

#### S7 approved import continuation

The owner explicitly reopens the same S7 for import, cached dual-width full
qualification, publication and push, then owner acceptance. S8 is not admitted.
The source is clean NXVM e86eb135ea8571c3d7230143fdfadb531a2a7ca9. Pre-change
estimate: eight-directory +750/-489, net +261; seven App C/H paths need about
20 substitutions; retain the broader local Monitor capacity matrix. No source
mirror, Compat device, user INI, disk or snapshot change is admitted.

Import preserves upstream production exactly. The obsolete composed-machine
wrapper is removed; seven App files directly use emulator_product_machine and
x86_ibmpc_product_ui_options, on their existing composition path. The standalone
include gate follows this inward include. The merged process-directory test
replaces the redundant standalone fixture. The local exhaustive title/status
capacity matrix remains alongside upstream tests, with its manifest updated.

The timestamp-based Ninja trees are reused without a cache purge or toolchain
change. These existing trees do not enable a ccache compiler launcher; this
delivery claims intermediate-object reuse, not ccache hits. Initial file copying
preserved upstream timestamps: the first x64 run passed 94/95 shared tests but
the Console retirement fixture linked its old directly compiled Console object
against new assertions. The normal library object had rebuilt. After identifying
that stale object, timestamps of changed imported files were refreshed; no
assertion or production behavior was weakened. Full qualification restarts with
both initial evidence and the corrected run retained under build/t93/s7/import.

The documentation validator gains an explicit-owner same-step reopen case,
with rejected-unapproved and accepted-approved fixtures. This implements this
owner instruction, not an implicit task admission or a blanket reuse exemption.

The finite import ledger contains all 265 paths in the eight directories at
the pinned source, plus seven App consumers and two local verification tools.
Every shared production path is byte-identical to upstream. Of all 265 paths,
263 are identical; only test/emulator/product/monitor_smoke.c and its manifest
retain the stated local coverage addition. All eight manifests and LF checks
pass. The completion predicate is both width builds, all 95 shared and 61 other
tests per width, all ten native desktop cases, protected-path checks, artifact
hashes, then pushed executor delivery and actual-change review. Runtime proof
is the local qualification, not upstream reports or this hash comparison.

Changed-path accounting against pre-import 7a523a42, including deleted/new
paths and CMake/manifests/READMEs in the component totals:

| Directory | Paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| src/lib | 11 | 100 | 75 | +25 |
| src/emulator | 7 | 101 | 101 | 0 |
| src/x86-debug | 6 | 44 | 26 | +18 |
| src/x86-ibmpc | 8 | 57 | 82 | -25 |
| test/lib | 9 | 167 | 95 | +72 |
| test/emulator | 5 | 76 | 9 | +67 |
| test/x86-debug | 4 | 80 | 23 | +57 |
| test/x86-ibmpc | 5 | 129 | 34 | +95 |
| App composition consumers | 7 | 15 | 15 | 0 |
| Local verification tools | 2 | 20 | 3 | +17 |

Production C/H alone totals 22 paths, +217/-223, net -6. Test C/H alone totals
ten paths, +300/-69, net +231. This accounting includes the new debug-result
fixture, which an unstaged tracked-only diff would omit. No preserved-source
mirror diff is added. The wider tests exercise the consolidated paths; no second
runtime implementation is introduced.

Post-edit public-quality review covers every changed C/H hunk and all eight
source/test boundary, naming, negative-fixture and manifest gates. Emulator's
single command-description table retains the same ten commands, help order,
argument policy and seven-command ERROR rejection; extensions/debug remain
outside that rejection. Machine's heartbeat implementation is unchanged and
its header still describes the execution gate. PCM faults use the existing
worker cancellation signal to release waits, not a parallel failure dispatcher.
KVM shares cursor shape resolution while its leaves retain their own rendering
and blink responsibility. Debug owns its failure result and cancellation, so
the PC adapter no longer duplicates that policy. App retains only its concrete
composition and snapshot/media extensions. The complete receiving regression
must confirm these source conclusions; no physical audio or Linux-runtime
claim follows merely from the source review.

Receiving qualification is complete: x64 and x86 each pass 95/95 shared tests,
61/61 other unit/integration/gate tests, and 5/5 native desktop tests, for
322/322 across both widths. Shared suites precede the other suites; desktop
cases run individually after both background passes. No Computer Use or owner
manual result is counted. The corrected run has qualification.exit=0 and 14
JUnit files under build/t93/s7/import/run2. The initial failed run remains
separate. The x86 Ninja build recovered a premature log EOF and rebuilt 613
steps; no agent cache purge occurred. This is not a claim that all old objects
were successfully reused. Protected INI and all guest assets remain unchanged.

Published SHA-256:

- softpc32.exe: `A49AA7DBEBD9DBFDF493FE95D4065931BD6CDCCDA83A0EE2873276040615B63E`
- softpc64.exe: `417F37115812071C5C397F876078913E0F7F267E2420934DA287FDD6E9A09DBB`

S7 remains open for the owner's acceptance of this additional delivery. S8 is
not admitted and T93 remains open.

After executor 7bb98d7990c210bd2cced168364be68ef82b7286 was pushed, the same
agent switched to coordinator review. The actual 68-path commit was checked
against the admitted import/App/tool boundaries, deleted-path dispositions,
265-path source comparison, retained failure coverage, all 14 JUnit results,
zero exit marker and published hashes. HEAD matched origin/main and the
worktree was clean. No unadmitted mirror, Compat, INI or guest-asset change was
present. The first stale-object failure is not erased or presented as a
successful run; the full corrected qualification is the delivery evidence.
Review accepts the implementation for owner testing, not S/T closure or S8
admission. This subsequent P records that review only.

### S8 — XP MIDI/FM device attachment

The owner accepts S7 and explicitly admits S8 on 2026-10-16:
"验收通过，准入s8". This supersedes the prior withdrawal of automatic S8
admission during the shared import. Baseline is e1e44d0c. The initial checkpoint
rechecks source ranges, dispatch and lifecycle against that accepted baseline;
local implementation below is not completed qualification.

Import necessary nt_sbmid.c and nt_sbfm.c subsets, using Lib MIDI and the
explicit no-host FM policy. Keep one guest parser and fix tail/pointer bounds.
Preserve DSP MIDI command and MPU port scopes from XP, not a new intelligent
MPU or MIDI-input device. Explain kernel/user status differences above.
Estimated retained XP source +450-750/-0; local production +80-180/-10-30,
net +50-170; tests +250-450/-0; roughly 5-9 code paths plus mirror records.

Tests: isolated first/last MIDI message, running status, SysEx/realtime and
oversize boundaries, blocked output recovery without another guest byte,
reset/stop and volume errors. FM tests use guest port writes/reads to prove
selected XP fallback status, reset and no false host-output success.
Host MIDI may sound; FM on this host need not. No synthesis dependency.
Unexpected native errors are diagnosed once; missing optional capability
does not create a retry loop or a second fallback engine.

Initial receiving-path check confirms SbInitialize currently registers only
DSP/Mixer ports; its existing I/O tables remain the sole binding authority.
ResetAll currently resets Mixer and DSP. MixerGetMidiGain already provides
decoded gain, but has no MIDI host receiver. Lib MIDI already exposes complete
short/SysEx submission, event-driven capacity wait, cancellation, reset,
optional volume and destruction; Lib FM explicitly reports unsupported native
acquisition. These are implementation inputs, not evidence that guest MIDI/FM
is already attached. The implementation must extend these existing paths,
not introduce another registry, parser, synthesizer or device executor.

Finite S8 coverage units: (1) retained source and exclusions; (2) MPU/FM I/O
registration and conflicts; (3) complete/partial/running-status/realtime MIDI
parsing; (4) bounded SysEx and tail delivery under backpressure; (5) host
admission, terminal errors and volume; (6) pause/resume/reset/stop/destruction;
(7) selected XP no-host FM index/status semantics; (8) receiving dual-width
qualification and unchanged speaker/DSP behavior. Each needs a source owner
and focused regression before S8 can close. Portable active-device archives
remain S9, not an inferred S8 completion claim.

#### S8 local FM subset checkpoint

The retained nt_sbfm.c implements the selected XP no-host index/status behavior,
not native FM playback. The pinned file has 366 lines; the retained reference
selects 84 lines and excludes native NT bindings, port batches, unused timer
state and driver lifetime. The local body has 63 lines. Its local adjustments
are portable declarations and the active XP no-host transition rule: B0..BD
also set immediate-expiry flags, while only register 04 resets them. Kernel
source is research evidence, not an imported payload. The source ranges are
recorded in t93-s8-xp-sound-subset-ranges.tsv; the mirror manifest keeps raw,
selected and local hashes separately. sndblst.h restores original AD_MASK;
nt_sb.h keeps its S6 subset reference with four local portable FM declarations.

The isolated FM test covers 524,288 index/data/prior-status transitions,
including repeated reads and reset of the selected register. Both width
targets build and pass; ignored evidence is build/t93/s8/fm-r2.log and
fm.exit (0). The first build's missing AD_MASK error is retained in fm.log/
fm.err, not counted as passing. This proves the selected function semantics
only: it does not yet prove guest I/O dispatch, lifecycle, host capability
attachment or MIDI. No EXE was published and no S8 implementation P is ready.
All eight coverage units above remain required before delivery.

#### S8 local FM port checkpoint

The existing SbPorts binding list now holds absolute addresses, including
228/229 and 388/389; the same I/O-table preflight checks every binding before
any is installed. VsbByteIn/Out retain XP's alias remap and final FM dispatch;
there is no second port registry or state. ResetAll restores ResetFM before
the existing Mixer/DSP reset. nt_sb.c and sndblst.h selected reference ranges
now include those exact XP branches/constants; excluded native work stays out.

The existing device_contract_sb_commands regression now checks conflicts at
each of ten ports without partial attachment, cross-alias writes and reads,
the unsupported data-read result, idempotent attachment, DSP reset, teardown,
reattachment and machine cold reset. Both widths build and pass this case and
the exhaustive isolated FM matrix (2/2 per width). Evidence:
build/t93/s8/fm-ports-r2.log, fm-ports-x64.xml, fm-ports-x86.xml and
fm-ports.exit=0. The initial hidden launch vanished without logs or an exit
record; it is inconclusive, not a failed/passed test. Its missing process was
checked before a durable hidden launch. Ninja recovered its existing build
record and rebuilt additional targets; no cache purge was requested.

Cumulative local S8 C/H against admission, including new files: production
+111/-6 (net +105), tests +75/-6 (net +69). This is an unfinished checkpoint,
not the final mirror-diff or delivery account. Mirror body verification passes
504 files. Public components, package EXEs and owner assets remain untouched.
MIDI parsing/output/backpressure and complete sound lifecycle remain active
work; FM host capability admission and final qualification are not inferred
from these port tests.

#### S8 local MIDI parser checkpoint

nt_sbmid.c is now a 131-line device parser with one 4096-byte message assembly
buffer, original running-status/length responsibilities and one pending
realtime byte. It replaces the old pointer-narrowing CMDHDR ring and native
MIDIHDR packing, rather than introducing a Compat parser. The pinned 896-line
file's selected reference retains 446 lines, including the complete original
BufferMidi and SendMidiRequest bodies: their local replacement is deliberately
visible in the mirror diff, not hidden as excluded source. Native handle,
allocation, batch-copy and diagnostic sections are excluded with ranges.

The original send loop excluded LastCommand, delaying the final complete
message. The new single SendMidiRequest entry submits complete messages now,
clears them only after acceptance, and can be called without a new guest byte.
Running status is materialized for Lib's complete-message API. Realtime does
not consume partial-message bytes; undefined statuses are not sent as invalid
host messages. Oversized SysEx is consumed through EOX and returns UNSUPPORTED,
never a valid truncated message. New status aborts an incomplete message.

The 132-line isolated test covers every channel/status family, system-common
lengths/cancellation, each defined realtime status inside short and SysEx
messages, malformed interruption, maximum-size and three over-capacity sizes,
retained short/SysEx/realtime after host rejection, explicit service without
another guest byte, and parser reset. Both width targets build and pass 1/1;
evidence is build/t93/s8/midi-parser-r2.log, midi-parser-{x64,x86}.xml and
midi-parser.exit=0. The first build failed because the test helper name byte
collided with the original header macro; feed_byte fixes that fixture naming
error. The initial failure log remains evidence, not a passing run.

The 11-line Compat declaration defines complete-message submission only.
Its implementation and actual MPU dispatch are not yet present; the parser
is currently compiled by its isolated test, not linked into the production
device archive. In particular these fixture results do not prove native
capacity wake/cancellation, actual note output or pause/stop behavior. Those
remain mandatory S8 units before deployment. This checkpoint adds +147/-0
production C/H and +132/-0 test C; no shared component or user asset changed.
Mirror verification passes 505 bodies; no complete implementation P is ready.

#### S8 local MIDI host-adapter checkpoint

Compat sb_midi.c now owns the one Lib MIDI output and a lifetime mutex used
only to protect handle publication/destruction against cancel. It does not
own guest bytes, running status, an output queue or another worker. Complete
SysEx backpressure waits on the existing Lib completion event; complete short
messages use Lib's ordering operation. A cancelled submission leaves the
device message pending. The existing CCPU lifecycle exit request is the
authority; a stale auto-reset cancellation signal can be consumed without
inventing another requested-stop flag or timed polling.

Prepare/dispose require quiescent callers. Machine integration must establish
that lifetime before exposing stop callbacks, then quiesce them before disposal.
The reset/EOX wait is outside the lifetime mutex and cancellable. Native final
destruction is serialized against cancellation so a handle cannot be freed
under cancel; its unavoidable native-call behavior is not claimed interruptible.
Terminal errors still reach explicit Lib destruction, rather than leaking a
handle because ordinary reset rejects a faulted output. Failure reporting and
remaining ownership are distinct. Optional host volume is preserved/restored
only while still owned; a newer external volume becomes the restore baseline
before a later guest gain change, or is left alone on close.

The new host-fixture regression covers no-device and repeated-open admission,
direct short output, event-driven SysEx capacity recovery, cancellation inside
the wait with the mutex free, existing-exit rejection, stale cancellation,
reset, optional volume, external volume changes, close-failure retention and
cleanup after terminal failure. Both widths pass 1/1 after the final volume
case; evidence is build/t93/s8/midi-output-r2.log, midi-output-{x64,x86}.xml
and midi-output.exit=0. These are scripted host and synchronization calls,
not native playback or cross-thread integration evidence. Actual MPU dispatch,
machine preparation/cancellation/destruction wiring, pause/reset policy and
the final composed/native regressions remain mandatory.

This checkpoint adds 149 production C lines and 13 header lines (+162/-0),
plus 136 test C lines (+136/-0). It does not change shared components, package
EXEs or owner assets. The initial local-host estimate is being consumed by
explicit cancellation/lifetime/volume handling; final S8 accounting must report
the complete actual subset and integration cost, not just imported XP lines.

#### S8 MPU attachment and cleanup checkpoints

The production device archive now includes the MIDI parser and Compat output
adapter. The existing I/O table owns optional MPU 330/331 admission, UART mode,
ACK/reset and detach; occupied or unsupported optional ports do not steal an
existing registration. Mixer gain uses the same device owner and host output.
Native rejection after successful capability admission is an I/O failure,
distinct from an oversized guest SysEx. No public component changes are made.

The combined real-port/device and parser/output-fixture checkpoint passes four
tests per width: midi-attachment-{x64,x86}.xml, midi-attachment.log and
midi-attachment.exit=0 in the admitted S8 evidence directory. This proves the
bounded port and scripted-host cases, not native playback or complete lifecycle.

A further owner-local regression adds 19 test lines and no production change.
It checks that a writable-wait I/O error is returned without resubmission, and
that cancelled disposal does not destroy the output or its guard: reopening is
rejected, cancellation still reaches the retained output, and subsequent
quiescent disposal succeeds. Both widths pass 1/1 in midi-cleanup-{x64,x86}.xml;
midi-cleanup.log and midi-cleanup.exit=0 retain the build/test evidence. These
are scripted cancellation/ownership proofs, not a concurrent-thread stress test.

Remaining S8 work includes pause/resume note continuity, cancellation-failure
propagation, native cleanup qualification, explicit FM capability admission,
oversize-message diagnostics and the complete package regression/publication.
The checks above do not admit S9 or complete S8.

The cancellation-failure repair now passes focused qualification. Lib's
cancel-wait operation returns a failed event signal directly; it does not latch
that cross-thread error in its output. Compat retains the first such failure
under its existing handle mutex, and SbGetStatus reads it through the existing
executor failure path. This is host-output failure state, not a second guest
stop flag or retry mechanism. Fresh output admission/disposal ends that lifetime.
Cleanup remains permitted after a fault. No shared API is changed. Injected
tests cover sticky failure despite a later successful cancel, disposal, and
propagation through device status to machine-run rejection. The owned
midi-cancel background job passes 2/2 on x64 and x86; both JUnit files and
midi-cancel.exit=0 record completion. An event-signal failure is not claimed to wake an already
blocked native wait successfully; native-call termination limits still apply.

The next lifecycle seam was rechecked against the pinned XP nt_sbmid.c:
ResetMidiDevice (153 onward) waits for native headers and calls midiOutReset;
CloseMidiDevice invokes that reset before volume/handle cleanup. It has no
paired suspend/resume state restoration entry. Copying that operation into
pause would stop notes without reconstructing them. The standalone continuation
must use the single device-owned MIDI state and restoration policy already
required by the snapshot design, rather than a separate Compat parser or a
second pause-only replay log. This source finding is not implementation or
dynamic proof of pause continuity; that remains an S8 completion requirement.

#### S8 device pause-continuation checkpoint

nt_sbmid.c now retains successfully submitted logical notes and the sustain/
sostenuto relationships in the device owner. Pause stops host output; resume
reconstructs sostenuto-captured notes before later notes, restores sustain,
then releases keys which were only pedal-held. A failed/cancelled restore
leaves continuation pending; the next actual entry first resets host output
instead of duplicating a partially submitted prefix. The original parser and
its partial/complete pending message remain the only guest-byte path.

This is a bounded one-voice-per-channel/key approximation, not restoration of
native envelopes or multiple stacked voices on one pitch. Unknown vendor SysEx
and physical instrument state remain subject to the already disclosed host
MIDI limits. S9 must serialize this same device state, not create a second
pause/snapshot reconstruction implementation. Controller/channel-mode semantics
were checked against the MIDI Association's
[control table](https://midi.org/midi-1-0-control-change-messages) and
[message summary](https://midi.org/summary-of-midi-1-0-messages).

The expanded parser fixture passes 1/1 on both widths, including repeated
pause/resume, sostenuto capture ordering, sustain with All Notes Off,
controller reset, All Sound Off and cancellation after a submitted restore
prefix. Evidence: midi-pause-r2.log, midi-pause-{x64,x86}.xml and
midi-pause.exit=0. The parser implementation grows from 131 to 253 lines and
its test from 132 to 186 lines; these are explicit local continuation changes,
not additional imported XP source. Full S8 accounting remains required.

The existing heartbeat receiver now calls SbSetRunning for both DSP and MIDI;
resumption drains a retained complete message without waiting for another
guest byte. This wiring and its actual machine-entry regression pass four
focused tests on each width in midi-lifecycle.log and the refreshed
midi-attachment-{x64,x86}.xml, with midi-attachment.exit=0.

The added owner-local native integration test runs the real parser, Compat
and Lib WinMM implementation, with only CCPU's quiescent exit predicate fixed
false. It covers a first note, pause/resume, note-off, a universal identity
SysEx request, subsequent short-message ordering after cancellation and close.
Both widths pass, not skip: midi-native-{x64,x86}.xml, midi-native.log and
midi-native.exit=0. This proves native API/resource integration, not acoustic
output, concurrent CCPU execution or exact instrument restoration. No public
Lib files, PCM implementation or WASAPI backend change.

The final input-boundary edit reports oversized SysEx once at EOX via the
Compat diagnostic endpoint; it neither submits a truncated message nor faults
the host output. The actual-port test now asserts that diagnostic count and
unchanged host submission count. That edit still requires the receiving full
qualification; the earlier logs do not verify it retroactively.

#### S8 first full qualification and cleanup review

The remaining FM capability checkpoint is resolved by the actual public
contract: audio/fm/output_interface.h exposes acquisition only, explicitly
returns UNSUPPORTED/null on every current backend, and exposes no register
transport or output lifetime. SoftPC selects the approved XP no-host branch;
it does not acquire a phantom handle or add a wrapper around that constant
capability result. The existing Lib capability regression and actual FM port
matrix cover distinct boundaries. A future raw backend/transport is a separate
admission, not silently enabled by this device subset. No audible FM claim is
made. This clarifies the earlier pending capability-admission wording without
adding an unused production path.

The first full x64 build and shared suite pass (95/95). Other background
tests pass 64/65; machine/sound_smoke fails its first/second PCM frame ratio.
An isolated rerun fails too, so parallel host activity is not assumed causal.
A temporary endpoint probe records 3072 first-run frames before cold reset,
11264 after reset, and 3072 second-run frames at comparison. The first counter
includes the intervening host cleanup while the second does not. The probe is
removed; the test now counts the same first three complete blocks in both
executions, publishing each frame count before its delivery count. The existing
ratio assertions remain. Each width now passes ten consecutive executions
(sound-window-{x64,x86}.xml and sound-window.exit=0);
the initial full-suite failure remains retained evidence, not a passing run.

Actual-change review also identifies a device lifecycle gap: SbSetRunning
skips MIDI pause cleanup when MidiStatus is already faulted, and does not check
the retained cancellation failure before restoration. Repair must preserve
normal pause continuity but use existing host close cleanup for a faulted
stop, retain error reporting and prevent failed output from replaying notes.
The repair stays in SbSetRunning: fold in the existing Compat cancellation
status before replay, preserve normal pause/reset, and use the existing close
operation when stopping a faulted output. No new state or shared API is added.
The added actual-machine test fails before the repair at the still-open host
assertion (midi-fault-before.xml), and verifies cancellation-fault and native
submission-fault stop/close plus rejection of subsequent replay. The
device/parser/Compat group passes 3/3 on both widths after repair
(midi-fault-after-{x64,x86}.xml, midi-fault-after.exit=0).
This is not completed S8 qualification. The full-suite failure and diagnostic
logs remain separate from subsequent passing targeted evidence.

The second full x64 run passes 95/95 shared and 63/65 other tests. Both failed
integrations wait for STOPPED after PAUSED. An isolated repeated input test
reproduces MIDI INVALID_STATE (2), exit predicate false, followed by machine
ERROR (4), not a timeout of a correctly running stop. The public stop operation
wakes the parked executor before calling its driver stop signal. On that legal
interleaving the callback returns without resuming MIDI, and the existing
platform service executes before CCPU exit is signalled. SendMidiRequest had
incorrectly classified idle paused service as a host failure.

The repair makes paused service return OK without submitting anything; the
BufferMidi guest-byte entry explicitly rejects paused admission, preserving
pending data. Existing MidiRunning remains the sole state. Actual-device tests
now service between heartbeat-disable and re-enable, and parser tests reject a
new paused byte. No Emulator ordering, second cancellation flag or retry loop
is added. Temporary result/state prints are removed. Focused x64 2/2 passes;
both affected integrations are being repeated twenty times per width before
another complete qualification. Initial probe compilation lacked stdio for
the temporary print; the corrected probe built and reproduced the failure.

The repeated integration campaign has a 3600-second total observation budget
and the existing 30 MiB log limit, distinct from each ordinary 1200-second
qualification suite. Its first real DOS restart case completes in 66 seconds;
twenty repetitions on both widths are intentionally not treated as a short
interactive check. The owned hidden job and child CTest/test processes are
observed directly; slow boot progress is not a reason to kill or restart them.

Similar-issue review searches SendMidiRequest, MidiSetRunning and SbService
throughout app-softpc. There is one production service caller in platform.c,
one device dispatch in nt_sb.c, and the existing parser/heartbeat entries;
there is no second MIDI dispatch to repair. DspServiceTransfer already returns
OK when inactive/paused and keeps its existing behavior. Guest-byte admission
remains separate from idle service. The deterministic actual-device regression
exercises that distinction without relying on reproducing the thread timing.

#### S8 repeated stop proof and third qualification

The stop-fix campaign completes successfully (exit 0): both affected real
integrations pass twenty repetitions each on each architecture. Its x64 and
x86 integration reports record zero failures; the deterministic device/parser
pair also passes on both widths. This closes the specific paused-service
regression proof, not the complete S8 qualification.

The automatically following third full run passes x64 build and 95/95 shared
tests, then 64/65 other tests. checkpoint_smoke is classified as Timeout with
no diagnostic output. Its configured budget remains 15 seconds; the two prior
full runs completed it in 11.87 and 10.23 seconds. Ten unchanged isolated
repetitions pass in 66.93 seconds total. An unchanged full 65-test parallel
rerun passes in 97.34 seconds, including checkpoint_smoke in 9.59 seconds.
Neither result establishes the timeout root cause. Native MIDI reset is now
part of its repeated machine-reset path, but attributing the failure to that
host call without a stack/timing trace would be speculation.

The failed run, isolated report and parallel rerun remain distinct evidence
under qualification-r3. No timeout, test assertion or production behavior was
changed to obtain the rerun. A fourth complete dual-width run is in progress;
no complete pass or resolved timeout is claimed at this checkpoint.

#### S8 complete fourth qualification

The fourth hidden job completes with qualification.exit=0. Each architecture
passes 165/165: 95 shared, 65 other background tests, and five individually
scheduled native desktop cases. All fourteen JUnit reports have zero failures
and zero skips. Both native MIDI integration cases execute rather than skip;
this is API/resource-path evidence, not a claim of audible listening or guest
Windows-driver acceptance. The user INI hash check succeeds and only the two
package EXEs change under assets.

The x64 checkpoint_smoke completes in 8.86 seconds in this full run. The prior
timeout remains unexplained and is not relabelled a pass or a fixed defect.
No product workaround, timeout enlargement or test deletion was used. Final
coordinator review must consider this retained qualification limitation.

Artifact SHA-256:

- softpc32.exe: A059BAF16D5A630186FEC46895E7ACAF12E19BC2C593E464B8E27FE544D6BA35
- softpc64.exe: BA980E0F46C63BF7F1DD86F872BA08E05102727DCE36368597D67F2E74C473E8

Ninja reports premature-end-of-file recovery during x86 build and recompiles
existing objects. No clean/restart was requested; this is not evidence of
perfect cache reuse. Public eight source/test directories remain unchanged.
Executor commit/push and coordinator actual-change acceptance are still
pending; S9 active sound-state serialization is not covered by these passes.

Finite-unit exit evidence (the eight units declared at admission):

| Unit | Retained owner and proof |
| --- | --- |
| 1 Source subset | Fixed XP range ledger and 505-body mirror gate; excluded host/kernel code separately accounted. |
| 2 Port ownership | nt_sb.c sole I/O registration; actual-device MPU/FM aliases, conflict rejection, reset and detach regression. |
| 3 MIDI parser | nt_sbmid.c sole parser; partial/running-status/realtime and complete-message assertions. |
| 4 SysEx/tail | Parser bounds/retained-tail tests plus Compat capacity-wait/cancellation fixture and executed native integration. |
| 5 Host ownership | Compat creation, volume ownership, reset/destroy failures and retained cancellation error tests. |
| 6 Lifecycle | Device/parser pause/pedal restoration, faulted-stop cleanup, and both real integrations repeated twenty times per width. |
| 7 FM semantics | Selected XP no-host branch; exhaustive 524288 transitions and real device-port checks; no audible FM claim. |
| 8 Receiving qualification | Fourth complete run: 165/165 per width, no skips, ten native desktop cases, refreshed package EXEs and unchanged INI. |

All eight admitted units have passing evidence. The third-run checkpoint
timeout is retained as an explicit test-reliability debt, not silently fixed
by a retry or used to assert universal scheduling robustness. Its owner and
investigation trigger are recorded in TODO. It does not replace S9's still
required active-device archive work.

#### S8 changed-path accounting before final qualification

Against admission fc5469c4, counted with git diff --numstat plus complete new
C/H files (not omitted as untracked), production is +715/-14, net +701;
tests are +599/-8, net +591, including the paused-service repair.
Documentation, manifests, CMake and EXEs are
excluded from these code totals. Production ownership is:

| Owner | Added | Deleted | Net |
| --- | ---: | ---: | ---: |
| SoftPC mirror, six paths | 501 | 11 | +490 |
| Compat, three paths | 205 | 3 | +202 |
| Machine, one path | 9 | 0 | +9 |

Six test paths comprise actual port/lifecycle coverage (+148/-6), the existing
sound measurement correction (+6/-2), FM matrix (+47), parser (+187), Compat
host fixture (+164), and native MIDI integration (+47). The local cost exceeds
the initial Compat estimate because cancellation, host-volume ownership and
cleanup are implemented and tested explicitly. There is no shared-eight
production/test change, second parser, native worker, MIDI queue or synthesizer.

The six touched mirror files compared with their pinned necessary subsets are
listed below. These are **current cumulative local patches**, including S6's
already accepted DSP adaptation, not six new S8 patch costs. The reconstructed
subset SHA-256 values match the manifest. S6 ranges use xp_first/xp_last; S8
ranges use start/end. Keeping final empty source lines is necessary for exact
subset identity; preliminary scratch reconstructions were not accepted counts.

| Mirror path | Added versus XP subset | Deleted versus XP subset |
| --- | ---: | ---: |
| base/inc/sndblst.h | 18 | 15 |
| host/inc/nt_sb.h | 83 | 63 |
| host/src/nt_sb.c | 124 | 275 |
| host/src/nt_sbdsp.c | 647 | 1428 |
| host/src/nt_sbfm.c | 24 | 45 |
| host/src/nt_sbmid.c | 224 | 413 |

Total cumulative patch is +1120/-2239, net -1119. The four S8 range ledgers
select 1172 of 2455 upstream lines, excluding 1283 separately: sndblst.h 48,
nt_sb.c 503, nt_sbfm.c 282, nt_sbmid.c 450. Excluded NT/native loading,
batching, unused commands and diagnostics are not counted as local functional
deletions. The S6 header/DSP subset remains unchanged; S8 adds declarations and
one Mixer forwarding call there. Final tests/publication/review are still
required; this accounting alone does not qualify S8.

### S9 — shared-component platform neutrality

Owner-directed reorder after S8 closure:

> 先不要转入S9.S8收口后停下来，准入一个新的公共组件的平台中立化审计和修复S任务，S9顺延到S10

The archive admission in 3f6e7cb3 is superseded before any archive implementation.
S8 stays closed. Earlier S9 references in retained checkpoint narratives mean
the former archive step, initially S10 and now S15; they do not authorize concurrent work.

Audit the eight public directories: src/test for lib, emulator, x86-debug and
x86-ibmpc. Include public declarations, neutral implementation, native leaves,
build/test registration and component documentation. Fixed coverage units:
(1) exported host types/contracts; (2) native function/header ownership and
Types authority; (3) inward dependency/product independence; (4) platform
selection and unsupported semantics; (5) platform assumptions in tests;
(6) duplicated platform paths or unnecessary abstraction; (7) build/package
portability declarations; (8) receiving regression and artifact qualification.

Each finding must distinguish legitimate platform implementation from leakage.
Windows backends need not become platform-free; x86 guest semantics are not
the same as host-platform binding. Do not replace WASAPI/WinMM, rename PCM to
a backend name, invent silent fallback success or build another state owner.
NXVM remains read-only comparison material, not a build dependency.

Start with read-only inventory and report concrete repair targets, examples,
affected files and added/deleted/net estimates before production edits. Scope
and line estimates are pending that inventory, not invented at admission.
Prefer removing leaks/duplicates within existing ownership over adding a new
generic framework. Preserve behavior and test all affected callers. Delivery
requires cached x86/x64 builds, all shared and receiving tests including native
desktop cases, refreshed EXEs, executor push and actual-change review. No new
platform runtime support is claimed without running it. The owner subsequently
approves splitting this work: "好 请拆分为多个s任务". S9 is read-only audit and
design; S10-S14 below are planned repairs, not simultaneous admissions.

#### Approved repair breakdown and evidence boundaries

S9 read-only audit disposition: Console native records and ambiguous KVM physical
identity map to S10; path encoding and platform directory rules map to S11;
Console/Window string interpretation maps to S12; PCM progress/lifetime maps
to S13; capture obligations and final Emulator/X86 review map to S14. These
seven requested areas form the finite design-audit universe. MIDI's documented
device-wide volume and FM's truthful UNSUPPORTED need no new implementation.
No broad Emulator/X86 native API leakage was established by the inspected
interfaces and token scan; assembly DWORD strings are guest semantics.
These are source observations, not dynamic reproductions or proof of future
repairs. Existing capture failure tests must be retained rather than duplicated.
S9 changes no production/test code (+0/-0, net zero), mirror or user configuration,
media and snapshots. Build publication is accounted separately below.

#### S9 executor qualification

Cached dual Release builds pass. x64 Ninja reports no work; x86 rebuilds and
publishes softpc32.exe. Each width passes 95 shared, 65 remaining unit/integration
and five native desktop cases: 165/165, zero failures/skips. Fourteen JUnit
reports and terminal exit 0 were inspected. This is receiving-baseline proof,
not implementation or playback proof for any S10-S15 change. No Computer Use.
An initial runner failed before build because Windows PowerShell lacked
Get-FileHash; the installed PowerShell 7 runner completed the full qualification.
The launch failure is not counted as a test failure or hidden retry of a test.

Published SHA-256:

- softpc32.exe: 60E60848AD7E0112295D335F16EE58D5761086B894046CA5E7D7F9B309B14BB8
- softpc64.exe: BA980E0F46C63BF7F1DD86F872BA08E05102727DCE36368597D67F2E74C473E8

Only the x86 artifact differs from the baseline; x64 is already current.
User INI hash is unchanged. Production/test code +0/-0, net zero; no mirror
diff. Current planning changes, S8 reorder annotation and this evidence are
delivered together. S9 awaits owner acceptance; S10 must not auto-start.

Coordinator review after executor 896aa2fd push: inspected the actual four-path
diff, original requests and scope, all fourteen JUnit reports, terminal exit,
both artifact hashes and HEAD/origin identity. Three documentation files plus
one rebuilt x86 binary changed; no production/test source, user INI, mirror,
media or snapshot change. Documentation is +211/-19, net +192 in that P;
binary size is unchanged. The plan separates input, paths, text, PCM and capture,
keeps unsupported capabilities honest and explicitly awaits owner acceptance.
No S10 implementation is admitted by this review. T93 remains open.

Owner delivery override: "好 批准照此办理 每个s任务完了都要编译测试提交推送后等我验收 不得自动准入下一条".
This supersedes earlier delegated sequential admission. Each S, including the
current design delivery, is built/tested and pushed before waiting for owner
acceptance. No following S starts automatically.

All repairs are limited to the eight public source/test directories and their
configuration/documentation. No app-softpc changes or new OS backends. S9
findings are source evidence, not dynamic reproductions. Native implementation
is legitimate; neutral public values, lifetime and caller duties are the target.
Emulator/X86 need no speculative production rewrite. Each implementation starts
with an actual file/caller census and refined added/deleted/net budget, then
finishes with owner-local tests, same-class sweep, Types/dependency/manifest
checks, cached dual builds, both EXEs, shared tests first, remaining unit and
integration tests, all ten native desktop tests, commit/push and actual-change
review. No Computer Use, foreground long jobs or timeout-based termination.
Estimates below exclude documentation, manifests and artifacts; they are design
budgets, not measured diffs. Later admission must resolve behavior choices before
editing, rather than treating this breakdown as approval of silent changes.

### S10 — neutral Console and KVM input

Owner admission after S9 acceptance additionally permits necessary App input
adaptation ("可以改softpc app啊"). For S10 only, this supersedes the earlier
no-App-edit constraint: migrate real consumers, not a compatibility alias layer.
The initial symbol census finds 26 production/test files using common key or
mouse vocabulary, including two App production input consumers. Refined
production budget is +300-450/-250-400 (paired net about +50), plus mechanical
consumer renaming; tests +180-300/-60-100. Account relocation/renaming separately.
No broad App platform migration, guest behavior change or mirror edit is admitted.

Own Console raw-key/raw-mouse contracts and KVM physical-key semantics together.
Current Console publishes native VK, UTF-16 units and native mouse masks;
KVM Console then normalizes them. Move conversion to the existing platform
boundary, reuse normalization rather than duplicate it, and locate shared
declarations without Console depending on KVM. Define Unicode scalar delivery,
repeat count, cell positions, neutral buttons, PC physical encoding and unavailable
physical identity. Preserve left/right modifiers, keypad, hotkeys and reset.
Audit special Pause/Print Screen representation before claiming full equivalence.
Files: Console interface/broker, Types input declarations, KVM input adapters
and Console receiver, corresponding fixtures; roughly 10-15 production paths.
Budget: production +180-300/-150-250, paired net +30-50;
tests +180-300/-60-100, net approximately +120-200.
Proof: surrogate pairs/malformed sequences, repeats, modifiers/keypad, no-physical
text, buttons/cell motion and reset through actual producer/consumer boundaries.
Public raw records may change; existing logical behavior must not silently change.

#### S10 implementation and finite review ledger

The frozen scope is neutral input values, their two native producers, the KVM
delivery entry and every source/test consumer found by the old-symbol census.
Each member is either migrated with focused proof or unchanged with a stated
reason. Completion requires this ledger, both full suites and actual-change
review; it does not claim completion of the separate path/text/audio tasks.

- Types owns the common key/button vocabulary and `lib_key_input`. The old
  key/button enum body is byte-identical after prefix substitution: no values
  are reassigned. Source consumers rebuild together; no aliases are retained.
- Base owns the relocated selected-platform key/layout decoder and the single
  UTF-16 decoder. The adapter record is explicitly internal native Lib support,
  not an application input contract. It does not depend on KVM or Console.
- Console-broker decodes before public delivery and translates native buttons
  to neutral bits. Mouse fields are absolute screen-buffer cells. Existing
  KVM support remains left/right only; ignored middle-button behavior is not
  silently expanded. Focus reset and binding activation discard partial text.
- KVM owns complete-scalar repetition, existing physical text synthesis and
  the existing held-key ledger. Console no longer owns a surrogate decoder or
  native-button conversion. Window reuses the same Base decoder before the
  same neutral KVM delivery path; no second input state machine is introduced.
- Emulator's change is the extended-bit symbol only. App input consumers use
  the moved vocabulary, retaining the existing guest-key/relative-mouse path.
  X86-IBMPC's injected chord must emit a single-byte physical code and a
  separate E0 flag, matching actual native input, not duplicate E0 in the code.
- Linux's existing terminal decoder reports unavailable physical identity as
  zero and retains TEXT fallback. This is tested as a decoder contract, not
  advertised as a newly implemented Linux presenter.

Focused fixtures cover real broker output (BMP, paired and malformed UTF-16,
repeat metadata, physical breaks, right Ctrl, reset and binding handoff), KVM
Window/Console equivalence, physical sides/keypad/Pause/Print Screen, no-scan
virtual-keyboard recovery, failed sink delivery and mouse position/buttons.
The special-key matrix uses actual native decoder inputs, not a second key map.

Similar-issue scans cover all `src`/`test` input symbols, physical-code writers,
adapter includes and native decoder calls. Old raw VK/UTF-16 fields and native
mouse field names are removed. Only Base implements native key/layout queries;
only broker and KVM native-leaf support invoke the shared decoder. The retained
KVM hotkey enum references canonical Types values for its existing registration
API, rather than defining a competing encoding. Base dependency and Types
ownership gates follow the move. No audio, mirror, INI, media or snapshot changes.

The first build found a fixture still accessing removed Console decoder state;
it now models the broker-owned decoder. Eleven focused x64 tests then passed.
The next build found an undeclared test-only keypad constant; the fixture now
uses the declared contiguous native keypad range. Both failures are retained
as failed attempts, not qualification. Review also added decoder reset at the
existing activation/flush boundary before any new reader starts.

#### S10 counted changes

Against reviewed S9 `449c0646`, using `git diff --numstat --no-renames` and
counting production/test C/H separately from CMake/gates, documentation,
manifests and artifacts. Moves count as deletion plus addition; symbol-only
consumer edits are not hidden. These figures are not new-logic line counts.

| Component | Production + / - / net | Test C/H + / - / net |
| --- | --- | --- |
| Lib | 501 / 493 / +8 | 235 / 148 / +87 |
| Emulator | 2 / 2 / 0 | 12 / 12 / 0 |
| X86-Debug | 0 / 0 / 0 | 0 / 0 / 0 |
| X86-IBMPC | 15 / 15 / 0 | 10 / 10 / 0 |
| App-SoftPC | 24 / 24 / 0 | 11 / 11 / 0 |
| Total | 542 / 534 / +8 | 268 / 181 / +87 |

There are 28 production C/H paths (including both sides of moved paths) and
16 test C/H paths. Three source CMake/gate files add 9/delete 11, net -2;
two test CMake/gate files add 6/delete 6, net zero. Mirror delta is zero.
Most gross edits move/rename existing declarations and platform decoding.
The extra neutral delivery entry replaces Console's native reconstruction;
the old decoder, private declarations and mouse conversion are removed from
their former owners. The only added persistent state is the relocated existing
two-field decoder in the native broker instead of KVM Console, not another
simultaneous state owner. KVM Window retains its existing per-source decoder.

#### S10 executor qualification

The final cached Ninja builds refresh both package executables. Each architecture
passes 165/165 with zero skips: 95 shared background tests, 65 other unit/
integration/gate tests and five serial native desktop cases. Shared suites run
first; no Computer Use, timeout killing, skipped failure or raised budget was
used. Final shared wall time is x64 77.39 s / x86 59.93 s; other suites are
x64 103.77 s / x86 91 s (JUnit integer). The earlier complete qualification
also passed 165/165 per width; after final modifier/chord cleanup the entire
qualification was rerun rather than reusing that earlier result.

Evidence is the ignored, bounded S10 final qualification log/exit/JUnit set.
Ninja again reported its existing premature-log recovery on x86 and rebuilt
unchanged targets; no manual cache purge occurred. This task does not claim
to repair that build-cache issue. Initial failed builds remain recorded above.
Final documentation review corrected the precise decoder reset ordering:
binding activation clears it before the new reader, not necessarily before
the logical reset callback. Only README/manifest changed after the full run;
metadata and documentation gates are rerun before commit.

Published SHA-256:

- softpc32.exe: `621DD465F8EB31E773BE153526E7E3DA3A286B56F108F462727069ACFD0CDD81`
- softpc64.exe: `D94BCB10D4ACF396E90785CF7ED97EA40B8EEA1F158A85178B82A5653293514C`

The changed Emulator path was separately checked: it only substitutes the
canonical extended-bit name in the same source-local release match. No native
API/type, platform branch, new state or dependency was introduced there.
All four public source/test pairs retain inward dependency, naming and Types
authority gates. App's two production edits are vocabulary substitutions only.
The special-key and RDP-style no-scan cases are injected native-record proofs,
not a claim of manual testing on every physical/RDP device. Existing actual
guest runtime/snapshot/package integrations passed; no new Linux presenter or
all-platform capability parity is claimed. At that delivery S10 awaited owner
acceptance and S11 was unadmitted; the later owner acceptance supersedes this.

#### S10 coordinator actual-change review

After executor `e7d37b65` was pushed, coordinator review checked its actual Git
delta against S9 `449c0646`, the owner input brief/App authorization and the
finite ledger above. It verified the relocated decoder and deleted former
owners, canonical enum values, native/public boundary, reset before reader
startup, neutral Console delivery, chord physical-code normalization, test
coverage and exact artifact hashes. The changed Emulator body remains only
the equivalent flag name; no native obligation moves outward. The inspected
App/mirror/audio/config/media boundaries match the stated scope.

Both final JUnit sets report 95 shared + 65 other + 5 desktop, zero failures,
disabled or skipped cases. A final documentation-only clarification and its
manifest passed an additional three metadata/documentation tests on each
width. The production/test C/H counts above match the committed delta;
no executable code changed after qualification. The source/test interfaces
must be imported together by other repositories; no compatibility aliases or
unrelated platform implementation are claimed. This accepts S10's delivery
for owner testing. That was the delivery state; the owner subsequently accepted
S10 and admitted S11 through closure. See the [S10 closure](../history/M9-T93-S10-neutral-input.md).

### S11 — UTF-8 paths and native directory rules

Admitted after S10 owner acceptance at 45d6f860. Finite convergence ledger:
Base executable-directory entry (Windows and Linux), Storage existing-file open
(reader/owned/medium), and Storage writer truncate/append. Every entry must use
UTF-8, preserve its existing ownership/locking semantics and have focused proof.
Windows conversion uses Types-owned native API vocabulary; Linux validates UTF-8
bytes and interprets slash only. No path framework or outward dependency.
Self-review refinement: the shared scalar validator belongs to Base, not Types.
Storage explicitly depends inward on Base rather than duplicating validation;
the exact dependency gate and its exhaustive edge self-test are updated.
This refines the initial no-new-edge estimate without adding a component,
state, recovery path or platform adaptation in an outer layer.
The first dual-width run passed 166/166 each, including ten desktop cases,
before this ownership correction; it is retained as preliminary proof, not
qualification of the final layout. Final qualification must be rerun.

Similar-issue census uses the production Base/Storage opens and directory
entry as the frozen universe: the obsolete Base ANSI module-path declaration
is removed; Storage's ANSI reader and root narrow writer are replaced; Linux
retains native byte opens after UTF-8 validation. Reader status now forwards
INVALID_ARGUMENT/NO_MEMORY rather than collapsing every open failure into I/O.
All owned-byte, medium and append/truncate routes reach these same native
boundaries. Static negative checks reject restoring implicit Windows code-page
opens. Native Console handle names and S12 text/title APIs are not file-path
entries and are unchanged. App config uses Base plus Storage already, so no
mandatory App edit was found. Non-ASCII Console command input is still S12's
encoding boundary, not an end-to-end claim made by S11.
Failure outputs remain unchanged/null as their existing contracts require.
Completion requires every entry's focused proof and complete dual-width cached
qualification, artifacts, pushed delivery and separate actual-change review.
Malformed input is INVALID_ARGUMENT; an executable with an unrepresentable name
is UNSUPPORTED; insufficient path capacity is LIMIT_EXCEEDED. ASCII behavior
is preserved; old code-page byte strings must now be UTF-8. S12 text APIs are
explicitly outside this finite path ledger. Estimate remains production net
+100–140 and tests net +100–160; count actual C/H separately at delivery.

Own Base executable directory and Storage read/write path interpretation.
Current generic directory extraction applies Windows rules to POSIX; reader
uses CreateFileA while writer uses narrow fopen. Define UTF-8 path bytes and
capacity/failure contracts, put native root/separator handling in existing
platform leaves, and apply the same encoding to reads and writes. Binary file
contents remain untouched. Explicitly limit POSIX non-UTF-8 name support.
No general path framework or App configuration repair.
Files: Base process, Storage reader/writer/platform files, private Types adapters
and tests; roughly 7-10 production paths. Production +180-280/-80-140,
paired net +100-140; tests +120-200/-20-40, net approximately +100-160.
Proof: Windows drive/UNC roots, POSIX slash roots and backslash names, Unicode
paths, malformed UTF-8, byte capacity, unchanged failure output and read/write
roundtrip. ASCII behavior stays; old ANSI bytes are a disclosed compatibility
change. Any mandatory App migration is a stop/report condition, not scope creep.

#### S11 executor qualification

All entries of the finite path ledger are implemented and verified:

| Entry | Single owner and proof |
| --- | --- |
| Windows executable directory | Base native wide query, strict UTF-8 conversion and native root parsing; drive/UNC/extended mixed-case UNC roots, multibyte byte capacity, malformed native surrogate and unchanged failure output. |
| POSIX executable directory | Actual Linux leaf under controlled readlink; slash roots, literal backslashes, UTF-8 names, capacity and non-UTF-8 rejection. No Linux-host runtime claim. |
| Existing-file reads and media | Storage native open preserves sharing/locking and stream ownership; reader, owned bytes and all three medium modes pass Unicode and binary-content checks. |
| Writer truncate/append | The same Storage path conversion feeds wide native binary opens; truncation/append and readback pass. An independent native wide-name open prevents symmetric wrong-encoding false positives. |
| Invalid encoding and dependency boundary | Malformed UTF-8 matrix, scalar boundaries and null-output checks pass; Base owns validation, Types only native declarations. Exact dependency tests admit Storage-to-Base, not a reverse/outer edge; negative tests reject narrow native path regressions. |

Final cached Release builds and publication passed on x64/x86. Each width
passed 96 shared, 65 other unit/integration and five native desktop tests:
166/166, zero failures, disabled or skipped cases. JUnit background durations
are x64 shared 32s/other 100s and x86 shared 33s/other 87s. Desktop tests ran
serially after both background suites without Computer Use. The retained first
full run also passed, but does not substitute for this post-refinement run.
Ninja again reported its previously observed premature-log recovery on x86;
this caused a larger rebuild and is not claimed repaired by S11.

Reproducible C/H accounting uses `git diff --numstat --no-renames 45d6f860`
with new tracked files included, separating generated binaries and metadata:

| Scope | Paths | Added | Deleted | Net |
| --- | ---: | ---: | ---: | ---: |
| Lib production C/H | 16 | 199 | 45 | +154 |
| Lib test C/H | 3 | 160 | 39 | +121 |
| Lib source build/gates | 3 | 8 | 3 | +5 |
| Lib test build/gates | 2 | 20 | 2 | +18 |
| Emulator, X86, App and mirror code | 0 | 0 | 0 | 0 |

The production net is 14 lines above the upper initial estimate: explicit
Base validation ownership/interface, native conversion failure checks and
complete extended-UNC root handling are retained instead of duplicate or
implicit paths. There is no worker, recovery state, platform fallback or path
framework. No user INI/media/snapshot was changed. The old generic Windows
directory parser and narrow writer route were removed from neutral roots.

Published SHA-256:

- softpc32.exe: `4BE23D708F0DB7319A5A5CD6190173450098AC8DA0C91DF769CBAF109BFBF3A6`
- softpc64.exe: `BF5E790708E41A8015E5B8B0E3EB4766096BDC3E267842870AC2C192362D7A18`

Compatibility is deliberate: ASCII names retain behavior; old ANSI-encoded
non-ASCII path bytes must be converted to UTF-8. POSIX non-UTF-8 names remain
outside the supported contract. S12's Console text/title encoding and broader
App portability are not claimed complete. No new deferred defect was found
inside this finite path ledger. Executor delivery precedes coordinator review
and owner-authorized closure; T93 remains open and S12 is not admitted.

#### S11 coordinator actual-change review and closure

After pushing executor 33a9c9f7, the coordinator reviewed its actual committed
delta against 45d6f860, not only the executor summary. The review traced
temporary allocations and all conversion/close paths, verified that reads
retain their original lock policy and writers retain binary append/truncate,
checked unchanged-output/null-output contracts, platform-local directory
rules and the explicit Storage-to-Base dependency. It inspected the native
wide-name oracle, controlled POSIX leaf tests, negative gates, complete JUnit
inventories and committed EXE hashes. No narrow fallback, second file route,
new state/worker, outward dependency or protected-file change was found.

All finite-ledger entries have their focused proof plus final 166/166 per
architecture. Production C/H counts match the committed +199/-45; tests match
+160/-39. Linux source-path proof is controlled, not a full Linux product run;
S12 text and old-ANSI compatibility limits stay explicit. Review accepts and
closes S11 under the owner's "准入执行S11到收口" authorization. T93 remains
open, and no S12 implementation is admitted. Closure changes documentation
only and does not replace the tested executables.

### S12 — UTF-8 Console text and Window titles

Owner admits S12 from 79eca79f through pushed delivery, waiting before closure.
After the read-only simplification review, the owner adds two bounded Base
cleanups: inline the one-use POSIX directory trim using readlink's length and
remove Windows' redundant allocation bound under INT32_MAX/two-byte WCHAR.
No new path abstraction. The frozen implementation ledger is Console write,
cooked complete-line read, initial/queued Window title, Session chunk boundary,
Base span validation and these two path cleanups. Existing raw BMP frames,
native named handles, file content, audio and capture remain unchanged.

The output implementation retains one temporary conversion allocation rather
than adding a chunk decoder. Bindings share one cleanup exit. Cooked input
collects one bounded native line before conversion: a native fragment can split
a surrogate pair, but overlong/malformed lines are drained and rejected once.
UTF-8 byte capacity remains 1023 excluding NUL/CR/LF. Window reuses Base's
validator before enqueue; its explicit inward Base edge replaces no owner.
Session's existing LF normalization only flushes between complete scalars and
retains an absolute buffer bound even for invalid continuation sequences.
No new thread, persistent decoder, recovery or fallback path exists.
Estimate after review: production net approximately +58, test net +120-180;
actual accounting at delivery. Proof uses real native conversion with injected
I/O plus owner-local API checks, then complete cached dual-width qualification.

Own text/line/title encoding, reusing S11's necessary conversion primitives
without a second converter. Replace locale-dependent public interpretation at
the existing Windows boundaries; do not mechanically replace internal ASCII
device names or convert arbitrary byte payloads. Define byte capacities and
partial-operation/error semantics before implementation.
Original broad estimate (superseded by the bounded review above): Console
broker, Window, Types and tests; production net +100-160. The implemented
scope additionally makes the existing Session chunker obey the new contract,
without adding a converter or a second output route.
Proof: ASCII, non-ASCII and supplementary scalars, malformed input, conversion
capacity boundaries, line completion and output errors. Shared callers must
not select ANSI versus Console code pages. No App edits.

### S12 implementation and review record

#### Request and boundary

Owner: "好的。准入下一个S任务执行到收口前等我验收。"
Refinement: "可以。就这么改，就在当前S里面改。然后当前S的本分任务，继续做，精简的做".
Baseline: 79eca79f. S12 delivers for acceptance; it does not admit S13.

The approved simplification audit adds only two S11 path cleanups: inline
Linux's one-use directory trim using the readlink length, and remove Windows'
redundant allocation bound under INT32_MAX and two-byte WCHAR. Together these
production changes are +8/-13, net -5. Platform-specific path interpretation
remains in its platform implementation; the public output transaction stays.

#### Implementation and finite convergence ledger

| Frozen unit | Single owner and proof |
| --- | --- |
| Console text write | Logical Console validates the exact UTF-8 span; broker converts once into temporary native storage and writes through the existing locked binding. API rejection, empty write, native units, stale binding and partial failure are tested. |
| Cooked line read | Existing reader collects a bounded native line and converts after LF; no incremental decoder or new reader. Fragmented surrogate pairs, ASCII/UTF-8 byte limits, malformed UTF-16, empty input, drain and cancellation are tested. |
| Initial/queued title | Window validates before admission, both native paths use one title conversion helper. Invalid input cannot enqueue; UTF-16 content and existing startup/queued failure paths are tested. |
| Session text chunks | Existing LF normalization flushes only between UTF-8 scalars; malformed continuation runs retain a hard buffer bound. Boundary offsets 1017-1024 and invalid runs are tested. |
| Base UTF-8 span | One validator supports bounded spans; the NUL interface delegates. Exact nonterminated spans, truncated scalars, empty/null and embedded NUL cases belong to Base tests. |
| Base path simplification | Existing path tests cover both platform rules; a two-byte native-unit assertion documents the removed redundant bound. |

Public byte capacities are unchanged: cooked input has 1023 bytes excluding
NUL/CR/LF; titles have 128 bytes including NUL. Public text is now explicitly
UTF-8 instead of implicit host code pages. Old non-UTF-8 callers are rejected,
not silently reinterpreted. Native partial output returns I/O error without
rollback or replay. Native conversion remains exclusively in Lib; Session
does not learn an OS API. No new worker, persistent state or fallback exists.
Unsupported Linux presenters remain unsupported; no new platform is claimed.

#### Similar-issue sweep and review

Search scope: all four public source packages, using rg for ReadConsoleA,
WriteConsoleA, SetWindowTextA and their Types aliases, plus public write/title
consumers. All three affected native production sites now use wide APIs.
The remaining matches are rejection patterns in the permanent Types gate;
negative fixtures cover the added aliases. Existing raw input/frames already
use wide records and remain unchanged. Fixed ASCII native handle names are
not public text and remain unchanged. The existing cancellation wake injects
only an ASCII Enter record, so its native A alias is not a text-conversion
path; unused Types vocabulary is not a second implementation. File paths were qualified in S11;
binary contents, audio, capture and guest character maps are not converted.
The existing Session chunker was the affected outer text consumer; its split
boundary is repaired in place rather than adding another output path.

Self-review checks Lib Types ownership, explicit Window-to-Base dependency,
Emulator's inward-only calls, and owner-local tests. No App/Core, original
mirror, INI, disk or snapshot edits are included.

#### Verification

Focused x64 and x86 builds and nine targeted tests per architecture pass.
The first focused compile exposed a test-macro collision: the existing stub
named title shadowed the new helper parameter through macro substitution.
Renaming that fixture to capture_title fixes the fixture only; the failed
attempt remains recorded alongside the successful focused run.

Final cached builds and complete qualification pass on both architectures:
97 shared background + 65 other unit/integration/gates + five native desktop
cases = 167/167 per width, zero skips. Evidence is the second aggregate run's
shared/other JUnit files and the final desktop run's ten JUnit files under
the owned ignored S12 evidence directory. No Computer Use was used.
No generated trace or external asset is a committed test dependency.

The first desktop sequence passed the normal x64 package case, then its
compact case failed at stage=14/error=0: DOS was visible but injected ver had
not produced Version. It was not the known stage=1 startup failure. All ten
desktop cases subsequently passed unchanged, without retries inside tests,
deadline changes or assertion changes. Root cause is unproven and explicitly
transferred to TODO; final success does not claim that intermittent issue fixed.

Both EXEs are republished. SHA-256:

- softpc32.exe: 3B68C6805B16A8D3080E7EF6AE4CC5063523D6DDBC99D0E81B2602A9DA32CAA6
- softpc64.exe: 0A618EC9A98C073EFBEF0144B3281B1738B74A43719725BD0188ABE41E1CF02D

The package INI has no diff; guest media and snapshots are untouched. x86 Ninja
reported recovery of its existing log and rebuilt required targets through the
existing cache; no clean/purge or compiler setting change was performed.

The first aggregate x64 run passed 97/97 shared and 64/65 remaining tests.
Only documentation governance failed: creating an S12 history file before
acceptance made the identifier scanner treat S12 as historical and require S13.
The pending delivery record now stays here; no gate was relaxed. The original
failed run is retained, and the full dual-width qualification is restarted.

#### Changed-path accounting

Measured against 79eca79f with git diff --numstat, selecting tracked C/H files
under src/ and test/ separately:

| Owner | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Lib production | 12 | 95 | 42 | +53 |
| Emulator production | 1 | 4 | 1 | +3 |
| Lib tests | 9 | 121 | 27 | +94 |
| Emulator tests | 1 | 20 | 0 | +20 |

Production totals +99/-43, net +56; test C/H totals +141/-27, net +114.
Three Lib CMake source/gate edits are +3/-3; the Lib gate selftest is +2/-2.
Manifests, README/design/governance and the two executable artifacts are
reported separately, not counted as production/test C/H. Both X86 packages
and the original mirror have zero diff. The approved Base path simplification
is included in the Lib production total, not an additional reduction.

#### S12 coordinator actual-change review

Executor 2fc25069 is pushed. After switching roles, the coordinator read the
committed file list and actual production diffs, checked the original request
against the frozen ledger, and inspected focused/final JUnit results and both
published hashes. The delivery stays within the approved two Base cleanups
and S12 text boundaries. Platform APIs/types stay in Lib platform code;
Window's Base dependency is explicit in build, verifier, negative fixture and
documentation. No second output/reader/decoder owner, generic conversion
framework or retry state was added. Production/test counts match the committed
diff; both X86 packages, App, mirror and user-owned assets other than EXEs are
unchanged. Documentation and whitespace checks pass.

The first fixture compile error, premature-history gate failure and desktop
stage=14 failure remain disclosed. The last has an explicit TODO rather than
a false repair claim. Final test coverage passes all 167 cases per width with
zero skips. Review approves delivery for owner acceptance, not S12 closure;
T93 remains open and S13 is not admitted.

### S13 — PCM progress and lifecycle contract

Owner admission: "很好。验收通过，准入S13，开始前和结束后都要进行简报汇报；
请你以极简方案实施，意思不是说diff一定要减少，而是完成后总代码数量要最少".
Baseline f43ea903, with documentation-only S12 closure 0d63e3bb.
The admitted implementation removes the destroy tail-only flush branch,
retains explicit flush and clear, and preserves set_active(false)'s established
flush-then-clear behavior. It moves only the Windows byte-rate guard out of
the common options predicate; channel/storage arithmetic remains bounded.
No new API, state, helper, worker, retry or recovery path is introduced.
Estimate: three production C/H files +15-25/-30-45 (net -10 to -25), two
existing tests net +40-80. Counts and changed-file total sizes follow delivery.

Finite ledger: (1) public progress/flush/clear/destroy contract; (2) destroy at
short, batch and full-FIFO boundaries while native output is held; (3) common
versus Win32 format ownership; (4) progress/epoch/overflow/error regressions;
(5) actual consumer compatibility. Each needs focused proof plus both-width
full qualification; unsupported Linux/MIDI/FM are preserved, not reimplemented.
The consumer sweep finds speaker gate-off/finite completion already flushes,
speaker shutdown intentionally stops/cancels, and SB close clears then destroys.
No App change is required; existing actual-machine audio regressions verify
first delivery and cold/warm lifecycle, without claiming audible observation.

Keep PCM/WASAPI ownership and existing counters. Define accepted/submitted/
consumed without requiring padding knowledge; move backend-only format limits
to the backend while retaining arithmetic validation. Resolve destroy's
sub-batch-only flush special case with one explicit lifetime rule.
The admitted rule is explicit flush for submission, clear for discard, destroy
for stop/release; the start brief identifies the changed short-tail destruction
behavior. Flush is NOT proof of audible playback or a drain.
No new drain API or recovery machine. Existing SB close clears first; speaker
shutdown calls destroy directly, so compatibility must be tested, not assumed.
Files: PCM interface/common/Win32 implementation and two existing fixtures;
the admission estimate above supersedes the initial broad audit estimate.
Proof: short/whole batches, blocked backend, cancellation, epochs/counter
monotonicity and failures. MIDI already documents device-wide volume and
optional support; FM unsupported remains valid. No gratuitous MIDI/FM change.

#### S13 implementation and qualification

The retained owner is the existing PCM worker, bounded FIFO and backend.
Destroy no longer inspects queue length or conditionally flushes; cancellation,
join and ownership retention on cleanup failure stay intact. No new callable
API, state, helper, retry, second worker or platform selection route was added.
Public progress counts input frames, not native padding. Windows alone rejects
rates whose native byte rate cannot be represented; fake-backend tests prove
the common layer forwards the full nonzero sample-rate domain.

Actual C/H accounting against f43ea903 (`git diff --numstat`):

| Path | Added | Removed | Net | File lines before -> after |
| --- | ---: | ---: | ---: | --- |
| src/lib/audio/pcm/stream.c | 1 | 14 | -13 | 537 -> 524 |
| src/lib/audio/pcm/stream_interface.h | 15 | 14 | +1 | 82 -> 83 |
| src/lib/audio/pcm/win32/stream.c | 3 | 2 | +1 | 335 -> 336 |
| test/lib/audio/pcm/stream_smoke.c | 26 | 19 | +7 | 591 -> 598 |
| test/lib/audio/pcm/win32_platform_smoke.c | 10 | 1 | +9 | 325 -> 334 |

Production totals +19/-30, net -11 (954 -> 943 lines in affected files).
Tests +36/-20, net +16 (916 -> 932). Combined +55/-50, net +5;
documentation, manifests and EXEs are counted separately. Mirror diff is zero.
This is smaller than the start estimate because the existing blocked-output
fixture became a five-size loop rather than acquiring another fixture.

Similar-issue sweep: searched PCM source/test public padding, byte-rate,
sub-batch and destroy/flush/clear/set_active uses; inspected speaker and SB PCM
call sites. Short/batch/full-FIFO cancellation uses the same path. Speaker
finite/gate-off completion already flushes; shutdown intentionally cancels;
startup failure destroys an empty stream; SB close clears before destruction.
set_active(false) retains flush-then-clear. Linux remains explicitly unsupported;
MIDI/FM and Emulator/X86 interfaces remain unchanged. No new out-of-scope
production finding was confirmed by this bounded sweep.

Focused tests pass on both widths. They cover blocked destruction at 1, 512,
1024, 1025 and 4096 frames, cancellation failure retaining ownership and a
successful subsequent cleanup; explicit flush while consumed remains zero;
existing epochs, overflow, partial submission, control and notification errors;
and mono/stereo native byte-rate boundaries. These are deterministic fixture
proofs, not claims of listening to physical sound. The first focused runner
had a PowerShell quoting error before any build/test started; its preserved
failure is tooling evidence, not a product failure or a passing test.

Full cached qualification exit 0: x64 and x86 each 97 shared + 65 other +
5 desktop = 167/167, zero failures or skips. Shared suites ran first, then
all remaining unit/integration cases; ten native desktop cases ran serially
under the owner's standing authorization, without Computer Use. No assertion,
timeout or product retry was relaxed. Ninja recovered its log and revisited
targets with the existing compiler cache; no cache purge was performed.
Evidence is the S13 focused/full runner logs and 14 full-suite JUnit reports.
The documentation gate and `git diff --check` also pass.

All five finite-ledger items are verified: (1) interface and source review;
(2) deterministic five-size destruction/cancel-failure matrix; (3) fake-backend
full-range forwarding and native boundary fixture; (4) retained counter,
epoch, overflow and error cases plus flush-not-consumed proof; (5) consumer
source audit and actual-machine first-tone/first-delivery/audio-lifecycle,
SB PCM, snapshot and package regressions. No physical-audibility, Linux audio
backend or external-consumer qualification is claimed.

Published SHA-256:
- softpc32.exe: 77A8FAC203C887753383EC7A7B49B50C1C3DEFE1E12EF8FDF8DB3E2E77D72A99
- softpc64.exe: 288EA3C8C81FBA11AAD63A48B338FCD255D3CFD70C266AE1FB79D8031FB80E99
- unchanged user INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

Both EXEs are 512 bytes smaller than the accepted S12 artifacts. No guest
media, snapshot, original mirror or external repository changed. Executor
delivery is ready for a separate actual-change review and owner acceptance;
S13 is not closed and S14 is not admitted.

Coordinator actual-change review after pushed executor 9960b37f:
reviewed its five C/H diffs, two manifests, published hashes and governance
against the original request and packet. Confirmed the old count-dependent
flush is deleted, not redirected; cancellation/join and failure ownership
checks remain on the existing path. Format multiplication remains guarded in
Win32. The unchanged real consumers explicitly flush or clear where needed.
Reviewed all 14 full-suite JUnit summaries: 167/167 per width, no skips; the
initial runner syntax failure is separately disclosed. Types, inward ownership,
dependency and naming gates pass. Actual counts match the ledger, user INI is
unchanged and the implementation P is synchronized with origin/main.
No blocking finding within S13's bounded scope; owner acceptance is still
required. This review does not close T93 or claim all platform tasks finished.

### S14 — capture boundary and shared-contract completion audit

Owner accepts S13 and admits S14 with before/after briefs, minimum necessary
final code and single-agent dual-role execution. Baseline e85a0015; S13 closure
833ac174 changes governance only. The start estimate is production +20-35/-10-20
(net +10-20), tests net +40-80, plus documentation/manifests and published EXEs.

The source-proven defect is an unprotected query followed by registration:
two Window workers can both observe no binding and subsequently replace it.
The native registry remains the sole ownership authority. One static native
lock serializes its query/update; no shadow owner pointer or registry is added.
Reserve before focus/capture/clipping changes. On release, finish native cleanup
before removing the reservation. Never hold the lock across focus/capture APIs:
they may dispatch messages. The private helper is a real atomic boundary,
not another input path. Types owns its four native lock declarations.

Finite convergence ledger: the seven original S9 audit areas below are the
frozen units, each requiring source disposition and its existing/new tests.
This is a cross-component contract audit, not a claim of complete backend
support or a line-by-line absence of all bugs in the eight packages.

| Area | Current disposition and proof |
| --- | --- |
| Console raw input | S10: Types key/scalar/repeat/button vocabulary, explicit absolute cell coordinates; Base normalization before Console/KVM. Input, raw normalization and key/text regressions. |
| Physical keyboard | S10: PC set-1 make identity, E0, Pause/Print Screen and unavailable-zero rules; Windows/Linux adaptation stays inside Lib. Base/KVM input fixtures. |
| Strings | S11/S12: UTF-8 paths, titles and complete Console text/lines; Windows wide calls at native boundary, binary data unchanged. Process/storage, title and Console encoding fixtures. |
| Path syntax | S11/S12: Windows root/separator interpretation stays in Windows, POSIX recognizes slash and rejects non-UTF-8 names. Directory root/bounds/encoding fixtures. |
| Audio | S13: frame/epoch progress, explicit flush/discard and batch-independent destruction; native format guard remains backend-local. MIDI optional device-wide volume and unsupported FM are already honest contracts. PCM/MIDI/FM fixtures. |
| Capture | S14: one serialized registry query/update; capture/release/reservation ordering, conflict and native failures. Existing capture fixture extended for actual task competition and synchronous message reentry. |
| Emulator/X86 | No native API/type or OS-selection C/H matches on renewed scan; no production changes warranted. All inward dependency, public-header, Types/naming, corpus and test gates must pass receiving qualification. |

Searches cover registered-raw-input calls across src (only mouse.c owns them),
native API/OS vocabulary in Emulator/X86 C/H, and the identified A/W/text/path
entry points. Remaining CreateFileA vocabulary is a Types declaration, not a
public path implementation; Linux fopen consumes the validated UTF-8 byte path.
Reviewed the public headers and corresponding native implementations, not just
the spelling search. Third-party direct Raw Input mutation is outside the Lib
lock; that limitation is retained in Lib's Windows integration documentation,
not silently erased. No Linux/macOS capability expansion is claimed.

Describe capture/release/conflict/failure neutrally; native third-party mixing
restrictions belong in platform integration documentation. First prove whether
two Lib Windows can race query/register, then add only necessary coordination
inside the existing owner. No new registration framework or warning-only fix.
Existing capture tests already cover query/register/remove errors, foreign
registration refusal and not removing a replacement owner; reuse them.
Files: Window interface/private mouse implementation and tests/docs, roughly
2-4 production paths. Production +0-35/-0-10, net 0-35;
tests +70-120/-0-20, approximate net +50-120, refined after concurrency proof.
Proof: deterministic competing Lib acquisitions and existing failure/release
paths, without claiming to control third-party native calls. Finish a finite
ledger mapping all seven owner-requested audit areas to S10-S14 fixes or justified
unchanged contracts, including Emulator/X86 interfaces and build/test boundaries.
No new Linux/macOS support claim. Complete receiving qualification before S15.

#### S14 implementation checkpoint — qualification blocked

The existing capture fixture now holds the first registration update while a
second task enters acquisition; exactly one binding wins. Query/update hooks
assert lock coverage. Synchronous focus/release callbacks attempt another
capture without deadlock, side effects or stealing the reservation. Native
activation denial releases the reservation before returning INVALID_STATE.
Existing foreign-binding, query/register/remove failure, clipping, relative/
absolute movement, freeze/reset and native-message cases remain in the same
fixture. Both-width focused tests and the final shared-suite capture cases pass.

Actual C/H diff against e85a0015:
- mouse.c +32/-18, net +14; 165 -> 179 lines.
- window_interface.h +4/-3, net +1; 46 -> 47 lines.
- types/win32/sync.h +4/-0, net +4; 37 -> 41 lines.
- capture fixture +93/-4, net +89; 658 -> 747 lines.

Production totals +40/-21, net +19 (248 -> 267 affected-file lines).
Tests +93/-4, net +89. The test estimate is exceeded by 9 lines because message
reentry ordering is proved separately from task contention, without duplicating
the native fixture. Public API/structure layout and motion algorithm are unchanged.
No manager, shadow registry, owner pointer, retry or recovery state is added.
One native static lock and one actual atomic helper replace split registration
handling. README/manifests, governance and two EXEs are separate accounting.

Cached dual-width builds published both EXEs. First x64 link was blocked by the
running package EXE; its exact process was closed under standing owner authority,
then the build resumed from cache. The next x64 shared run failed native MIDI.
Isolated runs also failed; a disposable direct WinMM probe, bypassing Lib and
SoftPC, returned midiOutOpen result 1 (Undefined external error), MIDI device
count 1 and wave-output device count 0. Both audio services reported Running.
No service, device configuration, audio production code or test expectation was
changed. The exact cause of the missing host playback endpoint is not established.

The complete receiving run continued through every test after failures:
each width ran 97 shared (96 pass, native MIDI fails), 65 other (41 pass,
24 failures at machine initialization/related expectations), and five native
desktop tests (three Lib cases pass, two package-start cases fail at stage 3).
Thus 140/167 per architecture pass, 27 fail, zero skip. Direct native failure
is established; attributing every machine/package failure to that same cause
requires rerun after host capability recovery. No passing qualification is
claimed. All ten native desktop cases ran without Computer Use.

Published but not qualified artifact SHA-256:
- softpc32.exe: 05DCE704C5F1719CED657D9CA55287E549CEAC2DFA13627EB982BC9FEB6F8822
- softpc64.exe: 1814D15A5B446545E228703DFC6190D08646A5E85341949A57076D87309E306A
- unchanged INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

S14 executor work remains uncommitted pending receiving qualification, not a
completed P. Coordinator actual-commit review and owner acceptance remain due.
The owner was asked whether the host/RDP audio session changed. Restoring host
playback capability or authorizing host-audio investigation is the next external
action; do not silently alter host services or weaken native tests. S15 is not
admitted. S13 acceptance/closure is already pushed as 833ac174.

#### S14 host recovery and retry verification

Owner requests "Ok retry", then confirms RDP/audio-session switching and says
"所以你别死磕了，现在能跑通就行了". This authorizes ending host investigation
once the current paths run; it does not establish a cause for every prior failure.
The direct native probe now reports MIDI devices=1, wave devices=1, open=0 and
close=0. No host services/settings, production audio or test budgets changed.

Both cached Release builds succeed and refresh the package EXEs. The complete
rerun executes every case, without skips:
- x64: shared 97/97, other 56/65, desktop 5/5; total 158/167.
- x86: shared 96/97, other 65/65, desktop 5/5; total 166/167.
- The nine x64 failures (machine, FDC, checkpoint, snapshot transaction/cross-
  process, runtime, restart, command provider and platform failure) and x86
  native MIDI all pass in the subsequent unchanged, serial isolated run (10/10).

All 334 unique architecture/test pairs therefore have passing evidence. This
is explicitly not a claim that the aggregate first rerun passed, or that the
known checkpoint timing debt is repaired. The x86 MIDI exit 0xc0000409 was its
failed-open assertion, not proof of memory corruption. Its isolated run passes
in 1.00 second; the earlier x64 native case took 10.50 seconds. Historical failed
qualification evidence is retained alongside the full retry and isolated XML.
All ten desktop cases passed without Computer Use. No further host investigation
is undertaken under the owner's direction; S14 capture tests passed throughout.

Final package SHA-256:
- softpc32.exe: 695FE67B4CE2F9713F83FFBEC0C11F4FAD35D35703EAE4C91090C57CD68E0B10
- softpc64.exe: 1814D15A5B446545E228703DFC6190D08646A5E85341949A57076D87309E306A
- unchanged INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

The seven-area ledger above is complete at the declared cross-component-contract
scope. Existing native capability limits and TODO debts remain explicit. No
App, Emulator/X86, mirror, guest media or snapshot changes. Executor self-review
confirms the single native registry authority, lock-free message dispatch,
existing cleanup/motion paths and Types ownership. Separate actual-commit review
follows executor push; S14 waits for owner acceptance and S15 remains unadmitted.

#### S14 coordinator actual-change review

After executor fe6d9fe3 was pushed, the same agent switched to coordinator and
reviewed its actual source/header/test/README/manifest diff, original S14
request, seven-area ledger, full-run XML and every isolated failure result.
The registration-call sweep still finds one implementation owner (mouse.c),
with only Types vocabulary elsewhere. No lock spans focus/capture dispatch;
the deterministic competing-task and synchronous reentry tests exercise the
same production helper. Public signatures and mouse-motion conversion stay
unchanged; native integration limits remain documented rather than hidden.

All eleven changed paths belong to the admitted packet. Production and test
counts match the executor accounting; package hashes match the published files.
No audio implementation, App/mirror, media, snapshot or INI change is present.
The review accepts the owner's direction to stop host-session investigation
after unchanged isolated cases pass, without recasting aggregate failures as
success or claiming their root causes fixed. Existing TODOs remain unchanged.
Documentation governance and whitespace checks pass. No blocking finding in
the bounded S14 change; owner acceptance remains required before S14 closure.

#### S14 partial-capture correction

Owner supplies downstream P2 audit of 4aacc52e, then explicitly approves repair:
"批准修复". This continues the unaccepted S14; it does not admit S15. The prior
actual-change review missed capture-success followed by focus-loss and is not
evidence that this failure path was correct. Source review confirms that only
Raw Input was unbound, leaving native capture held while captured remained false.

Minimal plan: before unbinding on activation rejection, release native capture
only if still owned by this Window. Always attempt registration cleanup even
when release fails; propagate cleanup failure rather than normal INVALID_STATE.
No clipping was acquired yet, so do not invoke full clipping cleanup or create
a false captured state. No new API/helper/state/retry or message-spanning lock.
Estimate: production net +4-6 and tests +25-40, manifests and EXEs separate.

The existing fixture now injects successful capture followed by focus loss,
capture replacement, release failure and registration-removal failure. Existing
focus/capture denial tests remain. Before the repair, the new regression fails
at its required release-count assertion on x64. Production repair is +3/-0;
tests +26/-1, net +25. Both cleanup calls remain on the same rejection path.

Similar-issue sweep covers every mouse_bind, SetCapture and ReleaseCapture call
in the Window owner and its component failure forwarding. Normal release
already checks native ownership and preserves errors; bounds failure already
uses normal release and returns IO_ERROR. The incomplete activation-rejection
branch is the sole newly identified hit. Component maps INVALID_STATE to normal
refusal and all other errors to its existing failure sink; that behavior stays.
The new controlled regression proves the local failure sequence, not an actual
RDP reproduction.

Corrected receiving qualification: cached Release builds pass both widths;
each architecture passes 97/97 shared, 65/65 other and 5/5 native desktop tests,
167/167 total, zero skips. This complete run passes without isolated retries.
The new capture regression passes both widths. Both EXEs are refreshed; INI
hash is unchanged. Native test harnesses ran without Computer Use.
Manifest, inward-boundary, Types, naming and documentation gates pass.
Executor self-review confirms that ReleaseCapture runs before unbinding and
outside the registry lock, including failure; another native capture owner is
untouched. No new API or persistent state. Correction stays within Lib source/
test plus manifests/governance and two package EXEs.

Final SHA-256:
- softpc32.exe: 5D137C99B97114285E3D5176B94518F18805B7CAD8D9E0631BA4C7112DE30D45
- softpc64.exe: 7176466BA4086D79C451869D773C9BFFF93805BE860D3456B8DA0ADD2FDBEF75
- unchanged INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

Correction coordinator review: after pushing 683f85d4, switched roles and read
the actual eight-path commit, original P2 report/owner approval, before-fix
assertion failure and all fourteen qualification XML reports. The three-line
repair preserves cleanup order and attempts unbinding even after failed native
release; the ownership guard leaves a replacement capture untouched. The test
uses the existing native fixture and checks status, release count, registration
and untouched clipping. Its release-failure injection proves error forwarding,
not every possible native failure side effect. No new public boundary or second
input path. Counts/hashes and both-width 167/167 evidence agree. Documentation
and whitespace gates pass; no blocking finding in this correction. S14 still
awaits owner acceptance; neither S14 nor T93 is closed by this review.

### S15 — portable archive and final qualification

#### Owner-local shared test layout extension

Owner requests preserved: "test里的公共4组件的test，目录文件结构和src的公共4组件
的src，不匹配！我需要你修复。" "跨组件的单元测试，那就放在最外层组件的
单元测试即可，不可以放到integration去。" "最内层只依赖和测试最内层组件；
中间和外层只依赖本层和内层组件，然后测试的话只测试本层组件。"

The frozen universe is all C/H fixtures and registered cases in test/lib,
test/emulator, test/x86-debug and test/x86-ibmpc at 0dfc02cf, plus their
registration, manifests and ownership checks. Unit of review: one assertion
group and its production owner. Permitted dispositions are unchanged owner,
structural move, split to independent owners, or real-host integration retained.
Completion requires every group to have a receiver, no unregistered smoke,
no unit-to-outward source dependency and dual-width qualification. Production,
mirror, INI, media and snapshot changes are outside this extension.

| Mixed original test | Assertion receivers |
| --- | --- |
| types_contract | Types scalars/atomics; Base clock |
| Emulator sync | Session queue concurrency; Machine input capacity; Types atomic operations; Base invalid mutex arguments |
| lib_failure_output_contract | Storage, Base task, Broker, KVM Console and KVM Window failed-create outputs |
| linux_wait_contract | Base synchronization; Broker Linux unsupported backend |
| midi_output | MIDI message validation; FM unsupported acquisition |
| win32_presentation | KVM Base hotkey matching (hotkey_policy); Window text-frame validation |
| win32_vocabulary_ownership | Base native clock/layout failure; Window modifier state; KVM Base neutral synthesis (input_delivery) |
| win32_keyboard / linux_input_contract | Base native physical/text decoding; KVM Base neutral delivery and registered chord matching |
| kvm_frame_copy | Window frame/copy publication; KVM Base cursor/cell contract; KVM Console character-frame validation; Console boolean fields |
| kvm_leaf_control_capacity | Window title/freeze admission; KVM Base opaque control capacity |
| console_blocking_gate | Console bindings/metadata contention; Broker output/transaction contention |
| kvm_keyboard_lifetime | KVM Base ledger/synthesis; separate Window and Console message-adapter behavior |

Keyboard adapters now assert expected key/scalar payloads and ordering rather
than comparing peers that might share an error. Test-owned native keyboard
queries and synthesis mapping remove dependence on the host keyboard layout.
No shared fixture or production path is introduced to couple the two leaves.
Existing UI composition assertions were not registered; the unchanged test
body is moved to ui/ and restored to the executable suite. The Session reducer
test loses its misleading integration suffix without changing its assertions.

Other Lib/Emulator tests and their private fixtures are moved with git mv to
matching source-owner directories. X86 test bodies already have matching
ownership and stay unchanged. Real native tests remain in integration/;
no cross-component unit was reclassified. Source-owner directories and inward
same-package includes are checked by the shared test boundary gate, reusing
production dependency authorities. Eleven new positive/negative probes exercise
layout, legal inward use, outward/sibling rejection and integration exemption.
Package-wide scripts and negative fixtures retain package ownership.

Owner's follow-up: "请你审计类似情况，确保src和test的各个公共组件的目录文件代码结构严格对齐";
approved implementation: "请你按照规范执行，完成后编译测试提交推送等我验收".
The finite inventory covers all C/H in the four suites (90 before this final
pass), their registrations, includes and source owner directories. No production
file changes. Native layout/UTF-16 assertions move to Base, while KVM delivery
uses neutral values. The former Window bounds test becomes geometry; the MIDI
unsupported-backend test moves to linux/. Foreign Base private platform bodies
are no longer used as leaf fixtures: lock tests intercept the public acquisition
boundary; shutdown injects the existing mailbox wake contract, without inspecting
Base handles; keyboard synthesis receives a deterministic neutral layout.
Pure native SDK test resources (guard pages, threads, file-name oracles) remain
legal unit fixtures, not reasons to create a nonexistent source platform owner
or relabel a unit as integration. Base's own two-platform directory matrix stays
at Base because it exercises that common public contract on both backends.
Emulator and both X86 suites have corresponding owner directories and no foreign
test package dependencies. The gate additionally rejects a nonexistent nested
source directory and another owner's private platform fixture. Three new
positive/negative probes cover those conditions. Assertion ownership still
requires actual-change review; static includes cannot prove assertion meaning.

An initial final-pass run rejected all four manifests because the regeneration
script used the wrong header/hash spelling. The generator was corrected to the
existing manifest contract; the failed run is retained, not accepted evidence.

Final qualification for the layout extension, relative to 0dfc02cf:

| Evidence | x64 | x86 |
| --- | --- | --- |
| Cached package/test build | pass | pass (existing Ninja-log recovery) |
| Shared non-native suites | 116/116 | 116/116 |
| Other unit/integration/checks | 64/64 | 64/64 |
| Native desktop | 5/5 | 5/5 |
| Native audio | 3/3 | 3/3 |
| Registered total | 188/188 | 188/188 |

Final comment-only corrections received another dual-width affected-target
build and 8/8 focused checks each (leaf tests, manifests, boundaries and docs).
The full-run exit is zero; evidence is retained under the packet's declared
ignored evidence directory as layout-delivery logs and qualification XML.
No failing run is substituted for a passing final result. Public changed-path
compilation remains under Werror; inherited mirror warnings and Ninja recovery
are not represented as zero-warning whole-repository builds.

`git diff --numstat 0dfc02cf` with rename detection gives production +0/-0;
test C/H +1276/-1032, net +244. Package totals including CMake, README and
manifests: test/lib +1374/-1086 (net +288), test/emulator +69/-60 (net +9),
test/x86-debug +2/-1 and test/x86-ibmpc +2/-1 (net +1 each). Shared root test
gates add 77 lines. Splitting independent ownership adds test entry/fixture
setup, not runtime abstractions. Final coverage universe is 91 C/H files;
all have an explicit owner, with real-host cases retained in integration.
Console now consumes complete neutral scalars; the seven malformed/repeated
UTF-16 cases are also asserted directly in Base. Window physical cases supply
PC scans, leaving scan-less native recovery to Base's deterministic probe.

Published artifact SHA-256:

- softpc32.exe: C5E3B709152B534F7B353D5E98B0606E8FFBACBD7D1A98DAC0AAAB0632F2F09C
- softpc64.exe: 56BEA1E29C4EB06C48050D444E640631AFF7865203A2653F89AE13495D9E5CCF

The x64 product artifact is unchanged and verified against the current cached
production build; x86 was rebuilt. Owner INI and snapshot hashes remain
unchanged, and no guest media was edited. The pre-existing owner-approved
Win95 Sound Blaster queue proposal is preserved verbatim with this delivery,
without admitting or implementing it. S15 awaits owner acceptance after the
separate actual-change review; T93 remains open.

#### S15 test-layout actual-change review

Coordinator role reviewed executor 77cd7263 against 0dfc02cf and the retained
owner requests, rather than relying on its test summary. The 96 changed paths
include structural test moves/splits, registrations/manifests, owner-requested
queue documentation and one rebuilt product artifact. Source production,
user INI, snapshot and guest-media diffs are empty. The four owner layouts,
native-name removals, inner assertion receivers, inward links and absence of
foreign test fixtures match the requested model. Existing real-host labels
remain integration-only; no cross-component unit was moved there.

The old combined keyboard test is replaced by independent expected-value
checks, not peer equality. Base owns native/UTF-16 mapping; Console receives
neutral complete inputs; Window covers its native message boundary with explicit
physical scans. Lifetime/reset/sink-failure assertions remain in the appropriate
leaf, with the shared matcher ledger tested by KVM Base. Static boundary checks
and their new negative probes pass, and the unchanged UI composition body is
now registered. Production dependency rules are reused rather than duplicated.

Review reconciled the full-run XML (188/188 each width), subsequent comment-only
focused checks (8/8 each), both artifact hashes and clean protected-file diff.
Earlier failed manifests/build attempts remain explicitly rejected evidence.
The known Ninja recovery/mirror warnings remain disclosed debt, not a false
zero-warning claim. Review passes for delivery; it does not replace the owner's
S15 acceptance and does not close T93 or admit another S.

Implementation review found and fixed split-fixture missing declarations before
qualification. An intermediate x86 build encountered the reducer's old filename
while the final rename was being applied; that run is not acceptance evidence.
The final source set is frozen before repeating qualification. Existing x86
Ninja log recovery still triggers unnecessary rebuilds and inherited mirror
warnings; it is already recorded separately and is not repaired by this task.


Owner accepts S14 and admits this step: "批准 验收通过 准入下一个s".
Baseline e2de3d3f. This first checkpoint is source inventory/design, not a
production change or passing snapshot qualification. The existing archive owns
DMA/PIC/quick-event capture and table-driven fixed-width encoding, but has no
SB state member or SB callback mapping. SB device owners retain static transfer,
parser and FM state; copying only exposed registers would be incomplete.

Inventory units before implementation: DSP command parser (including function-
static multi-byte command state), frozen PCM block/accepted-consumed offsets,
completion quick event and EOI/delayed IRQ continuation, Mixer registers and
gain, MPU binding/mode/input, MIDI partial message/running status/notes/pedals,
FM index/status, and host resources. Each must be classified as serialized guest
state, derived/reconstructed state or intentionally excluded host state, with
validation and a test receiver. Do not serialize native handles, callback
addresses, guest-memory pointers or host epochs. Reuse the current fixed-width
codec and callback identity map; no second serializer or Lib archive framework.
Exact field layout and restore ordering remain to be proved before coding.

#### S15 field and ordering audit

The frozen census is the five retained `nt_sb*.c` owners plus Compat EOI,
the existing device archive, and their snapshot callers. Public eight
directories are excluded from modification. Apply execution, architecture,
coding and documentation governance to this owner-local extension.

| Owner | Persisted obligations | Reconstructed or excluded | Proof receiver |
| --- | --- | --- | --- |
| DSP parser | Reset/read/write state, identify/reserved values, time constant, block size, partially received block-size word, table-munge byte and phase | DMA adaptor/controller pointers and physical channel derive from the existing binding | DSP command continuation across every partial command |
| DSP transfer | Frozen block, consumed prefix, starting DMA address/count, copied DMA observation, mode, active/paused/reset/exit/high-speed flags, virtual DMA query counters | Host handle, epoch/origin, accepted native prefix and completion polling counters are not portable; restore must rebase using the existing paused-output path | Active single/auto DMA, backpressure, pause, EOI continuation and failure injection |
| EOI | Delayed-IRQ eligibility mask and event handles | EOI function pointer derives from auto-init mode; callback identity uses existing queue codec | Pending delayed IRQ, cancellation and handle/callback consistency |
| Mixer | Selected register and all four volume bytes, including reserved bits | Host gains recomputed from restored Mixer and speaker state | Register selection/readback and subsequent gain operation |
| MPU | Admitted port presence, mode and unread response byte | Host MIDI handle/fault is not a guest register; unavailable required capability must fail explicitly | Optional admission, unread ACK and UART continuation |
| MIDI | Partial buffer/count, remaining bytes, last-command length, running status, realtime tail, discarded-SysEx flag, notes and sustain/sostenuto state | Native headers, host playback envelopes and execution-running flag excluded; loaded output remains paused | Partial short/SysEx/realtime messages, bounds rejection, held-note replay |
| FM | Selected index and status byte | No OPL synthesizer or host FM state exists in the selected XP fallback | Index continuation and exact status across restore |

Source findings, not dynamic qualification: snapshot checkpoint currently stops
the platform clock but does not park SB output. Therefore capture must not
blindly copy an accepted-prefix counter as a consumed-prefix counter. Also,
`StopDspTransfer` cancels events and clears PIC state: it must run before PIC/
queue restoration, never after restoring those obligations. A restored PCM
completion cannot reuse a prior host epoch. Queue identities and saved handles
must be checked together before installing callback-bearing state.

MIDI approximation here means the already tracked logical notes and pedals,
not precise host envelopes or a newly introduced synthesizer. Program/bank,
pitch-bend and other controllers are not currently remembered by the parser;
the implementation review must explicitly settle their restoration before
claiming approximate musical continuation. This audit is not such a claim.

Completion predicate: every census row has owner-local capture, validation,
restore and focused evidence; the existing byte codec passes both cross-width
directions; full receiving qualification and actual-change review pass. Small
owner-hook checkpoints alone do not satisfy this predicate or permit S closure.

#### S15 implementation checkpoint (not a delivery)

The owner-local implementation now extends the existing device maps and callback
identities. Capture reads host progress without altering the live transfer;
the copied consumed prefix is relative to the frozen device block. Restore
clears old output before PIC/queue restore, removes the saved host-progress
deadline, and opens inactive output with a new epoch. Resume uses the same
existing service/arm path. Delayed IRQ work remains in the existing queue.
No timer, second serializer or public-component change is added.

MIDI scope is the existing parser and logical note/pedal model. Previously
untracked program/bank/pitch/controller state is not newly promised by this
archive; host SysEx side effects and envelopes are likewise not reconstructed.
This is a documented approximation, not full musical-state equivalence.

First owner-local Mixer/FM/MIDI runs pass 3/3 per width. The complete archive
fixture covers a partially accepted block, a pending completion, a fully
consumed block with pending guest completion, single/auto DMA, delayed IRQ,
unread MPU ACK and partial MIDI. x64 passed; x86 exposed a new call-site ABI
mistake: the 32-bit saved handle was passed through a legacy non-prototype
declaration expecting a 64-bit q_ev_handle. A temporary probe recorded a stale
second completion with zero block size. The explicit q_ev_handle conversion
passes the seven focused checks on both widths; the temporary production trace
has been removed.
The same-issue search for snapshot-state arguments to delete_q_event and
delete_tic_event found only this new call site.

The receiving x64 build passed. Its initial focused set passed 14/16; native
MIDI admission and machine startup for snapshot_transaction failed. A direct
host probe independently reports one MIDI device, zero Wave devices and
WinMM result 1 (Undefined external error). This is not evidence of a passing
product snapshot test, nor permission to skip it. Full qualification, final
dual publication, malformed-wire/cross-width proof and actual-change review
remain required. No implementation P is committed at this checkpoint.

The subsequent complete run built and refreshed both EXEs and ran all 168
registered cases per width, including all ten native desktop cases. Each width
passed 96/97 shared, 42/66 other and 3/5 desktop tests (141/168 overall).
The direct native MIDI check still fails; the receiving failures include reset,
startup and both package routes. Source tracing confirms native MIDI admission
errors propagate through SbInitialize to machine reset. Do not infer that every
failed integration assertion is independently root-caused merely from that
shared symptom. No tests were skipped or relabelled to qualify the run.
Raw results are retained in the packet's ignored S15 qualification evidence.

The complete byte archive passes all four separate-process writer/reader width
combinations. The fixture also rejects a corrupted EOI mask/callback relation
before clearing current output. Additional coverage exercises every partial DSP
data-command boundary (including both TableMunger phases), compares continuation
with uninterrupted execution, and acknowledges the restored auto-init interrupt
through the actual PIC to start the next PCM block. Both widths pass this
expanded fixture. Two initial test assumptions were corrected: this fixture intentionally
has two IRQ requests, not one, and a direct owner restore requires old output to
be closed first. No production workaround was added for those test mistakes.

The host probe remains one MIDI device, zero Wave devices, result 1 and null
output handle. An audio-capable host session is needed to retry qualification;
do not change public audio behavior, hide IO_ERROR as UNSUPPORTED, or weaken
tests. The owner has been asked to restore that environment, not manually test
the unqualified binaries. User INI SHA-256 remains
38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D.

Current uncommitted accounting against S15 admission, from git diff --numstat:
eight production C/H paths +562/-8 (net +554), four test C paths +274/-1
(net +273), root test registration +2/-0. The five preserved SB owner files
account for +324/-4 (net +320); these are visible local additions against the
unchanged selected XP subset, not excluded from its comparison. The mirror
gate passes all 505 local bodies. No public eight-directory changes exist.
Both local EXEs were refreshed before the final test-only coverage extension;
production code has not changed since that dual build. Work remains uncommitted
pending a successful required receiving qualification and actual-change review.

#### S15 owner-approved test isolation

Owner extends this S: real audio and desktop checks belong to integration;
unit tests must not depend on an interactive session or audio endpoint, and
non-audio device tests must mock audio. This supersedes the earlier no-public-
test-change boundary, not the unchanged public-production boundary.

The finite receiving set is every registered test in the two package builds.
Real Lib MIDI and three Lib desktop cases move under test/lib/integration;
App's real speaker lifecycle check moves under App integration. Together with
the existing App MIDI and two package desktop cases, eight tests per width
remain explicitly native integrations. None is removed or replaced by a mock.
Shared registration rejects unit/native labels and ambiguous execution classes;
six negative/positive configure fixtures test this gate.

Twenty-five non-audio App machine/runtime executables link App-owned PCM and
MIDI memory sinks at existing public APIs. Each sink is compiled once per
architecture. The sound-delivery test retains its real PCM worker and existing
scripted native endpoint, replacing only unrelated MIDI. Specialized audio
failure/continuation tests keep their own fault/progress fixtures. No sink is
linked into the product or native integrations. Win32 keyboard unit input now
uses explicit protocol scan codes rather than querying the host keyboard map.
These changes do not create a production audio fallback or silence host errors.

The first isolated x64 run passes 160/161; the Types gate correctly rejects
direct allocation in the new test sinks. Replacing it with existing Lib Types
allocation fixes that violation. This failed attempt is not final qualification.
The final isolated and native suites are reported separately; passing mocked
tests cannot certify native playback or an unavailable desktop.

Frozen CTest inventory now contains 169 entries per width: 140 unit, 13
repository checks and 16 integration (eight native, eight non-native). The
native subset is five desktop plus three audio. Binary import inspection of
all 25 mocked executables on both widths finds no midiOutOpen, waveOutOpen or
CoCreateInstance imports; the product retains its real audio imports. Native
SDK calls in PCM/MIDI contract tests are explicitly replaced by their scripted
fixtures; Console logical-object tests use bound memory callbacks, not native
console attachment. Kernel threads/events and disposable filesystem fixtures
remain allowed; absence of an interactive/audio endpoint is the isolated
boundary, not a claim that tests use no operating-system services at all.

Final isolated suites pass 97 shared plus 64 other (161/161) on each width,
with zero skips. x64 was rebuilt and rerun after the final structural changes;
x86's preceding completed run already used that final configuration. Native
qualification then passes all eight integrations per width, including all ten
desktop cases. Thus final qualification is 169/169 per width, zero skipped,
failed or disabled cases. All four separate-process archive width routes pass
again. No host-service mutation or native-failure suppression was introduced.
This native run had a usable endpoint; it does not claim that disconnected
sessions always provide one. Mocked receiving tests now do not require one.

Evidence: build/t93/s15/isolation-final (x86 shared/other) and
isolation-qualification (final x64 shared/other, both native sets and archive
routes), each completed with result.exit=0. The first isolated Types failure
and earlier native-host failures remain historical failures, not overwritten
acceptance results. Final metadata/documentation gates are rerun after this
report; no executable code changes follow qualification.

Complete S15 accounting versus admission 41693590, using rename-aware Git
numstat, excluding artifacts/manifests/documents from source totals:

| Scope | Added | Deleted | Net |
| --- | ---: | ---: | ---: |
| Compat production C/H (3 paths) | 238 | 4 | +234 |
| Preserved SB owner C (5 paths) | 324 | 4 | +320 |
| Test C/H, including moves and new sinks | 391 | 8 | +383 |
| CMake/presets/registration self-test | 97 | 21 | +76 |

The isolation addition alone has zero production change, C tests +111/-7
(net +104), and configuration/self-test +95/-21 (net +74). Five tests move
without deleting their original coverage. Native audio/desktop is not replaced
by an always-successful sink. Shared production remains byte-identical to the
admitted baseline; shared test/lib and root test tooling intentionally change.
Mirror accounting remains +324/-4 relative to the unchanged selected XP subset.
All 505 selected local bodies pass the mirror gate. No user medium/snapshot or
INI was edited; the original INI hash remains unchanged.

Published SHA-256:

- softpc32.exe: `61CD580A6458645999FCC7841A0274FA2AADA77F579634A1C9F3D3FC83BAAF38`
- softpc64.exe: `402020D223D0335C98AE10E79D536E3320B3553FD138DE32749AC9AFFEBB9988`

These tests prove serialized device continuation and the isolated/native
boundaries exercised, not acoustic equivalence or full MIDI instrument state.
The approved approximation and old-format incompatibility remain explicit.
Executor 66058ab4 is committed and pushed. The coordinator subsequently reviewed
that actual 35-path commit against admission 41693590, not only the executor's
summary. Review covers all eight production paths, the existing archive maps
and ordering, fixed-width fields, the x86 queue-handle conversion, copied MIDI
state and its declared limitations, every changed test, five structural moves,
two test-only sinks, registration/build wiring and manifests. Public production
is unchanged. No native tests were deleted, converted to mocked tests or made
to return success on IO_ERROR. The product and native targets do not receive
mock objects. No user configuration, medium or snapshot appears in the commit.

The coordinator also checked the final JUnit sets (169/169 per width, no skips),
both completed exit records, four archive routes, current artifact hashes and
the post-report 3/3 metadata gates per width. The unchanged XP subset and five
local mirror-owner deltas remain separately accounted. Production state still
has one owner and one archive; no new backend, fallback or recovery state
machine was introduced. This accepts the complete S15 delivery for owner
verification. S15 awaits owner acceptance; T93 does not close automatically.

#### S15 owner-requested qualification retry

Owner: "再来". The independent native probe initially reports one Wave device
and MIDI open/close success. Without changing production code or weakening any
test, x64 passes 97/97 shared and 66/66 other tests; x86 shared passes 97/97.
During that same run the host loses its Wave device again. A fresh direct probe
reports zero Wave devices, MIDI result 1 and no handle. x86 receiving tests then
pass only 42/66, and native desktop passes 3/5 per width. All ten desktop cases
ran; the four failed cases are the product package routes. Overall x64 is
166/168, x86 142/168. Both refreshed EXEs remain unqualified. All four separate-
process archive width routes pass again. Evidence is retained separately from
the previous failed run in the packet's ignored qualification-retry directory.

The final test review adds six lines to exercise note/sostenuto and sustain
continuation through actual capture/reset/restore, not only pause/resume.
This changes no production code. Final test accounting becomes +280/-1, net
+279; production remains +562/-8, net +554. The focused dual-width rebuild/run
passes after that test-only extension. The owner was asked to keep an
audio-capable session available during qualification; no host-service mutation,
silent capability fallback or test skip is authorized or implemented.
Ninja repeatedly recovered a local log and rebuilt objects; that independent
build-efficiency observation is recorded in TODO rather than repaired here.
No complete S15 P is committed or pushed while required qualification fails.

The next owner-requested continuation repeats the complete unchanged suite in
separate qualification-retry-2 evidence. The preflight probe opens/closes MIDI
successfully and reports one Wave device. During x64 receiving tests, independent
probes in both normal and background execution contexts instead report zero
Wave devices and MIDI error 1. This establishes a native-host failure at that
time, not its cause or proof that every receiving failure has the same cause.
Both builds finish; x64 passes 97/97 shared, 52/66 other and 3/5 desktop
(152/168). x86 passes 96/97 shared, 42/66 other and 3/5 desktop (141/168).
All ten native desktop cases run, with both package cases failing per width;
all four separate-process archive routes pass. No test is skipped or weakened.
The job finishes normally with all.exit=1. Public directories and owner INI
remain unchanged. This attempt changes no production or test source; no complete
implementation commit, push, or qualified delivery is claimed. A stable usable
native audio endpoint remains necessary for the required receiving qualification.

The initial production estimate needs correction: fixed-width field maps,
owner copies and bounded MIDI/queue validation currently total approximately
560 added lines rather than at most 450. These are the existing codec's field
descriptors and necessary state boundaries, not new frameworks. Final line
accounting and simplification review remain pending.

Extend existing device capture/restore and fixed-width codec, not Lib archive
APIs. Reconstruct host handles and progress origin, approximate MIDI state,
and preserve exact emulated DMA/DSP/FM continuation. Validate all input sizes.
Estimated production +250-450/-10-30, net +220-440; tests +300-500/-0;
roughly 6-10 code paths plus manifest/evidence.

Prove uninterrupted versus restored device state, active DMA/pending IRQ,
partially parsed MIDI, no stuck notes/stale output, pause/debug/save/resume,
reset and destruction, and all cross-width directions. Generated guest
programs use disposable fixtures, not changed owner media. Real guest-driver
checks use only admitted available assets; do not invent Win3.x/95 driver
qualification from a port-level smoke. Final T acceptance explicitly records
the unavailable FM-host and approximate MIDI limits.

## Verification, accounting and approval gates

### S1 executor delivery checkpoint

The owner's continued implementation goal authorizes proceeding in single-agent
executor/coordinator order through the proposed shared Audio work and later
XP integration. This is not a manual playback acceptance or an automatic
waiver of the separately disclosed pause-hook/source-subset review boundaries.
The immediately preceding conversational turn only clarified the plan; this
checkpoint converts that plan into a verified design delivery.

Executor checks: git diff --check and the documentation governance gate pass.
Only this proposal and CURRENT differ from 6ac9bf0b9; production/test C/H,
EXEs, configuration and media are unchanged. The 24-entry ledger distinguishes
loaded-but-unused operations, native resources, host progress and device state.
Earlier source findings remain source evidence, not newly executed tests.
S1 makes no MyNES/NXVM runtime claim. Subsequent receiving repositories must
update PCM names and qualify their own callers; this repository cannot infer
their acceptance from its own shared-suite pass.

Coordinator actual-change review of pushed executor 2860b484: exactly two
planning files changed (+557/-181 including retained draft consolidation),
no source/test/package assets changed. Reviewed the actual committed proposal
and packet against the owner request: the host API census, no-synth decision,
public leaf layout, Compat/device distinction and S2-S8 proof requirements
are present. Source inference is not presented as playback evidence. S1 is
design-delivered under the continued implementation goal; T93 remains open.
S2 is next and may change names/paths only. The Emulator pause-hook approval
boundary remains at S6, not waived by this handoff.

Each runnable S: compile once per width in existing Ninja graphs, publish both
package EXEs, run shared suites first, all other unit/integration suites next,
then the five native desktop cases per width. The owner authorizes scheduling
these native tests without repeated approval requests on 2026-10-15.
Long work is hidden/supervised with declared logs, exit status and budgets;
no Computer Use or interactive-timeout termination. Do not call a background
pass a full pass. Preserve INI/media/snapshots and owner edits.

Each delivery reports production/test added, deleted and net lines; relocation,
retained XP source, local host patches and excluded XP ranges are separate.
Reference-size decreases do not erase real local mirror divergence. Update the
XP subset/manifest ledger, inspect actual diffs, commit/push the complete P,
then switch to coordinator for actual-change review. T closure waits for owner.

Specific approval boundaries remain: implementation admission after S1;
the disclosed Emulator pause-hook change; review of exact XP subsets and
notices before import. No external synth/font or kernel payload is requested.
A raw-FM driver or wider guest sound-card model is a separate scope, not a
hidden prerequisite to this XP-fidelity task.

The earlier source audit remains linked as
[XP sound evidence](../etc/evidence/softpc/xp-sp1-hunk-semantic-audit-zh.md).
S1 source findings are not tests. No new production build or playback was run
for this revision.

### S2 implementation checkpoint

The frozen S2 universe is the five existing PCM C/H files, two Lib PCM tests,
Compat audio consumer, four App sound tests, their build registration and
Lib ownership verification. All twelve C/H bodies equal baseline 14092722
after only the declared path/symbol/guard substitution. The Win32 SDK aliases
remain unchanged. No old public alias, root forwarding header, second backend
or XP source change is retained.

The five production files (793 lines) and two tests moved with git mv.
The renamed target is audio-pcm; the two test names are
lib.audio/pcm/stream and lib.audio/pcm/win32_platform. The dependency verifier
resolves the nested PCM leaf before applying the existing direct-edge and
private/platform header rules. Its exhaustive edge matrix remains intact;
new cases check same-leaf private/native access, cross-platform rejection,
root forwarding rejection and misplaced platform public headers.

Accounting command: git diff -M20% --numstat 14092722, grouped by owner and
file role, excluding manifests, docs and binary artifacts:

- Production C/H: six files, +166/-166, net 0.
- Test C: six files, +198/-198, net 0. This exceeds the initial reference-edit
  estimate because the existing test stubs contain more repeated symbol uses;
  no test case or behavior changed in those C files.
- Production build/verifiers: four files, +30/-21, net +9.
- Test build/verifier regression: two files, +35/-9, net +26.
- XP mirror: +0/-0. INI, media, snapshots and the other shared source packages
  are unchanged.

Both Release builds and each width's 90 shared plus 55 other background tests
pass. The standalone layout self-test also passes. The x86 Ninja invocation
reported recovery from a premature cache-file ending and rebuilt 584 steps;
x64 used 45 steps. No manual cache clearing or repeated compiler job was used.
Native desktop tests pass 5/5 per width: each architecture totals 150/150.
This includes the existing package startup and presentation roundtrip flows;
it is not a new manual audibility claim. Documentation and diff checks pass.
Build/test evidence is retained in the ignored S2 qualification and desktop
logs until the actual-change review; their result files both report zero.
Published EXE SHA-256:

- x64: 45CAA737DFAC408EE694768DDCB301CEF5A7A77CA3E33A66A9E7DE14D168CF29.
- x86: 34A4EB80F0138160E556AD6A25DAA0DFE6CB644DB564C45D7945C4CBA060B96D.

The similar-issue sweep checks stale audio stream API/guard/include/target
references throughout src, test and root build files. Only the deliberately
unchanged LIB_WIN32_AUDIO_STREAM_FLAGS SDK aliases remain. Executor delivery
7f642848 is pushed and actual-change reviewed; the
[S2 record](../history/M9-T93-S2-pcm-relocation.md) closes this step and CURRENT
admits S3 under the continued goal. T93 remains open.

### S3 native-stage checkpoint (not a delivery)

The first implementation stage changes PCM private/native code, Types
declarations and root rate/short-tail behavior, not Compat. Production is
currently +299/-28 across six C/H files; two Lib units and two affected
App fixtures are +374/-20. These
are interim counts, not the final S3 estimate or completion account.

- The Windows backend counts only successfully released input frames;
  initial silence is not submitted input. Its consumed result is input
  submitted minus native padding, clamped at zero while priming remains.
  Padding exceeding endpoint capacity returns I/O failure in all three
  observation/submission paths rather than looping on impossible geometry.
- Progress waiting reuses the same render/interruption event pair. No
  additional thread, polling timer or output queue is added.
- IAudioStreamVolume is acquired/released on the existing worker. Channel
  gains map 0..65535 to 0..1; there is no system/session volume change.
- Native unsupported-format errors remain UNSUPPORTED, distinct from I/O
  failures. Zero rates and byte-rate overflow are rejected before allocation.
- Linux retains truthful unsupported capability for these operations.

The real Windows adapter is tested with injected SDK calls, not a reimplemented
adapter. x64/x86 targeted builds and lib.audio/pcm/win32_platform pass. Tests
cover seven rates (3921, 8000, 11025, 12345, 22050, 44100, 48000), mono/stereo,
priming mixed with input, final consumption without another enqueue, underrun,
clear, committed prefix before interruption, counter overflow, invalid padding,
zero/full/intermediate gains, unsupported format and each initialization call
failure. Types-layout, manifests and whitespace checks also pass. This proves
native mechanics under injection, not root-worker notification or audibility.

The root now admits representable nonzero rates and submits any available
prefix, bounded by the existing batch size. A three-frame public enqueue
reaches the injected native writer without flush or a second producer call.
The clear regression explicitly holds the native writer before cancellation;
it no longer relies on the removed batching delay. Ordered delivery assertions
count frames/content, not scheduler-dependent batch counts. Root/native PCM
and the existing Compat first-delivery test each pass 50 consecutive runs per
width (150 runs each architecture). This is not a physical playback claim.

Startup now reuses control_done for the native admission result. Creation
publishes the task handle under the mutex before the worker can cancel it on
startup-signal failure. The creator waits on completion/cancellation and joins
before cleanup; an unproven worker exit never permits freeing its context.
Tests cover unsupported attach, failed startup signal, null failed outputs and
successful later creation. Gain uses the existing synchronous control slot and
same native worker; tests cover channel validation, zero/unity, inactive gain
and terminal native failure. After these changes all three targeted tests pass
another 50 repetitions per width. No App production or guest behavior is changed.

The public progress result now copies epoch and accepted/submitted/consumed
input-frame counters. Existing generation ownership becomes that 64-bit epoch;
clear cancels old ranges and rejects epoch overflow. Observation validates
monotonicity and bounds, and discards results captured for an older epoch.
The worker waits on native progress while a submitted tail remains, even when
its producer FIFO is empty. Enqueue interrupts that wait when new input arrives;
control requests use their existing interruption. There is no polling observer.
The optional wake callback is borrowed, invoked under the stream mutex and
must not reenter the stream. Replacing/unregistering quiesces the old context;
callback failure becomes terminal and wakes existing synchronous waiters.
Registration immediately reports current readiness, avoiding a missed prior
completion. Flush still proves submission, not physical audio consumption.
Tests separate submitted and consumed counts and drive a final native event
without another producer call. They also exercise enqueue during native wait,
clear cancellation, accepted/epoch overflow and failed sink notification.
All three targeted tests again pass 50 repetitions per width. Initial full
qualification exposed the machine sound test's incomplete native adapter
replacement: missing new symbols caused the linker to extract the real native
leaf and report duplicate definitions. Its existing fixture now supplies the
new progress operations and rejects unexpected gain/wait calls; no production
path changes for that repair. The search for platform-create/attach overrides
finds exactly this fixture and the already updated Compat first-delivery one.
Full dual qualification subsequently passes; the original failure is recorded
above rather than counted as success. See the final executor evidence below.

### S3 executor qualification

Counted against reviewed S2 endpoint 520f8b5b using `git diff --numstat`:

| Production path under src/lib | Added | Deleted | Net |
| --- | ---: | ---: | ---: |
| audio/pcm/stream.c | 162 | 24 | +138 |
| audio/pcm/stream.h | 10 | 1 | +9 |
| audio/pcm/stream_interface.h | 32 | 2 | +30 |
| audio/pcm/win32/stream.c | 71 | 1 | +70 |
| audio/pcm/linux/stream.c | 20 | 0 | +20 |
| types/win32/audio.h | 4 | 0 | +4 |
| Total | 299 | 28 | +271 |

Tests: Lib stream +218/-14, Lib native adapter +95/-4, App Compat first-delivery
+28/-0, App machine sound +33/-2: total +374/-20, net +354. Two hash manifests
are metadata, excluded from code totals. App/Compat production, Emulator,
shared X86, mirror, configuration, media and snapshots are unchanged; mirror
delta is zero. No obsolete second backend/worker/FIFO was introduced.

Qualification on both widths uses existing Ninja intermediates, shared suites
before other background suites, then serial reserved native desktop tests:

| Evidence | x64 | x86 |
| --- | --- | --- |
| Release package and all test targets | pass | pass |
| Shared background | 90/90, 40.14 s | 90/90, 42.76 s |
| Other unit/integration/checks | 55/55, 101.07 s | 55/55, 70.06 s |
| Native desktop | 5/5, 13.98 s | 5/5, 13.95 s |
| Registered total | 150/150 | 150/150 |

The final targeted PCM/root/Compat delivery set also passes 50 repetitions
of each case per width. Root tests exercise public APIs and real worker/FIFO
with an injected native endpoint; native tests execute the real Windows adapter
with SDK calls injected. Existing machine sound tests retain real PIT/PPI and
Compat/Lib delivery. These prove those boundaries, not physical audibility or
XP DSP integration (which remains S6). Linux has truthful unsupported stubs,
not a claimed tested output backend. Manifest, Types ownership, whitespace and
documentation gates pass. The x86 Ninja build reported a premature log end and
automatically rebuilt 584 steps; no agent cache purge or forced clean occurred.

Published SHA-256:

- softpc64.exe: `50C7167640CDAD7879840BDF0EC749BE460AD767155FB099164F72FAEBC07764`
- softpc32.exe: `199CFEC4C4848C1C47717C777B494CA5B49EE92DA98C35A9A2B30EB8EFD88F85`

Finite review covers six production files, four tests, two manifests and the
two package artifacts. Existing options remain two fields, so old callers do
not read uninitialized optional callbacks. The old native startup/queue owners
remain authoritative. All native fixture overrides found by the create/attach
search are updated. Clear epoch checks prevent stale native observations from
being published as current; the pending-wait clear test covers cancellation,
not an exhaustive proof of every concurrent schedule. No owner manual audio
acceptance is inferred. Executor `b31b6e17` is pushed; the separate
[actual-change review](../history/M9-T93-S3-neutral-pcm-contract.md) accepts S3
and admits S4 under the continuing goal. T93 remains open.

## Official host-contract references

- [WASAPI padding](https://learn.microsoft.com/en-us/windows/win32/api/audioclient/nf-audioclient-iaudioclient-getcurrentpadding):
  client-frame units and pending render data, not producer FIFO capacity.
- [MIDI long output](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutlongmsg):
  prepared buffer ownership; driver may complete synchronously or asynchronously.
- [MIDI reset](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutreset):
  returns pending buffers and stops notes/sustain; does not send EOX itself.
- [MIDI volume](https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-midioutsetvolume):
  optional support and handle versus device-wide scope.
- [FMSynth driver boundary](https://learn.microsoft.com/en-us/windows-hardware/drivers/audio/midi-and-directmusic-filters):
  hardware miniport, not a universal user-mode OPL API.
