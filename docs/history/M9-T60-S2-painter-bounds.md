# Original painter bounds

This is the completed S2 record under the admitted
[T60 repair plan](../history/M9-T60-reference-repairs-proposal.md).
Original owner request: 单人双角色模式执行 ntvdmx64 softpc 补丁导入mvdm任务.
Lib/Common and their tests/manifests remain unchanged without separate approval.

Retain original rendering algorithms and coordinate contracts. First classify
EGA low/medium/high, VGA packed/medium/high and V7 packed implementations,
their selected scale variants, and monochrome entries. Establish
source stride/capacity and destination pixel/group units before changing bounds.
MONITOR-only frozen painters are not the selected CCPU production path; account
for them explicitly rather than claiming test coverage for inactive code.

Finite proof ledger: empty/negative dimensions and origins; right/bottom edge;
nonzero valid origin; source last complete row; excessive dimensions; valid
pixel equality. Each selected painter needs a disposition and focused proof.
No universal pixel formula may be applied to group-count arguments. Narrow
existing source diffs, existing product tests and dual-width full suites are
required. No new renderer, panning policy, shared API or external payload.

Initial inspection confirms EGA high uses pixel screen_x and eight-pixel width
groups, whereas VGA high uses eight-pixel groups for both screen_x and width.
EGA's height-only upper bound admits zero to a decrementing do/while loop.
The first product regression calls that actual painter with zero height.
This is not yet a completed repair; all ledger rows remain under investigation.

Initial red/green evidence: the x64 product vga-frame-smoke faults with a real
EGA high zero-height call on ebd3b76. Adding an entry rejection for nonpositive
dimensions/negative origins to the standard low/medium/high EGA paths makes
the same test (now exercising all three) pass. This is partial implementation,
not S2 acceptance: right/bottom clipping, source capacity and the remaining
selected VGA paths still require proof before full builds, commit or closure.
Both fixed package EXEs remain the S1 delivery during this investigation.

## Selected-path ledger and implementation

Inspection of compat/platform.c config_inquire and nt_graph.c
select_paint_routines proves fixed scale 2 and depth 8. No other production
caller changes that scale. Seven standard colour painters are the bounded
production universe; no new mode or renderer is selected by this repair.

| Painter | x pixels / width-unit pixels / y pixels | Source unit and stride | Disposition |
| --- | --- | --- | --- |
| EGA low standard | 2 / 16 / 2 | Four-byte plane group; offset_per_line groups | Clip actual surface, reject empty/negative input, cap source rows. |
| EGA medium standard | 1 / 8 / 2 | Four-byte plane group; offset_per_line groups | Same rule in its own units. |
| EGA high standard | 1 / 8 / 1 | Four-byte plane group; offset_per_line groups | Same rule; retain SVGA-size surfaces rather than fixed 640x480 ceiling. |
| VGA packed standard | 2 / 2 / 2 | Byte; bytes_per_line bytes | Reject before arithmetic; subtraction-based destination bounds and finite source rows. |
| VGA medium standard | 8 / 8 / 2 | Four-byte plane group; offset_per_line groups | Clip before vertical doubling, preventing overflow/zero-row loop. |
| VGA high standard | 8 / 8 / 1 | Four-byte plane group; offset_per_line groups | Replace byte-conversion temporaries with equivalent group-space source bounds. |
| V7 packed standard | 1 / 1 / 1 | Byte; offset_per_line bytes | Replace fixed size-only guard with actual source/destination bounds. |
| big/huge variants | Nonselected scale 3/4 | Historical implementations/stubs | No production caller selects them; unchanged, no runtime qualification claimed. |
| mono variants | Nonselected display depth | Historical NOT SUPPORTED stubs | No paint loop or memory mutation; unchanged. |
| frozen variants | MONITOR-only | Uncompiled in CCPU product | Unchanged; not confused with current KVM Window freeze. |

Source pointers remain the original EGA_planes/regen_ptr mapping. Bounds are
checked before offset multiplication, addition or vertical scaling. Complete
groups/rows fitting the surface and finite allocation are retained; empty
regions are ignored. The original inner loops, lookup tables, stride ownership
and dirty notification remain unchanged. No generalized clipping framework,
duplicate renderer or helper ABI is introduced. Hardware wrap/panning remains
the original producer's responsibility, separately examined by S3/S4.

The existing product test temporarily substitutes a canary-guarded DIB and
restores the original pointer. All seven painters run the same 13 cases:
nonzero aligned origin, zero width/height, negative width/height/x/y/offset,
INT_MAX offset/origin, INT_MAX extents at the last group/row, final source
group with excessive width/height, and one-past-source. Every destination
pixel and both guards are verified, not just the dirty rectangle. The original
mode/palette tests remain. Initial EGA zero-height red proof was a real access
fault; the expanded 91-case x64 matrix passes after the narrow repairs.

Sweep commands: rg for graph_std/graph_big/graph_huge, MONITOR, BIGWIN and
do/while in nt_ega.c/nt_vga.c; rg for host_set_screen_scale and
host_display_depth in src. Inactive variants are explicitly not claimed fixed.

## Executor verification

Both full tests builds succeeded and refreshed the fixed packages. Serial
CTest passed x64 97/97 (76.78 s), then x86 97/97 (68.57 s). This includes the
91-case matrix, original mode/palette checks, package tests, shared manifests,
component gates and documentation governance. Historical TEXT macro-redefinition
warnings remain; no new warning class or manual guest acceptance is claimed.

Counted with git diff --numstat ebd3b76 -- src test: nt_ega.c +33/-12,
nt_vga.c +39/-50, production net +10; vga_frame_smoke.c +87/-3, test net +84;
combined net +94. Against the original OpenNT files, cumulative differences
are nt_ega.c +37/-12 and nt_vga.c +43/-19 (excluding line-ending differences).
The lookup/expansion loops and public ABI are unchanged. No Lib/Common, shared
tests/manifests, App/VM/Compat, INI, guest media or ROM path is modified.

Fixed x86: 3,488,093 bytes, SHA256
4FF6F6C12E09DDB199BE98F6595B6E5406C7593D261167DA1D8399EF6F8A06EE.
Fixed x64: 2,848,726 bytes, SHA256
100B6BA1E1DB8B8D571E06EF34D55002A45A6C329A5BA9C456CB6236AFC643C7.
These replace the earlier S1 checkpoint artifacts. Executor delivery awaits
actual-commit review; S3-S7 and whole-T60 acceptance remain separate.

## Coordinator actual-commit review

Reviewed pushed b693abe against ebd3b76 and the S2 packet. Each selected painter
keeps its original address units and inner pixel expansion. Nonpositive sizes
and negative/outside origins return before pointer formation. Subtraction and
division bound extents before scaling; source capacity arithmetic is performed
in bytes for packed modes and plane groups for interleaved modes. The source
row bound includes the final complete row without a new wrap implementation.
EGA partial final groups are not painted, preserving whole-group writes.

Tests call the production painters, check every output byte plus guards and
restore the original DIB pointer before later package assertions. They add
no test-only production branch. Actual protected-path diff is empty; P1 was
pushed clean. Post-commit VGA-frame/runtime-cursor/package checks pass 3/3 on
each width. Full-suite evidence is 97/97 at both widths. Only disposable
types-layout fixtures were removed; logs/build trees and user data remain.

S2 is closed by this review. P2 changes documentation only; T60 remains open
for S3-S7. This does not claim testing of nonselected historical scale/frozen
implementations or resolution of panning/dirty-address questions.
