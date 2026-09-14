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
common/session/
  control FIFO, copied completion reduction, command/provider injection and
  dispatch to the selected UI and machine interfaces
        ↑
common/machine/
  sole generic executor, lifecycle/input queues, complete-frame publication
  and injected product-machine driver boundary
        ↑
common/debug/ + common/xasm32/
  generic debug/assembly capabilities; app injects the debugger CLI, and
  synchronous machine access consumes the paused-state executor boundary
        ↑
common/ui/
  broker, cooked monitor Console, raw VM Console, Window/KVM instances and
  presentation execution; copied events are injected into the product
        ↑
app/
  product configuration, entity assembly, CLI/title/hotkey/status policy,
  SoftPC machine driver and guest-input adapter; no control reduction or
  generic executor in app
        ↑
lib/{types,console,host,storage,kvm-base,kvm-window,kvm-console}/
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
`common/session` is the sole owner of the product-neutral control queue,
desired/actual reduction, prompt scheduling and dispatch order. It receives
the SoftPC CLI and machine adapter as injected callbacks; it does not parse
configuration or interpret machine internals. `common/ui` is the sole owner
of the monitor logical Console, broker, raw VM Console and Window/KVM
instances; it receives only copied product policy and returns copied events.
`app/` owns configuration, entity assembly, the SoftPC machine driver,
guest-input adapter and the SoftPC CLI binding. `common/machine` owns the
single generic executor, request/input queues, run generation and copied-frame
publication; its injected app driver alone calls the SoftPC machine boundary.
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

`common/xasm32` is an imported copied byte/text assembly capability and
`common/debug` is an imported generic debug command capability. SoftPC exposes
it through its injected app command binding, not through a second input loop.
`common/debug` depends on `common/machine`'s optional paused-state adapter and
on `common/xasm32`; neither component may create an executor, own a Console,
or add a product command path. Import hashes remain provenance evidence; the
S9 integration changes to these components form the updated downstream corpus.
The control thread serializes lifecycle, media and synchronous debug requests.
Debug requests are copied into the machine's existing command rendezvous and
executed by the parked executor, never by the calling frontend. The machine
checks paused state and the lease again there; callbacks must not synchronously
reenter this control-thread API. `app/command_binding` owns debugger selection
and copied prompts; machine state changes do not select or exit the CLI.

T56's owner-approved debug port adds observation calls at original CCPU
instruction entry and successful completion, including interrupt-shadow
bypasses. The product driver owns the plan/result and binds it only during
its executor run. A hit parks that same executor through its existing callback;
the existing copied PAUSED fact reaches session, whose debug provider reads
the result through the paused rendezvous. No per-instruction frontend events,
second executor or guest TF/DR ownership are introduced. CCPU refetches after
the pause so edited CS:EIP/code/translation are observed. Cancellation uses the
existing command wake; pause, stop/reset and debugger close clear the plan.

Shared KVM key events are copied `kvm_key`, physical scan, neutral injection
flags, generic Ctrl/Alt/Shift state, and make/break values. Platform adapters
translate native records before the event reaches the shared contract; only
the application guest binding may translate that neutral value to a product's
guest-input protocol.

The common-machine executor is the sole caller of the injected machine driver
and compatibility host. Input producers enqueue records and signal it. The
executor publishes complete text or graphic frame snapshots; frontends consume
only those snapshots.

## Shared Console And KVM Composition

SoftPC's checked-in `lib/` corpus is the shared-library source of truth. NXVM
adopts it exactly; no runtime or build dependency crosses repositories.

`types` defines universal copied scalar/status values and header-only external
C/compiler/platform vocabulary. Platform declaration headers contain no
component policy; host owns clock composition, and kvm-base owns input
interpretation. Component platform implementations are selected by the build,
not by a generic types dispatcher. `console` defines
copied logical Console objects. `host` owns native Console
handles/modes, one I/O worker, and exactly one Current Console Object from
broker creation to destruction; replacement is transactional. Shared KVM is
split into `kvm-base` (copied KVM values, one event-construction path,
private-mailbox helpers, and source-local generic hotkey matcher),
`kvm-window` (one Window lifecycle), and `kvm-console` (one VM Console lifecycle).
The latter creates an optional logical VM Console object but neither KVM
component opens or registers the process Console. `common/ui` owns the monitor
object, decides which KVM components exist from injected actions and asks host
to replace the current object. SoftPC app policy derives and injects those
actions; common/ui does not interpret their product meaning.

The library's only direct component edges are:

```text
types    -> console + host + storage + kvm-base + kvm-window + kvm-console
console  -> host + kvm-console
kvm-base  -> kvm-window + kvm-console
```

There is no other library edge and no aggregate KVM target. In particular,
`kvm-window` does not depend on `console` or `host`, while `host` does not
depend on KVM. `kvm-base` is only a library dependency of the two KVM leaves;
the application may consume its public copied-value interfaces where the
control/input ABI requires them, never its leaf-support worker/mailbox or
input support interfaces. Cross-component support contracts use the same
`_interface.h` naming rule but have an explicitly narrower consumer set.

All cross-component contracts are declared at the owning component root.
Each component's `win32/` and `linux/` files are exclusively its own platform
implementation; neither sibling components nor root forwarding headers may
include them. The exception is `types/{win32,linux}`, which supplies shared
external vocabulary to matching platform sources. Existing Windows input
normalization support is declared by kvm-base root interfaces and implemented
only by kvm-base; callers never reach its platform headers. Common normalization
delegates raw decoding to same-shape selected platform functions; Window-only
message decoding and key-state queries remain in kvm-window. This support API
is not an application input API.

Common session control is the sole product-state writer. VM, host, and KVM
workers only enqueue copied events/completions to its common-owned queue. The control thread
derives runtime commands independently from the required component instances,
using config, frame route, and completed actual state; a derived action is
control-private and is never a shared VM/presenter intent. Runtime owns the
internal sequence of every accepted command, including reset, and reports its
one completed public fact. VM `run_generation` is SoftPC-only. Component
readiness is returned by each component's own creation/start contract; shared
KVM carries no global configuration generation.

SoftPC passes copied `{chord, identifier}` registrations to each KVM component.
The components may generically recognize and suppress a registered chord, but
only enqueue `KVM_HOTKEY(identifier)`; they never execute a product callback or
interpret pause, reset, guest CAD, or another identifier. Their ordinary input
and matched-hotkey events are `kvm_input_event` variants, constructed through the
one `kvm-base` utility path and carrying their source component handle for
lifetime tracing only. The cooked monitor instead produces handle-free
`monitor_input_event` values. Both families enter SoftPC's one input FIFO as
distinct payloads in arrival order; the control thread is their sole consumer.
The generic matcher keeps pending only possible chord prefixes inside its own
component instance, preserving guest-input order on mismatch. Window and VM
Console maintain separate ledgers. Matching accepts each record's modifier
snapshot even when a modifier make was not observed in that instance; no
additional source-local modifier-participation restriction is imposed.

Every `kvm-window` and `kvm-console` instance owns private control/frame/input
mailboxes and its own native worker(s). Their common mailbox mechanics live in
`kvm-base`; the mailbox instances remain per-component implementation details,
never public handles or shared KVM infrastructure. SoftPC invokes the specific
component API it has chosen; components communicate back only through the
copied input-queue entry supplied at creation.

Shared frame mailboxes accumulate unconsumed dirty bounds with the latest
complete image under one lock. Capture does not consume; a successful-output
acknowledgement clears only that still-current publication. Host reports logical
Console activation through its neutral event sink after binding succeeds;
kvm-console only wakes its existing worker to draw pending content. Empty means
no output, and NOT_CURRENT retains pending content without a retry loop.
Before native activation, the existing host binding helper synchronously sends
INPUT_RESET through the logical Console after old-reader quiescence. kvm-console
clears its held keys, incomplete text and mouse baseline, retaining registrations
and pending frames. No new input can precede that reset. Rollback uses the same
helper; RESET is not activation success or permanent KVM source retirement.
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

## KVM Input And Shutdown Admission

Window has one final event filter after shared matching. Frozen ordinary
key/text/mouse is consumed there; local capture/blink guards remain independent.
The existing pending make stores a leaf-supplied ordinary replay permission.
Window denies it for makes received while frozen; later unfreeze/repeats do
not upgrade that cached make. This does not alter chord matching or releases,
and kvm-base has no frozen state. Only keyboard events determine prefix replay; mouse and close do not flush it.
Keyboard order is preserved without buffering key/mouse interleaving. A partial
sink failure is terminal, never retried; the detecting thread reports the first
failure independently of wake success. Worker quiescence precedes retirement,
not fault notification. STOP/fault closes both frame and control admission
atomically without bypassing FIFO control consumption up to STOP. Frame and
control have independent locks; only terminal admission takes both, frame first.
Ordinary control does not wait for frame copying.

Logical Console event/output gates and broker transactions use private blocking
locks in their owning components, preserving existing lock order and callback
barriers. No callback may synchronously reenter binding/destruction. Storage
owns each CRT stream directly; writer embeds file state instead of separately
allocated pointer wrappers.

Pure Window geometry, cursor rectangles, frame pixel conversion and relative
motion scaling belong to kvm-window root helpers. Native files marshal SDK
values and own actual drawing/messages/capture. Worker context and frame share
one allocation; native cleanup runs on the worker, storage release after join.
Host event is one opaque platform allocation, not a pointer-only outer wrapper.
KVM destruction performs one bounded 5000 ms join. An unjoinable live worker is
an application-terminal infrastructure failure; Lib never terminates the
process or exposes a half-object recovery protocol. A KVM notification failure
after copied work has been accepted reports once through the existing failure
sink; it is not a second request rejection and is never replayed. Window blinking uses
one native timer message path, including native modal loops, not outer-loop
timeout scheduling.
Task owns cancellation and entry/context; its platform thread object retains
startup parameters until join. Root task destroy performs the join once before
platform disposal.

Synchronous request rejection returns `lib_status`, with normal wait outcomes
separated into output values. Ordinary synchronization and local cleanup stay
inside their owning component. A failed public create leaves its output null;
failed file close consumes the stream and cannot be retried. Expected local
outcomes remain local; only terminal live-worker/native-reader ownership
failures use the application's status boundary. Accepted KVM requests report
asynchronous failure through the existing control-queue sink. No alternate
event path exists.

Both KVM leaves marshal native keyboard records into the same kvm-base record
entry. It owns physical-versus-text choice and surrogate processing. Native Window
translation is requested only for unmapped transitions, so physical input has
no duplicate character stream. Text synthesis borrows the existing matcher
ledger and releases only keys it introduces, never separately tracking held
keys. One source-local held-key ledger owns pending/delivered/consumed
lifetimes for ordinary keys and modifiers alike. Repeats reuse their state;
a delivered make cannot retrospectively become a consumed chord.

Native adapters copy repetition metadata without expanding UTF-16 units.
kvm-base owns repetition after complete-character decoding, malformed-prefix
recovery, and TEXT fallback when either stage of physical mapping is unavailable.
This changes Console raw metadata, not the application KVM event schema.
