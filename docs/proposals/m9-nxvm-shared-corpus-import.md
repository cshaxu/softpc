# M9 T82: NXVM shared-corpus import readiness

## Original owner request

> 审查 nxvm 六组件，准备除了 audio 以外部分的原样导入。

## Objective

Make a controlled, evidence-based path for SoftPC to adopt the current NXVM
shared corpus unchanged wherever it is compatible. The frozen universe is the
six reusable directories:

```text
src/lib      test/lib
src/common   test/common
src/x86      test/x86
```

`src/lib/audio` and Audio-only tests, build wiring, manifests and documentation
are excluded from this task. T81's accepted Audio endpoint remains SoftPC-owned
until a separately admitted cross-repository Audio convergence task.

The task is complete only when every non-Audio shared path is either byte-equal
to the selected NXVM source, explicitly retained with an owner-approved reason,
or excluded because it is Audio-only. “Original import” means source and test
content are copied without local semantic edits; derived repository manifests
may naturally differ only by their own hash entries.

## Reference and boundary

NXVM is read-only comparison material. At admission its HEAD is `d485a54e` and
its worktree contains unrelated pending product/asset/document changes. S1
therefore records content hashes for the six actual directories being reviewed,
including their working-tree state, and never treats NXVM Git status as a
SoftPC dependency. No build, runtime or test reaches into NXVM.

SoftPC's reference endpoint is `cdeb5448`; the current T81 Audio implementation
and owner-approved media are retained. Neither Core, App, Compat, VM, package
INI, guest media nor public product behavior may change merely to make a shared
file fit. Any required local product adaptation proves that a path is not an
unaltered corpus import and stops that path for owner review.

## Ordered S tasks

| S | Scope | Expected change | Completion evidence |
| --- | --- | --- | --- |
| S1 | Build a six-directory path/hash/diff ledger, classify every non-Audio difference as exact, candidate import, SoftPC retention, or blocker; audit C11, DAG, include and platform-link contracts. | Docs only, +120..260/-0..40. | Frozen ledger, no source edit, documentation gate; wait for owner review. |
| S2 | Adopt approved non-Audio Lib paths exactly, together with matching Lib tests and derived manifests. | Audit-determined; source/test replacement only. | Exact selected-path hash proof, strict C11 dual-width Lib suites and SoftPC background regression. |
| S3 | Adopt approved Common paths/tests exactly, without Core or App adaptation. | Audit-determined; source/test replacement only. | Exact selected-path hash proof, strict C11 dual-width Common suites and product regression. |
| S4 | Adopt approved x86 paths/tests exactly, preserving SoftPC's explicit optional x86 connection. | Audit-determined; source/test replacement only. | Exact selected-path hash proof, strict C11 dual-width x86 suites and product regression. |
| S5 | Reconcile shared manifests, standalone package entries and negative dependency checks; no behavioral source fork. | Audit-determined; mostly manifests/CMake/tests. | Six-package isolated build/test and no forbidden dependency edge. |
| S6 | Perform task-level path ledger audit and byte-identity proof for all adopted non-Audio files; report all deliberate retained differences and close only after owner package acceptance. | Docs/build evidence only unless an S1 ledger item proves otherwise. | Dual-width regression, six-package proof, closure ledger and owner acceptance. |

Each implementation S is separately admitted after its predecessor is accepted.
No S may silently fold an NXVM Audio change into a non-Audio import.

## Acceptance ledger

| Unit | Allowed disposition | Receiver |
| --- | --- | --- |
| Every non-Audio file in the six-directory universe | byte-equal import, explicit retained SoftPC difference, or proven not applicable | S1 ledger, then S2--S6 path review |
| Every excluded Audio path | Audio-only exclusion, not partial import | S1 ledger/static path check |
| Public interfaces and component DAG | unchanged exact import or documented blocker | strict C11/DAG/package checks |
| SoftPC product integration | no required App/Core/Compat/VM semantic change | changed-path review and product regression |
| Final shared packages | selected adopted files match NXVM content; manifests remain self-consistent | S6 hash and isolated package proof |

No Linux runtime, NNES/MyNES product acceptance, new Audio behavior or claim of
future NXVM worktree stability is implied by this task.

## S1 frozen audit

The reference is NXVM `d485a54e`. Its six shared directories have no working
tree differences from that commit, despite unrelated dirty NXVM product/assets
and documentation paths. The audit may therefore name that commit without
depending on NXVM's mutable worktree.

NXVM has 221 shared paths. Nine are Audio-only and excluded: five Audio source
files, `types/win32/audio.h`, and three Audio tests. SoftPC has 213 shared
paths, of which eight are Audio-only (it does not have NXVM's additional Audio
platform smoke). The resulting non-Audio union is 213 paths:

| Root | SoftPC | NXVM | Exact | Same path, different content | SoftPC-only | NXVM-only |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `src/lib` | 91 | 98 | 44 | 47 | 0 | 7 |
| `src/common` | 23 | 23 | 10 | 13 | 0 | 0 |
| `src/x86` | 15 | 14 | 5 | 9 | 1 | 0 |
| `test/lib` | 46 | 47 | 8 | 38 | 0 | 1 |
| `test/common` | 20 | 20 | 6 | 14 | 0 | 0 |
| `test/x86` | 10 | 10 | 1 | 9 | 0 | 0 |
| **Total** | **205** | **212** | **74** | **130** | **1** | **8** |

The seven NXVM-only Lib source paths plus one test are `base/process` and
platform process declarations. They implement executable-directory discovery
and are used by NXVM applications only; SoftPC has no caller. They are not a
required repair or dependency, so S2 must not add this dormant capability just
to improve a path-count metric. Their absence is an explicit retained
difference unless the owner separately requests that capability.

The sole SoftPC-only path is `x86/debug/command_runtime.h`. It preserves the
DOS DEBUG command body’s original vocabulary while mapping it to SoftPC Types.
NXVM instead rewrites `x86/debug/command.c` directly to its current Types
vocabulary. Public Debug headers are unchanged, but deleting this shim would
abandon the previously approved “retain original command-body style” policy.
S4 is therefore blocked on an explicit owner choice; it may not silently
replace that style under the label of a corpus import.

The 130 same-path differences divide into three material groups:

1. **Types convergence, Lib and Common:** NXVM consistently uses fixed
   `lib_i*`, `lib_size`, `lib_bool`, `lib_uptr`, normalized text/memory helpers
   and additional status vocabulary. SoftPC's outer product code does not call
   its retired `lib_c_str*` spellings; current uses are inside x86, the same
   area requiring the S4 policy decision. Equality-only uses of
   `lib_memory_compare` do not depend on its exact negative/positive magnitude.
   This makes a Lib-first then Common mechanical candidate plausible, but it
   still requires strict dual-width compilation and product regression.
2. **Mixed root files:** `src/lib/CMakeLists.txt`, Lib/test CMake entries,
   manifests and READMEs contain both Audio and non-Audio facts. They cannot be
   byte-identical while Audio remains excluded. S2/S5 may copy only auditable
   non-Audio sections and regenerate repository-local manifests; they must not
   call that mixed-file merge an unchanged import.
3. **x86 source/tests:** besides the shim choice, xasm32/debug changes are the
   same Types vocabulary migration. Their published interfaces remain the
   visible boundary, but S4 must prove existing SoftPC DOS/X command semantics
   before replacement.

No six-directory source includes `app/`, `core/`, `compat/` or `vm/`. NXVM's
Common and x86 platform/DAG verifiers pass, and all six package CMake entries
select strict C11. Its Lib DAG verifier also passes. However its manifests are
not currently import-ready: `src/lib`, `src/common`, `test/lib`, `test/common`
and `test/x86` fail their own verification (first mismatches respectively
`types/atomic.h`, `session/session.c`, `kvm_window_modal_smoke.c`,
`machine_fixture.c`, and `debug_machine_smoke.c`). Only `src/x86` verifies.

This is a hard provenance blocker for an “original six-component corpus”
claim. S2 may only begin after NXVM publishes matching manifests, or after the
owner revises the task to adopt source content while deliberately treating all
NXVM manifests as noncanonical local derivations.
