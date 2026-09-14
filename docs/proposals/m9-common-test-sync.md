# T58: Common Test Corpus Synchronization

## Original request and boundary

“准入一新的T任务，引入nxvm的test/common/保持两边内容一致。
完成后作为S任务收口，保持T任务开放等我测试测试。不得改动src。”

Baseline a1c24ce, clean worktree. S1 imports the complete test/common snapshot
from the read-only NXVM checkout (HEAD 4da5d23c), with no src changes and no
runtime/build dependency on that checkout. No product semantics, configuration,
media or shared production source changes are admitted.

## S1 finite ledger and exit

Universe: every relative file path in both test/common trees at admission.
There are 16 files on each side: 15 identical and session_frame_smoke.c with
additional monitor callback assertions in NXVM. Preserve the 15 unchanged;
import that one file exactly, then recheck all paths and SHA256 values.
Source tree must have zero diff against the baseline. Run dual-width builds
and full suites, including the focused frame test. Fixed EXEs remain the
only package outputs; no INI or media edits. Stop if tests require src changes.

One executor P contains import and verification evidence, is committed/pushed,
then actual-diff review accepts S1 in a second P. T58 remains open with no
next S admitted while the owner tests. The XP candidate and existing TODOs
remain separate. This is a test-only import, not an NXVM architecture migration.

## Verification checkpoint: upstream manifest mismatch

The imported file matches NXVM SHA256
9D3BA1ED5437C6CC490A856B4DC4EFDC953B690C7CD3201AB016B9C716B20F2D,
but its unchanged manifest records
12F088CC881F3B70B1F1AA57A2CDCF38331B8B0B4CDE32E62F5DAADA04E61CA0.
The same verifier fails on the NXVM test directory itself. Both-width builds
succeed and Common tests pass 15/16 at each width; only common.test-manifest
fails. The new session_frame assertions pass at both widths. No src changes.
Test delta is one file +10/-1; no runtime changes or new license claim.
NXVM's root MIT notice remains upstream provenance; this bounded test update
does not alter existing notices or the repository's license policy.

Exact import and successful manifest verification cannot both be claimed for
this snapshot. Await NXVM manifest correction or owner direction on a local
manifest correction followed by NXVM synchronization. Do not suppress the
verifier, commit a passing claim, or close S1 while this mismatch remains.

## Owner-approved resolution

“批准，以softpc这边为准，nxvm后续从softpc拿代码。”
SoftPC corrects the single stale manifest hash without changing the imported
test or src. NXVM subsequently adopts this corrected corpus. Exact equality
to the old upstream manifest is superseded by this explicit owner decision.
All other 15 files remain equal to the admitted snapshot.

To represent the requested S-closed/T-open wait without a fictitious next S,
the product documentation verifier accepts the explicit idle marker
`Open task awaiting owner: T58.` only for the latest recorded, non-closed T.
Positive and missing/wrong/already-closed negative fixtures cover this state.
This supporting tooling correction does not modify src or shared test files.

## Executor evidence

Only test/common/session_frame_smoke.c (+10/-1) and its manifest hash (+1/-1)
change in the transferred corpus. The imported test remains byte-identical to
NXVM; the approved manifest correction is the sole downstream difference.
Product source accounting: zero. Governance tool accounting: +28/-2, covering
one idle-wait check and four deterministic positive/negative fixtures.
All previously existing assertion and manifest checks are retained.

Both checked-in build presets completed successfully. Fixed executable hashes
are unchanged from T57, since this task has no production-source change.
x86 full regression passes 85/85 (58.60 seconds); governance-focused rerun
passes 2/2 after the tooling change. The first x64 full run passed 84/85:
compact Console integration timed out at stage 5, while every Common test,
including the corrected manifest, passed. Both widths had been running
concurrently; that is context, not a proven cause. Serial x64 verification
follows without product or assertion modifications.

Serial x64 full regression passed 85/85 (49.60 seconds). The first stage-5
failure remains an observed baseline-package timeout, not a repaired defect;
it is transferred to the package acceptance TODO. Final comparison confirms
src/lib 94/94, src/common 36/36, test/lib 39/39 identical; test/common has 16
paths and only the approved manifest differs. Source diff from a1c24ce is zero.
Governance checker/self-tests and the real Common test manifest pass.

## S2: Application composition (owner-admitted extension)

Owner asks for a composition file integrating command and keyboard with
Common session, then admits: “准入S任务。编译测试，提交推送，保持工作区干净”。
The earlier no-src restriction applied to S1; S2 explicitly changes app only,
plus product tests/build/design. Lib, Common, VM, Compat, MVDM and shared tests
remain unchanged from 00d4461. T58 remains open after this S.

Finite ledger: every command_binding function/state and every consumer.
Command/debug/effect conversion moves into command; hotkey interpretation
moves to keyboard; composition holds only provider wiring and the adapter
that reserves command admission for a keyboard-derived lifecycle request.
No duplicate dispatcher, extra thread, policy rewrite or new state is allowed.
Main remains the sole VM consumer and owns entity construction; composition
does not take over the VM. Remove command_binding C/H and all live references.
Retain parser and debugger integration tests; extend provider-level hotkey
coverage, enforce the single wiring boundary, and run both full-width suites.
Complete executor commit/push then actual-diff review closes S2, not T58.

## S2 executor evidence

The former binding ledger is exhausted: seven provider callbacks and their
debug/conversion helpers moved to command; hotkey interpretation moved to
keyboard; provider registration and its single context/admission adapter are
in composition. Main constructs the same entities. Old binding C/H are gone.
No additional state, allocation, worker or dispatch queue was introduced.
The adapter still reserves the original command transition before returning
a CAP-derived request; rejected transitions return no request as before.

Both package builds succeed. Serial full x64 tests pass 85/85 (53.31 s),
x86 85/85 (61.74 s), including real debugger/provider integration, paused
CAD/CAF suppression, CAM, CAP reservation, CLI policy and package tests.
The existing debugger assertions remain; the renamed provider test additionally
checks direct callback wiring. The source gate rejects retired binding files,
policy interpretation in composition and hotkey registration elsewhere in app.
Protected src/lib, src/common, src/vm, src/compat, src/mvdm, test/lib and
test/common have zero diff from 00d4461; no INI or media changes.

Accounting via git diff 00d4461 --numstat: src/app +335/-304 (net +31),
primarily declarations required for direct provider wiring; product test C
+40/-24 (net +16), build CMake +8/-7, source-boundary gate +17/-0.
No compatibility forwarding implementation remains. Package SHA256:
softpc32.exe CF7A8B9096361EBEF02C711E7023C3D9A87B37F5B13A00F3B288408A9F145EEC;
softpc64.exe 427AEF45CCD85FD4DB001A22464FFD727D9DE964FFBCA53BA457F2FE1CF10E30.

## S3: Single application assembly owner

Owner approves the proposed relocation: “准入一个s任务进行搬迁”。
Baseline 1cc141e. Main retains argument/config loading and exit reporting;
composition owns existing entity construction, event callbacks, session run
and cleanup via app_composition_run(config). Composition becomes the sole
App VM consumer. S2's main-only assembly decision is superseded by this request.
Do not add state, threads, forwarding layers or reorder creation/destruction.
Protected source and shared tests remain unchanged; T58 remains open.

Finite ledger: main's VM/options conversion, machine/session/UI creation,
two machine event callbacks, provider/hotkey wiring, session run and cleanup.
Every member moves to composition; argument/config/path validation stays main.
Trace reset moves with VM assembly, after config validation: failed startup
validation now preserves the old diagnostic trace rather than clearing it.
Normal startup and all product output/order remain unchanged; no extra trace
API or duplicate trace implementation is introduced.
Audit the actual old/new body, update all boundary consumers and negative
fixtures, run both full suites and publish both fixed EXEs. Executor P is
pushed before actual-diff S acceptance; no production fallback remains.

## S3 implementation review

Every ledger member now resides in composition; main retains only startup
validation and config loading plus one blocking call. The existing function
body from options conversion through cleanup is byte-equivalent after changing
config member access to pointer access and returning lib_status instead of an
exit-code boolean. Main converts that status to the same exit code. Callbacks
are moved intact; startup errors still print before cleanup. No new state or
thread. Provider initialization remains directly used by the existing tests.
Its test target links Common session because composition now owns that run.

Boundary checks now allow only composition.c -> vm_interface.h; added negative
fixtures reject main and composition.h as VM consumers. Existing illegal-edge
fixtures remain. Searches of all app constructors, bindings and VM includes
find only composition. Protected source and shared test corpora have zero diff.
Production src/app accounting from 1cc141e: +118/-110, net +8; test C unchanged;
test support +6/-4, CMake +1/-1. No forwarding or old assembly path retained.

Initial concurrent -j8 builds exited with make Error -1, without compiler
diagnostics. Lower-concurrency x64 and serial -j1 x86 builds completed.
First x64 suite was 84/85: package startup debugger-help stage 17 failed with
a 30-column captured viewport; unchanged focused rerun passed (5.21 s).
No test or runtime change was made for that failure; the distinct observation
is recorded in TODO rather than claimed repaired. Final full verification
follows. Package hashes:
softpc32.exe 14EFCFB486AE33724291793F56D20967B718E8121809079D9C61143F81BC6642;
softpc64.exe ED4EA8B39B3A10511323EF515F3E0879B7B31F1668C45CE4F0D48E6AAD5DA4A2.

Second full x64 run passed every product test but the unchanged types-layout
self-test exited without diagnostics (84/85, 46.70 s). Its verbose focused
rerun passed (21.10 s); this transient tool exit is recorded separately in TODO.
x86 full verification passed 85/85 (57.23 s). No code/assertion changes were
made between these runs; final serial x64 acceptance is recorded below.

Final serial x64 suite passed 85/85 (49.34 s). Both widths therefore have
complete passing runs of the delivered code, with earlier observations retained
above and in TODO. Documentation governance and boundary negative checks pass.

## S4: Quiescence before ordered teardown

Owner requests UI -> session -> machine -> VM destruction. Inspection found
session exit only requests asynchronous stop; the machine can still emit state
and frame callbacks into session. Owner explicitly approved extracting the
existing stop/join into synchronous shutdown: “好的。批准。开始执行。”
Baseline f30b6c5. This supersedes the earlier Common no-change restriction only
for machine shutdown and its shared tests/docs/manifests. Lib/VM/Compat/MVDM
remain untouched. No new thread, state machine, lock or shutdown event.

Finite ledger: existing destroy stop/join, never-started/active/paused worker,
outstanding callbacks, repeated shutdown/destroy, partial create failure,
post-shutdown requests that would otherwise wait for a dead worker, and the
one App cleanup path. Shutdown permanently joins and clears the existing worker
handle; destroy delegates before resource disposal. The serialized owner must
not call shutdown from a worker callback or race it with API calls. Product stop
remains restartable; shutdown is not a new product command.
App shuts down first while all sinks live, destroys UI while session can still
receive retirement events, then session, command/debug, machine and VM.
Test callback completion with barriers, not sleeps; retain normal stop/restart
coverage and prove no second disposal callback. Add a permanent order check.
Build both fixed EXEs, run full suites/manifests, push executor then review
actual changes for S acceptance. T58 remains open.

### S4 implementation ledger

- Worker stop/join: extracted from destroy into shutdown; destroy delegates.
  Existing worker pointer becomes NULL after join, so repetition is a no-op.
- Callback targets: App shuts down while session/command/UI/VM are alive, then
  destroys UI, session, command/debug, machine, VM. UI retirement can still
  enqueue into the live session. Debug close can still reference machine.
- Request sweep: cold-run admission rejects a missing worker (start/reset);
  media admission rejects it before waiting. Pause/resume/input/debug execution
  already reject the terminal stopped/error state. Stop and debug cancellation
  do not synchronously wait or emit callbacks in this state. No new state enum.
- Partial create: all handles start NULL and worker is created last; shutdown
  skips a missing worker while destroy still releases each allocated resource.
- Verification: the shared machine smoke retains normal lifecycle/debug tests,
  covers never-started and stopped shutdown, and blocks a final callback during
  running/paused shutdown. A helper-thread completion barrier proves shutdown
  waits; repeated shutdown and destroy leave notification count unchanged.
  No sleep or polling delay is used to assert the barrier.
- Single production route: rg of App destroy calls finds only composition;
  its exact ordered cleanup is guarded in the existing source-boundary test.
  Lib, VM, Compat, MVDM and test/lib have zero diff from f30b6c5.

Accounting: production C/H +21/-7, net +14 (App +3/-2, machine C +12/-5,
public declaration/contract +6/-0); shared test C +64/-0; product gate +3/-0.
No added runtime object, state field, synchronization primitive or ABI payload.
Common source/test manifests mark shared-t58-s4-p1 for downstream adoption.

S4 verification: x64 build passed; focused machine/barrier, manifests and
source-boundary checks passed 4/4. Full x64 passed 85/85 (88.85 s), full x86
passed 85/85 (94.64 s), run serially. The first x86 parallel build command
reported exit -1 after linking the package without a compiler diagnostic;
the subsequent serial build was confirmed exit 0. No source or assertion was
changed for that retry. Documentation governance and diff checks pass.
Both fixed package hashes:
softpc32.exe 4DC9840EBDB285900898DD1373687DC9B66739086162C6F9D1F47868C369337A;
softpc64.exe 2FC00B4E09124D21EAC5FC30B4099F641B8807B8A4D698AED736374D85F4A64B.
INI and media are unchanged. Owner interactive acceptance remains separate.
