# Current

## Current Work

### M5 T1 S5: Real-media Windows Setup and outer-UX closure

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** S4 recovered and proved the original
  timer/quick-event/8042 executor path on both supported host widths. The
  owner directed queue-ordered standalone recovery under single-person
  dual-role execution.
- **Objective:** close real configured-media Windows Setup behavior and the
  NXVM-style console/window presentation path without replacing original
  SoftPC graphics, input, controllers, BIOS, ROMs or BOP.
- **Input and output boundary:** input is the completed M5 host-contract and
  event evidence plus the current outer runtime. Output is a reproducible
  real-media behavior record and only thin presentation/host endpoint changes
  proved necessary by that record. Guest state remains executor-owned.
- **Verification:** x64 and x86 console/window/RDP input, A:, C: and A:+C:
  media routes, original text and graphics presentation, and configured
  Windows Setup progression.
- **Similar-issue sweep:** text geometry/cursor, window repaint cadence,
  RDP input translation, graphics dirty updates, pause/reset/stop, media
  overlay selection and both host widths.
- **Stop condition:** recovery requires DOS/WOW/NTVDM/CSR/VDD behavior,
  changes ROM/BOP/controller semantics, introduces a second device state
  owner, or grants a frontend direct machine-state access.
- **Exit criteria:** real configured media has evidence for responsive
  console/window interaction and Setup progression on both widths, with only
  original SoftPC machine paths beneath the outer runtime.

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
The completed timer/8042 callback proof is in
[M5 event/8042 contract evidence](../etc/evidence/m5-event-8042-contract.md).
The first S5 outer-shell repair removes redundant full-frame copies and
repaints while retaining the original renderer path; its scope and remaining
interactive acceptance work are in
[M5 outer frame-cadence evidence](../etc/evidence/m5-outer-frame-cadence.md).
