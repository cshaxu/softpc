# SoftPC VM

Standalone PC virtual machine built around the recovered SoftPC machine
source, without NTVDM, DOS/WOW, VDD, BOP service dispatch, or an NTVDM host
process.  The machine shape is fixed to the currently selected SoftPC
configuration; users supply boot media rather than select a machine profile.

Build and start the VM with:

```text
cmake -S . -B build
cmake --build build
softpcvm --floppy disk.img
softpcvm --hdd disk.img
softpcvm --floppy disk.img --window
```

Console presentation is the default and runs continuously; press `Esc` to
leave it. `--window` selects the equivalent Win32 text window. The current
core links the detached CCPU, SAS, I/O, PIC and event packages through
standalone host ports; firmware, storage controllers and presentation are the
next machine-owned packages to complete.

Exactly one boot medium is accepted. The fixed machine has 16 MiB RAM, master
and slave 8259 PICs, PIT channel 0, an 8042-style keyboard queue, text video
at `B800:0000`, and one read-only ATA PIO backend. Its ROM boots a floppy
sector directly or reads hard-disk LBA 0 through ATA PIO. It provides these
guest-firmware services:

- `INT 10h/AH=0Eh` — 80×25 text teletype output.
- `INT 12h` — fixed 640 KiB conventional-memory report.
- `INT 13h/AH=02h` — contiguous CHS reads of up to 128 sectors.
- `INT 16h/AH=00h` and `AH=01h` — ASCII/scan-code read and non-consuming
  availability check for basic US keyboard input.

Floppy geometry is inferred from standard raw-image capacities (360 KiB,
720 KiB, 1.2 MiB, 1.44 MiB, and 2.88 MiB). For partitioned hard disks, the
firmware uses sectors-per-track and heads from the first partition's BPB;
unrecognised hard disks retain the fixed 16×63 compatibility geometry.

This is intentionally a compact fixed PC, not a complete BIOS or controller
emulation: writes, CMOS/RTC, VGA graphics, floppy-controller commands, sound,
and simultaneous floppy-plus-hard-disk attachment are not implemented yet.

For a real-media, non-interactive boot probe (not part of the default test
suite), run `build/softpc-real-boot-smoke --floppy disk.img` or replace
`--floppy` with `--hdd`. It succeeds when the guest reaches printable text
output in text video memory.

## Source layout

- `src/core/softpc/` — recovered SoftPC CPU baseline.
- `src/core/` — standalone lifecycle, physical-memory ownership, CPU/device
  composition and host-port contracts around that source tree.
- `src/vm/` — executable entry point, console, Win32 presentation and media
  attachment.  It never owns CPU, guest RAM or device state.
- `tests/` — boundary and lifecycle tests.

The standalone core never accepts a product-shell callback or selector
service. Hardware and firmware behavior is machine-owned state and typed
mechanical outcomes.
