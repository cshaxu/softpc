# M9 T44: Cold Restart Lifecycle Repair

## Outcome

The monitor's public `start` and `reset` path now always invokes the original
cold-reset branch.  Reusing a stopped standalone machine no longer marks its
next reset as a guest warm reset and therefore repeats the original FDC,
mouse, and fixed-disk initialization required by a fresh run.

Guest Ctrl+Alt+Del remains separate: it continues through the recovered
hardware CPU-reset route and keeps original warm-reset semantics.

## Regression

`runtime_smoke` now proves real executor completion, not request acceptance:

- `stop -> start -> RUNNING` reaches the new run's running state; and
- the reconciler's `reset -> stop -> start -> pause -> resume -> RUNNING`
  chain completes against the real machine/runtime envelope.

The repair also corrected an existing paused-input test that accidentally
asserted against a prior `SOURCE_RETIRED` record instead of the documented
late key-make record.

## Verification

- x64 Debug full CTest: 32/32 passed, with assertions enabled.
- x86 full CTest: 32/32 passed, including the asserted runtime lifecycle
  regression.
- x64 release focused runtime/control/package tests passed; x86 release
  package executable was rebuilt during its full regression.
