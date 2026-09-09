# M9 T46 — Console-control graphics handoff

## S1 — `console_control=1` must return Console to the monitor

## Request

With `display=console` and `console_control=1`, entering the Win3.1 graphics
state still leaves the native Console bound to the raw VM Console.  The
approved product policy requires the opposite: graphics uses a Window while
the SoftPC cooked monitor owns Console.

## Objective

Repair the SoftPC app-owned desired/actual reconciliation path so a completed
text/raw Console route followed by a completed graphics frame performs, in
order: create Window; bind Current Console to the monitor; destroy the VM
Console component.  No lib component decides this product policy.

## Non-goals

- Do not alter `console_control=0`, `display=window`, or text-mode policy.
- Do not change `src/mvdm/softpc.new/**`, host Console broker semantics,
  UX component APIs, hotkeys, package configuration, or guest media.

## Baseline and audit

Configuration parsing accepts `console_control=0|1` and passes it to
`app_presentation` and `app_reconciler`. `app_presentation_derive()` describes
the intended graphics result for value `1`, but existing coverage begins from
an empty monitor state and does not prove the real prior text/raw VM Console
route is retired. The repair must follow completed component/broker facts,
not infer actual state from local handles.

## Verification

Add a deterministic reconciler test with this exact sequence:

```text
running text -> create VM Console -> bind VM Console
graphics frame, console_control=1
  -> create Window -> bind monitor -> destroy VM Console
```

Assert there is no raw Console after the final completion.  Add any focused
SoftPC-level completion-path proof required to expose the actual fault.  Run
x64 and x86 focused tests plus full CTest, refresh both package executables,
and perform a similar-issue sweep over every `console_control` branch.

## Exit

The real text-to-graphics `console_control=1` path reaches monitor Console;
the `0` route remains Window plus raw VM Console; dual-width evidence passes;
all task changes are committed and pushed with a clean worktree.
