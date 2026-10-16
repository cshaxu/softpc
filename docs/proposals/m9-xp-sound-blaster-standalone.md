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
S2-S8 below are proposed implementation steps, not started by this document.

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
S7 must record which XP branch each test represents and retain one standalone
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
unsupported acquisition contract and inward dependencies. S7 handles the
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
is required in every integration S; S8 is combined qualification, not permission
to ship unsafe intermediate teardown.

## Planned serial S tasks

These estimates are engineering envelopes, not measured diffs or blanket
permission to exceed scope. C/H production and tests are separate; build,
manifests/docs and binary artifacts are counted separately at delivery.
Every implementation S refreshes its exact file/add/delete brief first.

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

### S5 — original DMA prerequisite repair

Change at_dma.c's existing decrement helpers/split and, only if justified,
the existing DMA result contract/header. Do not duplicate transfer logic.
Production +20-50/-10-25, net -5 to +40; tests +120-220/-0 in the existing
device_contract_smoke fixture. Roughly 2-3 code paths plus mirror manifest.
Use the complete canary/wrap matrix above. Remove the corresponding TODO only
after actual proof. Keep unrelated debt and T92 accepted repairs intact.

### S6 — XP DSP/PCM/Mixer and host integration

Import necessary subsets of sndblst.h, nt_sb.h, nt_sb.c, nt_sbdsp.c and
nt_sbmxr.c, preserving source identity/notices and minimal explicit patches.
Wire original ports/DMA/PIC, Lib PCM through Compat sound host files, and
Machine lifecycle; no NTVDM VDD loader or second device thread.
Estimated retained XP source +1300-2200/-0; local production +250-450/-20-60,
net +190-430; tests +300-500/-0. Approximately 12-18 code paths.
Estimate includes narrow pause-hook implementation if separately approved;
its Emulator tests add +60-100/-0. Do not silently implement that shared change.

Before payload, publish the exact selected-range/host-seam diff for review,
especially scheduling and DMA-position mapping. Preserve source behavior where
possible; source fidelity does not require copying unsafe host pointers or
polling teardown. No standalone-only branch inside mirror source.
Tests: reset/version, speaker gate, single/auto/high-speed transfer, count and
EOI/IRQ ordering, pause/continue, zero/master/channel gain, rate changes,
first/repeated bursts, no endpoint/stalled endpoint and no use-after-free.
Compare host progress and guest observations separately. Each changed source
policy or XP workaround gets an explicit disposition, not a hidden rewrite.

### S7 — XP MIDI/FM device attachment

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

### S8 — portable archive and final qualification

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
