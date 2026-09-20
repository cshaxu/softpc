# T73: Lib/Common x86 Dependency Audit

## Owner Request And Scope

Owner admits a new task outside the existing queue: audit Lib/Common x86
dependencies for reuse by a NEC emulator, using neighboring NNES design as
reference. Confirm whether Lib can transfer unchanged; identify Common bindings.
Requested names are common/x86-debug and common/x86-xasm32. Audit every API and
internal path of common/session, common/ui and common/machine and propose owners.

The preliminary S1 audit was delivered in f13c2780 with production/test +0/-0.
The owner subsequently revised this still-open S1; see the approved plan below.
Baseline ad66b615. NNES is read-only design evidence, not a build dependency or
proof of implemented neutrality. Its optional components outside Common are
not silently substituted for the owner's requested Common subdirectories.

## Finite Audit Ledger And Method

Inventory all tracked source/header/build contracts in src/lib and the three
Common mechanism components, plus debug/xasm public interfaces, build DAG,
tests/verifiers and product callers. Distinguish guest x86 semantics from host
compiler/platform dependencies and bounded KVM device capabilities.

Disposition for each boundary: retain neutral, isolate x86, generalize existing
transport, or document a receiving-adapter obligation. An unchanged-import
finding is conditional on supported host/toolchain/capacities; it is not proof
of a new emulator or ARM/Linux runtime. Compare NNES design to current code,
never assume its target contracts already exist.

The preliminary audit delivered API/internal inventory and a proposed design.
Lib bytes and INI/media remain unchanged throughout the approved rename.
No new universal
CPU schema, second executor, debugger frontend or negotiation framework.

## S1 Findings

Source baseline: ad66b615. NNES design reference: revision
0da1fd09db20ac9ecb74bcf7da29df7dec4dc7d8, especially
`docs/proposals/shared-neutral-contracts.md` and
`docs/design/ARCHITECTURE.md`. NNES proposes a neutral paused-executor
transport and optional x86 frontends; these are design targets, not evidence
that its imported Common is already neutral. This task keeps the owner's
`common/x86-debug` and `common/x86-xasm32` locations, unlike NNES's proposed
top-level locations.

### Lib: Unchanged Reuse, With Explicit Preconditions

No guest CPU execution, x86 register schema, guest port dispatch, instruction
decoder, BIOS or CP437 mapping ownership was found in Lib. No Lib source
change is required solely to emulate a different guest ISA. This is a source
boundary finding, not qualification of an unspecified receiving emulator.

| Component | Inspected responsibility and disposition |
| --- | --- |
| types | C/runtime and native declarations, atomics; retain. MSVC intrinsics are host/toolchain dependencies, not guest x86 semantics. |
| base | Task/event/mutex lifetime and wait; retain. No guest executor or CPU register interpretation. |
| storage | Files, medium modes and byte/page overlay; retain. No floppy controller, CHS or guest sector-command protocol. |
| console | Line/text/frame values and sink lifetime; retain. Cooked host console is not a DOS command interpreter. |
| console-broker | Native reader, console ownership and output; retain. Host Win32 behavior is not guest Windows behavior. |
| kvm-base | Opaque mailbox transport, key/text/mouse events, hotkey matching, neutral text cells; retain. Host scan identity is not a guest keyboard packet. |
| kvm-window | Frame rasterization, damage comparison, native input/capture; retain within its supported frame capacity. |
| kvm-console | Text cells plus supplied Unicode maps; retain. It neither decodes PC attribute bytes nor supplies CP437. |

Important qualifications, not reasons to silently expand this task:

- `kvm-base/input.c:26` synthesizes modifier scans 0x1d/0x38/0x2a during
  host-layout character conversion. `win32/input.c` obtains host scan codes;
  Linux text has no physical mapping and uses TEXT. These are PC-host input
  conventions. A receiving adapter must map KVM key/TEXT events to its own
  keyboard or controller, not feed scan bytes directly into a non-PC device.
- Graphics are complete 8-bit indexed images, palette size 256, at most
  1280x768, stride at most 1280. Arbitrary RGB images with more than 256 colors
  cannot be represented losslessly by this contract. Confirm the receiving
  product's actual output before promising unchanged feature coverage.
- Text is at most 80x25, fixed stride 80, sixteen colors, two 256-glyph banks,
  8-pixel glyph width and at most sixteen scanlines. Console maps are BMP
  code units excluding surrogates. These are bounded presenter capabilities,
  not x86 instruction semantics; richer text can be rasterized by an adapter
  only when the resulting graphics fit the existing contract.
- Linux Window, KVM Console and native Console broker remain UNSUPPORTED
  placeholders. Reusing unchanged on the existing supported Windows host is
  not a promise of a functioning Linux presenter or untested host ABI.

### Common Public API And Internal Coverage

The frozen mechanism inventory is the eighteen source/header/CMake files in
machine, session and ui. Every public entry point and driver/provider callback
is grouped below; internal ownership is recorded separately. The surrounding
debug/xasm interfaces, root CMake/verifier, shared tests and App/VM callers
complete the dependency closure. Retain means no ISA-driven redesign needed,
not proof that every possible application wants the same product policy.

| Public surface | Finding / owner |
| --- | --- |
| Machine create, shutdown, destroy; set_state_sink, set_frame_sink | Neutral resource/notification lifetime; retain. |
| Machine start, pause, resume, stop, reset, state_get | Neutral lifecycle with the existing cooperative driver; retain behavior. |
| Machine enqueue_input | Copied KVM event FIFO; receiving adapter translates device semantics. |
| Machine copy_published_frame, published_frame_sequence, published_frame_run_generation, run_generation | Complete presentation snapshot and generation; neutral within KVM capacity. |
| Machine set_removable_media | One default removable endpoint, path and storage mode; no CHS/FDC interpretation. Retain optional callback; not a universal multi-slot media API. |
| Machine read_state, write_state | Byte-stream callbacks, executor rendezvous and lifecycle; neutral. Driver owns image format and safe-point detection. |
| Machine debug_acquire, debug_cancel | Paused lease and cancellation are neutral mechanisms; retain. |
| Machine debug_execute_with_lease | Request/result ABI is x86-specific; replace payload contract, keep existing executor/lease. |
| Session create, bind_ui, destroy, run | Owns command-provider orchestration and lifecycle/UI coordination; no DOS command parser. Retain. |
| Session enqueue_ui_event, enqueue_runtime_completed, enqueue_frame_completed | Copied facts, sequencing and generations; retain. |
| Session provider open, reject_line, submit_line, begin_external, note_runtime, note_broker, note_monitor_current, handle_hotkey | Product text/commands/identifiers injected by caller; no x86 decode in Session. Retain. |
| UI create, destroy, set_run_generation, apply_action, set_state | Presenter/broker lifecycle; retain. |
| UI publish_frame, release_window_mouse | KVM data/capture routing, no guest register access; retain. |
| UI write_monitor, request_monitor_line, cancel_monitor_line | Host line editing and output; retain. |
| UI options/event sink | Product titles, status text and hotkey registry, copied outcomes; retain. |

All sixteen Machine driver callbacks were traced:

- reset/run/request_stop/request_wake/set_executor_callback: cooperative
  execution contract, not an x86 instruction engine.
- set_heartbeat: mandatory at create, enabled/disabled around run. This is a
  SoftPC-shaped scheduling obligation, not an ISA dependency. Preserve it in
  the initial change; a receiver can implement its contract without a new
  host timer. Making it optional is a separate justified decision, not an
  excuse to remove required executor callbacks or pause responsiveness.
- deliver_input/copy_frame/frame_published: adapter device mapping and complete
  frame observation; retain.
- set_removable_media: optional single-endpoint operation; retain.
- begin_state_read/take_state_read_result/write_state: adapter-owned state
  stream and safe-point progress; retain.
- execute_debug: x86 payload leaks here; generalize payload only.
- take_debug_stop/cancel_debug: executor stop fact and plan cancellation;
  no instruction/watchpoint interpretation in Common; retain.

Internal trace:

- Machine stores and copies the x86 request/result (`machine.c:21`), clears
  results, checks the 32-byte memory payload limit, and passes them through its
  paused service (`machine.c:185`, `:785`). It does not switch on x86 opcodes.
  Therefore one transport replacement suffices; no new execution thread.
- Machine frame comparison, input FIFO, terminal completion of pending
  requests and reset/run generations are not x86-specific. Preserve these
  failure/lifetime contracts, including contexts retained until quiescence.
- Session/control/control_state/presentation_plan handle queued lifecycle,
  physical-key lifetime, prompt readiness and presentation selection. Physical
  identity compares key/scan/EXTENDED, without interpreting scan values as
  8042 bytes. No segment arithmetic, x86 op dispatch or instruction decoding
  was found. Its header currently inherits x86 types through machine_interface.
- UI handles broker ownership, presenter teardown, console/window input
  routing and complete frames. Graphics-status text currently maps printable
  ASCII and blanks other bytes: a host status-text limitation, not a guest
  code page. Keep current behavior; do not claim arbitrary localized status
  strings are already rendered. No CP437 lookup or x86 CPU logic was found.
- Growing Session queues, fixed frame capacity, one removable endpoint and
  the existing graphics-to-Window policy are independent design constraints.
  None justifies an unrelated queue, device or presenter framework in T73.

### Actual x86 Ownership Leaks

`machine_interface.h:30-145` owns seventeen debug operation values, real-mode
segment/offset and linear addresses, byte port access, code default-size/base,
segment descriptor snapshots, ES/CS/SS/DS/FS/GS/TR/LDTR/GDTR/IDTR, CR0/2/3,
real/linear execution plans, and little-endian memory observations. Although
watch/trace are general concepts, this particular representation is x86.

`debug/debug_interface.h` adds EAX..EFLAGS, segments and CR register IDs.
The frontend owns DOS-style command semantics and uses xasm32. Those semantics
remain intentionally x86; neither an ISA-neutral command parser nor a universal
register model is needed. `vm/debug.c` currently includes this frontend header
for register vocabulary and dispatches Machine's x86 operations: it should
consume an x86 protocol value header, not the command object's interface.

Root Common CMake unconditionally builds common-debug and common-xasm32.
The three neutral targets do not link them directly, but Machine's public
types remain coupled. The corpus verifier hardcodes all five component names;
test/common builds debug_output/debug_linear/xasm tests unconditionally and
common_machine_smoke links common-debug. A directory rename alone cannot
prove a debugger-free neutral build or test corpus.

## Proposed Minimal Implementation

1. Rename the two components to the owner-requested Common directories and
   explicitly x86 target/API names, repairing consumers/tests/manifests/DAG
   checks in one complete step. Do not rewrite original debugger commands.
2. Put x86 operation/register/request/result values in an independent public
   protocol header owned by x86-debug. Both VM adapter and frontend include it;
   including the values must not require linking the debugger command parser.
3. Machine keeps its current serialized paused request slot and lease. Replace
   the CPU-shaped payload with a bounded copied operation identifier, request
   bytes/length and response bytes/length/capacity. Common validates bounds,
   admission, lease, completion and cancellation, not registers or addresses.
   Driver validates its protocol and returns status. No arbitrary callback
   supplied by a requester and no borrowed deferred payload. Use aligned local
   typed copies in adapters rather than casting byte arrays to CPU structs.
4. Choose transport limits from the existing largest request/result plus the
   qualification fixture, document them, and reject oversize. Do not guess an
   unlimited schema or introduce a dynamic message framework. This is an
   in-process protocol, not snapshot serialization or a cross-width file ABI.
5. Make x86 components/tests an explicit optional build selection. SoftPC
   enables them. Neutral-only builds must have no x86 link/header dependency;
   both a non-x86-shaped fake protocol and the x86 protocol exercise the same
   Machine rendezvous. Existing X/16-bit debugger behavior is preserved.
6. Keep Session/UI runtime behavior and Lib bytes unchanged. Receiving product
   supplies its command provider, VM driver, hotkeys, input mapping and frames.

Failure handling must preserve paused admission, stale-generation rejection,
shutdown completion and caller-context lifetime. Specify result initialization
and valid response length on failure explicitly; moving to byte transport must
not expose stale bytes or accidentally change existing x86 CLI output.

## Superseded Preliminary S Breakdown

The following preliminary breakdown is superseded by the owner's two-stage
plan below; it is retained only to explain the preliminary audit's estimates.

| S | Scope and acceptance | Preliminary footprint |
| --- | --- | --- |
| S1 | This audit and design; source/test +0/-0; review with owner. | Documentation only. |
| S2 | x86 component/protocol ownership and optional build; repair symbols, test ownership, verifiers and manifests. Preserve CLI and SoftPC behavior. | Roughly 20-35 tracked paths including mechanical callers/build/docs; hundreds of renamed references, little new runtime logic. |
| S3 | Replace Machine debug payload transport, wire existing x86 adapter/frontend, two-protocol and failure/lifetime tests. Session/UI need no new state. | Roughly 8-15 code/test paths; approximately +200..450/-170..350 production lines, tests additional. Re-estimate after S2; not a net-reduction promise. |
| S4 | Neutral-only independent configure/build/test, x86-enabled dual-width full regression, Lib byte equality and actual component boundary audit. | Build/test/docs chiefly; any new runtime need requires scope review. |

Completion requires x86-free neutral target/header closure, unchanged Lib,
preserved SoftPC debugger/lifecycle/presentation behavior, dual-width tests and
owner acceptance. It does not require inventing the receiving emulator or
claiming untested hardware/display capabilities. Existing three queued tasks
remain unchanged.

## S1 Evidence And Limits

Searches covered x86/register/BIOS/VGA/CP437 tokens, native compile conditions,
scan identity, includes, CMake links and debug callers; findings were followed
through the actual definitions and callers, not classified by keyword alone.
All mechanism APIs above have a disposition. No source, shared test, manifest,
media, INI or EXE was edited. Dynamic non-x86 integration is not yet proven.

Existing manifest/corpus/test-manifest, negative-DAG and documentation checks
passed (7/7); Lib types-layout/DAG/negative-include/naming/control-ownership
checks passed (6/6). These are static audit gates, not a new full regression
or Linux/native-NEC runtime test. Documentation governance and diff whitespace
checks also passed. S1 has no runnable-path change and produces no new EXE.

## Approved Plan After Preliminary Audit

This project's finalized design is authoritative. NNES is only the initial
reference. Lib remains unchanged, including host keyboard normalization and
current display capacities. The receiving product maps host input in its own
adapter. x86 debug/assembly remain optional product capabilities, not a new
generic debugger. Machine owns the future neutral execution transport; its
payload migration is expressly outside the rename step.

- **S1 (closed, revised scope):** rename common/debug to common/x86-debug and
  common/xasm32 to common/x86-xasm32; rename their public prefixes/targets and
  corresponding test ownership, repair every current consumer and verifier,
  preserve command text and behavior. Do not introduce protocol headers,
  transport changes or a compatibility alias. Existing build selection is
  preserved in this step; neutral-only selection belongs to S2's audit plan.
  Produce both EXEs, build/test, commit/push and stop for owner review.
  Delivered in 03a954dd; [actual record](../history/M9-T73-S1-x86-component-rename.md).
- **S2 (not admitted):** audit common/machine APIs, implementation and build
  closure against the agreed neutral transport direction; define bounded later
  migration S tasks and acceptance tests. Do not pre-admit those migrations.

S1 baseline f13c2780. Frozen rename ledger: both component trees, App command
and VM register consumers, Common/product debugger and xasm tests, root/shared
CMake, component/source gates, both Common manifests and current design/docs.
Historical task/source references remain unchanged. No Lib, Machine, Session,
UI, Compat, MVDM or guest-media implementation changes are expected.
Estimate: about 25-35 code/build/test paths plus documentation/manifests;
production C/H approximately +150..300/-150..300 (net zero mechanical lines),
tests approximately +100..250/-100..250 (net zero mechanical lines). Rename-aware
counts distinguish relocation from changed lines. No new runtime structure.
Proof: reverse the exact name substitutions and compare each C/H byte stream
with its baseline; both Release builds and full background presets, plus
debugger/xasm integration tests and static corpus gates. Desktop tests remain
explicitly excluded; no new guest installation or snapshot format is involved.
