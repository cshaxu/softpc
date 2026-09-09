# M9 T43 S7 — Raw Console input recovery and T43 closure

## Outcome

The owner accepted raw VM Console input after the Window-to-Console transition:
the first normal key is now usable and no second Enter is required.

The repair restores the historical RDP-safe behavior without restoring a
second product control path. `ux-console` now sends every raw Win32 key record
through the same `ux-base` transition normalizer used by `ux-window`. A
scan-less virtual key is consequently recovered through the active Win32
keyboard layout before SoftPC's original key table consumes the make/break
pair. The old Console-only Unicode fallback is gone.

The investigation also retained the bounded host behavior established during
S7: cooked Console binding has no reader until the monitor explicitly requests
a line after writing its prompt; a successful raw binding starts its raw
reader. Native raw activation restores Console focus only after that reader
exists; no app-facing focus API was introduced.

## Evidence

- `softpc-win32-keyboard-smoke` proves a zero-scan `VK_RETURN` becomes the
  expected `0x1c` make/break pair through the common normalizer.
- The owner reproduced and then accepted the Win3.1 MS-DOS Prompt transition.
- Fresh x64 and x86 packages were built; full CTest passed 32/32 on both
  widths. The final manifest, package, and keyboard subsets were rerun after
  manifest closure and passed on both widths.

## Closure

S7 completes T43. It changes no MVDM source, guest timing/device behavior,
user-owned `assets/binary/softpc.ini`, or guest media. Any subsequent runtime
defect requires a new admission.
