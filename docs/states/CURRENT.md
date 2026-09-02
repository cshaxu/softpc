# Current

## Current Work

### M5 T1 S4: Original timer, quick-event and 8042 contract recovery

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** S3 established capability owners for the six
  outstanding machine-source rows and moved Set-1 lookup ownership back to
  the original host input algorithm. The owner directed queue-ordered
  standalone recovery under single-person dual-role execution.
- **Objective:** recover the exact original timer/quick-event and 8042 host
  callback order needed for reliable multi-byte keyboard input and media
  activity, without creating a second clock, frontend polling loop, or device
  implementation.
- **Input and output boundary:** input is the S3 contract evidence, existing
  original timer/keyboard sources and standalone executor endpoints. Output
  is a callback-order trace and, only if proved necessary, a narrow host
  endpoint adjustment. Controller protocols, ROMs, BOP selectors, guest
  media, CCPU and C-VID are not changed.
- **Verification:** x64 and x86 focused multi-byte keyboard and machine
  tests, full applicable regression and real-media Setup input proof precede
  any presenter or UX work.
- **Similar-issue sweep:** HLT wake, bounded execution, repeated resets,
  timer quick events, typematic make/break, PPI Timer-2 audio, floppy/hard
  disk activity and both host widths.
- **Stop condition:** recovery requires DOS/WOW/NTVDM/CSR/VDD behavior,
  reinterprets a controller protocol, mutates BOP/ROM data, or gives the
  frontend direct guest-state access.
- **Exit criteria:** traced callback order explains all executor wake/input
  routes; original source owns device state; x64/x86 proof shows responsive
  multi-byte keyboard input and stable machine execution.

## Current Technical Baseline

M4's mailbox runtime, one executor and copied presentation snapshots remain
the outer-shell baseline. Fixed A:+C: boot and independent input rendezvous
are recorded in [M4 runtime-media/input evidence](../etc/evidence/m4-runtime-media-input.md).
The original dispatcher and finite DIB endpoint now provide the Setup welcome
path on both widths; the generated CCPU outer return also preserves repeated
bounded-run state. See the completed [M5 S1 history](../history/M5-T1-renderer-dispatcher-restoration.md),
[M5 renderer/executor acceptance evidence](../etc/evidence/m5-renderer-executor-acceptance.md)
the completed [M5 machine/host contract evidence](../etc/evidence/m5-machine-host-contracts.md),
and [M5 outer runtime and pristine restoration](../proposals/m5-outer-runtime-and-pristine-restoration.md).
