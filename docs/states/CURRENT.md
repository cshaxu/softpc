# Project Status

## Current Work

## M9 T79 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved the S1 repair design and requested S admission, implementation, dual-width build/test, commit/push and a wait for manual validation. S1 is complete; S2 is admitted. |
| Objective | Separate physical floppy identity from inserted media in the existing GFI host adapter; retain empty A:, absent B:, stable hardware type across insert/eject/reset and snapshots. |
| Non-goals | No guest-label workaround, Win95-specific branch, shared-corpus or Core mirror edit, user INI/media write, new controller or snapshot compatibility layer. |
| Reference Baseline | `f6dadddd`; unrelated queue/proposal edits are preserved and excluded. [S1 record](../history/M9-T79-S1-investigation.md). |
| Candidate Proposal | [Win95 A: floppy drive identification repair](../proposals/m9-win95-floppy-drive-identification.md). |
| Files And ABI Surface | Core compat GFI lifecycle and device archive, existing Core tests and snapshot integration tests. Private physical-identity snapshot fields; no shared API change. |
| Applicable Rules | `docs/rules/{EXECUTION,ARCHITECTURE,CODING,DOCUMENT}.md`, `docs/design/{ARCHITECTURE,CODING,UI}.md`, and the Core mirror boundary in `AGENTS.md`. |
| Verification | Both-width empty/present BIOS identity, media modes/profiles, insert/eject/reset/destroy, no-media FDC response and snapshot roundtrip; full background CTest and Release x86/x64 packages. Owner cold-boots Win95 for GUI acceptance. |
| Expected Markers | Empty boot and eject/reset retain A: type and equipment presence. B stays absent. Media access modes remain unchanged. Snapshot retains hardware type independently of media. |
| Asset Needs | Test-generated small images/snapshots under build only; no owner media/config writes. Background tests avoid desktop interaction. Refresh only the two package EXEs. Task-owned build/t79-s2 diagnostics are bounded to 120 seconds/run and 10 MiB logs, with agent cleanup. |
| Reporting Requirements | Before: plan and line estimate. After: counted production/test additions/deletions/net, focused/background results and both EXE links. Disclose snapshot layout change and manual acceptance remaining. |
| Stop Conditions | Stop for a shared API, mirror behavior edit, new controller or user-media mutation need. Do not claim Win95 acceptance from BIOS-only tests. |
| Exit Criteria | Approved bounded repair and tests complete; both Release packages and background suites pass; actual-change review, complete P committed/pushed; wait owner manual validation before closure. |
| Original Owner Request | “队列第二位追加一个T任务，用于修复win95把软驱A盘识别为‘可移动磁盘’而不是软驱的故障，Td治理。” |
| Similar-Issue Sweep | Map both floppy drives, all readonly/direct/overlay media modes, cold-reset and snapshot restoration routing; distinguish shared contract defects from Win95-only presentation. |

## Current Technical Baseline

- T79 S2 implements independent GFI physical identity and media lifetime.
  Production +56/-23 (net +33), tests/scripts +187/-37 (net +150); three
  production files, no shared-corpus or Core mirror changes. Both Release
  packages, focused 7/7 per width, background x64 110/110 (178.87s) and x86
  110/110 (163.39s) pass; five desktop cases excluded. Empty/present snapshots
  pass both cross-width routes. The device stream adds eight identity bytes;
  old-format snapshots are not accepted. Owner cold-boot Win95 validation
  remains outstanding; S2/T79 stay open. Executor P1 `cba189b5` is pushed;
  actual-change review accepts delivery for manual testing. Full ledger/hashes
  and review are in the proposal. Unrelated queue/proposal edits are preserved.
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
