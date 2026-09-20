# Project Status

## Current Work

Active task: M9 T73 S4, establish the separate x86 source corpus.
Owner accepted S3 manual testing; [S3 is closed](../history/M9-T73-S3-neutral-machine-debug.md).
Owner moved neutral build qualification to S5 and admitted S4's source relocation.
S5 will establish test/x86 and qualify the six-directory and neutral four-directory
transfer sets; it is not admitted. Stop after S4 delivery for owner testing.
See the [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md).
T73 remains open; Queue candidates remain unchanged and unadmitted.
S4 implementation and verification are complete: both Release builds and full
background suites passed (x64 108/108, 163.08s; x86 108/108, 148.73s).
Five desktop tests per width were excluded. Standalone x86 source build passed.
Production C/H +363/-363, tests C/H +401/-401 (both net zero); build/gates
+200/-41 (net +159). No Lib or neutral runtime changes. Delivery awaits push,
actual-change review and owner testing; S4 is not closed.

## M9 T73 S4 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepted S3, moved old S4 to S5 and explicitly admitted the new S4 establishing top-level x86. |
| Objective | Move debugger/assembler to src/x86/debug and src/x86/xasm32; make Common source/build neutral and repair all consumers without changing behavior. |
| Non-goals | No Lib or neutral runtime changes, command/protocol semantic changes, new executor/queue, test/x86 split before S5, INI/media/snapshot changes. |
| Reference Baseline | dc06671d; clean; S3 dual-width Release and background 105/105 each; owner manual test passed. |
| Candidate Proposal | [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md). |
| Files And ABI Surface | Both relocated trees and x86 public names/targets; App/VM/test includes and symbols; root/shared CMake, manifests, DAG/product gates and current design. |
| Applicable Rules | Architecture, coding, execution and documentation governance and local authorities. |
| Verification | Reverse mechanical comparison, all direct references, manifests/DAG and negative probes; both Release builds and full background presets, two packaged EXEs. |
| Expected Markers | One x86 implementation outside Common; no Common reverse edge, aliases or protocol/CLI behavior change. |
| Asset Needs | Refresh only assets/binary/softpc32.exe and softpc64.exe; preserve INI/media. Existing ignored build trees; no desktop interaction or guest traces. |
| Reporting Requirements | Before estimate and actual added/deleted/net counts for production C/H, tests and build/gates; distinguish moved lines and binaries. |
| Stop Conditions | Runtime semantic change or Lib modification requires review; S5 test separation remains unadmitted. |
| Exit Criteria | Implementation, actual-change review, dual-width build/test, clean commit/push and owner manual acceptance; keep T73 open. |
| Original Owner Request | 把S4推迟到S5；新S4把common/x86-debug、common/x86-xasm32迁至x86/debug、x86/xasm32；S5建立test/lib、test/common、test/x86；六目录供NXVM/SoftPC，Common/Lib四目录供NNEC；更新proposal和退出标准并准入S4。 |
| Similar-Issue Sweep | Both trees, every include/symbol/target and App/VM/test consumer, Common/x86 forward/reverse/private edges, complete manifests and current docs; historical references preserved. |

## Current Technical Baseline

- T73 S4 places debugger/assembler in src/x86/debug and src/x86/xasm32 with
  x86_ public names and x86- targets. Common has only machine/session/ui and
  cannot depend on x86. Tests remain temporarily mixed in test/common until S5.
  Reverse mechanical comparison proves all 22 changed C/H files retain behavior;
  package hashes and complete accounting are in the proposal.
- T73 S3 moves x86 protocol values to x86-debug, retaining one Machine executor
  and copied opaque 128/1536-byte request/response slot. VM alone validates x86
  access semantics; no CLI rewrite, Lib/Session/UI/Compat/MVDM or INI/media change.
  Build and verification evidence is in the proposal; owner manual test passed
  and S3 is closed. S4 relocates its protocol owner to src/x86/debug.
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
