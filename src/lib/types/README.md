# types

`types` is the root component. It provides scalar aliases, status values,
portable atomic helpers, small C-runtime vocabulary for memory, text, bounded
formatting, allocation, opaque byte-file primitives, and raw platform ABI
façades. Every other component may depend on it; it has no component
dependency or product I/O policy.

The platform façades own native headers, native handle representations,
constants, direct calls, and native link dependencies. They do not own a
consumer's state machine or behavior: `host` owns cancellation and wait
policy, `console` owns Console events, `ui-base` owns input normalization, and
the UI leaves own their lifecycle and rendering policy. A platform capability
not implemented on Linux returns explicit `LIB_STATUS_UNSUPPORTED`; it is not
simulated here.
