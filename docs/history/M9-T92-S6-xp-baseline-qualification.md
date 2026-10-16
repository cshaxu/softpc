# M9 T92 S6: XP Subset Baseline Qualification

Implementation-closed on 2026-10-15 under owner-authorized async S acceptance.
T92 remains open for owner verification and formal closure.

## Request and implementation

Make the necessary XP source subset the routine reference, preserve OpenNT
lineage, account for every retained/excluded path, and qualify the migrated
product without external build inputs or hidden local repairs. This includes
the owner's permission to remove proven useless pre-existing branches inside
the migration scope, not arbitrary removal of every inactive conditional.

The fixed-object audit reconciles 997 XP paths: 498 retained and 499 excluded.
S3-S5 line-range ledgers select 127 paths; remaining paths use whole counterparts.
The current manifest records upstream, subset and local hashes. 375 retained
bodies equal their subset; 123 carry explicit local patches. The normalized
local/subset diff is +26,267/-23,562; excluded upstream material is separate.

The root-owned mirror check verifies local hashes, exact path membership,
ordering, pin/disposition shape and patch identity. It needs no external
checkout and is not a build-time source converter. Its small negative fixture
checks modified/missing/extra paths, duplicate/traversal paths, inconsistent
identity and an incorrect pin, plus the exact-byte branch. Offline fixed-object
reconstruction, not this local gate alone, proves the upstream range selection.

## Verification and accounting

- Both Release package builds pass using existing Ninja intermediates.
- Each width passes 90 shared + 55 other background + 5 native desktop tests.
- All phase exits and the complete qualification exit are zero: 150/150 each.
- Documentation governance, mirror gate/selftest and diff whitespace checks pass.
- Production C/H: +0/-0. Verification scripts/registration: +136/-0, net +136.
- Manifest: +500/-0 metadata lines; not counted as production code.
- Executor complete text diff: +811/-10, net +801 across 12 text paths;
  the thirteenth path is the refreshed x86 EXE. The x64 build succeeds and
  retains S5's exact artifact bytes. No shared package, INI, medium or snapshot changes.

Artifact hashes and complete task requirements are in the
[T92 completion audit](M9-T92-completion-audit.md).

## Coordinator actual-change review

Reviewed pushed executor `48caa85407d27a67ccded3c13f63c1cd95efae66` against
the original migration/correction requests, active S6 packet and authorities.
Actual Git changes contain only the two root verification scripts, their CTest
registration, one manifest, provenance/design/status records, a comment-only
attributes correction and the x86 package artifact. There is no production
rewrite, ABI change, public-package mutation or hidden source generation.

Rechecked complete fixed-object reconstruction exit, exact manifest coverage,
all qualification phase exits and both artifact hashes. The earlier Windows
newline fixture mistake and initial whole-file import correction remain in the
proposal; green final results do not erase them. The same-issue sweep reconciles
current baseline statements while retaining historical OpenNT records as history.

Review passes. The final requirement ledger separates source proof, automatic
runtime tests and manual guest acceptance. The three existing TODOs and queued
Sound Blaster task are explicit, not falsely reported as repaired. No fresh
manual Windows acceptance, universal guest correctness or license grant is claimed.
S6 closes under delegated S acceptance; T92 closure remains the owner's decision.
