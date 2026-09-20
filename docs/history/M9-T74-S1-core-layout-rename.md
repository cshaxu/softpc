# M9 T74 S1 Closure

Owner reports testing passed and explicitly approves T74 closure.
S1 is accepted and closed; it is the only admitted S in T74.

Original request: pure relocation of mvdm to core, vm to core/machine and
compat to core/compat, retaining core/softpc.new. Executor 8b76baf5 and actual
change review 5baad03b were pushed before owner acceptance.

The [retained proposal](M9-T74-core-layout-rename-proposal.md) contains the
finite ledger, implementation proof, counts, verification and package hashes.
556 files moved: 540 exact renames and 16 include-only renames. All 498 mirror
blobs are identical; 107 product/test C/H/RC files passed path-only comparison.
No ABI, symbols, execution semantics, media, INI or shared corpus changed.
Source/resource production +34/-34; tests/gates +195/-184; build +190/-189;
tools +2/-2. Total +421/-409, net +12, excluding docs/artifacts and pure moves.

Both Release builds passed. Background x64 110/110 (165.45s), x86 110/110
(157.47s); five desktop tests excluded per width. Owner manual acceptance is
additional evidence, not a claim those excluded automated tests ran.
Closure is documentation-only; no new build or source change is necessary.

All admitted items are complete. Historical paths in archived evidence and
historical generators remain intentional; live paths and dependency gates
use the new layout. Existing Queue candidates remain outside this S.
