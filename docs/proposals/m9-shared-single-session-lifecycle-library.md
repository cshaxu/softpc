# M9: Shared Single-Session Lifecycle Library

## Purpose

Extract the policy-neutral lifecycle mechanics common to a single bootable
machine session into a local synchronized component. SoftPC is its first
consumer; NXVM may later use the same component once per independently
constructed VM session. NXVM's existing multi-session catalog remains above
this component and is neither moved nor required by SoftPC.

The component makes one executor, its copied command boundary, state
transitions, wake/stop/join ownership, and failure-atomic cleanup consistent
across projects. It is not a machine emulator, a profile system, a frontend,
or a product console.

## Target Shape

```text
SoftPC fixed-machine monitor                 NXVM session catalog / Console
              |                                          |
        SoftPC machine adapter                       NXVM session adapter
              |                                          |
              +-------- one shared single-session lifecycle --------+
                                       |
                            product machine execution adapter
                                       |
                 SoftPC CCPU + original host       NXVM core/platform runner
```

SoftPC retains exactly one session. NXVM may create and manage multiple
independent single-session instances, selecting or running them according to
its own product policy. The common component does not assign session IDs,
store a selected session, enumerate sessions, or impose serial execution.

## Required Boundary

The library owns only copied host values and one executor's mechanical state:

| Library owns | Product adapter owns |
| --- | --- |
| Opaque single-session handle and legal state transitions. | Machine/session construction and destruction. |
| `READY`, `STARTING`, `RUNNING`, `PAUSED`, `STOPPING`, `STOPPED`, and `FAULTED` state publication. | The guest-visible meaning of reset, pause, resume, stop, shutdown, and fault. |
| One execution owner, lifecycle-command queue, wake event, stop request, join, and idempotent finalization. | The safe machine boundary at which queued commands may be serviced. |
| Copied command/result records and failure outcome storage. | Media commands, debugger commands, speed policy, configuration, and validation. |
| A controlled run-turn loop that never touches machine state itself. | `reset`, `run_turn`, `request_wake`, input delivery, command service, and copied-frame production. |

The adapter contract must be opaque and product-neutral. Conceptually it
needs callbacks equivalent to:

```c
reset(context);
run_turn(context);                 /* returns at a product-proven safe edge */
request_wake(context);
submit_input(context, event);
service_command(context, command);
copy_frame(context, frame);
request_stop(context);
destroy(context);
```

The exact ABI is an admission decision. It must carry no raw CPU, RAM, VRAM,
controller, renderer, SoftPC CCPU, NXVM Core, or platform-runner pointer
across the library boundary.

## SoftPC Binding

SoftPC's adapter preserves the recovered execution model:

- CCPU `BOP FE` returns to its current outer `host_simulate` frame; it is not
  a guest stop. The adapter decides when to enter the next original run turn.
- Original SoftPC timer, quick-event, interrupt, controller, BIOS, ROM, BOP,
  and media ordering remain in the recovered machine and compatibility host.
- The existing fixed `softpc.ini`, one monitor, one fixed machine, original
  keyboard/InPort delivery, and presentation binding remain product code.
- The library may request that the adapter service a copied command at its
  safe edge; it may never mutate a SoftPC controller from a monitor or UI
  thread.

## NXVM Binding

NXVM's adapter preserves its Core/VM separation:

- VM profile selection, ROM/CMOS assets, topology materialization, debugger,
  speed, request transport, and platform execution provider stay in NXVM.
- `core_product_session_manager` remains a higher-level multi-session catalog
  with its own IDs, selection, listing, and product console semantics.
- Each NXVM session may own one lifecycle-library instance. The NXVM manager
  decides whether sessions run concurrently, which is selected, and how a
  close maps to the session adapter's lifecycle request.

## Explicit Non-goals

- No change to `src/mvdm/softpc.new`, its original host contracts, or any
  recovered CPU, C-VID, device, BIOS, ROM, BOP, timer, or media behavior.
- No DOS, DPMI, WOW, VDD, NTVDM, Console Server, or session-service behavior.
- No profile/YAML/INI parser, firmware asset loader, block-device protocol,
  media geometry, debugger, speed governor, or product command parser.
- No replacement or widening of the existing Win32 presentation library; it
  remains a separate copied-frame/input consumer.
- No cross-repository build, runtime, or acceptance dependency. Shared code
  remains locally synchronized and hash-verifiable.

## Migration And Evidence

1. Freeze the exact single-session state graph and command/result ownership;
   distinguish an execution request, a guest stop/fault, and a host teardown.
2. Introduce the generic lifecycle library with no machine-specific includes
   and focused state-transition, cancellation, stop-during-start, pause/resume,
   join, and failure-atomic-finalize unit tests.
3. Bind SoftPC without changing the recovered machine: retain one executor,
   prove CCPU outer-run reentry, keyboard/input continuation, pause/resume,
   media-at-safe-edge, Ctrl+Alt+Del reset, monitor start/stop, and frame
   publication on GCC x64 and x86.
4. Add a source-boundary check proving the library contains no SoftPC or
   NXVM machine/runtime symbols and that `src/app` no longer owns duplicate
   lifecycle thread/event/state machinery.
5. Adopt it in NXVM only under a separately admitted NXVM task. That task
   proves its existing multi-session manager remains above the per-session
   lifecycle layer and does not become a SoftPC dependency.

## Admission Questions

- Does the library expose a synchronous `run_turn` contract, an asynchronous
  adapter-runner contract, or both as separate adapters?
- Which lifecycle commands are truly generic (`start`, `pause`, `resume`,
  `stop`, `destroy`) and which remain typed product commands such as reset,
  media replacement, debugging, or speed selection?
- Should copied frame publication remain solely in the current Win32 mailbox,
  or should lifecycle expose only a generic producer wake hook?
- What source-hash manifest and versioning gate proves future SoftPC/NXVM
  synchronized copies are byte-identical without coupling their builds?
