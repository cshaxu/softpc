# Project Status

## Current Work

Owner admits T98 S1 on 2026-10-18 to audit non-public code quality and
performance, excluding unchanged mirror bodies but including local mirror diffs.
The [audit proposal](../proposals/m9-nonpublic-quality-performance-audit.md)
owns its finite scope and findings. S1 audit is delivered for owner review:
four new P2 findings, existing debt and bounded performance candidates. Cached
x64/x86 App unit/root gates each pass 56/56; an isolated real HDD adapter probe
confirms swallowed close failure and 4-GiB extent overflow. All 131 local mirror
subset hashes were independently reconstructed. No implementation fix admitted.
Owner-requested COM/LPT follow-up withdraws the public writer migration:
flush alone misses partial-write accounting. A bounded private UTF-8 opener
candidate preserves existing FILE operations and passed an isolated probe;
it remains a proposed repair, not deployed code.
User-owned untracked assets/binary/snapshot.debug remains untouched/excluded.

## M9 T98 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner: 准入一个新的t任务 帮我检验非公共组件的代码质量和性能问题（不包括镜像代码，但包括镜像diff）. |
| Objective | Audit App-SoftPC Product, Machine, Compat, their tests and root build/tooling; inspect local mirror patches rather than unchanged XP implementation. Identify concrete quality defects, avoidable work and measurement gaps with minimal remedies. |
| Non-goals | No public-package audit or edits, wholesale original-core audit, speculative speedup claims, implementation fixes, guest/media/configuration edits or new hardware capabilities. |
| Reference Baseline | main 4c8e185b; accepted runtime 5ea4e55e and unchanged dual packages; pinned XP subset manifest. |
| Candidate Proposal | [Non-public quality/performance audit](../proposals/m9-nonpublic-quality-performance-audit.md). |
| Files And ABI Surface | Read-only src/app-softpc excluding untouched mirror bodies; local mirror diff and ABI joins, test/app-softpc, root CMake/tools and mirror ledgers. Only audit/state documentation may change. |
| Applicable Rules | Execution, Document, Architecture and Coding; single-person executor/coordinator actual-change review; no Computer Use. |
| Verification | Frozen path inventory, six call-chain reviews, caller/test searches and existing deterministic checks where useful. Any long diagnostic uses hidden owned jobs with logs/exits and bounded budgets. No full rebuild/release for documentation-only audit. |
| Expected Markers | Findings cite precise production path, trigger, impact, test gap and minimal correction; distinguish existing TODO, new issue, measured cost and source-inferred cost. |
| Asset Needs | Preserve owner INI/media/snapshots and packages. Optional bounded evidence under build/t98/s1, up to 20 MiB and 15 minutes per diagnostic; executor owns cleanup. |
| Reporting Requirements | Findings by severity and responsibility, changed-file/line estimates for candidate fixes, scope coverage and verification limits; production/test/mirror audit delta +0/-0. |
| Stop Conditions | Report before production fixes, public contract changes, mirror expansion or externally dependent performance experiments. Do not treat tests passing as absence of defects. |
| Exit Criteria | Six finite audit areas reconciled with evidence or explicit limitations, independent actual-diff review, documentation checks, committed/pushed report awaiting owner direction. |
| Original Owner Request | 准入一个新的t任务 帮我检验非公共组件的代码质量和性能问题（不包括镜像代码，但包括镜像diff）. |
| Similar-Issue Sweep | Failure propagation, ownership, duplicated paths, hot-loop allocations/copies/synchronization, snapshot/media boundaries and test/build isolation across all admitted owners. |

## Current Technical Baseline

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
- Packages remain the accepted T96 runtime, without a T97 rebuild:
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
