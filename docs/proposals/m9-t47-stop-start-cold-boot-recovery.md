# M9 T47 S9 — Stop/start cold-boot recovery

## Original request

After the monitor accepts `stop`, a later `start` must re-enter the configured
guest boot path. It currently can remain at BIOS.

## Objective

Repair the standalone `running -> pause -> stop -> start` path so every new
run re-enters the configured DOS boot path without accepting any guest input
belonging to the prior run. An executor/IP observation alone is not
sufficient: each run must publish a copied `C:\\>` text frame with its own
SoftPC run generation.

## Baseline and root boundary

`app_runtime_start()` clears the app stop request, increments the SoftPC-only
run generation, and asks its persistent executor worker to call the public
`softpc_machine_reset()` boundary. The runtime's VM input FIFO previously
survived that boundary, and paused key/mouse releases could still enter it.
Those are guest facts from the old controller lifetime; a new BIOS must never
consume them. The repeatable package reproduction also proved a lower
host/CCPU boundary: the original `c_cpu_reset()` resets architectural CPU
state but retains CCPU's private pending asynchronous-interrupt map. A bit
from the completed run can redirect the new BIOS immediately after its reset
vector. The standalone CCPU port-ABI lifecycle boundary clears that private
map only after the old executor run completed and before a public cold run
begins. Monitor lines and registered hotkeys remain in the independent product
control FIFO and are not affected.

The audit preserves T44's approved `soft_reset = 0` public cold-start policy.
The earlier S9 claim that changing this original internal fact solved the issue
is superseded: it did not change the owner-observed failure. No MVDM reset
branch is modified; the standalone runtime owns its own input lifetime.

## Boundaries and non-goals

- May modify only standalone `src/app/`, `src/host/`, focused tests, task
  documentation, and package executables.
- Must not modify `src/mvdm/softpc.new/**`, interpret guest BIOS/DOS state in
  the frontend, create a second executor, or add a second restart path.
- `stop`, `start`, and `reset` retain the approved monitor matrix. A start is
  still a cold run; guest Ctrl+Alt+Del remains a distinct machine warm reset.
- This task does not alter lib UX, Current Console ownership, or user-owned
  `assets/binary/softpc.ini`.

## Implementation and proof

1. Clear the VM-input FIFO before each new run; do not clear or reinterpret
   the monitor/control FIFO.
2. Keep all ordinary guest key/mouse events, including releases and source
   retirement cleanup, out of a paused or stopped VM. Registered hotkeys and
   monitor commands continue through the product control path.
3. Prove with configured installed media that the initial run plus three
   successive `pause -> stop -> start` runs each publish their own copied
   `C:\\>` frame within ten seconds.
4. Prove the low-level queue clear and paused ingress gate independently;
   retain the existing run-generation frame boundary test.
5. Drive the shipping package through the actual native Console route:
   `start -> raw CAP -> monitor stop -> start`, and require the former DOS
   prompt to disappear before observing a new boot banner and a new `C:\\>`.
   Repeat that path without relying on an executor-only observation.
6. Build x64 and x86, run their full CTest suites and package smoke, refresh
   only the two agent-owned package executables, then retain corrected S9
   evidence in history.

## Exit criteria

The configured package reaches a new `C:\\>` frame after each tested monitor
`pause -> stop -> start`; stale VM input cannot reach a paused/stopped guest or
survive into a cold run; MVDM is unchanged; x64/x86 full CTest passes; all
changes are committed and pushed with a clean worktree.
