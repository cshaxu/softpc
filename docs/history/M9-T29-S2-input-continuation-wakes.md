# M9 T29 S2: Input Continuation Wakes

## Result

Closed.  After each standalone queue delivery, the runtime requests one
existing CCPU-safe wake only when another keyboard transition remains queued.
The original keyboard service remains one transition per executor callback;
the original 8042 buffer, quick events, controller timing, and device clock
remain owners of guest delivery.

## Verification

- New self-contained IRQ1 runtime smoke proves four queued transitions enter
  the original 8042-to-PIC-to-CCPU path without waiting one 50 ms device tick
  per transition.
- GCC x64 CTest: 21/21 passed.
- GCC x86 CTest: 21/21 passed.
- Package smoke passed for both rebuilt launchers; `softpc.ini` was untouched.
- Owner confirmed normal operation after testing.
