# M9 T47 — Console broker API recomposition

## Purpose

Remove the legacy `host_console_cooked` wrapper.  It incorrectly combines an
application monitor object, a logical `lib_console`, a cooked-line callback,
and the generic process Console broker.  The shared library must expose only
neutral logical Console binding and input-mode operations; SoftPC owns the
monitor object, its line callback, prompt text, and Current Console intent.

## S3 — Generic host broker API

Promote the existing opaque `host_console_broker` transaction machinery from
the private header into `lib/host/console.h`.  Its complete public contract is:

```c
lib_status host_console_broker_create(host_console_broker **out_broker,
    lib_console *initial_console, host_console_mode initial_mode);
lib_status host_console_broker_replace(host_console_broker *broker,
    lib_console *expected_current, lib_console *next_console,
    host_console_mode next_mode);
lib_status host_console_broker_request_cooked_line(host_console_broker *broker,
    lib_console *expected_current);
void host_console_broker_destroy(host_console_broker *broker);
```

`expected_current` is a compare-and-swap assertion supplied by the application;
it does not make host a product-state owner.  The broker neither exposes a
query for Current Console nor knows monitor, VM, prompt, lifecycle, or command
meaning.  It retains all existing transactional prepare/retire/activate,
generation, output-serialization, flush, and fail-closed behavior.

The shared broker treats cooked mode as only a native input mode; the logical
Console event sink already supplies the copied cooked line to its application
owner.  To keep every pushed implementation part buildable, this S retains the
old `host_console_cooked` wrapper as an explicitly temporary compatibility
surface. S4 migrates its sole SoftPC caller and then deletes it; it must not
gain new callers or new behavior.

**Exit:** the broker smoke uses only the public API and proves create, all four
mode replacements, line requests, preflight failure, activation restore, and
retirement fail-closed behavior. The only remaining legacy wrapper reference
is the existing SoftPC monitor and it is removed by S4.

## S4 — SoftPC monitor adoption

`app_monitor_console` becomes SoftPC's thin product object containing its
logical monitor `lib_console`, its `host_console_broker`, and its line-to-app
queue sink.  It creates the logical Console, installs its copied event sink,
and creates the broker in cooked mode.  It writes monitor text through
`lib_console_write_text`, requests lines through
`host_console_broker_request_cooked_line`, and switches monitor/VM ownership
through `host_console_broker_replace`.

The SoftPC reconciler remains the only source of the requested logical
Console.  The broker's expected-current parameter catches an integration
ordering error but does not replace SoftPC's desired/actual presentation
state.  No prompt string, monitor command, lifecycle decision, or Win32
operation enters `lib`.

**Exit:** app code has no `host_console_cooked` use; focused tests exercise the
monitor-created cooked Console and both monitor-to-VM and VM-to-monitor
replacement through the public broker, preserving S2's native handoff
contract.  x64/x86 builds and full CTest pass, package executables are
refreshed, and every implementation part is committed and pushed.

## Boundaries

- May change: `src/lib/base`, `src/lib/host`, `src/app/monitor*`, narrow
  integration tests, lib documentation/manifest, and package executables.
- Must not change: `src/mvdm/softpc.new/**`, guest behavior, monitor command
  semantics, lifecycle policy, guest media, or `assets/binary/softpc.ini`.
- The resulting shared API is a SoftPC-originated candidate for exact NXVM
  adoption; it contains no SoftPC type or callback.
