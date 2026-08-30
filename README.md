# SoftPC VM

Standalone PC virtual machine built around the recovered SoftPC machine
source, without NTVDM, DOS/WOW, VDD, BOP service dispatch, or an NTVDM host
process.  The machine shape is fixed to the currently selected SoftPC
configuration; users supply boot media rather than select a machine profile.

The initial executable owns only the standalone lifecycle and command surface:

```text
softpcvm --floppy disk.img
softpcvm --hdd disk.img
softpcvm --floppy disk.img --window
```

Console presentation is the default and runs continuously; press `Esc` to
leave it. `--window` selects the equivalent Win32 text window. The current
core links the detached CCPU, SAS, I/O, PIC and event packages through
standalone host ports; firmware, storage controllers and presentation are the
next machine-owned packages to complete.

Exactly one boot medium is accepted at present. The fixed firmware boots its
first sector and provides single-sector CHS reads for the selected medium;
this is sufficient for the checked boot and follow-on-read paths, not a claim
of complete BIOS or controller compatibility. A simultaneous floppy and hard
disk configuration will be enabled only with separate machine-owned device
backends.

## Source layout

- `src/core/softpc/` — imported SoftPC baseline.  This is the primary source
  tree being detached from NTVDM and BOP semantics.
- `src/core/` — standalone lifecycle, physical-memory ownership, CPU/device
  composition and host-port contracts around that source tree.
- `src/vm/` — executable entry point, console, Win32 presentation and media
  attachment.  It never owns CPU, guest RAM or device state.
- `tests/` — boundary and lifecycle tests.

The standalone core never accepts a DOS semantic callback or a BOP selector
service.  Hardware and firmware behavior will be expressed as machine-owned
state and typed mechanical outcomes.
