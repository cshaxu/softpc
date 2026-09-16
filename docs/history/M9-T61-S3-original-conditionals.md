# M9 T61 S3 — Restore original unselected conditionals

## Request and executor delivery

Restore ios.c NTVDM blocks and host_cpu.h non-CCPU declaration layout while
preserving the selected standalone CCPU ABI. P1 `dc71195` restores two files:
production +82/-10, net +72; test source zero. This reduces original-source
content diff by 92 changed lines and four hunks, rather than adding behavior.
ios.c now has original content; raw checkout line endings remain separately
accounted in the [ledger](../etc/evidence/softpc/pristine-divergence-current.md).

Both real builds select CCPU without NTVDM. Actual build.make plus dependency
files enumerate 57 affected compilations per width; preprocessing token hashes
match before/after for all 114. CCPU declarations and small-endian register
layout are untouched. Restored unselected branches are not claimed runtime
tested. Both EXEs have identical .text/.data/.rdata sections to S2.

Dual builds pass. x64 first full run was 96/97: package-smoke stage 14 did not
observe Version after ver despite reaching the DOS prompt. The unchanged
executable passed five subsequent package repetitions, then full x64 97/97;
x86 full 97/97. The original failure is retained rather than relabeled green
or claimed repaired. No production/test workaround was added. Final EXE
hashes and totals are in the ledger; temporary probes were removed.

## Coordinator review

Reviewed pushed dc71195 actual source changes, generated-path selection,
per-consumer proof, package section comparison and full results. Similar
removed-directive hits in reset/c_main/ica.h/ios.h/cfpu_def.h remain required
active contracts, not omitted restoration candidates. No Lib/Common/shared
tests, App/VM/Compat, INI, media or ABI changes. Documentation and diff gates
pass. Post-commit IRQ/serial/keycode/BOP tests pass 4/4 on each width; HEAD
matches origin/main and the reviewed worktree is clean.

S3 closes; S4 is admitted for the original keyboard branch assessment and
equivalent minimization. T61 remains open.
