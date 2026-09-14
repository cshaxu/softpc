# T58: Common Test Corpus Synchronization

## Original request and boundary

“准入一新的T任务，引入nxvm的test/common/保持两边内容一致。
完成后作为S任务收口，保持T任务开放等我测试测试。不得改动src。”

Baseline a1c24ce, clean worktree. S1 imports the complete test/common snapshot
from the read-only NXVM checkout (HEAD 4da5d23c), with no src changes and no
runtime/build dependency on that checkout. No product semantics, configuration,
media or shared production source changes are admitted.

## S1 finite ledger and exit

Universe: every relative file path in both test/common trees at admission.
There are 16 files on each side: 15 identical and session_frame_smoke.c with
additional monitor callback assertions in NXVM. Preserve the 15 unchanged;
import that one file exactly, then recheck all paths and SHA256 values.
Source tree must have zero diff against the baseline. Run dual-width builds
and full suites, including the focused frame test. Fixed EXEs remain the
only package outputs; no INI or media edits. Stop if tests require src changes.

One executor P contains import and verification evidence, is committed/pushed,
then actual-diff review accepts S1 in a second P. T58 remains open with no
next S admitted while the owner tests. The XP candidate and existing TODOs
remain separate. This is a test-only import, not an NXVM architecture migration.

## Verification checkpoint: upstream manifest mismatch

The imported file matches NXVM SHA256
9D3BA1ED5437C6CC490A856B4DC4EFDC953B690C7CD3201AB016B9C716B20F2D,
but its unchanged manifest records
12F088CC881F3B70B1F1AA57A2CDCF38331B8B0B4CDE32E62F5DAADA04E61CA0.
The same verifier fails on the NXVM test directory itself. Both-width builds
succeed and Common tests pass 15/16 at each width; only common.test-manifest
fails. The new session_frame assertions pass at both widths. No src changes.
Test delta is one file +10/-1; no runtime changes or new license claim.
NXVM's root MIT notice remains upstream provenance; this bounded test update
does not alter existing notices or the repository's license policy.

Exact import and successful manifest verification cannot both be claimed for
this snapshot. Await NXVM manifest correction or owner direction on a local
manifest correction followed by NXVM synchronization. Do not suppress the
verifier, commit a passing claim, or close S1 while this mismatch remains.

## Owner-approved resolution

“批准，以softpc这边为准，nxvm后续从softpc拿代码。”
SoftPC corrects the single stale manifest hash without changing the imported
test or src. NXVM subsequently adopts this corrected corpus. Exact equality
to the old upstream manifest is superseded by this explicit owner decision.
All other 15 files remain equal to the admitted snapshot.

To represent the requested S-closed/T-open wait without a fictitious next S,
the product documentation verifier accepts the explicit idle marker
`Open task awaiting owner: T58.` only for the latest recorded, non-closed T.
Positive and missing/wrong/already-closed negative fixtures cover this state.
This supporting tooling correction does not modify src or shared test files.

## Executor evidence

Only test/common/session_frame_smoke.c (+10/-1) and its manifest hash (+1/-1)
change in the transferred corpus. The imported test remains byte-identical to
NXVM; the approved manifest correction is the sole downstream difference.
Product source accounting: zero. Governance tool accounting: +28/-2, covering
one idle-wait check and four deterministic positive/negative fixtures.
All previously existing assertion and manifest checks are retained.

Both checked-in build presets completed successfully. Fixed executable hashes
are unchanged from T57, since this task has no production-source change.
x86 full regression passes 85/85 (58.60 seconds); governance-focused rerun
passes 2/2 after the tooling change. The first x64 full run passed 84/85:
compact Console integration timed out at stage 5, while every Common test,
including the corrected manifest, passed. Both widths had been running
concurrently; that is context, not a proven cause. Serial x64 verification
follows without product or assertion modifications.

Serial x64 full regression passed 85/85 (49.60 seconds). The first stage-5
failure remains an observed baseline-package timeout, not a repaired defect;
it is transferred to the package acceptance TODO. Final comparison confirms
src/lib 94/94, src/common 36/36, test/lib 39/39 identical; test/common has 16
paths and only the approved manifest differs. Source diff from a1c24ce is zero.
Governance checker/self-tests and the real Common test manifest pass.
