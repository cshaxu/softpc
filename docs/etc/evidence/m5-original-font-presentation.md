# M5 Original Font Presentation

## Boundary

The text window consumes only a copied runtime frame.  Its glyph bitmap is
copied from the currently selected original EGA/VGA primary font plane through
the machine presentation boundary.  The source layout and font-bank selection
are the original `EGA_planes`, `FONT_BASE_ADDR`, primary-font index and BIOS
character-height state; neither the window nor the runtime reads guest VRAM
or controller state directly.

The exported frame has 256 CP437 glyph slots of 8x16 rows.  Rows beyond a
shorter current original font are cleared.  Thus the normal fixed profile is
8x16 while the output remains tied to the font that the original firmware and
controller have actually loaded.

## Resolution

The Win32 text surface is a local 32-bit DIB.  It bitblts the copied glyph
rows with the copied text attributes.  It no longer creates or selects a
Windows font and does not call `TextOutA`.  A font snapshot change invalidates
the presentation cache even when the text cells themselves did not change.

`host_release_timeslice` yields only after the original idle keyboard-poll
path asks for it.  This is outer host scheduling only: it neither sleeps guest
time nor creates a second clock or changes original device/ROM/BOP behavior.

## Verification

- `softpc-vga-frame-smoke` proves that a reset machine exports the original
  8x16 font and contains source rows for printable CP437 `A`.
- `softpc-win32-window-smoke` proves the actual Win32 endpoint still starts,
  paints and closes through the runtime lifecycle.
- Serial CTest passed `18/18` on both x64 MinGW and x86 Clang.
- `softpc-real-boot-smoke --floppy O:\assets\fdd.img --hdd
  O:\assets\hdd1.img --overlay --windows-setup` reached the Windows Setup
  welcome screen on both widths after the change.

The media runs are local overlay evidence only; no guest-media byte is a
repository input or modification.
