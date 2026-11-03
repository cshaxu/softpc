# T99 S31 compiled-mirror review — in progress

## Frozen scope and baseline

The two cached Ninja dependency databases each name the same 470 distinct
files under `src/app-softpc/softpc.new`: 36 under `host/` and 434 elsewhere.
The [file ledger](m9-t99-s31-file-ledger.tsv) records paths, architectures,
byte hashes, prior S30 classification and S31 disposition. This covers
actual recorded build dependencies, including headers; it is not a proof for
unbuilt configurations or a promise that all retained files are build inputs.

All 470 files have a same-path XP SP1 original. The checked-out XP branch
points at `a2f6d7c93aa4f11efc51147566d5961c59a8ef89`; all 475 retained XP
originals match the existing upstream manifest after its documented text
normalization. Only XP was read as comparison source for S31.

## Current findings

245 compiled paths are byte-identical to XP. For another 210 paths an
independent recursive scan matches conditional signatures; for the remaining
15 it produced structural leads already present in S30 evidence. These
automated and historical results are leads, not a substitute for S31's
per-file review. An independent scan has established that 47 of the 225
non-identical file pairs contain no conditional, error or line directives in
either version. Another 178 pairs have completed review, including actual
excluded-arm evidence and retained `#line` positions. An independent line-context
scan produced 233 leads in 83 files after correcting its line splitter to count
only LF bytes. The first run counted form-feed bytes in `vgaports.h` as line
breaks and produced six false leads; this was identified and corrected before
any change to that file. The scan includes comments, local additions and
other harmless offsets; these are not confirmed defects. Of previously reviewed
files with conditional structures, all reopened line leads have now been
individually disposed. All 470 files now have individual dispositions. The 47 scanned files with no conditional, error
or line directives are not treated as missing conditional-boundary reanchors.

The owner also requires retaining original no-op stubs where semantics permit.
In `base/ccpu386/ntstubs.c`, XP has three empty global definitions also present
as empty definitions in the simultaneously compiled original `rom.c` and
`fpu.c`. Both originals cannot be compiled together without duplicate symbols;
the selected copies remain original no-ops and the duplicate omission remains
an exception. The previous ledger's claim that these were nonempty real
implementations was incorrect and has been corrected.

An initial syntax scan across the 470 XP counterparts found seven original
empty-function leads absent as empty definitions in the same current files.
Three are the `ntstubs.c` duplicate symbols above. `_simple_mark_lge`,
`_simple_mark_sml`, and the empty `gvi_term` are inside original NEC_98 arms
that are excluded as a whole. The original empty `nt_end_update` is instead
the same selected function with the accepted nonempty video-update completion
call; restoring an empty body would change current behavior. This scan is a
lead search for simple empty function definitions, not proof against every
possible macro spelling or disguised no-op.
An expanded scan including empty parameterized functions and comment-only
bodies also found `herc_alt_sel` in an excluded HERC arm and
`dummy_cursor_paint` in an excluded NEC_98 arm. Neither is eligible for
restoration inside an arm replaced by `#error`.

The new scan reports 39 potential structural differences in 15 files; all
39 match the S30 disposition by file, line and kind. Its 3,578 paired groups
all cross-index to the older per-group accounting using XP file, line and
conditional signature. These are leads and matching evidence, not by
themselves formal proof of the correct pairing of repeated siblings or the
necessity of every exception. The ledger must record the evidence for each
completed disposition rather than marking a file complete from a cross-index.

The pre-S31 actual-context probe at the same `ee2250bb` source used 580
x86/x64 translation-unit preprocessing contexts. It observed 2,364 selected
arms and 2,409 unselected arms whose bodies were already single-error; no
reachable but wholly unselected arm with retained code was observed. Its
temporary raw output was retired. This is evidence for those actual contexts,
not proof about hypothetical build profiles or of every `#line` semantic.

## Build and verification

S31's first correction reanchors `base/system/at_dma.c` immediately before
`do_transfer` with `#line 872`: the accepted relocated `dmaGetAdaptor` had
left the following original function at effective line 864 instead of XP
physical line 872. The second correction reanchors `base/keymouse/mouse.c`
at the retained 30 Hz case and subsequent PROD condition with `#line 385`
and `#line 393`; original comments and obsolete interrupt-burst logic had
previously been removed under the registered local exception. The third
correction adds `#line 385` before the original `Glue_reads` initializer in
`base/video/ega_read.c`, following three original declarations previously
relocated for ABI correctness. The fourth similarly adds `#line 312` before
`Glue_writes` in `base/video/ega_writ.c`, following six relocated declarations.
The fifth adds `#line 1503` before the cursor-end register assignment in
`base/video/vga_prts.c`; the accepted cursor-start change had shifted that
subsequent original statement's effective location. The sixth adds `#line 187`
before the retained `CheckForFullscreenSwitch` declaration in
`host/inc/nt_fulsc.h`, after an intentionally omitted NT host declaration.
The VGA source's 127 recursive groups have been paired with XP; its
remaining line-context leads follow accepted video fixes. No execution
statement changes. This final batch contains six changed mirror files,
with seven added source lines and no deleted source lines.

Both cached Ninja builds completed successfully (x86 and x64). Their PE
headers compare equal after excluding timestamp and checksum; every loadable
section, including contents, geometry and flags, compares equal to the
pre-batch EXE. The complete EXE hashes changed:

- x86: `45663a7f784ad56494057a0c7e9140af4dab3743ac97e2a04068ec0ec152ffa1`
- x64: `aaf50e7f1742b13211383f0d09eb449b23bf1229dfea23ee25b93fa6cffcf8fb`

The EXEs in `assets/binary` are the rebuilt outputs. Per the owner's explicit
rule for no substantive executable changes, tests were not rerun for this
batch. The XP mirror and documentation gates passed. S30's last full suite
and its intermittent native Console qualification caveat remain as recorded
in its final review.

A separate check for test-only mirror inputs used the actual `softpcvm` Ninja
graph and the object dependency databases on each architecture. All 470
canonical mirror paths in the ledger occur in at least one production object's
dependencies on both architectures. No test-only compiled mirror input was
identified, so no file was removed for that reason.

Before these corrections, cached x86 and x64 builds were prematurely run
before the required file review; each reported `ninja: no work to do`. The
owner rejected this ordering. They were not counted as this batch's build.
