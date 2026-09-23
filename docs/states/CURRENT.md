# Project Status

## Current Work

T81 S8 investigates and repairs the observed first-use PC-speaker playback
instability after the owner accepted S7's canonical Audio import: the first
`AUDIO.COM` run is silent, the second is discontinuous, and later runs are
continuous until a hot guest reboot repeats the sequence.

## M9 T81 S8 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepted S7's original product behavior and its canonical Audio corpus import, then reported the repeatable first-use symptom. Owner admits S8, delegates investigation and repair authority, does not require a manual test gate before an executor delivery, and permanently authorizes an original-mirror repair only when it reduces the total OpenNT mirror diff. |
| Objective | Establish one deterministic PC-speaker start/stop handoff so the first eligible guest tone is submitted and rendered like later tones, without product polling or a second producer/delivery path. |
| Non-goals | No Common/App/x86/snapshot/configuration change; no guest sound card, DMA, IRQ, OPL/MIDI, volume policy, guest timing change, platform-specific Compat API or net increase in the OpenNT mirror diff. Linux remains unsupported. |
| Reference Baseline | SoftPC executor `4157e918` after owner-accepted T81 S7. MyNES `b48e57f` remains the unchanged shared Audio baseline. |
| Candidate Proposal | [Neutral audio](../proposals/m9-neutral-audio-stream.md) |
| Files And ABI Surface | `core/compat/audio.c`, `core/compat/platform.c`, the preserved `core/softpc.new/base/keymouse/ppi.c`, their existing host sound endpoint and focused Core tests. Touch Lib only if a demonstrable Audio contract defect, rather than a PC-speaker ownership defect, is the root cause. Public Audio ABI and MyNES-shared code remain unchanged unless the same test proves a general defect. |
| Applicable Rules | Execution, Architecture, Coding, Documentation and source research policy; shared Lib strict C11/package rules. |
| Verification | Trace all original `LazyBeep` transitions, Compat tone requests, Audio FIFO submissions, native waveOut completion/reset operations and guest-reset state. Add a deterministic start/stop sequence test at the owning boundary; then run focused Audio/Compat tests, strict C11/package builds, background regression, manifests/DAG/governance on x64/x86. |
| Expected Markers | Exactly one Compat square-wave producer and one Audio delivery worker remain; standalone PPI writes use the original NTVDM complete post-gate state transition; no periodic readiness polling, product WinMM vocabulary or net-increased original-mirror diff appears. |
| Asset Needs | Refresh the two package EXEs. Existing owner media may be read for diagnosis but is never modified, staged or committed. |
| Reporting Requirements | Report the observed causal chain, why it reproduces after hot reset, production/test additions, deletions and net, all similar-path dispositions, exact automated results and artifact hashes. |
| Stop Conditions | The only reproducible correction requires a guest-timing change or a net-increased original-mirror diff; a native output callback must enter product code; a platform-specific Compat API would be needed; or the symptom cannot be isolated from user media. |
| Exit Criteria | A deterministic focused proof covers a fresh first tone, stop, repeated tone and guest reset boundary; the repair preserves one producer/worker topology and public Audio ABI; dual-width required gates pass. T81 remains open for its separate ledger audit. |
| Original Owner Request | Build neutral src/lib/audio and test/lib/audio first, then later connect SoftPC speaker and eventually separately study XP sound-card support. |
| Similar-Issue Sweep | Search all `softpc_standalone_audio_set_tone`, Audio create/clear/cancel/shutdown calls, original `LazyBeep` init/reset paths and all potential first-use output paths. Retain only the single Compat producer and the single Audio worker; record every hit's disposition. |

## Current Technical Baseline

- S8 P4 correctly converged the cold-reset Timer2 gate to original PPI state,
  but owner reproduction afterward disproves it as the first-use playback
  cause.  An owner-run trace now proves the entire first `AUDIO.COM` handoff:
  Timer2/PPI produces 439Hz, Compat's sole worker produces PCM, Audio accepts
  it, and the first `waveOutWrite` succeeds.  P6 established the need for a
  general Win32 Audio constructor reset, but its pre-header placement was not
  equivalent to a later `clear`: the owner still reproduced cold-boot silence.
  P7 moves that same reset after headers are prepared and before publication,
  matching the completed-stream reset used by MyNES. Later tones already had such a
  reset, explaining the first-versus-later distinction without a SoftPC
  exception.  Public Audio ABI, topology and original mirror remain unchanged;
  focused fake/native-thread proofs pass on x64/x86. The native Audio smoke
  now queues two valid 512-frame submissions then flushes their full 1024-frame
  batch, so an available WaveOut endpoint must accept a real native write; the
  first-tone proof also verifies nonzero PCM. The local x64 native endpoint
  completed that full path; owner desktop audibility remains the pending
  criterion.

- T81 S8 P1 restored eager neutral-stream creation before the Compat speaker
  task and removed the lazy-create wrapper. Its lifecycle tests passed, but the
  owner reproduced the first-run silence afterward, so P1 is not the causal
  repair and S8 remains active. The original `LazyBeep(CLICK, 1)` path reaches
  its native Beep device even though it resets its cached state; Compat had
  instead discarded every sub-10ms request. P2 transfers one coherent
  frequency/duration request to the existing speaker producer, renders finite
  PCM requests through the existing Audio flush operation, and leaves the
  original mirror, public Audio ABI, native worker and product polling topology
  unchanged. Focused x64/x86 proofs cover the delivered 48-frame 1ms tail;
  background regression passes x64 114/114 (151.94 s) and x86 114/114
  (154.18 s). Both package EXEs are refreshed for owner-visible testing; S8
  remains active pending that acceptance.

- T81 S6 executor `37562abb` closes the missing-Window admission gap with one
  Session condition: only actual RUNNING can emit `CREATE_WINDOW`. Paused
  Window retention and `window_suppressed` close reconciliation are unchanged.
  Production C +4/-1 (net +3), test C +32/-0; the source/test manifest hashes
  are refreshed. Direct state matrix, manifests/governance and background
  product x64/x86 119/119 pass; desktop tests excluded. [S6 evidence](../history/M9-T81-S6-window-creation-admission.md)
  records the finite non-running-state sweep and EXE hashes.

- T81 S4 executor `d5dc2245` changes no code: a fresh exact 189-file copy of
  `src/lib`, `src/common`, `test/lib`, `test/common` has 0 hash mismatches and
  independently passes strict C11 Lib 44/44 and Common 18/18 on both widths.
  Hidden product background regression passes x64 114/114 (210.77s), x86
  114/114 (176.91s), desktop excluded. Package launchers remain intentionally
  byte-identical to S3 because no code input changed. S4 review/closure is
  recorded in [S4 evidence](../history/M9-T81-S4-four-package-acceptance.md).

- T81 S3 executor `d8830b53` replaces the Win32 Audio placeholder with one
  direct four-slot `waveOut` owner. It adds only Types-owned WinMM vocabulary,
  Audio-private native state and Audio-owned `winmm` linkage; Linux remains an
  explicit unsupported placeholder. Production C/H +177/-15 (net +162), test
  C +165/-1 (net +164), generated/build/docs +28/-19 (net +9). Native silent
  open/enqueue/clear/reuse and injected open/prepare/write/reset/unprepare/close
  failures pass; `LIMIT_EXCEEDED` is recoverable backpressure rather than a
  terminal failure. Isolated Lib: x64 44/44 (64.02s), x86 44/44 (47.87s).
  Product background: x64 114/114 (212.80s); x86 1--107 plus independently
  rerun 108--119 pass after stale test-child cleanup. Desktop excluded, no
  Linux runtime/audible-product claim. Both package EXEs refreshed; S3 review
  and hashes are recorded in [S3 evidence](../history/M9-T81-S3-win32-audio.md).

- T81 S2 executor `fbfa9437` adds only the neutral Audio root contract,
  deterministic fake backend and C11/DAG/manifest integration: production C/H
  +259/-0, test C +121/-0. The two S2 platform leaves explicitly return
  UNSUPPORTED; no SDK header, real playback, worker/ring or product hookup is
  present. Isolated Lib passes 42/42 on x64/x86; product background x64 112/112
  and x86's same 112-item route pass. S3 owns native Win32 playback.

- Owner approved S1 amendment: S5 will adapt the existing SoftPC PC Speaker to
  finished Lib PCM after S2--S4, leaving `nt_sound.c` and guest device semantics
  unchanged. T81's retained plan is reactivated while implementation proceeds.

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
