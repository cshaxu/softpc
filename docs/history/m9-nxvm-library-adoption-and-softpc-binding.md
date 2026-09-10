# M9 T41: NXVM Library Adoption And SoftPC Binding (closed record)

> **Status:** Closed by owner direction. This retained proposal records the
> completed T41 import/binding work. Its former S5 Console-object material is
> implementation background for [M9 T42 Console-object and UX recomposition](m9-t42-console-object-ux-recomposition.md), not an unfinished T41 condition.

## Purpose

Replace SoftPC's current `src/lib/` directory in full with the reviewed NXVM
`lib/` directory. The approved NXVM library content is imported without source
modification, at a recorded upstream revision and content hash. It is not
merged with, supplemented by, or locally edited beside the present SoftPC
library. SoftPC then binds its fixed machine to that local library and removes
duplicative SoftPC code wherever the imported library owns the same capability.

This is a local source import, not a cross-repository build, runtime, or
acceptance dependency. NXVM remains read-only comparison material; after an
approved import, SoftPC builds solely from its own checked-in sources.

The former S5 delivery-order text is retained below solely as technical
background for T42. Current product and architecture authority is in
[Product UX](../design/UI.md) and [System Architecture](../design/ARCHITECTURE.md).

The desired source responsibility is:

```text
src/mvdm/softpc.new/  selected SoftPC machine and its narrow porting diffs
src/lib/              complete, unmodified approved NXVM library content
src/host/             SoftPC-specific host and original-host compatibility
src/app/              minimal SoftPC binding, monitor, and product policy
```

The objective is not merely to add a library dependency. Every MVDM-external,
product-neutral host capability actually covered by the imported `host`,
`storage`, or `ux` contract must use that contract: clocks, task/event/wait
mechanics, native presentation mechanics, ordinary owned-file access, and
byte-medium leases. `src/app/` and the non-library portion of `src/host/`
retain SoftPC policy, runtime coordination, device protocols, and
original-host adaptation around those calls. They must not retain, move, or
recreate a local platform implementation that lib already owns.

Any current SoftPC `src/lib/` code is deleted when the NXVM replacement covers
it. A current local mechanism that has no product-neutral lib equivalent is
not silently moved: S1 must classify it either as an irreducibly
SoftPC-specific machine/original-host boundary or as an upstream lib gap to
be resolved before its duplicate is removed.

## Admission Record

M9 T41 S1 is admitted against the reviewed complete corpus
`O:/repos.hobby/nxvm/src/lib` at
`c7b5e668b7d9fc8e8710c3dd77c863ce07801553`. Its `MANIFEST.sha256` file hash
is `C22012E985A30E130369AAC8A5E0C0E93DA9F56C842843A4C5B3685769E3E9A6`.
The review records the public headers, Windows-first platform boundary,
ownership model, and the required unchanged-import rule in the active T41
packet. Linux sources remain in the corpus; Linux runnable acceptance is
deferred, not removed. The corpus now deliberately contains only `base`,
`host`, `storage`, and `ux`; its README and manifest agree with that scope.

No SoftPC task may guess that selection, selectively copy a library subset,
retain part of the current SoftPC `src/lib/` beside it, or modify imported
NXVM source to satisfy a SoftPC boundary.

## Required Boundary

The imported library owns every product-neutral capability it exposes. SoftPC
must route all non-MVDM use of those capabilities through lib, rather than
through a direct SDK call or an old local wrapper: `host` owns clock and
task/event/wait mechanics, `storage` owns ordinary owned-file access, writers,
and byte-medium leases, and `ux` owns copied frames, host input
normalization, mailboxes, presenters, routing, and capture.

SoftPC retains its selected original machine, original-host ABI, fixed-machine
assembly, package configuration, media topology/geometry, monitor syntax,
product-visible policy, runtime input/command queue, and device endpoint
protocols. Those owners choose when to call lib and translate their own copied
values, but do not recreate lib's generic mechanism. If both products actually
need a missing generic mechanism, it is an NXVM lib change rather than a new
SoftPC-local platform module. NXVM UX deliberately leaves product input-queue
ownership outside lib, so the former `event_queue.c/.h` behavior may be
rehomed under `src/app/` as SoftPC executor coordination.

This boundary starts strictly outside `src/mvdm/softpc.new/`. Any direct Win32
API use or WinNT-derived implementation retained within that preserved MVDM
tree remains there unchanged. T41 neither audits it as a migration candidate
nor modifies it; a separate admitted port-ABI task would be required for any
such change.

SoftPC binding code may adapt copied values and opaque handles, but must not
fork or locally reimplement an imported-library capability. A proposed removal
is valid only when the replacement ownership, input/output contract, and
x86/x64 evidence are explicit.

## Non-goals

- Do not modify `src/mvdm/softpc.new` except through separately admitted,
  narrow portability-diff work.
- Do not migrate, wrap, replace, or otherwise alter any MVDM-local Win32 API
  use or WinNT-derived implementation as part of T41.
- Do not modify the imported NXVM library source after import.
- Do not introduce an NXVM runtime dependency, session catalog, profile
  system, debugger, asset loader, product console, or multi-machine policy
  into SoftPC.
- Do not replace SoftPC CPU, C-VID, controller, BIOS, ROM, BOP, timer, media,
  or original renderer behavior.
- Do not delete a SoftPC owner merely because it has a similar name; prove the
  imported implementation is a contract-compatible replacement first.

## Serial Subtasks

### S1 — Verbatim import and duplicate-ownership audit

After the admission prerequisite is met, replace `src/lib/` atomically with
the approved NXVM `lib/` scope byte-for-byte and record its source revision
and content hash. Do not edit imported files or retain a current SoftPC
library file in that directory. Produce a complete ownership ledger for every
active non-`mvdm` SoftPC source file, including every displaced current library
file: retained SoftPC binding/host responsibility outside `src/lib/`,
imported-library equivalent, candidate deletion, or blocked semantic mismatch.

The audit must identify build-system and public-header adaptation needed to
compile the imported library locally without altering its source. It must
inventory every non-MVDM direct platform/file/media operation and classify it
as a required `host`/`storage`/`ux` migration, a SoftPC-specific runtime,
machine/original-host, or device-endpoint boundary, or an upstream lib gap
that both products actually need.

**Exit:** `src/lib/` is exactly the approved NXVM library import and is
hash-verifiable; every displaced current SoftPC library file and every other
active SoftPC non-library platform owner has a disposition; every generic
duplicate has either a named lib replacement or an explicit upstream blocker;
no deletion or functional routing change has occurred without a proved
replacement.

### S2 — Bind SoftPC to imported library capabilities

Migrate every S1-approved generic operation to the imported library: direct
clock/event/task/wait use to `host`; direct ordinary file and media lease use
to `storage`; and copied-frame, input-normalization, mailbox, router,
capture, and native presenter use to `ux`. Bindings may retain SoftPC product
policy, runtime queueing, original-host ABI, media geometry, device endpoint
protocols, and machine-specific input/media translation, but may not retain a
second generic implementation. Preserve one fixed SoftPC machine and its
executor-safe boundaries; teach neither lib nor MVDM about SoftPC product
policy.

### S3 — Remove superseded SoftPC implementations

Delete every superseded old platform implementation, including the complete
former `src/lib/platform/win32/` corpus and any direct local wrapper that S2
has replaced. Delete only after dual-width focused and full-regression proof.
Keep a compact deletion/retention ledger: every remaining non-library file
must be explained by a real SoftPC runtime, machine/original-host, product, or
device-endpoint responsibility, not historical platform placement. A missing
shared generic facility that both products need blocks this step; a
product-specific queue or endpoint remains locally owned rather than forcing
an over-broad lib API.

### S4 — Freeze synchronization and source boundaries

Add a source-hash manifest and boundary checks proving the imported `src/lib/`
content remains byte-identical to its approved NXVM review revision. Document
the local update procedure: a future NXVM library update is a new reviewed,
atomic import, never an in-place SoftPC edit to library files.

### S5 archive — Superseded Console ownership contract

**Admission.** The owner added this subtask after runtime acceptance found
that an experimental SoftPC-side monitor/frontend handoff was unreliable. Its
uncommitted source is withdrawn from the working tree; it remains only in a
recoverable Git stash for forensic comparison and is not a candidate
implementation. S5 is a design and upstream-alignment subtask. It does not
authorize a local `src/lib/` edit or an MVDM edit.

**Problem.** A Windows process Console is one input resource. The existing
NXVM Win32 runner's private atomic flag is acquired only while its Console
runner exists, and `ux_run()` unconditionally starts that runner before it
considers the presenter target. Neither behavior expresses whether a Window
target keeps the Console for the presenter session or releases it to a
product's cooked command UI. A product must not infer that fact from a target
request, a frame, or a timing delay.

**Required public model.** The next reviewed NXVM UX contract needs one
immutable Boolean in `ux_binding`, fixed at `ux_run()` startup:

```text
retain_console_while_window = true
    the presenter owns the process Console from ux_run entry until ux_run
    returns, including while its active surface is Window.

retain_console_while_window = false
    the presenter owns the process Console only while its active surface is
    Console; a Window surface releases it.
```

The native Console runner's raw input mode and event reader exist only while
the active surface is Console. A `SESSION` lease held during Window is a
logical reservation, not a hidden raw reader. On a transition to Console the
native owner must first acquire the presenter lease, configure raw input, and
then report Console active. On a transition away it must stop its reader,
restore the saved mode, release the lease when the selected scope requires it,
and only then report the resulting active state. `ux_run()` must capture the
initial target before it creates a Console runner.

The public binding must report completed facts, not requested intent: active
surface (`NONE`, `WINDOW`, `CONSOLE`) and whether the presenter currently owns
the Console. It must not expose Win32 handles or encode SoftPC lifecycle,
commands, or guest hotkeys. A callback or copied-state/event interface is
acceptable, but it must be non-reentrant and documented as an observation of
completed native ownership transfer.

**Product boundary.** SoftPC sets this Boolean; lib does not name or infer a
product mode. When it is true, the original simple monitor model applies: the
monitor has no reader between `ux_run()` entry and return. When it is false,
SoftPC may run its monitor while Window is active, but it may request a Console
target only after its cooked reader has stopped, joined, and released its
Console lease. It may start that reader only after lib reports that Window is
active and the presenter lease is released. The generic lib must supply the
lease and cooked/raw platform transition required to make this exclusive; it
must not leave independent `stdin` and `CONIN$` readers to race over the same
process Console. Parsing `start`, `pause`, `resume`, `reset`, and `stop`
remains SoftPC product code.

**NXVM alignment and gate.** S5 records a clean NXVM revision and manifest,
then reviews that exact corpus against this contract. If NXVM adopts the
generic lease capability, SoftPC imports the resulting whole `src/lib/`
corpus atomically and byte-identically; it does not carry a local UX fork.
The same Boolean contract must have a Linux-compatible public shape, though
Linux runnable parity remains deferred.

**Exit:** an approved, minimal ownership/state table; an exact public API and
native transition order; a clean upstream revision/manifest implementing it;
and a SoftPC audit proving that the withdrawn coordinator/global-`stdin`
approach is absent. Follow-on implementation requires focused ownership tests,
dual-width builds, and owner runtime acceptance before it can replace the
current package behavior.

### S5 — Console-object contract and upstream UX redesign

The archived S5 text above is superseded in full. It incorrectly coupled VM
running state to UX Console ownership and assigned SoftPC Console ownership
to lib UX. The following is retained T42 implementation background; current
design authority is the linked UI and architecture documentation.

**Scope.** The owner moved this delivery order into T42: SoftPC implements this
generic shared-library candidate in local `src/lib/`, then NXVM adopts that
exact code and SoftPC re-imports it before T42 closes. This authorizes only the
APIs and generic implementations specified here; it does not authorize an `softpc.ini`
edit, an MVDM edit, or SoftPC product policy in lib. The withdrawn SoftPC
monitor/frontend coordinator and global-`stdin` handoff remain absent.
The retained [owner decision record and cross-audit](../etc/evidence/softpc/m9-t41-s5-console-design-review-record-zh.md)
is supporting evidence only. Current T42 product and architecture authority is
in [Product UX](../design/UI.md) and
[System Architecture](../design/ARCHITECTURE.md); this section is retained
implementation elaboration and acceptance planning.

**Three independent product facts.**

```text
DISPLAY TYPE  = CONSOLE | WINDOW
                Static SoftPC product configuration from softpc.ini.

PRESENTER SET = { window_enabled, console_enabled }
                Active presentation surfaces. SoftPC selects the set; lib UX
                creates/destroys each enabled native presenter.

CURRENT CONSOLE OBJECT
                The one logical Console object currently bound by host to the
                process Console: either SoftPC's monitor object or UX's object.
```

**One SoftPC reconciliation path.** SoftPC's control thread is the sole
product-state writer and the only component allowed to call VM lifecycle, host
replacement, or UX presenter-set APIs. It owns two copied records:

```text
actual  = completed VM lifecycle + completed presenter set + Current Console Object
inputs  = static config + current frame route + FIFO user intent
target  = derive(inputs, actual VM lifecycle)
```

Every source—monitor line, UX action, Window close request, frame-route update,
and VM/UX completion—copies an event to the one SoftPC input queue. The control
thread records the event as an input fact, derives the target, then runs the one
reconciler. No callback or worker directly pauses a VM, replaces a Console
object, or changes a presenter set.

```text
target VM raw:
  prepare UX presenter/object -> host monitor-to-UX replacement -> VM start/resume

target SoftPC monitor with VM paused/stopped:
  VM pause/stop completion -> host UX-to-monitor replacement -> retire unneeded UX surfaces

target SoftPC monitor with VM still running:
  ensure Window presenter -> host UX-to-monitor replacement -> retire UX Console; VM continues
```

The reconciler advances only after the relevant VM, host, or UX completion. On
failure it leaves `actual` intact, publishes an explicit SoftPC error, and does
not silently retry, fall back, or invent a second transition path.

**Lifecycle FIFO and run generation.** Lifecycle intents (`start`, `resume`,
`pause`, `reset`, `stop`, CAP, and X) are FIFO SoftPC commands. The control
thread has at most one active lifecycle transaction; it does not reinterpret or
overwrite the next lifecycle command until the current transaction has reached
its completed `actual` state. Thus two CAP chords are necessarily pause then
resume, never two racing writes to a mutable desired-state record. Frame-route
updates are not lifecycle intents: SoftPC coalesces them to the latest route and
re-derives presentation only between lifecycle transaction completion points.

`run_generation` is a separate monotonic `lib_u64` assigned by SoftPC before it
asks the VM executor to start a new machine run or reset one. Pause and resume
retain the current generation; stop invalidates it; reset creates a new one even
when the resulting machine is paused. Every app-side VM completion and copied
frame-route/frame record entering the SoftPC input queue carries this private
generation. The control thread accepts it only when it equals the currently
active generation; stale records are dropped before they can alter `actual`,
derive a presenter set, or call any UX API to publish a frame. An individual
lifecycle transaction also has its own operation token, so a completion must
match both its `run_generation` and the outstanding operation.

This is deliberately distinct from UX `configuration_generation`: UX generation
acknowledges a surface/object configuration within one presenter lifetime;
`run_generation` is SoftPC-only state and never appears in a lib UX type,
mailbox, frame, event, or API. It prevents output or completion from an old VM
run reaching a new presenter or session because SoftPC discards the record
first. Guest-input queues are cleared when a run generation is invalidated, and
input is forwarded only while the matching generation is actually running.

UX-originated input needs no lib-visible VM generation. The SoftPC UX binding is
the queue ingress: when it receives a copied UX action or guest-input event, it
reads SoftPC's current atomic input-generation and stores that value in the
*app-owned* input envelope alongside the unchanged UX event and its source
presenter reference. On dequeue the control thread rejects an envelope whose
input-generation is no longer current. Window-close and UX-completed events are
presenter-lifetime events, not VM-run events: they use the retained source
presenter check and do not require a run generation. Monitor lines are likewise
user intents interpreted against current `actual` state at dequeue.

`display=console` routes text frames to the Console presenter and graphic
frames to the Window presenter; `display=window` routes every frame to the
Window presenter. SoftPC alone makes this frame-routing decision.

The follow-on package setting is `console_control=0|1`; it is a SoftPC product
setting, not a lib setting. A missing key means `1`, preserving
the monitor-first package behavior. When `display=console`, an explicitly
invalid value is a clear SoftPC configuration error. When `display=window`,
SoftPC does not read or validate `console_control`: the Window/monitor policy is
fixed and the key is semantically absent.

| SoftPC-selected condition | Presenter set | Current Console Object / input |
| --- | --- | --- |
| running text under `display=console` | `{window=false, console=true}` | UX object / VM raw events |
| running Window; `console_control=1` | `{window=true, console=false}` | SoftPC monitor / cooked commands |
| running Window; `console_control=0` | `{window=true, console=true}` | UX object / VM raw events |
| paused or stopped while Window remains visible | `{window=true, console=false}` | SoftPC monitor / cooked commands |
| paused or stopped without a Window | no presenter (destroyed) | SoftPC monitor / cooked commands |

`1` means a running Window surface releases Console input to the SoftPC
monitor. `0` means a running VM retains raw Console input while its only
display surface is Window: Console and Window are then equivalent VM input
producers, including registered host actions. Under the object-lifetime rule,
this is the live presenter set `{window=true, console=true}`: the UX Console
object exists, owns raw Console input, and renders SoftPC's fixed Window-status
text. The cooked-monitor case is `{window=true, console=false}` and the current
object is SoftPC's monitor object.

Lib UX retains and redraws only the latest frame explicitly published to each
of its presenters; absent a new Console frame, it leaves the last Console
frame intact. Frame content is SoftPC product policy. In particular, while a
graphic frame is published to Window: `console_control=1` makes Console the
SoftPC monitor, while `console_control=0` requires SoftPC to publish a
fixed Console text frame explaining that SoftPC is running in Window and
listing the supported host shortcuts. Lib does not synthesize that text or
infer a mode.

**Console frame/output contract.** A Console presenter accepts only text frames;
a Window presenter accepts text or graphic frames. For
`display=console + graphics + console_control=0`, SoftPC explicitly publishes
the fixed Window-running status text to the enabled UX Console presenter. When
the monitor object is Current, only SoftPC monitor output may write Console
(prompt, command results, and status); UX publishes no Console output. Host
serializes each Current object's `write_text` batches through one output path,
so a prompt, status, or frame batch cannot interleave at byte granularity.

**Presenter set.** The public product semantics are two independent flags, but
a requested state is published as one atomic presenter-set snapshot:

```c
typedef struct ux_presenter_set {
    lib_bool window_enabled;
    lib_bool console_enabled;
} ux_presenter_set;

lib_status ux_presenter_set_presenters(ux_presenter *presenter,
    const ux_presenter_set *requested);
```

The three valid live sets are `{false,true}` (Console only), `{true,false}`
(Window only), and `{true,true}` (both). `{false,false}` is rejected: a caller
that wants presentation to end must use the presenter's terminal destroy
operation, which joins the native runners before releasing the object. This
does not make the flags conceptually dependent. Publishing the pair atomically
simply prevents a reader observing arbitrary halves of a Console-to-Window
change, such as an unintended transient `{false,false}` or `{true,true}`.

Internally the snapshot has a monotonically increasing generation. It is not a
product state or an INI option. Runners record the last generation they have
applied, a wake means only “a later generation may exist,” and SoftPC can wait
for acknowledgement of the exact requested generation. Completed state reports
the same generation alongside the active Window/Console set. Thus a completed
record such as `#43={window=true,console=false}` means that every runner has
finished applying request 43; it does not merely report two independently read
Booleans.

**Shared-library boundary.** Strict cooked/raw handoff cannot use independent
`fgets` and `CONIN$` readers. It must not make UX a manager of SoftPC's monitor
or VM. The shared `host` component therefore provides a generic opaque Console
object broker:

```text
host/win32/   Windows handles, modes, raw-event and cooked-line object bindings,
              exclusive transitions, cancellation, join and acknowledgement.
host/linux/   Same public contract as a placeholder; Linux runnable parity is
              deferred.
```

The broker knows neither SoftPC nor UX. `storage` is independent. `ux` is also
independent of `host`: both SoftPC and UX own only their respective generic
`lib_console` objects. SoftPC asks `host` to make exactly one such object
active; UX owns its own optional object, but never opens a process Console or
registers itself with host. A SoftPC monitor reads
and writes only its own object. UX reads and writes only its own object. The
host registration is the sole point at which either object reaches the user's
Console. SoftPC is the sole composition and policy layer.

Invariant:

```text
From app startup onward, host has exactly one active Console-object
registration. It atomically replaces the old registration with the next one;
two objects can never receive input or write native Console output at once.
```

**Reconciler-derived object order.** A host replacement always precedes
destruction of the old object's owner. For raw UX -> cooked monitor, after the
VM has completed pause/stop the reconciler asks host to replace the current UX
object with its already-existing monitor object; only after that completion may
it disable/destroy the UX Console presenter. For cooked monitor -> raw UX, the
reconciler first requests and awaits `console_enabled=true`, obtains the new UX
object, then asks host to replace the monitor object before starting/resuming
the VM. Thus host never has zero active objects, and a failed preparation leaves
the old object current.

For `console_control=0` while Window is active, SoftPC requests both
presenters, obtains UX's newly created Console object, and keeps it current in
host raw mode. The Window and Console presenters are equivalent VM input
producers; Console renders the fixed Window-status text. Pause/stop makes the
monitor object eligible to become current. Pause/resume do not determine the
presenter set.

**Proposed public APIs.** Names below are contract names, not authorization to
implement a local fork. Their ownership is deliberately asymmetric: `host`
binds one logical object to native Console I/O; `ux` owns its own optional
object; SoftPC composes the two.

```c
/* lib/base/console.h: generic, logical Console object ABI. */
typedef enum lib_console_event_kind {
    LIB_CONSOLE_EVENT_RAW_INPUT,
    LIB_CONSOLE_EVENT_COOKED_LINE
} lib_console_event_kind;
typedef struct lib_console_event {
    lib_console_event_kind kind;
    lib_u32 binding_generation;
    union {
        lib_input_event raw_input;
        struct { lib_u32 length; char text[LIB_CONSOLE_LINE_MAX]; } cooked_line;
    } value;
} lib_console_event;
typedef struct lib_console lib_console;
typedef void (*lib_console_event_sink)(void *context,
    const lib_console_event *event);
lib_status lib_console_set_event_sink(lib_console *console,
    lib_console_event_sink sink, void *sink_context);
lib_status lib_console_write_text(lib_console *console,
    const char *text, lib_u32 length);

/* lib/host/console.h: host-owned native binding and exclusivity. */
typedef struct host_console_broker host_console_broker;
typedef enum host_console_mode {
    HOST_CONSOLE_RAW_EVENTS,
    HOST_CONSOLE_COOKED_LINES
} host_console_mode;

lib_status host_console_broker_create(host_console_broker **out_broker,
    lib_console *initial_console, host_console_mode initial_mode);
lib_status host_console_replace_active(host_console_broker *broker,
    lib_console *old_console, lib_console *next_console,
    host_console_mode next_mode);
void host_console_broker_destroy(host_console_broker *broker);
```

Broker creation requires SoftPC's monitor object and activates it before the
broker is returned. There is deliberately no public unregister operation. A
replacement names the object host currently owns and the next object, so host
can reject stale or competing calls. `host_console_replace_active` is a
completion boundary: on return, the old native reader is joined, the old Console
mode is restored, and the next object is already attached to its newly
configured native reader/output path. The current object's one generic event
sink receives either copied raw-input events or copied cooked-line events; it is
not `stdin` and does not expose a raw file descriptor. Host does not parse
monitor commands. The SoftPC-owned monitor object's sink copies cooked lines to
the SoftPC input queue, while the UX-owned object's sink routes raw events to
UX. Raw events and text output use copied `lib_console_*` values only. Host
stamps input delivery with its binding generation; after a replacement, an old
object's queued events are discarded rather than delivered to the new owner.
Replacing or clearing an object's sink is synchronous with respect to future
callback entry. `LIB_CONSOLE_LINE_MAX` and overflow behavior are defined by
`base`: a line longer than that maximum becomes one explicit rejected-line
event, never a truncated command. An object which is not Current returns
`LIB_STATUS_NOT_CURRENT` from `lib_console_write_text`; it never silently writes
into another object's native Console. `host_console_broker_destroy` is the sole
normal teardown exception to the exactly-one-object invariant: it stops the
reader, restores native Console mode, and releases the native handles.

Replacement is transactional even though one native Console cannot run two
readers simultaneously: host validates the next logical object first, parks the
old reader, configures the next native mode/reader, and commits Current Console
Object only on success. If configuration fails after parking the old reader,
host restores its mode/reader and leaves the old object Current; no caller sees
a zero-object or partially switched state.

Host reports, rather than silently degrades, missing/redirected Console,
unsupported native mode, mode-restore failure, and operating-system shutdown
notification. SoftPC decides the product response. Broker teardown is the one
explicit OS/process-exit path; it does not attempt a second Console owner.
Cooked input preserves the native Console host's ordinary line-editing behavior
(echo, editing, paste, and any host-provided history). The broker and SoftPC do
not add a second command-history implementation; availability of history is a
host characteristic, not a portable lib promise.

```c
/* lib/ux/presenter.h: UX has no host dependency. */
typedef struct ux_completed_state {
    ux_presenter_set active_presenters;
    lib_u32 configuration_generation;
} ux_completed_state;
typedef void (*ux_completed_state_sink)(void *context,
    const ux_completed_state *state);

lib_status ux_presenter_create(ux_presenter **out_presenter);
lib_status ux_presenter_start(ux_presenter *presenter,
    const ux_binding *binding);
void ux_presenter_destroy(ux_presenter *presenter);
lib_status ux_presenter_get_console(ux_presenter *presenter,
    lib_u32 completed_generation,
    lib_console **out_console);
lib_status ux_presenter_publish_window_frame(ux_presenter *presenter,
    const ux_frame *frame);
lib_status ux_presenter_publish_console_frame(ux_presenter *presenter,
    const ux_frame *frame);
lib_status ux_presenter_set_window_mouse_enabled(ux_presenter *presenter,
    lib_bool enabled);
lib_status ux_presenter_release_window_mouse(ux_presenter *presenter);
lib_u32 ux_presenter_capture_completed_state(const ux_presenter *presenter,
    ux_completed_state *out_state);
```

`ux_presenter_create` only constructs an inert presenter control object;
`ux_presenter_start` starts the independent UX runner, which creates/destroys
native presenters. `ux_presenter_destroy` is called only by SoftPC's control
thread, never from a UX callback; it returns only after every Window/Console
runner is joined and any UX Console object is destroyed.
When `console_enabled=true`, that runner creates and owns its logical Console
object, then reports the completed generation through the copied completion
sink. Only then may SoftPC call `get_console` with that exact generation; it
returns a borrowed reference solely for host registration. Host never asks UX
whether creation succeeded. When `console_enabled=false`, `get_console` returns
an explicit unavailable status. SoftPC never creates, destroys, or injects the
UX object. Every presenter-set mutation increments one
`configuration_generation`. Host holds a strong reference to its current
object, so SoftPC may replace an active UX object before asking UX to destroy
its presenter; after replacement, writes or queued input on the old object are
inactive/stale and cannot reach the native Console. The binding contains an
optional non-reentrant `ux_completed_state_sink`; lib invokes it only after the
requested surface set is in effect. A Window presenter accepts only its native
Window input; a Console presenter consumes its own object; disabled presenters
consume neither.

The two frame mailboxes are intentionally separate. SoftPC publishes a Window
frame only when Window should render it, and publishes a Console frame only
when Console should render it; lib does not infer a destination from graphics,
DISPLAY TYPE, VM state, or the enabled set. A Console renderer may reject a
graphics frame by its documented frame contract, but it never reroutes one.
Each mailbox has its own generation and each enabled renderer captures, rather
than consumes, its own latest frame.

Window mouse state is explicitly named as Window-only.
`set_window_mouse_enabled` controls whether the active Window presenter may
receive/capture mouse input; `release_window_mouse` is an idempotent request to
release current Window capture. Neither affects Console input or host
cooked/raw object selection.

When the user presses Window X, UX prevents the native default close and emits
one copied `UX_EVENT_WINDOW_CLOSE_REQUESTED` through its ordinary serialized
event/input sink. It changes neither the presenter set nor any Current Console
Object at that point. The word *requested* is material: UX has not yet closed
the native Window, so no observer can mistake the event for a completed
teardown.

SoftPC records X as one ordinary input intent: paused (if currently running)
and presentation close requested. The derived target is monitor object current
with presenter destruction. The reconciler therefore waits for pause completion, replaces UX with the monitor object,
then destroys the presenter. Until native Window destruction actually occurs,
UX continues accepting and serially delivering normal Window events; there is
no special closing filter. For an already non-running machine, X leaves the VM
target unchanged but uses the same monitor/retires-presenter path. Lib names
neither pause nor SoftPC state and has no separate X policy.

This is intentional dependency inversion: `base` defines copied events and
the capability shape; `host` implements native binding; `ux` consumes only
base types; neither component includes or links against the other. SoftPC is
the only code that includes both public headers and owns host-broker lifetime.

**Host internals.** A broker is process-local and owns one mutex plus exactly
one active object registration. `host/win32` opens `CONIN$`/`CONOUT$`, snapshots
the original mode, applies raw or cooked mode to the active object, and runs
exactly one native reader. Raw mode normalizes key/mouse records into
`lib_console_event`; cooked mode performs line editing and queues completed
lines. Replacement cancels a blocked old read, joins its worker, restores the
snapshot, then attaches the next object while holding the broker mutex.
`host/linux` declares the identical public API and returns an explicit
unsupported status until its terminal/poll implementation is admitted. Neither
implementation includes UX or app headers.

**Thread topology.** UX control is a synchronized mailbox/state object, not a
thread. `host` owns exactly one current-Console I/O worker. UX owns zero to two
presenter runners (Window if enabled; Console if enabled). SoftPC owns one
control thread which is the sole product-state writer; its immutable machine
executor remains a distinct existing runtime thread. The existing optional
speaker worker is unrelated to Console/UX ownership.

**UX internals.** UX owns surfaces, its optional Console object, frame
consumption, action matching and delivery into the supplied event sink. A
Console surface registers a private mailbox sink on its own object instead of
opening `CONIN$`; a Window surface likewise receives raw Console events through
that mailbox when both presenters are enabled.
The native Window loop continues to wait only for its own messages and UX
mailbox; it never needs a host wait handle. Both Console and Window drain the
same copied-event mailbox through the same action registry and guest-input
sink. Window and raw Console events both enter one FIFO before matching. The
one dispatcher serially matches actions and invokes the SoftPC action handler;
ordinary events use the same SoftPC guest-input sink. There is no cross-source
deduplication: two physical CAP chords are two ordered toggle requests. UX does
not receive cooked monitor lines. Host and UX each copy events into the one
SoftPC input queue. Its app-owned envelope distinguishes `MONITOR_LINE`,
`UX_ACTION`, `GUEST_INPUT`, `WINDOW_CLOSE_REQUESTED`, `FRAME_ROUTE`,
`VM_COMPLETED`, and `UX_COMPLETED`. SoftPC's one control thread is the sole
consumer and parser; lib host and UX never parse or execute monitor commands.

UX coalesces Window mouse-move records before they enter the SoftPC queue.
Keyboard transitions, mouse buttons, and Window-close requests are never
coalesced or dropped. UX emits one `INPUT_RESET` only when a raw UX Console
object is permanently retired or a Window/Console presenter is destroyed; the
control thread clears the matching generation's guest key/button state. It does
not synthesize resets for ordinary focus or presenter-set changes.
Surface/object ordering is:

```text
stop consuming old surface/object -> capture requested presenter set ->
create/destroy presenters and UX object -> publish completed set/configuration generation
```

The app changes host registration through one explicit replacement path. A set
gaining Console must first complete so UX has created its object and installed
its private sink; then host may activate raw input. A set losing Console is
destroyed only after host has replaced its object with the monitor object. UX
does not create a monitor object, parse lines, infer DISPLAY TYPE, or decide an
object's mode.

Every UX-originated envelope carries the source `ux_presenter *` and retains it
until dequeue. SoftPC accepts it only when that source is its current presenter;
after a terminal destroy it rejects stale source events before releasing their
reference. This uses the presenter handle as lifecycle identity rather than
inventing a second product instance identifier.

**SoftPC composition.** The app's control thread stores the configured DISPLAY
TYPE, current frame route, FIFO user intents, and the completed `actual` record.
It derives the target presenter set, Console object/mode, and VM lifecycle from
those facts; it does not store a second mutable desired-state record or perform
separate ad-hoc pause, host, or UX sequences.
It obtains UX's object only after a completed Console presenter generation and
consumes cooked lines only from its monitor object. The reconciler consequently
has one owner for each transition:

```text
raw UX object -> cooked monitor object: VM pause/stop completes; host replaces; UX Console retires
cooked monitor object -> raw UX object: UX Console completes; host replaces; VM starts/resumes
raw UX object across Window/Console: host remains current; reconciler changes presenter set only
```

When `console_control=1` selects a cooked monitor while Window is running,
monitor commands are pure SoftPC policy: `pause`, `stop`, and `reset` append
FIFO user intents; `resume` is accepted only while paused; `start` is accepted
only while stopped. CAP and other action chords append the same intent kinds.
SoftPC parses its own `softpc.ini`; a missing `console_control` key is `1`.

**Required upstream work.** Lib must report completed—not requested—presenter
sets and configuration generations, capture the initial requested set before
starting native runners, and acknowledge host object replacement. Current UX
has only a requested scalar target and opens/closes `CONIN$` internally; current
SoftPC has a synchronous outer `fgets` loop. Neither meets this contract. The
follow-on implementation replaces that reader with host's single cooked/raw
Console I/O worker plus the SoftPC input queue. No timing delay, target request,
or VM state is a handoff acknowledgement.

**Adoption gate.** SoftPC implements and verifies the generic host
Console-object contract and completed UX facts locally. NXVM then adopts that
candidate byte-identically; SoftPC atomically re-imports the adopted corpus and
verifies its manifest before closure. Lib names neither DISPLAY TYPE,
`console_control`, pause/resume, monitor commands, nor guest hotkeys.

**Deterministic concurrency verification.** This design is multi-threaded, but
its unit tests do not depend on a real Console, elapsed sleeps, or timing luck.
They use scripted VM, host, and UX fakes plus a manually pumped SoftPC input
queue. Each fake records calls and exposes explicit completion gates. A test
issues one product event, asserts the next required call, releases precisely
that completion, and repeats until the reconciler is idle. The recorded trace
is the proof of ordering.

Verification is layered:

1. Pure SoftPC derivation tests cover every `DISPLAY TYPE`, frame route,
   `console_control`, completed VM state, and user intent without threads.
2. Reconciler tests use the completion-gated fakes to prove start/resume,
   pause/stop, X, and text/graphic routing make only the approved calls in the
   approved order; injected failures leave `actual` unchanged. They also prove
   lifecycle FIFO behavior and reject stale frame/completion generations.
3. `lib host` tests use a controllable blocked reader/callback to prove exactly
   one Current Console Object, transactional replacement/rollback, generation
   rejection, strong-reference lifetime, and broker teardown.
4. `lib UX` tests use controllable Window/Console runners to prove completed
   generations, no `{false,false}` live set, object creation/destruction, stale
   source-event rejection, permanent-retirement `INPUT_RESET`, and Window-side
   mouse-move coalescing.

Real-thread tests are narrow boundary tests only. They use barriers/events to
force the dangerous interleavings—callback in flight during replacement,
destroy racing queued input, and raw/cooked switch—not arbitrary sleeps. Their
assertion is the same recorded contract trace and eventual join, with a bounded
test-harness deadlock guard solely to fail a hung test.

**Exit.** Approved Console-object API, state table, transition-order proof, and clean
NXVM revision/manifest; a SoftPC audit proving the withdrawn coordinator is
absent; then focused ownership tests, dual-width builds, and owner runtime
acceptance before implementation closure.

## Verification

Each admitted implementation subtask must preserve local, independent SoftPC
builds and run focused tests for every replaced capability. Runnable changes
require clean GCC x64 and i686 x86 builds, full CTest, and both package smokes
without modifying user-owned `assets/binary/softpc.ini` or guest media.

## Stop Conditions

Stop for owner direction if the NXVM review does not provide a complete
unchanged import scope; an imported interface would require SoftPC-specific
source changes; a proposed deletion affects a machine/original-host contract;
or an x86/x64 behavior difference cannot be attributed to an explicit binding
boundary.
