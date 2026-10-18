# Repository-local build caches and disposable task work

Owner requests that ccache live in build/.ccache, Ninja trees in build/.ninja,
and everything else below build be disposable when its owning work completes.
Owner admits M9 Td S22: 准入一个td任务执行治理. T98 remains open.

Finite ledger: (1) persistent directory/preset layout; (2) ccache command routing
including direct build invocations; (3) explicit safe cleanup and residue gate;
(4) current instructions and closure obligations; (5) real dual-width cache/build
proof; (6) retirement of old generated trees/probes/logs. Each needs implementation,
focused proof and a recorded disposition before delivery. No runtime change.

Plan: reuse one canonical Ninja tree per architecture, whether ccache is enabled
or not. Never move an old CMake tree because its generated files retain absolute
paths. Retain optional ccache rather than silently requiring a new user dependency;
when selected, the generated launcher fixes its repository cache/temporary roots
and bounded size. Makefiles remain an explicitly disposable fallback.

The cleanup tool lists by default, requires exact build-child names and explicit
apply/quiescence confirmation to delete, rejects persistent roots and reparse
points, and checks closure residue. No cache manager, background janitor or
global cache purge is added. Global ccache can contain other projects and is not
safe to relocate wholesale; only this repository's future writes are confined.

Preserve compact verification summaries in task history before discarding raw
logs. Existing historical evidence paths are historical references, not retention
requirements. Never reclassify task recordings as a cache to evade cleanup.

Expected scope: roughly 10-14 root configuration/tool/document paths; zero C
production/test or mirror diff; scripts/gates/regressions about +200-300 lines.
CURRENT owns execution and budgets; final accounting uses git diff --numstat.

## Legacy retirement admission and retained checkpoint

The owner authorizes disposal of completed work outside the two persistent
roots. Freeze the legacy build children before cleanup; tools preflight exact
paths and reject links/Git/user configuration. A cleanup job may run alongside
validation only on these disjoint old children: it cannot delete .ninja,
.ccache or the active td-s22 scratch. Matching live-process references block
deletion; no foreign process is killed. Both jobs use separate bounded logs.

Retain this checkpoint before deleting old raw evidence: T98 S2 delivery
7afc9981, review db935fb7, owner closure 733a7f6e; both architectures passed
193/193 with no failures/skips, exact XML/registration match. Its production
+30/-16 and test +195/-0 accounting and proof limits are in the S2 history.
S1 reconstructed all 131 patched XP-subset hashes; deterministic checks and
mirror manifests remain reproducible from tracked source. No measured runtime
performance improvement was claimed. The old x86 .ninja_log is version 7,
212184 bytes after S2; repeated recovery warnings were observed, not diagnosed.
That metadata is disposable, not a new retained cache exception.

Legacy generated classes include old mingw/ninja build variants, lib CI/MSVC
proof trees, closed t93-t98 scratch and root diagnostic text. Summaries are
retained in tracked history/proposals; these old raw paths cease to be available
after retirement. Their deletion is not a runtime repair or a cache-hit claim.

## Executor delivery, 2026-10-18

All six ledger members are implemented and verified:

1. Presets share the canonical build/.ninja/gcc-x64-release and gcc-x86-release
   trees; noncanonical Ninja configure paths fail rather than creating variants.
2. Generated ccache commands pin build/.ccache, its tmp child and a 5-GiB limit.
   A fresh miss/hit probe succeeded. Direct builds of both architectures also
   hit that cache while hostile caller cache/temp paths remained absent.
3. Exact-name cleanup rejects cache roots, traversal, reparse points, Git data
   and protected output configuration. Safety regressions passed; the idle
   documentation gate rejects disposable residue.
4. Execution, source layout and contributor/quick-start instructions now use
   the same paths and require evidence summarization followed by cleanup.
5. Fresh dual-width test builds each completed 684 edges; second builds reported
   no work. x64 and x86 each passed all 180 registered unit/gate tests, with
   exact registration/XML equality and no failures or skips. Test wall times
   were 113.24 and 91.82 seconds. After final safety changes, both architectures
   passed all five affected gates and the clock smoke again. These are not
   whole-product timings or a measured warm-cache speedup: the initial local
   cache had 28 hits/976 cacheable calls, mostly cold misses.
6. Thirty obsolete direct build children were retired, including legacy build
   variants, t93-t98 scratch and diagnostics. One deletion initially stopped
   correctly on an embedded Git repository. Its local SoftPC origin, clean
   checkout at ancestor 5fbe692 and absence of reparse points were verified
   before explicit disposal of that old T50 checkout proof. The generic tool
   still refuses Git data. No global cache or user data was removed.

The final owned scratch build/td-s22 was removed after pushed-change review;
the residue check passes with only .ccache and .ninja remaining.
Raw logs are disposable; this summary preserves their outcomes. Generated files
deleted here are reproducible, but raw logs are not recoverable from Git.

Scope: root build/configuration/tooling and governance only. C/H production,
C/H tests, public component trees and the preserved mirror have zero diff.
Native desktop/audio/package integrations were not rerun; neither EXE was
rebuilt or published for this governance-only change. Both package hashes
remain the accepted T98 S2 hashes recorded in CURRENT. Existing compiler
warnings and the historical Ninja recovery debt are not claimed fixed.

## Coordinator actual-change review

Reviewed pushed executor d6523386 against baseline 733a7f6e and the owner request,
not just its summary. All 13 changed paths belong to root build governance;
git diff confirms src, test and assets have no changes. Independently counted
180 XML cases per architecture with zero failure/error/skipped nodes; checked
job exit codes, both no-work builds, actual direct cache-hit logs and the safe
cleanup outcome. No extra runtime owner, background cleanup service, global
cache mutation or new public dependency was introduced. Review found no blocker.

Reproducible executor accounting: git diff 733a7f6e d6523386 --numstat gives
+421/-27, net +394 across all 13 paths. Of this, root build configuration,
tools and check regressions are +236/-6, net +230; remaining changes are docs.
C/H production and C/H tests are +0/-0. The larger text total includes the
admission, retained retirement evidence and this review, not extra runtime code.

Final process check found no owned background script running. Cleanup removed
the exact validated task directory, then Clean-Build.ps1 -Check passed.
User snapshot.debug remains untouched and untracked; no other worktree changes
are excluded. Td S22 awaits owner acceptance; T98 is not closed or advanced.
