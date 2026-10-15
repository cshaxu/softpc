# XP Sound Blaster Standalone Capability

## Owner request and admission boundary

On 2026-10-15 the owner requests two tasks instead of coupling baseline
migration and new sound capability. This is the ordered follow-on candidate
to [T92 core migration](m9-xp-sp1-softpc-mirror-rebase.md); CURRENT and Queue
own status. No T number or implementation S is allocated before admission.
The complete former T92 S5 obligation transfers here, not to TODO or deletion.

## Objective and non-goals

Reuse the reviewed XP SB 2.0/DSP 2.01, A220/IRQ5/DMA1 device semantics with
standalone PCM/FM/MIDI output, reset and cross-width snapshot support.
No SB16, recording, second executor/device state owner or NTVDM product import.
Preserve existing PC-speaker playback. Public-package changes require a
separate explicit capability approval; device-specific state stays outside Lib.

## Planned serial S tasks

- S1: recheck frozen source, notices/admission, resolve missing OPL synthesis
  and MIDI destination, define one device/host ownership boundary and exact
  file/diff estimates. Exit is an approved executable design, not silent stubs.
- S2: device port/DSP/DMA/IRQ and PCM path. Prove reset/version, transfer
  modes, IRQ acknowledgement, sample conversion, first/repeated playback
  and stalled-host independence. Keep intermediate unsupported features honest.
- S3: Mixer and FM/MIDI completion. Prove gain/mute, register/timer behavior,
  actual synthesis/output, complete first/last MIDI message and SysEx/realtime
  boundaries; reject silent fake success.
- S4: lifecycle and snapshot integration plus full qualification. Earlier S
  steps must already clean up their own resources; this step proves combined
  reset/stop/destruction, active transfer save/restore and cross-width state,
  guest-driver use, PC-speaker coexistence and final complete scope ledger.

Each implementation S estimates files/additions/deletions/net before work,
then measures production, tests and XP mirror divergence, builds both widths,
publishes both EXEs, runs focused/full regression and reserved native desktop
checks as required, commits/pushes and receives actual-change review. Closure
requires owner acceptance. No user INI/media mutation or Computer Use.
Source rights and the missing output backend decisions are stop conditions,
not waived by retaining this candidate. Seven reference files/6,097 lines are
NOT an implementation estimate.

## Device and host design inherited from the source audit

S1 has established that XP's FM path depends on a native FM device and
NtVdmControl, not a software OPL-to-PCM generator. The final the sound task design must
explicitly account for that missing standalone capability rather than promise
a complete sound card through file copying alone. AT DMA already calls the XP
sound position hook in its normal path; migrate those changes with their
device/host owner, not as an independent low-risk representation change.

Evaluate XP-only Sound Blaster paths as an IBM PC/AT capability batch, not as
PC-98 work and not as an unrelated greenfield device. Bring accepted
XP-derived device-model files into the XP mirror, while explicitly excluding
VDD/`NtVdmControl`, NTVDM DMA/port hooks and other product-host material.
Attach the resulting device through repository-owned standalone boundaries for
ISA port decode, DMA, IRQ, DSP, FM/AdLib, mixer and audio output. The device
model remains XP-derived; only its host attachment is standalone-owned.
Account explicitly for the SbTerminate and SbCloseDevices obligations found
in XP's unselected terminat.c and nt_reset.c, plus initialization after reset
in nt_msscs.c and closure before host blocking in nt_event.c: attach these
obligations to the existing machine lifecycle, distinguishing device state
from host resource lifetime. Attach device teardown and host
resource closure to the existing lifecycle, without importing a second NTVDM
main/reset/worker path. Verify reset, stop, destruction and snapshot alongside
first playback; port registration alone is not complete sound-card adoption.

The MIDI/mixer audit adds explicit prerequisites: UART-output-only MPU scope,
a real MIDI output/synthesis destination separate from PCM Audio, full-message
tail delivery, native-width host pointers, bounded buffer ownership, zero-volume
correctness and independent master/channel gain. Do not preserve XP's 5-second
INQUEUE polling loop or reuse its global host-volume controls as device mixing.
The linked audit records the concrete source defects. the sound task requires a bounded
device/host design before implementation; importing seven files alone cannot
satisfy the IBM PC sound capability requirement.

#### Concrete standalone design and admission boundary

Keep one guest-visible device owner in the XP-derived files. Port dispatch,
DSP command parsing, DMA transfer count/address, IRQ assertion/acknowledgement,
MPU parser and mixer registers are executor-owned. Do not put a second copy of
those fields or a command interpreter in Compat. Replace only NT host seams:
NtVdmControl/VDD registration, WinMM handles/buffers, native volume, worker
completion and wall-clock reads. Preserve original source paths and mark each
host seam in the XP divergence ledger; no whole-file standalone-only fork.

Use existing device timing delivery and quick-event infrastructure, not host
playback position as a second guest DMA clock. Audio callbacks cannot read or
advance guest RAM/DMA/IRQ. PCM delivery carries copied samples outwards; a
host completion only reports sink capacity/failure, never raises a guest IRQ.
the sound task must prove this timing contract with DMA count/IRQ observations, including
host output stalled, pause/resume and restore. Actual chunk size and pacing
must be selected by that proof, not a new fixed Sleep or nominal CPU frequency.

The existing Lib stream accepts copied interleaved i16 PCM, up to 512 frames
per enqueue, at 22050/44100/48000 Hz, with authoritative accepted-frame count.
Unsigned DSP samples, ADPCM state, device sample rate and master/channel gains
are device semantics; conversion/mixing occurs before this boundary. The
private output adapter handles partial enqueue, wait cancellation and explicit
tail flush. Do not reinterpret queue acceptance as audible completion or use
Lib readiness as the guest clock. Do not change the accepted PC-speaker onset
handoff merely to attach a second source; any shared output refactoring needs
first/second playback and concurrent-speaker regression.

Machine initialization/reset/stop/destruction invoke the same device entry and
host-resource lifecycle. Snapshot captures fixed-width device/parser/sample
phase and pending-event state at the existing executor safe point; pointer,
WinMM handle, native queue and already-heard samples are not archive data.
Restore reconstructs host output and cancels stale-generation PCM. Existing
snapshot safe-point, single executor and cross-width format remain authorities.
No version compatibility is implied for a format extended with a new device.

Known file boundary: seven XP candidate files (6,097 reference lines), existing
at_dma.c/ios.h port integration, Compat audio/host seams, Machine lifecycle,
device snapshot records/codec and owner-local tests. Lib/Emulator/X86 product
APIs do not need SB-specific fields. A neutral MIDI host backend, if selected,
is a separate public-capability decision, not permission to put WinMM calls in
guest protocol code. A software OPL engine likewise needs its own reviewed
origin or implementation design; XP's native FM route supplies neither.

**S1's executable verdict is that the sound task is not yet source-import-ready.** Before
admitting its implementation, the owner must select and admit the OPL/MIDI
output solution and resolve source rights. T92 core batches can proceed
independently after their source-admission gate. The owner explicitly separated
sound from T92 on 2026-10-15. This candidate retains the full sound obligation;
a DSP-only demonstration does not complete it.
No defensible final source-line estimate for a missing synthesis backend is
available from this XP tree. Report the known seven-file reference size and
the precise interface list above; do not invent a small implementation budget.

The the sound task proof must include reset ACK/version, single and auto-init DMA, count
readback and IRQ acknowledgement, unsigned PCM/implemented ADPCM samples,
zero/master/channel gain and register readback, a one-message MIDI stream and
SysEx/realtime boundaries, FM timer/register and actual PCM output, first and
repeated playback, stop/cancel/teardown, and cross-width restore of an active
transfer. Unsupported XP operations remain explicitly unsupported; no SB16 or
recording claim is made. These tests observe ports/state/PCM, not just UI text
or a human hearing one tone.


## Evidence and completion

The [source audit](../etc/evidence/softpc/xp-sp1-hunk-semantic-audit-zh.md)
and its retained/unretained path ledgers provide the immutable source census.
Its old “T92 S5” references identify this transferred batch. Final acceptance
requires the entire DSP/mixer/FM/MIDI/lifecycle/snapshot ledger, tested on
x86/x64, rather than file-copy counts or one audible tone.
