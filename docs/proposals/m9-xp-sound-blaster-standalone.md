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
insertion below expands the remaining sequence through S9; CURRENT alone
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

### S8 — XP MIDI/FM device attachment

Admitted after the reviewed S7 closure under delegated sequential execution.
No S8 production change is claimed at admission.

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

### S9 — portable archive and final qualification

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
