# M9 T42 S7 — Real-thread integration barriers

## Outcome

S7 added real, completion-gated thread proofs to the generic Console/UX path;
it changes no SoftPC product policy.  `lib_console` now has an explicit test
that an event-sink detach cannot complete while its copied callback is active.
The actual `ux-console` worker is tested with a blocked raw-key sink: destroy
waits, then retirement is emitted exactly once after the key, then destroy
returns.  The broker fake models reader join during replacement and proves a
new Current Console is not committed until the old callback returns.

## Boundary audit

The only selected `CONIN$`/`CONOUT$`, `ReadConsole*`, and `WriteConsole*`
calls outside preserved MVDM code are in `src/lib/host/win32/console.c`.
There is no `ux_presenter`, `ux_run`, target-router, or app/lib `fgets` route.
Thus no second native reader or output path remains in the non-MVDM product.

## Proof

- x64 full build and CTest: 29/29 passed, including package smoke.
- x86 Console/broker barrier targets built and passed: 3/3.
- Tests use events and joins only; no timing sleep is used in the new cases.

The current GCC16 x86 toolchain still cannot compile the preserved MVDM
launcher corpus because its legacy C declaration model is incompatible with
that compiler, separately from the shared library work.

## Next

S8 is owner runtime acceptance.  It must use the refreshed package without
changing user-owned `assets/binary/softpc.ini` or media.

## Reopened repair closure

The owner reopened S7 for the implementation-audit control-fact/Console
repairs and then reported real Window lifecycle edges.  The repaired runtime
now keeps a completed frame per concrete Window/Console output object, so a
Window recreated after pause/resume replays the last complete frame rather
than opening blank.  Window mouse permission is established as a creation
fact without automatically capturing; native capture is transactional, and a
failed Win32 capture cannot leak guest mouse input.  The first client click
that acquires capture remains host-only.

Fresh x64 and x86 package builds each passed full CTest, 29/29.  The owner
manually accepted the resulting runtime behavior.  S7 is therefore closed;
S8 and S9 remain separate, unadmitted T42 work.
