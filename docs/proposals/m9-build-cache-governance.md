# Repository-local build caches and disposable task work

## Owner request and active scope

Owner admits M9 Td S22: 准入一个td任务执行治理. Persist only build/.ccache
and build/.ninja; dispose of completed task scripts, probes and logs elsewhere
under build. T98 stays open. No runtime/public-corpus/mirror, media, INI or EXE
change, global cache purge or new build framework.

Owner reopens this same unaccepted S: 请你重开本S，把编译规范、脚本、门禁治理干净！
Reopen baseline 5fa2995c; original baseline 733a7f6e. CURRENT is the active
packet; Execution owns cleanup policy and Coding owns the output layout.

## Retained design and finite correction ledger

1. Real configure owns Ninja containment under build/.ninja and generated
   ccache commands own build/.ccache, its tmp child and a 5-GiB bound.
   Keep the shipped x86/x64 preset trees shared with their ccache variants.
   Do not force custom configurations to use only two exact GCC names.
2. Delete the second preset parser, synthetic negative fixtures and their
   registrations. Verify actual configure and generated build behavior instead.
3. Clean-Build.ps1 alone owns exact-name deletion and the closure residue gate.
   Keep traversal/cache-root, reparse, Git and user-config protection, dry-run
   and explicit quiescence confirmation. Documentation gates check documents,
   not another implementation of filesystem cleanup policy.
4. Retain cleanup regression scenarios but invoke the actual script in-process,
   not thirteen child shells. Verify all x86/x64 unit/gates and remove owned
   scratch after summarizing evidence and pushed-change review.

No cache manager, retry state machine, new helper layer or background janitor.
Global ccache may contain other products; only future SoftPC writes are pinned.
Never move old CMake trees with absolute paths. Completed raw evidence paths are
historical references, not retention requirements or hidden persistent caches.

Estimate at reopening: 8-10 paths; build/check code net -40 to -70 lines.
Final accounting must use actual Git numstat, not this estimate.

## Prior delivery and legacy retirement checkpoint

Executor d6523386 and actual-change review 5fa2995c were pushed. First delivery
changed 13 paths, +446/-27 in total; configuration/tools/checks +236/-6.
Both architectures passed 180/180 unit/gates, latest five gates and direct
cache confinement/hit proofs. Fresh test builds each completed 684 edges;
second builds had no work. Test wall times were 113.24/91.82 seconds.
The cold cache's 28 hits/976 calls were not a warm-cache speedup measurement.
Neither EXE nor native desktop/audio/package integrations was refreshed.
Existing compiler warnings and old Ninja recovery debt were not repaired.

Thirty obsolete direct build children and prior Td scratch were deleted.
The generic cleaner correctly refused one embedded Git checkout; its local
SoftPC origin, clean ancestor 5fbe692 and lack of reparse points were verified
before explicit one-off disposal. Generic Git protection was not weakened.
Removed generated work is reproducible; raw logs are not recoverable from Git.

Before retirement, retained T98 S2 delivery 7afc9981, review db935fb7 and owner
closure 733a7f6e: both widths 193/193, zero failures/skips, exact XML/registration
match; production +30/-16, tests +195/-0, registration +3/-2. Accepted package
hashes stay in CURRENT. S1 reconstructed all 131 patched XP-subset hashes;
tracked manifests and source preserve reproducibility. The old x86 Ninja log
was version 7, 212184 bytes; recovery warnings were observed, not diagnosed.

## Reopen verification and delivery

All four correction members are implemented. Relative to 5fa2995c, build and
check code is +16/-120, net -104; C/H production/tests and public corpora have
zero diff. No second policy interpreter remains. The cleanup safety script
keeps all thirteen scenarios; expected rejection returns locally and unexpected
failure preserves the actual exception, without another status flag.

Both widths configured successfully in shipped trees and all test targets
built without work on first and second incremental calls. A deliberately
removed clock object then rebuilt through the actual launcher: direct cache
hit per width; hostile caller cache/temp directories were not created.
The real configure rejected an outside tree; the actual helper accepted a
nested path and rejected both .ninja itself and its same-prefix sibling.
Initial task-local probe invocation/path checks were corrected before the
successful run; they were not product defects or hidden passing results.

x64/x86 each passed all 178 unit/gates, exact XML/registration equality,
zero failures/errors/skips; wall times 82.52/80.95 seconds. Count fell by two
because the duplicate preset checker and its selftest were deleted, not because
product tests were removed. Final cleanup regression reruns passed in
1.15738/1.08503 seconds versus about 30 seconds with child shells. Different
load prevents treating whole-suite timing differences as controlled speedups.
Native desktop/audio/package integrations were not rerun; EXE hashes match
accepted S2. No production, test corpus, INI, media or snapshot change.

Six obsolete generated fixtures from deleted checks were removed after tests
finished. Coordinator reviewed actual pushed executor 7758a9c4 against all four
ledger members and independently matched both 178-case XML reports to current
registration, inspected final safety timings/cache logs and both exit-zero
markers. No blocker; src/test/assets have zero diff. P3 accounting is
+126/-251 over nine paths, including compacted documents; build/check code
alone is +16/-120. No safety scenario was dropped.

After confirming owned jobs ended, removed exact build/td-s22 scratch and ran
Clean-Build.ps1 -Check successfully: only .ccache/.ninja remain. Raw logs are
retired; the results above are retained. User snapshot.debug remains untouched.
Owner acceptance and numeric T/S advancement are not assumed.
