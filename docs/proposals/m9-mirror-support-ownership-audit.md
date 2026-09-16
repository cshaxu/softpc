# Remaining mirror and support ownership audit

Owner admitted continuation of the open mirror-minimization task. The active
identifier and boundary are solely in [Current](../states/CURRENT.md).
The original request and expanded bilateral ownership scope are retained in
[the task plan](../history/M9-T61-mvdm-diff-minimization-proposal.md#s7-追加剩余差异及-vmcompat-重复支持审计).

Audit the remaining OpenNT mirror differences, all VM/Compat files, duplicated
original/local support and potential additional original-file reuse. Include
Mirror-to-VM/Compat, VM/Compat-to-Mirror and VM-to-Compat responsibility checks.
Use actual consumers, build selection and state ownership, not path names or
line counts alone. Preserve all current product and D6 BOP behavior.

The completed audit was read-only production research. Findings and finite coverage belong to
[the existing evidence ledger](../etc/evidence/softpc/pristine-divergence-current.md#s7剩余镜像与-vmcompat-双向归属审计).

## Owner-approved implementation sequence

Original request: “请拆解一下S任务 并更新当前proposal记录 然后开始准入执行”。
Owner continuation: “执行到S13完成当前 mvdm/vm/compat 虚拟机核心组件的清理工作，使得每个组件有明确的产权和边界，每个代码文件和符号有正确的归属；vm和compat冗余无用代码应删尽删，能复用mvdm的尽可能复用mvdm。”
Additional requirement: “每个S任务执行前先审计，预估需要移动的文件/功能/行数，预估可以达成的diff和自主实现的增减情况；结束后也进行同类实际汇报。”
Before each remaining S, record its audited paths/functions, relocation count,
estimated original-mirror diff and independently maintained production/test
delta; after execution compare actual counts against the estimate. Pure moves
are reported separately from deletion/reimplementation. Estimates are not proof.
Continue within T61, serial admission only after the previous exit review.
Lib/Common and their shared tests stay unchanged. No wholesale external source
import, generated source overlay, guest behavior change or D6 withdrawal.

| Step | Scope and method | Exit proof / stop boundary |
| --- | --- | --- |
| S8 | Remove the six uncalled DIB/keyboard/XMS helpers and declarations; consolidate scancode dispatch, floppy path commit and HDD range validation locally. | Whole-tree caller census; existing input/media/palette/restart tests plus full x86/x64 suites, refreshed fixed EXEs. No mirror change or new abstraction. |
| S9 | Retire test-only status shell, unused presentation option, single-font wrappers and unselected PIG header; migrate tests to retained production contracts rather than losing coverage. | No production/test/build references to removed interfaces, explicit replacement of obsolete shutdown proof, VGA/font and full dual-width tests. Keep any item whose independent contract is demonstrated. |
| S10 | Unify GDP/SAS original declarations with the already retained mirror; keep pointer-width mechanics in existing Compat support. | Prove each translation unit's selected header, GDP expansion and SasVector slots before editing; report exact original diff cost. Prefer removing demonstrably unused slots; if new mirror ABI differences are required, present the precise alternative before adopting it. Dual-width layout/video/full tests. |
| S11 | Move concrete machine lifecycle/config backend and debug-request preflight into VM; keep original host callbacks, memory allocation and media endpoints in Compat. | git mv, no forwarding duplicate, no Compat-to-VM header edge, update architecture and source ownership gate; reset/restart/debug atomicity and full dual-width proof. No rewritten core initialization. |
| S12 | Resolve the duplicated V7 geometry inference by proving whether original surface geometry suffices; move the small host clock body out of the mirror into existing audio support where equivalent. | V7 60h–69h, standard VGA, partial dirty and transition proof precede deletion; compare original diff and preserve sound timing. If geometry equivalence fails, retain the needed behavior and report the evidence rather than guessing. |
| S13 | Whole-task convergence review against the S7 finite ledger and owner requests. | Every candidate implemented or explicitly justified; no duplicate production path, dual-width package/full regressions, original diff and production/test accounting, clean committed/pushed tree. T closure requires owner acceptance. |

S8–S12 each deliver one complete implementation P, build/test both widths,
refresh assets/binary/softpc32.exe and softpc64.exe without editing the INI,
commit/push, then review the actual commit in the coordinator role. No mere
relocation is counted as code reduction. Original-import candidates rejected
in the audit remain rejected, not silently included in this sequence.
