# Monitor line and raw viewport

Original request: 好的，准入一个新的S任务进行修复后编译测试提交推送并报我手动测试。Common和lib修复好了以后，如果app/compat/vm等外部组件可以简化清理，请一并清理。

Baseline d6bbbcc. Owner explicitly extends this S to Common and Lib, unlike
the preceding product-only architecture stages. MVDM is not in scope.

## Design and frozen convergence ledger

1. Common owns outstanding line admission through event consumption, not merely
   native read completion. Success sets pending; normal/rejected line consumption
   and confirmed cancellation clear it. Frame only drives presentation; broker
   completion controls monitor handoff. No pending flag suppresses callbacks or
   explicit requests. Common retains only pending_line; provider readiness is
   level-triggered. Notifications, command results and prompts share one outlet.
2. Owner accepts discarding an unfinished line to display notifications now.
   Host cancellation reuses retire/join and returns cancelled versus completed;
   only after join may Common output and rearm. Completed queued lines remain
   consumable. No saved prompt, second editor, cancel loop or extra state machine.
3. Raw viewport target dimensions are max(current viewport, 80x25); buffer
   dimensions cover that target and never shrink. Reposition only within the raw
   surface. Query final actual dimensions; API failure or insufficient actual
   surface is an explicit I/O failure. Restore original cooked screen unchanged.
   No automatic Window fallback is introduced.
4. Shared tests prove frame repetition, completed-but-unconsumed line, rejection,
   successful/failed handoff, notification/request delivery, prompt demand,
   80x13 and 120x13 expansion, large viewport preservation and expansion failure.
   Update all affected shared manifests; product regression remains independent.
5. Sweep App/Compat/VM for redundant prompt/reader state. Only remove state whose
   responsibility has actually moved; preserve lifecycle/debug product policy.

Execution is single-person executor then coordinator review. Produce x86/x64
fixed packages and full CTest evidence, commit/push implementation then review.
T59 stays open for manual acceptance. This task does not claim the intermittent
focus or three-line cursor symptom has been reproduced or fixed.

## Owner refinement

Owner accepts immediate notification and discarding an unfinished line: “我接受
这个体验 你说的流程是对的”. Further constraints: “Common 只保留一个 pending_line”;
“取消读行复用现有 retire/join 路径”; Common UI may retain its boundary forwarding;
viewport uses one target calculation, necessary adjustments and final validation.
“要求：极简主义实现 不得胡乱扩增代码”. No saved prompt or reader-state copy remains
in Common. App's existing prompt readiness remains a level until a transition
reserves it; the provider no longer consumes readiness merely by observing it.

## Executor verification

S6 P1 implementation, 2026-09-14, against d6bbbcc:

- Both Release package/test builds completed without compiler warnings in their
  captured output. Full `test-x64`: 89/89, 124.22 s; `test-x86`: 89/89, 104.36 s.
- New `common.session_monitor` exercises repeated observations/frames, incomplete
  cancellation, completed-but-unconsumed input, notification plus explicit
  request, normal/rejected line consumption, confirmed broker completion, cancel
  failure and rearm failure. No sleeps or alternate input loop.
- New `library.host_console_cancel` uses the real worker with controlled native
  read completion and cancellation barriers. Cancelled fragments produce no line;
  completion wins safely and remains delivered through repeated cancellation.
  Flush after join also excludes a leftover synthetic wake from the next read.
- `library.host_console_display` covers 80x13/120x13 expansion, larger/scrolled
  viewports, native resize rejection and success-without-resize, and cooked
  restoration over repeated roundtrips. Existing retirement/rollback, output,
  composition, lifecycle/debug, package and compact-Console tests all passed.
- Four shared manifests, component/source DAG gates and documentation governance
  passed. Linux keeps its existing unsupported Console implementation shape;
  these Windows tests do not claim Linux native parity.
- Production C/H: 10 files, +133/-34, net +99. Test C/H/CMake: 10 files,
  +292/-5, net +287. Count with `git diff --numstat d6bbbcc -- src test`, excluding
  README/manifests and artifacts; include newly tracked tests. This is a small
  API/behavior addition, not a claimed net code reduction. There is one added
  session boolean, no new object, queue, editor, retry loop or state machine.

Similar-issue sweep: `rg` over Common/App/Compat/VM and Lib Host for
`pending_line`, `prompt_due`, `note_monitor_current`, `request_monitor_line` and
`request_cooked_line`, plus all session result call sites. Dispositions:

- All provider results use the same result outlet; runtime results previously
  ignored are handled as well. Frame completion no longer asks for another line.
- Session owns pending through consumption; Host's existing flag describes native
  read completion and is observed only after join when cancelling. They represent
  different boundaries, not duplicate reader state.
- UI's single forwarding function asserts the expected monitor object through its
  owned broker. App keeps product readiness/debug continuation and wording only;
  its callback no longer consumes readiness. Compat/VM/MVDM require no change.
- Debug's internal pending command continuation is not a native reader and stays
  with Debug. No unrelated state or behavior was removed to reduce line counts.
- Native output retains its existing surface function and cooked restoration;
  no resize compensation loop or automatic Window fallback was added.

Bounded transient build/test logs are removed after retaining this evidence.
Packages: x86 3,504,568 bytes, SHA256
`D345345E501E61B692F35CB262288532DD18747B39A2880715B76A7831E5B68C`;
x64 2,851,477 bytes, SHA256
`DA559D3203C6FE17EED6379D87C6DC4120EF06FBEF8DE90F3EB0191F3C3703E1`.
User configuration and media are unchanged. Delivery still awaits the owner's
manual input/notification and raw/cooked roundtrip checks; T59 is not closed.
