# Project Status

## Current Work

Active task: M9 T73 S1, Lib/Common x86 dependency audit and reuse design.
Owner directly admitted this task outside the queue. T72 is closed.
See the [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md).
Existing queued candidates remain unadmitted.
S1 audit/design is delivered for owner review; no S2 implementation is admitted.
Production/test changes are +0/-0. Thirteen targeted static gates passed;
Lib and Common source, shared tests and accepted EXEs are unchanged.

## M9 T73 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner explicitly admits a new Lib/Common x86 dependency audit, referencing NNES design. |
| Objective | Verify unchanged Lib reuse and inventory every public/internal x86 dependency in Common session/ui/machine; design x86-debug/x86-xasm32 ownership. |
| Non-goals | No production rename/API implementation, Lib patch, new emulator, sibling change, binary/config/media edit or runtime behavior change. |
| Reference Baseline | ad66b615, clean, accepted T72. NNES read-only design reference; not assumed implemented. |
| Candidate Proposal | [T73 design](../proposals/m9-shared-x86-dependency-audit.md). |
| Files And ABI Surface | Read src/lib and src/common, shared tests/build verifiers and current App/VM callers; write task documentation only. |
| Applicable Rules | Architecture/coding/execution/documentation rules and shared governance skills; source research policy. |
| Verification | Finite source/API/build inventory; targeted searches plus call-chain inspection; existing corpus/DAG gates; documentation governance. No build/GUI claim from audit alone. |
| Expected Markers | Evidence separates guest ISA coupling, host dependencies and capability limits; each binding has one proposed owner. |
| Asset Needs | None; preserve both EXEs, snapshot, INI and media. |
| Reporting Requirements | Production/test +0/-0; concrete findings and later implementation estimates clearly distinguished. |
| Stop Conditions | Any source change or assumption about target hardware requiring new semantics returns to owner design review. |
| Exit Criteria | Evidence-backed report, proposal and scoped follow-up design delivered for owner review; Lib untouched. |
| Original Owner Request | 对 lib 和common做一次x86依赖审计，给nec模拟器使用，参考隔壁nnes设计；确认lib原样移植；common/debug改common/x86-debug，common/xasm32改common/x86-xasm32；审计session/ui/machine所有api和内部逻辑。 |
| Similar-Issue Sweep | Public types, callback payloads, dispatch/switches, lifecycle/media/input/frame policies, build targets, test dependency and verifier allowlists across the frozen corpus. |

## Current Technical Baseline

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
