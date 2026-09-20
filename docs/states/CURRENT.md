# Project Status

## Current Work

Active task: M9 T73 S2, Machine x86 API/internal audit and migration planning.
Owner accepted S1 testing and admitted S2. No Machine migration is admitted.
Owner directly admitted T73 outside the queue. T72 is closed.
See the [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md).
Existing queued candidates remain unadmitted.
See the [S1 record](../history/M9-T73-S1-x86-component-rename.md) for scope,
verification, changed-line accounting and package hashes.

## M9 T73 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: 通过测试，准入S2，开始审计machine的x86 api和内部实现问题。 |
| Objective | Audit every Machine API/internal path and direct debug adapter consumer; separate x86 payload ownership from neutral rendezvous, then split later migrations. |
| Non-goals | No source/test/ABI change, Lib patch, new executor, binary/media/config refresh, or implementation of later S tasks. |
| Reference Baseline | feee0fb1; clean, S1 manually accepted, both widths previously 105/105 background. |
| Candidate Proposal | [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md). |
| Files And ABI Surface | Six Machine files; x86-debug public/runtime consumers, VM adapter, App cancellation callers; Common build/DAG and related tests. Write docs only. |
| Applicable Rules | Architecture, coding, execution and documentation governance; local source/design authorities. |
| Verification | Full bounded source/API inventory with per-hit disposition; existing focused Machine/debug/build-boundary tests; documentation gate. No new full-runtime qualification claimed. |
| Expected Markers | Every API/callback and internal owner classified; payload, capacity, completion/cancellation and build coupling explained with concrete migration boundaries. |
| Asset Needs | None; preserve accepted EXEs, Lib, INI, snapshots and media. |
| Reporting Requirements | Audit findings versus confirmed defects separated; production/test +0/-0; later-S added/deleted estimates identified as estimates. |
| Stop Conditions | A proposed implementation or guest-visible semantics change requires owner review/admission. |
| Exit Criteria | Evidence-backed audit and bounded follow-up S plan delivered and committed; no migration silently starts. |
| Original Owner Request | 通过测试，准入S2，开始审计machine的x86 api和内部实现问题。 |
| Similar-Issue Sweep | Public x86 types/constants, all driver hooks, request/wait/failure/lease paths, frame/input/media/state paths, production callers and shared-test dependencies. |

## Current Technical Baseline

- T73 S1 renames x86 debug/assembly paths, public symbols and targets only.
  Both Release builds passed; background x64 105/105 (168.90s), x86 105/105
  (156.37s); five desktop tests per width excluded. Production +168/-168,
  tests C/H +181/-181, build/gates +26/-24. Lib/Machine/Session/UI and INI/media
  are unchanged; package hashes are in the S1 record. Owner accepted S1 tests.
- Accepted code delivery 95850780, actual-change review ae1dc6e0.
  Both Release builds passed; final background x64 105/105 (134.41s),
  x86 105/105 (135.13s). Five desktop tests per width excluded; owner manual
  S8 test passed. Closure changes documentation only, not source or binaries.
- T72 endpoint production C/H +294/-183 (net +111), tests +619/-90
  (net +529), relative to bea66d3f. Per-S accounting, limits and package hashes
  are retained in the completion audit and linked evidence.
- VM owns character mapping and device-attribute decoding. KVM text uses
  four-byte cells with independent glyph index/bank and foreground/background.
  Window owns fonts, Console owns character maps; both remain complete frames.
- Base transports opaque control FIFO and latest-wins frames. Window compares
  directly decoded text/graphics pixels with its existing surface, without a
  row buffer; cursor old/new-region damage remains separate.
- Native relative/absolute mouse records replace pointer recentering; owner
  desktop/RDP tests passed. Public copied integer-delta input contract remains.
- Common teardown retains live callback dependencies on failure; App terminates
  before freeing them. TEXT uses the existing running input sink. Fixed Debug
  arguments belong to its one object; command semantics are unchanged.
- MVDM/Compat and snapshot format were not changed by T72. Owner-approved
  snapshot c0355b2f remains tracked unchanged. INI/media are user-owned.
- TODO remains empty under the owner's tracking policy, not proof of universal
  correctness. Queue's three candidates remain separate and unadmitted.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T72 | S1--S8 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T72-completion-audit.md) |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner approved. | [Audit](../history/M9-T68-completion-audit.md) |

## Recent Governance

- M9 Td S18: T71 closure, TODO retirement and floppy-identification candidate;
  [record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
- T72 closure archives its proposal, records S8 acceptance and all-S audit;
  no new implementation task or T number is allocated.
