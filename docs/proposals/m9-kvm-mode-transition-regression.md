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

## Coordinator Delivery Review

Reviewed actual commit 1785787c against the original two-part failure report,
the packet, ownership rules and the finite ledger, rather than accepting the
executor summary alone. The only production changes are the selected extent
source and lossless ERROR admission; original renderer, lifecycle and failure
cleanup owners remain intact. The error-path repair intentionally does not
make terminal ERROR restartable. Tests prove prompt-preserving rejection,
not recovery from a failed machine.

Both real overlay startup observations and full suites pass on x86/x64. The
15-second graphics integration check covers the reported immediate failure,
not arbitrary long-running Windows behavior or visual desktop acceptance.
P1 is pushed. Original INI/media and unrelated Queue/proposal edits are retained.
S7 and T71 remain open pending owner retest; no whole-emulator closure claim.

## Continued Owner Retest: Captured Input Stall

Owner confirms Win3.1 entry now works, but reports: 捕捉了鼠标以后，guest内指针
不移动，宿主鼠标指针也没有隐藏，但是宿主鼠标又跑不出窗体；快捷键CAP,
CAM均无法释放被捕获的鼠标。Owner requests continued root-cause investigation.
S7 remains open; do not infer a root cause from the refactor's timing alone.
Compare native capture/message dispatch and Common input delivery with the
pre-T71 baseline. First use an owned native Window probe, then the installed
Win3.1 overlay path. Existing mocked capture tests are not native responsiveness
proof. Reuse build/t71-s7, 120 seconds and 4 MiB per run, with executor-owned
process cleanup. No user INI/media modification. Report concrete evidence and
repair estimate before production edits; retain original public input semantics.

Owner subsequently reports mouse did not fail and redirects investigation:
在win3.1里面切换msdos全屏模式和窗口模式，出现程序崩溃直接退出。
No mouse production repair was made. Native synthetic flood can starve posted
keys, but its relation to the owner scenario is unproved; normal-rate native
probe and real Win3.1 CAP pause/resume passed. Do not mislabel that stress result
as the product root cause. Continue S7 at the reported display transition:
distinguish native exception, explicit frame failure and terminal UI failure,
capture the offending stack/geometry, then compare the pre-T71 behavior.

The headless installed-image probe reproduces ERROR while launching COMMAND
from Program Manager: selected text is 80x25, but font height is 843047920.
Strict syntax checking confirms video.c calls sas_hw_at_no_check without its
declaration. The original byte return is incorrectly treated as implicit int;
the former >16 clamp concealed the ABI error. Including original sas.h was
a diagnostic correction, not a sufficient final repair: the correctly read
BIOS height also disagrees with the current renderer, as recorded below.
Investigate the reported process exit separately before claiming full repair.

After the declaration repair, the same probe exposes a second rejected
intermediate: 80x50, font 0, display_disabled=1, mode_change_required=0.
The original text update algorithms explicitly skip disabled display, while
our independent text snapshot bypasses that readiness boundary. Reuse the
existing disabled/pending-mode flags in text extent admission (+1 condition
line); retain strict validation for an enabled, selected unsupported mode.
No timer, debounce, capacity expansion or additional lifecycle state.

## Owner-Requested Before/After Semantic Audit

Owner requests comparison with pre-T71 code before further implementation:
"按道理我们只是架构调整，把cp437字符映射表挪走，然后 kvm-base的文本帧做一些结构调整，怎么会出现这么多的异常！"
Comparison baseline is 7557ca5; relevant changes are S4 17c9da90, S5 193ff7f0
and S7 1785787c. This ledger supersedes any implication above that startup
coverage proves arbitrary mode-transition correctness. The owner subsequently
admitted the serial repair plan below; the comparison remains its baseline.

| Boundary | Before T71 | Changed or retained behavior; required disposition |
| --- | --- | --- |
| Text extent | Fixed 80x25 surface plus VM capacity clipping. | S5 additionally replaced the extent source with live registers. S7 selects renderer dimensions; disabled/pending-mode readiness still needs verification. Do not classify an intermediate mode as a completed unsupported frame. |
| Font height | BIOS byte read without its declaration; zero or >16 became 16. | S5 removed the >16 fallback. Correctly declared BIOS reading still yields 32 while the current controller character height is 16. Candidate uses current character height; validate its association with selected text rendering, not just its numerical range. |
| No frame | Boolean false skipped publication. | Missing surface/DIB and absent dirty still skip publication. S5 turns invalid/unsupported acquired data into terminal Machine ERROR. Preserve no-frame versus genuine failure without treating mode transitions as faults. |
| ERROR admission | Session mapped ERROR to STOPPED. | Existing mismatch exposed by the new failure path; S7 preserves ERROR and prompt-preserving rejection. No new error-recovery protocol. |
| Snapshot completion | Read/write callers wait for state_event; completion occurs at the pause boundary. | Existing cleanup gap is reachable through the new pre-pause publication failure. Executor unwind must finish outstanding synchronous requests with failure; source-confirmed, fault-injection proof remains required. |
| Required font acquisition | Return value ignored; cleared data can become a valid blank frame. | Retained defect, not introduced by type splitting. Establish readiness/failure at the resource owner; do not substitute zero initialization for successful acquisition. |
| Text layout | Exported stride is 80; original nt_text fast path can address by offset_per_line/2. | Retained discrepancy; nonstandard-offset runtime reproduction remains outstanding. Do not claim it caused the reported exit. |
| Dirty delivery | Common publishes latest data; FIFO notifications carry only sequence/mode. | Retained loss opportunity: consuming notice A can copy latest B and skip A's dirty. Leaf merging cannot recover an update never delivered. |
| Captured input | Native message/input paths require actual responsiveness evidence. | Owner no longer reproduces the mouse symptom. Synthetic flood starvation is not proof of a product regression and does not authorize a speculative mouse repair. |

The current uncommitted Compat candidate rejects disabled/pending-mode text
exports and uses controller character height instead of BIOS bookkeeping.
A headless installed-image probe reached COMMAND and six Alt+Enter transitions
without Machine ERROR. This is neither native-window acceptance nor proof of
the owner's process-exit cause. No rebuilt release or full-suite pass is claimed
for this candidate.

## Superseded FIFO Direction And Implementation Concerns

Historical design discussion: the later owner-approved S9 complete-frame /
surface-difference brief replaces this FIFO direction. No FIFO implementation
was retained or introduced.

Owner requires: "kvm-base的 frame mailbox也必须是FIFO的，然后kvm-console和kvm-window收到以后自行转成正确的latest-win再消费。产品语义必须和以前一样，如果多个dirty需要合并dirty区域，等等。"

This replaces the prior latest-wins Base design; it is not a claim that the
pre-T71 implementation was FIFO. Base must transport opaque frames in order.
Console owns text coalescing; Window owns latest complete pixels and accumulated
dirty, including full invalidation on relevant representation changes. The
current Window update callback executes during Base publication on the producer
thread; moving its definition alone does not satisfy consumer-owned coalescing.

Common must also preserve the relationship between accepted frame data and its
notification until delivery. Currently machine.c overwrites its published pair
and session.c copies the latest frame when handling an older notification.
Changing only the Lib mailbox leaves this earlier loss intact. Do not introduce
a second direct VM-to-presenter path to bypass Session ownership.

Before implementation, specify bounded storage ownership, full-queue behavior,
stop/failure disposal and fair consumption. In particular a machine producer
must not wait indefinitely for Session to drain frames while Session is blocked
waiting for that machine's synchronous snapshot completion. Silent dropping,
unbounded allocation and accidental command starvation are not acceptable
substitutes for the requested FIFO contract. No new S is allocated by this audit.

Required proof remains: distinct A/B dirty updates delayed before Session;
multiple frames delayed before each leaf; mode/size/palette changes; queue-full
and STOP behavior; publication failure during save and restored-load pause;
both initial PIF modes and repeated Win3.1 fullscreen/window transitions on both
widths. Tests must distinguish readiness from a genuine unsupported completed
frame. Original INI/media, public input behavior and snapshot format stay intact.

## Admitted Serial Delivery Plan

Owner: "准入，开始。每个S任务完成都要提交推送保持工作区干净；每个S任务推送后要等我测试。"

- S7 continues with coherent ready text/font production, required-resource
  checking and real Win3.1 prompt roundtrip coverage. Estimate production
  +8--20/-4--12 and tests +80--140; investigate stride before changing the mirror.
- S8 will finish outstanding synchronous requests on executor termination,
  with fault injection and no second cancellation mechanism.
- S9 implements the revised opaque latest-wins complete-frame transport and
  Window-local surface comparison, replacing the earlier unimplemented FIFO plan.
- S10 will compare the complete admitted behavior with pre-T71, remove obsolete
  repairs and audit actual component/mirror diffs and real dual-width workflows.

S7/S8 are now owner-accepted and S9 is active. Every delivery builds both EXEs, runs focused/full evidence,
reports additions/deletions/net, commits/pushes and waits for owner testing.
Do not automatically activate the next S after pushing.

## S7 Continued Repair Evidence

The retained production repair is two existing boundaries: Compat refuses text
export while the original controller disables display or has pending mode
selection, and reads the same controller's character height as its font bank
selection. VM checks required font acquisition instead of publishing cleared
glyphs after failure. No clamp, retry, mode identifier, new state or allocation
is introduced. Relative to 83e185c7, production C/H is +5/-3 (net +2), all in
compat/video.c and vm/driver.c. Lib/Common/MVDM and snapshot format are unchanged.

vga_frame_smoke now separates BIOS bookkeeping from actual character height,
retains zero/default and genuine height-17 rejection, checks disabled and
pending-mode 80x50 as no-frame, and injects missing font planes to require an
explicit failure rather than a valid blank frame. The installed-image restart
test launches each existing fullscreen/windowed PIF, performs six Alt+Enter
roundtrips with CLS input for each, exits the prompt and then cold-restarts to
DOS. Media stays overlay-only. This proves actual execution/frame acceptance,
not visual correctness of every pixel or native Window interaction.
Test C additions/deletions are +117/-2 (net +115).

The prior implicit-declaration cause is removed, not masked: syntax-only
compilation of the modified Compat file with
-Werror=implicit-function-declaration passes on x64. Existing TEXT macro
redefinition warnings are unrelated and are not claimed fixed.

Two earlier x64 package runs failed with extra monitor text ("mpause", "you")
not emitted by the fixture. Input interference is a hypothesis, not a proved
product root cause. An unchanged three-repeat isolated package run passes;
the final full-suite result must also be recorded before delivery.

The stride discrepancy is not declared fixed. Original nt_text's fast path
uses offset_per_line/2 while the exported surface declares 80. The supported
PIF roundtrips pass; nonstandard-offset behavioral proof and disposition remain
in the admitted S10 whole-T audit. Do not introduce a speculative mirror change
or confuse this retained pre-T71 issue with the reproduced font/readiness fault.
S8 request completion and S9 upstream/leaf FIFO remain explicitly unfinished.

Final verification: x64 110/110 in 124.22 seconds and x86 110/110 in 142.06
seconds, each serial full suite including both PIF initial modes and six
roundtrips per mode, snapshot suites and native package tests. After the owner
agreed to leave input idle, the x64 native package test also passed three
consecutive times unchanged. Earlier failures remain disclosed above; no
product-input fix is inferred from successful reruns. Documentation gate and
diff --check pass. Owned temporary probes were removed after recording their
evidence; no user data or configuration was removed.

Package bytes remain x86 3,655,242 and x64 3,058,784. New SHA256 values:
x86 14F276CD400D445CEBBE2461AC4D5166F754B2F5D23579E92210EFC74011AF14;
x64 505C719077FFC9CDFFB5DA53603B8946C6A35BB736A7493D79F54432814D4AE2.
The preexisting owner-requested neutral-text proposal/Queue priority edits
are included intact in this delivery rather than reverted or hidden in stash.
Native visual acceptance remains the owner's next gate; no S8 work starts now.

Coordinator reviewed pushed P3 ff499749 against the admitted S7 boundary:
two production files, no public API or mirror change, strict genuine-capacity
rejection retained, no-frame readiness distinct from font failure. The tests
exercise both existing PIFs and repeated transitions on the actual executor;
they do not claim native visual acceptance. Full dual-width results and package
hashes above match the delivered artifacts. Preexisting Queue edits are intact.
S7 waits for owner feedback; S8--S10 and the T-level audit remain open work.

## S8 Admission And Implementation Brief

Owner accepted S7 and approved the S8 brief. Baseline is 92be0e6a. Common
Machine alone owns request admission/results/completion. Four public synchronous
entries (read_state, write_state, set_removable_media, debug_execute_with_lease)
retain their control-thread serialization and existing driver/event routes.
A short request mutex makes admission atomic with terminal state/cleanup; it
never spans a driver callback, frame copy or wait. Normal request completion
remains unchanged. After run unwind, pending requests fail once through their
existing result/event, while completed results remain untouched. Permanent
worker exit closes admission; a caller cannot register work after cleanup.

Frozen coverage: all four requests, pending read before/after arm, restored
load before pause, driver run failure, frame failure, parked/idle wait failure,
normal stop/reset/shutdown, no-frame success and already-completed results.
Every member needs a focused proof or explicit existing-test disposition.
No new public API, cancel protocol, timeout, retry, queue or product change.
Initial estimate was production +30--60/-10--25; admission race review revises
it to +80--110/-20--35. Tests may require +180--280 to cover both admission
orders and all failure phases. Exact diff and footprint follow verification.

### S8 Finite Exit/Request Review

The bounded sweep searched request flags, base_sync_event_wait/wait_any and
task_destroy throughout common/machine, then every registration and terminal
worker path. The four public synchronous operations use one admission helper;
the sole control caller still serializes payload/result ownership. The request
mutex is separate from frame_lock; no driver callback or wait holds it. Cleanup
occurs after run/driver unwind, before the terminal fact. The internal reset
handoff also fails old pending work before starting the new run. Permanent
worker exit sets the existing terminate flag so no later request can register.

| Member | Disposition and focused proof |
| --- | --- |
| Save before driver arm | Public read_state with frame failure returns IO_ERROR, without PAUSED; common_machine_smoke. |
| Save after arm | Driver run fails before returning a save result; public read_state returns IO_ERROR only after callback removal; common_machine_smoke. |
| Load before restored pause | Public write_state succeeds at the reader hook but fails first frame; returns IO_ERROR, no PAUSED; common_machine_smoke. |
| Pending read / load-request / load-wait / debug / media | Five pending forms crossed with frame failure, paused wait fault, cancellation and idle wait fault; machine_wait_smoke checks event/result, cleared flags and no duplicate completion. |
| Registration versus termination | Deterministic mutex seam ends execution immediately before registration; request rejects. The converse is covered by registered pending cleanup. A dead worker rejects registration even if state otherwise allows it. |
| Reset internal unwind | A pending read fails and signals before the second reset callback; it cannot silently move into the next run. |
| Completed outcomes | Cleared flags preserve result sentinels; normal public save, paused save, load, debug and media remain in common_machine_smoke. |
| Ordinary pause already pending | A save cannot complete merely because PAUSED was reached; the existing read-ready flag must be set. Public fake defers the result across the ordinary pause and proves exactly one payload write. |
| No frame / lifecycle | Existing scripted no-frame, resume/stop/reset and fault/cancel matrix remains; zero valid frame is not an error. |
| Shutdown ownership | Existing failed-join retention and real active/paused/never-started shutdown tests remain; no early object/context release. |
| ready_event | Search finds no reader at all; obsolete allocation/reset/signal/destroy path removed, not replaced by another event. |

The new public-call regression linked against baseline 92be0e6a Machine failed
to finish within its 10-second budget; the owned process was terminated. Current
focused tests pass, including five consecutive repetitions before the final
reset-boundary extension. The baseline probe changes no product or media.
Strict C17 syntax compilation of machine.c with -Wall -Wextra -Wpedantic -Werror
passes. Common and test/common manifests are refreshed independently.

Actual production C diff versus 92be0e6a: machine/machine.c +85/-39, net +46.
Test C diff: common_machine_smoke.c +47/-3, machine_wait_smoke.c +114/-6;
total +161/-9, net +152. No public header, Lib, VM, Compat, MVDM, INI, media or
snapshot-format change. One private mutex replaces the unused ready event;
no new thread, queue, lifecycle state or cancellation protocol is introduced.

The first full x64 run was 108/110: the deferred-save fake could execute one
callback twice because both caller/executor wake paths loaded its waiting flag
before either cleared it. Its single simulated result is now claimed by atomic
exchange, matching the intended exactly-once fixture. Both focused Common tests
then passed 30 consecutive x64 runs and 10 consecutive x86 runs. The unchanged
native modal test failed once per width at its move-loop-still-open assertion,
then passed three isolated runs on each width. The first x86 full run was
109/110, with only that modal test failing. No Lib repair or environmental root
cause is claimed; final full suites remain required. S10's whole-T test-evidence
audit owns this modal-test observation.

### S8 Final Executor Verification

Final Release builds succeeded on both widths. Serial full suites pass:
x64 110/110 in 169.85 seconds; x86 110/110 in 124.72 seconds. Both include
the real Win3.1 PIF roundtrips, snapshot and native package tests. The unchanged
modal test passes in both final suites; preceding failures remain disclosed,
not erased or claimed fixed. Strict C17 warning checks, both Common manifests,
documentation governance and diff --check pass. Owned baseline probe files and
its timed-out process were removed; original media/INI were not touched.

Package footprint versus 92be0e6a: x86 3,656,334 bytes (+1,092),
x64 3,059,363 bytes (+579). SHA256:

- x86: 9BB65EE85A85AD540404BF9A4C0BDF10FEE3376430C1BEA815BE2A5950179ACF
- x64: 58BCD5F45980184B331E692A2E43702EF84F2C81EDA14314139546B10B9B5AAC

The finite S8 request/exit ledger is implemented and verified. S7 owner
acceptance is recorded in its history. S8 still requires post-push review and
owner testing; S9/S10 and T71 remain open. No completion claim covers arbitrary
native synchronization failure or the separate modal-loop observation.

### S8 Post-Push Coordinator Review

Reviewed pushed implementation 81d52730 against baseline 92be0e6a, including
the actual production and test diffs, request admission/unwind ordering,
manifest changes, package hashes and verification records. The finite ledger
above is satisfied: pending work completes after driver unwind, completed
results are preserved, and admission cannot cross terminal cleanup. No public
API or out-of-scope production component changed. Actual production +85/-39
(net +46) and tests +161/-9 (net +152) match the executor report; both delivered
package hashes match the recorded final builds. Final dual-width suites pass
110/110 each; earlier modal-test failures remain assigned to S10, not claimed
fixed. The implementation push leaves HEAD equal to origin/main and the
worktree clean before this review-only record.

S8 delivery is reviewed and awaits owner testing. This is not owner acceptance
or S8 closure. S9/S10 are not started, and T71 remains open.

## S9 Admission And Initial Audit

The FIFO planning section below is superseded by the owner-approved revision
at the end of this document; no FIFO production change was implemented.

Owner accepted S8 and requested "测试通过。下一个S". Baseline 81dacb61;
S8 closure records its accepted evidence. S9 alone is active.

Source audit confirms two boundaries to replace together: Machine overwrites
its double buffer while Session handles sequence notifications by copying the
latest frame; Base publication invokes Window's merge on the producer thread.
Changing Base alone cannot recover dirty lost before Session. Searches cover
publish_frame, capture_frame, acknowledge_frame, published_frame, copy_frame
and FRAME_COMPLETED in Common and both leaves, plus Base mailbox support.

Base transports bounded opaque copied FIFO, without a merge callback. Window
consumes in order into latest complete pixels and accumulated dirty; relevant
representation changes require full refresh. Console retains latest text and
pending output while NOT_CURRENT. Common preserves frame/notification identity;
no driver-to-leaf bypass. Input, capacities and snapshot format stay unchanged.

Before production edits, complete the storage/scheduling ledger: owner, slot
and byte bounds, full behavior, notification failure, STOP, inactive Console
and run replacement. The executor must not wait for a Session synchronously
waiting for it. No silent accepted-frame loss, unbounded allocation, or turning
routine consumer delay into an unreviewed product failure. These are design
obligations still to prove, not completed implementation claims.

Initial planning range pending that ledger: 12--18 production C/H files,
+250--450/-150--250 (net 0--300); tests +250--500/-40--100 (net +150--460).
Documentation, manifests and EXEs are separate. Refine the estimate and report
queue memory footprint before production edits; this is not a naming-only edit.

Finite proof covers delayed upstream A/B and leaves, disjoint dirty, mode/size/
stride/palette changes, full/STOP/failure, Console activation, save/load progress
and run replacement. Build both EXEs, run serial full suites and real Win3.1 PIF
roundtrips, report actual diff/footprint, commit/push and review, then wait for
owner testing. S10 and T71 stay open.

## S9 Revised Complete-Frame / Surface-Difference Brief

Owner: "我觉得可以用这个方案重构，准入修复S9，现在开始。" This replaces
FIFO with independent complete-frame/latest-wins transports. Machine-side dirty
means publish a complete snapshot, not transport a partial patch. Window owns
damage relative to its own RGB surface. Base only copies opaque latest bytes;
Console keeps its existing latest-text/NOT_CURRENT behavior. Session/UI remains
the only route; no direct Machine-to-Base dependency or new queue is introduced.

Remove Base's update callback, Window's producer-thread merge, public graphics
dirty coordinates and redundant palette cache. Window scans the latest indexed
pixels through its supplied palette, compares/writes the existing RGB surface,
and invalidates the enclosing changed rectangle. First render/recreated surface
and representation transitions invalidate fully. Native pending invalidation
must accumulate until paint; exposure paints the surface without a new frame.
No additional full-frame cache is needed. Pure comparison belongs to Window
root render code, native invalidation to its platform implementation.

VM already copies full pixels for a completed dirty update. Keep readiness,
no-dirty suppression and Common text comparison; verify palette-only changes,
first completed frame and geometry changes still publish. Remove the obsolete
dirty fields from its exported frame and diagnostic formatting, not the
original renderer's local dirty/update transaction. No Compat/MVDM edit planned.

Estimate production +40--80/-100--160 (net -120 to -20); tests net +100--200.
Count documentation/manifests/artifacts separately. Frozen coverage: opaque
overwrite/ack/rejection, upstream A/B skip, leaf A/B skip, first/no-change,
pixel/used-palette/unused-palette/stride/size/mode, accumulated invalidation,
producer readiness/no-change/palette, STOP/failure and snapshot progress.
Verify through existing shared and product tests, bounded render timing, dual
Release builds and serial full suites with real PIF roundtrips. Update shared
manifests and actual architecture contract; push/review and wait for owner.

Owned performance probe: build/t71-s9/render-bench.c and its two executables,
no media or trace; at most 30 seconds per run. The executor removes these exact
owned files after recording results. It measures current render-only work,
not overall emulation speed or proof of improvement over a prior T.

### S9 Finite Implementation Ledger

The revised scheme is implemented without FIFO or a new surface cache. Search
of dirty_left/top/right/bottom, kvm_mailbox_frame_update_fn and
kvm_window_update_frame in Lib/Common/VM and active tests finds no retained
cross-boundary damage path. Original Compat dirty transactions remain unchanged.
Common Machine/Session algorithms need no replacement: latest copied snapshots
are self-contained once Window stops depending on producer-relative damage.

| Member | Disposition and proof |
| --- | --- |
| Base overwrite, capture, old ack, rejection and STOP | Callback removed; byte count, separate locks, wake/fault and generation-bound ack retained. frame_copy/frame_lock/input_admission/retirement tests cover the unchanged mechanics. |
| Machine first ready/no-change and upstream A/B | machine_wait publishes not-ready, first frame, unconsumed A then full B, checks both corners; no-change does not increment sequence. Text/cursor comparison remains. |
| Session older notice selects latest B | session_frame checks full B pixel/palette payload reaches UI, retains run-generation and component-readiness checks. |
| Leaf A/B and colour comparison | frame_damage_mouse overwrites A before consumption, checks both changed pixels/bounds, identical output, used/unused palette and colour-equivalent indices. |
| Geometry/stride | Frame validation remains; padded rows do not create damage; recreated surface forces full redraw. Existing geometry tests and native surface tests cover resize. |
| Text/graphics with identical pixel size | Native capture_contract consumes both modes and verifies text invalidates the graphics baseline, then full graphics invalidation. |
| Two updates before native paint | Native consumer test observes both InvalidateRect calls and both pixels in surface; production never validates/clears the native update region between them. Native exposure/paint contract stays unchanged. |
| Producer dirty/readiness/palette | VM still consumes only completed original dirty updates, copies all pixels and omits cross-layer rectangles. vga_frame checks both half updates, no-change suppression and palette-only complete publication across modes. |
| Console | Only support-call signature changes; no character/font/input/output semantic change. Existing NOT_CURRENT/activation/output failure tests retained. |
| Snapshot/lifecycle/input | No format, queue, request or input change; existing full regression remains required. |

Production C/H against 65ad144a: 12 files, +41/-89 (net -48). Test C/H:
8 files, +174/-44 (net +130). Documentation, manifests and EXEs are separate.
Common production change is a contract comment only. MVDM/Compat and INI/media
are unchanged. Each graphical frame loses 16 bytes of producer damage fields;
Window loses its 1024-byte palette cache, with no new buffers or workers.

The new Common publication test first omitted the mandatory fake request_wake
callback and failed creation; completing the fixture fixes that failure without
production changes. Focused x64 frame/native-consumer/Session tests and the
corrected Common publication test pass. Changed Lib C sources pass strict C17
-Wall -Wextra -Wpedantic -Werror syntax checks. Full suites remain required.

Render-only measurement, 1000 iterations each, milliseconds per consumed frame:

| Width | Pixels | Unchanged | One pixel changed | Palette-wide change |
| --- | --- | --- | --- | --- |
| x64 | 640x480 | 0.262 | 0.323 | 0.605 |
| x64 | 1280x768 | 0.849 | 0.958 | 1.871 |
| x86 | 640x480 | 0.456 | 0.500 | 0.751 |
| x86 | 1280x768 | 1.530 | 1.755 | 3.307 |

These bounded clock measurements include comparison and surface writes, not
native painting or emulation. They are not a prior-T speedup claim; x86 ran
alongside a build, so the numbers are indicative rather than isolated benchmarks.

### S9 P1 Delivery Evidence

Final serial full suites: x64 110/110 in 173.66 seconds; x86 110/110 in
163.13 seconds. Both include real Win3.1 initial window/fullscreen PIFs and
roundtrips, package, lifecycle, snapshot and shared manifest checks. This is
not pixel-by-pixel visual acceptance; the owner still tests the packages.
Strict changed-Lib syntax checks pass on both toolchains. Documentation
governance and diff whitespace checks pass. All four shared manifests use
shared-t71-s9-p1. The exact three owned performance-probe files and empty
build/t71-s9 directory were removed after runs; no owned probe remains.

| Package | Bytes | Delta from S8 | SHA256 |
| --- | --- | --- | --- |
| assets/binary/softpc32.exe | 3655249 | -1085 | F7A7508332A992E3697A88CB71F8672B7D47381F4831F94A9056AE53FA0D4C78 |
| assets/binary/softpc64.exe | 3058791 | -572 | 3665A3D3FBE516A900AAD7A11DC62045AD286F6DA758B0FE2C6DC32C32433CD1 |

Final executor review confirms no leftover frame-update callback or transported
dirty coordinates in the active paths. Native surface invalidity resets on
recreation and text transition; repeated identical RGB pixels need no drawing.
STOP, separate mailbox locks and generation-bound acknowledgements are unchanged.
Graphics-frame ABI drops its dirty fields and support publication APIs drop the
callback parameter; all consumers and tests migrate together. No extra public
input API or snapshot change. P1 is ready for commit/push and coordinator review;
S9/T71 remain open and S10 is not admitted.
