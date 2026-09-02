# Current

## Current Work

### M5 T1 S1: Original renderer dispatcher restoration

- **Owner and mode:** repository owner; single-person dual-role execution.
- **Admission and approval:** M4 acceptance exposed a source conflict: the
  compiled standalone `nt_graph.c` branch is a 377-line local duplicate of
  original graphics coordination, so Windows Setup cannot prove the required
  original renderer path. The owner directed execution of the ordered queue
  and approved the NXVM-style outer architecture.
- **Objective:** restore the original SoftPC `nt_graph` dispatcher, mode
  change, VLT, graphics-tick and paint-selection data flow. Move only its
  finite console-DIB outlet to the standalone compatibility host, while
  `vm/runtime` remains a frame client.
- **Input and output boundary:** input is pristine OpenNT `nt_graph.c`,
  original `nt_cga`/`nt_ega`/`nt_vga` painters, and the standalone DIB surface.
  Output is pristine renderer source plus a named host compatibility layer for
  DIB allocation, palette publication, text geometry and product-window
  endpoints. Controllers, firmware, ROMs, CCPU, BOP tables and guest media
  are not outputs.
- **Verification:** a source-divergence check proves the local renderer no
  longer contains a standalone dispatcher; VGA/text-surface checks exercise
  original mode selection, VLT and dirty rectangles; clean x64/x86 builds and
  runtime/lifecycle/keycode/source-boundary tests precede real A:+C: Setup.
- **Similar-issue sweep:** text/graphics transitions, palette changes, V7
  modes, DIB resize lifetime, text stride, cursor/product-window calls, frame
  contention and both host widths.
- **Stop condition:** restoration requires a DOS/WOW/NTVDM/CSR/VDD service, a
  BOP or ROM change, a second graphics-state owner, or a renderer rewrite
  instead of a finite host endpoint.
- **Exit criteria:** original dispatcher is compiled; host supplies only the
  DIB/window endpoint; Setup graphics and welcome screen work on x64 and x86.

## Current Technical Baseline

M4's mailbox runtime, one executor and copied presentation snapshots remain
the outer-shell baseline. Fixed A:+C: boot and independent input rendezvous
are recorded in [M4 runtime-media/input evidence](../etc/evidence/m4-runtime-media-input.md).
The original dispatcher and finite DIB endpoint now provide the Setup welcome
path on both widths; the generated CCPU outer return also preserves repeated
bounded-run state. See the [M5 renderer/executor acceptance evidence](../etc/evidence/m5-renderer-executor-acceptance.md)
and [M5 outer runtime and pristine restoration](../proposals/m5-outer-runtime-and-pristine-restoration.md).
