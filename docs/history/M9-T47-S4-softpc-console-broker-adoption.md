# M9 T47 S4 — SoftPC Console broker adoption

## Outcome

SoftPC's monitor now owns its own neutral `lib_console`, cooked-line event
sink, and `host_console_broker`. It writes prompt/status text through its
logical Console, asks the broker to arm a cooked line, and passes its own
known monitor or VM Console as the expected Current Console for every
replacement.

`host_console_cooked` has been deleted. The host library now has no monitor,
prompt, command, VM, or lifecycle type; `host_console_broker` only binds
caller-owned logical Console objects to raw or cooked native I/O. SoftPC's
existing reconciler remains the only owner of desired/actual Current Console
product state.

The source-boundary test now rejects any reintroduction of the old wrapper and
requires the monitor's direct broker replacement and cooked-line request path.

## Evidence

- Generic broker smoke covers expected-current line requests and every
  raw/cooked replacement pair, including existing prepare, activation restore,
  and retirement fail-closed paths.
- `src/lib/verify_manifest.cmake` passed.
- Fresh x64 and x86 package builds each passed full CTest, 32/32.
- `tools/Verify-DocumentationGovernance.cmake` passed.

## Closure

This is an ownership/API recomposition only. It changes neither MVDM source,
guest behavior, monitor command semantics, lifecycle policy, guest media, nor
the user-owned `assets/binary/softpc.ini`. T47 remains active; any next
subtask requires separate owner admission.
