# types

`types` is the root component. It provides scalar aliases, status values,
portable atomic helpers, and one-to-one C-runtime or SDK vocabulary wrappers.
Every other component may depend on it; it has no component dependency,
resource policy, platform worker, or product behavior.

`types` is header-only. It centralizes external declarations and typed,
one-to-one wrappers, but does not compile a platform implementation. Each
owning component supplies its own selected `win32` or `linux` source behind a
uniform component-private contract: `host` owns synchronization and Console
native work, `storage` owns file native work, and `ui-base`/the UI leaves own
their own wake, input, and rendering work. `types` never interprets a
consumer's state machine or input protocol.

The layout has one rule:

- Top-level headers contain only common scalar/status, ISO C library, or
  compiler atomic vocabulary. They never select an operating system.
- `win32/` contains the used Windows file, clock, input, sync, Console and
  Window declaration groups. `linux/` contains the used POSIX file, clock and
  sync declaration groups. Platform sources explicitly include the matching
  group; no common header selects one and no platform header dispatches again.
- `atomic.h` may select MSVC intrinsics versus C atomics by `_MSC_VER`.
  This compiler-only exception does not select component behavior.

Platform headers preserve SDK/POSIX signatures. Windows groups include the
SDK's common windows.h declaration source; repeated guarded SDK includes do
not create implementations or independent types. The groups name usage, not
a promise that windows.h hides unrelated SDK declarations.

`file.h` contains common C stream declarations. `win32/file.h` and
`linux/file.h` add their platform declarations. File operations and ownership
remain in storage. Counter validation/composition belongs to host; key-state
interpretation belongs to ui-base. There is no runtime layer or zero-result
fallback pretending to implement another platform's input query.
