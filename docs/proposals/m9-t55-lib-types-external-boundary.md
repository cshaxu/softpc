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

## Design

- `types` owns platform-private adapter implementations under `types/win32/`
  and `types/linux/`, plus C-runtime wrappers already begun in `runtime.c`.
- Its public contract exposes only `lib_*` scalar/copied values and opaque
  handles. It does not expose `FILE`, `HANDLE`, `HWND`, `DWORD`, `pthread_*`,
  or another native type.
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

