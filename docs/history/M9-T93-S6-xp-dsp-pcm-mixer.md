# M9 T93 S6 — XP DSP, PCM And Mixer Integration

## Request and delivery

The owner requests XP NTVDM-parity sound through neutral Lib audio, authorizes
sequential S delivery and explicitly approves the existing Emulator heartbeat
hook at pause/resume, conditional on a public-code-quality audit afterward.
S6 is DSP/PCM/Mixer only; MIDI/FM attachment and active sound-state snapshots
remain S8/S9. T closure still waits for owner acceptance.

Executor `065ac9ed` is pushed. The selected XP bodies and exclusions are pinned
by the mirror manifest and the S6 range ledger. Original DMA, PIC, DSP parser
and Mixer state remain their existing owners. Compat supplies copied PCM,
host progress and EOI callbacks; the existing executor and quick-event route
replace NTVDM thread/WinMM ownership. No second device clock, executor or
controller is introduced. Native playback failure remains observable.

Production C/H: +2300/-11, net +2289, across 19 paths. The nine mirror paths
account for +2077/-1, net +2076; Compat five paths +192/-1; Machine three
paths +21/-5; Emulator two paths +10/-4. Tests: five C files +1287/-13, net
+1274. Root CMake is +30/-1 and the mirror checker +1/-1. Manifests, provenance,
documents and EXEs are separate. Against the pinned XP necessary subsets,
the nine mirror files total +1081/-1811, net -730, including earlier local
patches; that is not the S6 incremental footprint. Per-file figures and
excluded-range dispositions are in the [proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s6-final-qualification-and-public-change-audit).

## Verification and limits

Both Release builds refresh the package EXEs. Final qualification passes
160/160 per architecture: 94 shared first, 61 other background next, then five
individually isolated native desktop tests. All ten native tests pass. Earlier
failed source-boundary/build attempts and the mistaken empty shared selector
remain recorded, rather than being erased by the successful final run.

Focused proof covers actual DSP/DMA/PIC ports, register/protocol matrices,
completion/IRQ/EOI, decrement/wrap and readback, partial accepted prefixes,
host failure, pause/resume, reset/teardown and first/repeated delivery. The
real DSP/Compat/Lib-worker test injects only the final platform leaf. It proves
PCM delivery, not physical audibility. Pause retains the copied guest block
and DMA obligation; the copied host checkpoint is not sample-exact physical
endpoint stopping. No user INI, media or snapshot is modified.

Published SHA-256:

- x86: `67e8d7b83ede74bd4f5b3cd533f55d1791f1bf3f41f8fc212f03b989f8fdf979`
- x64: `fd4b6fcec5e13982465bea67eab1303ee5cb236c41415269948e498dfbb16669`

## Coordinator actual-change review

After executor push, the same agent switched roles and inspected the actual
commit, device and Compat implementations, lifecycle diff, test implementations,
subset accounting, full-run terminal result and matching HEAD/origin identity.
Review confirms the existing I/O tables remain the sole registration authority;
the original PIC counts interrupts; EOI removal precedes withdrawal; failed
host destruction retains ownership; the copied PCM prefix is not guest DMA
completion; and ordinary/debug/private-capture pause use the same executor.
The public Emulator edit adds no dependency, public symbol, stored state,
platform call, lock, queue or thread. Types, corpus/manifest and inward-test
checks pass. Broader known public-quality gaps remain explicitly assigned S7.

The staged whitespace check reports one inherited trailing blank line in
sndblst.h, identical to the selected XP body; mirror fidelity retains it.
No new runtime defect was found in this bounded review. Active DSP archive
coverage and MIDI/FM are not claimed complete. S6 meets its admitted criteria
and closes under delegated sequential acceptance; S7 is admitted next.
