# T86 S6: Inward Component Boundaries

## Objective

Make the current source tree obey one acyclic ownership order everywhere a
component speaks for itself:

```text
lib < emulator < x86 < app-softpc
```

The admitted rename makes `x86` the shared x86-facing layer. It owns
`xasm32`, `debug`, and `product`; the former `product/surface` becomes
`x86/product`.

The rule applies to component-owned production code, public headers, tests,
fixtures, component CMake/configuration, manifests, and component READMEs.
An outer component may use an inner component; an inner component may not name,
include, link, configure against, or document a dependency on an outer one.
`app-softpc` is the outer product and may use all three inward tiers.

## Scope and interpretation

The finite audit roots are:

1. `src/lib` and `test/lib` — self only;
2. `src/emulator` and `test/emulator` — self plus Lib;
3. `src/x86` and `test/x86` — self plus Emulator and Lib; and
4. `src/app-softpc` and `test/app-softpc` — self plus all inward tiers.

The repository-root build files are the explicit assembly point and may name
multiple tiers to compose SoftPC.  Repository-level design, current-state and
history documents likewise describe the whole architecture, not a component
dependency.  This is not an exception for component-owned files.

The audit distinguishes a component path/target reference from ordinary prose:
``product-neutral`` and ``product behavior`` do not create a dependency;
`src/x86`, `test/x86`, `x86-*`, or an X86 include/link does.
Negative tests may demonstrate a generic foreign include without encoding a
specific outer product name when the owner verifier already proves that class.

## Initial audit

The source/header include and local target-link scans find no confirmed outward
production edge:

- Lib source includes and targets remain inside Lib.
- Emulator source and targets use only Emulator and Lib.
- X86 source and targets use only X86, Emulator and Lib.
- App source/tests only use its own or inward contracts.

Three component-owned self-description leaks are confirmed:

1. `src/lib/verify_kvm_naming.cmake` contains a stale `emulator/ui` special
   case although it scans only Lib roots.
2. `test/lib/README.md` advertises direct adoption of Emulator paths.
3. Emulator documentation/negative-test text names the outer shared X86
   package even though Emulator's own verifier already rejects every
   non-Emulator/Lib include and target edge.

These are configuration/documentation/test-boundary defects, not runtime
dependencies.  The repair removes the stale external knowledge, preserves the
same rejection coverage through generic foreign-edge probes, and adds one
central static gate over the six shared roots plus the App roots.

## Repair and proof

1. Remove stale outer component path/target spellings from Lib and Emulator
   component-local verifier/test/doc files.
2. Add one explicit inward-boundary static check.  It checks component-owned
   C/H include prefixes, local CMake target/source references, and explicit
   component paths/targets in component READMEs/configuration.  It reports the
   owning path and prohibited tier; it does not parse generic English words.
3. Register a focused negative receiver that proves each prohibited direction,
   then run the existing Lib/Emulator/X86 corpus gates and all changed
   App/static checks on x64 and x86.
4. Update manifests and architecture documentation so the rule is discoverable
   without reading task history.

No runtime behavior, public ABI, worker, configuration format, user INI,
guest-media or snapshot changes belong to this task.  The two executable
artifacts are rebuilt only because their linked public symbol identities move.

## Implementation and verification

The former shared Product source/test corpus is now the X86 corpus:

- `src/product` → `src/x86` and `test/product` → `test/x86`;
- its former `surface` child is `x86/product` in both roots;
- public includes, C types, functions, targets, test registrations, manifests,
  corpus verifiers, and negative boundary receivers consistently use the `x86`
  identity;
- `emulator/product` and `app-softpc/product` deliberately retain their
  existing meanings: neutral Emulator composition and the private SoftPC App
  layer, respectively.

The repair removes the three confirmed inner-layer self-description leaks and
adds `checks/inward_component_boundaries` plus a negative selftest.  It scans
every component-owned C/H/CMake/README/manifest path in all eight roots,
including target names and explicit source/test component paths.  The root
assembler remains outside this rule by design.

While making each reusable test corpus self-verifying, the delivery also
replaced the erroneous production-manifest invocation in `test/lib`,
`test/emulator`, and `test/x86` with one shared test-manifest verifier.  This
does not alter production behavior; it makes each test package actually
validate its own files, ordering, hashes and LF normalization.

Verification on 2026-10-10:

1. all six source/test manifests, the new inward-boundary gate and its
   negative receiver, the X86 boundary gate/negative receiver, test-boundary
   receiver, documentation governance and `git diff --check` pass;
2. x64 Ninja Release build passes; background CTest excluding desktop: 131/131
   in 148.05 s real time;
3. clean x86 Ninja Release build passes; background CTest excluding desktop:
   131/131 in 137.77 s real time; and
4. both package EXEs were rebuilt from the renamed source graph.  User INI,
   guest media and snapshots were not changed.

The final actual-change review must still inspect the staged rename map and
the no-outward-reference proof before the P commit.  This delivery then awaits
owner validation; it does not close T86.

## S6 P2: C spelling and test-target identity

The P1 relocation accidentally inverted the established C spelling split in
the X86 public debug surface: its opaque type became `X86_DEBUG`, while its
macros, enum values and include guards became lower-case `x86_debug_*`.
This is a naming-only defect, but it makes the source differ from the shared
corpus and obscures which identifiers are values versus types/functions.

The correction has one rule everywhere:

- types and functions use lower-case component prefixes, for example
  `x86_debug` and `x86_debug_create`;
- macros, enum values and include guards use upper-case component prefixes,
  for example `X86_DEBUG_READ_REGISTER` and `X86_XASM32_INTERFACE_H`.

The finite Lib/Emulator/X86 audit also checks explicit test executable names.
Lib has no manual target whose name loses its owner or subcomponent. Emulator
has one: `emulator-test-composition` builds `product/composition_smoke.c`, so
it becomes `emulator-product-composition-smoke`. X86 restores the admitted
`x86-product-entry-smoke` target and matching isolated work-directory name.
All are CMake target-name corrections only; CTest identities, target links and
test behavior remain unchanged.

The deliberate shared-corpus differences after this repair remain the P1
test-manifest ownership helper and the inward-boundary wording. No Lib or
Emulator production spelling defect was found; test-only lower-case macro
aliases that replace platform calls remain intentionally lower-case and were
not renamed.
