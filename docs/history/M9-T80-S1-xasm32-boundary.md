# M9 T80 S1: xasm32 bounded instruction access

## Admission and scope

Owner: “测试通过 可以收口t任务 准入下一个t”, then “开始执行”.
The admitted [proposal](M9-T80-shared-corpus-boundary-and-simplification-proposal.md)
owns the seven-row plan and S1 pre-change estimate, finite sweep and evidence.
Baseline `295c6963`; executor `73a5a889` and review `f4941481` are pushed.
Owner subsequently requests “收口s1 准入s2”. S1 is accepted and closed;
T80 remains open, and S2 is admitted through CURRENT.

## Actual-change review

After the executor push, the same session switched to coordinator and reviewed
`git diff 295c6963..73a5a889` for all twelve changed paths, not only its summary.

| Changed owner | Review and result |
| --- | --- |
| dasm32.c | One existing skip guard controls both skipping and pre-read admission. All six peeks restore only their saved valid position. No post-access guard, duplicate decoder, or new state. F1 was the only NULL map slot; it now uses the existing undefined-opcode path. |
| aasm32.c | Three existing write widths reject before mutation; prefix emission is bounded to eleven bytes and the final payload copy checks remaining capacity. The existing error flag also reaches paragraph assembly. |
| Public API/debug callers | Unchanged. At least fifteen input bytes, success-only output publication, original U/XU command and error presentation remain. Existing undefined-opcode/prefix behavior is not redesigned. |
| Four test/build paths | Guard-page test, all map entries at each remaining-byte count, truncated operands, 48 assembly write-position cases, U/XU rejection/recovery, and existing legal output/ModRM/SIB tests. Private assembly inclusion is test-only, no production bypass. |
| Two manifests | Only changed source/test hashes and the new test member; both corpora validate. |
| Two EXEs | Match the final tested Release outputs and proposal hashes; no INI/media changes. |
| Two documents | Admission scope and bounded implementation evidence; no task/rule expansion. |

Production: two files +11/-2, net +9. Tests C: three files +148/-0;
test CMake +1/-0; combined test/build net +149. Manifests, documents and EXEs
excluded from code counts. Reproduce with `git diff --numstat 295c6963..73a5a889`
and the explicit paths in that commit. Production is smaller than the +22
estimate because the original guard/return path was reused; test growth covers
the additional confirmed F1 hole and write-boundary sweep.

## Verification and limitations

- Original guard-page probe faults on byte sixteen; repaired probe passes.
- 130,050 original/new two-byte combinations preserve output and length,
  excluding original F1 NULL entries; not an exhaustive ISA proof.
- Independent x86 suite: x64 10/10, x86 10/10. Both strict C11 with
  `-Wall -Wextra -Wpedantic -Werror`.
- Final background regression: x64 111/111 (136.13s), x86 111/111 (140.98s).
  Five desktop cases excluded each; no Linux runtime claim.
- An intermediate startup-roundtrip timeout and corrected test expectations
  are disclosed in the proposal; final source was frozen before the final runs.
  No unrelated timeout or runtime patch was added.
- Manifests, dependency gates, documentation governance and diff-check pass.
  Task-owned diagnostic/independent-build children were removed after process
  completion; only normal build trees and package EXEs remain.

No remaining in-scope implementation gap found by this review. Owner closure
approval completes S1 acceptance; this does not infer whole-library correctness
from the bounded S1 proof. Closure/admission changes documents only, preserving
the accepted EXEs and their hashes.
