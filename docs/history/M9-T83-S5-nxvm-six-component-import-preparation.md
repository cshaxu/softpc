# M9 T83 S5: NXVM six-component import preparation

## Objective

Inspect the current NXVM working corpus under `src/lib`, `src/common`,
`src/x86`, `test/lib`, `test/common`, and `test/x86`, including uncommitted
files. Audit it against the shared-component governance contracts before any
later import is considered. This S is read-only and does not copy NXVM files
into SoftPC.

## Frozen comparison universe

The unit of coverage is one Git-tracked or visible untracked path in either
repository below the six named roots, with `MANIFEST.sha256` classified as a
local provenance record.  The NXVM reference is its visible working tree,
anchored to commit `3c410c2f4`; the audit records both tracked modifications
and untracked paths, so a later import can require a stable source revision.

Every path receives one disposition:

| Disposition | Meaning |
| --- | --- |
| Identical | Same normalized content in both corpora. |
| Line-ending only | Content differs only by CRLF/LF normalization. |
| NXVM semantic update | NXVM has a meaningful source/test/CMake change that requires adoption review. |
| Local divergence | SoftPC differs for a recorded product or test-boundary reason. |
| Manifest-only | Local revision/hash record; regenerated only after a separately admitted import. |
| Blocked | The path needs a frozen NXVM commit, outside-root dependency resolution, or ABI/product-policy decision before import. |

## Required quality evidence

1. Record path counts and an exact normalized-content comparison for all six
   roots.
2. Separate NXVM's tracked semantic changes from its line-ending-only churn.
3. Run NXVM's applicable manifests, source-boundary, Types-layout and
   Types-boundary checks against the visible working tree; report failures as
   findings, never as a SoftPC defect.
4. Inspect changed public headers and CMake/test registrations for consumers
   outside the six roots, and name each dependency or ABI consequence.
5. Do not alter NXVM, `softpc.new`, SoftPC shared source/tests, user INI,
   media, package artifacts, or unrelated SoftPC test output. Retain all
   evidence in the S5 closure audit.

## Readiness predicate

S5 is ready to hand off only when every visible NXVM change has a disposition,
every applicable quality gate has a recorded result, and no unreviewed
dependency crosses the six-component boundary. A dirty source or failed gate
is a blocker, not an import-ready result.

## Initial quality audit — awaiting owner review

SoftPC `3ae07afb` was compared read-only with NXVM's visible working tree on
committed base `3c410c2f4`. The six-root universe has 225 paths: 144 exact
matches, two CRLF/LF-only differences, 68 semantic differences, six manifests,
and six NXVM-only untracked paths. NXVM's six roots have 74 tracked changes.

The Lib/Common manifests, x86 corpus gate, Lib component DAG, Types layout,
KVM naming gate, and `git diff --check` pass. The new
`test/lib/verify_types_boundary.cmake` fails. Its reported first cases are raw
`LONG` in `test/lib/cleanup.h` and raw `LPCWSTR` in
`test/lib/kvm_window_retirement_smoke.c`; an independent sweep also finds raw
`LONG`, `LPCWSTR`, `SIZE_T`, and `SHORT` in the changed shared tests.

The change adds neutral Types-owned platform/C aliases and test wrappers, then
migrates Lib/Common/x86 tests to them. No Common/x86 production source changes.
It is not import-ready because the new gate fails and the source is dirty.
After NXVM corrects all Types-boundary findings and commits its six untracked
files plus 74 tracked updates, S5 will re-audit the named commit and, after
owner approval, import that exact six-component corpus atomically.

## Re-audit — 2026-09-24

NXVM is now clean at `440ae83bc` (`MyNES M6 T43 S3 P2: accept Types correction
and refresh artifacts`). Its six roots have no tracked or untracked delta.
Lib/Common manifests, x86 corpus, Lib component DAG, Types layout, KVM naming,
the shared-test Types boundary, and `git diff --check` all pass. A direct sweep
for the previously retained `LONG`, `LPCWSTR`, `SIZE_T`, `SHORT`, and Win32
pointer vocabulary in non-fixture shared tests is empty.

The prior blocker is resolved. This is now the immutable candidate for the
owner-approved, exact-corpus S5 import; no SoftPC corpus path has been copied
or changed yet.

## Owner-approved exact import

The owner approved importing the original NXVM corpus after the clean re-audit.
S5 copies every path in the six roots from `440ae83bc`, including the six
manifests and the seven paths absent from SoftPC. It permits no SoftPC-local
adaptation: the post-copy byte ledger is the acceptance condition. The existing
T82 provenance records this same locally maintained shared corpus relationship;
this S introduces no new external source, runtime dependency, notice, media,
or firmware.
