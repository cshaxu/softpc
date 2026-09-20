# T73: Lib/Common x86 Dependency Audit

## Owner Request And Scope

Owner admits a new task outside the existing queue: audit Lib/Common x86
dependencies for reuse by a NEC emulator, using neighboring NNES design as
reference. Confirm whether Lib can transfer unchanged; identify Common bindings.
Initially requested names were common/x86-debug and common/x86-xasm32; the
approved S4/S5 revision below moves them into the separate x86 corpus.
Audit every API and
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

## Preliminary Minimal Implementation (See Approved S4/S5 Revision)

1. Rename the two components to the owner-requested Common directories and
   explicitly x86 target/API names, repairing consumers/tests/manifests/DAG
   checks in one complete step. Do not rewrite original debugger commands.
2. Put x86 operation/register/request/result values in an independent public
   protocol header owned by x86-debug. Both VM adapter and frontend include it;
   including the values must not require linking the debugger command parser.
3. Machine keeps its current serialized paused request slot and lease. Replace
   the CPU-shaped payload with bounded copied request bytes/length and response
   bytes/length/capacity. The operation identifier stays inside the adapter's
   payload, not duplicated in a Common envelope. Common validates bounds,
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
- **S2 (closed by owner):** audit common/machine APIs, implementation and build
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

## S2 Machine Audit (Baseline feee0fb1)

Owner accepted S1 and admitted this audit, not the migrations below. Frozen
universe: Machine's six C/H/CMake files, all 22 public functions and 16 driver
callbacks inventoried above, plus their x86-debug/VM/App consumers and shared
test/build closure. Findings below refine the preliminary inventory against the
renamed baseline. Production and tests remain +0/-0; no EXE refresh is needed.

### Confirmed Coupling, Not A New Execution Bug

| Source boundary | Actual finding and disposition |
| --- | --- |
| machine_interface.h | Seventeen debug operations, real/linear/port fields, segment/CR snapshot, execution plans and little-endian observations belong to x86-debug's protocol. Move values, not CPU state ownership. Keep the neutral lease and lifecycle declarations in Machine. |
| machine.c | Owns one copied request/result slot. `service_debug` does not interpret operation values. The `request->bytes > COMMON_MACHINE_DEBUG_BYTES` check does interpret the x86 memory-transfer limit; replace it with transport-length checks, retaining protocol validation in VM. |
| input_queue.c/.h | Fixed copied KVM FIFO and wake; no scan-code-to-PC conversion. Retain implementation; remove the SoftPC-specific comment when documenting the neutral contract. |
| frame_interface.h | Complete KVM presentation values and run/sequence facts, not CPU/video-controller registers. Retain current capacities and publication behavior. |
| Machine CMake | No link to x86 frontend or assembler. Header payload coupling, not runtime linkage, is the defect. |
| x86-debug command.c/debug_interface.h | One `command_execute` boundary issues typed requests; register IDs reside in the frontend interface. Move IDs and protocol values to `x86-debug/protocol_interface.h`, use it from the frontend and VM; no parser dependency for VM. |
| vm/debug.c/.h and driver.c | Actual register/segment/address/port/trace/watch interpretation already lives here. Retain it. Adapt one driver callback using aligned local typed copies; do not cast an opaque byte buffer to a struct. |
| App cancellation callers | Cancel the current debug plan; no register payload interpretation. Preserve calls and CLI selection semantics. |
| Common root/shared-test CMake | x86 targets/tests are unconditional, and common_machine_smoke also runs x86 CLI transcripts. Build and test selection needs isolation to prove neutral-only reuse. |

The seventeenth-operation inventory is complete: register read/write, linear
read/write, real read/write, port read/write, code default-size/base, CPU snapshot,
watch set/clear/get, execution-plan set/clear and execution-result get. Common
must not reinterpret any of them. This does not require a generic register
catalogue, decoder, debugger CLI, device bus or protocol registry.

The two current host compilers, using stdin-only `sizeof` assembly probes,
both measure request **80 bytes**, result **1328 bytes**. The existing 32-byte
constant limits one memory payload, not the complete transport. These native
struct sizes are evidence for sizing the replacement, not a serialized ABI.

### Internal Contracts To Preserve

1. One control caller serializes requests. Machine owns their copied slot and
   executor rendezvous. Acquire checks PAUSED; executor rechecks the paused
   flag and lease generation before invoking the driver. Resume, stop, reset,
   restored/cold runs and terminal exit invalidate stale access. Do not add
   another executor, request queue or arbitrary caller-supplied execution hook.
2. `debug_cancel` wakes the executor to cancel the driver's pending execution
   plan. It is **not** cancellation/join of an in-flight synchronous request.
   Retain `take_debug_stop` as a Boolean stop fact; driver owns watchpoints,
   instruction counts, result capture and the exact stopping boundary.
3. Worker termination completes pending synchronous requests through the existing
   failure path. A failed wait does not prove quiescence: caller contexts and
   driver resources cannot be released or their request slot reused on that
   assumption. Preserve shutdown/join ownership; do not invent retry/recovery.
4. Current early request rejection leaves caller output untouched; accepted
   requests zero Machine's result before driver execution. The x86 frontend
   also initializes its typed result. The byte contract must explicitly return
   a valid length (zero on failure), reject oversized driver replies and keep
   the frontend's initialized-output/error behavior. Do not expose stale bytes.
5. Lifecycle/input/frame/media/state-stream paths need no ISA rewrite. Snapshot
   safe points and file format remain adapter-owned. An executor callback is
   not automatically a snapshot-safe boundary. Keep single removable endpoint,
   copied input and full-frame publication rather than broadening device models.
6. Driver run/debug work executes on the executor; thread-safe stop/wake signals
   also originate on the control thread. Existing architecture prose saying
   *all* driver calls belong to the executor is too broad; correct that wording
   with the implementation contract. Heartbeat remains mandatory: a new adapter
   must honor cooperative callback/wake responsiveness, not use an empty stub
   without proving pause/stop progress.

No new ISA interpretation was found in the neutral state/request machinery.
This audit does not establish that all concurrency/platform failure cases are
bug-free or that a receiving emulator is already integrated.

### Original Follow-up Plan (Superseded By The S4/S5 Revision Below)

**S3: one complete protocol/transport migration.** Move existing x86 vocabulary
to the protocol header, rename its prefixes to common_x86_debug/COMMON_X86_DEBUG,
and repair frontend, VM and tests in the same delivery. Machine retains the
three debug operations (acquire/execute/cancel) and fixed driver callback, but
execute accepts copied bytes with explicit lengths/capacity. The operation ID
exists only inside the protocol payload. Proposed fixed bounds: 128-byte
request, 1536-byte response, covering the measured 80/1328 and a non-x86 fixture;
compile-time assertions prove fit. No per-request allocation or extra object.
These are bounded capabilities, not universal maximum CPU-state sizes.

The x86 adapter checks exact protocol sizes and its existing operation-specific
limits, copies into aligned local values, then calls the original dispatcher.
Its result is copied back only within the declared capacity. Frontend packing
is concentrated in existing command_execute, not every DOS command. Keep all
command text, empty-line repeat behavior, 16/X semantics and debug CLI lifetime.
Do not split vocabulary extraction into a separate step that temporarily makes
Machine depend on x86-debug or creates compatibility aliases.

Estimate: 8-12 production/test C/H paths plus manifests/docs. Production roughly
+280..420/-220..330, expected net +60..100 after matching relocation pairs;
tests roughly +120..220/-40..90, expected net +80..130. These are planning
ranges, not measurements or a promise of net deletion. Protocol relocation
accounts for much of the gross churn; admission rechecks the exact ledger.
Required proof: executor-thread execution, copied payload, empty/oversized/
malformed requests, capacity and reply-length errors, stale lease, driver failure,
plan cancellation and terminal completion; existing x86 CLI and SoftPC debug
integration; x86/x64 build and background suite, both EXEs for owner testing.

**S4: neutral-only build/test qualification.** A single default-on Common x86
build option leaves SoftPC's existing targets enabled. Off selects only the
neutral three components; no x86 header or link dependency may remain. Separate
Machine mechanism proof from x86 CLI transcripts without copying the test runner
or creating a plugin framework. Exercise two unrelated fake protocol shapes
through the same rendezvous, and separately preserve x86 frontend integration.
Update independent test selection, manifests and forbidden-dependency proof;
corpus verification still checks all shipped source, including optional code.

Estimate: 5-9 build/test paths plus manifests/docs; runtime C/H +0/-0 unless a
specific gap returns for review. Build/tests approximately +100..200/-70..140,
expected net +30..60. Require neutral-only configure/build/tests using only the
four shared directories, and full x86-enabled dual-width regression/package.
Do not claim Linux presenter support or NEC runtime validation from fake drivers.

Session/UI runtime, Lib, Compat, MVDM, snapshot format and guest media are outside
both follow-up scopes. Owner accepted the design and admitted S3 after S2 closure.
T73 remains open; S3 delivery waits for manual verification. To avoid a collision
with the CLI's existing common_x86_debug_result, the moved protocol reply is
named common_x86_debug_response; this is a naming distinction, not a new layer.

### S2 Verification

Existing background Common suite: x64 22/22 and x86 22/22, including Machine,
wait, input FIFO, x86-debug output/linear, xasm and manifest/corpus/negative gates.
No desktop test or new non-x86 runtime was run. Source was not rebuilt because
only this proposal and CURRENT changed. S1 accepted packages remain untouched.

## S3 Implementation And Review Ledger

Owner request: 批准照此实现S3.请你收口S2先，然后准入S3，执行完成后编译测试提交推送等我验证。
S2 closure/admission commit: 56261c63. No S4 implementation is included.

| Admitted boundary | Implemented path and proof |
| --- | --- |
| Protocol ownership | x86-debug/protocol_interface.h owns unchanged operation values, fields and register IDs; Machine no longer includes or defines them. Existing DAG negative test now also rejects Machine including this header. |
| Neutral request slot | machine.c owns fixed 128/1536-byte buffers and their lengths; no operation field, extra queue, thread, allocation or callback registry. Empty values are permitted, invalid pointers/oversize rejected before dispatch. |
| Existing completion path | Paused state/lease recheck, cancellation and terminal completion remain in the same functions; zero failure length, no failed output copy. Wait failure cannot authorize slot reuse. |
| Frontend | Only command_execute changes executable logic: supplies sizes and rejects malformed reply length. Protocol response is distinct from existing CLI text result. |
| SoftPC VM | Existing driver callback copies exact-size opaque request into aligned local x86 values, calls unchanged dispatcher, copies successful result. Original 32-byte operation validation remains in vm/debug.c. |
| Shared tests | Existing native fake proves executor thread identity and x86 CLI; deterministic machine_wait adds unrelated byte-reversal protocol, source-copy proof, empty/max/oversize/pointer/capacity/error/stale-lease/cancel/terminal/wait-failure cases. Frontend test rejects a short successful response. |
| Product integration | Existing command_provider uses the new transport; adds malformed x86 shape/capacity and over-limit memory-payload rejection without mutating response. Existing register, memory, port, trace/watch and DOS/X behavior assertions remain. |
| Protected components | Git diff confirms Lib, Session/UI, Compat/MVDM, INI and media unchanged. App cancellation and all non-debug Machine operations retain their code. |

Counts relative to 56261c63 (`git diff --numstat`, including the new protocol
header, excluding docs/manifests/artifacts): production C/H nine paths +327/-268,
net **+59**; test C/H four paths +405/-222, net **+183**; boundary-gate scripts
two paths +2/-0. New protocol header has 124 lines, predominantly relocated
definitions. Most VM/CLI/test churn is the exact vocabulary substitution.
Production is just below the estimated net +60..100; tests exceed estimated
+80..130 because failure, terminal, malformed-reply and real-adapter shape proof
were kept together rather than deferred. No new production state machine.

Review evidence: reverse exact vocabulary/include substitutions in vm/debug.c
and it equals the baseline; reverse vocabulary in command.c and remove only
the command_execute function, and all remaining code equals the baseline.
This protects original CLI style/semantics beyond passing example commands.
Search of src/test finds no old Machine x86 type/operation aliases. Both actual
production transport endpoints are migrated; all direct test callbacks/callers
are in the finite ledger above. Constants and struct fields were moved intact.

Package sizes: x86 3,659,685 bytes (+637), x64 3,061,665 bytes (+635).
Two fixed buffers replace typed storage; aligned protocol conversion uses local
values. No per-operation heap allocation was introduced.
Both Release build presets and final full background regressions passed;
five desktop tests per width remain excluded and owner testing is
still pending. S3 does not claim neutral-only optional-build qualification (S4).

During full regression, x86 initially passed 104/105: the product public-header
allowlist had not included the newly extracted protocol header. Added that one
explicit public path (no wildcard/private-interface relaxation); focused retest
passed. The Common forbidden reverse-edge probe still rejects Machine consuming
the same protocol header. Final x64 background suite passed 105/105 (162.58s).
Final x86 background suite passed 105/105 (125.26s). Machine's changed C source
also passed GCC C17 -Wall -Wextra -Wpedantic -Werror syntax checking. Common and
test manifests, DAG, documentation and diff checks passed. Existing build trees
remain reusable; no disposable media or traces were created for this task.

Package SHA-256:

- softpc32.exe: 90C69067124A830BF3FBC59B879893F1A046B0661C935A349DED6798F0A53E0C
- softpc64.exe: 78375916A8C4695F177A6AD94648B17CBD458B61D91D86A74FD6A868BF6F23FC

After pushed executor delivery 006ecf32, the coordinator role reviewed actual
56261c63..006ecf32 paths/counts, transport/driver/caller ownership, original
owner request, failure-output/lease/completion proof and protected-path equality.
Both shared manifests and the Common DAG passed again. No duplicate execution
path, old x86 Machine alias or unrelated implementation change remains in scope.
At dc06671d S3 was verified and awaiting manual acceptance; the owner subsequently
accepted it and its closure is recorded in the S4/S5 revision below.

## Owner Revision: Three Source Corpora And Three Test Corpora

Original request: 把S4推迟到S5.增加一个新的S4：把 common/x86-debug,
common/x86-xasm32 移到：x86/debug, x86/xasm32。相当于我们的顶级组件从
lib+common变成 lib+common+x86. S5则要求拆分和建立 test/lib, test/common,
test/x86 三个。这样总共有6个可公用组件供nxvm和softpc使用；而common+lib
的4个组件也供 nnec使用。写入本t任务proposal和退出标准，然后准入S4进行
x86顶级组件的建立。

Owner accepted S3 manual testing; [S3 closes](../history/M9-T73-S3-neutral-machine-debug.md).
This revision supersedes the earlier optional-Common-x86 build plan. Common
does not need an x86 option: its source/build closure is simply neutral. Products
choose whether to add the separate x86 corpus. No aliases or compatibility tree.

### S4: Establish The x86 Source Corpus (Admitted)

Baseline dc06671d, clean. Move both component trees with Git to src/x86/debug
and src/x86/xasm32. Rename common_x86_* / COMMON_X86_* to x86_* / X86_*, and
common-x86-* targets to x86-*. Update every App/VM/test consumer mechanically.
Keep opcode/register values, struct layout, DOS/X CLI text and execution logic
unchanged. Common contains only machine/session/ui and cannot depend on x86.
x86/debug depends on Common Machine, x86/xasm32, Lib Storage/Types; xasm32 uses
Types. x86 owns its build entry, complete manifest and component boundary gate.
Reuse the existing manifest checker rather than duplicate its algorithm.

Finite migration ledger: both moved trees; App command and VM debug/driver;
all direct shared/product test consumers; root/Common/x86 CMake and product
include gate; source boundary negative probes; manifests and current docs.
Historical records remain historical. Test directory separation is S5, so S4
temporarily wires the existing test/common suite to the separate x86 target.
This transitional test dependency has one owner and ends in S5; no production
compatibility layer is permitted. Lib and neutral runtime C/H stay unchanged.

Estimate: 25--35 production/build/test paths plus manifests/docs; production
C/H +250..400/-250..400, net zero mechanical lines; build/gate net +120..200.
No runtime objects, threads, states or new dispatch. Review rename-aware counts
and reverse substitutions against baseline; report actual additions/deletions/net.
Verify x86 and Common manifests/DAG, forbidden reverse/private edges, all original
debug/xasm tests, both Release builds and full background suites; deliver two
EXEs, commit/push clean and await owner acceptance. Desktop exclusions explicit.

### S5: Separate And Qualify Shared Test Corpora (Planned, Not Admitted)

Move x86 tests to test/x86; keep test/common neutral and test/lib unchanged.
Separate the mixed Machine mechanism/CLI test without duplicating a runner or
introducing a framework. Each suite owns its build entry, manifest and focused
proof. Test x86 protocol/frontend together separately from neutral byte transport.
Independent configure/build/test must work from exactly these copy sets:

- NXVM/SoftPC: src/lib, src/common, src/x86, test/lib, test/common, test/x86.
- NNEC: src/lib, src/common, test/lib, test/common, with no x86 files/targets.

Neither set may depend on importing-product CMake, adapters, ROMs, tests or sibling
repositories. A product includes x86 explicitly; do not add an x86 selector to
Common. Runtime C/H expected +0/-0; re-audit mixed-test ownership and estimate
build/test churn before admission. Preserve full dual-width SoftPC regression.

### Revised T73 Exit Criteria

All five S steps accepted; Lib unchanged; Common source/header/build closure
neutral; x86 owns all debugger/assembly/protocol symbols and no old paths/aliases;
the six-directory and four-directory standalone proofs pass with self-contained
manifests and boundary-negative tests. SoftPC DOS/X CLI, lifecycle, presentation,
snapshot and input semantics remain unchanged with both packaged EXEs and
dual-width regression evidence. Final actual-change audit, clean push and owner
acceptance are required. No claimed NNEC integration or Linux presenter support.

## S4 Implementation Ledger

Baseline dc06671d. All eleven debugger/assembler C/H files moved with Git into
src/x86. Public names and targets now use x86_ / X86_ / x86-; no aliases or old
source trees remain. Common's CMake and DAG now contain only machine/session/ui.
New x86 CMake owns both targets and uses the adjacent Common/Lib corpora; its
manifest uses Common's existing checker, not a second hashing implementation.
The x86-owned DAG checks source and target edges, public/private boundaries,
platform leakage and Lib Types vocabulary. Products explicitly add x86; there
is no Common x86 option or runtime dispatch layer.

| Frozen unit | Disposition and proof |
| --- | --- |
| Eleven moved C/H files | Reverse path/prefix substitutions equal dc06671d; original protocol values, layouts, commands and parser retained. |
| App command and VM debug/driver (five C/H) | Every call/type/include mechanically updated; reverse substitutions equal baseline. |
| Six test C/H consumers | Same mechanical equality; no assertions removed or behavior weakened. Existing transcripts and real executor tests retained. |
| Root/Common/x86 build | Root explicitly adds x86; Common cannot link x86. Standalone x86 source build and x86-verify pass using only source corpora. |
| Source/build/product gates | Common rejects new x86 protocol include and target; x86 rejects reverse/private/platform edges. Product gate covers the seventh source root, including relative reverse edges. |
| Transitional shared tests | Existing test/common links x86 explicitly and runs three new manifest/corpus/negative checks. S5 owns removal of this dependency and creation of test/x86. |
| Protected scope | Lib, neutral Common C/H, Compat, MVDM, INI/media and snapshot format have no diff. |

Measured with git diff --numstat -M10% against dc06671d: production C/H
16 paths **+363/-363, net 0**; test C/H six paths **+401/-401, net 0**;
build/verifier scripts eleven paths **+200/-41, net +159**. Attribute rule +1/-0
and docs/manifests/binaries are separate. Low rename similarity on three public
headers reflects pervasive prefix replacement; explicit old/new path pairing
and reverse byte comparison prove relocation. Default Git rename threshold
would count those headers as removal/addition, not new runtime implementation.
Counts meet the production and build/gate planning ranges; no runtime state,
thread, object, request API behavior or allocation was introduced.

Both Release builds pass. x64 full background 108/108 (163.08s), x86 full
background 108/108 (148.73s). Five desktop tests per width are excluded, not
claimed as passed. The three additional tests check the new source corpus.
An extra standalone Ninja configure stalled at compiler ABI detection and was
cancelled; repeating with the repository's MinGW Makefiles generator completed
the standalone x86-debug build and x86-verify successfully. No source workaround
was introduced for that auxiliary toolchain run.

Package bytes: x86 3,659,622, x64 3,061,602 (both -63 versus S3).
SHA-256:

- softpc32.exe: 5ACA0FD034D59D37B576867435D60419EF4ED31A8CCD6BBAC2BC9C0C330327A0
- softpc64.exe: AE59961C7BEDC9EFD0F5FCFF813824EBC4D4AC2D12117E6A0DE7948766467B09

Manifest/DAG, documentation governance and diff checks pass. Both auxiliary
standalone build trees were removed after their processes stopped; main build
trees remain reusable. No guest media or trace was created. No current source,
target or symbol uses the former names; the negative probe intentionally keeps
one retired include to prove rejection. All original user requirements map to
the revised S4/S5 ledger; S5 work is explicitly planned, not silently omitted.
Executor delivery 0ef82055 was pushed to origin/main. The coordinator role then
reviewed dc06671d..0ef82055 changed paths, paired all eleven relocations, checked
App/VM endpoint diffs, ownership/DAG and complete original-request ledger, and
confirmed protected-path equality. Common/x86/test manifests and both source
gates passed again after push. No runtime semantics, duplicate implementation
or retired public alias was introduced. The only temporary boundary is the
explicit S5-owned mixed test directory. S4 is verified and awaiting owner manual
acceptance, not closed. T73 remains open and S5 remains planned.
