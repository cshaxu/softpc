# Project Status

## Current Work

T82 S2 imports the complete non-Audio Lib/Common/x86 source/test corpus from
NXVM, including Base executable-directory discovery, and uses that imported
capability for SoftPC's existing adjacent-INI lookup. Audio remains SoftPC's
retained corpus. The owner approves preserving SoftPC's correct x86 raw-CRT
negative test and its derived manifest until NXVM takes that one-line test fix.

## M9 T82 S2 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits S2 after accepting S1's strict non-Audio convergence plan, and clarifies that the intended scope is a whole six-directory import: every non-Audio Lib, Common, x86 and matching test path enters together. Owner clarifies that SoftPC must not import or alter Audio: NXVM independently adopts SoftPC Audio. Owner then approves retaining SoftPC's correct `memcpy()` raw-CRT negative probe and its derived x86 test manifest because NXVM's `lib_memory_copy()` probe cannot test the stated rejection. The approved product adaptation remains only SoftPC's existing adjacent-INI lookup through imported Base process discovery. |
| Objective | Import every selected non-Audio Lib/Common/x86/test path from NXVM `d485a54e`, except the owner-approved correct x86 negative-test pair; retain and validate SoftPC Audio; import Base process discovery; and replace the direct Win32 executable-directory call in SoftPC App with that neutral contract without changing the resulting `softpc.ini` path or media resolution. |
| Non-goals | No Audio source, Types Audio declaration, Audio build/test/document behavior, Core, Compat, VM, package INI, media, or NXVM worktree modification. No product behavior change beyond using the same resulting adjacent-INI path. Mixed Audio/non-Audio build and evidence files receive only necessary surgical reconciliation; a full Audio convergence is not admitted. |
| Reference Baseline | SoftPC `df9d1cf8`; NXVM `d485a54e`, with its six shared directories recorded clean in S1. |
| Candidate Proposal | [NXVM shared-corpus import readiness](../proposals/m9-nxvm-shared-corpus-import.md) |
| Files And ABI Surface | Selected non-Audio `src/{lib,common,x86}/**` and `test/{lib,common,x86}/**` paths, including imported `base/process` and platform declarations; Audio-owned portions of mixed Lib files remain excluded. `src/app/config.c` and its focused test coverage may change solely to call `base_process_executable_directory()` before appending `softpc.ini`. Public shared interfaces follow the exact imported corpus; no product ABI changes are admitted. |
| Applicable Rules | Execution, Architecture, Coding and Documentation rules; shared strict-C11/package/DAG requirements. |
| Verification | Exact selected-path comparator before/after, standalone strict-C11 Lib/Common/x86 suites on x64/x86, focused executable-directory/INI-path proof, product background regression on x64/x86, shared DAG/manifests as applicable, and documentation governance. Desktop tests remain excluded unless explicitly reserved. |
| Expected Markers | All imported non-Audio shared paths are byte-equal to NXVM; the one correct raw-CRT negative-test pair and Audio-only mixed paths are explicit retained differences; all six local manifests validate their actual selected corpora; `app_get_config_path()` has one Base process call and preserves output behavior. |
| Asset Needs | Refresh only package `softpc32.exe` and `softpc64.exe`; preserve owner INI and all media. |
| Reporting Requirements | Before implementation report actual selected path list and source/test line estimate. At delivery report exact changed-path and +/-/net code accounting, excluded Audio disposition, exact-path proof, builds/tests and package links. |
| Stop Conditions | An imported non-Audio path requires a product adaptation beyond the approved App directory call; an Audio-owned mixed section is required for successful build; public ABI/product behavior changes; NXVM becomes a build/runtime dependency; or exact source cannot compile without a local fork. Stop and report rather than making a SoftPC variant. |
| Exit Criteria | All imported paths match NXVM; the approved x86 negative-test pair and Audio-only mixed paths are the complete retained-difference ledger; all six manifests validate; App preserves adjacent `softpc.ini` discovery; admitted tests/builds pass on both widths; one complete P is pushed; worktree is clean except owner changes; and delivery awaits owner acceptance before a task-level convergence audit. |
| Original Owner Request | Audit NXVM six components and prepare import of every component except Audio. |
| Similar-Issue Sweep | Enumerate every Audio-related Lib/test/build path to prove exclusion; scan imported Base process consumers and all App executable-directory lookups to prevent a duplicate direct Win32 implementation; scan all six source/test roots for retired `lib_c_*` spellings and duplicate retained source paths. |

## Current Technical Baseline

- S7 shares one internal write algorithm; ordinary write flushes on success,
  nonempty DIRECT fill flushes once even after partial failure, preserving the
  first error. No public signature, platform, cache or rollback change.
  Production +23/-7 (net +16; three added comment lines), tests +88/-0;
  combined +111/-7 (net +104). Strict shared C11 dual Release builds pass.
  Background x64 111/111 (214.52s), x86 111/111 (194.39s); standalone Lib
  41/41 per width (60.92/63.05s). Desktop excluded; no Linux runtime claim.
  Six manifests/DAG pass; task scratch removed, EXEs refreshed; INI/media and
  Common/x86/Core unchanged. [S7 evidence](../history/M9-T80-S7-storage-fill.md)
  records actual-change review of pushed executor `75dfd5a6`;
  S7 is owner-accepted and closed with T80.

- S6 moves 512 fixed disassembler handlers to file-local const tables; parsing
  context remains per-call. Production +520/-521 (net -1), tests +42/-0;
  total code +562/-521 (net +41). No handler/output/public API change.
  Strict C11 Release builds and background x64 111/111 (177.48s), x86 111/111
  (169.15s) pass; standalone x86 suites 10/10 per width. Five desktop tests
  excluded per product width; no Linux run. Temporary task outputs removed.
  EXEs refreshed, INI/media and Lib/Common/Core unchanged. Owner closes S6;
  Executor `5d0515fd` pushed;
  [S6 evidence](../history/M9-T80-S6-xasm32-dispatch.md) records actual review.

- S5 removes Common's stale standard-header allowlist and rejects raw integer
  limits. Seven Machine constants use equivalent existing Types aliases.
  Runtime +7/-7, checker +4/-4, tests +24/-0: total net +24, no ABI change.
  Both Release/background builds pass: x64 111/111 (273.67s), x86 111/111
  (178.46s). Copied four-directory tests 59/59 and six-directory tests 69/69
  pass on both widths, strict C11; all 180/205 copied files remain identical.
  Desktop tests excluded; no Linux runtime claim. Isolated task outputs removed;
  package EXEs refreshed; Lib/Core/x86, INI/media unchanged. S5 accepted for continuation.
  Executor `c7c42ea5` pushed; actual-change review retained in
  [S5 evidence](../history/M9-T80-S5-common-types-gate.md).

- S4: frame publication skips 0 on u32 wrap; Session's two ordering sites share
  one private modular comparator. Production +16/-4 (net +12), tests +69/-0;
  public widths and UI equality deduplication unchanged. Compared serials must
  be less than 2^31 apart. No new state, queue or cache; Lib/Core/x86 unchanged.
  Both Release builds and focused 3/3 pass per width. Background x64 111/111
  (173.24s), x86 111/111 (160.81s); five desktop tests excluded per width,
  no Linux execution. Manifests/DAG and final documentation gate pass.
  Standard EXEs refreshed; hashes in the proposal, INI/media unchanged.
  Executor `478d04ec` pushed; actual-change review recorded in
  [S4 evidence](../history/M9-T80-S4-frame-sequence-wrap.md). Owner accepts S4 closure.

- S3: production +6/-4 net +2, tests +56/-0; reject an unrecordable new key
  before machine delivery, retaining original repeat/identity/retirement logic.
  Both Release builds and background suites pass: x64 111/111 (141.46 s),
  x86 111/111 (140.30 s). Five desktop tests excluded per width; no Linux run.
  Package EXEs refreshed; hashes and finite sweep are in the proposal.
  Executor `aea41c16`, review `2cd9e889`; owner accepts and closes S3.

- S2 audit baseline `1b5d0fd7`: production/tests +0/-0, existing EXEs unchanged.
  Existing Common suites pass 18/18 per width; three focused cases pass twenty
  repetitions each per width. No rebuild, new fault injection, full-product,
  desktop or Linux run. The owner accepts the documented native-failure limit;
  no new fatal callback, queue or synchronization recovery mechanism is added.

- T80 S1 is owner-accepted and closed: production +11/-2 (net +9), tests/build
  +149/-0; public API and debugger callers unchanged. Executor `73a5a889`,
  review `f4941481`; strict C11 isolated x86 10/10 and background 111/111 on
  each width. Five desktop cases excluded each; no Linux runtime claim.
  [S1 review](../history/M9-T80-S1-xasm32-boundary.md) and proposal retain hashes.

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
| T81 | S1--S8 complete; owner accepted cold first-run `AUDIO.COM` playback; neutral Audio and the PC-speaker handoff close. | [Audit](../history/M9-T81-completion-audit.md) |
| T80 | S1--S7 complete under approved scopes; owner acceptance and separate whole-task audit close the task. | [Audit](../history/M9-T80-completion-audit.md) |
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

- Owner reports acceptance and approves T80 closure. S7 closes, the proposal
  is archived, and no next task is admitted. This closure changes documents
  only; accepted EXEs, configuration and media remain unchanged.

- Owner closes T80 S6, admits S7 and approves its narrowed one-write-algorithm
  design. S7 delivered for acceptance; T80 not closed automatically.

- Owner admits T80 S6 after S5 delivery. S6 delivered for testing; S7 remains
  unadmitted. No separate S5 manual-test result is inferred.

- Owner approves T80 S4 closure and admits S5's Common Types boundary repair.
  T80 remains open; S6/S7 are not started.

- Owner closes T80 S3 and admits S4. Document-only handoff preserves tested
  package EXEs; T80 remains open.

- Owner approves T80 S2 closure after the narrowed audit, then admits S3.
  Document-only handoff preserves the accepted binaries; T80 stays open.

- Owner closes T80 S1 and admits S2's notification-failure design audit.
  Handoff is document-only; accepted EXEs unchanged; T80 remains open.

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
