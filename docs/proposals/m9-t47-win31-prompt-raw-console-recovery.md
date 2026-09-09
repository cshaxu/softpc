# M9 T47 S2 — Win3.1 Prompt raw-Console recovery

## Request and objective

When a Console-display guest is in Win3.1 graphics, opening the MS-DOS Prompt
full screen returns the guest to its text route.  The monitor must hand the
native Console to the VM raw Console, and the first subsequent physical key
must be usable immediately.  The present build instead requires a sacrificial
Enter: that Enter returns the prompt cursor to column zero, then later input
works.

The product boundary is explicit: the host broker must stop and join the old
native reader before it activates the next Current Console binding.  This is a
generic native-I/O transaction, independent of whether either logical Console
is raw or cooked.

## Baseline and diagnosis

The SoftPC reconciler already has one completion-driven state writer.  The
host broker requests cancellation for the old reader, but currently ignores
the result and waits without proving that a line-buffered `ReadConsoleA` was
actually interrupted.  Its active path is:

```text
stop old reader -> request cancellation -> wait for old reader -> activate next
```

If a terminal host leaves the cooked read blocked, the wait does not return
until the user supplies Enter.  That Enter wakes and is discarded by the old
reader; only then can the raw reader be created.  The observed symptom is
therefore a native broker handoff failure, not a SoftPC Window lifecycle
decision.

## Work

- Make native reader retirement report success only after the reader has
  actually stopped.  Its cancellation fallback must wake a line-buffered
  cooked read without relying on user input.
- Use a bounded completion barrier.  A failed retirement activates no new
  reader and makes the broker fail closed with explicit host-I/O failure;
  cancellation has made the old reader's future behavior unknowable, so it
  must not be advertised as Current.
- Apply the same retirement/activation transaction to every mode pair:
  cooked-to-raw, raw-to-cooked, raw-to-raw, and cooked-to-cooked.
- Add controllable broker/native tests which prove those orderings and the
  fallback path without sleeps or a real terminal.
- Run the focused test, complete x64/x86 CTest, refresh only the two agent-owned
  package executables, and keep `assets/binary/softpc.ini` untouched.

## Non-goals

- No MVDM change, no SoftPC reconciler change, no new product focus policy,
  and no change to monitor commands, hotkey meanings, or guest key mapping.
- No claim that host Console input buffered before a Current-Console handoff is
  preserved; the approved host contract intentionally discards it.

## Exit criteria

The native/broker tests prove all mode-pair handoffs retire the old reader
before activating the next reader and do not require a user Enter; both host
widths pass full CTest; package executables are rebuilt; and the working tree
is clean after the committed, pushed S2 part.
