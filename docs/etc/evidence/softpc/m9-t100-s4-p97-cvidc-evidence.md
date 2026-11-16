# S4 P97: generated C-VID retention-evidence audit

## Frozen universe and method

Owner request: 接下来 34的cvidc文件 找出所有没有证据的diff点位。
Local source baseline: 7a0bcfde. Reference: fixed XP SP1
 a2f6d7c93aa4f11efc51147566d5961c59a8ef89, corresponding base/cvidc paths.
No OpenNT comparison. No source, test, artifact, snapshot or configuration changes.

Universe is sevid000.c-sevid030.c and sinit011.c-sinit013.c, original ordinals
283-316. All 34 contain real differences; none contains an #error/#line excluded
body. Current zero-context Git diff: **15,129 blocks, +20,871/-21,359**.
These are physical hunks, not 15,129 independent semantic decisions.
[Per-file counts and SHA256](m9-t100-s4-p97-files.tsv) freeze all 34 members.

Every current hunk was classified, rather than treating an old point ledger as
current source. Exact GDP substitution matching compares original offset,
indirection type and untouched surrounding tokens, ignoring whitespace only.
It does not normalize arithmetic, comments, branch conditions or control flow.
Allocation matching checks original byte count / 4 against current IUH count.
The other hunks were matched to original-position evidence records and their
named repair families. Coverage completion here means every current hunk has
a category and evidence disposition, not every instruction has a fresh dynamic
negative control or every guest configuration has been tested.

| Exclusive classification | Blocks | Evidence scope |
| --- | ---: | --- |
| Exact GDP-address adaptation | 13,873 | Shared current storage/ABI source proof; original offset and type mechanically agree. No per-site dynamic claim. |
| Native local allocation | 1,057 | All counts agree with original 32-bit slot count: 795 malloc, 262 calloc. Capacity proof differs from zero-initialization dataflow proof. |
| GDP header include | 34 | Direct declarations for used adaptations in each file. |
| Other or mixed semantic blocks | 165 | CHAIN2 layout/dirty/continuation/copy/read corrections and twelve CHAIN4 mode2 stores; historical bounded counterexamples, detailed below. |

No standalone whitespace-only current hunk was found. This does not imply
whitespace inside an otherwise semantic replacement has independent value.

## Missing product-necessity evidence: eight generated entries

The important gap is **production admission**, not whether native-width
adaptation would be necessary if these functions were called. All five port
entries and three old selectors remain absent from the production reference
closure. Their adaptations have no demonstrated current product consumer.

| File / current function line | Entry | Blocks | XP diff |
| --- | --- | ---: | ---: |
| sevid004.c:3432 | S_2233_EGAGCIndexOutb | 7 | +9/-9 |
| sevid004.c:3470 | S_2234_VGAGCIndexOutb | 15 | +20/-20 |
| sevid004.c:3546 | S_2235_VGAGCMaskRegOutb | 12 | +14/-14 |
| sevid004.c:3609 | S_2236_AdapCOutb | 11 | +29/-29 |
| sevid004.c:3672 | S_2237_VGAGCMaskFFRegOutb | 8 | +10/-10 |
| sinit013.c:1369 | S_0441_CisetWritePointers_0000004e | 52 | +70/-70 |
| sinit013.c:1783 | S_0442_CisetReadPointers_0000004f | 25 | +43/-43 |
| sinit013.c:1993 | S_0443_CisetMarkPointers_00000050 | 15 | +33/-33 |

Total **145 blocks, +228/-228**: sevid004 has 53 blocks +82/-82;
sinit013 has 92 blocks +146/-146. The complete current-block/XP-line/local-line
list is [unwired points](m9-t100-s4-p97-unwired-points.tsv). These are candidates,
not newly approved restorations. Existing R0289/R0290 and R0409/R0411 broad
ABI justifications must not be read as proof that these particular entries
need product support.

Source evidence:

- Split all 1,063 generated S_* function bodies at case S_*_id entries.
  Seed a conservative reference closure from all other production .c files,
  including mere declarations and excluded mentions; follow S_ and j_S_
  references between generated bodies. 766 roots reach 1,055 bodies; these
  eight remain outside even that over-approximation. This is source reference
  analysis, not compiler/runtime call tracing. A root is not proof of execution.
- Full-repository symbol search outside the two defining files finds no code,
  assembly, build or test consumer of these eight exact entry symbols. Internal
  sevid004 calls connect the five unused entries to one another; they do not
  establish an external caller. File-local crules dispatch is not an exported
  dynamic selector service. No code modification was used for this search.
- ega_prts.c registers ega_gc_outb_index/ega_gc_outw and ega_gc_set_reset at
  lines1428/1435. The old !CPU_40_STYLE port alternative is excluded. These are
  the real GC port paths, not the five generated S_2233-S_2237 entries.
- vglfunc.c's C_Video initializer ends with S_0440. Selection uses the existing
  ev_glue.c setReadPointers/setWritePointers/setMarkPointers (lines496/558/673),
  not S_0441-S_0443. P66's 112 isolated selector comparisons demonstrate a
  controlled fixture, not production use of the generated selectors.

Recommended smallest candidate: restore only these eight original function
bodies' GDP/allocation differences. Keep the original functions, original
internal calls, original wrappers and the shared header needed by other bodies.
Do not remove whole files, replace them with stubs or add an exclusion branch.
Potential reduction is **456 XP changed lines**, not 456 source lines:
restoration adds228/removes228 and reduces current diff from +20,871/-21,359
to a candidate +20,643/-21,131. Hunk counts can merge and must be recomputed.
This candidate has NOT been written, compiled or run. Before delivery, prove
linkage/reference closure under both actual builds and follow the owner's
object-equivalence/full-regression rule. Restored unused native-unsafe code
must not be advertised as a supported callable API.

## Differences with evidence, and its limits

The existing GDP owner in compat/cvidc/gdp_state.c is a native side table plus
VGAGLOBALSETTINGS mapping, not an original flat offset buffer. Restoring raw
r1+offset dereferences in live bodies is not an equivalent simplification.
The matching pass checked every exact substitution, not just a representative
file. Mapped video fields, callback pointers and low-order byte aliases are
shared with the actual controller; neither x86 nor x64 can treat this owner as
the original flat GDP block. Replacing the whole owner would be a separate
architecture proposal, not a justified local restoration.

Native locals use IUH indexing. Retaining original 4-byte-per-slot allocation
can underallocate on x64. The 262 calloc cases also have the earlier per-function
narrow-write/native-read dataflow records; a whole-file poisoned-allocation
failure does not prove each calloc separately. In particular the 21 scalar
getters in sinit011/012 have an indeterminate IUH upper half without clearing,
but typed IU32 VideoVector callers discard it. This is defined-return/ABI
justification, **not evidence of guest corruption**. They are not classified
as having no evidence, nor as demonstrated guest bug fixes.

Historical evidence retained with explicit bounds:

- sevid000/001: CHAIN2 A0-gap layout and forward/backward lanes; consecutive
  WORD combination and Unchained DWORD offset. Named prior negative controls
  distinguish wrong offsets/upper halves from current results. No claim that
  each is the cause of a particular observed Win3.1 screen corruption.
- sevid016-020: selected dirty marker versus hard-coded CGA bookkeeping,
  CHAIN2 plane addressing, same-activation continuation ownership, and Copy
  head/tail/rotation semantics. Historical controls include dirty-bound failures,
  physical-address oracle mismatches, crashes or leaked allocation counts.
  Some continuation proofs are coherent group withdrawals, not each goto in
  isolation. sevid019's added odd-head tail block maps to R0364 despite being
  an insertion between old source lines, rather than an overlapping old range.
- sevid024/025: twelve addressed-plane extraction replacements have the
  accepted [S2 D004 evidence](m9-t100-s2-d004.md): real BIOS/controller selection,
  3,072 combinations per width and independent pre-fix BYTE/WORD negative
  controls. They must not be mistaken for unsupported GDP formatting edits.
- The other generated write families' native-size/zeroing evidence is the
  per-file allocation ledger and bounded P30-P67 experiments recorded in
  [owner decisions](m9-t100-owner-decisions.md). Restoration-equivalence tests
  are not exhaustive hardware correctness tests. Original declined D001-D003
  defects are not new mirror-diff retention justification.

## Counter-review and delivery boundary

All 34 hashes and counts were rechecked; categories sum to15,129. No source
writes occurred. The eight-entry gap is stronger than a missing test: the
selected production wiring is elsewhere. Conversely lack of a per-hunk dynamic
experiment is not enough to call a proven shared ABI adaptation unnecessary.

No fresh build, runtime negative control, desktop/guest test or EXE publication
was performed for this documentation-only audit. Historical tests above are
clearly historical. The report does not approve all remaining diffs as minimal,
close S4, or claim a runtime-safe restoration has already passed. The next
concrete decision is whether to implement/qualify the eight-body candidate.
