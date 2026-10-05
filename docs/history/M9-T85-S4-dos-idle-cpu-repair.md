# M9 T85 S4: DOS Idle CPU Repair Closure

Original requests: 准入一个S任务，对cpu占用的现象进行观测、记录，然后对根因进行调研。
Then: 请你在当前S内修复，然后自行测试看看是否解决。
Owner acceptance: 非常好，收口S4，验收通过，保持T开放。然后队列任务列一下。
S4 closes on 2026-10-04. T85 stays open with no active S.

## Actual-change review and requirement coverage

Observation P1 `1bc02185` and implementation P2 `a841b5d3` are pushed.
Coordinator reviews the actual seven-path implementation against the request,
packet and [proposal evidence](../proposals/m9-windows-101-startup-repair.md).
One production file, Compat platform.c, enables/resets original idle detection
on continuous run entry and disables it on exit. Its existing host idle callback
waits on the existing executor event unless work is pending or execution is
instruction-budget bounded. Actual idle duration is excluded from pacing credit.
No new API, state object, thread, event, fixed-loop sleep or guest-specific
condition exists. Lib/Common/x86, the original mirror, timer period and frame
publication remain unchanged. The other paths are two product tests, two EXEs
and two task documents.

Reproducible accounting: `git diff --numstat 1bc02185 a841b5d3 -- src test`.
Production +20/-7 (net +13); platform tests +34/-0; real DOS restart tests
+22/-0. Total code +76/-7 (net +69). Existing wait-failure/termination owner
is retained; no duplicate recovery or alternate executor path is introduced.

Finite sweep covers original idle enable/poll/wait sites, pacing, HLT/quick
events, input/control/timer wake ownership and frame publication. Each member
has source evidence, focused regression or an explicit qualification limit in
the proposal. Pending-work bypass, finite-budget bypass, signal retention and
pacing-origin adjustment receive permanent tests; native wait failure retains
the existing HLT termination proof.

## Verification and limits

Verified headless DOS prompt, ten-second settling, then three two-second CPU
samples: x64 mean 97.27 percent before versus 0.26 after; x86 96.78 before
versus below OS CPU-accounting resolution after. This is real product-core
execution, not a complete native Window CPU or input-latency benchmark.
Incorrectly linked scratch probes lacking firmware were discarded explicitly.

Both Release builds pass. Final x64 background CTest passes 121/121 (105.22s);
final serial x86 passes 121/121 (145.74s). An earlier parallel x86 run failed
the existing Win3.1 windowed DOS roundtrip; three focused repeats and the final
serial suite pass without weakening assertions or timeouts. Its intermittent
cause remains unproven. Five native desktop cases per width are excluded;
no Linux/RDP or exhaustive device-workload qualification is claimed.
The owner explicitly accepts the delivered behavior. This closure does not
reinterpret intermittent test success as proof of all possible interleavings.

Accepted EXE SHA-256:

- x86: CDAD3C003A9136962FFB60127BD94BAD7F213E99EAA91844AA0C676CDB76CF6F
- x64: 2586446090C690A956FD701AE13F02988C50E75B3CF75ED08C82ECFB0D0AF02F

Closure is document-only; accepted code and binaries remain unchanged.
Owned probe processes and disposable media have exited/been removed. Diagnostic
scratch is retired after retaining the bounded evidence above and in the proposal.
Owner INI and snapshot modifications are preserved and excluded from this
closure, consistently with the prior explicit exclusion. No clean-worktree
claim is made while those edits remain. Queue and TODO are unchanged.
