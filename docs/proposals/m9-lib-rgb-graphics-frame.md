# Lib RGB Graphics Frame Contract

## Status

Unadmitted queue candidate.  This proposal allocates no numeric `T` and does
not alter the active T95 MIDI/WAV diagnostic packet.

## Problem

`kvm_window_graphics_frame` currently publishes one byte per graphics pixel
and resolves it through a 256-entry RGB palette in Lib.  That makes palette
cardinality part of the generic Window contract.  It cannot carry a machine
frame whose final visible pixels depend on more than 256 palette entries, such
as the reported RAIDEN II 2048-colour palette, without reducing or changing
the image before publication.

The current SoftPC adapter demonstrates the unwanted boundary: it copies
indexed DIB bytes into `image.pixels`, then transfers the DIB's 256 RGB
entries to `image.palette`.  Lib's renderer resolves the index later.  A
generic UI consumer must not understand an emulated machine's palette or
pixel-index format.

## Objective

Make Lib graphics frames one canonical, copied RGB contract: 24-bit colour
meaning stored in a 32-bit XRGB word, never a packed three-byte pixel.

```c
/* XRGB32 storage; numeric colour meaning is 0x00RRGGBB. */
typedef lib_u32 lib_rgb24; /* high byte is reserved and must be zero */

typedef struct kvm_window_graphics_frame {
    lib_u32 width;
    lib_u32 height;
    lib_u32 stride; /* pixels, not bytes */
    lib_rgb24 pixels[KVM_WINDOW_GRAPHICS_MAX_PIXELS];
} kvm_window_graphics_frame;
```

Lib Window validates, copies, damages, and presents only resolved RGB values.
This exactly matches the current Win32 Window's 32-bpp `BI_RGB` DIB: on the
supported little-endian host, the `0x00RRGGBB` word occupies its BGRX bytes
without a per-pixel conversion.  The cross-platform Lib contract remains the
numeric RGB value, not that host-memory byte order.
Each product's Machine boundary converts its private source representation to
that format before publishing.  SoftPC resolves its original renderer's DIB
at the existing driver publication boundary; PC and MyNES do the equivalent
at their own Machine boundaries when they adopt this shared corpus.  Text
frames, including their 16-entry text attribute palette, are unchanged.

The objective is visual losslessness of the published pixels.  It does not
make Lib retain a machine's palette identity, DAC registers, bit planes, or
other device state.  Those remain Machine-owned.  If a source presentation
surface itself cannot expose every final RGB value needed for a cited machine,
the task records that upstream owner and stops rather than claiming the Lib
migration repaired the lost source information.

## Scope And Boundaries

- Change the `lib/kvm-window` graphics-frame ABI, copy size, validation,
  renderer, native presentation and owned tests together.
- Remove graphics `palette[256]` and `lib_u8` indexed-pixel interpretation
  from Lib and Emulator's copied-frame transport.
- Change SoftPC's `app-softpc/machine` publisher to produce RGB pixels from
  the original renderer's available presentation output.
- Define the exact downstream migration contract for PC and MyNES; this
  repository does not edit their checkouts or make them build dependencies.
- Preserve original SoftPC rendering, C-VID/DIB ownership, machine palette
  state, guest behaviour, text frames, and Console palette handling.  In
  particular, this is not permission to remove an indexed DIB that the
  recovered SoftPC renderer still uses internally.
- Do not add a format enum, 16-bit fallback, alpha semantics, a second
  indexed route, conversion cache, or host-side palette service.  One RGB
  graphics format is the compatibility boundary.

## Design

The existing renderer has one palette lookup in its graphics loop:

```c
const lib_u8 *source = frame->image.pixels + row * frame->image.stride;
colour = frame->image.palette[source[column]];
```

After migration it reads the canonical colour directly:

```c
const lib_rgb24 *source = frame->image.pixels + row * frame->image.stride;
colour = source[column];
```

`stride` remains a pixel count.  Frame validation retains geometry and
overflow checks; copy size becomes the header offset plus
`stride * height * sizeof(lib_rgb24)`.  Mailbox latest-wins semantics,
sequence handling, and damage derivation remain unchanged.  At the current
1280 x 768 bound, the active graphics storage grows from 983,040 bytes to
3,932,160 bytes, plus the removed 1,024-byte palette.  Admission must measure
all fixed frame buffers and desktop stack/heap placement before accepting this
cost.

SoftPC's publisher presently assumes an 8-bit indexed DIB and copies its
palette.  The repaired publisher must resolve a pixel to `0x00RRGGBB` before
filling `kvm_window_frame`.  It must validate the source representation and
fail explicitly for an unsupported source form rather than truncate a palette
index.  The original renderer and Compat retain their private DIB and palette
mechanics; the only removed palette route is Machine-to-Lib and onward.

## Sequential Plan

### S1 — Freeze contract and migrate Lib transport

Inventory every producer, consumer, fixed frame buffer, copy-size expression,
and test of `kvm_window_graphics_frame`.  Replace the indexed graphics member
with `lib_u32` RGB pixels and delete the graphics palette member.  Update
validation, frame copy sizing, renderer damage comparison and native Window
presentation to consume direct RGB values.  Prove that the high byte is zero
on every producer and that row padding never becomes visible pixels.

Focused tests include a same-frame set of more than 256 distinct RGB values,
colour changes at both ends of a strided row, copy-size/tail-canary checks,
and damage derived from direct RGB equality.  Existing text-frame and Console
palette tests remain unchanged and pass as regression proof.

### S2 — SoftPC Machine publication conversion

At `app-softpc/machine/driver.c`, replace indexed-DIB plus palette export with
per-pixel RGB publication.  Audit the source DIB's bit depth, row order,
stride and palette/DAC availability; do not assume the old 8-bit path covers
all sources.  Keep the original draw/update transaction and dirty ownership.

Focused proof publishes a frame with at least 257 distinct resolved colours
when the available source can supply it, checks RGB channel order and stride,
and retains the existing VGA/text/damage/snapshot cases.  If the original
surface cannot supply the stated RAIDEN II result, record the first loss point
and create a separate Machine/renderer proposal instead of adding a Lib
workaround.

### S3 — Downstream adoption and closure audit

Publish the exact Lib header/API migration notes and corpus hash for PC and
MyNES.  Their future import converts private graphics formats only at their
Machine boundaries.  The SoftPC delivery audits that no production Lib or
Emulator graphics code retains indexed palette lookup or an alternate pixel
format; text-only palette paths are explicitly excluded from this search.

Each admitted S builds x64 and x86 Release packages, runs changed focused
tests and prescribed background/desktop suites, records changed-path line
counts and a similar-issue sweep, commits and pushes its complete delivery.
The later task-level audit verifies the single RGB graphics route and reports
the fixed-frame storage cost and downstream adoption contract.

## Initial File Ledger

| Owner | Likely paths | Required disposition |
| --- | --- | --- |
| Lib frame ABI | `src/lib/kvm-window/frame_interface.h` | Replace indexed payload/palette with RGB pixels. |
| Lib rendering | `src/lib/kvm-window/render.c`, Win32 Window presentation | Direct RGB copy/compare; no graphics palette lookup. |
| Lib/Emulator transport | `src/lib/kvm-window/*`, `src/emulator/machine/*`, `src/emulator/ui/*` | Correct copy sizes and fixed buffers; preserve latest-wins ownership. |
| SoftPC adapter | `src/app-softpc/machine/driver.c`, narrowly related presentation declarations | Resolve original presentation into RGB only at this boundary. |
| Tests | `test/lib/kvm-window/*`, `test/emulator/*`, `test/app-softpc/unit/machine/vga_frame_smoke.c` and direct consumers | Replace index assertions with RGB results and add >256-colour/stride/copy/damage proof. |

The ledger is initial discovery, not an authorization to change every listed
path.  S1 freezes the actual finite universe before implementation.

## Risks And Admission Conditions

- The fourfold graphics-pixel storage affects mailbox and Window allocation;
  admission requires an allocation census and bounded x86 evidence before
  choosing a maximum-dimension or storage redesign.
- `0x00RRGGBB` specifies numeric channel order, not host-memory byte order.
  Native presentation must use an explicitly compatible 32-bpp DIB layout or
  perform its one boundary conversion; it must not reinterpret the value by
  host endianness.
- The 2048-colour report must be reproduced or supported by source evidence.
  The task does not claim a specific game is repaired until the Machine source
  can publish its resolved colours and an appropriate owner-provided runtime
  acceptance path is available.
- No implementation begins while T95 is active.  Owner admission supplies the
  next numeric T, active packet, baseline, exact downstream scope and
  verification budget.
