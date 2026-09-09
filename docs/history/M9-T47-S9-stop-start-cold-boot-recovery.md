# M9 T47 S9 — Stop/start cold-boot recovery

## Superseded outcome

This record was closed by `e991d93`, but manual acceptance showed that its
`soft_reset` conclusion did not fix the product path. It is retained as the
historical P2 claim only; S9 was immediately reopened in CURRENT and the
corrected evidence/closure is recorded by the subsequent P3 commit.

## Original P2 outcome

The standalone machine boundary again preserves the original reset routine's
internal `soft_reset` fact. A later product `start` is a new run, but it is
not the first initialization of the retained machine object: the original
timer, keyboard, and device teardown/rebuild branch must run before that
second boot. T44 had forced the fact to zero, which suppressed that branch and
could leave the second run at BIOS.

No MVDM source changed. The persistent SoftPC executor and its single
`stop -> start` route remain unchanged.

## Evidence

- `runtime_smoke` now uses explicit test checks, so Release `NDEBUG` cannot
  compile away lifecycle operations embedded in its assertions.
- New `runtime_restart_boot_smoke` uses the installed configured image with
  an in-memory media overlay. It exercises `pause -> stop -> start` and proves
  each run leaves the firmware reset segment. It pauses only to take the
  test-only public snapshot, then resumes; the bounded loop does not assume a
  particular x86/x64 boot duration.
- Fresh x64 CTest passed 34/34; fresh x86 CTest passed 34/34. Both package
  executables were rebuilt. User-owned `assets/binary/softpc.ini` and guest
  media were not changed.

## Superseded closure

This P2 closure is superseded. The manual acceptance path remained failing,
so T47 S9 was reopened rather than treating the direct-runtime observation as
product acceptance.
