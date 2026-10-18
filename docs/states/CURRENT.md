# Project Status

## Current Work

M9 Td S22 is verified, awaiting owner acceptance:
[build-cache governance](../proposals/m9-build-cache-governance.md).
Open task awaiting owner: T98.

Td S22 executor d6523386 is pushed and its actual changed paths reviewed:
canonical dual-width test builds,
180/180 unit/gates per architecture, latest five affected gates per architecture
and direct-build cache confinement/hits passed. Thirty obsolete build children
are removed. The coordinator independently checked the pushed diff, both
180-case XML reports and confinement/cleanup logs. Its review scratch is now
removed; build contains only .ccache and .ninja, and the residue check passes.
No EXE/runtime change. T98 stays open; owner acceptance of this Td is pending.

Owner accepts and closes [S2](../history/M9-T98-S2-media-boundaries.md) on
2026-10-18: 验收通过 收口s任务 保持t开放. Executor 7afc9981 and coordinator
review db935fb7 are pushed; both architectures passed all 193 registered tests.
T98 remains open. S3/S4 in the
[proposal](../proposals/m9-nonpublic-quality-performance-audit.md) are not active;
no automatic next admission. User-owned untracked assets/binary/snapshot.debug
remains untouched/excluded.

## M9 Td S22 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Governance |
| Admission And Approval | Owner: 准入一个td任务执行治理; persist only build/.ccache and build/.ninja, clean completed task scratch, update rules/gates/scripts. |
| Objective | Pin compiler-cache storage and Ninja build trees to their two repository-local roots; enforce safe task cleanup and remove obsolete build leftovers after preserving concise evidence. |
| Non-goals | No runtime, public corpus, mirror, INI/media/snapshot changes, global cache purge or new build framework. T98 stays open; no numeric T/S admission. |
| Reference Baseline | main 733a7f6e; accepted T98 S2 runtime and both package hashes below. |
| Candidate Proposal | [Build-cache governance](../proposals/m9-build-cache-governance.md). |
| Files And ABI Surface | Root presets/CMake, tools, README/CONTRIBUTING, Execution and source-layout documentation, Current/history. No production ABI. |
| Applicable Rules | Execution, Document, Architecture and Coding governance skills; single-person executor/coordinator review; no Computer Use. |
| Verification | Preset/cache confinement gate and negative/safety tests; fresh x64/x86 configure/build and background unit/gates in canonical trees, ccache miss/hit probe, no-op incremental build; final build-root residue check. |
| Expected Markers | Exactly two reusable cache roots; non-cache entries have finite task ownership and disappear on completion; no stale absolute-path CMake tree is relocated/reused. |
| Asset Needs | Disposable build/td-s22 for jobs/logs with 30-minute/5-minute no-progress/50-MiB budget; stop owned jobs and summarize evidence before removal. Keep user files; no global ccache deletion. |
| Reporting Requirements | Paths, actual script/config/test diff, cleanup inventory, cache proof and verification limits; preserve runtime hashes for governance-only work. |
| Stop Conditions | User data, live unrelated jobs, symlink/reparse traversal or cache outside repo must block unsafe cleanup; do not kill jobs or erase unrelated global caches. |
| Exit Criteria | Policy/scripts/gates implemented and tested, obsolete generated directories removed safely, only the two caches remain, evidence summarized and pushed actual-change review delivered. |
| Original Owner Request | build/.ccache, build/.ninja可以跨提交保存，以便节省时间。但是这两个之外的build里的内容属于用后即抛; 准入一个td任务执行治理. |
| Similar-Issue Sweep | All tracked build entry points, current docs, live build roots, compiler launcher routing and temporary-probe/log references; historical paths stay historical with explicit retirement notes. |

## Current Technical Baseline

- T98 S2 fixes Compat media close reporting and address bounds only. Production
  +30/-16 (net +14); tests +195/-0; root test registration +3/-2. Public and
  mirror code unchanged. Historical qualification: build/t98/s2/qualification,
  exit 0; raw logs retired by Td S22 after preserving the checkpoint;
  x64/x86 each 177 unit/gates, eight background integrations, five desktop and
  three native audio, exact XML/registration match. Existing Ninja recovery
  and original compiler warnings remain; no warning-removal claim is made.
- Current packages are rebuilt S2: softpc32 SHA-256
  4E67DBD9AAEBE99BB874BA77E0BFA7DBE7819DE5927D920BE4998769089231AC;
  softpc64 SHA-256
  5C692E0DE7ADFDF9D1AEF2446D90C7C4B733FD3ECA589B94CBA33D2E89D216A8.
- T97 is investigation/documentation only; production, tests and source mirror
  have no changes. Installed Win95 driver rejects mixer/volume capabilities for
  DSP 2.01. The observed hardware-change confirmation explicitly requests
  shutdown; normal Restart succeeds twice. The owner's original installation
  dialog was not recorded, so its exact wording/action remains unverified.
- T96 runtime/reference: 5ea4e55e; S8 delivery a404740f and review 5ea4e55e.
  All 191 tests per architecture passed with zero failures/skips: 175 unit/gates,
  eight background integrations, five desktop and three native audio.
  Evidence: build/t96/s8/qualification, qualification.exit=0; exact CTest/XML
  registration equality was checked. Existing compiler/Ninja warnings remain.
- Previously accepted T96 package reference, without a T97 rebuild:
  softpc32 SHA-256 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A;
  softpc64 SHA-256 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
- Current transport: RGB graphics, separate leaf text contracts, Machine-owned
  fixed four-slot exchanges shared through opaque Base mechanics; Driver owns
  readiness and Machine publishes every produced frame. Session/UI route
  references. See current architecture and T96 audit for accounting.
- Win3.1 verified MIDI configuration is MPU330/IRQ7, SB220/IRQ5, all sixteen
  Mapper channels to MPU. [Product UI](../design/UI.md#windows-31-midi-playback)
  owns instructions. No FM or SB-MIDI capability was added.
- Existing [TODO](TODO.md) items remain open. No unrelated debt is repaired or
  removed by this investigation closure; the [Queue](QUEUE.md) is empty.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T97 | Owner accepted S1 and T97, 2026-10-18. | [Audit](../history/M9-T97-completion-audit.md) |
| T96 | Owner accepted S8 and T96, 2026-10-17. | [Audit](../history/M9-T96-completion-audit.md) |
| T95 | Delegated closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |

## Recent Governance

- M9 Td S21 approved bounded frame ownership; see the
  [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 completed that implementation; no rule change accompanies T97 closure.
