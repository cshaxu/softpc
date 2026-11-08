# M9 Td S23 build timestamp repair

Baseline: ecf263a8 (S41 remains awaiting owner acceptance).

## Cause and single path

Emulator Product composition.c is the only production source using
__DATE__/__TIME__. Ninja reused the Nov 6 object; relinking did not refresh
those literals. No C behavior, banner ABI, mirror or user configuration changes.

The existing translation unit is compiled as a Product-owned object target.
An always-run build-local stamp is its dependency; its compiler launcher is
empty. Other objects retain ccache and incremental reuse. The existing static
library consumes this object. No generated C/H, source touching or runtime clock.

The root POST_BUILD check verifies stamp/object order, the compiler date and
both literal values in the EXE. Hex matching handles linker suffix-string
merging (the time may share a longer string). This verifies embedded banner
arguments, not an independently observed Console screenshot.

## Review and scope

The Emulator dependency verifier derives ownership from the component prefix
for internal targets and still validates every dependency edge. The root
source-generation gate permits target POST_BUILD checks, retaining the ban on
OUTPUT source generators and retired transformations. No runtime API or test
fixture changes. Public-tree changes are confined to src/emulator CMake,
its verifier and manifest; NXVM has not been modified or claimed synchronized.

## Verification ledger

- Initial gate implementation failed on CMake escapes, compiler-date spacing
  and COFF/linked-string parsing. These were corrected before qualification.
- First full x64 run: 185/188; three build ownership/source-generation gates
  rejected the new build shape. Their classifications were corrected;
  all five focused corpus/manifest/negative/boundary checks then passed.
- Unchanged-source x64 builds refreshed 10:41:24 to 10:41:26 on Nov 8 2026;
  only composition.c compiled, with the dependent links refreshed.
- S41 previous64.exe was rejected against the new object by the publication
  check (exit 1); current EXE accepted. Generated Ninja compiler rule directly
  invokes gcc for composition.c; normal targets retain the launcher.
- Final x64 and x86: each 188/188 background plus 8/8 native tests passed
  (392/392 total). Native tests include package, compact Console, audio/MIDI,
  window and broker checks; the Console session was active.
- Final repeated builds: x64 10:44:11 -> 10:44:13; x86 10:45:39 -> 10:45:41
  on Nov 8 2026. Both built without warnings. No source edits between repeats.
- Final documentation and source boundary checks passed after evidence updates.
  No temporary generated-source or runtime date path remains.

The finite scope is the one production timestamp site, its build dependency,
compiler launcher, both published architectures, and the old-artifact negative
control. No claim of all possible build generators or clock-change scenarios.

## Published artifacts

- softpc32.exe: SHA-256 `76e1099e10e8f1aee6885a87cc0b85db65c4b15c3a3fd0d1c2790805d33e4e59`.
- softpc64.exe: SHA-256 `0167d55840d582bcae9b6b6bf400fabc75f194f48866df4ae189d79080b9c052`.

Build scripts/checkers: +49/-2 (Product CMake +12/-1, root CMake +8/-0,
Emulator verifier +1/-1, root source gate +3/-0, timestamp checker +25/-0).
Production C/H and tests: +0/-0; mirror diff: unchanged. Manifest and governance
records are separate. This intentionally adds the previously missing freshness
contract; it does not reduce runtime source or introduce a second banner path.
