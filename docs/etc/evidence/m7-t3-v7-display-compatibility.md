# M7 T3 Original V7 Display Compatibility Proof

## Covered Original Paths

`softpc-vga-frame-smoke` creates the fixed standalone machine and drives the
loaded original V7 VGA BIOS extension through its original EGA BOP (`42h`),
not a frontend mode emulator. It proves the following original controller and
renderer combinations write the standalone-owned DIB surface:

- initial text mode and the original loaded EGA/VGA glyph planes;
- V7 mode `60h` (640x400 256-colour): original `nt_v7vga_hi_graph_std`, with
  the controller-selected 752x410 DIB geometry;
- V7 mode `63h` (800x600 256-colour): the same original packed V7 painter;
- V7 mode `65h` (1024x768 16-colour): original `nt_graph` selects
  `nt_ega_hi_graph_std`, including its original four-plane LUT expansion, and
  the controller selects a 1024x768 DIB; and
- original VLT/DAC palette propagation, dirty rectangles, text-to-graphics
  transition, and empty/wrap-edge dirty input handling.

The independent host contributes only a DIB allocation, mutex-compatible
surface, and invalidation record. It does not decode guest VRAM or select a
replacement renderer.

## Intentional Scope

The fixed V7 profile makes modes `60h`, `63h`, and `65h` representative of the
three distinct original routes used here: proprietary packed 256-colour,
proprietary geometry, and planar 16-colour. Other V7 table entries remain
owned by the same original controller/paint dispatch and are not advertised as
separately guest-tested modes.

## Verification

- x64 clean serial CTest: 19/19 passed.
- x86 clean serial CTest: 19/19 passed.
- Focused `softpc-vga-frame-smoke` passed on both widths as part of those
  sweeps.
