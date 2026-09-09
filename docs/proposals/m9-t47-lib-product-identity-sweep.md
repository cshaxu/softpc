# M9 T47 S7 — Shared-lib product identity sweep

## Objective

Remove project identity from the shared `src/lib` corpus. The library is an
exact NXVM-adoption candidate and must not expose either SoftPC or NXVM in its
public native registration, default UI, standalone CMake project identity, or
shared-library wording.

## Findings and disposition

| Location | Finding | Disposition |
| --- | --- | --- |
| `ux-window/win32/component.c` | Win32 class `SoftPCUxWindow` | rename to `LibUxWindow` |
| `ux-window/window.h`, `window.c`, `internal.h`, `win32/component.c` | initial native title `Insignia SoftPC` | add copied `initial_title`; application supplies its title at creation and native Window uses only that copied value |
| `CMakeLists.txt` | standalone project `nxvm_shared_library` | explicit NXVM-compatibility exemption; retain unchanged |
| `README.md`, `ux-console/console.h`, `ux-console/win32/component.c`, `ux-base/event.h` | SoftPC-only prose | replace with neutral application vocabulary |

The standalone CMake project name is the sole explicit `NXVM` exemption.
Outside that CMake identity, no `SoftPC`, `Insignia`, or `NXVM` occurrence may
remain in the checked-in shared corpus. `VM`, `Window`, `Console`, and host
monitor terminology are generic concepts and remain.

## Boundaries and exit

- May change only `src/lib`, its manifest, a narrow source-boundary test,
  current task record, closure evidence, and refreshed executables.
- Adds only the copied `initial_title` creation field. It adds no callback or
  product policy. The application supplies its initial title and continues to
  set later titles through the existing API.
- The source-boundary test must reject SoftPC, Insignia, and NXVM identity in
  shared production source and README, excluding the admitted standalone CMake
  project identity. Fresh x64/x86 full CTest must pass.
