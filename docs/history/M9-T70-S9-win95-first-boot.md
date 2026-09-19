# M9 T70 S9: Windows 95 first-boot repair

## Final closure after resolving package failures

P5 `7c0c786` resolves the previously unexplained failure in the test fixture,
not the application. Final x64 and x86 full suites each pass 109/109. Both
normal and compact Console-display supplemental cases also pass on each
width. Existing window visibility, responsiveness, debug and restart checks
remain enforced; no timeout increase or skipped assertion is used.

Coordinator reviewed P5's actual diff and the P2 PIT repair together. P5 is
test-only +14/-7; P2 production remains +8/-1 in timer.c. Lib/Common and user
configuration remain unchanged. Both rebuilt package hashes match those
recorded below. The original root cause, installation-stage evidence and
regression requirements now have affirmative evidence. This closes S9 under
the owner's delivery request; their instruction to stop subsequent interactive
installation verification still applies. Complete installation is not claimed.
T70 remains open for feedback. Earlier failed results and withdrawn closure
are retained below as history, not current unresolved test failures.

## Rejected closure and resumed investigation

The owner rejected S9 closure because the two failed integration tests remain
unexplained. The P3 closure decision is withdrawn, not treated as permission
to waive failures. S9 is active again; P2's repair remains intact. A fresh x64
package-smoke run fails after 10.60 seconds with Machine started printed but
without reaching pause. The next proof must distinguish a missing/invisible
Window from a failed test observation before any repair or closure.

Package-test follow-up: added timeout observation finds the Running Window
exists with its correct title but is not visible. The fixture launches the
whole process with STARTF_USESHOWWINDOW/SW_HIDE; Win32 applies that startup
value to the child's first ShowWindow, including the KVM Window. Repair the
fixture to hide only its dedicated Console after attaching, without a process
show override. Keep visibility and responsiveness assertions. Estimated change
is approximately fifteen test lines, no product or shared-corpus change.
Verify both package routes, both widths, and full regression before closure.

The fixture fault dates to `2c05238` (T59 S1), when the checked-in INI selected
Console display. `40f3936` later changed it to Window display, exposing the
contradiction between hidden-process startup and required Window visibility.
The first failing observation was exists=1, visible=0, title=Insignia SoftPC
(Running). With identical product binaries and unchanged assertions/timeouts,
the corrected fixture passes both package cases on both widths. Full CTest
now passes 109/109 on x64 and 109/109 on x86, replacing the earlier unresolved
107/109 result. Both release builds succeed; package hashes are unchanged.
Separately compiled fixture instances using a disposable Console-display INI
and the same overlay backing image pass normal and compact modes on both
widths. The user-owned package INI is untouched. This verifies the original
Console branch as well as the configured Window branch.

Similar-issue sweep: `rg -n 'STARTF_USESHOWWINDOW|wShowWindow|IsWindowVisible|ShowWindow'`
over test/tools finds this one process-wide hidden launch. The Console display
test hides only its own allocated Console; the KVM retirement test deliberately
hides its window while testing lifecycle, not visibility. Neither has the
contradictory acceptance condition. Keep the package visibility assertion as
the regression check and report existence/visibility/title on timeout instead
of only the coarse stage=16. This follow-up changes one test file +14/-7
(net +7), with no production or shared-corpus edits.

### Superseded P3 decision

Implementation delivery: `5d6fe18` (S9 P2). The owner subsequently instructed:
“等等 先不用 你就把最新修复的binary编译好 测试提交推送 收口这个s任务给我”.
This stops further interactive installation verification and closes this S at
the repair-delivery boundary; it does not assert that installation completed.
T70 remains open.

The final 580-second cold-start observation reaches the date/time properties
dialog during installation configuration. Its frame remains stable awaiting
interaction. Public snapshot save succeeds and leaves the machine paused;
the observation process exits normally. Original backing-disk SHA-256 remains
unchanged. A subsequent desktop diagnostic was stopped on the owner's request
before installation interaction; it contributes no additional acceptance proof.

Coordinator review inspected the actual P2 production, test and CMake diff:
one existing PIT calculation distinguishes estimated and actual time; no new
persistent state, lifecycle, library boundary or guest predicate is added.
The nine focused combinations cover modes 0/2/3 and divisors 1/16384/65536,
read-only interpolation and subsequent real heartbeats. Dual-width builds
pass; full regression remains 107/109 on each width, including the two
previously disclosed stage-16 package failures. They are not claimed fixed.
The final focused test expansion also passes on both widths.
At closure both complete builds were rechecked successfully and the focused
PIT, IRQ, snapshot transaction and snapshot boundary tests passed 4/4 on each
width. Package hashes remain identical to P2. The unchanged full-suite result
above is the earlier P2 run, not a newly repeated full suite.

| Closure requirement | Evidence / disposition |
| --- | --- |
| Establish cause, not infer from black pixels | Cold-start IRQ excess and fixed-clock reads independently reproduce manufactured PIT interrupts. |
| Minimal repair and similar paths | P2 corrects the common non-NTVDM count updater; sweep and accounting below. |
| Reach subsequent graphical setup | Cold start reaches hardware/PnP and then date/time configuration, with copied-frame evidence. |
| Dual-width artifact and regression | Both release builds and PIT tests pass; aggregate 107/109 each, known failures disclosed. |
| Complete interactive installation | Owner explicitly stopped this verification; not claimed complete. |
| Preserve assets/shared boundaries | No Lib/Common or INI change; original disk hash unchanged; writes used overlay. |

## Retained investigation record

Owner reports abnormal display on the first system startup after Setup has
finished copying files. It is not yet established whether the machine boots
with broken output or fails to boot. Owner supplied a cold-start recipe:
start the installed disk with the accepted x64 package; the startup banner,
first graphics initialization and a minutes-long blinking text cursor precede
a second graphics initialization which remains black. No snapshot load is
involved. Active admission is recorded in Current.

The investigation first records the supplied steps, exact executable/config,
media identity and any snapshot dependency. Observe guest execution progress
and disk activity alongside video registers, copied frames and final output.
Use existing debugger/observation interfaces first. A static screen alone is
not evidence of a CPU hang; continued execution alone does not prove boot
progress. Classify the earliest confirmed divergence before proposing repair.

| Coverage member | Required disposition |
| --- | --- |
| Reproduction | Owner steps reproduced, or precise difference/unavailable input recorded. |
| Guest progress | Boot stage and bounded CPU/interrupt/disk evidence; distinguish useful progress from a loop or wait. |
| Display | Compare guest video state, frame publication and visible result at the failing stage. |
| Restore dependency | Record whether the case requires snapshot load; compare a normal boot only when matching disposable input is available. |
| Root and repair boundary | Evidence-backed fault owner and minimal proposed fix, or explicit unresolved hypotheses and next discriminating observation. |

Initial production/test diff estimate is zero until a repair owner is evidenced.
Do not alter Lib/Common, patch guest binaries, add Setup-specific behavior,
or change snapshot/media semantics. The owner has now admitted root-cause
repair and next graphical installation-stage verification. Record an
evidenced brief and estimate before the corresponding production change.

Use owner media read-only or disposable copies as required by the reproduction
recipe. Diagnostic output belongs only under ignored `build/t70-s9`, capped
at 600 seconds and 64 MiB per run, with early stop on no additional evidence
after reaching the reported stage. The longer time budget accommodates the
owner's observed minutes-long initial boot; observations remain sparse.
The agent owns process termination and deletion of its exhausted diagnostic
outputs; retain only the minimal checkpoint needed to support conclusions.
No external fixture content enters source control. T70 remains open.

Owner additionally authorizes using the existing snapshot feature to shorten
repeated observation. Keep one local checkpoint (including overlays), with a
128 MiB checkpoint limit in addition to the 64 MiB diagnostic-output limit.
Use public machine state read/write APIs, never reconstruct an ad hoc state
from RAM dumps. Original media remains unmodified.

## Cold-start observation

The external x64 executable hash matches the accepted package. Its INI uses
16 MiB RAM, overlay hard disk, console display and console_control=0. The
supplied disk SHA-256 is unchanged after investigation. No snapshot was loaded.

Two ignored diagnostic executables linked the existing x64 production VM,
Compat, MVDM and Common machine objects. They invoke ordinary machine start
and copy published frames; they do not instantiate the desktop presenters or
exercise the monitor parser. Thus the core black-output symptom is reproduced,
not yet an independently witnessed end-to-end desktop interaction.

- Around 20 seconds, execution reaches protected-mode code; around 35 seconds
  the published output changes from the startup image to text.
- Around 95 seconds, output changes to 640x480 graphics and is black. The first
  run initially holds frame sequence 9 and unchanged storage-operation counts.
  Around 129 seconds disk activity resumes briefly. A later frame contains a
  small hourglass on a black background.
- After approximately 160 seconds, frames continue at roughly 10/second but
  remain black apart from cursor content. Storage counters stabilize at 3450
  reads and 4890 writes. These are medium API operation counts, not sectors or
  physical-disk writes; all writes remain overlay-only.
- CPU instruction sampling continues across different addresses, including
  kernel and ring-3 code. The second run reaches over 864 million observed
  instruction entries by 237 seconds. Repeated GP/invalid-opcode entries are
  counted, but their presence alone is not diagnosed as an emulator defect.
- Executor-side video capture at approximately 180 seconds finds only 415
  nonzero bytes in the 524288-byte plane allocation and a nonzero DAC palette.
  The black image is already present upstream of KVM output; a missing final
  window repaint is not sufficient to explain it. Incorrect emulated video
  writes, or guest initialization which never produces the intended picture,
  remain unresolved alternatives.

The first run was stopped after roughly 270 seconds once its observations
stabilized; the second exited normally at 240 seconds. This is not proof that
an indefinitely longer boot cannot progress. No production/test source or
package was modified. No repair or S closure is claimed.

| Coverage member | Current disposition and next proof |
| --- | --- |
| Reproduction | Cold-start core symptom reproduced twice; full desktop confirmation remains separate. |
| Guest progress | CPU remains active, late disk activity ceases; identify the sustained wait/fault/service path before calling it a boot hang. |
| Display | Published black frames agree with almost-empty emulated video memory; trace first intended display writes and their physical destination. |
| Restore dependency | Reproduced without load; snapshot restoration is not a necessary trigger. |
| Root and repair boundary | Not established; retain research scope, with no speculative Lib/Common or guest-specific fix. |

### Checkpoint follow-up

A checkpoint taken through the public machine-state API reproduces the black
frame with the same continuing kernel/cursor activity. Diagnostic disk reads
use the restored medium, including its overlay, not the untouched backing
file. The current IOS log reports fallback to real-mode disk drivers. The
controller's last command is rejected and completed, with BUSY and command-in-
progress cleared; the observed PIC has no pending/in-service disk interrupt.
This does not prove the disk path correct, but does not support a controller
stuck BUSY as the immediate explanation.

The repeated invalid-opcode address initially noticed in ROM is explicitly
recognized by the guest kernel as a V86 callback return sentinel. It is not
evidence of accidental execution of ROM font data. Likewise, the dominant
non-kernel drawing loop matches a VGA-driver cursor path, not installation
dialog rendering. Next observation must identify the blocked or unfinished
startup task rather than patching these expected paths. Root cause remains
unproven; production code and package are unchanged.

A subsequent read-only checkpoint observation finds a stronger timing lead:
PIT channel 0 remains in rate-generator mode with initial count 16384, while
the diagnostic IRQ0-entry counter advances about 4000 requests/second and the
BIOS tick value about 950/second. Counting wraps the existing PIC callback
without suppressing or changing requests. This is not yet a root-cause claim:
the next check must distinguish a sustained timer-generation defect from a
restored backlog, and reproduce the discrepancy on cold start before repair.

Cold-start timing observation reproduces the IRQ excess after channel 0 is
programmed to 16384; the host heartbeat remains about 20/second. A separate
fixed-clock device probe programs rate-generator channel 0 and performs 1000
latched reads without advancing any timer heartbeat or CPU execution. It
produces two immediate IRQ requests, an active deferred interrupt event and
3552 additional queued requests. Thus register reads alone create interrupt
work. The read-time guess reaches `updateCount`'s wrap handling and advances
the activation timestamp, allowing subsequent reads to repeat the effect.

Repair brief: distinguish estimated read progress from actual clock progress
inside the existing PIT count update. Preserve estimated visible count, but
do not issue terminal/wrap notifications or advance the actual activation
timestamp from a guess. No new stored state, host timer, OS predicate or
shared API. Estimate roughly 10 production lines in original timer.c and a
focused device regression test/build registration. Verify repeated reads at
several divisors and modes, then real elapsed ticks, dual-width regression,
and cold-start installation progress. The deterministic PIT defect is proven;
its sufficiency to explain and fix the installation stall is not yet proven.

The candidate production patch is +8/-1 in timer.c. The new device test
programs modes 0, 2 and 3 with divisors 1, 16384 and 65536, performs 1000 latched
reads without executor/heartbeat advancement, verifies no IRQ or deferred
interrupt work and an unchanged activation timestamp, then verifies that two
actual heartbeats still produce interrupts. Both x86 and x64 pass the final
nine-case focused test. The only production guess-to-wrap path is this shared
counter updater; callers include heartbeat, reads/latches and state transitions.
The fix is not a special treatment of any guest, divisor or read caller.

Candidate cold start now produces roughly 80 IRQ0 requests/second and 20 BIOS
ticks/second at divisor 16384, rather than thousands. At about 60 seconds it
renders the installation background; at 90 seconds it renders the hardware/
Plug and Play configuration dialog. Reads and writes continue beyond 220
seconds. This directly contrasts with the baseline black/cursor-only state.
It proves entry into the next graphical installation stage at the copied-
frame boundary, not yet full desktop interaction or completed installation.
Both complete builds pass. Full CTest is 107/109 on each width, with only the
previously disclosed package-smoke and compact-console tests failing at stage
16. These test failures remain unexplained, not waived or claimed fixed.
The final test expansion to mode 0 was rebuilt and rerun on both widths after
the full suite. Further installation observation remains in progress.

Delivery accounting against `bbfa863`: one production file +8/-1 (net +7),
one new 80-line test, and CMake +8/-0; total code/build net +95. No new
production allocation, persistent state, ABI, clock source or component edge.
The local `estimated` value distinguishes provenance within the existing
calculation. Source sweep: `rg -n 'guess\(|updateCount\(|setLastWrap\('`
over timer.c finds one read-time estimate producer and the shared count/wrap
consumer; all non-NTVDM counter modes use that corrected boundary. NTVDM's
separate real-time implementation does not call guess and is unchanged.

Package SHA-256: x86 `037D38F51F112C11210221931E2D3A9BEF24834FF12A2D1CAF505B0C6074953C`;
x64 `36F21B0B2C8BA68376355BF9FF98C494D4EEBD31E1FEC87BE87C785DEAA05CEB`.
The x64 package is also copied to the owner-designated reproduction directory.
No owner configuration, original disk, shared Lib/Common corpus or snapshot
format is changed. This delivery does not close S9 or prove full installation.
