# M7 T5 Original Speaker Endpoint Proof

## Boundary Result

The existing standalone speaker endpoint is sufficient for the fixed machine
and requires no new sound system. Original `nt_sound.c` continues to own all
PPI/TIMER2 state, frequency derivation, audibility limits, pulse behavior and
stop decisions. The standalone platform receives only `LazyBeep`'s resulting
frequency/duration request.

On Windows it lazily creates one presentation worker only while a valid tone
is active. That worker calls `Beep` in bounded 40 ms slices, observes stop and
frequency changes between slices, and is synchronously joined by
`host_timer_shutdown` during machine destruction. It neither mutates guest
state nor creates timer/interrupt events.

## Regression Evidence

- `softpc-sound-smoke` drives original PPI port bits and the original Timer-2
  waveform callback, verifies valid frequency selection, a second frequency
  transition, explicit stop, and cold-reset stop state.
- x64 clean serial CTest: 19/19 passed.
- x86 clean serial CTest: 19/19 passed.
