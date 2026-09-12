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
