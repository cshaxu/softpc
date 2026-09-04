# M9 T25: Warm Reset Through the Original CPU Line

## Result

Closed. Guest Ctrl+Alt+Del reaches the original keyboard BIOS callback,
`reboot()`. The standalone host callback now asserts `CPU_HW_RESET` through
the CCPU interrupt interface. CCPU resumes at the ROM reset vector and the
existing BIOS path owns the warm-reset controller initialization.

## Root Cause

The standalone callback had only set `soft_reset`, a classification flag. It
did not assert a processor reset, so execution continued with pre-reset CCPU
state after the keyboard service. This explains a restart that reached the
DOS startup banner but could not complete it.

An outer runtime re-entry was investigated and rejected: it changes the
original reset ownership and destabilizes the CCPU simulation stack. The
narrow host-side CPU-line assertion matches the original keyboard controller
reset route without adding DOS, DPMI, NTVDM, or frontend reboot semantics.

## Regression

`test/unit/irq_smoke.c` proves both reset paths:

- the original 8042 output-port reset pulse;
- the original keyboard BIOS `reboot()` callback used by Ctrl+Alt+Del.

Each must bring CCPU to `F000:FFF0`, the ROM reset vector. The test uses only
test-local media and source inputs; it does not consume package configuration,
ROM artifacts, or guest media.

## Verification

- GCC x64 rebuild and full CTest: 20/20 passed.
- GCC x86 rebuild and full CTest: 20/20 passed.
- Package-smoke integration test passed at both widths.
- Refreshed `artifacts/binary/softpc32.exe` and `softpc64.exe`; the adjacent
  user-owned `softpc.ini` was not changed.
