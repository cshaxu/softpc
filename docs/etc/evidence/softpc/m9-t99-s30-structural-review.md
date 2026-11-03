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
| `base/ccpu386/c_tlb.c` | Original FAST_TLB frame exists; a further FAST_TLB frame covers local snapshot state. |
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
