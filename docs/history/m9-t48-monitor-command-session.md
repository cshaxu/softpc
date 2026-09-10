# M9 T48 — Monitor command session extraction

## Original request

Extract monitor command handling from `src/app/main.c` into a new `command.c`
so `main.c` becomes process wiring and queue dispatch. The command session must
own the rule for when a cooked SoftPC monitor prompt is armed. In particular,
with `display=window`, both `console_control=0` and `console_control=1` keep
the cooked monitor usable while the VM is running: after accepted `start` or
`resume`, it must publish a new `SoftPC>` prompt and arm the next cooked line.

## Objective

Make the monitor command turn an explicit, testable SoftPC app state machine.
It parses monitor lines, tracks lifecycle intent/completion, and emits copied
product effects. `main.c` only owns object lifetime, control-queue dispatch,
and execution of those effects. `monitor.c` remains a neutral cooked-Console
adapter; `presentation/reconciler` remains the owner of Window/VM-Console/
Current-Console derivation.

## Boundaries and non-goals

- May change only `src/app/`, focused tests, task documentation, and the two
  agent-owned package executables.
- Must not modify `src/mvdm/softpc.new/**`, shared `src/lib/**`, host Console
  broker behavior, `softpc.ini`, guest media, or lifecycle semantics.
- This is not a second control queue or a new monitor input path. Monitor
  lines, UX events, runtime completions, and broker completions remain on the
  existing SoftPC control queue and retain arrival order.
- `console_control` remains ignored when `display=window`; it is read only for
  Console display routing.

## Design and implementation

1. Add `command.c/.h` with an app-owned monitor command-session object. It
   owns monitor stable state, reset/stop turn facts, line parsing, lifecycle
   command resolution, and prompt eligibility. It does not call runtime,
   broker, or native Console APIs.
2. The session emits copied effects only: write text, lifecycle/reconciler
   intent, arm prompt, and exit. `main.c` applies those effects through the
   existing monitor/presentation/runtime objects.
3. A lifecycle command consumes the active cooked line and creates one pending
   turn. The session may arm a new line only once the turn reaches a stable
   completion:
   - paused and externally stopped: arm monitor;
   - running with `display=window`: arm monitor;
   - Console display: do not arm while raw VM Console owns input; arm after a
     broker completion confirms the monitor became Current Console;
   - reset's internal stopped stage never arms a second line.
4. Local monitor commands (`help`, media commands, invalid input) complete
   synchronously and arm the next line through the same effect mechanism.
5. Move existing monitor command parsing and lifecycle guidance without
   changing the approved command matrix or text except where tests expose an
   existing defect.

## Verification

- Unit-test command effects for every lifecycle state/command cell, prompt
  behavior on runtime and broker completion, reset's internal stop, and local
  commands.
- Add integration coverage for `display=window` under both `console_control`
  values: after `start`, and after `pause -> resume`, the running monitor
  accepts a subsequent lifecycle command.
- Retain Console-display raw ownership coverage: running text does not arm
  monitor; graphical `console_control=1` arms only after monitor broker
  completion.
- Fresh x64/x86 build and full CTest; refresh only `softpc32.exe` and
  `softpc64.exe`; manual package verification for the Window display flow.

## Exit criteria

`main.c` no longer owns monitor command parsing, lifecycle turn state, or
prompt policy. `command.c` is the only product owner of those facts;
`display=window` running monitor input works independent of `console_control`;
Console-display raw ownership remains unchanged; MVDM/lib/host and user-owned
package configuration remain unchanged; dual-width tests pass and all work is
committed and pushed with a clean worktree.
