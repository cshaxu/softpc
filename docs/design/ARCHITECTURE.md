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
lib/{base,host,storage,ux-base,ux-window,ux-console}/
  byte-identical NXVM shared platform library: copied-frame mailbox, host
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
`lib/` is an exact checked-in NXVM import, not a runtime or build dependency
on NXVM or NTVDM64. It consumes and produces copied host values only. It owns
the generic mailbox mechanics, independent console/window message loops,
host-input normalization, registered-chord matching, mouse capture, clock,
synchronization,
and storage primitives. The app binding owns its executor queue, converts
events to the guest's input protocol, and makes all product lifecycle and
action decisions. An owner-admitted generic shared-library candidate may
temporarily originate in SoftPC only when its active packet requires NXVM to
adopt the exact code and requires SoftPC to re-import it before task closure;
this is a delivery order, not a permanent project fork.

The runtime executor is the sole caller of the machine and compatibility host.
Input producers enqueue records and signal it. The executor publishes complete
text or graphic frame snapshots; frontends consume only those snapshots.

## Shared Console And UX Composition

An admitted shared-library candidate may originate in SoftPC only when NXVM
adopts the exact code and SoftPC re-imports it before closure; this is delivery
order, not a permanent library fork.

`base` defines copied logical Console objects. `host` owns native Console
handles/modes, one I/O worker, and exactly one Current Console Object from
broker creation to destruction; replacement is transactional. Shared UX is
split into `ux-base` (copied UX values, one event-construction path,
private-mailbox helpers, and source-local generic hotkey matcher),
`ux-window` (one Window lifecycle), and `ux-console` (one VM Console lifecycle).
The latter creates an optional logical VM Console object but neither UX
component opens or registers the process Console. SoftPC owns its monitor
object and alone decides which UX components exist and asks host to replace the
current object.

SoftPC control is the sole product-state writer. VM, host, and UX workers only
enqueue copied events/completions to its app-owned queue. The control thread
derives the required component instances from config, frame route, FIFO intent,
and completed actual state, then advances one reconciler. VM `run_generation`
is SoftPC-only. Component readiness is returned by each component's own
creation/start contract; shared UX carries no global configuration generation.

SoftPC passes copied `{chord, identifier}` registrations to each UX component.
The components may generically recognize and suppress a registered chord, but
only enqueue `UX_HOTKEY(identifier)`; they never execute a product callback or
interpret pause, reset, guest CAD, or another identifier. Their ordinary input
and matched-hotkey events are `ux_input_event` variants, constructed through the
one `ux-base` utility path and carrying their source component handle for
lifetime tracing only. The cooked monitor instead produces handle-free
`monitor_input_event` values. Both families enter SoftPC's one input FIFO as
distinct payloads in arrival order; the control thread is their sole consumer.
The generic matcher keeps pending only possible chord prefixes inside its own
component instance, preserving guest-input order on mismatch. A Window and a
VM Console each process only their own raw key sequence, so no cross-component
key combination is possible.

Every `ux-window` and `ux-console` instance owns private control/frame/input
mailboxes and its own native worker(s). Their common mailbox mechanics live in
`ux-base`; the mailbox instances remain per-component implementation details,
never public handles or shared UX infrastructure. SoftPC invokes the specific
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
