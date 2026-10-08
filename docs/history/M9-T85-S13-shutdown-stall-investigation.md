# M9 T85 S13 Shutdown-Stall Investigation

## Scope and disposition

The owner admitted S13 to reproduce and repair, or remove if unreproducible,
two deferred observations:

- `softpc-command-provider-smoke` had once exceeded its 45-second limit after
  load/resume/stop activity.
- x86 `softpc-presentation-shutdown-smoke` had once exceeded its unchanged
  10-second limit.

No failure reproduced and no unfinished shutdown owner was found. Per the
owner's explicit disposition rule, both entries are removed from `TODO.md`.

## Reproduction matrix

The two existing focused executables were rebuilt only because the preceding
clean package build had removed them. No source was changed.

| Width | Initial isolated passes per test | Additional isolated passes per test | Result |
| --- | ---: | ---: | --- |
| x64 | 10 | 50 | command-provider 60/60; presentation-shutdown 60/60 |
| x86 | 10 | 50 | command-provider 60/60; presentation-shutdown 60/60 |

The additional run used each test's existing CTest timeout and stopped on first
failure. It completed 200 executions without a timeout. The slowest observed
x64 command-provider pass was about 1.6 seconds; the slowest observed x64
presentation-shutdown pass was about 1.4 seconds. The final x86 presentation
shutdown pass completed in 0.65 seconds. Native desktop tests were not run.

No full product regression is claimed: this S retires observations without a
production or test-code change, so the focused matrix is the proportional
verification evidence.

## Lifecycle sweep

The relevant product termination paths have one owner each:

- `common_machine_shutdown()` closes admission, signals the existing command
  and resume events, then joins the Common executor through the Base task
  owner.
- `softpc_platform_audio_shutdown()` clears the tone request, cancels the
  Audio stream's native writable wait, signals its existing stop/wake events,
  then joins its worker.
- `presentation_shutdown_smoke` supplies a scripted UI and does not create a
  native Console or Window worker.

The sweep covered product task joins, Audio's task join and fixed-fixture
integration shutdown tests. No path in that scope waits without its established
cancellation/completion owner. Consequently S13 adds no timeout, sleep,
polling loop, second wake path or public interface.

## Changed-path accounting and delivery

Production: `+0/-0` lines and paths. Tests/build: `+0/-0`. Documentation:
this history record, the active-task/proposal disposition and removal of the
two stale TODO entries. Temporary build scripts, logs and scheduled tasks used
for the repetitions were deleted. No package executable, INI, snapshot or
guest-media change is included.

The executor delivery and independent documentation/diff review are contained
in the same P commit. T85 remains open for owner direction.
