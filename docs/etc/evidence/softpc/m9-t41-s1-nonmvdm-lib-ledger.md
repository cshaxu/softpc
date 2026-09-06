# M9 T41 S1: Non-MVDM Library Migration Ledger

## Basis

This ledger audits the current checked-in SoftPC production C/H/RC inventory
outside `src/mvdm/softpc.new/` against the exact imported NXVM library at
`71e59ec09d669050ba98faca410ed8b9e9259d7c`. The imported `src/lib/` corpus
has 50 C/H files (56 files including its CMake, manifest, and documentation)
and verifies with its own manifest. It contains only the shared `base`,
`host`, `storage`, and `ux` roots.

MVDM, including all MVDM-local Win32 and WinNT-derived implementation, is out
of scope and has no entries below.

## Disposition Summary

| Disposition | C/H/RC files | Meaning |
| --- | ---: | --- |
| Delete from old lib location | 21 | Complete former `src/lib/platform/win32/` C/H corpus; shared portions use the imported corpus, while the runtime queue is rehomed as app coordination. |
| Modify for lib binding | 15 | Retain SoftPC policy/ABI but remove its generic platform operation. |
| Retain unchanged | 30 | No generic capability duplicated by the present shared library. |

The old lib's `README.md` and `MANIFEST.sha256` are also deleted as part of
the atomic root replacement. CMake and focused tests are separate build/test
artifacts and are not included in the 66-file production count.

## Delete And Replace

Delete every C/H file formerly below `src/lib/platform/win32/`: actions,
console, event, event queue, frame, geometry, input, mailbox, mouse,
presenter, router, and window. The imported corpus replaces the shared UX,
host, and storage roots atomically; no former SoftPC lib file may be retained
or moved to another local platform directory.

## Required Binding Migration

| File | Shared replacement | Retained SoftPC responsibility |
| --- | --- | --- |
| `src/app/main.c` | `lib_storage_file_read_owned` | Configuration syntax and launcher policy. |
| `src/app/runtime.c`, `runtime.h` | `host_sync_*`, `ux_mailbox_*`, `ux_event` | Sole executor, commands, machine calls, lifecycle policy. |
| `src/app/keyboard.c`, `keyboard.h` | `ux_event`, `ux_actions_*` | Guest Scan-1/key-number translation and SoftPC action meanings. |
| `src/app/presentation.c`, `presentation.h` | `ux_binding`, router, native runners | SoftPC pause/reset/close/action decisions. |
| `src/app/prompt_trace.c` | `lib_storage_file_writer_*` | Trace path and format. |
| `src/host/audio.c` | `host_sync_*` | Speaker frequency policy and audio endpoint. |
| `src/host/gfi_image.c` | `lib_storage_medium_*` | GFI/FDC addressing, geometry, and write-protection semantics. |
| `src/host/hdd_media.c` | `lib_storage_medium_*` | HDD geometry and original-host attachment. |
| `src/host/machine.c` | `lib_storage_file_read_owned` | Fixed machine assembly and resource policy. |
| `src/host/parallel.c`, `serial.c` | `lib_storage_file_writer_*` | Original LPT/COM endpoint protocol. |
| `src/host/platform.c` | `host_clock_*`, `host_sync_*` where covered | Original timer callback, CCPU safe point, resources, and machine timing. |

## Retain Unchanged

The following 30 C/H/RC files remain because they implement a fixed-machine
or original-host ABI contract rather than a duplicated generic platform
capability:

- `src/app/firmware.rc`, `src/app/prompt_trace.h`.
- `src/host/device_bop.c`, `dib_surface.c`, `dib_surface.h`, `hdd_media.h`,
  `input.h`, `keyboard.c`, `machine.h`, `memory.c`, `mouse_instance.c`,
  `v7_pointer.c`, `video.c`.
- Every current `src/host/compat/**` C/H file: BIOS, CCPU/C-VIDC facades and
  declarations, Console ABI compatibility, EDL BOP compatibility, graphics
  Console compatibility, CMOS/keymouse/system declarations, and the CCPU
  lifecycle adapter.

## Product-Local Retentions

The former `event_queue.c/.h` enforced that UI producers did not enter MVDM
and only the SoftPC executor consumed input. NXVM UX intentionally exposes an
event sink rather than imposing a product queue. Therefore this queue is
SoftPC runtime coordination, not a missing shared-lib facility: it must be
rehomed under `src/app/` while preserving the sole-executor boundary.

`serial.c` and `parallel.c` write arbitrary guest bytes to configured COM/LPT
device endpoints. The shared storage writer is deliberately a sequential text
writer and is correctly used by prompt tracing, but it does not describe those
endpoint protocols. Retain their byte-stream implementation in the SoftPC host
layer; do not widen lib unless NXVM obtains the same concrete endpoint need.

## Build/Test Follow-up

`CMakeLists.txt` must replace the former `win32-presentation` target with the
imported library targets. The presentation/runtime source-boundary and unit
tests must migrate from `win32_presentation_*` to the shared UX contract;
they must not preserve a compatibility copy of the old API.
