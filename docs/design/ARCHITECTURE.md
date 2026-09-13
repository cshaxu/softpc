# System Architecture

## Target Ownership

```text
mvdm/softpc.new
  selected original CCPU, C-VID, controllers, BIOS, firmware, BOP and renderers
        ↑
host/
  standalone host callback implementations: timer, media, BOP services,
  conapi-compatible surfaces, input, audio, serial and parallel endpoints
        ↑
app/
  one executor, machine snapshot producer, guest-input adapter, lifecycle,
  monitor, and product binding; no machine-state access from frontends
        ↑
lib/{types,console,host,storage,ui-base,ui-window,ui-console}/
  canonical shared platform library, delivered for NXVM to adopt exactly:
  copied-frame mailbox, host
  input normalization, generic registered-chord matching, independent Window/
  VM-Console components, clock,
  synchronization, and storage; no product queue or guest protocol
```

`mvdm/softpc.new` is the repository-owned selected recovered-machine layout.
It retains original relative paths and names for every included file so T14 can
compare them with the read-only OpenNT reference. It is a selected source
subset: wholly host-specific NT endpoint files may be absent when a standalone
host endpoint replaces that contract. Narrow compiler, declaration, calling-ABI
and pointer-representation corrections may be direct, source-visible diffs at
the affected point when they remain mechanical and introduce no machine policy.
`host/` owns larger host adaptations, including new state, lifecycle,
capability, ownership, and policy, but does not own guest-visible state.
`app/` owns the single executor, machine snapshot producer, guest-input
adapter, monitor, and product binding.
`lib/` is the canonical checked-in shared-library corpus, not a runtime or
build dependency on NXVM or NTVDM64. NXVM adopts this corpus exactly. It
consumes and produces copied host values only. It owns
the generic mailbox mechanics, independent console/window message loops,
host-input normalization, registered-chord matching, mouse capture, clock,
synchronization,
and storage primitives. The app binding owns its executor queue, converts
events to the guest's input protocol, and makes all product lifecycle and
action decisions. SoftPC publishes each admitted shared-library change as the
canonical corpus for NXVM to adopt exactly; the projects do not maintain
parallel variants.

Shared UI key events are copied `ui_key`, physical scan, neutral injection
flags, generic Ctrl/Alt/Shift state, and make/break values. Platform adapters
translate native records before the event reaches the shared contract; only
the application guest binding may translate that neutral value to a product's
guest-input protocol.

The runtime executor is the sole caller of the machine and compatibility host.
Input producers enqueue records and signal it. The executor publishes complete
text or graphic frame snapshots; frontends consume only those snapshots.

## Shared Console And UI Composition

SoftPC's checked-in `lib/` corpus is the shared-library source of truth. NXVM
adopts it exactly; no runtime or build dependency crosses repositories.

`types` defines universal copied scalar/status values and header-only external
C/compiler/platform vocabulary. Platform declaration headers contain no
component policy; host owns clock composition, and ui-base owns input
interpretation. Component platform implementations are selected by the build,
not by a generic types dispatcher. `console` defines
copied logical Console objects. `host` owns native Console
handles/modes, one I/O worker, and exactly one Current Console Object from
broker creation to destruction; replacement is transactional. Shared UI is
split into `ui-base` (copied UI values, one event-construction path,
private-mailbox helpers, and source-local generic hotkey matcher),
`ui-window` (one Window lifecycle), and `ui-console` (one VM Console lifecycle).
The latter creates an optional logical VM Console object but neither UI
component opens or registers the process Console. SoftPC owns its monitor
object and alone decides which UI components exist and asks host to replace the
current object.

The library's only direct component edges are:

```text
types    -> console + host + storage + ui-base + ui-window + ui-console
console  -> host + ui-console
ui-base  -> ui-window + ui-console
```

There is no other library edge and no aggregate UI target. In particular,
`ui-window` does not depend on `console` or `host`, while `host` does not
depend on UI. `ui-base` is only a library dependency of the two UI leaves;
the application may consume its public copied-value interfaces where the
control/input ABI requires them, never its leaf-support worker/mailbox or
input support interfaces. Cross-component support contracts use the same
`_interface.h` naming rule but have an explicitly narrower consumer set.

All cross-component contracts are declared at the owning component root.
Each component's `win32/` and `linux/` files are exclusively its own platform
implementation; neither sibling components nor root forwarding headers may
include them. The exception is `types/{win32,linux}`, which supplies shared
external vocabulary to matching platform sources. Existing Windows input
normalization support is declared by ui-base root interfaces and implemented
only by ui-base; callers never reach its platform headers. Common normalization
delegates raw decoding to same-shape selected platform functions; Window-only
message decoding and key-state queries remain in ui-window. This support API
is not an application input API.

SoftPC control is the sole product-state writer. VM, host, and UI workers only
enqueue copied events/completions to its app-owned queue. The control thread
derives runtime commands independently from the required component instances,
using config, frame route, and completed actual state; a derived action is
control-private and is never a shared VM/presenter intent. Runtime owns the
internal sequence of every accepted command, including reset, and reports its
one completed public fact. VM `run_generation` is SoftPC-only. Component
readiness is returned by each component's own creation/start contract; shared
UI carries no global configuration generation.

SoftPC passes copied `{chord, identifier}` registrations to each UI component.
The components may generically recognize and suppress a registered chord, but
only enqueue `UI_HOTKEY(identifier)`; they never execute a product callback or
interpret pause, reset, guest CAD, or another identifier. Their ordinary input
and matched-hotkey events are `ui_input_event` variants, constructed through the
one `ui-base` utility path and carrying their source component handle for
lifetime tracing only. The cooked monitor instead produces handle-free
`monitor_input_event` values. Both families enter SoftPC's one input FIFO as
distinct payloads in arrival order; the control thread is their sole consumer.
The generic matcher keeps pending only possible chord prefixes inside its own
component instance, preserving guest-input order on mismatch. Window and VM
Console maintain separate ledgers. Matching accepts each record's modifier
snapshot even when a modifier make was not observed in that instance; no
additional source-local modifier-participation restriction is imposed.

Every `ui-window` and `ui-console` instance owns private control/frame/input
mailboxes and its own native worker(s). Their common mailbox mechanics live in
`ui-base`; the mailbox instances remain per-component implementation details,
never public handles or shared UI infrastructure. SoftPC invokes the specific
component API it has chosen; components communicate back only through the
copied input-queue entry supplied at creation.

Shared frame mailboxes accumulate unconsumed dirty bounds with the latest
complete image under one lock. Capture does not consume; a successful-output
acknowledgement clears only that still-current publication. Host reports logical
Console activation through its neutral event sink after binding succeeds;
ui-console only wakes its existing worker to draw pending content. Empty means
no output, and NOT_CURRENT retains pending content without a retry loop.
Before native activation, the existing host binding helper synchronously sends
INPUT_RESET through the logical Console after old-reader quiescence. ui-console
clears its held keys, incomplete text and mouse baseline, retaining registrations
and pending frames. No new input can precede that reset. Rollback uses the same
helper; RESET is not activation success or permanent UI source retirement.
Cooked rollback restores only an unfinished line request observed after reader
join; ordinary cooked activation remains explicitly armed by the caller.
Window displacement retains integer remainders
internally without changing the copied input ABI. Post-start worker exits share
one failure/retirement cleanup path; startup failure remains distinct. Unexpected
Console I/O failures are copied events, not product decisions. Broker replacement
includes old-binding cleanup before the next transaction can enter. Types owns
external declarations; actual component consumers own OS linkage requirements.

## BOP And Firmware Boundary

Original BOP instruction decoding and `BOP FE` control flow remain in the
original CCPU. The compatibility host implements only the finite hardware and
firmware BOP services actually reached by the selected ROM. DOS, WOW, NTVDM,
and product-service selectors remain unavailable.

## Timing Boundary

The original host timer remains the only source advancing original SoftPC
timer/device work. Runtime scheduling may wait or yield host CPU time but may
not manufacture guest ticks, map nominal MHz to instruction counts, or add a
second guest clock.

## UI Input And Shutdown Admission

Window has one final event filter after shared matching. Frozen ordinary
key/text/mouse is consumed there; local capture/blink guards remain independent.
The existing pending make stores a leaf-supplied ordinary replay permission.
Window denies it for makes received while frozen; later unfreeze/repeats do
not upgrade that cached make. This does not alter chord matching or releases,
and ui-base has no frozen state. Only keyboard events determine prefix replay; mouse and close do not flush it.
Keyboard order is preserved without buffering key/mouse interleaving. A partial
sink failure is terminal, never retried; worker quiescence precedes its one
failure/retirement completion. STOP/fault closes both frame and control admission
atomically without bypassing FIFO control consumption up to STOP. Frame and
control have independent locks; only terminal admission takes both, frame first.
Ordinary control does not wait for frame copying.

Logical Console event/output gates and broker transactions use private blocking
locks in their owning components, preserving existing lock order and callback
barriers. No callback may synchronously reenter binding/destruction. Storage
owns each CRT stream directly; writer embeds file state instead of separately
allocated pointer wrappers.

Pure Window geometry, cursor rectangles, frame pixel conversion and relative
motion scaling belong to ui-window root helpers. Native files marshal SDK
values and own actual drawing/messages/capture. Worker context and frame share
one allocation; native cleanup runs on the worker, storage release after join.
Host event is one opaque platform allocation, not a pointer-only outer wrapper.
Task owns cancellation and entry/context; its platform thread object retains
startup parameters until join. Root task destroy performs the join once before
platform disposal.

Both UI leaves marshal native keyboard records into the same ui-base record
entry. It owns physical-versus-text choice and surrogate processing. Native Window
translation is requested only for unmapped transitions, so physical input has
no duplicate character stream. Text synthesis borrows the existing matcher
ledger and releases only keys it introduces, never separately tracking held
keys. One source-local held-key ledger owns pending/delivered/consumed
lifetimes for ordinary keys and modifiers alike. Repeats reuse their state;
a delivered make cannot retrospectively become a consumed chord.

Native adapters copy repetition metadata without expanding UTF-16 units.
ui-base owns repetition after complete-character decoding, malformed-prefix
recovery, and TEXT fallback when either stage of physical mapping is unavailable.
This changes Console raw metadata, not the application UI event schema.
