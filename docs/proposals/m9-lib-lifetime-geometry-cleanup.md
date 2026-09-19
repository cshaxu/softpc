# Lib lifetime, geometry and file-lock cleanup

## T70 S12 admission

Original owner request: "以上四处 准入一个s任务开始修复到推送".
Baseline: 58ac394, clean main; S11 manually accepted. One executor/coordinator
session implements and independently reviews the actual diff before delivery.

## Frozen scope and design

| Unit | Defect and retained owner | Required proof |
| --- | --- | --- |
| Base task lifetime | Join failure currently permits disposal. Base returns status from cancellation/join/destroy; unsuccessful cancellation or join retains ownership. Existing consumers must not free referents after failure. | Inject native wait and pthread join failure; retained task/event; successful retry and normal destruction. |
| Window integer geometry | Floor rounding alternates the limiting axis on repeated fit. Existing geometry uses ceiling pixel rounding within the available rectangle. | Repeated fit is identical across aspect/size matrix, bounded output, edge anchors and existing sizing tests. |
| Linux storage lease | Process-associated fcntl locks can be converted/released by another same-process open. Use open-file-description flock shared/exclusive nonblocking locks. | Controlled failures plus Linux-native same-file and hard-link open/close lifetime test. |
| Window startup cleanup | Four acquired-window failure branches duplicate cleanup. One local failure label retains the original startup status. | Title/cursor/timer/notification failures leave no live Window and preserve the failure result. |

Permitted dispositions: repaired with focused proof, unchanged with explicit
reason, or verification unavailable explicitly recorded. Completion requires
all four production repairs and dual-width regression; Linux-native execution
must be reported separately, never inferred from Windows fakes.

No new manager, retry loop, thread, callback, configuration, guest workaround
or MVDM change. Common may only propagate Base shutdown failure to its caller;
App must not destroy live callback targets, and audio must retain live globals.
No user INI or guest media changes. Shared manifests follow their changed corpus.

Pre-change estimate: 140--220 changed production lines, net -10 to +60;
120--200 additional test lines. Account tracked production/tests with
`git diff --numstat 58ac394`, excluding documentation/manifests/EXEs.

Similar-issue sweep: all Base task join/destroy consumers and selected platform
implementations; all Window aspect fitting callers; Linux storage open/close
lock ownership; all Window worker startup failure branches. No whole-Lib
correctness claim. Retain the existing KVM bounded-join contracts.

Verification: deterministic fault tests; x86/x64 builds and serial complete
CTest suites; corpus hashes, component DAG/types and documentation gates.
Linux-native test execution depends on an available Linux host (WSL absent on
the current Windows host). Diagnostics/logs stay in ignored build/t70-s12;
no installation of a new OS, no unbounded traces, no asset media mutation.

Linux lock choice follows [flock(2)](https://man7.org/linux/man-pages/man2/flock.2.html):
independent opens conflict even within one process; close releases only that
open description's lock. This remains advisory cooperation, not protection
against arbitrary uncooperative writers. Network filesystem locking depends
on the mounted filesystem; this task does not promise new network semantics.

## Implementation review and similar-issue dispositions

Searches: `rg base_sync_task_(join|destroy|request_cancel) src test`, platform
task joins, Window fit/constrain callers, Storage lock declarations/opens and
Window worker startup exits. The retained paths are:

- Base: both selected backends return join failure; Linux sets its existing
  joined flag only after success. Root cancellation/destroy propagate status
  without freeing task/Event. No timeout or retry manager was introduced.
- Consumers: Common shutdown/destroy retain machine/worker on error. App reports
  failed shutdown and exits without freeing callback targets. Audio reports
  failure and retains its process-global task/events. Common create failures
  precede a successful task creation, so their existing cleanup is safe.
  KVM's separate bounded joins already retain unjoined workers and stay unchanged.
- Geometry: fit/client/outer/maximize/enforce and all drag edges use one ceiling
  rule. Dirty-rectangle floor/ceiling edges express coverage, not aspect fitting,
  and remain unchanged. No dimension history, tolerance or debounce was added.
- Storage: one `flock` per open description replaces process-scoped fcntl state;
  no lock-conversion or path registry exists. Failed opens close only their own
  stream. The ordinary binary writer is not a medium lease and is unchanged.
- Window: class/window creation fails before acquired-resource cleanup; title,
  cursor, timer and notify selection share the new cleanup label. Ready-signal
  and post-start failures keep the existing runtime retirement path.

Focused tests exercise cancellation/WAIT_FAILED/pthread_join failures, retained
Common/audio referents and successful cleanup retry, 69,204 aspect combinations,
drag-edge anchors, and all four acquired-window startup failures. Retry in tests
proves retained ownership; production does not introduce automatic retries.
The first x64 full run passed 108/109: the existing capture test still expected
floor(700*4/3)=933. It now expects ceiling=934 and additionally verifies refit
stability; no capture implementation or assertion removal was used to fix it.
The initial x86 build used the wrong toolchain PATH and failed to launch some
compiler subprocesses; selecting the configured mingw32 runtime fixed the build
environment, not product code.

## Changed-path accounting

Against 58ac394, tracked production C/H changes are 15 files, +93/-81, net +12.
Within that total Lib is 11 files, +67/-70, net -3. The four other files are
Common machine.c/machine_interface.h, App composition.c and Compat audio.c:
only propagation/handling of failed worker shutdown. No VM or mirror diff.
Tests/build checks are 12 files, +177/-29, net +148, including one new native
Linux lock test. Documentation, manifests and package EXEs are excluded.

The surviving ownership remains Base task, Window geometry/startup and Storage
file. No new object, state field, thread or fallback path is introduced.
The only public changes are status returns for Base cancellation/join/destroy
and Common machine shutdown/destroy. KVM public APIs/events are unchanged.

Linux-native execution is unavailable on this host: `wsl --list --quiet`
reports WSL is not installed. Controlled Linux join/file tests pass on Windows;
they do not prove kernel lock behavior. The native test is registered only on
Linux and checks conflicting same-process opens, hard-link identity, unrelated
descriptor close and survival of a remaining shared reader. No Linux-native
execution pass is claimed.

## Delivery verification

Both Release package EXEs rebuilt. Final serial regression: x86 109/109
(100.84 seconds), x64 109/109 (72.35 seconds), including all 33 Lib and 18
Common tests, package interaction and snapshot tests. Logs are retained in
ignored build/t70-s12/tests-x86-final.log and tests-x64-delivery.log.
All four corpus manifests, source/type/DAG checks, documentation governance
and `git diff --check` pass. A manual supplementary DAG invocation omitted its
required absolute root, was interrupted, then rerun with the explicit Lib
root; the CTest invocation already supplied it correctly. No verifier change.

Final executable SHA-256:

- x86: B70BCB79FD6E378032E7569600AB9F6F19FDA2D185E77F340D35FA89E9CDF7A4
- x64: 6503A85A89755335866F344F399220B923514AA14865CE345D769897E13575D9

Executor review compared the production/test diff with all four admitted units:
no extra lifetime manager or compatibility branch, no mirror/VM/INI/media
change, and no disabled tests. Delivery awaits the owner's Windows validation;
T70 remains open. Native Linux lock execution remains the disclosed limit above.
