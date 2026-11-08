# T99 S39 retained mirror difference review

## Scope and proof boundary

Source baseline: SoftPC `19a6878290640a4ec304488eb2ac335cdd849f6a`.
Reference: XP SP1 `a2f6d7c93aa4f11efc51147566d5961c59a8ef89`, same-relative
files below `Source/XPSP1/NT/base/mvdm/softpc.new/`. No OpenNT source was used.
S38 is accepted and closed; this investigation changes documentation only.

**Already-#error branch bodies are excluded by owner instruction.** Their
removed contents and exclusion decisions are not reviewed. They are not
counted as semantic review achievements. Original retained conditional
structure remains governed by the existing source policy.

The finite universe is all 475 tracked files beneath the current mirror,
including headers and the build-consumed c-files list. Each has a same-relative
XP file and a row in [the inventory](m9-t99-s39-mirror-inventory.tsv).
Hashes cover LF-normalized bytes, not physical CRLF files. Binary files are
compared unchanged. No source is generated for the build.

| Comparison disposition | Files | Meaning |
| --- | ---: | --- |
| IDENTICAL | 249 | LF-normalized identity; no local difference to justify. |
| EXCLUSION_LINE_OR_LEXICAL_ONLY | 89 | Retained C tokens and conditional structure match after masking already-#error bodies and ignoring comments/#line. No retained semantic difference identified by this comparison. |
| ABI_RULE_PATTERN_ONLY | 27 | Generated C-VID changes reverse exactly through the enumerated ABI transformations below. |
| REVIEW | 110 | Residual mixed changes: family/caller review below; this label does not certify every hunk necessary. |

The inventory converges at 475/475 classified files. This is **not** a claim
that every retained hunk in the 110 mixed files has an independent deletion
counterfactual or fresh runtime proof. In particular, whole audio, timer,
keyboard, mouse and snapshot adaptations cannot be certified redundant or
minimal from file-level comparison. They remain retained, not safe-to-delete.
Raw whole-file addition/deletion totals include excluded content and are not
used as the scope or claimed savings for this audit.

## Reproducible comparison

Enumerate `git ls-files src/app-softpc/softpc.new`; match each path against
`xp-sp1-current-manifest.tsv` and retrieve the fixed XP blob with `git cat-file`.
Normalize CRLF to LF only for comparison. For changed C files, tokenize while
preserving string/character literals; ignore comments and #line. Compare
conditional trees, masking the corresponding original body only where the
local body is a single #error. Unmatched structure remains REVIEW, never an
assumed match. Compare generated C-VID files separately by reversing only:

- GDP slot/address accessor substitutions to their original address expressions;
- local rule allocations from calloc(N, sizeof(IUH)) to original malloc(4*N);
- native-width shift operands back to their original expression;
- the added accessor include and lexical comments/whitespace.

This uses the existing pattern descriptions in `tools/Audit-MirrorDiff.ps1`;
it is an audit transformation only. The 27 exact matches do not cover the
seven other sevid files containing accepted behavioral repairs. The latter
remain in REVIEW. The inventory records both comparison hashes so the frozen
inputs can be reconstructed without retaining copied source or raw traces.

## Necessity findings

| Retained family | Actual owner/consumer and reason | Disposition |
| --- | --- | --- |
| C-VID GDP/native local slots | Compat gdp_state/gdp_rule_access bridge fixed guest fields to native pointer storage. Original four-byte local allocation and raw GDP pointer arithmetic cannot simply be restored on x64. | Keep ABI mapping. Do not replace it with a second generated-code or global-64-bit guest representation. |
| Seven behavioral sevid exceptions | Selected markers, CHAIN2 layout/reads, Unchained DWORD spacing and bounded Copy fill share the original rule flow. S36/S38 retain focused before/after evidence. | Keep accepted fixes; zero-offset helper calls also preserve the uniform address contract, despite yielding the same base pointer. |
| CCPU state and safe points | Compat ccpu/archive.c calls mirror capture/restore functions; executor/debug controls depend on instruction and access boundaries. Operand width and stack-address width are distinct. | Keep snapshot transport, safe points and corrected CALL/RET/IRET stack updates. Similar-looking calls at different execution boundaries are not duplicate paths. |
| Device and video snapshot state | Compat devices/archive.c invokes PIC/PIT/DMA/CMOS/FDC/PPI/keyboard/mouse/video/serial/parallel capture and restore. State includes pointers/callbacks requiring explicit transport. | Do not replace field transport with native struct memcpy or move private device state into a second owner. |
| BIOS/device correctness | DMA auto-init, INT13 capacity split, BIOS PG_U and fallback width, slave PIC EOI, VGA cursor and CHAIN2 fixes have dedicated existing evidence. | Keep. No reduction justified by returning to XP defects. |
| Host graphics | Original painters still own pixels; retained DIB/stride/bounds and source-layout adaptations connect them to current presentation. | Keep the single painter path. Removing consumer layout corrections without equivalent producer semantics is unsafe. |
| Host audio | Current endpoint glue substitutes host capabilities, while DSP/device timing remains original device logic. | Do not equate the whole adapter with redundant reimplementation; a larger redesign needs separate behavior proof. |
| Build and declarations | Root CMakeLists.txt reads base/ccpu386/c-files; source boundary gate also reads it. Native declarations and calling widths remove implicit-int/pointer truncation. | Keep live list omissions and required ABI declarations. The list is not unused metadata. |

Source cross-check disproves one tempting cleanup: XP nt_sbdsp.c already has
PreviousWaveOutPos/StartingWaveOutPos progress handling, a ten-check limit,
SBBlockSize/8 adjustment and the 0x1800 threshold (around original lines
2766, 2786 and 2894). Those policies are not newly invented SoftPC guest
special cases. Removing them requires more than disliking magic numbers.
Likewise the emulated FM probe/status path is not the unused Lib FM API;
absence of a synthesizer does not authorize deleting guest-visible status.

The added-condition scan finds SOFTPC_STANDALONE, SOFTPC_CCPU_TLS_SIMSTACK
and CVIDC_RULE_WORD selections. Thus an unconditional statement that the
mirror contains no local special selection would be false. These are fixed
build/host/ABI choices, not evidence of guest-version detection. The retained
added-condition scan did not identify a new Win3.1/Win95/program-name/address
workaround; this is a bounded scan, not proof of the absence of all indirect
product assumptions.

## Concrete minimal cleanup proposal

Do not change production in S39's research delivery. Three local wrappers can
be removed while keeping their bodies, because all current applicable product
and test builds select SOFTPC_STANDALONE. Unlike original XP conditionals,
these wrappers were added locally:

| Location | Preserve | Proposed removal |
| --- | --- | --- |
| host/src/nt_cga.c:22 | #undef BIGWIN and standard CGA painter selection | Added SOFTPC_STANDALONE #ifdef/#endif only. |
| base/system/ica.c:1242 | Master INTR withdrawal and host_clear_hw_int | Added SOFTPC_STANDALONE #ifdef/#endif only. |
| base/keymouse/ppi.c:225 | Snapshot restore HostPpiState(ppi_register), diagnostic mapping | Added SOFTPC_STANDALONE #if/#endif only. |

Expected production: **+0/-6, net -6 lines in three files**. This is a small
readability cleanup, not a performance improvement or major mirror reduction.
Do not remove #undef BIGWIN globally: other original graphics paths use the
host scaling selection. Do not erase original NTVDM alternatives to remove
local OR terms, define NTVDM globally, or reopen any #error body.

Before accepting such a patch, compare x86/x64 preprocessed tokens for each
applicable translation unit, retaining correct #line diagnostics; audit all
test compile definitions. Compile both artifacts with caches and compare
substantive code. Run affected regressions if compiled behavior differs.
Update mirror registrations to retain the real fix and remove only the local
wrapper. Original branch preservation is not permission to preserve redundant
locally introduced wrappers forever.

A separate documentation observation: the added comment in host/inc/nt_event.h
still calls nt_event.c the provider of PointerAttachedWindowed, but the current
src scan finds only the declaration. This is not a runtime defect (an unused
extern does not create storage). Do not invent a replacement provider. Correct
its provenance wording or prove the declaration itself unnecessary before
proposing deletion; no savings are counted for this unproven candidate.

## Verification and remaining limits

Production, test and assets changes: **0/0/net 0**. No rebuild or runtime tests
were run for this read-only investigation. S38's 390/390 qualification remains
historical evidence, not fresh proof for every possible simplification.
The relevant prior repair evidence is linked below; this review does not
restate those tests as exhaustive DOS/Win3.1/Win95 validation.

- [S36 CHAIN2 reads](../../../history/M9-T99-S36-chain2-dword-read.md).
- [S37 device audit](m9-t99-s37-original-device-audit.md).
- [S38 GDP/Unchained/bulk repairs](m9-t99-s38-gdp-subfields.md).

Recommendation: keep accepted fixes and ABI/snapshot ownership. If pursuing a
small cleanup, implement only the three wrappers after equivalence checks.
There is no established large safe rollback. The mixed-file residual is
explicitly unresolved at individual-hunk minimality level; do not turn this
report into a blanket assertion that all current mirror differences are either
necessary or fully audited. No implementation or S39 closure is implied.
