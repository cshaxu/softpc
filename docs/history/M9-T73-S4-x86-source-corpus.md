# M9 T73 S4: Separate x86 Source Corpus

Owner requested moving common/x86-debug and common/x86-xasm32 to x86/debug
and x86/xasm32, moving test qualification to S5, and defining six-/four-directory
shared transfer sets. Full request and finite ledger remain in the
[proposal](M9-T73-shared-x86-dependency-audit-proposal.md).

Baseline dc06671d; implementation 0ef82055; actual-change review fb07a0b7.
Owner reported 测试通过 and admitted S5. S4 closes; T73 remains open.

All eleven source files moved with Git. Names, build ownership, consumers,
manifests and dependency gates follow the x86 boundary. Reverse substitutions
prove all 22 changed C/H files are mechanical changes only. No Lib/neutral
Common runtime, Compat/MVDM, INI/media or snapshot changes.

Rename-aware production C/H +363/-363; tests +401/-401; both net zero.
Build/gates +200/-41 (net +159). Both Release builds passed; background
x64 108/108 (163.08s), x86 108/108 (148.73s), with five desktop tests per width
excluded. Standalone x86 build/verification passed. Owner manual acceptance
completes the exit criteria. Package hashes and auxiliary-build findings remain
in the proposal. Actual Git review and post-push manifest/DAG checks completed.

The explicitly temporary test/common -> x86 dependency is now owned by S5,
not deferred debt or a second production implementation.
