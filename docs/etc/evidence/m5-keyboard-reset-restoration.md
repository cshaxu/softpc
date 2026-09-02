# M5 Original CPU_40 Keyboard Reset Restoration

## Boundary

The original `base/keymouse/keyba.c` 8042 output-port pulse sets the CPU_40
reset bit directly and calls `host_cpu_interrupt`. The standalone CCPU keeps
that map private, so a generated CCPU ABI accessor exposes its address only to
the source-specific `keyba/cpu4.h` port overlay. The overlay names the
selected original bit-14 representation; it creates no controller protocol,
host policy or second CPU owner.

The historical CCPU host implementation of `host_cpu_interrupt` is empty.
The standalone endpoint remains empty as well: the sole executor is already
executing the original 8042 command and consumes the original reset request at
its next CCPU boundary.

## Resolution

`keyba.c` now matches the selected original SoftPC source after line-ending
normalization. The prior direct `cpu_interrupt(CPU_HW_RESET, 0)` replacement
is gone. The accessor is generated from the pristine CCPU source through the
existing reproducible port-ABI transform.

## Verification

- `softpc-irq-smoke` sends the original 8042 output-port pulse (`0x64/0xFE`),
  runs one bounded machine step, and requires `F000:FFF0`.
- The focused reset smoke passed on x86 Clang and x64 MinGW.
- Serial CTest passed `18/18` on both host widths.
