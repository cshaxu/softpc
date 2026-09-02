# M5 T1 S5: Real-Media Windows Setup And Outer-UX Closure

## Result

The standalone fixed SoftPC machine now meets the admitted M5 closure scope.
It retains original SoftPC CCPU, C-VID, controllers, BIOS, ROM/VGA ROM and
BOP paths. The NXVM-style outer shell remains a single executor, monitor,
copied presentation frames and thin Win32/RDP input endpoint; it does not
introduce a DOS, WOW, NTVDM, CSR, VDD or multi-session service.

## Final Evidence

- The rerunnable pristine-divergence audit reports 58 classified rows: 39
  reproducible port-ABI representations and 19 original-host compatibility
  endpoints. It reports no `restore-pristine` controller, BIOS, firmware,
  BOP or renderer route.
- Independent clean Ninja builds regenerated the port-ABI inputs and passed
  serial CTest 18/18 on x64 MinGW GCC and x86 Clang/MSYS2.
- The real configured A:+C: media path was exercised on each clean width in
  non-mutating overlay mode. The original normalized input path progressed
  Windows Setup through its welcome choices to a visible original graphics
  frame. This is a Setup-progression acceptance check, not an unattended
  guest installation claim.
- Runtime keyboard and actual Win32 window input each retain their bounded
  original 8042 -> PIC -> CCPU IRQ1 proof; the frontend has no direct guest
  state access. Original text attributes, loaded font maps, cursor position
  and original cursor height are copied through the presentation boundary.
- The delivered `softpc32.exe` and `softpc64.exe` each read only adjacent
  `softpc.ini`, reject arguments, begin at the monitor without auto-running
  the guest, and directly accepted the monitor `exit` command with exit code
  zero.

## Closure

The packet's real-media, dual-width, original-machine and outer-UX exit
criteria are met. Later work must be admitted as a new packet rather than
silently extending M5 T1 S5.
