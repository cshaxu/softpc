# M9 T82 completion audit

## Scope and result

T82 made SoftPC's six reusable corpus directories ready for direct adoption by
NXVM, excluding Audio from the import decision:

- `src/lib`, `src/common`, and `src/x86`;
- `test/lib`, `test/common`, and `test/x86`.

The retained [proposal](M9-T82-nxvm-shared-corpus-import-proposal.md) records
the admitted S1 audit and S2 import scope.  S1 established the comparison
ledger; S2 imported the approved non-Audio corpus, aligned the shared test
infrastructure, regenerated the six local manifests, and retained SoftPC's
correct x86 negative-test coverage.

The owner accepted the result and requested T82 closure.  This is a corpus
alignment task, not a claim that either repository becomes a build, runtime, or
acceptance dependency of the other.

## Delivered changes

The final implementation consists of two import revisions:

| Revision | Result |
| --- | --- |
| `2ef228f8` | Imported the approved NXVM non-Audio corpus; adopted `lib/base/process`; updated SoftPC's App configuration discovery to use the shared process helper; removed the obsolete x86 DEBUG runtime wrapper; and aligned test/component boundaries. |
| `0448fa4f` | Adopted NXVM's shared Lib-test aggregate target and clearer test README, then regenerated the six local corpus manifests. |

The App remains the product assembly boundary.  Its only related behavior
change is using `base_process_executable_directory()` before appending
`softpc.ini`; Common, Lib, and x86 components retain no SoftPC-specific
configuration path policy.  Core, Compat, VM, user INI, guest media, and
runtime assets were not changed by T82.

## Final corpus ledger

The final comparison against NXVM's current six-component working corpus
contains 220 tracked paths:

| Comparison | Paths |
| --- | ---: |
| Byte-identical source, headers, tests, CMake, and documentation | 214 |
| Deliberately local manifest files | 6 |
| Missing paths on either side | 0 |

The six remaining differences are exactly:

- `src/lib/MANIFEST.sha256`, `src/common/MANIFEST.sha256`, and
  `src/x86/MANIFEST.sha256`;
- `test/lib/MANIFEST.sha256`, `test/common/MANIFEST.sha256`, and
  `test/x86/MANIFEST.sha256`.

They carry SoftPC's local corpus-revision marker and hashes for the final
local corpus.  They are provenance records, not implementation divergence.
Audio was excluded from T82's import scope; NXVM subsequently adopted the same
Audio content, so it is also byte-identical in the final six-component
comparison without a T82 Audio change.

## Changed-path accounting

Compared with the admitted S1 baseline `df9d1cf8` and the implementation
endpoint `0448fa4f`:

| Category | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production corpus | 73 | 1,746 | 1,654 | +92 |
| Test corpus | 80 | 824 | 754 | +70 |
| Documentation | 3 | 80 | 20 | +60 |
| All text changes | 165 | 2,777 | 2,520 | +257 |

The net change is corpus synchronization, not new SoftPC product behavior.
The limited production delta includes the shared process helper and the removal
of SoftPC-only x86 wrapper structure; test changes align reusable test entry
points and corpus verification.

## Verification

- Both x64 and x86 Release packages built from the S2 import.
- Background CTest completed all 116 intended cases on each width.  The x64
  ledger is 115 cases from the complete run plus the corrected x86 negative
  gate; x86 completed 116/116 in the complete run.
- After P2, x64 and x86 targeted manifest, corpus, and negative-test gates all
  passed, including the shared Lib-test aggregate target.
- Component DAG/corpus verification, documentation governance, and
  `git diff --check` passed.
- Desktop-interactive tests remain intentionally outside the background test
  routes; no Linux runtime or NNES integration claim is implied by this
  SoftPC-side corpus closure.

The packages made for S2 remain the accepted dual-width package artifacts; this
closure itself is documentation-only and does not replace them.

## Closure disposition

All admitted T82 scopes are complete and accepted.  The active packet no
longer carries a T82 subtask; TODO is empty.  The sole remaining Queue candidate
is the unrelated XP SP1 SoftPC mirror rebase.  T82 is closed.
