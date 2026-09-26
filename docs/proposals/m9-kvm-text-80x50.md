# Bounded 80x50 KVM text frames

## S8 shared Console convergence

NXVM has independently prepared a raw-Console repair: an undersized or
scrolled viewport must not reject a complete frame when the backing screen
buffer can store it. Its preserved patch correctly separates backing capacity
from native viewport geometry, but its proposed output extent derives from the
backing buffer height. That would make a 25-row frame write 50 rows whenever a
host already has a tall backing buffer, undoing SoftPC S7's steady-frame
repair.

S8 therefore combines the two independent rules rather than importing either
implementation wholesale:

1. Surface preparation ensures only backing capacity: width is at least 80 and
   height is at least the active frame row count. It never calls
   `SetConsoleWindowInfo`, so the host retains its font, visible rectangle and
   scroll position. A failed or ineffective backing growth remains I/O failure.
2. Frame output remains S7's completed-content rule:
   `max(active rows, prior committed rows)`. It clears only a confirmed former
   tail and is never based on backing-buffer or viewport height.

The change has no public ABI and no product-policy branch. The focused fake
proves all four axes together: a short scrolled viewport is unchanged; full
80x50 cells still reach backing storage; failed and ignored backing growth do
not acknowledge a frame; and the S7 22/25/43/50/25/50 tail-clearing matrix
remains exact. The native broker smoke continues to cover raw/cooked screen
handoff. This is the canonical implementation for NXVM to adopt after its S3
lands, rather than a permanent divergent patch.

### S8 execution evidence

`console_broker_ensure_text_surface()` now has one responsibility: grow and
verify backing storage to the active frame extent. It no longer reads, moves or
resizes `srWindow`; therefore no raw-frame presentation operation can change a
Terminal's visible rectangle, font fit or scroll position. The existing S7
write rule is deliberately untouched: output covers only the active frame and
one previously committed tail when that tail is larger.

The fake contract smoke preserves a `40x13` viewport at `(7,3)` over a
`120x60` buffer, writes the last cell of an 80x50 frame, and confirms both the
viewport and buffer stay intact. It also rejects both a failed buffer-grow call
and a native call that reports success without applying the requested size.
The native broker display smoke now verifies that preparation leaves its
configured viewport byte-for-byte unchanged while growing storage as needed.

Counted C/H paths against S7 `5d7b2619`: production `+7/-31` (net `-24`);
tests `+34/-27` (net `+7`); combined `+41/-58` (net `-17`). There is no new
source file, API, product branch, Common/App/Core change or original-mirror
diff. The x86 package shrinks by 730 bytes and x64 by 722 bytes. Focused
Console smokes pass on both widths. Complete hidden-background regression
passes x64 121/121 (247.54 s) and x86 121/121 (225.86 s); the standard five
desktop-labelled cases remain excluded there, while the self-owned hidden
native Console smoke passed in each focused run. Lib/test manifests,
documentation governance and whitespace checks pass.

### S8 P2 failure-retry closure

Review of S8 P1 found that `previous_rows` was serving two incompatible roles:
it both identified a fully committed frame and selected the old tail to clear.
Invalidating it before a partial 50-to-25 native write correctly prevented a
stale cache hit, but incorrectly discarded the required 50-row retry extent.

P2 adds one private `coverage_rows` field. `previous_columns` and
`previous_rows` remain the completed-frame cache and are zeroed before a native
frame write. `coverage_rows` instead records the highest row that an attempted
or completed frame may have changed. A retry writes
`max(frame rows, coverage rows)`; only a complete native write reduces that
coverage to the active frame height. On every surface query, if native backing
storage was externally shrunk, surface preparation invalidates the completed
cache and clamps coverage to rows that still physically exist, rather than
inventing a lost tail. This check is deliberately independent of whether the
next frame needs a resize.

The existing test now performs a complete 50-row frame, injects a partial
50-to-25 write, verifies cache invalidation with retained 50-row coverage, and
requires the successful retry to write through row 50 and blank rows 26--50.
It also shrinks the native backing store from 50 to 30 while a 25-row frame
still fits, proving that the next write covers only 30 rows and succeeds.
The Lib README now accurately says backing storage grows while the viewport,
font fit and scroll position remain host-owned. No API, component boundary or
second rendering path is added.

P2 counted code paths against S8 P1 `b8c1ed3f`: production `+24/-10` (net
`+14`) and tests `+38/-0` (net `+38`), combined `+62/-10` (net `+52`). The
extra field is two bytes per broker instance; x86 package size is unchanged
and x64 grows 512 bytes from the compiler's layout/output. Both Release builds
pass. Hidden-background regression passes x64 121/121 (301.17 s) and x86
121/121 (251.51 s); the two focused Console smokes, including the hidden native
Console display case, pass on both widths. Lib/test manifests, documentation
governance and whitespace checks pass. No user configuration or guest media is
changed.

## Request and admitted design

Owner: "收口T83，准入T84进行kvm-*组件的文本帧容量升级 80x50".
The earlier request explicitly includes Lib, Common and App-SoftPC end-to-end
delivery. The active packet is in [Current](../states/CURRENT.md).

Support active extents of 1..80 columns and 1..50 rows, including 80x22,
80x25, 80x43 and 80x50. Storage remains bounded with an 80-cell row stride.
Keep complete copied frames and opaque latest-wins mailboxes. Cells retain
their existing glyph/bank/colour contract; Window owns font bitmaps and
Console owns character mappings. Fonts and graphics capacities are unchanged.
This changes shared structure sizes: consumers must rebuild together.

Separate capacity from default layout. The common status page remains 80x25.
Window derives pixels from active rows and font height. In particular, 50 rows
at eight scanlines are 400 pixels, while 50 at sixteen are 800 pixels; text
surface allocation must be verified independently of the graphics height cap.

Logical Console and the native broker must carry the same bounded capacity.
The broker establishes a surface for the active extent, preserves its existing
no-shrink policy, and clears formerly occupied cells when a smaller frame
replaces a larger one. Successful native output must cover the requested
rectangle before its completed-frame cache is updated. A host that cannot
provide the requested surface returns an explicit failure, not clipped success.
Cooked screen ownership/restoration remains with the existing broker.

App-SoftPC copies committed painter dimensions and its existing 80x50 Compat
surface. It must not infer active dimensions from capacity or transient device
registers. Out-of-range dimensions remain unsupported; zero dimensions remain
invalid. Invalid submissions preserve the previous mailbox publication.

## Ordered S plan

S1 is admitted: implement the connected capacity change in one coherent source
delivery, including Lib/Console/broker, Common consumers, App adapter, boundary
and transition tests, manifests, dual builds and background regression.
Keeping the producer and consumers in one S prevents a partially enlarged ABI.
Preliminary estimate: 10--18 production files, +80--180/-50--140 lines;
8--14 test/build files, +180--350/-40--100 lines. These are estimates and will
be replaced with Git counts; mechanical renames are counted explicitly.

S2 is admitted after owner acceptance of S1: perform the whole-task
actual-change and requirement audit, dispose of task-owned scratch, and prepare
T84 closure evidence for owner approval. It does not close T84 itself. No
speculative second implementation or new state machine is planned.

S3 is owner-admitted after S2: inspect the latest NXVM worktree's six shared
roots (`src/lib`, `src/common`, `src/x86`, `test/lib`, `test/common`,
`test/x86`) read-only against SoftPC. It is an import-readiness audit only:
capture revisions and uncommitted state, compare all component content and
contracts, and report whether a later byte-identical import is safe. It must
not edit NXVM, import changes, rebuild packages or reinterpret T84's accepted
80x50 delivery.

## S2 whole-task audit

S2 compares the complete accepted delivery `39366670` with implementation
baseline `9fbf7369`; it makes no product-code change. The audit found one
governance omission only: S1 had evidence in this proposal but no matching
`docs/history/M9-T84-S1-*` record, which prevented a valid continuation packet
from passing the documentation gate. The new history record states only the
already accepted S1 facts. It does not change S1 scope, verification or the
T84 closure decision.

### Requirement and ownership audit

1. `kvm_text_frame_validate()` remains the single shared acceptance boundary:
   zero dimensions are invalid and extents beyond 80 columns or 50 rows are
   unsupported. It retains fixed 80-cell row stride. No consumer infers an
   active extent from storage capacity.
2. `kvm-window` derives text surface height from active rows and font height;
   it retains existing decoded-pixel comparison and cursor handling. No second
   25-row surface or renderer remains.
3. `kvm-console` converts all bounded stored cells, but forwards active rows
   to logical Console. Logical Console validates only the active rectangle
   before its broker sink receives it.
4. The Win32 raw-Console broker creates/validates at least the frame's active
   height, never shrinks an existing surface, and clears all visible rows
   within the 50-row capacity before acknowledging a replacement frame. Thus
   50-to-25 removes stale lower rows without making an ordinary 25-row status
   page enlarge a Terminal to 50 rows.
5. Common's graphics status page explicitly remains 80x25. It is a raw VM
   Console status surface, not the cooked monitor; storage is initialized
   blank through capacity solely so stale data cannot be retained.
6. App-SoftPC's existing Compat surface and driver already use committed
   active geometry, fixed 80-cell stride and 80x50 capacity. Its source checks
   accept 50 and reject 51 without a compensating conversion path.

The capacity sweep covers `KVM_TEXT_ROWS`, `LIB_CONSOLE_TEXT_ROWS`, literal
80x25/2000/1999 uses and `text_rows` throughout Lib, Common, App-SoftPC and
their test roots. Remaining fixed 25-row occurrences are intentional default
fixtures, the explicit Common status page, or native display-test fixtures.
The original Core mirror's `VIDEO_PAGE_SIZE` is a distinct historical host
definition and was neither made a KVM owner nor changed. Test uses of cell
1999 intentionally validate an in-range cell in smaller frames; no production
loop uses it as a capacity limit. No alternate raw-Console writer, hidden
25-row capacity, App/Compat workaround or original-mirror modification was
found.

### Complete delivery accounting

The accepted implementation has exactly 24 changed paths relative to
`9fbf7369`: four production C/H paths (+19/-11, net +8), nine test C paths
(+108/-19, net +89), four manifests, four design/state/proposal documents,
the Lib README and two rebuilt EXEs. This is +401/-54 across all textual paths
plus binary artifacts. The per-file ledger above lists every path and purpose;
no source path is added, deleted or renamed. In particular, there is no CMake,
INI, guest-media, snapshot-format, x86-source or original Core-mirror change.
The x86 EXE's apparent tracked size increase is a toolchain rebuild artifact
except for the separately measured 512-byte capacity increment; x64 size is
unchanged.

### Evidence and remaining limitation

The recorded S1 final serial background evidence is 120/120 on both widths
(x64 371.65 seconds, x86 373.98 seconds), followed by dual-width 18/18
delivery rechecks. Both Release builds passed. S2 reran the six affected
source/test manifests (four total), Lib component DAG/naming/corpus boundaries,
documentation governance and `git diff --check`; all pass. S2 does not rerun
product tests because it changes documentation only. The five desktop tests
per width, Linux runtime and manual real-guest 50-row-mode exercise remain
explicit exclusions.

The pre-existing intermittent x86 `BIOS[0x52]` null dispatch remains deferred
in TODO by owner instruction. It reproduced on unchanged `9fbf7369` and is
not attributed to the 80x50 work. It is the only open risk recorded by this
audit; it is not silently masked and it is not a T84 code change.

## S3 NXVM six-component audit

Audit timestamp: 2026-09-25. SoftPC baseline is `bf1c0a96`; NXVM is checked
out at `735d155a9c47cde4a34d6fdcaf452ffe8e39f6ac`. NXVM has uncommitted changes
only under its MyNES documentation and default INI. Its six shared roots are
clean, so the following comparison is against their checked-out content, not
against NXVM's whole (otherwise dirty) worktree. The most recent NXVM commit
touching a shared root is `b7cbb30a9` (2026-09-25).

Content SHA-256 comparison produces this finite ledger:

- `src/lib`: 109 files in each repository; all 109 are identical.
- `src/common`: 23 files in each repository; all 23 are identical.
- `src/x86`: 14 files in each repository; all 14 are identical.
- `test/common`: 20 files in each repository; all 20 are identical.
- `test/x86`: 10 files in each repository; all 10 are identical.
- `test/lib`: 51 files in each repository; 48 are identical. `CMakeLists.txt`
  and `MANIFEST.sha256` differ only because SoftPC owns
  `audio_native_smoke.c` while NXVM owns `audio_win32_platform_smoke.c`.

Thus 224 of the 227 files in each shared-root set have a byte-identical
counterpart; there are no source-component differences and no missing
non-audio component. Both
repositories' Lib/Common/x86 source and test manifests verify, and NXVM's Lib
component DAG plus Common/x86 corpus checks pass. No build or product binary
was run for this read-only audit.

### The one semantic difference

NXVM commit `b7cbb30a9` replaces the physical WASAPI loopback probe with a
deterministic adapter test. SoftPC's `audio_native_smoke.c` links the public
Audio stream to an actual default endpoint and capture loopback, waits on real
time, and intentionally skips when RDP or the host exposes no usable endpoint.
NXVM's replacement compiles the same production `audio/win32/stream.c` into a
test translation unit after substituting only its Win32 calls. It deterministically
checks format creation, mono/stereo rates, PCM delivery, cancellation, wait
outcomes, every initialization failure and balanced resource release. It links
Types/Base rather than the public `audio` target; `audio_stream_smoke.c` remains
the separate public-stream contract test.

This is not a second production implementation and does not alter Audio API or
runtime behavior. It is a better unit-test seam because it does not depend on a
physical device, mixer, RDP endpoint, wall-clock deadline or audible signal.
It is nevertheless an Audio-only test-policy choice. The owner approves its
verbatim import within S3 on 2026-09-25: replace the physical probe, update its
CMake target and manifest, then prove dual-width behavior and six-root identity.
No Audio production source or public interface changes.

### S3 implementation evidence

SoftPC imports the four files from NXVM shared commit `b7cbb30a9` verbatim:
`test/lib/audio_native_smoke.c` is deleted; the 216-line
`audio_win32_platform_smoke.c` replaces it; `test/lib/CMakeLists.txt` switches
the test target from public `audio` to the deterministic `types`/`base`
translation unit; and `test/lib/MANIFEST.sha256` records the corresponding
hashes. The imported source SHA-256 is
`5D38A50BB3E55D85301C052F33721C2B3D0F1541772A371239D2BA407BA14C85`; the
complete test-lib manifest SHA-256 is
`8A9AA8CC5150CA41881C84FCCA1487D54C72E11E5D4AEE24C436DA4519434A1F` in both
repositories at that pinned shared revision.

Production C/H: +0/-0. Test/build source: +220/-212, net +8 (one 208-line
physical probe removed; one 216-line deterministic adapter test added; the
CMake and manifest substitutions account for the remaining eight changed
lines). No Core, Compat, VM, App, public API, ABI, INI, media or snapshot
change is present.

The focused `library.audio_stream` and `library.audio_win32_platform` tests
pass on both x64 and x86. Full background CTest passes x64 120/120 (250.98 s)
and x86 120/120 (249.05 s); desktop tests remain excluded and no native
endpoint or Linux runtime claim is made. Both package builds complete.
x86 package SHA-256 is
`FEA84DEF15C2A25DC962579087D60F5DC94E11B9E46A792E13B3C3B60408D8A5`; x64
package SHA-256 is
`0EC6096600E1702CE92751F4AEE1BC16C5FD009FF4E6D2DF537A97774672148F`.
All six source/test manifests, Lib DAG, Common corpus, x86 corpus and
documentation governance pass.

NXVM received uncommitted shared-root edits after the S3 audit baseline. A
post-build worktree comparison therefore shows those later NXVM edits as
differences; they are not silently imported by this task. S3's exactness claim
is specifically against the admitted commit `b7cbb30a9`, whose sole
SoftPC/NXVM difference was the four-file Audio test substitution now removed.
The owner accepts S3 on 2026-09-25; T84 remains open.

## S4 admission — NXVM six-component diff and quality audit

S4 is a read-only intake audit. Its frozen universe is every regular file in
SoftPC and NXVM's `src/lib`, `src/common`, `src/x86`, `test/lib`,
`test/common`, and `test/x86`, plus each root's manifest, CMake entrypoint,
README and verifier. For every difference, the audit records the path, file
class, committed versus uncommitted NXVM source, line delta where meaningful,
public-contract effect, quality/boundary result and one disposition:
`candidate`, `reject`, `defer`, or `already covered`. Matching files are
counted by root rather than listed individually.

The audit does not import, build, alter a manifest or modify NXVM. It must keep
the previously admitted `b7cbb30a9` Audio-test import distinct from NXVM's
later worktree edits. It ends with a bounded follow-up proposal only; owner
approval remains required before any candidate is copied.

## S5 admission — complete NXVM six-component import

The owner approves importing NXVM clean revision
`057d8c9aa5edde1bda794a03fb77d07fc8b67bba` on 2026-09-25.  This replaces the
earlier non-Audio intake boundary: Lib Audio production and its deterministic
tests are included with Lib/Common/x86 and all three test roots.  The selected
source universe is `src/lib`, `src/common`, `src/x86`, `test/lib`,
`test/common`, `test/x86`, plus the owner-approved generic test-support file
`test/register.cmake`.

NXVM's former `test/lib/register.cmake` must be relocated to
`test/register.cmake`, and all three test CMake roots must include the shared
parent location.  This is a deliberate test-infrastructure exception, not a
new product component or a `test/common -> test/lib` dependency.  Apply the
same relocation to both shared corpora and update test manifests.

The import includes NXVM's one-byte `lib_bool` contract.  Copied KVM/Console
frame layouts may therefore change; rebuild every SoftPC receiver from source.
Do not retain a legacy Audio path, partial predicate ABI, or alternative test
registration implementation.  Core mirror, Compat, VM, App semantics, INI,
snapshots and guest media remain outside scope unless compilation proves a
strictly source-compatible receiver adjustment necessary.  That condition
stops the task for separate review.

Verify exact shared-root inventory, strict standalone C11 package builds,
both product Release builds, focused Machine/Audio/xasm/KVM tests, full
background CTest on x64/x86, six manifests, Lib DAG, Common/x86 corpus and
negative gates, documentation governance and whitespace.  Report actual
per-category line deltas, any residual difference and both package hashes.

### S5 delivery evidence

All six roots and `test/register.cmake` now compare byte-for-byte with NXVM
`057d8c9aa5edde1bda794a03fb77d07fc8b67bba`: no changed file and no
one-sided path.  The imported shared corpus changes 104 paths, +1177/-1006
(net +171).  The only SoftPC-specific source work is six receiver adjustments
for the imported one-byte `lib_bool` callback/layout contract, +40/-36
(net +4); they preserve existing boolean behavior and add no alternate path.
With documentation, the text total is +1261/-1056 (net +205), plus refreshed
dual-architecture EXEs.

Both Release package targets build.  Full hidden-background CTest passes
120/120 on x64 (235.80s) and 120/120 on x86 (229.96s), excluding the existing
desktop-labelled tests.  Shared sources and tests compile as C11 with
`-Wall -Wextra -Wpedantic -Werror`; the six manifests, Lib DAG/Linux contract,
Common/x86 corpus and negative verifiers all pass.  `git diff --check` passes.
Owner accepted S5 after manual verification on 2026-09-25. Executor
`e2b81d70` is pushed; S5 is closed and T84 remains open.

- x86 SHA256: `F1E748E86FF800ACF4C1BE8FF75B675BC4995CD3FC39752B948D5D32620E8658`
- x64 SHA256: `840347402647A6F4988DCC16A4B22E6E5FF853D5E15E7C78A26331DB74410856`

## S6 admission — product host-boundary failure closure

Owner admits S6 on 2026-09-25 after the post-S5 read-only quality audit.
The work closes five related product-boundary defects without changing the
shared six-component corpus or the preserved `softpc.new` mirror:

1. Make the standalone LPT sink safe after partial/failed CRT writes: never
   append beyond its fixed buffer, and never replay a prefix known accepted by
   `fwrite`.
2. Validate the whole parallel-host snapshot state before allocating or
   replacing any live port; reject invalid inactive/pending and threshold
   combinations and release every provisional allocation on failure.
3. Return the existing audio shutdown result through the VM/App ownership
   chain.  A failed worker join retains all callback-owned state and keeps the
   singleton admission closed; a completed shutdown may release its machine
   even if native stream disposal reports an error.
4. Recognize `#` and `;` only outside quoted INI values.
5. Remove the always-on T28 prompt trace and its stale repository-relative
   output path.  It is no longer an acceptance input or a product feature.

The implementation must use the existing ownership paths, fixed LPT buffer,
audio task and product terminal-cleanup rule.  It must not add a second printer
queue, retry worker, configuration parser, trace switch, Lib/Common change,
mirror diff, or guest-media mutation.

Focused tests cover LPT partial/failure capacity and snapshot invalid-state
rejection, audio join failure ownership, and quoted INI comment markers.
Run both Release package builds, their focused tests and the complete
background CTest suites on x64/x86, all documentation/manifest/boundary gates,
and whitespace checking.  Report production/test/docs add/remove/net counts,
the actual changed-path ledger, package hashes, test counts and exclusions.

### S6 delivery evidence

Owner manually accepted S6 on 2026-09-25. Executor `13b4aef3` is pushed;
this acceptance closes S6 only and leaves T84 open.

- Parallel output now preserves only the unconfirmed suffix after a short CRT
  write, uses overlap-safe movement, and refuses a further byte when that
  suffix occupies the fixed buffer. A successful full write is never replayed
  merely because the subsequent flush fails.
- Parallel snapshot restore validates every serialized port before allocating
  any replacement buffer; inactive buffered data and invalid active thresholds
  are rejected without partially installing state.
- Audio worker/stream teardown has one result path: failed join or stream
  disposal retains the live dependency and prevents VM admission from opening.
  The App reports the terminal teardown failure after its normal Common
  lifetime has ended.
- The INI scanner recognizes `#` and `;` only outside quotes. The permanent
  repository trace hook and its two source files are removed rather than kept
  as a dormant alternate path.
- Actual text change, including the required S5 history repair: application
  production +80/-154 (net -74), build/checks +14/-3 (net +11), tests +140/-7
  (net +133), documentation +106/-16 (net +90): total +340/-180 (net +160).
  Two package binaries are refreshed. The task adds no Lib/Common/x86/Core-mirror source,
  user configuration, snapshot or media change.
- Final Release builds and hidden-background CTest pass 121/121 on x64
  (265.78s) and x86 (265.42s). Focused parallel-failure smoke and documentation
  governance also pass on both widths; desktop-labelled tests remain excluded.

## S7 admission — DOS startup cursor-position investigation

The owner reports that, after DOS starts, its cursor appears one row below the
expected original position. S7 is deliberately read-only: trace the copied
cursor value from the original text/video producer through SoftPC VM and Common
to the KVM Window and raw Console consumers. The inquiry must distinguish a
guest-frame cursor from the cooked monitor's native cursor and must not invent
a compensating `-1` at any layer.

The deliverable is a causal ledger naming the first divergent row, the affected
presenter(s), and a smallest justified repair owner. It also classifies adjacent
cursor conversions so a follow-up repair can be uniform rather than scenario
specific. No source, package, configuration, media, manifest or test change is
authorized. If source inspection cannot prove the origin, stop and request a
bounded, user-coordinated reproduction rather than disturb the desktop.

### S7 investigation result

The first static candidate was the Win32 `CONSOLE_SCREEN_BUFFER_INFOEX`
rectangle conversion. It is not a valid repair: the existing native display
smoke proves that passing the saved `srWindow` unchanged makes a 30-row cooked
viewport restore as 29 rows. The broker's explicit right/bottom conversion is
therefore required by the actual setter behavior and must remain. No source
change has been retained from that rejected hypothesis.

The source ledger still proves that the guest cursor `(x, y)` is passed
unchanged through Compat, VM, Common and both KVM consumers; no `+1` or `-1`
coordinate conversion exists in that chain. The Window path is a same-frame
control and remains correct. The remaining inquiry must distinguish an
incorrect native cursor coordinate from a native Console viewport/cursor
rendering observation with a bounded runtime probe; it must not guess a
coordinate correction from source inspection alone.

The owner-supplied Windows Terminal capture confirms that the visible cursor is
on the following *guest* row, rather than merely above a larger host viewport.
A disposable x64 package probe was therefore run through the shipping raw
Console route: after cold boot and again after `cls` followed by `dir`, it read
the active native buffer and cursor and required the cursor to be immediately
after the final `A:\\>` or `C:\\>` on the same row. Both observations passed.
This excludes the copied guest-coordinate chain for the tested Conhost route,
but does not reproduce the Windows Terminal rendering observation. The probe
was removed: it is not a suitable permanent regression until it can reproduce
the reported host-specific failure.

A second disposable ConPTY probe then exercised the actual x64 package through
`start` and `dir`, and captured its terminal stream. Its final cursor operation
was the absolute `CSI 23;5H`: row 23, column 5, immediately after the DOS
prompt. A minimal 80-column `WriteConsoleOutputW` plus
`SetConsoleCursorPosition` control case likewise emitted the requested absolute
coordinate. This rejects the proposed delayed-wrap explanation as well. No
product layer has yet been shown to publish a wrong coordinate; the supplied
Windows Terminal rendering remains unreproduced by the native Console and
ConPTY protocol observations, so S7 must not apply a speculative coordinate
shift.

### S7 approved bounded repair

Comparison with pre-S1 raw Console behavior identifies one justified output
boundary change: before S1, every normal DOS frame wrote exactly 25 rows. S1
correctly added 50-row capacity, but selected each write extent from the host
viewport height (bounded at 50). Consequently a 30-row terminal receives five
extra blank rows for every unchanged 25-row DOS frame. The cursor value itself
remains correct, but this changes the terminal update shape relative to the
previous product behavior.

The repair stays entirely in the Win32 Console broker. Surface establishment
continues to ensure the active frame is visible and never shrinks a host
viewport. The native write extent instead becomes the larger of the current
frame row count and the prior successfully committed frame row count. Thus a
steady 25-row frame writes 25 rows regardless of a taller Terminal viewport;
a 50-to-25 transition writes 50 rows once and clears only the old tail. The
existing fixed 50-row storage remains the bound. This neither changes a cursor
coordinate nor introduces terminal detection, product policy, a second output
path or a public API.

### S7 execution evidence

The broker now has one output-extent rule: after surface preparation, write
`max(frame.rows, previous committed rows)`. `previous_rows` is reset on every
native partial-write, raw text-write or surface-replacement invalidation, so it
never claims an unconfirmed tail. The committed current row count then becomes
the sole next-frame tail bound. The surface/viewport rule remains independent:
it guarantees that the active frame is visible but does not dictate how much
frame data is sent to a host.

The focused fake starts with an 80x30 host buffer and viewport. It proves a
steady 80x25 frame writes exactly rows 0--24, and the active matrix
`22,25,43,50,25,50` writes `25,25,43,50,50,50`; every row outside the active
frame and inside that one prior-coverage tail is blank. The native display
smoke was adjusted only for the removed private helper output parameter and
still proves viewport restoration/failure behavior.

Similar-output sweep searched every Win32 `WriteConsoleOutputW` call and every
`previous_rows` producer/reset in the broker. This is the sole raw text-frame
write path. The cooked text writer invalidates the same cache before its direct
CRT output; activation, native shrink and failed/clipped frame writes reset it.
No Window, Common, App or Core presenter has a competing host-row-derived
write extent.

Counted C/H changes are production `+9/-6` (net `+3`) and tests `+10/-9` (net
`+1`): combined `+19/-15`, net `+4`. The retained comment states the one
non-obvious boundary distinction. Manifests, task documents and refreshed
dual-width packages are tracked separately. Focused x64 tests pass 2/2;
background CTest passes x64 121/121 (215.37 s) and x86 121/121 (270.27 s),
with desktop-labelled tests excluded. Documentation governance and whitespace
checks pass. Manual Windows Terminal confirmation remains the owner acceptance
step; S7 and T84 stay open until then.

## Frozen coverage ledger and exit evidence

Each member below must finish with a source disposition and focused test proof:

1. Base text cell storage, extent validation and fixed stride.
2. Window text copy, fonts, rendered pixels, surface allocation and cursor/dirty
   rectangles, including same-pixel-size 25x16 to 50x8 transitions.
3. KVM Console mapping and logical Console validation/storage.
4. Native broker buffer/viewport, completed cache and shrinking active coverage;
   partial writes and host geometry failure must not acknowledge success.
5. Common frame comparison/forwarding and independent 80x25 status layout.
6. App-SoftPC surface-to-frame adapter, last row and actual geometry reporting.
7. Shared tests, product integration tests, manifests and documentation.

Test 22/25/43/50 rows, smallest extents, last cell/cursor, 50-to-25-to-50,
font heights 8/14/16, zero dimensions, 81 columns and 51 rows. Assert no
out-of-bounds access, no stale lower rows and no invalid-frame publication.
Use native API fakes for geometry failures and background-compatible tests;
do not disturb the user's desktop without arranging the native test session.
Run both Release builds, required background suites, shared manifest/DAG gates
and documentation governance. Deliver both EXEs with actual added/deleted/net
counts and disclosed exclusions. Preserve INI, snapshots and guest disks.

Search all text constants and numeric capacity uses across src/lib, src/common,
src/app-softpc and corresponding test roots; classify each as capacity,
active geometry, fixed default, font/pixel calculation or fixture. The original
mirror is read-only in this task. A required mirror algorithm or snapshot
format change stops implementation for a separate scope decision.

## S1 implementation audit

Status update, 2026-09-25: owner directs recording the pre-existing BOP fault
in [TODO](../states/TODO.md) and not repairing it now. That delivery blocker
is lifted; the investigation and earlier blocked status below are retained as
evidence. Owner accepts S1 after P1/P2 delivery and manual verification.
S1 is closed; T84 remains open because S2's whole-task review is unadmitted.
The deferred fault is not fixed.

P1 `503d6632` is pushed. Coordinator actual-change review checks all 24 paths
against the seven-member coverage ledger and original scope; no production
or test edits follow P1. This P2 records delivery/review only, not acceptance.
The package hashes still match the verified artifacts; INI/media and mirror
remain unchanged. S2 and T closure await the owner.

Diagnostic continuation: export baseline9fbf7369 sources into ignored
`build/t84-baseline/`, build only x86 runtime-smoke, and compare its failure
with the modified build. Executor owns cleanup. No guest media is imported;
the test creates its own512-byte fixture. Limit each diagnostic batch to100
processes/5minutes, each build to15minutes, logs to20MiB and retain only the
causal checkpoint below. Baseline sources remain unmodified. This is read-only
regression attribution, not admission to change the original CPU algorithm.

### Bounded text capacity implementation

Baseline: `9fbf7369`; owner request and frozen coverage ledger are in the
[proposal](../proposals/m9-kvm-text-80x50.md). This is S1 evidence, not T closure.

### Actual implementation and ownership

Four production paths change: `src/lib/kvm-base/frame_interface.h`,
`src/lib/console/console_interface.h`,
`src/lib/console-broker/win32/console.c`, `src/common/ui/ui.c`.
The two public capacities become 80x50. Active dimensions and fixed stride80
remain distinct. Broker establishes the requested active height, preserves
native viewport dimensions, writes and clears its visible bounded rows, then
commits the cache only on a complete native write. Common's status page stays
80x25. No new object, state flag, queue, rendering path or allocation is added.

App-SoftPC already copies a committed painter extent from an 80x50 Compat
surface. Its existing capacity check/copy now accepts the enlarged bound.
Window already allocates/render-scales the active pixel extent: 50x16 is 800
pixels high, independent of the graphics limit. Neither requires a new path.
No App production, original mirror, media, INI or snapshot format changes.

Base text, Window text and Console text structures each grow by 8,000 bytes;
their new sizes are 16,084 / 24,276 / 17,108. Logical Console frame is 16,084.
Window's graphics-dominated union remains 984,084 bytes. All consumers rebuild
together; this is not binary compatibility with an old shared-library build.

### Frozen ledger disposition

1. Base: capacity/validation/copy proven by `library.kvm_frame_copy`; 51 rows,
   81 columns, zero dimensions and invalid last-cell attributes reject.
2. Window: `library.kvm_frame_damage_mouse` covers rows22/25/43/50, all font
   heights0..16, guarded full raster bounds, 25x16-to-50x8 same-size damage and
   last-cell cursor. Existing native DIB allocation has no graphics-height cap.
3. KVM Console: `library.kvm_console_retirement_barrier` proves last-cell
   secondary-glyph mapping and colours, plus invalid-frame retention;
   `library.lib_console` proves the enlarged logical frame ABI.
4. Broker: `library.lib_console_io_contract` captures last-cell output and
   blank lower rows for 22/25/43/50/25/50, normal25 startup, resize rejection,
   clipped write failure and cache behavior. Native desktop test call sites
   are updated and built but not executed in the background preset.
5. Common: `common.machine_wait` proves last-cell publication/change detection;
   `common.composition` proves50-row forwarding and independent25-row status.
6. App: `softpc-vga-frame-smoke` exercises the real driver against committed
   painter dimensions22/25/43/50/25/50 and last-cell source data;51 rejects.
   This is a producer-boundary fixture, not a real guest selecting all modes.
7. Shared manifests and UI/Lib contract documentation are updated.

### Similar-issue sweep and accounting

Searched text bounds and numeric assumptions with `rg` over src/lib,
src/common, src/app-softpc/machine, src/app-softpc/compat and their tests:
`TEXT_ROWS`, `text_rows`, `80x25`, `2000`, `1999`, and old structure sizes.
Capacity arrays/loops follow the shared constants. Compat's50-row capacity
already matches. Its initial80x25 native-emulation geometry is an intentional
mode default, not a storage limit. Common status25 is retained explicitly.
Other numeric hits are timeouts, input counts, palettes, sound frequencies,
register offsets or tests of intentionally smaller active extents.
No remaining in-scope alternate capacity owner or compensating renderer.

Reproducible count: `git diff --numstat 9fbf7369 -- src test`, C/H paths only.
Production four files +19/-11, net+8. Tests nine files +108/-19, net+89.
Combined thirteen code paths +127/-30, net+97. Documentation, four manifests
and two generated EXEs are excluded. Initial estimate was deliberately wider;
existing generic consumers avoided additional production changes.

### Verification

Final background reruns pass x64 120/120 (371.65s) and x86 120/120
(373.98s), serial within each width. Both runtime-smoke and the Win3.1 restart
roundtrip pass in these runs; roundtrip times are 55.95s and 55.27s.
These results prove this execution of the suite, not the absence of the
intermittent failures recorded below. No failure was suppressed or timeout
relaxed. Documentation governance and `git diff --check` also pass.

Delivery recheck, 2026-09-25: both complete Release build presets finish
successfully. Focused capacity/forwarding/Console tests, six manifests,
Lib DAG, Common/x86 corpus and documentation checks pass 18/18 per width
(x64 14.41s, x86 13.96s). Production/test sources and EXE hashes are unchanged
from the final full regression above. Five desktop tests remain excluded.

Both MinGW Release builds pass. Initial background runs finish x64 119/120
(398.83s), x86 118/120 (393.57s). The stale size assertion was corrected and
passes focused reruns on both widths. No closure claim is made. Five desktop
tests are excluded on each width;
no Linux runtime or real guest50-row manual acceptance is claimed.

A later x64 parallel rerun finished119/120 (163.69s), failing the restart/Win3.1 prompt
roundtrip wait (graphics frame, no expected prompt within its deadline).
The initial serial run passed that test; concurrency/timing versus a real
regression is not yet resolved. This is additional unaccepted evidence,
not a reason to relax the timeout or change guest rendering.

Initial regression exposed a stale test assertion for the old Console frame
size; it was corrected and the focused test passes on both widths. One x86
runtime smoke access exception passed fifty subsequent native repetitions,
but recurred on attempt70 in a later serial GDB run. The executor calls address0
from ccpu.constprop -> c_cpu_simulate -> softpc_machine_run -> vm_driver_run
-> common_machine_worker. A second current-build GDB batch fails on attempt89
with ECX=ESI=0x52. Disassembly identifies the call as BIOS[0x52].
Unmodified baseline9fbf7369, independently exported and built with the same
x86 compiler/Release flags, reproduces the same null call and index on
attempt28. Thus the failure predates the80x50 changes; its guest trigger and
repair remain unresolved. This initially activated the packet's failure stop
condition. On 2026-09-25 the owner explicitly deferred that pre-existing fault
to TODO and authorized S1 delivery without repairing the CPU/BOP path.
An overlapping diagnostic invocation produced a fixture-open failure; that
invocation is invalid evidence, and subsequent diagnostics run serially.
The diagnostic baseline export/build was removed after recording its causal
checkpoint and binary comparison, confirming no process used that exact
task-owned directory. Source control can reproduce the unmodified baseline;
no user media or normal build tree was removed.

Artifacts:

- x86 SHA256: `E25D3DBBF2281EB0B8C9049A5406290E4272C0334DD027831FD154D56EFCC0E8`
- x64 SHA256: `F42C8BB536094FC4FE9BEFB7333CE9A164C8D3CC0103CB99DBE24C395089D8C4`

Owner accepted S1 on 2026-09-25. T84 remains open for its planned but
unadmitted S2 whole-task review.

### Binary footprint comparison

The tracked x86 EXE grows3548935 ->3709303 bytes. A clean build of unchanged
baseline9fbf7369 using the current compiler produces3708791 bytes: the actual
capacity-change increment against that build is512 bytes, not160368.
The larger remainder comes from rebuilding the unchanged baseline with the
current build environment; it is not newly added T84 logic. x64 remains
3094766 bytes. Expanded frame storage is runtime memory, not an8KB executable
payload; structure sizes are listed above.

### Per-file review ledger

This ledger compares the S1 implementation with `9fbf7369`. The pushed
implementation commit is identified in the delivery report and Current.
Use `git diff 9fbf7369 <implementation-commit> -- <path>` for each complete
patch; binary changes are identified by SHA256 and size, not text line counts.

Production changes:

- `src/lib/kvm-base/frame_interface.h`: +3/-2, net +1. Change the shared row
  capacity from 25 to 50; document that storage capacity is not visible size.
  Cell format, stride, fonts and validation semantics remain unchanged.
- `src/lib/console/console_interface.h`: +2/-1, net +1. Match logical Console
  storage to 50 rows; active dimensions remain per-frame fields.
- `src/lib/console-broker/win32/console.c`: +12/-6, net +6. The existing
  surface helper accepts active rows and returns bounded visible write rows.
  Buffer/viewport establishment, fill loop and native completion rectangle
  now agree on that extent. Preserve no-shrink behavior and clear old lower
  rows without forcing every 25-row frame to open a 50-row viewport.
- `src/common/ui/ui.c`: +2/-2, net 0. Keep the fixed status layout at 25 rows
  and bound status text against its active extent rather than frame capacity.

Test changes:

- `test/app-softpc/unit/machine/vga_frame_smoke.c`: +21/-2, net +19. Exercise
  real driver copying at 22/25/43/50 rows and the last cell; move unsupported
  height to 51. Completed 50-row transitions now succeed; incomplete ones
  retain the existing no-frame behavior.
- `test/common/composition_smoke.c`: +5/-1, net +4. Assert 25-row status and
  50-row guest text forwarding, including cell 3999.
- `test/common/machine_wait_smoke.c`: +11/-0, net +11. Detect a change at the
  last cell and suppress an identical subsequent publication.
- `test/lib/console_broker_display_smoke.c`: +4/-3, net +1. Adapt the three
  existing native geometry helper calls to the explicit 25-row requirement.
  Compiled only; this desktop test was not run in background verification.
- `test/lib/kvm_console_retirement_barrier_smoke.c`: +7/-1, net +6. Reject
  51 rows and prove final-cell glyph-bank and colour conversion at 50 rows.
- `test/lib/kvm_frame_copy_smoke.c`: +6/-6, net 0. Update three structure-size
  assertions and validate attributes at the enlarged last cell; unchanged
  graphics-union size remains asserted.
- `test/lib/kvm_frame_damage_mouse_smoke.c`: +23/-3, net +20. Expand guarded
  raster coverage, verify equal-pixel-size grid transitions and bottom-right
  cursor geometry; no production mouse behavior changes.
- `test/lib/lib_console_io_contract_smoke.c`: +29/-1, net +28. Capture native
  output cells/rectangle, verify the row transition matrix and blank lower
  rows, and reject failed viewport expansion without acknowledging output.
- `test/lib/lib_console_smoke.c`: +2/-2, net 0. Update the frame ABI size and
  unsupported row boundary from 26 to 51.

Remaining changed paths:

- `src/lib/README.md`: +5/-2, net +3; describes active rows and bounded
  clearing instead of a universal 25-row minimum.
- `docs/design/UI.md`: +4/-0, net +4; records capacity, fixed stride,
  independent status default and explicit unsupported results.
- `docs/proposals/m9-kvm-text-80x50.md`: P1 +219/-0; implementation review,
  verification failures and this ledger. P2 adds the delivery review record.
- `docs/states/CURRENT.md`: P1 +9/-1; delivery status and full file/diff
  reporting. P2 updates that status to pushed and awaiting owner verification.
- `docs/states/TODO.md`: +7/-4, net +3; records the owner-deferred existing
  x86 BOP crash and its separate investigation admission condition.
- `src/lib/MANIFEST.sha256`: +4/-4, net 0; hashes for the three changed Lib
  production files and Lib README only.
- `src/common/MANIFEST.sha256`: +1/-1, net 0; UI source hash only.
- `test/lib/MANIFEST.sha256`: +6/-6, net 0; the six changed Lib test hashes.
- `test/common/MANIFEST.sha256`: +2/-2, net 0; the two changed Common tests.
- `assets/binary/softpc32.exe` and `assets/binary/softpc64.exe`: rebuilt
  artifacts; hashes and the controlled footprint comparison are above.

Total: 24 changed paths, including 13 C/H paths. No new source file,
renamed/deleted path, build-system change, INI edit, guest media change,
snapshot-format change or original mirror diff belongs to this implementation.
