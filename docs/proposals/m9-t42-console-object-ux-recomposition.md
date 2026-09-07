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
2. Recompose `ux` as independently enableable Window and Console presenters,
   with separate frame mailboxes, completed presenter-set facts, an optional
   UX-owned Console object, Window-close request delivery, and no host or
   SoftPC dependency.
3. Rework only non-MVDM SoftPC code into the sole policy/reconciler owner:
   derive presenter set, current Console object, and VM lifecycle from
   configuration, completed facts, frame route, and FIFO intent; own monitor
   parsing, `console_control`, DISPLAY policy, CAP semantics, and X policy.
4. Prove the state and ordering matrix with deterministic completion-gated
   fakes before real-thread boundary tests. Build/test both package widths and
   obtain owner runtime acceptance.
5. Submit the generic `src/lib` candidate to NXVM, require its byte-identical
   adoption, then import and manifest-verify the adopted corpus in SoftPC.

## Serial Subtasks

### S1 — Decomposition and implementation-boundary audit

Inspect the actual T41 baseline and produce this S plan before any code change.
For every existing Console reader/writer, presenter owner, input sink,
lifecycle callback, and relevant test, assign one destination S or explicitly
record it as unaffected. Cross-check against the current UI/architecture
authorities and prove that MVDM is excluded. **Exit:** owner-readable source
inventory, dependencies, and acceptance gates for S2–S8; documentation checks
pass.

### S2 — Base logical Console object

Add only the platform-neutral copied-event/output object in `lib/base`, with
its event-sink, output, lifetime, rejected-line, and current-object result
contracts. No native handles, reader thread, UX presenter, SoftPC monitor, or
policy may appear here. **Exit:** base-only unit tests prove copied ownership
and API failure behavior; public-header/CMake boundaries are explicit.

### S3 — Host current-Console broker

Build the generic host broker over S2: initial monitor registration, one
Current Console Object, raw/cooked native modes, transactional replacement and
rollback, serialized output, strong references, binding-generation stale-event
suppression, and broker teardown. **Exit:** controllable blocked-reader/
callback tests prove every ordering/failure case without sleeps. UX and SoftPC
remain unchanged.

### S4 — UX dual-presenter composition

Refactor only shared UX to independently enable Window and Console presenters,
separate Window/Console frame mailboxes, optional UX-owned Console object,
completed presenter-set/configuration facts, Window-close-request delivery,
and local mouse/input rules. UX includes `base` only, never `host` or SoftPC.
**Exit:** controllable-runner tests prove creation, destruction, stale-source
rejection, no live empty set, and permanent-retirement input reset.

### S5 — SoftPC monitor and derived-state reconciler

Replace the old non-MVDM Console handoff with SoftPC's sole FIFO control
thread/reconciler. It owns `display`, `console_control`, monitor parsing,
CAP/X intent, VM run generation, current frame route, host replacement calls,
and presenter lifecycle ordering. **Exit:** pure derivation and
completion-gated fake tests prove the complete approved state/transition matrix
without running a native Console.

### S6 — Integration and concurrency boundaries

Connect S3–S5 in the actual non-MVDM runtime. Add only narrow real-thread
barrier/event tests for reader replacement during callback, destruction racing
queued input, and raw/cooked switching; no timing sleeps. **Exit:** no second
reader/output path remains, source audit is clean, and both widths build/test
to the extent available on the host.

### S7 — Owner runtime acceptance

Produce the two package widths without touching `softpc.ini` or media, then
run the approved Console/Window acceptance matrix with the owner. **Exit:**
owner accepts runtime behavior or records bounded defects; no design change is
silently folded in.

### S8 — NXVM adoption and re-import closure

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
| `src/lib/ux/presenter.{h,c}` and `ux/internal/presenter_internal.h` | one scalar Console-or-Window target and requested, rather than completed, state | S4 replaces this surface with completed presenter-set facts and separate frame mailboxes |
| `src/lib/ux/win32/{runner,console,window,mailbox,actions,input,mouse}.*` | one sequential runner opens `CONIN$` itself and changes native modes; Window X currently directly controls runner result | S4 makes UX own only presenters/optional logical UX object and emit copied events; S3 alone owns native Console I/O |
| `src/lib/ux/linux/**` | parallel public UX implementation must retain the same API shape, although Linux runnable parity is deferred | S4 supplies compile-time placeholder/API parity only; no Linux runtime acceptance in T42 |
| `src/app/main.c` | synchronous `fgets` monitor owns lifecycle loop and shares process Console implicitly | S5 creates SoftPC monitor object/sink and moves command parsing into the sole control queue; cooked terminal behavior becomes host S3 |
| `src/app/{runtime,presentation}.{c,h}` | product frame router, direct `ux_run`, action callbacks, lifecycle calls, title/capture changes interleave synchronously | S5 creates the sole derived-state reconciler and run-generation envelopes; it becomes the only host/UX/VM composer |
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
  -> S4 UX dual presenters
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
