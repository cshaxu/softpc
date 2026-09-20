# Core Layout Relocation

## Admission And Intent

Owner admitted M9 T74 on 2026-09-20 after T73 closure. Baseline d7c6931f.
Original request: rename mvdm to core, move vm into core/machine and compat
into core/compat, retaining softpc.new under core; pure rename.

## Design And Scope

One atomic S1 relocates src/mvdm to src/core, src/vm to src/core/machine,
and src/compat to src/core/compat with git mv. The mirror is byte-identical.
Core is a directory grouping, not a new runtime object or build wrapper.
Machine and Compat keep their existing responsibilities and dependency edges.
Existing vm_* symbols, headers, target names, thread ownership, snapshot format,
guest behavior and public ABI remain unchanged. No forwarding include paths.
The original device target's private include root moves from src to src/core
for its existing compat/... includes (src remains for copied Lib contracts);
this preserves all 498 mirror blobs without extending App/shared search paths.
App composition alone imports core/machine/vm_interface.h; no other App file
imports Core. Compat cannot import Machine, Common, x86 or App. Shared corpora
cannot import Core. Existing original host-to-Compat includes remain allowed.

Update include/resource paths, build ownership, negative boundary fixtures,
live audit tool defaults and current layout documentation. Historical records,
T14/T16 historical map generators and invalid-input shared test fixtures retain
their historical vocabulary. The old preserved-mirror spelling in Coding Rules
identifies this same corpus; its new location is owned by Source Layout. Rules
are not edited by this implementation task (rule edits require Td).

## S1 Brief And Estimate

Move 556 tracked files: 498 mirror, 15 Machine, 43 Compat. About 50 referenced
files and 400--550 changed path lines outside pure renames; production logic
net zero. Boundary tests need small path-classification changes, not relaxed
rules. All six shared source/test corpora, user INI and guest media stay intact.
Build both Release packages, run both background presets, source/build and
documentation gates, and verify every mirror blob and reverse-transformed C/H
against baseline. No desktop input/window testing without a reserved period.

## Finite Convergence Ledger

Frozen universe: baseline tracked files under the three relocated roots plus
every live build/include/resource/tool/layout reference to those roots.
Permitted dispositions: exact move; path-only edit; preserved historical or
negative-test reference with reason. No semantic change is permitted.

| Unit | Required proof | Status |
| --- | --- | --- |
| 498 mirror files | Same relative filenames and Git blob IDs | Passed: 498/498 exact blobs, no unstaged mirror edits |
| 58 Machine/Compat files | Only required include path substitutions | Passed: included in 107-file path-only proof |
| App/resources/product tests | Only path substitution; embedded ROM unchanged | Passed: 107-file proof and unchanged ROM blobs |
| CMake and dependency gates | New roots; positive and negative edges retained | Passed: configure ownership, source gate and negative fixtures |
| Live documentation/tools | Current pointers resolve; historical facts retained | Passed: scoped old-path sweep and documentation gate |
| Shared corpora and user assets | No diff | Passed: all six corpora, INI and media untouched |
| x86/x64 packages | Release build and full background regression | Passed: 110/110 per width; five desktop cases excluded per width |

Completion requires every row proven, actual rename-aware +/-/net accounting,
P implementation push, independent actual-change review, clean worktree and
two EXE links. S/T remain open for owner testing and acceptance. No queue
candidate is implicitly admitted; the four existing candidates retain order.

## Implementation Audit

Relocation uses git mv for all 556 files. 498/498 mirror index blob IDs equal
d7c6931f at the mapped paths, and the mirror has no unstaged changes. A separate
107-file C/H/RC comparison over App, Machine, Compat and product tests allows
only the enumerated old/new include and resource path substitutions; it passes.
The deeper Compat snapshot header adjusts its relative Lib include by one level.
No function, declaration, statement or data representation changes.

Rename-aware `git diff -M --numstat d7c6931f` (excluding docs/artifacts) yields:

| Group | Content-changed paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C/H/RC | 18 | 34 | 34 | 0 |
| Product tests and boundary gates | 30 | 195 | 184 | +11 |
| Root CMake | 1 | 190 | 189 | +1 |
| Live audit tools | 2 | 2 | 2 | 0 |
| Total | 51 | 421 | 409 | +12 |

The 556 moves are not 556 rewritten implementations. Tests add the explicit
retired-root rejection, exact three-child Core layout and original-controller
private include-root fixture/resolution. CMake adds one private include root.
All shared corpora, user INI and media remain unchanged. Live old-root scanning
finds only intentional original compat includes and historical/negative cases.
T14/T16 map generators are historical replay tools, not current build inputs;
their historical source paths and all archived evidence remain unchanged.

## Verification And Artifacts

- `cmake --build --preset tests-x64 -j 6` and `tests-x86 -j 6`: passed.
- `ctest --preset test-x64 --output-on-failure`: 110/110, 165.45s.
- `ctest --preset test-x86 --output-on-failure`: 110/110, 157.47s.
- Five desktop cases per width excluded; no interactive Windows guest or RDP
  session was exercised. Background tests include restart boot, snapshot and
  device proofs; no claim of universal runtime correctness.
- Documentation gate, build-ownership and product-boundary positive/negative
  fixtures, full standalone source gate and `git diff --check`: passed.
- assets/binary/softpc32.exe: 3659622 bytes,
  SHA256 34443AC93F7B17DD68D802B66022E71DB7EAB02270D5908F8FF46454E66E444B.
- assets/binary/softpc64.exe: 3061602 bytes,
  SHA256 097C8D2654EB96DC41A3A0B9B953F23A26D8FF49768C7CB3B0CA96ED899EC7B9.

EXE sizes equal the accepted baseline. Both are refreshed T74 packages, not a
claim of byte-identical binaries. S1 is verified, awaiting P push/review and
owner testing; T74 is not closed.
