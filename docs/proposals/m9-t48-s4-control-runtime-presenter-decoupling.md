# M9 T48 S4 — Control / runtime / presenter decoupling

## Original request

Separate SoftPC product control, VM runtime, and shared UX presenters into
three independent subsystems. Each owns only its own queues, APIs, and output
sinks. No lifecycle or presentation decision may be made by a VM callback or
presenter callback.

## Objective

Make SoftPC control the one product-state writer and reconciler. Runtime owns
machine execution and reports copied facts; each presenter owns native display
and input collection; control consumes their events in one arrival-ordered
queue and separately derives VM work and presenter work.

## Target topology

```text
monitor cooked line ─┐
ux-window event ─────┼─> control event queue ─┬─> VM runtime event queue
ux-console event ────┘                        │      lifecycle / guest input
                                                │
VM completion / copied frame ──────────────────┤
presenter / broker completion ─────────────────┤
                                                └─> presenter / broker APIs
```

### Presenter contract

- Every `ux-window` and `ux-console` instance has its own private control FIFO,
  latest-wins frame mailbox, native worker(s), and copied input-source state.
- Control calls only its public API: create/destroy, frame publish, title,
  freeze/unfreeze, mouse operations, and other component-local controls.
- Each component sends copied UX events only through the input sink registered
  by control at creation. It does not read runtime state, access the VM queue,
  parse monitor commands, or execute a product hotkey action.
- This product creates at most one Window and one VM Console today, but control
  treats instances independently and does not encode that cardinality as a
  cross-component rule.

### Runtime contract

- Runtime owns its executor and one private input/command queue.
- Control submits lifecycle requests and already-authorized guest input to that
  queue. Neither monitor nor UX submits directly to runtime.
- Runtime emits copied frame and actual-state completion facts only through
  control-registered sinks. It does not call presenter APIs or write monitor
  text.

### Control contract

- Control owns one arrival-ordered product event queue for monitor input, UX
  input, runtime completion/frame facts, component completion, and broker
  completion.
- Control is the only writer of product state. It maps monitor/hotkey input to
  control requests, then derives independently:
  1. the next runtime command; and
  2. the required presenter set, component controls, and Current Console.
- A reset is a control-private lifecycle plan (`stop → cold start → pause`).
  It exists only to select the next runtime command and classify its internal
  completions. It is not a presenter intent and is never exposed to lib UX.
- A runtime completion is the sole trigger for a human-facing lifecycle
  outcome. Once monitor is actual Current Console, control atomically orders:
  `outcome text → SoftPC> → request cooked line`. Raw VM-Console running routes
  discard monitor-only running outcomes.

## Required migration

1. Retire `app_reconciler_intent` as the shared VM/presenter control token.
   Replace it with a control-owned lifecycle request/plan consumed only to
   issue runtime commands.
2. Make presenter reconciliation a pure derivation of configuration, actual
   runtime state, copied frame route, Window-close fact, component completions,
   and broker completions. It must not receive a lifecycle request.
3. Route every registered hotkey—Window and VM Console alike—through the same
   control request reducer. Hotkey receipt produces no monitor status text.
4. Classify `RUNNING` by prior stable control state, not command provenance:
   `INIT|STOPPED → started`, `PAUSED → resumed`; retain reset's internal-stage
   suppression. Handle paused, stopped, and error by the same outcome path.
5. Preserve all existing shared-lib contracts. This is app orchestration work;
   no new product semantics enter `lib/`.

## Verification

- Deterministic fake-driven control tests cover monitor and both UX sources for
  pause/resume, including CAP; prove only a runtime completion emits status.
- Prove `INIT|STOPPED → RUNNING` emits `Machine started.`, `PAUSED → RUNNING`
  emits `Machine resumed.`, reset suppresses internal completions, and error
  returns monitor text/prompt.
- Prove raw VM Console routes never emit a delayed monitor running outcome;
  monitor routes order outcome, prompt, then cooked-line request.
- Prove no presenter callback and no runtime callback can call another
  subsystem's product API directly.
- Fresh x64/x86 build and applicable full CTest; refresh only the agent-owned
  executable pair. Manual acceptance covers monitor, raw Console, Window, CAP,
  reset, stop/start, and error paths.

## Exit criteria

Control, runtime, and presenters have one-way boundaries and no shared
lifecycle/presentation intent. VM and UX callbacks only enqueue copied facts;
control independently derives runtime and presenter work; lifecycle outcomes
are completion-driven and prompt-safe; dual-width evidence and owner runtime
acceptance are recorded.
