# Project Status

## Current Work

## M9 T80 S1 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | New |
| Admission And Approval | Owner reports T79 testing passed, approves its closure and admission of the next queued task. T80 adopts the shared-corpus proposal; S1 is the sole active subtask. |
| Objective | Bound xasm32 instruction fetch and position advance before memory access; preserve valid disassembly and existing failure/output contracts. |
| Non-goals | No Core, media, snapshot, guest behavior, debugger syntax redesign, capacity expansion, new decoder or mutable global parsing state. S2--S7 are planned, not active. |
| Reference Baseline | `b9413d65`, owner-accepted T79 artifacts. Queued shared-corpus proposal was previously uncommitted and is incorporated by this admission. |
| Candidate Proposal | [Shared corpus boundary and simplification](../proposals/m9-shared-corpus-boundary-and-simplification.md). |
| Files And ABI Surface | src/x86/xasm32/{xasm32.c,dasm32.c}, existing x86/debug callers, test/x86 and matching manifests; exact changed paths/count estimate after pre-implementation audit. Public API shape remains unchanged. |
| Applicable Rules | `docs/rules/{EXECUTION,ARCHITECTURE,CODING,DOCUMENT}.md`, `docs/design/{ARCHITECTURE,CODING,UI}.md`, and the Core mirror boundary in `AGENTS.md`. |
| Verification | Guard-page or memory-check proof for 15-prefix/truncated immediate/ModRM/SIB/displacement; valid 14-prefix plus NOP; public API and U/XU behavior; strict C11 x86/x64, manifests/DAG, independent x86 tests and full background CTest, both package EXEs. |
| Expected Markers | No byte-16 access or unbounded prefix loop; failure before access, valid original output unchanged; all direct fetch/skip paths accounted for. |
| Asset Needs | No guest assets. Any S1 diagnostic lives under build/t80-s1, bounded to 120 seconds/run and 10 MiB output with agent cleanup. Desktop tests require a reserved window. |
| Reporting Requirements | Before implementation audit report expected paths, additions/deletions/net; after execution actual counts, focused/background evidence, dual EXE links and uncovered-platform limits. |
| Stop Conditions | Stop for public API redesign, altered valid instruction semantics, Core changes or an unrelated issue requiring expansion. S2 notification-failure design needs its own concrete review before implementation. |
| Exit Criteria | S1 bounded repair, regression and similar-issue sweep complete; manifest/gates, x86/x64 packages, pushed complete P and actual-change review. T closure additionally requires all seven proposal rows and owner acceptance. |
| Original Owner Request | “测试通过 可以收口t任务 准入下一个t”; queue proposal records “审计一下，lib/common/x86六组件代码质量” and “将上述置于队列首位，写入proposal”。 |
| Similar-Issue Sweep | Enumerate fetch, skip, prefix and direct memory access paths plus assembly output bounds in existing xasm32; map public and debugger consumers, disposition each hit without duplicating decoders. |

## Current Technical Baseline

- T79 S2 implements independent GFI physical identity and media lifetime.
  Production +56/-23 (net +33), tests/scripts +187/-37 (net +150); three
  production files, no shared-corpus or Core mirror changes. Both Release
  packages, focused 7/7 per width, background x64 110/110 (178.87s) and x86
  110/110 (163.39s) pass; five desktop cases excluded. Empty/present snapshots
  pass both cross-width routes. The device stream adds eight identity bytes;
  old-format snapshots are not accepted. Owner reports testing passed and
  approves S2/T79 closure. Executor `cba189b5`, review `b9413d65`; unchanged
  artifact hashes and coverage are in [the closure audit](../history/M9-T79-completion-audit.md).
- T78 S1 commit `0fb40f48` removes Common's direct integer-limit import and
  aligns the unsigned run-generation storage with its public `lib_u32`
  contract. Production/test C/H +52/-17 (net +35); no behavior or public ABI
  change. Lib/Common 44/44 and background regression 109/109 plus the direct
  long case pass on x64/x86. Owner manually accepted the package EXEs; closure
  evidence is in [the audit](../history/M9-T78-completion-audit.md).
- T77 S3 executor 7a6ac879 completes final acceptance. Product tests now have
  App/Core/Integration/checks ownership and one Integration machine fixture.
  Whole-task code/build +229/-2129 (net -1900); production/shared corpora unchanged.
  Final Release/background x64 110/110 (155.50s), x86 110/110 (151.26s).
  Isolated Lib 41/41, Common 18/18 and x86 9/9 pass on both widths;
  180/204 copied files identical. Both EXEs retain T76 hashes. Five desktop
  cases excluded per width; no new Linux or downstream integration claim.
- T77 S2 executor 217ec7a5 removes obsolete test wrappers/diagnostics and keeps
  one Integration fixture; code/build +181/-2096 net -1915. Release and background
  x64/x86 110/110 pass; production/shared corpora and EXE hashes unchanged.
- T77 S1 executor be23165f relocated 33 unchanged test blobs (8373 lines).
  All 115 CTest definitions identical; both Release/background 110/110 pass.
  Code/build +50/-35 net +15; production/shared corpora unchanged.
- T76 uses one direct page-pointer array for Storage overlay lookup; no public
  API or snapshot format change. Production +39/-30 (net +9); tests +89/-2
  (net +87). No benchmark per owner direction; array memory scales with capacity.
  Final Release/background x64 110/110 (146.29s), x86 110/110 (146.52s), five
  desktop cases excluded per width. Old/new x86/x64 snapshot matrix 16/16 passes;
  golden media payload matches old linked-list codec on both widths. Artifacts
  and hashes are recorded in the archived T76 proposal; INI/media unchanged.
- T75 uses the existing task cancellation object for the outer Machine wait;
  deterministic lost-command-wake coverage and repeated native shutdown pass.
  All six shared packages select strict C11 in standalone and embedded builds.
  Public API, App/Core, INI and media remain unchanged. Endpoint source/test/build
  +126/-79 (net +47), of which production C/H is +34/-33 (net +1).
- T75 S4 verifies sixteen independent four/six-package builds on both widths;
  each width passes isolated Lib 41/41, Common 18/18, and optional x86 9/9.
  All 180/204 copied files remain identical. Final product background x64
  110/110 (127.19s), x86 110/110 (182.23s); both Release EXEs rebuilt and match
  S3 hashes. Desktop cases excluded; no Linux runtime or NNES integration claim.
- T74 S1 moves 556 files into src/core/{machine,compat,softpc.new}.
  498 mirror blobs are identical; 107 product/test C/H/RC files pass path-only
  comparison. Runtime symbols and behavior remain unchanged. Both Release
  builds and background suites pass: x64 110/110 (165.45s), x86 110/110
  (157.47s); five desktop cases excluded per width. Shared corpora, INI and
  guest media are untouched. [T74 proposal](../history/M9-T74-core-layout-rename-proposal.md)
  holds the +421/-409 (net +12) source/test/build/tool ledger and EXE hashes.
- Accepted T73 implementation 879c30ac, actual-change review ad665a86.
  Owner manual S5 testing passed. Closure changes documents only.
- Shared source corpora are src/lib, src/common, src/x86; matching suites are
  test/lib, test/common, test/x86. The six-directory set serves x86 products;
  the four Lib/Common directories build/test with x86 absent.
- Common contains machine/session/ui only. Machine retains one executor and
  copied opaque 128/1536-byte debug transport with its existing paused lease.
  x86/debug owns CPU protocol and DOS/X CLI; x86/xasm32 owns assembly/disassembly.
  VM validates x86 requests; App explicitly connects optional x86 capabilities.
- Lib and test/lib are unchanged by T73. Host-PC key identity and existing KVM
  capacities are retained; receiving adapters own guest input mapping.
- Final background x64 110/110 (181.72s), x86 110/110 (165.87s); both Release
  builds passed. Five desktop cases per width were excluded. Native Common/x86
  tests each passed 50 repetitions per width. Owner testing does not imply
  those excluded automated cases or Linux/NEC integration were exercised.
- Isolated neutral copy: Lib 41/41, Common 18/18. Isolated six-directory copy:
  Lib 41/41, Common 18/18, x86 9/9. No product resources/build are required.
- T73 endpoint C/H production +487/-428 (net +59), tests +1104/-819 (net +285);
  complete ledger and artifact hashes are in the completion audit. S5 runtime
  code is unchanged; its EXEs remain byte-identical to accepted S4.
- VM owns character mapping and device-attribute decoding. KVM uses four-byte
  text cells; Window owns fonts and Console owns character maps. Base transports
  opaque control FIFO and latest-wins frames. Window compares decoded pixels
  against its own surface; native relative/absolute mouse records remain.
- Common Session/UI, Compat, MVDM, snapshot format, user INI and media were not
  changed by T73. T75 S2 subsequently repairs the debug-close/paused-destroy
  wake race; its bounded schedule proof does not certify all interleavings.
- TODO remains empty under the owner's tracking policy, not proof of universal
  correctness. Queue candidates are unadmitted.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T79 | S1 investigation and S2 repair complete; owner testing passed and closure approved. | [Audit](../history/M9-T79-completion-audit.md) |
| T78 | S1 complete; owner manual test accepted; Common type boundary closure. | [Audit](../history/M9-T78-completion-audit.md) |
| T77 | S1--S3 complete; owner-authorized self-review closure; test ownership cleanup, isolated packages and dual-width acceptance. | [Audit](../history/M9-T77-completion-audit.md) |
| T76 | Owner testing passed; S1 measurement cancelled by owner, S2/S3 complete; overlay index and snapshot compatibility accepted. | [Audit](../history/M9-T76-completion-audit.md) |
| T75 | S1--S4 complete; owner approved closure; strict C11 six-package and dual-width acceptance. | [Audit](../history/M9-T75-completion-audit.md) |
| T74 | S1 complete; owner accepted; dual-width 110/110 background; pure Core relocation. | [Audit](../history/M9-T74-completion-audit.md) |
| T73 | S1--S5 complete; owner accepted; final dual-width 110/110 background. | [Audit](../history/M9-T73-completion-audit.md) |
| T72 | S1--S8 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T72-completion-audit.md) |
| T71 | S1--S10 complete; owner approved; final dual-width 105/105 background. | [Audit](../history/M9-T71-completion-audit.md) |
| T70 | S1--S12 complete; owner approved; disclosed Linux limit. | [Audit](../history/M9-T70-completion-audit.md) |
| T69 | S1--S4 complete; reopened cleanup accepted. | [Audit](../history/M9-T69-completion-audit.md) |

## Recent Governance

- T79 closes after owner manual acceptance; its proposal is archived. Queue
  head is admitted as T80 S1; seven-step plan retained, XP rebase stays queued.
  This handoff changes documentation only; accepted EXEs are unchanged.

- T78 closure archives its proposal after owner manual acceptance. No active
  packet remains; queue candidates retain their original order until separately
  admitted.
- T77 closure archives its proposal and accounts for all 48 original product
  files and 115 CTest definitions. Task-owned disposable outputs removed;
  TODO remains empty and the two unrelated queue candidates remain unadmitted.
- T76 closure archives its proposal and verifies scope, hashes and full-task
  coverage. Owner admits queue head as T77 S1; this handoff changes docs only.
- T74 closure archives the proposal and records S1/T-level requirement coverage,
  actual-change review and owner acceptance. Sources and tested EXEs unchanged.
- T73 closure records S5 acceptance, all-S requirement/changed-path audit and
  the separate queued wake-race receiver, and archives its proposal. No new
  implementation task or T number is allocated.
- M9 Td S18: T71 closure, TODO retirement and floppy-identification candidate;
  [record](../history/M9-Td-S18-t71-closure-and-floppy-queue.md).
