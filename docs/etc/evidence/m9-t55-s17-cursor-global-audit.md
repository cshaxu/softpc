# M9 T55 S17: cursor conversion and whole-library failure audit

Original request: “哎，这个window模式的光标位置完全错了嘛，跑到上方去了，
你当时的改正是错误的，要求修复！另外：所有故障、通知等问题，怎么每次审计
修复完都一直有新的？我要你全局审计而不是局部审计和修复”.

Baseline 77d3d7f. Owner-modified package INI is preserved byte-for-byte and will
travel with delivery. No MVDM, media, native mouse or deferred CLS changes.

## Plan and finite coverage

1. Correct the application percentage-to-bottom-scanline conversion, using real
   font height. Preserve lib's literal top/bottom drawing. Test producer values
   and the final rendered rectangle, then both executable widths.
2. Inventory every lib component, public failure/notification/lifetime contract,
   selected platform implementation and application consumer. Classify every
   family as checked result, asynchronous fault, explicit unsupported, or a
   verified gap. Do not limit discovery to S16 files or successful regression.
3. Produce a unified repair design for remaining contracts; API/behavior changes
   beyond the approved UI-destroy boundary require explicit design admission,
   not another hidden local patch. Do not claim global defects repaired merely
   because the cursor fix and current tests pass.

| Family | Coverage unit | Disposition |
| --- | --- | --- |
| types | Header-only vocabulary; no runtime owner | No independent failure channel needed; wrappers must not acquire component policy. |
| console | object/reference/binding/event/output and both mutex implementations | Checked binding/output paths; Linux mutex result gap G4. |
| host | broker, readers, activation, clock, event/task and both platforms | Broker/task teardown and notification gaps G2/G4; clock/platform errors are distinct from ownership completion. |
| storage | file/writer/medium and selected file primitives | Writer read/write/flush/close checked; medium close observability gap G5. O(n) overlay remains explicitly deferred. |
| ui-base | input/matcher/frame/control/notify/fault/retire/destroy | Checked admission and bounded destroy; asynchronous failure visibility gap G1. |
| ui-window | startup, input, rendering, timer, capture, notification, shutdown | Startup G3; capture cleanup G5; phase/metric consistency G6. |
| ui-console | activation/input reset, read delivery, frame output, shutdown | Uses common terminal path, but activation notification can fail without waking worker: G1. Linux remains explicit UNSUPPORTED. |
| SoftPC integration | cursor producer, monitor/broker disposal, control and UI failure sinks | Cursor fixed; monitor assumes void broker destroy completed, part of G2. |

Unit of audit is the whole operation chain: request -> acceptance -> worker
completion/failure -> caller observation -> join -> disposal. Success fixtures
alone never prove fault-path completion. The code snapshot contains 36 lib C
files; platform placeholders are recorded rather than mistaken for parity.

## Cursor root cause

The compatibility boundary supplies CONSOLE_CURSOR_INFO.dwSize (bottom-aligned
percentage). Runtime encoded top=0 and bottom=percentage of a hard-coded 16.
The former bottom-forcing renderer masked that producer error. Literal scanline
rendering exposed it. Correct the producer; do not break legitimate top/middle
shapes in lib. Use font height from the copied frame, not a second font source.

## Whole-library findings and unified repair boundaries

These are findings, not claims that the following contracts have been repaired.
Audit covered the operation families above in both platform source selections;
Linux inspection is not Linux runtime acceptance.

### G1 / High: failure reporting depends on the failed wake path

`ui-base/component.c` records failure and closes admission in
`ui_component_fail`, but the failure sink is called during worker retirement.
`ui-console/win32/component.c` can enter this path from an ACTIVATED callback
whose wake failed. If notification remains broken, the sleeping worker cannot
retire, so the app receives neither the failure nor SOURCE_RETIRED. The existing
`ui_shutdown_failure_smoke` mode 3 actually observes zero failure/retirement
callbacks before its rescue. Bounded destroy alone does not notify an app that
has no reason yet to call destroy.

Proposed common correction: publish the first terminal failure exactly once
independently of worker retirement, through the existing nonblocking failure
sink. Close admission immediately; retire the source only after input ends;
dispose only after join succeeds. Define callback thread/lifetime and prohibit
reentrant destruction. Do not create a fallback queue, polling thread or retry
loop. This changes the callback contract and needs explicit admission.

### G2 / High: void teardown hides failure while callers free dependencies

`host/console.c:host_console_broker_destroy` returns early on failed backend
deactivation but returns void. `app/monitor.c:app_monitor_console_destroy`
then clears the sink, releases the Console and frees the monitor. The broker's
retained resources are not communicated to that caller. Broker creation failure
cleanup must be reviewed under the same retained-resource rule, not only normal
destroy.

`host/sync.c` similarly destroys task resources after a void join. Windows ignores
the native wait result; Linux ignores pthread_join's result and sets joined true.
These are conditional native-failure paths, not a claim of observed normal-run
use-after-free. They nevertheless violate proof-before-dispose.

Proposed common correction: checked join/deactivate/destroy, clear ownership only
on confirmed completion, and update every application caller in the same change.
For creation, specify how an incompletely retired worker retains its context;
never return an ordinary failed-create/null handle while abandoning a live
worker's dependencies. Do not impose an arbitrary short timeout on every valid
host task or conflate logical STOP with proven physical retirement.

### G3 / High: Window creation still waits on an unchecked sole notification

`ui-window/win32/component.c` signals ready with unchecked SetEvent calls on
startup exits; the creator waits INFINITE on ready alone. A diagnostic compiled
against the actual worker injected class-registration failure plus ready-signal
failure. It observed the attempted signal, then creator wait result 258
(WAIT_TIMEOUT) while the worker had exited. No native Window was created.

Use ready-or-worker-exit as the startup completion observation, check the wait,
and make failure cleanup obey G2. A bounded public wait by itself is insufficient
if the caller may free callback context while the worker is still alive. Review
all creation/startup paths with this same ownership invariant.

### G4 / Medium: synchronization failures are not consistently observable

`host/win32/sync.c` discards SetEvent/ResetEvent results; public signal/reset/cancel
are void. Linux host signal/reset discard mutex/broadcast results. The logical
Console's `console/linux/mutex.c` likewise ignores lock/unlock results, and Linux
mailbox wait ignores its final unlock result. A failed lock must not authorize
an unprotected operation. These are error-contract gaps, not evidence that valid
mutexes commonly fail.

Classify fallible public operations as checked results and internal invariant
failures as terminal faults; propagate each once to its existing owner. Keep
platform vocabulary in types and implementation in the owning component. No
console-to-host dependency and no new generic error framework.

### G5 / Medium: distinguish attempted side effects from completed side effects

Window mouse release clears local capture state before unchecked ClipCursor(NULL)
and ReleaseCapture calls. Clear-first is needed for reentrancy, but an unclip
failure must not be silently reported as successful cleanup. Route required
native cleanup failures through the existing fault channel without restoring a
stale captured flag or adding retry machinery.

Storage writer close already returns a checked status; medium destroy discards
file-close status. Direct writes already flush, so this is not a reproduced data
loss claim. Expose finalization failure without retrying a CRT stream whose
close has consumed it. This differs deliberately from a failed thread join,
where resources must stay alive. Focus/foreground requests remain best effort;
resource cleanup during an already reported terminal failure must preserve the
primary error rather than cascade duplicate notifications.

### G6 / Medium: cursor phase and text units need a separate small consistency fix

The actual Window timer schedules the next deadline from dispatch time. An
injected clock sequence 260,500,750,1000 ms produced only three flips: the first
late tick moved the deadline to 510 and skipped the 500 tick. Advance from the
existing phase deadline, not now; no second timer or catch-up loop is needed.
This is a deterministic timing probe, not a measurement of host timer precision.

Text rendering/frame sizing use a fixed 16-row cell, whereas cursor geometry
uses frame font_height. The producer fix below corrects the reported top stripe,
but does not prove 8/14-row glyph layout equivalence. Preserve fixed glyph storage
stride separately from active glyph height; validate a complete glyph plus cursor
in the same rendered cell before changing the common renderer. Do not patch only
another rectangle formula.

## Verification and limits

- The new producer-to-geometry test covers 42 combinations (three font heights,
  seven percentages, two scales). It calls the real runtime helper and real
  rectangle function. It is not a pixel-font-layout test for G6.
- Actual runtime smoke checks produced cursor bounds against font height.
- Both executable builds completed. Full CTest: x64 55/56, x86 55/56.
  Both failures are package stage 5: the owner's preserved display=window INI
  keeps Console as monitor, but the fixture unconditionally waits there for
  C:\\>. This is a configuration-dependent test gap, not proof of a runtime
  regression or a full package pass. Do not change owner INI to force green.
- Existing fault tests prove their specified injections, not absence of G1-G5.
- No lib ABI or MVDM change was made under the cursor repair. The global repair
  should be admitted as one contract-led work package: G1-G4 ownership/observation
  first, G5 side-effect completion next, G6 visual-unit consistency separately.
  Exit tests must inject each operation failure before/after acceptance and
  assert app observation, worker liveness and retained/disposed ownership together.

Search/review inventory: `rg --files src/lib`, public `*_interface.h` signatures,
and every native call family returning wait/signal/lock/read/write/draw/close
results in the 36 C files, followed through caller disposal and callback sinks.
Probes: actual worker ready-signal failure and injected timer deadlines; existing
shutdown-failure fixture mode 3 supplies the persistent-wake evidence. Optional
focus, teardown after a primary error and unsupported Linux leaf implementations
are not mislabeled as normal-path faults. No claim of exhaustive OS fault
injection or actual Linux runtime validation is made.

## Executor delivery accounting

Production: one C path (`app/runtime.c`), +13/-3. Tests: two C paths,
+29/-0 including the new 26-line fixture. Build wiring: one path, +9/-0.
Total code/build +51/-3, net +48; no lib production or public header changes.
Method: `git diff --numstat 77d3d7f` plus new fixture line count before staging.
The runtime is the sole percentage-to-scanline owner; no parallel rendering path.
Standalone library gates 3/3 and documentation governance passed. Both EXEs
are delivered for manual cursor testing, not as a clean full-regression claim.
S17/T55 remain open; P delivery does not imply owner acceptance.
