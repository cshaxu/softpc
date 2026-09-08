# M9 T42: Console-object And UX Recomposition

## Purpose

Implement the owner-approved Console-object architecture after T41's completed
NXVM-library import/binding baseline. The work replaces the unreliable
SoftPC-specific Console/frontend handoff with one generic `base`/`host`/`ux`
candidate, composed only by SoftPC. NXVM must adopt the resulting generic
library corpus byte-identically; SoftPC then imports that adopted corpus
byte-identically before T42 closes.

Current product behavior is owned by [Product UX](../design/UI.md); component
boundaries, lifecycle ordering, generations, and threading are owned by
[System Architecture](../design/ARCHITECTURE.md). The retained [T41 proposal](m9-nxvm-library-adoption-and-softpc-binding.md)
and [decision record](../etc/evidence/softpc/m9-t41-s5-console-design-review-record-zh.md)
are background, not competing authority.

## Implementation Scope

1. Add the generic logical `lib_console` object to `base` and the one-current-
   object native Console broker to `host`, including raw/cooked modes,
   transactional replacement, output ownership, and stale-event suppression.
2. Replace the old unified `ux` controller with independent `ux-base`,
   `ux-window`, and `ux-console` components. Each Window/VM-Console component
   has a private mailbox and lifecycle; `ux-console` has an optional logical
   Console object; Window close is only a copied event; no UX component depends
   on `host` or SoftPC.
3. Rework only non-MVDM SoftPC code into the sole policy/reconciler owner:
   derive component existence, current Console object, and VM lifecycle from
   configuration, completed component facts, frame route, and FIFO intent; own monitor
   parsing, `console_control`, DISPLAY policy, CAP semantics, and X policy.
4. Prove the state and ordering matrix with deterministic completion-gated
   fakes before real-thread boundary tests. Build/test both package widths and
   obtain owner runtime acceptance.
5. Submit the generic `src/lib` candidate to NXVM, require its byte-identical
   adoption, then import and manifest-verify the adopted corpus in SoftPC.

## Serial Subtasks

### S1 — Decomposition and implementation-boundary audit

Inspect the actual T41 baseline and produce this S plan before any code change.
For every existing Console reader/writer, unified-UX owner, input sink,
lifecycle callback, and relevant test, assign one destination S or explicitly
record it as unaffected. Cross-check against the current UI/architecture
authorities and prove that MVDM is excluded. **Exit:** owner-readable source
inventory, dependencies, and acceptance gates for S2–S8; documentation checks
pass.

### S2 — Base logical Console object

Add only the platform-neutral copied-event/output object in `lib/base`, with
its event-sink, output, lifetime, rejected-line, and current-object result
contracts. No native handles, reader thread, UX component, SoftPC monitor, or
policy may appear here. **Exit:** base-only unit tests prove copied ownership
and API failure behavior; public-header/CMake boundaries are explicit.

### S3 — Host current-Console broker

Build the generic host broker over S2: initial monitor registration, one
Current Console Object, raw/cooked native modes, transactional replacement and
rollback, serialized output, strong references, binding-generation stale-event
suppression, and broker teardown. **Exit:** controllable blocked-reader/
callback tests prove every ordering/failure case without sleeps. UX and SoftPC
remain unchanged.

### S4 — Split UX components and generic registered-hotkey input

Delete the unified `ux` controller and replace it with three flat shared
components:
`ux-base` for copied frame/input values, unified UX-event construction,
reusable private-mailbox mechanics, and source-local generic hotkey matching;
`ux-window` for one Window lifecycle; and `ux-console` for one VM
Console lifecycle. Window and Console have independent Win32/Linux code and
independent mailboxes/workers. `ux-console` creates its logical Console object
but never opens or registers native Console I/O. The component DAG permits
`ux-window` and `ux-console` to depend directly on both `base` and `ux-base`;
neither may depend on `host`, SoftPC, or the other UX leaf.

SoftPC supplies each component a copied registration table of `{chord,
identifier}` records. Matching consumes a registered chord and enqueues only
`UX_HOTKEY(identifier)` at SoftPC's queue entry; lib executes no product action.
The generic matcher buffers only possible chord prefixes per component instance,
flushes unmatched input in order, and cannot combine Window/Console keys.
Every `ux_input_event` carries its source component handle solely for tracing
and safe lifetime handling, never product dispatch; monitor lines are separate,
handle-free `monitor_input_event` payloads in the same SoftPC FIFO. Window X
likewise becomes only an event. **Exit:** controllable-runner tests
prove independent creation/destruction, logical Console availability, separate
frame mailboxes, registered-chord suppression/mismatch flush, source isolation,
Window-close delivery, and permanent-retirement input reset. To remove the
second production route, S4 also mechanically replaces the old app-facing
`ux_presenter`/`ux_run` calls with the new component calls. That replacement
does not choose product state, monitor behavior, lifecycle policy, or a frame
route; S5 remains the sole owner of those decisions.

### S5 — SoftPC monitor and derived-state reconciler

Replace the old non-MVDM Console handoff with SoftPC's sole FIFO control
thread/reconciler. It owns `display`, `console_control`, monitor parsing,
CAP/X intent, VM run generation, current frame route, host replacement calls,
and component lifecycle ordering. **Exit:** pure derivation and
completion-gated fake tests prove the complete approved state/transition matrix
without running a native Console.

#### S5 closure constraints

S5 is not complete merely because its public calls compile.  The following
properties are one inseparable acceptance set:

1. **Immutable published frame.** The runtime's published snapshot is always
   its last complete frame.  A graphics callback with no dirty region must not
   clear, mutate, or reclassify it.  The executor builds a staging snapshot,
   commits it with its sequence and SoftPC run generation only after success,
   and readers copy only the committed snapshot.  The reconciler consumes a
   frame, derives its text/graphics route, and publishes it to UX only when it
   observes a new committed sequence; otherwise it retains the previous
   completed route.
2. **One product control path.** Monitor lines, copied UX events, runtime
   state completion, frame publication, component completion, and broker
   replacement completion are tagged records in the sole SoftPC control FIFO.
   No presentation worker, native callback, monitor reader, or runtime worker
   may directly decide product lifecycle or component existence.
3. **Explicit desired and actual facts.** A pure reducer owns pending user
   intent, desired component set/current Console/VM target, actual completed
   facts, and one in-flight action.  It emits at most one next action.  The
   action's completion must be queued back before another action is derived;
   no caller may infer completion from a convenient synchronous side effect.
4. **SoftPC-only run envelope.** Each run has a monotonically increasing
   SoftPC generation.  Frame snapshots and every asynchronous UX/runtime/
   component/broker completion carry or are enveloped by it.  The control
   thread rejects a result from an old run.  Shared UX remains unaware of this
   generation and supplies only its own stable source identity.
5. **Transactional Current Console replacement.** Host first prepares and
   confirms the next native mode/reader without disrupting the old active
   object, then commits Current Console Object, then retires the old reader.
   Pre-commit failure leaves the old object active; post-commit failure has
   one defined rollback completion.  There is never a product-visible
   zero-Console interval.
6. **Retirement and backpressure are explicit.** Permanent raw-source
   retirement yields one ordered completion after no further raw events can be
   emitted; SoftPC releases only that source's guest keys.  The control FIFO
   must not silently discard make/break, hotkey, close, or retirement records.
7. **Proof before native runtime.** Pure tests use controllable fakes for all
   reducer transitions.  Barrier tests prove reader replacement during a
   callback, component retirement with queued input, broker failure/rollback,
   stale-generation rejection, and the complete display/console-control/X/
   lifecycle matrix.  These tests use completion barriers, not `Sleep()`.

### S6 — Lib Console/UX lifecycle contracts

Tighten the generic library before further app-thread integration.  The host
broker is the process-wide owner of one native Console I/O endpoint: normal
replacement prepares non-disruptive prerequisites, then serially stops/joins
the old reader, invalidates its binding, activates the next binding and either
commits it or restores the old binding before returning.  Logical Console
generation is host-private and rejects stale native callbacks before an app
sink observes them.  Native text/frame writes and output-sink switches share
one serialized boundary; a non-current Console returns `NOT_CURRENT`.

Each UX leaf retains private frame/control mailboxes and a unique, non-reused
source identity.  STOP is FIFO-ordered and terminal: the worker consumes it,
quiesces native input, emits exactly one source-retired event, then exits;
destroy waits for that exit.  Every non-droppable copied input must be accepted
by the application sink or produce an explicit generic delivery-failure
callback.  Registered hotkeys remain source-local identifiers only.  Tests use
controllable fakes and barriers, never sleeps, for rollback, stale callbacks,
output serialization, hotkey suppression/replay, and retirement.

**Exit:** generic host/UX mechanics contain no SoftPC policy, one-current and
retirement contracts are proven deterministically, and the shared corpus is
ready for S7 integration.

### S7 — Integration and concurrency boundaries

Connect S3–S5 in the actual non-MVDM runtime. Add only narrow real-thread
barrier/event tests for reader replacement during callback, destruction racing
queued input, and raw/cooked switching; no timing sleeps. **Exit:** no second
reader/output path remains, source audit is clean, and both widths build/test
to the extent available on the host.

### S8 — Owner runtime acceptance

Produce the two package widths without touching `softpc.ini` or media, then
run the approved Console/Window acceptance matrix with the owner. **Exit:**
owner accepts runtime behavior or records bounded defects; no design change is
silently folded in.

### S9 — NXVM adoption and re-import closure

Submit only the generic `src/lib` corpus, obtain NXVM's byte-identical
adoption, atomically re-import it into SoftPC, and verify the returned manifest
before final dual-width regression and closure. **Exit:** no permanent SoftPC
library fork and exact corpus evidence on both sides.

## S1 Source-Boundary Inventory And Dependency Proof

The following is the concrete S1 assignment from the T41 baseline. An entry is
owned by exactly one implementation S; a later S may call a completed earlier
contract but may not rewrite its owner.

| Current surface | Present responsibility/problem | Destination |
| --- | --- | --- |
| `src/lib/base/base.h` | shared scalar/status vocabulary only; lacks a logical Console object | S2 adds the generic copied object contract and its tests |
| `src/lib/host/{clock,sync}.*`, platform subdirectories, `src/lib/CMakeLists.txt` | host has no Console broker; existing clock/sync remain valid | S3 adds broker/platform implementations and target wiring; S2 owns the base-public-header portion |
| `src/lib/ux/**` | old unified controller/runner combines target choice, native Console access, and Window lifecycle | S4 deletes it and creates `src/lib/{ux-base,ux-window,ux-console}/`; S3 alone owns native Console I/O |
| old `ux` Win32/Linux code | sequential runner opens `CONIN$`, changes native modes, and lets Window X control runner result | S4 replaces it with independent Window/VM-Console API-parity components; Linux runtime acceptance remains deferred |
| `src/app/main.c` | synchronous `fgets` monitor owns lifecycle loop and shares process Console implicitly | S5 creates SoftPC monitor object/sink and moves command parsing into the sole control queue; cooked terminal behavior becomes host S3 |
| `src/app/{runtime,presentation}.{c,h}` | product frame router, direct `ux_run`, action callbacks, lifecycle calls, title/capture changes interleave synchronously | S4 mechanically removes direct old-`ux` calls; S5 creates the sole derived-state reconciler and run-generation envelopes, becoming the only host/UX/VM composer |
| `src/app/{input_queue,keyboard}.{c,h}` | UX-only queue and policy callbacks; mouse coalescing is currently app-owned | S5 changes it to the one tagged SoftPC input queue; S4 moves only Window mouse-move coalescing into UX |
| `test/unit/{runtime_smoke,runtime_input_continuation_smoke,win32_presentation_smoke,win32_window_smoke,win32_keyboard_smoke}.c` | current tests encode direct-runner and synchronous callback behavior | S5 rewrites/adds pure derivation and completion-gated reconciler coverage; S6 adds boundary integration proof |
| top-level/test CMake and package smoke | target/test registration and final package evidence | S2–S6 add only their own focused targets; S7 runs package/owner acceptance and S8 records exact corpus evidence |
| `src/host/compat/**`, `src/host/{machine,platform,video,dib_surface,audio,device_bop,keyboard,mouse_instance,serial,parallel,*.h}` | original-host ABI, fixed-machine endpoints, and MVDM-facing compatibility | intentionally unaffected. They are neither a Console broker nor a T42 migration surface |
| `src/mvdm/softpc.new/**` | preserved selected machine and MVDM-local Win32/WinNT implementation | permanently excluded |

Dependencies are strictly linear where an ownership handoff occurs:

```text
S1 scope proof
  -> S2 base object
  -> S3 host broker
  -> S4 split UX components
  -> S5 SoftPC reconciler
  -> S6 threaded integration
  -> S7 owner acceptance
  -> S8 NXVM adoption, re-import, T42 closure
```

`storage`, clock, and existing sync implementations are not rewritten by T42.
They may be used by the implementation but are not a covert second workstream.

## Non-goals

- No modification to `src/mvdm/softpc.new/`, its Win32 calls, or WinNT-derived
  implementation.
- No SoftPC lifecycle, DISPLAY TYPE, `console_control`, monitor syntax, or
  guest hotkey in shared lib.
- No permanent SoftPC-only fork of `src/lib`; no NXVM runtime/build dependency.
- No package configuration or guest-media modification.

## Exit

The derived-state and transition tests cover every approved Console/Window/
monitor lifecycle combination; host proves exactly one active Console object;
UX and host concurrency boundaries are deterministic; x64 and x86 builds/tests
pass; the owner accepts runtime behavior; NXVM has adopted the generic corpus
unchanged and SoftPC has re-imported and verified it.
