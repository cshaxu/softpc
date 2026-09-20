# Project Status

## Current Work

Active task: M9 T73 S5, independent Lib/Common/x86 test corpora.
Owner accepted S4; [S4 is closed](../history/M9-T73-S4-x86-source-corpus.md).
S5 implementation and automated qualification are complete: three independent
test suites, six-directory x86 and four-directory neutral copies pass. Awaiting
owner testing after implementation/review; S5/T73 are not closed. Existing Queue order is
retained; a shutdown-wake follow-up found during S5 is appended, not admitted.
See the [proposal](../proposals/m9-shared-x86-dependency-audit.md).

## M9 T73 S5 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner reported S4 testing passed and explicitly admitted S5 to build the three test component packages. |
| Objective | Establish test/lib, test/common, test/x86 ownership and standalone build/test closure; neutral four-directory set must not need x86. |
| Non-goals | No production C/H/API, Lib, guest behavior, new runtime abstraction, INI/media or snapshot changes; no desktop interference. |
| Reference Baseline | fb07a0b7 clean; S4 both Release builds, background 108/108 each and owner manual acceptance. |
| Candidate Proposal | [T73 proposal](../proposals/m9-shared-x86-dependency-audit.md). |
| Files And ABI Surface | Test/Common mixed smoke and neutral fixture; relocated x86 tests/verifier, suite/root CMake, manifests and current documentation. No production ABI. |
| Applicable Rules | Architecture, coding, execution, documentation governance and local source/build authorities. |
| Verification | Preserved assertion/case inventory; isolated four-/six-directory configure/build/background tests; dual-width Release/full background presets, manifests/DAG/docs and protected-path equality. |
| Expected Markers | Common tests contain no x86 includes/link dependencies; x86 tests own CLI/assembler; one shared neutral fake runner, three independently buildable suites. |
| Asset Needs | Existing main build trees and bounded disposable transfer trees under build; no guest traces/media or INI changes; both EXEs supplied. |
| Reporting Requirements | Before estimate, actual added/deleted/net production/test/build counts and separate relocation evidence; exact suite results and desktop exclusions. |
| Stop Conditions | Production behavior/ABI change or Lib modification requires review; receiving emulator implementation is not included. |
| Exit Criteria | Complete finite ledger, builds/tests, executor commit/push and actual-change review with clean workspace; wait for owner testing before S5/T73 closure. |
| Original Owner Request | 测试通过 准入s5 构建test的三组件包。 |
| Similar-Issue Sweep | All shared tests, local/product includes, build/fixture dependencies, manifests, old paths/targets, mixed machine/CLI assertions; no neutral-to-x86 reverse edge. |

## Current Technical Baseline

- T73 S5 implementation 879c30ac is pushed; the subsequent actual-change review
  accepts the bounded test split, not T closure or a fix for the queued runtime
  shutdown issue. S4 is owner-accepted/closed; S5 awaits manual acceptance.
- T73 S5 separates test/lib, test/common and test/x86; Common tests have no x86
  dependency. Both standalone transfer sets passed, and final full background
  x64 110/110 (181.72s), x86 110/110 (165.87s); five desktop cases excluded.
  Test C/H +621/-519 (net +102); CMake +50/-18 (net +32); runtime C/H unchanged.
  Native Common/x86 tests each passed 50 repetitions per width. Both EXEs retain
  S4 hashes. A separately recorded shutdown-wake race is not fixed by S5.
- T73 S4 places debugger/assembler in src/x86/debug and src/x86/xasm32 with
  x86_ public names and x86- targets. Common has only machine/session/ui and
  cannot depend on x86. S5 removes its transitional mixed test dependency.
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
