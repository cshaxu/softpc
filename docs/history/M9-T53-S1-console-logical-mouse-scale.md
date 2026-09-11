# M9 T53 S1 — Console Logical Mouse Scale

`ui-console` now maps a one-cell raw Console delta to eight logical relative
units on both axes.  The previous vertical sixteen-unit conversion caused the
observed two-row movement.  The first raw position remains a zero-relative
baseline; no MVDM, Window, application queue, guest protocol, capture policy,
or user configuration changed.

Production paths: one changed (`src/lib/ui-console/win32/component.c`). Test
paths: one added (`test/unit/ui_console_mouse_scale_smoke.c`).  The focused
smoke proves `{8,0}` and `{0,8}` output.  Sweep: `rg -n "\\* *8|\\* *16|delta_[xy]"
src/lib/ui-console` found only this raw-coordinate conversion; no retained
font/presentation multiplier is an input conversion.

Evidence: fresh x64 and x86 builds and CTest each passed 37/37; library
manifest, documentation-governance, and diff checks passed.  Package outputs
were refreshed as `assets/binary/softpc32.exe` and `softpc64.exe`.  Executor
delivery: `13c5075`.

## T53 Closure Audit

The owner requested a narrow repair for raw Console vertical mouse movement,
then requested test, dual-width build, commit, push, and a runnable package.
S1 is the only admitted subtask and changed only the declared `ui-console`
conversion plus its focused proof.  No deferred work or queue transfer arose.
The retained input owner is `ui-console`; Window conversion and guest mouse
handling remain separate, untouched owners.  Relative to `291afe4`, tracked
production code is +7/-3 (net +4) and test code is +69; documentation and
binary package artifacts are excluded from that count.
