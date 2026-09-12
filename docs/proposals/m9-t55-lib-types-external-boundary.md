# M9 T55 — Types Vocabulary and Component Platform Boundaries

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
  `native`, `internal`, or `private` qualifiers. Platform vocabulary lives in
  `types/win32/` or `types/linux/`; `host/sync_interface.h` is the public sync
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

## S3 admission: types platform layout

Original owner request: “同意，按照这个准入一个S任务清理lib types，要求符合收口标准后供我检查。”
Baseline: `1b368d4`, plus the owner's formatting-only atomic.h changes.
S3 supersedes S2's unfinished layout/ownership cleanup; it does not certify
the broader S2 external-vocabulary migration or close T55.

The frozen source universe is all seven baseline types headers and every
direct consumer. Disposition requires both a source review and an executable
layout gate, plus focused tests where function bodies move.

| Baseline header | Disposition / owner |
| --- | --- |
| types_interface.h | Retain common scalar/status and C-runtime wrappers; no OS selection. |
| atomic.h | Retain compiler-only MSVC/C atomics selection and owner formatting. |
| win32.h | Remove umbrella; explicit purpose headers under types/win32. |
| posix.h | Remove umbrella; explicit purpose headers under types/linux. |
| win32/file.h | Keep Windows file declarations; common stdio declarations move to types/file.h. |
| clock.h | Move combined counter validation/conversion to host/{win32,linux}/clock.c; only raw clock declarations/wrappers in platform types headers. |
| input.h | Move modifier interpretation to ui-base/win32/actions.c; direct key-state/layout wrappers in types/win32/input.h; remove unused Linux zero-result fallback. |
| README.md (documentation companion, not a header) | Describe the one platform layout and compiler exception. |

Only actually consumed purpose headers are created. No new platform runtime,
implementation C file in types, duplicated registry, state owner or generic
platform dispatcher is allowed. CMake continues selecting the owning
component's same-shape platform implementation. Platform headers themselves
do not need OS-selection branches. Windows SDK declarations keep their exact
external signatures; this task does not manufacture a second OS API or claim
that header organization alone completes S2's entire wrapper audit.

Verification: layout/forbidden-include checks, header-only and DAG gates,
counter/atomic smoke, existing keyboard coverage, strict library compilation,
x86/x64 full CTest and package smoke, manifest and documentation governance.
Existing Linux function bodies are preserved (clock is relocated verbatim).
Linux execution coverage must be reported separately from Windows results.
No MVDM, app policy, INI or media change. S3 is delivered for owner inspection;
T55 remains open.

## S3 P1 verification and review

All seven baseline header dispositions are implemented. The resulting types
tree has 12 headers: three common, six Windows and three Linux. Every old
umbrella/dispatcher reference is removed from production and tests. The
layout gate rejects OS branching, platform imports from common headers,
non-C common includes, platform control flow and external include bypasses;
its self-test deliberately submits seven invalid forms and valid controls.

Focused proof exercises the actual moved Windows clock/actions bodies with
controlled external queries: null arguments do not query, query failures and
invalid counters preserve outputs, valid counters are copied, all eight
modifier combinations retain their original meaning. Common C/atomic and
real public monotonic-clock calls pass at both widths. All nine common/Windows
types headers compile independently with strict GCC warnings. The existing
keyboard/hotkey tests remain in the full suite.

- x64 full CTest: 41/41 passed.
- x86 simultaneous parallel run: 40/41; package smoke failed at stage 7
  (CAP sent, cooked SoftPC prompt not observed within its existing deadline).
  No source or timeout was changed in response. Isolated package smoke then
  passed three consecutive runs (5.52, 5.35, 5.55 seconds), followed by a full
  serial x86 41/41 pass. The concurrent failure is recorded, not claimed fixed
  or conclusively attributed to scheduling. Owner review remains required.
- Strict library build and standalone CTest: 3/3 passed, including manifest,
  types layout and the Linux CMake contract. The latter is not a Linux runtime
  test. WSL is not installed; no Linux execution claim is made.
- Documentation governance, source boundary and diff checks passed.

Changed-path accounting uses `git diff --numstat --no-renames 1b368d4`:
production C/H is 32 paths, +146/-132 (net +14); test/support is 5 paths,
+156/-3 (net +153); build/check CMake is 3 paths, +85/-0. Documentation,
manifest and package EXEs are excluded. The atomic formatting was present on
entry and retained; only its old BASE include guard was renamed by S3.
storage/file.h gained its missing direct types include, eliminating implicit
include-order dependence without changing its contract.

Reviewer compared clock/error behavior and modifier bits against `1b368d4`;
other Windows and Linux component changes are include substitutions, not
worker/reader/mailbox rewrites. MVDM, app, standalone host, INI and media have
no changes. Ownership remains host clock and ui-base input, with no types
platform control flow. Delivery is for owner inspection, not T55 closure.

The remaining T55 whole-library review is still required. In particular, the
pre-existing Linux mailbox wait currently uses a timed sleep rather than
waiting on its signaled condition; Linux host sync also polls. Those bodies
were not introduced or rewritten by S3. Their behavior and S2's wider raw-call
wrapper coverage are not certified by this layout-only delivery.
