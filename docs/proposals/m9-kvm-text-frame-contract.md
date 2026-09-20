# KVM Frame Ownership And Transparent Mailboxes

## Request And Status

Owner admitted T71 and S2 design. S1 delivered text-only admission at 04d76945;
its evidence is retained below. S2 remains the sole active design packet.
The latest owner-approved direction supersedes the earlier monolithic-frame
mapping-only and per-cell proposals; no implementation of this revision exists.

Original request ledger:

- "1-迁移映射表，映射表归vm，这个所有权必须清理干净；2-kvm-console和kvm-window的文本帧格式和接口要对称"
- "在 kvm-base 里面，定义文本帧相同需要的字段结构；然后kvm-console和kvm-window各自所需的实际文本帧是扩展了kvm-base的基础文本帧加上各自所需的内容，比如字符映射表和字体位图表"
- "图形帧，只归属 kvm-window所有"
- "kvm-base 只管两个不同mailbox的实现和传输机制 (fifo and latest-wins)，实际命令处理都交给消费者 (kvm-console kvm-window)"
- "写入本T任务的proposal并进行S任务拆分。"

The independent [cell/colour cleanup candidate](m9-kvm-text-cell-glyph-refactor.md)
remains queued only for per-cell struct/attribute normalization. It must not
repeat this task's frame ownership, mapping relocation or mailbox work.

## Observed Baseline

The current kvm-base frame contains text, both raster fonts and graphics pixels.
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

## T71 S2 Concrete Design (For Owner Review)

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

## Planned S Tasks And Delivery Boundaries

Only S2 is active now. S3--S6 are planned successors; each receives the sole
Current packet before execution, following predecessor review. S1 identifiers
and evidence are unchanged.

| S | Scope and implementation | Exit proof | Estimated production / test changed lines |
| --- | --- | --- | --- |
| S2: design and finite inventory | Record ownership, old/new API migration, upstream layout checkpoints, staged plan and queue boundary. Documentation only. | Reviewed plan, links/gates, commit/push; no new EXE or runtime claim. | 0 / 0 |
| S3: opaque control FIFO | Move leaf command kinds/payloads and validation to leaves; base transports bounded control data and owns STOP envelope/admission. Migrate both workers and every internal caller; frame type temporarily remains the existing single implementation. | FIFO order/full rejection, copied payload, repeated STOP/reserved slot, fault closure, title/freeze/release behavior; no base Window-specific commands. | 120--220 / 60--110 |
| S4: typed leaf frames and opaque latest-wins | In one coherent migration split text extensions/Window graphics, migrate frame mailbox and dirty operations, move CP437 to VM, update logical Console, all Common/VM producers/consumers and tests. Delete old monolithic ABI; do not stage a second pipeline. | Both map/font banks, mapping-only/font-only repaint, pending dirty/late ack, mode/size/palette transitions, activation/NOT_CURRENT, typed Console admission, exact native output and snapshot regressions. | 420--700 / 150--250 |
| S5: capacity and failure contract | Audit all active extents, strides, source clipping and caller statuses using the new owning types. Keep existing limits unless evidence/approval supports change. | At/below/above-limit matrix, no out-of-bounds copy or false success; distinguish no new frame from unsupported source. Any required public status or fallback policy first receives owner review. | 40--90 / 40--90 |
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

## Finite Convergence Ledger

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
