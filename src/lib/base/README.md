# base

`base` owns blocking mutexes, events, tasks, wait/cancellation, sleep/yield and
monotonic clock composition. It depends only on header-only `types`; no Console,
KVM, storage or application dependency is permitted. The build selects same-shape
Win32/Linux implementations. Their headers remain component-local.

Use `sync_interface.h` and `clock_interface.h`. Mutexes require a live object,
same-thread unlock and no recursion; destroy requires no owner or waiters.
Events retain manual-reset semantics and task cancellation wins wait-any ties.
Task destroy cancels and joins before disposal. These are the existing primitives,
not forwarding aliases for another component. Caller-owned locks retain their
existing lock order and callback non-reentry requirements.
