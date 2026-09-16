# M9 T61 S13: ownership cleanup convergence

## Request and coverage

Owner objective: “执行到S13完成当前 mvdm/vm/compat 虚拟机核心组件的清理工作，使得每个组件有明确的产权和边界，每个代码文件和符号有正确的归属；vm和compat冗余无用代码应删尽删，能复用mvdm的尽可能复用mvdm。”
Owner also requires before/after file, function, relocation, original-diff and
independent-code accounting for each S, permanent push permission, and no
Lib/Common changes. T closure remains subject to owner manual acceptance.

The [serial plan](M9-T61-S7-support-ownership-proposal.md) and
[final ledger](../etc/evidence/softpc/pristine-divergence-current.md#s13-最终归属复核)
map all S7 A/B/C candidates, all six ownership directions, 48 original support
files to 45 current files, and all 498 mirror files. Removed items and retained
distinct contracts have explicit reasons; rejected wholesale original imports
remain rejected rather than being silently counted as unfinished cleanup.

## Actual-commit coordinator review

Evidence delivery `891e642` is pushed. Reviewed S8–S12 production/test/build
diffs and source ownership: original CPU/device/renderer remain MVDM; concrete
backend and Common translation/debug belong to VM; host endpoints and width ABI
belong to Compat. No duplicate initialization, GDP/SAS copy, status shell,
single-font path, media enum or geometry inference remains. App has only the
composition VM entry; Compat has no VM/Common dependency. Original keyboard
table bounds TODO is a distinct behavior repair, not hidden incomplete cleanup.

S13 production/test/build delta is zero as estimated. S8–S12 total production
26 paths +102/-1949 = -1847; tests 24 paths +246/-131 = +115; CMake +8/-6 = +2.
606 relocated backend lines are not counted as deletion. VM has 12 files/1501
lines, Compat 33/4866. Original mirror: 498 retained, 404 byte-identical,
94 divergent, +23132/-22334, 5131 hunks; raw and trailing-whitespace-ignored
counts agree after preserving original line endings. C-VID inverse proof 34/34.
No new imports or changes to the four shared directories/media. The sole INI
change since S7 is the owner's 0→1 setting committed unchanged in S9.

Final dual-width builds/full suites: x64 98/98 (56.65s), x86 98/98 (58.31s).
After `891e642`, coordinator reran product/build negative gates, standalone source,
documentation governance, VGA, sound-state and actual runtime restart: x64
7/7 (9.54s), x86 7/7 (9.97s). Checks cover their named contracts, not a claim
that every possible guest behavior is defect-free. S12 remains the executable
code baseline; final builds reproduce its fixed EXE hashes in the ledger.
Temporary owned diagnostics/logs were removed; existing build trees retained.

S13 is closed. All requested S8–S13 cleanup is complete; T61 stays open with no
active S, awaiting owner testing and explicit T closure. Queue and TODO unchanged.
