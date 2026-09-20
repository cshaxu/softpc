# M9 T74 Completion Audit

## Authority And Scope

Original owner request: rename mvdm to core, place vm in core/machine and
compat in core/compat, keep softpc.new at core/softpc.new; pure rename.
Owner now says: 测试无误。收口当前T任务。
Baseline d7c6931f; implementation 8b76baf5; reviewed delivery 5baad03b.

Frozen universe is the retained proposal's relocation/reference ledger and
the sole admitted S1. No additional S or unfinished migration remains.

| Requirement | Result and proof |
| --- | --- |
| Requested layout | src/core has machine, compat and softpc.new; old roots removed. |
| Preserved mirror | Closure rechecked all 498 mapped Git blobs; identical to baseline. |
| Pure relocation | 540 exact renames, 16 include-only renames; 107-file path-only C/H/RC proof. |
| Boundary preservation | App composition alone imports Machine public API; original controller private includes resolved without editing mirror; positive/negative gates pass. |
| S coverage | [S1 closed](M9-T74-S1-core-layout-rename.md), owner-tested and accepted; no S2. |
| Shared/user data | Closure rechecked six shared corpora, INI and media: no diff. |
| Verification | Both Release builds and background suites passed, 110/110 per width; owner reports no test issue. |
| Follow-ups | Four prior Queue candidates remain unadmitted; none is an unfinished T74 requirement. TODO remains empty under owner policy. |

## Counts, Review And Limits

Rename-aware endpoint diff d7c6931f..5baad03b, excluding docs/artifacts:
production 18 content-changed paths +34/-34 (net 0); tests/gates 30 paths
+195/-184 (net +11); CMake +190/-189 (net +1); tools +2/-2 (net 0).
Total 51 content-changed paths, +421/-409 (net +12), plus unchanged moves.
Detailed ledger and unchanged artifact hashes are in the
[retained proposal](M9-T74-core-layout-rename-proposal.md).

The closure review reconciled original request, all-S coverage, actual Git
diff, existing P2 review, mirror equality, protected-path equality and current
owner acceptance. Five desktop cases per width were excluded from automated
regression. No claim of universal runtime correctness or resolution of the
separately queued Machine shutdown-wake race is made.

T74 and S1 are closed. This closure changes documentation only and preserves
the tested EXEs. The owner's next Lib/Common audit is a distinct T75, with its
report pending; it is not silently included in T74.
