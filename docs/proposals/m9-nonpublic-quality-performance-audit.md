# Non-public Code Quality And Performance Audit

## Admission and boundary

Owner admits T98 on 2026-10-18:
"准入一个新的t任务 帮我检验非公共组件的代码质量和性能问题（不包括镜像代码，但包括镜像diff）".
Audit baseline: 4c8e185b. CURRENT owns current task status.

S1 audits src/app-softpc/product, machine and compat; test/app-softpc;
repository-root build/test assembly and tools; reviewed local changes against
the pinned XP necessary subset and their ABI/caller joins. The eight reusable
src/test trees are outside the audit except as read-only interface context.
Unchanged original mirror implementation is outside scope. Local subset
exclusions are provenance, not automatically defects or removable patches.

S1 changes documentation only. No rebuild, EXE refresh, guest changes or
implementation repair is implied by this audit. Findings will propose minimal
owner-local corrections; the owner decides follow-up S tasks.

## Finite convergence ledger

The frozen unit of coverage is each of these six responsibilities, supported
by a tracked-path inventory and direct source/caller/test review. Dispositions
are confirmed defect, performance opportunity, existing debt, no finding in
reviewed path, or explicit verification limitation. Closure requires every
area's disposition and evidence; this is not exhaustive proof of all behavior.

1. Execution, input and debugging: lifecycle, wake, locks, instruction callbacks.
2. Presentation: readiness, pixel/text conversion, frame ownership and copies.
3. Audio: speaker, PCM, MIDI, completion/failure and snapshot interactions.
4. State and media: save/load transactions, stream/archive validation, disk I/O.
5. Product: configuration, composition, CLI extensions and failure reporting.
6. Qualification/tooling: tests, dependencies, build targets and mirror patches.

Performance claims must name frequency and repeated work. Source estimates
are not measured CPU/latency improvements. Existing TODO entries retain their
status; rediscovering them does not establish a new regression.

## S sequence

- S1 admitted: inventory, source audit, relevant checks, findings and minimal
  remedy estimates, actual-diff review and pushed report for owner review.
- Owner approved established repairs on 2026-10-18: 批准执行当前既定修复任务.
- S2: F1/F2 real media adapter failure and capacity bounds; owner accepted and
  closed on 2026-10-18. See [closure](../history/M9-T98-S2-media-boundaries.md).
- S3: finite instruction budget rejection, no hot-loop counter widening.
- S4: Compat-private UTF-8 append opening, retaining FILE write/flush/close.
- Each S builds/releases/tests/commits/pushes and waits for acceptance before
  the next admission. Performance profiling follows correctness by separate admission.

## Findings and verification

S1 report delivered on 2026-10-18. No production or test modification has been
made. Findings below are against 4c8e185b; none is claimed to explain an owner's
specific historical symptom without a matching guest reproduction.

### Coverage and proof limits

Tracked inventory contains 139 paths: 71 App production/header/resource files,
52 App test/fixture files, 14 tools and two root CMake files. The frozen mirror
manifest contains 505 paths; 131 have local patches. Independently reading the
fixed XP objects and applying the recorded necessary-subset ranges reproduces
all 131 subset hashes. Diff statistics are +27832/-25922, net +1910, against
those subsets, not against whole XP files and not changes made in T98.

All admitted paths received inventory/caller/pattern screening. Direct detailed
reviews concentrated on the six chains below and the changed CPU, C-VID,
display, input, DMA/audio and archive seams. Hash reconstruction is provenance
proof, not a claim that every line of all 131 patches was semantically proved.
Unchanged original-core algorithms and the eight public trees were not audited.

| Responsibility | Disposition |
| --- | --- |
| Execution/input/debug | Budget truncation confirmed by source; existing idle waits preserved; per-instruction host calls are a profiling candidate. |
| Presentation | Direct leaf production/four-slot transport retained. Mouse-triggered full refresh/full RGB conversion is an optimization candidate; obsolete backing-stride comment noted. |
| Audio | Reviewed PCM conversion, MIDI cancellation/volume and speaker ownership with DSP completion seams. No new confirmed audio defect; unsupported SB MIDI remains existing TODO. |
| State/media | Close failures and HDD capacity overflow confirmed with production-code isolation probe. Existing physical-address debt independently rechecked. Snapshot hashing/full scan is explicit cost. |
| Product | Config/snapshot extension path reviewed; COM/LPT endpoint path encoding still bypasses the existing UTF-8 file abstraction. |
| Tests/build/mirror | Dual cached unit/gates pass, missing adapter-close/budget/large-image cases identified. Mirror provenance and selected high-risk patches reviewed; no full semantic or performance qualification claim. |

### F1 - P2: media retirement can falsely report success

`src/app-softpc/compat/hdd_media.c:174` consumes the old lease with
`lib_storage_medium_destroy()` but ignores its status, clears metadata and
returns OK. Replacement similarly ignores retired-close status at line 184.
`compat/gfi_image.c:348,404,422` has the analogous attach/restore pattern.
Lib's close contract explicitly consumes the lease even on I/O failure, so
retaining a stale lease or retrying its close is not the remedy.

An isolated x64 probe includes the actual hdd_media.c and injects IO_ERROR from
destroy. The real restore-detach path clears the medium and returns OK. This
is dynamic proof of the adapter defect, not a native disk-close reproduction.
The archive unit replaces both real adapters with a test implementation that
propagates close status (`test/app-softpc/unit/compat/media_snapshot_smoke.c:17`),
so its successful result cannot qualify the real failure path.

Minimal remedy: finish the existing mandatory metadata/ownership cleanup,
return its close failure through existing attach/restore results, and cover
the real FDD/HDD adapters. No retry, rollback or new lifecycle state. Review
void shutdown callbacks separately rather than pretending they can return a
new result without changing their original contract. Estimate: production two
files +12-24/-8-16, net +4-8; focused existing-owner tests +45-80/-0.

### F2 - P2: accepted HDD size exceeds its address representation

`compat/hdd_media.c:46,113,186` accepts file length, stores sector count in
IU32, and later calculates `total_sectors * 512u` in 32 bits. A 4-GiB medium
has 8388608 sectors; its computed byte extent wraps to zero. The actual
production-code probe verifies attach succeeds and a one-sector read at
offset zero is rejected. Its medium is mocked; no multi-gigabyte file was
created or guest disk altered.

The original host offset is a signed int. Widening one multiplication would
not make the whole controller support arbitrary large images. Minimal remedy:
establish the actually addressable sector-aligned bound, reject unsupported
attachments explicitly, and calculate transfer bounds in lib_size with checked
subtraction. Apply the same validation to restoration. Do not change geometry
to hide truncation or add another disk backend. Estimate: one production file
+8-18/-3-6, net +5-12; boundary tests +30-50/-0. No mirror edit expected.

### F3 - P2: a finite instruction budget can become unbounded

`machine/machine.c:350` narrows a lib_u64 budget to Windows unsigned long.
For 0x100000000 it stores zero and enables the finite-budget flag.
`compat/platform.c:159` treats zero as nothing to consume, so this request
never stops through its instruction budget. UINT64_MAX is separately handled
as continuous execution; normal App execution uses that sentinel and is not
shown broken by this finding. Existing tests exercise small finite budgets.

Minimal remedy: reject finite budgets beyond the existing 32-bit counter before
entering execution; document the limit and preserve the continuous sentinel.
No 64-bit per-instruction counter or new scheduling path is needed. Estimate:
machine.c/machine.h +3-6/-0; existing machine test +10-20/-0. Source proof only;
an intentionally unbounded guest run was not launched to demonstrate a hang.

### F4 - P2: COM/LPT file paths have inconsistent encoding

Product reads paths from the same config used for media and resolves them
relative to the UTF-8 executable directory. Storage treats those paths as
UTF-8. `compat/serial.c:155` and `compat/parallel.c:180` instead pass them to
narrow fopen. On Windows where the CRT ANSI code page is not UTF-8, non-ASCII
paths can fail or identify a different name; ASCII paths remain unaffected.
This is a source-established encoding mismatch, not a claim that every host
configuration fails. Current endpoint tests use ASCII paths.

The initially suggested Storage-writer migration is withdrawn after the
owner-requested follow-up below: flush is not its only missing contract.
LPT also needs the exact partial-write count. Adding just writer_flush would
not preserve its already-tested unwritten-tail behavior.

Recommended narrow candidate: retain the existing FILE write/flush/close
paths and change only UTF-8 path opening through one Compat-private append
helper shared by COM/LPT. The existing Windows-only host can convert the
bounded 1024-byte path to UTF-16 and use _wfopen in binary append mode.
This deliberately retains a small native path conversion at the private host
boundary rather than expanding the public writer ABI or replacing its I/O
semantics. It does not claim all file opening is unified with Storage.
Estimated production: platform.c/platform.h plus two call sites, +10-16/-2,
net +8-14; focused existing endpoint tests +20-35/-0. Public and mirror delta
zero. The helper has been validated only in an isolated candidate probe;
production implementation and product regressions are not yet admitted.

### Performance candidates - evidence of work, not measured speedup

1. `compat/cvidc/gdp_state.c:107` linearly searches slot records on each access.
   The mirror contains 22603 textual rule-slot call sites. Mapped VGA members
   also search for their aggregate carrier. This is local ABI adaptation cost,
   not an inherent XP device algorithm. Call-site count is not a dynamic call
   count. Measure slot count, hit position and time in a controlled VGA workload
   before choosing an owner-local lookup improvement. Do not replace the C-VID
   implementation or introduce a second fast rendering path.
2. `machine/machine.c:286` requests an original full refresh after each mouse
   delivery. `machine/driver.c:304` resolves every pixel of a produced graphics
   frame, even if source damage is small; Window damage comparison happens
   later. A 1024x768 frame writes 3 MiB of RGB payload; at 20 produced frames/s
   that alone would be 60 MiB/s, excluding reads. This is arithmetic, not a
   measured bandwidth bottleneck. Removing the refresh without proving the V7
   software-pointer path would reintroduce missing cursor updates. First measure
   refresh, conversion and paint separately. Complete immutable frames remain
   required; no partial-frame ABI or history cache is proposed.
3. `compat/platform.c:115,174` uses locked compare/exchange for pending/pacing
   reads; local c_main.c hooks call into this seam at instruction/quick-loop
   boundaries. Debug and snapshot observers also have inactive fast returns.
   These calls warrant instruction-throughput profiling, not unconditional
   deletion. Control/stop/HLT wake and snapshot semantics must survive any change.
4. `compat/media_snapshot.c:verify_base/capture_slot` hashes complete base media;
   overlay capture reads both base and effective data in 4-KiB blocks. Load
   validates before mutation and again after reopening the saved path. This is
   substantial save/load I/O, not idle CPU. SHA validation and re-open validation
   are distinct obligations. No unsafe hash cache or removed validation is
   recommended merely to reduce reads.

Performance changes have no defensible implementation line estimate until a
bounded profile selects the actual bottleneck. A follow-up profiling S can
change zero production lines and use task-local probes. Avoid a speculative
buffer/cache/index framework based solely on these source counts.

### Existing debt and reviewed non-findings

Compat memory.c still routes its named physical API through c_sas_loads/stores;
the existing High TODO already owns this. Debug memory uses explicit translation
and physical access, and sound DMA uses physical SAS. Preserve that distinction.
The 38h SB MIDI parser, package/readiness and Ninja recovery debt are not fixed
or rediscovered as new regressions here.

`compat/dib_surface.c:18` still explains enlarged text storage using the old
physical-stride fast path, although nt_cga now enforces fixed target stride.
Correct that comment alongside a future presentation change; an allocation
shrink needs separate writer-bound evidence, not just deletion of the comment.

Early reset failure can leave process-global initializations before
hardware_initialized becomes true; normal destruction is guarded by that flag.
This is a cleanup review lead, not a proven user leak or reason to add retry
states. Current terminal machine failure policy and native ownership must be
checked before proposing recovery semantics.

No new second executor, steady-state whole-frame Session copy or per-frame
allocation was found in reviewed production paths. Current slot ownership,
direct leaf production, body-ready publication and retained Console restoration
remain distinct justified responsibilities. PCM/MIDI completion and cancellation
were reviewed against existing mocked tests; a unit pass is not native-device
availability or audible playback proof.

### Verification and delivery

Existing cached x64 and x86 binaries each pass 56/56 selected tests: 43 App unit
cases and 13 root gates, zero failures/skips. Selection:
`-R '^(app-softpc/unit/|checks/)' -LE 'desktop|native_audio' -j 8`.
No test binary was rebuilt and no full integration/desktop/audio rerun is
claimed for this read-only audit. Reports/logs and the final assertions-enabled
x64 HDD probe are under build/t98/s1; run.exit and mirror.exit are zero.
The initial scratch probe omitted stdio, and an intermediate Release probe had
NDEBUG inherited; those attempts do not establish the capacity result. The
final -UNDEBUG probe executes the real attach and validates pointer/size/IU32
widths 8/8/4, 8388608 sectors and rejected first-sector transfer.

No production/test/mirror changes: +0/-0, net zero. EXEs, owner INI/media and
snapshot.debug remain untouched. Documentation gate and actual-diff review
qualify this report; they do not qualify hypothetical fixes.

Suggested follow-up ordering, subject to admission: fix real media adapter
failure and size boundaries; fix bounded-run contracts; fix endpoint path
opening without migrating the write contract; then profile the three runtime
candidates before implementing performance changes. Unchanged mirror
algorithms stay outside those suggested repairs.

## Actual-commit review

Coordinator role reviewed executor commit 849709b5 against the owner request,
packet, real adapter probe, both 56-test XML reports and 131 reconstructed
subset identities. Its three changed paths are documentation only, +292/-5;
production/tests/mirror remain +0/-0. Package hashes match T96. Source-only,
injected, provenance and performance evidence are distinguished; F4 explicitly
retains the public-flush decision instead of promising an invalid direct swap.
Documentation governance and whitespace checks pass. S1 awaits owner review;
T98 remains open and no repair subtask is automatically admitted.

## Owner-requested COM/LPT flush follow-up

Owner asks why other file handling does not need a new flush API, then requests
research before proceeding. No new S or production change is admitted.

Source/history establishes these distinct boundaries:

- Fixed XP nt_com.c submits the normal host byte through WriteFile, waiting
  for pending completion there; it does not use CRT fflush or FlushFileBuffers.
  Its existing native batching route is not the standalone file endpoint.
- The optional standalone synchronous append endpoints were introduced in
  d82389aa7 (M7 T4 S1). That commit introduced serial fputc plus fflush, not
  an XP hardware requirement. Original XP LPT has its own buffer and submits
  a batch through WriteFile; current Compat keeps its 1020-byte threshold.
- Current serial output flushes per byte; current parallel output flushes at
  a completed batch/doc boundary and on close/reset. Neither operation is a
  physical power-loss durability guarantee. No new host timer/worker is needed.
- Snapshot save and Debugger W finish by closing the writer and checking close
  status. DIRECT medium writes already flush internally. Ordinary writer_write
  deliberately leaves buffering intact, which avoids flushing every tiny
  snapshot field. Making every public write flush is not a suitable shortcut.
- Serial/printer machine tests read after machine destruction. They prove final
  contents, not live visibility. Parallel failure tests separately assert exact
  partial-tail retention and flush failure without replay of accepted bytes.
- Storage writer reports exact-write status, not transferred byte count. A
  failed partial write cannot tell the LPT adapter which tail to retain. This
  was omitted from the initial writer-plus-flush migration estimate.

A task-local x64 probe links the unchanged real Storage archive. A one-byte
writer_write leaves native observed size zero; after writer_close it is one.
CRT append leaves size one until fflush, then size two. Observation uses an
explicitly read/write-sharing native handle. The first attempt used Storage
reader, whose sharing mode conflicts with an existing writer; that failure is
not evidence of flush behavior or a new Storage defect.

The same assertions-enabled probe validates the Compat-only candidate with a
non-ASCII UTF-8 filename, two binary appends containing NUL/FF/LF/1A, explicit
fflush and malformed UTF-8 rejection. The candidate is not production code.
Evidence: build/t98/s1/flush-design.log; flush.exit=0. No Computer Use, user
media/config edits, public edits or EXE rebuilds occurred.

Conclusion: do not assume per-byte flush is a hardware necessity, but do not
silently remove existing synchronous submission while repairing path encoding.
Its frequency may be profiled separately if optional file output is a measured
bottleneck. For this bounded fix, a public Storage migration would broaden
scope beyond the actual encoding defect, so the narrow private-open candidate
supersedes the earlier public-flush proposal and its estimates.

## S2 implementation and finite qualification ledger

Owner approval: 批准执行当前既定修复任务. Baseline 3b368d26.
The bounded universe is F1/F2 in the two real media adapters and their existing
callers. Completion requires every member below, full registered dual-width
qualification, released EXEs and actual-commit review. S3/S4 are not part of
this S's completion claim.

- HDD attach: consume/clear both old leases, report any retirement error before
  opening a new path. Failure cleanup already returns failure and keeps it.
- HDD restore: absent slots return the close status after clearing metadata;
  replacement transfers once, commits its metadata and returns retired-close
  status without pretending the old lease can be retried.
- FDD attach/restore: preserve physical drive identity, commit the real current
  medium metadata and return retired-close failure; absent restore propagates
  attach's failure. No retry, rollback or new storage path exists.
- Capacity: original fdisk.c::dosearchid returns signed long byte offsets, and
  host_fdisk_rd/wt accept signed int. Both Windows widths use 32 bits for these.
  A 2-GiB image's last sector starts at 0x7ffffe00; larger images are rejected
  on attach and replacement restore. Existing whole-cylinder geometry remains
  unchanged; this does not add an LBA or large-disk mode. Transfer validation
  compares sector count against remaining extent before multiplication.
- Similar FDD narrowing: attach already rejects byte lengths above LONG_MAX;
  restore now applies the same bound before consuming its replacement.
- Remaining destroy calls: both void hardware-teardown callbacks keep their
  original no-result ABI; rejected-candidate/error cleanup already reports the
  primary failure. Snapshot archive callers check each restore status and stop;
  their mutation phase is explicitly non-rollback. Machine attach/set-media
  callers already propagate false. Its stored startup options are not the
  authoritative mounted-medium view after hardware initialization; capture uses
  the real adapter view and cold reset retains live attachments.

Permanent owner-local tests include the actual hdd_media.c and gfi_image.c in
separate test translation units, sharing only a 39-line scripted-medium fixture.
It creates real 512-byte Lib leases but scripts reported size and close result.
The HDD unit checks both-slot cleanup, consumed replacement ownership, failed
retirement metadata, repeated detach, 2-GiB attach/restore/last-sector access,
oversized/short rejection, negative offsets/counts and x86 multiplication bounds.
The FDD unit checks attach and restore retirement, retained physical identity,
repeated detach and oversized replacement ownership. Native close failure and
a real multi-gigabyte guest disk are not reproduced by these injected tests.

Production estimate resolves to +30/-16, net +14 across two paths; tests are
195 added lines across three files, plus root CMake +3/-2. Test scaffolding
exceeds the original +75-130 estimate because both actual legacy adapters need
separate translation units (host_def.h is not multiply-includable) and scripted
size/close behavior. No production helper/state was added for testing. Initial
combined-TU test compilation failed on that original header; separate-owner
translation units fix the test setup, without changing mirror headers.

Both adapter units pass on x64/x86. Final cached full builds refresh both
assets/binary EXEs. Qualification under build/t98/s2/qualification exits zero:
each architecture passes all 193 registered tests, with zero failures/skips:
177 unit/gates, eight background integration, five native desktop and three
native audio. audit-results.ps1 independently compares registration against
the unique XML case names; both exact sets match. No Computer Use was used.
The x86 build re-emits existing Ninja-log recovery and original macro/switch
warnings; S2 changes none of those original bodies. The recovery causes extra
recompilation, not an authored source change or a measured performance gain.

Accounting method: git diff --numstat against 3b368d26, with production limited
to the two Compat paths, tests to the three new owner-local files, and root
CMake accounted separately. No public tree, mirror, owner INI or media change.
F1/F2 are removed from deferred TODO; S3/F3 and S4/F4 remain unimplemented.
No real native close error or 2-GiB guest installation was reproduced; those
boundaries have injected adapter proof. Existing real-guest snapshot/restart
and package paths pass the integration regression. S2 waits for owner acceptance.

### S2 actual-commit coordinator review

Executor P1 7afc9981 was committed and pushed before this role switch. The
coordinator read its actual CMake, both production diffs, all three test files
and four documentation diffs; the two binary changes were reconciled with the
full link logs and recorded hashes. The twelve-path scope contains no public,
mirror, INI or guest-media change. Accounting independently agrees with
production +30/-16, tests +195/-0 and root CMake +3/-2. Closed leases are not
retained/retried, transfer multiplication follows the capacity guard, and valid
replacement ownership/metadata survive a reported retirement error.

The original request, active packet and six S2 ledger members are satisfied
within their stated proof limits. The coordinator reran the independent XML
set comparison: x64/x86 each 193 unique passing registered cases, no skips or
failures. Documentation/whitespace checks pass. Existing mirror warnings and
Ninja recovery remain disclosed, not fixed. Main matches the pushed executor
commit; only owner-owned untracked snapshot.debug remains and is excluded.
This review accepts executor delivery, not owner acceptance or T98 closure.

Owner subsequently accepts and closes S2: 验收通过 收口s任务 保持t开放.
The [S2 history](../history/M9-T98-S2-media-boundaries.md) records closure;
T98 remains open without an active S. S3/S4 are not automatically started.
