# SoftPC Product Test Ownership And Layout Cleanup

## Status and owner request

Owner accepted T76 and requested "测试通过 收口提交t任务 准入下一个".
Admitted as T77, beginning with S1. CURRENT.md owns the executable packet;
this proposal retains the bounded plan and evidence ledger. S1/S2 are closed;
S3 performs final acceptance. Owner explicitly authorizes serial completion
and self-reviewed S/T closure: "继续啊 做完自行审计收口s和t任务".

Original owner requests:

> softpc的test里面太谜了：有integration，合理，所有集成测试；有 common, lib, x86，给这3个公共组件，也合理；但还有support和unit这两个目录，是干嘛的？

> 将这个整理工作加入队列首位作为新的T任务proposal

## Findings and goal

The inspected tree has 33 C files under `test/unit`. They test SoftPC App and
Core behavior; some assemble a real machine and are not isolated unit tests.
`test/support` mixes product machine fixtures, legacy `app_runtime_*` and
`app_input_queue_*` adapters, CMake boundary checks, cross-process snapshot
orchestration, documentation, and two unregistered manual diagnostics.

Organize these files by actual ownership and test purpose, remove redundant
test-only interfaces, and make the test inventory understandable without
changing production behavior or reducing verification coverage.

## Proposed ownership

| Location | Responsibility |
| --- | --- |
| `test/lib`, `test/common`, `test/x86` | Existing independently reusable test corpora; unchanged by this task |
| `test/app` | Tests focused on App configuration, commands and bindings |
| `test/core` | Tests focused on Core machine adaptation, compatibility and devices |
| `test/integration` | Cross-component product, boot, package and cross-process flows |
| `test/checks` | Product source-boundary and build-ownership checks |

Classify by inspected dependencies and assertions, not filenames alone. Keep
fixtures beside their owning tests. A genuinely shared product fixture has one
implementation at the narrowest appropriate product-test location; it must not
be moved into a reusable corpus merely because its name contains `common`.
Do not introduce a fixture framework, generic support object, or extra build
aggregation layer just to reorganize directories.

Preserve the useful machine/driver assembly performed by
`common_machine_test.c/h`. Replace legacy naming-only wrappers in `runtime.h`
and `input_queue.h` with direct calls to the existing contracts after auditing
every caller. Do not duplicate assembly or force tests through a different
production path to remove a header.

Inspect both manual diagnostics for actual consumers and unique coverage.
Delete obsolete, unreferenced programs only after recording that evidence;
retain useful probes with an explicit owner and documented invocation. Do not
silently discard unique tests or merely rename the miscellaneous directory.

Replace the stale `test/support/TESTS.md` inventory with a concise
`test/README.md`, linking the reusable packages and explaining product test
ownership, execution and desktop requirements. Update affected current design
documentation rather than creating a second layout authority.

## Proposed S tasks on admission

### S1: Inventory and ownership relocation

- Freeze the then-current file, target and CTest inventory, including helper
  callers, manual probes and cross-directory dependencies.
- Record each file's destination or justified retention/removal in a finite
  disposition ledger. Map every registered test to its existing assertions.
- Move App, Core and integration tests to their proper locations, preserving
  history and behavior; update root CMake and all live path references.
- Keep existing CTest names and operational labels unless an audited correction
  is explicitly recorded. Directory names alone must not change execution mode.
- Exit: relocated tests build and run with equivalent coverage; no stale live
  paths or duplicated implementations remain.

### S2: Fixtures, wrappers and checks

- Remove obsolete naming-only test adapters and update all callers to canonical
  interfaces while retaining one necessary product fixture assembly path.
- Place static gates in `test/checks` and dynamic snapshot orchestration with
  integration tests; resolve the two manual diagnostics using the S1 ledger.
- Remove emptied `test/unit` and `test/support` only when every entry has an
  evidenced disposition; do not replace them with another catch-all directory.
- Exit: no legacy forwarding aliases or dead helper registrations remain;
  focused tests and source/build boundary checks pass.

### S3: Documentation and complete acceptance

- Finish the test inventory and current layout documentation; verify CMake
  inputs, labels and assertions against the pre-change inventory.
- Verify the three shared source/test pairs remain untouched and independently
  usable; no product fixture dependency may leak into a reusable test package.
- Run applicable x86/x64 full tests and package acceptance, using the established
  background test configuration. Publish both EXEs for owner verification.
- Exit: complete per-file ledger, no lost coverage, all gates passing, and no
  unfinished cleanup hidden in TODO. Owner has authorized self-reviewed T closure.

## Change estimate and reporting

Admission baseline: 7ebb435f. Read-only enumeration still finds 33 C files in
test/unit, 13 files in test/support (including two diagnostics), and two existing
integration C files. This 48-file product inventory is the frozen file universe;
S1 must add the exact registered-target/CTest and caller mapping before moves.
Disposition is relocation without body changes, justified retention for S2, or
evidenced removal in S2; no entry can disappear without its proof. S3 verifies
the complete before/after inventory, operational labels and six shared corpora.

Initial S1 estimate: production +0/-0; up to 33 test C files relocated, test
assertion changes +0/-0, build/include reference edits roughly +60--120/-60--120
(net near zero). Exact moved lines and destination counts follow caller audit,
before relocation. No throughput or runtime improvement is claimed.

Production source change budget: **+0/-0**. The initial relocation inventory is
33 product test C files plus the contents of `test/support`; the exact movement
count depends on ownership inspection at admission. Existing integration files
also participate in the ownership review. Root CMake and live documentation
references need updates. Reusable corpora are outside the change budget.

Movement should preserve test bodies. Removing aliases should reduce helper
code, but no numerical net reduction is promised before caller analysis.
Before each S task, report affected files/functions, estimated moved lines and
added/deleted lines separately. After each S task, report actual rename-aware
diffs, additions, deletions, net change, preserved test inventory, build/test
results and both executable links; commit and push the completed scoped work.

## Boundaries and stopping conditions

- No production API, runtime, guest behavior or preserved Core mirror changes.
- No changes to INI, disk images or snapshots; no test data written into assets.
- No modification of `src/lib`, `src/common`, `src/x86`, or their test corpora.
- Do not weaken assertions, drop tests, or change desktop isolation to make
  acceptance pass. Unexpected behavior changes require separate diagnosis.
- If cleanup requires production fixes or a reusable contract change, report
  the dependency for separate admission instead of expanding this task.

## Authority and references

- [Execution rules](../rules/EXECUTION.md)
- [Current source layout](../design/CODING.md)
- [Current architecture](../design/ARCHITECTURE.md)
- [Ordered candidate queue](../states/QUEUE.md)

## T77 S1 frozen relocation ledger

Baseline 1fe946a2: 115 CTest entries including five desktop cases; background
execution retains 110. Names, labels, commands, working directories and timeout
properties are compared before/after (only relocated script paths may differ).
Each row below retains its exact test body and existing target/assertions.

| Existing test/unit file | Destination under test/ | Lines moved |
| --- | --- | --- |
| audio_failure_smoke.c | core | 78 |
| audio_lifecycle_smoke.c | core | 13 |
| bop_smoke.c | core | 287 |
| checkpoint_smoke.c | core | 971 |
| command_provider_smoke.c | integration | 1031 |
| command_smoke.c | app | 232 |
| config_smoke.c | app | 62 |
| dual_media_smoke.c | core | 111 |
| fdc_smoke.c | core | 231 |
| irq_smoke.c | core | 148 |
| keyboard_smoke.c | app | 104 |
| keycode_smoke.c | core | 67 |
| lifecycle_smoke.c | core | 69 |
| machine_smoke.c | core | 1282 |
| media_snapshot_smoke.c | core | 322 |
| mouse_smoke.c | core | 150 |
| partition_image_smoke.c | core | 111 |
| pit_smoke.c | core | 80 |
| platform_failure_smoke.c | core | 139 |
| presentation_shutdown_smoke.c | integration | 118 |
| printer_smoke.c | core | 56 |
| quick_time_smoke.c | core | 25 |
| runtime_cursor_smoke.c | integration | 40 |
| runtime_input_continuation_smoke.c | integration | 145 |
| runtime_smoke.c | integration | 217 |
| serial_smoke.c | core | 92 |
| snapshot_boundary_smoke.c | core | 89 |
| snapshot_transaction_smoke.c | integration | 461 |
| sound_smoke.c | core | 79 |
| sound_state_smoke.c | core | 78 |
| text_console_compat_smoke.c | core | 210 |
| vga_frame_smoke.c | core | 1231 |
| x87_layout_smoke.c | core | 44 |

App owns configuration, command parsing and key-binding assertions. Core owns
device/host ABI, renderer, media and its machine implementation. Integration
owns actual command-to-machine, worker input/frame, snapshot rendezvous and App
shutdown chains. Directory ownership does not redefine legacy CTest labels.
Existing integration/package_smoke.c and runtime_restart_boot_smoke.c stay.

The remaining 13 support files are deliberately retained until S2:
build_ownership.cmake, product_boundary.cmake, product_boundary_negative.cmake,
standalone_source_boundary.cmake, win32_presentation_manifest.cmake -> checks;
snapshot_cross_process.cmake -> integration; common_machine_test.c/h -> one
integration fixture; runtime.h and input_queue.h -> remove aliases after caller
conversion; TESTS.md -> replace with test/README.md; diagnostics/real_boot_smoke.c
and diagnostics/runtime_boot_smoke.c -> inspect obsolete APIs and unique utility,
record deletion or supported invocation before disposal. These are the entire
48-file ledger; no unclassified catch-all is introduced.

Before S1 movement: three App, 24 Core and six Integration C files; source
bodies byte-identical. CMake has 32 path-bearing lines (one expands two probes),
plus the existing boundary gate scan and sample path need relocation updates.
Expected code/build roughly +42/-40, with production +0/-0. Existing support
gate must continue scanning every self-contained relocated test; the two
pre-existing package consumers alone retain their media boundary exceptions.

### S1 delivery

33 files / 8373 lines relocated with identical Git blobs: App 3, Core 24,
Integration 6. All 115 CTest command/properties definitions are identical to
the pre-move JSON inventory. No target, label, timeout or assertion was lost.
Root CMake +32/-32, build-ownership sample +1/-1, source boundary +17/-2:
code/build +50/-35, net +15. Production and shared six corpora +0/-0.
The extra naming scan reuses Lib's checker for the new App/Core roots without
editing its shared script's legacy product-root list. This is why actual +15
exceeds the initial near-zero estimate; no copied naming implementation.

Both Release builds pass; full background x64 110/110 (184.63s), x86 110/110
(169.86s), five desktop cases excluded per width. The final extended source
gate was additionally rerun on both widths (2.32s/2.39s), passing. A manual
gate invocation with a relative PowerShell -D argument failed to locate the
source root; explicit absolute root and both CTest invocations pass.
EXEs rebuilt, unchanged SHA256 from T76: x86
D61305E116367F8CBD5B885351D81A7FA26FD74B5981AD66F3F7FFEF02206516;
x64 4DF851DB91A4559AE7C3E6CEB35753040355FE53CC4A8C2417C6E79EC9A5C8A0.
No assets/INI changes. Remaining support files have explicit S2 ownership in
the ledger; no other live build/tool reference to test/unit remains.

### S2 brief and diagnostic disposition

Owner authorizes continuing through self-reviewed T closure. Production +0/-0;
estimate code/build +140/-2080, net about -1940. Move five checks (864 lines at
S1), snapshot script (37) and fixture C/H (41); remove runtime.h (85) and
input_queue.h (22), converting three compiled runtime consumers to actual
Common types/calls and caller-owned fixture storage. No extra fixture allocation
or per-operation forwarding remains. Frame copies explicitly pass current run
generation; removable-media calls retain OVERLAY mode; queue init explicitly
compares LIB_STATUS_OK. Fixtures still assemble/tear down the same VM driver
and Common machine, in the same order.

`git grep` across build/tools/current docs and test callers finds neither
diagnostic registered or invoked; only their historical descriptions survive.
real_boot_smoke.c (482 lines) uses the direct-slice runner already rejected as
invalid for continuous execution in m4-runtime-media-input.md; its extra thread
can concurrently inject/read the old machine. runtime_boot_smoke.c (1356 lines)
uses removed frame fields text/attributes/graphics_width and undefined
SOFTPC_RUNTIME_DIB_MAX_* values. Its old Setup automation and dump switches
are not a currently runnable test contract. Delete both obsolete programs,
not port their alternate execution/observation paths. Current boot/prompt/Win31
roundtrip proof remains runtime_restart_boot_smoke; IRQ1 input proof remains
runtime_input_continuation_smoke; frame/device and snapshot proofs retain their
existing registered tests. Historical one-off Setup automation is retired,
not falsely claimed as equivalent automated coverage. Git retains both programs.

Shared verify_kvm_naming.cmake deliberately remains byte-identical, including
its legacy product path list; the existing product gate compensates by invoking
that checker for all new product roots. No runtime or shared-package dependency
on retired directories exists. S2 removes test/support/TESTS.md in favor of the
owned test README; S3 audits that inventory and current design together.

### S2 delivery

All 13 support entries resolved: eight relocated (five checks, snapshot script,
fixture C/H), two wrapper headers and two obsolete diagnostics deleted, stale
inventory replaced by test/README.md. Empty support/diagnostics, support and
unit directories removed. Git retains deleted historical material. Three
consumers use common_machine directly and stack-owned fixture storage; state,
frame generation, media mode and init status match the removed wrappers.
Their assert/REQUIRE counts remain 46/27/22; actual expressions and call order
were reviewed, not inferred equivalent solely from those counts. The other 31
relocated C files remain byte-identical. No second fixture or executor exists.

Rename-aware `git diff --find-renames=20% --numstat` over CMakeLists.txt and
test excluding Markdown: +181/-2096, net -1915; production +0/-0. The lower
rename threshold identifies the short renamed fixture header. Of the reduction,
1838 lines are obsolete diagnostics and 107 are alias headers; direct-call
sites and necessary ownership checks account for the offset. Added lines exceed
estimate because explicit call arguments/wrapping and a 13-line registration
check replace implicit aliases. No syntax compression or lost assertion.

Existing build-ownership gate now compares all product test C files with actual
root target inputs. A temporary unregistered C file made configure fail with
the expected error; after removing it both configure/builds pass. Existing
source gate rejects retired directories and reuses the Lib naming checker for
App/Core/checks. All 115 CTest definitions match after normalizing only moved
script paths and architecture, with labels/timeouts unchanged.

Both Release builds pass; full background x64 110/110 (155.22s), x86 110/110
(155.38s), five desktop cases excluded per width. EXE hashes unchanged from S1;
source and all six shared corpora untouched. Documentation gate/diff check pass.
No remaining alias callers: the product gate's forbidden app_runtime pattern
is intentionally retained, and shared naming script's legacy roots remain
immutable with new roots checked by the product gate. Historical evidence
retains historical paths; it is not a live build reference.

### S3 admission brief

Baseline 217ec7a5. Expected production/test/build +0/-0; only concise document
completion and acceptance evidence. Freeze six shared directories at T baseline:
src/lib 91, src/common 23, src/x86 15, test/lib 46, test/common 20, test/x86 9
tracked files. Verify copied neutral four-directory set (180 files) and full
six-directory set (204 files) without App/Core/product fixtures. Independently
configure/build/test Lib and Common from the neutral copy and x86 from the full
copy on both widths. Do not modify any copied corpus to make its build pass.
Final product background suites and package builds remain required; desktop
tests stay excluded under the standing no-desktop-interference constraint.
Complete all 48 original file dispositions, preserved targets/CTest definitions,
body/call-equivalence review and exact endpoint counts. No new TODO or queue
work is needed for this bounded layout cleanup. Owner explicitly authorizes
self-reviewed S/T closure, superseding the earlier wait-for-manual-test step.

### S3 acceptance results

All 48 original product files have a disposition: 41 relocations (33 test C
files and eight helpers/checks), two existing Integration files retained,
four obsolete C/H files deleted, and the old inventory replaced by the new
test README. The resulting product tree has 44 files: App 3, Core 24,
Integration 11, checks 5 and README 1. Thirty-one relocated test bodies remain
byte-identical; the other two and the pre-existing boot test use canonical
Common calls as reviewed in S2. All 115 CTest definitions match on both widths
after only relocated script paths and architecture are normalized.

Fresh isolated Release builds use only the copied packages, without App/Core
or product fixtures. Lib/Common use the four-directory copy; x86 uses the
six-directory copy. All copied files match the originals after testing:
180/180 and 204/204 respectively. Both widths pass all three package suites:

| Width | Lib | Common | x86 |
| --- | --- | --- | --- |
| x64 | 41/41, 55.89s | 18/18, 13.63s | 9/9, 6.41s |
| x86 | 41/41, 57.33s | 18/18, 17.31s | 9/9, 9.60s |

S3 code/build +0/-0. Whole-task endpoint, computed with
`git diff 1fe946a2 --find-renames=20% --numstat -- CMakeLists.txt test ':!*.md'`,
is +229/-2129, net -1900. This differs from adding individual-S additions and
deletions because two intermediate path edits cancel; net change agrees.
All production sources, six shared corpora and assets remain unchanged.
Source/build gates retain coverage of all relocated product tests; the shared
checker's historical path list is not a runtime dependency and remains intact.
No production state owner, executor, ABI or user-visible behavior changed.

Final product Release builds pass. Background x64 110/110 (155.50s), x86
110/110 (151.26s), including native worker restart/Win31 prompt roundtrips,
device/media/snapshot cases, shared manifests/DAG and documentation checks.
Five desktop cases remain excluded per width; no fresh desktop, Linux runtime
or downstream NNES integration acceptance is claimed. Both rebuilt EXEs retain
the S1 SHA256 values. No INI or media was modified. Task-owned isolated copies,
builds, inventory dumps and logs are removed after recording these results;
normal build configurations and user-owned build/output are preserved.
