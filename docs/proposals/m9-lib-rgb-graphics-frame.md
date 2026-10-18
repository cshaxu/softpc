# Lib RGB Graphics Frame Contract

## Status

Owner delegates admission as T96 on 2026-10-17 after T95 closure. CURRENT owns
the active packet. S2 delivered the atomic ABI/producer migration at fa569001,
reviewed at 94a0debd. Owner permits S3 without manual S2 acceptance; final S4
remains the buffer-simplification task. The problem/examples below describe
the pre-migration baseline, not a second current graphics format.

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
typedef struct kvm_window_graphics_frame {
    lib_u32 width;
    lib_u32 height;
    lib_u32 stride; /* pixels, not bytes */
    /* Numeric 0x00RRGGBB, not an OS byte-order or alpha contract. */
    lib_u32 pixels[KVM_WINDOW_GRAPHICS_MAX_PIXELS];
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
const lib_u32 *source = frame->image.pixels + row * frame->image.stride;
colour = source[column];
```

`stride` remains a pixel count.  Frame validation retains geometry and
overflow checks; copy size becomes the header offset plus
`stride * height * sizeof(frame->image.pixels[0])`. Mailbox latest-wins semantics,
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

### S1 — Inventory and freeze the coherent migration design

Inventory every producer, consumer, fixed frame buffer, copy-size expression,
and test of `kvm_window_graphics_frame`. Measure both-width layouts and bounded
allocation feasibility. Freeze validation, source conversion and test ownership.
This is a documentation/design delivery, not a production migration or runtime
qualification. The original split of Lib ABI and SoftPC producer is superseded:
they must ship together, with no compatibility format or broken intermediate.

### S2 — Atomic Lib ABI and SoftPC publication migration

Replace the indexed graphics member with `lib_u32` RGB pixels, delete the
graphics palette member, update byte sizing and direct-colour rendering, and
migrate every affected producer/test in the same qualified delivery. Emulator
copies already delegate to the owning Lib helper; no new transport is needed.
Retain native 32-bpp surface, common pixel damage helper and latest-wins rules.

At `app-softpc/machine/driver.c`, replace indexed-DIB plus palette export with
per-pixel RGB publication.  Audit the source DIB's bit depth, row order,
stride and palette/DAC availability; do not assume the old 8-bit path covers
all sources.  Keep the original draw/update transaction and dirty ownership.

Focused Lib proof publishes at least 257 (preferably 2048) distinct RGB values
in one synthetic frame, checks high-byte rejection, channels, row padding,
copy extent/tail canaries and damage. SoftPC tests validate its actual 8-bit
source conversion, palette-only updates and existing VGA/text/snapshot paths;
they must not manufacture a 257-colour source claim. Move oversized automatic
test frames off the stack without changing test ownership or product storage.

### S3 — Downstream adoption and RGB contract audit

Publish the exact Lib header/API migration notes and corpus hash for PC and
MyNES.  Their future import converts private graphics formats only at their
Machine boundaries.  The SoftPC delivery audits that no production Lib or
Emulator graphics code retains indexed palette lookup or an alternate pixel
format; text-only palette paths are explicitly excluded from this search.

### S4 — Final frame-buffer simplification and task closure

Owner request: "把这个无份清理的任务排到本t的最后一个s任务 然后接着做现在的升级".
Audit the five existing frame buffers after RGB qualification, then remove
copies/lifetimes that can be eliminated through the existing path. Start with
Session's retained copy and prove route-switch/no-frame behavior. A current
responsibility is not proof that every buffer is minimal. Retained buffers need
explicit concurrency/lifetime evidence; reductions must preserve failure,
latest-wins and rendering semantics. No frame pool, reference-count framework,
second publication path or lock held across painting merely to reduce count.
Estimate and qualify the actual minimal change within S4. Whole-T closure audit
follows this final S, not S3. S2 deliberately retains the five-buffer model;
its measured cost is accepted for this stage, not declared irreducible.

Each code-changing S builds x64 and x86 Release packages, runs changed focused
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

The actual S1 ledger below supersedes these preliminary path estimates.

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

## S1 audited design and finite coverage ledger

Owner confirms "批准开始" after the T96 S1 admission. Audit baseline is
f6385a76719c41ea60124ab871d5ea333221b8a9 (production unchanged from 6c3b604f).
Frozen universe: tracked src/test/tools consumers of `kvm_window_frame`,
`kvm_window_graphics_frame`, `emulator_machine_frame`, graphics pixels/palette,
the palette constant and hardcoded frame sizes. Commands: `rg -n` for those
symbols and direct member accesses, then inspect each producer, transport,
allocation and native surface owner. Units of coverage are paths/responsibilities;
dispositions are change, retain with reason, or explicit external non-goal.
Completion means every category below has a disposition and test receiver.

### Production path and minimum changes

1. `src/lib/kvm-window/frame_interface.h`: delete graphics palette/constant;
   change pixels to existing `lib_u32`; multiply active extent by element size.
   No new `lib_rgb24` alias or Types domain declaration. Geometry validation
   remains first; reject a visible pixel with high byte nonzero as
   INVALID_ARGUMENT before any copy/publication. Padding is uninterpreted and
   copied, never rendered; inspect only width pixels per row for colour validity.
2. `src/lib/kvm-window/render.c`: read the RGB word directly and call the same
   existing compare/write/damage helper. Do not add a row buffer, cache or
   full-frame redraw path. Text decoding and cursor invalidation stay intact.
3. `src/app-softpc/machine/driver.c`: validate the actual published DIB before
   consuming dirty; require the existing top-down 8-bit BI_RGB form with one
   plane, matching positive width/negative height and the owned 256-entry
   palette. Existing bounded dimensions make aligned byte stride arithmetic
   safe. Resolve each source index through RGBQUAD into the final destination
   word, directly, with destination stride=width. No intermediate RGB palette
   cache, extra conversion surface or format switch. Set valid only afterward.
   Missing completed damage remains OK/no-frame; malformed/unsupported source
   is an explicit error, not an absent frame. Use existing test-visible DIB
   metadata for negative fixtures, restoring it afterward.

The three files are the expected production C/H delta. Geometry helpers,
`window.c`, `window.h`, `window_interface.h`, `render.h` and Win32 `component.c`
already use typed fields/sizeof or opaque active-byte copy. Retain them. Win32
creates a top-down 32-bpp BI_RGB surface; numeric 0x00RRGGBB fits the existing
supported little-endian BGRX storage. Linux remains explicitly unsupported.
Neither platform needs a second conversion/renderer or changed cursor policy.

Emulator `machine/{frame_interface.h,machine_interface.h,machine.c}`,
`session/session.c`, `ui/{ui.c,ui_interface.h}` retain their existing typed
copy/allocation/dispatch behavior. Machine uses two heap buffers; Session embeds
one in its heap allocation; UI forwards the reference and makes only a small
Console text frame. Base mailbox knows byte length only; no format knowledge
or new transport is added. X86 packages have no graphics interpretation to move.

SoftPC `machine/{machine.c,machine.h}` simply expose the Compat surface;
`compat/dib_surface.c/.h` owns the indexed presentation. Its initializer and
bind always expose 8-bit top-down BI_RGB with 256 RGBQUAD slots. Original 1-bit
painters are expanded there; 8-bit painters share the composed pointer surface.
Palette changes already mark full damage; outer update/body readiness remains.
These paths and all original softpc.new files are retained, not rewritten.
This source can express at most 256 colours simultaneously: it does not prove
RAIDEN II's reported 2048-colour source. That downstream machine is outside
this audit; Lib's independent synthetic test proves the transport capacity.

### Allocation and bandwidth census

A disposable strict C11 probe compiled and ran on x86 and x64, defining the
proposed layout alongside current headers (not replacing production headers).
Both executions returned zero and reported identical sizes:

| Payload | Current bytes | Proposed bytes | Increase |
| --- | --- | --- | --- |
| Window frame | 984080 | 3932176 | 2948096 |
| Machine frame including sequence/map | 985108 | 3933204 | 2948096 |
| Five production frame payloads | 4923484 | 19663964 | 14740480 |

The five are Machine staging/publication (two), Session capture (one), Window
mailbox (one), Window worker context (one). Each is heap-backed; metadata outside
these payloads is excluded and unchanged. The probe allocated/touched all five
proposed payloads concurrently on both widths. This is allocation feasibility,
not a gameplay/performance benchmark or a guarantee under memory exhaustion.
Native RGB surface (up to 3932160 bytes) and private Compat DIBs already exist
and do not grow. Even a text-only machine retains the larger union buffers;
text active-copy size stays 32480 bytes. No stack-size linker change is needed.

Graphics pixel offset becomes 16 rather than 1040. Active copy is
`16 + 4 * stride * height`; at 640x480 it is 1228816 vs 308240 bytes. There
are three frame memcpy handoffs (Machine->Session->mailbox->worker), not an
additional producer memcpy after conversion. At a hypothetical 60 complete
640x480 publications/second these payload copies alone total about 211 MiB/s
instead of 53 MiB/s, excluding read/write doubling, rendering and validation.
This is arithmetic, not a measured frequency or speed claim. Existing validation
is reached at several copy/render boundaries; added high-byte checking costs
visible-pixel scans. Do not claim a performance gain or add a trusted bypass.
S2 must observe publication/consumption cost with existing qualification, retain
no-dirty suppression, and report any material regression before closing.

Probe evidence is retained under ignored build/t96/s1 (layout source, hidden-job
logs and zero exit). The first unprivileged launch produced no completion marker
and is inconclusive; the hidden retry completed both architectures without
warnings. Budget 120 seconds, output below 1 MiB, executor owns cleanup. No
native Window, guest or audio device was opened by this probe.

### Test ownership and complete affected-path disposition

Expected test edits, nine existing files; no new test framework:

- Lib `kvm-window/kvm_frame_copy_smoke.c`: new sizeof, element-sized extent,
  byte-tail canaries, high-byte invalid publication/copy, strided padding,
  geometry boundaries and text/graphics transitions. Replace frame compound
  resets with direct memset/field initialization if they create large temporaries.
- Lib `kvm-window/kvm_frame_damage_mouse_smoke.c`: direct RGB damage, 2048
  distinct synthetic colours, unchanged-colour and padding-no-damage, skipped
  publication, text/cursor regressions. Its automatic text/tall frames must be
  static test-local storage or owned heap, with explicit reset between cases.
  Machine palette identity assertions no longer belong in this Lib test.
- Lib `kvm-window/win32/kvm_window_capture_contract_smoke.c`: update actual
  message-to-render fixtures to direct colour and preserve invalidation/cursor
  behavior. Native calls stay mocked in this unit.
- Emulator `machine/machine_wait_smoke.c`: new sizeof, RGB-valued skipped-frame
  payload and retained invalidation/tail preservation. No independent Lib colour
  validation tests here; audit compound resets for stack temporaries.
- Emulator `machine/machine_smoke.c`: move its automatic Machine frame off
  stack, retain publication and active-copy assertions.
- Emulator `session/session_frame_smoke.c`: RGB forwarding/latest publication,
  no graphics character-map copy and no palette field.
- App `unit/machine/vga_frame_smoke.c`: compare published RGB to actual DIB
  indices/palette across original modes; palette-only mutation must alter visible
  RGB, unused entries must not change pixels. Add channel/odd-row-padding/source
  rejection cases through the existing publisher, no test-only production API.
- App `integration/snapshot_transaction_smoke.c`: expected colours are resolved
  RGB, not BIOS palette indices (current 0Ch/0Ah inputs); establish expected DAC
  colours independently of publisher conversion. Move its two automatic frames
  off stack; saved guest device state/format stays unchanged.
- App `integration/runtime_restart_boot_smoke.c`: move both automatic frames
  in prompt observation/failure reporting off stack; preserve original assertions.

Other direct frame users retain semantics and need only regression/rebuild:
Lib `kvm-window/{text_frame_smoke.c,win32/kvm_input_admission_smoke.c}` and
`integration/kvm_window_retirement_smoke.c`; Emulator `ui/composition_smoke.c`,
`session/session_monitor_smoke.c`, `product/composition_smoke.c`,
`machine/machine_fixture.c`; X86-debug `product/machine_smoke.c`; App integration
`{presentation_shutdown_smoke.c,runtime_cursor_smoke.c,runtime_smoke.c,
command_provider_smoke.c}`. They use pointers, heap/static storage, text or
callbacks without indexed graphics assertions. Window embedded fixture users
`kvm_leaf_control_capacity_smoke.c` and `win32/window_keyboard_lifetime_smoke.c`
also use static storage. Existing Base mailbox tests remain opaque, Console
palette tests remain text-only. Root tools contain no direct graphics member
interpretation in the frozen search; ignored past diagnostic probes are not
release/build dependencies and are not silently declared compatible.

### Estimated implementation diff and proof plan

Estimates are paired small/large endpoints, not measured implementation totals:

| Owner / files | Added | Deleted | Net |
| --- | --- | --- | --- |
| Lib frame header | 12-18 | 4-6 | +8 to +12 |
| Lib renderer | 2-4 | 2-4 | 0 |
| SoftPC driver | 20-28 | 12-18 | +8 to +10 |
| Production total, 3 files | 34-50 | 18-28 | +16 to +22 |
| Lib tests, 3 files | 90-130 | 30-45 | +60 to +85 |
| Emulator tests, 3 files | 15-25 | 12-18 | +3 to +7 |
| App tests, 3 files | 55-85 | 20-35 | +35 to +50 |
| Test total, 9 files | 160-240 | 62-98 | +98 to +142 |

Documentation/manifests/binaries are separate: update Lib Window README, current
architecture and affected src/test manifests (Lib, test Lib, test Emulator);
App tests belong to root gates, not shared manifests. No expected Emulator
production edit, no Types alias, no mirror delta and no snapshot format change.
Exact counts and all changed paths must be reported from S2's actual diff.

S1 validation is source inspection, dual-width layout/allocation probe and
documentation gate only. S2 qualification is dual-width cached full builds,
two published EXEs, all public and App units/gates, background integrations,
five desktop and three native-audio cases per current architecture registration.
Report discovery of changed registration counts and native availability honestly.
Focused tests above must pass before full runs. Existing snapshot/restart/video
mode matrix protects no-frame readiness, pointer/body update ordering and text.
S3 is corpus/adoption audit, not a second conversion implementation; S4 owns
buffer simplification and the subsequent whole-T audit.
Downstream imports rebuild all ABI consumers together and convert their own
private images at their Machine boundary; never transmit old palette indices.

### S1 executor review

Compared the design with the original owner-approved queue objective and active
read-only packet. Removed the unnecessary colour typedef from the proposal,
kept Types unchanged, corrected the split that would break intermediate builds,
and separated >256-colour Lib proof from SoftPC's 256-colour source limit.
Production/test tracked delta is +0/-0. No EXE, INI, media or checkpoint change.
The code estimates above belong to proposed S2, not this design delivery.

## S2 implementation and convergence ledger

The ABI and producer migration is one change against c951e24c. The only
production changes are Window frame storage/validation/copy extent, Window
direct-colour rendering and App Machine indexed-DIB conversion. No Types,
Emulator production, X86, Compat, preserved mirror or snapshot-format change.
The five production frame owners remain intact for final S4.

Counted with `git diff --numstat c951e24c -- src test`, excluding manifests and
README: production three files +27/-19 (net +8); tests ten files +147/-53
(net +94). Documentation, manifests and the two EXEs are separate.

| Frozen obligation | Implementation / proof |
| --- | --- |
| RGB contract, channel order and >256 colours | Lib damage test publishes/renders 2048 distinct colours and exact first/last row changes. |
| Geometry, stride, padding and copy extent | Existing capacity/text tests retained; padded RGB rows, byte-tail canary and full-capacity copy updated. |
| Invalid publication atomicity | Visible nonzero high byte rejected without changing mailbox generation, waking renderer or overwriting destination. Padding remains uninterpreted. |
| Machine source boundary | Actual driver tested against indexed DIB across existing mode matrix, palette-only changes and padded 3-pixel rows. Invalid DIB headers fail before consuming damage. |
| Snapshot and restart | Existing guest tests retain state/flow assertions; expected DAC RGB values independently established as FC5454/54FC54. |
| Allocation and unchanged ownership | Automatic test frames moved to static storage; production Machine, Session, mailbox and worker remain heap-owned. |
| Old indexed contract removal | `rg` over src/test finds no `image.palette` or `KVM_WINDOW_GRAPHICS_PALETTE_ENTRIES`; private machine/text palettes remain. |
| Qualification | Dual-width builds, 177 unit/gates and 16 integrations per architecture; retained XML/log evidence under build/t96/s2. Completion requires all final results passed, not the first attempt. |

First attempts exposed two test-fixture defects: the new RGB mailbox case did
not select its existing wake notification, and two automatic Session fixtures
in session_monitor embedded the enlarged frame. The latter adds a tenth test
path to the nine-path S1 inventory. Enable the existing notifier and move those
test objects to static storage; do not alter production transport. Preserve
the failed logs and requalify. No tests or assertions were disabled.

Storage is still 19,663,964 bytes across five payloads versus 4,923,484 before
(+14,740,480 bytes); this is a stage cost, not proof of minimality. RGB copies
also move four bytes per pixel. Correctness qualification is not a throughput
benchmark or a claim that RGB is faster. S4 owns the subsequent buffer audit.

Final verification: each architecture passes 193/193 distinct tests without
skips (177 unit/gates, eight background, five desktop, three native audio).
Both focused sets pass 13/13. Evidence is retry1/focused-*.xml,
retry1/unit-x86.xml, retry1/integration-*.xml, retry1/native-*.xml and
recheck/unit-x64.xml under build/t96/s2. The retry1 job retains exit 1 for its
earlier x64 fixture failure; the explicit final x64 rebuild/recheck exits 0.
Do not interpret or erase that earlier failure as a clean first pass.
Both package EXEs were rebuilt/deployed. Existing mirror warnings and three
unchanged VGA-test pointer-cast warnings remain; x86 Ninja recovered its prior
log and rebuilt affected targets. No external repository or user input changed.

Executor review: inspected actual production/test diff, all changed frame
callers and automatic embedded-frame declarations. Production allocations
remain owned by their existing heap objects; tests alone move large automatic
objects to static storage. Shared manifests, full boundary gates and
documentation governance pass. Delivery is ready for coordinator review and
owner acceptance; this does not close S2 or admit S3.

Coordinator review of executor commit fa569001: inspected the committed diff
and its changed-path inventory rather than relying on the executor summary;
checked direct RGB ownership, validated byte extents, no new production buffer
or renderer, and retained snapshot/state assertions. Reconciled first failures
with the fixed test fixtures and final XML: x64 recheck 177/177, x86 177/177,
both architectures 8+5+3 integrations without skips. Reviewed source manifests,
EXE hashes and untouched user inputs. S2 is verified and delivered, awaiting
owner acceptance; S3 is not admitted and S4 remains the final buffer task.

## S3 RGB corpus audit and downstream adoption

Scope is the S2 delivery at full commit
`94a0debd2b67bf4663159b455dc002bcb4eed7b0`. S3 changes documentation only, not any source/test corpus or
runtime binary. Owner authorizes S3 without manual S2 acceptance. This section
defines adoption instructions, not a claim that another repository is migrated.

### Bounded audit ledger

Universe: Window graphics schema/validator, copy helpers, opaque Base mailbox,
Machine publication/copy, Session forwarding, UI routing, root/native renderer,
SoftPC DIB producer and their direct tests. Each entry must have an inspected
owner and retained proof; unrelated device/audio APIs are not a whole-tree
quality certification. No new runtime defect was confirmed in this scope.

| Obligation | Disposition and evidence |
| --- | --- |
| Single graphics representation | frame_interface.h has only u32 RGB; src/test searches for image.palette, the removed palette-capacity macro and old sizeof values return no matches. |
| Copy extent and rejection | Window validates before mutation, copies offset + stride*height*sizeof(pixel); bounds precede indexing. Emulator copies through that helper before changing sequence/text resources. S2 copy/damage tests pass. |
| Opaque handoff | Base mailbox uses caller-provided byte count/capacity and generation-bound acknowledgement; no pixel interpretation. Machine owns staging/published slots, Session forwards the current full frame, UI does not convert pixels. |
| Direct rendering | render.c graphics arm reads u32; shared update_pixel compares/writes damage. Win32 keeps its 32-bit top-down BI_RGB surface; Linux remains explicitly unsupported. S2 2048-colour and native tests pass. |
| Retained palette references | Window render.c and Emulator machine.c/ui.c references are text-only. Machine callback comment includes source palette change, not a transported graphics palette. Compat's indexed surface is private and intentional. |
| SoftPC producer | driver.c resolves its validated 8-bit indexed source at the Machine boundary; rejects invalid/unsupported DIB before taking dirty. No mirror or snapshot-format change. S2 actual-source/palette/snapshot tests pass. |
| Allocation and cost | Five existing production payloads are heap-owned; enlarged test automatic objects were removed in S2. No new buffer/route. Repeated validation and larger copies remain costs, not a speedup claim; S4 owns minimization. |
| Corpus identity | All eight manifests checked against current files: 121/76/29/22/14/8/12/6 entries, total 288, all match. Exact Git tree identities below include package metadata. |
| Proof limits | S2 x64/x86 193/193 is existing runtime evidence. S3 does not rerun unchanged code or claim user/RAIDEN/MyNES validation. No external checkout read or modified. |

Searches use `rg` over src/test for image pixels/palette/stride, Window copy/
size/validate helpers, indexed/8-bit/palette references, and automatic frame or
Session declarations; inspect every production hit along the path above.
Source derivation and S2 dynamic evidence are distinct. Documentation gate and
unchanged source/test/EXE hashes complete this documentation-only delivery.

### Atomic import checklist

1. Import the chosen shared src/test pairs from the fixed revision together,
   including deletion entries and manifests. Do not take just frame_interface.h
   or mix an old binary consumer with the new union size.
2. In the importing Machine producer, resolve each source pixel's final colour
   using its own complete palette/planes/format. Publish
   `(r << 16) | (g << 8) | b` with channels widened to lib_u32. An index of 1
   is not a red pixel: publishing numeric 1 means RGB 000001.
3. width/height remain bounded by 1280/768; stride is pixels, at least width
   and at most 1280. Active storage is stride*height u32 elements. Only visible
   pixels require a zero high byte; padding is copied but not drawn. No alpha,
   byte-order flag, alternate format or palette service is introduced.
4. A source palette-only change affecting displayed colour requires a new
   complete RGB frame. Never publish only a dirty rectangle or 256-entry
   reduction of a richer source palette. Source palette/register state and
   snapshot serialization remain owned by the importing machine.
5. Use existing copy/size helpers after validation; update indexed fixtures,
   memcpy byte counts and allocations. Window frame sizeof is 3,932,176;
   Emulator Machine frame is 3,933,204 on both qualified widths. Do not place
   either object or a containing Session/Window on a small thread stack.
6. Preserve no-frame semantics (OK plus invalid frame), explicit error status,
   sequence/latest-wins handling, text fonts/palettes/maps and input contracts.
   Do not add a second indexed compatibility route.
7. Rebuild every ABI consumer and run the importing product's own units and
   real frame/palette/snapshot integrations. Lib's synthetic 2048-colour proof
   does not prove any external producer retains 2048 distinct source colours.

No root test infrastructure change is required by this migration;
test/register.cmake remains Git blob f13693901137ea3310154fabb23b3b67962afb42.
The exact shared directory trees at the fixed revision are:

| Directory | Git tree object |
| --- | --- |
| src/lib | e50d37168bd8fbb710611a8742da9830c9d9ac1c |
| test/lib | a6828c1df5a2144d8c1668856d0d46cda7523e5e |
| src/emulator | 0793dca7152d47fc02b638f387c84b393056cb06 |
| test/emulator | d8e0027222b3b60cc3baf49bc83ee535923a913a |
| src/x86-debug | 5e2159bd6deaeefd0f4fca547d3eaa0f13a6e9ba |
| test/x86-debug | 00e9d5494e46854d2ab85550ff1e8d16f438efff |
| src/x86-ibmpc | ed0394e0fe730810a8e79e7cea9b9799dbd3e6c4 |
| test/x86-ibmpc | df890b719c80bff01c7424bf9b1f61cd674dcc86 |

Reproduce with `git rev-parse <revision>:<directory>`; manifests carry per-file
SHA-256. Git tree hashes are Git object identities, not SHA-256. External source
trees can be compared after import; no current NXVM equality is asserted here.

S3 coordinator review: inspected cba71230's actual three-document diff against
the owner request and S3 scope. Verified fixed commit/tree identities, manifest
counts, exact unchanged src/test/root infrastructure/EXEs, and the distinction
between S2 runtime qualification and unperformed user/downstream acceptance.
No source repair or buffer restructuring was smuggled into the audit. The
documentation governance gate passes. Delivery awaits owner review; S4 remains
last and is not automatically admitted by completion of this audit.

## S4 admitted lifetime audit: design before code

Owner now admits S4 and asks how to simplify cleanly. Baseline 3758a33e.
This is a source/test-contract audit, not a new runtime experiment or final
proof that five buffers are the mathematical minimum. Production/test delta
is +0/-0; implementation has not begun. The S1 suggestion to start with Session
was a candidate, not proof that its frame was only a redundant forwarding copy.

Frozen universe is the five frame payloads, plus native surface/private DIB
as exclusions. Each needs a lifetime/contract disposition before deletion:

| Buffer | Existing responsibility and deletion cost |
| --- | --- |
| Machine staging | Driver builds a candidate while prior publication remains intact. No-frame/error and text-change comparison use the old publication. Writing in place changes the driver contract or requires backup/preflight logic. |
| Machine published | Latest complete frame for independent readers under frame_lock, including paused reads. Deleting it removes the publication owner, not just a memcpy. |
| Session retained | One coherent sequence/route/content snapshot survives asynchronous Window/Console setup and rejected later copies. Deleting it requires changing retention or borrowing/pinning another owner's frame. |
| Window pending mailbox | Latest complete submission survives producer return while the Window worker is busy. A caller pointer cannot replace owned copied storage. |
| Window worker frame | Stable captured content while publisher may replace the mailbox; retained text metadata drives cursor/exposure behavior. Deleting it requires rendering while holding the mailbox lock or a new transfer/borrow contract; keeping only metadata does not eliminate the need for stable pixels during rendering. |

Concrete Session counterexample: session_frame_smoke.c consumes notice A but
captures latest graphics frame B, requests Window creation, and publishes B
only after its completion event. Its subsequent invalid-publication and
run-generation-mismatch assertions retain B. session.c:drive also runs on
component/broker/runtime/input events, not just new-frame events. A later
fresh fetch can return a different route or no frame; simply replacing the
retained member with a pointer or a new fetch is not behavior preservation.

Machine publish currently holds frame_lock during candidate construction;
the second slot is needed here for transactional preservation/comparison, not
a claim that readers concurrently inspect an unlocked slot. Base capture copies
while locked, then Window performs native surface work after unlock. Session
owns no native surface; UI synchronously copies into the selected leaf mailbox.
Native surface and private Compat DIB are different representations/owners and
are not counted as removable copies of the public frame.

Rejected shortcuts: allocating a same-sized temporary merely relocates the
buffer; swapping existing buffers can reduce copying but not their count;
publishing a borrowed pointer without a lifetime protocol races reuse;
pinning/reference counting or lock-spanning UI callbacks adds contracts rather
than deleting redundant logic. Do not implement them just to meet a count.

Recommendation at this design checkpoint: retain current runtime pending owner
discussion. No direct, behavior-preserving deletion has been established.
Removing Session would save 3,933,204 bytes (about 3.75 MiB), but requires an
explicitly approved different handoff/retention design, not a few-line cleanup.
This is not a claim that broader redesign cannot use fewer buffers. No code
diff estimate for an unselected redesign is presented as a committed plan.
Owner subsequently approves closing S4 with retention and explicitly keeps T96
open. This disposition does not authorize whole-T closure or another subtask.
Any future reduction must first establish its changed contract and tests.

## S5 owner-approved pointer circulation

Owner approves implementation after the S4 discussion: preserve existing layer
ownership, circulate pointers rather than whole-frame values, retain native
surface. Window has four roles (producing, latest pending, processing,
displayed); Console has three (producing, latest pending, processing).
Pending replacement returns the unconsumed buffer; completed Window display
retires the old displayed buffer; completed Console output returns its input.
Returned buffers remain reusable instead of being allocated/freed every frame.
Machine must not wait for consumer completion or cancel generation to acquire
storage. Brief pointer-exchange synchronization is distinct from such waiting.

Frozen convergence universe and required proof:

| Boundary | Required disposition/proof |
| --- | --- |
| Producer | Build only into exclusively owned storage; failed/no-frame construction preserves last valid delivery; no consumed dirty without storage. |
| Machine publication | Replace duplicate published payload with one delivery authority; public diagnostic readers and observer lifetimes explicitly accounted for. |
| Session/UI | Routing and run generation remain coherent without pinning extra history; asynchronous creation and simultaneous text consumers do not double-transfer a pointer. |
| Mailbox | Atomic replace/take, latest-wins, exact-once return; controls remain independent and no content interpretation in Base. |
| Window | Stable processing input, retirement after successful display transition, existing surface/damage and cursor behavior retained. |
| Console | Processing input retained until conversion/output returns; NOT_CURRENT and failed output cannot leak or prematurely reuse storage. |
| Lifecycle | Pause/final frame, reset/load generations, recreation, startup failure, close during publish and teardown release every allocation exactly once. |
| Dependency | Leaf formats remain leaf-owned; no Lib references to Machine implementation; no duplicate runtime transport or per-frame heap churn. |

Implementation is not yet claimed. Inventory existing callbacks and lifecycle
tests before choosing signatures. The previous copied-frame architecture rules
must be reconciled explicitly, not silently bypassed. No original source or
user assets are part of this migration. Full x86/x64 qualification and actual
executor/coordinator diff review remain required before a complete P delivery.

### S5 admission checks

Abstract exhaustive role exploration (not a production/threaded test) starts
with producer slot 0, all other slots free, and empty pending/processing/
displayed roles. Transitions are: successful producer publish exchanges with
pending or takes a free slot; consumer take moves pending to processing;
consumer finish returns old displayed and promotes processing for Window, or
returns processing directly for Console; failed/no-frame production leaves
roles unchanged. Every transition checks that each allocated slot has exactly
one role and that publish can acquire its next writable slot without waiting.
Canonical free-slot ordering yields 93 states/266 transitions for four-slot
Window and 21 states/60 transitions for three-slot Console, all satisfying
those invariants. This proves only the simplified role model, not mode changes,
native output, simultaneous consumers or real synchronization.

Concrete integration inventory confirms that Machine currently owns two frame
buffers, Session a retained frame, and UI independently publishes Console and
Window representations. The existing observer/copy-reader API also exposes the
last publication. Those readers and asynchronous creation must not silently
introduce another retained history into the bounded model.

Governance prerequisite: rules/ARCHITECTURE.md currently mandates copied frame
snapshots and copied values, while rules/EXECUTION.md allows rules edits only
under an admitted Td. Owner approval establishes the new technical direction,
and the owner subsequently approves M9 Td S21 to reconcile the rule text.
Production migration has not begun; the governance change is not runtime proof.

### S5 implemented ownership and verification ledger

Implementation baseline is 4fcaa573 (the admitted Td prerequisite). One public
KVM Base exchange owns three or four allocated slots and a short pointer lock.
It knows neither frame contents nor Emulator. A producer writes exclusively
outside the lock, validates before committing, and always receives reusable
storage. There is no per-frame allocation, reference counter or second queue.

Machine owns the four-slot exchange. Its aggregate begins with the Window leaf
frame (compile-time offset assertion); publication serial and Console maps are
opaque to Window. UI binds that same exchange, rather than copying its payload
into another mailbox. Window retains processing/displayed pointers until finish
or quiesced destruction. UI owns the three-slot Console exchange, converts text
directly into its writer and releases processing after logical Console output.
The existing RGB surface, renderer, cursor overlay and native output remain.
Lock order is Machine publication metadata -> exchange for observation/commit,
and leaf admission -> exchange for take/finish. Controls retain their independent
FIFO lock. There is no reverse exchange -> Machine/leaf acquisition. Construction,
validation and native rendering run outside those pointer-exchange locks; bounded
text conversion/diagnostic observation is serialized but does not wait for rendering.

Session no longer retains historical payload through asynchronous creation.
It observes the latest publication on each reconciliation, including completion
events. Run, route and serial come from one locked observation; invalid/no-frame
observations cannot publish uninitialized content. This is the explicitly
approved latest-wins retention change, not a claim of identical internal timing.
Diagnostic copied snapshots remain separate, bounded observers. Machine is
destroyed only after UI/Window; failed UI destruction retains their dependencies.

| Obligation | Implementation and focused proof |
| --- | --- |
| Bounded producer progress | frame_exchange unit: both slot counts, 10,000 overwritten publications with two reusable writer addresses; real second producer thread publishes 100,000 times while held processing/displayed data remain unchanged. |
| Allocation and retirement | Same unit injects every object/frame allocation failure, proves zero live allocations, unsuperseded failure retry, newer-pending preservation, detach/recreation and invalidation while held. |
| Admission and notification | Existing mailbox/control/input tests preserve STOP, terminal failure and independent control locks; leaf fixture helpers only prepare writer data before real typed commit/notification. |
| Rendering and invalid values | Window damage/copy tests preserve RGB padding, rejected writes without notification, text geometry, mode changes and dirty/cursor checks. Native capture contract still invokes production message/consumer code with host calls mocked. |
| Machine and Session | machine_wait proves no-frame/text suppression, serial wrap, immutable held old run, coherent Console conversion and invalid-publication preservation; session_frame proves delayed notices, route reconciliation and recreation. |
| UI and lifecycle | composition retains publication-cache failure, creation, binding, destruction-failure and callback lifetime coverage. Retirement, snapshot and real restart integrations remain required. |
| Dependencies | Public frame_exchange is explicitly admitted by Emulator's header gate; private worker/mailbox support remains forbidden with negative tests. No new outward edge, platform code or Types definition. |

Code accounting includes C/H, CMake and verification scripts, excluding README,
manifests, other documentation and EXEs. Relative to 4fcaa573: Lib production
17 files +263/-111 (net +152), Emulator production seven +122/-85 (net +37).
Tests: Lib 18 files +428/-210 (net +218), Emulator five +119/-57 (net +62),
App one +6/-3 (net +3). Total production +385/-196 (net +189); tests +553/-270
(net +283). No X86, original mirror, Compat, driver, INI or media changes.

Same-format large storage is four Machine aggregates rather than three Machine
and two Window payloads: 15,732,816 versus 19,663,964 bytes, saving 3,931,148
bytes before small Console slots and exchange bookkeeping. This is layout
accounting, not a throughput benchmark. The three large transport copies are
removed; text-to-Console format conversion and explicit diagnostic copies remain.

Evidence lives under build/t96/s5. Hidden owned jobs use completion markers and
retained stdout/stderr/XML; a five-minute no-progress interval triggers inspection,
not automatic termination. Executor owns cleanup. Initial compile failures were
unmigrated test interfaces; the first focused failure was a UI test union reused
across mode changes without reinitialization. Both were corrected without dropping
assertions. An intermediate full run also identified the stale public-header
allowlist and manifests while final changes were being reconciled. Those failed
attempts remain evidence, not qualification. CURRENT will identify the final
complete dual-width results and delivery; no user acceptance is inferred here.


Final qualification: x86 and x64 each pass 194/194 with no skips: 178 unit/gates,
eight background integrations, five desktop and three native audio integrations.
CURRENT identifies the exact final XML sets. The first final-x64 native run
failed three audio opens and two package cases while the user was disconnected;
all eight passed unchanged after reconnect (native-reconnected-x64 exit 0).
Failed attempts remain evidence. No audio workaround, assertion weakening or
skipped qualification was introduced. Both EXEs are rebuilt and deployed.
Owner acceptance remains pending.

Coordinator P2 reviewed executor commit ccecf953, not merely the proposed patch.
The committed Machine, Session/UI, exchange/mailbox and Window/Console paths
retain one pending authority and bounded storage; processing/displayed slots
cannot be selected as the writer. Native work stays outside exchange locks.
Destroy ordering keeps the owner alive through consumer retirement. The
diagnostic copy API and Console representation conversion remain explicit,
not alternative production transports. Committed accounting matches the ledger;
no mirror, Compat, App production, user configuration or media changes occur.
Final dual-width XML and deployed hashes were checked. No blocking finding
remains within this review; this is not a claim of exhaustive race freedom or
measured performance. Owner runtime acceptance is pending.


## T96 S6: independent audit corrections

Owner admitted the audit repair after S5 delivery. Keep the four/three-slot
ownership model, native surface and FIFO notification policy unchanged.

- Exercise concurrent producer and consumer turnover, failure retry, immutable
  held pointers and final latest delivery; retain stalled-consumer coverage.
- Session frame notifications carry only run generation. Sequence/graphics stay
  in the actual publication and in the Machine observer contract.
- Private Window rendering consumes validated immutable input; full validation
  stays at publication/copy boundaries, not repeated repaint or cursor ticks.
- Reconcile design and Console documentation with pointer ownership and actual
  unbound-output success semantics; do not invent NOT_CURRENT retention.

Scope: Lib/Emulator production, owner-local tests, manifests and documentation.
No App/Core/mirror/Compat, assets configuration or guest media edits. Estimates
and current execution status are in CURRENT. Full dual-width qualification is
required before executor delivery and coordinator committed-diff review.

### S6 finite convergence and similar-issue ledger

Frozen universe: the four S5 audit findings above, their direct callers and
owner-local tests. Completion requires all four dispositions below plus full
x86/x64 qualification and actual committed-diff review, not a general claim
of race freedom or benchmark improvement.

1. Exchange coverage: retain stalled-consumer tests and add concurrent turnover
   for both slot counts. Check held contents, monotonic observations (failed
   retries may repeat), final latest delivery and detach/reuse. Existing failure
   cases exercise supersession and invalidation; allocator accounting remains.
2. Notification ownership: search all src/test for enqueue_frame_completed,
   queue_push_frame_completed and value.frame. Remove the unused Session fields
   and arguments, update every direct caller/test. Keep Machine observer fields
   and actual frame sequence. FIFO growth/order and stale-run rejection remain.
3. Validation ownership: search all Window/Emulator frame_validate calls and
   private size/cursor/render callers. Machine publish and typed commit/copy
   remain complete validation boundaries. Window consumes immutable pinned
   frames under its existing public precondition. Native failure checks, target
   surface dimensions, null/valid checks and cursor visibility remain. Existing
   malformed-publication, damage, cursor and native-message tests cover these
   boundaries. No validation flag, cache or alternative renderer is introduced.
4. Documentation: search NOT_CURRENT and copied-frame claims in Lib and current
   design. Correct Console README/binding comments, broker comment, KVM README
   and architecture descriptions against current output code. No Console output
   behavior is changed. Historical delivery records remain historical.

The notification FIFO is deliberately retained: coalescing would change ordering
and introduce another policy outside this narrowly admitted repair. Exchange
production code, Machine observer ABI, App/Core and original mirror are unchanged.

### S6 changed-path accounting

Counted from d76355ed with git diff --numstat, C/H production and test paths
only (exclude Markdown, manifests and EXEs). Production +19/-17, net +2;
tests +88/-20, net +68. No mirror delta. Compared with the estimate, replacements
mostly remained one line, and explicit private-helper contract comments account
for the small positive production net; no new runtime state was added.

- Lib production +10/-7: kvm-window/{geometry.c,geometry.h,render.c,render.h,
  win32/component.c}; console/binding_interface.h and console-broker/console.c
  are comment-only corrections.
- Emulator production +9/-10: product/composition.c;
  session/{control.c,control.h,session.c,session_interface.h}.
- Lib tests +75/-0: kvm-base/frame_exchange_smoke.c.
- Emulator tests +13/-20: product/composition_smoke.c;
  session/{session_frame_smoke.c,session_monitor_smoke.c,
  session_queue_failure_smoke.c,sync_smoke.c}.

The only public call-signature change removes unused sequence/graphics arguments
from emulator_session_enqueue_frame_completed. All repository callers are
migrated atomically. Producers still validate every complete frame, and all
Window repaint/cursor operations use the same pinned immutable publication.

### S6 executor verification

Both architectures pass 194/194 distinct tests without skips (178 unit/gates,
8 background integration, 5 desktop, 3 native audio). Hidden job
build/t96/s6/full1 exits 0; focused sets pass 15/15 each. After the initial x64
build, the new test's consumed counter was separated from the short-circuit
success expression so the minimum-progress assertion counts every take. Final
x64 test was rebuilt and passed ten consecutive runs under recheck-x64 (exit 0);
x86 full qualification already used the final version. No production change
followed either artifact build. CURRENT records exact XML locations and hashes.

A preliminary documentation check rejected S6 admission until S5's delivered,
not-owner-accepted checkpoint was recorded in history. The unchanged gate now
passes. Existing mirror compiler warnings and x86 Ninja recovery remain; no new
changed-path compiler warning was observed. All native tests ran in the active
host session. These are automated test results, not owner guest acceptance or
measured speedup. No Computer Use or interactive timeout termination occurred.

### S6 coordinator actual-commit review

Reviewed executor 21b64548 after push: all production/test diff, current design
and contract descriptions, manifests and packaged EXE hashes. The immutable
publication precondition is already part of the Window public contract; removing
private rescans does not remove producer validation or native failure checks.
Session notification cleanup preserves the run filter, FIFO and Machine observer
signature. The new test advances both sides with test-only event checkpoints;
it supplements rather than replaces the producer-with-stalled-consumer proof.
No new runtime state, allocation route or ownership path was introduced.

Recomputed 253 manifest entries across changed source/test corpora; all match.
Compared final XML testcase names with both CTest enumerations: 194/194 each,
no skips/failures, plus the final x64 ten-repeat proof. Actual C/H accounting
matches the ledger. Original mirror and owner INI/media/checkpoint are untouched.
No blocking finding remains in this bounded review. Runtime owner acceptance
and T96 closure remain pending; no exhaustive race/performance claim is made.

## T96 S7 direct Console production

Owner accepts S6 and requests: "那你准入一个新的S修复吧。另外一个测试的问题也一起修复。同类问题审计扫描一起修复。然后给我提交推送等我验收。"

Baseline b25f9969. S7 corrects the S5 implementation's retained Console
transport copy; prior claims that this was necessary conversion are withdrawn.
T71 S4 introduced the mixed Machine envelope at 17c9da90; S5 changed its
Window layout for pointer consumption but left the Console extraction in place.

Frozen convergence universe:

1. Machine owns Window four-slot and Console three-slot exchanges. Its sole
   executor passes the actual writable leaf frames to the driver. Session/UI
   forward borrowed exchanges and metadata, never copy guest text.
2. Driver fills both consumers from one source sampling path. Preserve native
   graphics/text decoding, validation, no-frame and ERROR behavior. Maps belong
   only to Console; the owner correction below removes the Window envelope.
3. Existing UI status-page construction remains one pure implementation;
   Product composition injects it into the executor's Console production path.
   No extra exchange/output route. Immutable context lives until executor shutdown.
4. Prove native raw/cooked/raw retention by reading before any second write.
5. Scan every producer, Session/UI payload write, mailbox/leaf handoff, observer
   and diagnostic reader. Source decoding, native rendering and explicit
   diagnostic copies remain distinct from transport copies.
6. Build/deploy both EXEs, focused then complete unit/gate/integration tests;
   executor commit/push, coordinator actual diff review, owner acceptance.

No original mirror, Compat, guest configuration or media changes. No new slot
algorithm, rendering path, per-frame allocation or consumer-dependent wait.
Expected production +120..200/-130..220, test net +100..200. Final accounting
uses git diff --numstat from the baseline, excluding documents/manifests/EXEs.
Evidence is under build/t96/s7 with hidden owned jobs, five-minute no-progress
inspection rather than automatic termination. Earlier build failures remain
recorded: stale migrated test signatures and a missing UI mock symbol. Native
Window, audio and Console tests remain separate from background unit runs.

### S7 owner-approved schema correction

Owner: "好的，按照这个方案整改。" The approved feedback removes valid and
graphics booleans, uses exactly one non-null text/graphics pointer in the Window
frame, and moves sequence into that frame instead of a Machine envelope.
The producer writes bounded slot-owned backing storage, never guest memory or
a transient stack buffer. Selection does not allocate per frame. Console keeps
its independent text/maps representation and three slots. Machine owns both
exchanges; Driver fills them only, Session/UI forward references as before.
No-frame readiness must remain an explicit producer outcome, not an invalid
published frame. Diagnostic copies must copy payload and rebind destination
pointers; transport remains pointer-only. Tests cover both-null/both-present
rejection, mode changes, destination ownership, held-slot immutability and
sequence preservation. Expected additional C/H migration: about 30 files,
+250..400/-300..500; exact accounting follows final verification.

The pre-correction full1 run had native Console retention failures in both
widths and an x64 compact-package interruption. Its failure evidence is retained and is not qualification of this
revised ABI. Final verification must rebuild and exercise the revised sources.

The first added retention assertion was placed in the extent stress loop that
intentionally shrinks the native viewport. It read 30 columns after restoring
an 80-column raw buffer (saved=80x30, actual=30x30). This does not qualify full
content retention under host resizing. The admitted no-rewrite regression is
now placed in the existing raw/cooked/raw preservation loop before its next
write, checking all saved visible cells and geometry, not merely a marker.
The separate deliberate-resize loop retains its original frame-repair proof.
The host-resize retention finding is separately recorded, not declared fixed.

### S7 changed-path accounting and executor audit

Counting C/H only with git diff --numstat b25f9969 -- src test (documentation,
manifests and EXEs excluded): production +237/-298, net -61; tests +749/-671,
net +78. The 13 production paths comprise App Machine driver +51/-36,
Emulator seven paths +102/-109 (including deletion of the old frame header),
and Lib Window five paths +84/-53. Test paths: App seven +170/-139, Emulator
seven +240/-213, Lib eight +334/-317, X86 Debug one +5/-2.
Most test churn repairs direct field access and fixture ownership for the new
pointer ABI, not additional test routes. Combined C/H net is +17.

The frozen production sweep uses rg for the exact retired frame type and
emulator_ui_frame_target, plus kvm_window_frame_copy/lib_memory_copy/memcpy
through Machine, Session, UI and both KVM leaves. Retired types/target have no
callers. The similarly named Machine exchange accessor and notification sink
are retained, distinct APIs, not the deleted schema.

| Member | Disposition / proof |
| --- | --- |
| App Machine producer | Samples source once and directly fills both text destinations; only Console receives code-point maps. VGA mode/attribute matrix verifies identical common cells and no-frame behavior. Graphics RGB decoding stays single-path. |
| Machine publisher | Owns both exchanges; validates before commit, assigns sequence once, preserves no-frame/error/reset semantics. machine_wait proves producer/taken Console pointer identity, held-slot immutability, map-only changes, invalid-map rejection and status production. |
| Session/UI | Metadata and exchange references only. Removed writable UI frame target and Console copy/commit. Session/UI tests verify dispatch; no guest payload assignment remains. |
| Product composition | Binds the existing pure UI status builder before running. Existing shutdown precedes UI destruction, preserving callback context lifetime; composition mock verifies binding. |
| Window schema/consumer | Exactly one payload pointer, slot-owned union backing, existing four-slot algorithm. Copy test proves pointer rebinding and inactive-tail preservation; damage/cursor tests retain rendering behavior. |
| Console consumer | Same three-slot ownership. Remaining palette copy belongs to conversion into native Console output, not transport of a second KVM frame. |
| Diagnostics | Machine copy_published_frame is the only production caller of the explicit Window deep-copy helper; it holds the publication/exchange locks. App observers are synchronous and do not retain the borrowed pointer. |
| Other copies | Machine media/debug requests and Session command text are copied controls, deliberately outside frame ownership transfer. |
| Native retention test | Normal raw/cooked/raw verifies saved content before rewriting. Deliberate viewport narrowing has a separately recorded finding; no claim that this S fixes that unrelated geometry path. |

No Base exchange algorithm, mirror, Compat, INI, media or snapshot-format
change. No new allocation per frame, producer wait on rendering, historical
payload copy, or driver-to-presenter bypass. Public ABI consumers must rebuild
together; this is not a compatible header-only update.

The final1 run passes both 178-test unit/gate suites, all native tests and
x64 background integration. Its x86 runtime_restart_boot_smoke fails the first
prompt_roundtrip while still graphics (sequence 36). Source review finds no
retained shallow frame copy or changed keyboard sequence in this migration;
the fixture uses a fixed 15-second startup observation, which does not prove
Program Manager readiness. This remains a hypothesis, not the cause.
Unchanged independent repetitions and a complete x86 integration rerun are
recorded separately under recheck; final1's exit 1 is preserved. A bounded
follow-up investigation is in TODO, not hidden by changing assertions.

Follow-up: the first three unchanged independent x86 repetitions pass, but the
complete background integration rerun fails again (graphics sequence 62).
PROCESSORS=8 already reserves all eight CTest slots for this test; therefore
concurrent CTest CPU load is not established as the cause. Added test-only
failure-stage reporting and a PPM dump of the already copied graphics frame
to the test-owned working directory. No input, timeout, success condition,
production code or artifact changed. The instrumented x86 probe run passes
8/8 background integrations; absence of failure in that run produces no image.
This is improved observability, not a claimed repair of the intermittent case.

### S7 final verification

Both architectures have final passing evidence for all 194 registered names
without skips: final1 unit-*.xml (178 each), final-check integration-*.xml
(8 each), final1 native-*-0..7.xml (5 desktop and 3 audio each).
Compared those XML names to CTest's complete JSON enumeration: no missing or
duplicate qualification member. Focused sets are 13/13; final-check exit is 0.
The later test-only diagnostics are compiled for both architectures and exercised
by final-check. Production EXE hashes are in CURRENT and unchanged after them.

All 292 entries across the eight shared manifests match. Documentation governance
and git diff --check pass. Existing mirror macro warnings, VGA fixture pointer
casts and x86 Ninja recovery are retained; changed shared code builds under its
strict warnings. No Computer Use, timed termination, automatic retry in product/
tests, user INI/media mutation or snapshot-format modification occurred.
The intermittent test and deliberate viewport findings above remain disclosed;
final passing coverage is not a claim that either root cause has been repaired.

Executor reviewed the actual changes and all public frame consumers. Source
sampling/decoding and native rendering remain necessary transformations; only
explicit diagnostic reads retain deep copying. Tests that formerly asserted
Machine-envelope copying now assert Lib copy ownership and Machine Console
pointer identity at their proper owners. S7 is ready for delivery and actual
coordinator commit review; no S/T closure is claimed.

### S7 coordinator actual-commit review

Reviewed pushed executor 8841e5fd07ad9b212f97f4f577554811aedd7396 against
the owner-approved schema and original S7 requests. Inspected committed
Machine/Driver production, Session/UI/Product wiring, Lib consumer/schema and
test changes, not only the executor's report. Confirmed both exchanges remain
owned by Machine, payload pointers target reusable owned storage, and the pure
status callback is installed only after UI binding and outlived by shutdown.
No live guest pointer, per-frame allocation, second renderer or transport copy
is introduced. The removed Machine wrapper has no source/test consumer left.

Recomputed C/H accounting and checked all eight manifests (292 entries),
both EXE hashes, full registered-name coverage (194 each), final-check exit 0
and retained failing runs. All task changes are in P1; the only unstaged path
is the owner's excluded snapshot.debug. Diagnostics add observability only;
they do not establish the cause of the intermittent x86 integration failure.
The viewport-narrowing finding is likewise not presented as repaired.
No blocking schema/transport finding in this bounded review; S7 remains
awaiting owner acceptance and T96 stays open.

### S7 native narrow-Console follow-up admission

Owner admits investigation and repair within still-open S7, baseline aa8f75c3.
The old retained-extent log proves clipping, not the first failing operation:
its temporary assertion source was replaced. Earlier attribution to deliberate
viewport resizing is unproven. A separate hidden probe preserved the wider
buffer, but is not the original integration test.

Scope: existing Win32 Console broker and its native display integration test.
Trace native operations in that test's real path, including handles, saved and
actual geometry/cursor and column-79 content, before choosing production or
test correction. Preserve normal no-rewrite retention; deliberate resize/frame
repair is a separate requirement. No extra renderer, backup, replay or retry.
Temporary tracing lives locally in the test and evidence under
build/t96/s7/console-narrow; finite six-case/three-round runs, no unbounded loop.
Remove instrumentation after diagnosis. Expected final production 0..20 added/
0..20 removed; tests 30..80 added/0..20 removed, provisional until evidence.
Run affected dual-width unit/native tests; production changes additionally
require dual EXE publication and prescribed qualification. Hidden owned jobs
do not share active build trees; inspect five minutes without progress, never
kill by interaction timeout. No Computer Use or owner INI/media/snapshot edits.
Executor P3 and actual-commit coordinator P4 precede owner acceptance.

### S7 narrow-Console diagnosis and minimal correction

The original extent loop, instrumenting its actual native API wrappers, reproduces
80x30 -> 30x30. In round 1 the first change is SetConsoleActiveScreenBuffer(raw):
immediately before it the saved raw handle is 80x30 with column 79 readable;
immediately after it is 30x30. Metadata restores 80 columns (temporarily 29 rows),
the existing buffer restore returns 80x30, then the final SetConsoleWindowInfo
again reports 30x30 despite returning success. This precedes the test viewport
setter. Palette application uses the same path; frame output subsequently
establishes its required extent before writing cells.

The default host is PseudoConsoleWindow. The same test chain inside an owned
classic ConsoleWindowClass keeps the wide buffer. Microsoft host source has an
explicit backing resize in SetConsoleWindowInfoImpl's VtIo branch:
https://github.com/microsoft/terminal/blob/main/src/host/getset.cpp
This explains a platform branch, not identity with the installed host binary.

After the final viewport call, handoff restoration now queries capacity and
returns IO_ERROR if it differs from the saved buffer. No additional resize,
backup, replay or renderer is added. Palette-only output is distinct: its
existing ensure-text-surface precedes the complete cell write. Unsupported
distinct geometry now fails explicitly instead of reporting clipped success.

Tests retain normal no-rewrite comparison and extend the extent loop with
patterned cells, attributes and a visible cursor. Successful restoration is
read before ensure/viewport/write. Limited native handoff must return IO_ERROR,
retain cooked binding, expose a capacity mismatch and restore cooked display;
it is not counted as successful wide-buffer preservation. Deliberate resize
and complete-frame repair remain distinct. The mock injects successful viewport
mutation of capacity and proves failure without another resize. The hide helper
now precedes API substitution; previously main's hide call received null.

Evidence under build/t96/s7/console-narrow: baseline-native.log retains wrapper
traces; native.log records the corrected default host; classic.log records the
original chain in ConsoleWindowClass; final-classic.log records final assertions
(Classic complete, no limited-host branch). Scratch classic-test.c changes only
startup to retain the explicitly owned classic host instead of allocating the
default host. It does not replace broker logic. Tracked tracing is removed.

History: final viewport operation predates T96 (1f7383f4c); pre-viewport capacity
check is from eaf05bff2. S7 previously changed no broker production. This is an
existing false-success gap, not the frame-pointer migration. These commits prove
code lineage, not the first historical user-visible occurrence. The old TODO's
unproven attribution to deliberate viewport resizing is superseded here.

The clipping log did not prove content loss. Column 79 became readable again
during intermediate expansion; one cell does not prove complete preservation.
No all-terminal-version or ConPTY wide-buffer retention guarantee is claimed.

Similar-issue sweep: rg for set_console_window_info,
set_console_screen_buffer_size and apply_display across src/lib finds one
viewport setter, in apply_display; buffer setters are its saved-state restore
and ensure_text_surface. The latter already queries and checks the required
extent after its resize. apply_display has only two callers: select_output
requires saved display restoration, while palette output subsequently ensures
capacity and writes the complete cell rectangle. Types occurrences are SDK
aliases, not a second implementation. No Machine/Session redraw is added.

Follow-up accounting (C/H only against aa8f75c3): production +7/-0, net +7 in
one Lib broker file; tests +60/-6, net +54 in native display and mocked Win32
Console tests. Manifests, documentation and EXEs are excluded from C/H counts.

### S7 candidate qualification blocker

The seven-line postcheck above is a candidate, NOT an accepted repair. Full
qualification under console-narrow/final exits 1: both 178-test unit/gate suites
and both focused 10-test sets pass; native compact-package fails stage 12 in
both widths (pause/stop/start remains in the cooked screen instead of completing
the expected raw handoff). The old product acceptance therefore cannot be
declared preserved merely because the limited native test now fails truthfully.
x64 background integrations pass 8/8; x86 command_provider additionally fails
its expected-IP assertion (0xc0000409). That separate cause is not established.
Other native cases pass. No acceptance assertion or test deadline was weakened.

The bounded reexpand-test scratch keeps the original broker path and writes a
patterned 80x30 frame. After the rejected narrow handoff it only restores raw
capacity and reads, without redrawing: reexpand.log records 2310 mismatched
character/attribute cells out of 2400. This proves expansion alone does not
restore the original layout; it does not establish whether bytes were deleted
or reflowed elsewhere internally. No such expansion is added to production.
Classic final assertions pass both widths (classic.exit and classic-x86.exit 0;
final-classic-x64.log and final-classic-x86.log contain Classic complete).

Decision boundary: accepting explicit unsupported narrow ConPTY handoff changes
the existing compact product contract. Otherwise retaining that contract needs
a different approved host strategy; no backup/replay/extra Machine publication
is authorized. Do not silently bless compact-package failure or claim this
candidate meets the requested complete repair. Candidate remains uncommitted;
published EXEs are restored to the pre-follow-up S7 commit. No P3/P4 yet.

### S7 continued activation isolation

Owner requests continued investigation, not acceptance of the unsupported-host
candidate. All additional probes are hidden, test-owned runs under the same
console-narrow evidence directory; no published EXE or production source is
changed by these experiments. The actual native test/broker chain is retained.
Experimental ordering edits are scratch copies, not an alternative renderer.

activation-cells.log isolates SetConsoleActiveScreenBuffer before the broker's
metadata, cursor and viewport restoration. Raw 80x30 becomes 30x30. Before any
re-expansion, all 750 surviving sampled coordinates still match their prior
characters/attributes. Immediately expanding to the saved size, without any
write or metadata operation, leaves 1940/2000 sampled coordinates mismatched.
Thus activation clips the addressable extent; expanding it cannot be treated
as preserving the old layout. This does not identify hidden host storage or
prove every inaccessible character was deleted. Height-only cases 80x25 to
80x12/13 similarly retain surviving coordinates but fail full-frame recovery.
activation.exit and activation-cells.exit are 0 (diagnostic completion, not
product qualification).

Bounded ordering experiments:
- preactivate: setting the inactive target's extended geometry before activation
  does not prevent 80x30 becoming 30x30; the same 1940/2000 mismatch remains.
- active-enlarge: enlarging the outgoing active screen first prevents that
  activation shrink, but the subsequent saved viewport restoration still fails;
  rollback then changes geometry again. It also touches cooked geometry, so this
  is not adopted as a repair. Diagnostic assertion exit is 3.
- rectangle: compensate extended-metadata rectangle endpoints and avoid a
  redundant final viewport setter in a scratch copy. A later no-rewrite cell
  assertion fails even though the capacity postcheck passes (exit 3). Geometry
  equality alone is not a sufficient restoration-success oracle.

The remaining design boundary is native content retention across distinct
buffer/viewport extents in the tested ConPTY host, not frame transport or a
missing full-frame publication. No claim that all possible native strategies
are exhausted is made. The seven-line candidate is still unqualified, and no
test is weakened to admit it. Classic-host results remain separately scoped.

### S7 owner-approved retained Console recovery

Owner approves: 行了你赶紧干活 确保符合我的要求. Console keeps its third input
through idle time; taking a newer frame retires the old input atomically. Window
still retains old/new separately with four slots. Activation requeues held input
through the existing pending role, preferring newer pending work; no additional
queue, frame, producer publication, backup or rendering path is introduced.
Generation invalidation suppresses stale redraw. Output never holds exchange
locks. Cooked remains native-owned; raw metadata selection is followed by normal
frame output establishing capacity and writing contents/cursor.

Bounded verification: exchange ownership under stalled/concurrent production,
coalesced and in-flight redraw, newer-frame priority, invalidation, activation
without another publication, STOP/failure and destruction. Native proof separates
ordinary no-rewrite retention from narrow raw reconstruction using KVM-held input,
and compares cooked contents/geometry/cursor. Required qualification remains
dual-width focused/full unit/gate/integration including native cases, artifacts,
actual diff review, commit/push then owner acceptance. Earlier unsuccessful
candidate logs remain historical, not proof of this revised implementation.

### S7 retained recovery implementation and executor review

The frozen follow-up universe is the seven changed production C/H paths,
four test C files, their test linkage, manifests and affected documentation.
Every production member remains on the existing ownership/output path:

- Exchange: three-slot take retires the prior current input; finish retains
  the replacement even if output was unbound or failed. Four-slot failure still
  preserves the previously displayed input. Redraw puts processing/current
  into the existing pending role only if no newer input is pending and the
  publication generation is valid. No additional state or allocation exists.
- Mailbox: frame lock protects exchange binding and terminal admission, then
  takes the existing exchange lock. Producer releases that lock before notify;
  no reverse lock ordering or rendering under either lock is introduced.
- Console activation: requeue then wake the same worker. STOP priority and the
  existing notification/output failure path remain unchanged.
- Native broker: cooked restoration verifies capacity after the final viewport
  operation. Raw restores metadata, then ordinary frame output ensures capacity
  after palette/geometry changes and writes complete cells and cursor. This is
  reconstruction from the held input, not a claim of native content preservation.

The original failure's first observed operation was native raw activation,
before the test's own resize; final viewport restoration can clip again. Raw
native storage therefore cannot be the sole retained content owner in that
host. The owner-approved current Console slot supplies the missing lifetime.
No new backing screen snapshot, renderer, retry loop or Machine publication is
added. This supersedes the unqualified capacity-only candidate above.

Proof dispositions:
- Exchange tests prove held storage is not writable, repeated/in-flight redraw
  coalesces, newer pending input wins and invalidation suppresses stale output;
  existing stalled/concurrent producer and lifetime stress remain passing.
- Console unit exercises activation without publication, superseding output,
  failed output and STOP. Broker mock proves a successful native viewport call
  that silently changes cooked capacity returns IO_ERROR, not false success.
- Native integration publishes one patterned 80x25 KVM frame into a three-slot
  exchange, then switches with a 30x30 cooked surface three times. It checks
  all cells/attributes and cursor after worker output and cooked saved state
  after return. No new commit/producer notification occurs after the first.
  Ordinary no-rewrite retention is still checked before test resizing; deliberate
  viewport changes followed by ordinary output remain a separate case.
- Similar-issue rg sweep covers frame_exchange_redraw/finish/detach/invalidate,
  mailboxes_redraw_frame, set_console_window_info, set_console_screen_buffer_size
  and apply_display in Lib/Emulator. The only new production redraw caller is
  Console activation. Window retains its four-slot path; Machine's existing
  invalidations suppress stale generations. Broker's only geometry paths are
  saved metadata restoration and existing ensure_text_surface; no new outer
  dependency or secondary renderer was found in this bounded sweep.

Qualification evidence: build/t96/s7/console-retained. Both x64/x86 builds and
25 focused tests pass. Each architecture has 178/178 unit/gates, 8/8 background
integrations and passing evidence for five desktop plus three native audio tests.
x86 all pass first run. x64 compact package first fails stage=1/error=6 before
the tested handoff, then unchanged independent package-recheck passes. Its first
failure and qualification.exit=1 are retained; package-recheck.exit=0 does not establish
a repair of the existing startup TODO. No skips, shortened deadlines, relaxed
assertions, Computer Use or owner manual reproduction were used. No universal
ConPTY version guarantee or new real-guest manual acceptance is claimed.

Accounting against aa8f75c3 (git diff --numstat, C/H only):
- Production: broker +12/-3; exchange implementation +15/-2 and interface
  +10/-5; mailbox implementation +8/-0 and interface +1/-0; Console event
  +1/-0 and interface +2/-0. Total +49/-10, net +39.
- Tests: broker mock +16/-2; native display +117/-4; exchange +17/-0;
  Console retirement +4/-4. Total +154/-10, net +144. Test linkage +1/-0
  separately. Documentation, manifests and EXEs are excluded from code counts.

Dual EXEs are rebuilt and deployed; their hashes are recorded in CURRENT.
Existing original-mirror warnings and Ninja metadata recovery remain disclosed.
User configuration/media and untracked snapshot.debug are untouched. The narrow
restoration debt is removed from TODO because its admitted repair now has scoped
proof; unrelated startup/prompt-roundtrip debts remain. Executor actual-diff
review finds no extra slot, transport copy, lock-held output or second path.
Commit/push and coordinator actual-commit review precede owner acceptance; S7
and T96 are not closed by these test results.

### S7 retained recovery coordinator review

Executor commit 881a3df90639bd534c238424d6276f9704e0f4e5 is pushed to main.
Coordinator switched roles and read its actual production/test/documentation
diff, rather than accepting only the executor summary. The review maps the
owner's retained-three-slot request to take/finish ownership, activation's sole
requeue caller, native normal-output reconstruction and the one-publication
integration test. Producer storage remains distinct from held/processing input;
newer pending frames and invalidated generations cannot be replaced by redraw.
No native surface, extra frame allocation, Machine/Session path or product
configuration was added. Cooked uses its existing native screen and strict
restoration; raw restoration is explicitly reconstruction, not false native
retention. Test-owned readback occurs after worker output, not a producer rewrite.

Reviewed XML gives passing evidence for all 194 tests per architecture, with
the preserved x64 package startup failure followed by its unchanged independent
pass. This is not an all-green first aggregate run or repair of startup debt.
Both artifact hashes match CURRENT. Manifests, documentation gate and whitespace
check pass. Original warnings remain disclosed. Only the owner-owned untracked
snapshot.debug remains outside Git; it was neither staged nor changed.
No blocking issue found in this bounded actual-commit review. S7 awaits owner
acceptance and T96 remains open; this governance P does not close either.
