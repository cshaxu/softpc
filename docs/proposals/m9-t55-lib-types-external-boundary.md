# M9 T55 — Types Vocabulary and Component Platform Boundaries

## S4 admission: single input and lifetime paths

Owner: “按照这些准入新的S任务修复以上7条反馈意见。” Baseline
`5ea133c`. This admits S4, not T55 closure. Freeze these seven dispositions:

| Item | Sole owner / implementation | Proof |
| --- | --- | --- |
| 1 | Window uses its existing final normalized delivery filter for every event; remove duplicate sink filtering, retain local capture/blink guards. | Frozen Ctrl then X sends only close; hotkeys still work. |
| 2 | ui-base matcher changes keyboard prefixes only for key/text sequence; mouse/X pass without flushing. Keyboard order is retained, keyboard/mouse interleaving is not buffered. | Ctrl/mouse/Alt/P complete suppression; mismatch/release replay. |
| 3 | Partial sink failure clears pending state, closes input/admission, wakes worker and uses its single retirement path; no retry. | Rejected second replay emits first make once, no later ordinary events, fault/retirement once. |
| 4 | STOP and fault share mailbox admission closure; frame and ordinary control reject after closure, STOP idempotent. Accepted controls drain FIFO up to STOP. | Concurrent admission barrier, STOP ordering, failed worker rejection. |
| 5 | console event/output gates and host broker transaction use component-private blocking locks. Keep lock order and quiescence; callbacks cannot reenter binding/destruction. | Real contenders block behind output/callback/replace barriers; native backend output critical section unchanged. |
| 6 | Storage embeds stream state in the writer/file owner instead of separate pointer-only allocations. Public API and platform-specific open/lock/seek behavior remain. | Binary writer/read/medium tests, allocation/cleanup review. |
| 7 | Add TODO at linear overlay page lookup; defer algorithm by owner decision. | Only comment changes in medium, matching deferred debt entry. |

No new public lock API, dependency edge, app policy, mouse scale, MVDM/media/INI
change. Same-shape blocking helpers live inside console/host platform owners;
types supplies external declarations only. Full x64/x86, strict library,
manifest, DAG and documentation checks precede complete P push. Review actual
diff after push; deliver packages and leave T55 open.

### S4 P1 executor verification

All seven dispositions are implemented. The seventh is deliberately only an
O(n) lookup TODO and its long-term ledger entry, not an algorithm change.
The Window's old ordinary/lifecycle delivery wrappers are removed; the existing
final normalized sink is the sole frozen event filter. Local capture and blink
guards remain because they control native behavior rather than sink delivery.
Source retirement remains the base worker's terminal event after quiescence.

The matcher preserves keyboard ordering while mouse/close bypass pending-key
replay. A rejected replay clears its pending state and fails the component;
no later ordinary event can retry the partially delivered stream. Worker
retirement reports the stored failure and attempts SOURCE_RETIRED once.
STOP closes both mailbox admission paths under the same short lock without
skipping controls accepted before STOP. Fault closure uses that same boundary.

Console output and callback gates use private blocking mutexes; the host
transaction lock uses its backend critical section. The short field/mailbox
locks remain spin locks because they do not cover native I/O or callbacks.
Lock scopes and replacement cleanup ordering are preserved. Callback reentry
into binding/destruction remains forbidden. Linux logical Console mutexes use
pthread; the unsupported Linux host Console backend stays explicitly unsupported.
No Linux desktop execution is claimed.

Storage writer now embeds the stream-bearing file; medium file owns one stream
object. Separate file-platform and writer-file allocations are removed. Binary
open modes, platform locking/seeking and public writer/file ABI are unchanged.

Focused proof includes frozen Ctrl/close/release, mouse-interleaved CAP and all
breaks, rejected second replay with no duplicate make, actual Window worker
failure/retirement, FIFO controls before STOP, concurrent publish/STOP, and
proven contending output/callback/replace threads held behind completion barriers.
No Sleep-based scheduling assertion is used. Existing binary writer and media
tests cover the flattened storage path.

Fresh final tests: x64 48/48 (28.34 s), x86 48/48 (71.33 s), strict standalone
library 3/3. Both package EXEs rebuilt. Strict library build uses C11 with
`-Wall -Wextra -Wpedantic -Werror`; 18 public headers passed independent C17
compile checks. Manifest, exact DAG, Linux build contract and documentation
gates pass. An initial documentation test exposed missing S3 history; the
truthful S3 handoff was added, then both complete suites were rerun.

Accounting from `git diff --cached --numstat --no-renames` against `5ea133c`:
22 production C/H paths +235/-173 (net +62); five test C paths +263/-13
(net +250); four CMake/gate paths +9/-3 (net +6). Documentation, manifest and
two EXEs are excluded from those code totals. No app, standalone host, MVDM,
guest media, INI or Console mouse conversion changed. No owned temporary
diagnostic directory remains; reusable build caches are retained.

Similar-issue sweep used `rg` over Window `input_sink`/emit/accepting paths,
matcher pending replay, component stopping/failure/retire and mailbox admission;
Console event/output atomic gates and host transaction entry/exit; and storage
allocate/release/open/close paths. Each hit belongs to the seven dispositions
above: one final Window sink, one worker failure path, one admission boundary,
three replaced long-held gates, and one stream owner. No extra repair is
silently added. Overlay's only medium diff is its approved TODO.

## S3 P5 admission: correctness and boundary convergence

Original owner requests: “以上。写入设计/任务文档，然后开始清理”,
“我有一些疑问的你先解答一下，然后可以执行/”, and
“照此办理，请你执行。” Baseline `3a71b50`; continue S3; keep T55 open.
Freeze these twelve approved classes and all callers of their changed contracts.

| Item | Change / sole owner | Focused proof |
| --- | --- | --- |
| 1 | ui-base mailbox accumulates unconsumed dirty bounds with latest complete pixels; incompatible mode/size/palette forces full refresh. | Skipped frames, consume reset, incompatible images. |
| 2 | Window worker has one post-start cleanup/retirement path, separate from startup failure. | STOP, render/wait failure, disappearance; retire once and join. |
| 3 | Console unexpected I/O failures follow logical events and UI failure sink; monitor forwards to control for fault termination, not prompt rearm. Cancellation/NOT_CURRENT are expected. | Reader/output failure and expected handoff; no product decision in lib. |
| 4 | Broker transaction lock includes old binding cleanup after backend output unlock. | A-to-B cleanup completes before B-to-A can enter; no lock inversion. |
| 5 | Window retains integer scaling remainders, reset on capture/release/ratio change. | Positive/negative/alternating/resize; integer ABI and Console X8/Y16 unchanged. |
| 6 | ui-base delivers valid Unicode scalars through TEXT and clears malformed surrogate state. | Supplementary, malformed/recovery, rejected sink. |
| 7 | Window-only message/key-state decoding returns to Window; shared normalizer stays ui-base with same-shape selected-platform API. | All callers, RDP/scan/chord regression, explicit Linux unsupported cases. |
| 8 | Actual consumers declare OS linkage; types retains declarations only. | Direct link contracts; storage does not inherit UI libraries. |
| 9 | storage root shares identical CRT stream operations; platform open/lock/seek differences remain selected implementations. | Storage smoke and both implementation shapes. |
| 10 | Remove duplicate Window component header. | Include sweep and independent headers. |
| 11 | Delete unused suppression field, empty discard-prepare and obsolete event/sink aliases after migrating callers. | Zero obsolete references and regression. |
| 12 | Enforce exact permitted include and CMake component DAG. | Every allowed/forbidden edge and platform/traversal exceptions. |

Execution order: shared frame/input fixes; worker and Console transactions;
platform/storage consolidation and dependency gates; full diff review and fresh
strict/x64/x86 build/tests. Every disposition needs focused evidence before
delivery; no silent deferral. Existing Linux paths remain connected, unavailable
UI parity stays explicit; no claim of Linux desktop execution. No MVDM, media,
INI, Console mouse conversion or product routing change. App changes only
propagate faults and adopt renamed contracts. Refresh only the two package EXEs;
commit/push complete delivery, account changed paths and leave T55 open. Ignored
build/t55-lib-audit probes are bounded diagnostics to remove after permanent
tests replace them.

### P5 executor verification

All twelve ledger items are implemented without deferral. Shared damage uses
latest complete pixels plus the enclosing pending rectangle, never an extra
pixel queue. Window mouse uses signed integer remainders, not floating-point
events; Console X8/Y16 is unchanged. The neutral keyboard path preserves the
existing Windows layout/scan recovery and hotkey sequence; Linux terminal key
mapping remains connected and text without a physical mapping uses TEXT.
Window-only message flags/key-state queries are now private Window helpers.

Worker proofs execute the actual production worker with controlled barriers:
STOP, wake failure, surface allocation failure, WM_QUIT, unexpected destruction,
retirement rejection and fault plus retirement rejection. Retirement is once,
exit failure is reported once, and destroy joins. Console tests cover reader
failure, output failure, expected NOT_CURRENT and callback/detach ordering.
The host reader reports generation-checked IO_FAILURE; ui-console retires via
its failure path. Monitor posts a copied control failure and the app terminates
the failed path without arming another prompt. Failure cleanup now destroys
runtime before monitor/control queue, keeping callback targets alive until join.
This is failure plumbing, not a lifecycle/routing policy change.

The broker test pauses A-to-B during old sink cleanup and proves the reverse
thread actually encounters the held transaction lock; only after cleanup can
B-to-A install A again. Its final output write succeeds. Static gates test all
49 include and 49 CMake edges, uppercase commands, computed/angle includes,
component-private platforms and traversal. OS libraries are attached to actual
consumers, not types. Repeated CRT stream bodies, obsolete event/sink aliases,
unused suppression state, empty prepare rollback and duplicate Window header
are removed, with every production caller migrated in this P.

Fresh full tests: x64 46/46 (53.10 s), x86 46/46 (69.26 s), including package
smoke. A subsequent test-only atomic cleanup of the broker probe passed again
at both widths. Strict C11 `-Wall -Wextra -Wpedantic -Werror` library build and
3/3 standalone checks pass; all 18 public/support interface headers independently
compile as strict C17. Manifest and documentation gates pass. Linux input/host contract fakes
are not evidence of Linux desktop execution; manual package acceptance is pending.

During verification, strict compilation caught a neutral key narrowed to u16
and a missing scalar vocabulary include; both were corrected before delivery.
The first focused run passed 7/9: the new Console IO_FAILURE kind was missing
from event validation. Adding its validation case made both tests pass, followed
by the full passes above. No failed verification is treated as acceptance.

Similar-issue sweep: inspect all worker STOP/fault/destruction exits, all Console
read/write sinks, both broker terminal failures and success cleanup; search
`ui_event`, `ui_event_sink`, `suppressed_virtual_key`, `discard_prepare` and old
`ui_win32_keyboard` names across src/test (zero obsolete references). Review all
changed input callers and selected platform operations; exact DAG negative
fixtures permanently guard the mechanically detectable boundary errors.

Accounting versus `3a71b50`, no rename detection, excluding documents/manifest/
EXEs: library C/H 30 paths +465/-438 (net +27); app 11 paths +43/-27 (net +16);
tests 14 paths +470/-49 (net +421); build/gates five paths +94/-14 (net +80).
No MVDM, media or user INI changes. The two package EXEs are refreshed.
S3 awaits owner inspection; this delivery does not close T55.

### P6 post-push review

Reviewer inspected the actual `3a71b50..996e4c6` changed paths and critical
worker, broker, input, mailbox and app failure diffs against the twelve-item
ledger, then checked the verification evidence and protected-path diff.
No unresolved delivery blocker was found. The generated standalone flags are
C11 with all four strict warning switches; the independent header probe used
C17. This evidence correction changes documentation only. Both package EXEs
belong to P5, and no MVDM/media/INI path changed. Owner acceptance remains
pending; neither S3 nor T55 is closed by this review.

## S3 P4 admission: component-private platform directories

Original owner request: “很好 所以所有的lib组件 win32和linux都只是组件内部的实现，组件外不能访问 包括隔壁其他的lib组件 对吗”,
then “没错 你说的完全正确 请开始实现”. Baseline `a6bf81f`.
Continue S3, keep T55 open. The frozen universe is all library C/H include
edges and all consumers of the two current Windows input/action headers.

- Move their actual copied-value declarations to ui-base root support
  interfaces with git mv; update every caller. Keep implementation, symbols,
  state and behavior unchanged, without forwarding headers or a second path.
- Platform-specific helper contracts remain explicitly Windows-specific; they
  are not the same-shape operations called by platform-neutral parent sources.
  Do not invent Linux implementations for Windows record decoding. Existing
  Linux mappings and the 41 same-shape parent operations remain unchanged.
- Enforce root-only cross-component interfaces and forbid importing platform
  implementation headers from parent sources or other components. Only types
  platform vocabulary is shared, from the matching platform implementation.
- Add positive and negative boundary probes, including a misleading interface
  suffix, root forwarding, sibling platform access and types exceptions.
- Refresh current design/READMEs and manifest; build both widths, run full
  regression, strict standalone tests and review the exact body-preserving diff.

No new API behavior, product policy, Linux parity, MVDM, media or INI changes.
The owner-provided command.c formatting is preserved unchanged and ships in
this P under the owner's additional instruction “请一起提交 不留尾巴”.
Completion requires all five dispositions, a complete pushed P, and package
links for owner inspection; it does not close T55.

### P4 verification

All five dispositions are implemented. The only library C-body differences
from `a6bf81f` are include substitutions in four files; exact comparisons after
those substitutions pass. Two declarations moved with git mv; guards and a
contract comment changed, not symbols, struct layout or signatures. Existing
Linux code, types, parent operation contracts and CMake selection are unchanged.
The complete C/H include sweep has no remaining cross-component platform
include. Tests deliberately retain forbidden path strings as negative probes.

The gate now requires canonical paths, rejects public headers in platform
directories, root forwarding, cross-component platform includes even with an
interface suffix, opposite-platform includes and relative traversal. Positive
controls cover root support access, own-platform helpers and matching types
vocabulary. Application production is separately barred from leaf-support APIs.
The two relocated headers independently pass C17 strict syntax compilation.

Both package EXEs were rebuilt. Full x64: 42/42 in 21.47 s; full x86: 42/42
in 27.34 s, serial including package smoke. Strict library build and 3/3
standalone checks, manifest, documentation governance and diff checks pass.
These are Windows tests, not a claim of Linux desktop parity.

Accounting (`git diff --numstat --no-renames a6bf81f`, excluding documentation,
manifest and EXEs): library C/H eight paths +59/-55, net +4 (the contract
comment); tests/support four paths +34/-5, net +29; library static gate one
path +18/-1, net +17. Separately, owner-provided command.c formatting is one
path +186/-61, net +125, included unchanged by explicit owner request.
No new compiled code, runtime layer, API route or platform behavior was added.
S3 remains for owner inspection; T55 is not closed.

## S3 P3 admission: nine-item audit repair

Owner request: “开始”, approving the preceding whole-library audit. Baseline
`d236757`. Continue S3; do not close T55. Freeze these nine dispositions before
implementation; completion requires each focused proof plus dual-width full
regression, strict build, manifest, actual diff review and pushed clean delivery.

| Finding | Repair and proof |
| --- | --- |
| UINT64_MAX bypass | types alias, both consumers migrated, negative gate probe. |
| Alias-derived gate blind spot | Independent finite external vocabulary patterns and unwrapped negative probes. |
| Window-only capture abstraction | Store its boolean in Window mouse state; delete unused shared abstraction, preserve capture behavior. |
| Unused UI color helper | Remove helper and build references after full caller sweep. |
| Shared header names | Cross-component contracts use `_interface.h`; own implementation headers remain unqualified. Enforce direct includes. |
| Repeated chord leaks releases | Preserve outstanding suppressed keys, suppress repeats, bound capacity; deterministic repeated-chord and mismatch tests. |
| Console wake fault skips retirement | One detach/retire exit for STOP and wake failure; failure notification, callback barrier tests. |
| Linux waits poll/sleep | Real condition waits, monotonic deadlines, safe initialization unwinding; deterministic fake-platform proof. |
| Missing Linux cooked-line operation | Same-signature UNSUPPORTED implementation and link proof. |

No new component or types implementation, product callback, MVDM/media/INI
change, or mouse scaling change. Linux existing behavior is repaired, not
expanded into UI parity. Fake-platform evidence must not be presented as a
real Linux desktop/runtime test. Relevant Linux debt is now admitted here.

Verification discovery: the release-build assertion list omitted the existing
leaf-control-capacity and control-reconciler-integration tests. Add both and the
new Linux test to that list, so full regression executes their operations
instead of compiling out assertions. This is test proof repair, not a product
state-machine change. The Console startup review also found that starting the
worker before installing the input sink could reattach it after an immediate
worker fault; install it first and detach on thread-create failure.
The shared-mailbox review found its enqueue error return rereading
`stop_queued` after unlocking. Snapshot the result under the existing lock;
capacity/STOP semantics are unchanged and covered by the enabled tests.
The x86 repeated full run exposed a stale negative-probe file in the types
layout self-test: its initial positive control read `UI_WIN32_KEY_CONTROL`
from a prior probe. Explicitly remove the three owned probe files at entry;
the production gate must still reject every deliberate negative probe.

### P3 executor verification

All nine dispositions are implemented. The resulting library contains 71 C/H
files, including 41 headers. No new production file or execution layer was
added: six contracts were renamed with `git mv`, and Window-only capture plus
unused UI color helpers were removed. Types remains header-only. The mouse
capture field substitution preserves the exact Win32 calls/order and no raw
Console coordinate conversion changed. No app, standalone host, MVDM, INI or
media file changed.

Evidence by ledger member:

1. `LIB_UINT64_MAX` owns the external constant; both production uses migrated.
2. The layout gate adds independent integer/SDK/POSIX/CRT families and
   cross-component include validation. Negative tests cover unwrapped tokens
   and calls; both widths passed three consecutive self-test runs. It remains
   a finite static check, supplemented by the whole C/H call/include sweep.
3. `ui_win32_mouse` alone owns the boolean capture field; the old shared
   capture header has no remaining production caller.
4. `ui_win32_colorref_from_rgb` and its CMake entry are gone; the separate
   host Console color path remains untouched.
5. Shared binding, worker, mailbox, wake and Windows input/action headers now
   have `_interface.h` names; application access to these leaf-support
   contracts is rejected by the source-boundary gate.
6. The repeated Ctrl/Alt/P regression failed against the baseline matcher,
   then passed with preserved suppression state. Tests include auto-repeat,
   trigger re-press, modifier repeats, final breaks, mismatch replay, and
   capacity rejection without overwriting existing entries.
7. The actual Console worker runs under real Windows threads with a controlled
   wake result. Both STOP and fault wait for an in-flight callback, retire once,
   join before destroy returns, and reject later input on a retained logical
   Console. Fault reports IO_ERROR. No new lifecycle callback was introduced.
8. Actual Linux host/mailbox sources run against deterministic POSIX fakes:
   all initialization failures unwind, spurious wakes reuse one monotonic
   deadline, infinite waits wake, event reset semantics hold, wait/clock errors
   report failure, and interrupted sleep uses the remaining interval. The wait
   paths make zero sleep calls. Platform sources also pass strict syntax checks
   against the available Windows pthread headers; this is not Linux execution.
9. The Linux cooked-line placeholder is linked and returns UNSUPPORTED with
   the same signature as its Windows implementation.

Final regression: x64 42/42 (17.02 s), x86 42/42 (20.33 s), serial and including
both fixed-package smokes. Strict library build and standalone 3/3 pass; all
41 headers independently pass C17 Wall/Wextra/Wpedantic/Werror on the available
compiler. Documentation governance, manifest and diff checks pass. Earlier
x86 runs failed only the stale-fixture self-test (41/42); that failed evidence
is retained here, not replaced by a claim that every run passed. Linux desktop
execution/parity remains explicitly unverified and outside this delivery.

Changed-path accounting uses `git diff --numstat --no-renames d236757`:
production C/H 37 paths, +369/-366, net +3; tests/support 12 paths,
+281/-13, net +268; build/static-check 3 paths, +28/-3, net +25.
Documentation, manifest and EXEs are excluded. The increase is predominantly
tests, not new abstraction. T55 remains open and S3 awaits owner inspection.

## S3 follow-up admission and convergence ledger

Original request: “准入使用当前或者新的S任务修复以上所有问题，完成后工作区清理干净并让我检查。”
Continue S3 from `2f54899`; do not close T55. The frozen universe is every
library C/H external vocabulary consumer, plus the nine audit classes below.
Each requires implementation, direct-call sweep and focused/full proof before
delivery. Types wraps original external definitions even for a single consumer;
component-defined behavior and contracts stay in their owning component.

| Audit class | Required disposition |
| --- | --- |
| Include-only types shells | Replace with actually consumed external aliases; no duplicate SDK definitions. |
| Windows host and UI SDK use | Adopt types vocabulary without moving/reimplementing worker behavior. |
| Linux pthread/time use | Adopt existing wrappers and fill missing ones; retain platform ownership. |
| Storage CRT/OS use | Wrap original file definitions in types; storage retains file policy. |
| Copied UI Windows key constants | SDK-backed definitions in types; neutral mapping stays ui-base. |
| VkKeyScan modifier mismatch | Translate raw SHIFT=1/CTRL=2/ALT=4 to UI masks; deterministic tests. |
| Console raw numeric contract | Document raw platform values at the Console boundary; UI alone normalizes. |
| Root policy leakage | Move NOT_CURRENT to Console; make text length direct CRT and audit null callers. |
| Weak static proof | Reject raw external tokens outside types, with negative tests, not only includes. |

No new types implementation files, component edges, product policy, MVDM or
media changes. Refresh both EXEs, test both widths, review the actual diff,
commit/push all changes and leave a clean worktree for owner inspection.

### S3 P2 result

All nine audit classes above are addressed. The complete library C/H universe
is 74 files (73 at P2 entry plus the shared Windows scalar declaration header).
Twenty-one non-types C/H consumers changed; component sources retain their
original operations and sequencing. Windows/POSIX/CRT types, calls and constants
now use types-owned aliases. UI virtual keys bind SDK values rather than a
second numeric table. Types remains header-only/INTERFACE; no component DAG
or SoftPC product path changes.

The only executable semantic correction is the UI-owned conversion of
VkKeyScan's high-byte SHIFT=1, CONTROL=2, ALT=4 into the existing UI masks.
The deterministic test compiles the actual input implementation with a fake
layout query and covers all eight combinations, emitted make/break symmetry,
and failed-query output preservation. Existing hotkey/input tests still pass.
Console's raw platform key/button/cell-position contract is documented without
moving event types or UI normalization to types/host. NOT_CURRENT retains value
6 but is declared by Console. Both production text-length callers already
reject null before calling; the root wrapper now matches strlen exactly.

The static gate derives raw tokens from external aliases and rejects their use
outside types. Eight new negative probes cover SDK/CRT/POSIX types and calls,
SDK key constants and the removed UI key table; all original probes remain.
Manual call-site and type sweeps supplement this finite-token gate: it is not
a claim that a regex understands arbitrary future C code. No speculative unused
wrappers were retained.

Verification: x64 41/41 (42.53 seconds), x86 41/41 (58.38 seconds), each serial
and including fixed-package smoke; strict library build and 3/3 standalone
checks; ten common/Windows headers independently compile with C17 and
Wall/Wextra/Wpedantic/Werror. Linux platform aliases are source-reviewed but
not executed on Linux. Existing Linux wait-policy debt described below is not
claimed repaired by this vocabulary migration. Documentation and diff gates
pass. Both package EXEs are rebuilt. T55 remains open for owner review.

After removing unused new aliases, both widths were rebuilt and the full suites
repeated: x64 41/41 in 42.47 seconds, x86 41/41 in 53.60 seconds; strict library
3/3 repeated. The pre-existing Linux execution gaps are explicitly retained in
[TODO](../states/TODO.md), not silently certified by these Windows results.

P2 changed-path accounting (`git diff --numstat --no-renames 2f54899`, including
the added scalar header): production C/H 32 paths, +1038/-684, net +354;
tests 3 paths, +59/-9, net +50; static gate 1 path, +33/-0. Documentation,
manifest and package binaries are excluded. Added declarations replace direct
SDK usage, not parallel production execution. The original component workers,
storage operations, Console broker and UI normalizer remain the sole owners.

## Objective

Make `lib/types` the header-only shared vocabulary for C-runtime, SDK, POSIX,
and compiler-atomic declarations. Preserve every observable library behavior
while moving all compiled platform behavior into the component that owns its
meaning.

`types` is not a runtime component. It has no `.c` files, worker, handle
ownership, state machine, product policy, or component dependency. Its typed
inline façades are one-to-one external vocabulary only.

## Final boundary

Each component has a platform-neutral base source and a selected platform
source with one identical component-private operation shape:

```text
host/clock.c       -> host_clock_platform_counter(...)
host/sync.c        -> host_sync_platform_*(...)
storage/file.c     -> storage_file_platform_*(...)
ui-base/mailbox.c  -> ui_mailbox_wake_*(...)
```

The `win32` and `linux` implementations of each shape are peers. CMake selects
exactly one. A base source does not use platform preprocessor branches, raw SDK
types, or platform function declarations. A platform source may implement only
its owning component's internal shape; it cannot introduce a cross-component
runtime, dependency, or product policy.

`types` centralizes the external header and typed wrapper vocabulary used by
those sources. It does not normalize UI keys, define a storage file object,
own events/tasks, or implement Console/Window behavior. In particular:

- `storage` owns file access, exact-transfer and ownership policy;
- `host` owns synchronization, task/cancellation, clock and Console policy;
- `ui-base` owns event normalization, hotkey matching and mailbox meaning;
- `ui-window` and `ui-console` own their lifecycle and rendering behavior.

## Required changes

- Delete every `src/lib/types/**/*.c`; make CMake target `types` INTERFACE.
- Move compiled raw file work to `storage/win32/file.c` and
  `storage/linux/file.c`, behind `storage_file_platform_*`.
- Move compiled synchronization/task work to `host/win32/sync.c` and
  `host/linux/sync.c`, behind `host_sync_platform_*`.
- Keep mailbox wake work in the existing `ui-base/win32` and `ui-base/linux`
  sources, with the identical `ui_mailbox_wake_*` contract.
- Keep platform clock work in `host/win32/clock.c` and
  `host/linux/clock.c` behind `host_clock_platform_counter`.
- Move the Win32-only modifier query to `ui-base/win32/actions.c`; do not
  retain a generic UI source which includes a platform adapter.
- Move text-to-native-key layout interpretation into `ui-base/win32/input.c`.
  `types` exposes only raw SDK facts, never UI input mapping policy.
- Centralize external SDK/CRT/POSIX declaration headers under `types`; the
  component source consumes that vocabulary and exposes no native type through
  any public `*_interface.h`.

## Invariants

- Implementation filenames and identifiers describe their operation, without
  `native`, `internal`, or `private` qualifiers. Platform vocabulary lives in
  `types/win32/` or `types/linux/`; `host/sync_interface.h` is the public sync
  contract and `host/sync.h` declares the selected platform operations.
  UI worker start/join/state names describe actual lifecycle responsibilities.
  This naming pass changes no control flow or synchronization behavior.

- Public interfaces expose only `lib_*` copied values and opaque component
  objects; no `FILE`, `HANDLE`, `HWND`, `DWORD`, or `pthread_*` value leaks.
- The component DAG remains:
  `types -> console + host + storage + ui-base + ui-window + ui-console`,
  `console -> host + ui-console`, and `ui-base -> ui-window + ui-console`.
- No MVDM source, guest media, `softpc.ini`, or SoftPC product behavior is
  changed.
- Existing x86/x64 observable behavior is retained.

## Verification

- Static gate proves `types` has no `.c` and CMake declares it as an INTERFACE
  target.
- Static audit proves every neutral base delegates platform work through its
  component-private same-shape operation, rather than a platform `#ifdef`.
- Public interface audit rejects native SDK/POSIX types.
- Fresh strict library, x86/x64 full CTest, package smoke, manifest and
  governance checks pass after the complete migration.

## P16 naming audit

The owner requested removal of ambiguous `native`, `internal`, and `private`
names. The bounded universe is library filenames and C identifiers, together
with their test references. Both remaining prefixed headers were renamed;
all matching identifier qualifiers now name their actual operation or value.
Comments describing OS behavior are prose, not another API layer. No control
flow changed. The source-boundary gate rejects recurrence in paths and code
identifiers, excluding comments. x64 and x86 full CTest each passed 38/38;
the strict library build and its two checks passed. Both package EXEs were
rebuilt. T55 S2 remains active pending owner acceptance.

## P17 file declaration header naming

Rename the sole `*_runtime` header, `types/win32_runtime.h`, to
`types/win32/file.h` and update its only consumer, `storage/win32/file.c`.
This is a declaration-header relocation, not a completed typed-wrapper
migration: the external declarations remain unchanged and file implementation
stays in storage. No runtime layer is introduced. T55 and S2 remain active
by explicit owner direction.

Verification: no `*_runtime` path or old header reference remains in the
library; x64 and x86 rebuilt and each passed 38/38 CTest, including package
smoke. Strict library build, 2/2 standalone checks, documentation governance
and diff checks passed. Production changes are limited to the header move,
include guard and its one include site; no executable logic changed.

## S3 admission: types platform layout

Original owner request: “同意，按照这个准入一个S任务清理lib types，要求符合收口标准后供我检查。”
Baseline: `1b368d4`, plus the owner's formatting-only atomic.h changes.
S3 supersedes S2's unfinished layout/ownership cleanup; it does not certify
the broader S2 external-vocabulary migration or close T55.

The frozen source universe is all seven baseline types headers and every
direct consumer. Disposition requires both a source review and an executable
layout gate, plus focused tests where function bodies move.

| Baseline header | Disposition / owner |
| --- | --- |
| types_interface.h | Retain common scalar/status and C-runtime wrappers; no OS selection. |
| atomic.h | Retain compiler-only MSVC/C atomics selection and owner formatting. |
| win32.h | Remove umbrella; explicit purpose headers under types/win32. |
| posix.h | Remove umbrella; explicit purpose headers under types/linux. |
| win32/file.h | Keep Windows file declarations; common stdio declarations move to types/file.h. |
| clock.h | Move combined counter validation/conversion to host/{win32,linux}/clock.c; only raw clock declarations/wrappers in platform types headers. |
| input.h | Move modifier interpretation to ui-base/win32/actions.c; direct key-state/layout wrappers in types/win32/input.h; remove unused Linux zero-result fallback. |
| README.md (documentation companion, not a header) | Describe the one platform layout and compiler exception. |

Only actually consumed purpose headers are created. No new platform runtime,
implementation C file in types, duplicated registry, state owner or generic
platform dispatcher is allowed. CMake continues selecting the owning
component's same-shape platform implementation. Platform headers themselves
do not need OS-selection branches. Windows SDK declarations keep their exact
external signatures; this task does not manufacture a second OS API or claim
that header organization alone completes S2's entire wrapper audit.

Verification: layout/forbidden-include checks, header-only and DAG gates,
counter/atomic smoke, existing keyboard coverage, strict library compilation,
x86/x64 full CTest and package smoke, manifest and documentation governance.
Existing Linux function bodies are preserved (clock is relocated verbatim).
Linux execution coverage must be reported separately from Windows results.
No MVDM, app policy, INI or media change. S3 is delivered for owner inspection;
T55 remains open.

## S3 P1 verification and review

All seven baseline header dispositions are implemented. The resulting types
tree has 12 headers: three common, six Windows and three Linux. Every old
umbrella/dispatcher reference is removed from production and tests. The
layout gate rejects OS branching, platform imports from common headers,
non-C common includes, platform control flow and external include bypasses;
its self-test deliberately submits seven invalid forms and valid controls.

Focused proof exercises the actual moved Windows clock/actions bodies with
controlled external queries: null arguments do not query, query failures and
invalid counters preserve outputs, valid counters are copied, all eight
modifier combinations retain their original meaning. Common C/atomic and
real public monotonic-clock calls pass at both widths. All nine common/Windows
types headers compile independently with strict GCC warnings. The existing
keyboard/hotkey tests remain in the full suite.

- x64 full CTest: 41/41 passed.
- x86 simultaneous parallel run: 40/41; package smoke failed at stage 7
  (CAP sent, cooked SoftPC prompt not observed within its existing deadline).
  No source or timeout was changed in response. Isolated package smoke then
  passed three consecutive runs (5.52, 5.35, 5.55 seconds), followed by a full
  serial x86 41/41 pass. The concurrent failure is recorded, not claimed fixed
  or conclusively attributed to scheduling. Owner review remains required.
- Strict library build and standalone CTest: 3/3 passed, including manifest,
  types layout and the Linux CMake contract. The latter is not a Linux runtime
  test. WSL is not installed; no Linux execution claim is made.
- Documentation governance, source boundary and diff checks passed.

Changed-path accounting uses `git diff --numstat --no-renames 1b368d4`:
production C/H is 32 paths, +146/-132 (net +14); test/support is 5 paths,
+156/-3 (net +153); build/check CMake is 3 paths, +85/-0. Documentation,
manifest and package EXEs are excluded. The atomic formatting was present on
entry and retained; only its old BASE include guard was renamed by S3.
storage/file.h gained its missing direct types include, eliminating implicit
include-order dependence without changing its contract.

Reviewer compared clock/error behavior and modifier bits against `1b368d4`;
other Windows and Linux component changes are include substitutions, not
worker/reader/mailbox rewrites. MVDM, app, standalone host, INI and media have
no changes. Ownership remains host clock and ui-base input, with no types
platform control flow. Delivery is for owner inspection, not T55 closure.

The remaining T55 whole-library review is still required. In particular, the
pre-existing Linux mailbox wait currently uses a timed sleep rather than
waiting on its signaled condition; Linux host sync also polls. Those bodies
were not introduced or rewritten by S3. Their behavior and S2's wider raw-call
wrapper coverage are not certified by this layout-only delivery.
