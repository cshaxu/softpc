# M9 T47 S2 — Win3.1 Prompt raw-Console recovery

## Outcome

The shared host Console broker now treats native-reader retirement as the
mandatory first phase of every Current Console replacement.  It confirms the
old reader has exited before invalidating its binding or activating the next
one.  For a cooked line read which does not respond to the documented Win32
cancellation requests, the host injects an internal terminating record while
the old reader remains sole owner; that reader observes its stop request and
discards the record.  The subsequent binding cutover flushes it before the new
reader can observe it.

The same path governs cooked-to-raw, raw-to-cooked, raw-to-raw, and
cooked-to-cooked replacement.  Retirement has a bounded liveness barrier.  If
the old reader still cannot be proved retired, the broker fails closed with
host-I/O failure: it starts no next reader and invalidates the indeterminate
binding instead of hanging or pretending the old Console remains safe.

## Evidence

- `softpc-host-console-broker-smoke` covers retirement failure as fail-closed
  and all four raw/cooked mode-pair replacements through one transaction.
- The library manifest was reconciled to the current checked-in corpus and
  verified with `src/lib/verify_manifest.cmake`.
- Fresh x64 and x86 package builds each passed full CTest, 32/32.
- The owner manually verified the Win3.1 full-screen MS-DOS Prompt transition
  and accepted the resulting package.

## Closure

S2 is closed by owner acceptance.  It changes no MVDM source, SoftPC
reconciler behavior, monitor command semantics, guest media, or user-owned
`assets/binary/softpc.ini`.  T47 remains active for separately admitted work.
