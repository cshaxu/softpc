# Windows 1.01 startup diagnosis and repair

## S5: minimal Audio repair extension

Owner request: 我们的原则永远是减少复杂度而不是增加复杂度。第三项否决。
修复一二的逻辑和四五的契约说明。可以，那就按这个方案走。你现在开始执行。

The frozen four-item ledger is: idle wait failure uses existing failure and
detach; failed native cancellation returns before join and retains ownership;
Console mouse documents fixed 8x16 logical scaling; graphical status text
documents printable ASCII and fixed 80x25 output. The first two require fault
injection in the existing Audio smoke; the last two are documentation-only.
Session notification redesign is rejected and is not an implementation item.
No new API, state, object, thread or recovery loop is allowed. Production net
estimate is 10-20 lines in stream.c; tests and documentation are counted separately.
Both Release packages and background suites are required before delivery;
owner acceptance, not executor verification, closes S5.

### S5 implementation evidence

One production owner remains: audio/stream.c. The existing first-failure helper
now signals existing producer/control waiters, replacing duplicated attach-failure
signals. Idle waits continue only on SIGNALED, exit normally on CANCELLED and
latch IO_ERROR then detach on other results. Destroy checks native cancellation
before joining; failure retains the whole stream. A retry after worker disposal
does not cancel an already detached worker again. There is no added state/API,
allocation, task, retry policy or platform implementation.

The existing Audio test injects idle wait failure, checks a flush returns
IO_ERROR, joins the worker and confirms a single idle wait and no PCM submission.
Native cancellation failure leaves the blocked worker/platform owned; a later
successful cancellation completes destroy without delivering the cancelled batch.
Existing PCM accepted-prefix, flush, clear and native-wait tests still pass.
Focused Audio smoke passes 20 consecutive runs per width. Both Release builds
pass. Background CTest: x64 121/121 in 253.74 s; x86 121/121 in 153.84 s.
Five desktop tests per width are excluded; no listening or new RDP proof claimed.
An initial test build used incorrect wait-enum names; those were corrected to
the existing FAULT/TIMED_OUT vocabulary before all final builds/tests.

The finite sweep covers Audio idle/native/producer/control waits and teardown:
native LIMIT_EXCEEDED retains its existing interruption meaning; producer and
clear/flush waits already inspect results; clear already checks native cancel;
destroy now does too. Ordinary event wake failures are not expanded into a new
recovery model. Session notification redesign remains expressly rejected.
README text agrees with actual Console cell-difference scaling and status-frame
construction; neither behavior changes. All four admitted items are covered.

Reproducible accounting: git diff --numstat 8361c1bf -- src test.
Production +19/-10 (net +9), tests +42/-1 (net +41); code total +61/-11
(net +50). Component READMEs add 10 lines; three manifests change four entries.
The other retained paths are the two task documents and dual package EXEs.
x86 SHA-256: 7446EFFD24961DE265E8C56FE0A6806A1B2BCB076CF7FDAA35F2A55C5F3FCDC4.
x64 SHA-256: 3251B1DBC8FE2C4B4979C17D84EAC18D91CCFB5973FC1F76D11E247AFDA6488E.
Owner INI/snapshot changes remain untouched and excluded. S5 awaits owner
acceptance; T85 stays open. Task-owned logs are summarized here before cleanup.

Coordinator review after pushed P1 23990ab2 compares its actual eleven paths
with the original owner request and packet. The only production change is the
existing Audio owner; Common Session, App/Core and x86 are byte-unchanged.
The two approved documentation items and both fault cases are covered. Package
hashes still match the final verified artifacts. No public contract expansion,
extra state, workaround or second recovery path is retained. Owner INI and
snapshot remain the only unrelated dirty paths. S5 is ready for owner acceptance,
not closed; T85 stays open.

## S3: owner-admitted NXVM subset synchronization

Owner request (2026-10-04): append an S to the open T and import the useful
NXVM six-component subset, excluding new x86 chips/Core/Product capabilities;
the selected shared subset must remain byte-identical.

Pinned source: NXVM 9240a3041f8298bc8b166848e3aed6db2ea542ac. Its six roots
are clean. Existing shared provenance/notices are preserved; this imports only
two test scheduling lines and their manifest hash, no new external code or
license claim. NXVM stays read-only and is never a build/runtime dependency.

Finite ledger: SoftPC's 227 existing six-root paths are the frozen universe.
The 219 exact-shared paths comprise all src/lib (109), src/common (23),
test/lib (51), test/common (20), plus ten src/x86 and six test/x86 files.
The shared test/register.cmake helper is also exact. Before import only
test/lib/CMakeLists.txt and its manifest differ within that subset.

Eight explicit local packaging exceptions remain: src/x86/{CMakeLists.txt,
README.md,verify_corpus.cmake,MANIFEST.sha256} and test/x86/{CMakeLists.txt,
README.md,verify_negative.cmake,MANIFEST.sha256}. They describe/build/check
the existing debug/xasm32-only package, not NXVM's expanded package. Their
manifests must remain valid for local contents, not claim missing chip files.
NXVM-only 120 source-package and 375 test-package files are excluded. All
existing C/H tool and test contents are exact; no parallel implementation.

Implementation: import RUN_SERIAL for library.kvm_window_modal verbatim and
its manifest. Estimate: test/build +2/-0, production +0/-0. Verify selected
byte equality, existing manifests/DAG, generated serialization property,
dual Release packages and serial background tests. Desktop interaction and
RDP mouse investigation are not acceptance claims. Await owner after delivery.

### S3 delivery evidence

Actual test/build +2/-0 (net +2), manifest +1/-1, production +0/-0,
using git diff --numstat a8e91e44 -- src test. Both Release trees successfully
configure/build softpcvm and tests. Generated CTest files on both widths and
the x64 CTest JSON listing confirm RUN_SERIAL TRUE and the retained desktop
label. Exact SHA-256 checks pass for all 219 selected paths and the helper.
All 495 NXVM-only files stay excluded; existing tool/test C/H files are exact.
No new state, interface or implementation path is added.

Serial x86 background regression passes 121/121 (144.52 seconds). x64 passes
120/121 (205.19 seconds); its only failure was a document-record ordering error: prematurely
creating an S3 history filename made the active-packet gate expect S4.
The uncommitted premature record was removed and its evidence retained here;
no governance rule or checker was weakened. The failed x64 check reruns 1/1
successfully (0.22 seconds), and x86 governance recheck also passes 1/1.
Thus every background case has a passing result; this is not a claim that
the first x64 full invocation passed. All six manifests and DAG/corpus gates
pass in both suites. Whitespace and final documentation gates pass.
Five native desktop tests per width remain excluded; no RDP/Linux runtime
qualification is claimed. No new trace/media/scratch tree was created.

The owner's pre-existing INI edit (display=window) and modified snapshot stay
unmodified and unstaged. Safety review rejected including the INI based on an
earlier owner exclusion; this delivery does not override that restriction. The earlier
owner-requested RDP escape report is now in TODO with suspected Lib ownership,
unconfirmed cause and diagnostic admission conditions, not a repair claim.

Package hashes remain unchanged after successful incremental builds:
x86 B13CD6242C69B479609F61EA1E4DCCDAD6A956BD65812DF511E72E6093956DDF;
x64 A3D6DAD0702FFDF9E11D71ADEC7D149BA70865D18B775D107B455636476D123F.

Coordinator actual-change review after pushed P1 cec8a3dc confirms five paths:
three governance records and two test-package files. The production tree and
EXEs are unchanged; the imported scheduling lines/hash match upstream exactly.
Review of the original request against the finite ledger confirms no omitted
selected-file difference, and no excluded chip/Core/board implementation was
imported. All eight package exceptions have their subset-only responsibility
above. NXVM's pinned revision and clean shared roots were rechecked. Final
documentation governance passes; only the owner's existing INI and snapshot
remain dirty. Owner accepts and closes S3 on 2026-10-04; T85 stays open.
[S3 closure](../history/M9-T85-S3-nxvm-shared-subset-sync.md) records acceptance.

## Product observation

With `O:\assets\softpc-win95\softpc.ini` and its selected `hdd.img`, a
fresh SoftPC start reaches DOS, but entering `win` does not show the Windows
1.01 startup screen or reach an interactive desktop.

## Objective

Find the first observable machine/host contract violation on this reproducible
Windows 1.01 route, repair it at its actual owner, and prove that `win` reaches
an interactive Windows 1.01 session without regressing existing background
tests or the supplied Windows 95 configuration.

## First implementation-step boundary

S1 first establishes a non-mutating reproduction and a finite execution/video
trace around `win`.  It then admits only the smallest causal repair supported
by that evidence.  If the failure needs a broader machine/device semantic
change, S1 records the evidence and stops for an owner-approved follow-on S
rather than guessing at a guest-version workaround.

## Ownership constraints

- Preserve the recovered `src/app-softpc/softpc.new` source baseline.  A direct
  mirror edit is permitted only for a narrow, source-visible mechanical/ABI
  correction under the coding rule; functional host behavior belongs in
  `app-softpc/compat` or `app-softpc/machine`.
- Do not add a Windows-1.01-specific product branch.  The repaired contract
  must be generally valid for the original controller/renderer path.
- Lib/Common/x86 remain unchanged unless evidence shows their public contract
  itself is the failed boundary; such expansion stops S1 for approval.
- The supplied `O:\assets\softpc-win95\softpc.ini`, `hdd.img`, snapshot and
  other media are read-only test inputs.  Disposable copies/overlays belong
  under the task build tree.

## Delivery convention

Every repaired build refreshes `assets/binary/softpc32.exe` and
`assets/binary/softpc64.exe`, and copies the latter to
`O:\assets\softpc-win95\softpc64.exe`.  The extra copy is a test deliverable,
not a tracked repository asset; no supplied INI or media is rewritten.

## Verification

- Reproduce from a byte-for-byte disposable copy of the supplied INI/HDD.
- Capture enough bounded evidence to identify the failing owner before editing.
- Add focused regression proof at the owning product boundary when a causal
  repair is made.
- Build Release x64/x86; run relevant focused tests and both standard serial
  background CTest presets.  Report desktop/manual verification separately.
- Verify `git diff --check`, documentation governance, and every applicable
  component/mirror gate.

## S1 diagnosis and owner-local repair

The bounded Windows 1.01 run identified the first failed contract at the
Compat DIB handoff.  The original mono painter creates a `640x400x1` DIB;
the detached Compat buffer accepted only 8-bit painters and therefore left
the KVM Window with its initial blank surface.  This is a general source
pixel-format mismatch, not a Windows-version policy.

S1 retains the original painter header and gives it source-format staging
storage.  Compat publishes its existing 8-bit indexed surface after
translating the completed dirty rectangle from the original 1-bit packed
pixels.  The original Core remains unmodified, and the established 8-bit
painters retain their direct byte-copy path.  The focused proof includes a
9-pixel mono row, verifying the first bit of a second source byte as well as
the published dirty rectangle.

The disposable normal-process probe starts the supplied configuration, sends
`start` and `win` through the real Console, and captures the KVM Window.
It reached the displayed MS-DOS Executive at a 640x400 client size. Mouse
interaction was not verified by that capture. The
probe, trace, copied media and capture remain only under `build/` and are not
repository inputs.

## Owner-approved InPort completion

The owner requests correct InPort hardware semantics, minimum complexity and
maximum reuse of the original SoftPC implementation, together with locating
an appropriate Windows 1.01 mouse driver. This extends S1 beyond its delivered
graphics repair; it does not claim that the original interaction criterion
was already met.

The existing guest driver uses the earlier 8255 Bus Mouse interface, not
InPort. Microsoft KB Q28502 documents a replacement MOUSE.DRV for Windows
1.01 distributed on Microsoft Mouse setup media. A later setup-disk driver
is a research candidate until its binary protocol and guest use are verified.
No downloaded driver is a repository fixture or redistributable package asset;
the original user disk remains unchanged and testing uses a disposable copy.

The admitted device boundary is the existing mouse.c owner, its original
quick-event scheduler, the existing snapshot callback/field mapping and
product-owned tests. Functional mirror changes for these InPort semantics
are explicitly within the owner's requested hardware repair. No Bus Mouse
emulation, guest-version detection, alternate input route or Lib/Common/x86
change is admitted. Keep original formatting and register dispatch, removing
the invented bus-probe handshake rather than expanding it.

Finite verification ledger: (1) identity/index and unsupported bus probe;
(2) signed movement, buttons and HOLD; (3) data/timer interrupt gating and
30/50/100/200 Hz scheduling; (4) reset and snapshot timer ownership;
(5) updated-driver Windows 1.01 movement/click; (6) existing product regression
and dual builds. Each member needs evidence before interaction is claimed.
Initial estimate is 5--8 production/test paths, approximately +150--250 and
-100--180 C/H lines; this is an estimate, not a target or permission to rewrite.
Disposable traces are bounded to one five-minute run and 16 MiB per trace,
owned by this task under build/t85-inport; stop owned probe processes after
each run and remove obsolete scratch after recording results.

### InPort implementation verification

The implementation replaces the finite Windows-specific IRQ burst counter with one
existing quick event at 30/50/100/200 Hz. It preserves the original indexed
registers, signed deltas, button/HOLD latching and IRQ9 route, gates data IRQs
by their enable bit, and removes the invented 8255 Bus Mouse probe echo.
No additional thread, device model or shared-component change is introduced.
The private archive registers the timer callback and stores its queue handle.
The owner explicitly approves changing the snapshot format without old-format
compatibility. The device wire layout removes the obsolete probe/burst fields
and records the timer handle; new snapshots remain identical across x86/x64.
Existing old-format snapshots must be recreated. No version discriminator or
compatibility fallback is added.

Verified research driver: Microsoft Mouse 6.24 setup disk `MOUSE.DRV`, 3358
bytes, SHA-256
`8573C81A7ED76C043C722DB5DDC445B5B836D12096303FCA7F8E1EF6DFA05B32`.
Its MREADME.DOC explicitly instructs Windows 1.01 users to replace the Setup
disk's driver and reinstall. [Microsoft KB Q28502](https://msarchive.pcjs.org/kb/Q28502/)
independently documents the original driver's missing InPort support.
The [PCjs Microsoft Mouse archive](https://www.pcjs.org/software/pcx86/dev/mouse/microsoft/6.xx/)
contains the setup disk; this is provenance, not a redistribution-license claim.
Local research binaries remain outside Git in the OS temporary directory.

The supplied HDD combines installation sources and installed outputs:
its root MSDOS.EXE is the one-byte installed placeholder, not the 41904-byte
Build-disk source. Reusing it as installation input caused Setup's generic
space error. The disposable installation used the original Build-disk source
from the [PCjs Win1.01 archive](https://www.pcjs.org/software/pcx86/sys/windows/1.01/ega/),
without changing the supplied HDD. This is not diagnosed as corrupt installed
Windows, and no guest-file-specific emulator workaround was added.

Verification ledger:

- Identity and unsupported bus probe, signed input/buttons/HOLD, reset and
  data IRQ delivery: extended `softpc-mouse-smoke` on both widths.
- Timer rates/recurrence and HOLD suppression: inspect the actual quick-event
  queue and dispatch its callbacks; no sleeps or startup burst assumption.
- Timer ownership: capture/restore the device archive while HOLD is active;
  checkpoint testing also serializes/deserializes an active timer, restores
  its handle/mode, and proves that its next callback generates IRQ9. Both
  widths pass. New-format cross-process x64→x86 and x86→x64 snapshot
  tests pass; these are not evidence of old-format compatibility.
- Real Windows 1.01 x64: install updated driver using real DOS/Setup in a
  disposable HDD, then inject input through Common Machine. Cursor moves;
  clicking A: changes directory; clicking File opens its menu. Captures are
  task-local `before.bmp` and `menu-click.bmp`.
- Real Windows 1.01 x86: same installed copy boots; movement/click reaches A:
  and the guest displays its expected no-floppy error (that run has no floppy).
  This is a click result, not a successful floppy access or menu-open claim.
- Real Windows 3.1 x64: disposable installed-media copy boots; mouse click
  opens Program Manager's Options menu. No guest driver replacement there.
- Release packages build on both widths. Final hidden-background CTest passes
  121/121 on x64 (124.02s) and x86 (122.86s). The five desktop tests per width
  are excluded; no Linux or physical mouse automation is claimed.

Final C/H scope is five files: mouse.c +70/-80, snapshot.h +5/-3,
archive.c +9/-6, mouse_smoke.c +92/-16, checkpoint_smoke.c +22/-8.
Production +84/-89 (net -5); tests +114/-24 (net +90); combined net +85.
All probe processes have exited; original media and user configuration remain
unchanged. Standard dual EXEs are rebuilt; the external
`O:\assets\softpc-win95\softpc64.exe` matches the x64 package byte-for-byte.
SHA-256 x86: `1C1D2CA700AE823FC0FDC0DD60A4B364E0E4C16CA24600312D77FBA3A23955C8`;
x64/external: `9A01E522D40E9FF199133C62990248E516D8133D812F4E6577BA09BCF865130C`.
P2 `ee62ad01` is committed and pushed. Coordinator actual-change review confirms
the ten-path delivery matches the admitted scope: five product/test C/H files,
three governance/evidence documents and two EXEs. Shared corpora and user media,
INI and snapshots are unchanged at that delivery checkpoint.

Similar-issue sweep: all `mouse_init` calls remain in the original BIOS reset
sequence after queue reset; mode writes, input injection, HOLD, device reset
and timer callback are the only controller IRQ/scheduling sites. The old
`loadsainterrupts` and invented test-state/data have no remaining production
references. DOS INT33 is a separate driver, not a second InPort implementation.
The archive callback registry is updated with one semantic ID, never a host
function address. Win3.1's real driver includes its own InPort ID path, confirmed
by read-only driver inspection and actual menu response; the removed 8255 echo
is not required to preserve that path. No guest-version predicate was added.

## Completion condition

The original repro reaches an observable interactive Windows 1.01 state using
the supplied configuration, the repair has one owner and no guest-version
special case, package/copy deliverables are refreshed, and all admitted
verification evidence is recorded for owner acceptance.

## S1 owner acceptance

After delivery the owner explicitly requested replacing the external test HDD
and EXE. The original HDD was backed up before copying the tested updated-driver
installation; both destination hashes matched their sources. INI and snapshots
were not modified. This later authorization supersedes the original read-only
media restriction only for that external test delivery, not repository media
or third-party redistribution. The updated installation is under C:\WINDOWS.

The owner reports successful testing and approves S1 closure on 2026-09-28.
[S1 closure](../history/M9-T85-S1-windows-101-startup-repair.md) records the
whole-S accounting and accepted evidence. T85 remains open without an active S;
this proposal is retained until separate task-level closure.

## S2: indexed DIB pointer regression

Owner reports the repository Win3.1 image has no visible mouse pointer. A
headless reproduction confirms consumed InPort movement but no cursor pixels.
S1 P1 split painter and published buffers for every format; the V7 compositor
still writes the published indexed buffer, which end_update then overwrites.
S1's click-response proof did not verify pointer visibility and missed this.

Keep the original single indexed surface for 8-bit painters and V7 composition.
Only packed 1-bit input needs a distinct conversion buffer. Select storage by
the existing pixel format at bind, not OS identity; skip conversion when source
is already indexed. No extra cursor cache, redraw loop, API or device change.
Expected production scope: dib_surface.c and its private header comment,
roughly +15--30/-15--30; product test additions roughly 80--130 lines.

Finite ledger: indexed writer aliasing; pointer paint/move/clear at actual
update completion; palette-only preservation; mono translation and mono/indexed
rebind; headless Win3.1 visible movement; Win1.01 retained mono; dual builds and
background regression. Each must have recorded proof before delivery. Scan
DIBData, lpBitMap and published-surface users for the same ownership defect.
No change to the InPort repair, shared corpus, original mirror or snapshot ABI.
S2 remains open until owner acceptance after complete pushed delivery.

### S2 implementation and proof

The regression is introduced by S1 P1 `fc74d65e`, not the subsequent InPort
repair. A red test invoking the actual V7 paint callback inside the real
host_start_update/host_end_update pair fails after completion: its white
pointer pixel is overwritten. The same test passes with indexed storage
restored to a single surface. No pointer redraw/cache or callback change is
needed. Conversion remains only for the packed mono format.

Counted C/H changes against `aa719ce5`: dib_surface.c +11/-11;
dib_surface.h +2/-1; vga_frame_smoke.c +71/-0;
text_console_compat_smoke.c +4/-0. Production +13/-12 (net +1), tests
+75/-0 (net +75), combined +88/-12 (net +76). The allocation count, device
state, snapshot layout and public APIs are unchanged.

Focused proof covers pointer appearance after completed update, move/old-area
restoration, hide, bottom/right clipping, palette-only update, generation
invalidation on rebind, and indexed/mono/indexed format switches. Both widths
pass the four focused DIB/VGA/mouse/checkpoint tests. Actual headless Win3.1
frames on x64 and x86 visibly contain the moved arrow; InPort mode 09h and
latched movement confirm the original guest-input path. Win1.01 x64 retains
its 640x400 mono desktop and visible arrow. These are copied-frame observations,
not a claim of new native capture/RDP testing.

Similar-issue sweep command:
`rg -l 'softpc_standalone_dib_surface|DIBData|ConsoleBufInfo.lpBitMap' src/app-softpc --glob '*.c'`.
All eight production files are accounted for: dib_surface.c selects/owns
storage; graphics_console_compat.c hands out that selected pointer; original
nt_graph.c/nt_cga.c/nt_ega.c/nt_vga.c draw through the bound native bitmap;
v7_pointer.c composites the indexed surface; machine.c only reads it for
publication. Indexed painters and compositor now target the same pixels.
Mono retains the original packed writer plus one conversion at update end.
No owner bypass or guest-version condition is introduced. Regression tests
permanently exercise the shared-writer invariant and completion, rather than
only inspecting memory before the point where the defect occurred.

Built delivery candidates: x86 SHA-256
`B13CD6242C69B479609F61EA1E4DCCDAD6A956BD65812DF511E72E6093956DDF`;
x64 SHA-256
`A3D6DAD0702FFDF9E11D71ADEC7D149BA70865D18B775D107B455636476D123F`.
Owner acceptance is pending; this is not S2 or T85 closure.

Final serial background regression passes x64 121/121 (163.92s) and x86
121/121 (135.71s), including snapshot tests and shared manifests/ownership
gates. Five native desktop cases per width remain excluded. Both Release
EXEs are rebuilt; the external x64 copy is hash-identical. Whitespace and
documentation governance pass. Four disposable S2/recheck media copies are
removed after probe exit; bounded logs/captures remain for pending acceptance.
User INI, repository media and the accepted external guest installation are
unchanged. An initial x64 link was blocked by the running test EXE; only that
identified process was stopped under standing owner authorization and the
complete build then passed. Scratch x86 probe linking used the actual generated
x86 toolchain after correcting an initial PATH/compiler-selection mismatch;
no product build configuration was changed.

P1 `8c15e893` is pushed. Coordinator review of the actual committed eight-path
diff confirms the four C/H changes and counts above, two evidence/status
documents and two EXEs. Format selection occurs once at the existing binding;
the V7 writer stays unchanged and mono conversion remains covered. The shared
six roots, original mirror, repository media and INI compare unchanged against
`aa719ce5`. This was the delivery checkpoint before owner testing.

Owner accepts and closes S2 on 2026-09-28; see the
[S2 closure](../history/M9-T85-S2-indexed-pointer-repair.md). T85 remains open.

## S4: DOS idle CPU observation and root-cause research

Original owner request: 准入一个S任务，对cpu占用的现象进行观测、记录，然后对根因进行调研。

Diagnostic extension admitted on 2026-10-04; its executable contract is the
S4 packet in CURRENT. No optimization is admitted. Expected retained changes
are documents only, with production +0/-0 and unchanged accepted packages.

Finite coverage ledger: (1) provenance and confirmed DOS prompt; (2) settled
x64/x86 process and thread CPU; (3) original idle enable and keyboard call
sites; (4) pacing; (5) HLT, pending quick events and event wake ownership;
(6) publication/presentation contribution; (7) bounded follow-up design and
measurement limits. Each entry must receive evidence or an explicit unresolved
disposition. Headless Core/Common evidence cannot stand in for a native Window
measurement. The supplied approximate 96-percent observation is owner evidence,
not an independently reproduced result.

Initial source evidence: standalone compilation does not define NTVDM;
idetect.c initializes ienabled to zero and all detection calls return while
disabled; no production idle_ctl/IDLE_ctl caller was found. The original host
contract describes blocking until interesting activity, whereas Compat only
yields. Separately, pacing spins through yield for sub-millisecond remainder.
These are candidates, not a measured division of CPU cost. No root cause is
declared experimentally confirmed yet.

### S4 observation checkpoint

The existing accepted-S2 headless probes exercise the actual VM driver,
Common executor and original DOS machine, without creating a native presenter.
S3 changed no production code. Probe SHA-256: x64
`A9507A5B82E66B600D770D87AE24BB3F098CBC955679F70637065EFE34D0C6AD`;
x86 `22CE5CDAB1C2456A0DDDE99BC0BA51E7132ECC01B8A8D0BC24215BA41E9227FF`.
The probes were built on 2026-09-28; they are not the package EXEs, and no
fresh build or native Window benchmark is claimed. Source medium SHA-256 is
`2DE4B03A68B1E1AC23D482C3A3402726A853A8520B7ACC370DB694053DFFA3A4`.
Each run used its own disposable media copy; originals were never passed to
the probe's direct writer. Both runs printed RUNNING state and C:\\> before
sampling, settled ten seconds, then sampled three two-second intervals.

| Headless machine | Interval CPU, one logical core = 100 percent | Mean | Private memory |
| --- | --- | --- | --- |
| x64 | 95.53, 100.10, 96.19 | 97.27 | 24.23 MiB |
| x86 | 93.37, 100.09, 96.87 | 96.78 | 24.47 MiB |

The slightly over-100 samples reflect OS CPU accounting and wall-clock
measurement granularity, not more than one executor. x64's hottest thread
contributes 94.75/99.32/96.19 percent; x86's contributes
93.37/98.53/96.09 percent. Other threads are approximately zero, with isolated
0.78-percent increments. Both processes exit normally (code zero); their
disposable images are removed. This reproduces high idle CPU without native
Window or Console painting; presenters are not a necessary cause. It does not
measure their additional cost in a complete product.

A separate x64 run, after the measurement runs, was attached read-only by GDB
for two stack checkpoints. One executor stack is ccpu.constprop ->
c_cpu_simulate -> softpc_machine_run -> vm_driver_run -> common_machine_worker
-> base_sync_platform_main. Another has d_mem above that same chain. Main is
sleeping in the probe and native pool workers are waiting. Runtime reads show
ienabled = 0, pacing_enabled = 1 and pacing_instructions = 24032271. No variable
was changed and no guest function was invoked. Debugger pauses are excluded
from the CPU sample intervals. An earlier x86 attach lacked useful unwind
frames and is not used as function-level attribution.

### S4 cause ledger and follow-up boundary

1. Provenance/prompt: measured using the hashes and real text frames above.
2. Process/thread CPU: measured on both widths; reproduced at approximately
   one core even without a presenter. Native package measurement remains
   separate owner evidence, not an independently replicated benchmark.
3. Idle: runtime-disabled detection is confirmed, not only inferred. Search
   `rg -n 'IDLE_ctl|idle_ctl' src test` finds definitions/declarations only.
   Original keyboard wait/status loops call IDLE_poll/IDLE_waitio and timer.c
   calls IDLE_tick, but idetect returns while disabled. Enabling alone still
   leaves Compat host_release_timeslice as yield rather than original blocking
   semantics. This is an incomplete host idle integration, not a Lib defect.
4. Pacing: runtime enabled; platform.c checks every 1024 instructions against
   a one-million-instruction-per-second target. Its sub-millisecond remainder
   repeatedly yields and queries the clock. This is a separate CPU candidate;
   the two stack checkpoints do not quantify its time share. No percentage
   attribution or causal A/B optimization result is claimed.
5. HLT/events: c_main.c drains pending clock/input work and quick events before
   waiting; it waits only with zero interrupt map and zero quick-event count.
   Compat has one auto-reset event, 50-ms host-clock signaling and VM stop/input
   signaling. BIOS input may use nested host_simulate, so idle waiting must
   preserve original continuation and dispatch wake work rather than treating
   it as a snapshot-safe boundary. Full lock/quick-event timing qualification
   belongs to a separately admitted repair, not this observation result.
6. Frames: Common compares copied text before publication and calls the sink
   after releasing frame_lock. No presenter exists in this measured fixture;
   no evidence supports changing rendering cadence to solve the reproduced
   executor consumption.
7. Follow-up: minimally enable/reset original idle detection in the product
   initialization owner and implement its existing host callback's wait using
   existing wake resources in Compat. Do not add a second executor/clock,
   change Lib/Common, or sleep every guest instruction. Qualify keyboard,
   mouse, pause/stop/reset, quick events, timer/audio/serial and snapshots;
   measure latency and idle CPU before claiming 1--2 percent. Address pacing
   separately only if a function-level profile shows remaining significance.

All ledger entries have measured/source evidence or explicit qualification
limits. No repair is implemented, no media/INI/package changes are made, and
tracked production/test code is +0/-0, net zero. The observation report is
ready for owner review; S4/T85 are not declared closed.

### S4 repair admission and implementation

Owner adds: 请你在当前S内修复，然后自行测试看看是否解决。
This explicitly supersedes the diagnostic-only restriction on 2026-10-04;
the active packet records the repair boundary. The source change stays in
Compat platform.c: the existing continuous-run heartbeat enables/initializes
original idle detection and disables it on run exit. Finite-budget CPU calls
remain nonblocking. Original idle classification, guest interrupts, host-clock
period, frame publication, original mirror and shared six corpora are unchanged.

host_release_timeslice checks pending input/control/clock work and otherwise
waits on the existing auto-reset executor event through the HLT wait/failure
owner. A signal between checking and blocking is retained. Actual idle wait
duration is excluded from the existing governor's wall-clock origin so idle
cannot accumulate catch-up execution credit; its rate/check interval/algorithm
are unchanged. No new thread, event, idle heuristic or recovery state exists.

Permanent tests extend platform_failure_smoke for pending input/clock bypass,
finite-budget bypass, native idle wait, the check-to-wait signal race and pacing
origin adjustment. HLT continues to cover the same native wait-failure unwind.
The real DOS restart/Win3.1 roundtrip test also bounds prompt CPU to less than
25 percent of one core over two seconds after heuristic settling; this generous
regression ceiling is not the measured product performance target.

The first scratch relink omitted the embedded firmware resource and did not
remain at a valid RUNNING DOS prompt. Its samples and shutdown timeout are
discarded, not attributed to the repair. After linking the exact existing VM
objects and firmware resource, both probes confirm RUNNING DOS and sample
0.00/0.00/0.00 percent per width (OS accounting resolution), versus prior means
97.27/96.78 percent. This means below sampling resolution, not literally zero
work or a native Window benchmark. x64 stack reads now show ienabled=1,
i_counter=12 and keyboard_io -> idetect -> host_release_timeslice -> native
WaitForSingleObject. Original 50-ms host events continue to wake that path.
Further final-build evidence is recorded before complete delivery.

### S4 final delivery evidence and actual-change review

Final probes use the final production code, the exact existing seven VM
objects and embedded firmware. x64 SHA-256 is
0EE4B624C8B1F030D34EBCE64BF359D86720F4F08DC222DD90AABFE34DCAA068;
x86 is A7208CC153E452FFD9CC2B4F137E79BB012251733607EF02BFAAE31E3D1DE49C.
After verified RUNNING DOS prompt and ten seconds settling, three two-second
samples are x64 0.00/0.78/0.00 percent (mean 0.26), x86 0.00/0.00/0.00
(below accounting resolution). Private memory is 24.22/24.39 MiB respectively.
Both probes exit zero; disposable media and owned processes are cleaned up.
These are headless product-core measurements, not a full native Window CPU
benchmark or a new desktop/RDP input-latency qualification.

Both final Release builds pass. Final x64 background CTest passes 121/121
in 105.22 seconds; final serial x86 passes 121/121 in 145.74 seconds.
An earlier final-code parallel x86 run failed the existing Win3.1 windowed
DOS-prompt roundtrip (120/121); independent focused repeats then pass three
times, followed by the full serial pass. No timeout or assertion was weakened.
The intermittent failure's cause is unproven; it is retained rather than
declared fixed by repetition. Earlier direct-Sleep test code failed the Types
boundary gate and was corrected to the existing Base test sleep before final
verification. Five native desktop tests per width remain excluded.

The finite similar-issue sweep confirms that original idle counters and call
sites remain unchanged; run entry/exit controls their existing enable switch;
finite-budget calls cannot block; pending input/control/clock work bypasses
waiting; the existing auto-reset event retains check-to-wait signals; native
wait failure retains the existing executor termination owner. HLT, quick-event
dispatch, timer cadence, frame publication and pacing rate/check interval are
unchanged. Only actual idle duration shifts the pacing origin. Existing
background input, lifecycle and snapshot regressions pass; no claim is made
that every device workload or native desktop interleaving was tested.

Actual source/test diff: platform.c +20/-7 (net +13); platform failure tests
+34/-0; runtime restart tests +22/-0. Total code +76/-7 (net +69). There is no
new API, state object, thread, event, mirror diff or shared-corpus change.
Two package EXEs and the two task documents are the other retained paths.
Final EXE SHA-256: x86
CDAD3C003A9136962FFB60127BD94BAD7F213E99EAA91844AA0C676CDB76CF6F;
x64 2586446090C690A956FD701AE13F02988C50E75B3CF75ED08C82ECFB0D0AF02F.
Original owner request, admitted scope and actual diff were reviewed together:
the high idle CPU is reproduced and substantially reduced without fixed-loop
sleep, Lib/Common changes or an original-mirror behavior branch. Owner-owned
INI and snapshot modifications are preserved but excluded from this delivery.
Implementation is ready for owner testing; neither S4 nor T85 is closed.

### S4 owner acceptance

Owner states: 非常好，收口S4，验收通过，保持T开放。然后队列任务列一下。
S4 closes on 2026-10-04 after review of pushed observation P1 1bc02185 and
repair P2 a841b5d3 against the admitted request and evidence. The
[closure record](../history/M9-T85-S4-dos-idle-cpu-repair.md) preserves
verification limits and changed-path accounting. T85 remains open without an
active S. Queue order and deferred TODO items are unchanged; no new task is
admitted by listing them. Closure changes documents only, not the tested EXEs.
