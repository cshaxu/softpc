# System Architecture

## Target Ownership

```text
app/main                    entry/config validation -> app/composition
app/composition             assembly: existing Common + vm public interface
app/config, command, keyboard  configuration and CLI/hotkey policy -> Common/Lib

common/session -> common/ui       control and monitor/KVM composition
common/session -> common/machine  sole generic executor and copied facts
common/debug + common/xasm32      shared debugger/assembly contracts
                         |
                 injected existing driver callbacks
                         v
src/vm                     SoftPC input/frame/debug/backend adaptation
                         |
src/compat                 original host callbacks and ABI support
                         |
src/mvdm/softpc.new         original CPU, controllers, renderer and firmware

Lib supplies shared platform mechanics to the owning consumers.
No app -> Compat/MVDM edge; only composition -> VM; no VM/Compat -> app edge.
```

`mvdm/softpc.new` is the repository-owned selected recovered-machine layout.
It retains original relative paths and names for every included file so T14 can
compare them with the read-only OpenNT reference. It is a selected source
subset: wholly host-specific NT endpoint files may be absent when a standalone
host endpoint replaces that contract. Narrow compiler, declaration, calling-ABI
and pointer-representation corrections may be direct, source-visible diffs at
the affected point when they remain mechanical and introduce no machine policy.
`compat/` owns original host callbacks, host resources and port ABI adaptations,
but no product machine lifecycle/configuration or guest-visible device state.
`common/session` is the sole owner of the product-neutral control queue,
desired/actual reduction, prompt scheduling and dispatch order. It receives
the SoftPC CLI and machine adapter as injected callbacks; it does not parse
configuration or interpret machine internals. `common/ui` is the sole owner
of the monitor logical Console, broker, raw VM Console and Window/KVM
instances; it receives only copied product policy and returns copied events.
`app/` owns configuration, entity assembly and the SoftPC CLI/hotkey binding.
Within app, command owns CLI/debug semantics, keyboard owns hotkey semantics,
and composition creates the entities, wires their callbacks into Common
session, runs it and performs teardown. Main validates arguments/configuration.
Composition does not parse commands or hotkey identifiers or add a state machine.
At exit it synchronously shuts down the machine worker with callback targets
still alive, then destroys UI, session, command/debug, machine and VM in order.
Machine shutdown and destroy share one stop/join path; shutdown retains storage
so referenced objects can be released safely before the machine itself.
Only app/composition.c consumes vm/vm_interface.h; no app source consumes Compat or
MVDM. `vm/` owns the concrete machine backend, initialization/reset/teardown
sequence, driver, frame/input conversion and debugger request preflight.
Its implementation calls Compat and the original machine while its
public interface exposes only copied options and Common/Lib contracts.
Compat never calls app or Common. `common/machine` owns the
single generic executor, request/input queues, run generation and copied-frame
publication; its injected VM driver calls the SoftPC machine boundary.

### Product Build And ABI Boundaries

These are ownership boundaries, not six mandatory forwarding layers. Device
I/O follows original MVDM host callbacks into Compat and then Lib storage;
Common machine schedules execution, not individual disk reads/writes. Original
CPU debug observations call the VM observer symbols without importing Common
or App implementation. Such callbacks do not transfer CPU state ownership.

All VM C sources belong to one `softpc-vm` OBJECT target. App and product
integration tests link it; the exe's own sources are App and resources only.
The recovered `softpc-machine` archive retains its CCPU/device/Compat OBJECT
composition. VM objects are explicit because the original CPU calls debug
observers; no pair of mutually extracting static archives is introduced.

`vm_create` owns the process-single-machine resource admission, before machine
or audio acquisition. A second live/concurrent creation fails without touching
the first; failure releases admission and destruction releases it after cleanup.
This restriction reflects original global CPU/device state, not a new machine
lifecycle state. Internal non-owning driver wrappers serve serialized low-level
tests, not an alternative application creation API.

Compat's narrow CCPU ABI and host support declarations are internal contracts.
Existing original declarations are reused where applicable; the narrow CPU
contract avoids leaking original CPU macros into the VM adapter. Declaration
consolidation does not add forwarding functions, duplicate state or change the
original device ABI. Lib and Common remain product-independent; neither may
import App, VM, Compat or MVDM.

`lib/` is the canonical checked-in shared-library corpus, not a runtime or
build dependency on NXVM or NTVDM64. NXVM adopts this corpus exactly. It
consumes and produces copied host values only. It owns
the generic mailbox mechanics, independent console/window message loops,
host-input normalization, registered-chord matching, mouse capture, clock,
synchronization,
and storage primitives. Common machine owns its executor queue; VM converts
copied input to the original machine protocol; app supplies product command
and hotkey policy to Common session. SoftPC publishes each admitted shared-library change as the
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
reenter this control-thread API. `app/command` owns debugger selection
and copied prompts; machine state changes do not select or exit the CLI.

Debug owns growable command output; its public result borrows that text until
the next producing call/open/destroy. The App binding forwards it as Session's
borrowed additional text, valid until the next provider invocation. Session
consumes it synchronously through its existing output/reader transaction with
chunked LF normalization. No second output owner or input loop is introduced.

T56's owner-approved debug port adds observation calls at original CCPU
instruction entry and successful completion, including interrupt-shadow
bypasses. The product driver owns the plan/result and binds it only during
its executor run. A hit parks that same executor through its existing callback;
the existing copied PAUSED fact reaches session, whose debug provider reads
the result through the paused rendezvous. No per-instruction frontend events,
second executor or guest TF/DR ownership are introduced. CCPU refetches after
the pause so edited CS:EIP/code/translation are observed. Cancellation uses the
existing command wake; pause, stop/reset and debugger close clear the plan.
CPU operand notifications in `c_page.c` copy successful accesses into the same
driver-owned instruction record. Retirement commits watch hits; the next
instruction boundary parks the executor. Three independent watch addresses and
bounded copied observations use the existing result query, not a second event
queue or instruction sink. Fault candidates are discarded before the next
instruction; internal translation/device/debugger reads are outside this boundary.

Shared KVM key events are copied `kvm_key`, physical scan, neutral injection
flags, generic Ctrl/Alt/Shift state, and make/break values. Platform adapters
translate native records before the event reaches the shared contract; only
the VM guest binding may translate that neutral value to a product's
guest-input protocol.

The common-machine executor is the sole caller of the injected machine driver
and compatibility host. Input producers enqueue records and signal it. The
executor publishes complete text or graphic frame snapshots; frontends consume
only those snapshots.

## Shared Console And KVM Composition

Common is platform-independent source: it has no platform subdirectories,
native API/types or OS-selected implementation branches. It owns its queues
and state machines through public Lib Base mutex/event/task/wait contracts and
Types atomics. Its complete manifest and source/build dependency gate travel
with the corpus and run independently of the importing product.
Shared unit suites live in test/common and test/lib, each with its own CMake
entry and manifest. They require only the four-directory transfer set and
the toolchain/system libraries, never importing-product adapters or firmware.

SoftPC's checked-in `lib/` corpus is the shared-library source of truth. NXVM
adopts it exactly; no runtime or build dependency crosses repositories.

`types` defines universal copied scalar/status values and header-only external
C/compiler/platform vocabulary. Platform declaration headers contain no
component policy; base owns synchronization/clock composition, and kvm-base owns input
interpretation. Component platform implementations are selected by the build,
not by a generic types dispatcher. `console` defines
copied logical Console objects. `console-broker` owns native Console
handles/modes, one I/O worker, and exactly one Current Console Object from
broker creation to destruction; replacement is transactional. Shared KVM is
split into `kvm-base` (copied KVM values, one event-construction path,
private-mailbox helpers, and source-local generic hotkey matcher),
`kvm-window` (one Window lifecycle), and `kvm-console` (one VM Console lifecycle).
The latter creates an optional logical VM Console object but neither KVM
component opens or registers the process Console. `common/ui` owns the monitor
object, decides which KVM components exist from injected actions and asks the broker
to replace the current object. SoftPC app policy derives and injects those
actions; common/ui does not interpret their product meaning.

The library's only direct component edges are:

```text
types    -> base + console + console-broker + storage + kvm-base + kvm-window + kvm-console
base     -> console + console-broker + kvm-base + kvm-console
console  -> console-broker + kvm-console
kvm-base  -> kvm-window + kvm-console
```

There is no other library edge and no aggregate KVM target. In particular,
`kvm-window` does not depend on `console` or `console-broker`, while `console-broker` does not
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

Common session control is the sole product-state writer. VM, broker, and KVM
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

The common control FIFO transports a consumer-owned nonzero kind and bounded
opaque payload. Only STOP is a shared terminal transport marker; Window owns
its title/freeze/release encoding and worker validation. Console has no ordinary
control commands and rejects unknown records. Native leaf APIs remain typed;
there is no application-facing raw control mailbox.

Shared frame mailboxes copy opaque values into fixed leaf-owned storage.
Window accumulates unconsumed dirty bounds with its latest complete image
under the frame lock; Console stores common text fields and caller-supplied
character maps only. Common machine composes the upstream publication, while
VM alone supplies SoftPC character meaning. Capture does not consume; a successful-output
acknowledgement clears only that still-current publication. The broker reports logical
Console activation through its neutral event sink after binding succeeds;
kvm-console only wakes its existing worker to draw pending content. Empty means
no output, and NOT_CURRENT retains pending content without a retry loop.
Before native activation, the existing broker binding helper synchronously sends
INPUT_RESET through the logical Console after old-reader quiescence. kvm-console
clears its held keys, incomplete text and mouse baseline, retaining registrations
and pending frames. No new input can precede that reset. Rollback uses the same
helper; RESET is not activation success or permanent KVM source retirement.
Cooked rollback restores only an unfinished line request observed after reader
join; ordinary cooked activation remains explicitly armed by the caller.
Common session alone holds pending_line through consumption of the copied line
event. Provider prompt readiness is level-triggered; notifications and explicit
requests are handled even while that line is pending. Frame events drive display,
not input admission. Notification output cancels and joins an unfinished cooked
read through the broker's existing retirement path; completed queued commands survive.
Common UI forwards this operation without new state. Confirmed broker handoff
also clears pending_line, never a desired-state change alone.
Win32 broker isolates raw frame output from the original cooked screen buffer.
The broker selects and restores native display metadata inside its existing
output transaction, before reader startup; same-mode replacements do not switch
screens. Frame output does not shrink the native window to its fixed cell grid.
The original cooked text/cursor/scrollback remain native-owned, not copied or
reconstructed by common/UI. Broker disposal restores that original buffer.
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

Logical Console metadata/event/output gates, KVM frame/control and broker
output/transaction locks use Base blocking mutexes. Each consumer owns its lock
instances, scope and order; Base owns the primitive implementation, not handoff
policy. No callback may synchronously reenter binding/destruction. Storage
owns each CRT stream directly; medium and writer embed file state, while
owned-byte reads use a stack file. Close consumes the stream, not its container.

Pure Window geometry, cursor rectangles, frame pixel conversion and relative
motion scaling belong to kvm-window root helpers. Native files marshal SDK
values and own actual drawing/messages/capture. Worker context and frame share
one allocation; native cleanup runs on the worker, storage release after join.
Base event is one opaque platform allocation, not a pointer-only outer wrapper.
KVM destruction performs one bounded 5000 ms join. An unjoinable live worker is
an application-terminal infrastructure failure; Lib never terminates the
process or exposes a half-object recovery protocol. A KVM notification failure
after copied work has been accepted reports once through the existing failure
sink; it is not a second request rejection and is never replayed. Window blinking uses
one native timer message path, including native modal loops, not outer-loop
timeout scheduling.
Task owns cancellation and entry/context, embedded in one platform allocation.
The native thread directly invokes that entry; root task destroy joins before
releasing cancellation and disposing the allocation. Cancellation/join/destroy
return status; failure retains ownership. Common machine propagates shutdown
failure so its owner cannot release live callback targets or the driver.

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
