# EGA dirty source-address preservation

T60 S3 implements the [admitted plan](../history/M9-T60-reference-repairs-proposal.md)
against 06b267e. Original request and protected shared-corpus boundary remain
those recorded there; CURRENT alone owns active state.

## Design and finite ledger

Retain original dirty marks, dirty rectangles and selected host painters.
The dirty bitmap has one mark per four source offsets; reconstructing a source
address from its index alone discards low bits. Preserve the exact source
origin explicitly within the existing private call chain, not a global mutable
alignment variable. The NTVDMx64 ega_align patch is a research lead only.

| Member | Required disposition |
| --- | --- |
| Ordinary EGA, all four start-address residues | Incremental output equals complete rendering; aligned case unchanged. |
| Split upper and lower region | Upper retains exact origin; lower starts at zero independently. |
| Wrapped region and boundary row | Post-wrap source origin and destination row remain exact. |
| Byte, aligned and V7 dirty searches | Account for every caller and its address/stride units before changing shared mechanics. |
| Dirty mark clearing | Preserve marks needed by another split/wrapped region; no lost terminal partial group. |

Tests belong only to product test/unit. Use existing renderer/plane fixtures,
not a second production renderer. Verify focused red/green cases, both package
widths and serial full suites, protected-path zero diff, whitespace and
documentation governance. No external source payload, guest media, new trace,
Lib/Common or public ABI change is admitted.

One complete executor P is built/tested/committed/pushed before switching to
coordinator actual-diff review. Closure records source divergence, same-class
dispositions, exact test coverage and production/test added/removed/net lines.
S4 horizontal panning and later T60 stages remain separate.

## Investigation checkpoint

The product VGA smoke now compares the actual EGA complete and dirty paths
using a patterned original plane buffer and nt_ega_hi_graph_std. On unchanged
production code, start offset zero agrees and start offset one fails with
`EGA dirty/full mismatch at start offset 1` (x64 focused CTest). This is pixel
evidence, not merely arithmetic inspection. The fixture uses egacpu.h's selected
Video vector; the unrelated direct vglob setter symbols are not its ABI.

The same-class scan found three shared search variants and separate EGA/VGA
paint-record consumers. Text records have their own consumer and must remain
unchanged. Search bounds and dirty clearing also use quarter offsets, so adding
only an alignment value at paint time is not yet sufficient proof for a final
partial group. Wrap additionally labels the post-boundary scan from ht1 even
though full output starts it at ht1+1. These remain active S3 verification work,
not a claim that the upstream patch alone closes the ledger.

That initial checkpoint preceded production edits. It was not a delivery or
closure; the observations drove the following bounded repair.

## Owner-directed minimum-diff review

Owner correction: "mvdm的代码修改要求是以和原始代码的diff最小为原则，
除非必要的修复；不是减少代码量". The review includes committed S1/S2,
not only the uncommitted S3 candidate. Added plus removed original lines,
preserved structure and necessity of each hunk are the review criteria;
negative net lines do not establish minimal divergence.

The uncommitted S3 rewrite was withdrawn. gfx_updt.c is again identical to
06b267e. The expanded regression fixture remains. Earlier dual-width 97/97
results and the currently modified EXEs belong to that withdrawn candidate,
not a verified minimal repair. They are not a release or closure proof.

Review uses both git diff 549ed44 HEAD -- src/mvdm and direct no-index
comparison with the selected original OpenNT files (ignoring line endings).

| Member | Review finding and next action |
| --- | --- |
| S1 c_intr.c | Four-line rejection guard only; original interrupt algorithm and branches retained. Keep the necessary PIC sentinel rejection. |
| S2 nt_ega.c | Three local pre-painter guard/clipping blocks; pixel kernels unchanged. Recheck preservation of original diagnostic blocks while retaining bounds proof. |
| S2 nt_vga.c | Bounds and pre-scaling checks are necessary; replacing original conditional clipping with min and replacing safe shifts with multiplication need not accompany the fix. Restore original forms where safe. The removed source_* block was an earlier local patch, not original OpenNT code; do not restore it merely to reduce the T60-only delta. |
| S3 gfx_updt.c | Rejected consolidation removed original search variants and wrap algorithms. Restore them, then repair address/boundary calculations locally; retained tests must prove the revised implementation. |

Do not alter Lib/Common or their shared tests. Do not rewrite published Git
history. Any corrective changes to S1/S2 travel in a new verified commit.
The remaining S3 ledger and later T60 stages are still required.

## Minimal-patch checkpoint

Restored the original conditional clipping style and post-validation shifts
in nt_vga.c; the overflow-safe comparisons and necessary source bounds remain.
The first local gfx_updt.c changes keep all original search and paint routines:
signed dirty-range subtraction, inclusive four-byte dirty-group extent, the
missing screen origin in the wrap-padding row, and the post-boundary row index.
This candidate is only +8/-6 in gfx_updt.c, not a completed S3 repair.

The focused x64 target builds. Its expanded VGA test still fails at the
visible full-refresh dirty-mark assertion (vga_frame_smoke.c line 138).
That failure is retained as the next investigation point; it does not justify
removing the assertion or reporting the withdrawn candidate's green results
as proof of this implementation. No S3 commit or push has occurred.

The diagnostic now identifies the first uncleared mark precisely: ordinary
EGA, stride 80, origin 1, address 320. A 320-byte range starting at 1 intersects
81 bitmap groups, not 80. The three existing contiguous full-refresh memset
counts now round the range including its starting residue; no clearing helper
or alternate path was added. After rebuilding, the same test passes that
check and fails on dirty/full pixel zero (7 versus 1) at origin 1. The remaining
address-reconstruction defect is therefore still reproducible, not masked.

The existing EGA/VGA paint_records helpers now receive an explicit private
alignment argument. All ordinary, split and wrapped callers supply their own
origin residue; the record layout, searches and rectangle merge algorithm are
unchanged. Rebuilt x64 proof advances past ordinary origin-one all-dirty
pixels, then identifies sparse address 80 (stride 80, origin 1) as a mismatch:
the group intersects the end of row zero, while the original quarter-stride
search assigns it to row one. This requires intersecting-row coverage, not
another paint-time alignment adjustment. S3 remains unverified and open.

The byte search now checks each row's intersecting dirty groups before
building the original dirty record. Aligned and V7 routines and rectangle
coalescing remain present. Paint-record clearing includes the same source
residue in start/end group calculations. Split full-refresh counts use the
unrounded stride; the original wrapped boundary row now clears its marks
after painting, because it never had a dirty record to clear them.

The latest focused x64 run passes stride-80 ordinary, split and wrap routes
for all four residues, including their sparse edge updates, then fails at
wrap+split full/oracle, residue zero. The original explicit ignore-wrapping
branch remains the next repair point. Other strides and VGA routes have not
yet been reached in this run; no broader pass is claimed.

The wrap+split placeholder now calls a private upper-region painter that
splits rows at the original display-memory limit. It checks dirty groups and
clears them only after all upper rows have been considered; lower-region
records are collected first so overlapping marks remain represented. No
other update entry is redirected through this helper. VGA aligned search
now checks start-address alignment as the EGA path already did.

The focused x64 test advances through all six stride-80 routes and residues.
It next fails ordinary EGA at stride 82, residue zero, pixel 3200: the original
V7 paired-row search paints beyond the four-row fixture. The diagnostic now
includes route and stride. Preserve the paired-row routine while correcting
its range and record boundaries; do not infer all-stride success from the
completed stride-80 portion. No S3 delivery or closure yet.

The V7 correction retains its paired-row search and first/last dirty-group
bounding, but intersects that source span with each actual row. This removes
the erroneous extra row and includes a group shared across two pairs. Its
private ABI is unchanged. The byte, aligned and V7 search entries, existing
record structure, original merge loops and all six update entries remain.

The focused x64 test now passes all 432 configurations (2.40 s). The current
gfx_updt.c delta is +133/-137, versus the withdrawn +108/-386 consolidation;
these counts document divergence, not an optimization objective. The x86
focused proof and final full-suite/artifact checks remain separate gates.

The revised focused x86 matrix passes (1.62 s). Both full builds have refreshed
the fixed EXEs, and x64 full CTest passes 97/97 (80.33 s). Final source review
identified a remaining coverage question: the fixture fixes pc_pix_height=1,
while original wrap entries use screen height differently from ordinary split.
Add the doubled-scanline case before declaring S3 complete. These results
prove the present fixture, not that untested geometry contract.

The preceding x86 full suite also passed 97/97 (69.18 s). The added
pc_pix_height=2 case then reproduced wrap full/oracle failure. Both original
wrap entries now convert screen height (and split position) to source rows,
as the ordinary split entry already does; no new state is introduced.
The expanded 864-configuration matrix passes x64 (3.03 s) and x86 (2.18 s).
Full builds/suites are being rerun for this final geometry correction, so the
previous 97/97 results are not yet its complete delivery evidence.

## Final source review

Every modified production hunk is local to gfx_updt.c or the owner-requested
S2 style correction in nt_vga.c. The dirty bitmap still owns four-byte marks;
records keep their original layout and quarter-offset plus adjustment fields.
No global alignment variable, new renderer, merged update entry, changed text
consumer or external source import remains. The aligned scan is unchanged.
The byte and V7 scan changes are required by the reproduced shared-group
failures; retaining their original disjoint-group arithmetic is not correct.
The wrap+split helper is limited to the original unimplemented branch.

Against 06b267e: gfx_updt.c +139/-143; nt_vga.c +11/-6;
vga_frame_smoke.c +128/-0. Production totals +150/-149; test +128/-0.
Against original OpenNT (ignoring line endings), gfx_updt.c is +165/-145
and nt_vga.c +45/-16, including earlier task changes. Counts are evidence of
source divergence, not a net-reduction goal. S1's four-line guard remains;
S2's required source/destination bounds remain, while avoidable conditional
and shift spelling changes have been restored toward the original.

Fixed x86: 3,489,174 bytes, SHA256
939169CE8DD97BAA43F39409449BCF1A73621BC6D6FDCBB8CEEFEB91B40FE957.
Fixed x64: 2,849,806 bytes, SHA256
897AB059D96F6223DE378B4A70894BC5E2908C92AB6314F3417C6DF4DB40A6F7.
Both builds succeeded. Shared corpora, App/VM/Compat, INI, media and ROMs have
zero task diff. Manual guest acceptance is not claimed.

Final serial full suites pass 97/97 at both widths (x86 54.81 s); the serial
command starts x86 only after successful x64 completion. An earlier pair of
overlapping successful runs was not used as the required serial proof.
Documentation governance and whitespace checks pass. S3 executor delivery is
ready; coordinator actual-commit review and closure remain separate.
