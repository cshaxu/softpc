# Horizontal pel-panning repair

T60 S4 follows the owner-admitted [finite plan](../history/M9-T60-reference-repairs-proposal.md).
Baseline is the closed S3 implementation 6931729. Preserve original MVDM
structure and minimize original-source diff, not net line count. Lib/Common
and their shared tests remain protected; no source import or new renderer.

Inspect the original attribute-controller register write, panning state,
refresh invalidation and selected graphics painters before changing code.
The NTVDMx64 patch is read-only research, not an implementation payload.

| Member | Required proof |
| --- | --- |
| Register write | A panning-only change requests the required refresh. |
| Pixel displacement | Selected original graphics path reflects the register value. |
| Zero pan | Existing output stays identical. |
| Split region | Confirm the original split/panning compatibility contract and test it. |
| Dirty/full | Incremental and complete rendering agree after pan changes. |

Use product tests only. Each source hunk needs a defect-specific reason and
original-code comparison. Deliver both EXEs, focused proof, serial dual-width
full suites, protected-path and governance checks, then commit/push and review
the actual commit before closure. S5-S7 are not part of S4.

## Initial evidence

The selected VGA attribute-controller case 0x13 stores the register without
refresh invalidation. A product probe invokes the original vga_ac_outb twice
through its index/data sequence: after changing pan from zero to one, the
register is updated but the dirty-total refresh assertion fails on x64.
Repeating the same value is also checked to avoid needless invalidation.
The first local repair adds the missing refresh request on changed values;
pixel shifting and split compatibility are still pending, not claimed fixed.

The reference patch expands several complete painter loops and adds row
scratch copies. Those implementations are not imported. Selected original
nt_ega painters and nt_munge conversion will be inspected for the narrowest
correct integration, including source lookahead bounds. The alternative EGA
port source stores this register only under NON_PROD; its build applicability
must be accounted for separately from the selected VGA path.

## Pixel-path implementation checkpoint

The register refresh probe passes on x64. A second probe comparing a one-pel
render with pixels 1..8 of the zero-pan render failed before the pixel change.
The current local change retains the three standard EGA painter loops and
their lookup tables. A four-plane byte shift supplies their existing conversion;
the medium/high converter receives the row's effective pan explicitly. The
register owner supplies the pan, including the existing split compatibility bit.
No frame scratch buffer, replacement painter or shared-corpus change is added.

The x64 VGA smoke now passes 384 pixel cases: three standard painters, two
display-bank sizes, ordinary/end-of-bank source, pan 0..7, split compatibility
off/on and rows above/below the split. The next byte wraps at the display-bank
boundary. Zero-pan expected pixels come from the retained painter and are
shifted by the test, not by a duplicate bitplane implementation.

This is not S4 completion: sparse dirty dependencies on the following plane
byte, register changes to split compatibility, the alternate EGA source's
applicability, x86 and full regression remain pending. No package executable
has been refreshed or implementation P committed at this checkpoint.

The subsequent sparse test reproduced a mismatch at route 0, stride 80,
origin residue 0, changed address 80: panning makes that next-row byte affect
the previous row's last pixels. The repair marks the preceding existing dirty
group before the four EGA update entries search/coalesce their original records.
Ascending traversal avoids transitive propagation; the first bank group also
marks the final group. It does not replace incremental painting with full redraw.
The doubled 1,728-configuration update matrix passes on x64 with zero/one pel.

A separate register probe reproduced missing invalidation when AC mode-control
bit 5 changes. A changed-bit refresh guard fixes it without altering the other
mode-control handling. This guard and predecessor dirty dependency are required
integration details of S4, not claims of verbatim upstream fixes.

## Executor source and scope review

The selected adapter is fixed VGA and screen scale is fixed 2 in Compat's
existing config_inquire. Original gvi_init consequently selects vga_init;
nt_graph selects std_colour_paint_funcs. The preserved EGA-only register
implementation and big/huge painters are not current product paths and are
not changed or claimed repaired. The admitted planar EGA modes on VGA use
the three corrected standard painters. Packed-pixel VGA and text panning
are not part of the reference EGA graphics repair.

The six production paths each have one necessity: egaports declares the
register query; vga_prts exposes existing state and refreshes on its changes;
nt_graph declares the existing converter's extra pan argument and shared
four-plane shift; nt_munge shifts before existing lookup operations; nt_ega
passes each row's effective pan; gfx_updt includes the preceding dirty group.
No existing lookup kernel, rectangle coalescer or painter lifecycle is removed.
The product test is the only changed test path. Lib/Common and shared tests
remain byte-unchanged against T60 start 549ed44.

git diff --numstat against S3 reports production +74/-5 across six files;
test +92/-1, combined net +160. Original-OpenNT comparisons ignoring line
endings report egaports +2/-0, gfx_updt +189/-145, vga_prts +14/-0,
nt_graph +9/-2, nt_ega +43/-15 and nt_munge +29/-1. These cumulative figures
include pre-S4 portability and correctness diffs; minimizing those diffs,
not reducing source line count, governs this repair.

## Executor delivery

Both package builds succeeded. Serial full CTest passed 97/97 on x64
(81.23 s) and 97/97 on x86 (70.77 s); the focused VGA smoke passes at both
widths. Documentation governance and whitespace checks pass. This proves the
listed register/pixel/update contracts, not manual game/Windows acceptance.

Fixed x86 SHA256:
4E2D0A60F0B3CF91E27FCA4199F7ABBE903D0EBCF9E15A4F3913D1312346EEC9.
Fixed x64 SHA256:
BB073D36D1AFF5F1C4C6E04402A0695BBD9664DAF82D3203BC072FF346ACBE4D.
The preceding checkpoint's pending build items are superseded by this delivery;
actual-commit coordinator review remains required before S4 closes.
