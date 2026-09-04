# M9: Guest Warm-Reset Lifecycle Recovery

## Reported Failure

`Ctrl+Alt+D` correctly reaches the guest as Ctrl+Alt+Del, but the resulting
warm restart stops at `Starting MS-DOS ...`.  Cold boot remains the baseline.

## Objective

Determine which standalone lifecycle, input, timing, or media host boundary
fails to reproduce a hardware warm reset, then repair that boundary without
adding DOS, DPMI, or other product-service semantics to SoftPC.

## Constraints

- Preserve the selected original machine, BIOS, BOP, controllers, ROMs, and
  guest media unchanged.
- Preserve the user-owned `artifacts/binary/softpc.ini`.
- Treat guest Ctrl+Alt+Del as input to the recovered keyboard/reset hardware;
  do not substitute a frontend-only machine restart unless evidence proves
  that is the original host contract.
- Any change below `src/mvdm/softpc.new/` requires an admitted port-ABI scope;
  prefer the application/host lifecycle boundary.

## Investigation Plan

1. Trace the frontend hotkey, keyboard scancode queue, original reset signal,
   executor state, timer/event state, and media/controller reset sequence.
2. Compare the warm and cold paths at the machine façade with bounded,
   non-mutating diagnostics.
3. Add a regression that drives guest Ctrl+Alt+Del and proves continued
   progress beyond the DOS startup banner, without depending on artifacts.
4. Run dual-width GCC build and the full CTest suite; only then refresh the
   executable pair in `artifacts/binary/`.

## Exit

A guest Ctrl+Alt+D warm restart progresses through DOS startup under the
fixed standalone profile, without a new guest-semantic implementation or
configuration/media mutation.
