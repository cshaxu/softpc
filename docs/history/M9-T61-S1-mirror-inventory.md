# M9 T61 S1 — Mirror inventory

## Request and boundary

Owner admitted T61 from the supplied MVDM/OpenNT audit: minimize unnecessary
original-source diff while preserving required ABI, fixes and user experience.
S1 is read-only production research and evidence governance, not simplification.
The [proposal](M9-T61-mvdm-diff-minimization-proposal.md) defines S2–S6.

## Executor delivery

P1 `e2e91c1` records the [complete ledger](../etc/evidence/softpc/pristine-divergence-current.md)
and optional read-only `tools/Audit-MirrorDiff.ps1`. The old M1 table is retained
under a historical name and linked, not silently overwritten as current truth.

- Frozen universe: 498 tracked files, 401 identical, 97 different, zero no-peer;
  650 original files not selected. External reference worktree is unchanged.
- Raw diff: +46,241/-44,893, 5,112 hunks. Ignore-space-at-eol supplementary
  diff: +23,779/-22,434, 5,138 hunks. Twelve groups reconcile to these 97 files.
- 34 generated rule files pass whole-file inverse-pattern classification;
  63 remaining files have content reasons and receiver/disposition. No unknown
  standalone keyboard/audio branch is claimed minimal before S4/S5 assessment.
- All 18 previous whitespace-only restorations match original hashes.
- No production/test/package/INI/media change. Existing owner-tested T60
  x86/x64 hashes are retained; no new build or runtime result is claimed.
- Documentation gate and diff whitespace check pass. Script total recomputation
  agrees across repeated runs; it writes no files and is not a build dependency.

## Coordinator actual-commit review

Reviewed pushed `e2e91c1` changed paths and actual script/evidence. The pattern
comparison is strictly research in memory, not a return to generated sources.
Raw and whitespace-ignored statistics are distinguished, and hunk count is not
misrepresented as a feature count. The per-file reasons preserve ABI/device
repairs while assigning formatting, inactive branch restoration and standalone
host assessments to their approved stages. Source and test diff is zero.
P1 and origin/main match, and the post-P1 worktree was clean.

S1 closes; S2 may restore no-behavior differences only after its token-equivalence
checks. Production/test added/removed/net lines for S1 are 0/0/0; tooling adds
89 lines. T61 remains open.
