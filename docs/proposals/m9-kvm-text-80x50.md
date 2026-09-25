# Bounded 80x50 KVM text frames

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

S2 is planned: after owner testing, perform the whole-task actual-change and
requirement audit, dispose of task-owned scratch, and close T84 with owner
approval. No speculative second implementation or new state machine is planned.

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
evidence. Owner authorizes S1 build/test/commit/push delivery, then waits for
manual verification. No S1/T84 closure is claimed. The deferred fault is not fixed.

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

T84 remains open for owner acceptance and planned S2 whole-task review.

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
