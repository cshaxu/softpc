# Win3.1 File Manager VGA width-transition repair

Archived after owner-approved T88 closure on 2026-10-11. Historical open,
planned and deferred statements below describe their original delivery stage;
the [completion audit](M9-T88-completion-audit.md) records final dispositions.

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

### S4: CHAIN2 dirty-contract repair

Owner admits source-local single-path marking. Replace each of the 28 inline
CHAIN2 CGA dirty blocks with the same selected byte/word/string marker call
convention already used by Unchained writers. Retain parameter saves, actual
memory writes, rule labels outside the removed blocks and all render/update
algorithms. Every changed site carries DIVERGENCE(T88-CHAIN2-DIRTY).
Original marking selection supplies CHAIN2 EGA markers accepting CPU offsets
and producing plane-group/bank bitmap entries; CGA/text retain original marker
semantics. No helper fallback, second write path or build generator is added.

Finite convergence ledger: sevid016/017/018/019/020 contain respectively
2/5/5/14/2 blocks. Each must have exactly one selected marker dispatch and
no retained inline dirty accounting. Verify byte/word/dword write, fill,
forward/backward move, BIOS copy entry, zero count and odd/even boundaries;
compare bitmap/bounds, incremental/full pixels, bank switching and CGA/text
transitions on both widths. Static review covers all 28 replacements; runtime
proof covers selected byte writers, marker word/range contracts and CCPU SAS
byte/word/dword calls, not all legacy ModeX compound entry points. An
exploratory compound-entry crash is separately dispositioned below. Existing
19-mode forward/reverse matrix remains enabled. Completion requires the
bounded ledger and background suites green;
desktop and arbitrary untested register mixtures are not claimed qualified.
Expected production replacement removes several hundred repetitive lines;
final accounting distinguishes those deletions from changes to memory writes.
Evidence is owned in ignored build/t88/s4, with hidden child processes,
per-phase logs/exits and 1800-second build / 3600-second test budgets; executor
owns timeout cleanup. Keep regular Ninja intermediates for incremental reuse.

#### S4 implementation and source accounting

The repair leaves all memory-write algorithms intact. Each retired inline
block now calls the existing selected marker slot and retains the original
generated call convention. Byte/word markers leave their inputs unchanged;
range callers restore inputs after count/range normalization. String counts
are byte counts: word/dword counts multiply by two/four; backward ranges
begin at the lowest affected byte. Zero ranges do not dispatch a marker.
No parallel CGA bookkeeping survives. The 56 labels, enum members and local
wrappers belonging only to the removed dirty comparisons are removed too.

Finite rule ledger (original generated function numbers):

- sevid016: 2625, 2629.
- sevid017: 2635, 2643, 2647, 2653, 2661.
- sevid018: 2665, 2671, 2679, 2683, 2689.
- sevid019: 2696, 2697, 2698, 2699, 2700, 2702, 2703, 2706, 2708,
  2712, 2714, 2718, 2720, 2724.
- sevid020: 2726, 2729.

`ev_glue.c` adds three CHAIN2 marker ABI shims sharing one address conversion.
CPU bytes map to plane offsets in pairs; the original Unchained range marker
still owns bitmap writes, signed dirty bounds and bank offsets. Its existing
conservative endpoint coverage is retained, not tightened into a new contract.
The shims preserve the jcode input carriers after conversion. `gfx_updt.c`
selects these markers in its existing EGA/CHAIN2 branch. The unsupported
non-CPU40 configuration retains its original assertion; no standalone branch,
mode number, guest identity or new renderer is introduced.

An initial experiment using the original C `ega_mark_string()` was discarded:
its signed/unsigned comparison leaves dirty_high at the initial -1 in this
CCPU build. The delivered code uses the already-correct C-VID marker instead;
it does not add a second copy of the bookkeeping implementation.

| Production file | This S added/deleted | Full OpenNT diff before | Full OpenNT diff after |
| --- | --- | --- | --- |
| ev_glue.c | +42/-0 | +11/-8 | +53/-8 |
| sevid016.c | +15/-75 | +651/-650 | +652/-711 |
| sevid017.c | +41/-208 | +462/-461 | +468/-634 |
| sevid018.c | +48/-209 | +481/-480 | +494/-654 |
| sevid019.c | +154/-545 | +608/-604 | +664/-1051 |
| sevid020.c | +26/-76 | +620/-616 | +632/-678 |
| gfx_updt.c | +6/-0 | +189/-145 | +195/-145 |

Production total: +332/-1113, net -781. Full original-mirror touched lines
for these seven files increase from 5986 to 7039 (+1053). Code reduction is
not mirror-diff reduction: deleting original inline blocks and their labels
necessarily adds deletions against OpenNT. These counts include pre-existing
native-width adaptations; they are not all new functional edits.

The existing VGA smoke adds marker-owner/parity/group/bank/zero-count
receivers and real SAS byte/word/dword writes, including odd/even addresses,
in all 19 modes, in both mode orders. A prefix guard also verifies that these
writes cannot mutate the unrelated beginning of the plane store. The source
gate inventories exactly 28 local divergence markers,
rejects residual inline CGA accounting and rejects the 56 retired labels.
No existing assertion is relaxed. There is no new test executable or fixture
framework. Byte-entry tests cover five writer sets and three marking owners;
the string/word receiver verifies actual bitmap/bounds and input preservation.

An exploratory direct legacy ModeX compound-entry matrix is not accepted as
passing qualification. Odd-address dword writes crashed with both original
inline dirty code restored and the repair applied, with the same nested
generated rule stack. It exposes a separate old local-carrier issue, not a
reason to rewrite memory writes inside this dirty-only S. CCPU SAS string
operations use byte/word calls; the production receiver uses those paths.
The confirmed old failure is explicitly tracked separately in TODO. This
delivery does not claim every dormant generated compound rule is functional.

Final verification also caught an implementation error before delivery: the
old inline dirty segments in Copy word/dword rules 2699/2702 interleave seven
lines each of address parity/alignment calculation. Those original statements
are retained verbatim; only their dirty bookkeeping is replaced. Removing them
left zeroed address locals, making wide writes change the plane-store prefix
while dirty bounds named the requested offset. The unchanged baseline passed
the same CGA wide-write matrix on both widths; the corrected implementation
passes the complete matrix. No assertion is removed or relaxed. This does not
support rewriting the underlying compound memory algorithms.

Code accounting excludes documents and EXEs: production +332/-1113 (net -781),
existing VGA test +89/-0, source gate +21/-0; combined +442/-1113 (net -671).

Dual-width regular Ninja Release builds and the complete background selection
pass: shared Lib/Emulator/X86 84/84 first (x64 57.73 s, x86 57.92 s), then
App/checks 47/47 (x64 104.50 s, x86 79.83 s). The five native desktop cases
per width remain excluded, not declared passed. Focused VGA passes on both
widths (5.15/7.47 s); this includes snapshot video-write/resume coverage,
the colour/resolution matrix and the new CHAIN2 receivers. No INI, media,
snapshot or shared-corpus file changes. Documentation/source-boundary gates
and git diff --check pass. Disposable baseline exports and obsolete probe
scripts are removed; bounded regression and old-failure evidence is retained
while T88 remains open. Regular Ninja intermediate caches are preserved.

The final artifact refresh/focused receiver also passes on both widths
(4.88/7.31 s). Ninja reports recovery of a prematurely ended x86 log and
rebuilds affected targets; no clean/cache deletion or source change is used
to conceal that additional build work.
Published SHA256: x86
9B3CF4244E8B27D40590A97D5414B000383D9F0808CC6B94FCB8E8FE14057BE2;
x64 72E99EA137FA24868B9ADDB57C1A7A4561F57F2E9DA487240482BE66427FBC7F.
Package sizes are 3503170 and 3101412 bytes respectively. Subsequently the
owner-requested native desktop tests pass 5/5 per width, without Computer Use
or artifact changes. The owner accepts S4 by admitting S5 for design first;
[acceptance record](../history/M9-T88-S4-chain2-dirty-contract.md).
T88 remains open.

### S5: Compat generation readiness

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

#### S5 design brief

Both original body painters and V7 pointer port updates use host_start/end_update;
therefore update depth reaching zero cannot establish body reconstruction.
The existing body-painter InvalidateConsoleDIBits macro is the natural owner-local
classification boundary. Pointer paint/clear and palette invalidation continue
to merge damage but cannot qualify a freshly bound generation.

Keep one generation-owned readiness fact in the existing DIB owner, cleared
on bind even for identical dimensions. Reuse transaction-local pending damage
to record body participation; successful outer completion may establish
readiness only after the original reconstruction is proved complete. Conversion
failure must not establish readiness. Before that, consume no publishable dirty;
after it, preserve pointer-only and palette-only publication and existing merge
semantics. A first successful publication must expose the whole rebuilt surface.
No extra bitmap, previous-frame cache, timer, queue, generation counter or
consumer-side filter is proposed. No pending-mode flag or dirty bounding-box
size is treated as a substitute for body reconstruction.

Before implementing this qualification, audit every graphics bind/resize route
against the original full-refresh trigger: a small successful body paint alone
must not be mistaken for a reconstructed new surface. If an existing completion
cannot supply that fact, stop and report the exact missing contract instead of
adding an inferred readiness workaround. Snapshot restoration keeps its original
host_mark_screen_refresh / host_flush_screen path and must pass the same rule;
palette/invalidate_all alone cannot manufacture restoration success.

Refined estimate: three Compat production files (dib_surface.c/h, v7_pointer.c),
approximately +25--40/-8--15 lines, net +10--32; existing VGA test +60--120 lines.
This is slightly wider than the initial estimate because body, pointer and
palette currently share the same damage API. Original-mirror diff is expected
to remain +0/-0. This is a design estimate, not measured implementation.
The finite receiver set is bind plus pointer-only, bind plus palette-only,
completed body plus pointer, later pointer-only, same-size rebind, nested update,
no-op/failed conversion and snapshot reconstruction, plus all retained modes.

#### S5 implementation contract and finite ledger

The original `graphicsResize -> CreateConsoleScreenBuffer` path has one
Compat binding receiver. Bind now invokes the existing
`host_mark_screen_refresh -> nt_mark_screen_refresh -> screen_refresh_required`
request: original C-VID dirty total >= 1000000 denotes a full refresh. The
selected CGA, EGA/VGA normal, split and wrapped update loops choose their
existing whole-screen branches before calling the body painters. Those
painters invalidate only after writing their pixels. Their callbacks precede
the original dirty clear and final host_end_update.

The existing pending-damage validity value carries body participation in
that original full refresh. At the successful outer completion it establishes
one DIB-owned ready fact; palette and pointer damage cannot set it. Small
body updates without the full-refresh request cannot establish readiness.
The first publication covers the whole surface. This is not qualification
by bounding-box size, BIOS mode, guest identity, mode-change flag or time.
No additional renderer, cache, public contract or original-source patch exists.
The CPU-state query applies only to an actually bound, unready surface:
pre-initialization text painting shares the invalidation macro but must not
read uninitialized C-VID state.

Pointer composition reads that same ready fact or this transaction's body
participation. It cannot save a cleared background before reconstruction and
restore it over the first completed body. Original draw_v7ptr still performs
composition after the body, inside the same transaction. No pointer state or
alternate renderer is introduced. Transparent-pointer pixels are checked in
both painter formats along with the final body publication.

Frozen coverage units and receivers:

- Fresh 1-bit and 8-bit binding, actual V7 pointer-only update, palette-only
  output and explicit invalidation: verify_dib_generation_readiness rejects
  publication before body reconstruction.
- Empty update, invalid/outside damage and non-full incremental body:
  the same receiver verifies no readiness or publication is fabricated.
  Failed damage admission precedes conversion; it is not a simulated allocator
  failure or proof of arbitrary memory-corruption recovery.
- Full body, nested completion, original dirty clear before final end,
  first/last converted pixels and full first dirty: the same receiver checks
  both painter formats, with no publication at the inner end.
- Ready pointer/palette changes: verify_pointer_update_pixels covers actual
  pointer paint/clear callbacks and retained indexed pixels; generation
  readiness verifies subsequent small damage and full palette invalidation.
- Same-size rebind within a nested transaction: generation readiness rejects
  the preceding generation's pending body qualification and pointer damage.
- Real original rendering and VM publication: verify_driver_geometry uses
  host_mark_screen_refresh / host_flush_screen before consuming a new surface;
  full-height half updates retain dimensions and pixel content afterwards.
- Existing low-level Compat transaction/mono tests initialize through the
  actual Machine owner, using a disposable 1.44 MiB image in their isolated
  working directory. They qualify the first full body before testing later
  small transactions; invalidate_all alone cannot qualify a new binding.
- Retained colour/resolution, CHAIN2, split/wrap and snapshot paths: existing
  VGA matrix and snapshot reconstruction/video-write receivers remain required.

The completion predicate is dual-width passing receivers and required shared,
App and integration regression, published EXEs and a pushed complete delivery.
Native desktop tests require the separate reserved-input window; unrun tests
are never declared passed. T88 and S5 await owner acceptance after delivery.

## S5 measured delivery evidence

## Scope and retained ownership

The owner approves implementation, dual-width publication, testing, commit and
push, followed by owner acceptance. Baseline is `e4ad4bd8`; S4 implementation
is `7cda5375`. S5 does not close T88.

Compat retains one DIB and one transaction/damage publication path. A fresh
binding requests the existing original full refresh. A body invalidation in
that refresh qualifies readiness only after successful outer completion.
Pointer/palette invalidations cannot qualify a new generation. The pointer
reads this same fact, including body participation inside the current update,
so it cannot save a cleared pre-body background and restore it over that body.
No original updater, machine state, snapshot format or shared API changes.

The finite production receiver ledger is in the
[proposal](M9-T88-win31-file-manager-vga-width-transition.md#s5-implementation-contract-and-finite-ledger).
Its searched scope is Compat binding/damage, original body invalidation,
pointer paint/clear, palette invalidation and snapshot reconstruction. Searches
used `rg` for `softpc_standalone_dib_damage`, `InvalidateConsoleDIBits`,
`softpc_standalone_dib_bind` and `softpc_standalone_dib_invalidate_all`.
The single graphics binding receiver is retained; body calls use the existing
macro, pointer/palette use the same transaction with non-body classification,
and snapshot reconstruction uses the original refresh/flush before invalidation.
No additional production receiver or parallel output path was found.

## Actual changed-path accounting

Counts use `git diff --numstat e4ad4bd8` before the delivery commit, excluding
documentation and EXEs. Production:

- `dib_surface.c`: +38/-4, net +34.
- `dib_surface.h`: +4/-2, net +2.
- `v7_pointer.c`: +4/-3, net +1.
- Total: +46/-9, net +37 across three Compat files.

Tests:

- Existing VGA frame smoke: +100/-7, net +93.
- Existing text-console Compat smoke: +31/-6, net +25.
- Total: +131/-13, net +118 across two files.

Combined production/tests: +177/-22, net +155. Original mirror, Machine/App
production and all six shared source/test roots have zero changed paths.
The additional ready value is one integer; pending body qualification reuses
the existing pending-valid integer. No buffer, queue, thread or state machine
is added. Production is five lines above the upper net estimate because the
pointer must compose after body but before transaction completion, and invalid
damage must not qualify readiness. Test growth includes actual Machine-owned
initialization rather than a duplicated C-VID fixture setup.

## Verification corrections and limits

Initial experiments exposed a C-VID getter being reached by text output before
CPU initialization. Qualification now requires an actually bound generation.
The low-level Compat fixture binds graphics and therefore initializes through
the actual Machine owner with a disposable image in its isolated working
directory; it does not recreate CPU state itself.

Several old graphics fixtures used `invalidate_all` or replaced the initial
full-refresh request with dirty total 20001 to publish manually initialized
surfaces. They now establish the first body via the original refresh request.
Later 20001-threshold, raw row/stride, pixel, 19-mode, CHAIN2 and split/wrap
assertions remain. Assertions were not weakened to accommodate readiness.

An early x86 aggregate run failed the timed DOS-prompt roundtrip; independent
serial three-test qualification then passed on both widths. This is not proof
of its cause: concurrent architecture scheduling was a hypothesis, not a
confirmed defect. Final App regression uses sequential architecture jobs.
No production delay, test timeout relaxation or skipped roundtrip was added.

Regular cached Ninja trees were used without manual cleaning. x86 Ninja
reported premature-log recovery and rebuilt unchanged targets; this is
disclosed build behavior, not a source-mirror change or a claim that every
component compiled once. Owned background logs/exit records remain below
`build/t88/s5`, with declared build/test budgets of 1800/3600 seconds.

Native desktop tests are separate from background regression. The reserved
input-window question had no reply at delivery; five tests per width are not
run or claimed passed. No Computer Use, guest media/configuration edit or
manual Win3.1 acceptance is claimed. The owner's pre-existing untracked
snapshot is preserved and excluded from the delivery commit.

Final production-qualified shared suites pass 84/84 per width (x64 90.48 s,
x86 115.31 s). The final three-test selection passes 3/3 per width (71.24 /
71.42 s), including the real runtime restart and DOS-prompt roundtrip. Final
x64/x86 App/checks regression passes 47/47 (84.23 / 85.07 s), bringing each
background selection to 131/131. Snapshot transaction, cross-process/cross-width,
media and safe-boundary receivers are part of that App regression.

Published packages:

- x64: 3102540 bytes, SHA-256
  `45371D8F53C0AD5B4E33039B9721085C7673A4C4B9ECB10B944277FDB28F623C`.
- x86: 3504300 bytes, SHA-256
  `93C2EE38FC8975CC7003F6E2F419A531F17F44084B39D66947C64800209C384A`.

Compared with S4, the EXEs grow by 1128/1130 bytes respectively. Published
files are the existing two package EXEs only. The owner subsequently reports
S5 acceptance and requests including the existing snapshot unchanged in P2.
[S5 acceptance and asset record](../history/M9-T88-S5-compat-generation-readiness.md).
T88 remains open; no next S is admitted.

## S6 nested CHAIN2 rule investigation

The owner admits investigation of the deferred compound-entry crash. Scope is
the exact dword-to-word nested failure, original ENTER/LEAVE and continuation
contract, all analogous CHAIN2 continuations and actual CPU/SAS call sites.
Production/source tests remain unchanged during diagnosis; ignored probes reuse
existing link inputs and an isolated disposable test image. Completion needs
runtime locals/stack evidence, comparison with original OpenNT, caller
reachability and a minimal repair proposal, not a speculative writer rewrite.

### S6 causal result and bounded repair proposal

Both widths reproduce the direct mode-0 odd-address dword write crash with
unmodified production source and debug symbols for sevid017. The first failing
read is sevid017.c:1323 in S_2637_Chain2ModeXDwordWrite_00000000, immediately
after S_2631_Chain2ModeXWordWrite_00000000 returns. LocalIUH is null; the dword
activation's CopyLocalIUH still contains its non-null saved outer carrier.

The odd-word branch calls L21_19if_d as a C function. Its wrapper re-enters
crules at the continuation ID, initializing the automatic CopyLocalIUH to
null without ENTER_SECTION. The continuation frees the word carrier and
assigns that null to the shared static LocalIUH. It returns to the dword
activation, which dereferences null. OpenNT sevid017 contains the same wrapper,
call and ENTER/LEAVE mechanism; SAVED means static in both trees. This is a
generated continuation-frame defect, not an x64 pointer-width error, dirty
marker regression or allocation failure.

A disposable diagnostic changes only this tail transfer to
`goto L21_19if_d;`, preserving the word activation's saved parent carrier.
The original variant crashes at the same source read on x86 and x64; the
one-transfer variant reaches the end and exits 0 on both widths. This proves
the causal repair boundary, not all dword data semantics. No diagnostic source
is a product build input, and no production source or published EXE is changed.
An initial GDB script reports an error after the positive process exits because
there is no stack; the qualified positive rerun invokes the EXE directly and
requires its actual exit 0. The qualified matrix supervisor exits 0.

Current production reachability is separately accounted for: phy_w32 performs
two phy_w16 calls; CCPU SAS fills/moves loop over scalar byte/word operations.
The selected C_VID Glue_writes table registers only byte/word write, fill and
move handlers. No current App production caller of write_dword_ev_glue is
found. Odd word scalar output may reset the static carrier on return but has
no nested generated caller consuming it in that CPU path. The directly
invoked dword table entry is a real low-level defect, not a demonstrated normal
guest crash. This does not certify every indirect legacy entry as unreachable.

The mechanical sweep finds 440 same-function tail-continuation calls across
31 generated files. Forty-eight are in CHAIN2 procedures: ten read continuations
in sevid000/001 and 38 write continuations in sevid016--020 (2/7/7/16/6).
These are candidates of the same defect form, not 440 proven crashing cases.
Independent S_* procedure calls must remain calls; only L_* continuations
within the current procedure are candidates for a local branch. A broader
source rewrite is not admitted by this investigation.

Smallest confirmed repair is one existing two-line tail-call block replaced
by one local goto (+1/-2, net -1 with an inline provenance comment). A consistent
CHAIN2 write cleanup would touch five files and 38 blocks, +38/-76
(net -38 with inline comments), requiring actual nested write/fill/move and pixel
oracles before acceptance. It creates no helper, stack object or second writer.
Because it increases pristine-mirror touched lines, production implementation
requires owner approval. The active S6 owns this confirmed item; it is removed
from the deferred TODO rather than left as an unexplained hypothesis.

### S6 per-site validation admission

Owner request: "请你逐项验证！给我完整答案". The frozen units are 38 write
tail-continuation call sites in sevid016--020 and ten CHAIN2 read sites in
sevid000/001. Each needs a named owner, target, source line, saved-carrier and
ENTER/LEAVE disposition, plus runtime reach/variant evidence or an explicit
uncovered reason. Similar text alone is not proof of a defect. Diagnostics
remain ignored, background-only, with no product-source or published EXE change.

### S6 per-site qualified result and minimal proposed diff

本节记录 S6 逐项诊断，不是生产修复或全产品验收。冻结范围为上述
48 个 CHAIN2 续接调用；未把另外 392 个机械相似项纳入修复范围。

逐项对照源代码：48 个目标都是当前过程自己的 L_* 标签，目标到出口之间
没有建立新局部帧的 ENTER，却执行现有 LEAVE；旧 C 调用重新进入 crules，
其自动 CopyLocalIUH 从零开始，因而错误恢复静态 LocalIUH。原始 OpenNT
保留同一调用/包装/恢复形式。这不是 S4 dirty 修复或 x64 类型移植引入。

#### 逐项运行证据与边界

在独立后台进程中完成 192 次隔离实验：48 个位置 × 原调用/局部跳转
× x86/x64。每次只保留目标位置的旧调用，其余 47 处采用局部跳转，以免
前面的错误遮挡目标。诊断入口设置非空父局部帧并调用真正的规则表入口；
该父帧及探针只存在于忽略的诊断副本，不是生产代码或第二条显存路径。

每种宽度的 48 个局部跳转实验都退出 0，恢复父局部帧正确。每种宽度
旧调用的 48 个目标均实际到达，返回检查都记录 actual=NULL、
expected=非空。外部表现分为 2 个崩溃、14 个父帧检查失败、32 个最终
返回 0（内部恢复已错，但后来外层 LEAVE 又恢复它自己的父帧，掩盖错误）。
两种宽度的分类完全一致。不能把 48 处都描述成正常客户机必然崩溃。

之前较宽的入口矩阵共收集 1664 次进程结果，每种宽度每种变体 416 次。
旧调用变体每种宽度 252 次正常退出、164 次崩溃；局部跳转变体每种宽度
316 次正常退出、100 次崩溃。收集进程退出 0 不代表这些实验全通过。
剩余 100 项的分类在两个宽度一致：

- 96 项为 ModeX 显存来源的复合 move，目标续接尚未到达就调用空的
  legacy string-read 回调。setReadPointers 没有安装该 bulk 回调，当前
  CCPU 标量路径也不使用它；探针没有满足这一旧入口的回调前提。
  代表 GDB 停在地址 0，调用方为 S2629。不凭这些实验提出新的产品补偿。
- 4 项为读串探针把目的地址设成整数偏移，而该入口要求宿主目的指针。
  代表 GDB 停在实际存储操作，目标地址为 0x700。隔离实验已统一使用
  合法目的指针，不把这四项作为镜像代码错误。

这些事实限定了本轮证明：续接作用域/恢复错误已经逐项证明；没有以哈希
或进程存活替代显存、像素、dirty、snapshot 的完整正确性验收。生产修复
实施时还需要常设回归和双宽度产品回归。

#### 最小修改

只替换每个两行的 L_* 尾调用块，保留现有标签、ENTER/LEAVE、
真实 S_* 过程调用和全部读写/dirty 算法。例如：

```diff
- { extern IUH L21_19if_d();
- IUH returnValue,v1=0,v2=0,v3=0,v4=0; returnValue=L21_19if_d(v1,v2,v3,v4); return(returnValue); }
+ goto L21_19if_d; /* T88: keep this activation's saved locals. */
```

示例仅压缩空白便于阅读；拟议实际补丁保留周边原始制表符、语句和注释。
同一函数局部跳转保留这一调用层已保存的父帧，现有目标负责原有清理。
不新增 helper/状态/接口，不改变 snapshot 格式，不删除包装函数或重排条件。

校正之前的行数预估：原块是两行而非三行。单点 +1/-2，净 -1；38 个
写入点为五文件 +38/-76，净 -38；加十个读取点后七文件 +48/-96，净 -48。
理由标记与 goto 同行，以上包括标记。镜像新差异仍是 48 处原始代码修改，
不能把净减少代码误说成减少了与 OpenNT 的差异。拟议完整补丁的
git apply --check 通过，未应用到生产源码。

| 文件 | 位置数 | 新增 | 删除 | 净变动 |
| --- | ---: | ---: | ---: | ---: |
| sevid000.c | 6 | 6 | 12 | -6 |
| sevid001.c | 4 | 4 | 8 | -4 |
| sevid016.c | 2 | 2 | 4 | -2 |
| sevid017.c | 7 | 7 | 14 | -7 |
| sevid018.c | 7 | 7 | 14 | -7 |
| sevid019.c | 16 | 16 | 32 | -16 |
| sevid020.c | 6 | 6 | 12 | -6 |

#### 48 项逐项处置

行号指 S6 诊断基线 21220ac1 的调用点。所有条目的目标/恢复关系都已与
原始源码核对；“旧表现”是隔离父帧实验，不是日常客户机发生频率。

| 调用点 | 续接标签 | 所属过程 | 旧表现（x86/x64 相同） | 处置依据 |
| --- | --- | --- | --- | --- |
| sevid000.c:828 | L26_1if_d | RdMode0Chain2WordRead | 崩溃 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid000.c:1712 | L26_24if_d | RdMode1Chain2WordRead | 崩溃 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid000.c:1855 | L26_26if_d | RdMode1Chain2StringReadFwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid000.c:1920 | L26_28if_d | RdMode1Chain2StringReadFwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid000.c:937 | L26_3if_d | RdMode0Chain2StringReadFwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid000.c:982 | L26_5if_d | RdMode0Chain2StringReadFwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid001.c:1162 | L26_58if_d | RdMode0Chain2StringReadBwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid001.c:1198 | L26_60if_d | RdMode0Chain2StringReadBwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid001.c:1474 | L26_78if_d | RdMode1Chain2StringReadBwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid001.c:1539 | L26_80if_d | RdMode1Chain2StringReadBwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid016.c:4026 | L21_1if_d | Chain2ModeXByteWrite_00000000 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid016.c:4362 | L21_9if_d | Chain2ModeXFwdByteMove_00000000 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:824 | L21_19if_d | Chain2ModeXWordWrite_00000000 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:1150 | L21_24if_d | Chain2ModeXFwdWordMove_00000000 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:1712 | L21_35if_d | Chain2ModeXByteWrite_00000001 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:2048 | L21_43if_d | Chain2ModeXFwdByteMove_00000001 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:2237 | L21_53if_d | Chain2ModeXWordWrite_00000001 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:2563 | L21_58if_d | Chain2ModeXFwdWordMove_00000001 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid017.c:3153 | L21_69if_d | Chain2ModeXByteWrite_00000002 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:2188 | L21_103if_d | Chain2ModeXByteWrite_00000003 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:2524 | L21_111if_d | Chain2ModeXFwdByteMove_00000003 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:2713 | L21_121if_d | Chain2ModeXWordWrite_00000003 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:3039 | L21_126if_d | Chain2ModeXFwdWordMove_00000003 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:1098 | L21_77if_d | Chain2ModeXFwdByteMove_00000002 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:1287 | L21_87if_d | Chain2ModeXWordWrite_00000002 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid018.c:1613 | L21_92if_d | Chain2ModeXFwdWordMove_00000002 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1184 | L21_137if_d | Chain2ByteFill_Copy | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1339 | L21_141if_d | Chain2ByteMove_Copy_Fwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1296 | L21_143if_d | Chain2ByteMove_Copy_Fwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1369 | L21_147if_d | Chain2ByteMove_Copy_Fwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1401 | L21_149if_d | Chain2ByteMove_Copy_Fwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1491 | L21_153if_d | Chain2WordWrite_Copy | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1604 | L21_155if_d | Chain2WordFill_Copy | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1760 | L21_161if_d | Chain2DwordWrite_Copy | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:1889 | L21_163if_d | Chain2DwordFill_Copy | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:2160 | L21_167if_d | Chain2ModeXBwdByteMove_00000000 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:2433 | L21_177if_d | Chain2ModeXBwdWordMove_00000000 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:2790 | L21_185if_d | Chain2ModeXBwdByteMove_00000001 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:3063 | L21_195if_d | Chain2ModeXBwdWordMove_00000001 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:3420 | L21_203if_d | Chain2ModeXBwdByteMove_00000002 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:3693 | L21_213if_d | Chain2ModeXBwdWordMove_00000002 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid019.c:4050 | L21_221if_d | Chain2ModeXBwdByteMove_00000003 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid020.c:875 | L21_231if_d | Chain2ModeXBwdWordMove_00000003 | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid020.c:1179 | L21_239if_d | Chain2ByteMove_Copy_Bwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid020.c:1137 | L21_241if_d | Chain2ByteMove_Copy_Bwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid020.c:1209 | L21_245if_d | Chain2ByteMove_Copy_Bwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid020.c:1241 | L21_247if_d | Chain2ByteMove_Copy_Bwd | 外层掩盖，返回 0 | 两宽度已到达；旧恢复错误，局部跳转正确 |
| sevid020.c:1329 | L21_251if_d | Chain2WordMove_Copy_Bwd | 外层恢复失败 | 两宽度已到达；旧恢复错误，局部跳转正确 |

S6 调研给出的建议是统一修复这 48 处已经证明的同类作用域错误，不扩大到
其他规则文件，不新增任何显存写入分流。生产实施尚待 owner 批准；本轮
没有修改 src、test、发布 EXE、INI、介质或 snapshot，T88/S6 不自动收口。

### S6 P3 owner-approved implementation

Owner 批准同一提交统一清理 38 个写入和十个读取续接点；在明确说明
这会增加与原始镜像的差异后，Owner 再次明确回答“批准”，要求完成编译、
发布、测试、提交、推送后等待验收。以下实施记录承接 P2 诊断，不将其
未改生产源码的历史事实误当作 P3 当前状态。

实际生产修改恰为七文件 +48/-96，净 -48，包含同行理由注释；每个旧
两行 L_* 尾调用块换成一行同过程 goto。包装函数、真实 S_* 调用、
显存读写、dirty、snapshot 格式和所有公共六组件均未改变。逐点检查
48 个替换都对应原调用，没有额外生产变化。与原始 OpenNT 的七文件
增删合计 8490→8634，增加 144；逐文件账目见
[pristine ledger](../etc/evidence/softpc/pristine-divergence-current.md#t88-s6-chain2-续接作用域修复)。

常设证明复用现有 VGA case，不增加产品入口或重复测试组件：

- 16 组 ModeX dword/双 word 分解对照覆盖四种写入方式、四种地址余数。
  这是该规则实际使用的嵌套调用组合；上层值取 0xA5 保持 legacy
  wrapper 的既有值宽度语义，不夹带数据算法修订。
- 264 组 byte/word/dword 写入、fill 和双向 move 配置各重放两遍，
  比较完整 plane 数据；每次复合调用后再执行一个普通 byte 哨兵调用。
  五种 writer 都覆盖四种地址余数；RAM 来源使用真实原生指针，Copy
  move 额外覆盖原有 video 来源，不安装缺失的 ModeX bulk 回调。
- 56 组 byte/word/dword 读取和双向 string-read 配置各重放两遍，
  对照结果并检查目的缓冲区两端 canary；字符串覆盖两种既有布局 flag。
- 独立 test 翻译单元仅承接原生宽度 C-VID 寄存器 ABI，避免改变 CPU
  fixture 的 guest-width IUH。它只链接进既有 VGA test，不进入产品。
- 原有源码门禁逐点核对全部 48 个 local transfer，并拒绝恢复为旧的
  returnValue=L_* 调用；没有把其他生成家族的机械候选自动升级为修复。

开发阶段先补齐新 fixture 缺失的 EGA 容量头声明；初始把所有 writer 的
dword 都假定为两个 word 的 oracle 也未通过。源码核对后将该分解断言
限定于真正用此分解的 ModeX，Copy 原有内联四字节算法保持不动，仅作
续接/重放验证。不能把此测试修正宣称为认证了 Copy 的全部硬件算法，
也不为了让这个过宽断言通过而扩大生产补丁。

测试、构建和门禁四路径共 +142/-1，净 +141：native-rule fixture
+116/-0，现有 VGA test 接线 +2/-0，root CMake +3/-1，既有源码门禁
+21/-0。生产与这些验证代码合计 +190/-97，净 +93；文档与 EXE 不计入
该代码账目。不会以生产净减少 48 行掩盖验证代码的增加。

后台作业分别声明 build 1800 秒、suite 3600 秒预算，保留日志和退出码，
退出由启动方负责，不做 30 秒截断。复用现有 Ninja build 树和已编译的
未变对象；x86 原有 Ninja 日志尾部恢复触发完整 578-step 构建，成功后
测试继续复用其产物。最后的 EOF 空行清理后 focused 重编又遇到同一
日志恢复，触发 305-step 重建；这属于现有 cache 的实际例外，不宣称
全部对象本轮只编译一次，也未新建另一套 build 树或主动清空缓存。
早期 fixture 失败
不计为通过；最终资格只采用修正后当前 source 对应的 focused/full 结果。

最终资格：两种架构各有 84/84 共享测试、47/47 App/门禁测试通过，
合计 131/131 后台测试。最新 native fixture 的 focused VGA 结果也在
两种架构通过。App cohort 覆盖 snapshot transaction/cross-process、
snapshot boundary、runtime restart、input continuation、command provider
和 presentation shutdown；VGA cohort 保留 CGA/EGA/VGA/V7、颜色/分辨率、
pointer/dirty 和 snapshot surface 重建矩阵。五项原生 desktop 测试每种
架构未跑，不冒称 136/136 或人工 Win3.1 验收通过。

源码门禁、文档门禁和 git diff --check 通过。构建日志中的新 fixture
初期失败已修正，最终结果只认当前源码对应的重跑；没有修改生产算法
来绕过测试。忽略区的诊断临时 compiler inputs/objects 已清理，正式
Ninja 中间体和有用诊断证据保留，测试与发布不依赖这些临时输入。

发布产物：

- x64：3094348 bytes，SHA-256
  `99D8F132642A0A776E0F1E4FE6635D081FA3CE6CD145067AD171E3EF633259E3`。
- x86：3494060 bytes，SHA-256
  `386DE2AF207FA40845029D3540FC26212B47BC5CCE2F1B7D7035A5731D752D47`。

相对已接受 S5 产物，x64 减少 8192 bytes，x86 减少 10240 bytes。
snapshot.sol 的 SHA-256 仍为
`D171247173D4954CC41F4A17A606985FF71F9EA0337C5AD1FA2F6DB31E8F6594`；
没有配置或介质变化。本 P3 完整交付后等待 owner 验收，S6/T88 保持开放。

### S6 P4 complete native desktop qualification

Owner requires all five previously excluded native desktop cases on both
architectures before acceptance. On the unchanged f1d394ca sources/artifacts,
the existing CTest desktop selection runs serially, x64 then x86, without
Computer Use, rebuilding, changed test criteria or a 30-second cutoff. Each
phase has an owned hidden supervisor, captured stdout/stderr, actual exit
record and 1800-second declared outer budget.

| Registered desktop case | x64 | x86 |
| --- | --- | --- |
| app-softpc/integration/package_smoke | Passed | Passed |
| app-softpc/integration/package_smoke/compact_console | Passed | Passed |
| lib.kvm_window_retirement | Passed | Passed |
| lib.kvm_window_modal | Passed | Passed |
| lib.console_broker_display | Passed | Passed |

x64 finishes 5/5 in 11.88 seconds; x86 finishes 5/5 in 12.04 seconds. Both
CTest invocations and the supervisor exit 0. Together with P3's 131/131
background cohorts, all 136 registered tests are covered and pass per width;
the registration counts are checked again, not inferred from old task totals.
Artifact hashes retain the P3 values above. Source, tests, build configuration,
INI, media and snapshot are unchanged. P4 commits evidence/state documents
only; S6/T88 remain open pending owner acceptance, not automatically closed.

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
