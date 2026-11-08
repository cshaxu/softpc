# Public Handoff Simplification Review

## Admission and scope

### S16: NXVM public eight-tree import audit

Owner request: 好，收口S15准入一个S16，审计一下NXVM当前我们可导入的公共八组件的代码改动和代码质量情况，给我一个汇报。
Read-only inventory covers all eight corresponding src/test trees, with each
path classified as equal, changed, only SoftPC or only NXVM. SoftPC baseline is
2faad954; NXVM HEAD observed 06ce2710d3a67c4cacdcb3f6e0402b14459c021f.
The observed NXVM worktree has four modified public paths: Window component,
Window fixture and their two manifests. Recheck before any later import.

There are 296 files on each side and identical paths. Eight differ: src/lib
component.c and manifest; test/lib CMakeLists.txt, types_layout_selftest.cmake,
Window capture fixture and manifest; test/emulator CMakeLists.txt and manifest.
The other five trees, plus src/emulator, match byte-for-byte. The shared root
test/register.cmake also matches. Component and Window fixture hashes match the
modified NXVM manifests. No external build or test was run for this audit.

NXVM's active S85 is still qualifying an uncommitted Window change: prevent
the system minimum size from becoming persistent scale, retain snap-region
ratio during frame changes, and record user sizing at EXITSIZEMOVE. Its CURRENT
reports the latest draft passes focused dual-width tests; prior full suites
predate it. Existing fixture checks snap geometry immediately after restore but
does not assert the next frame's scale after a restore delivered only by
WM_SIZE. Since WM_SIZE no longer records scale, that scenario can retain an
older scale; whether actual Windows emits WM_SIZING/EXITSIZEMOVE for the user's
snap restoration requires native message-path proof. The real desktop fixture
also injects sizing without an enter/exit sequence and does not test this
restore-then-frame behavior. This is a concrete coverage gap and conditional
regression risk, not a claimed real-desktop reproduction. Obtain evidence
before declaring the current draft safe to import.

Committed NXVM test changes extend exact fixture budgets (two Lib selftests
30 -> 300 seconds; Emulator verifier 60 -> 300) and replace a long list of
fixture-file removals with recursive cleanup of its owned types-layout-fixture.
Normal registration supplies TEST_WORK_DIR from the package binary directory;
verify fixture ownership/path safety and isolated negative/positive behavior
for downstream contexts. Timeouts are test budgets, not production waits, but
the 300-second value should be judged against measured failures rather than
assumed necessary. No added platform dependency or public API change appears
in the bounded diff. Recommendation: wait for upstream final qualification
and check snap restore/frame sequence and cleanup safety before exact import;
do not silently merge only a subset. User decides import as a separate step.

### S15 admission: V7 pointer/body handoff

Owner requests: 好，请你准入一个新的S完成此项修复，并补上最小必要测试覆盖此情况，老规矩完成后编译发布测试提交推送等我验收.
The retained V7 compositor saves a background and restores it when redrawing;
an original full body repaint can therefore be overwritten by the old saved
background. An existing App VGA fixture now exercises actual callbacks and
pixel values for a changed body followed by pointer disable. Baseline x64
fails the new `pixels[0] == 0x42` assertion. This confirms the sequence, not a
reproduction of every observed guest trail. No production repair is yet made.
Investigate nested transactions and packed mono conversion before choosing the
single transaction implementation. Both widths and the full requested tests
remain outstanding; do not treat the intentional baseline failure as delivery.

**Implementation and qualification.** The existing original host transaction
bridge now calls Compat host-update entry points. The outer host transaction
removes the V7 overlay before body painting; at its end it finishes any packed
monochrome conversion, restores the original controller-selected pointer and
commits the existing accumulated damage. Nested updates retain the existing
depth. The ordinary standalone damage transaction remains its existing path.
No new frame owner, thread, timeout, polling or public component is added.
The retained callbacks still own the 32x32 saved background and pattern logic.

Against 844c7e17, production is +33/-3 (Compat C +29/-1, internal header +2,
mirror host bridge +2/-2); test C +43/-1. Only the two existing mirror bridge
calls change, with current local body hash a568b07b2859a96a2c001ef12960b13156903b9da88af5eaf9ee96be1c4d8d4c;
XP raw/subset identities and source selection are unchanged. The existing VGA
fixture verifies changed background after clearing and packed monochrome
conversion, including an otherwise empty host update. The first 8-bit baseline
failed the expected new-background assertion; an intermediate implementation
passed that case but failed monochrome and then empty-monochrome assertions.
The final focused x64 fixture passes. Existing move, clipping, palette, nested
update and resize tests remain; these do not prove every real guest trail.

Cached final x64/x86 builds refresh both EXEs. Each passes 193/193 registered
tests: 185 background, five native desktop, three native audio; JUnit names
match the registered inventory with no failures or skips. The earlier x64
desktop package attempt failed stage 5 while waiting for the DOS prompt and
the child remained alive; its compact and three Lib desktop tests passed. Its
cause was not established, and this final passing run is not claimed as a repair
of that intermittent result. Initial expected fixture failures and this failed
package run are not concealed. Warnings in preserved mirror headers remain;
no non-mirror warnings were observed. INI/media/snapshot are unchanged.

Published SHA-256: softpc32 C411721341B38CCCF184C27A12201AFA97BEF931FD7E15D96D8772303985B13C;
softpc64 FEADEBB940AC53A139012B8F488AE7BB16EAD620A8C7672C7BAE19ED7F3CD81D.
The private pixel tests prove the addressed source sequence and final suite
proves the registered scenarios; neither establishes that all guest cursor
trails have been independently reproduced or eliminated. Await owner testing.

**Coordinator actual-commit review.** Reviewed executor 81a7a096 against the
owner request, the existing host transaction, all original V7 pointer call
sites, the 8-bit and packed-mono producer paths, fixture assertions, mirror
manifest and the final registered test inventory. The existing update-depth
boundary owns ordering; no extra frame owner, thread, polling or public-package
change was introduced. Published EXE Git blobs match the committed assets.
The first desktop package failure remains disclosed; no root cause is claimed.
The dynamic tests prove the covered pixel sequences, not every guest behavior.
Documentation, whitespace and residue gates pass; scratch is retired. No
delivery blocker found. S15 remains open for owner acceptance; T99 stays open.

**Owner-approved palette follow-up in progress.** The owner manually accepted
the earlier cursor-trail repair, then requested a minimal unified repair for
the audited packed-monochrome palette-update gap. An assertion in the existing
VGA fixture fails on the prior source: after a palette update, the visible
pointer pixel was overwritten. A first candidate wrapping all invalidation in
the host transaction instead failed the existing indexed pointer/palette check
and was discarded. The current candidate tags body damage in the existing
pending-damage bit field and converts packed pixels only when body data changed.
Palette and invalidation retain the same transaction and commit; no pointer
owner or renderer is added. Focused x64 and both complete background groups
pass; full dual-width qualification is not yet complete. An initial x64 desktop
run while the Windows session reported disconnected failed both package cases
at stage 3 with child exit 1 and `cannot compose machine`; this correlation
does not prove the cause. Three Lib desktop cases passed. Await connected-session
rerun; do not describe the failed attempt as a pass.

**Follow-up qualification.** Relative to dd10399b, production is +4/-1 in
the existing Compat DIB owner, test C +4/-0. `body` damage sets one bit in the
existing pending-damage value; the existing packed conversion skips copying
when only presentation damage is pending. Both palette and explicit full
invalidation now preserve the visible pointer in the existing VGA fixture.
The 8-bit callback/palette assertion remains intact; no extra callback,
renderer, persistent frame or device policy is added. No public eight-tree,
mirror, INI, media or snapshot change is made. No automatic recovery or test
override is introduced.

Both cached dual builds refreshed the EXEs. Each architecture's 185 background
registrations passed; after session reconnection, each architecture's five
desktop and three native-audio tests passed. Registered/executed names match
exactly (193 each), with zero failures/skips in those combined groups. The
previous disconnected-session package failures remain a separate failed run,
not proof of their cause or of an independent product repair. No non-mirror
warnings were observed. Published SHA-256: softpc32
CE9FB9E203E5C5FCF4AD6DCE98708418119804B47496B0ABFC157DBE870BFC3B;
softpc64 8B0816A7D97C950E328A001B0EECB561D29D32D59D5F9C5874E057A0B197B1C5.
INI SHA-256 remains 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D.

**Follow-up actual-commit review.** Executor 099e2104 is reviewed against
the owner-approved minimal unified scope. The body flag is carried only in
the already-existing pending-damage value and is reset with that value;
all callers continue through the same commit and existing body conversion.
The fixture retains its indexed assertions and adds monochrome palette and
full-invalidation pixel checks. Both committed EXE blobs match the published
files. No public component or mirror path changes, extra rendering path,
pointer owner or cleanup mechanism is introduced. Disconnected package
failures and the scope of the subsequent passing connected groups remain
explicitly recorded. Documentation, whitespace and residue checks pass.
S15 awaits owner acceptance; T99 remains open.

### S14 admission: original cursor-shape semantics

Owner requests: 1. 准入新S：原始 VGA 光标结束扫描线及回绕语义审计，可能涉及光标粗细;
2. 把这个从TODO里面清理掉; 3. Td S22 这个直接收口.
Move the sole remaining TODO here by admission, not by declaring a repair.
Separately record accepted Td S22 closure; no runtime/source change follows
from that governance disposition. S14 starts research/design at 889b309a;
the performance Queue and T99 closure are outside scope.

Finite research ledger: (1) authoritative VGA/V7 versus EGA row numbering,
end inclusion, wrap and out-of-cell behavior; (2) original register/shape
writers, adapter selection and host/Driver/frame/render consumers; (3) existing
oracle/history and possible perceived-thickness regression; (4) fresh private
x86/x64 actual-port/producer observations at font heights 8/14/16/32, including
zero/equal/reversed/overscan/disabled shapes; (5) minimum original-path diff,
mirror accounting and permanent test plan. Each needs proof or an explicit
limit. Private counterfactuals are not shipping fixes. No second cursor owner,
fallback, percentage reconstruction or public two-span capability removal.

The transferred lead is retained in [S10 research](#s10-research-two-site-cursor-disable-repair):
the original VGA path uses End-Start and splits End<Start, and the current
fixture expects those values. Recheck against the selected adapter's primary
manual before changing either implementation or oracle. This is an audit
lead, not proof of the reported cursor-thickness symptom.

Initial tracked production/test delta +0/-0. Preserve the S10 disable fix,
genuine EGA semantics and existing Lib frame schema. A functional mirror
repair requires the researched diff and owner decision first. Task-owned
scratch is bounded in CURRENT and retired after evidence promotion.

#### S14 research: original VGA cursor shape

Owner starts the admitted audit: 开始执行s14 规矩你懂. Baseline is
0ab81c52. This is research/design, not approval to ship a cursor-policy change.
The five-member ledger above converges to the following evidence and limits.

**1. Hardware contract.** Video Seven V7 VGA Technical Reference, 30 June
1988, pages 3-41/3-42, numbers rows from zero, includes the ending row in VGA,
hides reversed ranges in VGA, and distinguishes EGA's exclusive end and wrap.
Equal start/end produces one line. It also specifies a full cell for an end
greater than cell height; that V7 rule must not be replaced by generic clipping.
IBM VGA/XGA Technical Reference, May 1992, VGA Function 2-66 independently
confirms that a reversed range hides. These are controller rules, not Window
scale policy. Primary manuals consulted through their indexed text:

- [V7 VGA manual](https://dosdays.co.uk/media/video_seven/v7_vga_techref.pdf), CRA/CRB.
- [IBM VGA/XGA manual](https://bitsavers.org/pdf/ibm/pc/cards/IBM_VGA_XGA_Technical_Reference_Manual_May92.pdf), CRTC 0B.
- [IBM EGA manual](https://www.minuszerodegrees.net/oa/OA%20-%20IBM%20Enhanced%20Graphics%20Adapter.pdf), page 33, distinct original adapter.

The exact physical boundary End==cell-height is not independently hardware-
measured. The minimum keeps the original greater-than-height full-block rule;
the existing Lib clips a span at the cell boundary. The proposal does not
assert every overscan combination on every VGA-compatible chipset is identical.

**2. One state and presentation path.** A search for original video
set_cursor_start/height/visible (including the second span) finds three owners:
cga.c, ega_prts.c and vga_prts.c. gvi_init selects the adapter; host_def.h
defines EGG/VGG, and actual probes report VGA (5). vga_init installs its CRTC
handlers. Thus ega_prts.c's conditional VGA-labelled branch is not the active
shape handler for this fixed model, even though it is compiled. No EGA/CGA
algorithm is to be changed using VGA rules. V7's separate graphics pointer is
not this CRTC text cursor and is outside the repair.

vga_prts.c do_new_cursor has five normal caller sites: font height (09),
shape (0A/0B) and location (0E/0F). S10's disable authority and original V7
write-protection remain. The existing base_cursor_shape_changed tail marks
original video dirty. Original BIOS vd_set_cursor_mode writes the same ports;
it does not own a second shape formula. Existing snapshot replay also writes
the CRTC through vga_crtc_outb, so a repair at this owner applies on replay
without a snapshot-layout change. Fresh snapshot qualification is still required
after implementation, not claimed by this source trace.

Compat presentation_text_style copies calculated original visibility and two
spans; Driver copy_text uses it for both leaves. Machine/Session transport
does not recompute shape. Lib Base clips/merges explicit spans; Window geometry
scales their rectangles. Console deliberately approximates a native cursor,
not arbitrary scanline origin or a split gap. No shared interface, renderer,
percentage fallback, new state, thread, polling or alternate path is needed.

**3. Why it can look thinner.** The shape defect is present in retained
0e91ada1 and XP migration 41aa0bc2: ordinary ranges use End-Start, End==0 is
special-cased to the bottom of the cell, and reversed ranges split. It is not
introduced by T99 S4's Console reader-retirement change (52d8fcc3).
T98 S6 8e66d797 replaced the old NT-percentage reconstruction with direct
original scanline transmission, correctly preserving original positioning.
Before that change, nt_graph.c make_cursor_change called GetCurrentConsoleFont;
Compat returned FALSE, causing the original 20% approximation. The old Driver
rounded that percentage up: a 16-line cell became four cursor lines. The
current exact route exposes one line where the controller should calculate
two. Restoring the four-line approximation would conceal, not repair, that
original defect. This explains the available source/history example; no capture
of the owner's earlier DOS register state exists, so the entire subjective
report is not claimed as a reproduced historical guest session.

**4. Actual-port and producer proof.** Hidden owned jobs reuse the existing
VGA fixture, mock PCM/MIDI, cached compile/link commands and private objects.
They create disposable test media, use original BIOS mode 3, write/read actual
CRTC ports, advance original timer work and call real Driver copy_frame.
Heights 8/14/16/32 use complete character rows (400/392/400/384 scanlines),
not a deliberately partial last row. Each width executes all 32x32 start/end
pairs, with disable clear/set: 8192 observations, all producing frames in the
final sweep. Window and Console modes/spans agree, and 1:1 Window cursor
rectangles reproduce the frame's resolved row mask.

The independent oracle covers in-cell ends, explicit disable and outside-cell
starts: 7492 cases per width. Baseline failures by font height are 57/183/241/993,
total 1474; the private candidate has zero. Concrete H=16 observations:
14..15 is one line before, two after; 4..7 is three before, four after;
12..2 is two visible spans before, hidden after; 0..0 is full cell before,
one line after; 6..6 is one line in both. The initial BIOS-mode shape is
13..14, one line before and two after. These are frame/geometry assertions,
not real desktop pixel captures or a DOS application recording.

The other 700 overscan cases per width are observed rather than included in
that hardware-correctness count. Their resolved output/visibility aggregate
is identical before/after and across architectures (FNV64 1c311dc4e39109e8,
ordered height/start/end/mask/mode scalar values). Samples End==H remain
cell-clipped; End>H retains the full block. Existing disable/protection/font/
mode checks also pass on both private variants. The unchanged registered
app-softpc/unit/machine/vga_frame_smoke passes baseline on both widths;
its explicit exclusive-end/split oracle is the coverage defect, not new proof
that those shapes are correct.

Research setup failures are retained here, not treated as product failures:
an initial probe wrongly required production after every synthetic geometry;
complete-row CRTC geometry removes those suppressed frames. Reusing the
existing disable helper after ending at a 32-line font violates its 16-line
initial precondition; running it before the matrix restores that precondition.
The first candidate left End-1 underflow after deleting the zero special case;
four zero-range checks caught it, and the final direct End>height comparison
removes the underflow while preserving the prior nonzero threshold. Final
research/candidate exit markers are zero. Private candidate compilation has
existing mirror-header endif/macro warnings, not a warning-free full build.

**5. Minimum repair and accounting.** Only vga_prts.c do_new_cursor needs a
production change: extend its existing hidden condition with End<Start;
delete the zero-end, split and redundant equal-end branches; compare End>height
directly; use End-Start+1 in the ordinary branch and align its debug assertion.
Keep the existing notifications, off-screen row hide and dirty tail unchanged.
The essential algorithm (not a second implementation to retain) is:

```c
if (cursor_off || End < Start || Start >= H)
    /* existing hidden path */;
else if (End > H)
    /* existing V7 full-block path */;
else
    /* existing normal path, height = End - Start + 1 */;
```

The exact reviewed private source diff is +6/-33, net -27, with local reason
markers and no unrelated formatting/condition reordering. Production repair
would add a necessary mirror patch, not reduce mirror difference. Reconstructing
the fixed XP S5 necessary subset verifies raw hash 57a3318256a0b9392329fd4afada8b9926b32345c4262f08c3f3324b65d291f2
and subset hash 084da18a959a825d66ef900d60b212a9bcffffd3cea23da0607cbf561946b120.
Git default line diff is +226/-12 now versus +232/-45 for the candidate:
238->277 changed lines, +39. No source ranges are excluded to hide this patch.
One rejected accounting attempt used pre-S5 baseline ranges; only the XP
selection ranges reproducing the registered hash supply these accepted counts.

Proposed permanent tests replace the existing four shape samples in the
App-local VGA fixture with a compact actual-port table/matrix, not another
test executable or test-only production API. Preserve underline and S10 tests;
check metadata/produced and both text leaves, normal/zero/equal/reversed/outside
ends, disable, font change and snapshot replay. Estimated test diff +45..65/
-20..30 (net +15..45); final implementation accounting must be measured.
Mirror manifest and existing repair evidence need only their local hash/reason
updated. Public eight trees, App/Compat interfaces and guest data stay unchanged.
After owner approval, use cached dual builds, publish both EXEs, focused VGA/
snapshot/geometry checks and full prescribed regression including native tests.

Actual tracked production/test delta for this research is zero; only task
records change. No new EXE, full-suite, native desktop, audio or real guest
qualification is claimed. S14 waits for the owner's repair decision, not
acceptance of an already shipped fix. T99 remains open.
Final registered-test wall times are x64 3.76s and x86 7.35s (one test each),
not complete-suite timings. The research scratch held about 12 MiB at retirement and
was retired after both owned jobs exited and results were promoted here.
Documentation, the unchanged 505-file mirror, whitespace and build-residue
checks pass; production/test/assets Git diff is empty. EXE and INI SHA-256
remain identical to the accepted S11 hashes in CURRENT.

Coordinator actual-commit review of executor f074452f confirms all five
research members have a proof or explicit limit, only two documentation paths
changed, and the original request has not been expanded into a shipping repair.
Source/test/assets and all public trees remain unchanged. The complete minimum
retains one VGA shape owner and original dirty/protection/snapshot routes;
permanent-oracle correction and post-repair full qualification remain required.
No research-delivery blocker is found; S14 awaits the owner's repair decision.

#### S14 approved implementation

Owner approves at reviewed research ff97104b: 批准修复 按老规矩.
Apply the exact +6/-33 original-path minimum above, with no public-eight-tree
or EGA/CGA change. Replace the old four-sample oracle in the existing App VGA
fixture with explicit inclusive/zero/equal/reversed/outside/overscan shapes at
8/14/16/32 rows, each enabled/disabled. Reuse existing port/frame assertions,
add original snapshot capture/replay for zero, reversed and ordinary ranges,
and retain underline, disable/protection and existing graphics/snapshot tests.
No new test target, helper framework or production test API is admitted.
Then cached dual builds/publish and all registered background, desktop and
native-audio tests, exact inventory matching, changed-path/mirror accounting,
warning audit and actual-commit review. Preserve INI/media/snapshot-midi;
task scratch and budgets stay in CURRENT. Wait for owner acceptance afterward.

**Implemented minimum and permanent proof.** Against ff97104b, production is
exactly +6/-33 (net -27), in do_new_cursor only. Three old special branches
disappear; the existing hidden/full-block/ordinary paths, notifications and
dirty tail remain. No renderer compensation, added owner/state/API or fallback.
The source Git blob aa04e3e68e18bcf94b71c173f59b153e3b78cda0 matches the reviewed
private candidate. Current normalized source SHA-256 is
961eccd23cf76b531efc9dc92a3bc9dc38db4773fce366bf18e6b9333edfde6f.
EGA/CGA, Compat, Machine/Session and all eight public trees have zero diff.

The existing VGA fixture is +49/-19 (net +30), with no new target/helper.
Ten explicit expected shapes at four font heights and both disable values
run in each of its two existing mode-3 passes: 160 shape observations and
48 selected controller-archive replays. They check actual register readback,
original visibility, real Driver-produced metadata and both text leaves,
including no VGA second span. Complete-row geometry is restored afterward.
Zero/equal/inclusive end, reversed/zero-ended hide, last row/outside start and
retained overscan policy have direct cases. Existing underline, disable/V7
protection, colour/graphics and snapshot tests remain. Focused VGA passes on
x64/x86 in 5.96/7.77 seconds. This is actual executable proof, not screenshot
or manual guest-application qualification.

The freshly reconstructed pinned XP subset again matches both registered
raw/subset hashes. Default Git mirror diff is +232/-45 (277 changed lines),
versus +226/-12 (238) before: +39 historical changed lines, despite net source
reduction. Only the local body hash changes; no extraction range, source
baseline or external dependency is changed. Mirror worktree CRLF and fixture
LF follow existing .gitattributes, with no whole-file normalization.

**Build, publish and complete inventory.** Both cached Ninja/ccache Release
builds succeed and publish their EXEs directly in assets/binary. Each runs
34 edges, compiling only vga_prts.c and the changed VGA fixture, then archiving/
linking existing objects; CCPU and unchanged public components are not rebuilt.
Configuration has no warning; build warnings are 178 existing mirror-header
lines combined, zero outside the preserved mirror. No unrelated warning repair
is claimed. The first task-local shared-test selector used slash instead of
the registered dot prefix and selected zero cases; --no-tests=error correctly
stopped it. The selector was corrected from actual CTest JSON before proceeding;
this is not a passed test or a hidden case retry.

Every registered case subsequently passes once in the disjoint full groups:
x64/x86 each 118 shared background +67 other background +5 desktop +3 native
audio =193/193. JUnit test names exactly equal each architecture's JSON
registration, with zero failures/skips/non-run cases or duplicate execution.
The separate focused VGA run is not included in those totals. Group wall times
are x64 63.88/86.31/22.75/2.93 seconds and x86 66.20/63.86/23.37/2.63 seconds;
individual JUnit records preserve all group results during
qualification. Both package modes, Console/Window desktop, native MIDI/PCM,
snapshot transaction/cross-process, boot/restart and every source/dependency/
manifest gate pass. Native tests run in the active desktop session without
Computer Use. No exhaustive manual DOS/Windows application acceptance is claimed.

Published SHA-256:

- softpc32.exe: 74641B4AF7E7B07E0B33D958038E5882F7001DD8467FAB63CB10E2222F56E570.
- softpc64.exe: B5A1FBAA7A85CF814DE57F669827A3D4126375108871A47A726EA4C46F25A52E.
- User INI remains 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D.

**Executor quality review.** Repeated rg searches for original video
set_cursor_start/height/visible and cursor-end/start arithmetic still find
only cga.c, ega_prts.c and the repaired vga_prts.c. Distinct CGA/EGA algorithms
are retained, including EGA's compiled but unselected VGA-labelled branch;
they are not blanket-certified for other adapters. BIOS, five recomputation
callers and snapshot replay all reach the one selected VGA owner. Lib remains
a neutral explicit-span consumer. Necessary errors, disable/protection and
existing cleanup remain; no polling, allocation, thread, recovery mechanism,
compatibility alias or test-only production entry is added. Code totals are
+55/-52, net +3, excluding records/manifests/artifacts. All runtime changes are
inside the approved single-file original-hardware repair; tests stay App-local.
The End==cell-height physical-hardware limit stated in research remains a limit,
not an invented new chipset guarantee. Wait for owner acceptance; do not close
S14 or T99 solely on these test results.
Final documentation and 505-body mirror checks pass, whitespace is clean, and
both persistent build trees report no work on a second actual build. EXE/INI
hashes remain as listed. All owned jobs have exited; promoted results above
replace the disposable scripts/logs/subset copy, and exact build/t99-s14 was
removed. Clean-Build.ps1 -Check passes with only the two reusable cache roots.

**Coordinator actual-commit review.** Reviewed pushed executor 77326df5,
including all eight actual changed paths, against the original request and
approved minimum. Source/test counts, single-owner control flow, the sole local
manifest hash, immutable XP selection and disclosed mirror growth agree.
Published EXE blobs equal their committed blobs; no public-tree/config/media/
snapshot or build-interface change is present. Exact full inventories and
focused evidence support the stated scope, not universal hardware/guest proof.
No delivery blocker or new second path/ownership/cleanup defect is found.
Documentation/whitespace/residue checks pass. S14 remains verified awaiting
owner acceptance, not closed; T99 and the performance candidate are unchanged.

**Owner acceptance and closure.** Owner now approves: 批准收口S14.然后列出所有的S任务、T任务、TODO.
S14 closes after delivered implementation 77326df5 and actual-commit review
755d0445; the [closure record](../history/M9-T99-S14-vga-cursor-shape.md)
maps acceptance to the existing full qualification. This governance-only
closure does not rebuild or rerun tests. S1 through S14 are closed; T99 stays
open for its separate completion audit. No next S/T is admitted, TODO is empty,
and the one unnumbered runtime-performance Queue candidate is unchanged.

### S13 admission: exact eight-tree test-ownership import

Owner first requests a read-only audit of latest NXVM including uncommitted
changes, then authorizes: 好，准入新S导入nxvm最新公共八组件，老规矩，可直接完成S13收口后等我。
This makes S13 the import, not the suggested but unrecorded T-completion audit.
S12 is closed at 06430c21; T99 stays open. Delegated S13 closure requires full
qualification, pushed executor delivery and actual-commit coordinator review.

**Provenance and frozen universe.** Source is the existing admitted shared
NXVM corpus at HEAD 9436f08fe6ece2dd9a3afc6729e3ee648b0e71fc, plus its two
audited uncommitted test files explicitly included in the owner's request.
Do not misrepresent this as an upstream committed revision. Freeze the exact
payload by hashes before import; the SoftPC executor commit will provide the
committed downstream baseline. Existing shared-source notices/provenance stay
unchanged, with no third-party addition, new license or redistribution claim.
All 296 original Git entries have mode 100644; no file is added or deleted.
During qualification NXVM commits the exact reviewed payload as
ad5c532a8d627a51748ea7d209a8d2d9fd558eed. Its eight trees have no working diff,
and the frozen hashes below remain identical. This fixed upstream commit is
the final import revision; the initial uncommitted observation is provenance,
not the delivered baseline. Upstream task documents are not imported.

Per-tree SHA-256 covers sorted case-sensitive relative paths, a tab, uppercase
file SHA-256, LF per record, UTF-8 without BOM. Counts and content identities:

- src/lib: 123, 6E816EA3C583A3CC83E4F041772CCA5238C9DDD5F32BD73C4FB8671DDEFB011C.
- test/lib: 78, E6E07FEA499AD2C5FEF9EC158258F91D93C91923934BEEEC693454324182DEE4.
- src/emulator: 28, 651573320EB1F23EA4E78BA5F30F7285E67AD9E496FE0D6D097432236DD6FC40.
- test/emulator: 23, 28D77D98508F444C8ED496FED36CF8AA119AE0947CB60859B6F7FA70D23894E1.
- src/x86-debug: 15, 9D5D4F2E894A22B10833ABDBDD61AED19D27055DF37D70AD35F3918C9E1C4A60.
- test/x86-debug: 9, B49F5BFBBF1FA4E84F4A0F2403DA6550367B9B595960714A5E8C779097E415E2.
- src/x86-ibmpc: 13, FE65B9699FB9B7CD97EB793E7057A13D2E15093F5097DE296628A4E1B2DB5B02.
- test/x86-ibmpc: 7, 72BFA4C356B44AB481DC79FE1EDD375A15A24BE851ADAB5369631BBAA0D32B88.

Only test/lib/integration/kvm_window_modal_smoke.c and its manifest differ
from SoftPC. The test SHA-256 is
380D944A4A1A175590E1010669D3072418EDA2F63B47F535BEE2ED52FBFDC667.
Its manifest SHA-256 is
07A167C4BEEE0B917FB77E60E7A4B7C5EA131FE722EDF2827EBB575B22EF3977.
Expected production +0/-0, test C +0/-7, manifest +1/-1; root helper/build
changes +0/-0. All other bytes remain identical. No format/identifier rewrite.

**Reviewed behavior.** Delete the synthetic LBUTTONDOWN, native capture query,
release and associated test-only observer branch from the modal/scale fixture.
Upstream reports RDP ClipCursor access denial; that report is context, not a
new SoftPC reproduction. Frame-scale/type switches, eight sizing edges, fit/
restore, modal-loop entry, control/title delivery and terminal teardown stay.
Dedicated capture-contract units retain successful/failed capture, rollback,
off-screen clipping and move/resize paths. Controlled units are not equivalent
to real desktop capture: this intentionally removes that incidental native
coverage, without pretending to fix or silently bypass capture in production.
No new path, state, polling, skip or product exception is introduced.

**Finite verification ledger.** (1) all 296 paths/bytes/modes match the frozen
payload; (2) both full cached builds and relevant focused tests; (3) all current
193 registrations per width, partitioned into 185 background, five desktop and
three native audio, with exact name equality and failure/skip accounting;
(4) source/test boundaries, manifests and governance gates; (5) unchanged
production-input/artifact and INI/media identities; (6) actual executor diff
review, push, scratch retirement and delegated closure. A source/test discovery
or inventory change must be explained, never silently excluded. No downstream
repository is modified or used by the build/tests after import.

#### S13 executor qualification and quality review

All 296 local files match the fixed upstream ad5c532a Git blobs and 100644
modes, independently checked against all eight path/content digests above.
Actual delta from 06430c21: production +0/-0, test C +0/-7 (net -7), manifest
+1/-1. Only test/lib changes; the other seven trees stay byte-identical.
No root infrastructure, CMake registration, public ABI, App or mirror changes.
The original audit found root helpers outside the eight trees with unrelated
differences; the imported test introduces no helper calls, existing helpers
remain compatible, and their ownership/manifest/boundary tests pass unchanged.
No root-helper import or downstream runtime/build dependency is required.

Hidden owned qualification job 40836 completes 09:22:20..09:27:27, exit 0.
Cached full builds take x64 5.24s and x86 4.01s: each compiles exactly one
changed modal-test C object and links its test executable. No production
object/library or package EXE rebuilds. Both follow-up builds report no work.
No compiler warning or error appears. Final-source full tests pass on each
width: 193/193, comprising 185 background (164 unit, 14 checks and seven
integrations), five desktop/package and three native-audio integrations.
Recorded group wall times: x64 120.69/22.92/2.75s; x86 122.70/23.20/2.86s.
Inventory JSON and all three JUnit sets have exactly equal unique names,
zero disabled registrations and no failure/error/skip/missing/duplicate case.
No initial failure or unchanged retry occurs in this qualification.

The changed modal integration passes x64 0.505s/x86 0.503s; the dedicated
capture contract passes 0.451s/0.517s within the complete suites. They remain
distinct proof: real native modal/geometry flow versus controlled capture
semantics and rollback. No new real-desktop capture success or reproduction
of upstream's RDP access denial is claimed. The session is Console Active;
no Computer Use, timeout increase or skip is used. All public/nonpublic unit,
Types/dependency/naming/manifest gates and existing product integrations are
represented in the frozen registration universe. Mirror (505 bodies),
documentation and whitespace gates pass.

Package EXE SHA-256 remains softpc32
C093BA279A7462D532FD3B186B7EEF2E6ABCD1C8A708A74F777BC0FEABAEB7B7,
softpc64 394939CE667FEF60D7BC0B8541EB842419BCEEE54461EEC458A5BC8C4B59E58B.
Both are up-to-date in their full production graphs and exercised by matching
package integrations; no artificial recompilation or publication occurs for a
test-only import. INI SHA-256 remains
38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D;
snapshot-midi remains
0ABD8902275A52CA72504EACD7B277AD801BA234E6BFFC41152383FD67D8B8F4.
No assets/media path changes.

Executor self-review reads the actual test/manifest and task-record diff:
only the approved incidental capture segment disappears. Modal/geometry,
frame-type/scale and teardown assertions remain; no registration is removed.
The dedicated unit retains capture failure/ownership coverage without using a
real host policy. Search of all test/lib/integration finds no remaining native
capture exercise; no test-only production API, alias, new platform dependency,
polling, retry, exception branch or second path is added. This is a bounded
changed-path quality review, not a fresh line-by-line audit of every unchanged
public source. All six verification-ledger members have proof above; final
commit/review/closure is recorded after executor push. Scratch is retired only
after evidence promotion and owned job exit; reusable caches stay intact.

**Coordinator actual-commit review and delegated closure.** Executor 350dcd95
is pushed. After role switch, review reads all four actual changed paths
against the request and packet. Independent committed-tree comparison matches
all 296 paths, blob identities and modes to fixed NXVM ad5c532a; no production,
root infrastructure or asset is changed. The seven deleted lines are precisely
the approved incidental capture segment; modal/sizing/teardown assertions and
all registrations remain. The full passing inventory and unchanged artifact
identities agree with recorded proof. No new API, platform/Types dependency,
polling, recovery route or quality blocker is found in the changed surface.
Owned jobs exit, scratch is removed and residue/documentation/mirror/whitespace
checks pass. Close S13 under the owner's explicit delegation, not a claimed
manual owner test. T99 stays open and work stops; its separate completion audit
and the existing TODO/Queue are not silently admitted.

### S12 admission: Ninja incremental-build integrity

Owner accepts S11 and requests: 准入S12，老规矩. S11 closes at 722ffaca;
its [closure](../history/M9-T99-S11-sb-midi-framing.md) retains the full delivery.
T99 remains open. Research/design first; do not erase caches or equate recent
successful builds with repair of the historical metadata defect.

Finite research ledger: (1) historical exact symptom and retained evidence;
(2) executable/tool versions and resolved preset/cache commands; (3) untouched
x64/x86 metadata identity and validity; (4) all repository build/cleanup writers
and observable concurrent processes; (5) serialized repeated no-op builds;
(6) unchanged configure/build cycles and causal dirty explanations. Each member
must end in source proof, measured proof or an explicit missing-evidence limit.
Private scratch experiments may refine causality but never become a second
production route or a replacement for observing the real trees.

Initial scope: task documents and disposable probes only; estimated production,
test and build-script diff +0/-0 pending evidence. No public API/runtime/guest
change. Capture metadata before running Ninja, since normal recovery can rewrite
the evidence. Use the stable existing ccache presets, one owned hidden job,
serialized per tree. Report exact compile/link/no-op counts and unchanged EXE
identity. A genuine tooling fix requires its causal design and diff first.
No new cache manager, retry loop, background service or speculative recovery.

#### S12 research and minimum disposition

Baseline 722ffaca, hidden serialized jobs 14732 (09:04:13..09:04:29) and 30068
(09:06:01..09:06:04), both exit 0. Before any Ninja load, copy both real trees'
dependency/build logs, cache, graph and rules into owned scratch. Tools resolve
to Ninja 1.13.2 and CMake 4.3.3 from the existing x64 WinLibs installation;
ccache is 4.13.6 from the existing x86 installation. Both cached make-program
paths resolve to the same Ninja executable. Different compiler architectures
retain their established toolchains; no tool upgrade or launcher change occurs.

1. **Historical symptom: confirmed, historical producer unknown.** T93's retained
   proposal records repeated recovery followed by hundreds of rebuild edges,
   including 584 at its input-ownership continuation and 613 during an earlier
   build. It does not retain the original corrupted metadata, byte offset,
   failing-record type, executable identity or contemporaneous writer timeline.
   Those missing inputs cannot be recovered from the now-successful cache.
   Prior generic wording about a premature Ninja log did not identify the file.
   [Ninja v1.13.2 DepsLog::Load](https://github.com/ninja-build/ninja/blob/v1.13.2/src/deps_log.cc)
   owns the exact premature-end recovery warning: it reads binary .ninja_deps,
   detects invalid/truncated records or path-ID conflicts and truncates for
   recovery. This message alone does not distinguish interrupted writes,
   concurrent writers or other corruption. Its separate
   [build log](https://github.com/ninja-build/ninja/blob/v1.13.2/src/build_log.cc)
   has version-change diagnostics. A v7 .ninja_log header cannot prove a healthy
   dependency database. Missing recovered dependencies can explain subsequent
   recompilation, but the exact old dirty-edge explanation is no longer present.

2. **Current configuration: source and measured proof.** CMakePresets.json uses
   stable separate x64/x86 trees and eight-way jobs. tools/build_cache.cmake
   embeds one ccache launcher/environment; the unchanged configure does not
   accumulate launcher prefixes or RC flags. Generated rules and complete
   `ninja -t commands` output have identical before/after hashes per width:
   x64 8E59B895FDAC638544523E766FD9CAA949C109C98166FEC6FFBCF998FF538CFE;
   x86 B6DF95C4ED71A2B3CBA13096B993E828B27770D7435C175EA4CEA8B12045DAA4.
   Cache, build.ninja and rules.ninja identities also remain equal.

3. **Untouched metadata: structural proof, not historical reconstruction.**
   Inspect the saved bytes without loading/repairing them through Ninja. Check
   signature/version, record lengths, complete EOF, path-ID checksums and
   dependency/output ID bounds. x64 dependency v4: 1,232,540 bytes, 1,693 path
   records and 1,359 dependency records; x86: 608,000 bytes, 1,579 paths and
   869 dependency records. Both reach exact EOF without invalid records.
   Build logs are v7 with 761/768 well-formed entries. Untouched dependency
   SHA-256: x64 48EED4B030ADD14B2CA5FA7AA5ADC23AFE6296FC053F9514B18C6899C2898565;
   x86 5BDA00FCDFA141F5D396CB578F14B6CCCFE6FC233808228B3EF3178877363AED.

4. **Writers/ownership: bounded source and process audit.** Search repository
   source/configuration for .ninja_log, .ninja_deps, recompact, make-program,
   launcher and build/cache/cleanup references. No tracked project code directly
   edits the Ninja metadata or creates a recovery path. Root CMake emits the
   graph and targets the two distinct package filenames; configure owns graph
   generation, Ninja owns build/dependency records. Clean-Build.ps1 refuses
   both persistent roots; its selftest uses its own disposable child. No other
   process targets either SoftPC tree at the observed preflight. A build process
   for another repository is left alone; it is not a competing writer to these
   trees. This snapshot cannot prove no historical concurrency or later writer.

5. **No-op/configure proof on both real trees.** Three repeated full cached
   builds, two unchanged configure/build cycles, then a final build all report
   no work: six no-op builds per architecture, zero compile/link commands and
   no recovery warning. Build calls take 0.41..0.54s; configure takes 2.19..2.41s.
   The first three no-ops preserve both metadata hashes exactly. Configure
   changes the text log's representation but not the binary dependency hash:
   latest-entry maps retain exactly 718 output keys; only the 26 existing
   configure-output records change, not any object/archive/executable entry.
   Duplicate historical entries collapse; no output key is lost. This is not
   evidence of dependency loss or an unwanted C rebuild. No copied or cleared
   cache substitutes for testing the original trees.

6. **Real incremental proof.** Temporarily update only config_smoke.c's mtime,
   not content; restore its original mtime in finally. Full-graph `-n -d explain`
   identifies exactly that older test object and its link. Each actual cached
   full build executes exactly two edges: one test object and one test EXE;
   takes x64 0.78s/x86 0.75s. The corresponding config test passes 1/1 each.
   After restoration, both full builds again report no work (0.34s/0.32s).
   Explain text appears on stderr and receives PowerShell's NativeCommandError
   formatting in captured logs; native exit codes are zero, not compiler/test
   failures. No shipping EXE is relinked. Test source content is unchanged.

**Recommendation: zero production/test/build-script diff.** Current incremental
behavior is correct, and no specific project defect is proved. Do not add a
lock wrapper, recovery manager, purge, automatic retry or speculative version
pin merely to dispose of historical warnings. If recovery recurs, preserve
.ninja_deps and .ninja_log before the next Ninja invocation, record process and
tool identity and run the existing explain path; this is an investigation
procedure, not a new service or permanent API. A future patch must follow the
actual failing record/writer, not the warning's generic wording. An isolated
manufactured truncation would reproduce Ninja's known warning but cannot
identify the historical writer, so it is not used as a causal substitute.

All six ledger members are disposed above; the missing historical damaged bytes
and writer sequence remain explicit limits. S12 research is delivered, not a
claim that the historical defect is fixed or owner-deferred. Owner decision is
needed to accept this bounded disposition versus require further investigation;
T99 is not closed. No new full unit/desktop/audio qualification is claimed:
this turn changes only documentation and runs two relevant unit tests plus
measured full build graphs. S11's 193/193 per architecture remains historical
runtime qualification. Shipping EXE hashes remain S11's, INI/media untouched.
An externally modified snapshot.midi appears during research; preserve it and
exclude it from this task's staging. Scratch (about 35MB) is retired after
promoting this report; both persistent caches remain. No reusable production
probe, external source copy or dependency is introduced.

**Coordinator actual-change review.** Executor 5d44c2cf is pushed. After role
switch, inspect its complete three-document diff against admission and S11
acceptance: no source, test, tooling, manifest or artifact is committed. The
six-member research ledger separates measured current behavior from unprovable
historical causality and does not equate zero reproduction with repair. S11
closure cites its actual accepted delivery. The source links support the
metadata distinction, not an allegation against a particular old writer.
Documentation/mirror/whitespace/residue checks pass; owned jobs are gone and
scratch removed. Unrelated snapshot changes remain outside staging. No research
delivery blocker; S12 still requires owner disposition and T99 is open.

**Owner closure.** Owner approves: 批准收口，请你把外部重命名变化一起提交推送。
S12 closes at reviewed research 0108e47e under the no-code disposition; the
[closure](../history/M9-T99-S12-ninja-integrity.md) records its verification
limits and the explicitly approved byte-identical snapshot rename. No historical
corruption repair, new full regression or T99 closure is implied.

### S11 admission: Sound Blaster MIDI command boundary

Owner accepts S10 at 2866cbd5: 好，收口本S，转入下一个S任务。老规矩.
S10's [closure](../history/M9-T99-S10-vga-cursor-disable.md) retains its reviewed
delivery; T99 stays open and S12 remains planned. S11 starts source research
and executable design, not an unapproved expansion to SB-MIDI support.

Freeze all defined MIDI command bytes, the one DSP write-state dispatch,
port busy/read status, reset, snapshots and existing MPU byte/message output.
Confirm command lengths from primary hardware documentation; reproduce the
38h/data collision through original ports with mocked audio. Compare the
smallest correct supported or explicit unsupported disposition, including
subsequent PCM, IRQ/busy and reset/archive behavior. Do not reuse another
device's mutable parser without proving its ownership semantics.

Research edits task records only. Temporary probes live solely in build/t99-s11
under the CURRENT budgets and never become build dependencies or release
inputs. Present code examples, exact scope, mirror/test estimates and proof
limits before any production implementation. Shared eight trees, published
EXEs, INI, media and snapshots remain unchanged in this phase.

#### S11 research: bound unsupported MIDI at the DSP parser

Research baseline is reviewed S10 2866cbd5. This delivery edits documents
only; private candidates below are counterfactuals, not shipping repairs.

**Primary hardware contract.** Creative's Sound Blaster Series Hardware
Programming Guide (copyright 1993-1996), sections 5-1 through 5-3 and 6-12
through 6-14, distinguishes one-byte normal output from persistent UART mode.
38h precedes each individual MIDI data byte. Commands 34h/35h/36h/37h enter
UART mode, where DSP writes are MIDI data until the separate DSP reset.
Write-buffer status expresses readiness, not a per-command semantic ACK/NACK.
The [primary manual](https://pdos.csail.mit.edu/6.828/2018/readings/hardware/SoundBlaster.pdf)
is consulted as a hardware reference, not copied as code or a repository asset.

**Finite ownership and opcode ledger.** Searches for MIDI_READ/MIDI_WRITE,
DspWrite, WriteCommandByte, DSPWriteState, bHighSpeedMode, BufferMidi and
snapshot fields freeze these distinct responsibilities:

- nt_sb.c VsbByteOut is the only production caller of DspWrite, via the
  original 022Ch I/O binding. DspWrite calls WriteCommandByte only in its
  WriteCommand state. No frontend, Compat or Lib command parser is involved.
- nt_sbdsp.c owns eleven existing write states, pending command arguments,
  PCM starts and reset. WriteCommandByte has no MIDI cases. Unknown 38h thus
  leaves WriteCommand active, and data 91h is dispatched as DSP_WRITE_HS.
- nt_sb.c rejects further DSP writes and reports FFh busy while
  bHighSpeedMode is set. The busy behavior is correct for actual high-speed
  PCM; changing it would hide, not repair, the wrong command dispatch.
- 38h needs exactly one following data byte. Add an unsupported-byte state
  to the existing parser and return to WriteCommand after consuming it.
- 34h/35h/37h are already declared; documented 36h lacks its corresponding
  constant. All four have the same unhandled-entry problem: ignored entry
  lets subsequent bytes become PCM commands. An unsupported UART sink in the
  same write state consumes data until the existing separate DSP reset.
- 30h/31h input has no outgoing payload byte. No host MIDI input exists, so
  keep the unsupported empty-input/no-generated-IRQ boundary, not fabricate
  input support. 32h/33h are unused legacy constants with no handler and no
  entry in this manual's command table; do not invent their protocol here.
- ResetAll -> ResetDSP already returns DSPWriteState to WriteCommand. Keep
  this single reset owner; no second cleanup path or compensating PCM reset.
- The archive already serializes DSPWriteState as one uint8 field. Append
  values 12/13 after the existing 1..11 and extend its existing validator;
  do not add fields or change archive layout. Existing valid old states keep
  their identities. An older executable would reject a snapshot captured in
  a new pending state; do not promise reverse-reader compatibility.
- nt_sb.c owns MPU-401 mode/status and calls the sole nt_sbmid.c BufferMidi
  parser. That parser owns running status, notes and pending host messages;
  Compat owns native delivery only. Feeding an unsupported DSP path into it
  would add capability and require explicit shared-stream/error ownership,
  not merely repair framing. Leave the working MPU route unchanged.
- Existing device_contract_sb_commands explicitly asserts that every byte
  30h..38h leaves the parser free for E1h. This is an incorrect framing oracle
  for 38h and the UART entries, not missing production code in the shared Lib.

**Fresh dynamic evidence.** Hidden owned research jobs 41300, 17316 and 40508
exit normally with research.exit=0. The first isolates 38h; the second includes
all UART entries and actual PIC requests; the final pass adds an explicit
pending-normal-byte reset assertion. They reuse the existing
device-contract fixture's PCM/MIDI mocks, cached native compile/link commands,
original outb/inb dispatch and real machine initialization. Private objects
and EXEs do not replace cached objects, archives or published products.

For each of x64/x86, after 38h the baseline remains in WriteCommand for all
256 possible next-byte values. Fifteen values alter the checked DSP state or
PIC request; 90h/91h produce busy status, E1h starts version output, and F2h
raises IRQ5. Specifically 38h->91h sets bHighSpeedMode=1 and busy=FFh. Each
34h..37h entry followed by 91h likewise enters high-speed PCM (four collisions).
These are current actual-port observations, not only the historical T95 trace.

The private bounded-unsupported candidate has zero checked data effects,
busy results or spurious IRQs in the same 256-value test per architecture.
It consumes each 38h payload then answers a genuine subsequent E1h normally.
For each UART entry it consumes all 256 byte values without PCM dispatch or
MIDI submission, ignores an in-band E1h, and accepts version commands after
the separate reset. Direct existing archive capture/restore preserves both
pending states; reset also cancels pending input. No native audio, guest
media/configuration change or Computer Use is involved.

The unchanged registered device_contract_sb_commands passes x64 (0.19s) and
x86 (0.21s) in the final pass, exposing its incorrect existing oracle. The
candidate is not run as a replacement for that unchanged fixture: its incompatible old MIDI
assertions must be corrected in the permanent test. Private 36h compilation
supplies only its documented missing constant; implementation would put that
constant beside its existing peers, not add a build switch. An initial probe
without PIC observation found fourteen DSP-state effects; the strengthened
probe also detects F2h's separate delayed IRQ, giving fifteen. No earlier
limited observation is represented as full IRQ proof.

**Recommended minimum and capability decision.** Preserve the original
explicitly unsupported SB-MIDI capability, but correctly consume its protocol
framing. This prevents accidental PCM/IRQ behavior; it does **not** play
SB-MIDI music. Do not set permanent busy, terminate the machine or invent an
error byte: this port has no semantic command-failure return. Ready means
input space, not confirmation that an attached MIDI instrument sounded.
Document this limitation and keep MPU-401 as the supported MIDI output.
This is a device capability boundary, not a Lib native-output success claim.

The candidate adds two enumerators to the existing write-state authority,
not another state variable, queue, worker, MIDI decoder or recovery machine:

```c
/* Existing command dispatch. */
case MIDI_WRITE_POLL:
    DSPWriteState = UnsupportedMidiByte;
    break;
case MIDI_READ_POLL_WRITE_POLL_UART:
case MIDI_READ_INTERRUPT_WRITE_POLL_UART:
case MIDI_READ_TIMESTAMP_POLL_WRITE_POLL_UART:
case MIDI_READ_TIMESTAMP_INTERRUPT_WRITE_POLL_UART:
    DSPWriteState = UnsupportedMidiUart;
    break;

/* Existing data dispatch. */
case UnsupportedMidiByte:
    DSPWriteState = WriteCommand;
    break;
case UnsupportedMidiUart:
    break; /* Existing separate DSP reset exits this mode. */
```

Measured private behavioral diff, excluding private EOF whitespace: nt_sbdsp.c
+23/-2. Add the missing 36h constant in base/inc/sndblst.h (+1/-0): proposed
production total **+24/-2, net +22**, two mirror files only. Incremental mirror
changed lines increase by 26, not decrease; full necessary-XP-subset accounting
and manifest hashes are required during approved implementation. No new public
header, Compat function, configuration, native dependency or build framework.
Fixing 38h alone would be smaller but knowingly retain four same-cause UART
collisions, so is not the recommended complete boundary repair.

Extend only existing device_contract_smoke.c, estimated +65..95/-4..8:
replace the wrong MIDI oracle, cover all 256 normal payload values, all four
UART entries, required reset, version/PCM continuity, unchanged legitimate
high-speed busy behavior, actual PIC non-delivery and pending-state snapshot
acceptance/rejection. Existing fixture owns real ports and native mocks, so
no new target or test-only production interface is needed. Root registration,
public eight trees, nt_sb/nt_sbmid, Compat and media stay unchanged. Task/design
and existing mirror provenance documents change separately from code counts.

After owner approval: apply the bounded original-source change with a local
reason marker, update the one existing test and two mirror identity rows,
run focused dual-width proof, cached dual full builds/publication, exact full
test inventories including native integration and quality gates; record actual
diff and conduct actual-commit review before owner acceptance. The research
does not qualify the shipping repair, run the full suite, replay a Windows
application or demonstrate audible SB-MIDI. S11 remains design-ready awaiting
the owner's capability/diff decision; S12 is not admitted. All owned jobs are
joined/exited, concise evidence is promoted here and the task's disposable
scratch (under 15 MB of its 300 MB budget) is retired before delivery. The
three-document delivery has zero tracked production/test/artifact changes;
documentation, 505-body mirror, whitespace and residue gates pass. No native
integration or full repository test is claimed by this research.

Coordinator reviews the actual pushed df70efc0 three-document diff, its owner
request and source/port evidence. S10 is closed by explicit acceptance, not
by research results. S11's finite opcode ledger, private baseline/candidate
proof, corrected IRQ observation, missing 36h constant, archive constraints
and unsupported capability decision remain distinct from implementation.
No source/test/asset edit, new public contract, native-output success claim or
downstream synchronization is present. Mirror, documentation, whitespace and
residue gates pass; scratch is retired. No blocking research finding remains;
owner approval is still required for the proposed mirror/capability change.

#### S11 approved implementation

Owner approves the displayed minimum at b38ff693: 批准，就按这个最小方案在S11内实施。
完成后编译双EXE、发布、跑全量测试、提交推送，再自行审计代码质量，汇报实际diff，等我验收。
The exact two-file production diff is implemented: +24/-2, net +22. One existing
App fixture changes +67/-2, net +65; no root registration or public-eight-tree
change. The existing architecture description now states the unsupported DSP
framing boundary and supported MPU route. Original reason/semantic ledgers
and the two current-manifest local hashes are updated; source selection,
upstream/subset hashes and snapshot layout remain unchanged.

Permanent tests retain 30h..33h's empty-input behavior but replace the wrong
38h/UART oracle. Five actual-port command cases each consume all 256 payload
values, checking parser state, ready/read status, PCM fields/native-open count,
MIDI submission count, DMA address/count and actual PIC IRQ/delay state. The
single-byte mode accepts a following version query; the UART modes absorb it
as data and resume command handling only after the separate reset. Values
12/13 round-trip through existing DSP capture/restore; value 14 is rejected
without consuming the pending state. Existing valid PCM/high-speed/MPU and
full archive tests remain, with no copied test-only production entry.

The first focused build fails in new test code: the local variable named
byte expands through a legacy header macro, shadowing IU8. Rename it data;
do not alter headers or production logic to work around the fixture mistake.
The failure is retained until evidence promotion. Final focused actual-source
tests pass x64 2/2 (commands 0.20s, archive 0.05s) and x86 2/2 (0.26s, 0.11s).
Final cached dual qualification/publication completes successfully. Hidden owned
job 31944 runs 08:50:35..08:55:42, exit 0. x64 build takes 5.63s, background
116.65s, desktop 22.82s and audio 2.99s; x86 build 4.77s, background 112.92s,
desktop 22.99s and audio 2.74s. Both widths execute 193/193 registrations:
185 background (164 unit, 14 checks, seven integrations), five desktop/package
and three native-audio integrations. Inventory JSON and the three JUnit sets
have exact name equality, no duplicate, failure, error, skipped or disabled
case. These are fresh final-source results, not the research candidate's proof.
No tests need an unchanged retry in full qualification. Legacy mirror-header
warnings remain; no warning originates outside softpc.new. The corrected initial
test-draft compile failure above is not hidden by this final green result.

Mirror (505 bodies), documentation and whitespace gates pass. Public eight trees,
root CMake/registration, Compat/Driver, INI, media and snapshots are unchanged.
Both package EXEs are rebuilt/published from the persistent ccache/Ninja trees;
no cache clearing or duplicate architecture tree is used. SHA-256:

- softpc32.exe: C093BA279A7462D532FD3B186B7EEF2E6ABCD1C8A708A74F777BC0FEABAEB7B7.
- softpc64.exe: 394939CE667FEF60D7BC0B8541EB842419BCEEE54461EEC458A5BC8C4B59E58B.
- Unchanged INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D.

Executor self-review examines the actual three code-file diff and complete
changed-path list. It confirms one existing DSP state owner, unchanged reset,
no added output decoder, worker, queue, polling, fallback or product special
case. The two appended states are necessary protocol positions, not a second
device-state model. The permanent fixture uses actual ports, existing snapshot,
DMA and PIC observations; it adds no test-only production API. Ordinary PCM,
high-speed busy/reset and supported MPU behavior retain their existing paths.
SB-MIDI remains inaudible/unsupported; neither write readiness nor no side
effects proves MIDI playback. No fresh interactive Windows guest playback is
claimed. New pending-state snapshots are readable by this executable; older
readers may reject states 12/13, despite unchanged archive layout. There is no
claim to implement UART MIDI input, timing or full hardware support.
Source and actual-port proof are distinct from native-host integration passes.
Task scratch is retired after promotion of this evidence; the reusable caches
remain. S11 awaits owner acceptance; S12 is not admitted.

Complete mirror accounting checks pinned XP a2f6d7c93aa4f11efc51147566d5961c59a8ef89,
both raw source hashes, S8/S6 subset-range hashes and before/current local
hashes. On LF-normalized bodies Git default reports sndblst.h +18/-15 ->
+19/-15 and nt_sbdsp.c +835/-1462 -> +839/-1445, combined 2330 -> 2318 (-12).
Git --minimal instead reports combined +772/-1396 -> +793/-1395, 2168 -> 2188
(+20). This corrects the design's additive historical-diff estimate: the S's
own +24/-2 is 26 changed lines, but matching against a much larger historical
patch can realign. Neither matching statistic changes provenance, authorizes
source exclusion or implies removal of semantic local fixes. No external
source is copied into the repair or made a build/test dependency.

**Coordinator actual-commit review.** Executor 478c680f is pushed. After role
switch, review reads all eleven committed paths against the owner's minimum
and the active packet, not just the executor report. Production +24/-2 and
test +67/-2 match the displayed plan; executor documentation/manifest is
+149/-17, with two rebuilt binary artifacts separately counted. Only the
two local mirror hashes change; original/subset identities and selection are
retained. The new parser states reuse the existing reset and archive field;
no Compat/Driver/public path changes or MIDI-output claim are present.
Permanent port, PIC, DMA, reset and archive assertions replace the old wrong
oracle without exposing test-only interfaces. The full final-source qualification
and initial fixture failure are explicitly distinguished. Published EXEs match
committed blob identities 95dbbe54b210197036b09a6af6bed687f99624cd (x86) and
b9c82d945a92f3310d75e0adf852dc49521c1aee (x64). All owned jobs have exited;
scratch is removed, only the two reusable caches remain, and residue,
documentation, mirror and whitespace checks pass. No delivery blocker found.
S11 is verified, awaiting owner acceptance; T99 remains open and S12 unadmitted.

### S10 admission: original cursor-disable research, design and repair

Owner request: 收口s9 准入s10. S9 is accepted and closed at reviewed delivery
46d821d4; the [closure](../history/M9-T99-S9-public-import.md) retains its
complete qualification. S10 is the next consecutive task. S11 SB-MIDI and
S12 Ninja integrity remain planned; no task is skipped or reordered.

Research first: trace CRTC 0x0A writes, original do_new_cursor and all other
visibility/shape writers through Compat and Driver to the existing text frame.
Recheck VGA, EGA and V7 semantics against authoritative hardware references.
The current 0x0A code reads the pre-write cursor_off, updates the byte only
when start scanline bits differ, then calls a shape routine that may set
visibility true. These are source observations, not fresh dynamic proof or a
complete root-cause finding. T98 S6's disabled-register observation remains
historical evidence and does not establish the cause of perceived thin cursors.

Deliver a finite writer/consumer and behavior ledger, exact minimum repair,
line-count estimate (including original mirror delta), and owner-local tests
for disable-only changes, enable/disable, shape/start/end, font and mode changes.
Preserve original card-specific behavior and the existing single cursor owner.
No public compensation, alternate renderer, polling or API change is planned.
Implementation approval follows that researched proposal; this admission alone
changes no production/test code or EXE and claims no new test execution.
Only build/t99-s10 may hold disposable probes, with the CURRENT packet's
budgets and mandatory evidence promotion/cleanup. INI and guest assets stay fixed.

#### S10 research: two-site cursor-disable repair

Research baseline: 74461df9. Owner request: 老规矩 调研审计 设计.
This delivery changes task records only. The candidate below is a private
counterfactual, not an implemented product repair or refreshed release.

**Hardware evidence.** IBM VGA/XGA Technical Reference, preliminary May 1992,
VGA Function 2-65 defines CRTC 0A bit 5 as Cursor Off. Video Seven's V7 VGA
Technical Reference, June 1988, 3-41 defines the same read/write disable bit
and EREB[4] write protection. IBM EGA Technical Reference, August 1984, page
33 instead marks the upper Cursor Start bits unused. Therefore the VGA fix
belongs in vga_prts.c, not in a common Lib renderer or copied into EGA.
Primary references consulted through their indexed manual text:

- [IBM VGA/XGA manual](https://bitsavers.org/pdf/ibm/pc/cards/IBM_VGA_XGA_Technical_Reference_Manual_May92.pdf)
- [Video Seven manual](https://www.bitsavers.org/components/video7/700-0242_V7_VGA_Technical_Reference_Manual_Jun88.pdf)
- [IBM EGA manual](https://www.minuszerodegrees.net/oa/OA%20-%20IBM%20Enhanced%20Graphics%20Adapter.pdf)

**Frozen ownership/call ledger.** Full source search for set_cursor_visible,
PC_cursor_visible, cursor_start.as.abyte, do_new_cursor and their consumers
finds the following distinct responsibilities, not interchangeable paths:

- gvi.h owns the original PCDisplay visibility field and access macros.
- vga_prts.c has nine visibility writes: seven in shape recomputation and two
  premature 0A-handler writes. Its five recomputation callers are CRTC 09
  (font height), 0A/0B (shape), 0E/0F (location). These form the repair universe.
- ega_prts.c has eight visibility writes in its own shape routine; EGA is a
  separate original adapter. cga.c has four for its mode/shape rules. Neither
  path receives the VGA bit-5 repair. gvi_init selects the original adapter;
  current config_inquire returns VGA (5), confirmed by both actual probes.
- v7_ports.c EREB preserves the existing crtc_9_b_protect gate around writes.
  The proposed assignment remains inside that gate; it cannot bypass protection.
- gfx_updt.c base_cursor_shape_changed calls cursor_changed, increasing the
  original video-dirty counter. No extra Machine/Session wake or dedup is needed.
- host/src/nt_graph.c make_cursor_change consumes original visibility for the
  native cursor and converts shape to a percentage. It does not own VGA policy.
- compat/video.c presentation_text_style copies PCDisplay visibility/spans;
  machine/driver.c calls it for both Window and Console texts after original
  dirty production. Session and Lib consume those frames, not device registers.
- Snapshot preserves original state through the existing archive; no field,
  layout, extra cursor authority or snapshot-format change is proposed.

**Root cause.** The 0A handler examines the old off bit, stores the new byte
only if the low five start bits differ, then always recomputes shape. Thus
0E->2E loses the disable-only write entirely. Changing start as well (2D)
stores the bit but the shape routine sets visibility true again. End/location/
font changes use that same routine and can override a disabled cursor too.
Clearing only the off bit can likewise leave the stale register byte behind.
The defective code is present in history 0e91ada1 and the XP-import delivery
41aa0bc2. Since that import, vga_prts.c changes only by T98 S6's four underline
refresh lines. This is a retained original-path defect, not S4 Console changes,
the shared-library import or the newer two-span frame contract.

**Dynamic proof.** Hidden research job 13216 exits normally with research.exit=0.
The probe reuses the existing VGA fixture and exact cached compile/link commands,
but emits only private objects/executables. It uses original outb/inb, original
mode-3 BIOS dispatch and timer updates, then real Driver copy_frame. PCM/MIDI
use the existing test mocks; no native desktop/audio or Computer Use is needed.
Nine checkpoints observe register readback, PCDisplay visibility, produced and
both leaf cursor modes: enabled; disable-only; disable+shape; end while disabled;
location while disabled; enable-only; off+outside font; enabled+outside font;
valid shape restored. For both x64/x86 the baseline violates six of nine
contracts; the two-site candidate passes all nine (18/18 combined). Every
checkpoint produces a frame through original dirty handling, with both leaves
reporting the same mode. This is actual producer-chain proof, not a screenshot
or a guest-application reproduction.

The unchanged registered vga_frame_smoke also passes on both architectures
(3.96s/7.54s), proving its current green result misses this defect. Its existing
four shape samples never set bit 5. Full repository, desktop, native audio,
font-change/protection stress and new snapshot cases were not run in this
research, and are not represented as newly qualified. Two initial probe builds
failed due to an omitted I/O prototype, then the legacy ios.h declaration mode;
using the fixture's explicit native declaration style resolves the probe setup.
Those are research fixture mistakes, not production build failures. The final
four private baseline/candidate executions all return normally.

**Recommended minimum.** Remove the premature visibility selection and the
low-five-bit-only assignment guard. Store the actual register byte and call
the one existing shape routine. Add off-bit handling to that routine's existing
hidden branch, so every recomputation honors the same register authority:

```c
/* Inside the existing unprotected CRTC 0A branch. */
crt_controller.cursor_start.as.abyte = value;
do_new_cursor();

/* In do_new_cursor, extend the existing hidden condition. */
if (crt_controller.cursor_start.as_bfld.cursor_off ||
    crt_controller.cursor_start.as_bfld.cursor_start >= (unsigned)get_char_height() ) {
    /* Existing hidden handling, host notification and dirty tail unchanged. */
}
```

Both changes are necessary: saving alone still lets shape re-enable the cursor;
checking alone cannot see a discarded disable-only write. No new helper, state,
lock, thread, timeout, polling, format or fallback is needed. Re-enabling uses
the existing shape branches; invalid start rows remain hidden as before.
The two local reason comments are the only added bookkeeping.

**Estimated implementation scope.** One production file, vga_prts.c: measured
private diff +5/-12, net -7, including two reason comments. This adds 17 changed
lines to mirror accounting, not zero mirror diff and not a claim about the
file's total historical patch. Existing underline/snapshot/ABI edits stay intact.
Extend existing test/app-softpc/unit/machine/vga_frame_smoke.c in place,
approximately +55..85/-0..5; no new target or framework. Update one XP manifest
hash row and the existing local-change ledger. Shared eight trees, Compat,
Driver, root CMake, firmware, user media/INI and snapshots need no production edit.

Permanent proof should use explicit expected register/mode values, not merely
mirror the implementation formula. Reuse the current fixture for enable/disable
with unchanged start; disable+shape; end/location/font recomputation while off;
font heights 8/14/16/32; out-of-cell start; V7-protected ignored write; re-enable
and original mode transition. Observe produced plus both leaf frames without
test-supplied dirty or host transaction. Retain existing shape/underline and
snapshot regression. After approval: cached dual build/publication, focused
tests and full registered x64/x86 regression including native integrations,
exact inventory, mirror/manifest and quality review, then push and await acceptance.

**Adjacent differences, not silently repaired.** The manuals also specify VGA
End-Start+1 and no wrap for End<Start; this retained VGA routine instead uses
End-Start and supports a split. Current tests explicitly encode those legacy
results. These require their own reviewed hardware-semantics/test-oracle change,
not changing Lib's valid ability to represent two spans. They are recorded in
TODO, not mixed into the disable-only candidate. This research does not prove
the historical perceived-thin-cursor report's full cause.

Actual tracked production/test changes in this research are +0/-0. Published
dual EXE and INI hashes remain S9's. Scratch peaks below 13 MB of the 300 MB
budget. Findings are promoted here; all owned jobs exit before scratch cleanup.
The task remains open for owner approval of this minimal implementation.

Coordinator review of pushed research 2b20b2ae checks its actual three-path
diff against the original request and the complete finite ledger. Hardware
references, private candidate scope, nine-point results per width, unchanged
VGA-unit passes, failed probe setup, pending permanent coverage and adjacent
TODO are explicitly distinguished. Source/test/EXE changes remain zero; no
implementation approval or full-regression result is invented. Documentation,
505-body mirror, whitespace and residue checks pass. Both research jobs have
exited and scratch is retired. S10 remains open for implementation approval.

#### S10 approved implementation

Owner approves at reviewed research 65afb9be: 批准实施 老规矩执行.
The exact two-site candidate is now implemented, including its reason markers.
Production is +5/-12 (net -7) in vga_prts.c; the existing VGA fixture adds 70
lines, with no new target or public/test-only API. No Compat/Driver, public
eight-tree, root assembly, snapshot layout, user INI or guest asset changes.
The existing XP manifest changes only the local body hash; raw XP and selected
subset identities/ranges are unchanged.

Full-file mirror accounting independently reconstructs the existing S5 **XP
subset ranges**, checks pinned raw SHA-256 57a3318256a0b9392329fd4afada8b9926b32345c4262f08c3f3324b65d291f2
and subset SHA-256 084da18a959a825d66ef900d60b212a9bcffffd3cea23da0607cbf561946b120,
then compares LF-normalized text with Git default diff (CR ignored). Against
that necessary subset the file is +221/-0 before and +226/-12 after: changed
lines 221->238, an increase of 17. The first accounting draft mistakenly used
the separate pre-S5 **baseline** ranges, was rejected by subset hash mismatch,
and supplied no accepted count; corrected XP ranges reproduce the fixed hash.
No source extraction is introduced into the build.

Permanent proof uses actual outb/inb and BIOS mode programming, original timer
updates, and real Driver copy_frame, with existing audio mocks. Four font
heights (8/14/16/32) each cover unchanged-start disable/re-enable, hidden shape,
end and location recomputation, repeated transitions and protected ignored
0A/0B writes. Out-of-cell starts remain hidden, and original mode-3 reset
restores visibility. Both Window and Console modes are checked. The existing
shape/underline and snapshot cases remain unchanged.

The first x64 test draft incorrectly required a new output frame for every
register write, even a hidden-to-hidden shape change. Local temporary
diagnostics establish successful register/state updates but produced=false at
font height 14: original host presentation may suppress a visually unchanged
hidden shape. The final fixture requires real production for every visibility
transition and font/mode change; hidden-only changes still check readback,
original visibility and both output modes, without requiring redundant output.
Protected writes must produce no frame. Temporary diagnostics are removed;
production is not expanded to satisfy the mistaken expectation. The focused
final-source VGA test passes x64 (5.37s) and x86 (7.62s). Full qualification is
separately completed below, not inferred from these focused results or earlier
private probes.

**Final-source qualification.** Hidden owned job 35532 finishes normally with
qualification.exit=0. Both cached full builds pass and publish the changed
EXEs, reusing the already rebuilt original video object. Registration is
unchanged at 193 cases per architecture. For each width, sorted registered
names exactly equal the union of the three JUnit execution inventories, with
no duplicate, failed, disabled or skipped case:

- x64: 185/185 background (120.86s), 5/5 desktop (23.12s), 3/3 native audio
  (2.99s); VGA regression also passes under full scheduling (3.96s).
- x86: 185/185 background (198.34s), 5/5 desktop (23.47s), 3/3 native audio
  (2.96s); VGA regression also passes under full scheduling (7.81s).
- Each inventory covers 164 unit, 14 checks and 15 integration cases, including
  seven background, five real desktop and three real audio cases. Existing
  snapshot transaction/checkpoint, restart, input and package paths pass.
- x86 types-layout-selftest takes 146.25s within its existing 180s budget;
  no timeout is changed or long job killed. Concurrent repository test work
  is observed but neither interrupted nor modified.
- Final full-build logs have zero warnings. The final focused x86 fixture
  build has 89 diagnostics, all in preserved mirror headers (legacy endif
  labels/macro redefinitions); zero warning locations are outside the mirror.
  Original-header cleanup is not admitted. No public source recompilation
  policy or warning suppression is changed.
- Documentation, current 505-body XP manifest, dependency/Types/naming/test
  manifest gates and whitespace checks pass. User INI hash remains
  38d9eecd0a002fa6bf924d5f87bda154c71d89a2790674965739179c2d56623d.

Published EXE SHA-256:

- x64: 9e11fdf3463dfca727b52583a624d335c990f96e19006b429b3ab0cb913a84ba.
- x86: c1725fe575740eb6d435d2a1b1c3b1c741282ef0a08f671f46672dbf6d832650.

**Executor quality review.** Actual source diff has exactly the two reviewed
sites, preserving the original protection and shared shape/dirty tail. The
repair removes competing premature visibility handling rather than adding a
state machine, helper, second renderer or frontend register interpretation.
The existing App-owned test consumes its inward real device/Driver contracts;
expected visibility/register values are independent of the production branch.
Hidden-to-hidden output may be elided, but visible transitions cannot. No
permanent diagnostic, synthetic dirty injection, fixed sleep or test-only
production interface is added. Public eight directories, root infrastructure,
Compat/Driver, INI, guest media and snapshots have no diff against 65afb9be.
The finite writer/consumer ledger above remains the similar-issue sweep;
distinct VGA end-row/wrap semantics remain explicitly deferred in TODO.
Source proof and real producer-chain test proof do not claim a fresh manual
guest application or downstream acceptance. Test C is +70/-0, production
+5/-12 (net -7); only two EXEs and five evidence/governance documents accompany
those two code files. Task scratch peaks below 2 MB; concise evidence is promoted
here, all owned jobs exit and scratch is retired with the residue check passing.
S10 remains awaiting owner acceptance, not closed or followed automatically by S11.

**Coordinator actual-diff review.** After executor ad9d5fcf is committed and
pushed, switch roles and read its actual complete nine-path Git diff, not just
the executor summary. The two original-source sites match the approved design;
the App fixture retains existing shape tests and adds real state/leaf checks,
without changing public interfaces or production behavior to satisfy the
corrected test draft. The manifest changes only local identity; the fixed raw
and subset hashes/ranges remain intact, with full-file +226/-12 and incremental
+5/-12 correctly distinguished. Five document changes record approval, finite
coverage, failed drafts, unchanged scope and remaining TODO rather than claiming
those adjacent semantics fixed. Both committed EXE blobs equal the published
files and the recorded SHA-256 values. No unrelated tracked change, extra build
tree or leftover task job/scratch remains. Final mirror, documentation,
whitespace and build-residue gates pass. No blocking quality finding remains
in this admitted diff. This review is not owner acceptance; S10/T99 remain open.

### S9 admission: exact public-eight-tree import

Owner admits the import as S9 and requires continuous numbering. The earlier
unadmitted VGA, SB-MIDI and Ninja candidates move in order to S10, S11 and S12.
Historical reports retain their then-current identifiers. S8 closes by authorized
advancement from reviewed delivery 304cea4f; T99 remains open.

Original request: 准入一个新的S任务，原样导入nxvm的公共八组件，老规矩，
编译发布测试提交推送+代码质量审计后等我验收。

Import identity: cshaxu/nxvm shared commit
04674703a9b0c977e94e7334450a022de3e6514c. Latest observed HEAD
197076521acf9e593a6c0340ae59fb5d617806f8 has identical eight trees.
Source/destination are the same src/test paths for lib, emulator, x86-debug and
x86-ibmpc. Existing notices are preserved; this owner-approved same-project
shared-corpus update adds no separately licensed dependency or redistribution
claim. NXVM stays read-only, never a build/runtime dependency.

Frozen ledger: 296 files, identical path sets, no additions/deletions. Source/test
counts are Lib 123/78, Emulator 28/23, X86-debug 15/9 and X86-IBMPC 13/7.
Six differences: Machine implementation, Session control, source manifest,
Machine wait test, Session queue-failure test and test manifest. Pre-import
measured production +58/-20 (net +38), test C +52/-2 (net +50), four manifest
hash rows changed. No root helper import.

The reviewed changes check lifecycle wakes through existing Machine failure/
cancellation, prevent ERROR being overwritten by RUNNING/PAUSED and commit
Session queued work only after successful notification under its existing mutex.
Private enqueue uses existing lib_status to distinguish allocation and I/O errors.
No public API, new state/worker/queue, polling, retry or product-specific path;
existing pause/FIFO policy remains.

Acceptance requires pinned equality for every member, cached dual full builds,
complete registered background/desktop/audio tests and exact execution inventory.
Review actual diff, inward dependencies, Types, naming, manifests and ownership;
commit/push executor delivery, then coordinator actual-change review. Preserve
INI/media/snapshots and publish both EXEs. S9 owns disposable build/t99-s9:
30-minute run, five-minute no-progress and 300MB limits. Promote evidence and
retire scratch after all jobs exit; await owner acceptance.

#### S9 executor delivery and quality review

Final import is byte-identical for all 296 frozen paths; no path is added or
deleted and all use LF. Six changed paths match the audited upstream commit.
Actual production C is +58/-20 (net +38), test C +52/-2 (net +50), and source/
test manifests together +4/-4. Lib and both X86 pairs are unchanged. App,
original mirror, root build/test helpers, guest media and user INI are unchanged.

Quality review follows the actual six-file diff and affected callers. Machine
keeps one executor/cancellation path; failed lifecycle wakes no longer report
acceptance, and guarded state commits cannot overwrite an observed ERROR.
Session's existing queue mutex covers notification and commit: a woken reader
must acquire that mutex before observing the appended event. Failed notification
does not append; existing fault slots retain the actual lib_status. No public
header/ABI, fallback, extra queue/thread/state machine, retry or polling is added.
Lib Types remains the sole base vocabulary and inward dependency ownership is
unchanged. Tests remain in their respective Machine and Session owners, using
their existing fixtures rather than test-only production APIs.

Similar-issue sweep: all command/resume signals in Machine and all private
Session enqueue callers were inspected. Ordinary lifecycle sites now check
results; request registration already returns its status and cancels a rejected
flag; completion already uses fail_sync. Shutdown's best-effort ordinary wakes
remain distinct from its independent task-cancel/join authority. Session fault-
slot notification remains the existing terminal reporting mechanism, not a new
accepted-work path. No additional in-scope defect was confirmed. This is source
reasoning plus controlled failure injection, not exhaustive OS failure proof.

Fresh cached qualification: hidden job 42348 exits normally, qualification.exit=0,
elapsed 5m02.50s. Both architectures configure/build successfully with zero
compiler warnings/errors in this incremental run. Four C objects per width are
rebuilt (two production, two tests); other objects/archives are reused for links.
Both focused tests pass per width. Full x64/x86 each pass 193/193, exact registered
name equality, zero duplicate/failure/skip and no retry: 185 background (164
unit, 14 checks, seven integration), five desktop/package and three native audio.
Background durations are 116.95s/112.41s, desktop 22.81s/22.36s, audio 2.76s/2.52s.
This includes all four shared packages, App integrations and both real package
paths. The host Console session is Active; no Computer Use is used. All manifests,
Types/dependency/naming gates, documentation, 505-body XP mirror and whitespace
checks pass. No claim is made that unchanged original-source warnings are fixed
or that every hardware/host interleaving has been exercised.

Published SHA-256:

- softpc32: 783F37579C11647B8761C5648C3BDF1AF584DA1BF5B98CC91E575B20C16B4EFE
- softpc64: F97A5F9E0140BC1C48F27E32EA6789D1592EBD0DA506473170A682709E782745
- preserved INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

Evidence is promoted here before retiring build/t99-s9 after confirmed job exit.
Executor delivery and coordinator actual-commit review precede final handoff;
S9 waits for owner acceptance, T99 remains open and S10-S12 are not admitted.

Coordinator actual-change review: executor a69fc1e0 is pushed. Its twelve paths
are six exact imported paths, two rebuilt EXEs and four task records. The actual
code/test/manifest hunks, full qualification evidence, corrected S numbering,
original owner request and preserved App/root/media scope have been checked.
Committed EXE/INI blobs equal published files. No new outward dependency,
Types alias, extra runtime path, weakened assertion or blocking quality defect
is found in this change. Build residue, documentation, mirror and whitespace
checks pass; task jobs have exited and scratch is retired. This review is not
an exhaustive concurrency proof or a downstream synchronization claim. S9 is
verified/reviewed awaiting owner acceptance; no next S is automatically admitted.
One final manual mirror-check invocation supplied a relative root and resolved
the manifest incorrectly; the corrected absolute-root invocation passes all
505 bodies, as did the original full qualification. No source/test fix or retry
was involved.

### S8 admission: physical memory adapter correctness

The owner accepts and closes S7 at reviewed delivery 7fac795d and admits
planned S8, requesting its task brief first. T99 remains open; S9-S11 remain
planned. S7 closure is recorded in
[history](../history/M9-T99-S7-app-test-contracts.md).

S8 first rechecks named physical read/write adapters, every actual caller and
the original physical SAS contract. The intended repair stays in App Compat,
using the original physical route with SAS_VIDEO behavior rather than adding
a direct-RAM implementation. Nonidentity paging in both directions, mapped
video and boundaries define the initial proof universe. Historical T94 DMA
observations motivate this audit but are not fresh S8 dynamic evidence.

No code is changed by admission. Current-source findings, the exact file set
and a justified minimal diff estimate precede implementation. Public eight
components, mirror, media and user configuration have no planned change.

### S8 research: physical SAS, callers and minimal repair

Research baseline is f251686b. The owner requests investigation through an
executable proposal, not production implementation. No tracked production,
test, mirror, public-component or asset file changes in this delivery.

Subsequent owner approval at bdcdd28b admits implementation of this complete
minimal proposal, including the phyR mirror condition, its local marker and
accounting. The research statements below retain their original proof limits;
fresh implementation qualification is recorded separately after execution.

#### Frozen ledger and current-source findings

The universe is the two Compat physical adapters, their two Machine wrappers,
all direct callers, original SAS routing and end-of-memory validation. Search
uses `rg -n` for `softpc_platform_(read|write)_physical`,
`softpc_machine_(read|write)_physical`, `c_sas_(loads|stores)` and physical
SAS entries over src/test, then reads each implementation and caller group.

1. `compat/memory.c` calls c_sas_stores/c_sas_loads. Original
   `base/ccpu386/ccpusas4.c` explicitly defines these as linear operations;
   bios_read_byte/bios_write_byte translate through guest page tables when PG
   is set. Allocation bounds are checked against the physical memory size
   before that wrong translation. This is a confirmed adapter contract bug.
2. The only production-source callers of the Compat pair are the two wrappers
   in `machine/machine.c`. All direct uses of those wrappers are in 13 App
   test C files: unit machine machine, bop, checkpoint, device_contract,
   dual_media, irq, fdc, mouse, partition_image and sound; unit Compat
   platform_failure; integration command_provider and runtime_input_continuation.
   No shipping Product, driver or snapshot routine calls the pair. Do not
   attribute live playback or snapshot failures to these adapters.
3. Original c_sas_PWS/c_sas_PRS are the explicit physical block counterparts.
   They call phy_w8/phy_r8, retain SasWrapMask/A20 and route SAS_VIDEO through
   existing C-VID dispatch. They do not walk guest page tables. Reusing these
   entries repairs the adapter without new declarations or memory logic.
4. DMA already uses original sas_PWS/sas_PRS. Debug memory deliberately
   preflights a linear request, translates once and accesses phy_w8/phy_r8;
   its rejection/transactional semantics are distinct. Snapshot captures SAS
   storage through its original archive path. Retain these separate owners;
   none should be converted to another adapter or rewritten to share a helper.
5. Original phyR has `addr + typeSize + 1 >= Length_of_M_area` as its range
   rejection. Byte reads reject the last two valid bytes; word reads reject
   valid near-end ranges too. The addition also needlessly risks overflow.
   This is an independent existing mirror defect, not caused by changing the
   Compat calls. Both the current and candidate physical adapter inherit it.
6. `machine/machine.h` promises ROM writes are rejected. Actual in-range
   calls return SOFTPC_MACHINE_OK while phy_w8 ignores SAS_ROM writes. The
   existing bounds reject out-of-allocation requests; they do not reject ROM.
   Preserve the original bus behavior and correct the misleading comment,
   rather than adding type scans or changing accepted status into a new error.

#### Dynamic evidence and its limits

An isolated probe compiles against each existing architecture's real object
and library set. It includes the owner-local device fixture for its existing
audio mocks; all generated code, a disposable 512-byte boot medium and logs
are task scratch. Nothing is linked into a shipping EXE or cached target.

Set physical 00005000=31h and 00105000=72h, then enable a nonidentity page
mapping from the former linear address to the latter physical address.
Both x64 and x86 show the same results:

- Current adapter read returns 72h instead of 31h.
- Current adapter write of 94h leaves physical 00005000=31h and changes the
  wrong physical page 00105000 to 94h.
- Original physical SAS reads/writes the intended address with PG still on.
- B8000 is actually SAS_VIDEO. An original physical write/read returns 31h;
  the current paged adapter instead reads 00h from its shadow RAM mapping.
- With wrapping disabled, at the 16-MiB allocation end, byte reads at size-3,
  size-2 and size-1 return 31h, FEh and FEh after physical writes of 31h.
- An in-range ROM write returns OK and leaves its previous byte unchanged.
  One-past-end and overflowing wrapper requests return INVALID_ARGUMENT.

A separate scratch-only candidate substitutes precisely the two Compat
calls with c_sas_PWS/c_sas_PRS. Both architectures then read 31h, write 94h to
the correct physical page, leave shadow RAM at 72h and read video as 31h.
The final dual probe job exits zero. The same tail-byte FEh results remain,
proving why the two-call repair alone is insufficient for complete boundary
acceptance. The original phyR range correction is source-derived and has NOT
yet been compiled or dynamically validated.

Discarded harness drafts include missing boot-media setup, using test-target
instead of Compat compile flags, and duplicate unguarded original headers.
Those failures are not product findings. Candidate compilation reports legacy
original-header endif/redefinition warnings; no warning-free full-build claim
is made. No actual guest boot, desktop acceptance, new full suite or mirror
patch validation is claimed by these isolated probes.

#### Recommended single-path implementation, pending scope approval

The two adapter replacements are:

```diff
-    c_sas_stores(address, (IU8 *)bytes, length);
+    c_sas_PWS(address, (IU8 *)bytes, length);
-    c_sas_loads(address, bytes, length);
+    c_sas_PRS(address, bytes, length);
```

For complete boundary correctness, additionally repair the original owner:

```diff
-    if ((addr + typeSize + 1) >= Length_of_M_area) {
+    if (addr >= Length_of_M_area || typeSize > Length_of_M_area - addr) {
```

Short-circuiting validates addr before subtraction; the comparison accepts
exactly the remaining byte capacity. Keep original A20 masking, type/device
dispatch and out-of-range value handling. Add the required local reason marker
and update the XP manifest and local patch ledger. This adds mirror diff and
therefore REQUIRES owner approval: the admission's initial no-mirror expectation
must be revised, not silently bypassed with a Compat tail workaround.

Production estimate: memory.c +2/-2; original ccpusas4.c +2/-1 including its
reason marker; Machine header comment approximately +3/-3. Total approximately
+7/-6, net +1. Only three executable statements change; no API/layout changes,
new state, locks, workers, polling or direct-RAM path. Shared eight trees stay
unchanged. Mirror delta is one replaced condition plus one reason marker,
not a source import or formatting sweep.

Extend the existing Machine device-contract executable with one physical-memory
case, reusing its creation/reset/mock/cleanup path; register it in root CMake.
Estimated test addition is 100-150 C lines and two registration lines, not a
new test framework. Proof obligations: nonidentity read/write including a page
boundary and unchanged shadow RAM; SAS_VIDEO reads/writes preserving mapped
storage; A20 on/off; null/zero/outside/overflow refusal with unchanged outputs;
first/last RAM bytes and a valid span ending exactly at memory end; ROM ignore
behavior and inaccessible-read semantics. Use known sentinels and independent
test-only RAM observations for the tail, not the repaired reader as its own
oracle. Restore CPU/page/wrap state and destroy through existing ownership.
Retain existing text boot, paged DMA, checkpoint/TLB and debug regressions.

After approved implementation: cached dual build/publication, focused memory
and affected caller tests, complete background tests and applicable native/
package qualification, mirror/accounting/manifest and documentation gates,
actual-diff review, commit/push and owner acceptance. Counts above are an
implementation estimate, not an already implemented or tested full repair.

Coordinator review of pushed research c28eb8c1 checks its actual two-file diff:
only evidence/design and active status changed; no implementation or assets.
The proposal distinguishes the reproduced adapter defect, the still-failing
original boundary and the untested boundary patch. Final dual probe exit is
zero; concise evidence is promoted and all owned processes have exited.
Disposable probes/media/logs are removed through the approved cleanup tool;
documentation, whitespace and build-residue checks pass. S8 remains open for
the owner's decision on the minimal mirror scope, not accepted as repaired.

### S8 approved implementation and qualification

Implementation baseline is bdcdd28b. The owner explicitly approves the
three-statement proposal, including the original phyR correction. No new
implementation path, API, allocation, state, lock, worker or polling is added.

Actual production C/H changes are memory.c +2/-2, machine.h +3/-3 and
ccpusas4.c +2/-1: +7/-6, net +1. The first switches linear to original physical
SAS; the header describes actual bus/ROM behavior; the mirror changes only its
range condition and adds the local reason marker. Existing snapshot bodies,
A20 masking, device dispatch and out-of-range handling remain untouched.

Test C changes are +111/-1, net +110, in the existing device-contract fixture.
One new memory case reuses its creation/reset, firmware, audio mocks and
cleanup. Two original physical-width declarations are test-local, not new
production exports. Root CMake adds two lines for that case; no new executable
or shared test infrastructure. The finite proof ledger is:

1. Active nonidentity paging: a real linear read proves the mapping, while
   physical reads/writes cross a page boundary and preserve shadow RAM.
2. SAS_VIDEO: original mapped reads observe wrapper writes; wrapper reads
   preserve the video value and leave paged shadow RAM unchanged.
3. Allocation ends: first and final bytes, an eight-byte span ending exactly
   at the end, and original physical word/dword reads use independent expected
   values. Test-only host-byte observations verify the writes, avoiding a
   broken reader validating its own output.
4. Refusal: null machine/buffer, zero length, one-past-end, partial overrun and
   address overflow return INVALID_ARGUMENT, preserving destination and RAM.
   Rejected two-byte operations still receive adequately sized test buffers.
5. ROM and inaccessible pages: original ignored-write and inaccessible-read
   behavior remain observable; the temporary memory type is restored.
6. A20: enabled wrapping accesses low memory; disabled wrapping accesses the
   independent high-memory value. Original CR0, CR3 and wrap state are restored.

All six groups pass in the focused x86/x64 run. The first complete run passes
193/193 per architecture (185 background, five desktop, three native audio).
Self-review then replaces undersized buffers in rejected-request test cases
with existing arrays and adds unchanged-array assertions. A final-source full
run follows; preliminary results are not substituted for that qualification.

The mirror increment relative to this S is +2/-1. A comparison-only read of
the XP file matches pinned normalized SHA256
1673d63843341ed4861a1cada87e9c62bcc8d020587e68638fb97a663fceaab5.
Against that whole-file subset the current normalized diff is +62/-1: the
other 60 additions predate S8. The manifest retains XP raw/subset identity,
updates local hash to
5be2dbef6e0ec480e59a796588932d1b64d8a5a4920fdc222d4ecde386e681c6
and records REVIEWED_LOCAL_PATCH. The current semantic ledger identifies the
condition and marker. No external source is a build/test input.

The similar-issue sweep retains the research ledger: the only two named
Compat physical operations now call physical SAS; their Machine wrappers and
13 test consumers keep one interface. Debug's explicit one-time linear
translation and DMA's existing physical SAS route remain distinct and
unchanged. This is not an exhaustive repair of every original SAS primitive.
All eight shared trees are byte-unchanged against the implementation baseline;
no importing project needs a shared-component synchronization for S8.

Final-source qualification passes 193/193 per architecture: 185 background
(164 unit, 14 checks, seven integration), five desktop and three native-audio
tests. Combined JUnit names equal each architecture's 193 registered CTest
names exactly, without duplicates, failures, disabled cases or skips. The
final focused memory cases also pass. Background time is 118.88s x64 and
121.19s x86; native desktop is 24.27s/23.67s and audio 3.05s/3.20s. The final
Win3.1 installed-image roundtrip is 54.93s/53.73s; checkpoint is 1.26s/1.37s.
Desktop session is active; no Computer Use or manual guest test is used.

Ninja/ccache trees and original objects are reused, not cleared. The initial
production build publishes both EXEs; after the test-only safety correction,
only the changed fixture is recompiled and the complete build reports no work.
Across actual compiled paths, 712 repeated warning diagnostics belong to
unchanged original-mirror headers; no warning belongs to shared, App or test
source outside the mirror. This is not a zero-warning mirror-build claim.

Published SHA256 identities are:

- softpc32.exe: AF0E4D7DFD87C67F1A7E0EF5B10502F7A6EDF96FCF67C36A02F1937113B4CF8B
- softpc64.exe: 95104A23CB440B3A1D366705A36DFBC604C072C419A0891F12222AE5B3261CF6
- unchanged user INI: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

Both complete runs produce the same EXE identities. The final job exits zero;
mirror verification accounts for all 505 bodies and documentation/whitespace
checks pass. The six focused groups use the default 16-MiB fixture and selected
RAM/ROM/VIDEO/inaccessible/A20 cases, not an exhaustive hardware-address-space
proof. Registered DOS package and installed Win3.1 flows pass; no extra Win95
manual acceptance is claimed. S8 waits for owner acceptance after delivery;
S9-S11 remain unadmitted.

All owned qualification processes exit before cleanup. Concise results and
identities above replace the disposable scripts/logs; build/t99-s8 is retired
with the existing cleanup tool. Reusable caches are retained and the residue
check passes. No guest medium, snapshot or user configuration is changed.

Coordinator actual-change review of executor 349057b1 covers all eleven
committed paths, including source/test bodies, registration, XP identities,
task records and binary identities. It confirms +7/-6 production, +111/-1
test C, +2/-0 registration, the sole +2/-1 mirror hunk, valid failed-request
buffers and restored temporary CPU/mapping state. No duplicate production
route, public ABI, test-only production entry or dependency edge is introduced.
The shared trees and protected assets have zero diff against bdcdd28b; both
committed EXE blobs match publication. Final-source evidence distinguishes
the two complete runs, original-header warnings and untested hardware cases.
Gates and scratch retirement are verified after delivery. S8 is reviewed and
awaits owner acceptance; T99 stays open and no next S is admitted.

### S7 admission: deterministic App test contracts

The owner closes S6 without a second acceptance round and admits S7 at
819ff0cc. T99 stays open; S8-S11 remain planned. This is admission, not a
claim of completed S7 diagnosis or verification.

The frozen research universe has four members:

1. Checkpoint aggregate timeout: measure existing phases and waits under
   parallel load before proposing any budget change.
2. INT 1Ah oracle: replace the invalid nonzero low-byte assumption with
   independently meaningful guest/timer evidence; zero is legal.
3. Win3.1 prompt roundtrip: identify an observable readiness condition before
   menu interaction, preserving actual frame and mode-transition assertions.
4. Package viewport oracle: distinguish visible rectangular rows from linear
   backing-buffer reads; preserve separate full-buffer checks.

Each member needs a current-source finding, smallest design and diff estimate,
focused proof and final disposition. Test fixtures and their necessary
registration are the planned boundary. A production finding requires scope
revision rather than an unannounced repair. Host audio/session unavailability
is reported separately; the owner-disposed native startup observation is not
reopened. No S7 code or fresh test result is recorded by this admission.

### S7 approved implementation and qualification

The owner approves governance of the measured proposal: "批准治理，规矩你懂".
The implementation baseline is 7d58115c. The frozen four-case universe stays
unchanged; checkpoint receives the approved no-change disposition, not an
invented repair of its historical timeout. S8-S11 remain unadmitted.

Actual changes are confined to four existing App test C files:

- machine_smoke: +8/-44 (net -36). Remove the redundant INT 1Ah polling image,
  run and cleanup. Keep one real boot/INT 1Ah path with IRQ delivery disabled
  around full CX:DX/BDA comparison and an explicit 55h completion marker.
  The oracle accepts zero; no new mock clock or specific zero-time guest run
  is claimed. Dedicated PIT coverage remains.
- irq_smoke: +20/-6 (net +14). Observe the original BIOS IRQ0 handler's actual
  INT 1Ch callback through the original PIC request. The synthetic callback
  increments the known-zero high byte of the boot-completion word, uses CS
  override rather than assuming BIOS DS, and returns with IRET. Restore the
  old vector afterwards. Neither original IRQ0 handling nor the real executor
  clock is replaced.
- runtime_restart_boot_smoke: +74/-39 (net +35). One private RGB-region
  predicate and bounded waiter replace graphics-only startup and assumed Run
  dialog delay. The Program Manager region includes its initialized Main
  group; both Program Manager and Run require the original 640x480 extent.
  All mode/sequence assertions remain. Correct padded text-row indexing and
  type-aware diagnostics; stream text rows rather than writing an 81-byte
  array. Deterministic positive/negative row-padding controls share the actual
  prompt predicate, and a text frame cannot satisfy graphical readiness.
- package_smoke: +49/-25 (net +24). One native viewport-row reader writes the
  existing diagnostic buffer directly, removing the duplicate local text
  copy. Reopen the active output for each query; preserve the separate full
  guest-buffer reader. Add a real inactive 80x30 buffer / 30x10 viewport
  regression with OUTSIDE at (40,0) and VISIBLE at (0,8); never activate that
  buffer. Oversized diagnostic input fails rather than returning a clipped
  viewport as complete; native row reads must return their requested width.

Total test C is +151/-114, net +37, measured by git diff --numstat against the
baseline over those four paths. The increase over the +17 estimate is chiefly
the actual IRQ observation and padded-row regression rather than a new runtime
layer. Production C/H, public eight trees, mirror, root CMake and test inventory
have no change. No public API, worker, recovery state, polling service or retry
is introduced. The test's existing bounded sampling moves into one UI waiter;
it is not a new production polling path. Shipping EXEs need no refresh.

Implementation experiments refine the proposal without broadening the scope:

1. Merely clearing BDA then running a finite slice leaves zero: that slice
   need not contain a timer tick. A stopped-host-clock experiment also correctly
   fails Machine's existing executor-ready contract. Both failed test drafts
   are discarded. The final test instead observes real IRQ dispatch, with
   the normal clock intact; it does not report either draft as a product bug.
2. Program Manager's title alone can precede Main initialization. Captured
   frames distinguish a blank Program Manager (region hash 05edad9f) from the
   initialized Program/Main region (dc71b0df). Run's active caption region is
   fd3f1585 in both fullscreen and windowed rounds. Include the initialized
   region, not an extra sleep or Windows-specific production callback.
3. The extra nonzero-origin inactive-buffer experiment is not a valid fixture
   on this host. After buffer resize the queried origin is (0,0); reapplying
   its viewport can yield an actual 30x10 buffer despite the earlier 80x30
   size. The resulting stage-22 failures are test setup failures. Keep the
   admitted zero-origin counterexample, not a Lib workaround or a second
   drawing path. Nonzero native-origin behavior is not dynamically certified
   by the final inactive-buffer fixture; the reader uses actual queried origin.

Focused evidence includes actual Machine/IRQ/checkpoint passes on both widths,
and the live installed-image fullscreen/windowed prompt roundtrips. Independent
scratch probes compile the exact final caption/prompt predicates on both widths
and compare the original row-80 predicate: it misses a real padded-row prompt
and accepts an unused-tail prompt; the new predicate does the opposite. Four
real captured-frame classes per width reject uninitialized UI, distinguish
Program Manager from Run, and reject a changed expected pixel or insufficient
extent. These are predicate proofs, not new full guest runs or host screenshots.
No captured image or generated probe becomes a tracked fixture or product build input.

An earlier complete test round passes exactly 192/192 per width, with zero
failures/skips. Its wrapper then fails because Get-FileHash is unavailable in
that hidden Windows PowerShell environment; this is not a failed test or a
successful job exit. The wrapper now uses .NET SHA-256. Final self-review also
restores the explicit 640x480 guard. Fresh final-source qualification R3 now
completes with job exit zero: x64 and x86 each pass 184 background, five desktop
and three native audio tests, exactly 192/192 registered names, zero failures
or skips. This is a new complete run, not relabelled R2 evidence. Background
durations are 151.60 / 159.27 seconds, desktop 23.79 / 23.78 seconds and native
audio approximately three seconds per width. Final checkpoint is 1.32 / 1.33
seconds and the installed-image roundtrip 53.85 / 51.55 seconds. All shared
manifests, ownership/naming gates and mirror checks participate. Both cached
full builds have no compiler warnings; unchanged Core objects are reused.

Published EXEs and owner INI retain their SHA-256 identities:

- softpc32.exe: 0EA1A248BC280F9CA04B52005EE2594C5D9568AFCF07311DD81262B37E08F6BF
- softpc64.exe: 337EBE62DA50318FE53C82B4275C0F73CDBA2389EDFEE8BE29261208ECBF6077
- softpc.ini: 38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D

Executor self-review confirms four test owners, unchanged production/public
ABIs and frame ownership, retained original IRQ0 and mode assertions, one
viewport reader and no retry/compensation path. Modified C files remain LF.
Raw evidence is promoted here before all owned jobs and temporary probes,
captures and logs are retired. The delivery is committed/pushed before the
coordinator reviews its actual Git diff; owner acceptance closes S7, not this
executor result. No next S is admitted.

Coordinator review of pushed executor 13e32442 checks its actual six changed
paths and four complete code diffs, not just this report. Code accounting is
+151/-114 in App tests only. The full-tuple completion marker, original
PIC/BIOS callback and restored vector, exact 640x480 guard, typed/strided frame
inspection and single native viewport reader agree with the approved scope.
No full-buffer guest assertion, product path or lifecycle boundary is removed.
Final 192-name ledgers and dual-width predicate proofs agree with the promoted
evidence; limits of historical causation and inactive native origins remain
explicit. Both EXEs equal their unchanged committed Git objects and SHA-256
identities. User INI/media and all public trees have no diff. Documentation,
all 505 mirror bodies, whitespace and build-residue checks pass; all owned jobs
have exited and scratch is retired. S7 is verified awaiting owner acceptance;
T99 stays open and S8 is not admitted.

Similar-issue sweep searches App tests for fixed row*80 / 81-byte diagnostic
rows, bda_tick_low, INT 1Ah image references and the removed startup wait. The
incorrect forms are removed from the owning paths. The intentional row-80
negative control is retained. Existing mode-stability dwells and distinct
whole-buffer guest assertions retain their purpose; no generic wait framework
or speculative timeout widening is added. Current checkpoint timing and audio
isolation do not establish the historical timeout cause. Likewise these UI
oracle repairs do not establish the cause of the old sequence-36 report.

### S7 preceding research and minimal implementation proposal

Research baseline is 7d89b010. No permanent source/test change is implemented
by this report. Temporary probes were added only to the two existing App
fixtures, removed afterwards, and both original targets rebuilt on both widths.
No public tree, source mirror, shipping EXE, configuration or medium is changed.

#### 1. Checkpoint budget: retain, do not speculate

Current checkpoint links the App PCM/MIDI memory sinks. The historical T93 S8
timeout predates the T93 S15 isolation commit 66058ab4. Source and binary-path
inspection therefore exclude native audio acquisition from the current test,
but do not prove which native call or other cause delayed the old execution.

Actual phase markers in the existing executable record, in milliseconds:

| Width / schedule | Main-body total | Create/reset/first capture | Deliberate deadline | Archive round trip |
| --- | ---: | ---: | ---: | ---: |
| x64 isolated | 1200 | 16 | 1008 | 97 |
| x64 eight-way App group | 1229 | 22 | 1008 | 110 |
| x86 isolated | 1239 | 21 | 1011 | 113 |
| x86 eight-way App group | 1226 | 22 | 1016 | 105 |

These are measured phase differences, not complete process/CTest durations.
For example the first x64 CTest duration is 2.89 seconds, including work before
the first main marker. All 29 tests in each parallel group pass. The intended
one-second deadline is verified by snapshot.c's existing counter comparison;
there is no five-second product wait in this case.

Recommendation: retain the existing 15-second budget and code. Do not split
the fixture, add an audio workaround or claim the historical timeout repaired.
This is a proposed no-change disposition for owner review, supported by current
path isolation and measured headroom, not merely a passing repeat.

#### 2. Timer oracle: one INT 1Ah case and a deterministic IRQ owner

time_day.c's AH=00 path obtains a host timestamp, writes DX/CX and refreshes
both BDA words. A zero low byte is legal; programming PIT faster does not make
the timestamp's low byte nonzero. The current second INT 1Ah fixture tests
that invalid implication, whereas the first already checks the returned full
tuple against the BDA. A cold zero-filled tuple could also make equality alone
look successful before guest completion.

Minimal proposal: consolidate the two INT 1Ah boot cases into one. Its
disposable boot program disables IRQ delivery around the read, stores full
CX:DX and writes a completion marker before its terminal loop. Assert the
marker and full BDA/result equality, accepting zero without a clock mock.
Remove the redundant looping image, run and cleanup. Preserve PIT programming
in run_timer_boot_image and actual IRQ delivery in irq_smoke.c. Strengthen the
latter by explicitly clearing all four BDA tick bytes in its existing synthetic
boot program before programming PIT/enabling interrupts: its existing nonzero
assertion then measures IRQ progress from a known zero, not host time-of-day.
No installed guest or fixed media is changed. This is a proposed fixture
consolidation, not already verified new test coverage.

#### 3. Win3.1 readiness: observe the fixed guest UI, not a generic OS hook

The actual installed-image test passes twice on x64, in 62.28 and 62.50 seconds.
The second run's existing startup loop observes first 640x480 graphics at
597 ms / sequence 13, and the Program Manager caption at 4991 ms / sequence 22.
A copied guest-frame capture independently shows the Program Manager desktop.
This demonstrates distinct observable stages, not reproduction of the old
roundtrip failure or proof that this failure was caused by early input.

Minimal proposal: use the existing bounded startup loop to recognize a small
expected active-caption pixel region in this fixed image. Compare complete
RGB pixels, not just black text, so an inactive background caption is not a
readiness match. A private test-local region fingerprint avoids an OCR engine,
Win16 memory introspection, guest changes and new production callbacks. This
oracle deliberately belongs to the existing fixed-image App integration;
its expected region must be reviewed if that fixture's UI layout changes.
After one File/Run key sequence, wait for the actual Run dialog region before
typing instead of assuming 300 ms is enough. Reuse the same small predicate
and existing bounded waits, without resending keys. Preserve mode-transition
assertions and verify the same Program Manager readiness on return.

The current probe distinguishes early graphics from caption appearance only.
The proposed active-caption/Run-dialog predicates still need dual-width
negative and actual-path verification; a screenshot alone is not that proof.

The same file has two additional oracle issues in scope: DOS text scanning
uses row*80 instead of frame stride, and failure reporting interprets the
payload as text before branching on its type and uses an 81-byte row for
dynamic columns. Use the actual tagged payload and stride; stream diagnostic
row characters without another fixed-size buffer or allocation layer.

#### 4. Visible Console rectangle: one reader, direct counterexample

The current package_screen_contains opens the active native screen buffer but
reads width*height characters linearly. Win32 advances those reads by backing
buffer width, not viewport width. A native probe compiles that exact existing
helper, redirects only its open/close to a test-owned buffer, and leaves the
actual geometry and ReadConsoleOutputCharacterA calls native.

For buffer 80x30, viewport 30x10, HIDDEN at (40,0) and VISIBLE at (0,8), the old
helper returns hidden=1 / visible=0. Reading each visible row returns hidden=0 /
visible=1; probe exit is zero. Initial probe harness attempts lost diagnostics
or read GetLastError after another call; those harness defects were corrected
and are not evidence of a Console product fault.

Minimal proposal: keep active-buffer acquisition, use one private row reader
for the viewport and existing diagnostics, and retain the distinct full-buffer
guest-frame query. Add the counterexample using an inactive test-owned buffer
inside the existing package fixture; do not activate it or alter the product's
screen. Its assertions call the same row reader, not a copied implementation.
No Console broker or Emulator repair is indicated.

#### Estimated implementation and verification

The following are design estimates, not measured implementation counts:

- machine_smoke.c: approximately +12/-54, consolidating INT 1Ah cases.
- irq_smoke.c: approximately +4/-0, deterministic initial BDA ticks.
- runtime_restart_boot_smoke.c: approximately +50/-30, readiness and correct
  dynamic-frame inspection, including negative predicate checks.
- package_smoke.c: approximately +55/-20, rectangular reader and native oracle.
- Total test C: approximately +121/-104, net +17, with about 20 percent sizing
  uncertainty. Production/shared/mirror C/H: +0/-0. No new target or timeout
  change is planned. Documentation is counted separately.

Proposed order: timer/IRQ oracle, Console reader, then Win3.1 readiness. Verify
each through the existing owning test; run x86/x64 affected tests and complete
background suites, then native package cases in an available desktop session.
Test-only work leaves shipping EXE identities unchanged. No S7 implementation
or full-regression completion is claimed by these research runs.

The similar-issue scan of App tests finds the low-byte timer assertion only
in machine_smoke, and fixed row*80 / 81-byte diagnostic rows only in the
runtime-restart fixture. Full-buffer queries have distinct guest-frame purpose
and remain. The historical startup/device observation is not reopened.
All research workers have completed; original fixture sources are restored,
both widths rebuild without warnings and the restored checkpoint passes on
both widths (3.15 / 1.36 seconds CTest duration). S7 remains active for owner
review of this implementation proposal; scratch is retired after promotion.

### S6 delivery and owner disposition

The owner rejects carrying the historical native-start observation as deferred
work after the passing current-environment qualification: "干嘛保留？你都测试通过了，保留个毛线？"
Accordingly, its research stays below as history, without an active TODO, Queue
candidate or delivery blocker. This is an explicit acceptance disposition,
not a claim that a Windows initializer was repaired or that RDP causation was
proved. S6 is delivered, reviewed and now owner-closed by advancement to S7;
T99 stays open and S8-S11 remain planned.

The frozen four-case ledger is complete under that owner decision:

| Case | Final disposition and evidence |
| --- | --- |
| HLT/debug timeout | Correct the fixture to enter its own EB FE loop before resume/pause. Preserve the original HLT/interrupt contract; checkpoint regression passes on both widths. |
| TF quick eligibility | Clear quick_mode on the existing local debug-hook refetch edge. Source invariant and historical stale-TF observation support the correction; focused repetitions and complete regression pass. Do not claim a new deterministic failing-before TF reproduction. |
| Invalid boot/null BOP | Restore the fixture media ejected by earlier assertions before reset/restart. Paired actual-path probes change 7C00 from zeros to EB FE; original CPU reset returns F000:FFF0. No BIOS/BOP workaround is introduced. |
| Native Console initialization | Historical rejection is before main. Current full qualification and 24 additional desktop package executions pass. Owner accepts this disposition without retaining a deferred task; no native-failure repair is claimed. |

Production changes only c_main.c's existing port hook: +4/-0, net +4 relative
to the S6 implementation baseline (898e4054 and 7d3b711d have identical code).
Test changes only command_provider_smoke.c (+17/-0) and runtime_smoke.c
(+3/-0): +20/-0, net +20. The same four added source lines increase the local
patch against the XP necessary subset; upstream and subset hashes are unchanged.
Its manifest changes only c_main.c's normalized local hash. The public eight
trees, package fixture, user INI, media and snapshots have no diff. No new API,
state owner, execution path, retry, timeout, thread or polling is introduced.

Verification is the earlier fresh cached dual build/publication and exact
192/192 passing registrations per architecture (184 background, three audio,
five desktop/package), supplemented by 120 uninstrumented focused executions
and the separately recorded native diagnostics. The final temporary PID probe
is removed; both package fixtures rebuild and their four registered executions
pass. The additional desktop package total is 24 (4 initial capture, 16 bounded
compact samples, 4 restored-fixture cases), all passing. It is not another full
suite rerun or a repair claim for an unreproduced native failure.

Self-review checks the actual three code-path diffs, retained assertions,
single ownership, normalized mirror accounting and the artifact hashes below.
Both ETW sessions are confirmed absent; all owned jobs exit before scratch
retirement. Only build/.ccache and build/.ninja remain. Final documentation,
mirror, whitespace and residue checks precede commit/push and coordinator
review of the actual committed changes. User acceptance, not this record,
will close S6.
Final checks pass: documentation governance, all 505 XP mirror bodies,
diff whitespace and build residue. One manual relative-root mirror invocation
cannot resolve its manifest; the exact registered absolute-root invocation
passes. No source change is made to obtain that result.

Executor P2 8566eb42 is pushed to main. The coordinator then reviews that
actual eight-path commit rather than relying on the executor summary: its
three code diffs match the admitted repairs, original assertions and HLT/BOP
ownership remain intact, and only c_main.c's local normalized mirror hash
changes. Git object identities of both published EXEs equal their committed
objects; the SHA-256 identities above are unchanged. Both CTest inventories
still contain 192 registrations. No public tree, user INI/media, temporary
probe or raw trace is in the commit. Final gates and residue checks pass;
the workspace is clean. The owner subsequently closes S6 and admits S7;
see the closure record and current packet for the authoritative transition.

### S6 preceding research and repair record

The following records research states before the final disposition above.

Implementation progress after the owner's repair authorization: the integration
fixture now installs EB FE at 0000:0520 before its deliberate resume and checks
that paused CS:EIP remains there. This adds 11 test lines, removing none. With
that setup alone and the original quick-mode code, 20 command-provider runs per
architecture pass (40/40). This is not a failing-before/passing-after TF proof.
The local debug-hook return now clears quick_mode before the existing NEXT_INST
edge, adding four production lines and no second execution path. Another 20
command-provider runs per architecture pass (40/40). Both jobs complete; these
are bounded diagnostic runs, not full qualification. Mirror accounting updates
only c_main.c's local normalized hash; upstream/subset identities are unchanged.
The rebuilt checkpoint test passes on both widths, preserving its existing HLT
and interrupt-shadow assertions. XP mirror and its negative selftest also pass
on both widths (six focused test executions). No EXE publication or completed
repair P is claimed.

Further actual-path probes distinguish CPU reset, device POST and the first
executor callback. CPU reset and POST both return F000:FFF0. RESET_COMPLETED is
not this return: Emulator installs its executor callback and runs the driver
before publishing that fact at the pause rendezvous. The earlier inference of
already-corrupt reset output was therefore premature. The original nested POST
simulation is real, but its register save/restore is not implicated by these runs.

The outer fetch sequence reaches F000:FFF0 -> F000:E05B (reset BOP) -> E05E
(INT 19h) -> the original bootstrap -> 0000:7C00. At 7C00 the original fixture
reads 00 00 00 00, not its intended EB FE. command_provider_smoke previously
executed a real floppy eject while testing ERROR command admission, then reset
without reinserting its only boot medium. The fixture's supplied monitor state
is ERROR but the actual Machine is stopped, so that eject succeeds. The original
bootstrap reports failed boot but still transfers control to 7C00; executing
uninitialized guest memory can reach arbitrary BOP bytes. No BIOS no-op handler
or CPU reset workaround is appropriate for this test preparation error.

Keep the eject test, assert its actual completion, and reinsert the fixture image
before reset (+6 lines). With the same temporary probes this changes 7C00 to
EB FE and the first callback to the intended loop; both widths pass 20 runs
(40/40). An initial added assertion omitted the existing CRLF output suffix and
failed before boot; correcting that assertion is not counted as a product fix.
The earlier 40 no-media probe runs also pass as tests despite observing wrong
boot bytes, demonstrating why pass counts alone missed this precondition.

A same-class scan of all App tests finds only runtime_smoke's two direct ejects
and command_provider_smoke's provider eject. runtime_smoke ejects before its
restart/reset sequence too, matching the owner of the historical T84 null-BOP
report. It now reinserts its fixture at the stopped boundary (+3 lines); its
final eject remains valid teardown coverage. This does not prove every possible
guest null BOP has the same cause or reproduce the historical AV instruction.
All temporary production probes are removed. Uninstrumented command-provider,
runtime and checkpoint each pass 20 repetitions on each width (120 executions).
The subsequent complete cached dual build publishes both EXEs. Each width passes
184 background, three native-audio and five desktop/package tests, 192/192,
without failed/skipped registrations; build logs contain no compiler warnings.
This is fresh full regression, not proof of the still-unexplained native failure.
Published SHA-256: softpc32.exe
0ea1a248bc280f9ca04b52005ee2594c5d9568afcf07311dd81262b37e08f6bf;
softpc64.exe 337ebe62da50318fe53c82b4275c0f73cdba2389edfee8be29261208ecbf6077.
Actual production C/H delta is +4/-0 in one local CCPU hook; tests +20/-0 in
two App fixtures. Public components, original BIOS/BOP, INI and media are
unchanged. Actual-diff review finds no additional execution path or polling.

Native-start follow-up uses an independent small Win32 child (no SoftPC linkage)
with an event set from main. Forty alternating launches compare immediate
AttachConsole/hide against attachment only after that event. Both groups reach
main and attach (20/20 each); this provides no evidence for a fixture timing
change. The first diagnostic launch was incomplete because PowerShell treated
its stderr record as a terminating script error; switching diagnostic output to
stdout fixed the harness, not a native initialization failure. A separate
sandbox-launched group stops after seven recorded successes when its owner
environment disposes the job, without an observed child failure status; it is
inconclusive, not another 40-run pass. Four real package cases in full regression
also pass. Neither these results nor absence of a matching host crash establishes
the historical Console connection refusal's cause. No startup retry, window
hiding policy, timeout increase or system setting change is justified. S6 remains
active with that fourth case unresolved; full qualification does not close it.

The next native-source review finds a useful experimental distinction in
Microsoft Terminal's [handoff selection](https://github.com/microsoft/terminal/blob/main/src/server/IoDispatchers.cpp):
STARTF_USESHOWWINDOW with SW_HIDE suppresses delegation, while an ordinary
visible initial Console can delegate. Its handoff failure handler can continue
with the original server. Therefore the earlier hidden-start result neither
proves a SoftPC fix nor identifies a failing handoff operation. The current
source's [VT startup](https://github.com/microsoft/terminal/blob/main/src/host/VtIo.cpp)
also has a connection-abort path when its pipes close during startup, but no
local failing stack identifies that path. These mutable upstream references
explain candidate boundaries; they are not a matched-source diagnosis of the
installed binary. No external code is imported.

Read-only host inspection identifies Terminal 1.24.12741.0 and an OpenConsole
COM server. The default command token has Low integrity; the unsandboxed user
token has Medium integrity, not administrative elevation. A serial same-source,
same-EXE package comparison runs both registered variants on each architecture
under both tokens: Low 4/4 and Medium 4/4, zero failed/skipped cases. The current
unchanged user configuration selects Console, and the fixture reaches its raw
DOS/input/restart assertions. Thus this comparison covers raw presentation;
neither restricted integrity nor early attachment alone is proved causal.

An attempted bounded native ETW capture cannot start: the initial repeated -p
syntax is rejected; the corrected provider-file form is denied by Windows
permissions before any package tests run. No trace or startup evidence is
claimed from those attempts, and no session remains active. The installed
event channels expose no corresponding Console host diagnostic channel. This
is a diagnostic limitation, not evidence that the historical server failure
has disappeared. Case 4 remains open; no retry, hidden-start shipping policy,
timeout change or privilege-dependent product behavior is added.

The owner permits UAC and an administrator ETW session starts successfully.
Only error-level Console/Terminal providers are enabled, with a 64 MB circular
cap. The separate ordinary-user test job runs both existing package variants
on both widths: 4/4 pass, no native startup failure. Named events delimit those
executions. This distinguishes diagnostic elevation from elevating the product.
The administrator's finally block attempts stop and delete, but logman returns
an unrecognized-provider GUID error and the ETL continues growing after that
job exits. The planned four-minute lifetime is therefore exceeded; the size
cap remains. A cleanup UAC is cancelled before process creation; a normal-user
fallback returns access denied (5). After renewed owner confirmation, native
ControlTrace reports query=0, stop=0, then query=4201 (session absent). Cleanup
succeeds and exports 1,329 records. No persistent data-collector configuration
exists. The failed logman cleanup and excess duration are disclosed, not
represented as a correctly bounded initial capture.

Initial error payload inspection finds E_FAIL at SystemConfigurationProvider
line 212 in both inbox conhost and OpenConsole, pipe-not-connected at the inbox
ConDrvDeviceComm line 147, and invalid-parameter at OpenConsole screenInfo line
1410. These occur during a trace containing passing package executions and
other Console activity; an error code alone is not a causal failed-start record.
The historical first connection refusal has not been captured in this trace.
Raw traces remain disposable research data, not checked-in evidence or inputs.

A second diagnostic corrects session retirement to that proven native stop/
query sequence. It samples only the historical compact Console path, alternating
architectures with an eight-run limit per width and stopping at the first
failed test. A temporary package-fixture PID record correlates each child;
no production code changes. All 16 executions pass. Native stop=0 and subsequent
query=4201 confirm normal session retirement, and the exported trace has 455
records. Its error-site inventory repeats configuration/icon E_FAIL, process
handle/pipe closure and screen-parameter diagnostics seen alongside successful
starts. No historical 0xc0074005 connection refusal is captured. Both fixture
executables are then rebuilt without the temporary PID probe, and the ordinary
and compact package registrations pass again on each width (4/4). These bounded
diagnostics supplement, not replace, the earlier complete 192/192 qualification.

Read-only retained System events identify 89 SoftPC 0xc0000142 popups, from
14:03:19 through 15:41:38 on 2026-10-22 (Pacific). Session events show remote
reconnection at 13:27:47 and transfer to the local Console at 15:45:51, with
no intervening recorded connection transition. No retained popup follows that
transfer. This is an environmental association, not proof that RDP causes the
failure. HKCU Console enumeration finds no SoftPC-specific application key;
it does not exclude every possible external configuration source. The owner
confirms current testing is on the desktop and will not be asked to switch
merely to obtain a failing result. Available desktop runs establish their own
passes, not a repair of the historical remote-session initializer.

The administrator permission/cleanup blocker is removed. The remaining case
has an evidenced native failure phase and healthy-current-environment evidence,
but still lacks the first server rejection cause and a justified repair. S6's
four-case exit criterion remains unmet; accepting an external limitation or
deferring this case requires an explicit owner decision. The three local fixes
are preserved, and no partial implementation delivery or completed S is claimed.

The following records the initial research/design before implementation.
Owner admits S6 audit/research/design at 898e4054 and advances from S5.
This report separates four cases; it does not claim a completed four-root-cause
investigation or authorize permanent production patches. Existing case 5's
fixture-oracle work remains assigned to S7. No public component change is
proposed for any of these four cases.

**HLT continuation and execution-plan fixture.** Current c_main.c enters
RESUME_HALT for a restored halted continuation and stays there until its
original RESET or enabled-INTR condition is met. The same loop handles an
ordinary live HLT. Debug writes in machine/debug.c change registers only;
they do not unwind the CCPU's live instruction phase. command_provider_smoke
resumes arbitrary post-reset/BIOS execution, pauses it, then execution_plans
sets EFLAGS=2 and CS:EIP=0:500. That is not proof of a fetch-boundary pause.
Historical capture of RESUME_HALT with executed=0 explains that timeout.
Do not make EIP writes secretly cancel HLT or fake a guest interrupt.
Minimal fixture design: before its first deliberate resume, use the existing
paused debug access to install/select a test-owned non-HLT loop with a marker;
observe that loop before the pause used by execution_plans. Keep a separate
existing-path HLT/interrupt test so deterministic fixture setup does not erase
halt coverage. This is test setup, not a new guest/production execution path.
Estimated permanent delta: one App integration fixture, +15..30/-0..10;
production/mirror zero. Counterfactual still needed before choosing exact code.

**TF quick-path invariant.** DO_INST's local softpc_host_debug_begin() return
goes directly to NEXT_INST. Unlike normal retirement at c_main.c:4439, this
edge does not clear quick_mode. The hook parks the executor and permits flag
edits. RESUME_FETCH resamples TF but only sets quick_mode TRUE when eligible;
it never assigns FALSE in the ineligible branch. Therefore TF can be sampled
as one while stale quick_mode remains true, and subsequent retirement skips
the ordinary trap path. Historical controlled recomputation evidence is
consistent, but this turn did not execute a new patched counterfactual.
Preferred minimum candidate at the already-local hook:

```c
if (softpc_host_debug_begin())
   {
   quick_mode = FALSE;
   goto NEXT_INST;
   }
```

This reuses the original slow/refetch path and leaves global quick eligibility
and original device logic unchanged. Estimated production +4/-1 including a
short reason marker; existing App debug tests +10..25 lines to make the
quick-to-paused-TF transition deterministic. No public API change. Compare
TF=0/1, interrupt shadow, ordinary resume and stepping on both widths; count
this as a local-hook mirror correction, not an upstream XP bug. Do not broaden
the repair to every NEXT_INST edge without evidence.

**Null BOP dispatch.** Current c_cpu_reset initializes F000:FFF0, then the
App cold-reset path calls original reset/device POST before reporting done.
That larger sequence is not equivalent to the CPU reset setter alone. Original
reset.c has explicit CS:IP restore branches gated by soft_reset, while the App
cold path sets soft_reset=0. Thus blindly clearing CMOS or forcing F000:FFF0
again after POST is not justified by the old bad-register capture. The first
bad reset-completed sample predates synchronous_access, so that fixture's later
register edits cannot explain that sample. No first corrupting writer is yet
identified. Next probe must record CPU registers/phase at CPU reset return,
original reset entry/return, host_reset and POST boundaries, then the first
executor fetch; preserve the earliest mismatch and its call stack. Only then
choose an owner-local correction. Estimated production diff: undetermined,
not a fabricated one-line fix. No null BIOS handler, retry or new CPU state.

**Pre-main Console startup.** package_smoke still launches the shipping child
with CREATE_NEW_CONSOLE; it subsequently attaches and hides only the Console.
Historical matched-symbol evidence places the rejection in KernelBase native
Console connection (0xc0074005), followed by loader rollback (0xc0000142),
before App/Lib code. It proves the failure phase, not the server rejection's
cause. Moving FreeConsole, SW_HIDE or debugging the server has not supplied a
complete causal repair. Next probe should preserve the ordinary launch mode,
compare successful/failed native connection returns with child/server lifetime,
and capture the first rejecting server operation without global registry or
terminal configuration changes. No shared Lib repair is justified. A fixture
change is acceptable only if its own lifetime/ordering violation is proven;
otherwise report the native environment limit rather than hiding/retrying it.
Production diff remains undetermined; shipped window behavior must not change.

Fresh bounded verification on unchanged published binaries: x64 and x86 each
pass five command-provider executions and three ordinary package executions,
16/16 total, zero failed or skipped testcases. Session is Active at launch.
This is repeated diagnostic sampling, not a retry-to-green qualification;
no compact-package, patched TF, controlled HLT or first-BOP-write experiment
ran in this turn. No historical intermittent case is declared repaired.
Hidden job 31636 exits; source, test, EXE, INI and media are unchanged.
Scratch build/t99-s6 is retired after promoting evidence. S6 remains open for
the two required counterfactuals and two unresolved first-failure captures;
the owner receives this bounded design, not a claim of completed repair.

### TODO audit and owner-admitted T99 expansion

On 2026-10-22 the owner requests an audit of every TODO: remove confirmed
resolved items and move all unresolved work into this T as S tasks. Frozen
universe: the eight TODO entries at 9ee4af66. Source/caller/history inspection
confirms four unchanged defect paths and four unresolved intermittent reports;
none has sufficient repair evidence to be classified resolved. This is a
read-only source/status audit, not a fresh hardware or failure reproduction.
All eight leave TODO by transfer, not by an assertion of successful repair.
The separate runtime-performance Queue candidate is unchanged. S5 remains
verified awaiting acceptance; only one S may become active at a time.

Audited dispositions and implementation sequence:

- **S6, private CPU/debug and native startup (existing plan).** Merge the high
  priority historical BIOS[0x52] null-dispatch TODO into existing case 3 below,
  without a duplicate task. Current first-invalid-reset and BOP evidence is
  stronger than the old report, but no permanent repair or first corrupting
  transition is proved. Preserve HLT, TF and pre-main startup cases 1-4.
  Existing case 5's test-oracle work is assigned to S7, not implemented twice.
  Receiver: App/CCPU/debug and native startup fixture, not public Lib.
  Original evidence: [T84](../history/M9-T84-S1-kvm-text-80x50.md).
- **S7, deterministic App test contracts.** Consolidate three TODOs: checkpoint
  aggregate timeout, INT 1Ah zero-byte oracle, and Win3.1 prompt-roundtrip
  readiness. CMake still gives checkpoint_smoke 15 seconds, with no diagnosed
  repair; its prior isolated passes do not close the parallel failure.
  machine_smoke.c still asserts bda_tick_low != 0 in run_int1a_tick_boot_image,
  although zero is legal. runtime_restart_boot_smoke.c still observes graphics
  for 15000 ms before menu interaction, without Program Manager readiness.
  Instrument actual phase/waits under parallel load; replace the proven invalid
  timer oracle with deterministic guest/IRQ evidence; establish observable
  readiness before changing the menu sequence. Preserve frame assertions,
  budgets and failure evidence; no blind retries. Cover existing S6 case 5's
  viewport/buffer read distinction and host-device attribution in this same
  fixture audit, without treating disconnected devices as product failures.
  Historical evidence: [T93 S8](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s8-repeated-stop-proof-and-third-qualification),
  T92 S2 timer failure and T96 S7 graphics sequence 36. Pass/fail repetition
  alone cannot establish a cause or justify closure of the intermittent cases.
- **S8, physical memory adapter correctness.** memory.c still routes named
  physical write/read through c_sas_stores/c_sas_loads. machine.c's public
  physical wrappers call these adapters; the direct physical Sound DMA fix
  did not repair them. Audit all callers and use the original physical SAS
  route, preserving SAS_VIDEO mapping, not a direct-RAM second implementation.
  Test nonidentity paging for both directions plus mapped video and boundaries.
  Retain T94 S2's observation that c_sas_PRS sees the actual DMA write while
  the old named-physical oracle reads another page. Owner: Compat memory.
- **S10, original VGA cursor-disable semantics.** vga_prts.c case 0x0a still
  reads the old cursor_off, updates the register only when start bits differ,
  then calls do_new_cursor whose shape branches set visibility true. T98 S6's
  port observation (start=0x20/end=15 remaining visible) therefore has no
  identified repair. Verify authoritative VGA/EGA and V7 write semantics,
  then propose a minimal single original-path mirror correction; test off-bit
  only changes, enable/disable, start/end and mode transitions through ports.
  Do not change public text-frame transport or assume this explains perceived
  cursor thickness. Mirror accounting and focused dual-width proof required.
- **S11, Sound Blaster MIDI parser boundary.** sndblst.h still defines 38h,
  but nt_sbdsp.c has no MIDI_WRITE_POLL handler and still treats 91h as
  DSP_WRITE_HS. The original header explicitly excludes SB-MIDI support.
  MPU-401 playback success therefore does not fix the observed SB command/data
  collision. Establish original command lengths/state and a correct supported
  or explicit unsupported disposition before implementing; no second audio
  path or silent expansion to full SB MIDI. Test 38h followed by MIDI data
  including 91h, subsequent valid PCM commands and busy/IRQ behavior.
  Evidence: [T95 S1](../history/M9-T95-win31-midi-wav-playback-proposal.md#paused-transfer-and-midi-follow-up).
- **S12, Ninja incremental build integrity.** The current reusable x86 log
  has a v7 header and recent builds succeed, but no captured damaged metadata
  or causal repair explains T93 S15's repeated premature-end recovery and
  large rebuilds. Cache relocation is not proof of repair. Preserve affected
  metadata if recurrence occurs, compare command/dependency hashes and no-op
  rebuilds under the normal serialized build flow. Do not clear caches first
  or create another cache manager. Establish normal incremental behavior and
  either repair a proved tooling cause or report a bounded unresolved host
  limitation for explicit owner disposition; do not declare fixed from absence.

These S tasks own all eight original entries: cursor->S10, SB-MIDI->S11,
physical memory->S8, Ninja->S12, checkpoint/timer/readiness->S7, BOP->S6.
No original evidence is discarded. Source inspection commands include searches
for cursor_off/do_new_cursor, MIDI_WRITE_POLL/DSP_WRITE_HS, physical adapter
callers, bda_tick_low, checkpoint registration and the 15000-ms graphics wait,
plus relevant path history and the existing S6 failure ledger.

Each S starts with current-source confirmation and a minimal diff estimate,
then owns its narrow implementation, focused dual-width proof, proportionate
regression/publication and actual-commit review. Mirror changes require the
existing source-policy accounting; no standalone branches or fake-success
fallbacks. Unreproduced failures must have an explicit remaining-evidence
disposition rather than an invented fix. T99 cannot close until all S and
these eight transferred entries have owner-accepted dispositions. This plan
does not launch S6-S11 concurrently or silently close S5.

### Current S5 quality audit and S6 transfer

#### S5 P6 final private-surface cleanup

Owner-requested complete shared-unit rerun at 40c2860e: both architectures
pass exactly 118/118 registered unit tests (Lib 73, Emulator 23, X86 Debug 12,
X86 IBMPC 10), including their static gates. Sorted executed names match each
architecture's independently queried CTest unit inventory; zero failures,
disabled/skipped tests or retries. x64 takes 70.09 seconds; x86 57.90 seconds.
Existing P6 binaries are reused, without rebuilding or changing production,
tests, EXEs or configuration. Native desktop/audio and App tests are outside
this requested unit universe. Hidden owned job 38432 exits zero; evidence is
promoted here before retiring its task scratch.

Owner approves the two residual audit items at eac62924. Move the Product
opaque typedef to composition.c (no public function uses it); remove the
static source-ID allocator's null-argument check. Its only production caller
passes the static counter and a local output; owner-local exhaustion tests
also pass real addresses. Preserve exhaustion, public initialization validation,
composition ordering and ownership checks. No test deletion or new abstraction.
Estimated source delta is a few removed lines; two source manifests change.
Frozen proof scope: dual cached builds/publication, component/composition and
shared boundary/manifest regressions. Unrelated native/audio/package tests
are not repeated for this declaration/private-precondition cleanup.

P6 delivery: production C/H +2/-5 (net -3); test C +3/-3 (net zero).
The complete caller scan also identifies the IBMPC entry stub borrowing the
old typedef; rename its local dummy to fixture_product, without changing any
assertion. Three source files and one test file change, with three corresponding
manifests. No App, mirror, INI, media or root test-infrastructure change.
Both cached full builds succeed and publish both EXEs without compiler warnings.
Each architecture passes 21 selected component/composition/manifest/boundary/
corpus tests, plus the separately selected IBMPC entry test: 22/22, no failures
or skips. These are focused P6 results, not a new full-suite qualification.
The initial sandboxed hidden launches exited without executing the script;
the authorized hidden job 22928 completes with exit zero, and scratch is retired
after confirming it exited. No test or build was terminated by a time budget.
Caller/actual-diff review confirms the sole allocator production caller supplies
valid addresses, and no production consumer names the private Product type.
Public validation, identity exhaustion and cleanup coverage remain intact.
Documentation governance and whitespace checks pass.
Published SHA-256: softpc32.exe
8D99A7F11BFB204C6A0178F81A46677D5D82C8C5F5A08DDFAC414AA5D8BB5546;
softpc64.exe
19B3FCB294D93F8DC9908083872E39E9E6DB7CAC0616A1503439468C22A89235.

#### S5 approved follow-up at 6a9bf601

Owner admits all five audit findings as a single bounded follow-up: remove
duplicate preconditions from implementation-private composition stages while
retaining real failure/ownership checks; delete unused composition.h; make
the component-local source identity allocator static; use char text buffers
and consistent booleans/direct returns; replace Machine fixture's Win32
synchronization with existing Base events/tasks and Types atomics. Keep thread
affinity and shutdown completion assertions, without a new production API.
Estimate: production net -25..-40 lines, test changes primarily mechanical
replacement across three Machine fixture files, with no new test framework.
The frozen verification universe remains all 192 Windows registrations per
width plus focused changed-owner tests; removing WIN32-only registration makes
the neutral Machine test available on Linux but does not claim Linux execution.
No App/mirror/media changes. Scratch and budgets remain those of CURRENT.

Follow-up implementation and qualification: all five items are complete.
Private composition stages no longer validate impossible caller order or repeat
the public run preconditions. Allocation, downstream failure and ownership
barriers remain. Remove only the two corresponding invalid-order assertions;
creation failure, destroy ordering, retained-owner failure and ordinary run
coverage remain. The source identity allocator is static; its existing boundary
test includes the owner implementation rather than exporting a test entry.
The unused forwarding header is deleted. UTF-8 buffers use char directly, and
presentation booleans/direct-return style are consistent at the audited sites.

Machine fixture events/tasks now use Base; counters use Types atomics. C11
thread-local fixture identity preserves executor-affinity assertions. An explicit
completion event plus task join replaces the native helper-thread wait, retaining
the blocked-final-callback proof without sleeps. The existing Machine test is
registered on every platform, not just Windows. No new production interface,
platform test framework, polling, retry or lifecycle state was introduced.

Against 6a9bf601, production C/H +19/-43 (net -24), test C/H +149/-155
(net -6), test CMake +2/-4 (net -2). Five public trees change: src/lib,
test/lib, src/emulator, test/emulator and src/x86-ibmpc. Other public trees,
App, mirror, INI and media are unchanged. Manifests are synchronized. No root
test-infrastructure changes are needed for downstream adoption.

Cached x64/x86 builds refresh both EXEs. Focused tests pass 14/14 each.
Full runs pass 192/192 each (184 background, three native audio and five
desktop/package), with exact registration/passing-name equality, zero skips,
failures or retries. Desktop session was Active. No non-mirror warnings occur;
preserved mirror warnings are not changed. This proves Windows dual-width
qualification, not an executed Linux or MSVC runtime qualification.

Self-review scans all allocator/header callers, composition stages and test
assertions, UTF-8 buffer uses, and Machine native-symbol references. No stale
header or native Machine fixture reference remains; exported lifetime contracts
from the preceding P remain intact. No in-scope delivery blocker was found.
The larger textual test diff is primitive substitution, not extra behavior.

Published SHA-256 for this follow-up:
- softpc32.exe: 0BE8C7308FB3BD94C4A9CD261FCC2B4483837DC13294764F26F59C58D8D20CEA
- softpc64.exe: EAC612787CFA5C5AE5DB16AD8E16854F8A97BD9A409236337D9B7381ABEB047E

After job exit, retire task scratch and preserve only reusable caches. The
follow-up awaits committed-diff review and owner acceptance; S6 is not admitted.

Coordinator review of pushed 29cf8a95 confirms all 22 changed paths match the
five-item admission. Public run validation and actual allocation/lifetime
errors remain checked; deleted assertions concern only impossible private
order. Machine callback-affinity, input ordering and join barriers remain
observable through neutral primitives. Allocator exhaustion coverage remains
owner-local. No old forwarding-header or Machine Win32 references remain.
Actual diff, manifests, artifact hashes and complete test-name evidence agree.
Documentation, whitespace and build-residue checks pass; S5 awaits acceptance.

#### Approved S5 implementation qualification

Owner approves the unified minimal repair after the audit delivery 164a5f70.
Frozen production scope: Broker root/native owner, PCM root/native creation
cleanup, Emulator UI and Product (six C files), with directly affected contracts,
existing owner-local fixtures and manifests. No Base, Types, X86, App or mirror
implementation change is admitted. Broker/UI destroy consume on ordinary return;
Machine/private owner failures and public PCM destroy retain their existing
contracts. No new lifecycle state, callback, recovery thread or retry is added.

Finite verification ledger: inventory both cached CTest trees after configure;
every registered name belongs to background, native_audio or desktop. Require
the complete union per architecture with no skipped/failing cases, plus focused
Broker/PCM/UI/Product tests. Expected universe is 192 names per width. Native
audio availability and desktop/package readiness are separately reported.
Retain initial failures and their dispositions rather than calling an unchanged
rerun a repair. All jobs use S5 scratch and CURRENT's finite budgets.

Source review checks: completed-reader I/O failure still closes its handle;
post-join cleanup continues and reports first error; failed creation releases
the process claim; unconfirmed exit terminates without atexit; UI consumption
propagates through both normal and failed-assembly Product paths. Protected
Machine/driver ownership barriers remain. Existing tests are extended rather
than new production test entry points or a second cleanup implementation.

Initial configure failed because the new child-process wrapper attempted to
register two existing CTest names twice. The registration now uses the existing
lifetime-script registration loop with each test registered once; no test name
is added or removed. This configure failure is not a product failure or pass.

#### S5 executor verification and self-review

Final cached x64/x86 builds publish both EXEs. Focused regressions pass 12/12
per width. Full qualification passes 192/192 per width: 184 background, three
native-audio and five desktop/package tests. Sorted actual passing names equal
the frozen registration inventory exactly (192 unique names each); no failure,
skip or individual-test retry occurs in this final run. Native tests run in an
Active desktop session. Initial builds report only preserved mirror-header
warnings; no public-package or other non-mirror compiler warning is present.

Against 164a5f70, `git diff --numstat` counts six production C files +58/-69
(net -11); including five contract headers, production C/H +72/-78 (net -6).
Seven test C files total +88/-49 (net +39); existing test CMake/fixture script
changes add +20/-5 (net +15). Documentation, manifests and two binary artifacts
are counted separately. Only Lib and Emulator source/test public trees change;
X86, App, original mirror, user INI and media remain unchanged. Root test
infrastructure does not change.

The retained path is stop, confirm worker exit, then independent cleanup.
An exited reader's I/O status does not prevent handle disposal. Post-join
flush/close/mode errors remain observable without retaining inaccessible
objects. Raw and cooked handles are closed by ownership, not the mutable output
alias. Broker/UI ordinary destroy returns consume their objects; Product clears
those pointers and continues independent cleanup. Machine shutdown/unbind and
private owner failures retain their existing dependency safety barriers; those
later errors take precedence over an earlier UI cleanup error. Public PCM
destroy ownership is unchanged; only creation rollback and the detached native
wrapper are repaired. Linux unsupported paths create no worker/object and stay
unchanged. No timeout, public signature, state, thread, retry or callback added.

Similar-issue sweep uses `rg` for all Broker/UI destroy callers and inspects
Broker create/replace/cancel/destroy, PCM root/native cleanup and Product normal
and failed assembly. All actual consumers are adapted. Existing Window/Console
worker ownership and Machine dependency barriers have distinct contracts and
are retained, not mechanically rewritten. Source review found no new in-scope
blocker. Tests exercise real worker retirement plus injected native failures;
isolated child tests verify non-returning fatal exit without atexit cleanup.
These are controlled fault proofs, not claims of naturally occurring OS handle
failure, Linux runtime qualification, or repair of private S6 intermittency.

Published SHA-256:
- softpc32.exe: F74CBF62E248D46D8F28004A67D904CBE0C78CBC3C79407A1C2FA75418A7C811
- softpc64.exe: BE3F330A1B9953FED8BD5FC623BDFE29608122D5139AA2740E2888A255FDF541

Owned qualification jobs have exited. Concise evidence is promoted here before
scratch retirement. S5 is verified pending committed-diff review and owner
acceptance; S6 remains planned and T99 remains open.

Coordinator review of pushed executor 55a08f0c inspects the actual 31 changed
paths, including production ownership transitions, consumed-object test doubles,
child-process fault registration, manifest and artifact changes. The change
matches the approved six-C-file scope; dependency barriers are not weakened,
test failures are not converted into success, and no new recovery path exists.
The full passing-name ledger and published hashes agree with recorded evidence.
Whitespace, documentation and build-residue checks pass. Task scratch is removed
after job exit, retaining the two reusable caches. S5 awaits owner acceptance.

The owner's latest direction supersedes the historical S4/S5 split below:
S5 now owns confirmed public-package quality issues; the former five-case
private research packet transfers intact to planned S6. No private evidence
is discarded and S6 is not simultaneously active.

Audit baseline is cf60e14f. This is source inspection, not a new dynamic
qualification. The bounded coverage ledger is:

- Lib source/test: inspect Types/Base boundaries, UTF-8 directory ownership,
  storage I/O, Console broker lifecycle and associated failure fixtures,
  PCM/MIDI ownership, dynamic frame storage/exchange and Window rendering.
  Confirmed finding is the broker cleanup issue below. Other inspected paths
  did not establish a new repair requirement.
- Emulator source/test: inspect Machine input admission/FIFO, Session output,
  UI destruction and Product composition; scan direct platform/outer-package
  references. No confirmed new defect; do not reopen accepted pause semantics.
- X86 Debug and X86 IBMPC source/test: inspect package dependencies, command
  adaptation and registration, scan outer-package/platform references. No new
  confirmed violation. This is not exhaustive xasm32 instruction verification.
- Whole-tree structural searches are candidate filters, not proof that every
  branch or edge case was exercised. No fresh unit/native tests were run;
  accepted S4's 192/192 per width remains historical evidence only.

**Confirmed P2: Console reader failure and cleanup ownership are conflated.**
In the native broker, retire_reader observes WAIT_OBJECT_0 but returns
reader_status before closing/clearing the reader handle. Thus a completed
reader with an I/O error repeatedly prevents deactivate/destroy from reaching
cleanup. Separately, deactivate can fail after successful retirement (flush,
stop-handle close or mode restoration). Root broker create treats every such
rollback error as an unquiesced reader and returns without releasing the
broker, binding or process claim. The output stays null, so the caller has no
object to destroy. This is a source-proven ownership defect; no claim is made
that it caused the owner's cursor or input symptoms.

Existing reader-failure tests prove notification, not cleanup after a completed
failed reader. Root broker smoke covers activation failure with successful
retirement, not activation failure plus post-retirement cleanup failure.

Minimal repair direction: retain one stop/join/cleanup sequence; use actual
exit evidence, not the worker's I/O result, to decide whether cleanup is safe.
Preserve the first error while completing independent safe cleanup. A genuinely
unconfirmed live worker remains a separate safety case; do not silently free it
or introduce a half-object/retry protocol. Establish its exact existing policy
before changing it. Do not alter retirement timeouts as part of this fix.
Reuse existing root broker and Win32 failure fixtures, including recreate after
a safely cleaned failed creation. No new public API or persistent state is
currently justified. Planning estimate: two production files +15..35/-20..45;
existing tests +30..60/-5..20, plus contract/manifest edits if required. This is
an estimate, not an implemented or verified diff.

No production, test, EXE, INI or media changes accompany this audit. Before
implementation, confirm the ordinary-error cleanup contract against all broker
callers and test the concrete failure chains. The audit does not justify a
general lifecycle framework or rewriting other public components.

Owner initially admitted T98 S16 against 8165664a, then on 2026-10-22 closed
T98 and immediately transferred this unimplemented research to new T99 S1.
CURRENT owns status. No S16 research completion is claimed; scope and evidence
transfer intact.
The aim is fewer competing responsibilities and states, not clearing TODOs
with individual patches. Review the public eight src/test trees: Lib,
Emulator, X86 Debug and X86 IBMPC. Actual App callers and fixtures are evidence
for attribution, not permission to repair App or the preserved core.
Do not read or modify downstream repositories.

S1 was research-only: no production/API or permanent test repair was authorized
by its original admission. The subsequent owner-approved S2 scope is below.
Temporary probes may exercise existing paths without a second implementation,
guest modifications or Computer Use. Further implementation options require
owner approval; S3 and S4 do not inherit S2's repair admission.

## Owner-approved sequence after S1

On 2026-10-22 the owner directs completion of the two definite cleanups first,
with pause-input semantics and intermittent failures investigated separately.
S1 research (29c260b5, reviewed in bf18a2f7) is closed by that advancement.

- S2: admitted minimal cleanup. Merge Broker deactivate/reset failure cleanup;
  delete the extra ResetEvent after auto-reset wait in the App fixture. Preserve
  first error, cleanup order, rollback, all timeouts and input semantics. Add
  reset-delivery failure coverage by reusing the existing retirement scenario.
  Estimate production net -15 lines; test net +5 to +20. Build cached x86/x64,
  publish dual EXEs, run focused plus complete background regression and the
  affected desktop group. Unrelated native audio is not required. Commit/push
  and actual-diff review precede owner acceptance. No repair of the historical
  intermittent failures is claimed merely by removing redundant code.
- S3: owner admits minimal cleanup after accepting that pause need not preserve
  ordinary host transitions or resynchronize keys on resume. Keep Machine FIFO,
  KVM filtering and Session key ledger unchanged. Remove only duplicate
  SOURCE_RETIRED admission and no-op paused zero-button mouse admission.
  Update the existing Session test, contract documentation and manifests.
  Estimate production C +1/-3, test C about +5; no API/state/thread changes.
  Run cached dual builds, publish EXEs and all 192 tests per width; commit,
  push, review actual changes and wait for owner acceptance.
- S4: owner-admitted after S3 acceptance. Investigate observations 2 through 5 using exact
  missing completion and child/input boundary evidence. Distinguish startup
  death, live blank output and post-start input loss. Do not add retries or
  shared recovery machinery before identifying the first failing operation.

S3 is owner-closed; S2 is closed by the prior advancement. The owner's latest
scope split retains public-package repair planning in S4 and transfers the
other investigation to the next S5 described below. S5 is planned, not active;
neither it nor S4 is a separate T-queue candidate.

## Owner-directed S4/S5 split

### S4: shared Console reader retirement

Owner now approves the two-site minimal production repair, without S5 scope.
Implementation baseline is 2e85ed53. Frozen qualification universe: both cached
Ninja trees' current CTest registrations (expected 192 per architecture), each
classified as background, native audio or desktop/package. Record actual names
before execution and require exact coverage, no skips and no failures for a
full-pass claim. Existing S5 failures retain their own disposition rather than
being concealed by test retries or scope expansion. Production estimate is
+5..10/-10..20 lines; existing tests +20..50/-5..15; manifests and records are
counted separately. Scratch owner is S4, build/t99-s4, with the packet budgets.

The confirmed shared-package candidate is the Win32 Lib Console broker's
cooked-reader retirement before raw input. The actual product failure loses
input before the broker's raw delivery; independent native experiments reproduce
the loss with either cancellation API. The terminating-record/join control
retains all input in 10,000 cycles. The detailed counts below remain evidence,
not a qualified production fix.

Plan the minimal repair at the existing native reader owner: reuse the existing
terminating-record wake and join, removing cancellation only if the actual
reader lifecycle proves it sufficient. Do not add a reader, retry, timeout,
input replay, Machine/Session admission state or application-specific branch.
Before implementation, verify empty and partially edited cooked lines, completed
lines racing retirement, raw-reader retirement, wake failure, shutdown and
rollback. Preserve real error reporting and join before releasing reader context.
Keep ordinary text/echo and line-completion behavior unchanged; a wake record
must not become a user command or leak into the next raw reader.

Retirement and the reader are both private to
`src/lib/console-broker/win32/console.c`; Base owns neither operation. The two
cancellation wrappers have no other production consumers in Lib. Root broker
serialization excludes new request/activation until retirement ends. Raw reading
already waits on stop and input together; cooked reading needs the existing
terminating-record wake. No Base or public API change is currently indicated.
Companions are existing Lib broker contract/integration tests and manifest.
Qualification must include both architectures, controlled cancellation/retirement
regressions and the actual raw package handoff. Unit proof and native desktop
proof are separate. Other public packages remain unchanged unless a concrete
call-site dependency is established. No production repair is implemented yet.

#### Real-reader counterfactual and long-line discovery

A task-private probe includes the actual native broker implementation and real
Console I/O, using cached Lib dependencies. It cycles empty cancellation,
partial-line cancellation, over-capacity cancellation and a completed `ok` line.
Only the candidate's two native cancellation calls are removed initially;
wake, reader, join, pending state and flush remain the real implementation.
After the oversized line, the next delivered command is 33 old `x` characters,
not `ok`. The original unchanged native broker also fails the same scenario,
delivering an old long-line tail. This is not merely a mock-specific failure.

Source explains the separate defect: ReadConsoleW can return a fragment of an
already completed native line. The reader checks stop and returns immediately
after that fragment, before reaching LF; FlushConsoleInputBuffer does not
discard the native line's unread fragment. Thus thread exit plus input-record
flush does not prove that the cooked line has been fully retired. This is the
same ownership boundary as the input-loss candidate, not another input path.

The second private candidate moves the existing stop check after the existing
through-LF read loop, before UTF-8 conversion/event delivery, and removes the
two cancellation calls. It neither enlarges line storage nor delivers an
abandoned line. The x64 and x86 real-reader tests each complete all 400 cycles:
100 completed lines per width, no abandoned-line event, no I/O failure, no
pending input record, ordinary join and cleanup. Both jobs exit zero. This is
local controlled proof, not shipping or complete
package qualification. An initial probe compile incorrectly compared a void
release result; it was corrected before runtime evidence was collected.
The x86 launch initially misread the CMake cache compiler property's type;
correcting that task-only script enabled the recorded x86 build/run.

The minimal production candidate is one file: remove two calls, relocate
the stop check, and correct nearby cancellation comments (roughly +5 to +10 /
-10 to -20 lines, net reduction). Reuse current unit scenarios with wake-driven
completion and add native long-line retirement coverage to the existing
integration owner. Test estimate is about +20 to +50 / -5 to -15 lines; actual
accounting is required before delivery. Wake failure, delayed reader entry,
completion races and raw handoff require targeted proof. No existing timeout
or fatal policy changes are proposed.

#### S4 approved implementation and verification ledger

The actual production patch is +6/-13 (net -7), confined to the native broker:
remove both cancellation calls, move the existing stop observation past LF,
and update two adjacent comments. No API, state, timeout, allocation or thread
is added. Existing transaction serialization, stop wake, join, completion
observation and post-join input flush retain their owners and order. Raw-reader
stop remains its existing event wait; read errors remain on its existing path.

The existing worker cancellation unit is +31/-11 (net +20). Its wake mock now
uses the actual terminating-record route rather than cancellation mocks. It
checks an over-capacity two-fragment line is fully drained without delivery,
a completed command survives retirement, repeated cancellation is harmless,
and failed wake reports IO_ERROR while the worker/context remain owned. The
fixture explicitly releases its blocked read and joins before freeing context;
this introduces no production retry. Existing deactivate-flush failure coverage
is preserved. All shared values are read after event/join synchronization.

Final test placement is narrower than the research estimate: retain native
long-line evidence above and make its deterministic regression permanent in the
existing worker unit. The display integration deliberately replaces reader
threads with signaled events; adding a second reader mode there would conflate
its geometry fixture with input ownership. Existing actual package integration
supplies native handoff regression; no duplicate integration runner is added.

Similar-issue sweep: search `cancel_io_ex|cancel_synchronous_io|read_console_w`
under src/lib and test/lib/console-broker. The broker is the only production
read/cancellation owner; removed calls have no remaining production consumer.
Types retains its legitimate one-to-one SDK declarations. Reader-failure and
fragment/UTF-8 tests use the same source inclusion and remain in qualification.
Normal read, raw stop, replacement/rollback and destroy use the same retirement
helper; no other path or public package needs alteration. S5 is not implemented.

The first task runner's focused regex used a slash where registered names use
a dot, selecting zero tests. That invocation is not passing evidence; full
qualification still includes every registered test, and the corrected focused
invocation uses `--no-tests=error`. No production or test budget is enlarged.

Final implementation qualification on 2026-10-22: both cached ccache/Ninja
builds exit zero, 33 incremental steps each, with no compiler warning/error.
Both published EXEs are rebuilt from this source; no user INI/media is touched.
The corrected focused groups pass 7/7 per architecture (0.33s x64, 0.37s x86).
The full run passes on its first execution, without retries:

- x64: background 184/184 in 122.03s; audio 3/3 in 2.54s;
  desktop/package 5/5 in 14.27s.
- x86: background 184/184 in 114.89s; audio 3/3 in 2.86s;
  desktop/package 5/5 in 13.78s.

Each width's combined JUnit names exactly equal its 192-name frozen registry,
with 192 unique cases, no missing/extra names, failures or skipped cases.
Audio claims remain those of each test: default MIDI open/release alone is
not audible-playback proof. Normal and compact package paths pass on both
published products. Session was Active at launch. Historical S5 intermittency
is not declared repaired by this successful run.

Published SHA-256:

- softpc32.exe: CFD828584B0CB8F9BBB54593E5482FA7C599487B52CAEC9D58DBBCCE4C13619E
- softpc64.exe: E66A8962E3362CF2A0399B93DE4BE30E79DC4301B0061774A9C8D00E73172B35

Executor self-review checks the actual production/test diff against the two
approved corrections: no second path, polling, new timeout/state/API, changed
dependency or product policy. The existing bounded native line buffer and
join-before-release contract are unchanged. Test cleanup waits for the reader;
failed wake does not masquerade as completion. Both manifests and all corpus,
Types, naming, dependency and documentation gates pass in the full run.
Counted C paths are precisely the two files above; relative to 2e85ed53,
`git diff --numstat -- src test` reports production +6/-13, test +31/-11,
and manifest hashes separately +2/-2. Original mirror diff is zero. Only Lib's
src/test pair changes among the public eight trees; other shared owners are
unchanged. Both binaries and task documentation are separate from C counts.
All jobs must exit before task scratch retirement; S4 waits for acceptance.

Coordinator reviews pushed executor 52d8fcc3, including actual C, manifest,
documentation and binary differences: the eight changed paths match the packet,
tests preserve successful completion and failure ownership, and no extra public
contract or alternate input path exists. The complete name ledger and artifact
hashes above match verification. All owned jobs exited; disposable scratch was
removed after evidence promotion. Documentation, whitespace and residue checks
pass. This accepts the executor delivery for owner review, not S4/T99 closure.

Follow-up controlled tests now pass on both widths using the existing cancellation
fixture with the candidate reader and wake-driven completion. They preserve
already committed lines, discard cancelled fragments and verify a failed wake
returns IO_ERROR while the reader/context remain alive. Fixture-controlled
completion then permits safe join; this is not a new production retry protocol.
The builds use Wall/Wextra/Wpedantic/Werror without diagnostics.

A private x64 product links only the candidate broker object ahead of the
existing archive; all other cached production objects remain unchanged. The
actual package fixture, pointed only at its private raw-Console/overlay
configuration, passes 40/40 complete compact runs, including initial `ver`,
pause, stop and restart. All attempts are counted; none is retried or waived.
This supplements, but does not turn into, full dual-product qualification or
proof that S5's pre-main initialization failure is fixed. The shipping x64
hash remains AFA8D42BC1C3C0B21DCBD30527C4D6DCC054E40914946F74E575700416A88D94.
Temporary tracked command-provider diagnostics were removed and both cached
test executables rebuilt successfully. Production and test tracked code are
unchanged. All task-owned jobs exited before evidence retirement.

### S6: private CPU/debug and native startup investigation (transferred from S5)

Originally admitted as S5 at c5d6cf64, now transferred intact to planned S6.
This carries current
evidence forward rather than restarting the investigation. The five entries
below are the finite research ledger: each needs source ownership, evidence
class, first-bad-transition findings, candidate scope and explicit unknowns.
Prior passing experiments are not new runs. No permanent fix is authorized.
It must separate these cases:

1. **HLT instruction-plan timeout (App/CCPU/debug fixture).** The failed executor
   remains at RESUME_HALT after the fixture changes PC and clears IF. No trace
   instruction retires, so PAUSED cannot complete. Establish a deterministic
   valid fixture entry state and preserve the documented architectural HLT
   contract; do not infer that public Machine event delivery is broken.
2. **TF quick-path invariant (local CCPU debug hook).** The debug-begin jump
   bypasses the original quick_mode clearing invariant. The private
   recomputation experiment passes 60 runs and observes the stale TF case.
   Audit the smallest original-path correction and mirror accounting before
   proposing implementation; no public X86 Debug API change is established.
3. **x86 null BOP dispatch (private machine/boot execution).** Timing probes now
   capture actual AVs, not merely a timeout: one executes BDA bytes at
   0000:046f and dispatches BOP 0f; another reaches physical f1c00's BOP 52
   with CS=e7f4. Both reach an empty BIOS slot. In the latter run the first
   reset-completed register read already shows CS:IP=0000:5ce1, before the
   synchronous debug mutation sequence and first resume. This moves the next
   investigation to reset/bootstrap's first invalid transition. It does not
   prove that small fixture media, reset itself or a shared data race caused
   it. The last-instruction ring proves the eventual dispatch, not the first
   corrupt transition. Do not install a no-op BIOS handler. Cross-reference the
   historical BIOS[52] TODO without claiming its entire history explained.
4. **Pre-main native Console startup.** Matching KernelBase symbols locate
   0xc0074005 at ConsoleCreateConnectionObject's NtCreateFile after successful
   server launch; loader rollback surfaces 0xc0000142. Owned-server observation
   sees Windows Terminal OpenConsoleProxy loaded. All 40 starts under server
   debugging succeed, so that instrument perturbs reproduction. A separate
   SW_HIDE-start counterfactual has 20/20 successful native initializations,
   but only 16/20 complete fixture successes; the four later failures were not
   attributed and cannot be counted as repaired. Terminal delegation is a
   lead, not a proved server-side root cause. Compare normal-start reversal
   and obtain the first server rejection before suggesting a host/test change.
   This occurs before shared Lib or App main, so it is not assigned to S4's
   broker repair. Do not hide the shipping window or add startup retries.
5. **App fixture oracles and environment attribution (transferred to S7).** Preserve the demonstrated
   viewport-versus-buffer linear-read limitation and the already tracked legal
   zero timer-byte assertion. Keep disconnected PCM/MIDI composition failure
   separate from native loader failure. Repair only proven test-owned defects;
   an online rerun is not a repair of unavailable host devices.

S6 must report source inference, controlled counterfactual and real failure
captures separately, retain existing assertions until their contract is proved
wrong, and request approval for permanent private/mirror repairs. Do not expand
into work outside the owner-expanded S6-S11 allocation or downstream repositories.
The following asset restriction describes this research transfer, not later
owner-admitted implementation and publication: no guest, shipped EXE,
INI or media changes are part of this transfer. The completed diagnostic jobs
are stopped and S4 scratch has been retired. Future S6 diagnostics need an
admitted CURRENT packet and bounded scratch policy; publish concise evidence
and remove disposable files, rather than retaining raw logs permanently.

#### Historical S5 admission source recheck (retained for S6)

At admission, current source still matches the earlier captures. In
`c_main.c`, a retained halted entry goes directly to RESUME_HALT; ordinary
PC/register edits do not change that entry. The actual command-provider test
resumes the booted machine, requests pause, then execution_plans clears IF and
sets CS:EIP to 0:500. These operations do not establish that the CPU was parked
at a fetch boundary. First research target: establish a deterministic fixture
entry without changing architectural HLT or creating a second resume path.
This is source confirmation, not a newly reproduced timeout.

The debug-begin hook's return jumps to NEXT_INST. Its implementation explicitly
permits paused register/code edits before returning. At RESUME_FETCH the CPU
resamples TF, but the eligibility block only assigns quick_mode=TRUE, never
FALSE. Thus the existing local hook can bypass the original clearing invariant.
Compare clearing at that newly introduced return edge against broader changes
to original recomputation; prefer the narrower hook-local correction if all
entry paths and the controlled TF test prove it sufficient. No patch is made.

Next evidence targets are a controlled HLT/fetch fixture comparison, a TF
return-edge counterfactual on both widths, the first invalid bootstrap register
transition for the BOP case, and native startup before application entry.
No new dynamic test, all-case root-cause claim or S5 EXE publication is reported
at admission. Current production and tests remain identical to accepted S4.

## S4 bounded investigation report

Baseline 8072abdc; owner closes S3 and admits root-cause research. This report
does not complete the root-cause objective: S4 remains active. No production,
permanent test, public manifest, package EXE or user configuration is changed.
Temporary probes and raw logs are retired after promoting these results.

The finite investigation ledger keeps four distinct cases:

1. Command-provider wait: 40 unchanged isolated repetitions per width all pass.
   The existing single wait_for assertion is shared by running, paused, stopped
   and program-marker waits. Historical completion=2 does not identify which
   next wait failed. The fixture directly feeds runtime notifications and does
   not exercise the complete Session event loop. Its program marker is observed
   through the executor copy_frame callback. No causal proof of a lost wake,
   delayed callback, driver hang or fixed error is available. Receiver remains
   this fixture/executor boundary; next discriminating evidence is the call-site
   identity and producer signal sequence under aggregate load, not a longer
   deadline or a second completion mechanism.
2. Pre-prompt composition failure: previous disconnected experiments established
   unavailable native PCM/MIDI endpoints. Source chain is main -> compose_machine
   -> vm_create -> softpc_platform_audio_start -> PCM stream acquisition; failure
   is returned before monitor assembly. This explains those recorded exit-1
   cannot-compose-machine executions, not every blank screen or loader exit.
   No audio fallback or startup policy change is authorized by this research.
3. Loader/init exit: online x64 ordinary package passes twice then fails on its
   third attempt; compact passes once then fails on its second. Both failures
   report stage=1/error=6 and child exit 0xc0000142. x86 ordinary and compact
   each pass all 12 bounded repetitions. Stage 1 is the failed GetConsoleCP
   check after attachment attempts; it is not evidence that Broker restoration
   failed. A separate scratch copy using the unchanged fixture and shipping
   x64 EXE, Console mode and overlay media, passes odd attempts and fails all
   six even attempts out of 12 with the same loader exit. This also excludes
   CTest as a necessary trigger. No failure reached raw-input stage 14.
4. Post-boot raw input: current user INI selects Window. Both registered package
   names therefore take the Window branch; the compact name alone does not
   prove raw Console input coverage. The scratch Console configuration exercised
   the real raw route, including ver, in its six successful starts. There is no
   reproduced post-boot loss in this run and no evidence tying it to the loader
   failure. Startup failures remain failures, not waived raw-input passes.

Additional discriminating startup experiments:

- A process-local Win32 debug-event collector enabled loader diagnostics only
  in its debugged processes (no IFEO/registry change). Six direct original
  package-fixture executions and 12 original CTest executions all pass under
  that instrumentation. Thus the failing initializer is still unidentified;
  debugger timing is not an adequate negative proof. DLL export/probing warnings
  also occur in successful runs and are not assigned as the root cause.
- An independent native program creates a child Console, attaches, waits for
  a named event proving child main was entered, detaches and terminates its own
  child. Twenty repetitions pass. Adding the same six native DLL imports as
  SoftPC passes another 20; adding immediate Console hiding passes another 20.
  This does not reproduce the full product startup and cannot exonerate all
  attachment timing, but disproves an unconditional failure of those operations.
- No matching Application-log failure record identified a DLL. No source-level
  TLS/DllMain implementation was found in the repository. The loader status
  narrows the phase; it does not establish a particular module or OS defect.

Source audit also confirms package_screen_contains reads viewport area as a
linear buffer count, although native linear reads wrap at backing-buffer width.
For differing widths this is not a rectangular viewport oracle. This is a
real fixture limitation, but stage 14 uses package_buffer_contains instead;
it does not explain the recorded raw ver failure. Keep any repair test-owned.

The raw fixture's first scratch link omitted existing Base clock/sleep symbols
and failed; linking the existing cached Base archive fixed the probe build.
No source was rewritten to obtain a pass. All reported runtime counts above
are from the linked probe. This run is research, not a fresh full qualification.

Recommendation: do not alter shared lifecycle, pause or Console recovery paths
on this evidence. The next bounded step should capture the specific initializer
on the uninstrumented failing startup and name the exact command wait under
aggregate load. There is no justified minimal production fix yet. Preserve
separate dispositions for live delayed prompts, exit 1, exit 0xc0000142 and
post-boot input; only the exit-1 native-endpoint chain is explained here.

### S4 continued failure-site evidence

These are additional active-research observations, not a completed repair or
full root-cause closure. Temporary fixture diagnostics and task-local probe
objects remain outside any downstream import baseline.

- The real command-provider first execution plan timed out again. A debugger
  attached only after its ten-second wait failed. Its executor stack is in
  `ccpu`, `c_main.c:4065`, the `RESUME_HALT` checkpoint loop. The live debug
  state is TRACE, target_count=3, executed=0, stop_pending=false,
  result_ready=false, in_instruction=false; quick_mode=0 and interrupt_map=1.
  The test's preceding requests set CS:EIP=0:500 and EFLAGS=2. Thus the CPU
  retained its pre-pause HLT continuation while debug changed the next PC and
  cleared IF. The HLT loop does not reach the ordinary debug begin/retirement
  hooks; neither its enabled-INTR nor RESET exit condition is satisfied.
  This explains this reproduced PAUSED timeout, not every historical wait.
  It is not an event-wait deadlock or an undersized ten-second budget.
- Separately, x64 trace with guest TF set sometimes returns IP=702 rather
  than taking INT1 to 700. The local debug-begin hook jumps to NEXT_INST
  without clearing quick_mode; the original resampling code only sets it
  true when eligible, relying on earlier callers to clear it. A task-local
  counterfactual clears it before recomputation: 60 iterations pass and one
  records stale quick_mode with TF set at IP=701. This is evidence for a port
  hook violating the original quick-path invariant, not proof that it also
  repairs the distinct HLT continuation problem. No mirror change is made.
- Parallel x86 command-provider also raised an access violation before the
  execution-plan section. Fifty runs under a debugger pass. Its root remains
  unproved; debugger-altered timing is not an exoneration.
- The unchanged x64 package fixture reproduced stage 1 with a still-live
  child, not just an exited child. Post-failure attachment shows the sole
  ordinary thread in ntdll's loader initialization failure path, invoking
  ZwRaiseHardError with STATUS_APP_INIT_FAILURE (0xc0000145) and parameter
  STATUS_DLL_INIT_FAILED (0xc0000142). Only ntdll is in the loaded module list.
  Matching Microsoft ntdll symbols identify LdrpInitializationFailure,
  LdrpInitialize, LdrpInitializeInternal and LdrInitializeThunk. This proves
  that this live/no-Console sample is a native initialization failure before
  SoftPC main, not application monitor delay. Which earlier native operation
  returned the error still requires evidence. Merely extending attachment
  timeout is not a demonstrated correction.

Further native evidence narrows this last case: the failed child's existing
LdrpSnapsUnicodeString log explicitly reports KernelBase.dll's process-attach
initializer returning failure, then Kernel32 load failure, then process
initialization failure. Read-only observation catches BasepInitializationStage
at 600. Matching KernelBase symbols/disassembly place stage 600 immediately
before ConsoleInitialize; its false return exits the DLL initializer without
advancing to stage 700. Thus this sample's native Console initialization is
the failed operation, not native PCM or SoftPC main. VirtualQueryEx confirms
only EXE/ntdll remain mapped after loader rollback; an empty later DLL list
does not mean KernelBase was never mapped. The trigger of ConsoleInitialize's
failure remains under investigation. A task-local A/B keeps the same EXE and
fixture but moves the parent's FreeConsole before versus after CreateProcess;
results must be inspected, not presumed. KernelBase symbols match RSDS
79B505276F140450EB83DF0440BF06C6 age 1.

The FreeConsole-order comparison is now complete: the original order fails
one of ten starts, while releasing before CreateProcess fails three of ten.
Moving that call alone is therefore not a demonstrated fix. A separate
15-start diagnostic retains the parent's Console and does not AttachConsole
until the child advances beyond native Console initialization. Two starts
still fail at initialization stage 600 with the same KernelBase initializer
failure. This rules out early parent detach/attach as a necessary trigger in
these samples. Its later stage-3 prompt report observes the retained parent
Console, not a running child; the native loader evidence is the useful result.
The failed primary thread's final LastError is 6, but that stale value alone
does not identify the originating native API. A minimal Console child that
immediately returns, with no SoftPC code or attachment, passes 60/60 starts;
this does not reproduce the package lifecycle or exonerate its native path.
Raw compact-input experiments now use a private overlay configuration so the
real raw route is exercised without changing the user's Window configuration.
Those results remain pending; no passing Window route is counted as raw input
coverage.

The unchanged raw compact fixture now reproduces the historical post-boot
failure: attempt 47 reports stage 14, a live child, and a fresh DOS prompt
with no characters from `ver`. Of these 47 attempts, 39 pass the complete
route, seven fail native startup, and one fails post-boot input. This is a
separate reproduced failure, not a window-mode result or a guessed input
cause. A subsequent task-local fixture attaches the debugger only after
failure to capture the actual blocked threads. The private package executable
hash matches the shipping executable; only its private configuration selects
raw Console with overlay media.

An additional minimal native-only experiment reproduces parent detach,
child Console attachment, hiding and forced child termination, but passes
60/60 cycles. Along with the immediate-exit child experiment, this shows that
these simplified lifecycles have not reproduced the package initialization
failure; it does not prove that native initialization is sound under the full
package lifecycle. Separately, 500 consecutive real provider resume/debug-pause
cycles complete without access violation; the following instruction-plan wait
times out again. The access violation still lacks a captured faulting stack.

The 60-start raw post-failure-stack experiment completes with 40 successful
routes and 20 native-initialization failures, but no new stage-14 input failure.
This does not supersede the reproduced stage-14 sample above. Input-chain
diagnostics now instrument task-local copies of the broker, KVM Console,
Session and App keyboard adapter. Their private link reuses existing objects,
does not replace shipping executables, and writes only to task scratch. A
lower-perturbation variant records fixed-size in-memory observations and reads
them only after failure, avoiding per-event file I/O. It also records original
keyboard-controller disable, scanning and buffer facts; no controller behavior
or production input routing is changed. Results remain pending.

The file-observation input-chain experiment reproduces stage 14 on attempt
80. PID 16656's raw reader observes only Enter release, not the three letters
or Enter press. The adapter decodes that release correctly; the unmatched
release is not delivered as a guest make. No App keyboard delivery occurs.
The native reader subsequently waits for input, Session waits for its queue,
and the guest executor waits normally in BIOS keyboard idetect. Thus this
sample loses the command before Broker native key delivery, not in Machine
FIFO, Session admission or the guest keyboard adapter. The native cause
(flushing, another reader or host input handling) remains to be distinguished.

A native-only controlled experiment now reproduces the exact missing-input
shape without SoftPC, Session or Machine. It starts a cooked ReadConsoleW,
cancels through the two existing native cancellation calls, joins the reader,
flushes input, switches to raw mode and successfully writes all eight records
for `ver` plus Enter. On cycle 470, only Enter release remains. A fresh repeat
reproduces on cycle 388. In both failures ReadConsoleW returned TRUE with zero
characters and ERROR_OPERATION_ABORTED; the reader thread had exited before
new input was written. Thus joining the application reader does not prove the
native cancelled cooked-read state can no longer consume future input.

The counterfactual removes only the two cancellation calls, retaining the
existing terminating-key wake, join, flush and raw transition. All 10,000 cycles
retain all eight key records. This is dynamic causal evidence for cancellation
in this native probe, corroborating the real product's pre-Broker loss. It is
not qualification of a permanent broker repair or every terminal implementation.
The native observation is consistent with Microsoft's public Terminal issue
[17791](https://github.com/microsoft/terminal/issues/17791), which reports a
successful cancelled ReadConsole and discarded first subsequent input. The
independent local experiment, not that issue alone, supplies the evidence here.
Per-call isolation remains useful before choosing the smallest correction.
That isolation now also reproduces with CancelIoEx alone (cycle 220) and
CancelSynchronousIo alone (cycle 203), each leaving only Enter release.
Replacing one cancellation API with the other is therefore not a supported
correction. The existing terminating-record/join route is the useful minimal
candidate; its product-level output/editing behavior still needs qualification.

A one-shot debugger observes ConsoleInitialize's ConsoleAllocate return in
40 private package children. Every observed status is success; each child is
then terminated before product execution. This deliberately limited experiment
does not reproduce the native initialization failure or the full package
lifecycle, and cannot close that root-cause item. All these diagnostic children
and the cancellation isolation/control jobs have exited.

The x86 fixture's exact prefix through its first resume and second pause now
runs in 1,000 fresh processes. All pass; later attempts overlap an eight-way
Machine/unit stress group. The prefix retains its original register/memory,
port, command-open/close and pause calls, then uses ordinary owner teardown;
only subsequent execution-plan/snapshot sections are omitted. This separates
the historical access violation from the known later HLT/TF assertions, but
does not prove the violation repaired. Source review confirms debug cancellation
is queued atomically and applied on the executor, not a direct frontend write
to the driver's debug state. No unsupported shared-state-race claim follows.
The supporting stress group remains active until its bounded repeats finish.
Read-only System log inspection finds the native 0xc0000142 popup records but
no Win32k or Resource-Exhaustion event in the preceding twelve hours. This is
not proof that resource pressure is impossible or the initialization root cause.

The startup debugger now retains the actual complete package fixture lifecycle
instead of terminating immediately after successful native initialization.
Its second start observes ConsoleAllocate returning 0xc0074005; subsequent
starts reproduce the same return. The child never reaches main in those
failures. This provides the first live originating status beneath the loader's
generic 0xc0000142. A serialized follow-up observes the individual allocation,
server-launch, connection and standard-I/O returns in the same native chain;
its results must be checked before attributing that status to a particular call.
The one-shot breakpoints affect only private diagnostic child memory and are
removed before each observed instruction executes; no DLL on disk is changed.

The complete-lifecycle experiment finishes 40 starts: 22 native allocation
successes and 18 failures with 0xc0074005. The serialized deeper observation
then captures one success and one failure. In the failing child, both initial
NtCreateFile operations and ConsoleLaunchServerProcess return success;
ConsoleCreateConnectionObject returns 0xc0074005, propagated unchanged through
ConsoleAllocate. Matching local disassembly shows the connection routine
obtains its failure from its NtCreateFile request; allocation failure would
instead return 0xc000009a. Standard-I/O creation is never reached in that
sample. This establishes the failing client/server connection operation before
main, not why the native server rejects it. No product retry or timeout change
is justified by this evidence. Both native diagnostic jobs have now exited.

The supporting x86 eight-way repeated Machine/unit group completes in 609.15
seconds: 30 of 31 registrations pass; machine_smoke fails its existing
run_int1a_tick_boot_image assertion bda_tick_low != 0 (exit 0xc0000409).
This is the already-recorded host-time oracle debt in TODO, not a new access
violation: the fixture repeatedly executes INT 1Ah, whose original time_of_day
overwrites TIMER_LOW from get_host_timestamp. A zero low byte is legal.
The failed timestamp was not captured, so no timer-delivery regression is
inferred and that test-only repair remains outside this S.

The HLT finding also requires a contract distinction: Product UI explicitly
preserves HLT until an interrupt. The fixture resumes firmware before pausing
and installing its instruction-plan program, so a retained HLT continuation is
a possible entry state. Editing PC and clearing IF does not itself provide an
architectural interrupt. The timeout is explained; automatically clearing HLT
in production is not yet an approved or demonstrated correct repair. A repair
must distinguish deterministic fixture setup from any separately intended
debugger override of halted CPU state.

The x86 first-chance exception-observed prefix adds 200 successful fresh runs.
No access violation is observed; neither a captured crash nor a repair is
claimed. The debug cancellation call-chain review remains negative evidence,
not a substitute for the missing fault site.
This 80-attempt group has 61 complete passes, 18 startup failures and this
one input failure. Its private executable is instrumented; it is not a claim
about unmodified production timing. Shipping hashes remain unchanged.

The loader symbol file is the public Microsoft ntdll.pdb matching RSDS
C031D45B0DA18A6E74D06ED6AB2DD9E1 age 1. Symbol lookup corrects GDB's nearest
export names; no OS registry, system binary, downstream source, user INI or
guest media is changed. The documented status meanings are in Microsoft's
[NTSTATUS reference](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55).

## S3 implementation and verification record

Baseline 22cdeb7f. Production C changes only session/control.c: +1/-3,
net -2. The existing control_reconciler test adds five lines, net +5.
No ABI, state, thread, queue, driver callback or resume synchronization is added.
Only src/emulator and test/emulator change among the public eight trees;
their manifests and the architecture/contract documentation are synchronized.
The other six trees, Machine implementation, App, mirror, INI and media remain
unchanged. Dual published EXEs are rebuilt from this implementation.

Similar-issue sweep inspected accept_kvm_event and dispatch_input for every
KVM event type, plus Session callers and existing Machine/physical-key tests.
SOURCE_RETIRED and INPUT_RESET already enter before state/generation filtering;
their ledger cleanup remains necessary. Paused key-up also clears the local
ledger without guest delivery. Paused mouse has no ledger or dispatch action;
its redundant admission is removed. Hotkeys and close retain control semantics.
Machine admission/FIFO and KVM filtering remain their existing single owners.
No additional gate or abstraction is warranted for these two predicates.

Cached x64/x86 builds each complete 23 incremental steps, no warning/error
matches in their logs. Focused Machine/Session tests pass 14/14 per width.
Full background runs pass 184/184 each (113.70s x64, 114.62s x86).
Native tests were held while the owner session was disconnected, then executed
after reconnection: audio 3/3 and desktop/package 5/5 on each width, no skips
or failures. No native test was attempted during the disconnected interval.
Frozen qualification universe: all 192 registered CTest names per width.
Exact registration/XML name equality across background/audio/desktop proves
192 unique passing cases per width, zero missing/extra/failed/skipped cases.
This proves this build's regression result, not repair of historical S4
intermittency or acceptance by other products. No downstream repo was read.

Published SHA-256:

- x86: EB924BBA391EB0F88E989EBC9A7F7CDCC4331B8D1D124296474BF2678DD0937E
- x64: AFA8D42BC1C3C0B21DCBD30527C4D6DCC054E40914946F74E575700416A88D94

Self-review confirms the two removed predicates had no paused guest-delivery
effect. The new assertions retain retirement acceptance and distinguish paused
mouse rejection from running admission. Necessary ownership and error handling
are unchanged. S3 waits for owner acceptance after delivery and committed-diff
review; S4 is not admitted.

Coordinator review of pushed c4a624ee confirms the actual code/test diff matches
the approved two-predicate cleanup, no competing input path or dependency was
added, and both published binaries match the recorded hashes. Manifest,
documentation, whitespace and build-residue checks pass. Owned jobs exited;
disposable S3 scratch was removed after evidence promotion, caches retained.
S3 is verified awaiting owner acceptance, not closed or advanced to S4.

## S2 implementation and verification record

Baseline bf18a2f7. Production cleanup changes only
`src/lib/console-broker/console.c`: +4/-18, net -14. A successful deactivate is
followed by reset; failure of either enters the single existing broken-binding
cleanup. The original error is retained, reset is not attempted after failed
retirement, and lock release/output unbinding/reference-release order is
unchanged. Activation rollback remains necessary and untouched.

Tests: `test/lib/console-broker/console_broker_smoke.c` +27/-14 and
`test/app-softpc/integration/command_provider_smoke.c` +0/-1: total +27/-15,
net +12. The existing retirement-failure assertions run for both retirement
and old-input reset failure. Added assertions prove the fault was consumed,
next activation was not attempted, and a second replacement is refused.
No test-only production entry or extra test framework is introduced.

Similar-issue review: all `wait_for` callers in command_provider_smoke use its
three completion handles or program_completed; all four are auto-reset events.
No remaining ResetEvent call exists in that fixture. Broker's prepare,
allocation, activation/rollback and notification failures are distinct phases,
not duplicate copies of the retired-binding cleanup; they remain unchanged.
No adjacent Session change is included. Public changes are limited to src/lib
and test/lib with their manifests; other six public trees are unchanged.

Qualification is complete for the S2 scope, with the supplements below.
Cached builds completed on both widths (28 incremental steps each), without
compiler warnings in either build log. x64 focused 4/4 and background 184/184
pass. The initial x86 focused run is 3/4: command_provider_smoke reaches debug
resume (completion 2) then fails its unchanged 10000ms wait assertion with
0xc0000409, while all three Broker tests pass. This is the historical symptom,
not proof of an identical root cause or a repair; S4 retains investigation
ownership. No code or timeout was changed for the supplemental run.
The x86 full background run passes 184/184, followed by focused 4/4. Both
background XML sets exactly match registered names, with zero failures/skips.
Background elapsed: x64 125.76s, x86 123.23s. Both desktop groups were attempted
while the owner session was Disc: three Lib cases pass on each width, but both
App package routes on each width fail stage 3/error 0 waiting for the initial
prompt. This records correlation with session state, not causal proof. Desktop
elapsed: x64 69.81s, x86 69.18s; group exit 8. These failures are not waived and
do not constitute complete qualification. All launched jobs have exited.

After owner reconnection, session query confirmed Active. Connected package
results: x86 2/2 (24.62s); x64 ordinary passes, compact fails stage 1/error 6
(17.32s group). An unchanged isolated x64 compact run passes in 12.41s. No
code, fixture budget or product behavior changed between these attempts. The
earlier command wait and package failures remain S4 evidence, not repaired by
this cleanup. Passing JUnit names combined with the original three successful
Lib desktop cases and background results exactly cover 189 registered cases
per width excluding native_audio, without skipped cases in the passing set.
This is supplemented qualification, not an all-green first run. Three unrelated
native-audio cases per width were not rerun under this bounded S2 packet.

Published build hashes:

- softpc32: F657EC43BADB3BE205C837A0605376F2F0E29EC0F863298712AE9D0C786A63E4.
- softpc64: B5E940DC7F101AF8D484C2E6FE272114C8A39AA58EBE13355944FD722BC5E362.

INI/media, other six public trees and mirror are unchanged. Source/test diff
review preserves a single fail-closed path without new state or interface;
combined production and test C net is -2. Manifest, documentation and whitespace
checks pass. All owned jobs exited; concise evidence is retained here and task
scratch is retired before delivery. No ABI, platform dependency, lock order,
queue, retry, timeout, guest behavior or native rendering path is added.
Executor delivery is followed by actual-commit coordinator review; S2 awaits
owner acceptance and neither S3 nor S4 is automatically admitted.

## Observation ledger retained

### S2 owner-requested fresh full regression

On 2026-10-22 the owner expands verification to every registered test, including
native audio. Baseline 27946581; no source, binary, INI or timeout changes.
The frozen universe is all 192 registrations per architecture, partitioned into
184 background, five desktop and three native-audio cases. All six groups ran
to completion; XML names exactly match registration, with no missing, duplicate
or skipped cases. No older result substitutes for this run.

- x64: background 184/184, desktop 3/5, audio 0/3; total 187/192.
- x86: background 184/184, desktop 3/5, audio 0/3; total 187/192.

Both widths fail the same five names: app-softpc/integration/package_smoke,
app-softpc/integration/package_smoke/compact_console,
app-softpc/integration/audio_lifecycle_smoke,
app-softpc/integration/sb_midi_native_smoke and
lib.integration/native_midi_output. x64 package failures are stage 1/error 6;
x86 package failures are stage 3/error 0. Audio failures are the existing
assertions for PCM startup or native MIDI output opening, not skipped capability
checks. Command-provider passes in both fresh background groups.

Session was Active at job launch and Disc at the subsequent audio-failure
inspection. This is recorded environmental evidence, not proof that every
failure has that single cause. Full execution is complete but full passing
qualification is not. Owner has been asked to reconnect for the five failing
cases on each width. No disconnected retry, deadline change or production
repair was made. Prior supplementary passes remain historical evidence only.
The owned job exited with result 1; results are promoted here before scratch
retirement. S2 remains awaiting complete qualification and owner acceptance.

These five observations originate in TODO, not five established public bugs.
Owner explicitly transfers all five entries out of TODO into this S. Their
unresolved status and historical evidence are retained below, not marked fixed.
This proposal is their sole investigation record; no duplicate TODO or queued
candidate remains. QUEUE contained none of these five entries; its separate
product-performance candidate is unchanged and outside this admission.

1. Paused key release/source retirement. Session control removes host pressed
   records while PAUSED without delivering release to Machine. Source behavior
   is confirmed; an end-to-end stuck-key reproduction is not. Emulator Session
   is the direct public owner to inspect. Distinguish host observation, FIFO
   acceptance and actual driver delivery. S15's frozen FIFO is accepted and
   is not authority to add pause-release exceptions or restore-time bursts.
2. Command-provider completion wait. T98 S7 observed a 10000ms assertion after
   debug resume/pause in the App integration fixture; unchanged follow-up
   passed. No Window is created. Trace X86 IBMPC command handling, Emulator
   request/completion and actual driver safe points. Identify the exact missing
   completion before blaming RDP, Machine or test event consumption.
3. Console attach/prompt failure. Package tests have observed empty live
   buffers, missing prompts and machine-composition exits, stages 1/error 6
   and 3/error 0. Some occurred while Active. A 30-second startup budget fixed
   one observed slow-prompt case, not all failures. Inspect Lib Console/Broker,
   Emulator Product and App fixture boundaries without assuming one root cause.
4. Compact Console input loss. T93 S12 reached DOS but failed stage 14/error 0
   after injected ver; unchanged rerun passed. Distinguish native injection,
   Broker read, KVM decoding, Session/FIFO acceptance and guest consumption.
   Startup failure and post-start input loss are separate observations.
5. Native child initialization failure. Earlier package probes observed
   0xC0000142, AttachConsole error 5 and subsequent error 6. This may explain
   a subset of observation 3, but the responsible DLL/path is unproved.
   Investigate jointly with startup ordering; do not treat downstream Console
   errors after child death as proof of a Console implementation defect.

All five require a disposition: confirmed public defect, removable redundancy,
necessary behavior retained, product/fixture/host-owned, or unresolved with
specific missing evidence and receiver. Completion means all five have honest
dispositions, not that all intermittent failures were reproduced or fixed.

## Three responsibility reviews

### Input ownership

Inventory existing Session/KVM ledgers and Machine FIFO, their writers,
readers, locks and teardown. Separate their distinct necessary purposes from
genuine duplicate authority. Trace press-before-pause, release-during-pause,
reset/retirement, genuine resume, capture continuation and stop/reset.
First evaluate correcting/removing contradictory accounting branches. Do not
add a second ledger, release queue, epoch or product callback. If a coherent
solution needs changed admission semantics, report the tradeoff before coding.

### Command completion

Trace request acceptance to executor action to completion publication to the
waiting consumer. Check whether status reads or extra waits duplicate a known
completion fact, but do not assume all state reads are redundant. Preserve
necessary request identity and synchronization. A wrong test wait should be
fixed in its owner, not compensated by a public retry/acknowledgment protocol.

### Console ownership

Use the existing Broker as the native handoff owner. Identify actual callers
of activation, display restore and input-reader handoff. Seek duplicated upper
layer interference or a missing operation in the one existing path. Do not add
replay, redraw, reattach loops or recovery state to mask child startup failure.
Combine observations 3 and 5 where evidence supports it; keep 4 distinct.

## Decision and evidence standard

For each candidate report actual user impact, source/call locations, one
responsible owner, the current sequence and synchronization, any duplicate
state/branch, the smallest alternative and a no-change alternative. Estimate
production/test additions, deletions and net size separately. Explain the
number of states, owners and paths removed, not merely line-count savings.
Necessary memory/resource safety and explicit errors must remain.

Use existing owner-local tests and bounded real-path probes to discriminate
hypotheses; no fixed-sleep proof, test-only production API or replacement
implementation. Record failure stages, session state and whether a run is a
repeat. Passing later does not erase earlier failure or prove repair.
Do not rerun the full suite merely to make a source-only review appear complete;
name exactly what was exercised and what was not. Follow background execution
and scratch retirement rules for any potentially long probe/test.

If the only proposed gain requires new compensation state for an unconfirmed
edge case, recommend no change or explicit deferral. Zero production diff is
an acceptable result. Shared layer dependence and lib/types authority remain
unchanged; broad shared-library cleanup is outside this S.

## Exclusions and delivery

The separate queued performance T owns C-VID lookup, product mouse/graphics
conversion, CPU hot-path checks and snapshot I/O. Original VGA/SB/BOP faults,
Compat physical-memory adapters and Ninja corruption are not folded into T99.
X86 Debug is reviewed only where the admitted command path actually reaches
it; do not manufacture changes to touch all eight trees.

Deliver the five dispositions, exact responsibility maps, concrete proposed
deletions/changes and minimal verification plan for any recommended repair.
Only return unresolved work to TODO on explicit deferral with a receiver;
do not keep duplicate active and deferred records. Complete
actual-diff self-review and coordinator review under the single-person mode,
documentation checks and one complete research delivery before acceptance.
This is not a planning-only implementation P or an automatic repair admission.

## S1 research delivery (2026-10-22)

Reviewed repository baseline: 4bc1ff35; runtime remains edb91df1. This is a
bounded review of the three named handoffs, not a whole-library correctness
certificate. No production or permanent test change accompanies this report.

### 1. Input: different records, not interchangeable copies

`src/lib/kvm-base/hotkey.c` owns source-local pending/consumed/delivered-to-sink
keys for chord recognition. `component.c` discards that matcher on input reset
or retirement and emits the corresponding event. It does not synthesize every
guest break. `src/emulator/session/control.c` owns source-key records used to
synthesize those releases while running. Machine's `input_queue.c` owns
accepted events, with admission and push ordered by the same mutex. Only the
Machine executor delivers them to the driver. None of these records is a
guest device's held-key state.

The concrete sequence is:

1. KVM worker posts copied events to Session; Session's control thread alone
   updates its source ledger.
2. Session dispatches to `deliver_machine_input()` in `session.c`. That adapter
   treats both acceptance and INVALID_STATE refusal as successfully handled;
   other failures remain failures.
3. Machine queue mutex orders push against the pause cutoff. S15 freezes
   existing FIFO contents, closes admission, and publishes PAUSED from the
   executor. It does not drain the queue. Genuine resume reopens admission;
   state-read continuation does not.
4. A release/reset seen while Session believes the machine PAUSED removes its
   record without calling the Machine sink. On resume there is no retained
   Session release to send for that record.

Observation 1 is therefore a confirmed accounting/semantic gap, not a newly
reproduced guest fault. Existing `physical_key_identity_smoke.c` explicitly
asserts that paused reset forgets the record without another break; the focused
run passes that assertion. `control_reconciler_smoke.c` also tests suppression
of paused guest input. The problematic state is therefore currently deliberate
test behavior, not an uncovered accidental branch alone.

Do not delete either ledger: removing KVM's loses chord/prefix behavior;
removing Session's loses source-wide cleanup after matcher discard. Moving
`remember_pressed` after a boolean sink call is insufficient: handled refusal
still returns true, and successful enqueue is not driver delivery. Session's
queued completion facts can also lag the actual Machine admission boundary.

Recommendation: retain the accepted frozen-FIFO contract in this research
delivery. Before repair, explicitly choose the semantics for a release that
occurs during pause: retain an ordered release obligation until genuine resume,
or permit a defined paused release operation. Both change present behavior;
neither is authorized merely by calling it cleanup. Do not introduce a second
ledger, special product callback or restore-time release burst. A minimal repair
estimate cannot honestly be fixed until that choice and source-retirement
ordering are established. Current recommended production/test diff: zero.
Receiver remains T99 input-semantics investigation, not a duplicate TODO.

One independent redundant predicate exists in `accept_kvm_event()`: PAUSED tests
SOURCE_RETIRED again after the early unconditional acceptance. Removing that
single line is safe simplification, but does not repair observation 1 and is
not sufficient reason to reopen the input architecture.

### 2. Command completion: preserve one fact, remove redundant fixture reset

`src/x86-ibmpc/product/command.c` translates debug lifecycle requests to Session
requests and delegates completed-runtime reporting to the Emulator monitor.
It does not independently complete a pause. Machine `machine.c` publishes
PAUSED from its executor after closing input and stopping heartbeat; genuine
resume publishes RUNNING. Internal state-read continuation suppresses a false
user pause/resume pair. Session reduces copied completed facts; its desired
state is a request, not a second completed-state authority.

The affected App integration fixture directly drives Machine and feeds provider
runtime notifications; it does not exercise the complete Session run loop.
`command_provider_smoke.c:wait_for()` waits for an auto-reset event and then
calls `ResetEvent()` again. All its completion handles, including
`program_completed`, are created with manual-reset FALSE. The second reset is
unnecessary and could erase a signal arriving between successful wait and reset.
This proves a removable operation, not that such an interleaving caused the
historical debug timeout. Its exact missing event remains unidentified.

Recommendation: App-owned fixture cleanup, production +0/-0, test +0/-1.
No new completion ID, acknowledgment, retry or public event model. Verify all
wait callers and rerun the fixture on both widths. If the original failure
recurs, record the particular wait and preceding executor completion before
changing shared code. Observation 2 remains unresolved; receiver is the App
command-provider fixture/executor boundary. This run passes unchanged on both
widths and does not reproduce the historical aggregate failure.

### 3. Console: one owner already exists; one duplicate cleanup is removable

`console_broker_replace()` is the native handoff transaction owner. It prepares
the next binding, takes the output gate, retires/joins the old reader, resets
old source input, invalidates the old binding, activates the next binding, then
commits current identity/generation. Old output bindings are removed after the
output gate is released so in-flight writers can settle. Failed activation
restores the old object through the same activation helper; failed restoration
marks the broker broken rather than claiming a usable binding.

The transaction lock serializes replace; the output lock prevents concurrent
native output during the switch; generations reject stale writers. These are
different obligations. `emulator/ui/ui.c` calls replace and emits a completed
handoff only on success. Session's broker completion and Lib's activation
notification have different consumers: control/prompt sequencing versus render
activation. Neither is evidence for a second native restoration path.

In `src/lib/console-broker/console.c`, adjacent deactivate-failure and
reset-bound-failure branches duplicate the same broken-state/output-binding
cleanup. The minimal alternative is:

```c
status = console_broker_backend_deactivate(...);
if (status == LIB_STATUS_OK)
    status = console_broker_reset_bound(...);
if (status != LIB_STATUS_OK) {
    /* one existing broken-state cleanup block */
}
```

Projected production +2/-15 (approximately net -13, final comment placement
affects count); no new helper, state or API. Existing broker fault tests are
the starting point; budget test +0 to +15/-0 only if reset-failure assertions
need an explicit counterpart. This is cleanup, not a claimed fix for package
attach/input failure. Keep the old-binding restoration path: it is necessary
to preserve the replace failure contract, not a second renderer.

Native deactivate and activate both flush input at different boundaries:
retired-reader cancellation residue versus records arriving before new-reader
activation. Do not remove either merely because the same API occurs twice.

### 4. Package observations 3, 4 and 5: improve attribution, not Lib recovery

`test/app-softpc/integration/package_smoke.c` retries AttachConsole within its
fixture deadline without checking child exit in that loop. Stage 1 is assigned
by the subsequent code-page query, so its error alone cannot identify the
original startup fault. A historical 0xC0000142 is distinct from a live blank
buffer and from an explicit machine-composition failure. Observations 3 and 5
share an evidence boundary, not a proved common root cause.

The compact route sees DOS, hides its Console again, injects `ver`, and waits
for Version. Stage 14 combines injection failure and successful injection with
missing output into one result. A visible prompt alone does not identify which
input boundary later discarded a record. Hide/focus, activation flush, KVM
decode, Session admission and guest delivery need discriminating evidence;
none is proved responsible here. Observation 4 stays separate from startup.

Recommended receiver for all three is the App package fixture/native handoff
investigation. No shared production change is justified yet. Minimal next
probe: record child liveness/exit and first attach error, distinguish injection
return from output timeout, and timestamp existing activation/read/admission
boundaries only if needed. Estimated temporary fixture diagnostics +15 to +30
lines, production zero; remove temporary traces after evidence promotion.
No retry of startup, extra Session redraw, larger product timeout or recovery
manager. Do not combine these observations into one purported Console bug.

The visible-screen helper also uses a linear read count based on viewport area;
that is not a rectangular viewport read when backing stride exceeds viewport
width. Review its intended oracle before changing it; stage 14 uses the separate
buffer-text helper, so this is not evidence of its cause. This candidate remains
within the package-fixture receiver and is not an admitted test rewrite.

### Verification, scope and decision

Existing cached binaries, without rebuilding or changing source, were exercised
with CTest on both x64 and x86. Selection:
`physical_key|control_reconciler|command_provider|session_monitor|console_broker`,
excluding `desktop|native_audio`. Each width passes 7/7, zero failed tests:

- app-softpc/integration/command_provider_smoke;
- lib.console-broker/console_broker;
- lib.console-broker/win32/console_broker_reader_failure;
- lib.console-broker/win32/console_broker_cancel;
- emulator.session/control_reconciler;
- emulator.session/physical_key_identity;
- emulator.session/session_monitor.

Times: x64 1.02s, x86 1.19s; job exit 0. The first hidden launch left no results;
it is not counted as a test run. The subsequent logged launch completed. No
desktop/package/audio test, aggregate-stress reproduction, guest stuck-key
experiment or fresh compilation was performed. Source traces and cached-test
evidence are reported separately. Scratch is retired after this record.

All five observations have an owner and an honest disposition above; none is
declared repaired. The research recommends fewer duplicated statements, not
fewer necessary safety boundaries. Suggested next implementation is the tiny
Broker cleanup and auto-reset fixture cleanup, separately approved; input
semantics and package diagnosis remain research. No production/test code was
changed here (both +0/-0), no ABI or published artifact changed, and no downstream
repository was read. The task awaits owner review, not automatic repair admission.

## Historical evidence retained below

The following five former TODO entries are retained as source observations,
including their original admission conditions, now questions owned by T99 S1.
Moving them does not establish a fix or a public-component root cause.

### Observation 1: Audit key release across paused Session input. Existing
  session/control.c forgets host held keys on release/source retirement while
  PAUSED without delivering them to the driver. T98 S15 freezes Machine FIFO
  but does not turn the host ledger into a guest-delivery ledger or resolve
  device-held keys. Source ordering is confirmed; a complete user-visible stuck
  key reproduction is not claimed. Owner: Session/driver input ownership.
  Admission condition: reproduce make-before-pause and release-during-pause,
  then define neutral reconciliation without product callbacks, treating
  host-pending FIFO and already-delivered device input as different states.

### Observation 2: Diagnose the intermittent command-provider event wait failure.
  T98 S7's second x64 aggregate run failed the 10000ms event assertion after
  debug resume/pause observations; the first aggregate and unchanged isolated
  follow-up pass. This fixture does not create a Window; RDP is not an
  established explanation. Owner: App command-provider integration fixture/
  executor event boundary. Admission condition: identify the exact awaited
  event and producer/consumer ordering under aggregate execution; do not relax
  assertions or increase product timeouts. S7 does not claim this repaired.

### Observation 3: Diagnose package integration Console attach/prompt intermittency.
  T98 S6 observed stage 1/error 6 and stage 3/error 0, including after RDP
  reconnect. Unchanged final tests later pass on both architectures; this is
  passing execution evidence, not a repair or proof of environmental cause.
  Owner: App package fixture/Console integration boundary. Admission condition:
  capture child exit, active buffer identity and cursor at the first failure
  against the unchanged baseline; do not weaken prompt assertions or add retries
  as a production fix. Temporary diagnostic code was removed. T98 S7 also
  observed a live blank initial buffer and, in a separate attempt, child exit 1
  with cannot compose machine. Some attempts failed even after RDP was Active;
  later unchanged package tests pass. These are distinct observations, not
  proof that all stage-3 failures share one cause.
  T98 S8 also observed these failures while Active: a live child with blank
  31x30 buffer/cursor 0,0, and a separate pre-attach exit 0xC0000142. Ordinary
  package subsequently passed on both architectures while compact failed.
  The temporary diagnostic was removed; no causal repair is claimed.
  A subsequent S8 probe proved one live child reached the unchanged prompt
  another 5157 ms after the five-second check. The startup-only test budget
  was corrected to 30 seconds, without a retry or product timeout change.
  This does not explain the separate initialization exits or prove that all
  attach/prompt failures are timing-budget failures.
  T98 S9 repeats stage 1/error 6 in compact package on each architecture while
  RDP is Active; ordinary package and all other cases pass. Each unchanged
  isolated compact rerun passes. No fixture/product change or causal repair
  is claimed; retain both first failures and supplemental qualification facts.
  T98 S10 final x86 compact package again failed stage 1/error 6; an unchanged
  complete desktop-group repeat passed 5/5. No causal repair is claimed.
  T98 S13 x64 compact package repeats stage 1/error 6 while RDP is Active;
  its unchanged isolated repeat passes. x86 passes all desktop cases first run.
  S13 does not claim to repair this existing package-initialization debt.
  T98 S15 x64 repeats compact stage 1/error 6 after reconnection while Active;
  the unchanged isolated case passes. All other desktop/audio cases pass.
  This supplemental qualification does not establish a repair or root cause.

### Observation 4: Diagnose intermittent package compact-Console raw input loss.
  T93 S12's first desktop x64 compact case reached DOS but timed out at
  stage=14/error=0 waiting for Version after injected ver; the captured screen
  contained only C:\>. Normal package passed immediately before it. A complete
  unchanged ten-case desktop rerun passed both widths. This differs from the
  stage=1 child-startup debt below and does not prove the cause or a repair.
  Owner: App-SoftPC package-test/input handoff. Admission condition: bounded
  native input/activation instrumentation around first raw command and hide/
  focus transition; do not add retry or relax assertions. See
  [S12 verification](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s12-implementation-and-review-record).

### Observation 5: Diagnose native package-test child startup under sequential
  CTest execution. T93 S5 x64/x86 compact-console cases intermittently fail
  before start at stage=1/error=6. A scratch diagnostic reproduces child exit
  0xC0000142 (DLL initialization failed), AttachConsole error 5, then code-page
  query error 6. Direct native/package probes pass; this does not identify the
  responsible DLL or prove a CTest defect. Admission condition: test-owned
  child-startup/Console lifetime investigation with loader evidence, without
  retrying product startup or changing guest/device behavior to hide it.
  Evidence: [T93 S5 checkpoint](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s5--original-dma-prerequisite-repair).

### S2 owner-authorized failure investigation, after fresh full regression

The owner requests repair and full passing verification. This expands S2 to
investigate the five failing native registrations; it does not admit pause
semantics work or claim the historical intermittent failures repaired.

An independent system-API probe, in the disconnected owner session, reports
`midiOutOpen(MIDI_MAPPER)=1` (Undefined external error), with one enumerated
device, and `GetDefaultAudioEndpoint=0x80070490` after successful COM setup.
Enumeration is not usable-device proof. No Lib status is changed to conceal
these failures and no native integration is skipped.

The package fixture had a real diagnostic defect: after its child exited it
continued waiting for a prompt, reporting stage 3/error 0 with no screen.
The existing attach/prompt waits now also observe process exit; failure captures
the final buffer before detachment and reports the child exit code. This adds
no retry and leaves successful-path assertions and budgets intact. Both widths
compile without warnings. In the same disconnected environment all four package
executions now terminate in approximately 0.7-0.8 seconds, reporting child exit
1 and `softpcvm: cannot compose machine`, instead of waiting 30 seconds.
This proves accurate failure classification, not successful product startup.
Source inspection places native PCM acquisition in driver creation before
monitor composition, consistent with the absent endpoint; it does not identify
the separate historical 0xC0000142 loader failure.

Follow-up test C diff is +11/-3 (net +8), no production or EXE change.
At this checkpoint real-host verification was blocked on an active session;
it was not delivered as a completed P. Subsequent evidence is recorded below.

On the owner's subsequent reconnection, direct native acquisition succeeds:
MIDI open returns 0 and default PCM endpoint lookup returns S_OK. The new
complete run uses the existing cached dual builds (both report no work).
Both widths again execute exactly 192/192 registered names with no skips:
184/184 background, 3/5 desktop and 0/3 native audio. Background durations
are 117.49 seconds x64 and 112.26 seconds x86. The session is recorded Active
at launch but is Disc when the x64 native failures are inspected and remains
Disc at completion. Both package tests per width report exit 1 and cannot
compose machine. This is not full passing qualification; reconnection with
the session held active is required for the eight native cases per width.
Their next invocation is ordered before any further background suite.

### S2 P4 connected verification and review

The owner reconnects and requests retry. Both native groups pass serially on
both widths, unchanged code and budgets: x64 audio 3/3 in 41.21 seconds and
desktop/package 5/5 in 31.20 seconds; x86 audio 3/3 in 41.25 seconds and
desktop/package 5/5 in 31.36 seconds. No skip or individual-test retry occurs
inside this connected invocation. The preceding disconnected failures remain
above; this is supplemented qualification, not a repaired loader-race claim.

The frozen universe is the 192 CTest registrations per width. Final evidence
combines the latest full-run background XML (184 cases) with connected desktop
(five) and audio (three). Exact sorted name equality with registration proves
192 unique cases per width, zero missing/extra names, failures or skipped cases.
Both cached full builds report no work; the changed fixture's preceding dual
builds compiled and linked without warnings. Published EXE hashes remain:

- x86: F657EC43BADB3BE205C837A0605376F2F0E29EC0F863298712AE9D0C786A63E4
- x64: B5E940DC7F101AF8D484C2E6FE272114C8A39AA58EBE13355944FD722BC5E362

Actual-change self-review: only the fixture's existing attach and monitor wait
paths observe child exit, and failure captures output before detachment. There
is no new retry, thread, API or production path. Successful prompt assertions
are unchanged. Similar-issue sweep covers all waits in this fixture: subsequent
guest/debug operation waits retain bounded budgets and now share final child
exit/screen diagnostics; they are not startup waits and are not broadened here.
The historical 0xC0000142 failure remains unproved and belongs to planned S4.
No shared tree/manifest, guest, configuration or media changes in this P.
Task scratch is retired only after owned jobs exit and evidence promotion.

Coordinator review of pushed executor commit e6090b97 confirms the three actual
changed paths match the admitted follow-up, test assertions are not weakened,
production/EXEs remain unchanged and no new public dependency or recovery path
exists. Both success and early-exit behavior have direct execution evidence.
The complete name ledger and disconnected-history qualifications above agree
with the inspected XML results. Documentation, whitespace and build-residue
checks pass; scratch has been removed after all owned jobs exited. S2 remains
verified awaiting owner acceptance, with T99 open and S3/S4 not admitted.


### S17 admission: exact NXVM shared import

Frozen universe: 296 paths across four public src/test pairs at NXVM
`346094bf2b166a547b0a38abe1c98b8956e77fd6`. The eight changed paths are
the Lib Window implementation and manifest, four Lib test paths, and Emulator
test CMake and manifest. Exact bytes and file set must match the pinned commit.
S16 identified no import blocker; controlled snap restore is tested upstream,
while actual physical snap/drag order remains unverified. Receiver qualification
requires x86/x64 builds, all registered tests, packages and actual-diff review.


### S17 import qualification and self-review

All 296 paths across the eight shared trees are byte-identical to pinned NXVM
346094bf2. Production C +33/-6 (net +27); test C/CMake +60/-28 (net +32),
excluding manifest and documentation. The Window change retains the existing
scale/snap path and adds one sizing marker; no public API, outward dependency,
retry or second renderer is introduced. The fixture lock and its cleanup own
only the Types test fixture; changed test budgets are targeted. Review covers
the initial minimum, manual sizing, snapped frame and restore transitions;
physical Windows snap/drag message ordering is not independently tested.

Cached x64/x86 builds succeeded and refreshed both EXEs, with no compiler
warnings reported. Each architecture has passing evidence for all 193 distinct
registered tests: 185 background, five desktop and three native-audio. The
complete desktop groups initially failed compact Console at stage 1/error 6
with child exit 0xc0000142 on each width; x64 repeated this on a second full
desktop invocation. The unchanged isolated test subsequently passed on both
widths. This is supplemental evidence, not an uninterrupted full green run
or a claim that the pre-existing startup failure was fixed.

Two initial sandboxed Ninja attempts stalled in Windows I/O completion with no
visible compile subprocess; they were terminated as failed execution attempts,
not test results. A normal authorized execution environment completed the builds
and tests. Both EXEs were published:

- softpc32: FF467362EA83C0A960A13CE5E1CC7304BDC177B5A2788B3886CE3FBBD692E134
- softpc64: 24F3B9AE9D7E0EECE0092C7554E9D11874F7D439D604F91E1335E13FF70DA3D0

User INI, media and snapshots are unchanged. Native physical snapping remains
an owner acceptance item; no Computer Use was performed.

Coordinator review of pushed executor `7a262fb1`: the actual thirteen paths
match the admitted scope. Git blob inventory confirms all 296 shared paths
match the pinned NXVM commit, both published EXE hashes match the record, and
no App, mirror, configuration or media is changed. Documentation, diff and
residue gates pass. The recorded supplemental compact-Console results are not
a single uninterrupted all-green run. S17 awaits owner acceptance; T99 stays
open.


### S18: whole-file mirror selection

The frozen 30-file candidate universe consists of 10 C translation units with
zero text/data/bss under both cached product builds and 20 headers not observed
in 676 x86/x64 product/App-test preprocessing contexts. Remove only original
XP-identical bodies: 10 C and 17 headers. Retain monregs.h, nt_pif.h and
nt_reset.h because they contain reviewed local patches pending separate audit.
Keep all selected in-file conditional paths; no runtime or ABI change is
intended. Preserve source hash and reason for each removed file in this record.
The original c-files list is narrowed as a selected XP subset; no build-time
transformation or second execution route is introduced. Verify by cached dual
build, exact binary comparison, mirror/source gates and all registered tests.


S18 exclusion ledger: `empty-object` means zero text/data/bss under both
product widths; `unreached-header` means no inclusion observed in 676 tested
product/App-test preprocessing contexts. Raw and subset hashes are retained:

| XP selected path | Reason | XP SHA-256 |
| --- | --- | --- |
| `base/ccpu386/bswap.c` | empty-object | `16b8260d51041fc1a350018397ca4098de279eb94043727eae9160724d4bddd7` |
| `base/ccpu386/c_getset.c` | empty-object | `bc3b2655b2eaeebfc9fa343d1f4f7153d3ba31d5019a0246ec8c684478635dcd` |
| `base/ccpu386/ccpupig.c` | empty-object | `f9fe472f9979f510a56cdfeebb150870fa78b51bfff4189668745cc7f3fc3780` |
| `base/ccpu386/cmpxchg.c` | empty-object | `31864ed50ec06922d3220501f713b9137c241d18ce9cf9c7d39b4f2d59257731` |
| `base/ccpu386/evidgen.h` | unreached-header | `05737651808fa634f26fa4dd5c3c11b14b066acb7d8c01dd88ce79c0e2ed29b6` |
| `base/ccpu386/invd.c` | empty-object | `ca64ec64cfd906bc4d229c6b9bb00f9b8a0cc865535f9c775ea7cc737bef054f` |
| `base/ccpu386/invlpg.c` | empty-object | `909b84cf9466a4f36756ff98b1949bec64e7a632443cf9ead6dabc94ef1bde97` |
| `base/ccpu386/univer_c.h` | unreached-header | `be241ef045b48ab5744e5ff2c71e129e655e2acc4d0178e292afd7bc346a83fa` |
| `base/ccpu386/wbinvd.c` | empty-object | `a0e32377cb179eb3be70a5ff1c9c9a7194daff135e1617173cc46a18870b965a` |
| `base/ccpu386/xadd.c` | empty-object | `87b973e680365c518f41e0096b219f06850cbfe0edde6866b91cf80f7f21d6c4` |
| `base/cvidc/accessfn.c` | empty-object | `1e3e5e520c3f20df5f5039d0711a66f6acad04510c48af04a8a7a9cf4cf19ed1` |
| `base/cvidc/cpu4gen.h` | unreached-header | `79825604d1efce8ce1c2068a13c476fe929413bb43f68c17fe42127cf896a6c3` |
| `base/cvidc/cpu_c.h` | unreached-header | `b8053aab0869cae44ec197df7937876f03858d7a50da834cc8d3199d7dc91a31` |
| `base/cvidc/cpuint_c.h` | unreached-header | `6d5a28e8e3922b63814ebeaeafce0c0176c69cb1d7ec2f900fe16d0eab9e195b` |
| `base/inc/dasm.h` | unreached-header | `25701d961e72cb47ef84cbfc6efb702de83b7198b837edad8d227ae5ccd5aa68` |
| `base/inc/decode.h` | unreached-header | `c4502f6c9d2439c0764abd0d80eee077888d9902a689e5f7dc854be30826d3d5` |
| `base/inc/egatrace.h` | unreached-header | `c1c63a5cfdb8b7497a5c5cb7d25b4fab8d3d125dd4e8f6a6de6e9457e3975d47` |
| `base/inc/gisp_sas.h` | unreached-header | `51225563e31793b166df3d38f47eda3955bef665a53a318d01c8ffdf842d175a` |
| `base/inc/gore.h` | unreached-header | `a12a6e13ba5b6a1b3253ad53514d241501e64445bebe7c9d35e0b633c0b1e48f` |
| `base/inc/herc.h` | unreached-header | `0930eee3ae09b767a823727da81e232591627c329cb340cbd8044c45fe5aedd5` |
| `base/inc/hg_cpu.h` | unreached-header | `1236eb8a3c4afcaa1f8b3a898fae4e9b82d8c2cbd166041d5cde6e2e55ec0bf2` |
| `base/inc/hwvga.h` | unreached-header | `6b05bcb5cbe9ce8a344a9ac39dbc50c3b302d28481a063e4579b299edb39865b` |
| `base/inc/mouse16b.h` | unreached-header | `32fd663da37039e112d52edde8f0834131c9478af71296f2aed95a367d28ca07` |
| `base/inc/novell.h` | unreached-header | `027b12b8898d930fd2ecaf72489da03383108a21b1623ee0a13e6ffc949d557c` |
| `base/video/ega_trcr.c` | empty-object | `cec51198bb43de3a7ed960cf7ae08f5039eb49240fb622b870181f4afce13c38` |
| `host/inc/nt_eoi.h` | unreached-header | `672cb99d548ab30416ed753598a64c223daff951d1e331846e04e949bd004175` |
| `host/inc/nt_extra.h` | unreached-header | `8660e7b1ae9b31dd4f85b717a7e40194fdbf159a4c5533a03653488940796daf` |


S18 implementation and qualification: 27 XP-identical whole files were
removed: ten zero-object C files and 17 headers unobserved in the 676 frozen
product/App-test preprocessing contexts. The eight corresponding CCPU entries
were removed from `base/ccpu386/c-files` as a selected XP subset: its original
raw XP hash remains in the current manifest, while the selected and local hashes
match. The two remaining empty C files were removed from the root source list.
The three unobserved headers with reviewed local patches are retained pending
a separate audit. No original code branches were edited; Compat and shared
components are unchanged. The mirror count is 478. Mirror/source/documentation
gates pass. Tracked mirror diff +0/-7168 lines including eight c-files entries;
build and verification scripts +1/-4.

Both cached x86/x64 builds completed without reported warnings and published
both EXEs. x86 and x64 each passed all 193 unique registered tests on first run:
185 background, five desktop and three native-audio, no skips or retries.
XML inventory exactly matches registrations. Byte comparison against S17:
lengths unchanged; each file differs in six bytes exclusively in PE timestamp
and checksum fields, and zeroing those two fields makes the files identical.
Raw binaries are not byte-identical. No artifact was modified to conceal that.

- softpc32 SHA-256: 702560228E2B84D953B1FD9D15DAB11510DB30F92A6DC8670D616711B7078CB3
- softpc64 SHA-256: 8F5D8AFE9E1C293D00947EE8D2AEFCC55910E594CD6EA81D89D3843D991BBF9E

The inactive includes in retained historic branches are not expanded by these
selected profiles; in-file pruning and future profile support are outside S18.
User INI/media/snapshots are preserved.

Coordinator review of pushed `664be095`: the actual commit removes the 27
recorded files and eight source-list entries, preserves the three local-patch
headers, leaves all public trees and user inputs unchanged, and passes the
mirror, documentation, whitespace and build-residue gates. Rechecking the
committed EXE blobs after zeroing PE timestamp/checksum confirms identical
remaining bytes; raw files differ. The documented first-run test inventory and
qualification are retained. S18 awaits owner acceptance; T99 remains open.


### S18 owner extension: three unobserved patched headers

Owner explicitly authorizes removal of `host/inc/monregs.h`, `nt_pif.h`,
and `nt_reset.h`. Each was unobserved across 676 frozen product/App-test
preprocessing contexts; only monregs.h has a textual include, within the
unselected CPU_30_STYLE and MONITOR path. The S3 patches are NTVDM-only
product declarations, not selected machine behavior. The following exclusion
records retain XP raw, selected subset and former local hashes; earlier S18
statements that these files were retained are historical, superseded here.

| Path | XP raw | XP subset | Former local |
| --- | --- | --- | --- |
| `host/inc/monregs.h` | `c5d1d8e08a269c2dbb09dd32fe11dfb4dd6386fa4dec5299385c9f0d2e377ac1` | `01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b` | `21ca0069d71627bae01020712269ba835c4e28d6b2ebb3b5503fe144a9ae77e1` |
| `host/inc/nt_pif.h` | `965e72f463590478f391bcd9119bd1aadb1b310d38cd191a138ecd41f7fb7e61` | `01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b` | `0e970eb7fb050158cc2038f348e5d4de1d1ffd0d40f6babb7f15c64230eb9133` |
| `host/inc/nt_reset.h` | `951a97fbbda076a1a0e63a39ee090efe59ab739e7fbb8ee3461cd73e45343170` | `01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b` | `6491fb94edc53544e53bdffdac61d1887574308b447bf3e3153254c47001b43f` |

The three headers above are removed; the manifest now records 475 retained
bodies. Their 289 former local lines belong to unselected NTVDM-only host
interfaces and are not reproduced elsewhere. The original, subset, and local
hashes are retained in the preceding exclusion ledger. The inactive monregs.h
include remains in the historical CPU_30_STYLE/MONITOR branch; that branch is
outside the selected machine and will be handled only in a later branch task.

Cached x64/x86 builds each reported no work; published EXEs are byte-for-byte
unchanged from 664be095. Both architectures passed all 193 registered tests
on the first run, with exact XML/registration equality, no skips and no
failures: 185 background, five desktop and three native-audio. Mirror and
documentation gates pass; no public component, user INI, media or snapshots
changed. This follow-up removes three whole files and does not alter retained
production code.

Coordinator actual-commit review of pushed `bcbf0bb8` confirms only the
three approved headers and three task/provenance records changed. The original,
subset and removed local hashes remain recorded, all public trees and EXEs
are unchanged, and mirror, documentation, whitespace and residue gates pass.
Both architectures have first-run passing evidence for every registration.
S18 awaits owner acceptance; T99 stays open.


### S19: fixed mirror compile boundary in Compat

Owner closes S18 and admits a focused audit and minimal cleanup. The finite
universe is every compiled `src/app-softpc/compat` translation unit and its
actual x86/x64 selected mirror and own-header include paths. Classify every
original `const` macro crossing into own declarations, and the PIG/WDCTRL_BOP
conditions, as required, corrected, or proven harmless with evidence.
Preserve the selected CCPU/C-VID and native ABI; no mirror files, generic
macro framework, parallel device implementation, or public trees change.
Use in-memory preprocessing first and document exact scope before editing.


S19 final-source implementation and qualification:

- The fixed profile does not select PIG and does select WDCTRL_BOP. Removing
  the Compat wrappers leaves both x86 and x64 preprocessed outputs unchanged
  for those blocks. Other selected mirror configuration remains owned by the
  existing build; no original mirror branch is pruned.
- `compat/mirror_types.h` includes the original type declarations and ends the
  legacy `const` macro at the Compat boundary. All 25 compiled Compat
  translation units per architecture were checked with their real compiler
  preprocessing commands: zero retain the macro at translation-unit end.
  `compat/cmos/port.h` intentionally keeps the original include because it is
  also consumed from mirror code. The new compile test checks representative
  const-qualified declarations. Three audio fixtures use the same boundary.
- Against S19 admission: Compat production +27/-35 (net -8); App test C
  +24/-3 (net +21); root build +4/-0. No public component or original mirror
  source changed. These are line counts, not a claim that ABI qualifications
  provide exhaustive runtime proof.
- Cached dual builds completed and produced the assets binaries below. Each
  architecture has passing evidence for all 194 unique registered tests:
  186 background tests, five desktop and three native-audio. The initial
  native groups ran while the executing Windows session was disconnected and
  had failures; after reconnection, both native groups were rerun unchanged
  and passed. This does not establish that disconnection caused every failure.
  The first fixture draft also failed the Types gate and was corrected before
  final builds. No non-mirror build warning was observed in final builds.
- softpc32 SHA-256: 52B2C6B55134EFC82265D0858759DC9D88BB3673932FBA6066000738848414DE
- softpc64 SHA-256: A389FCC072843520E96896EAF5F29D2F8098DF1CE0BFDD065D341461A7DBF4C7

Self-review: one owner-local header replaces scattered const cleanup; the
fixed-profile blocks use existing behavior directly. No new runtime path,
state, polling, public interface, or mirror patch is introduced. The native
test evidence is a supplemented run rather than a first-pass all-green run;
user media, INI and snapshots are unchanged.


### S20: retained mirror conditional-branch audit (in progress)

Frozen starting universe: the 475 bodies in the current XP manifest at
S19 delivery `23f65b5c`: 229 C, 241 headers, three ROM, one RAM and one
extensionless source-list file. The first textual inventory finds conditional
directive spellings in 109 C files and 85 headers, with 2,887 `#endif` lines.
This is a discovery inventory, not an actual preprocessing or semantic
classification: comments, include guards, nesting, multiple build contexts
and macro redefinitions must be resolved before any removal decision. Binary
ROM/RAM and the source list are not C preprocessor branch candidates.

The actual x64 CMOS compile command explicitly selects CCPU, CPU_40_STYLE,
CPU_486, C_VID, PROD, SPC386, V7VGA and WDCTRL_BOP. This alone does not prove
the value at every later conditional or in every x86, test or include context.
CMOS combines NTVDM/macintosh alternatives with existing non-NTVDM config
handling, so it must be analyzed as complete nested chains, not deleted by
macro search. V7VGA and WDCTRL_BOP include selected device paths and must not
be treated as unused merely because they are conditional. S18's removed
NT-only monregs header is still named from a retained inactive CPU_30_STYLE /
MONITOR branch; its branch is a candidate for detailed audit, not proof that
the entire containing condition is removable.

Remaining before a deletion proposal: enumerate each branch and its nested
condition, actual production and test inclusion contexts for both widths,
source-level redefinitions, shared declarations and selected local patches;
then map each proposed exclusion against the existing XP subset ledger and
provide per-file estimates. No branch deletion, compatibility replacement or
complete-domain claim follows from this initial inventory.


#### S20 actual-command discovery evidence

At `23f65b5c`, the cached Ninja release trees provide 332 distinct C
compile commands per width: 233 mirror-source contexts, 47 own App/Compat
contexts and 52 App-test contexts. All 664 commands were preprocessed with
their original compiler, definitions and includes; zero reported failure.
The two derived inventories are [file inventory](../etc/evidence/softpc/m9-t99-s20-file-inventory.tsv)
and [conditional-arm observations](../etc/evidence/softpc/m9-t99-s20-branch-observations.tsv).
The former covers all 475 manifest bodies; the latter records 3,983 textual
conditional arms across 194 C/H files. Source paths are case-folded for
matching Windows preprocessor line markers, retaining manifest spelling.

The discovery marker reports a nonempty output line in an arm, not a semantic
branch decision: generated comments may count, nested arms overlap, compiler
line mappings do not reveal all macro state and an include-only arm can emit
no parent-file output line even when selected. Hence `no` does not mean dead,
`yes` does not prove runtime relevance, and no line total or auto-removal
list can be derived by adding these overlapping ranges. Actual observation:
1,490 arms have nonempty lines observed for both widths, one only for x86,
and 2,492 have none observed for either width. One known include-only false
negative is `c_main.c:33-34`, while its TLS code at 832-833 is observed.
The first exploratory scan doubled Windows CRLF line numbers and was rejected;
the replacement uses binary line reading and was checked against the known
CPU_30, V7VGA and TLS examples. A first probe also stalled on output pipes and
was retired; neither attempt is evidence of a product failure.

Concrete examples and preliminary disposition:

- `host/inc/host_cpu.h:17-111`: CPU_30_STYLE body has no observed lines; no
  scanned compile command defines CPU_30_STYLE and a source scan found no
  direct definition in App source/tests. Its MONITOR-only deleted monregs.h
  include is nested inside. Candidate for an isolated, reviewed subset change:
  preserve outer `#ifdef/#else`, replace the 94 body lines with one fixed
  exclusion comment (estimated source +1/-94). The file's non-CCPU/local ABI
  work remains outside that body. This is an estimate, not implementation or
  proof for every possible alternative build.
- `base/ccpu386/c_main.c:832-835`: retain the selected TLS simulation-stack
  arm; it is enabled by `SOFTPC_CCPU_TLS_SIMSTACK` despite the other NTVDM
  alternative. It contains a previously recorded local ABI correction.
- `base/video/vga_prts.c:398-435`: V7VGA declarations are observed on both
  widths and the file has reviewed local cursor/video patches. Retain the
  selected behavior; do not infer the fate of unselected arms by file label.
- `host/inc/host_def.h:234-237`: the `i386` declaration is observed only on
  x86, with an alternate x64 arm. Both supported compile contexts matter;
  selected text alone is not proof that the symbol has a live consumer.
- `base/comms/com.c`: a candidate SEGMENTATION arm and several IRET_HOOKS
  arms have no direct output lines; the same file contains the active UART
  and snapshot integration, and the chosen non-IRET path is observed. Audit
  its whole nested chains and snapshot callbacks before a deletion estimate.

The 2,492 discovery candidates include 919 arms in files with reviewed local
patches and 889 in files with preserved local patches. These are arm counts,
not unique files or removable changes. Semantic classification, complete
consumer review, potential original-code reuse and per-file XP subset diff
estimates remain open; S20 is not complete and no mirror or EXE was changed.

The largest no-direct-output clusters are `cpu4gen.h` (652 arms),
`sas4gen.h` (166), `c_main.c` (87), `ega_read.c` (84), `gfx_updt.c`
(68), `com.c` (64), `reset.c` (62), `ica.c` (62), and `video.c` (54).
They mix nested, generated and diagnostic paths; these counts cannot be
summed into a diff or interpreted as approved deletion. In `com.c:670-699`,
IRET_HOOKS changes interrupt/receive scheduling, while the observed alternative
uses the existing host input-ready callback and quick event; this is an
alternative device behavior, not a demonstrated replacement for Compat.
The selected UART/snapshot path and its tests must remain intact.


#### S20 branch markers and first bounded cleanup candidate

To distinguish include-only and empty arms from non-selection, 470 C/H
mirror bodies were instrumented **only in a disposable copy** with a unique
preprocessor pragma after each of 3,983 conditional arms. All 664 frozen
x86/x64 App/product/test commands were preprocessed against that copy; it was
never a product build input. Known PIG, CPU_30, V7VGA, and mixed TLS examples
were checked. Case-normalized file markers cover all 470 bodies. Initial
64 contexts failed because the copy lacked relative shared headers; after
copying those headers, the 66 retry commands covering those contexts succeeded.
A projection-tool draft also failed on differing historical TSV column names
and was corrected. These are research fixture failures, not product failures.

The branch observation ledger now includes marker evidence: 1,515 arms
selected in at least one context on both widths, one observed only on x86,
and 2,467 not selected by those contexts. An arm may be selected in one
context and not another even for the same width; `selected` is a union, not
a promise that every context selected it. The temporary path and pragma probes
are bounded evidence, not proof of equivalent behavior under other builds or
permission to delete a branch. Macro continuations, nested parents, generated
ABI, selected device paths and local repairs remain subject to manual review.

A first independently reviewable candidate is `#ifdef SEGMENTATION`: all 93
matching arms were unselected in the scanned contexts. Of those, 90 arms in
53 files contain only original `.seg` includes and comments; they do not
overlap and their 475 body lines match the pinned XP raw sources. All candidate
body matches are included in the existing necessary-XP subset ranges; repeated
identical bodies were checked at all matching original positions. The three
other arms contain nested directives and are explicitly excluded. The `.seg`
blocks describe historical Mac compiler code-segment selection, not a machine
implementation that could replace standalone Compat. The existing source
contains an `#undef SEGMENTATION`, but no source/test definition or selected
compiler `-DSEGMENTATION` was found; this scope does not prove future profiles.

Estimated implementation diff if the owner admits this candidate: keep all
existing outer conditions, replace each 90-body block by one fixed comment,
source +90/-475, net -385 lines across 53 files. This is not implemented and
excludes manifest, range records, tests and documentation. Because the 475
lines are present in the old XP necessary subset, the new subset must record
their deliberate exclusions separately; the 90 local comment lines remain
visible in the local-versus-new-subset comparison. Compute actual per-file
raw, subset and local diffs after implementation rather than assuming Git
alignment in files containing duplicate blocks. The 53 files include 56 candidate
arms in REVIEWED_LOCAL_PATCH files and one in a PRESERVED_LOCAL_PATCH file;
raw-XP exact body matches are not a blanket license to touch adjacent patches.

S20 remains open: the other 2,377 unselected arms (including three nested
SEGMENTATION cases), cross-component alternatives and original-code reuse
have not all received semantic or patch-by-patch review. No broad deletion
estimate or full-domain semantic completion is claimed.


#### S20 PIG comparison

The actual-context marker audit identifies 84 plain `#ifdef PIG` arms in
18 files, with 977 body lines before any deduplication. They are mutually
non-overlapping and unselected in this fixed x86/x64 evidence set. Against
pinned XP raw bodies, 82 match some original contiguous body; two in
`base/ccpu386/c_main.c` do not: line 2911 includes the local
`softpc_host_debug_retired()` hook, and line 5055 contains the local
`SOFTPC_CCPU_TLS_SIMSTACK` condition. These are inside unselected outer PIG
branches, but their prior purpose, provenance and comparison accounting must
be recorded before deletion. Other duplicate bodies have multiple original
locations, so raw-body identity alone is not a per-location patch proof.
The remaining PIG family includes `#ifndef`, compound and nested forms and
is not equivalent to these 84 cases. No PIG cleanup is approved by this audit
and no generalized reuse or runtime effect is claimed.

#### S20 ROI guard against mechanical churn

There are 22 unselected plain SCCSID arms, each with one body line in a
different file. Replacing each with the requested one-line placeholder would
produce approximately +22/-22 source lines before provenance changes; it
does not materially reduce source length and would touch 22 files. Do not
recommend this as a separate cleanup just because the condition is inactive.
Similarly, counts for generated PROD arms must not be interpreted as a
worthwhile batch without considering nesting, ABI, placeholder overhead and
the exact current XP subset. This is a prioritization decision, not a claim
that every such arm is permanently necessary.


#### S20 owner direction: preserve conditions and reject excluded arms

The owner now specifies the binding [coding rule](../rules/CODING.md#selected-mirror-conditional-branches):
retain or restore original conditional structure in retained mirror subsets;
replace each deliberately excluded body with `#error` alone, including any
existing comment-only placeholders. Earlier S20 discussion of one-line
comment placeholders is superseded. This is not permission to classify an
unobserved arm as excluded or to alter selected behavior; classify original
context and existing local patches first. Whole-file exclusions remain a
distinct decision. Audit the 85 recorded subset-baseline files as well as
other retained files, and distinguish actual local source from range projection.
Current S20 evidence and 90-arm sizing are research, not an implementation
or proof that all former removed conditions have already been restored.

The owner also requires necessary `#line` directives for preserved diagnostic
locations. Audit the original and local line-control directives and verify
file/line mapping on each applicable boundary. `#line` is retained line control,
not an explanatory comment or a replacement for the required `#error`.


#### S20 owner-requested first implementation and build validation

The owner directs an initial application of the rule and build verification.
This is a bounded first application, not conversion of all 85 prior XP
subset files or all 2,467 unselected arms. Against S19, six mirror files
change +64/-6: five previously selected-subset-identical files restore only
their original pinned XP conditional directives (`cmos_bis.c`, `timestrb.c`,
`ega_mode.c`, `egawrtm0.c`, `egwrtm12.c`); their normalized local contents
now exactly match the respective XP raw objects. Their prior subset projection
is retained as the independently recorded old baseline; their actual restored
directives are visible in the local-versus-subset manifest identity rather
than hidden as a new baseline. No non-directive original body was added.

In `com.c` the original SEGMENTATION directives remain, the excluded Mac
segment body is replaced with `#error`, and `#line 27` preserves the existing
source location following the branch, also matching the pinned XP position.
Other candidate SEGMENTATION blocks, prior excluded-body placeholders and
restoration of previously removed condition structures remain unconverted.
The prior audit ledger describes the frozen pre-pilot source, not the new line
numbers. No Compat or public production source changed.

Final x86 and x64 cached all-target builds succeeded. Each architecture passed
five focused cases: serial, CMOS, VGA-frame, XP mirror and mirror selftest.
The actual `com.c` preprocess command with `-DSEGMENTATION` failed with the
expected diagnostic on both widths, while normal preprocessing succeeded and
emitted the line-27 marker. Final build logs include 406 original-mirror warning lines, and zero warning
lines outside the mirror; no full regression or guest visual verification is
claimed. Task-owned scratch was retired after evidence promotion. First mirror-check
invocation used an incorrectly formed relative source parameter and failed
to find the manifest; rerunning with the explicit absolute source path passed.
Both package EXEs were rebuilt: their lengths match S19 and each differs in
six raw bytes, only in PE timestamp/checksum; the remaining bytes are equal.
- softpc32 SHA-256 f87f88c7bb730f404c03ac1216cf6ee6f27a7a93ffee8b7da4b08c0acc507160
- softpc64 SHA-256 813b6a33b1b7a24d3326fcada9930a1568ac61092ce09e8527fd650e0adb7c35


#### S20 macro-state inventory at 31ee31e1

Owner requests classification of all mirror conditional identifiers while
deferring coupled-condition edits. The inventory covers 470 retained C/H
mirror bodies and 664 distinct x86/x64 App, Compat and App-test C compilation
commands, 332 per width; other public-component-only translation units are not
in this universe. Conditional locations are physical source lines at 31ee31e1.

A disposable instrumented copy inserts independent `defined(NAME)` probes
before reachable `#if`, `#ifdef`, and `#ifndef` conditions. The original
commands and include order are reproduced with mirror/App/test paths redirected
to the temporary copy and supporting headers copied. The copies are used only
for preprocessing, not as build inputs. All 664 contexts exit successfully.
Known CPU_30, PIG and mixed NTVDM/TLS states were independently spot checked.
Two `#elif` conditions (`ppi.c:234` SOFTPC_STANDALONE and `ica.c:1932` ASPI)
are explicitly marked unprobed; a probe before `#elif` would change conditional
semantics. The preprocessor controls probe reachability, so conditions nested
inside inactive parents remain unknown rather than being reported false.

[Macro inventory](../etc/evidence/softpc/m9-t99-s20-macro-inventory.tsv) has
241 identifiers: 176 undefined only where observed, 27 defined only where
observed, 24 both defined and undefined across observations, and 14 with no
reachable observation. Seventy-five identifiers have at least one occurrence
not reached on a width or one unprobed `#elif`; category names apply only to
observed positions. [Per-location evidence](../etc/evidence/softpc/m9-t99-s20-macro-locations.tsv)
records 3,070 probed identifier occurrences and two unprobed `#elif` uses,
with direct-expression symbol count and enclosing depth. Include guards,
compiler/native helper names and domain selectors are all retained; nesting
includes ordinary include guards. Defined is not necessarily enabled: `#if X`
may evaluate false when X is defined as zero. The values were not measured.

Only `NDEBUG` appears in all 664 command lines as `-D`, but 130 commands also
contain `-UNDEBUG`, illustrating why command flags alone are not the actual
state. CPU_30_STYLE is undefined at all reached occurrences on both widths;
one nested occurrence is unreached. SEGMENTATION is undefined at all 93 of
its occurrences on both widths. CCPU, CPU_40_STYLE, PROD, V7VGA and
WDCTRL_BOP have both defined and undefined observations across product/test
contexts; do not treat the product target's flags as uniform across tests.
The two inventories do not decide deletability or compile support: outer
conditions, `#elif`, macro values, include guards, selected local patches,
original conditions and test dependencies remain relevant. No source, EXE,
media or configuration was changed by this macro investigation.

#### S20 outermost exclusion reconciliation in progress

The owner directs that an excluded outer arm replaces its entire body including
inner conditionals, while the arm itself, outer and sibling directives and
required `#line` mapping remain. Coupled arms remain deferred. This is a
whole-mirror audit goal, not authorization to assume that an unobserved arm
is unsupported.

The current batch converts 56 independently observed top-level SEGMENTATION arms
in 38 mirror files. Each excluded body contains only the original Mac `.seg`
include and accompanying comments; no local device implementation is removed.
The prior XP subset projection remains fixed; its local difference and hashes
are separately visible. The segmentation-exclusions TSV records each baseline
location, restored next logical line and excluded-body hash. The initial 51
resulting line-control boundaries were checked against HEAD before the batch.
Two independent CPU_30_STYLE arms and 73 top-level PIG arms matching XP are
also replaced, with separate ledgers. The 73 PIG body hashes and line-control
boundaries were verified. PIG bodies with local patches and mixed line-control
contexts are not included. The existing pilot in com.c and nested/coupled and other unverified
branches remain outside this batch. The old branch and macro inventories are
frozen pre-change evidence, not updated current physical locations.

Before the NEC_98 restorations, both cached x64 and x86 all-target builds
succeeded. On each architecture 31 focused tests passed: 28 App-owned unit tests, runtime smoke,
XP mirror and its selftest. X64 preprocessor probes deliberately defining
SEGMENTATION, CPU_30_STYLE and PIG reached their expected `#error` directives.
After reversing just the 73 PIG body replacements and their line controls,
all 14 affected files exactly match the pre-batch source. Full mirror
reconciliation and full runtime regression remain open; no full audit or
full-test completion is implied by this batch. Five file-leading NEC_98
structures were subsequently restored from pinned XP in diskbios.c, floppy.c,
floppy_i.c, time_day.c and printer_.c, including printer_'s whole-file
alternative and closing directive. The restoration ledger records excluded
body hashes and XP locations. `#line 1` preserves the previous local source
mapping. An x64 NEC_98 preprocess probe reaches the intended error in
diskbios.c. After these restorations, both architecture builds and the same
31 focused tests per architecture pass; original directive and excluded-body
hash checks pass for all five restorations. A subsequent batch replaces 21
top-level HERC and 14 top-level REAL_VGA bodies uniquely matched in the local
baseline and present in pinned XP; 16 ambiguous REAL_VGA cases were not changed.
The video hardware exclusion ledger records hashes and line boundaries, which
were independently checked. Nineteen standalone HUNTER debug-comparison
arms in ten files were subsequently excluded after checking their uniquely
matched current bodies against the pinned XP source. Their body hashes and
line boundaries are recorded in the HUNTER exclusion ledger and checked.
An x64 HUNTER probe reaches the intended error. In the current x64 configuration,
baseline versus changed preprocessing yields identical output for bootstra.c,
c_intr.c, diskbios.c and printer_.c when each pair is preprocessed at the same
temporary source path. This is sample evidence, not full-source equivalence.
Subsequent work reconciles the 16 duplicate REAL_VGA cases using their unique
baseline order, adds six SECURE arms, the final independent com.c SEGMENTATION
arm with its prior logical line map, and CPU_30_STYLE under xt.h's include
guard. It also restores 84 previously omitted, uniquely anchored standalone
conditions from the pinned subset records: 46 NEC_98, 29 NTVDM, three JAPAN
and six MONITOR. For every restored arm the fixed XP directive, excluded body
hash, existing subset exclusion and retained local boundary are recorded and
checked. All 84 original ranges were rechecked line by line against complete
subset dispositions; no included or unaccounted lines occur in excluded bodies.
The subsequent batch restores 108 uniquely anchored selected NEC_98/NTVDM
guards, ten selected guards with their excluded `#else` bodies, and excludes
18 independently matched old hardware arms. XP directives, selected and
excluded hashes, local line controls and the ten excluded subset ranges were
independently checked. Selected x64 preprocessing matched the baseline for
reset.c, fdisk.c, keybd_io.c and mouse_io.c. Composite expressions and coupled
parent contexts remain deferred. This record preceded the correction below.

Against the frozen 3,983-arm observation ledger, the 227 new exclusions plus
the earlier com.c pilot cover 262 arms once nested arms inside excluded bodies
are included. All 262 were unselected on both architectures in that ledger;
2,205 other unselected arms remain to classify, and may include coupled,
unsupported, unobserved or still-required cases. Their count is not a deletion
estimate. Preserve the 1,516 arms observed selected in at least one width.

#### S20 correction: classify original parents, not only local depth

The local subset may have lost an enclosing XP condition. An independent
lexical scan of the matched original blocks found that 59 previously changed
arms had XP feature parents, despite appearing top-level locally. Those 59
were restored individually from the pre-batch local bodies, without reverting
other edits, and are recorded in the coupled-deferred TSV. After subsequent
independent batches, a repeat scan finds 181 retained new exclusions with no
XP feature parent; one additional com.c arm uses an existing line-control map
and was separately verified. Three
CPU_30_STYLE arms were separately checked against XP parents. This supersedes
the earlier batch counts and descriptions, not their historical validation.

After correction and further standalone restorations, the intermediate tree
contains 185 new exclusions of existing arms and 336 restored original
structures. The frozen inventory accounts for 220 unselected arms covered by
the new exclusions and the prior com.c pilot, including nested children;
2,247 unselected arms remain for classification, many in deferred or already
unsupported contexts. Exact-match restoration scans for standalone selected
NEC_98/NTVDM guards and simple excluded arms have been exhausted within their
stated anchors, not across all mirror branches. Two originally missing guards
became unambiguously matchable after restoring the coupled bodies and were
reinstated. Current source has not reached whole-domain audit or acceptance.
After that correction, eight uniquely matched, independently parented legacy
IPC/VDM-service and null-sound arms were excluded. Their XP body hashes and
local line boundaries are recorded separately; the coupled candidate in
ios.h remains unchanged.

The frozen 470 C/H XP bodies were independently rehashed against the pinned
manifest: 470/470 match. A preliminary per-arm classification of all 3,983
frozen observations is recorded in the branch-classification-preview TSV.
Its original positions belong to S19 delivery 23f65b5c, not the later pilot;
using the S19 mirror resolves the 97 apparent position mismatches previously
reported against the wrong baseline. It assigns 1,516 observed selected arms
to preservation, 220 arms to existing outer exclusions, 1,193 to a local
feature parent, 194 to an XP feature parent, and 98 to composite/value
conditions. Another 366 independent-looking observations and 351 else-chain
arms still need semantic review; 19 have XP/local matching questions, and
22 duplicate and four mixed-parent matches need disambiguation. These are preliminary mechanical dispositions, not
proof of semantic necessity or permission for bulk deletion. The currently
last build overlapped source edits and is not final-source qualification; it
has exited. A clean frozen-source dual build with ccache disabled is running;
focused tests remain necessary.

Subsequent verification checked all 331 anchored restored structures against
their XP directive text, selected/excluded body hashes and retained local line
controls; the five file-leading structures were checked separately. Two
restored `#if defined(NEC_98)` lines in egacpu.h had original trailing spaces;
the trailing spaces alone were removed to satisfy the diff whitespace gate.
This does not change the condition or its original structural placement.

Further review shows why macro observation alone is insufficient. `NPX` is
defined conditionally by the original host header under non-CCPU/non-WINWORLD
conditions, and `YODA` under non-MONITOR/non-PROD conditions; neither is a
globally absent independent selector. `TAKE_REAL_MODE_LIMIT_FAULT` controls
guest CPU exceptions, and `FLOPPIES_KEEP_TRYING` changes floppy interrupt
retry behavior. They remain for separate semantic evaluation. Ordinary
original documentation and historical copyright strings are not silently
replaced. In contrast, the independently matched SLAVEPC host-floppy and
PS_FLUSHING host-printer configuration arms were excluded; they are not
the current media or file-backed LPT paths. Following rollback, actual x64
preprocessing of cga.c also matches the frozen baseline at the same source
path. These observations do not finish the remaining per-arm audit.

Additional inspection shows that unselected SPC486 arms affect actual CPU
instruction/control behavior, while ANSI arms carry alternate C function
declarations. They are not equivalent to dead host glue and are retained
pending explicit CPU-model and compiler-support evaluation. The selected
CPU_486 flag is distinct from SPC486. A matched-block scan found 25 SPC486
and 23 ANSI arms without XP feature parents, plus two ANSI cases with such
parents; this is not evidence that every occurrence or future build is covered.

Of the else-chain observations, 270 are generated `CCPU` alternatives;
the CCPU macro is not uniform across all compile contexts, so the alternate
ABI and actual consumers need review before any such branch is rejected.
Other examples include PROD, CPU_40_STYLE and LITTLEND alternatives. Header
guards, language/type macros, original diagnostics and copyright content
cannot be treated as unused product features solely from a missing marker.


#### S20 CCPU alternatives and current verification

An original-parent and body audit identified 215 direct `#ifdef CCPU`
alternatives in `base/ccpu386/cpu4gen.h`. Each local excluded body matches
the pinned XP body byte for byte after newline normalization; all have only
the file include guard as parent, and both observed architecture builds select
CCPU at each location. The selected side contains the existing ABI patches,
which remain unchanged. These 215 excluded bodies are now `#error` with
original outer directives and baseline `#line` controls retained. The
excluded-body hashes and locations are in the CCPU exclusion ledger. Nested
`PROD` directives inside the rejected bodies are intentionally not retained.
This finding applies to these reached header locations, not every compile
context or all CCPU alternatives in the mirror.

The frozen-source, ccache-disabled x64 and x86 clean builds both succeeded.
After this CCPU-only change, ccache-disabled builds of affected targets again
succeeded for both architectures; the 31 mirror/machine focused checks pass
on each architecture, including the XP mirror gate. The two published EXEs
differ from HEAD even after PE timestamp and checksum normalization, so binary
identity is not established. Other branch categories remain open; the frozen
preliminary counts above predate the CCPU disposition. No claim of a complete
mirror audit or full regression is made.


#### S20 product and legacy Macintosh branch review

Further exact-source review identified 99 independently parented non-PROD
diagnostic arms in 24 files and four independently parented legacy Macintosh
arms in three files. Both actual architectures leave each arm unselected; the
XP body and its local S19 copy match uniquely, and the original S19 files do
not contain line directives. The original conditions are retained and the
removed body is replaced with `#error`; S19 logical line positions are
restored. Separate ledgers record removed-body hashes and baseline lines.

Other non-PROD and Macintosh cases are not assumed removable: original XP
feature parents, non-standalone chains or non-unique matches require separate
review. This is not removal of the public x86 DEBUG component. Final builds
and checks after these batches are tracked separately; earlier passing results
do not qualify a later source revision.

After both batches, ccache-disabled affected-target builds succeeded on x64
and x86. Final-source focused verification passes 36/36 on each architecture,
including the XP mirror, documentation and dependency checks. This does not
qualify a full product regression. Relative to the earlier frozen preliminary
classification, 103 independent observations and 215 branch-chain observations
are now accounted for by the new ledgers; the remaining 263 and 136 in those
two preliminary categories, respectively, still require semantic review.
Coupled, composite, ambiguous and matching cases remain separate.


#### S20 owner clarification: excluded local patches and conjunctions

The owner directs the same `#error`/`#line` treatment for branches proved
unreachable by one false condition or one false conjunct, whether or not the
body contains a local patch. The patch registration is retired for the removed
patch, not for unrelated selected patches in the same file. Merely observing
an unvisited branch is not proof that the condition itself is false, and a
false operand does not establish an OR expression is false.

Both previously identified PIG arms containing local additions in c_main.c
were excluded; the retained file still has independently needed ABI and
lifecycle patches, and its file-level local-patch status remains correct.
A scan of the observed PIG bodies against pinned XP identified no further
local-body mismatches. Twenty AND arms were excluded after both-width
at-location operand checks and original-structure matching; siblings and
ambiguous original structure remain under review. Further local-modified AND
arms, including the SoftPC PPI/snapshot paths, need individual treatment
while preserving the selected branch. Builds after the latest AND changes
are in progress and prior results do not qualify that later source.

The broader single-condition scan identifies 152 exact-XP-body candidates
whose relevant macro is false at the observed compile location on both widths.
Twenty contain historical SCCS copyright strings: when converted, those
notices must be retained outside the excluded branch, not silently discarded.
The same scan identifies 12 non-matching local bodies under false single
conditions; unlike the earlier conservative policy, local difference alone
is not a reason to retain a proven unreachable body. These candidates and the
four PPI conjunctions were staged for individual treatment after the current
owned build, not yet claimed as applied or validated.

The single-condition audit also found 85 exact-XP-body cases with a retained
`#else` and four with locally divergent excluded bodies. The macro state and actual reachability on both architectures are checked. The selected sibling and its
original logical line position must be preserved. These staged candidates are
not yet represented as completed work or qualified builds.


#### S20 single-condition reconciliation after owner clarification

After the prior build, 152 single arms and 85 arms with selected `#else`
were excluded based on at-location macro states and unique XP body matching.
Twenty SCCS copyright notices were preserved outside their rejected arms.
Sixteen additional proven-false arms had locally divergent bodies and were
excluded without touching selected code; four PPI conjunction bodies were
likewise excluded while retaining their selected snapshot/host paths. Two more
AND arms with selected siblings were handled and two original PPI NEC_98
outer structures restored. The original-PPI and local-patch ledgers record
these separately. The retained file-level local-patch status is still necessary
where other selected repairs remain.

A transient manifest write error interrupted one script after PPI source was
written. Its SHA entry and the two-guard evidence ledger were independently
reconciled before remaining work; the XP mirror gate subsequently passes.
The resulting 784 newly excluded existing arms and 338 restored structures
do not include the earlier pilot. Dual builds and final-source tests are in
progress; no whole-domain completion is asserted.


After the above checkpoint, 25 more single-condition arms were excluded,
including eight outer com.c arms mapped from S19 to the actual probe source.
Two original NTVDM PPI guards were restored around the rejected non-standalone
branches. Current totals are 809 newly excluded existing arms and 340 restored
original structures, plus the earlier pilot; earlier raw counts are historical.
The last source revision is undergoing dual builds and is not yet qualified.


The next batch includes 55 SAS generated non-CCPU alternatives, 33 other
uniquely matched false `#else` arms, two false `#else` arms with local body
differences, and the EGA_DUMP arm whose S19/probe lines map 114 to 118.
The two architecture builds for this 900-arm checkpoint are running.
Some remaining original XP outer conditions were removed before S19; the
corresponding local bodies may actually compile, so their parent restoration
requires source-by-source analysis rather than assuming a nested marker
proves the current local code is unreachable.


Current S20 accounting includes 1,358 directly excluded existing arms and
341 restored original structures, plus the earlier pilot. Recent batches
include false single/AND conditions with local differences and selected `#else`
chains, and 402 nested single-condition arms. Their macro states were
verified by mapping S19 positions to the 31ee31e1 probe source: 280/280 and
122/122, respectively. Forty-three earlier branch-chain candidates were
reviewed further; selected parent polarity is important and a parent-table
entry alone does not identify whether its `#if` or `#else` arm was selected.
Nested-body descendants already removed by an excluded outer arm are not
separate production edits. The latest dual build is still in progress.


Current direct ledgers contain 1,568 exclusions and 343 restored structures,
plus the earlier pilot. This includes false nested `#else` arms, the
source-point-tested ASPI `#elif`, and restoration of original NEC_98 outer
guards around retained Video and EGA CPU content. The original NTVDM and
JAPAN/KOREA child structures within the Video guard remain a separate review;
restoring the outer guard does not certify those omitted children.

The source-point probes for `ica.c` were built with each architecture's actual
Ninja compiler flags: the ASPI marker was not selected, while an injected
marker in the `#else` was reached. Earlier complete build results do not
qualify the later guard edits; their dual builds are in progress. Prior EXE
comparison found that after normalizing PE timestamp/checksum and the
`Built on` date/time strings, both binaries matched the committed reference
byte for byte. This remains a checkpoint, not evidence about later edits.


The retained `video.h` NTVDM guard was restored after an instrumented
copy of that header was compiled with the actual Ninja commands for all
38 dependent x86/x64 translation units. All 38 reached the probe with NTVDM,
JAPAN and KOREA undefined. The separately coupled JAPAN/KOREA structure
remains deferred under the owner's stated single-condition/AND scope. Four
additional original NEC_98 structure/member guards were restored in egacpu.h;
the current count of those original guards now matches XP at seven, while
selected C-VID ABI changes remain. Current accounting is 1,568 direct
exclusions and 348 restorations plus the earlier pilot. The latest guard build
and final-source checks remain in progress.


#### Latest S20 verification checkpoint

After the four EGA CPU member guards and the Video NTVDM guard were restored,
both ccache-disabled architecture builds completed successfully. The current
source passes 36/36 selected tests on each architecture, including XP mirror,
documentation and dependency gates; this is not the full product test suite.
The source diff against HEAD is 165 files, +4,936/-15,863, net -10,927 lines.
After normalizing only PE timestamp/checksum and the Built on date/time
strings, both EXEs are byte-identical to HEAD. Literal SHA-256: x86
`e54435973a3b1095a657d9904b311f7218d3db5c4746512961feda9ff794109c`; x64
`5272039ee894967c086afe30c4ffbd2a06359be935df8c6e133ae4803acc71d9`.
Remaining OR/value conditions and original XP structure gaps require audit;
no complete reconciliation is claimed.


#### S20 continued single-condition and AND checkpoint

The corrected scope includes every proven excluded single-condition or AND
arm, including one containing a local patch; it is not restricted to PIG.
Additional original guards were restored in BIOS, communications, keyboard,
video and relevant headers using source-point evidence, while selected code
and unrelated local changes remain. The accumulated ledgers record 1,568
existing arms excluded and 378 structures restored, plus the earlier pilot.
These are ledger entries rather than a claim that all original structures have
been reconciled. The video-header macro check covered all 38 actual dependent
x86/x64 compile commands; subsequent guarded video positions were checked with
the actual commands for each architecture.

After the latest source change, both architecture builds passed with ccache
disabled, and the focused 36 tests passed per architecture. Both rebuilt
EXEs match HEAD after normalizing only PE timestamps, checksums and the build
date/time string; their literal SHA-256 values are x86
`e66230ac3a7c1b0e7ce0270230b434fd137e13bdcf2b49b54069558d7a91f9eb` and x64
`79d56112eb0c909cf1d9db2b465f4bc67c810d67ccc15e398b53e101cf9f0395`.
Original structure gaps and unobserved conditions remain under review; OR and
other coupled forms are deferred according to the owner scope. This is not
full-suite qualification or S20 closure.


#### S20 recheck of observed single-condition branches

A fresh line-mapped check of the S19 observation inventory identifies 1,573
single-identifier `ifdef`/`ifndef` arms observed unselected on both
architectures. Of these, 1,079 match a direct exclusion ledger entry and 478
lie within a recorded excluded range; seven additional entries are already
covered by the prior pilot or an outer replacement. The remaining nine are
listed in `m9-t99-s20-mapped-single-review.tsv`. Eight were not reached by
the original macro probes and must not be classified false solely from that
absence; the ninth is an `ifndef GISP_SVGA` with the macro observed undefined
and thus is not a false condition. Lexical parent reporting alone does not
identify else polarity.

Among 51 S19 observed-unselected pure AND expressions, 42 map to a current
condition followed by `#error`; the other nine were changed or eliminated and
require checking against the outermost replacement and local-patch ledgers.
These counts are a line-mapping aid, not proof about unobserved contexts or
removed XP structures. Audit is still active.


A further cross-file comparison identified 37 possible omitted original
single-condition frames using unique surrounding XP/S19/current anchors.
Actual x86 and x64 preprocessing proved 11 of them unselected, with matching
original and current outer structures; those 11 were restored in five files
and recorded in `m9-t99-s20-omitted-single-guard-restorations.tsv`. Other
candidates include selected or unobserved branches, a header, and cases whose
outer structure first needs review; they are not automatically replaced. Both
architecture builds after this batch passed with ccache disabled.

The `sas.h` candidate was tested using 114 actual dependent compile commands.
Only two test-object invocations reached the shadow header; the other 112
resolved the original header before the shadow search directory. Both reached
probes had NTVDM undefined, but this is insufficient to establish all header
contexts, so no production change was made for that candidate.

Four further original NEC_98 outer guards in communications and video were
restored after unique XP/S19/current boundary checks and actual x86/x64
source-point probes. The corresponding five missing, proven-unselected inner
guards were then restored. Both architecture builds passed after this batch.
The respective ledgers record source boundaries and retained diagnostic
line numbers. The total restored structure count is now 398; source review
continues.

A separate check of bare `#if MACRO` and single `defined(MACRO)` forms
found one additional false SFELLOW arm in the selected PIG else path of
`c_main.c`. XP and S19 bodies match, and actual x86/x64 source-point probes
confirm SFELLOW is undefined there. It is now replaced by `#error` with
its baseline `#line`; the other four bare expressions are inside previously
replaced PIG arms. The other 16 observed-unselected single `defined` forms
are directly replaced or covered by a recorded outer replacement. Both
architecture builds passed after the correction.

The previously omitted NEC_98 selection enclosing the COM input/output
functions was restored with both original branch boundaries and baseline
line controls. Four NTVDM-only guards inside the selected branch were then
restored after x86/x64 source-point probes. Both architecture builds passed.
The current restoration-ledger count is 403.

Three additional EGA function-level NEC_98 outer structures and eight
original unselected JAPAN/NTVDM guards within them were restored after
XP/S19/current boundary matching and x86/x64 actual source-point probes.
Both architecture builds passed; the restoration ledger total is 414.

Two originally omitted NEC_98 outer guards in printer and keyboard were
observed selected in both builds, while their nested NTVDM and JAPAN arms
were observed unselected at the actual source points. The selected outer
frames and rejected inner frames are restored without treating the selected
outer branches as errors. Both architecture builds pass; restoration ledger
total is 418.

After verifying actual x86/x64 source-point states, two NT graphics NEC_98
outer structures and their two disabled inner guards, plus the CMOS NTVDM
outer structure and its disabled nested guard, were restored. Both builds
passed; the restoration ledger total is 424. A selected, unused NT graphics
variable remains a subset decision rather than a false-condition candidate.

Owner clarified again that no nested framework may survive inside an excluded
outer arm. A repository-wide lexical scan of current mirror C/H files finds
1,871 branch arms containing this task's `Unsupported SoftPC mirror
configuration` errors; none also contains nested preprocessor structures or
other code in the same arm. A sibling `else` that is selected remains a
distinct arm and may retain its original nested conditions. This structural
scan does not prove that every original omitted frame has been found.

The owner further specified the rule recursively for every selected outer
branch; `docs/rules/CODING.md` now states that rule explicitly. For the
previously uncertain `sas.h` candidate, full preprocessor macro-state output
from all 114 dependent x86/x64 commands shows two reach the relevant
declaration with NTVDM undefined and the remaining contexts do not reach it.
The original NTVDM inner guard was restored at that point; no unsupported
condition was inferred from an unreached context. Both architecture builds
passed and the restoration ledger total is 425.


#### Recursive restoration and verification correction

The recursive comparison identified original selected outer frames. After
matching XP/S19/current boundaries, checking the existing conditional stack,
proving source-point macro state with both actual architecture commands and
excluding overlapping or ambiguous boundaries, 214 additional original
frames were restored across 31 files. Nested candidates remain for subsequent
review. The ledger total is 1,569 excluded arms and 639 restored frames.
A current lexical scan finds 1,933 task error arms and no nested structure or
other code within those arms.

Some earlier statements above that post-change incremental builds and focused
tests passed were based on stale status files: the background PowerShell
scripts were blocked by execution policy. Those intermediate claims must not
be used as verification evidence. After explicitly passing the execution-policy
argument, x64 and x86 each completed 99 build steps with exit zero, and both
Ninja trees reported no pending work. A fresh actual focused test run passed
36/36 per architecture. The EXEs match HEAD after normalizing PE timestamps,
checksums and the build date/time; literal SHA-256 is x86
`296f4582e5aa793074690140905ac485819564223a2cd49c4c790226f5af4828` and x64
`7cf48b970ba808c316178ed888c27cb47fdce221125827181f6cb9a05b4ee1ad`.
Existing original mirror compiler warnings remain.

A subsequent recursion pass restored nine more proven selected outer frames,
including two adjacent printer frames whose shared boundary was explicitly
ordered as closing the first and then opening the second. Both Ninja builds
completed successfully with no remaining work. The cumulative restoration
ledger is 648; the current error-arm scan reports 1,942 with no nested
condition or other code in the excluded arm. Matching results remain
candidates, not a claim of comprehensive original-frame reconciliation.

The V7 mode-setting and reset outer frames were restored after actual
source-point probes. Reset also retains its selected NTVDM-negative frame
and replaces the contradictory nested NTVDM arm with one `#error`. Both
architectures were rebuilt and both Ninja trees report no pending work.
The current ledger counts 1,569 excluded arms and 652 restored frames.

The reset signon path's two selected original frames were restored after
source-point x86/x64 verification. The latest x86/x64 build completed 33
steps per architecture with no pending Ninja work. The restoration ledger
total is 654; the 1,943 current error arms contain no nested conditions or
other code. Source reconciliation remains active.

The selected NEC_98 outer guards in `host_lpt.h` and `rs232.h` were
restored after inspecting all actual dependent x86/x64 preprocessing
contexts (14 and 6 respectively). The former's two original excluded NTVDM
inner guards, including one conjunction, were restored as `#error` after
separately checking NTVDM at every reached context. Both builds pass and
Ninja reports no pending work. The restoration ledger count is 658.

A further original/S19/current line-mapping pass recovered 21 selected
frames where unique boundary and following-context checks, actual dual
architecture source-point probes, and matching current preprocessor stack
proved safe placement. Both builds pass; 1,957 task error arms contain no
nested directives or other code. The restoration ledger total is 679.
Candidates with ambiguous mappings remain under review.

Further proven selected outer frames were restored using original/S19/current
line alignment, matching following context, preprocessor-stack checks and
x86/x64 actual source-point probes. Both builds pass with no Ninja work
pending; the ledger totals are 1,569 excluded arms and 693 restorations.
The current structural scan reports 1,960 error arms without nested content.
Ambiguous and unreached mappings remain under review.

Additional selected mouse and COM structures were reconciled against the
original outer/inner hierarchy, including an excluded COM NTVDM/FIFO_ON
conjunction. Actual x86/x64 source-point probes and builds passed; the latest
Ninja trees have no pending work. The restoration ledger now contains 720
frames; the structural scan finds 1,969 task error arms with no nested content.
Remaining uncertain original boundaries and coupled forms are not claimed done.

The selected `time_day.c` NTVDM-negative parent and two selected NEC-negative
function guards were restored after x86/x64 source-point probes. Twelve
identical adjacent line-control duplicates created by nested restoration were
reduced to one directive each without changing subsequent line mapping. Both
architectures built successfully; the restoration ledger count is 723.

Additional selected timer and COM declaration/function parent frames were
restored after dual-architecture source-point checks. COM's excluded NTVDM
declaration arm is one `#error`; its nested FIFO_ON framework is deliberately
not retained. Both builds pass and no Ninja work is pending. The ledger now
contains 728 restorations; the structural audit found 1,973 error arms with
no nested content.

The COM receive parent was corrected to include the full original selected
region, including the following `recv_char` function; the ledger boundary
was corrected. The other restored parent IDs were compared against mapped
unique XP/current lines, with no remaining mismatch found within those
mapped positions. ROM and EGA selected outer structures were also restored
after dual-architecture macro probes. Both architectures build and neither
Ninja tree has pending work. The current restoration ledger is 732; the
error-arm structure audit finds 1,975 arms and no nested code.

The selected ICA and graphics split/scroll outer structures were restored
with actual x86/x64 source-point and header-context evidence; existing inner
conditions remain in their selected arms. Both architectures built successfully
and the restoration ledger now contains 738 entries. The current structural
scan reports 1,975 excluded error arms with no nested content.

Printer NEC and NTVDM/MONITOR nested guards, the corresponding header
selection, and the selected timer guard were restored using actual x86/x64
source-point and header-context probes. Both builds completed successfully
with no Ninja work pending. The restoration ledger is now 749 frames; the
structure scan finds 1,985 error arms without nested code.


#### Owner-directed breadth-first execution

Audit retained mirror conditions by nesting depth, starting at depth 1,
including sibling arms at their guard's depth. The task-local queue contains
children only of arms that are selected or potentially selectable in a
supported build; once an arm is conclusively excluded and replaced, no
descendant of that arm is queued. An unreached probe is an unknown, not proof
of exclusion. Each depth reports its total, classified, processed, pending
and explicitly deferred arms. Previously completed selective edits are
reconciled with this queue rather than repeated. Batch validation per depth.

Before closing each depth, independently audit every guard and sibling arm
at that depth against its original boundary and all applicable supported
production and test compilation contexts. Confirm that retained arms can be
selected where required and that every `#error` arm is actually excluded in
all applicable contexts; an unobserved or unreached context is not proof.
Review the specific mirror exception for any omitted content in a selected
arm and verify the next-depth queue contains only eligible children. Resolve
or explicitly account for each point before that depth's build, publication,
tests, review, commit and push; do not treat a successful build alone as this
point-by-point audit.

#### Owner-directed per-depth S allocation

Owner closes T99 S20 after the first-depth mirror reconciliation. Every later
conditional depth is a separate S task. The owner authorizes automatic
admission in depth order after the preceding S is built, published, tested,
committed, pushed and closed.
The next candidate is depth 2; its provisional queue and known original-child
candidates are recorded in the [S20 closure](../history/M9-T99-S20-mirror-depth1.md).
These S tasks remain within T99 and are not global Queue entries. For each
depth, deliver excluded `#error`/`#line` changes with the required build,
publication, tests, commit and push before retained-arm audit and queueing
eligible direct children for the following depth.

## T99 S34 unchanged shared import

Owner accepted S33 and admitted import from NXVM 09ebe521e7fede9c6e5985efadf1acbad4ecfc8e.
Frozen universe: all 296 files across src/test lib, emulator, x86-debug and
x86-ibmpc. Six changed files in Lib (including two manifests), no additions or
deletions; every other file must remain byte-identical. The five new code lines
preserve Break separately from Pause and test native decoding. SoftPC needs one
private Break-to-VK_CANCEL case and a regression through the existing adapter.
Estimated production +5/-0, tests +10 to 20/-0, plus manifest hash updates.
Completion requires every file compared, full cached dual builds and tests,
published EXEs and an actual-diff review. Owner snapshot changes join delivery.

## T99 S36 CHAIN2 DWORD read composition

Owner requests independent repair of the reported read-mode 0/1 DWORD rules.
S35 was withdrawn without production changes; do not reuse its number.
Both current rules advance the second WORD address by four instead of two,
and modify r20 without committing that high WORD to the returned local value.
Keep the two existing WORD calls; change the byte offset and explicitly compose
the result. No new helper, layout, ABI, frame or guest-specific behavior.
Estimated source +8 to 12/-24 to 30, tests +30 to 50/-0. Update mirror identity
and exception evidence. Frozen coverage: both readers, odd/even and crossing
addresses, complete high/low values and real read-mode masks. Demonstrate the
old failure and dual-width corrected pass, then full cached qualification.
Do not attribute historic Win31 corruption to this independent defect.

The independent odd-address mode-1 oracle also exposed the WORD dependency's
remaining next-group offsets +4/+6 after the S33 plane-spacing repair. Correct
these to +8/+10 in the same original path; without this prerequisite DWORD
composition is correct but returns wrong bytes. This adds only two changed
constant lines, no layout change. A separate GDP byte-mask alias omission
(offsets 1379/1383 versus 1376/1380) was exposed during mask variation; record
that adapter defect in TODO rather than mixing it into this original read fix.

## T99 S37 five original-device defect audit

Frozen universe: the five owner-reported items below. Each needs actual compile
and call evidence, a failing/passing local behavioral proof if repaired, or
specific non-applicability evidence. Audit all before grouped implementation.

1. Six BIOS SAS access checks: PG_U/access_check/paging/fallback; CPL 0-3,
   supervisor/user pages, read/write, byte/word/dword and relevant V86 cases.
2. D11 slave EOI: ISR clearing, mask preservation, master EOI and exceptional
   no-slave/no-hardware paths through actual PIC operations.
3. recalc_text: establish whether virtual CRTC differs from physical height
   and is reached locally; only then change it, retaining V7, EGA and index
   behavior and proving Win31 mode/font transitions where applicable.
4. PIC ICW1: enumerate real EOI callbacks and negative-count semantics; pending,
   in-service/empty, both chips, repeated init/auto EOI, requeue/locking and
   Sound Blaster effects. Do not infer an absent TimerEoiHook.
5. Keyboard: actual controller/PIC ordering; read-before/after EOI, status reads,
   controller replies, reset/cancel and queue advancement if the defect applies.

No guest binary edits/recompiles or foreign patch imports. Same fixtures before
and after any fixes; keep source proofs separate from dynamic and guest proof.
Production/test estimates follow completion of the overall applicability audit.
Public components and unrelated queued debt are outside this scope.

### S37 audited implementation decision

Real compiled CCPU/SAS test observes all six reversed PG_U requests across
120 cases (CPL0-3 plus V86, S/U and RO/RW, read/write, three widths). D11's
slave ISR remains 08 and its mask becomes 20 after the old EOI. Real Win31
V86 probe, using the existing snapshot and original BIOS font entry, reads
virtual CRTC 400 versus physical calculation 481: 8/16-dot fonts produce
60/30 rows instead of 50/25. Repair these three original sites only.

PIC reset matrix and Sound Blaster negative-notification/restart tests pass;
only AutoInitEoiHook is registered and negative count intentionally does not
advance DMA. Keyboard's existing output_full ownership already passes both
EOI/read orders. No PIC or keyboard production change is justified.

Minimal production estimate: three mirror files, six CPL conditions, one EOI
port, and one VGA V86 CRTC read with index restoration and unchanged multiplier
and rounding (about +21/-7, net +14). Tests approximately +300 lines in existing
owner-local fixtures plus one SAS permission executable. No public ABI/state,
guest binary edits, imported patches or new device implementation. The actual
Win31 probe invokes the original BIOS entry from a real V86 checkpoint; it is
not evidence that an unmodified guest application naturally issued that call.

### S37 P2 admitted fallback-width completion

Owner explicitly adds the adjacent IU8 truncation to this S. Four casts in the
WORD/DWORD protected/V86 fallbacks become their return widths. Existing mapper,
ROM entry, register save/restore and byte/write paths remain. Estimate: no net
production growth; about 60 test lines in bios_access_smoke, no new target/API.
The existing test-local mapper wrapper will request fallback once for a valid
address, then the original CCPU executes the unchanged embedded ROM read entry.
Two nonzero-high-byte values, both CPU modes and all three widths verify the
result and saved registers. This proves fallback transport, not an OS page-fault
handler. Record before/after, dual-architecture full qualification and actual
diff; retire the corresponding TODO only after proof.

### S37 owner acceptance

Owner accepted P1/P2 and approved S37 closure on 2026-11-08. See
[S37 closure](../history/M9-T99-S37-original-device-audit.md) for final scope,
verification and remaining debt. No next S is admitted; CURRENT owns the
open T99 state. Earlier pending statements above describe historical stages.

## T99 S38 GDP aliases and Unchained WORD spacing

Owner approves the preceding read-only audit's two fixes. Frozen scope: five
subfield aliases (1378/1379, 1382/1383, 1407), one Unchained +8 offset,
existing original-rule regression and investigation-only bulk-fill suitability.
The x64 counterfactual already proves 16 Unchained mismatches become correct
with +2 alone; WORD composition remains unchanged. Alias correction makes
non-default CHAIN2/CHAIN4 comparison and rotate reads see their shared fields.

Native GDP mapping groups each subfield with its existing full field; original
historical suffixes identify low-order bytes/WORDs, not native +2/+3 addresses.
No duplicated field storage or consumer compensation. Estimate production
+8/-1 (net +7), existing tests about +80 to 110 lines. Verify nonuniform masks
and field values to prevent an all-zero/all-one endian-blind oracle.

Bulk fill remains a separate capability question: CCPU SAS fill iterates
linear scalar stores, while C-VID glue receives video addresses and subtracts
the current aperture base. Admission requires valid mapped/type-homogeneous
ranges and preserved boundary/fault behavior, plus measured benefit. Do not
activate or remove it as a side effect of the two approved correctness fixes.

### S38 P2 admitted Copy DWORD fill correction

Owner requests repair of the retained bulk entry plus a concrete integration
recommendation, without asking to enable a new CPU consumer. Original odd-start
code writes a leading byte, forms incorrect rotated WORDs and writes one extra
tail byte; zero count still enters the leading-byte path. Preserve the existing
loop and selected dirty marker, fix word formation and bounded final write,
and skip all writes for zero count. Estimate roughly 10-20 changed production
lines and 30-40 existing-fixture test lines, with no new interface or state.
Compare complete physical plane storage against independent repeated-byte
expectations for odd/even/boundary addresses, nonuniform patterns and counts
0/1/2/7. Full dual-width qualification follows; bulk activation stays separate.

The same-file sweep also finds that Copy WORD odd-start fill writes its head
and decrements zero before checking count. Include the identical early exit
through its existing cleanup, plus byte/word zero-count control cases. This
is the same admitted bulk-fill boundary, not activation of another consumer.

P2 implementation: production +16/-7, existing tests +36/-2; both architectures
pass 187 background and eight native cases each, with refreshed EXEs. See
[S38 evidence](../etc/evidence/softpc/m9-t99-s38-gdp-subfields.md) for the
independent pattern oracle, exact XP accounting and concrete SAS integration
boundary. No bulk consumer is enabled; S38 remains pending owner acceptance.

### S38 owner acceptance

Owner accepts P1/P2 and closes S38 on 2026-11-08. See
[S38 closure](../history/M9-T99-S38-cvid-rules.md). The next owner-requested
S39 audits all current mirror differences for necessity and simplification;
no rollback of accepted repairs is authorized by that research request.

## T99 S39 whole-mirror necessity audit

Freeze production at 19a68782 and XP at a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Enumerate every tracked file beneath softpc.new, matching the current manifest
and same-relative XP path. Separate identical, binary/format identity,
necessary-subset exclusions, ABI/compiler adapters, semantic repairs and
unresolved/cleanup candidates. Count each file and diff block; an exclusion is
not an invented runtime feature. Read all suspicious added conditions and
adapter families against actual callers and existing evidence.

Completion requires a per-file disposition and explicit coverage limits; a
mechanical inventory is not proof that every semantic patch is necessary.
No code is changed in this research stage. Any proposed restoration must
preserve an equivalent single path and identify the test/counterfactual needed
before implementation. Keep already accepted fixes unless equivalence is shown.

### S39 owner scope clarification

Already-#error branch bodies are outside this audit. Do not review their old
contents, re-prove exclusion or count them as completed semantic audit work.
File identity enumeration remains only an inventory. Mask those bodies when
comparing retained code; review remaining ABI/behavior changes and added
product/host conditions. Do not reopen approved subset policy.

### S39 retained-difference findings

See [the bounded review](../etc/evidence/softpc/m9-t99-s39-mirror-necessity.md)
and its 475-file inventory. Already-#error bodies are excluded. There are
27 exact generated-rule ABI pattern matches and 110 mixed residual files;
file classification is not individual-hunk minimality proof. Three added
fixed-profile wrappers are concrete -6-line cleanup candidates, preserving
their actual bodies. No broad rollback is justified. Production is unchanged;
individual-hunk necessity in mixed adapters remains explicitly unproven.

### S39 owner-approved wrapper cleanup

Owner authorizes the three concrete candidates, not a broad rollback. Remove
only six local preprocessor lines; retain CGA BIGWIN selection, PIC INTR
withdrawal and PPI snapshot host update. Compare both architecture compile
contexts before/after, build/publish with existing caches, run all background
and serial native tests, then deliver and review before authorized closure.
The frozen review inventory remains evidence of its original baseline.

S39 implementation resolves all three admitted candidates: production +0/-6,
no test changes, six identical preprocessed comparisons, and dual builds with
390/390 full qualification. See [delivery evidence](../etc/evidence/softpc/m9-t99-s39-wrapper-cleanup.md).
The initial research inventory and its stated proof limits remain historical.

### S39 authorized closure

Implementation 26e659c8 passed post-push actual-diff review. The owner's
authorization includes closure after verification; S39 is closed with all
three cleanup candidates resolved. See [closure](../history/M9-T99-S39-mirror-wrapper-cleanup.md).
The initial research limitations remain explicit; T99 is not closed.

## T99 S40 remaining mixed differences

Owner admits read-only continuation over S39's 110 REVIEW rows at f7537b1c.
Exclude identical text, already-#error bodies and changes with exact prior
audit evidence. Do not skip an entire mixed file merely because its family
was mentioned before. Classify each remaining retained change as necessary,
concrete simplification, or unresolved with the missing proof stated. Reuse
accepted repairs and ABI-pattern evidence without re-investigating them.
Completion requires all 110 rows accounted and residual scope explicitly
disposed; broad inventory or name scans alone do not prove minimality.

S40 read-only results are recorded in [the retained-difference audit](../etc/evidence/softpc/m9-t99-s40-retained-differences.md)
and its frozen 110-row ledger. Three local simplifications and two conditional
validation gaps affect four files; no source change is admitted or implemented.
Estimated production reduction is about 13 lines without a shared registry or
new execution path. The two failure cases are source-proven conditional defects,
not demonstrated normal guest failures. Await owner implementation decision.

## T99 S41 admitted cleanup

Owner approves S40 candidates as a new S. S40 is closed; S41 implements C1–C3
and F1–F2 in the four existing owners. Mirror diff (XP additions + deletions)
is the primary benefit: estimated -13, separate from source net line counts.
Frozen completion units: HDD mapping IDs/invalid IDs; Mixer declaration owner;
queue next assignment; inconsistent hash/list failure; anomalous keyboard
capture validity. Each requires focused proof and the packet's full delivery.

S41 implements all five units and passes cached dual builds plus 392/392 tests.
Actual mirror diff decreases by 13 lines, with all original snapshot IDs retained.
[Delivery evidence](../etc/evidence/softpc/m9-t99-s41-mirror-cleanup.md) records
test growth, initial fixture link failure, final qualification and limits.
Owner accepts and closes S41 on 2026-11-08; see [closure](../history/M9-T99-S41-mirror-cleanup.md). No subsequent S is admitted; T99 remains open.
