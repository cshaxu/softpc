# M9 T81 S7: Canonical Audio Corpus

## Closure

Executor `4157e918` adopted the owner-approved MyNES `b48e57f` Audio corpus,
including its copied PCM FIFO, one delivery worker, one selected platform leaf,
short-tail `flush`, and Audio tests. The SoftPC Compat PC-speaker remains the
sole PCM producer; the original `nt_sound.c` mirror is unchanged.

The owner accepted that existing product behavior remained intact and observed
that later `AUDIO.COM` runs produce a continuous tone. The owner also reported
a reproducible first-use defect: first run silent, second discontinuous, later
runs continuous, with the sequence recurring after a hot guest reboot. That is
not silently declared an S7 success condition: it is explicitly admitted as
T81 S8, whose narrow scope is the initial-tone lifecycle handoff.

## Actual Change Accounting

Compared with `ae76ecbf`, tracked text changed +828/-196 (net +632), including
production, tests/build and documentation. Package EXEs were refreshed. The
only retained MyNES-corpus text differences are SoftPC's accurate Audio README
and this repository's generated manifests; copied Audio C/H/platform code and
three Audio C tests hash-match `b48e57f`.

## Verification

- Focused fake/native Audio and Compat failure tests passed on x64 and x86.
- Strict C11/package builds passed on x64 and x86.
- Background regression passed except the then-pending S8 documentation
  continuation record; no product test failure was attributed to S7.
- User-owned INI and guest media were not committed.

T81 remains open. This record closes only S7's canonical-import scope; S8 owns
the first-use playback defect and its final dual-width regression.
