# M9 T55 S2 — Types Vocabulary and Component Platform Boundaries

## Objective

Make `lib/types` the header-only shared vocabulary for C-runtime, SDK, POSIX,
and compiler-atomic declarations. Preserve every observable library behavior
while moving all compiled platform behavior into the component that owns its
meaning.

`types` is not a runtime component. It has no `.c` files, worker, handle
ownership, state machine, product policy, or component dependency. Its typed
inline façades are one-to-one external vocabulary only.

## Final boundary

Each component has a platform-neutral base source and a selected platform
source with one identical component-private operation shape:

```text
host/clock.c       -> host_clock_platform_counter(...)
host/sync.c        -> host_sync_platform_*(...)
storage/file.c     -> storage_file_platform_*(...)
ui-base/mailbox.c  -> ui_mailbox_wake_*(...)
```

The `win32` and `linux` implementations of each shape are peers. CMake selects
exactly one. A base source does not use platform preprocessor branches, raw SDK
types, or platform function declarations. A platform source may implement only
its owning component's internal shape; it cannot introduce a cross-component
runtime, dependency, or product policy.

`types` centralizes the external header and typed wrapper vocabulary used by
those sources. It does not normalize UI keys, define a storage file object,
own events/tasks, or implement Console/Window behavior. In particular:

- `storage` owns file access, exact-transfer and ownership policy;
- `host` owns synchronization, task/cancellation, clock and Console policy;
- `ui-base` owns event normalization, hotkey matching and mailbox meaning;
- `ui-window` and `ui-console` own their lifecycle and rendering behavior.

## Required changes

- Delete every `src/lib/types/**/*.c`; make CMake target `types` INTERFACE.
- Move compiled raw file work to `storage/win32/file.c` and
  `storage/linux/file.c`, behind `storage_file_platform_*`.
- Move compiled synchronization/task work to `host/win32/sync.c` and
  `host/linux/sync.c`, behind `host_sync_platform_*`.
- Keep mailbox wake work in the existing `ui-base/win32` and `ui-base/linux`
  sources, with the identical `ui_mailbox_wake_*` contract.
- Keep platform clock work in `host/win32/clock.c` and
  `host/linux/clock.c` behind `host_clock_platform_counter`.
- Move the Win32-only modifier query to `ui-base/win32/actions.c`; do not
  retain a generic UI source which includes a platform adapter.
- Move text-to-native-key layout interpretation into `ui-base/win32/input.c`.
  `types` exposes only raw SDK facts, never UI input mapping policy.
- Centralize external SDK/CRT/POSIX declaration headers under `types`; the
  component source consumes that vocabulary and exposes no native type through
  any public `*_interface.h`.

## Invariants

- Implementation filenames and identifiers describe their operation, without
  `native`, `internal`, or `private` qualifiers. `types/clock.h` and
  `types/input.h` define wrappers; `host/sync_interface.h` is the public sync
  contract and `host/sync.h` declares the selected platform operations.
  UI worker start/join/state names describe actual lifecycle responsibilities.
  This naming pass changes no control flow or synchronization behavior.

- Public interfaces expose only `lib_*` copied values and opaque component
  objects; no `FILE`, `HANDLE`, `HWND`, `DWORD`, or `pthread_*` value leaks.
- The component DAG remains:
  `types -> console + host + storage + ui-base + ui-window + ui-console`,
  `console -> host + ui-console`, and `ui-base -> ui-window + ui-console`.
- No MVDM source, guest media, `softpc.ini`, or SoftPC product behavior is
  changed.
- Existing x86/x64 observable behavior is retained.

## Verification

- Static gate proves `types` has no `.c` and CMake declares it as an INTERFACE
  target.
- Static audit proves every neutral base delegates platform work through its
  component-private same-shape operation, rather than a platform `#ifdef`.
- Public interface audit rejects native SDK/POSIX types.
- Fresh strict library, x86/x64 full CTest, package smoke, manifest and
  governance checks pass after the complete migration.

## P16 naming audit

The owner requested removal of ambiguous `native`, `internal`, and `private`
names. The bounded universe is library filenames and C identifiers, together
with their test references. Both remaining prefixed headers were renamed;
all matching identifier qualifiers now name their actual operation or value.
Comments describing OS behavior are prose, not another API layer. No control
flow changed. The source-boundary gate rejects recurrence in paths and code
identifiers, excluding comments. x64 and x86 full CTest each passed 38/38;
the strict library build and its two checks passed. Both package EXEs were
rebuilt. T55 S2 remains active pending owner acceptance.

## P17 file declaration header naming

Rename the sole `*_runtime` header, `types/win32_runtime.h`, to
`types/win32/file.h` and update its only consumer, `storage/win32/file.c`.
This is a declaration-header relocation, not a completed typed-wrapper
migration: the external declarations remain unchanged and file implementation
stays in storage. No runtime layer is introduced. T55 and S2 remain active
by explicit owner direction.

Verification: no `*_runtime` path or old header reference remains in the
library; x64 and x86 rebuilt and each passed 38/38 CTest, including package
smoke. Strict library build, 2/2 standalone checks, documentation governance
and diff checks passed. Production changes are limited to the header move,
include guard and its one include site; no executable logic changed.
