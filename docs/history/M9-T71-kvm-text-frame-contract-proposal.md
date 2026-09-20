# KVM Frame Ownership And Transparent Mailboxes

Archived at owner-approved T71 closure. Status statements below are historical;
see the [final audit](M9-T71-completion-audit.md) for current disposition.

## Request And Status

Owner admitted T71 and S2 design. S1 delivered text-only admission at 04d76945;
its evidence is retained below. S2--S6 are reviewed; T71 awaits owner acceptance.
The latest owner-approved direction supersedes the earlier monolithic-frame
mapping-only and per-cell proposals. S3 implements control ownership first;
the frame/mapping migration remains S4 work.

Original request ledger:

- "1-迁移映射表，映射表归vm，这个所有权必须清理干净；2-kvm-console和kvm-window的文本帧格式和接口要对称"
- "在 kvm-base 里面，定义文本帧相同需要的字段结构；然后kvm-console和kvm-window各自所需的实际文本帧是扩展了kvm-base的基础文本帧加上各自所需的内容，比如字符映射表和字体位图表"
- "图形帧，只归属 kvm-window所有"
- "kvm-base 只管两个不同mailbox的实现和传输机制 (fifo and latest-wins)，实际命令处理都交给消费者 (kvm-console kvm-window)"
- "写入本T任务的proposal并进行S任务拆分。"
- S5 owner refinement: "保留现有容量，不扩容、不动态分配；统一做完整校验，超限明确失败，禁止静默裁剪。" The owner explicitly authorizes the necessary copy_frame result-contract change, without side-channel error flags, automatic presenter switching or guest-mode changes.

The independent [cell/colour cleanup candidate](M9-T72-kvm-text-cell-glyph-refactor-proposal.md)
remains queued only for per-cell struct/attribute normalization. It must not
repeat this task's frame ownership, mapping relocation or mailbox work.

## Observed Pre-migration Baseline

At admission the kvm-base frame contained text, both raster fonts and graphics pixels.
Its mailbox embeds that entire value even for Console and implements graphics
validation, copying and dirty accumulation. Console reserves unused graphics
capacity, but text copies skip pixel storage; S1 rejects graphical publications
before mailbox mutation. Do not describe this as copying every graphic frame
into Console. Text publications still copy unused font data.

Control FIFO currently defines Window title/freeze/release commands and payloads.
Base validates the kind and handles STOP admission; leaf workers execute the
Window actions. Console broker implicitly maps text bytes through Lib's CP437
table. These are the ownership boundaries being changed, not newly reproduced
hardware faults.

## T71 S2 Accepted Concrete Design

### Data And Dependency Ownership

- kvm-base owns the shared text value: dimensions, fixed-stride glyph-index and
  attribute arrays, palette, common cursor/font-selection metadata. Keep the
  current parallel arrays and observable attribute interpretation in this task.
- kvm-window owns its text extension with primary/secondary raster bitmaps.
  It also owns graphics dimensions, stride, palette, pixels, dirty bounds,
  graphics validation/copy ranges and dirty accumulation.
- kvm-console owns its text extension with primary/secondary character maps.
  It has no graphics publication type, font bitmap or reserved pixel capacity.
- Use C struct embedding and fully copied, bounded values. No borrowed resource
  pointers, font registry, reference counting, separate font-update messages,
  additional workers or per-frame allocations.
- Both leaf public APIs are typed; their signatures are analogous, not forced
  to accept the same oversized union. Window may use its own tagged text/graphics
  value; Console accepts text only. Freeze exact names/layout in S4 preflight.
- Generic mailbox metadata owns publication generation; base must not reach
  inside a leaf frame to set its sequence.
- Logical Console remains independent of KVM. Console leaf converts to explicit
  16-bit character output; broker marshals/cache-compares that output without
  CP437 interpretation. Preserve existing attributes and output barriers.
- VM owns the existing CP437 constant table and supplies both Console maps,
  preserving today's approximation. Remove Lib's table/query API, without
  duplicating it. Common status text supplies its own known mapping and never
  depends on VM. No code-page detection or bitmap-to-Unicode inference.

Graphical content is unchanged semantically. Do not move drawing into VM,
alter cursor blink/freeze, input/mouse scaling, device timing, MVDM, media or
snapshot format. Existing unsupported Linux presenters remain unsupported.

### Mailbox Mechanics And Leaf Behavior

Base owns separate blocking locks for latest-wins frames and FIFO controls,
bounded copied storage, capacity checks, notification, generation/capture/
acknowledgement and terminal admission. Storage capacity is selected once by
the owner; bytes/length are internal support contracts, never application APIs.
Check lengths/alignment and retain storage until worker join. Prefer caller-owned
or existing component allocations; do not add pointer-only allocation shells.

Each leaf defines and validates its own control payload and frame types.
Base neither enumerates Window commands nor interprets graphics/text fields.
A narrow leaf operation may update/copy the opaque pending frame under the
existing frame lock. It must not call sinks, notify, allocate, reenter mailbox
operations or take uncontrolled locks. No plugin registry or callback cascade.

Window's operation accumulates unacknowledged dirty bounds against the latest
complete pixels, including a captured frame still awaiting output completion.
Size/mode/stride/palette changes retain existing full-dirty behavior. Capture
does not consume; only a successful acknowledgement for the current generation
clears pending state. NOT_CURRENT and output failure must not silently consume
content. Console text replaces the complete value without graphical processing.

STOP remains a transport-level terminal operation: close frame/control admission
under the established lock order, reserve its FIFO position, retain repeat-STOP
idempotence and earlier accepted control ordering. Consumer performs shutdown.
Fault closure shares admission mechanics but does not masquerade as normal FIFO
STOP. Ordinary control remains independent of frame copy. Preserve notification
failure's accepted-work semantics; do not retry already accepted work.

### Upstream Composition

Common retains its existing executor, frame completion and UI routing ownership;
VM remains the single product conversion owner. Migrate the driver output,
Common storage/comparison and UI forwarding coherently in S4. An upper-level
copied output can compose the leaf-owned types without teaching base their
layout. Avoid embedding independent maximum-sized graphics storage for every
text destination; no shared-corpus dependency on VM.

S4 preflight must settle the exact upper-level value layout, actual live-buffer
count, text/graphics copy lengths and all driver consumers before code changes.
This is a required design checkpoint, not permission to create another frame
pipeline. Common-generated graphics status remains a Console text frame.
Graphic traffic must never enter the Console mailbox. S1 runtime graphics
rejection is superseded by the typed interface, not retained as a legacy API.

### S5 Fixed Capacity And Explicit Failure (Owner Approved)

This defines current supported output, not new display modes. Keep bounded
storage; do not expand capacity or dynamically allocate to accommodate oversized
content. The approved behavior change is that a mode previously silently cropped
to 80x25 now reports unsupported display content rather than false success.

| Boundary | Required rule | Validation owner |
| --- | --- | --- |
| Text dimensions | 1--80 columns and 1--25 rows; nonzero over-capacity dimensions return UNSUPPORTED. Zero rows/columns and malformed arguments return INVALID_ARGUMENT. | kvm-base shared text validator, reused by both leaves |
| Text row stride | Fixed 80 cells in storage; document it, with no configurable stride field. | kvm-base |
| Raster font height | Preserve the supported default-value convention; reject heights outside bitmap storage, never clamp or access out of bounds. | kvm-window |
| Cursor | Hidden or off-surface cursor is not drawn; it does not invalidate the complete frame. | Shared text meaning, leaf drawing |
| Character maps | Validate the Console-owned character representation at its boundary; do not require maps in Window or bitmap knowledge in Console. | kvm-console; logical Console checks its own independent output value |
| Graphics | Retain existing 1280x768 upper limits; validate dimensions, stride and effective pixel extent with overflow-safe arithmetic before copying. | kvm-window only |
| Rejection | No replacement/merge of the pending frame, generation advance, drawing wake or successful-output cache update. | Leaf admission before mailbox publication |

There is no all-knowing frame validator. Base validates only common text fields;
Window owns fonts and graphics; Console owns character maps. Reuse each owner's
validator at the appropriate entry instead of maintaining divergent copies.
The opaque mailbox still checks transport capacity, not frame meaning.

Remove producer min(actual, capacity) and equivalent silent truncation in VM.
Unsupported source output must propagate through the existing completion/error
path as a display failure, not as no new frame. If the current boolean copy_frame
callback cannot distinguish those results, change its public result contract
and migrate all callers/fakes together. This necessary boundary change is already
authorized for S5; do not seek approval again merely because it changes that
signature. Settle the exact status/no-frame representation in preflight. Do not
add an out-of-band error flag, second notification route or next-frame inference.

Do not switch presenters automatically, alter guest video mode, or expand limits
for unknown modes. Other lifecycle or fallback changes remain outside approval.
Test zero/minimum/exact-limit/over-limit dimensions, default/max/invalid font
height, hidden/off-screen cursor, graphics stride/extent bounds and rejection
without mutation/wake/cache changes. Through the actual Common/VM result path,
prove that no-new-frame is benign but unsupported output produces failure.
Re-audit all producers, callers and test doubles, including clipping written
without min(). Refresh S5 diff estimates after that inventory; the earlier range
is provisional and must not justify skipping the result-contract migration.

## Planned S Tasks And Delivery Boundaries

Current identifies the sole active S. Successors receive that packet before
execution, following predecessor review. S1 identifiers and evidence are unchanged.

| S | Scope and implementation | Exit proof | Estimated production / test changed lines |
| --- | --- | --- | --- |
| S2: design and finite inventory | Record ownership, old/new API migration, upstream layout checkpoints, staged plan and queue boundary. Documentation only. | Reviewed plan, links/gates, commit/push; no new EXE or runtime claim. | 0 / 0 |
| S3: opaque control FIFO | Move leaf command kinds/payloads and validation to leaves; base transports bounded control data and owns STOP envelope/admission. Migrate both workers and every internal caller; frame type temporarily remains the existing single implementation. | FIFO order/full rejection, copied payload, repeated STOP/reserved slot, fault closure, title/freeze/release behavior; no base Window-specific commands. | 120--220 / 60--110 |
| S4: typed leaf frames and opaque latest-wins | In one coherent migration split text extensions/Window graphics, migrate frame mailbox and dirty operations, move CP437 to VM, update logical Console, all Common/VM producers/consumers and tests. Delete old monolithic ABI; do not stage a second pipeline. | Both map/font banks, mapping-only/font-only repaint, pending dirty/late ack, mode/size/palette transitions, activation/NOT_CURRENT, typed Console admission, exact native output and snapshot regressions. | 420--700 / 150--250 |
| S5: capacity and failure contract | Apply the owner-approved fixed limits and component-owned validation above; remove producer clipping and migrate copy_frame result/callers if needed. No expansion or dynamic capacity. | At/below/above-limit and rejection-atomicity matrix; explicit display failure distinct from no new frame; no automatic fallback. Necessary result-contract change already approved. | 40--90 / 40--90, provisional pending full callback inventory |
| S6: integration and simplification audit | Close every finite-ledger entry, remove task-introduced obsolete wrappers/fields, update current design/manifests and review measured storage/copy costs. No unrelated cleanup. | Serial full x86/x64, four corpus/DAG gates, package/snapshot and bounded product checks; owner receives both EXEs. T stays open for owner acceptance. | 0 planned; material new repairs require scope revision |

Aggregate provisional production estimate: 580--1,010 changed lines across
roughly 16--27 unique C/H paths; tests 250--450 across roughly 12--20 paths.
Changed means additions plus deletions, excluding docs/manifests/binaries.
These are not a net-growth promise or an exact API specification. Recount
unique paths and added/deleted/net lines per S against its preflight baseline;
table relocation counts as edits, not new functionality. S6 discoveries are
not hidden in this estimate. Report exact sizeof, aggregate allocations and
copied bytes for text/graphics; Console must lose unused bitmap/pixel storage.

Every code-changing S delivers focused proof, dual-width full builds/tests,
updated affected manifests, both assets/binary EXEs, commit/push and a
coordinator actual-diff review. Do not claim a design-only S rebuilt packages.
No owner INI or media edit; use bounded ignored build fixtures for testing.
Whole T closure requires a separate original-request audit and owner decision.

## S3 Delivery

Opaque control ownership is implemented and reviewed at c1782fcc; see
[S3 accounting, evidence and closure](../history/M9-T71-S3-control-transport.md).

## S4 Preflight: Concrete Layout And Migration

Baseline b30cc0da (production c1782fcc). The existing frame-related C/H inventory
is 30 production paths and 25 test/support paths using kvm_frame or logical
Console text frames; mailbox initialization adds the direct component fixtures.
Not every logical Console binding caller needs an edit: its sink shape stays
the same while the owned character element becomes u16. Each unchanged caller
must still compile and be reviewed. No snapshot codec contains kvm_frame: the
codec owns device state, not presenter buffers; transaction tests still migrate
their runtime assertions and must pass. MVDM/Compat stay untouched.

Freeze the following layout before implementation:

- Base frame_interface.h defines kvm_text_frame: current fixed-stride parallel
  cell arrays, text dimensions/palette, cursor and font-selection metadata only.
  It has no graphics limit, pixels, bitmap, mapping, sequence or whole-frame union.
- Window frame_interface.h defines kvm_window_text_frame (base + both bitmaps),
  kvm_window_graphics_frame (dimensions/stride/dirty/palette/pixels), and a tagged
  kvm_window_frame (valid/graphics + text/image union). Preserve current fields
  and algorithms inside their new owner, including default font-height behavior.
- Console frame_interface.h defines kvm_console_character_map (two 256-entry
  u16 banks), kvm_console_text_frame (base + character maps). No graphic tag or
  bitmap storage. Typed publish accepts only that text value.
- Common machine frame_interface.h composes sequence + Window frame + Console
  maps. The maps are meaningful/copied/compared only for text. This costs 1 KiB
  of inactive reserved storage on a graphics value, but avoids duplicating the
  common text arrays or constructing a second graphics frame for Window output.
  It is the sole upstream copied value, not a second publication path.
- Common UI accepts the Window value, Console maps and upstream sequence as
  typed arguments; session passes the machine output fields. UI does not import
  machine or vice versa. Machine's type dependency on the leaf frame contracts
  is explicit in its DAG; it does not create a Window/Console or call leaf workers.
  UI constructs one small Console text value for either content or status.
- Logical Console text[] becomes explicit u16 output characters. Broker copies
  them without interpreting an encoding and widens its successful-output cache.
  Existing CP437 constant moves unchanged to VM's one producer; remove Lib query.
  UI status uses its own ASCII/blank map, not the VM table or a new table registry.

The frame mailbox stores a pointer to leaf-embedded typed pending storage,
capacity, active byte length, generation and pending flag. Initialize the storage
once before exposure, retain it through join, and allocate no per-frame object.
Publish checks byte capacity then, under the existing lock, either copies bytes
or invokes Window's single private update operation. That operation performs the
existing dirty union and copied-frame update without callbacks, allocation or
reentry. Capture copies only active bytes to caller-owned storage after checking
its capacity; generation is returned separately, never written into opaque bytes.
Only matching successful acknowledgement clears pending. STOP/control are unchanged.
Move the generic component publication declaration to leaf-support worker header;
applications continue to use typed leaf publication. No compatibility aliases.

Measured prototype sizeof is identical on x86/x64 (bounded build/t71-s4 probe):

| Value/copy | Old bytes | Planned bytes |
| --- | ---: | ---: |
| Complete frame / Window value | 998384 | 984100 |
| Upstream Common value | 998384 | 985128 |
| Console pending/captured value | 998384 | 7112 |
| Common text base | embedded | 6088 |
| Window text copied payload including tag | 15344 | 14288 |
| Console text copied payload | 15344 | 7112 |
| Graphics copied prefix before active pixels | 15344 | 1060 |

Existing live storage remains two machine buffers, one session value, one pending
and one captured value per active leaf, plus UI's temporary status/content value.
No additional maximum-pixel buffer. With both leaves, those seven main frame
values shrink by 2050880 bytes before small mailbox metadata/alignment changes;
UI temporary shrinks by 991272 bytes. These are layout-prototype measurements,
not a claim about final whole-process footprint; verify compiled production types
and aggregate storage again after migration.

The earlier 420--700 production / 150--250 test changed-line range underestimated
field/type and initialization migrations across upstream callers. Revised working
estimate: production +650/-550 (net +100; roughly 900--1500 changed), tests
+650/-500 (net +150; roughly 850--1450 changed), excluding docs/manifests/build
registration. Most is relocation or mechanical field access, not added behavior.
Actual accounting must separate that movement from new mailbox metadata and proofs.
Keep a single coherent implementation P after full verification, not intermediate
public aliases or a partial-build commit. S5 retains the explicit failure/capacity
matrix and producer clipping cleanup; do not silently fold capacity expansion in.

### S4 Implementation And Delivery

Production producers/consumers now use the split frame contracts; the first
x64 product build succeeds. Common machine compares both supplied character
banks as well as text/font content. UI constructs the small Console value and
does not copy Window graphics into it. VM owns the unchanged PC glyph table;
its diagnostic reads only the active Window union arm. No Compat/MVDM, media,
snapshot serialization or user configuration change was needed.

The migrated focused common-machine, product VGA-frame, snapshot-transaction,
runtime and runtime-cursor smokes pass. Both package binaries now contain S4;
no partial implementation commit or whole-T closure is claimed.

Shared test migration now compiles on both widths. The first x64 full run
passed 110/110 (109.97 seconds). An earlier library-only run passed 41/42:
the Console display fixture still filled the now-u16 cells with byte memset.
Its two fills were converted to per-cell assignment; rebuilt Console display
and I/O tests pass. Other intermediate build failures were unmigrated test
signatures/fields, not runtime failures. Old CP-display mapping assertions moved
to the VM frame producer test rather than being deleted with the Lib API.

New focused proof covers resource-only changes to both map/font banks, actual
Console bank selection and Unicode output, Window font selection, opaque
mailbox capacity/short destination rejection, dirty merging through Window's
real publication entry, and independent lock/STOP behavior. All pass on x64.
Final review also narrowed VM initialization to the active Window payload;
the text producer test checks the inactive tail remains untouched.

Final Release builds succeeded for both widths. With source/tests/manifests
held unchanged during each run, full serial x64 passed 110/110 (71.78 seconds)
and x86 passed 110/110 (97.91 seconds). These include package interaction,
restart, snapshot transaction/cross-process and all four corpus manifests.
One intervening x64 run passed 109/110: its test-manifest check overlapped an
edit to the negative-gate fixture and its manifest. The complete reruns above
replace that contaminated verification; no product workaround was introduced.
Native Linux presentation and fresh manual Win3.1/Win95 interaction are not
claimed. Existing unsupported platform workers retain their explicit status.

Compiled actual types match the preflight sizes on x86 and x64. Mailbox sizes
are 4408/4440; Window objects 989864/989936; Console objects 12760/12840 bytes.
These are type sizes, not a process-memory claim. Final production C/H is
+469/-345 (net +124), tests +406/-312 (net +94), including new headers and
excluding manifests, documentation and build files. Counts use
git diff --numstat b30cc0da plus the three newly added headers: 31 production
and 28 test C/H paths. Build/dependency/test gates add 9 and remove 3 (net +6,
four files); manifests add 65 and remove 62 (net +3, four files). Documentation
is counted separately in the review record. Unrelated Queue/cell-attribute
proposal edits are excluded. No INI, media, Compat or MVDM edits occur.

The 256 moved CP437 values compare identical, in order, to the original Lib
table. Searches for the old kvm_frame and lib_console_pc_glyph in src/test C/H
return no hits. Mailbox code has no text/font/map/graphics interpretation.
Window alone merges dirty bounds; Console uses one complete small text value.
Common machine may include leaf frame-value headers, not leaf instance APIs;
the dependency gate and two negative fixtures enforce that distinction.
Unchanged logical output bindings forward the same typed object pointer, and
the Linux backend remains unsupported; neither needs a duplicate conversion.
All active union reads, byte lengths, capture/acknowledge and producer paths
were reviewed. Snapshot codecs serialize device state, not these frame values.
The existing seven main frame values shrink by 2050880 bytes before metadata;
UI's temporary shrinks by 991272 bytes, without another allocation or worker.

Package sizes: x86 3654118 bytes (was 3652604, +1514); x64 3058686 bytes
(was 3057163, +1523). SHA-256:

- x86: 66BAA0C96DEC3E8DF57D08289AC33232FAE901FF181C77E28CB096DE376B9921
- x64: 58A5E4B67FB74D3F7092E09E8CD68E0CE3BB4129EB9D87F61C226F9E0F1DBDF5

S4 executor verification and coordinator review are complete at 17c9da90;
[the S4 closure](../history/M9-T71-S4-frame-ownership.md) records acceptance.
S5 retains fixed-capacity validation and producer error/no-frame distinction;
S6 retains final integration/simplification review and owner T acceptance.

## Finite Convergence Ledger

### S5 Preflight Finding And Approved Scope Refinement

Preflight baseline 0623dc5e. The direct driver
copy_frame consumers are Common machine, VM driver, two Common fakes, product
command-provider observation and VGA-frame tests; published-frame readers keep
their independent run-qualified copy contract. A status-returning driver copy
with an explicit no-frame result can use the existing worker error/unwind path;
there is no need for a second event queue or persistent error flag.

Source audit found a prerequisite outside the current no-Compat packet:

- compat/dib_surface.c::softpc_standalone_text_surface always reports 80x50
  backing storage. Its separate geometry helper and compatibility fill code
  intentionally use that storage capacity; test/unit/text_console_compat_smoke.c
  asserts it. VM driver then clips rows to 25. Removing that clip without first
  distinguishing current display extent from storage would reject ordinary DOS.
- compat/video.c::softpc_platform_presentation_fonts replaces height >16 with
  16 before VM sees it. Window validation cannot detect that lost information.
- Original nt_graph.c::textResize derives current text dimensions from the
  original display state and retains now_width/now_height; these are not the
  fixed backing capacity. Publication must use the correctly established mode
  extent, not a guessed 25 or a host Window size. Audit initialization/transition
  availability before selecting the exact existing boundary.

Recommended scope refinement: allow narrowly bounded Compat presentation-query
adjustments to report current text extent separately from storage stride/capacity
and preserve unsupported font metadata without copying beyond storage. Keep
original device code, snapshot format, storage capacity and console fill semantics
unchanged. VM then rejects unsupported output, rather than silently clipping it.
Do not add device state, mode-specific exceptions, timers or fallback presenters.
Owner subsequently approved: "批准修改compat". Current now permits these narrow
presentation-query changes. Do not add original device interpretation to
Common/Lib or hardcode 80x25 in VM. No speculative implementation is claimed
as verified.

Revised S5 preflight estimate: production +160--230/-90--140 (net about
+70--90), tests +160--240/-25--45. Documentation, manifests and artifacts are
counted separately. Replace existing validators rather than add boolean aliases;
retain each component's single admission path. No-frame and failure must migrate
together through the driver and executor before delivery.

S5 implementation checkpoint: validators now return explicit status; no boolean
compatibility alias remains. Common copy_frame uses status plus the existing
window.valid field (OK/invalid means no new frame), and errors request the existing
executor unwind before its ERROR completion. Compat uses original PCDisplay
text geometry, with the same row calculation as the original V7 query; backing
capacity remains a separate unchanged contract. Zero character/pixel height is
not-yet-available geometry. Font metadata is preserved and oversized glyphs are
not copied. VM clipping is removed. x64 build and focused VGA/Common/Lib checks
pass, including actual producer 80x25, oversized rows/columns and font/default
cases. Delivery verification follows below.

### S5 Delivery Verification And Sweep

Both Release builds succeeded. Final full runs: x86 110/110 in 100.87 seconds,
then x64 110/110 in 71.06 seconds with no build or source modification active.
An earlier x64 run also passed 110/110 in 124.71 seconds, but overlapped the
tail of a build and subsequent x86 activity; the final isolated x64 run replaces
that timing evidence. Intermediate compile failures were missing original-header
prerequisites for the controller geometry macro and a missing test declaration;
the original headers/declaration were used, not a replacement hardware constant.

Finite S5 proof:

- Base/Window/map matrix: kvm_frame_copy exercises null/zero/min/max/over-limit,
  default/max/unsupported fonts, off-surface cursor acceptance, both map banks,
  surrogate rejection and stride/extent overflow bounds.
- Window admission: kvm_frame_damage_mouse preserves pending data/generation
  and an unsignalled wake across invalid stride/font/height publications. Existing
  dirty and cursor/motion checks remain. Console retirement does the same across
  dimensions and invalid maps; logical Console checks no output callback on error.
- VM: vga_frame verifies actual 80x25 rather than the 80x50 backing capacity,
  unsupported columns/rows/fonts, zero/default font preservation and no-frame
  before character geometry exists. Existing original mode and snapshot rebuild
  tests remain. No fixed 25-row substitute was introduced.
- Common: machine_wait executes the real worker with unsupported/invalid frame
  statuses, proving one stop, no paused wait/publication and ERROR only after
  cleanup. Normal no-frame cases retain pause/resume/reset/wait behavior.
- Search scope: all src/test C/H copy_frame, old is_valid names, text limit clamps
  and font-height fallbacks. Every driver callback/fake migrated; published-frame
  readers retain their separate run-qualified boolean contract. Hidden cursor
  geometry and dirty intersection are intentional clipping, not truncated frames.
  Compat fill/storage bounds and original device painters are unchanged owners.

Accounting from 0623dc5e, C/H only: production +137/-59, net +78; tests +139/-17,
net +122. Four manifests +23/-23; no build-target/gate edits. No new frame storage,
thread, queue, state owner or compatibility alias. Header comments are included
in C/H counts. Documentation is counted separately at commit review.

Package x86 is 3655213 bytes (+1095), SHA-256
E454171E2DE2444B605D7F00724189C058F6598DCDB5351EB3F5B6E21293036B.
Package x64 is 3058756 bytes (+70), SHA-256
4F44D805DBA1C320029B76623CF7536FBEC695C02D4D8D203A0ABA786CEE6581.
No fresh manual Windows guest acceptance or native Linux presenter execution is
claimed. S6 integration/simplification audit and T-level owner acceptance remain.

Freeze the exact path list at each preflight using references to kvm_frame,
lib_console_text_frame, publish/capture/acknowledge, graphics/dirty/font/map,
control kinds and copy_frame callbacks under src and test. Each hit receives
migrated / unchanged-with-reason / owner-approved-transfer, with proof.

| Coverage unit | Owner / planned S | Required disposition |
| --- | --- | --- |
| Base frame/control/support headers and implementations | S3/S4 | No leaf command, graphics, font or encoding interpretation; no reverse leaf dependency. |
| Window public types, root helpers and both platform workers | S3/S4 | Typed values, owned graphics/dirty, unchanged native lifecycle/render behavior. |
| Console public types, workers, logical output and broker cache | S3/S4 | Text-only storage; supplied map; full-write acknowledgement and binding barriers retained. |
| Common UI, machine, session and driver contracts | S4 | One copied output route, correct status text/comparison; no product encoding dependency. |
| VM driver and every alternate production frame producer | S4 | Single CP437 owner, explicit data generation, no old ABI bypass. |
| Source dimensions and no-frame/error callers | S5 | Extents validated by owner; clipping and unsupported results explicitly resolved. |
| Snapshot codec/device archives, input/cursor and hardware | S4/S6 | Inspect dependency; demonstrate unchanged behavior and archive compatibility, not assumption. |
| Shared/product tests, manifests and DAG checks | S3--S6 | Migrate fixtures; retain behavioral coverage; prevent dependency back-edges and obsolete interfaces. |

No claims of whole-Lib/hardware correctness. Stop for review on MVDM changes,
new runtime ownership/threads, a second frame path, unsupported Linux feature
implementation, guest-visible fallback, or unexplained scope/complexity growth.

## S6 Integration Audit Checkpoint

Baseline 910ecdcc. The original production estimate was +0/-0. Source review
found a bounded default-font inconsistency: Window render treats height zero as
16, but cursor geometry skipped scanline bounds when height was zero. S5 now
preserves that metadata from Compat, so this must be resolved before acceptance.
Pre-change estimate: about +8/-7 in the existing Window geometry function and
one default-height regression. No public API, state, allocation or producer change.

The existing damage/motion/render smoke was extended with a visible underline
at the last text row using height zero. It failed on the baseline (expected
407..410 but received the full cell), then passed after resolving zero to the
same existing Window default before cursor arithmetic. Actual production C/H
is +7/-6 (net +1); test C/H +4/-0. Existing inverted-shape fallback is unchanged.
Both focused widths pass; full delivery evidence follows below. Coordinator
acceptance and owner T acceptance remain separate from implementation evidence.

## S6 Detailed Integration Audit

## Request And Scope

Owner requested common text fields in KVM Base, leaf-owned fonts/maps and
Window-only graphics, with opaque FIFO/latest-wins transport. S5 additionally
requires fixed-capacity validation and explicit producer errors, not clipping.
S6 audits the complete T71 migration against that request. T remains open for
owner acceptance. Baseline: 910ecdcc, production delivery 193ff7f0.

## Finite Audit Ledger

The frozen universe is the changed src/test paths from 7557ca5 through this
delivery, plus their direct frame/control callers and both selected platform
workers. Reproduce with `git diff 7557ca5 --name-only -- src test` and searches
for publish/capture/acknowledge, copy_frame, graphics/dirty/font/map and control
kinds. Dispositions below are migrated, unchanged with reason, or separately
queued. They do not claim an audit of unrelated hardware or all Lib behavior.

| Unit | Inspected result | Proof |
| --- | --- | --- |
| Base contracts/mailbox | Common text fields only; opaque bytes, independent locks, generation and STOP envelope. No leaf command or graphics interpretation. | mailbox.c/frame_interface.h; mailbox selection, frame lock, leaf control capacity and negative ownership gate |
| Window control/frames | Typed private commands, consumer validation, tagged text/graphics, private dirty union under the existing frame lock. | window.c and Win32 worker; damage/motion, modal, retirement and capture tests |
| Console/broker | Typed text plus two map banks; worker converts once to logical Unicode cells. Capture survives NOT_CURRENT and only successful current-generation output acknowledges. | console.c, worker and broker Unicode output; retirement, display and I/O tests |
| Common machine/session/UI | One composed upstream value; both maps/fonts participate in text change detection; session forwards fields; UI constructs only a small text/status Console value. Success caches advance only after accepted output. | machine.c/session.c/ui.c; common_machine, session_frame, composition and machine_wait tests |
| VM/Compat | VM owns the sole PC glyph table. Compat reports display extent separately from backing capacity and preserves unsupported font metadata. VM rejects rather than crops. | driver.c/video.c; VGA producer dimensions/font/error tests |
| Failure boundary | Driver lib_status plus existing valid bit distinguishes failure from no frame. Existing executor unwind reports ERROR; no side-channel flag or fallback. | machine_wait real-worker tests and VGA producer tests |
| Input/lifecycle/snapshot | Input owners and native capture paths are unchanged by the frame migration. Snapshot stores device state, not presentation structures. | Diff against 7557ca5; keyboard/capture/retirement, restart, snapshot transaction and cross-process tests |
| Linux workers | Still explicitly UNSUPPORTED; no fake implementation or new runtime dependency. | Both leaf linux/component.c; build-contract checks, not native Linux execution |
| Shared boundaries | Four corpora retain manifests; DAG rejects Base reverse edges and Common machine imports of leaf instance APIs while permitting frame values. | Source verifiers and negative fixtures, not filename checks alone |

Searches of src/test C/H find no old `kvm_frame` or `lib_console_pc_glyph`
API. `vm_driver_pc_glyphs` has one definition and two bank copies. Base mailbox
has no dirty/font/map/graphics fields or Window action constants. Window alone
merges graphics damage. No extra frame worker, per-frame allocation, alias or
parallel publication path is retained.

## Bounded Repair And Similar-Issue Sweep

Initial production estimate was +0/-0. Review found the default font-height
path: glyph rendering interprets zero as 16 rows, while cursor geometry skipped
scanline bounds and drew a full cell. S5 now preserves zero metadata from Compat.
The announced revision estimated about +8/-7 production lines and one test.

The added last-row underline assertion fails on the unmodified geometry and
passes after resolving the default locally before existing arithmetic. Actual
production change is +7/-6, net +1; test +4/-0. No new helper/API/state. Explicit
heights, inverted-shape fallback and off-surface behavior are unchanged.

Sweep: search font_height/cursor_top/cursor_bottom in Window, Console and broker.
Window render/size already use the same default. VM cursor percentages also use
16 for zero. Logical Console's zero-height native percentage fallback remains its
independent existing contract; it owns no raster-font default. This repair does
not assign Window bitmap rules to Console. The focused regression passes on both
widths; full delivery verification is recorded below when complete.

## Storage And Simplification

The S4 measured layouts remain unchanged: S5/S6 change validators/arithmetic,
not frame structs. Base text is 6088 bytes; Window value 984100; Console 7112;
Common upstream 985128. Window text copies 14288 bytes, Console 7112, graphics
1060 plus stride times height. Compared with the old 998384-byte common value,
seven main frame values save 2050880 bytes; UI temporary saves 991272 bytes.
These are object/copy sizes, not whole-process memory claims.

Allocation review confirms two machine buffers, one session value, pending and
captured storage per leaf and one small UI temporary. No second maximum-pixel
buffer was introduced. Common's 1 KiB map storage is inactive for graphics but
keeps one upstream publication; it is not copied down to Console as graphics.
The private Window update operation is necessary to merge dirty bounds while
the mailbox lock is held; removing it would relocate leaf policy back into Base.
Console's conversion helper is the worker-to-logical-Console boundary, not a
second public publication API. No speculative wrapper elimination is warranted.

UI status text intentionally wraps/truncates its fixed help surface; this is not
clipping an actual machine mode. Per-cell/neutral attribute redesign remains the
separate queued proposal and is not silently implemented by T71.

## Verification And Accounting

Both complete Release builds succeed. Final serial full CTest passes x64
110/110 in 113.24 seconds and x86 110/110 in 98.52 seconds. Source/tests remained
unchanged during these runs. The source/test manifest revision comment was
advanced between runs; hashes were unchanged and both manifest gates were
rechecked on x64 afterward. Package, snapshot transaction/cross-process, restart,
four manifests and source/DAG negative gates pass. Documentation and diff checks
pass after retaining unfinished audit records here rather than prematurely in
history. No new disposable diagnostic directory or guest-media changes.

S6 production C/H: +7/-6, net +1 (geometry.c); test C/H: +4/-0, net +4
(kvm_frame_damage_mouse_smoke.c). Two manifests: +4/-4; build/gates: +0/-0.
Documentation is counted separately at commit. Whole T71 final C/H difference
from 7557ca5: production +626/-407, net +219 in 34 paths (Lib 23, Common 6,
VM 3, Compat 2); tests +661/-336, net +325 in 30 paths. This is final-tree
difference, not a sum that counts repeatedly edited lines multiple times.
The 256 relocated PC glyph values compare exactly with the original Lib table.
No MVDM or snapshot codec source differs from the T71 baseline.

Both refreshed package sizes are unchanged from S5:

- x86: 3655213 bytes, delta 0; SHA-256 E5229A2176F8E83E960B0C9C430E6C5246B2CFFEDEC3370D6D3CC1F7E4AB6182.
- x64: 3058756 bytes, delta 0; SHA-256 90949A63700FAB618EA37E17ADDFBD6181DB4027484BB1BE2D781DECE084E684.

No fresh manual Win3.1/Win95 acceptance or native Linux presenter execution is
claimed. T71 stays open for owner testing. Coordinator actual-P review passed
after the implementation push; see the [S6 closure](../history/M9-T71-S6-integration-audit.md).
Unrelated queued-proposal edits remain untouched.

## Historical S1 Delivery

The following records the old-ABI admission repair truthfully. S4 may remove
its runtime graphics check only when the replacement typed API and tests prove
that graphical content cannot reach Console.

## T71 S1 Preflight And Finite Ledger

Search: `rg -n 'kvm_console_publish_frame|publish_text_frame' src test`.
Common UI is the single external production publisher; its graphics status
route already constructs text. Console root is the admission owner; the selected
Win32 worker is the sole conversion caller. Linux presenter remains unsupported.
Window and the common mailbox legitimately support graphics and stay unchanged.

Reuse kvm_frame_is_valid at Console admission, then reject graphics before the
shared publication/notification path. Malformed arguments take precedence over
unsupported representation; unsupported remains unsupported after STOP. Valid
text still reaches the existing locked STOP admission boundary. Do not add a
separate unlocked stopped check or change shared mailbox semantics.

Estimated production C/H: 8--15 changed lines, net +3--8; existing tests about
35--60 added lines. Documentation/manifests/artifacts counted separately.
Finite ledger: null/malformed rejection; valid graphics rejection; unchanged
pending text/generation and no wake/failure; subsequent activation draws old
text; normal text/latest-wins; stopped text rejection; Common graphics-status
publication stays text. Existing tests cover retirement and NOT_CURRENT replay.
Use event/semaphore barriers, not sleeps. Full suites run serially to avoid
concurrent native Console fixtures. No external guest installation exercise is
needed for this admission-only batch; final task integration remains in batch D.

### S1 Implementation And Similar-Issue Review

Production C/H: two existing paths +7/-3, net +4 (three added lines are the
public contract comment). Tests: two existing paths +37/-6, net +31. Counts
use `git diff --numstat 7557ca5 -- src test`, excluding README/manifests/EXEs.
No new object, state, thread, signature or runtime option. Common/VM/Compat/
MVDM production and user INI/media are unchanged.

Public Console admission now reuses kvm_frame_is_valid, rejects valid graphics
with UNSUPPORTED and delegates accepted text to the original mailbox boundary.
The worker no longer silently acknowledges graphics. Shared validation remains
unchanged, as required for later schema work. There is no second public path.

Call-site dispositions: common/ui's only production Console call submits text
or its existing status text; session selects that status surface for the running
graphics/raw route. Window supports graphics legitimately and is unchanged.
The Linux Console worker cannot start and remains explicitly unsupported.
The sole private text-conversion caller is the Win32 worker. Existing retirement,
NOT_CURRENT, activation and latest-wins checks remain active.

The Common composition fake previously accepted direct graphics and counted it
as delivered. It now matches the real admission rule, verifies UNSUPPORTED
propagation and unchanged completion cache, and uses valid fixture dimensions.
This changes an obsolete test expectation, not a production routing policy.
The Lib test preserves a pending text frame across null/invalid/graphics
rejections and verifies generation, output/failure counts, wake state, activation
replay and stopped-publication results without sleeps.

### S1 Delivery Verification

Both complete Release builds succeed. Serial full CTest: x64 109/109 in
74.85 seconds; x86 109/109 in 74.79 seconds. These include all 33 Lib and
18 Common tests, package interaction and snapshot regressions, four corpus
manifests and dependency checks. Focused x64 Console retirement/activation and
Common composition tests also pass 2/2. Documentation governance and whitespace
checks pass. Logs are bounded in ignored build/t71-s1; no guest trace/media
was created. No fresh Win3.1/Win95 manual interaction is claimed by this S.

Both assets/binary EXEs were rebuilt without touching the adjacent INI:

- x86 SHA-256: 6D2F2684858E00C688C90CB095E980507C9A12BDB5E3699A50C1B47448CD5594
- x64 SHA-256: F75530F92A236EEF041A660FFE2A82B77BBBB95599AA71781D4F26C2A443075E

Executor review compared every changed production/test path with the S1 ledger;
no schema, capacity, worker lifecycle or product policy changes were introduced.
This is S1 P1 delivery for owner testing, not task-wide completion. Later batches
remain pending and must start with their concrete contract/consumer review.
