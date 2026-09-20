# Project Status

## Current Work

Active task: M9 T73 S3, neutral Machine debug transport and x86 protocol migration.
Owner accepted and closed [S2](../history/M9-T73-S2-machine-x86-audit.md), then
admitted S3. S4 build-selection work remains unadmitted. Stop for manual testing
after S3's complete dual-width delivery; do not close T73.
Owner directly admitted T73 outside the queue. T72 is closed.
See the [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md).
Existing queued candidates remain unadmitted.
See the [S1 record](../history/M9-T73-S1-x86-component-rename.md) for scope,
verification, changed-line accounting and package hashes.

## M9 T73 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved the explained copied-byte transport, requested S2 closure then S3 implementation/build/test/commit/push and manual verification. |
| Objective | Move x86 protocol ownership to x86-debug; make Machine request/result transport opaque while preserving the sole executor, paused lease and completion/cancellation paths. |
| Non-goals | No Lib/Session/UI/Compat/MVDM changes, new executor/queue, dynamic message framework, command semantics change, S4 build selection, INI/media or snapshot changes. |
| Reference Baseline | 2c724e10; clean; S2 Common tests x64/x86 22/22, S1 full background 105/105 each. |
| Candidate Proposal | [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md). |
| Files And ABI Surface | Machine header/request slot, new x86 protocol header, existing x86-debug command adapter, VM debug/driver and every direct test consumer; manifests and design evidence. |
| Applicable Rules | Architecture, coding, execution and documentation governance; local source/design authorities. |
| Verification | Copied payload/thread identity, bounds/length/error/stale-lease/cancel/terminal tests; original CLI/product debugger integration; dual-width Release builds and full background presets; corpus/DAG/document gates and Lib equality. |
| Expected Markers | No x86 payload in Machine; one existing request path; exact x86 typed adapter validation; initialized failure outputs; preserved lifecycle/CLI and both EXEs. |
| Asset Needs | Refresh only assets/binary/softpc32.exe and softpc64.exe; preserve INI/media. Existing ignored build trees hold bounded test output; no guest trace acquisition. |
| Reporting Requirements | Before estimate and after actual added/deleted/net C/H and test counts, separate relocation and artifact costs; disclose excluded desktop tests. |
| Stop Conditions | New runtime framework, Lib changes or guest-visible behavior change require review; S4 is not included. |
| Exit Criteria | Implemented, actual-change reviewed, both widths built/tested and pushed clean; await owner's manual verification before S3 closure. |
| Original Owner Request | 批准照此实现S3.请你收口S2先，然后准入S3，执行完成后编译测试提交推送等我验证。 |
| Similar-Issue Sweep | All typed debug requests/results/constants and execute callbacks/callers/tests; exact copy bounds/alignment/failure lengths; no old aliases or Machine-to-x86 include edge. |

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
