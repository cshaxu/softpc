# Project Status

## Current Work

Owner approves the established T98 repairs on 2026-10-18, accepting the
[S1 audit](../history/M9-T98-S1-nonpublic-quality-audit.md) and admitting S2:
propagate real media retirement errors and enforce HDD address bounds.
The [proposal](../proposals/m9-nonpublic-quality-performance-audit.md) records
S2-S4 ordering. S2 implementation and qualification are complete: both cached
builds/releases and all 193 registered tests per width pass, zero failures/skips.
Executor delivery and actual-commit review are being finalized for owner acceptance.
Each implementation S delivers builds/tests and a pushed actual-diff review,
then waits for owner acceptance before the next S. No public writer expansion
is admitted; S4 retains FILE operations and changes only UTF-8 opening.
User-owned untracked assets/binary/snapshot.debug remains untouched/excluded.

## M9 T98 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 批准执行当前既定修复任务. Accept S1 findings; implement established repairs in S2-S4 order, correctness before performance. |
| Objective | Return consumed-medium close failures through existing FDD/HDD attach/restore results, retain correct ownership/metadata, reject unaddressable HDD sizes and check transfer arithmetic. |
| Non-goals | No public or mirror edits, retry/rollback state, new backend, performance redesign, guest/media/configuration edits or status API expansion for void teardown. |
| Reference Baseline | main 3b368d26; accepted runtime 5ea4e55e; pinned XP subset manifest unchanged. |
| Candidate Proposal | [Non-public quality/performance audit](../proposals/m9-nonpublic-quality-performance-audit.md). |
| Files And ABI Surface | compat/hdd_media.c, gfi_image.c; owner-local adapter unit and root test registration; state/proposal/history. Existing signatures unchanged. |
| Applicable Rules | Execution, Document, Architecture and Coding; single-person executor/coordinator actual-change review; no Computer Use. |
| Verification | Real adapters with injected consumed close failures, rejected replacement ownership and mocked large media; x86/x64 cached full builds, all unit/gates, background integrations and serial native desktop/audio integrations; exact CTest/XML coverage. |
| Expected Markers | Cleanup completes once on error; valid replacement metadata commits even when retirement reports failure; unsupported sizes do not attach/replace; transfer bounds cannot wrap. |
| Asset Needs | Preserve INI/media/snapshot.debug. Refresh only assets/binary dual EXEs. Owned hidden qualification under build/t98/s2: 30-minute budget, 5-minute no-progress observation, 50 MiB logs; executor owns cleanup. No Computer Use. |
| Reporting Requirements | Production/test added, deleted and net lines, unchanged public/mirror delta, exact proof boundaries and dual EXE links. Initial estimate production +20-42/-11-22, tests +75-130. |
| Stop Conditions | Public/mirror changes, new storage semantics or ABI expansion require owner direction. Host-dependent failure is not a unit pass or an automatic retry. |
| Exit Criteria | F1/F2 ledger satisfied; focused and full qualification passed, dual EXEs published, complete P pushed and actual-change coordinator review delivered; wait for owner acceptance. |
| Original Owner Request | 批准执行当前既定修复任务; earlier: 先把已知缺口修复 再处理性能问题. |
| Similar-Issue Sweep | Every medium-destroy and size/offset conversion in the real FDD/HDD adapters, callers and snapshot replacement joins; status-bearing failures fixed, void/cleanup-only calls explicitly disposed. |

## Current Technical Baseline

- T98 S2 fixes Compat media close reporting and address bounds only. Production
  +30/-16 (net +14); tests +195/-0; root test registration +3/-2. Public and
  mirror code unchanged. Qualification: build/t98/s2/qualification, exit 0;
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
