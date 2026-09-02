# M5 Original Timer and Quick-Event Restoration

## Boundary

The original PIT timer computes a delay for a historical quick-event queue.
With a valid one-clock guest counter, integer division can produce zero
microseconds. The standalone queue endpoint executes zero-delay events
immediately, which can recursively drain a timer backlog. The required
independent-host behavior is a minimum defer interval, not a replacement PIT,
queue or interrupt policy.

## Resolution

`base/system/timer.c` now matches the selected original source after
line-ending normalization. `scripts/transform_timer.py` produces the
build-only standalone copy. It changes exactly one original sequence: after
the original multiple-interrupt delay calculation, a zero is clamped to one
microsecond before the original `add_q_event_t` call. All timer state,
multiple-interrupt accounting, quick-event dispatch and IRQ generation remain
in original source.

## Verification

- Direct comparison with the selected original `timer.c` succeeds after
  line-ending normalization.
- `softpc-quick-time-smoke`, `softpc-machine-smoke` and
  `softpc-runtime-smoke` passed on x86 Clang and x64 MinGW.
- Serial CTest passed `18/18` on both host widths.
