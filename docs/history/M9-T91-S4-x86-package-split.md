# M9 T91 S4: Split X86 tools, PC product and NXVM contracts

Status: coordinator-reviewed delivery, waiting for owner acceptance.
T91 stays open. Qualification date: 2026-10-15.
Executor P1: `8046b085`, pushed to main.

## Original request

"x86/debug -> x86-debug/product; x86/xasm32 -> x86-debug/xasm32;
x86/product -> x86-ibmpc/product (softpc和nxvm当前都要的 keyboard, command, entry这些);
x86/product -> x86-nxvm/product (只是nxvm需要的东西，拆出来，别删，回头我让nxvm/core拿回去).
这样一个x86公共组件就拆成了3份，对应测试也要一起拆分，保证目录结构一致。
准入一个S任务完成本次要求。"

## Design and bounded convergence ledger

Baseline: `7d2ca0f6`. The frozen universe is the 24 source and 12 test files
of the former X86 corpus, their tracked consumers, and the new package gates.
Every former implementation/test moves once; manifests and package build/gate
files are regenerated or split. No old alias or second implementation remains.

- x86-debug owns the existing debugger as product and the unchanged xasm32
  engine. Its dependencies remain Lib and Emulator only.
- x86-ibmpc owns the existing entry/command/keyboard policy and consumes the
  debugger. It must not know NXVM speed/info or private machine configuration.
- x86-nxvm preserves the existing speed/info vocabulary and callback bundle,
  with explicit package symbols and a self-contained contract test. This is
  preservation of already-shared declarations, not an import of Core behavior.
- App still owns its composition and extension callbacks. No CLI, prompt,
  banner, keyboard, snapshot, media or lifecycle semantics change.
- Tests follow their source subdirectories, never load sibling test fixtures,
  and retain all existing behavioral coverage. Each new source/test package
  has a standalone C11 build entry, manifest and boundary proof.

Dependency order refines the former X86 tier: Lib, Emulator, x86-debug,
x86-ibmpc, x86-nxvm. SoftPC consumes only the first four; NXVM contracts are
built/tested for preservation, not linked into the SoftPC product.

Expected work: move 36 existing package paths; update approximately 15-25
consumer/build/gate paths; zero algorithm changes. Estimated net +150-300
lines primarily for independent package builds/gates and contract coverage.
Actual rename-aware production/test accounting supersedes this estimate.

Completion requires all ledger members assigned, no old paths or leaked
NXVM fields, independent package checks, both Release packages, full background
and reserved desktop tests, pushed P delivery and coordinator actual-change
review. Evidence jobs are hidden, owned and bounded under build/t91/s4.

## Implementation and finite audit

All 24 former source and 12 former test paths have a disposition: existing
runtime/test bodies move to their named owners; package CMake, manifests and
verifiers split into independent packages. No compatibility alias or old
source/test package remains. App includes and link targets select the new owners.

Normalized comparison of 18 moved production C/H files, excluding the split
machine contract, matches the baseline exactly after path/symbol substitutions.
The assembler namespace is x86_debug_xasm32; PC shell symbols are
x86_ibmpc_product. Debugger x86_debug symbols already match their owner.
The old machine information fields, both speed values, context and callbacks
are retained in x86_nxvm_product_machine. PC app_composed_machine retains only
the existing Emulator composition; no new lifetime path or flattening is added.

The source/header/build/document and test-boundary gates cover all three
packages. Negative fixtures reject debugger-to-PC, PC-to-NXVM, assembler-to-
debugger/Emulator, and sibling-test fixture dependencies. Independent test
packages consume production inward dependencies, never sibling test suites.
The shared registration helper normalizes hyphens only when forming CMake
variable names; external test/target names retain their owning package names.

The old-path/symbol sweep over src, test, tools, root CMake/README and current
design finds no src/x86/, test/x86/, x86_product_ or x86_xasm32_ references.
Historical records retain historical names. Speed/info declarations exist only
in x86-nxvm and its contract test. SoftPC does not link that package. Lib and
Emulator source/test corpora, Compat, the preserved mirror, guest media,
snapshots and owner INI are unchanged. No external repository was modified.

## Verification

Existing Ninja Release graphs compile affected targets and publish both EXEs;
already built intermediates are reused. All tests run under hidden owned
supervision. Native desktop tests run serially in the owner-reserved period.

| Width | Shared background | Other background | Native desktop | Total |
| --- | ---: | ---: | ---: | ---: |
| x64 | 97/97 | 48/48 | 5/5 | 150/150 |
| x86 | 97/97 | 48/48 | 5/5 | 150/150 |

Each width includes 48 Lib background, 21 Emulator, 12 Debug, 9 PC and 7 NXVM
tests; other background has 29 App unit, 8 integration and 11 repository checks.
Desktop includes three Lib native cases and both package integration variants.
No test is waived; no Computer Use is used. No Linux runtime claim is made.

Each new package independently configures for x64 and x86 and passes a Ninja
dry-run build-plan check (six configurations). Actual compilation and execution
use the aggregate graph above, avoiding repeated compilation of Lib for every
standalone graph. This is not a claim of six additional standalone runtime runs.

Initial migration failures are retained separately: non-ordinal manifest sorting,
an incorrect test assertion name, and two boundary-check migration omissions
(include-prefix extraction and wrapped diagnostic matching). These were corrected
in their owners before the final complete background runs. No production runtime
failure was encountered. Final desktop runs passed on their first attempt.

## Rename-aware accounting

Method: git diff --numstat -M20% from baseline 7d2ca0f6 to executor delivery,
classified by src C/H, test C/H, and CMakeLists.txt/.cmake. The lower similarity
threshold accounts for repetitive namespace changes without counting relocation
as full deletion/recreation. Documentation, manifests and binaries are separate.

| Category | Paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C/H | 27 | 216 | 205 | +11 |
| Test C/H | 13 | 120 | 68 | +52 |
| Build and verification CMake | 24 | 717 | 197 | +520 |

Production net growth is the 11-line PC composition-only contract; the old
34-line NXVM declaration is preserved by relocation. Test net growth is one
52-line NXVM callback/field fixture. Other C/H changes are names/includes only.
The build/gate increase exceeds the original +150-300 overall estimate: each
new standalone package retains its own manifest, C11 entry and dependency gate.
This adds packaging/verification text, not runtime state or algorithms; no new
cross-package verifier framework is introduced to hide that cost.
Original-mirror diff: +0/-0.

Published SHA256:

- softpc32.exe: 63D3F3CD27D8DF2DB40B0F8F1B7830672D4236A3F286243BAD336549028BD670
- softpc64.exe: 5DC11982BACDB80816BC186B9C1C64F0223DBA050C0BC8ED3677203E135A58BC

The executor P includes the complete implementation, qualification and both
artifacts. A separate coordinator actual-commit review follows its push;
owner acceptance is still required before S4 closure.

## Coordinator actual-change review

After the P1 push, the same session switched from executor to coordinator and
reviewed the actual committed path/diff inventory against the original request,
the finite ledger, all test exit records and published EXE hashes. The 18-file
normalized comparison and the original 34-line machine contract confirm that
this is relocation/namespace separation, not replacement of runtime algorithms.
App main still passes product_name to the existing banner field; snapshot and
extension implementations retain their callbacks and behavior. No unrelated
source, configuration, media or external-repository changes are present.

Both architecture results are 150/150, including all native desktop tests;
the six separate configure/dry-run graphs are not misrepresented as extra
compiled runtime qualifications. The initial failed migration checks remain
disclosed above. Documentation governance and whitespace checks pass.

Review accepts the technical delivery, not owner acceptance or S/T closure.
The source/test package split is intentionally no longer byte-identical to
NXVM's old six-root layout. NXVM-only declarations remain available for its
later adoption; no NXVM Core code has been imported. T91 remains open.
