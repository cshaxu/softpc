# Explicit KVM Text Cells, Colours And Raster Glyphs

## Request And Queue Status

Original owner request: "第二项请你拆分到新的t任务proposal加入队列".
This is an independent, unnumbered candidate, appended to the queue. It is not
T71 implementation scope and receives a T number only upon owner admission.
It retains the broader design considered in T71 S2 P1 (d1afdbd5) as a starting
point, not an approved ABI. Rebase it on the completed T71 contract at admission.

## Objective And Boundary

Separate raster glyph selection and colours from PC attribute interpretation,
while preserving current SoftPC display and input behavior. T71 separately owns
removing implicit Console character mapping; this task must reuse that outcome,
not implement a competing mapping path or require T71 to wait for this refactor.

The proposed per-cell/atlas form below is a candidate. Compare it against the
post-T71 copied mapping representation before selecting it. Justify ownership,
mental-model benefit and memory cost, not abstraction for its own sake.

## Candidate S Breakdown

1. Audit the post-T71 producers/consumers; choose the smallest cell/colour/glyph
   schema. Record exact file, line and memory estimates and obtain design review.
2. Migrate the chosen schema end-to-end in one coherent ABI change; remove old
   fields and decoders without a permanent compatibility bridge. Preserve
   character mapping, palette, font, cursor and snapshot semantics.
3. Audit simplification and all changed paths; run dual-width complete builds,
   regression, manifests and dependency gates; publish both EXEs for owner tests.

Each implemented S reports estimated versus actual production/test diff,
preserved ownership and verification. Stop if a new renderer, lifecycle,
dynamic ownership or product-visible fallback is needed. Final task closure
requires owner acceptance; no implementation is admitted by queue placement.

## Retained Design Candidate (Requires Revalidation)

The following was drafted before the split. Its file/line estimates and mapping
relocation assume the pre-T71-S2 baseline; they are not incremental promises for
this future task. Recalculate them after T71 and reuse any completed work.

This stage edits documentation only. It proposes a source ABI migration, not
implementation approval of every detail below. Preserve S1 admission and current
product display policy. S3 owns text-capacity/status propagation decisions;
S2 leaves 80x25 capacity and fixed 80-cell storage stride unchanged.

### Data Shape And Ownership

Prefer explicit per-cell values over encoding IDs or copied per-font Unicode
lookup tables. The existing Window has two 256-glyph raster banks; expose them
as one fixed 512-glyph atlas, without adding a glyph allocator or registry.

```c
/* kvm-base owns this copied display value. Proposed names. */
typedef struct kvm_text_cell {
    lib_u16 character;   /* explicit BMP character for Console output */
    lib_u16 glyph;       /* 0..511: index into the copied raster atlas */
    lib_u8 foreground;   /* 0..15: index into text_palette */
    lib_u8 background;   /* 0..15: index into text_palette */
} kvm_text_cell;

/* Replaces text[], attributes[], font[], secondary_font[],
 * and attribute_font_select in kvm_frame. */
kvm_text_cell cells[KVM_TEXT_COLUMNS * KVM_TEXT_ROWS];
lib_u8 glyphs[512u * 16u];

/* Console owns its independent output value; no KVM dependency. */
typedef struct lib_console_text_cell {
    lib_u16 character;
    lib_u8 foreground;
    lib_u8 background;
} lib_console_text_cell;
```

Keep text_palette/palette, dimensions, font_height, cursor fields and graphics
representation. Glyphs remain 8 pixels wide, each with 16 storage scanlines;
font_height selects active scanlines. The caller supplies both representations:
character is not derived from glyph inside Lib. A single glyph can have a
different Console approximation per cell without another map. No independent
mutable encoding state can fall out of step with the copied frame.

This consumes approximately 6 KiB more per KVM frame and 2 KiB more per logical
Console frame at 2000 cells; exact aggregate sizeof/alignment and the number of
live copies must be measured before implementation acceptance. No new allocation
or worker is needed. Atlas bytes are unchanged. This cost buys direct semantics;
do not introduce packing pragmas or bit fields to hide it.

### Producer Conversion

- VM driver reads original byte/attribute and font-selection facts. It emits
  glyph = byte + (selected_secondary ? 256 : 0), explicit colours equal to
  today's low/high attribute nibbles, and today's CP437 approximation as character.
  Bit 3 interpretation is confined to this existing VM conversion. Preserve
  its current colour outcome even when selecting the second font; this task
  does not silently change VGA intensity/blink emulation.
- Existing Compat font copy fills the atlas halves through its unchanged two
  destination arguments. Original devices/Compat remain the hardware owners;
  no new font extraction path or MVDM edit.
- Move the existing fixed PC-byte mapping unchanged into a static VM driver
  helper, removing lib_console_pc_glyph from Lib public ABI and its implicit
  broker use. Move its full mapping proof to product tests; shared tests instead
  test supplied characters. Do not duplicate the table or invent detection.
- Common UI status generation fills known ASCII characters and explicit colours.
  Its status surface remains Console-only and does not need a raster font.
  SoftPC's actual status string is ASCII. Declare that bounded source contract;
  reject unsupported high bytes before publication rather than guessing an
  encoding. A general UTF-8 status API is outside this batch.

Custom fonts remain exact in Window and explicitly approximate in Console.
The same CP437 approximation may be used for both banks by SoftPC; the generic
cell representation does not force that choice on other producers. No existing
customer code page is claimed to have been discovered.

### Consumer And Validation Rules

Window renderer reads cell.glyph and colours directly. Delete VGA attribute
tests and font-bank selection there. Console leaf copies character/colours and
existing cursor metadata into its independent text frame; broker forms native
colour attributes only at the Win32 boundary and writes the supplied character.
The native cache compares actual cells, not old byte indices. Palette-only and
font-only changes still trigger their respective outputs.

Initial character capability is one explicit BMP code point per cell, not UTF-16
strings. Exclude surrogate code units, NUL and C0/C1 controls; producers emit
U+0020 for blank. No surrogate concatenation, grapheme shaping, combining marks
processing or width classification is introduced. Each value occupies one API
cell; host glyph availability/visual width is not guaranteed for arbitrary BMP
input. Full Unicode terminal typography is not promised. Existing ASCII/CP437
outputs must retain their exact chosen code points and observed output.

Validate active cells only: glyph <512 and colours <16; Console character rules
at text publication. Padding cells must be deterministically initialized by
producers for reliable comparison; never treat C struct padding as semantics.
Keep validation in each owning value boundary and use those helpers at public
admission, rather than duplicating literal checks across leaves/backends.
The logical Console remains safe for direct non-KVM callers. Do not change
graphics checks or add encoding validation to graphics payloads.

Use effective font height under the existing zero-means-default convention in
this migration; stricter font/dimension rejection belongs to the separately
reviewed S3. Cursor shape, phase, native blinking and Window freeze remain
unchanged. This prevents a schema change from silently redefining cursor UX.

### Complete Production Migration Ledger

| Path / owner | Required migration or retained reason |
| --- | --- |
| lib/kvm-base/frame_interface.h | New cell/atlas values; unchanged complete-prefix copy and graphics storage. |
| lib/console/console_interface.h, console.c | Independent Unicode/colour cells, checked text values; remove PC mapping API/table. |
| lib/kvm-console/console.c | Explicit cell conversion; keep admission, worker, activation, STOP and failure lifecycle. |
| lib/kvm-window/render.c | Direct glyph/colour rendering; no VGA attribute decoder. |
| lib/console-broker/win32/console.c | Native marshalling and cache cell representation; preserve full-write acknowledgement/output locking. |
| vm/driver.c | Sole product byte/attribute/font-selection conversion and CP437 approximation; copied atlas destinations. |
| common/ui/ui.c | Known status characters/colours; no DOS mapping or font detector. |
| common/machine/machine.c | Compare new cells/atlas; detect character-only, glyph-only, colour-only and font-only changes. |
| kvm-base/mailbox.c, common/session/session.c | Retain generic copied-frame path; verify new prefix is fully copied, no parallel buffer. |
| vm/machine.c, compat/video.c | Existing hardware/font access stays; no signature change expected. |
| vm/snapshot_image.c, Compat archives | No kvm_frame serialization found in snapshot_image: archives encode original CPU/device/media state. Verify device archive and cross-width restoration; no on-disk format/version change planned. |
| Linux Console backend/worker | Existing unsupported endpoint remains; compile shared headers without creating a new presenter. |

Searches used: frame/text/attribute/font/mapping references under src and test;
all kvm_frame and lib_console_text_frame consumers, Common text comparison,
Compat font extraction and snapshot_image serialization entry points. Before
implementation freeze the exact test list and expand the archive review; no
snapshot-format-independence claim rests on the top-level codec alone.

### Tests, Size Estimate And Exit

Identified test consumers include Lib Console I/O/broker display/retirement,
Window presentation/rendering/capture/input-admission; Common machine/composition;
product runtime cursor/VGA/restart and the existing runtime boot diagnostic.
Update fixture producers instead of keeping legacy field aliases. No silent
test deletion. Add explicit coverage for both atlas halves, equal glyph with
different character, equal character with different glyph, mapping/colour/font
changes without dimension changes, invalid active cells, and native cache failure.
Relocate CP437 equivalence coverage with its product-owned mapping.

Initial refined estimate: 9 production C/H files about 250--450 changed lines,
net -20 to +100; roughly 14 existing test/diagnostic files, 180--350 changed
lines. These are estimates including table relocation, not implementation
results. No new production component/file is needed. If cell semantics require
additional public policies or broad changes outside this ledger, stop and revise.

Design exit: owner reviews the explicit cell schema, bounded Unicode semantics,
CP437 ownership, memory cost, complete migration and S3 split. Implementation
requires subsequent direction; then dual-width complete tests/builds, four
manifests, dependency/documentation gates and snapshot roundtrip must pass.
Current EXEs remain S1 and no new executable verification is claimed here.
