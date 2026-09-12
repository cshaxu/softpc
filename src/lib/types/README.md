# types

`types` is the root component. It provides scalar aliases, status values,
portable atomic helpers, one-to-one C-runtime wrappers, and raw platform ABI
façades. Every other component may depend on it; it has no component
dependency, resource policy, or product behavior.

The platform façades own native headers, native handle representations,
constants, direct calls, and native link dependencies. They do not own a
consumer's state machine or behavior: `host` owns cancellation and wait
policy, `console` owns Console events, `ui-base` owns input normalization, and
the UI leaves own their lifecycle and rendering policy. Adapter headers such
as `file.h`, `native_sync.h`, and `native_clock.h` are internal shared ABI
surfaces, not application contracts: they expose only raw handles/calls where
peer components need the same primitive. A platform capability not implemented
on Linux returns explicit `LIB_STATUS_UNSUPPORTED`; it is not simulated here.
