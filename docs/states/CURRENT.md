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
Console and window now share one thin Win32/RDP packet normalizer while
retaining original SoftPC key translation and controller ownership; see
[M5 shared input-normalizer evidence](../etc/evidence/m5-win32-input-normalizer.md).
The console now presents original text attributes rather than flattening them
to the host default palette; see
[M5 console text-attribute evidence](../etc/evidence/m5-console-text-attributes.md).
Paused floppy insertion/ejection now follows the documented monitor contract
without giving the monitor controller access; see
[M5 paused-media evidence](../etc/evidence/m5-paused-media-contract.md).
The delivered fixed INI now names the verified A:+C: media pair and both
width-specific launchers reach the monitor without arguments; see
[M5 delivered-launcher configuration evidence](../etc/evidence/m5-delivered-launcher-configuration.md).
Graphics snapshots now consume the original renderer dirty signal before
copying a full DIB, keeping static graphics from consuming executor time; see
[M5 original dirty-frame publication evidence](../etc/evidence/m5-original-dirty-frame-publication.md).
The actual Win32 window now has a proved close-to-runtime-stop lifecycle;
see [M5 Win32 window lifecycle evidence](../etc/evidence/m5-win32-window-lifecycle.md).
Its client rectangle follows the NXVM sizing rule and contains no frontend
display margin.
Window text now uses the currently loaded original EGA/VGA font plane rather
than a host font; see [M5 original font presentation evidence](../etc/evidence/m5-original-font-presentation.md).
Three controller/firmware extraction routes remain after restoring the original
PPI controller, incompatible-driver BOP path and CPU_40 keyboard reset path
through their standalone host contracts; the PPI and keyboard results are recorded in
[M5 remaining pristine-controller boundaries](../etc/evidence/m5-remaining-pristine-controller-boundaries.md).
