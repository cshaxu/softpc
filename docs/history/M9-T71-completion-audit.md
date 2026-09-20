# M9 T71 Completion Audit

## Owner Authority And Frozen Universe

Owner accepted S10: "测试通过，准许收口S10"; then requested:
"清空TODO所有项目，收口本T71. 队列第二位追加一个T任务，用于修复win95把软驱A盘识别为“可移动磁盘”而不是软驱的故障，Td治理。"

Baseline 7557ca5 through final accepted 5bfceb36. Code delivery f3ff1b11,
S10 coordinator review 449dad66. This is the separate T-level audit, not another
implementation S. Universe: original frame/mapping request, all admitted S1--S10,
resulting changed component paths and explicitly separated follow-ups.
Completion requires a proof or owner disposition for every row below; it does
not assert correctness of the entire emulator. No S11 is admitted.

## Request And S Coverage

| Member | Result and proof |
| --- | --- |
| Text-only Console admission | [S1](M9-T71-S1-console-frame-admission.md): reject unsupported graphics before mutation/wake; dual-width 109/109. |
| Symmetric leaf text extensions, VM mapping ownership | [S2 design](M9-T71-S2-frame-design.md), [S4 implementation](M9-T71-S4-frame-ownership.md): common text fields in Base; fonts in Window, character maps in Console; CP437 in VM; dual-width 110/110. |
| Leaf-owned commands, opaque FIFO | [S3](M9-T71-S3-control-transport.md): leaf validation/dispatch, transport only in Base; dual-width 110/110. |
| Fixed capacities, explicit failure rather than clipping | [S5](M9-T71-S5-frame-capacity.md): local validators and copy-frame status; dual-width 110/110. Producer regressions subsequently corrected by S7, not denied or hidden by reverting validation. |
| Default font-height cursor | [S6](M9-T71-S6-integration-audit.md): zero resolves to 16; focused before/after and dual-width 110/110. |
| Valid selected-renderer frame and preserved error state | [S7](M9-T71-S7-mode-transition-repair.md): readiness/dimensions/controller font and required-font result, ERROR not STOPPED; real Win3.1 initial PIF modes and roundtrips, dual-width 110/110. |
| Terminal completion of synchronous requests | [S8](M9-T71-S8-request-completion.md): admission and executor terminal cleanup synchronized, no new cancellation path; save not completed by ordinary pause; dual-width 110/110. |
| Final owner-approved frame transport | [S9](M9-T71-S9-latest-frame-delivery.md): complete frame, opaque latest-wins Base, leaf surface comparison; replaces the intermediate FIFO/dirty-chain proposal. No extra framebuffer/thread. Dual-width 110/110; subsequent background defaults 105/105 with five desktop tests explicit-only. |
| Four bounded interface/layout corrections | [S10](M9-T71-S10-text-contract-cleanup.md): original fixed-80 destination copy, cursor normalization, request/context documentation, delete destroy alias. Final background 105/105 each; owner manual pass. |
| Neutral text attributes | Separate [queued proposal](../proposals/m9-kvm-text-cell-glyph-refactor.md); not unfinished T71 implementation. No S11 started. |
| Three former TODO observations | Owner explicitly retires tracking; [Td disposition](M9-Td-S18-t71-closure-and-floppy-queue.md). No false repair claim. |
| New Win95 A: classification issue | New second-position [candidate](../proposals/m9-win95-floppy-drive-identification.md), not T71 regression proof or an admitted numeric task. |

## Final Changed-Path Accounting

Recomputed with git diff --numstat 7557ca5 5bfceb36 -- src test,
counting tracked C/H only; endpoint diff, not the sum of intermediate patches.
Manifest, CMake, documentation and binaries are excluded from these code counts.

| Component | C/H files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| App | 2 | 8 | 1 | +7 |
| Common | 7 | 223 | 111 | +112 |
| Compat | 2 | 27 | 5 | +22 |
| Lib | 23 | 341 | 289 | +52 |
| MVDM | 1 | 3 | 2 | +1 |
| VM | 3 | 114 | 69 | +45 |
| Production total | 38 | 716 | 477 | +239 |
| Tests | 34 | 1220 | 358 | +862 |

Changed paths map to the ledger: App command error handling; Common frame
definition/Machine completion/Session/UI distribution; Compat selected-renderer
frame readiness/fonts; VM typed frame assembly and character mapping; Lib leaf
frames/control/validation/rendering and Console lifecycle; MVDM only nt_cga.c
fixed-destination copy. No standalone guest-specific branch was added there.
That file's total OpenNT difference is +21/-3 per S10 evidence, of which this T
adds +3/-2. This T-level review does not re-audit unrelated historical mirror diffs.

## Final Evidence And Limits

S10 Release builds succeeded on both widths. Latest serial background suites:
x64 105/105 (163.44s), x86 105/105 (145.85s), including actual linked painter
matrix, cursor conversion/native mocks, request termination, headless Win3.1
roundtrips, restart and snapshots. Owner accepted the delivered manual test.
Five desktop-only tests per width were not rerun in S10; no native Linux
verification or pixel-exhaustive claim. Earlier transient modal-test failures
remain historical observations, not repaired facts.

Unchanged delivered packages:

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3655192 | F3A891A9FE4274A0DB441380BAEC4DA9C132A6E4E3E9636F0039D75F5BC03CD3 |
| softpc64.exe | 3058735 | 8C4CEC14C777AA5FE9355046275AFF8A4581890BDED43D2FADB3DBD388C044F0 |

## Closure And Retrospective

T71 is closed by owner approval. Its [original proposal](M9-T71-kvm-text-frame-contract-proposal.md)
and [regression brief](M9-T71-mode-transition-regression-proposal.md) are retained
as history. CURRENT alone owns present state.

The migration initially mixed ownership changes with stricter behavior and missed
producer validity. S7--S10 restore a coherent selected-frame boundary, terminate
pending requests on failure and remove reliance on skipped dirty history.
Lesson: validator tests alone cannot prove the producer or real mode transition;
retain before/after regressions and real roundtrips. No new layer is justified
merely by a stricter check. This closure changes no source, tests, binaries, INI,
media or snapshot format and does not rerun runtime tests.
