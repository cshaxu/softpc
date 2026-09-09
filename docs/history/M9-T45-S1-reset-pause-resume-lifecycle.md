# M9 T45 S1 — Reset, pause, and resume lifecycle

## Outcome

S1 revalidated the cold-reset repair delivered in `010c401`.  The standalone
public reset boundary always uses original cold initialization, while the
guest hardware Ctrl+Alt+Del warm-reset route remains untouched.  This fixes a
stopped machine failing to start again and ensures reset can complete its
normal stop, cold start, pause, and resume chain.

## Evidence

- Source review confirms `softpc_machine_reset()` alone selects cold reset;
  no file below `src/mvdm/softpc.new/` changed.
- Fresh x64 `softpc-runtime-smoke` passed.
- Fresh x86 `softpc-runtime-smoke` passed.
- The real executor probe asserts both `stop -> start -> running` and
  `reset -> stop -> start -> pause -> resume -> running`.

## Closure

S1 changes no source beyond its already committed implementation and no
package configuration or guest media.  T45 S2 is the separately admitted raw
Console hotkey investigation.
