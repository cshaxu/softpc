# Non-public Code Quality And Performance Audit

## Admission and boundary

Owner admits T98 on 2026-10-18:
"准入一个新的t任务 帮我检验非公共组件的代码质量和性能问题（不包括镜像代码，但包括镜像diff）".
Audit baseline: 4c8e185b. CURRENT owns current task status.

S1 audits src/app-softpc/product, machine and compat; test/app-softpc;
repository-root build/test assembly and tools; reviewed local changes against
the pinned XP necessary subset and their ABI/caller joins. The eight reusable
src/test trees are outside the audit except as read-only interface context.
Unchanged original mirror implementation is outside scope. Local subset
exclusions are provenance, not automatically defects or removable patches.

S1 changes documentation only. No rebuild, EXE refresh, guest changes or
implementation repair is implied by this audit. Findings will propose minimal
owner-local corrections; the owner decides follow-up S tasks.

## Finite convergence ledger

The frozen unit of coverage is each of these six responsibilities, supported
by a tracked-path inventory and direct source/caller/test review. Dispositions
are confirmed defect, performance opportunity, existing debt, no finding in
reviewed path, or explicit verification limitation. Closure requires every
area's disposition and evidence; this is not exhaustive proof of all behavior.

1. Execution, input and debugging: lifecycle, wake, locks, instruction callbacks.
2. Presentation: readiness, pixel/text conversion, frame ownership and copies.
3. Audio: speaker, PCM, MIDI, completion/failure and snapshot interactions.
4. State and media: save/load transactions, stream/archive validation, disk I/O.
5. Product: configuration, composition, CLI extensions and failure reporting.
6. Qualification/tooling: tests, dependencies, build targets and mirror patches.

Performance claims must name frequency and repeated work. Source estimates
are not measured CPU/latency improvements. Existing TODO entries retain their
status; rediscovering them does not establish a new regression.

## S sequence

- S1 admitted: inventory, source audit, relevant checks, findings and minimal
  remedy estimates, actual-diff review and pushed report for owner review.
- Owner approved established repairs on 2026-10-18: 批准执行当前既定修复任务.
- S2: F1/F2 real media adapter failure and capacity bounds; owner accepted and
  closed on 2026-10-18. See [closure](../history/M9-T98-S2-media-boundaries.md).
- Owner inserts S3 on 2026-10-19: exact shared import, dual EXE and full tests.
- S4: owner-admitted public API consolidation: remove unused FM, internalize
  staged Emulator Product assembly and replace the over-wide frame-exchange
  locked-pointer observation with a scoped callback observation. Machine
  metadata remains locally cached and no observer can retain the exchange
  pointer. Owner accepted and closed it on 2026-10-21; see
  [S4 closure](../history/M9-T98-S4-public-api-consolidation.md).
- S5: audit the KVM native-worker destruction wait and replace the 5000ms
  destroy deadline only if the complete stop/wake/join/ownership chain proves
  synchronous destruction is safe. This supersedes the formerly planned
  finite-budget candidate. Owner accepted S5 on 2026-10-21; see
  [closure and verification limits](../history/M9-T98-S5-worker-destruction.md).
- S6: owner-inserted text underline and split cursor; audit/design first.
- S7: Window sizing policy; owner revises the still-design-only packet.
- S8: dynamic frame storage and producer integration, moved after S7.
- S9 (formerly planned S8): finite instruction budget rejection, no counter
  widening.
- S10 (formerly planned S9): Compat-private UTF-8 append opening, retaining FILE I/O.
- Each S builds/releases/tests/commits/pushes and waits for acceptance before
  the next admission. Performance profiling follows correctness by separate admission.

## Findings and verification

### Ordered remaining S work

Owner separates the two changes and prioritizes Window sizing. This is T98's
internal S queue, not the repository T candidate queue. S7 had only admission
and preliminary audit, no implementation P. Owner revises that active packet
to sizing and carries its storage audit forward to S8; no completed S is reused.

1. S7: Window sizing policy (current admission).
2. S8: dynamic frame storage after S7 acceptance.
3. S9: finite instruction-budget bounds (not yet admitted).
4. S10: private COM/LPT UTF-8 append opening (not yet admitted).

### S7 - Window sizing policy

Preserve initial work-area fitting and maximize-to-work-area. Allow manual
resize and title-bar double-click natural-size restoration beyond the current
monitor/RDP work area. Keep the existing content aspect ratio and checked native
coordinate boundaries; natural size means content-frame pixels, not desktop size.

Scope: existing kvm-window geometry and Win32 sizing/message paths, their
owner-local tests, README/UI contract and manifests. No payload/schema, guest
mode, frame exchange, Console, App or mirror changes. Audit initial placement,
first-frame fitting, maximum tracking, maximize and double-click independently;
do not globally remove startup/maximize guards or add a second sizing path.

Verification: oversized content initially fits a small work area; maximize
fits with aspect preserved; manual sizing exceeds work area; double-click
restores oversized natural dimensions; ordinary small frames retain behavior.
Use geometry and actual message-path assertions, dual cached builds/packages,
affected regression/gates and serial desktop tests; no Computer Use.
Implementation estimate: production +20/-8 (net +12), focused tests about
+60/-3; manifests/UI/task records are counted separately. Two causes: natural
restore used the automatic work-area fit, and unhandled WM_GETMINMAXINFO retained
the native virtual-desktop tracking limit. Retain existing aspect and maximize
helpers; pass the fit policy through the same private resize helper and override
only maximum tracking dimensions. No public API/state is added. Microsoft
documents the default at
https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-minmaxinfo.
Finite coverage ledger: initial/automatic fit, manual tracking and aspect,
natural restore (oversized and small), maximize, failure propagation, and dual
runtime regression. Existing initial geometry tests retain their owner; actual
message-path assertions cover the changed policies. Each member requires passing
evidence or an explicit unresolved disposition; only verified delivery can await
acceptance. Each S is delivered separately and waits for acceptance.

### S7 implementation and verification

The existing resize helper takes one private fit_work_area value: automatic
frame sizing passes true, title-bar natural restore passes false. No second
geometry algorithm or persistent policy state is added. WM_GETMINMAXINFO only
replaces the desktop-derived maximum tracking extent with the signed native
extent limit; minimum tracking and maximized geometry remain unchanged.
This does not promise windows beyond native coordinate/resource capabilities.

Actual tracked C/H accounting against 6500790b: production +16/-7 (net +9)
across four files; tests +55/-3 (net +52) across two files. Public eight-tree
changes are confined to src/lib and test/lib. Types adds only one-to-one
MINMAXINFO/message wrappers. No public API/frame ABI, App, Emulator, mirror,
input, allocation or exchange changes. The owner's INI display change is
preserved verbatim and included under package policy; snapshot.debug is
neither modified nor staged.

Sweep: searched rcWork, fit_client_size, fit_outer_rect, GETMINMAX, MAXTRACK,
resize_client and constrain_sizing throughout kvm-window. Initial placement,
automatic frame sizing and maximize retain fitting; manual aspect and WM_SIZE
enforcement contain no desktop clamp; natural restore bypasses only that fit.
These are all production hits. No obsolete sizing path remains.

The ledger is covered by existing pure initial/aspect tests, actual WndProc
assertions with a deterministic 600x500 work area, existing resize failure
propagation, and native modal-message verification. Tests prove initial
1280x960 content fits 600x477 outer bounds, manual 1616x1239 is retained,
double-click restores 1296x999 including decorations, maximize shrinks it back
to 600x477, and a small natural frame remains 336x279. Native modal tests verify
the installed procedure's tracking response. Physical user drag/RDP acceptance
is still the owner's step, not claimed from screenshots.

Cached x64/x86 full builds succeeded and published both EXEs. The first build
caught a new fixture function/local-variable naming collision, fixed before
verification. Existing mirror/test-header compiler warnings remain outside
this repair. Final builds only rebuilt the restored package fixture; shared
production/test code passes its strict warning policy.

Final universe: 192 registrations per architecture (184 background, five
desktop, three native audio). x86 passed all 192 in that run. x64 passed
183/184 background, 4/5 desktop and 3/3 audio; the unchanged isolated
command_provider_smoke and normal package_smoke subsequently passed. Independent
XML/name comparison confirms all 192 unique names have passing evidence per
architecture, with no missing/skipped cases. This is coverage with supplements,
not a claim that the x64 aggregate run was all green.

Earlier runs also observed the known runtime_restart_boot prompt roundtrip
failure, package stages 1/error 6 and 3/error 0, and a disconnected session.
The runtime test passed independently and in both final full suites. A temporary
failure-only package probe saw a live blank Console in one run and a terminated
child printing cannot compose machine in a later run. RDP was subsequently
confirmed Active; some package failures still occurred before final success.
Neither RDP nor this Window repair is asserted as their root cause. Temporary
App probe changes were removed before final qualification. Package debt remains
open; the command-provider wait has a deferred receiver. No assertion, timeout,
retry loop or product behavior was changed to manufacture success.

Final package SHA-256:
- softpc32: BEAE1647F8F177123045F99AACE6A6195212A5F89F611C47F57EA18C9A346C5B
- softpc64: 9314E59A4B8143F230E0A2D1833C388BD8A214F72DF0513114149B97DCED9CEB

Raw evidence and runners in build/t98-s7 are disposable after promotion of
this record and actual-change review. Commit/push and coordinator review
precede owner delivery; S8 is not started.

### S7 actual-commit coordinator review

Reviewed pushed executor ed9bb7b8 against the owner sizing requirements,
packet and actual 17 changed paths. Independently checked all four C/H
production diffs, both test diffs, manifests, UI/task/debt records, owner INI
delta and package identities. Accounting remains +16/-7 production and
+55/-3 tests. The only new Types values wrap native declarations; the fit
argument is private and introduces no new public API, state or second path.
No Console, frame ownership, guest or mirror changes are hidden in this delivery.

The independent test-registration/XML comparison before scratch retirement
found all 192 registered names with passing evidence per architecture. x64's
two final aggregate failures and later isolated passes remain explicitly
distinguished; no intermittent debt is declared fixed. Both EXE hashes match
the recorded delivery. App diagnostics are absent from the committed diff.

Confirmed all owned jobs exited, previewed and removed exact build/t98-s7;
Clean-Build -Check passes. Concise evidence above replaces disposable raw logs;
the reusable caches remain. Tracked worktree is clean after delivery; the
owner's untracked snapshot.debug is preserved. Review finds no blocker in
the bounded sizing change. This accepts executor delivery only, not owner
acceptance or T98 closure. S8 remains unstarted.

### S8 - Dynamic frame storage

Owner admitted after accepting S6 on 2026-10-21. Baseline 1b95cfd7.
Owner subsequently moved implementation after S7 Window sizing is accepted.
S9 finite-budget and S10 UTF-8 append work remain unstarted. This retained
audit/design is for resumption, not evidence of implementation.

Initial source audit confirms four independent fixed-capacity assumptions:

1. Base exchange allocates four frame_size blocks during create.
2. Window storage embeds a union dominated by 1280x768 RGB words, so text
   slots reserve about 3.75 MiB apiece without using the graphics payload.
3. Shared text and lower logical Console independently embed 80x50 cells;
   render, conversion, validation and broker writes all assume stride 80.
4. SoftPC's original nt_text writes a target stride of 80. Compat advertises
   80x50 despite a larger private allocation. Removing Driver bounds alone
   would not make wider producer frames correct. This requires a narrow audit
   of original host output, not another VRAM reader or a new shared maximum.

Preliminary design:

- Keep Base text metadata/cells, Console text plus copied character banks,
  Window text plus owned font banks, and separate Window RGB graphics.
- Make row stride and capacity explicit; validate the last addressed element
  and checked byte size before allocation or traversal. Frame dimensions remain
  content dimensions. Native SHORT/DIB constraints are backend checks, never
  arbitrary shared text capacity constants.
- Keep four stable slot identities and existing roles. Metadata may exist at
  create, but payloads start null. Leaf-owned allocation/destruction handles
  their own layouts; Base stays ignorant of Console/fonts/graphics content.
  Allocation is confined to the producer-owned slot; same-spec reuse allocates
  nothing. Specification replacement retires incompatible free storage and
  marks held storage for disposal only after its last exchange role releases it.
  No collection thread, polling, per-frame allocation or reference-counted
  resource framework is proposed. The exact minimal leaf/exchange preparation
  interface must be checked against all callers before implementation.
- Preserve last displayed content until successful replacement. Allocation
  failure produces no new publication and leaves held frames intact. Session
  continues routing exchanges without copying or retaining payload pointers.
- Lower Console conversion and native CHAR_INFO scratch become actual-size,
  reusable buffers. Coverage tracks both width and height so shrink clears old
  tails without imposing a maximum rectangle or writing unrelated scrollback.
- Preserve the private 80x25 status-page design as an actual producer choice,
  not a shared maximum. Its preparation callback must return allocation errors.
- Audit original host text-buffer writes and geometry establishment before
  adjusting Compat stride. Any necessary mirror diff must be separately
  explained and recorded; no guest/firmware or video-mode programming changes.

Initial inventory search hits 39 source/test files for fixed constants,
frame_storage and exchange create/writer. This is a lower bound, not final
coverage: copied values, fixture storage, resource ownership and backend
temporary buffers require follow-up. Expected total including documentation,
manifests and new regressions is roughly 45-65 files. Preliminary production
estimate +700..1100/-450..750 (net +250..350), tests +450..800/-250..500
(net +200..300). These are budgeting ranges, not an approved reason to add
abstractions; refine after selecting the smallest preparation contract.

Finite verification ledger: creation allocates no payload; first/additional
slot allocation; identical-spec reuse; dynamic sizes 80x25/80x50/132x43/100x60;
text/graphics growth/shrink/type transitions; decoration/font/map preservation;
slow reader across transition; allocation/overflow/stride failures; final
stop/destroy frees every allocation. Record exact allocation bytes and free
events using owner-local fixtures, never a production test API. Integration
also covers Driver -> Machine -> Session/UI -> both leaves, plus the status
producer and explicit diagnostic deep copies. No snapshot format change is
currently planned. Cached dual builds, shared units/gates, private regression
and serial desktop qualification follow implementation; prior S6 passes are
not S8 evidence.

### S6 - Text decoration and cursor design

Audit baseline is `8132d0db`, 2026-10-21. This section records proposed design,
not implemented capability or guest verification. The current packet owns
admission. No production, tests, package, media or snapshot changes accompany
this design. Linked remote governance skills could not be fetched (DNS/cache
failure); repository-local authorities were used.

Finite audit ledger: original shape/underline state; original update/host
boundary; Compat presentation; Driver leaf construction; neutral schema;
Window pixels/overlay; Console representation; frame lifetime. Source traces
below establish the loss points. Exact device-mode qualification and dynamic
proof remain required before implementation can be accepted.

1. Original CGA `cga.c` cursor computation and EGA/VGA `do_new_cursor()`
   already set `PCDisplay.cursor_start/height` and `cursor_start1/height1`,
   plus visibility. End-before-start interpretation belongs there. Their
   formulas are not to be reconstructed from registers by Lib or Driver.
   `base_cursor_shape_changed()` uses the existing video dirty count.
2. `nt_graph.c:make_cursor_change()` reduces the first height to a native
   Console percentage. Compat's font query returns false, selecting the
   original 20-percent fallback. Compat then stores that percentage and Driver
   `vm_driver_cursor_shape()` synthesizes a bottom-aligned range. Neither
   original top nor the second span survives. Read the already computed
   original shape in Compat before this lossy conversion; remove the Driver
   percentage conversion from frame production. Keep native compatibility
   callbacks required by the original host, not a second rendering authority.
3. `EGA_GRAPH.underline_start` and `UL_ATTR_MASK/UL_ATTR` already define the
   scanline and X000X001 attribute predicate. The retained NT text host has no
   consumer of those underline constants. Compat must select the applicable
   original display family, using existing original state/constants; never
   apply bit 0 unconditionally to CGA or every text mode. Original colour and
   glyph-bank extraction remain unchanged. Before coding, register tests must
   pin the selected EGA/VGA and CGA cases, including disabled/out-of-height
   underline; comments saying mono-only are not themselves runtime proof.
4. EGA underline-location writes already call `screen_refresh_required()`;
   VGA CRTC 0x14 currently updates the position without that call. Proposed
   narrow mirror repair: request the existing refresh when that position
   changes. Do not create metadata polling or another dirty cache. Original
   updates reach `softpc_standalone_dib_end_update()`, which marks text dirty
   even if its character comparison drew nothing. Driver already gates on
   that dirty flag. Prove this with a position-only register write.

Proposed shared contract (one replacement, not legacy plus new shape APIs):

- One `lib_u8 underline` per text cell, zero meaning disabled; one frame-level
  scanline. The scanline is zero-based; a position outside effective font
  height produces no underline. No magic value is necessary because cell
  enable and bounds already express absence. No bitfield/packing framework.
- One cursor with two fixed `{start, height}` spans, both in glyph scanlines.
  Zero height is empty; clip to effective font height; the union is the visible
  shape. Normalize overlaps before native inversion so overlap is not inverted
  twice. Shared position, mode and phase remain unchanged. No segment count,
  second visibility flag or second clock is needed.
- Replace old top/bottom fields and migrate every in-tree initializer and
  helper. Existing single-span results, including deliberate full-cell
  fallback users, are translated explicitly. This is a source/ABI layout
  update requiring downstream rebuild/adaptation, not binary compatibility.
  Zero-initialized new underline stays off; migrated single cursors stay single.
- Base owns scanline normalization only. Window owns pixel rectangles and
  native drawing. Underline ORs foreground pixels into the existing glyph
  decoder before the existing pixel comparison; fonts/VRAM are untouched.
  Cursor retains its existing overlay and blink path, drawing the two resolved
  spans and invalidating old/new shapes. Full-cell cursor invalidation is an
  acceptable small bounded region; it is not permission to draw the gap.
- Machine/Session/UI still route the same immutable four-slot payloads. No
  extra frame, retained pointer, queue, timer, thread or per-frame allocation.
  Emulator status-page initializers need mechanical schema adaptation only;
  X86 packages must not acquire display interpretation.

Owner-approved Console contract: retain native percentage/host-blink output.
An active in-font underline maps to native character underline (host-selected
position). A split cursor maps the topmost normalized span to native percentage;
it does not combine the gap into a full-height block. These are documented
approximations, not UNSUPPORTED, a product failure, or a second VT renderer.
Real native I/O failures still use the existing error path. Window remains exact.

Estimated production scope: shared `kvm-base/frame_interface.h`, Window
render/geometry declarations and native overlay caller, Console mapper; App
Compat presentation declarations/implementation, Machine presentation forwarding
and Driver; status-page shape initializers. Approximately 12--18 C/H files,
+150--230/-70--120, net +80--110. This is a planning range, not measured diff.
Mirror candidate is one VGA refresh site, approximately +3--6/-0, separate
from that estimate and requiring source-policy exception/ledger update. No
device cursor formula rewrite is proposed. If the live-chain tests expose a
different original semantic defect, report it rather than expanding this task.

Tests: extend existing Base shape, Window render/geometry/worker, Console map
and App presentation/device fixtures. Estimate +180--280/-30--60, net
+150--220; use table-driven cases instead of separate near-duplicate fixtures.
Cover attribute-only and position-only changes, font heights/colours/banks,
empty/single/split/overlap/clipped shapes, old second-span erasure, SOURCE/HOST,
freeze/redraw and existing exchange retirement. Original I/O-to-frame tests
must use actual register dispatch and producer, not only hand-built frames.
Reuse lifetime/concurrency suites; no test-only production entrypoints.

Implementation is owner-approved; qualification is in progress. Final
delivery must count actual production/test/mirror lines, update manifests and
source ledger, run cached dual builds and affected public/App regressions,
publish both EXEs, review actual diff, commit/push, then await acceptance.

### S6 implementation and convergence ledger

Frozen scope: Base text schema, Window glyph/overlay rendering, Console native
mapping, SoftPC Compat/Driver production and one VGA refresh hook. Four-slot
transport, dimensions, snapshots, guest media and audio remain unchanged.

- Schema: five-byte cells add a 0/1 underline enable; one frame-level scanline;
  two fixed start/height spans replace top/bottom. Empty spans hide, bounds clip,
  adjacent/overlapping spans merge. This is a source/ABI change: downstream
  producers must migrate cursor initializers and rebuild; no binary shim remains.
- Original authority: PCDisplay's calculated cursor position, visibility and
  two spans, EGA_GRAPH underline position and original UL_ATTR constants.
  Compat maps these; Driver fills both leaf payloads. Removed the old cached
  percentage getter, Machine forwarding and percentage-to-scanline helper.
- Window: underline enters the existing foreground pixel decoder and colour
  comparison. The existing overlay paints the union of normalized spans; old
  and new bounds are invalidated when position, visibility, font or shape changes.
  Even equal outer bounds with a different gap redraw. Same-shape frames retain
  the prior no-op optimization. No new frame, clock, queue, thread or allocation.
- Console: native character underline and topmost-span percentage are explicit
  approximations; the raw screen buffer enables the native underline mode.
  Setup/write errors remain I/O errors. Retained handles do not bypass a failed
  mode setup. Cooked screen state and output mode are not changed by this setup.
- Proof owners: Base normalization/validation; Window pixel/shape and native
  overlay mocks (including erasing a second segment); Console conversion and
  native error injection; actual raw/cooked display restoration; App actual
  register/timer-to-Driver publication and all 256 attribute combinations.
  Existing exchange, phase, freeze, retirement and lifecycle suites remain.
- Test cleanup: remove the obsolete percentage-helper-only runtime cursor
  executable and its duplicate App linkage. Real App cursor coverage lives in
  the existing VGA frame test. Runtime smoke observes BIOS quiescence before
  checking idle publication; a valid original cursor exists before BIOS finishes.
- Similar-issue sweep: all former cursor getters/top-bottom fields, cell
  initializers, hard-coded frame sizes, frame comparison and damage consumers.
  Only lower Console's intentional native single-cursor conversion retains
  top/bottom. The independent original VGA cursor-off defect is in TODO, not
  hidden by this task or claimed fixed.
- Mirror: `vga_prts.c` +4/-0, request the existing refresh on underline-location
  change; no device formula rewrite. See the current divergence ledger.

Qualification universe: 184 non-desktop/non-native-audio tests and five desktop
tests per architecture. Three unrelated native-audio integrations per architecture
are outside this display change; no claim is made that host audio is available.
Completion requires focused results, applicable regressions, exact diff review,
published package hashes and honest disposition of host-dependent failures.

### S6 executor verification, 2026-10-21

Cached dual builds publish both EXEs. Final tracked C/H accounting against
b044613f: production +153/-114, net +39 (19 paths); tests +261/-116, net +145
(13 paths). Shared production is +121/-50; private adapters +28/-64; mirror
+4/-0. Counts exclude manifests, documentation, CMake and binary artifacts.
No second renderer, phase clock, frame allocation or ownership path is added.

Focused six-test suites pass on both architectures. Final background x64 is
184/184; x86 is 183/184 with the sole stale test manifest caused by the last
mock regression edit. After refreshing that manifest and rebuilding the
changed test, the four-test supplement passes on both widths: broker console,
test manifest, documentation governance and XP mirror gate. This also proves
native mode query/set failures and success using a retained raw handle.

Final desktop x86 is 5/5. Final desktop x64 is 4/5 plus the unchanged compact
package test passing separately (1/1); all five have final-code passing evidence.
Earlier runs failed package attachment or initial prompt checks, including
after reconnect. No causal fix is claimed: the unresolved intermittent test
is recorded in TODO. Diagnostic-only edits were removed and the final test
was rebuilt. Passing retries do not prove stability. All three Lib desktop
tests consistently pass, including raw/cooked underline/content restoration.
Three unchanged native-audio integrations are outside this display scope.

Self-review checked actual schema/producer/consumer diffs, cursor shape lifetime,
old/new overlay invalidation (including changed gap with equal bounds), overflow
clipping, underline-only refresh, retained output handle failure and all former
percentage API consumers. No compatibility alias remains; lower Console's
native cursor percentage is an intentional approximation, not another hardware
authority. The original VGA disable-bit defect is separately recorded, not
silently repaired. Window pixel and original-port frame assertions are dynamic
proof; arbitrary real guest/adapter combinations are not claimed exhaustive.

Published hashes are in CURRENT. INI/media/snapshot are unchanged. Downstream
imports must take changed Lib source/test trees and the Emulator initializer,
rebuild, and migrate cursor producers to two start/height spans (zero height
empty) and initialize the optional underline enable to zero. No downstream
repository was modified or claimed synchronized. Await owner acceptance after
executor push and coordinator actual-change review.

### S6 coordinator actual-change review

Reviewed pushed executor 8e66d797 against the admitted scope and actual 47-path
diff. The shared schema has one hardware-neutral shape definition; consumers
normalize once per conversion and retain the existing phase/exchange owners.
Compat consumes original calculated state, not registers reinterpreted in Lib.
The only mirror addition is the reviewed four-line refresh hook. Removed
percentage forwarding and duplicate test linkage have no remaining callers.
Native Console limitations and downstream source migration are explicit.

Before retiring logs, independently counted 189 distinct passing final-code
test names per architecture across final/supplement reports. This is aggregate
passing evidence, not a clean single-run or a stability claim; package attach/
prompt intermittency remains an explicit TODO. No product change was made to
hide it. Original cursor-off handling is also explicitly deferred. The review
does not waive either issue or claim all hardware combinations tested.

Published binary hashes and protected INI/snapshot hashes match the executor
record. All owned runners exited; exact disposable S6 scratch was removed via
the cleanup owner and residue check passes. Only the two reusable caches remain.
Git retains only the owner's untracked snapshot.debug outside committed work.
Implementation is delivered for owner acceptance; S6/T98 remain open and no
next task is admitted by this review.

S1 report delivered on 2026-10-18. No production or test modification has been
made. Findings below are against 4c8e185b; none is claimed to explain an owner's
specific historical symptom without a matching guest reproduction.

### Coverage and proof limits

Tracked inventory contains 139 paths: 71 App production/header/resource files,
52 App test/fixture files, 14 tools and two root CMake files. The frozen mirror
manifest contains 505 paths; 131 have local patches. Independently reading the
fixed XP objects and applying the recorded necessary-subset ranges reproduces
all 131 subset hashes. Diff statistics are +27832/-25922, net +1910, against
those subsets, not against whole XP files and not changes made in T98.

All admitted paths received inventory/caller/pattern screening. Direct detailed
reviews concentrated on the six chains below and the changed CPU, C-VID,
display, input, DMA/audio and archive seams. Hash reconstruction is provenance
proof, not a claim that every line of all 131 patches was semantically proved.
Unchanged original-core algorithms and the eight public trees were not audited.

| Responsibility | Disposition |
| --- | --- |
| Execution/input/debug | Budget truncation confirmed by source; existing idle waits preserved; per-instruction host calls are a profiling candidate. |
| Presentation | Direct leaf production/four-slot transport retained. Mouse-triggered full refresh/full RGB conversion is an optimization candidate; obsolete backing-stride comment noted. |
| Audio | Reviewed PCM conversion, MIDI cancellation/volume and speaker ownership with DSP completion seams. No new confirmed audio defect; unsupported SB MIDI remains existing TODO. |
| State/media | Close failures and HDD capacity overflow confirmed with production-code isolation probe. Existing physical-address debt independently rechecked. Snapshot hashing/full scan is explicit cost. |
| Product | Config/snapshot extension path reviewed; COM/LPT endpoint path encoding still bypasses the existing UTF-8 file abstraction. |
| Tests/build/mirror | Dual cached unit/gates pass, missing adapter-close/budget/large-image cases identified. Mirror provenance and selected high-risk patches reviewed; no full semantic or performance qualification claim. |

### F1 - P2: media retirement can falsely report success

`src/app-softpc/compat/hdd_media.c:174` consumes the old lease with
`lib_storage_medium_destroy()` but ignores its status, clears metadata and
returns OK. Replacement similarly ignores retired-close status at line 184.
`compat/gfi_image.c:348,404,422` has the analogous attach/restore pattern.
Lib's close contract explicitly consumes the lease even on I/O failure, so
retaining a stale lease or retrying its close is not the remedy.

An isolated x64 probe includes the actual hdd_media.c and injects IO_ERROR from
destroy. The real restore-detach path clears the medium and returns OK. This
is dynamic proof of the adapter defect, not a native disk-close reproduction.
The archive unit replaces both real adapters with a test implementation that
propagates close status (`test/app-softpc/unit/compat/media_snapshot_smoke.c:17`),
so its successful result cannot qualify the real failure path.

Minimal remedy: finish the existing mandatory metadata/ownership cleanup,
return its close failure through existing attach/restore results, and cover
the real FDD/HDD adapters. No retry, rollback or new lifecycle state. Review
void shutdown callbacks separately rather than pretending they can return a
new result without changing their original contract. Estimate: production two
files +12-24/-8-16, net +4-8; focused existing-owner tests +45-80/-0.

### F2 - P2: accepted HDD size exceeds its address representation

`compat/hdd_media.c:46,113,186` accepts file length, stores sector count in
IU32, and later calculates `total_sectors * 512u` in 32 bits. A 4-GiB medium
has 8388608 sectors; its computed byte extent wraps to zero. The actual
production-code probe verifies attach succeeds and a one-sector read at
offset zero is rejected. Its medium is mocked; no multi-gigabyte file was
created or guest disk altered.

The original host offset is a signed int. Widening one multiplication would
not make the whole controller support arbitrary large images. Minimal remedy:
establish the actually addressable sector-aligned bound, reject unsupported
attachments explicitly, and calculate transfer bounds in lib_size with checked
subtraction. Apply the same validation to restoration. Do not change geometry
to hide truncation or add another disk backend. Estimate: one production file
+8-18/-3-6, net +5-12; boundary tests +30-50/-0. No mirror edit expected.

### F3 - P2: a finite instruction budget can become unbounded

`machine/machine.c:350` narrows a lib_u64 budget to Windows unsigned long.
For 0x100000000 it stores zero and enables the finite-budget flag.
`compat/platform.c:159` treats zero as nothing to consume, so this request
never stops through its instruction budget. UINT64_MAX is separately handled
as continuous execution; normal App execution uses that sentinel and is not
shown broken by this finding. Existing tests exercise small finite budgets.

Minimal remedy: reject finite budgets beyond the existing 32-bit counter before
entering execution; document the limit and preserve the continuous sentinel.
No 64-bit per-instruction counter or new scheduling path is needed. Estimate:
machine.c/machine.h +3-6/-0; existing machine test +10-20/-0. Source proof only;
an intentionally unbounded guest run was not launched to demonstrate a hang.

### F4 - P2: COM/LPT file paths have inconsistent encoding

Product reads paths from the same config used for media and resolves them
relative to the UTF-8 executable directory. Storage treats those paths as
UTF-8. `compat/serial.c:155` and `compat/parallel.c:180` instead pass them to
narrow fopen. On Windows where the CRT ANSI code page is not UTF-8, non-ASCII
paths can fail or identify a different name; ASCII paths remain unaffected.
This is a source-established encoding mismatch, not a claim that every host
configuration fails. Current endpoint tests use ASCII paths.

The initially suggested Storage-writer migration is withdrawn after the
owner-requested follow-up below: flush is not its only missing contract.
LPT also needs the exact partial-write count. Adding just writer_flush would
not preserve its already-tested unwritten-tail behavior.

Recommended narrow candidate: retain the existing FILE write/flush/close
paths and change only UTF-8 path opening through one Compat-private append
helper shared by COM/LPT. The existing Windows-only host can convert the
bounded 1024-byte path to UTF-16 and use _wfopen in binary append mode.
This deliberately retains a small native path conversion at the private host
boundary rather than expanding the public writer ABI or replacing its I/O
semantics. It does not claim all file opening is unified with Storage.
Estimated production: platform.c/platform.h plus two call sites, +10-16/-2,
net +8-14; focused existing endpoint tests +20-35/-0. Public and mirror delta
zero. The helper has been validated only in an isolated candidate probe;
production implementation and product regressions are not yet admitted.

### Performance candidates - evidence of work, not measured speedup

1. `compat/cvidc/gdp_state.c:107` linearly searches slot records on each access.
   The mirror contains 22603 textual rule-slot call sites. Mapped VGA members
   also search for their aggregate carrier. This is local ABI adaptation cost,
   not an inherent XP device algorithm. Call-site count is not a dynamic call
   count. Measure slot count, hit position and time in a controlled VGA workload
   before choosing an owner-local lookup improvement. Do not replace the C-VID
   implementation or introduce a second fast rendering path.
2. `machine/machine.c:286` requests an original full refresh after each mouse
   delivery. `machine/driver.c:304` resolves every pixel of a produced graphics
   frame, even if source damage is small; Window damage comparison happens
   later. A 1024x768 frame writes 3 MiB of RGB payload; at 20 produced frames/s
   that alone would be 60 MiB/s, excluding reads. This is arithmetic, not a
   measured bandwidth bottleneck. Removing the refresh without proving the V7
   software-pointer path would reintroduce missing cursor updates. First measure
   refresh, conversion and paint separately. Complete immutable frames remain
   required; no partial-frame ABI or history cache is proposed.
3. `compat/platform.c:115,174` uses locked compare/exchange for pending/pacing
   reads; local c_main.c hooks call into this seam at instruction/quick-loop
   boundaries. Debug and snapshot observers also have inactive fast returns.
   These calls warrant instruction-throughput profiling, not unconditional
   deletion. Control/stop/HLT wake and snapshot semantics must survive any change.
4. `compat/media_snapshot.c:verify_base/capture_slot` hashes complete base media;
   overlay capture reads both base and effective data in 4-KiB blocks. Load
   validates before mutation and again after reopening the saved path. This is
   substantial save/load I/O, not idle CPU. SHA validation and re-open validation
   are distinct obligations. No unsafe hash cache or removed validation is
   recommended merely to reduce reads.

Performance changes have no defensible implementation line estimate until a
bounded profile selects the actual bottleneck. A follow-up profiling S can
change zero production lines and use task-local probes. Avoid a speculative
buffer/cache/index framework based solely on these source counts.

### Existing debt and reviewed non-findings

Compat memory.c still routes its named physical API through c_sas_loads/stores;
the existing High TODO already owns this. Debug memory uses explicit translation
and physical access, and sound DMA uses physical SAS. Preserve that distinction.
The 38h SB MIDI parser, package/readiness and Ninja recovery debt are not fixed
or rediscovered as new regressions here.

`compat/dib_surface.c:18` still explains enlarged text storage using the old
physical-stride fast path, although nt_cga now enforces fixed target stride.
Correct that comment alongside a future presentation change; an allocation
shrink needs separate writer-bound evidence, not just deletion of the comment.

Early reset failure can leave process-global initializations before
hardware_initialized becomes true; normal destruction is guarded by that flag.
This is a cleanup review lead, not a proven user leak or reason to add retry
states. Current terminal machine failure policy and native ownership must be
checked before proposing recovery semantics.

No new second executor, steady-state whole-frame Session copy or per-frame
allocation was found in reviewed production paths. Current slot ownership,
direct leaf production, body-ready publication and retained Console restoration
remain distinct justified responsibilities. PCM/MIDI completion and cancellation
were reviewed against existing mocked tests; a unit pass is not native-device
availability or audible playback proof.

### Verification and delivery

Existing cached x64 and x86 binaries each pass 56/56 selected tests: 43 App unit
cases and 13 root gates, zero failures/skips. Selection:
`-R '^(app-softpc/unit/|checks/)' -LE 'desktop|native_audio' -j 8`.
No test binary was rebuilt and no full integration/desktop/audio rerun is
claimed for this read-only audit. Reports/logs and the final assertions-enabled
x64 HDD probe are under build/t98/s1; run.exit and mirror.exit are zero.
The initial scratch probe omitted stdio, and an intermediate Release probe had
NDEBUG inherited; those attempts do not establish the capacity result. The
final -UNDEBUG probe executes the real attach and validates pointer/size/IU32
widths 8/8/4, 8388608 sectors and rejected first-sector transfer.

No production/test/mirror changes: +0/-0, net zero. EXEs, owner INI/media and
snapshot.debug remain untouched. Documentation gate and actual-diff review
qualify this report; they do not qualify hypothetical fixes.

Suggested follow-up ordering, subject to admission: fix real media adapter
failure and size boundaries; fix bounded-run contracts; fix endpoint path
opening without migrating the write contract; then profile the three runtime
candidates before implementing performance changes. Unchanged mirror
algorithms stay outside those suggested repairs.

## Actual-commit review

Coordinator role reviewed executor commit 849709b5 against the owner request,
packet, real adapter probe, both 56-test XML reports and 131 reconstructed
subset identities. Its three changed paths are documentation only, +292/-5;
production/tests/mirror remain +0/-0. Package hashes match T96. Source-only,
injected, provenance and performance evidence are distinguished; F4 explicitly
retains the public-flush decision instead of promising an invalid direct swap.
Documentation governance and whitespace checks pass. S1 awaits owner review;
T98 remains open and no repair subtask is automatically admitted.

## Owner-requested COM/LPT flush follow-up

Owner asks why other file handling does not need a new flush API, then requests
research before proceeding. No new S or production change is admitted.

Source/history establishes these distinct boundaries:

- Fixed XP nt_com.c submits the normal host byte through WriteFile, waiting
  for pending completion there; it does not use CRT fflush or FlushFileBuffers.
  Its existing native batching route is not the standalone file endpoint.
- The optional standalone synchronous append endpoints were introduced in
  d82389aa7 (M7 T4 S1). That commit introduced serial fputc plus fflush, not
  an XP hardware requirement. Original XP LPT has its own buffer and submits
  a batch through WriteFile; current Compat keeps its 1020-byte threshold.
- Current serial output flushes per byte; current parallel output flushes at
  a completed batch/doc boundary and on close/reset. Neither operation is a
  physical power-loss durability guarantee. No new host timer/worker is needed.
- Snapshot save and Debugger W finish by closing the writer and checking close
  status. DIRECT medium writes already flush internally. Ordinary writer_write
  deliberately leaves buffering intact, which avoids flushing every tiny
  snapshot field. Making every public write flush is not a suitable shortcut.
- Serial/printer machine tests read after machine destruction. They prove final
  contents, not live visibility. Parallel failure tests separately assert exact
  partial-tail retention and flush failure without replay of accepted bytes.
- Storage writer reports exact-write status, not transferred byte count. A
  failed partial write cannot tell the LPT adapter which tail to retain. This
  was omitted from the initial writer-plus-flush migration estimate.

A task-local x64 probe links the unchanged real Storage archive. A one-byte
writer_write leaves native observed size zero; after writer_close it is one.
CRT append leaves size one until fflush, then size two. Observation uses an
explicitly read/write-sharing native handle. The first attempt used Storage
reader, whose sharing mode conflicts with an existing writer; that failure is
not evidence of flush behavior or a new Storage defect.

The same assertions-enabled probe validates the Compat-only candidate with a
non-ASCII UTF-8 filename, two binary appends containing NUL/FF/LF/1A, explicit
fflush and malformed UTF-8 rejection. The candidate is not production code.
Evidence: build/t98/s1/flush-design.log; flush.exit=0. No Computer Use, user
media/config edits, public edits or EXE rebuilds occurred.

Conclusion: do not assume per-byte flush is a hardware necessity, but do not
silently remove existing synchronous submission while repairing path encoding.
Its frequency may be profiled separately if optional file output is a measured
bottleneck. For this bounded fix, a public Storage migration would broaden
scope beyond the actual encoding defect, so the narrow private-open candidate
supersedes the earlier public-flush proposal and its estimates.

## S2 implementation and finite qualification ledger

Owner approval: 批准执行当前既定修复任务. Baseline 3b368d26.
The bounded universe is F1/F2 in the two real media adapters and their existing
callers. Completion requires every member below, full registered dual-width
qualification, released EXEs and actual-commit review. S3/S4 are not part of
this S's completion claim.

- HDD attach: consume/clear both old leases, report any retirement error before
  opening a new path. Failure cleanup already returns failure and keeps it.
- HDD restore: absent slots return the close status after clearing metadata;
  replacement transfers once, commits its metadata and returns retired-close
  status without pretending the old lease can be retried.
- FDD attach/restore: preserve physical drive identity, commit the real current
  medium metadata and return retired-close failure; absent restore propagates
  attach's failure. No retry, rollback or new storage path exists.
- Capacity: original fdisk.c::dosearchid returns signed long byte offsets, and
  host_fdisk_rd/wt accept signed int. Both Windows widths use 32 bits for these.
  A 2-GiB image's last sector starts at 0x7ffffe00; larger images are rejected
  on attach and replacement restore. Existing whole-cylinder geometry remains
  unchanged; this does not add an LBA or large-disk mode. Transfer validation
  compares sector count against remaining extent before multiplication.
- Similar FDD narrowing: attach already rejects byte lengths above LONG_MAX;
  restore now applies the same bound before consuming its replacement.
- Remaining destroy calls: both void hardware-teardown callbacks keep their
  original no-result ABI; rejected-candidate/error cleanup already reports the
  primary failure. Snapshot archive callers check each restore status and stop;
  their mutation phase is explicitly non-rollback. Machine attach/set-media
  callers already propagate false. Its stored startup options are not the
  authoritative mounted-medium view after hardware initialization; capture uses
  the real adapter view and cold reset retains live attachments.

Permanent owner-local tests include the actual hdd_media.c and gfi_image.c in
separate test translation units, sharing only a 39-line scripted-medium fixture.
It creates real 512-byte Lib leases but scripts reported size and close result.
The HDD unit checks both-slot cleanup, consumed replacement ownership, failed
retirement metadata, repeated detach, 2-GiB attach/restore/last-sector access,
oversized/short rejection, negative offsets/counts and x86 multiplication bounds.
The FDD unit checks attach and restore retirement, retained physical identity,
repeated detach and oversized replacement ownership. Native close failure and
a real multi-gigabyte guest disk are not reproduced by these injected tests.

Production estimate resolves to +30/-16, net +14 across two paths; tests are
195 added lines across three files, plus root CMake +3/-2. Test scaffolding
exceeds the original +75-130 estimate because both actual legacy adapters need
separate translation units (host_def.h is not multiply-includable) and scripted
size/close behavior. No production helper/state was added for testing. Initial
combined-TU test compilation failed on that original header; separate-owner
translation units fix the test setup, without changing mirror headers.

Both adapter units pass on x64/x86. Final cached full builds refresh both
assets/binary EXEs. Qualification under build/t98/s2/qualification exits zero:
each architecture passes all 193 registered tests, with zero failures/skips:
177 unit/gates, eight background integration, five native desktop and three
native audio. audit-results.ps1 independently compares registration against
the unique XML case names; both exact sets match. No Computer Use was used.
The x86 build re-emits existing Ninja-log recovery and original macro/switch
warnings; S2 changes none of those original bodies. The recovery causes extra
recompilation, not an authored source change or a measured performance gain.

Accounting method: git diff --numstat against 3b368d26, with production limited
to the two Compat paths, tests to the three new owner-local files, and root
CMake accounted separately. No public tree, mirror, owner INI or media change.
F1/F2 are removed from deferred TODO; S3/F3 and S4/F4 remain unimplemented.
No real native close error or 2-GiB guest installation was reproduced; those
boundaries have injected adapter proof. Existing real-guest snapshot/restart
and package paths pass the integration regression. S2 waits for owner acceptance.

### S2 actual-commit coordinator review

Executor P1 7afc9981 was committed and pushed before this role switch. The
coordinator read its actual CMake, both production diffs, all three test files
and four documentation diffs; the two binary changes were reconciled with the
full link logs and recorded hashes. The twelve-path scope contains no public,
mirror, INI or guest-media change. Accounting independently agrees with
production +30/-16, tests +195/-0 and root CMake +3/-2. Closed leases are not
retained/retried, transfer multiplication follows the capacity guard, and valid
replacement ownership/metadata survive a reported retirement error.

The original request, active packet and six S2 ledger members are satisfied
within their stated proof limits. The coordinator reran the independent XML
set comparison: x64/x86 each 193 unique passing registered cases, no skips or
failures. Documentation/whitespace checks pass. Existing mirror warnings and
Ninja recovery remain disclosed, not fixed. Main matches the pushed executor
commit; only owner-owned untracked snapshot.debug remains and is excluded.
This review accepts executor delivery, not owner acceptance or T98 closure.

Owner subsequently accepts and closes S2: 验收通过 收口s任务 保持t开放.
The [S2 history](../history/M9-T98-S2-media-boundaries.md) records closure;
T98 remained open without an active S; later admissions are recorded below.

## S3 - Owner-directed exact shared import

Owner: 准入一个s任务完成原样导入并构建双exe并跑全量测试.
Baseline f805483f. This explicit scope addition does not rewrite S1's private
audit boundary; it inserts one import S ahead of the two remaining repairs.
Td S22 remains awaiting owner acceptance with no active governance work.

Frozen source: NXVM b2f8b93112897529c0437f37e26c2a70e4b87cb9 worktree;
shared changes are committed as 9b41a82ca, with no uncommitted shared edits.
The eight trees are src/test pairs for lib, emulator, x86-debug and x86-ibmpc.
Audit found 292 identical files, five modified and one added, no deletions.
Only optional Types sin/cos/atan/sqrt aliases, docs/test/linkage and manifests
change. Estimate +34/-3, net +31: C/H production +12, test C +10, CMake +3,
documentation +5 and manifests +4/-3. No new state or runtime implementation.

Finite ledger and completion predicate:
1. Capture source manifest identities and import exact bytes/path sets across
   eight trees. No root helper or NXVM outer machine/product code is imported.
2. Check manifest/LF, Types authority, dependency/naming/test ownership and
   optional math link. The existing inward test helpers remain sufficient.
3. Reuse stable .ninja/.ccache trees, build every target including both package
   EXEs, preserve INI/media/snapshot and record final binary hashes.
4. Per architecture execute all registered unit/gates, background integrations,
   five desktop and three native audio integrations. Partition exact names and
   reconcile executed XML with registration; no failures/skips may be called pass.
5. Executor commits/pushes, then coordinator reviews the actual change and
   evidence. Retain concise results before deleting owned build/t98-s3 scratch;
   final residue gate passes and owner acceptance remains pending.

Every member requires verified disposition; failure requires diagnosis rather
than relaxed contracts or automatic retries. No Computer Use. Existing native
desktop authorization applies; foreground interference and device availability
are recorded independently of unit results. No known TODO is silently repaired.

### S3 source identity

Initial import comparison verifies all 298 files and exact path equality.
The following SHA-256 values identify the eight MANIFEST.sha256 files; their
listed members are checked by the existing registered manifest gates.

| Tree | Manifest SHA-256 |
| --- | --- |
| src/lib | F93120DCE8E2D5C22E656EAED4440044C8DC0025B279185C50F6189C2ECD077B |
| test/lib | 2BBDDF388A33A35CCD8191DA7727E54DE7150BD7AEDA591FB307FB9FF80C1047 |
| src/emulator | 02DF54BC3B8C0CDC8B1C3EE4D3FAE64072AA2B25B5E22847583DDEE5F9EB81A5 |
| test/emulator | 8DB4D1E679E2C66E0FD126F1CA73BEAD933413BCB3FF1BE711118911844D39A8 |
| src/x86-debug | A4CD0C19EE53EFF1A00382A76321D49FC5B7B1772540973DE41F5A6EDFE7B6C3 |
| test/x86-debug | F6296ABB5A81D3EAD78DA2591E8CE55D2007E45C27DCBA122C1A6C08469CC751 |
| src/x86-ibmpc | E1E272D833B594A3380924F24D7B5D16B73117881A9B26D2404CB0F773D5A8F5 |
| test/x86-ibmpc | D225CAF1E669E6A26AB095D821B1794D3D38CB93AEE22DBD0D5B1C9A013B2CE7 |

No importer helper is added: root test registration/manifest tools already
match; four other NXVM root boundary/selftest scripts are deliberately left
out because this import does not require their extra outer-layer support.

### S3 implementation and full qualification

All five ledger members are implemented, with actual pushed-change review
and final scratch cleanup still preceding owner delivery. The final comparison
against NXVM HEAD 930075de1f406f9eddee258fd65714aa19343b36 again finds all
298 files identical; NXVM advanced only outside the frozen shared trees.

Actual shared accounting matches +34/-3 (net +31) across six paths, separated
as estimated above. The six other trees, original mirror, App source and root
test helpers have zero diff. No alternate math path or additional state exists.
The optional aliases retain ISO C contracts and only the Types test consumes
them. Existing Types/dependency/naming/manifest/LF and test-owner gates pass.

Cached configure and full build complete for both widths; each builds ten
edges including the changed Types test, previously uncached package objects
and final EXE/package-test links. The second full build reports no work.
CCPU/device intermediates are reused, not rebuilt unnecessarily. Both package
EXEs are published directly in assets/binary; hashes are recorded in CURRENT.
No new compiler warning/error was emitted in these incremental build logs;
this does not claim historical mirror warning debt is repaired.

Per architecture all 194 registrations execute exactly once in disjoint
phases: 119 shared unit/gates, 59 remaining unit/gates, eight background
integrations, five serial desktop and three serial native audio. All pass,
zero failures/errors/skips. The controller independently compares the union
of ten XML reports with current registrations; full coverage is exact.
Desktop includes both normal and compact package paths. Native PCM/MIDI
passed in the active Console session with audio services running; capability
success here is not a claim about disconnected sessions or arbitrary devices.

The first task-local runner completed x64 194/194, then stopped because its
post-test Get-FileHash cmdlet was unavailable in background Windows PowerShell.
The reporting dependency was removed and only the unexecuted x86 was resumed;
its runner exits zero. No x64 test was retried to hide a failure. Independent
controller verification of both complete XML sets and package hashes exits zero.
Raw first-run diagnostic is retained until review, then retired with scratch.

Protected user files keep their admission hashes: package INI
38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D;
untracked snapshot.debug
AEBD86C4AD8A2B7A41B60B985EE18E11B3BE46C635B5659C8FC3CC75AB9EA310.
No user media changed. Await owner acceptance after commit/push and actual
coordinator review; S4/S5 and T98 closure are not admitted.

### S3 actual-commit coordinator review

Reviewed pushed executor d08ac104 against the original admission, five ledger
members and all actual changed paths. Eleven paths comprise six shared files,
three task/state documents and both freshly linked EXEs. Shared diff remains
+34/-3; P1 total text is +178/-45 including governance records. No original
mirror, private runtime, root helper, user configuration or media change.

Independently rechecked all ten XML reports: each architecture has 194 unique
passing names exactly equal to current CTest registrations, zero skipped/error/
failure nodes. Both published EXEs match their pushed Git objects and recorded
hashes. Build logs confirm new links and second-build no-work results. The
task-local reporting failure is disclosed rather than classified as a product
failure or hidden through test retries. Optional math ownership/linkage and
source identity remain as audited; review finds no blocker.

After confirming no owned runner remains, removed exact build/t98-s3 scratch.
Clean-Build.ps1 -Check passes and only the two persistent roots remain. Raw
logs/XML are retired under Td S22 policy; their concise evidence remains here.
User snapshot.debug stays untracked/untouched. S3 waits for owner acceptance;
T98 and Td S22 owner status remain unchanged, and S4/S5 do not auto-start.

Owner accepted and closed S3 on 2026-10-19: 验收通过 批准收口s任务.
Its durable closure record is [M9 T98 S3 shared import](../history/M9-T98-S3-shared-import.md).
T98 remains open; S4/S5 still require separate owner admission.
