# Win3.1 File Manager VGA width-transition repair

## Observed contract failure

With Window presentation, starting Windows 3.1 File Manager can make the
host KVM Window briefly become roughly twice its normal width and then shrink
again.  This is visibly a presentation-size transition, not an accepted
application resize policy.  Similar historical transitions around Win3.1 DOS
Prompt make it especially important not to solve this by identifying a guest
program or applying a fixed host width.

## Investigation first

1. Reproduce with the documented Win3.1/File Manager path on both x86 and
   x64, recording each published graphics/text frame's dimensions, stride,
   sequence, and display-enable transition without changing guest timing.
2. Trace the producing VGA state through the selected original renderer and
   Compat frame conversion.  Compare the relevant state transition to the
   read-only OpenNT and NTVDM64 mirrors only as source evidence.
3. Establish whether the fault is an original-host renderer defect, a current
   Compat conversion mismatch, or an App/VM frame publication of a transient
   mode.  The answer determines the sole repair owner; no consumer-side
   size-filter, debounce, delay, or File Manager/Win3.1 special case is
   permitted.

## T88 S1 observation ledger

The bounded, no-desktop probe uses the ordinary machine input queue to boot
the installed Win3.1 fixture, start `WIN`, open Program Manager's Run command,
and enter `WINFILE`.  It installs a Machine frame-sink before the Session/UI
boundary, so every graphics frame that can reach a KVM presenter is recorded.
It does not create a native Window or intentionally change guest timing;
diagnostic logging can nevertheless perturb host scheduling.

Both architectures received the input sequence intended to open File Manager;
input delivery alone does not prove that the intended guest application opened.
The renderer did expose
short-lived DIB allocation transitions during that route: x86 recorded
`1280x480` followed by `640x480`; x64 recorded `640x350` followed by
`640x480`.  In each observation, however, every completed graphics frame
published by Machine was `640x480`.  The transient binds carried no dirty
publication and therefore did not become a frame.

Static tracing gives the same boundary: `kvm_window` calls its client-resize
routine only after it captures a published mailbox frame and obtains that
frame's dimensions.  Session copies the published frame without changing its
dimensions.  A DIB bind alone therefore cannot resize the host KVM Window.

This rules out a consumer-side geometry filter and does not yet identify a
sole producer for the reported native Window flash.  The next permitted
observation, if required, is a short reserved-desktop run of the actual
Window presentation with the same geometry ledger.  No production repair is
admitted from the current evidence.

The task-owned native-window harness was also built to put the real KVM Window
worker on a private Win32 desktop.  This runner is denied `CreateDesktop`.
Its hidden-window fallback is usable, but booting the full File Manager route
exceeds the execution service's bounded foreground slot; keeping a detached
visible Window on the owner's desktop would violate the task's no-interference
rule.  It therefore cannot substitute a user-desktop observation here.  This
is an environment limit, not a product result.

Graphics publication reads DIB geometry and then consumes dirty state on the
one executor. Inspection of the intervening calls finds no guest execution,
message dispatch or renderer reentry; separate calls alone are not evidence of
a race. The later native observations identify an already-bound wide surface,
not a surface changing during that copy operation.

Read-only mirror comparison does not supply a transplant: OpenNT retains the
same `nt_graphics_tick` mode-settle sequence, while NTVDM64 replaces the host
presentation route with its own copied-DIB/text-video bridge.  Its additional
post-update publication is specific to that bridge, so importing it would
create a second presentation path rather than explain a SoftPC-published frame.

## Owner-confirmed native observation

The owner reproduced the visible jump with the diagnostic package. This
supersedes the earlier bounded non-reproduction: two intervals contain
completed `1280x480` graphics frames, not merely temporary DIB allocations.
Machine published two such frames per interval; UI forwarded all four, and
Window consumed all four before receiving the returning `640x480` frames.
The publication intervals were approximately 140 and 453 milliseconds.

The finite boundary ledger now disposes of DIB as the first observed
double-width surface, Machine as its unchanged publisher, UI as its unchanged
forwarder, and Window as its unchanged geometry consumer. Lib/Emulator did not
invent the doubled width. These observations do not yet prove which VGA
register transition produced the temporary DIB or which painter made it
publishable.

All four double-width frames carried damage no larger than the V7 hardware
pointer's 32-by-32 extent. Source inspection identifies a specific candidate:
the original V7 controller may invoke the Compat pointer compositor outside
an ordinary display update; Compat makes that standalone damage a completed
transaction, including when a DIB bind has just cleared and replaced the
surface. OpenNT's original host exposes dummy pointer callbacks instead, so
its behavior does not prove this added compositor path is correct.

The follow-up diagnostic records DIB generation, binding, dirty completion
and consumption, plus the pointer callback's mode-change/display-disable
flags. The follow-up correlation confirms two distinct publication paths:
one wide generation is committed by a pointer paint while mode change is
pending and display is enabled; another is committed by a full-screen paint,
with mode change no longer pending at the following pointer paint. A pointer
gate alone therefore cannot account for every wide published frame. A
task-owned no-UI snapshot attempt did not progress past loading and was stopped
as inconclusive; the actual Product diagnostic package did load and exit
successfully. No production behavior repair is admitted or retained in S1.

## Calculation and publication qualification

The investigation treats these as two independent obligations:

1. Explain the computed width from the raw CRTC display range, graphics shift
   mode, addressing/sequential-chain state and selected host painter. A large
   allocation is not proof of a valid visible mode.
2. Determine whether that generation is ready to publish at actual dirty
   consumption. A correct instantaneous width calculation is not proof that
   the surface is a completed display update.

Source inspection shows that `vga_gc_mode()` halves the stored horizontal
display count for 256-colour shift mode. `recalc_screen_params()` derives
`bytes_per_line` from that count and addressing state; it is not the physical
VRAM row stride, which is separately represented by `offset_per_line`.
The selected packed V7 painter emits one pixel per source byte, and the
standalone host uses `bytes_per_line` for its output width. Ordinary painters
use the separate character-width/colour calculation. These are different
contracts, not interchangeable width or stride fields.

The original host postpones mode selection at its graphics tick, but CRTC
start-address wrap paths can also call `choose_vga_display_mode()` directly.
The V7 compatibility-control write changes sequential-chain state without
calling screen-parameter recalculation or flagging mode change in that case.
These are source-confirmed paths to examine, not yet proven causes of the
observed resize and not permission to change device semantics.

The next observation records raw and derived state at recalculation, selected
host geometry, DIB bind, completed update and dirty consumption. The last two
points explicitly record mode-change/display-disable state and generation.
This distinguishes changing CRTC range, changing interpretation, stale derived
state, and premature publication without a width filter or timing workaround.
The owner permits at most three further manual localization runs; none is
claimed as performed by this diagnostic preparation.

## Raw-state reproduction disposition

The owner's completed combined-probe run consumes one of the three further
localization attempts. It records thirteen 1280-wide host-size calculations
and three published 1280-by-480 frames. Temporary source instrumentation has
been reverted; the package x64 EXE remains diagnostic, not a repaired release.

Across the captured doubling, horizontal display count stays 160, the stored
character count stays 80, graphics shift register stays 0x40, sequential-chain
control stays 0x6c, CRTC offset stays 0x80, and height stays 480. Sequencer
memory mode alone changes from 0x0e to 0x06: CPU chain4 becomes false. Host
painter selection then leaves the packed V7 painter, whose width is 640 source
bytes, and selects the doubled-pixel planar painter. Its width formula is
80 * 8 * 2 = 1280. Restoring CPU chain4 selects the packed painter and 640 again.
Lib and Emulator forward these dimensions unchanged.

The original Video Seven V7 VGA Technical Reference Manual (June 1988),
printed page 3-26, explicitly assigns SR4 chain4 to CPU memory accesses and
states that it does not affect CRTC accesses. The extended CPU/CRT addressing
description on printed pages 3-117--3-118 also separates the two domains.
[Original manual scan](https://dosdays.co.uk/media/video_seven/v7_vga_techref.pdf).
Thus the observed CPU-addressing toggle is not a legitimate doubling of the
visible display. This is a wrong display-path interpretation, not arithmetic
overflow, a changed horizontal display register, or a KVM resize error.

Publication is a separate confirmed gap: all three wide frames are consumed
from the DIB while mode-change-required is true and display-disabled is false.
A newly bound generation may acquire completed damage from the standalone
hardware-pointer compositor before a complete display has been reconstructed.
The first pointer paint in one generation occurred after the mode flag had
been cleared, so testing that flag alone is not a sufficient completeness
contract. Earlier evidence also includes a complete wide paint; suppressing
pointer-only damage would not correct the erroneous scanout interpretation.

The missing V7 compatibility-control recalculation and CRTC start-address
direct selection remain source-review concerns, not causes established by
this capture: compatibility control and the captured start address are
unchanged. They must not be patched merely because they were candidates.

## Separately admitted repair brief

Correct CPU-addressing versus scanout interpretation at the existing original
video/host boundary. Audit painter selection together with interleaved backing
memory, display row stride, start address, wrap and dirty-line indexing. Simply
removing one chain4 test or clamping width would leave these related contracts
unproved. Preserve actual CPU read/write chain4 behavior and legitimate CRT
mode changes; do not identify Win3.1 or File Manager.

Then establish generation readiness in the existing Compat output transaction:
a cleared replacement surface plus pointer damage is not a complete new
display. Reuse the existing update/generation lifecycle rather than creating
a second presenter, consumer filter or timing delay. Exact implementation and
mirror diff are to be scoped in the repair S after this diagnosis is reviewed.

Regression proof must hold scanout registers constant while toggling CPU
chain4, validate pixels and row layout as well as geometry, preserve legitimate
mode transitions, and cover pointer damage around surface replacement.
Dual-width tests must recheck Win3.1 DOS Prompt roundtrips and snapshot resume.
No Lib/Emulator production change is justified by the current evidence. S1
does not claim a production repair or repaired-binary acceptance.

The existing App VGA-frame test verifies stable V7 BIOS-mode geometry and
separately verifies the current chain4-dependent stride formula with sequential
chain4 disabled. Neither exercise holds the captured packed scanout state
constant while switching CPU chain4 through its real port receiver. The
follow-up must add that missing sequence rather than weaken existing assertions
or treat a stable-mode test as proof of mode-transition correctness.

The packed painter advances source rows by `get_offset_per_line()`, while
`vga_graph_update()` uses that same value to map dirty groups to rows. The
captured toggle changes this derived stride from 1024 to 256 even though
visible bytes per line remain 640. Therefore painter selection and display
stride must be corrected coherently: selecting the packed painter alone would
read overlapping rows and misinterpret dirty positions. This is why the
repair is not just deletion of the Host chain4 condition. Standard planar
formats retain their own plane-relative contracts; the regression must prove
them rather than apply the packed factor to every VGA mode.

## Owner-directed separate repair steps

The owner requests two separate repair subtasks: original-mirror scanout
interpretation first, Compat generation readiness second. These are separate
deliveries and acceptance boundaries, not permission to combine production
changes into diagnostic S1. The active packet remains the status authority.

### S2: Original-mirror scanout interpretation

Retain CPU chain4 read/write and bank-selection semantics. At the existing
display boundary, use a consistent scanout-layout interpretation for painter
selection, source row units and hardware-pointer coordinate conversion.
CPU chain4 alone must not change those display decisions. Sequential-chain4
alone is not a universal hardware scanout selector either. The repair retains
the original model's existing V7 layout convention, already used by
`recalc_screen_params()`, and must prove it against supported CRTC address
configurations; retain distinct standard VGA and planar rendering contracts.
Do not introduce a saved last-good mode, guest-program check or width clamp.

Primary reviewed sites are `host/src/nt_graph.c`, `base/video/vga_mode.c`,
and `base/video/v7_ports.c` under the preserved mirror. Do not add a helper,
interface, descriptor object, new lifecycle or second renderer. For the
captured byte-scanout mode,
CRTC Offset 0x80 advances 256 plane addresses per row, corresponding to 1024
bytes in the four-way interleaved backing store. Packed rendering and dirty
row indexing must retain that byte stride when CPU chain4 is disabled.

The pointer CPU-dependence is a source-confirmed similar-issue hit, not a
separately reproduced cause of window resizing. CPU bank-selection uses of
chain4 remain necessary and must be explicitly retained. Start address,
split and wrap handling are review/test members; they are not automatically
authorized for speculative changes because they were unchanged in the
captured failure.

The fixed minimal candidate replaces three conditions only:

1. Inside `nt_graph.c`'s existing 256-colour branch, replace
   `get_seq_chain4_mode() && get_chain4_mode()` with `get_seq_chain4_mode()`.
2. For the multiplied row-stride branch in `vga_mode.c`, retain
   `get_chain4_mode()` and add `|| (get_256_colour_mode() &&
   get_seq_chain4_mode())` under the existing V7 compile-time boundary.
3. In `draw_v7ptr()`, replace `get_seq_chain4_mode() && get_chain4_mode()`
   with `get_256_colour_mode() && get_seq_chain4_mode()`; retain the existing
   coordinate conversion. This aligns its condition with the Host path.

This is a candidate to validate, not a claim of repaired hardware semantics.
Failure of address/pixel/pointer proof rejects the candidate; do not add a
fallback to make it pass. The original CPU bank/read/write predicates remain
untouched. Standard VGA predicates remain unchanged by the new V7 alternative.
The previous broad implementation budget is withdrawn. Planning estimate:
3 production files, +6/-3 lines, net +3 before comments; focused test additions
of 80--160 lines. These are not actual diff statistics.
Proof must include real port toggles, patterned pixels and partial dirty
updates, nonzero start address, CPU memory-access correctness, legitimate
CRTC mode changes, and unaffected standard/planar modes on x86/x64. Geometry
alone is insufficient. Publish both EXEs and report pristine-mirror and
current-baseline additions/deletions separately before owner acceptance.

The test-source review identifies four concrete proof receivers in the
existing App VGA-frame smoke, not four new test executables:

- Preserve `verify_display_stride()`'s standard-mode matrix: it explicitly
  disables sequential-chain4, so its existing scale and wrap assertions are
  outside the new V7 alternative and must not be weakened.
- Add real SR4 port toggles to the existing stable V7 mode exercise. Its
  current packed-geometry check requires CPU chain4 to be true, so it cannot
  detect the failing transition. Test both chain4 states without changing
  the CRTC configuration or pre-correcting the derived stride.
- Use patterned backing memory to extend pixel/dirty proof. Existing direct
  packed-painter calls and dirty-alignment cases manually supply the painter
  or stride; they remain useful algorithm regressions but are not evidence
  that mode selection supplied correct values.
- Exercise `draw_v7ptr()` from programmed pointer registers, not only
  `paint_v7ptr`. Existing pointer callback tests bypass the coordinate
  conversion containing the proposed condition change. Retain those callback
  tests and add an independent expected-coordinate check across SR4 toggles.

### S2 implementation evidence

The frozen repair universe is the three display predicates above. Painter,
row-byte stride and pointer coordinates use the existing V7 packed convention;
CPU bank/read/write paths retain their distinct access interpretation. Each
predicate is exercised by the same owner-local `vga_frame_smoke` receiver.
No additional production path, state, helper or compatibility fallback exists.
This evidence does not certify the separate Compat generation contract in S3.

Against S1 `cfa4ae23`, production changes are `nt_graph.c` +1/-1,
`vga_mode.c` +6/-1 and `v7_ports.c` +1/-1: +8/-3, net +5 lines.
The estimate was +6/-3 before comments; the actual includes the short byte-unit
comment and conditional formatting. The existing App VGA smoke adds 101 lines,
deletes none, and creates no new test executable. Shared components, Compat,
INI, snapshots and media are unchanged.

The added receiver programs BIOS V7 modes 66/67/68/69, real SR4 enabled/disabled/
enabled transitions, zero/nonzero CRTC start and the captured 1024-byte virtual
pitch. It checks complete patterned pixels and one-region dirty repaint against
an independent backing-byte row oracle, real CPU chain4 address/readback
differences, and pointer coordinates through the original register receiver.
Existing standard stride/wrap and legitimate mode matrices are retained. Both
x64 and x86 focused runs pass. Linking the new test against the retained S1
behavior fails at the intended width assertion (runtime exit 3), so this is
not merely an assertion of the new implementation's geometry.

The similar-issue review retains CPU bank selection and CPU read/write receiver
conditions because they control CPU access, not scanout. Existing screen-pointer
and wrap branches retain their separate contracts; no speculative fix is added.
Whole-file differences from OpenNT, including earlier accepted changes, are
`nt_graph.c` +62/-14, `vga_mode.c` +9/-3 and `v7_ports.c` +1/-1. These must not
be confused with the +8/-3 incremental S2 repair.

Release builds reuse the regular Ninja intermediate objects, refresh both
package EXEs, and contain no S1 probe strings. Published SHA256 values are:

- x86: `2D0CBF0916256BE4965A663C47CE913DD2617346ADC037C786627DD7EBDA1CAC`
- x64: `C9A10C9744A134AF5AA7DFF3958CCC7F6246D1F261715301BEBEC2FDDA92CBF8`

The owned hidden validation job completes with exit 0. On each width, the
shared suites run first (Lib 48, Emulator 21, x86 15: 84/84), followed by all
App/checks background tests (47/47): 131/131 per width. Qualified elapsed
times are x64 59.45 + 71.02 seconds and x86 58.49 + 73.68 seconds. The App
suite includes snapshot regressions and real Win3.1 boot with six DOS Prompt
full-screen/window roundtrips and CLS. The initial shared-name filter selected
zero tests and is explicitly discarded; qualified reruns use the actual dot/
slash prefixes and `--no-tests=error`.

Five desktop cases per width are intentionally excluded: package smoke and its
compact-console variant, Lib Window retirement/modal, and native Console
display. No Computer Use or foreground desktop interaction occurred. The
owner's File Manager visual acceptance remains pending; these results do not
claim the complete T88 flicker problem, including S3, is solved. Documentation,
source-boundary gates and whitespace checks pass. Task logs remain under the
ignored `build/t88/s2`; disposable negative-proof EXE/media are removed after
their owned process exits. The regular build intermediates are retained for
incremental reuse. S2 is delivered for acceptance, not owner-accepted closure.

### S3: Colour/resolution regression coverage

Owner accepts S2 and admits a separate regression step. Frozen coverage is
standard BIOS graphics 04h/05h/06h/0Dh--13h, V7 graphics 60h--69h, and existing
text-mode/stride/panning/wrap tests. Cases use real BIOS-programmed modes and
the original selected update algorithm. Full repaint after a guest-memory write
must agree with incremental repaint; mode transitions must restore dimensions.
V7 hardware pointer registers must produce the appropriate packed/non-packed
coordinate and restore old background on move/removal. Existing CPU-chain4
packed-mode and independent pixel oracle remain unchanged.

The expected delta is +100--160 lines in the existing App VGA smoke. Owner
additionally requires preserving original condition order in draw_v7ptr():
`get_seq_chain4_mode() && get_256_colour_mode()` (+1/-1, net zero, no truth-value
change). No new target, fixture framework or renderer is introduced. Unsupported
arbitrary register mixtures are not called valid modes. The completion predicate
is dual-width focused and background regression plus an explicit disposition
for each mode class; discovery of a production bug stops implementation and
requires a separate explained repair. Original Compat S3 moves to S4.

Source-qualified mode dispositions: original ega_set_mode explicitly maps V7
07h/0Fh to text 00h, so these requests receive exact fallback assertions, not
graphic-size assumptions. Original video.c's MODE_ENTRY table defines V7 61h
as 720x540; the initial test's 720x400 assumption was wrong and is corrected
against that independent source, not merely against observed output.

### S3 accepted coverage disposition

The owner requests S3 closure after the failing matrix is explicitly reported.
This accepts the added regression coverage, not production correctness. Frozen
graphics cases are 04/05/06/0D/0E/10/11/12/13 and 60--69, each selected through
the original BIOS in forward and reverse order on x86/x64 (38 states per width).
Every state checks explicit dimensions, colour/painter selection and actual CPU
writes followed by incremental/full pixel comparison. All ten V7 modes also
check three pointer positions, old-background restoration and removal. Requests
07/0F have separate exact text-fallback assertions. The matrix collects repaint
failures; its final assertion follows existing stride/pointer receivers and
fixture cleanup, so a known repaint failure cannot skip those checks.

Disposition: geometry, painter selection, fallback and hardware-pointer checks
pass throughout this finite matrix. Incremental/full comparison passes for the
other 17 graphics modes but fails for 63/64 in both orders and widths. The final
assertion remains enabled and fails; no mode is excluded, no result is converted
to success. The production defect is transferred to TODO with a separate repair
admission condition. It is not folded into Compat generation-readiness S4.

The sole production change restores original predicate order in v7_ports.c,
+1/-1, net zero, without changing truth values. Removing temporary 63h diagnostic
printing leaves the permanent test delta +115/-0, net +115, in one existing
test file; no new target, cache, state, public component or runtime path is added.

Background regression reuses the Ninja build trees and refreshes both package
EXEs. Shared suites pass 84/84 per width, and the other App/checks receivers
pass 46/46 per width. The added VGA receiver remains a failed test, not a green
qualification: the final focused rerun records the 63h/64h mismatches and reaches
its final assertion after existing checks and fixture cleanup. Five desktop
cases remain excluded by policy. Initial x64 eight-way shared regression times
out in test-types-boundary-selftest and verifier-negative; the two-way rerun
passes all 84 without budget changes (64.60 seconds x64, 64.74 seconds x86).
App/checks totals are 112.57 seconds x64 and 86.81 seconds x86. A temporary
test-order experiment put a 64x48 isolated pointer fixture before real BIOS
mode changes and produced a heap exception; it is discarded as invalid fixture
ordering, not attributed to production. Final ordering retains the original
terminal pointer fixture and reports repaint failures only after cleanup.

Package SHA256 values:

- x86: `35C7EA49665C7B3F3345981B3E644DE827CE1EB5599AF8A73F580E9281F80D99`
- x64: `D8D2DD40B9E06F4AEE60E43D3B91482718A06CC147D8CBEBE9E1A33228203D26`

User INI, guest media and snapshots remain unchanged. Shared six-component
sources/tests remain unchanged. S3 accepts coverage only; T88 is not qualified
or closed, and the retained failing test prevents an all-green claim.

### S4: Compat generation readiness

Use the existing DIB bind/update/generation lifecycle to distinguish a newly
cleared surface from a reconstructed display. Pointer-only damage must not
qualify a replacement generation for publication. A completed body repaint
qualifies it; subsequent pointer-only updates on that completed generation
remain publishable. Preserve valid completed frames during ordinary updates.

Primary scope is `src/app-softpc/compat/dib_surface.c` and its owner-local
tests, with only an existing callback declaration/caller adjustment if the
body/pointer distinction is not already available. Do not infer completeness
from geometry, elapsed time, a cleared mode-change flag or any nonempty dirty
rectangle. Do not put the rule in Lib/Emulator or create a second publication
path. Planning estimate: 1--3 production files, 15--35 touched production
lines, estimated net increase of 5--20 lines; 60--120 focused test lines.

Proof covers bind then pointer-only damage, completed body plus pointer
damage, later pointer-only movement, replacement of an already complete
generation, and failed/incomplete update. Run dual-width focused and full
required regression, snapshot resume and Win3.1 DOS Prompt roundtrips; publish
both EXEs, report actual added/deleted/net lines and await owner acceptance.

## Repair constraints and acceptance

If the source renderer computes an invalid visible geometry, prefer the
smallest source-local correction consistent with original VGA register
semantics and record its pristine-mirror delta.  If Compat exports an
incomplete transition, correct the producer's publish readiness rather than
having KVM Window guess.  Lib and Emulator may be changed only if evidence
shows their documented copied-frame contract itself is wrong.

Acceptance requires stable host Window geometry during File Manager startup,
unchanged legitimate guest mode changes, dual-width targeted proof and manual
Win3.1 verification.  The task also rechecks the existing Win3.1 DOS Prompt
width cases so a shared mode-transition correction does not regress them.
