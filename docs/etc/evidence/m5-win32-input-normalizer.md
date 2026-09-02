# M5 Shared Win32/RDP Input Normalizer

## Result

Console and Win32 window now use one outer-shell packet normalizer in
`src/vm/win32_keyboard.c`.  It accepts native scan/VK transitions and
scan-less Win32/RDP UTF-16 packets, recovers physical scans from the active
host layout where needed, preserves extended-key state, and suppresses the
duplicate `WM_CHAR` generated after a recovered `WM_KEYDOWN`.

Its only output is a make/break request through the runtime input queue.  It
constructs a `KEY_EVENT_RECORD` and calls the original SoftPC
`KeyMsgToKeyCode`; that original `nt_keycd.c` table still assigns the SoftPC
key number.  The established runtime then owns executor handoff, and original
`keyba.c` owns 8042/PIC guest delivery.  There is no guest text injection,
keyboard-controller table, direct RAM access, BOP route or NTVDM service.

## Focused Proof

`softpc-win32-keyboard-smoke` verifies:

- physical A make/break maps via the original table to SoftPC key 31;
- a recovered scan-less RDP A followed by its `WM_CHAR` is consumed once;
- a scan-less UTF-16 A becomes the same original-table make/break sequence.

## Verification

- x64 MinGW: focused normalizer smoke and serial full CTest `17/17` passed.
- x86 Clang: focused normalizer smoke and serial full CTest `17/17` passed.
- `softpc64.exe` and `softpc32.exe` were relinked in `build/output`.

The full suites are intentionally serial for this record because three older
temporary-media tests have cleanup-only Windows handle contention under a
parallel CTest run; each of those machine/FDC/IRQ tests passes serially.  That
test-fixture cleanup issue is separate from the input path and is not masked
as a controller result.
