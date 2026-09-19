# KVM Text And Frame Contract Clarification

## Request And Status

Original owner request: "你对这个制作一个更具体的修正意见稿 写入proposal 队列首位".
Owner subsequently admitted "准入T71" and then "准入s2 开始设计".
T70 is closed; S1 delivered batch A; T71 S2 is the concrete schema design stage
under Current, with no implementation yet. Baseline: 04d76945.
This proposal is grounded in SoftPC, not in hypothetical NES needs.

## Observed Problems

- kvm-console/console.c accepts graphics frames and later returns OK without
  displaying them; its worker acknowledges that publication as consumed.
- kvm-base/frame_interface.h carries byte glyph indices and raster fonts;
  kvm-window/render.c uses the supplied font, but console-broker/win32/console.c
  unconditionally interprets the same bytes through lib_console_pc_glyph().
  This fixed CP437 display mapping cannot reproduce arbitrary downloaded fonts
  or infer the active customer code page. Existing tests verify the fixed map,
  not equivalence with arbitrary raster fonts.
- KVM and logical Console text arrays have fixed 80x25 capacity and fixed
  80-cell row stride. vm/driver.c silently clips larger source dimensions.
- Window text rendering interprets attribute nibbles and bit 3 for secondary
  font selection. These device-like rules are implicit in the copied contract.

These are source observations, not claims of newly reproduced guest failures.
The previously accepted CP437 improvement remains useful; the problem is its
implicit application to every frame, not the existence of the mapping table.

## Goals And Non-goals

Keep current DOS/Win3.1/Win95 display, cursor, handoff, snapshot and input behavior
while making accepted output and representation limits explicit. Preserve one
publication path, complete copied frames, latest-wins/dirty accumulation, worker
ownership and broker serialization. Do not add a renderer framework, dynamic
frame ownership, reference counting, encoding registry, font recognition,
automatic code-page detection or product-specific branches in Lib.

Keep indexed 256-colour graphics and its existing capacity. Do not change mouse
8x16 conversion, keyboard normalization, timing or MVDM. Logical Console and KVM
retain distinct contracts: broker must not depend on KVM. Common routes copied
output; it neither discovers DOS code pages nor interprets VGA attributes.

## Proposed Contract

### Capability And Admission

Check text-only capability at kvm_console_publish_frame(), before mailbox
mutation or notification. Valid graphics input returns the existing unsupported
status; malformed frames return invalid argument. Rejection preserves pending
text. Remove the downstream graphics-success branch; do not add asynchronous
failure for a request that can be rejected synchronously. Common's existing
graphics-status text remains text and must continue to work.

Use one shared validation policy per frame representation. State which dimensions,
stride, palette indices, font ranges and cursor fields are valid. A hidden or
off-surface cursor need not invalidate otherwise valid content; distinguish this
from impossible buffer/font extents. Scan all publishers before tightening rules.

### Character Meaning Versus Raster Glyph

Preferred direction: a logical Console text cell contains an explicit Unicode
character plus defined colours, not an implicitly encoded PC byte. Start with
an explicit single-cell repertoire and rejection/replacement rules; do not
promise surrogate pairs, combining sequences or terminal-width correctness
without implementation and tests. Scalar validity alone does not imply one cell.

KVM text retains the raster representation needed by Window, and carries explicit
Console character meaning supplied by the producer. The first implementation
brief must choose per-cell characters versus a copied glyph-to-character table
after inventorying both fonts, Common status text and all other producers. A
single 256-entry table must not silently conflate two distinct font banks.
Prefer the smallest representation that meets these actual consumers; no borrowed
deferred pointers. Colour and raster glyph selection restructuring is deferred to the independent
[queued proposal](m9-kvm-text-cell-glyph-refactor.md), not required for T71.

SoftPC preserves its current CP437 approximation explicitly where no better
mapping is known. A bitmap has no reliable reverse Unicode mapping. The adapter
must not claim to know a DOS code page merely from glyph bytes. Common-generated
status text supplies its own known characters. The existing fixed mapping may
remain an explicitly named utility; broker must not invoke it implicitly.

On the broker side, compare/cache the actual submitted character/colour values
and commit cache validity only after complete successful native output. A mapping
change with unchanged byte indices must repaint. On Window, changing raster
fonts must continue to repaint independently of Console character meaning.

### Capacity And Layout

Fixed capacity is acceptable; silently clipping the source is not. Inventory
actual text surfaces and supported hardware modes before selecting capacity.
Do not automatically replace 80x25 with arbitrary dynamic allocation or merely
increase a constant in one component. Explicitly distinguish active columns/rows
from storage row stride; validate both ends of each copy.

For a mode exceeding the admitted contract, either extend the bounded contract
end-to-end with demonstrated need, or report a checked unsupported result before
publishing. Do not retain a stale screen indefinitely, silently crop, or claim a
false successful frame. The current boolean copy_frame path cannot by itself
distinguish unsupported content from no new frame: audit the existing error path
and obtain approval for any additional status contract needed before proceeding.
No automatic presenter switch or other UX change is authorized by this proposal.

## Candidate Work Batches (Assign S Identifiers Only At Admission)

| Batch | Implementation boundary | Required result |
| --- | --- | --- |
| A: text-only admission | kvm-console public admission, focused Lib tests, Common callers | Graphics rejected synchronously; pending text retained; normal status text unchanged. |
| B: explicit Console character mapping | Copied mapping, Console values, VM/Common producers, comparison and broker | Remove implicit encoding; preserve Window raster/attribute layout; no second publication path. |
| C: dimensions and capacity | Frame validation, VM copying, both output boundaries and existing failure propagation | No silent clipping; chosen bounds justified by actual modes; no unsupported-content success. |
| D: integration and simplification audit | Changed source/tests/manifests and product verification | No duplicate conversions or obsolete fields/helpers; unchanged accepted UX and snapshot behavior. |

Batch B starts with a reviewed concrete schema and consumer migration table,
not an open-ended implementation. B/C may be combined if their ABI edits would
otherwise create a temporary contract. Stop for owner review if the design
requires new ownership, MVDM changes or a product-visible fallback policy.

Likely affected components: Lib console, console-broker, kvm-base, kvm-console,
kvm-window; Common UI/Machine frame comparison; VM driver. Audit Compat and
snapshot serialization for dependencies before declaring them unchanged.
Initial planning ranges, not measured promises: A 10--40 production changed
lines; B requires a refreshed narrowed estimate; C 40--160 depending on the admitted capacity/status decision;
D no predetermined production changes. Count added/deleted/net production and
tests separately at each batch's preflight and completion; exclude docs,
manifests and EXEs. Refresh estimates from the actual call-site inventory.

## Finite Coverage And Verification

At admission enumerate every frame producer, copy/comparison/serialization site,
mailbox, renderer and cache in the affected components. Each receives a disposition
of migrated, unchanged with evidence, or owner-approved scope decision. Completion
requires all admitted entries verified; no whole-Lib correctness claim.

- Admission: text success, graphics rejection, malformed input, STOP rejection,
  pending frame unchanged on rejection, latest-wins and activation redraw.
- Text: ASCII, CP437 control-picture and box glyphs, high bytes, both font banks,
  palette changes, changed mapping with unchanged indices, custom raster font
  with explicit Console approximation, invalid character/colour handling.
- Layout: active size below capacity, exact limit, over-limit, row-stride bounds,
  invalid font extent, hidden/clipped cursor and producer failure propagation.
- Output: native write failure/clipping cannot update completed-frame cache;
  raw/cooked handoff and notification/prompt behavior remain unchanged.
- Product: both EXEs, full x86/x64 regression and snapshot tests; bounded DOS text,
  Win3.1 roundtrip and Win95 checks using owner media without modifying it.
  No extra guest assets or generated images in assets/; use owned build fixtures.
- Run all four corpus manifest and dependency gates and documentation governance.
  Publish both assets/binary EXEs per implemented S and await owner acceptance
  before any final T closure. No builds are claimed for this planning-only change.

## Alternatives Rejected

Moving all Window rasterization into VM would reduce Lib's text semantics but
requires revisiting cursor blink/freeze and duplicates work; not selected here.
Renaming fields alone cannot remove implicit encoding. Merging Console and KVM
frame owners would violate the existing dependency boundary. An arbitrary-size
Unicode terminal or universal font engine is unnecessary for this bounded repair.

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

## T71 S2 Concrete Design (For Owner Review)

Owner split request: "第二项请你拆分到新的t任务proposal加入队列".
The broad per-cell/colour/raster-atlas design has moved to the independent
[queued proposal](m9-kvm-text-cell-glyph-refactor.md). It is no longer T71 scope.

S2 is narrowed to explicit copied Console character mapping. Preserve existing
KVM byte indices, attributes, colours, primary/secondary raster fonts and Window
rendering. Preserve SoftPC's current CP437 approximation, but make it producer
supplied instead of an implicit broker interpretation. Keep one copied frame
and one publication path, without borrowed pointers or an encoding registry.

Before implementation, specify the copied mapping's placement/count, its two
font-bank semantics, logical Console character values, producer ownership and
all copy/comparison/cache updates. A mapping change with unchanged text indices
must repaint. Common status text must remain correct. Do not claim a bitmap
identifies a code page. The exact reduced schema and refreshed diff estimates
remain design work; the earlier 250--450-line broad migration estimate no
longer describes this S.

S3 retains capacity/layout review and S4 integration review. This split does
not mark any implementation complete. No code, tests, binaries or media changed.
