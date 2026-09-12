# M9 T55 S2 — Types-Owned External Boundary

## Objective

Make `lib/types` the sole library component that directly declares, includes,
links, or calls the C runtime and native platform libraries. Every other
library component consumes only copied `lib_*` values, opaque `types` handles,
and `types` wrapper functions. This is a structural boundary change: it must
retain the existing observable library behavior.

## Frozen convergence ledger

The universe is every tracked `*.c` and `*.h` below `src/lib/`, classified by
its direct external dependency:

| Class | Current direct consumers outside `types` | Required disposition |
| --- | --- | --- |
| C runtime | `console`, `host`, `storage`, `ui-base`, `ui-window`, `ui-console` | Replace direct allocation, memory/text, formatting, and file/runtime calls with `types` API. |
| Windows SDK | `host/win32`, `storage/win32`, `ui-base/win32`, `ui-window/win32`, `ui-console/win32` | Move all `windows.h` types, constants, callbacks, API calls, and `user32`/`gdi32` link ownership into `types/win32` adapters. |
| Linux/POSIX | `host/linux`, `storage/linux`, `ui-base/linux` | Move all POSIX headers, types, calls, and thread-library link ownership into `types/linux` adapters. |
| Compiler atomics | `types/atomic.h` | Retain in `types`; no other component may expose or call the compiler primitive directly. |

Completion means the scan finds no external-system include, external type
name, or external function call outside `src/lib/types/`, except C language
keywords and compiler-required builtins explicitly enumerated by the static
gate. `types` remains component-rooted: it gains no dependency on `console`,
`host`, `storage`, or either UI component.

## Frozen remaining platform ledger

The C-runtime/file members are complete: only `types` includes the C-runtime
headers or calls its allocation, memory, text, and byte-file functions. The
remaining direct platform consumers are frozen below. They are the only
allowed work universe for the next implementation pass; each must end with no
external header, native type, constant, callback signature, or native call in
the named component source.

| Current direct consumer | Native dependency | `types` disposition |
| --- | --- | --- |
| `host/win32/clock.c` | Win32 monotonic clock | `types/win32` copied-time primitive. |
| `host/win32/sync.c` | Win32 event/thread/wait handles | `types/win32` opaque event/task primitives. |
| `host/linux/sync.c` | POSIX mutex/condition/thread/time | `types/linux` implementation of the same opaque event/task primitives. |
| `host/win32/console.c` | Win32 Console reader, renderer, focus, output serialization | `types/win32` opaque native-console primitive; `host` retains only broker policy. |
| `ui-base/win32/{actions,input,mailbox}.c` and private headers | keyboard state, native records, wake event | `types/win32` copied key transition and opaque wake primitive. |
| `ui-window/win32/{component,geometry,mouse}.c` and private headers | Window handle/message loop, drawing, geometry, pointer capture | `types/win32` opaque native-window/surface/pointer primitives; `ui-window` retains only frame/control/input policy. |
| `ui-console/win32/component.c` | worker handle and Console coupling | `types/win32` opaque worker primitive and neutral Console binding. |

No component may receive a native pointer, integer handle, SDK structure, or
SDK callback signature as an escape hatch. Where a platform callback is
unavoidable, it is declared and called wholly inside `types`; its component
consumer receives a copied neutral event or invokes a neutral callback supplied
by `types`. The replacement must preserve the existing component DAG and all
observable behavior.

## Design

- `types` owns only platform-private type declarations and direct function
  façades under `types/win32/` and `types/linux/`, plus C-runtime wrappers.
  It never owns a component state machine or derived behavior: Console event
  schemas remain in `console`; manual-reset/cancellation/wait policy remains
  in `host`; input normalization remains in `ui-base`; Window/Console
  lifecycle and rendering policy remain in their respective leaves.
- `types/win32/**` and `types/linux/**` are the **only** shared-library
  sources permitted to include an external platform header, name a native
  type/constant/callback signature, call a native function, or own a direct
  native link dependency. A path such as `host/win32/**` or
  `ui-window/win32/**` receives no exception merely because its name names a
  platform.
- Its public contract exposes only `lib_*` scalar/copied values and opaque
  handles. It does not expose `FILE`, `HANDLE`, `HWND`, `DWORD`, `pthread_*`,
  or another native type.
- A `types` primitive is capability-level (`lib_monotonic_*`,
  `lib_native_event_*`, `lib_native_task_*`, `lib_native_console_*`,
  `lib_native_window_*`, or `lib_file_*`), never product- or component-level.
  It carries no `host_*`, `ui_*`, monitor, lifecycle, or machine policy. A
  required native callback remains wholly within `types`; its consumer gets a
  copied neutral event or an opaque `lib_native_*` handle.
- `ui-base` owns the one native-input normalizer and its conversion to
  `ui_input_event` before the existing source-identity and registered-hotkey
  path. It obtains native-only facts (layout translation, scan lookup and
  modifier sampling) through small `types` primitives. Neither `types` nor
  either UI leaf interprets a hotkey or owns a second event mapping route.
- Component-specific behavior stays in its present owner: storage still owns
  storage policy, host still owns broker/synchronization policy, and UI leaves
  still own UI lifecycle and rendering decisions. They request primitive
  operations from `types`; `types` does not learn their product semantics.
- CMake makes `types` the sole owner of native link libraries. Other library
  targets link only library component targets in the approved DAG.
- Add a permanent static boundary gate over all library source/header paths,
  plus existing behavior tests. The gate is the proof receiver for every
  mechanically discoverable direct external dependency.

## Boundaries

No MVDM, guest media, `softpc.ini`, application/host product behavior, or
NXVM checkout changes are admitted. The canonical library changes here become
the source corpus for NXVM to adopt; exact equality with the old NXVM revision
is intentionally no longer an S2 acceptance condition.

## Verification

- Ledger-backed static gate finds zero external includes/types/functions
  outside `types`.
- Component CMake DAG has no direct native-link ownership outside `types`.
- Existing library, host, storage, UI, and package tests retain behavior.
- Fresh x64 and x86 build, full CTest, strict-library build, manifest, and
  documentation-governance checks pass.
