# Public Handoff Simplification Review

## Admission and scope

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
