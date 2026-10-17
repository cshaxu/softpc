# M9 T94 S1 - Windows 95 Sound Blaster Diagnosis

## Request and boundary

Owner admits the next task and supplies a Windows 95 snapshot without an audio
driver, authorizing independent installation. The full objective remains
standalone XP-NTVDM-parity audio usable for Win95 Sound Blaster playback.
S1 is diagnosis/design, not playback qualification. T94 remains open.

## Evidence and causal result

Executor delivery `20298b44` records the baseline, overlay-only installation,
bounded port traces and controlled callback experiment in the
[task proposal](M9-T94-win95-sound-blaster-recognition-proposal.md).
Windows 95 4.00.950 uses Microsoft mssblst.drv 4.0.950 with IRQ 5, DMA 1 and
base 0220, matching the existing DSP 2.01 model.

Baseline guest restart clears adapter 36 to adapter 0 during original
`io_init()`. No subsequent sound registration occurs. The driver consequently
reads FF instead of AA at the reset acknowledgement. Only the outer Machine
reset path calls `SbInitialize()`; guest BIOS reset bypasses that site.

An ignored probe wraps the existing reboot and host-reset callbacks with the
existing termination/initialization functions. It restores AA, version 02/01,
complement reply 55, subsequent IRQ/DMA commands and normal device status.
The original core and public components are unchanged. This is causal guest
evidence, not a production patch. Test audio receivers permit diagnosis without
a host endpoint; they do not prove audible playback or DMA completion.

## Repair receiver

S2 must move binding into existing Compat reset callbacks, preserve teardown
before queue reset, remove Machine's redundant late initialization, and propagate
callback failures through existing executor readiness/exit. No second device
path or guest-specific condition is justified. Expected production scope is
two files, +20-35/-8-12 lines, net +8-27; tests need cold/warm/repeated reset,
pending event teardown and injected failures. S3 retains actual playback and
full lifecycle/dual-width regression qualification.

## Actual-change coordinator review

Reviewed the complete P1 diff against the original request and active packet:
three documentation files, +128/-19, net +109; tracked production and tests
each +0/-0. No EXE, public API, mirror, INI, snapshot or guest-image edits.
Documentation governance and whitespace checks pass. The diagnostic exits
with shutdown, destroy and VM destroy all successful. Source trace, guest
evidence and absent native playback proof are explicitly distinguished.

S1's finite scope is satisfied: restore, installation/resources, first failed
exchange, causal experiment, minimal repair design and evidence limits are
recorded. Native playback and formal repair remain incomplete T94 work; no
whole-task acceptance is claimed. S1 is closed after the pushed delivery and
this separate actual-change review.
