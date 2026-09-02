# M7 T5 S1: Original Speaker Host Endpoint Closure

## Result

The original SoftPC speaker path is complete for the fixed standalone VM. The
existing bounded Windows playback sink is retained because it already provides
the required lifecycle without introducing a replacement audio device.

## Evidence

- The [speaker proof](../etc/evidence/m7-t5-speaker-endpoint.md) records the
  original PPI/Timer-2 ownership, bounded host playback behavior, reset and
  shutdown lifecycle, and dual-width regression result.
- No sound-card emulation, mixer, guest-clock synthesis, BIOS/BOP behavior,
  or original sound-controller source changed.

## Closure

M7 T5 is closed with clean x64/x86 serial CTest at 19/19. The fixed M7
original-host capability-completion queue is complete.
