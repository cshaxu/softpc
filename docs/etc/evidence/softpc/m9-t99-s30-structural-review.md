# M9 T99 S30: mirror condition structure review

The comparison uses the fixed XP SP1 source at
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`; OpenNT is read-only diagnostic
reference. It does not compare inside a current arm wholly replaced by
`#error`. Repeated condition names can fool sequence-based alignment; candidate
pairings were therefore checked using source anchors and recorded below.

## Corrected original structure

- `base/system/rom.c`, `rom_init`: the original outer expression has an
  `#else` containing NTVDM/X86GFX code. The current source had dropped that
  sibling. Restore its original `#else` and replace its unsupported body
  with a single error; the existing post-frame `#line 415` remains. Actual
  x86 and x64 preprocessing is byte-identical for selected paths against
  S29, and forcing NTVDM plus X86GFX reaches the intended error on each.

## Confirmed local differences

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
Additional locations are recorded below.

## Additional candidate file groups

The initial post-ROM tree comparison flagged 20 files; after the TLB
correction the refreshed provisional list flags 19. The entries below record
observed origins; they do not yet establish that each additional guard is
indispensable or that the tree matcher cannot have missed an occurrence.

| Current path | Locations / review lead |
| --- | --- |
| `base/bios/reset.c` | Standalone floppy topology and omitted DOS mouse-driver calls; compare the two altered NTVDM predicates with XP. |
| `base/ccpu386/c_tlb.c` | Original FAST_TLB frame exists; two redundant local snapshot guards have been removed, preserving copies. |
| `base/inc/cpu_vid.h`, `base/inc/gmi.h`, `base/inc/egacpu.h` | C-VID generated function signatures and globals carrier; verify existing ABI divergence and any extra unsupported-mode guards. |
| `base/keymouse/mouse_io.c`, `base/keymouse/ppi.c`, `base/video/video.c` | Five extra NEC_98 error siblings absent from XP were removed after supported-path and forced-NEC probes; PPI standalone and snapshot guards remain. |
| `base/keymouse/mouse.c` | The apparent missing PROD frame is present; confirm the full file has no second alignment discrepancy. |
| `base/system/ica.c` | Local interrupt-line and snapshot handling introduces conditions; assess each against its accepted contract. |
| `base/video/gfx_updt.c` | Local CPU_40_STYLE guard protects accepted CHAIN2 dirty-marker fix. |
| `base/video/vga_mode.c` | The original V7 conditions at XP 510 and 554 remain, but the local condition in the chain4 expression adds a distinct frame. |
| `host/inc/cfpu_def.h`, `host/inc/host_cpu.h` | Little-endian and selected CCPU ABI guards; distinguish extra fail-fast branches from original conditions. |
| `host/src/nt_cga.c`, `host/src/nt_graph.c` | Standalone rendering and V7 fields / sizing; assess each local condition against approved port and display behavior. |

The six added `LITTLEND` conditions in `cfpu_def.h` retain explicit
rejection of unsupported configurations. Five NEC_98 error siblings absent
from XP were later proven redundant in their actual translation units and
removed; see the guard-probe record below.

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
[context review](m9-t99-s30-context-review.tsv).

## Source-anchored local differences identified so far

The following is a location-level record. Line numbers refer to the
first S30 correction commit; the later TLB simplification removes two
snapshot guards. Retained conditions preserve the selected product ABI,
device behavior or explicit rejection of unsupported configurations.

| Current file and location | Local structural difference and evidence |
| --- | --- |
| `base/bios/reset.c:407,633,745` | Standalone terms select attached-FDC topology and reject an omitted DOS mouse-driver path; compare XP's NTVDM arms. |
| `base/ccpu386/c_main.c:34,755,4497,4664,4683,4699` | Six predicates extend XP NTVDM with CCPU TLS simulation-stack selection. Each original group was paired; nested original code in the error arm is intentionally excluded. |
| `base/ccpu386/c_tlb.c:151,188` | The original FAST_TLB declaration frame is present at current 70 / XP 69; the two always-true local snapshot guards were removed. |
| `base/inc/cpu_vid.h:88,115`; `base/inc/gmi.h:19` | CPU_40_STYLE and C_VID guards around adapted generated-function signatures; local ABI divergence explicitly recorded in source. |
| `base/inc/egacpu.h:99,120,252` | C_VID forward declaration and generated globals carrier retain required ABI adaptation; the extra error sibling absent from XP was removed after testing all 50 actual x86/x64 consumers with C_VID forced off. |
| `base/inc/ica.h:91` | CPU_40_STYLE nested guard separates legacy CPU_HW_INT macro 0 from CCPU's enum value 2; S29 records test consumers. |
| `base/inc/ios.h:23` | Standalone term extends the XP I/O-space selection; existing S20 records I/O sizing consumers. |
| `base/keymouse/mouse_io.c:2419`; `base/keymouse/ppi.c:181,205`; `base/video/video.c:328,2967` | The five extra NEC_98 error siblings absent from XP were removed after selected-path and forced-configuration probes; original conditions remain. |
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

The post-TLB initial matcher found 61 entries in 19 files. The corrected
arm-aware matcher found three further PPI entries. The resulting 64 entries
are overlapping matcher results, not a count of defects. The low-context
review reflects its pre-TLB baseline.

During the paired `c_main.c` check, a diagnostic listing inadvertently
enumerated one XP `#ifndef PROD` child inside an arm whose current body is
a single `#error`. That child was not used to classify or alter the excluded
arm. Future checks must stop at the error boundary as the S30 packet requires.

The initial matcher incorrectly stopped on a group when current added an
error-only sibling and XP had no sibling. The corrected matcher continues
through the matched selected arm and found three previously hidden candidate
entries in `base/keymouse/ppi.c`: current 146 and 169, paired with XP 170.
They are recorded in the [candidate dispositions](m9-t99-s30-candidate-disposition.tsv):
standalone rejection of the obsolete speaker path, and the existing
standalone extension of the PPI host-state delivery predicate. The refreshed
matcher reports 64 entries in 19 files; entries overlap, and this is not a
number of defects. The matched selected arms were then compared rather than discarded.

## Final comparison and disposition

The arm-aware matcher examined all 471 manifest text files, matched 3,613
conditional groups and stopped at 2,392 matched `#error` arms. It reported
64 overlapping candidate entries in 19 files. Each is categorized in the
[candidate disposition](m9-t99-s30-candidate-disposition.tsv), including
the two manually identified local conditions missed by the first alignment.
Matched repeated-condition checks are recorded separately: 11 changed-body
contexts, 24 low-context pairings, 59 middle-context pairings and 1,376
high-context pairings. Source or original-line anchors were used where
available; ten high-context cases without automatic anchors were manually
checked. See the [changed-body review](m9-t99-s30-context-review.tsv),
[low-context review](m9-t99-s30-repeated-review.tsv),
[middle-context review](m9-t99-s30-repeated-middle-review.tsv), and
[high-context review](m9-t99-s30-repeated-high-review.tsv).

The matcher is an audit aid, not a formal proof of every possible textual
alignment; repeated guards and local code movement require source judgment.
The original compile-frame comparison intentionally excludes contents wholly
replaced by `#error`. Existing local guards and changes listed above retain
ABI, snapshot, device, video or unsupported-configuration contracts.

Relative to S29, final mirror production changes are six files: `rom.c`
+2/-0, `c_tlb.c` +1/-4, `egacpu.h` +0/-2, `mouse_io.c` +0/-2,
`ppi.c` +0/-4, and `video.c` +0/-4: total +3/-16 (net -13). There are
no test-source changes. Earlier deliveries passed 194/194 per architecture;
final-source full qualification is recorded below.
Neither user INI, media nor snapshots were modified.

## Redundant NEC_98 sibling correction

Five error-only siblings absent from XP were removed from `mouse_io.c`,
`ppi.c` and `video.c`, preserving each original `#ifndef NEC_98` and its
post-boundary line control. The three actual translation units directly
include `ios.h` or `sas.h`, both of which independently reject NEC_98.
On each architecture, preprocessed selected output of each scratch candidate
matched its original byte-for-byte. With NEC_98 forced and both optional port
counts predefined, each actual compilation context still failed at an
independent included-header guard. This proves the observed contexts, not
all hypothetical future include graphs. Final-source dual-architecture builds and full qualification passed: 186
background and 8 native tests per architecture, with no skips.

## Redundant C_VID sibling correction

The XP `C_VID` group in `base/inc/egacpu.h` has no else. Its extra local
error sibling was removed while preserving the original guard. An overlay
probe exercised all 50 actual x86/x64 compilation contexts that include the
header; with `C_VID` forced off, all 50 still reached independent explicit
`#error` guards. This evidence covers the current consumers, not hypothetical
future include graphs. The final matcher reports 58 overlapping candidate
entries in 17 files, compared with 64 entries in 19 files before removing
the six redundant siblings. The matcher is not a proof of perfect matching.
Final-source dual-architecture builds and full qualification passed: 186
background and 8 native tests per architecture, with no skips.

The first background qualification attempt found a manifest registration
error: the new `egacpu.h` digest initially used raw CRLF bytes, while the
mirror gate hashes LF-normalized text. The manifest was corrected; that failed
run is not counted as a full pass. Build timestamps also showed that the
final header edit had not yet been rebuilt at that point.

## Final qualification

Both final-source cached GCC builds and the XP mirror and documentation
gates passed. The full test suite passed on each architecture: 186
background tests and 8 native desktop/audio tests, none skipped. Compiler
warnings were confined to the preserved mirror sources (1,307 x64 and
1,257 x86 warning lines); no non-mirror warning was found. Published SHA-256:

- `softpc32.exe`: `2a847dfdc1ea2a3c44bc657c29a9d169afe757bdab25a02b77e534c038b52ab9`
- `softpc64.exe`: `0b5f31f36709f5ee70a2bc923ba937242519592e3fb28293f3609c25f7b1a4db`

The [final matcher candidates](m9-t99-s30-final-structural-candidates.tsv)
are 58 entries in 17 files, not 58 confirmed defects. The earlier disposition
and anchored reviews record the comparisons; automated matching of repeated
conditions remains a limitation and does not prove complete correspondence.
