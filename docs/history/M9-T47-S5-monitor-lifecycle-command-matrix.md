# M9 T47 S5 — Monitor lifecycle command matrix

## Outcome

Monitor lifecycle commands now pass through one pure SoftPC reducer over
`init`, `stopped`, `paused`, and `running`. Every matrix cell returns exactly
one lifecycle intent or one local explanation. Incompatible commands do not
overwrite the reconciler intent and the monitor immediately rearms its prompt.

`init` is now explicit until the first runtime completion. It shares cold
start/reset mechanics with stopped while reporting “not started” rather than
“stopped” for rejected commands. `start` is restricted to init/stopped and
never acts as resume; reset always retains its cold-start-then-pause behavior.

The reset stop completion no longer rearms cooked monitor input in the middle
of its reset chain. The sole next prompt is produced after reset reaches its
final paused completion, so no second monitor line can replace reset during
the internal stop/start sequence.

## Evidence

- `softpc-monitor-command-smoke` asserts all twenty stable matrix cells and
  the five lifecycle command spellings, including the exact rejection text.
- Existing `softpc-reconciler-smoke` continues to prove the cold reset chain:
  stop, start, then pause.
- Fresh x64 and x86 package builds each passed full CTest, 33/33.
- Documentation governance verification passed.

## Closure

This changes only SoftPC monitor product control. It does not alter MVDM,
host/lib Console behavior, UX ownership, guest timing, guest media, or
user-owned `assets/binary/softpc.ini`. T47 remains active; a later subtask
requires separate owner admission.
