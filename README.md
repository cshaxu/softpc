# SoftPC VM

Project governance, target architecture, and the ordered recovery plan are in
[docs/README.md](docs/README.md). The current executable remains a
transitional baseline; do not treat its temporary standalone branches as the
approved final architecture.

Standalone PC virtual machine built around the recovered SoftPC machine
source, without NTVDM, DOS/WOW, VDD, BOP service dispatch, or an NTVDM host
process.  The machine shape is fixed to the currently selected SoftPC
configuration; users supply boot media rather than select a machine profile.

Build the VM, then set the fixed machine defaults in
`build/output/softpc.ini`:

```text
cmake -S . -B build -G Ninja
cmake --build build --parallel 8
build/output/softpc64.exe
```

The CMake build supports both 32-bit and 64-bit Windows hosts.  A 32-bit
build requires a real i686 MinGW toolchain (including its Windows import and
CRT libraries), for example:

```text
cmake -S . -B build-x86 -G Ninja -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc
cmake --build build-x86 --parallel 8
```

The x86 configure writes `build/output/softpc32.exe`; the native x64
configure writes `build/output/softpc64.exe`. Both use the same adjacent
`softpc.ini`.

`softpc.ini` has five `key=value` keys: `memory_mb`, `floppy`, `hard_disk`,
`display` (`console` or `window`), and `media_mode`. Both media keys may be
set together, creating fixed `A:` and `C:` slots; the machine boots `A:`
first, then `C:`. The launchers accept no command-line parameters and always
load the `softpc.ini` beside themselves; relative image paths are relative to
that file. Console presentation runs continuously; press `Esc` to leave it.
The current
core links the detached CCPU, SAS, I/O, PIC, event, original FDC/FLA/GFI,
fixed-disk BIOS and V7 VGA packages through standalone host ports.  The
original ROM reaches only machine-resident C services through its historical
BOP instruction table; it has no NTVDM, DOS/WOW, VDD or product-service
dispatcher.  Fixed firmware, raw-media storage and console/Win32
presentation are supplied by the standalone VM, not a product host.

Set `media_mode` to choose how both configured images are attached:
`readonly` passes writes back to the original controller as write-protected,
`direct` writes the source image files directly, and `overlay` loads both
images into RAM at startup and directs all guest writes to those volatile
copies. The distributed configuration uses `overlay` for safe experimentation.

The fixed machine has 16 MiB RAM by default (configurable through
`memory_mb`), master and slave 8259 PICs, PIT channel 0, the original
keyboard/mouse controller path, original FLA/GFI/FDC floppy path, original
fixed-disk BIOS path, and original V7 VGA controller. The console presents
text memory at `B800:0000`; the Win32 window presents that text path and the
original CGA-compatible BIOS modes 04h/05h/06h, VGA mode 13h (320×200,
256-colour) planes/DAC, and the EGA/VGA 4-plane BIOS modes 0Dh through 12h
through RGB32 host surfaces. They are presentation front ends, not
replacement video controllers.
The Win32 window forwards relative pointer movement and its two buttons to
the original Microsoft Bus Mouse adapter; it does not implement a second
mouse device.

The ROM and restored controllers provide their original BIOS/interrupt and
I/O behavior.  The standalone host provides raw floppy and hard-disk image
files plus a fixed 16-head/63-sector hard-disk compatibility geometry; it
does not reinterpret partition BPBs as a second disk controller.

Serial and printer controllers are original SoftPC code connected to idle
standalone host endpoints. The original PPI and PIT channel 2 speaker path
now drives a bounded asynchronous Win32 beep sink; it is stopped with the
machine on reset or teardown. Graphical presentation remains host-front-end
work, not a reason to substitute the original VGA controller.

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
