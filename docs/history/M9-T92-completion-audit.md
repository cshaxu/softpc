# M9 T92: XP Core Migration Completion Audit

Status: owner accepted and closed T92 on 2026-10-15: “验收通过，准入下一个T任务！”
S1-S6 implementation and verification are complete; S6 executor `48caa854`
and its separate actual-change review `93d0b8d6` are pushed. The same request
admits the sound candidate as T93 S1. This handoff changes documentation only;
accepted EXEs, source, INI, media and snapshots remain unchanged. All original
requirements and explicit sound transfer are accounted for below. Existing
TODOs remain independently tracked, not falsely claimed as repaired.

## Original request and scope

“按照治理规范和执行规范，单人双角色模式，实现现有softpc核心迁移到winxp源码的基线上。”
The owner approves importing XP source, then corrects the policy to the
necessary source subset rather than retained whole files. Proven useless old
branches may also be removed in the migration scope. Preserve accepted local
fixes and XP formatting; exclude unrelated products/machines; separately record
XP exclusions and actual local patches. No generated build input, second device
path, user-asset mutation or Computer Use. S tasks may proceed asynchronously;
only T closure waits for owner confirmation. Sound Blaster is a separate task.

## Finite completion ledger

| Requirement | Evidence and disposition |
| --- | --- |
| Fixed XP identity, original notices and explicit admission | Proposal records owner authorization and revision `a2f6d7c93aa4f11efc51147566d5961c59a8ef89`; no new license or redistribution grant is inferred. |
| Every upstream path accounted for | Fixed-object tree census equals 498 retained plus 499 unretained entries; no duplicate or omitted path. Unretained ledger assigns every path, including the seven deferred sound candidates. |
| Necessary subset, not whole-file import | S3/S4/S5 retain reviewed include/exclude line ranges for 127 paths. Independent reconstruction checks complete disjoint coverage and hashes. Other retained paths use their full unchanged XP counterpart. |
| Real local fixes remain visible | Current manifest distinguishes raw XP, subset and local hashes. 375 bodies match their subset; 123 carry local patches. Complete normalized subset diff is +26,267/-23,562, including historical ABI/behavior repairs. |
| DMA auto-init address repair | Current at_dma.c restores current_count and current_address from their respective bases. Actual DMA contract tests cover second round and non-auto completion. Separate decrement-copy debt remains explicit. |
| INT 13h AH=15h capacity repair | Current read_dasd splits sectors at 65536, without XP's UCHAR truncation. BIOS-entry tests independently check registers, AH/CF and missing drives. |
| V7 enable-bit precedence repair | Current A5 branch compares both masked bit-7 values. Actual register transition tests cover draw/clear, unchanged enable and unrelated attribute bits. |
| ROM full CS guard | Current ROM initialization retains full segment width. A generated checksummed test ROM proves execution CS; no external ROM or asset replacement. |
| XP controller/BIOS and input/timing upgrades | S3 and S4 pushed deliveries, independent local-patch/exclusion ledgers and actual-change reviews. CMOS comparisons, descriptor ABI and Pause/NumLock handling are recorded; unsafe XP narrowing is not imported. |
| XP video upgrade without renderer replacement | S5 pushed delivery and review preserve selected marker, geometry, stride, snapshot and detached DIB contracts. All 40 C-VID paths unchanged in S5. New 256-entry DAC tests use the real BIOS entry. |
| Snapshot and established behavior | S3-S5 and S6 each pass all registered snapshot, media, mode/dirty, input, runtime and package regressions on both widths. No schema change or new manual Windows-session claim. |
| Local-only routine mirror gate | Root CMake gate validates current body identity and exact path coverage without external checkout; positive and negative checks pass. It does not pretend to reconstruct unavailable upstream bodies during ordinary builds. |
| OpenNT retained only as lineage | Original audit/history retained. Current Architecture/Coding points to fixed XP; old divergence/upgrade ledgers explicitly identify their historical status. |
| Protected assets and packages | No public-package source/test, user INI, guest medium or snapshot changes. Only approved package EXEs are refreshed; final hashes below. Actual commit/protected-path review passes. |
| Full verification and delivery | S2 146/146 per width; S3-S5 148/148 per width; S6 150/150 per width, all including five desktop cases. S6 executor is pushed and reviewed. |
| Single-agent dual-role governance | Every S has pushed executor delivery then separate actual-change review. S6 review is recorded separately. T closure is not delegated. |

## Changed-path accounting

Frozen source start: `087e512f`. Through reviewed S5:

- Mirror: 76 files, +11,244/-18,625, net -7,381.
- Compat: one file, +2/-2, net zero.
- Owner-local tests: four files, +545/-4, net +541.

S6 does not change production C/H. Its manifest, verification scripts, CTest
registration and documentation are counted separately at final delivery. The
above source delta is not the total difference from XP; subset-local and
excluded-XP numbers answer different questions.

## S6 final qualification

Both Release package builds succeed, reusing existing Ninja intermediates.
This is not a clean-cache rebuild claim. Each architecture passes 90 shared,
55 other background and five reserved native desktop tests: 150/150 each.
x64 phase times are 57.79, 70.78 and 13.44 seconds; x86 times are 49.96,
67.51 and 13.62 seconds. All phase exit records and the full job exit are zero.
No Computer Use or interactive-timeout termination was used.

Published SHA-256:

- softpc32.exe: `f72e847756c1b09e137cd579496a58108648dbbff1e40bf23e2da39cf05230d4`
- softpc64.exe: `6e57b7e29088fdda0c80b219bdf6cdf1dd3eda7e013e4a9bb065c93eae8f6423`

S6 adds 129 verification-script lines and seven CTest-registration lines,
net +136; the 500-line identity manifest is metadata, not production code.
Production C/H and public packages have no S6 changes.

## Deferred obligations and limits

The owner-approved next task is standalone Sound Blaster capability, including
backend decisions and its own device/snapshot qualification. It is not a hidden
unfinished T92 feature. Existing TODOs remain: host-time-dependent INT 1Ah test
oracle, original DMA decrement-copy semantics, and explicitly deferred x86
null BOP dispatch. No workaround is added to hide them. Their source/runtime
proof limits and admission conditions remain in TODO.

Automated package/native tests are not manual Win1/Win3/Win95 acceptance.
Retained XP whitespace exceptions are disclosed in the S records. The initial
whole-file import plan, merge/fixture mistakes and subsequent corrections are
preserved in the proposal rather than erased by green final tests.

## References

- [Migration proposal and detailed qualification](../history/M9-T92-xp-sp1-softpc-mirror-rebase-proposal.md)
- [Current mirror identity manifest](../etc/evidence/softpc/xp-sp1-current-manifest.tsv)
- [S2 device repair](M9-T92-S2-device-repair-handoff.md)
- [S3 controller/BIOS subset](M9-T92-S3-controller-bios-subset.md)
- [S4 input/timing subset](M9-T92-S4-input-timing-subset.md)
- [S5 video subset](M9-T92-S5-video-subset.md)
- [S6 final baseline qualification and review](M9-T92-S6-xp-baseline-qualification.md)
