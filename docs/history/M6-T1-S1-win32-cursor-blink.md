# M6 T1 S1: Win32 Cursor-Blink Closure

## Result

The standalone Win32 text frontend now owns the same class of cursor blink as
the original Console endpoint: a frontend-local 250 ms on/off phase. It
invalidates only the copied cursor cell while guest text is otherwise static.

## Boundary And Evidence

- The change is limited to `src/vm/win32_window.c` and its smoke fixture. It
  neither changes original SoftPC CCPU, C-VID, controllers, BIOS, ROM/VGA ROM,
  BOP, guest video RAM nor the original host-timer contract.
- The window smoke starts an idle HLT guest, waits for the copied original text
  cursor, and proves a client pixel toggles without a new guest frame. Its
  keyboard -> runtime queue -> original 8042/PIC/CCPU IRQ1 proof still passes.
- Clean x64 and x86 builds each passed serial CTest 18/18, including the
  focused Win32 cursor-blink smoke. Existing overlay-media runtime boot probes
  also completed on both widths.
- The text overlay remains restricted to copied text frames; graphics frames
  do not receive a frontend cursor.

## Closure

M6 T1 S1 meets its bounded frontend-only exit criteria. Later UX work must be
admitted as a new packet.
