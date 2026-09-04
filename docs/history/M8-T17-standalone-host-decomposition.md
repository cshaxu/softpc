# M8 T17: Standalone-Host Decomposition

## Status

Active.

## Task Brief

Move only repository-owned standalone host code out of the transitional
`src/core/softpc_*` aggregation and organize it beneath
`src/host/{platform,media,video,input,compat,machine}`. The recovered
`src/mvdm/softpc.new/` tree remains source-shaped and is not a source of new
host implementation. The task changes paths and build routing, not controller,
firmware, ROM, BOP, CCPU/C-VID, guest-media, or presentation semantics.

## Entry Evidence

T16 completed with a 98-row current-path ledger, zero `local-standalone` rows
under the recovered machine root, and GCC x64/x86 CTest 20/20. The next audit
will assign every standalone host source one taxonomy owner before it moves.

## S1 Ownership Inventory And First Path Migration

The repository-owned standalone implementation was inventoried as the former
`src/core/softpc_*` aggregation plus the existing host compatibility adapters.
Its first path migration is content-preserving:

- `host/machine`: `softpc_machine.[ch]` and the standalone machine-BOP
  dispatcher;
- `host/media`: the GFI raw-image backend;
- `host/video`: the standalone DIB surface;
- `host/input`: the Scan-1 to original-key mapping contract;
- `host/platform`: the still-to-be-split platform callback aggregate;
- `host/compat`: the unselected XMS compatibility endpoints.

The compatibility endpoints now live in `host/compat/`: conapi/DIB surface,
console compatibility, and fast-BOP adapters. CMake's selected inputs and
private machine header routing now use the taxonomy paths; no selected
`src/core/softpc_*` path remains. GCC x64 and GCC x86 both rebuilt and passed
all 20 CTest cases.

## S2 Input Callback Separation

The host Scan-1 mapping, keyboard reset/key delivery functions, keyboard
callback table, and non-interactive `host_error` endpoint now live together in
`src/host/input/softpc_platform_keyboard.c`. The remaining platform code only
binds that callback table when original firmware reset occurs. This is a host
ownership move only: original keyboard-controller tables and device state
remain in the recovered SoftPC source. GCC x64 and GCC x86 each pass 20/20
CTest after the split.

## S2 Fixed-Disk Media Separation

The fixed-disk raw-image adapter now lives in
`src/host/media/softpc_hdd_media.c`.  It retains the existing direct,
read-only, and overlay file handling, and continues to export the same
`host_fdisk_*` callbacks consumed by the recovered original FDISK controller.
The platform aggregate now asks the private media endpoint only whether each
configured image is attached, so original CMOS and configuration callbacks
retain their existing guest-visible result without owning image files or
overlay buffers.  No source under `src/mvdm/softpc.new/` changed; its BIOS,
VGA, and CMOS ROM inputs remain together in `src/mvdm/softpc.new/roms/`.

Fresh GCC MinGW x64 and x86 builds each passed the complete 20-test CTest
suite after the separation.  The first run in each fresh build directory saw
a transient Windows test-image cleanup handle; the immediate stable rerun was
green and is the recorded result.

## S2 Video Presentation Host Separation

The standalone video presentation bridge now lives in
`src/host/video/softpc_platform_video.c`: DIB startup, presentation refresh,
text-font export, palette-frame observation, and the disabled stream-I/O
endpoint moved out of the platform aggregate.  This is explicitly not a new
renderer.  The original `nt_cga.c`, `nt_ega.c`, `nt_vga.c`, and V7 controller
sources remain the sole renderer/controller owners beneath
`src/mvdm/softpc.new/`.

GCC MinGW x64 and x86 each rebuilt and passed the complete 20-test CTest
suite, including VGA-frame, Win32-window, source-boundary, and package tests.

The obsolete disabled duplicate video vtable was removed from the remaining
platform aggregate.  It was not compiled or selected by any reset path;
`nt_graph` remains the only active original `VIDEOFUNCS` owner.

The single-machine NIDDB allocation endpoint used solely by the recovered
mouse driver now lives in `src/host/input/softpc_mouse_instance.c`.  It owns
only its host allocation; the original mouse controller retains all guest
device state.  GCC x64 and x86 compile the isolated endpoint, and the x86
mouse/source-boundary checks pass.

The standalone speaker sink is now an isolated platform source,
`src/host/platform/softpc_platform_audio.c`; it retains the original host
frequency request contract and teardown ordering.  GCC x64/x86 compile it and
the focused sound/source-boundary test passes on x86.

The former `src/host/softpc_compat/` directory has also been retired. Its
conapi/DIB compatibility declarations and console/fast-BOP implementation
sources now live in the approved `src/host/compat/` taxonomy, alongside the
existing XMS host endpoint. The current 98-row direct-difference ledger is
generated from this path map and rejects a missing current path. GCC x64 and
x86 both rebuilt the package and passed all 20 CTest cases after the move.

## S3 Machine-Memory Host Separation

The sole standalone SAS backing allocation and its original callback symbols
now live in `src/host/machine/softpc_memory.c`. It retains `host_sas_init`,
`host_sas_term`, and the existing XMS physical-pointer surface, while physical
read/write calls still go through the original `c_sas_loads`/`c_sas_stores`
mapping route. This moves host allocation ownership to the machine endpoint
without creating a second RAM, C-VID, or controller implementation. GCC
x64/x86 rebuilt `artifacts/binary/softpc64.exe` and `softpc32.exe` and each
passed all 20 CTest cases.

## S2 V7 Hardware-Pointer Presentation Separation

The standalone V7 hardware-pointer DIB composite now lives in
`src/host/video/softpc_v7_pointer.c`.  It retains the same `paint_v7ptr` and
`clear_v7ptr` callbacks: the original recovered V7 controller still supplies
the hardware pattern address and coordinates, while this host-only endpoint
saves, composites, restores, and invalidates the already-rendered DIB region.
No controller register, ROM, palette, plane-rendering, or pointer algorithm
changed; the move merely removes the last video presentation implementation
from the platform aggregate.

The standalone source-boundary and documentation governance checks pass. GCC
MinGW x64 and x86 each rebuilt and passed all 20 CTest cases. The checked-in
`artifacts/binary/softpc64.exe` and `softpc32.exe` were then relinked from
those respective build trees.
