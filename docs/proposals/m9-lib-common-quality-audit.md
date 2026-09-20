# Shared Corpus Quality And C11 Qualification

## Owner Request And Admission

T75 follows accepted T74 at 188d2bfd. The owner supplied the NNES audit,
clarified that NNES also uses root test/ beside src/, and required C11 for
src/lib, src/common, src/x86, test/lib, test/common and test/x86.
The owner admitted all four S tasks serially: before each report the method
and estimated additions/deletions; after each build, test, commit, push and
provide both EXEs and actual counts. Continue without intermediate manual
approval; stop after S4 for owner acceptance, keeping T75 open.

## Frozen Coverage Ledger

The universe is the four supplied report items plus the explicit six-corpus
C11 scope. Dispositions are verified repair, disproved premise, or explicitly
bounded validation. Completion requires evidence for every row, not a claim
that every possible runtime interleaving or receiving emulator was tested.

| Item | Evidence and disposition | Owner |
| --- | --- | --- |
| Paused shutdown wake | Outer worker waits on command_event without cancellation; paused callback can reset it during shutdown. Queued report records dual-width timeouts. Deterministic schedule required. | S2 |
| Session initialization | Five positional initializers in control.c fail strict C11 and C17; omitted members are zero-initialized, not garbage. | S3 |
| Test location | Both projects use test/* beside src/*; ../../src is correct. No configurable path mechanism needed. | S1, S4 proof |
| Language standard | Lib standalone says C11; Common, x86 and three test entries say C17. Select C11 in standalone and embedded builds. | S3 |
| Exact-copy reuse | Four neutral directories and six-directory set build/test without App/Core or external repositories. | S4 |
| x86 strict warnings | Expanded six-package syntax audit finds unused parameters and empty trace-macro else bodies in aasm32.c/dasm32.c. Preserve dispatch shape and behavior while making intent explicit. | S3 |

## S1: Intake And Baseline

Finalize dispositions and plan. Production/test estimate +0/-0, net 0; task
records only. Rebuild both Release packages and run background presets.
Baseline passes do not prove the queued race safe. Actual-change review and
closure precede S2 under the owner's serial authorization.

## S2: Cancellable Machine Shutdown

Use existing Base task cancellation in the outer worker wait. Preserve the
single executor, normal command wake, terminal request completion and callback
lifetime. No new state, Lib API, polling or timeout recovery. Extend existing
tests with deterministic old-fails/new-passes scheduling, native-thread
repetition and dual-width background regression. Absorb the queued terminal
wake proposal here, not a second active repair. Estimate files/lines before edits.

## S3: Six C11 Packages

Select strict C11 without extensions for all six package scopes, including
under the product C17 build. Fix five initializers with explicit fields and
sweep equivalent strict-compiler failures. No ABI, CLI, device or runtime
semantic change; no warning suppression. Check actual compile commands and
-Wall -Wextra -Wpedantic -Werror builds on both widths. Refresh manifests.
App/Core retain their standard.

## S4: Independent Reuse Acceptance

Copy four neutral directories and all six directories byte-for-byte into
task-owned ignored build/ children, preserving src/ and test/ layout.
Independently build/test, verify manifests/DAGs and repeat product background
regression. Prefer existing verification entries, not a new runner framework.
No NNES runtime acceptance or Linux presenter parity is implied.

## S1 Audit Evidence

Source searches: C_STANDARD/compile options across six package CMake entries;
base_sync_event_wait/wait_any/task_cancelled/terminate_requested across Common;
read Base wait/task destruction and Machine inner/outer loops. Five Session
initializers reproduce -Wmissing-field-initializers with strict C11. The other
six Common translation units passed strict C11 syntax checks on both widths
in the intake audit; no link/runtime claim follows from syntax checks.
The x86 source entry and test/x86 entry also explicitly select C17.
Both test/common and test/lib locate ../../src correctly in the confirmed
layout. Existing shutdown schedule tests and native x86 test are available;
the latter currently waits for STOP before destruction, avoiding rather than
proving the reported immediate-close interleaving safe.

S1 changes no production, test, build or tool source. Rebuilding kept both
package hashes unchanged from T74. Both Release builds passed; background
x64 110/110 (191.93s), x86 110/110 (186.10s). Five desktop tests per width
were excluded. Documentation gate and diff check pass. Source/test/build/tool
actual +0/-0, net 0, matching estimate. No new runtime correctness claim.

## S2 Implementation And Sweep

Admission estimate: production +5/-3, tests +45/-3, net +44 total, plus three
manifests and records. Actual source scope is the same three C files.
The deterministic existing machine_wait harness scripts debug wake, stop and
task cancellation before the paused callback resets command_event. The old
worker fails its bounded assertion (x64, 0.68s); the new worker uses the real
Base cancellation object after proving the command event is now unsignaled.
Focused machine_wait/common_machine/x86.debug_machine pass (3/3).
No production hook, task field, allocation or Lib change is introduced.
The native x86 test now destroys immediately after debugger close; it no longer
uses STOP completion as a workaround.

Sweep command: rg base_sync_event_wait/wait_any/task_cancelled in src/common.
Both executor waits now observe task cancellation. Four synchronous caller
waits (media, state read/write, debug) wait for request completion, not executor
work; existing finish_requests completes their owned slots before disposal.
Session queue_take is a caller-supplied bounded/infinite queue wait, not a task
join loop; its existing control event owns termination. No second matching
uncancellable worker wait remains in Common. Existing tests cover fault versus
cancel, reset/resume/stop, all five pending-slot forms, join failure retention,
never-started shutdown and blocked-final-callback ownership.

Actual C diff: machine.c +5/-3; machine_wait_smoke.c +35/-2;
debug_machine_smoke.c +1/-3. Production net +2, tests net +31; total
+41/-8, net +33 (estimate net +44). Manifest hashes refreshed for those
three files. Each focused test passed 50 repetitions on x64 (18.00s) and
x86 (19.31s). The extra cancellation task exists only in the scheduling test.
Both Release builds and background suites pass: x64 110/110 (185.64s),
x86 110/110 (168.37s); five desktop tests per width excluded. EXE SHA256:
x86 E6D738B78BB07F1B9F25F98AE07A6E6AEFAB3655B1DB9F88D990C399E0F6F46F;
x64 E483BF53CE7D694B0AA0F3343704FF09079CE80E17152AE5C343260737735BFF.

## S3 Design And Estimate

Baseline 48454ba9. Estimate 12--16 code/build/test files: C/H production
+20/-10, build/tests +55/-65, net near zero, excluding manifests/docs/artifacts.
Each package sets its own C11/no-extension directory scope after standalone
project initialization, independent of the parent C17 setting. GNU/Clang
strict warnings apply in those scopes only; replace Lib's optional per-target
warning loop with the same direct policy. No new configuration knobs/framework.
Design/CODING records the owner's explicit six-package C11 override; rules and
App/Core settings remain unchanged. Partial event/keyboard initializers become
designated; unused legacy parameters retain call shapes with explicit void use;
trace-break keeps breaking the caller loop, never a new do/while loop.

The preliminary raw-file sweep also encounters the intentionally broken include
fixture and Linux-only hard-link test under a Windows compiler. Neither is a
configured Windows translation unit; do not alter these tests to make that
diagnostic invocation pass. Actual configured compile commands define the
strict-build universe. Linux platform runtime coverage remains separate.

Code-generation diagnostics refine that universe: retain five intentional
fallthroughs with comments; remove only unreferenced static assembler INT3 and
disassembler POP_CS helpers (no table/dispatcher edits). SIB initialization
cannot affect a decoded operand: all uses require mod != 3 and rm == 4, whose
earlier path reads SIB or fails. Add complete MOV ModRM/SIB length coverage.
An optimized included-source Base test exposes a handles initialization false
positive: the existing count guard already rejects zero. A do/while states
that existing nonempty-copy invariant without clearing unused array slots or
adding validation/allocations. These are strict build fixes, not new behavior.
The same optimized sweep flags t/xt count after invalid argument counts: return
immediately after the existing seterr, rather than relying on the subsequent
error-position test to prove count is assigned. Valid trace behavior is unchanged.
The x86 compiler additionally requires an initial false value for the cooked
restore test's pending output. Its first assertion still requires true, then
later assertions require false; no assertion is removed or relaxed.
The now-unused not_cancelled stub left by S2 is deleted, not annotated unused.

Actual compile-command audit: each width has 94 shared-target C entries:
Lib 25, Common 7, x86 4, test/lib 37, test/common 15, test/x86 6.
All select -std=c11 and all four strict warning flags. Product/Core settings
are unchanged. The MOV test covers 6376 ModRM/SIB combinations.
Current tracked C/H/build accounting (16 files): production +29/-30 (net -1),
six CMake entries +36/-35 (net +1), tests +20/-6 (net +14); total +85/-71,
net +14. This exceeds the near-zero estimate only through additional targeted
test coverage, not new runtime state or API. Readmes/manifests/docs are separate.
Final post-build background runs: x64 110/110 (135.03s), x86 110/110
(158.30s), five desktop cases excluded per width. Both strict Release builds
pass. Earlier in-progress regressions are not used as final evidence.
All six manifests, DAG/negative gates and documentation/diff checks pass.
EXE SHA256: x86 6CF44CB6C5F74EC003237BDB6687D918458757018279C95B9831114EFEC99A40;
x64 A37641357713B3225BCDE8D4649F4BDF1B137F026E8B0CF016949E69BB7101E7.

## Delivery Conditions

One S active at a time. No desktop interaction: background presets and
standalone -LE desktop. Preserve INI, media, snapshots and Core.
Per-S git diff --numstat uses the admission baseline; separate production,
tests/build, documents and artifacts. Complete implementation P precedes
coordinator review and closure P. Stop on unbounded scope/new behavior;
never suppress failures. Remove task-owned temporary outputs after recording
evidence. T75 needs final coverage review and owner acceptance before closure.
