# M9 T46 S1 — Console-control graphics handoff

## Outcome

With `display=console` and `console_control=1`, the completed transition from
text/raw VM Console to a graphics frame now returns native Console to the
SoftPC cooked monitor.  The monitor publishes and arms a fresh `SoftPC>`
prompt after that broker completion, instead of leaving the prior guest text
on screen with no monitor input line.

The reconciler already derived the correct component plan.  The defect was
the monitor's missing post-handoff prompt rearm, not configuration parsing,
the host broker, or a lib UX policy decision.

## Evidence

- The expanded reconciler test proves the real sequence: text/raw VM Console
  to graphics with value `1` performs create Window, bind monitor, then
  destroys VM Console.
- Fresh x64 and x86 full CTest each passed 32/32.
- The owner accepted the package behavior.

## Closure

T46 S1 changes only SoftPC monitor orchestration, its transition coverage,
product UX documentation, and the agent-owned executable pair. No MVDM
source, shared lib contract, user-owned configuration, or guest media changed.
