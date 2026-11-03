# M9 T99 S30 final review — awaiting owner acceptance

## Scope and outcome

Compared the 475 retained mirror paths to same-path XP SP1 source fixed at
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`: 471 text files and four binary
files without preprocessor trees. The recursive comparison keeps retained
parent/arm structure and does not descend into an arm wholly replaced by
`#error`. The full-tree physical-byte parser found 3,614 paired groups and
39 structural matching leads; their [current dispositions](m9-t99-s30-final-structural-disposition.tsv)
are 37 documented local differences and two repeated-V7 alignment artifacts.
Existing XP frames were restored where safe; retained ABI, snapshot and
rendering differences are recorded in the disposition and
[mirror exception ledger](pristine-divergence-current.md). The P42 `nt_sbdsp.c`
line position was found incorrect and corrected in P47.

The current [66 first-code leads](m9-t99-s30-final-first-code-disposition.tsv)
are cross-indexed to their P45 contextual reviews after physical insertions.
The [990 no-ordinary-code markers](m9-t99-s30-p48-no-code-directive-audit.tsv)
have 744 paired first-directive matches, 158 without a unique conditional
pair, and 88 without a directive before the next marker or EOF. Of the 158,
[157 match XP text at their effective position and one is a documented local guard](m9-t99-s30-final-unpaired-directives.tsv).
The end-marker review and correction history is in the
[independent review](m9-t99-s30-independent-review.md).

These are source-structure and diagnostic-location checks, not proof that
retained function bodies equal XP, that all hypothetical macro configurations
work, or that guest behavior has been exhaustively verified. Ordered matching,
first-directive and textual-anchor evidence have the limitations stated in the
independent review; local additions do not have invented XP source positions.

## Actual change and review

Relative to S29 `f5af155b`, the committed mirror subtree changes 82 files,
+1,711/-1,772 (net -61). This includes the conditional-structure corrections
and line directives across P1–P48; see the per-batch mapping and actual Git
diffs for the complete per-file list. Across the entire tracked repository through P48,
including the evidence ledgers, the same range changes 219 files and
+35,045/-1,864; this includes audit tables, not just production code. The
final review commit itself adds this report and its indexes.
Both packaged EXEs were refreshed. User INI, media and snapshots were not
modified by this task. No public component implementation was changed.

Independent self-review compared the committed tree and actual source diff,
including original line mapping, byte preservation for non-UTF-8 mirror
sources, registered exceptions, and the P42/P47 correction. Automated source
matching is used as evidence, not treated as a formal semantic proof.

## Verification

The last full dual-architecture suite was P43 `a251a777`: on each architecture
186 background tests and eight native desktop/audio tests passed first run.
Several earlier batches had intermittent `compact_console` failures at Console
attachment; full reruns passed, but the cause was not established. This is not
claimed fixed. Those failures and rerun evidence remain in the independent
review.

After the owner instructed not to rerun tests when the EXEs have no substantive
change, P44–P48 were built and published without re-running unit/integration
suites. The inherited build script mistakenly ran two CTest gates per
architecture in P44–P46 despite that instruction; it was corrected for P47
onward. P46–P48 changed complete-file hashes, but executable entry metadata and
all PE loadable sections matched their immediate predecessors. The final EXEs
also match P43 loadable PE content and entry metadata. This comparison does not
assert debug information equivalence.

Final cached x86/x64 `cmake --build` completed successfully with Ninja reporting
no work. Publication was checked; the final files are byte-identical to P48.
Per the owner’s latest instruction, no tests or CTest gates were run during
this final no-substantive-change build.

- `softpc32.exe` SHA-256: `de2ae6092c11f4fc915dbf02a05bc4ad1781a5ef3f12f991c143fec2d37f71ae`
- `softpc64.exe` SHA-256: `4eb4a0d58f4ec9ca7d493e7290717c7b93506ac893d72a7bb8691860c227cc1c`

The executor has completed this review and delivery. S30 remains open pending
the owner’s acceptance; T99 remains open.
