# KVM Text And Frame Contract Clarification

## Request And Status

Original owner request: "你对这个制作一个更具体的修正意见稿 写入proposal 队列首位".
This is the first unnumbered candidate, not implementation admission. T70
remains open with no active implementation S. Baseline: f6ab0dc / shared source
8cb23e9. This proposal is grounded in SoftPC, not in hypothetical NES needs.

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
deferred pointers. Colour and glyph selection should be resolved once by the
producer rather than reinterpreted as VGA control bits in Lib.

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
| B: freeze text schema | KVM/Console public values, VM and Common producers, Window renderer and broker | One explicit character/glyph/colour contract; all callers migrated together; no permanent old/new ABI bridge. |
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
lines; B 150--350; C 40--160 depending on the admitted capacity/status decision;
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
