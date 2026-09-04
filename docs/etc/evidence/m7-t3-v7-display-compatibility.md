# M7 T3 Original V7 Display Compatibility Proof

## Covered Original Paths

`softpc-vga-frame-smoke` creates the fixed standalone machine and drives the
loaded original V7 VGA BIOS extension through its original EGA BOP (`42h`),
not a frontend mode emulator. It proves the following original controller and
renderer combinations write the standalone-owned DIB surface:

- initial text mode and the original loaded EGA/VGA glyph planes;
- the V7 memory query (`INT 10h/6Fh/07h`) returns its original `AX=826Fh`
  identity: two 256 KiB VRAM blocks, i.e. 512 KiB total;
- V7 mode `60h` (752x410 16-colour) and mode `63h` (1024x768 2-colour)
  select their original controller geometry; neither is misrepresented as a
  packed 256-colour mode;
- V7 mode `65h` (1024x768 16-colour): original `nt_graph` selects
  `nt_ega_hi_graph_std`, including its original four-plane LUT expansion, and
  the controller selects a 1024x768 DIB; and
- V7 modes `66h` (640x400 256-colour) and `67h` (640x480 256-colour): the
  original `nt_v7vga_hi_graph_std` copies guest pixels and publishes its
  original dirty rectangle to DIB output;
- V7 modes `68h` (720x540 256-colour) and `69h` (800x600 256-colour): the
  same original painter and dirty-output route operate at their original V7
  geometries; and
- original VLT/DAC palette propagation, dirty rectangles, text-to-graphics
  transition, and empty/wrap-edge dirty input handling.

The independent host contributes only a DIB allocation, mutex-compatible
surface, and invalidation record. It does not decode guest VRAM or select a
replacement renderer.

## Intentional Scope

The standalone DIB adapter uses the original `vd_ext_graph_table` only to size
the V7 proprietary 256-colour output surface. This replaces the old NT console
formula that incorrectly allocated a 1280-pixel surface for modes `66h` and
`67h`; it does not alter V7 registers, VRAM, BIOS, or the original painters.

The full-frame paths for the high-resolution planar modes remain unaccepted
until their original dirty update/painter limits have direct regression
coverage.

## Verification

- x64 clean serial CTest: 19/19 passed.
- x86 clean serial CTest: 19/19 passed.
- Focused `softpc-vga-frame-smoke` passed on both widths as part of those
  sweeps.
