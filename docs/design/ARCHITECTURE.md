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
platform input interfaces. Cross-component support contracts use the same
`_interface.h` naming rule but have an explicitly narrower consumer set.

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
component instance, preserving guest-input order on mismatch. A Window and a
VM Console each process only their own raw key sequence, so no cross-component
key combination is possible.

Every `ui-window` and `ui-console` instance owns private control/frame/input
mailboxes and its own native worker(s). Their common mailbox mechanics live in
`ui-base`; the mailbox instances remain per-component implementation details,
never public handles or shared UI infrastructure. SoftPC invokes the specific
component API it has chosen; components communicate back only through the
copied input-queue entry supplied at creation.

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
