# Project Status

## Current Work

M9 T85 S7 is verified for delivery: IBM PC source/test import and PC-test ownership.
T85 remains open; S6 delivered verification remains recorded without inferred
manual acceptance.

## M9 T85 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner admits src/ibmpc and test/ibmpc import, correct inward boundaries, and relocation/deduplication of previously excluded PC-dependent Core tests. NXVM stays read-only. |
| Objective | Establish the fourth shared layer and independently owned IBM PC tests; restore unique PC-composition coverage without cross-suite borrowing. |
| Non-goals | No SoftPC CPU/backend replacement, IBM PC product launch, new firmware/media, six-component runtime redesign or user INI/snapshot change. |
| Reference Baseline | SoftPC pushed 45a461a2; read-only NXVM ebdb40098dfaa7b2bcc0f294d52e3f19dafd6629 with clean admitted roots. Owner INI/snapshot remain excluded. |
| Candidate Proposal | [T85 proposal](../proposals/m9-windows-101-startup-repair.md), S7 eight-package and prior-exclusion ledger. |
| Files And ABI Surface | src/ibmpc and test/ibmpc; test/x86 ownership cleanup if required, shared test tools, root test registration/LF attributes, design/README/manifests and dual EXEs. Existing SoftPC public ABI unchanged. |
| Applicable Rules | Execution, Documentation, Architecture, Coding, source-research policy and shared governance skills; shared C11; Lib then Common then x86 then IBM PC; tests own fixtures. |
| Verification | Independent IBM PC Release x64 161/161; final background x64 443/443 (161.57s), x86 443/443 (237.47s); eight manifests/DAG and Types/ownership negative probes pass. Dual Release PE widths and product link audit pass; five desktop cases per width excluded, no Linux execution claim. |
| Expected Markers | Inner production/tests never require IBM PC; no cross-test-package source; unique PC cases retained once; SoftPC still uses its original executor. |
| Asset Needs | Refresh only standard dual EXEs; synthetic in-code fixtures only, no guest media or native desktop automation. |
| Reporting Requirements | Before/after production/test files and added/removed/net lines; imported versus local changes; every former S6 excluded case mapped to retained, deduplicated or unused disposition. |
| Stop Conditions | Unknown provenance, unexplained failed tests, original-mirror/runtime policy changes or required third-party firmware stop delivery. |
| Exit Criteria | Finite import/exclusion ledger complete; independent and product gates pass; P committed/pushed; wait for owner testing, T remains open. |
| Original Owner Request | 很好。准入一个Ｓ，把ibmpc的2个src和test组件也导入了吧，然后正确实现边界；原来在core的被删除的测试，是不是该挪进ibmpc的test，如果没有重复的话？ |
| Similar-Issue Sweep | Eight source/test layers; include/link/build/fixture closure and duplicate scenario registrations; all 51 prior excluded paths and all upstream IBM PC members. |
| Scratch And Cleanup | Ignored build/t85-s7-ibmpc only; thirty-minute cold build budget, ten-minute per test/suite budget, bounded logs; remove owned scratch after evidence summary, preserve reusable root builds/build/output and owner configuration/media. |

S7 imports all 126 IBM PC production paths byte-identically and owns 269 test
paths. All 51 prior PC exclusions are dispositioned: 45 reachable paths return,
six unused helpers remain omitted; 38 unique PC cases return. The retained
suite has no cross-test-package source dependency; duplicate protected IRET
remains owned once by x86. Code/build +64,583/-10, net +64,573, principally
imported source/tests. Existing Lib/Common/x86/App/Core production is unchanged;
IBM PC and the shared x86 Core are absent from SoftPC's EXE link inputs.
The [S7 ledger](../etc/evidence/m9-t85-s7-ibmpc-import-ledger.md) records exact
scope, local adaptation, failed/interrupted attempts and final qualification.
Executor P1 `8a967acf` is pushed. Coordinator actual-change review confirms
the 413 delivered paths match the packet and exclusion ledger, all 126 source
paths remain exact, no existing production/API/runtime link changes, and the
dual regression/architecture results recorded above. Owned scratch is removed;
only the two preexisting owner INI/snapshot changes remain. S7 awaits owner
testing and T85 stays open; delivery is not an S/T closure.

S6 P1 376f9278 and P2 45a461a2 are delivered: dual Release/background
282/282 each; isolated Lib/Common/x86 48/48, 20/20, 167/167.
[Delivery handoff](../history/M9-T85-S6-shared-boundary-delivery.md).
New S7 admission does not fabricate a manual-test acceptance or close S6/T85.

S5 remains verified awaiting owner acceptance; admitting S6 does not infer
an unreported manual-test result or undo its delivery.
Actual S5 code: stream.c +19/-10 (net +9), existing Audio smoke +42/-1
(net +41); combined +61/-11 (net +50). Two component README changes add
10 lines; three manifest hashes are refreshed. No Common runtime, x86,
App/Core, public ABI or mirror change. Dual EXEs are refreshed; owner INI and
snapshot edits are preserved outside this delivery. See proposal S5 evidence.
Executor P1 `23990ab2` is pushed. Coordinator actual-change review confirms
the eleven delivered paths match the packet, no prohibited boundary change,
and unchanged EXE hashes after verification. S5 awaits owner testing; not closed.

M9 T84 S8 is owner-accepted and closed. It converges raw Console backing-store
preparation with exact frame-owned output coverage: native viewport, font fit
and scroll position remain host-owned; steady 25-row output writes 25 rows;
former tails remain covered across partial writes and are clamped after an
external backing-store shrink. Both Release packages, focused Console tests
and hidden-background CTest pass 121/121 on x64 and x86. Executor P1
`b8c1ed3f` and P2 `b79769c1` are pushed. T84 remains open for later owner
direction. [S8 closure](../history/M9-T84-S8-console-convergence-closure.md).

M9 T84 S9 is owner-accepted and closed. It restores source-local key balance
after a KVM producer loses host focus or freezes through one semantic reset and
the existing Common Session delivered-key ledger. P1 `3186afcd` delivers the
behavior; P2 `40da7d00` adds direct native-loss and rejected-reset coverage.
Serial hidden-background CTest passes 122/122 on x64 and x86. Package EXEs
remain the accepted P1 artifacts. [S9 closure](../history/M9-T84-S9-input-reset-closure.md).

M9 T84 S10 is owner-accepted and closed. It synchronizes complete `test/lib`
and `test/common` to read-only NXVM `0c71110b0`, including deletion of the
Common-private Window reset smoke in favor of canonical Lib-owner coverage.
P1 `bbdc1d68` passes exact 71-path equality and serial x64/x86 background
CTest 121/121. [S10 closure](../history/M9-T84-S10-shared-test-corpus-sync-closure.md).

M9 T84 S11 is owner-accepted and closed. It documents the direct checked-in
x64 package route and portable MSYS2 x64/x86 source-build routes, and removes
the x86 preset's private developer path. P1 `e238a470` passes fresh x64 package
build, real 32-bit MinGW configuration, marker sweep, whitespace and
documentation governance. No product source, INI, media, snapshot or package
EXE change is retained. [S11 closure](../history/M9-T84-S11-quick-start-closure.md).

M9 T84 S12 is owner-accepted and closed. It imports NXVM `572293efc`'s
source-local unmatched-release behavior into the canonical Lib corpus without
an API or state expansion. P1 `b8c0f5fb` passes exact `src/lib` (109-path) and
`test/lib` (51-path) equality, focused lifetime proof, Release builds and
serial background CTest 121/121 on x64 and x86. [S12 closure](../history/M9-T84-S12-nxvm-orphan-release-import.md).

M9 T84 is owner-accepted and closed. Its twelve accepted S deliveries cover
the bounded 80x50 KVM route, subsequent shared-corpus convergence, raw Console
output/input correctness and fresh-checkout usability. The task-level ledger
and retained boundary/deferral dispositions are in the
[T84 completion audit](../history/M9-T84-completion-audit.md).

## T85 Awaiting Owner Direction

M9 T85 S1 is owner-accepted and closed on 2026-09-28. P1 `fc74d65e`
repairs mono-painter publication; P2 `ee62ad01` repairs InPort scheduling and
snapshot ownership; P3 `9d102563` records delivery review. Both Release
packages and background suites pass 121/121 per width; cross-width snapshot
tests pass. The owner accepts the updated-driver guest installation after
the separately authorized, backed-up test-HDD replacement.

[S1 closure](../history/M9-T85-S1-windows-101-startup-repair.md) records the
whole-S path counts, acceptance, retained artifacts and verification limits.

T85 S3 is owner-accepted and closed on 2026-10-04. P1 cec8a3dc and review
P2 a6a777d1 are pushed. The 219-file
NXVM shared subset plus test/register.cmake is byte-identical; eight local x86
packaging files retain debug/xasm32-only scope. Both Release builds pass.
x86 background passes 121/121; x64 passes 120/121 initially, with the sole
documentation-record ordering failure corrected and its rerun passing 1/1.
No production change; EXEs retain accepted S2 hashes. T85 remains open.

[S3 closure](../history/M9-T85-S3-nxvm-shared-subset-sync.md).

T85 S4 is owner-accepted and closed on 2026-10-04. Observation P1
`1bc02185` and repair P2 `a841b5d3` are pushed. Original idle detection and
the existing executor event now park idle DOS execution. Headless idle CPU
falls from x64 97.27/x86 96.78 percent to x64 0.26 percent and x86 below
accounting resolution. Final background suites pass 121/121 per width;
the earlier intermittent x86 roundtrip failure and passing reruns remain
disclosed, not diagnosed by repetition. Production +20/-7, tests +56/-0:
combined code net +69. No shared-corpus or original-mirror change.
[S4 closure](../history/M9-T85-S4-dos-idle-cpu-repair.md) retains acceptance,
actual-change review and limits. Owner INI/snapshot edits remain preserved.
T85 stays open; S5 is subsequently admitted above.

M9 T85 S2 is owner-accepted and closed on 2026-09-28. P1 `8c15e893`
repairs indexed DIB ownership; P2 `f8fe3df8` records actual-change review.
Both Release builds and serial background suites pass 121/121 per width.
The owner accepts the mouse regression repair. T85 remains open; the next
Windows 1.01 color question is read-only investigation, not implementation.
[S2 closure](../history/M9-T85-S2-indexed-pointer-repair.md).

M9 T84 S6 is owner-accepted and closed. It closes the admitted product
host-boundary defects without changing Lib, Common, x86, the Core mirror, user
INI, snapshots or media: parallel output retains only an unconfirmed suffix on
short writes; parallel snapshot validation completes before allocation; audio
teardown propagates failure through the App/VM lifetime boundary; quoted INI
paths retain `#`/`;`; and the obsolete permanent VM trace path is deleted.
Focused injected cases, both Release packages and hidden-background CTest all
pass 121/121 on each width. The final x86 SHA-256 is
`566CD3F1922C6F0F6427DE4B307E3E974562ABB3478CC4C8DD6D05BBA6275D8F`; x64 is
`8885A1C99E98CEEB623A644ED50B2CDE87EB4314161F59D88E0B9F014B22DF1B`.
Executor `13b4aef3` is pushed. T84 remains open for later owner direction.

M9 T84 S5 is owner-accepted and closed: NXVM's complete six-component
shared corpus, including Audio, was imported from clean `057d8c9aa5`; the generic
test helper is now at the owner-approved shared `test/register.cmake` location.
All six roots and that helper are byte-identical to NXVM. The `lib_bool` ABI change
required a complete dual-width rebuild and six narrow SoftPC callback/test receiver
adjustments; it did not add a product behavior branch.
Both Release packages and both background CTest suites pass 120/120. Refreshed
x86 `F1E748E86FF800ACF4C1BE8FF75B675BC4995CD3FC39752B948D5D32620E8658`,
x64 `840347402647A6F4988DCC16A4B22E6E5FF853D5E15E7C78A26331DB74410856`.
Executor `e2b81d70` is pushed. T84 remains open for later owner direction.

Owner defers the pre-existing x86 BIOS[0x52] null-dispatch fault to
[TODO](TODO.md) on 2026-09-25; no CPU/BOP repair belongs to S1.
The 80x50 implementation builds on both widths; final background regression
passes 120/120 on x64 and x86. The deferred fault is not claimed repaired.
T84 S1 is owner-accepted and closed. Executor P1 `503d6632`, delivery
review P2 `dc9c34ce`, and S1 acceptance P3 `39366670` are pushed. Owner
accepts S2's document-only whole-task audit and admits S3 on 2026-09-25.
S3 is owner-accepted and closed. It imports NXVM shared commit `b7cbb30a9`'s
deterministic Audio adapter test exactly: test/build +220/-212 (net +8),
production +0/-0. Both Release packages build, focused Audio tests pass on
both widths, and background CTest passes x64 120/120 (250.98s) and x86 120/120
(249.05s). Six manifests, Lib DAG, Common/x86 corpus and documentation
governance pass. NXVM later acquired uncommitted shared-root edits; they are
not part of S3. [S3 record](../history/M9-T84-S3-nxvm-audio-test-import.md).
Refreshed packages: x86 `FEA84DEF15C2A25DC962579087D60F5DC94E11B9E46A792E13B3C3B60408D8A5`,
x64 `0EC6096600E1702CE92751F4AEE1BC16C5FD009FF4E6D2DF537A97774672148F`.
The established S1 evidence confirms 24 paths,
production +19/-11 and tests +108/-19, no mirror/INI/media change, matching
dual-EXE hashes and the seven-member coverage ledger. Both Release builds,
full background 120/120 and delivery rechecks 18/18 pass per width.
Five desktop tests per width remain excluded. Owner reports manual acceptance
on 2026-09-25. S2 performs no product-code change; no T84 closure is claimed.

## Current Technical Baseline

- Owner closes T83 after S1--S6 delivery and admits T84 for bounded 80x50
  text frames. [T83 completion audit](../history/M9-T83-completion-audit.md)
  preserves scope and verification; this admission changes documents only.

- T83 S6 is owner-accepted and closed. The selected C-VID glue ABI is declared
  once in `cpu_vid.h`; EGA dot-read is declared once in `egavideo.h`. Both
  Release builds and background CTest presets pass. [S6 audit](../history/M9-T83-S6-cvid-abi-closure-audit.md).

- The completed T83 App Types/Base convergence covers 109 tracked non-mirror App files; 54 direct Types consumers migrated and four mirror-shared Compat ABI bridge headers remain. Both Release builds and their full CTest runs pass. EXE hashes and the full ledger are in [the S4 audit](../history/M9-T83-S4-lib-types-base-closure-audit.md).


- T82 closes after owner acceptance of S2. The six shared directories have
  220 paths: 214 content paths are byte-identical to NXVM's current worktree;
  the only six differences are local `MANIFEST.sha256` revision/hash records.
  Base process discovery now owns executable-directory lookup and App appends
  the unchanged `softpc.ini` filename. S2 P1/P2 are `2ef228f8`/`0448fa4f`;
  x64 and x86 builds passed, each background suite passed 116/116 before the
  manifest-only P2, and the final dual-width manifest/corpus/negative checks
  passed. Package EXEs remain from P1; INI, media, Core, Compat and VM are
  unchanged. [T82 audit](../history/M9-T82-completion-audit.md) records the
  complete changed-path and difference ledger.

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
| T84 | S1--S12 complete; owner approves task closure. | [Audit](../history/M9-T84-completion-audit.md) |
| T83 | S1--S6 complete; owner approves task closure. | [Audit](../history/M9-T83-completion-audit.md) |
| T82 | S1/S2 complete; owner accepts the six-component convergence. | [Audit](../history/M9-T82-completion-audit.md) |
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
- M9 Td S19: refreshes the unadmitted XP candidate so XP SP1 is its intended
  routine mirror baseline, while OpenNT remains lineage evidence;
  [record](../history/M9-Td-S19-xp-baseline-proposal-refresh.md).
