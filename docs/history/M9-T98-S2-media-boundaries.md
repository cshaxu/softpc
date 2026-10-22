# M9 T98 S2 - Media failure and capacity boundaries

Owner admission: 批准执行当前既定修复任务; correctness before performance.
Owner acceptance on 2026-10-18: 验收通过 收口s任务 保持t开放.

Baseline 3b368d26. Executor delivery 7afc9981; actual-change coordinator review
db935fb7. Both are pushed. The [S2 ledger](M9-T98-nonpublic-quality-performance-proposal.md#s2-implementation-and-finite-qualification-ledger)
records the finite scope, every media-close/size-path disposition and proof limits.

F1/F2 are complete: real FDD/HDD adapters propagate consumed-lease retirement
failure while finishing ownership/metadata cleanup. HDD attach and replacement
restore reject sizes beyond the existing signed-offset boundary; transfers
validate remaining sectors before multiplication. FDD restore shares attach's
existing LONG_MAX limit. No retry, rollback, second backend, public contract or
mirror modification was introduced. Void teardown retains its original ABI.

Reproducible accounting against 3b368d26 via git diff --numstat: production
hdd_media.c +22/-9, gfi_image.c +8/-7; total +30/-16, net +14. Three test files
+195/-0, net +195; root CMake +3/-2, net +1. No INI, guest media or snapshot edits.

Both cached full builds and EXE releases completed. Each architecture passed
193/193 registered cases: 177 unit/gates, eight background integrations, five
desktop and three native audio. No failures/skips; XML and registration sets
match exactly. Evidence: build/t98/s2/qualification, qualification.exit=0.
Close errors and extreme sizes use injected real-adapter units, not a native
close failure or real 2-GiB guest disk. Existing guest snapshot/restart/package
integrations pass. Existing Ninja recovery and original compiler warnings
remain disclosed, not repaired by S2. Both EXE hashes remain in CURRENT.

S2 is closed after owner acceptance. T98 stays open; no S3 admission or new
implementation occurs in this closure. User-owned untracked snapshot.debug is
preserved and excluded. This closure changes documentation only; the accepted
runtime, tests and artifacts are unchanged.
