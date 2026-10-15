# M9 T91 S5: Remove the unused NXVM package pair

Status: verified executor delivery, pending coordinator review and owner acceptance.
T91 remains open. Qualification date: 2026-10-15.

Owner request: "好。准入一个新S，删掉这俩没用的组件。"
Admitted scope and active status are in CURRENT. Baseline: 3af19559.

The bounded ledger is the ten tracked files below src/x86-nxvm and
test/x86-nxvm plus all their active build, gate and documentation references.
They are unused by SoftPC's production link graph. Delete rather than replace
them; retain lib, emulator, x86-debug and x86-ibmpc source/test packages intact.
The removal is recoverable from S4 Git history. No claim is made about external
NXVM adoption or acceptance, and no external repository is changed.

Clean the root suite registration and layer lists. Replace obsolete negative
test examples with existing outer App dependencies, preserving coverage of
forbidden includes and links rather than silently deleting those assertions.
Historical task descriptions keep their original names and outcomes.

Estimate: ten deleted package files, roughly 350-400 net removed lines including
registration cleanup. No surviving production C/H implementation changes.
Qualify both existing Ninja graphs, all remaining background suites and static
gates; S4's native desktop evidence remains separate and is not claimed rerun.
Complete when every ledger member is deleted, updated or retained as history,
the eight-package tests pass, artifacts are accounted for, and P delivery and
coordinator review are pushed for owner acceptance.

## Implemented ledger and self-review

- Ten files (357 lines) removed from the two admitted roots; empty directories
  removed too. The old header has no surviving production consumer.
- Root CMake drops exactly two add_subdirectory entries; no linked runtime
  target changes. Seven NXVM-specific test registrations disappear.
- Root source/test layer lists and include/target checks retain four owners.
  Two negative boundary examples now use App includes/links, keeping the same
  forbidden-outward-edge assertions. The now-identical PC/outer default branch
  in the test verifier is collapsed. Generic branding rejection still rejects
  NXVM identities; it is not a reference to the removed package.
- Current design, root and test READMEs describe eight reusable directories.
  S4 history remains unmodified. No claim of external NXVM adoption is needed
  for the owner's direct removal instruction.

The finite sweep is all tracked src/test, tools, root CMake/presets/README and
current design documents. No x86-nxvm path, target or symbol remains there.
All eight retained package roots, App, Compat, original mirror, owner INI and
guest media have zero diff against 3af19559. No replacement implementation or compatibility
alias is introduced.

Code accounting (git diff --numstat 3af19559 to executor delivery): production
C/H +0/-34 (net -34); test C/H +0/-52 (net -52); CMake build/gates +20/-279
(net -259). Combined code/build +20/-365 (net -345); documentation and
manifests are excluded from those categories.

Build observation: x64 reports no work and retains its accepted EXE hash.
x86 Ninja reports "premature end of file; recovering" and rebuilds 581 steps
successfully. The agent did not clear the cache or force a clean rebuild; the
underlying cache-file truncation cause is not established. The resulting x86
EXE is refreshed and will ship with this delivery. This is disclosed build
evidence, not attributed to changed production code or a new runtime fix.

## Qualification and artifacts

Both Ninja Release configurations/builds succeed. Each architecture passes
90/90 shared background tests and 48/48 other tests: 138/138 total. The latter
include all 29 App units, 8 background integrations and 11 repository gates.
There were no failed S5 test runs or waived background cases. CTest inventories
contain 143 entries each, exactly seven fewer than S4. The five native desktop
cases remain registered but are not rerun in S5; S4's 5/5 per width is prior
evidence only, not a claim that the refreshed x86 artifact was desktop-tested.

No Computer Use or foreground desktop test is used. The task-owned hidden
supervisor completes successfully and leaves bounded exit/log evidence in its
declared ignored directory. Governance and whitespace checks pass. The change
does not add a runtime path, alter a surviving public interface, or modify
remaining shared manifests. Historical retained contracts remain recoverable
from S4's commits.

Published SHA256:

- softpc32.exe: A7652542623FDE0F4F6CB609FCB10755D2CCD8DACD7C2C95D8C12ADEA9068258
- softpc64.exe: 5DC11982BACDB80816BC186B9C1C64F0223DBA050C0BC8ED3677203E135A58BC

Only the x86 artifact differs from the accepted S4 package. The x64 package
already matches the current build and is not replaced for cosmetic purposes.
Executor delivery includes all deletion/registration/documentation work and the
updated artifact. Coordinator reviews the actual pushed commit next; S5 waits
for owner acceptance, while T91 stays open.
