# System Architecture

## Target Ownership

The former single X86 tier is split into x86-debug and x86-ibmpc,
with matching independent test packages. x86-debug depends only on
Lib and Emulator; x86-ibmpc adds the debugger dependency and owns existing PC
product interaction. NXVM-specific speed/info contracts are not part of this
repository's reusable packages. No machine/device implementation is duplicated.
This is the owner-admitted decomposition of the existing tier, not permission
for inward packages or their tests/configuration/docs to know outward owners.

```text
app-softpc/product/main             entry/config validation -> x86-ibmpc/product
app-softpc/product/composed_machine private machine/driver composition
app-softpc/product/config, extensions  INI and snapshot/media commands
x86-ibmpc/product                        debugger and X86 CLI/hotkey policy
emulator/product                   neutral monitor, assembly and session lifetime

emulator/session -> emulator/ui       control and monitor/KVM composition
emulator/session -> emulator/machine  sole generic executor and copied facts
x86-debug/product + x86-debug/xasm32      shared debugger/assembly contracts
                         |
                 injected existing driver callbacks
                         v
src/app-softpc/machine               SoftPC input/frame/debug/backend adaptation
                         |
src/app-softpc/compat                original host callbacks and ABI support
                         |
src/app-softpc/softpc.new            original CPU, controllers, renderer and firmware

Lib supplies shared platform mechanics to the owning consumers.
No app -> Compat/MVDM edge; only composition -> VM; no VM/Compat -> app edge.
```

`app-softpc/softpc.new` is the repository-owned selected recovered-machine layout.
It retains original relative paths and names. T92 upgrades the routine baseline
to the necessary subset of XP SP1 revision
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`; OpenNT comparisons remain frozen
migration lineage. The [current mirror manifest](../etc/evidence/softpc/xp-sp1-current-manifest.tsv)
separates upstream, selected-subset and local identity. Unneeded machine/host
branches may be excluded within files, with reviewed ranges; real local repairs
remain visible patches. No external checkout is a build or test dependency.
Narrow compiler, declaration, calling-ABI
and pointer-representation corrections may be direct, source-visible diffs at
the affected point when they remain mechanical and introduce no machine policy.
`app-softpc/compat/` owns original host callbacks, host resources and port ABI adaptations,
but no x86 machine lifecycle/configuration or guest-visible device state.
`emulator/session` is the sole owner of the product-neutral control queue,
desired/actual reduction, prompt scheduling and dispatch order. It receives
the SoftPC CLI and machine adapter as injected callbacks; it does not parse
configuration or interpret machine internals. `emulator/ui` is the sole owner
of the monitor logical Console, broker, raw VM Console and Window/KVM
instances; it receives only copied product policy and returns copied events.
`app-softpc/product/` owns SoftPC configuration parsing, private machine
composition, resources and its snapshot/media command extension. Shared
`x86-ibmpc/product` owns X86 CLI/debug semantics and hotkey policy; it delegates
neutral monitor, entity assembly, session execution and teardown to
`emulator/product`. App passes an already composed
private machine/driver lifetime and the two UI choices; shared X86
does not parse configuration or see media, memory or model fields.
At exit it synchronously shuts down the machine worker with callback targets
still alive, then destroys UI, session, command/debug, machine and VM in order.
Machine shutdown and destroy share one stop/join path; shutdown retains storage
so referenced objects can be released safely before the machine itself.
UI teardown stops the broker and unbinds output before joining KVM producers;
failure preserves UI and remaining callback dependencies. App treats that failure
as terminal rather than releasing Session beneath a live producer.
Only app-softpc/product/composed_machine.c consumes machine/vm_interface.h; no shared X86 source consumes Compat or
the original mirror. `app-softpc/machine/` owns the concrete machine backend, initialization/reset/teardown
sequence, driver, frame/input conversion and debugger request preflight.
Its implementation calls Compat and the original machine while its
public interface exposes only copied options and Emulator/Lib contracts.
Compat never calls app or Emulator. `emulator/machine` owns the
single generic executor, request/input queues, run generation and bounded-frame
publication; its injected VM driver calls the SoftPC machine boundary.

The GFI host adapter distinguishes the physical drive from its inserted image.
First hardware initialization installs A: (default 1.44 MB when empty, or the
existing initial-image profile); B: remains absent. Insert/eject changes media,
not physical identity; reset preserves both and teardown removes the drive.
The Core device archive carries physical A/B types independently of media
presence. Original FDC/BIOS logic consumes these host capabilities; no frontend
or guest-version-specific identity correction is involved.

### x86 Build And ABI Boundaries

Core is only a source grouping of Machine, Compat and softpc.new, not a new
runtime layer. The relocation preserves the original mirror byte-for-byte.
Only the original device target privately searches src/app-softpc for its existing
compat/... includes. App and shared components acquire no additional include root.
The preserved-corpus spelling in Coding Rules and historical evidence refers
to this same corpus at its current app-softpc/softpc.new location.

These are ownership boundaries, not six mandatory forwarding layers. Device
I/O follows original MVDM host callbacks into Compat and then Lib storage;
Emulator machine schedules execution, not individual disk reads/writes. Original
CPU debug observations call the VM observer symbols without importing Emulator
or App implementation. Such callbacks do not transfer CPU state ownership.

All VM C sources belong to one `softpc-vm` OBJECT target. App and x86
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
original device ABI. Lib and Emulator remain ISA-independent; none may
import App, VM, Compat or MVDM.

`lib/` is the canonical checked-in shared-library corpus, not a runtime or
build dependency on NXVM or NTVDM64. NXVM adopts this corpus exactly. It
consumes and produces copied controls/input and bounded immutable frame references. It owns
the generic mailbox mechanics, independent console/window message loops,
host-input normalization, registered-chord matching, mouse capture, clock,
synchronization,
storage primitives and bounded PCM playback. Emulator machine owns its executor queue; VM converts
copied input to the original machine protocol; app supplies product command
and hotkey policy to Emulator session. SoftPC publishes each admitted shared-library change as the
canonical corpus for NXVM to adopt exactly; the projects do not maintain
parallel variants.

`x86-debug/xasm32` is an imported x86 copied byte/text assembly capability and
`x86-debug/product` is an imported x86 debug command capability. SoftPC exposes
it through its injected app command binding, not through a second input loop.
`x86-debug/product` depends on `emulator/machine`'s optional paused-state adapter and
on `x86-debug/xasm32`; neither component may create an executor, own a Console,
or add a product command path. Import hashes remain provenance evidence; the
S9 integration changes to these components form the updated downstream corpus.
The control thread serializes lifecycle, media and synchronous debug requests.
Debug requests are copied into the machine's existing command rendezvous and
executed by the parked executor, never by the calling frontend. The machine
checks paused state and the lease again there; callbacks must not synchronously
reenter this control-thread API. `x86-ibmpc/product/command` owns debugger selection
and copied prompts; machine state changes do not select or exit the CLI.
Machine copies opaque, pointer-free debug requests/results through one fixed slot
(128-byte request, 1536-byte response), with explicit sizes and no per-request
allocation. It knows no register, address or operation schema. The independent
`x86-debug/product/protocol_interface.h` owns x86 vocabulary; frontend and VM adapt at
their existing boundaries using aligned local values. Driver validation owns
protocol sizes and operation limits. Failure returns zero response length and
does not change caller output bytes; a failed wait requires shutdown before
reusing the slot. Plan cancellation remains distinct from request completion.

Debug owns growable command output; its public result borrows that text until
the next producing call/open/destroy. The X86 command binding forwards it as Session's
borrowed additional text, valid until the next provider invocation. Session
consumes it synchronously through its existing output/reader transaction with
chunked LF normalization. No second output owner or input loop is introduced.

T56's owner-approved debug port adds observation calls at original CCPU
instruction entry and successful completion, including interrupt-shadow
bypasses. The x86 driver owns the plan/result and binds it only during
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

Shared KVM key events are copied `base_key`, physical scan, neutral injection
flags, generic Ctrl/Alt/Shift state, and make/break values. Platform adapters
translate native records before the event reaches the shared contract; only
the VM guest binding may translate that neutral value to an architecture's
guest-input protocol.

The emulator-machine executor alone invokes state-access/execution driver hooks
and compatibility work. Control callers may also invoke the driver's thread-safe
stop/wake signals. Input producers enqueue records and signal it. The
executor publishes complete text or graphic frame snapshots; frontends consume
only those snapshots.

## Shared Console And KVM Composition

### Approved frame-transport migration

T96 uses bounded ownership circulation: both leaves use Base's fixed four
producing/pending/processing/displayed roles. Displayed remains immutable until
processing succeeds. Session/UI route without an
additional historical payload owner. Unconsumed replacement and completed
retirement return storage for reuse; controls/input remain copied values.
Existing native surface remains separate. The source change is under qualification;
runtime delivery and acceptance are recorded in CURRENT, not inferred here.

### Current implementation

Emulator is platform-independent source: it has no platform subdirectories,
native API/types or OS-selected implementation branches. It owns its queues
and state machines through public Lib Base mutex/event/task/wait contracts and
Types atomics. Its complete manifest and source/build dependency gate travel
with the corpus and run independently of the importing x86.
Shared unit suites mirror lib, emulator, x86-debug and x86-ibmpc
under test/. These four src/test pairs form eight reusable directories.
Each corpus/suite owns its build and manifest; none may
require an importing application's adapters or firmware. Each test package owns its
fixtures; it may consume inward production APIs but never another package's
tests or an outer package. Generic test tools live directly in test/. The
dependency rule is direct: Lib has no outer dependency; Emulator consumes Lib;
x86-debug consumes Lib and Emulator; x86-ibmpc also consumes x86-debug.
This rule is checked at public-header and
CMake target-link boundaries.

SoftPC's checked-in `lib/` corpus is the shared-library source of truth. NXVM
adopts it exactly; no runtime or build dependency crosses repositories.

`types` defines universal copied scalar/status values and header-only external
C/compiler/platform vocabulary. Platform declaration headers contain no
component policy; base owns synchronization/clock composition and neutral input
definitions/native decoding, audio/pcm owns PCM output, and kvm-base owns
hotkey policy, held-key state, synthesis and event delivery. Component platform implementations are selected by the build,
not by a generic types dispatcher. `console` defines
copied logical Console objects. `console-broker` owns native Console
handles/modes, one I/O worker, and exactly one Current Console Object from
broker creation to destruction; replacement is transactional. Shared KVM is
split into `kvm-base` (copied KVM values, one event-construction path,
private-mailbox helpers, and source-local generic hotkey matcher),
`kvm-window` (one Window lifecycle), and `kvm-console` (one VM Console lifecycle).
The latter creates an optional logical VM Console object but neither KVM
component opens or registers the process Console. `emulator/ui` owns the monitor
object, decides which KVM components exist from injected actions and asks the broker
to replace the current object. SoftPC app policy derives and injects those
actions; emulator/ui does not interpret their x86 meaning.

The library's only direct component edges are:

```text
types    -> base + console + console-broker + storage + audio/pcm + audio/midi + kvm-base + kvm-window + kvm-console
base     -> console + console-broker + storage + audio/pcm + kvm-base + kvm-window + kvm-console
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

`audio/pcm` owns a bounded copied-PCM FIFO and one stream worker that drains it
through the native output adapter. The Windows adapter owns one shared WASAPI
endpoint and waits on its event or cancellation; it adds no second worker.
Lib owns neither synthesis, emulator clock nor guest-device protocol. Compat's
speaker worker supplies synthesized PCM and owns tone timing; an output failure
ends that worker with one diagnostic, while shutdown cancellation stays silent.
There is no automatic retry or restart. A native-output failure is observable
to the consumer; no platform silently discards samples as successful playback.

`audio/midi` accepts complete short and bounded copied SysEx messages. Its
Windows owner submits stable headers directly to WinMM and waits on completion
or cancellation events, without an additional worker or timed polling. It
does not own guest byte parsing, synthesis or instrument banks. MIDI volume
is an optional host-device capability, unlike stream-local PCM gain.
Neither audio leaf depends on the other audio leaves.

All cross-component contracts are declared at the owning component root.
Each component's `win32/` and `linux/` files are exclusively its own platform
implementation; neither sibling components nor root forwarding headers may
include them. The exception is `types/{win32,linux}`, which supplies shared
external vocabulary to matching platform sources. Native input decoding is
declared by Base's adapter support interface and implemented only by Base;
callers never reach its platform headers. KVM owns neutral event delivery and
text synthesis; Window-only message decoding and key-state queries remain in
kvm-window. Emulator consumes neutral KVM events, not native records. The
Base adapter support interface is not an application input API.

Emulator session control is the sole product-state writer. VM, broker, and KVM
workers only enqueue copied events/completions to its emulator-owned queue. The control thread
derives runtime commands independently from the required component instances,
using config, frame route, and completed actual state; a derived action is
control-private and is never a shared VM/presenter intent. Runtime owns the
internal sequence of every accepted command, including reset, and reports its
one completed public fact. VM `run_generation` is SoftPC-only. Component
readiness is returned by each component's own creation/start contract; shared
KVM carries no global configuration generation.

Machine's input queue mutex orders admission against the executor's pause
cutoff. Pause/debug stop freeze ordinary input delivery without draining or
discarding pending events. Genuine resume preserves FIFO order at the normal
one-event-per-safe-callback cadence. Internal paused state-read continuation
does not reopen input. Driver snapshots contain machine/device state, not
pending host FIFO events; cold start/reset and successful state replacement
clear the previous run's queue. State refusal is distinct from queue overflow.
Pause does not promise to preserve ordinary host input transitions or reconcile
host and guest key state on resume. Session retains local keyboard-release and
source-lifetime bookkeeping, not a second guest-input delivery path. Mouse
input has no Session ledger and remains running-only.

SoftPC passes copied `{chord, identifier}` registrations to each KVM component.
The components may generically recognize and suppress a registered chord, but
only enqueue `KVM_HOTKEY(identifier)`; they never execute a x86 callback or
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

Every `kvm-window` and `kvm-console` instance owns a private control mailbox
and its own native worker(s), and borrows a producer-owned frame exchange.
Their transport mechanics live in `kvm-base`; control mailbox instances remain
private, while the opaque frame exchange carries immutable held references. SoftPC invokes the specific
component API it has chosen; components communicate back only through the
copied input-queue entry supplied at creation.

The emulator control FIFO transports a consumer-owned nonzero kind and bounded
opaque payload. Only STOP is a shared terminal transport marker; Window owns
its title/freeze/release encoding and worker validation. Console has no ordinary
control commands and rejects unknown records. Native leaf APIs remain typed;
there is no application-facing raw control mailbox.

Machine owns a four-slot Window exchange and a four-slot Console exchange.
Window slots contain a Lib-defined reference (sequence and exactly one text/
graphics pointer) and reusable payload backing. Console slots contain text and
character maps, without bitmap fonts. No Machine frame wrapper exists.
Only slot metadata exists at exchange creation. Each leaf prepares its payload
at the producer's actual dimensions and stride; text storage never reserves
graphics pixels. Equal specifications reuse allocations. The exchange invokes
the leaf retirement callback only on unheld slots; incompatible old payloads
are freed when their reader releases them. No collector, polling or second
ownership mechanism is used. Allocation errors do not publish a partial frame.
Its sole executor passes both writable leaf storage areas directly to the driver;
Session/UI hold no payload and forward exchange references. Product composition
injects UI's pure status-page builder for graphics-mode Console production into
the same executor path. Stop Machine before destroying that immutable context.
Base moves pointers,
with no frame update callback or per-publication allocation/copy. Window derives
damage on consumption by comparing resolved colours with its existing RGB
surface; it updates changed pixels and invalidates their enclosing rectangle.
Machine publishers resolve graphics into numeric 0x00RRGGBB words before crossing
the Window ABI; source palettes stay machine-private. Text decodes its own palette
and graphics reads direct RGB, both using the same comparison helper. First
frames and recreated surfaces invalidate fully; same-size mode changes compare
actual pixels. Cursor overlays invalidate their old/new rectangles separately.
Native invalidations accumulate until paint. No previous-frame cache is added.
Console stores emulator text fields and caller-supplied character maps only.
Shared text uses one array of five-byte cells, each holding a glyph index,
glyph bank, foreground/background palette indices and underline enable. VM alone decodes device colour/bank bits; KVM leaves select resources
from explicit independent values. Logical Console also carries separate colour
indices; only its native broker packs them for the host API. This changes no
mailbox, readiness, graphics or lifecycle policy.
Emulator machine publishes every complete driver-produced frame, preserving
no-frame readiness but performing no content comparison. Dirty/readiness belongs
to the driver; a driver without dirty tracking may always produce. Machine
and Window use the same pending authority: an older notice may select the latest
complete frame without losing damage. VM alone supplies SoftPC character
meaning; original producer dirty does not cross the KVM frame ABI.
Take pins processing while releasing the pending slot. Both leaves retain old
displayed input until successful finish, then retain its replacement through
idle time. Console compares mapped cells against that old input; the broker
keeps only geometry/palette/tail metadata, not duplicate cell arrays. Native
recovery overrides cell reuse. Failure requeues only an unsuperseded input; it never
replaces newer pending content. The broker reports logical
Console activation through its neutral event sink after binding succeeds;
kvm-console only wakes its existing worker to draw pending content. Empty means
no output. Unbound Console output succeeds without native drawing and retains
that input. Activation requeues held input if no newer frame is pending, through
the existing worker/output path, without another producer publication.
Before native activation, the existing broker binding helper synchronously sends
INPUT_RESET through the logical Console after old-reader quiescence. kvm-console
clears its held keys, incomplete text and mouse baseline, retaining registrations
and pending frames. No new input can precede that reset. Rollback uses the same
helper; RESET is not activation success or permanent KVM source retirement.
Cooked rollback restores only an unfinished line request observed after reader
join; ordinary cooked activation remains explicitly armed by the caller.
Emulator session alone holds pending_line through consumption of the copied line
event. Provider prompt readiness is level-triggered; notifications and explicit
requests are handled even while that line is pending. Frame events drive display,
not input admission. Notification output cancels and joins an unfinished cooked
read through the broker's existing retirement path; completed queued commands survive.
Emulator UI forwards this operation without new state. Confirmed broker handoff
also clears pending_line, never a desired-state change alone.
Win32 broker isolates raw frame output from the original cooked screen buffer.
The broker selects and restores native display metadata inside its existing
output transaction, before reader startup; same-mode replacements do not switch
screens. Frame output does not shrink the native window to its fixed cell grid.
The original cooked text/cursor/scrollback remain native-owned, not copied or
reconstructed by emulator/UI. Broker disposal restores that original buffer.
Window displacement retains integer remainders
internally without changing the copied input ABI. Post-start worker exits share
one failure/retirement cleanup path; startup failure remains distinct. Unexpected
Console I/O failures are copied events, not x86 decisions. Broker replacement
includes old-binding cleanup before the next transaction can enter. Types owns
external declarations; actual component consumers own OS linkage requirements.

## BOP And Firmware Boundary

Original BOP instruction decoding and `BOP FE` control flow remain in the
original CCPU. The compatibility host implements only the finite hardware and
firmware BOP services actually reached by the selected ROM. DOS, WOW, NTVDM,
and x86-service selectors remain unavailable.

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
Ordinary control does not wait for rendering or frame observation.

Logical Console metadata/event/output gates, KVM frame/control and broker
output/transaction locks use Base blocking mutexes. Each consumer owns its lock
instances, scope and order; Base owns the primitive implementation, not handoff
policy. No callback may synchronously reenter binding/destruction. Storage
owns each CRT stream directly; medium and writer embed file state, while
owned-byte reads use a stack file. Close consumes the stream, not its container.

Pure Window geometry, cursor rectangles, frame pixel conversion and relative
motion scaling belong to kvm-window root helpers. Native files marshal SDK
values and own actual drawing/messages/capture. Worker context borrows processing/displayed frames from the producer exchange;
native cleanup runs on the worker, context storage release follows join.
Base event is one opaque platform allocation, not a pointer-only outer wrapper.
KVM destruction requests terminal stop, uses a dedicated terminal wake distinct
from ordinary mailbox notification, waits for the worker to exit, then releases
its context and callback dependencies. A KVM native stop/join failure that leaves
worker exit unconfirmed terminates the process without application callbacks.
Every normal KVM destroy return consumes the object; post-join cleanup errors
are reported while independent resources are still released. A KVM notification failure after copied work has been
accepted reports once through the existing failure
sink; it is not a second request rejection and is never replayed. Window blinking uses
one native timer message path, including native modal loops, not outer-loop
timeout scheduling.
Task owns cancellation and entry/context, embedded in one platform allocation.
The native thread directly invokes that entry; root task destroy joins before
releasing cancellation and disposing the allocation. Cancellation/join/destroy
return status; failure retains ownership. Emulator machine propagates shutdown
failure so its owner cannot release live callback targets or the driver.

Synchronous request rejection returns `lib_status`, with normal wait outcomes
separated into output values. Ordinary synchronization and local cleanup stay
inside their owning component. A failed public create leaves its output null;
failed file close consumes the stream and cannot be retried. Expected local
outcomes remain local; Base task/native-reader ownership failures still use
the application's status boundary. Accepted KVM requests report
asynchronous failure through the existing control-queue sink. No alternate
event path exists.

Native Window and Console-broker records use one Base decoder for physical-
versus-text choice and surrogate processing. Console publishes neutral values;
both KVM leaves use the same neutral delivery entry. Native Window
translation is requested only for unmapped transitions, so physical input has
no duplicate character stream. Text synthesis borrows the existing matcher
ledger and releases only keys it introduces, never separately tracking held
keys. One source-local held-key ledger owns pending/delivered/consumed
lifetimes for ordinary keys and modifiers alike. Repeats reuse their state;
a delivered make cannot retrospectively become a consumed chord.

Native adapters copy repetition metadata without expanding UTF-16 units.
Base owns complete-character decoding and malformed-prefix recovery. KVM owns
repetition and TEXT fallback when physical mapping is unavailable. Physical
identity is PC set-1, with zero indicating unavailable, not a native OS scan
contract. Console raw events and KVM events share the Base-owned key vocabulary.
