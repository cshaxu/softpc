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

`src/lib/audio`, `types/win32/audio.h`, Audio-only tests, and the Audio-owned
portions of mixed build/document files are excluded from this task. T81's
accepted Audio endpoint remains SoftPC-owned until a separately admitted
cross-repository Audio convergence task.

The task is complete only when every non-Audio shared source/test path is
byte-equal to the selected NXVM source. There are no discretionary SoftPC
retentions: the former process-component and x86-command-shim exceptions are
explicitly revoked by the owner. “Original import” means source and test
content are copied without local semantic edits. Manifests are derived hashes,
not executable source; because Audio is intentionally different and NXVM's
current manifests are stale, they are regenerated against SoftPC's selected
content and independently checked by a non-Audio exact-path comparator.

## Reference and boundary

NXVM is read-only comparison material. At admission its HEAD is `d485a54e` and
its worktree contains unrelated pending product/asset/document changes. S1
therefore records content hashes for the six actual directories being reviewed,
including their working-tree state, and never treats NXVM Git status as a
SoftPC dependency. No build, runtime or test reaches into NXVM.

SoftPC's reference endpoint is `cdeb5448`; the current T81 Audio implementation
and owner-approved media are retained. The one admitted product adaptation is
SoftPC App's adjacent-INI discovery: it must use the imported
`base_process_executable_directory()` exactly as NXVM applications do, then
append its product-specific `softpc.ini` filename. Its existing relative-media
resolution and all Core/Compat/VM behavior remain unchanged. No other App,
Core, Compat, VM, package INI, guest media, public ABI or product behavior may
change merely to make a shared file fit.

## Ordered S tasks

| S | Scope | Expected change | Completion evidence |
| --- | --- | --- | --- |
| S1 | Build a six-directory path/hash/diff ledger, classify every non-Audio difference as exact, candidate import, SoftPC retention, or blocker; audit C11, DAG, include and platform-link contracts. | Docs only, +120..260/-0..40. | Frozen ledger, no source edit, documentation gate; wait for owner review. |
| S2 | Adopt every non-Audio Lib path exactly, including `base/process`; replace SoftPC's direct Win32 EXE-directory lookup with the NXVM-style imported Base call, retaining the `softpc.ini` product filename. Audio-owned mixed-file portions and derived manifests remain local. | Lib/test replacement plus narrow App config use; estimate 70--105 files, +1,100..1,800/-900..1,500 shared C/H/test lines, App +8..20/-18..45. | Non-Audio exact-path hash proof, strict C11 dual-width Lib suites, INI-path focused proof and SoftPC background regression. |
| S3 | Adopt every non-Audio Common path/test exactly, without Core/App adaptation. | 27 differing shared paths plus derived manifest/CMake handling; estimate +180..320/-170..300. | Exact selected-path hash proof, strict C11 dual-width Common suites and product regression. |
| S4 | Adopt every non-Audio x86 path/test exactly, delete SoftPC's `command_runtime.h`, and retain the existing explicit optional x86 connection. | 19 source/test paths; estimate +1,200..1,500/-1,200..1,500. | Exact selected-path hash proof, strict C11 dual-width x86 suites, DOS/X command semantic regression and product regression. |
| S5 | Reconcile derived manifests and Audio-carved mixed CMake/README/verifier files; add one exact non-Audio path comparator, not a second source copy. | 8--14 build/docs/check paths; estimate +100..220/-30..120. | Six-package isolated build/test, no forbidden dependency edge and exact non-Audio comparator pass. |
| S6 | Perform task-level path ledger audit and byte-identity proof for all adopted non-Audio files; report all deliberate retained differences and close only after owner package acceptance. | Docs/build evidence only unless an S1 ledger item proves otherwise. | Dual-width regression, six-package proof, closure ledger and owner acceptance. |

Each implementation S is separately admitted after its predecessor is accepted.
No S may silently fold an NXVM Audio change into a non-Audio import.

## Acceptance ledger

| Unit | Allowed disposition | Receiver |
| --- | --- | --- |
| Every non-Audio source/test file in the six-directory universe | byte-equal import | S1 ledger, then S2--S6 exact-path proof |
| Every excluded Audio path | Audio-only exclusion, not partial import | S1 ledger/static path check |
| Public interfaces and component DAG | unchanged exact import or documented blocker | strict C11/DAG/package checks |
| SoftPC product integration | only App's imported Base EXE-directory call; no behavior change | INI-path proof, changed-path review and product regression |
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
platform process declarations. They implement executable-directory discovery.
The owner requires strict non-Audio convergence, so S2 imports them and moves
SoftPC's existing adjacent-INI lookup to this Base contract. SoftPC retains its
`softpc.ini` filename and current relative-media resolution; Base only supplies
the executable directory.

The sole SoftPC-only path is `x86/debug/command_runtime.h`. It preserves the
DOS DEBUG command body's original vocabulary while mapping it to SoftPC Types.
NXVM instead rewrites `x86/debug/command.c` directly to its current Types
vocabulary. The owner requires strict non-Audio convergence, so S4 deletes the
shim and adopts NXVM's direct source; public Debug headers remain unchanged and
the existing DOS/X command semantic suite becomes the receiver.

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

These failures prevent NXVM manifests from serving as the authority for the
import, but do not prevent a source/test import: they are derived hashes and
Audio intentionally makes root manifests differ anyway. S5 therefore generates
SoftPC manifests from its selected content and proves strict non-Audio source/
test equality with a dedicated comparator. This is one verification path, not
a competing source corpus.
