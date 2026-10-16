# base

`base` owns blocking mutexes, events, tasks, wait/cancellation, sleep/yield,
monotonic clock composition, current-executable directory discovery and native
keyboard decoding. It depends only on header-only `types`; no Console,
KVM, storage or application dependency is permitted. The build selects same-shape
Win32/Linux implementations. Their headers remain component-local.

Use `sync_interface.h`, `clock_interface.h` and `process_interface.h`.
`base_process_executable_directory` copies the actual host executable directory
into a caller buffer; callers append their own product configuration name.
Mutexes require a live object,
same-thread unlock and no recursion; destroy requires no owner or waiters.
Events select manual or automatic reset at creation. Signal/reset report native
failure; auto-reset coalesces signals and wakes one waiter. Manual-reset stays
signaled until reset. Task cancellation retains manual reset and wins wait-any ties.
Task fields are embedded in one platform allocation, with one native entry
trampoline and the existing separate cancellation Event. Task destroy cancels
and joins before disposal. Cancellation, join and destroy return status;
failed destroy retains the task and requires its context to remain alive.
The owner serializes these operations and never joins from the task itself.
These are the existing primitives,
not forwarding aliases for another component. Caller-owned locks retain their
existing lock order and callback non-reentry requirements.

`input_adapter_interface.h` is internal Lib native-producer support, not an
application event API. One source-local decoder maps native key records and
combines UTF-16 pairs before emitting neutral `lib_key_input` values. Definitions
live in `types/input_values.h`; neither public Console nor KVM events carry
native virtual keys or UTF-16 units. The selected layout query returns neutral
physical identity or failure, allowing the consumer's existing TEXT fallback.
Decoder reset discards an unfinished prefix; repeated characters are expanded
by the consumer only after complete scalar decoding. Linux terminal keys may
have no physical identity; no missing presenter is implemented by this decoder.
