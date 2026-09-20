# KVM Mode Transition And Error Restart Regression

## Owner Failure Report

"启动后，输入win，没能如愿进入windows 3.1，直接machine error；再次尝试start机器，程序直接崩溃退出。"

T71 remains open. This failed manual acceptance follows delivery a044fa36;
S1--S6 test evidence remains historical, not proof of the reported scenario.
Continue as S7. Retain the accepted ownership split and strict capacity contract.

## Investigation Plan

1. Reproduce normal DOS boot then win using the existing Win3.1 disk in overlay.
   Capture the exact rejected producer status/geometry and call stack. S5's
   controller-derived text extent and terminal publication error are suspects,
   not established causes. Do not classify transient values without evidence.
2. Reproduce start after the error and capture its exception/cleanup path.
   Determine whether this is the same cause or a distinct restart defect.
3. Repair the responsible ownership boundary, add focused transition and
   post-error restart coverage, and verify the actual Win3.1 sequence.
4. Build both EXEs, run full suites serially, report production/test additions,
   deletions and net, then commit/push for owner acceptance.

No capacity expansion, silent clipping, Windows-version/mode special case,
guest-media/configuration edit, second frame path or new state owner. Material
interface or MVDM changes require a revised approved brief. Current production
estimate is deliberately unresolved until the failing boundary is reproduced;
report a concrete repair and diff estimate before changing production code.

## Finite Coverage And Diagnostic Budget

Cover initial DOS text, graphics-entry transition, valid graphics publication,
genuine unsupported output, unwind/ERROR completion and subsequent cold start.
Source status, geometry and process exception are distinct evidence. Existing
package tests stop short of entering Win3.1 and do not cover this sequence.

Owned temporary probes may use build/t71-s7 only, with overlay media and no
original image writes. Bound each reproduction to 120 seconds and each trace
to 4 MiB, stop at first terminal error/exception or a confirmed graphics desktop.
The executor owns stopping its launched processes and removing temporary probes
after retaining a concise checkpoint. User processes and preexisting unrelated
Queue/proposal edits remain untouched. Diagnostic and source changes are not
complete implementation commits until both reported failures are resolved.

## Reproduction And Bounded Repair

The real overlay DOS win probe rejects status 3 with live controller extent
80x480 while the original textResize result remains 80x25. S5 sampled mutable
registers independently of the selected renderer. Reuse its existing now_width /
now_height; do not add stabilization, clipping or a mode exception. Genuine
selected renderer overflow remains UNSUPPORTED. Estimate roughly ten changed
Compat lines, with focused intermediate-register and selected-extent tests.

The shipping baseline also reproduces the second symptom: after Machine error,
start exits with code 1, not an access violation. Session collapses ERROR into
STOPPED, App accepts start, Machine rejects it, and Session terminates on that
dispatch failure. Preserve ERROR through Session/App and reject machine commands
with a prompt; keep help/debug/exit available. This preserves Machine's existing
terminal-error contract, not a new recovery protocol. Total production estimate
+15--25/-8--12, with transition and ERROR command-matrix regressions. No Lib or
MVDM changes are required.

## Similar-Issue Sweep And Verification Ledger

Frozen scope: T71 frame validation inputs and the ERROR completion-to-command
path, not all video emulation or general error recovery. Search used
presentation_text_extent/text_surface/now_width/now_height across Compat, VM
and original nt_graph; copy_frame/status branches in VM/Common Machine; and
ERROR/monitor_actual mappings in App/Common Session.

| Member | Disposition and proof |
| --- | --- |
| Text extent | Replace live-register calculation with existing textResize dimensions; focused test mutates live columns/character height without changing selected 80x25. |
| Selected unsupported text | Selected width 81 or rows 50 still fails UNSUPPORTED; uninitialized selected extent has no frame. No clipping. |
| Font capacity | Existing zero/default and height 17 rejection retained; no glyph storage expansion. |
| Graphics extent | Already copied from the allocated renderer DIB, not recomputed from live registers; unchanged. |
| Cursor/palette | Existing renderer callbacks/DIB remain their source; unchanged. |
| ERROR completion | Session retains the existing ERROR fact instead of replacing it with STOPPED. Matrix test verifies it. |
| Monitor admission | Actual App provider rejects start/pause/resume/reset/stop/save/load/floppy in ERROR, retains prompt and permits help. No rejected request is dispatched. |
| External/debug paths | Existing pause/resume admission requires running/paused; synchronous debugger access retains its paused lease check. No new recovery path. |
| Real installed-image path | Extended existing overlay-only restart integration test: boot DOS, enter win, observe running graphics for 15 seconds, stop/start and regain DOS prompt. |
| Shipping failure behavior | Baseline real win then start: process exit 1. Post-repair diagnostic link injects invalid text extent through the same production App/Session: start rejection is displayed and process remains STILL_ACTIVE. Probe is not a shipping build. |

Production accounting relative to a044fa36: four C/H paths, +16/-7, net +9.
Tests: four C paths, +72/-5, net +67. Two Common manifests +4/-4;
documentation and binary artifacts counted separately. The original painter
remains dimension owner, Machine remains lifecycle owner and App owns wording.
No new allocation, thread, state owner, guest-specific branch or retry loop.
Lib, MVDM, INI, media and snapshot format are unchanged.

x64 Release build and full serial suite pass 110/110 in 123.75 seconds,
including the extended real-win integration, package and snapshot tests.
x86 Release build and full serial suite pass 110/110 in 108.17 seconds.
Both suites include snapshots and package tests. Documentation gate and
diff --check pass. Owned diagnostic probes/processes were removed after
retaining the bounded reproduction facts above; no user configuration/media
was removed. This is an implemented/verified delivery, not owner acceptance.

Package footprint: x86 3,655,242 bytes (+29), x64 3,058,784 bytes (+28).
SHA256 x86: 416D4E24C48A8CEF7C83B9C407037ECCF6EE91088F01C4076F0474AFD0B3398A.
SHA256 x64: 9412FC72D5A5AE5DDB7FE9DE0C9DECD4893B50551F2369A5A6270FA2B11A9D65.
Preexisting neutral-text Queue/proposal edits remain outside this delivery.
