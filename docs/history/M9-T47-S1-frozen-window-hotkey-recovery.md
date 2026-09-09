# M9 T47 S1 — Frozen Window hotkey recovery

## Outcome

A frozen `ux-window` now sends native key transitions through the one
source-local `ux-base` matcher. Its post-matcher Window delivery policy silently
consumes ordinary key output, including mismatch replay, and forwards only a
matched copied `UX_EVENT_HOTKEY` to SoftPC. `WM_CHAR` and mouse input remain
frozen guest-input paths and are not retained for later replay.

SoftPC now admits current-run UX hotkeys while paused so pause-toggle can
request resume. The guest adapter independently consumes hotkeys while paused;
CAD and CAF therefore cannot synthesize or deliver guest input before the VM
is running. Cooked monitor lines remain independent of UX admission.

## Evidence

- `softpc-ux-component-contract-smoke` proves post-matcher filtering preserves
  source identity, delivers the registered pause-toggle identifier, and
  consumes ordinary output.
- `softpc-control-reconciler-integration-smoke` proves paused current-run
  hotkey admission, stale-run rejection, and zero paused CAD/CAF guest
  injection.
- Fresh x64 and x86 builds each passed full CTest, 32/32; both package
  executables were refreshed.
- Documentation governance and source-boundary review passed.

## Closure

The owner accepted S1. It changes no MVDM source, guest timing, host Console
ownership, user-owned `assets/binary/softpc.ini`, or guest media.
