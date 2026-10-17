# M9 T93 S15: Portable Sound Archive And Test Ownership

Owner accepts S15 on 2026-10-16: "好的，验收通过。接下来，进行全T审计收口".
S15 closes after the reviewed executor sequence 66058ab4, 65dacdd5/0dfc02cf,
77cd7263 and ac98a7b6, followed by fresh final qualification 161305b2.
The separate [T audit](M9-T93-completion-audit.md) owns whole-task closure.

## Delivered obligations

The [retained proposal](M9-T93-xp-sound-blaster-standalone-proposal.md#s15--portable-archive-and-final-qualification)
contains the original requests, field census, failures, repairs and reviews.

- Existing fixed-width device archive includes DSP transfer/parser, Mixer,
  pending EOI, MPU/MIDI and FM state. Host handles and native progress origins
  are reconstructed, not serialized. Stop/restore ordering preserves PIC and
  queued obligations; the x86 q_ev_handle call-site conversion is corrected.
- Continuation tests cover partial DSP/MIDI commands, active/auto DMA,
  backpressure, pending completion/IRQ, restored actual PIC acknowledgement,
  notes/pedals, malformed state and all four cross-width writer/reader routes.
- Owner-approved units no longer require live native MIDI/audio endpoints.
  App-owned sinks replace audio only in non-audio units; real native device,
  audio and desktop integrations remain separately registered and executed.
- Shared test folders match the tested source owner, including platform leaves.
  Units test their own responsibilities using legal inward dependencies.
  Five added platform tests and split owner assertions preserve capability
  distinctions; Linux unsupported contracts do not masquerade as full backends.
- Four Lib negative fixtures are flat and byte-preserved. Build fixtures keep
  required negative directory contexts. Full relative paths identify targets.
- The sound unit compares a fixed sample prefix across variable worker batches,
  retaining amplitude/polarity assertions rather than assuming equal dequeues.
- Owner Monitor/snapshot edits were preserved as explicitly requested. The
  intermediate two manifest failures were corrected in 0dfc02cf and are not
  reported as passing evidence.

## Scope accounting and final proof

Against S15 admission 41693590 through 91a3fde2, rename-aware C/H numstat:
production +563/-9, net +554; tests +1790/-941, net +849. This includes the
owner's Monitor text change and all approved test-layout continuations, not
only the initial archive patch. Docs, manifests, CMake and EXEs are separate.
Five mirror sound owners account for +324/-4 against unchanged XP subsets;
the remaining archive code is in Compat. Final closure adds no source/test code.

Fresh full qualification passes 193/193 per width: 121 shared, 64 other
background, five native desktop, three native audio. Exact registered-name
coverage has no skips, failures, missing or duplicate entries. Four cross-width
archive routes pass. Both Release builds pass; final hashes and evidence are
in the T audit. No Computer Use or interactive-timeout termination occurred.

## Actual-change review and limits

Following pushed P9, coordinator separately inspects its three-path change:
final audit, status checkpoint and rebuilt x86 EXE. Source/test trees are
unchanged from accepted 91a3fde2; final hash/inventory/exit records match the
delivery. Prior actual-change reviews cover each implementation continuation.
The closure review rechecks the entire S/T request ledger and independent debt
receivers; it is not a new claim of exhaustive concurrency/guest compatibility.

INI/media/current snapshot are untouched by final qualification. MIDI
restoration remains approximate and old snapshot formats intentionally fail.
Win95 driver recognition is separately queued, not certified by these tests.
Known intermittent test/build debts remain explicit. S15 is accepted and closed.
