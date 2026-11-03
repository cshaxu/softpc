# M9 T99 S30: mirror condition structure review — in progress

The comparison uses the fixed XP SP1 source at
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`; OpenNT is read-only diagnostic
reference. It does not compare inside a current arm wholly replaced by
`#error`. This is not yet a complete exception inventory: repeated condition
names can fool sequence-based alignment and each candidate needs source
anchors and a supported-context disposition.

## Corrected original structure

- `base/system/rom.c`, `rom_init`: the original outer expression has an
  `#else` containing NTVDM/X86GFX code. The current source had dropped that
  sibling. Restore its original `#else` and replace its unsupported body
  with a single error; the existing post-frame `#line 415` remains. Actual
  x86 and x64 preprocessing is byte-identical for selected paths against
  S29, and forcing NTVDM plus X86GFX reaches the intended error on each.

## Confirmed local differences requiring further disposition

- `base/ccpu386/c_main.c`: six XP `#ifdef NTVDM` frames use the existing
  local `NTVDM || SOFTPC_CCPU_TLS_SIMSTACK` selection to provide the CCPU
  TLS simulation stack. See the S20 original-condition ledger.
- `base/inc/ios.h`: XP's NTVDM/GISP/SFELLOW OR has the existing standalone
  term, needed by the selected I/O dispatch sizing; S20 recorded actual
  contexts.
- `host/inc/insignia.h`: local `CVIDC_RULE_WORD` chooses a pointer-width
  generated C-video carrier while the CPU-side carrier remains 32-bit.
  See T98 C-VID ABI review; the distinction cannot be erased without
  reconsidering the generated rule ABI.
- `base/inc/ica.h`: the nested `#ifndef CPU_40_STYLE` prevents the old
  `CPU_HW_INT` macro (0) from colliding with the CCPU protocol enum (2).
  It is selected by four existing unit-test targets per architecture; see
  the S29 context ledger.
- `base/video/vga_mode.c`: the additional local V7 conditional inside the
  chain4 expression is part of the accepted V7 scanout correction.

The comparison also falsely paired the original `FAST_TLB` condition in
`base/ccpu386/c_tlb.c` with a later snapshot condition: the original is
present at current line 70 and XP line 69. Likewise, two `V7VGA` groups in
`vga_mode.c` were paired to the wrong occurrences. Such initial candidates
are not counted as confirmed defects. The `mouse.c` XP `#ifndef PROD` at
line 442 is present at current line 377; it too is an alignment artifact.
More paths remain under review.

## Additional candidate file groups (review in progress)

The tree comparison currently flags 20 files. The entries below record
observed origins; they do not yet establish that each additional guard is
indispensable or that the tree matcher cannot have missed an occurrence.

| Current path | Locations / review lead |
| --- | --- |
| `base/bios/reset.c` | Standalone floppy topology and omitted DOS mouse-driver calls; compare the two altered NTVDM predicates with XP. |
| `base/ccpu386/c_tlb.c` | Original FAST_TLB frame exists; two redundant local snapshot guards have been removed, preserving copies. |
| `base/inc/cpu_vid.h`, `base/inc/gmi.h`, `base/inc/egacpu.h` | C-VID generated function signatures and globals carrier; verify existing ABI divergence and any extra unsupported-mode guards. |
| `base/keymouse/mouse_io.c`, `base/keymouse/ppi.c`, `base/video/video.c` | Some current NEC_98 frames have an extra error sibling; XP corresponding frames lack that sibling. Assess the fail-fast requirement before deciding whether to preserve or remove it. PPI also has standalone and snapshot guards. |
| `base/keymouse/mouse.c` | The apparent missing PROD frame is present; confirm the full file has no second alignment discrepancy. |
| `base/system/ica.c` | Local interrupt-line and snapshot handling introduces conditions; assess each against its accepted contract. |
| `base/video/gfx_updt.c` | Local CPU_40_STYLE guard protects accepted CHAIN2 dirty-marker fix. |
| `base/video/vga_mode.c` | The original V7 conditions at XP 510 and 554 remain, but the local condition in the chain4 expression adds a distinct frame. |
| `host/inc/cfpu_def.h`, `host/inc/host_cpu.h` | Little-endian and selected CCPU ABI guards; distinguish extra fail-fast branches from original conditions. |
| `host/src/nt_cga.c`, `host/src/nt_graph.c` | Standalone rendering and V7 fields / sizing; assess each local condition against approved port and display behavior. |

The six added `LITTLEND` conditions in `cfpu_def.h`, the added NEC_98 error
siblings and other local guards must not be removed simply because XP lacks
them: whether they are necessary to enforce the supported configuration
remains an explicit review question.

## First correction qualification

Dual architecture builds and mirror/documentation gates passed. Both
architectures passed 186 non-native and eight native desktop/audio tests
without skips. The production change is `rom.c` +2/-0; no test source changed.
Published EXE SHA-256:

- softpc32.exe: `67dc53e3e4a202002ac127077634f38a5fbdc918f7f4cecfb589856309d8a4d8`
- softpc64.exe: `62972f736ed1faa166f4035ccae008560c9c96d5e95eca862ce54753d18afd11`

The [post-correction candidate list](m9-t99-s30-structural-candidates.tsv)
is a provisional structural-matcher output, not a list of confirmed defects
or complete exception dispositions. More source-anchor review remains.

## Matching caution: retained-arm context

A second heuristic compared non-directive text inside paired arms and flagged
11 low-similarity matches in eight files. These are candidates, not defects.
Spot checks found several legitimate reasons: C-VID ABI signature changes
(`cpu_vid.h`, `host.h`), existing XP-excluded bodies (`sas.h`, `ica.c`),
accepted dirty-region changes (`gfx_updt.c`), and a genuine alignment error
in `vga_mode.c` where the local V7 condition was paired with XP's later V7
condition. This check demonstrates that a nominally matched group cannot be
assumed to have the same source context. The local V7 condition is included
in the exception review above. The 11 low-similarity matches were individually dispositioned in the
[context review](m9-t99-s30-context-review.tsv); wider candidate and
necessary-exception review remains open.

## Source-anchored local differences identified so far

The following is a location-level record, not an assertion that these are
the entire remaining set. Line numbers refer to the S30 correction commit.
The conditions are retained pending the full minimality review.

| Current file and location | Local structural difference and evidence |
| --- | --- |
| `base/bios/reset.c:407,633,745` | Standalone terms select attached-FDC topology and reject an omitted DOS mouse-driver path; compare XP's NTVDM arms. |
| `base/ccpu386/c_main.c:34,755,4497,4664,4683,4699` | Six predicates extend XP NTVDM with CCPU TLS simulation-stack selection. Each original group was paired; nested original code in the error arm is intentionally excluded. |
| `base/ccpu386/c_tlb.c:151,188` | The original FAST_TLB declaration frame is present at current 70 / XP 69; the two always-true local snapshot guards were removed. |
| `base/inc/cpu_vid.h:88,115`; `base/inc/gmi.h:19` | CPU_40_STYLE and C_VID guards around adapted generated-function signatures; local ABI divergence explicitly recorded in source. |
| `base/inc/egacpu.h:99,120,252` | C_VID forward declaration and generated globals carrier, plus an error sibling for unsupported non-C-VID; XP's C_VID frame at 442 has no else. |
| `base/inc/ica.h:91` | CPU_40_STYLE nested guard separates legacy CPU_HW_INT macro 0 from CCPU's enum value 2; S29 records test consumers. |
| `base/inc/ios.h:23` | Standalone term extends the XP I/O-space selection; existing S20 records I/O sizing consumers. |
| `base/keymouse/mouse_io.c:2419`; `base/keymouse/ppi.c:181,205`; `base/video/video.c:328,2967` | Explicit error siblings for NEC_98, absent from the corresponding XP frames. They reject unsupported configurations; whether they are redundant with other guards is not yet proven. |
| `base/keymouse/ppi.c:70,219,236` | Standalone rejection and snapshot PPI state selection, absent from original product code. |
| `base/system/ica.c:1242,1684,2017,2037,2082,2095` | Standalone PIC line handling, CCPU IRET handling and snapshot callback/hook state paths. |
| `base/video/gfx_updt.c:3891`; `base/video/vga_mode.c:462` | CPU_40_STYLE and V7VGA guards around accepted CHAIN2 dirty / packed scanout fixes. |
| `host/inc/cfpu_def.h:21,32,43,55,66,83`; `host/inc/host_cpu.h:37` | Little-endian guards around local host layouts; original layouts differ and supported x86/x64 bit ordering must be preserved. |
| `host/inc/host_cpu.h:275`; `host/inc/insignia.h:77` | CCPU vector typing and generated C-video carrier ABI conditions; XP common vectors/carrier were unconditionally declared. |
| `host/src/nt_cga.c:22`; `host/src/nt_graph.c:83,782,1190,1617,1647,1648,2213` | Standalone graphics integration, V7 DAC declaration and packed-scanout width; local product port paths and rejected non-standalone regions. |

`base/keymouse/mouse.c`'s PROD frame, `base/ccpu386/c_tlb.c:70`,
`base/video/vga_mode.c:488,532` and `host/inc/host_cpu.h:295` are matching
original structures whose initial pairings were wrong. The 11 low-context
pairings have individual records in the context review. These findings do
not dispose every possible repeated-condition pairing in the corpus.

A recursive check of current-only candidate subtrees, stopping at single-error
arms, found one additional nested local guard: `nt_graph.c:1648` V7VGA under
the already recorded standalone packed-scanout width condition. The guard is
now listed explicitly. This is a candidate-sweep check, not a license to
inspect original code hidden behind error replacements.

## TLB snapshot condition simplification

`base/ccpu386/c_tlb.c` defines `FAST_TLB` unconditionally before its includes;
no `#undef FAST_TLB` exists in the repository. Its two additional local
snapshot-save / snapshot-restore guards were therefore always true in current
compilation and had no XP counterpart. They were removed while preserving all
four copies and the original XP FAST_TLB frame. `#line 124` resets the original
source position following the local snapshot functions. Actual x86/x64
preprocessing of the selected output was byte-identical to `fde49418`; build
and full test qualification passed: both architectures completed 186
non-native and eight serial native tests without skips. Snapshot checkpoint
regression is included. Published binary SHA-256 values are:

- softpc32.exe: `ae26ad03a004098d4291b79ce59831ccf734eb20ddfe6a08127ec59a8c1755cc`
- softpc64.exe: `db2122acb5e838cb0a1f91f910c72aca6da002caf192d04a5608865c7329c3e8`
