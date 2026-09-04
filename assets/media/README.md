# Guest media

`assets/media/` contains reusable guest disk images for the fixed SoftPC
machine. `assets/binary/softpc.ini` may reference them with paths relative to
its own directory, normally `../media/<image>.img`.

The shipped images are package inputs, not build output. The VM must respect
the user-selected `readonly`, `direct`, or `overlay` attachment mode; tests and
agents must not mutate an image unless an admitted task explicitly authorizes a
disposable copy.

This directory contains guest media only. The selected original BIOS, VGA ROM,
and CMOS inputs remain source-mirror resources under
`src/mvdm/softpc.new/roms/`, are embedded at build time, and have no
`assets/roms/` runtime equivalent.
