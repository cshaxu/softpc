# M5 Original CMOS Media-Port Restoration

## Boundary

The original CMOS POST code assumes its NT product configuration always
supplies a C: fixed drive. A standalone machine legitimately supports a
floppy-only topology, so retaining that assumption would make the controller
advertise an unattached disk. The only independent fact required is the
already-attached raw-image backend topology; no frontend, DOS or disk
controller state participates.

## Resolution

`base/system/cmos.c` is restored to the original source after line-ending
normalization. `scripts/transform_cmos.py` produces the build-only port-ABI
copy used by the standalone target. It changes exactly two host boundaries:

- the historical pointer-encoded graphics scalar is converted through
  `ULONG_PTR`, preserving x86 and x64 host width;
- the original fixed-product C:/D: configuration branch calls
  `softpc_host_cmos_fixed_disk_type`.

That host endpoint reads only the media paths successfully attached by the
existing fixed-disk backend and returns the original CMOS type encoding: no
disk (`0x00`), C: type 3 (`0x30`), or C:+D: types 3/4 (`0x34`). It neither
interprets guest requests nor owns controller registers.

## Verification

- Direct source comparison with the selected original `cmos.c` succeeds after
  line-ending normalization.
- `softpc-fdc-smoke` creates a floppy-only machine, runs original CMOS POST
  and requires `CMOS_DISK == 0x00` and the original 1.44M floppy type.
- `softpc-dual-media-smoke` requires original POST to report C: type 3 for
  the independently attached A:+C: image pair.
- Both focused regressions passed on x86 Clang and x64 MinGW.
