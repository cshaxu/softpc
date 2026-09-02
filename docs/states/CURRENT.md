# Current

## Current Work

### M5 T1 S3: Machine-host contract declaration extraction

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** S2's fail-closed audit identified six machine
  files whose standalone branches can be removed only after host declarations
  exist: reset, keyboard/PPI, CMOS, timer and illegal-driver handling. The
  owner directed queue-ordered standalone recovery under single-person
  dual-role execution.
- **Objective:** define the smallest independent host-contract surface needed
  to restore those six original machine inputs. Contracts cover media presence,
  Timer-2 audio notification, keyboard ingress/reset notification, timer and
  explicit unavailable-product reporting. They do not implement a controller.
- **Input and output boundary:** input is the S2 route ledger and existing
  standalone host providers. Output is host-facing declarations and a source
  replacement map. Original controller/BIOS behavior, ROMs, BOP selectors,
  guest media, CCPU and C-VID are not changed in this subtask.
- **Verification:** every proposed contract has one existing or deliberately
  unavailable host owner; source-boundary checks prevent frontend access to
  guest state; clean x64/x86 builds and focused contract tests precede any
  source restoration.
- **Similar-issue sweep:** fixed-disk presence, floppy CMOS reporting,
  keyboard reset and Set-1 ingress, timer quick events, PPI Timer-2 sound,
  product-driver BOP failure, and both host widths.
- **Stop condition:** an endpoint requires DOS/WOW/NTVDM/CSR/VDD behavior,
  reinterprets a device protocol, mutates BOP/ROM data, or owns guest state.
- **Exit criteria:** named contracts and their existing providers cover all
  six restoration rows; the next source-restoration patch can remove only
  machine-side standalone branches and passes x64/x86 focused proof.

## Current Technical Baseline

M4's mailbox runtime, one executor and copied presentation snapshots remain
the outer-shell baseline. Fixed A:+C: boot and independent input rendezvous
are recorded in [M4 runtime-media/input evidence](../etc/evidence/m4-runtime-media-input.md).
The original dispatcher and finite DIB endpoint now provide the Setup welcome
path on both widths; the generated CCPU outer return also preserves repeated
bounded-run state. See the completed [M5 S1 history](../history/M5-T1-renderer-dispatcher-restoration.md),
[M5 renderer/executor acceptance evidence](../etc/evidence/m5-renderer-executor-acceptance.md)
and [M5 outer runtime and pristine restoration](../proposals/m5-outer-runtime-and-pristine-restoration.md).
